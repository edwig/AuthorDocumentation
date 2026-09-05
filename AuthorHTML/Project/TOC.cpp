//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    TOC.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Interface of the Table Of Contents (TOC)
// A context file is the *.HHC file that's used in the Microsoft 
// HTML Help compiler from the HTML Workshop Suite to compile 
// the Table Of Contents (TOC) of the CHM file
//
#include "StdAfx.h"
#include "TOC.h"
#include "AuthorHTML.h"
#include "MainFrm.h"
#include "Misc.h"

TOC::TOC(CString tocFile)
    :m_tocFilename(tocFile)
    ,m_needSaving(false)
{
  Reset();
}

TOC::~TOC()
{
  Reset();
}

void
TOC::Reset()
{
  m_siteType       = "";
  m_linenumber     = 0;
  m_imageWidth     = 0;
  m_windowStyles   = 0L;
  m_ExWindowStyles = 0L;
  m_needSaving     = false;
}

////////////////////////////////////////////////////////////////////////
//
// WRITE BACK TO FILE
//
////////////////////////////////////////////////////////////////////////

bool
TOC::WriteTOCFile()
{
  if(m_tocFilename.IsEmpty())
  {
    return false;
  }
  MainFrame::SetStatusText("Writing TOC file: " + m_tocFilename);

  // See if really necessary
  if(!m_needSaving)
  {
    return true;
  }

  WinFile file(m_tocFilename.GetString());
  file.Open(winfile_write | open_trans_text, attrib_normal,AUTHOR_HTML_ENCODING);
  if(!file.GetIsOpen())
  {
    return false;
  }
  file.Write(_T("<html>\n"));
  file.Write(_T("<!-- Sitemap 1.0 -->\n"));
  WriteProperties(file);
  WriteList(file,&m_list,0);
  file.Write(_T("</html>\n"));
  file.Close();

  // Reset saving
  m_needSaving = false;
  return true;
}

void
TOC::WriteProperties(WinFile& p_file)
{
  //<object type="text/site properties">
  //  <param name="SiteType" value="toc">
  //  <param name="Image Width" value="16">
  //  <param name="Window Styles" value="0x800002">
  //  <param name="ExWindow Styles" value="0x100">
  //</object>
  p_file.Write(_T("<object type=\"text/site properties\">\n"));
  p_file.Write(_T("  <param name=\"SiteType\" value=\"toc\">\n"));
  if(m_imageWidth)
  {
    p_file.Format(_T("  <param name=\"Image Width\" value=\"%d\">\n"), m_imageWidth);
  }
  if(m_windowStyles)
  {
    p_file.Format(_T("  <param name=\"Window Styles\" value=\"0x%x\">\n"), m_windowStyles);
  }
  if(m_ExWindowStyles)
  {
    p_file.Format(_T("  <param name=\"ExWindow Styles\" value=\"0x%x\">\n"), m_ExWindowStyles);
  }
  p_file.Write(_T("</object>\n"));
}

void
TOC::WriteList(WinFile& p_file,TOCEntry* list,int level)
{
  //<ul>
  //    <li><object type="text/sitemap">
  //           <param name="Name" value="Introduction to the Call Center sample">
  //           <param name="Local" value="Introduction_to_the_Call_Center_sample.htm">
  //           <param name="ImageNumber" value="18">
  //        </object>
  //    <li><object type="text/sitemap">
  //           <param name="Name" value="Introduction to the Call Center Assistant system">
  //        </object>
  //    <ul>
  //      More list
  //    </ul>
  //</ul>

  CString levelString;
  for(int ind = 0;ind < level; ++ind)
  {
    levelString += CString("    ");
  }
  if(!list->GetTitle().IsEmpty() || !list->GetDocumentFilename().IsEmpty())
  {
    CString image;
    image.Format("%d",list->GetImageNumber() + 1);
    if(image == "0") image = "";

    p_file.Format(_T("%s<li><object type=\"text/sitemap\">\n"), levelString.GetString());

    WriteParameter(p_file,levelString,"Name",       list->GetTitle());
    WriteParameter(p_file,levelString,"Local",      list->GetDocumentFilename());
    WriteParameter(p_file,levelString,"Comment",    list->GetComment());
    WriteParameter(p_file,levelString,"FrameName",  list->GetFrameName());
    WriteParameter(p_file,levelString,"WindowName", list->GetWindowName());
    WriteParameter(p_file,levelString,"ImageNumber",image);

    p_file.Format(_T("%s</object>\n"),levelString.GetString());
  }
  if(list->GetChildren().size() > 0)
  {
    p_file.Format(_T("%s<ul>\n"), levelString.GetString());
    for(unsigned int num = 0; num < list->GetChildren().size(); ++num)
    {
      WriteList(p_file,list->GetChildren()[num],(level + 1));
    }
    p_file.Format(_T("%s</ul>\n"), levelString.GetString());
  }
}

