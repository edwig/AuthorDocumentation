//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    IndexFile.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// A index file is the *.HHK file that's used in the Microsoft 
// HTML Help compiler from the HTML Workshop Suite that contains
// all the index keywords and file references to HTML pages
//
#include "StdAfx.h"
#include "AuthorHTML.h"
#include "MainFrm.h"
#include "Misc.h"
#include "IndexFile.h"

IndexFile::IndexFile(CString indexFilename)
          :m_indexFilename(indexFilename)
          ,m_needSaving(false)
{
}

IndexFile::~IndexFile()
{
  Reset();
}

void
IndexFile::Reset()
{
  m_linenumber    = 0;
  m_frameName     = "";
  m_needSaving    = false;
  m_list.Reset();
}

//////////////////////////////////////////////////////////////////////////
//
// WRITE THE HHK Index FILE
//
//////////////////////////////////////////////////////////////////////////

bool
IndexFile::WriteIndexFile()
{
  if(m_indexFilename.IsEmpty())
  {
    return false;
  }
  MainFrame::SetStatusText("Writing index file: " + m_indexFilename);

  // See if really necessary
  if (!m_needSaving)
  {
    return true;
  }
  Misc::ResetTokenizer();

  WinFile file(m_indexFilename.GetString());
  file.Open(winfile_write | open_trans_text, attrib_normal,AUTHOR_HTML_ENCODING);

  if(!file.GetIsOpen())
  {
    return false;
  }
  WriteHeader(file);
  WriteProperties(file);
  WriteList(file,&m_list,0);
  WriteFooter(file);
  file.Close();
  m_needSaving = false;
  return true;
}

void
IndexFile::WriteHeader(WinFile& p_file)
{
  p_file.Write(_T("<!DOCTYPE HTML PUBLIC \"-//IETF//DTD HTML//EN\">\n"));
  p_file.Write(_T("<HTML>\n"));
  p_file.Write(_T("<HEAD>\n"));
  p_file.Write(_T("</HEAD>\n"));
  p_file.Write(_T("<!-- Sitemap 1.0 -->\n"));
  p_file.Write(_T("<BODY>\n"));
}

void
IndexFile::WriteFooter(WinFile& p_file)
{
  p_file.Write(_T("</BODY>\n"));
  p_file.Write(_T("</HTML>\n"));
}

void
IndexFile::WriteProperties(WinFile& file)
{
  //  <object type="text/site properties">
  //    <param name="FrameName" value="right">
  //    <param name="SiteType" value="index">
  //   </object>
  file.Write(_T("<OBJECT type=\"text/site properties\">\n"));
  file.Write(_T("  <OBJECT name=\"SiteType\" value=\"index\">\n"));
  if(!m_frameName.IsEmpty())
  {
    file.Format(_T("  <param name=\"FrameName\" value=\"%s\">\n"),m_frameName.GetString());
  }
  file.Write(_T("</OBJECT>\n"));
}

void
IndexFile::WriteList(WinFile& file,IndexEntry* list,int level)
{
  if(list->GetInHHK() == false)
  {
    // Not to be listed in HHK file
    return;
  }

  CString levelString;
  for(int ind = 0;ind < level; ++ind)
  {
    levelString += CString("    ");
  }

  CString title = Misc::FormatXMLString(list->GetTitle());
  if(!title.IsEmpty())
  {
    file.Format(_T("%s<LI><OBJECT type=\"text/sitemap\">\n"), levelString.GetString());
    WriteParameter(file,levelString,"Name",title);

    for(unsigned int num = 0; num < list->GetDocuments().size(); ++num)
    {
      IndexDocument*  doc = list->GetDocuments()[num];
      DocumentFile* dfile = doc->m_document;
      if(dfile)
      {
        CString localfile = dfile->GetFilename();
        if (!doc->m_bookmark.IsEmpty())
        {
          localfile += CString("#") + doc->m_bookmark;
        }
        WriteParameter(file,levelString,"Name", Misc::FormatXMLString(doc->m_title));
        WriteParameter(file,levelString,"Local",Misc::FormatXMLString(localfile));
      }
    }
    WriteParameter(file,levelString,"WindowName",list->GetWindowName());
    WriteParameter(file,levelString,"FrameName", list->GetFrameName());
    WriteParameter(file,levelString,"Comment",   Misc::FormatXMLString(list->GetComment()));
    file.Format(_T("%s</OBJECT>\n"), levelString.GetString());
  }
  if(list->GetChildren().size() > 0)
  {
    file.Format(_T("%s<UL>\n"), levelString.GetString());
    for(unsigned int num = 0; num < list->GetChildren().size(); ++num)
    {
      WriteList(file,list->GetChildren()[num],(level + 1));
    }
    file.Format(_T("%s</UL>\n"), levelString.GetString());
  }
}

