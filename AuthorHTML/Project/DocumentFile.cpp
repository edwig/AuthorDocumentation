//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    DocumentFile.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: A DocumentFile is a part of a project file, which in it's turn
// is a part of the Microsoft HTML Help Workshop system
// If the DocumentFile is in fact an *.HTML file (most of them are)
// The attributes are stored and read from the HTML file
//
#include "StdAfx.h"
#include "DocumentFile.h"
#include "ProjectView.h"
#include "Misc.h"
#include "AuthorHTML.h"
#include "MainFrm.h"
#include "TOC.h"
#include "IndexFile.h"
#include <algorithm>

DocumentFile::DocumentFile(CString p_filename
                          ,bool    p_payload /* = false*/)
             :m_fileName(p_filename)
             ,m_payload(p_payload)
             ,m_status(0)
             ,m_priority(0)
             ,m_timeSpent(0)
             ,m_todo(0)
             ,m_width(0)
             ,m_height(0)
             ,m_metadataRead(false)
             ,m_didSweep(false)
{
}

DocumentFile::~DocumentFile()
{
  // TOCEntries are not in our ownership (TOC::m_children is the owner)
  // Just clear the map of pointers
  m_tocEntries.clear();
  // IndexEntries are not in our ownership (IndexFile::m_list is the owner)
  m_keywords.clear();
}

void    
DocumentFile::RetrieveFromDocument(CComPtr<IHTMLDocument2>& doc)
{
  // Title
  CComBSTR bTitle;
  doc->get_title(&bTitle);
  m_title = bTitle;

  m_author    = Misc::MetaTag(doc,_T("AuthorOriginalAuthor"));
  m_comment   = Misc::MetaTag(doc,_T("AuthorDocComment"));
  m_template  = Misc::MetaTag(doc,_T("AuthorHTMLTemplate"));
  m_compatible= Misc::MetaTag(doc,_T("X-UA-Compatible"),NULL,true);
  m_status    = _ttoi(Misc::MetaTag(doc,_T("AuthorStatus")));
  m_priority  = _ttoi(Misc::MetaTag(doc,_T("AuthorPriority")));
  m_timeSpent = _ttoi(Misc::MetaTag(doc,_T("AuthorTimeSpent")));
  m_todo      = _ttoi(Misc::MetaTag(doc,_T("AuthorToDo")));
  m_width     = _ttoi(Misc::MetaTag(doc,_T("AuthorWidth")));
  m_height    = _ttoi(Misc::MetaTag(doc,_T("AuthorHeight")));

  m_metadataRead = true;
}

void    
DocumentFile::SetOnDocument(CComPtr<IHTMLDocument2>& doc)
{
  // Put title on document
  CComBSTR bTitle = m_title;
  doc->put_title(bTitle);

  CString status,priority,timeSpent,todo,width,height;
  status   .Format(_T("%d"),m_status);
  priority .Format(_T("%d"),m_priority);
  timeSpent.Format(_T("%d"),m_timeSpent);
  todo     .Format(_T("%d"),m_todo);
  width    .Format(_T("%d"),m_width);
  height   .Format(_T("%d"),m_height);
  Misc::MetaTag(doc,_T("AuthorOriginalAuthor"),&m_author);
  Misc::MetaTag(doc,_T("AuthorDocComment"),    &m_comment);
  Misc::MetaTag(doc,_T("AuthorHTMLTemplate"),  &m_template);
  Misc::MetaTag(doc,_T("X-UA-Compatible"),     &m_compatible,true);
  Misc::MetaTag(doc,_T("AuthorStatus"),        &status);
  Misc::MetaTag(doc,_T("AuthorPriority"),      &priority);
  Misc::MetaTag(doc,_T("AuthorTimeSpent"),     &timeSpent);
  Misc::MetaTag(doc,_T("AuthorToDo"),          &todo);
  Misc::MetaTag(doc,_T("AuthorWidth"),         &width);
  Misc::MetaTag(doc,_T("AuthorHeight"),        &height);

  // If we have a Project grid, update it
  MainFrame* _tmain = (MainFrame*) theApp.m_pMainWnd;
  if(_tmain->m_wndProjectView)
  {
    _tmain->GetProjectView()->UpdateRecord(this);
  }
}