void
TOC::WriteParameter(WinFile& p_file,CString& levelString,LPCTSTR name,CString value)
{
  if(value.IsEmpty())
  {
    return;
  }
  value.Replace("\"","\'");
  p_file.Format(_T("%s    <param name=\"%s\" value=\"%s\">\n"), levelString.GetString(),name,value.GetString());
}

////////////////////////////////////////////////////////////////////////
//
// READING PARSER
//
////////////////////////////////////////////////////////////////////////

bool
TOC::ReadTOCFile()
{
  if(m_tocFilename.IsEmpty())
  {
    return false;
  }
  MainFrame::SetStatusText("Reading TOC file: " + m_tocFilename);

  Reset();
  WinFile file(m_tocFilename.GetString());
  file.Open(winfile_read | open_trans_text);
  if(!file.GetIsOpen())
  {
    return false;
  }

  bool result = true;
  try
  {
    // HHC TOC parser must be really relaxed!!!!!
    // A lot of old code is hanging around (in Microsoft)
    // Where body and head tokens do not have the opposing end-token

    Misc::ResetTokenizer();
    Misc::SkipToken(file,PF_HTML,m_linenumber);
    ReadComment(file);
    Misc::SkipToken(file,PF_HEAD,m_linenumber);
    ReadProperties(file);
    Misc::SkipToken(file,PF_ENDHEAD,m_linenumber);
    Misc::SkipToken(file,PF_BODY,   m_linenumber);
    ReadList(file,&m_list,0);
    Misc::SkipToken(file,PF_ENDBODY,m_linenumber);
    Misc::SkipToken(file,PF_ENDHTML,m_linenumber);
  }
  catch(CString mess)
  {
    CString message;
    message.Format("Error reading TOC file '%s' Line:%i\n%s.",m_tocFilename.GetString(),m_linenumber,mess.GetString());
    theApp.ErrorMessage(message);
    result = false;
  }
  file.Close();
  m_needSaving = false;
  return result;
}

void
TOC::ReadComment(WinFile& file)
{
  // <!-- Sitemap 1.0 -->
  CString word;
  if(!Misc::SkipToken(file,PF_COMMENT,m_linenumber))
  {
    // Not a sitemap comment;
    return;
  }
  while(true)
  {
    if(Misc::GetToken(file,word,m_linenumber) == PF_EOF)
    {
      break;
    }
    if(word.Right(2).Compare("--") == 0)
    {
      break;
    }
  }
}

void
TOC::ReadProperties(WinFile& file)
{
  //<object type="text/site properties">
  //  <param name="SiteType" value="toc">
  //  <param name="Image Width" value="16">
  //  <param name="Window Styles" value="0x800002">
  //  <param name="ExWindow Styles" value="0x100">
  //</object>
  CString partialError = "First object before list ";
  CString word;

  if(!Misc::SkipToken(file,PF_OBJECT,m_linenumber))
  {
    // No properties in this TOC
    return;
  }
  if(!Misc::SkipToken(file,PF_TYPE,m_linenumber))         throw partialError + ": no type found";
  if(!Misc::SkipToken(file,PF_EQUAL,m_linenumber))        throw partialError + ": no equal found";
  if(Misc::GetToken(file,word,m_linenumber) != PF_STRING) throw partialError + "does not have a string type";
  if(word.CompareNoCase("text/site properties") != 0)
  {
    throw partialError + "is not of type 'text/site properties'";
  }
  while(true)
  {
    TOCToken tok = Misc::GetToken(file,word,m_linenumber);

    if(tok == PF_ENDOBJECT)
    {
      break;
    }
    if(tok != PF_PARAM)
    {
      throw partialError + "sub-object is not a parameter";
    }
    if(!Misc::SkipToken(file,PF_NAME,m_linenumber))         throw partialError + ": no name found";
    if(!Misc::SkipToken(file,PF_EQUAL,m_linenumber))        throw partialError + ": no equals found in parameter";
    if(Misc::GetToken(file,word,m_linenumber) != PF_STRING) throw partialError + "parameter name is not a string";
    CString parameterName = word;
    if(!Misc::SkipToken(file,PF_VALUE,m_linenumber))        throw partialError + ": no value found for parameter";
    if(!Misc::SkipToken(file,PF_EQUAL,m_linenumber))        throw partialError + ": no equal-sign parameter name=value";
    if(Misc::GetToken(file,word,m_linenumber) != PF_STRING) throw partialError + "parameter value is not a string";
    CString parameterValue = word;
    TRACE("Text/Site properties. Parameter: %s Value:%s\n",parameterName.GetString(),parameterValue.GetString());
    // 
    if(parameterName.CompareNoCase("sitetype")    == 0) m_siteType   = parameterValue;
    if(parameterName.CompareNoCase("image width") == 0) m_imageWidth = atoi(parameterValue);
    if(parameterName.CompareNoCase("window styles") == 0)
    {
      Misc::ParseNumber(parameterValue,m_windowStyles);
    }
    if(parameterName.CompareNoCase("exwindow styles") == 0)
    {
      Misc::ParseNumber(parameterValue,m_ExWindowStyles);
    }
  }
  // CHECKS
  if(!m_siteType.IsEmpty())
  {
    if(m_siteType.CompareNoCase("toc") != 0)
    {
      CString message;
      message.Format("ERROR: The table-of-contents (HHC) file: %s\n"
                     "Has a 'sitetype' of '%s'. This is not a 'toc'\n"
                     "Did you rename some files or opened a damaged location?"
                    ,m_tocFilename.GetString(),m_siteType.GetString());
      theApp.ErrorMessage(message);
    }
  }
}