void
IndexFile::WriteParameter(WinFile& file,CString& levelString,LPCTSTR name,CString value)
{
  if(value.IsEmpty())
  {
    return;
  }
  value.Replace("\"","\'");
  file.Format(_T("%s    <param name=\"%s\" value=\"%s\">\n"),levelString.GetString(),name,value.GetString());
}

//////////////////////////////////////////////////////////////////////////
//
// READ THE HHK Index FILE
//
//////////////////////////////////////////////////////////////////////////

bool 
IndexFile::ReadIndexFile()
{
  if(m_indexFilename.IsEmpty())
  {
    return false;
  }
  MainFrame::SetStatusText("Reading index file: " + m_indexFilename);

  Reset();

  WinFile file(m_indexFilename.GetString());
  file.Open(winfile_read | open_trans_text);
  if(!file.GetIsOpen())
  {
    return false;
  }

  bool result = true;
  try
  {
    Misc::ResetTokenizer();
    ReadDocType(file);
    Misc::SkipToken(file,PF_HTML,m_linenumber);
    ReadHeader(file);
    ReadComment(file);
    Misc::SkipToken(file, PF_BODY, m_linenumber);
    ReadProperties(file);
    ReadList(file,&m_list,0);
    Misc::SkipToken(file,PF_ENDBODY,m_linenumber);
    Misc::SkipToken(file,PF_ENDHTML,m_linenumber);
  }
  catch(CString mess)
  {
    CString message;
    message.Format("Error reading index file '%s' Line:%i\n%s.",m_indexFilename.GetString(),m_linenumber,mess.GetString());
    theApp.ErrorMessage(message);
    result = false;
  }
  file.Close();
  m_needSaving = false;
  return result;
}

// Read past:
// <!DOCTYPE HTML PUBLIC "-//IETF//DTD HTML//EN">
// 
void
IndexFile::ReadDocType(WinFile& file)
{
  CString word;
  if (!Misc::SkipToken(file, PF_DOCTYPE, m_linenumber))
  {
    // Not a DOCTYPE, but we can continue anyway
    return;
  }
  Misc::SkipToken(file, PF_HTML,m_linenumber);
  Misc::GetToken (file, word,   m_linenumber);       // Skip PUBLIC
  Misc::GetToken (file, word,   m_linenumber);       // Skip "-//IETF//DTD HTML//EN"
}

void
IndexFile::ReadHeader(WinFile& p_file)
{
  CString word;
  TOCToken token = PF_EOF;

  if (!Misc::SkipToken(p_file, PF_HEAD, m_linenumber))
  {
    // No header, but we can continue anyway
    return;
  }
  token = Misc::GetToken(p_file, word, m_linenumber);
  if(token == PF_META)
  {
    token = Misc::GetToken(p_file, word, m_linenumber);
    if(token == PF_NAME)
    {
      Misc::SkipToken(p_file, PF_EQUAL, m_linenumber);
      if(Misc::GetToken(p_file, word, m_linenumber) != PF_STRING) throw "does not have a meta generator name";

      Misc::SkipToken(p_file,PF_CONTENT,m_linenumber);
      Misc::SkipToken(p_file,PF_EQUAL,  m_linenumber);
      if(Misc::GetToken(p_file, word, m_linenumber) != PF_STRING) throw "does not have a meta content name";
    }
  }
  while(token != PF_ENDHEAD && token != PF_EOF)
  {
    token = Misc::GetToken(p_file, word, m_linenumber);
  }
}