void    
DocumentFile::AddLinkToMe(DocumentFile* p_doc,CString& p_anchor)
{
  CString file = p_doc->GetFilename();
  file.MakeLower();
  
  PageLinks::iterator it = m_linkToMe.find(file);
  if(it == m_linkToMe.end())
  {
    LinkCount lc;
    lc.m_number = 1;
    lc.m_doc = p_doc;
    m_linkToMe.insert(std::make_pair(file,lc));
  }
  else
  {
    LinkCount lc = it->second;
    lc.m_number++;
  }
  AddAnchor(p_anchor);
}

void
DocumentFile::AddLinkFrom(DocumentFile* p_doc,CString& /* p_anchor*/)
{
  CString file = p_doc->GetFilename();
  file.MakeLower();

  PageLinks::iterator it = m_linkFrom.find(file);
  if(it == m_linkFrom.end())
  {
    LinkCount lc;
    lc.m_number = 1;
    lc.m_doc = p_doc;
    m_linkFrom.insert(std::make_pair(file,lc));
  }
  else
  {
    LinkCount lc = it->second;
    lc.m_number++;
  }
}

void    
DocumentFile::RemoveLinkToMe(DocumentFile* p_doc)
{
  CString file = p_doc->GetFilename();
  file.MakeLower();

  PageLinks::iterator it = m_linkToMe.find(file);
  if(it != m_linkToMe.end())
  {
    LinkCount lc = it->second;
    if(--lc.m_number == 0)
    {
      // Last link reaced, remove it altogether
      m_linkToMe.erase(it);
    }
  }
}

void
DocumentFile::RemoveLinkFrom(DocumentFile* p_doc)
{
  CString file = p_doc->GetFilename();
  file.MakeLower();

  PageLinks::iterator it = m_linkFrom.find(file);
  if(it != m_linkFrom.end())
  {
    LinkCount lc = it->second;
    if(--lc.m_number == 0)
    {
      // Last link reaced, remove it altogether
      m_linkFrom.erase(it);
    }
  }
}

void
DocumentFile::AddAnchor(CString& p_anchor)
{
  if(HasAnchor(p_anchor) == false)
  {
    m_anchors.push_back(p_anchor);
  }
}

bool    
DocumentFile::HasAnchor(CString& p_anchor)
{
  AnchorMap::iterator it = m_anchors.begin();
  while(it != m_anchors.end())
  {
    if(p_anchor.CompareNoCase(*it) == 0)
    {
      // Anchor already in anchormap
      return true;
    }
    // Next anchor
    ++it;
  }
  return false;
}

void    
DocumentFile::RelateToTOCEntry(TOCEntry* entry)
{
  m_tocEntries.push_back(entry);
}

void
DocumentFile::RelateToIndexEntry(IndexEntry* entry)
{

  m_keywords.push_back(entry);
}

void   
DocumentFile::UnRelateTOCEntry(TOCEntry* entry)
{
  TOCMap::iterator it = find(m_tocEntries.begin(),m_tocEntries.end(),entry);
  if(it != m_tocEntries.end())
  {
    m_tocEntries.erase(it);
    return;
  }
}

void
DocumentFile::UnRelateIndexEntry(IndexEntry* entry)
{
  IndexMap::iterator it = find(m_keywords.begin(),m_keywords.end(),entry);
  if(it != m_keywords.end())
  {
    m_keywords.erase(it);
    return;
  }
}

// Alsoo delete TOC entries and Index Entries
// Called after delete from project
void
DocumentFile::CascadeDelete()
{
  bool redisplayIDX = false;
  bool redisplayTOC = m_tocEntries.size() > 0;
  IndexFile* idx = theApp.GetIndex();
  TOC*       toc = theApp.GetTOC();

  // Delete all TOC entries pointing to this DOcumentFile
  while(m_tocEntries.size())
  {
    TOCMap::iterator it = m_tocEntries.begin();
    toc->DeleteEntry(*it);
  }
  // Delete all IndexEntries pointing to this DocumentFile
  redisplayIDX = (idx->DeleteDocumentFromTree(m_fileName) > 0);
  // Re-display TOC if neccesary
  if(redisplayTOC)
  {
    theApp.RedisplayTOC();
  }
  // Re-display Index if neccesary
  if(redisplayIDX)
  {
    theApp.RedisplayIndex();
  }
}

