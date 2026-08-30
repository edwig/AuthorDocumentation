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

  m_author    = Misc::MetaTag(doc,"AuthorOriginalAuthor");
  m_comment   = Misc::MetaTag(doc,"AuthorDocComment");
  m_template  = Misc::MetaTag(doc,"AuthorHTMLTemplate");
  m_status    = atoi(Misc::MetaTag(doc,"AuthorStatus"));
  m_priority  = atoi(Misc::MetaTag(doc,"AuthorPriority"));
  m_timeSpent = atoi(Misc::MetaTag(doc,"AuthorTimeSpent"));
  m_todo      = atoi(Misc::MetaTag(doc,"AuthorToDo"));
  m_width     = atoi(Misc::MetaTag(doc,"AuthorWidth"));
  m_height    = atoi(Misc::MetaTag(doc,"AuthorHeight"));

  m_metadataRead = true;
}

void    
DocumentFile::SetOnDocument(CComPtr<IHTMLDocument2>& doc)
{
  // Put title on document
  CComBSTR bTitle = m_title;
  doc->put_title(bTitle);

  CString status,priority,timeSpent,todo,width,height;
  status   .Format("%d",m_status);
  priority .Format("%d",m_priority);
  timeSpent.Format("%d",m_timeSpent);
  todo     .Format("%d",m_todo);
  width    .Format("%d",m_width);
  height   .Format("%d",m_height);
  Misc::MetaTag(doc,"AuthorOriginalAuthor",&m_author);
  Misc::MetaTag(doc,"AuthorDocComment",    &m_comment);
  Misc::MetaTag(doc,"AuthorHTMLTemplate",  &m_template);
  Misc::MetaTag(doc,"AuthorStatus",        &status);
  Misc::MetaTag(doc,"AuthorPriority",      &priority);
  Misc::MetaTag(doc,"AuthorTimeSpent",     &timeSpent);
  Misc::MetaTag(doc,"AuthorToDo",          &todo);
  Misc::MetaTag(doc,"AuthorWidth",         &width);
  Misc::MetaTag(doc,"AuthorHeight",        &height);

  // If we have a Project grid, update it
  MainFrame* main = (MainFrame*) theApp.m_pMainWnd;
  if(main->m_wndProjectView)
  {
    main->GetProjectView()->UpdateRecord(this);
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
  char extension[_MAX_EXT+1];
  _splitpath(m_fileName.GetString(),NULL,NULL,NULL,extension);
  if((stricmp(extension,".vbs") == 0) ||
     (stricmp(extension,".js")  == 0) )
  {
    return COLUMN_ICON_SCRIPTTYPE; // Scripttype
  }
  if((stricmp(extension,".bmp")  == 0) ||
     (stricmp(extension,".jpeg") == 0) ||
     (stricmp(extension,".jpg")  == 0) || 
     (stricmp(extension,".gif")  == 0) ||
     (stricmp(extension,".png")  == 0) )
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

       if(m_todo & TODO_FIRSTDRAFT) text = "Firstdraft";
  else if(m_todo & TODO_ADDTOTOC)   text = "Add to TOC";
  else if(m_todo & TODO_LINKS)      text = "Do links"; 
  else if(m_todo & TODO_INDEX)      text = "Do index"; 
  else if(m_todo & TODO_IMAGES)     text = "Do images";
  else if(m_todo & TODO_BROWSESEQ)  text = "Browse sequences";
  else if(m_todo & TODO_TEST)       text = "Testing";  
  else if(m_todo & TODO_REVIEW)     text = "Review";   
  else if(m_todo & TODO_DEPLOY)     text = "Deploy";   

  return text;
}

CString
DocumentFile::GetPriorityDescription()
{
  CString text;
  switch(m_priority)
  {
    case PRIO_HIGH  : text = "High";   break;
    case PRIO_MEDIUM: text = "Medium"; break;
    case PRIO_LOW   : text = "Low";    break;
  }
  return text;
}

CString
DocumentFile::GetStatusDescription()
{
  CString text;
  switch(m_status)
  {
    case STATUS_PROGRESS: text = "Progress"; break;
    case STATUS_REVIEW  : text = "Review";   break;
    case STATUS_READY   : text = "Ready";    break;
  }
  return text;
}

void
DocumentFile::DisplayPageLinks(CListCtrl& p_linksFrom,CListCtrl& p_linksToMe)
{
  int nr = 0;
  char* error = "Cannot insert item in page link list";

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
    item.pszText = (LPSTR)(LPCSTR) doc->GetTitle().GetString();
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
    item.pszText = (LPSTR)(LPCSTR) doc->GetTitle().GetString();
    item.lParam  = (LPARAM)doc;
    item.iItem   = nr++;
    if(p_linksToMe.InsertItem(&item) == -1)
    {
      theApp.Panic(error);
    }
  }
}