void
IndexFile::ReadComment(WinFile& file)
{
  // <!-- Sitemap 1.0-->
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
IndexFile::ReadProperties(WinFile& file)
{
  //  <object type="text/site properties">
  //    <param name="FrameName" value="right">
  //    <param name="SiteType" value="index">
  //    </object>
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
    if(parameterName.CompareNoCase("sitetype")  == 0) m_siteType  = parameterValue;
    if(parameterName.CompareNoCase("FrameName") == 0) m_frameName = parameterValue;
  }
  // CHECKS
  if(!m_siteType.IsEmpty())
  {
    if(m_siteType.CompareNoCase("index") != 0)
    {
      CString message;
      message.Format("ERROR: The index (HHK) file: %s\n"
                     "Has a 'sitetype' of '%s'. This is not a 'index'\n"
                     "Did you rename some files or opened a damaged location?"
                     ,m_indexFilename.GetString(),m_siteType.GetString());
      theApp.ErrorMessage(message);
    }
  }
}

bool
IndexFile::ReadList(WinFile& file,IndexEntry* list,int level)
{
  CString word;

  if(level >= RECURSION_MAX_LEVEL)
  {
    CString message;
    message.Format("Max level of recursion in index tree (HHK) reached (%i)",RECURSION_MAX_LEVEL);
    throw message;
  }

  if(!Misc::SkipToken(file,PF_LIST,m_linenumber))
  {
    // Empty index table
    return false;
  }
  IndexEntry* entry = nullptr;
  TOCToken tok = Misc::GetToken(file, word, m_linenumber);

  while(true)
  {
    // Rest of the list
    if(tok == PF_ENDLIST)
    {
      break;
    }
    // Tag </UL> sometimes not present
    if (tok == PF_ENDHTML || tok == PF_ENDBODY)
    {
      // Empty index
      Misc::PushToken(word, tok);
      return (level == 0);
    }
    if(tok == PF_EOF)
    {
      throw CString("Error in index tree: Broken or corrupted HHK file!");
    }

    // Extra level of index
    if(tok == PF_LIST)
    {
      if(!entry)
      {
        entry = new IndexEntry();
        entry->SetInHHK(true);
        list->AddChildIndex(entry);
      }
      Misc::PushToken(word,tok);
      if(!ReadList(file,entry,(level + 1)))
      {
        return false;
      }
      tok = Misc::GetToken(file, word, m_linenumber);
      continue;
    }
    else if(tok != PF_LISTITEM)
    {
      throw CString("Unknown tag in index tree. Broken or corrupted HHK file!");
    }

    // Add to list of index entries
    entry = new IndexEntry();
    entry->SetInHHK(true);
    list->AddChildIndex(entry);

    // Read one more item
    int num = 1;
    CString name;
    CString value;
    CString title;

    if(!Misc::SkipToken(file,PF_OBJECT,m_linenumber))       throw CString("Index item must be an OBJECT");
    if(!Misc::SkipToken(file,PF_TYPE,m_linenumber))         throw CString("Index item must have a type");
    if(!Misc::SkipToken(file,PF_EQUAL,m_linenumber))        throw CString("Index item type must have an equal");
    if(Misc::GetToken(file,word,m_linenumber) != PF_STRING) throw CString("Index item type must be a string");
    if(word.CompareNoCase("text/sitemap"))                  throw CString("Index item type must be 'text/sitemap'");
    if(!Misc::SkipToken(file,PF_PARAM,m_linenumber))        throw CString("Index item must have parameters");

    tok = GetIndexParameter(file,num,name,  value);
    if(name.CompareNoCase("name")    == 0 ||
       name.CompareNoCase("keyword") == 0 )
    {
      // Now we have our title of the index
      entry->SetTitle(value);
    }
    else
    {
      throw CString("Index must have a keyword 'name'");
    }
    while(tok == PF_PARAM)
    {
      ++num; // next parameter
      tok = GetIndexParameter(file,num,name,value);
      if(name.CompareNoCase("name") == 0)
      {
        title = value;
      }
      else if(name.CompareNoCase("keyword") == 0)
      {
        entry->SetTitle(value);
      }
      else if(name.CompareNoCase("local") == 0)
      {
        // allow name/local pairs 
        entry->AddDocument(title,value);
      }
      else if(name.CompareNoCase("WindowName") == 0)
      {
        entry->SetWindowName(value);
      }
      else if(name.CompareNoCase("FrameName") == 0)
      {
        entry->SetFrameName(value);
      }
      else if(name.CompareNoCase("Comment") == 0)
      {
        entry->SetComment(value);
      }
      else if(name.CompareNoCase("See also") == 0)
      {
        title = value;
      }
      else
      {
        CString message;
        message.Format("HHL Indexfile: Unknown parameter name '%s' in %dth parameter",name.GetString(),num);
        throw message;
      }
    }
    if(tok != PF_ENDOBJECT)
    {
      throw CString("Index item's object not closed with /OBJECT");
    }
    tok = Misc::GetToken(file,word,m_linenumber);

    // Tag </LI> is not always present !!
    if(tok == PF_ENDLISTITEM)
    {
      tok = Misc::GetToken(file,word,m_linenumber);
      if (tok == PF_ENDLIST)
      {
       //  break;
      }
    }
  }
  return true;
}

