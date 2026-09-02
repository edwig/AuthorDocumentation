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

  FILE* file = fopen(m_indexFilename,"w");
  if(!file)
  {
    return false;
  }
  fprintf(file,"<html>\n");
  fprintf(file,"<!-- Sitemap 1.0 -->\n");
  WriteProperties(file);
  WriteList(file,&m_list,0);
  fprintf(file,"</html>\n");
  fclose(file);
  m_needSaving = false;
  return true;
}

void
IndexFile::WriteProperties(FILE* file)
{
  //  <object type="text/site properties">
  //    <param name="FrameName" value="right">
  //    <param name="SiteType" value="index">
  //   </object>
  fprintf(file,"<object type=\"text/site properties\">\n");
  fprintf(file,"  <param name=\"SiteType\" value=\"index\">\n");
  if(!m_frameName.IsEmpty())
  {
    fprintf(file,"  <param name=\"FrameName\" value=\"%s\">\n",(LPCTSTR)m_frameName);
  }
  fprintf(file,"</object>\n");
}

void
IndexFile::WriteList(FILE* file,IndexEntry* list,int level)
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

  CString title = list->GetTitle();
  if(!title.IsEmpty())
  {
    fprintf(file,"%s<li><object type=\"text/sitemap\">\n",(LPCTSTR)levelString);
    WriteParameter(file,levelString,"Name",title);

    for(unsigned int num = 0; num < list->GetDocuments().size(); ++num)
    {
      IndexDocument*  doc = list->GetDocuments()[num];
      DocumentFile* dfile = doc->m_document;
      if(dfile)
      {
        WriteParameter(file,levelString,"Name", doc->m_title);
        WriteParameter(file,levelString,"Local",dfile->GetFilename());
      }
    }
    WriteParameter(file,levelString,"WindowName",list->GetWindowName());
    WriteParameter(file,levelString,"FrameName", list->GetFrameName());
    WriteParameter(file,levelString,"Comment",   list->GetComment());
    fprintf(file,"%s    </object>\n",(LPCTSTR)levelString);
  }
  if(list->GetChildren().size() > 0)
  {
    fprintf(file,"%s<ul>\n",(LPCTSTR)levelString);
    for(unsigned int num = 0; num < list->GetChildren().size(); ++num)
    {
      WriteList(file,list->GetChildren()[num],(level + 1));
    }
    fprintf(file,"%s</ul>\n",(LPCTSTR)levelString);
  }
}

void
IndexFile::WriteParameter(FILE* file,CString& levelString,const char* name,CString value)
{
  if(value.IsEmpty())
  {
    return;
  }
  value.Replace("\"","\'");
  fprintf(file,"%s        <param name=\"%s\" value=\"%s\">\n",(LPCSTR)levelString,name,(LPCSTR)value);
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
  FILE* file = fopen(m_indexFilename,"r");
  if(!file)
  {
    return false;
  }
  Misc::SkipBOM(file);

  bool result = true;
  try
  {
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
    message.Format("Error reading index file '%s' Line:%i\n%s.",m_indexFilename.GetString(),m_linenumber,mess.GetString());
    theApp.ErrorMessage(message);
    result = false;
  }
  fclose(file);
  m_needSaving = false;
  return result;
}

void
IndexFile::ReadComment(FILE* file)
{
  // <!-- Sitemap 1.0 -->
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
IndexFile::ReadProperties(FILE* file)
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
IndexFile::ReadList(FILE* file,IndexEntry* list,int level)
{
  //  <ul>
  //    <li><object type="text/sitemap">
  //    <param name="Name" value="Hergebruikte deelvensters">
  //    <param name="Name" value="Inleiding standaard componenten">
  //    <param name="Local" value="Taal\Inleiding_standaard_componenten.htm">
  //    </object>
  //    <li><object type="text/sitemap">
  //    <param name="Name" value="ALS">
  //    <param name="Name" value="ALS DAN (toekenning)">
  //    <param name="Local" value="Taal\ALS_DAN_(toekenning).htm">
  //    <param name="Name" value="ALS DAN EINDALS (commando)">
  //    <param name="Local" value="Taal\ALS_DAN_EINDALS_(commando).htm">
  //    <param name="Name" value="DECLAREER (commando)">
  //    <param name="Local" value="Taal\DECLAREER_(commando).htm">
  //    </object>
  //    <ul>
  //    ..
  //    </ul>
  //  </ul>
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
  IndexEntry* entry = NULL;
  while(true)
  {
    // Rest of the list
    TOCToken tok = Misc::GetToken(file,word,m_linenumber);
    if(tok == PF_ENDLIST)
    {
      break;
    }
    if(tok == PF_ENDHTML)
    {
      // Empty index
      Misc::PushToken(word,tok);
      return (level == 0);
    }
    else if(tok == PF_EOF)
    {
      throw CString("Error in index tree: Broken or corrupted HHK file!");
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
      throw CString("Unknown tag in index tree. Broken or corrupted HHK file!");
    }
    // Add to list of index entries
    entry = new IndexEntry();
    entry->SetInHHK(true);
    list->AddChildIndex(entry);

    int num = 1;
    CString name;
    CString value;
    CString title;
    // One more item
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
  }
  return true;
}

TOCToken
IndexFile::GetIndexParameter(FILE* file,int num,CString& name,CString& value)
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