CString 
DocumentFile::GetRelativeDirectory()
{
  return Misc::DirectoryPart(m_fileName);
}

CString 
DocumentFile::GetRelativeFilename()
{
  return Misc::FilenamePart(m_fileName);
}

int     
DocumentFile::GetDocumentType()
{
  if(!m_payload)
  {
    return COLUMN_ICON_HTMLTYPE; // HTM or HTML
  }
  TCHAR extension[_MAX_EXT+1];
  _tsplitpath(m_fileName.GetString(),NULL,NULL,NULL,extension);
  if((_tcsicmp(extension,_T(".vbs")) == 0) ||
     (_tcsicmp(extension,_T(".js"))  == 0) )
  {
    return COLUMN_ICON_SCRIPTTYPE; // Scripttype
  }
  if((_tcsicmp(extension,_T(".bmp"))  == 0) ||
     (_tcsicmp(extension,_T(".jpeg")) == 0) ||
     (_tcsicmp(extension,_T(".jpg"))  == 0) || 
     (_tcsicmp(extension,_T(".gif"))  == 0) ||
     (_tcsicmp(extension,_T(".png"))  == 0) )
  {
    return COLUMN_ICON_IMGTYPE; // Image
  }
  // Unknown document payload type
  return -1;
}

CString
DocumentFile::GetToDoFirstDescription()
{
  CString text;

       if(m_todo & TODO_FIRSTDRAFT) text = _T("Firstdraft");
  else if(m_todo & TODO_ADDTOTOC)   text = _T("Add to TOC");
  else if(m_todo & TODO_LINKS)      text = _T("Do links"); 
  else if(m_todo & TODO_INDEX)      text = _T("Do index"); 
  else if(m_todo & TODO_IMAGES)     text = _T("Do images");
  else if(m_todo & TODO_BROWSESEQ)  text = _T("Browse sequences");
  else if(m_todo & TODO_TEST)       text = _T("Testing");  
  else if(m_todo & TODO_REVIEW)     text = _T("Review");   
  else if(m_todo & TODO_DEPLOY)     text = _T("Deploy");   

  return text;
}

CString
DocumentFile::GetPriorityDescription()
{
  CString text;
  switch(m_priority)
  {
    case PRIO_HIGH  : text = _T("High");   break;
    case PRIO_MEDIUM: text = _T("Medium"); break;
    case PRIO_LOW   : text = _T("Low");    break;
  }
  return text;
}

CString
DocumentFile::GetStatusDescription()
{
  CString text;
  switch(m_status)
  {
    case STATUS_PROGRESS: text = _T("Progress"); break;
    case STATUS_REVIEW  : text = _T("Review");   break;
    case STATUS_READY   : text = _T("Ready");    break;
  }
  return text;
}

void
DocumentFile::DisplayPageLinks(CListCtrl& p_linksFrom,CListCtrl& p_linksToMe)
{
  int nr = 0;
  TCHAR* error = _T("Cannot insert item in page link list");

  for(PageLinks::iterator it = m_linkFrom.begin();it != m_linkFrom.end(); ++it)
  {
    LinkCount     lc  = it->second;
    DocumentFile* doc = lc.m_doc;
  
    if(doc->m_payload)
    {
      continue;
    }
    LVITEM item;
    memset(&item,0,sizeof(item));
    item.mask    = LVIF_TEXT|LVIF_PARAM;
    item.pszText = (LPTSTR)(LPCTSTR) doc->GetTitle().GetString();
    item.lParam  = (LPARAM)doc;
    item.iItem   = nr++;
    if(p_linksFrom.InsertItem(&item) == -1)
    {
      theApp.Panic(error);
    }
  }
  for(PageLinks::iterator it = m_linkToMe.begin();it != m_linkToMe.end(); ++it)
  {
    LinkCount     lc  = it->second;
    DocumentFile* doc = lc.m_doc;

    if(doc->m_payload)
    {
      continue;
    }
    LVITEM item;
    memset(&item,0,sizeof(item));
    item.mask    = LVIF_TEXT|LVIF_PARAM;
    item.pszText = (LPTSTR)(LPCTSTR) doc->GetTitle().GetString();
    item.lParam  = (LPARAM)doc;
    item.iItem   = nr++;
    if(p_linksToMe.InsertItem(&item) == -1)
    {
      theApp.Panic(error);
    }
  }
}