TOCToken
IndexFile::GetIndexParameter(WinFile& file,int num,CString& name,CString& value)
{
  CString word;
  // Get image number
  if(!Misc::SkipToken(file,PF_NAME,m_linenumber))         
  {
    ParameterError("Index item %dth parameter must have a name",num);
  }
  if(!Misc::SkipToken(file,PF_EQUAL,m_linenumber))        
  {
    ParameterError("Index item %dth parameter name must have an equal",num);
  }
  if(Misc::GetToken(file,word,m_linenumber) != PF_STRING) 
  {
    ParameterError("Index item %dth parameter name must be a string",num);
  }
  // This is our parameter
  name = word;
  if(!Misc::SkipToken(file,PF_VALUE,m_linenumber))        
  {
    ParameterError("Index item %dth parameter must have a value",num);
  }
  if(!Misc::SkipToken(file,PF_EQUAL,m_linenumber))        
  {
    ParameterError("Index item %dth parameter value must have an equal",num);
  }
  if(Misc::GetToken(file,word,m_linenumber) != PF_STRING) 
  {
    ParameterError("Index item %dth parameter value must be a string",num);
  }
  // This is our value
  value = word;

  TOCToken tok = Misc::GetToken(file,word,m_linenumber);
  return tok;
}

void
IndexFile::ParameterError(const char* error,int num)
{
  CString message;
  message.Format(error,num);
  throw message;
}

//////////////////////////////////////////////////////////////////////////
//
// Index TREE
//
//////////////////////////////////////////////////////////////////////////

void
IndexFile::DisplayIndexInTree(IndexTreeCtrl* tree)
{
  if(tree->GetCount())
  {
    tree->DeleteAllItems();
  }
  HTREEITEM root = tree->GetRootItem();
  InsertIndex(tree,root,&m_list,0);
}

void
IndexFile::InsertIndex(IndexTreeCtrl* tree,HTREEITEM item,IndexEntry* entry,int level)
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
    tree->RedisplayEntry(thisItem,entry); // boldness and image
  }
  for(unsigned int ind = 0; ind < entry->GetChildren().size(); ++ind)
  {
    InsertIndex(tree,thisItem,entry->GetChildren()[ind],level + 1);
  }
  tree->RedisplayEntry(thisItem,entry);
}