//<ul>
//  <li><object type="text/sitemap">
//         <param name="Name" value="Introduction to the Call Center sample">
//         <param name="Local" value="Introduction_to_the_Call_Center_sample.htm">
// opt:    <param name="ImageNumber" value="18">
// opt:    <param name="Comment" value="Test">
// 1 of:   <param name="WindowName" value="SecondWindow">
//   or:   <param name="FrameName" value="_parent">
//      </object>
//  <li><object type="text/sitemap">
//     <param name="Name" value="Introduction to the Call Center Assistant system">
//      </object>
//  <ul>
//      More list
//  </ul>
//</ul>
bool
TOC::ReadList(WinFile& file,TOCEntry* list,int level)
{
  // Check on too deep recursion
  if(level >= RECURSION_MAX_LEVEL)
  {
    CString message;
    message.Format("Max level of recursion in TOC tree reached (%i)",RECURSION_MAX_LEVEL);
    throw message;
  }

  if(!Misc::SkipToken(file,PF_LIST,m_linenumber))
  {
    // Empty table of contents
    return false;
  }
  CString   word;
  TOCEntry* entry = NULL;
  while(true)
  {
    // Rest of the list
    TOCToken tok = Misc::GetToken(file,word,m_linenumber);
    if(tok == PF_ENDLIST)
    {
      break;
    }
    else if(tok == PF_EOF)
    {
      throw CString("Error in TOC tree: Broken or corrupted TOC file!");
    }
    else if (tok == PF_LIST)
    {
      Misc::PushToken(word,tok);
      if(!ReadList(file,entry,(level + 1)))
      {
        return false;
      }
      continue;
    }
    else if(tok != PF_LISTITEM)
    {
      throw CString("Unknown tag in TOC treelist. Broken or corrupted TOC file!");
    }
    // One more item
    if(!Misc::SkipToken(file,PF_OBJECT,m_linenumber))       throw CString("TOC item must be an OBJECT");
    if(!Misc::SkipToken(file,PF_TYPE,m_linenumber))         throw CString("TOC item must have a type");
    if(!Misc::SkipToken(file,PF_EQUAL,m_linenumber))        throw CString("TOC item type must have an equal");
    if(Misc::GetToken(file,word,m_linenumber) != PF_STRING) throw CString("TOC item type must be a string");
    if(word.CompareNoCase("text/sitemap"))                  throw CString("TOC item type must be 'text/sitemap'");

    int     imageNumber = -1;
    CString fileName;
    CString title;
    CString comment;
    CString frameName,windowName;
    CString name,value;
    int     num = 0;
    tok = Misc::GetToken(file,word,m_linenumber);
    while(tok == PF_PARAM)
    {
      ++num; // next parameter
      tok = GetTOCParameter(file,num,name,value);
      if(name.CompareNoCase("name") == 0)
      {
        title = value;
      }
      else if(name.CompareNoCase("local") == 0)
      {
        fileName = value;
      }
      else if(name.CompareNoCase("ImageNumber") == 0)
      {
        imageNumber = atoi(value) -1;
      }
      else if(name.CompareNoCase("Comment") == 0)
      {
        comment = value;
      }
      else if(name.CompareNoCase("FrameName") == 0)
      {
        frameName = value;
      }
      else if(name.CompareNoCase("WindowName") == 0)
      {
        windowName = value;
      }
      else
      {
        CString message;
        message.Format("HHC TOC-file: Unknown parameter name '%s' in %dth parameter",name.GetString(),num);
        throw message;
      }
    }
    if(tok != PF_ENDOBJECT)
    {
      throw CString("TOC item's object not closed with /OBJECT");
    }
    // Make topic
    entry = new TOCEntry(title,fileName,imageNumber,comment,frameName,windowName);
    list->AddTopic(entry);

    // Old format's *.HHC have an OPTIONAL <A> Anchor after each object with the same info
    if(Misc::SkipToken(file,PF_ANCHOR,m_linenumber))
    {
      TOCToken tok2;
      while(true)
      {
        tok2 = Misc::GetToken(file,word,m_linenumber);
        if(tok2 == PF_EOF || tok2 == PF_ENDANCHOR)
        {
          break;
        }
      }
      Misc::SkipToken(file,PF_BREAK,m_linenumber);
    }
  }
  return true;
}