void
IndexFile::AddEntry(IndexEntry* here,IndexEntry* toinsert)
{
  if(here == NULL)
  {
    m_list.AddChildIndex(toinsert,true);
  }
  else
  {
    IndexEntry* parent = here->GetParent();
    parent->AddChildIndex(toinsert,true);
  } 
  m_needSaving = true;
}

void
IndexFile::AddChild(IndexEntry* here,IndexEntry* toinsert)
{
  if(here)
  {
    here->AddChildIndex(toinsert,true);
  }
  else
  {
    m_list.AddChildIndex(toinsert,true);
  }
  m_needSaving = true;
}

void
IndexFile::DetachEntry(IndexEntry* entry)
{
  IndexEntry* parent = entry->GetParent();
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
IndexFile::DeleteEntry(IndexEntry* entry)
{
  IndexEntry* parent = entry->GetParent();
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

//////////////////////////////////////////////////////////////////////////
//
//  Free-topic keywords: MS-HKWD
//
//////////////////////////////////////////////////////////////////////////

void
IndexFile::AddKeywords(CString keywords,DocumentFile* doc)
{
  CString keyword;
  IndexEntry* parent = &m_list;

  // Break into seperate strings.
  while(keywords.GetLength() > 0) 
  {
    int pos = keywords.Find(',');
    if(pos >= 0)
    {
      keyword  = keywords.Left(pos);
      keywords = keywords.Mid(pos);
      keywords.TrimLeft(',');
    }
     else
    {
      keyword  = keywords;
      keywords = "";
    }
    
    // Use keyword
    IndexEntry* entry = parent->FindEntry(keyword);
    if(!entry || entry->GetInHHK())
    {
      entry = new IndexEntry();
      entry->SetInHHK(false); // !!
      entry->SetTitle(keyword);
      parent->AddChildIndex(entry,true);
      m_needSaving = true;
    }
    if(keywords.IsEmpty())
    {
      // Only add to the last node in the tree.
      if(!entry->HasDocument(doc->GetFilename()))
      {
        entry->AddDocument(doc->GetTitle(),doc->GetFilename());
        m_needSaving = true;
      }
    }
    // Chain to next
    parent = entry;
  } 
}

void
IndexFile::DeleteEntry(CString keywords,CString filename)
{
  CString keyword;
  IndexEntry* parent = &m_list;
  IndexEntry* leaf   = NULL;

  // Break into seperate strings.
  while(keywords.GetLength() > 0) 
  {
    int pos = keywords.Find(',');
    if(pos >= 0)
    {
      keyword  = keywords.Left(pos);
      keywords = keywords.Mid(pos);
      keywords.TrimLeft(',');
    }
    else
    {
      keyword  = keywords;
      keywords = "";
    }
    leaf = parent->HasKeyword(keyword);
    if(leaf)
    {
      if(keywords.IsEmpty())
      {
        if(leaf->HasDocument(filename) && !leaf->GetInHHK())
        {
          // To be deleted
          leaf->DeleteDocument(filename);
          m_needSaving = true;
        }
        return;
      }
      else
      {
        // Another level
        parent = leaf;
      }
    }
  }
}

// Returns the fact if we deleted references to documents
int
IndexFile::DeleteDocumentFromTree(CString filename)
{
  m_needSaving = true;
  return m_list.DeleteDocumentRecursive(filename);
}

void 
IndexFile::SortIndex()
{
  MainFrame::SetStatusText("Sorting the index ...");
  m_list.SortEntries();
}

// Remove indexes before a reindex sweep
void
IndexFile::RemovePageIndexes()
{
  RemovePageIndexes(m_list);
}

void
IndexFile::RemovePageIndexes(IndexEntry& p_entry)
{
  for(auto& index : p_entry.GetChildren())
  {
    if(index->GetInHHK() == false)
    {
      p_entry.DeleteChild(index);
    }
  }
}