TOCToken
TOC::GetTOCParameter(WinFile& file,int num,CString& name,CString& value)
{
  CString word;
  // Get image number
  if(!Misc::SkipToken(file,PF_NAME,m_linenumber))         
  {
    ParameterError("TOC item %dth parameter must have a name",num);
  }
  if(!Misc::SkipToken(file,PF_EQUAL,m_linenumber))        
  {
    ParameterError("TOC item %dth parameter name must have an equal",num);
  }
  if(Misc::GetToken(file,word,m_linenumber) != PF_STRING) 
  {
    ParameterError("TOC item %dth parameter name must be a string",num);
  }
  // This is our parameter
  name = word;
  if(!Misc::SkipToken(file,PF_VALUE,m_linenumber))        
  {
    ParameterError("TOC item %dth parameter must have a value",num);
  }
  if(!Misc::SkipToken(file,PF_EQUAL,m_linenumber))        
  {
    ParameterError("TOC item %dth parameter value must have an equal",num);
  }
  if(Misc::GetToken(file,word,m_linenumber) != PF_STRING) 
  {
    ParameterError("TOC item %dth parameter value must be a string",num);
  }
  // This is our value
  value = word;

  TOCToken tok = Misc::GetToken(file,word,m_linenumber);
  return tok;
}

void
TOC::ParameterError(LPCTSTR error,int num)
{
  CString message;
  message.Format(error,num);
  throw message;
}

////////////////////////////////////////////////////////////////////////
//
// TOC TREE
//
////////////////////////////////////////////////////////////////////////

void
TOC::DisplayTOCInTree(TOCTreeCtrl* tree)
{
  tree->DeleteAllItems();
  HTREEITEM root = tree->GetRootItem();
  InsertTOC(tree,root,&m_list,0);
}

void
TOC::InsertTOC(TOCTreeCtrl* tree,HTREEITEM item,TOCEntry* entry,int level)
{
  // Standard recursion test
  if(level == RECURSION_MAX_LEVEL)
  {
    return;
  }

  HTREEITEM thisItem = item;
  if(level)
  {
    thisItem = tree->InsertItem(entry->GetTitle(),item);
    tree->SetItemData(thisItem,(DWORD_PTR) entry);
    tree->RedisplayEntry(thisItem,entry);
  }
  for(unsigned int ind = 0; ind < entry->GetChildren().size(); ++ind)
  {
    InsertTOC(tree,thisItem,entry->GetChildren()[ind],level + 1);
  }
}

void
TOC::AddEntry(TOCEntry* here,TOCEntry* toinsert)
{
  if(here == NULL)
  {
    m_list.AddTopic(toinsert);
  }
  else
  {
    TOCEntry* parent = here->GetParent();
    parent->AddBetween(here,toinsert);
  } 
  m_needSaving = true;
}

void
TOC::AddChild(TOCEntry* here,TOCEntry* toinsert)
{
  if(here == NULL)
  {
    m_list.AddTopic(toinsert);
  }
  else
  {
    here->AddTopic(toinsert);
  }
  m_needSaving = true;
}

void
TOC::DeleteEntry(TOCEntry* entry)
{
  TOCEntry* parent = entry->GetParent();
  if(parent)
  {
    parent->DeleteChild(entry);
  }
  else
  {
    m_list.DeleteChild(entry);
  }
  m_needSaving = true;
}

void
TOC::DetachEntry(TOCEntry* entry)
{
  TOCEntry* parent = entry->GetParent();
  if(parent)
  {
    parent->DetachChild(entry);
  }
  else
  {
    m_list.DetachChild(entry);
  }
  m_needSaving = true;
}

void
TOC::MoveEntry(TOCEntry* here,TOCEntry* tomove)
{
  DetachEntry(tomove);
  AddEntry(here,tomove);
  m_needSaving = true;
}
