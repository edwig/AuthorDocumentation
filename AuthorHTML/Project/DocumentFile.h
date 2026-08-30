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
#pragma once
#include <vector>
#include <map>

class TOCEntry;
class IndexEntry;
typedef std::vector<TOCEntry*>   TOCMap;
typedef std::vector<CString>     AnchorMap;
typedef std::vector<IndexEntry*> IndexMap;

// STATUS VALUES
#define STATUS_PROGRESS      0x001
#define STATUS_REVIEW        0x002
#define STATUS_READY         0x004

// PRIORITY VALUES
#define PRIO_HIGH            0x001
#define PRIO_MEDIUM          0x002
#define PRIO_LOW             0x003

// TODO VALUES
#define TODO_FIRSTDRAFT      0x001 
#define TODO_ADDTOTOC        0x002
#define TODO_LINKS           0x004
#define TODO_INDEX           0x008
#define TODO_IMAGES          0x010
#define TODO_BROWSESEQ       0x020
#define TODO_TEST            0x040
#define TODO_REVIEW          0x080
#define TODO_DEPLOY          0x100

class DocumentFile
{
public:
  DocumentFile(CString p_filename,bool payload = false);
 ~DocumentFile();
  void    RetrieveFromDocument(CComPtr<IHTMLDocument2> &doc);
  void    SetOnDocument       (CComPtr<IHTMLDocument2> &doc);
  bool    MetaDataRead();
  void    DidReadMetaData(bool set);

  // GENERAL GETTERS AND SETTERS
  bool    GetPayload();
  CString GetRelativeDirectory();
  CString GetRelativeFilename();
  int     GetDocumentType();
  void    SetFilename(CString pFilename);
  CString GetFilename();
  void    SetTitle(CString pTitle);
  CString GetTitle();
  void    SetTemplate(CString pTemplate);
  CString GetTemplate();
  void    SetAuthor(CString pAuthor);
  CString GetAuthor();
  void    SetComment(CString pComment);
  CString GetComment();
  void    SetStatus(int pStatus);
  int     GetStatus();
  CString GetStatusDescription();
  void    SetPriority(int pPriority);
  int     GetPriority();
  CString GetPriorityDescription();
  void    SetTimeSpent(int pTimeSpent);
  int     GetTimeSpent();
  void    SetToDo(int pToDo);
  int     GetToDo();
  CString GetToDoFirstDescription();
  void    SetWidth(int pWidth);
  int     GetWidth();
  void    SetHeight(int pHeight);
  int     GetHeight();
  void    SetSweeped(bool p_sweeped);
  bool    GetSweeped();

  // LINKS
  void    AddLinkToMe(DocumentFile* p_doc,CString& p_anchor);
  void    AddLinkFrom(DocumentFile* p_doc,CString& p_anchor);
  void    RemoveLinkToMe(DocumentFile* p_doc);
  void    RemoveLinkFrom(DocumentFile* p_doc);
  void    DisplayPageLinks(CListCtrl& p_linksFrom,CListCtrl& p_linksToMe);
  void    AddAnchor(CString& p_anchor);
  bool    HasAnchor(CString& p_anchor);
  // RELATIONS
  void    RelateToTOCEntry  (TOCEntry*    entry);
  void    RelateToIndexEntry(IndexEntry*  entry);
  void    UnRelateTOCEntry  (TOCEntry*    entry);
  void    UnRelateIndexEntry(IndexEntry*  entry);
  // Alsoo delete TOC entries and Index Entries
  void    CascadeDelete();

  // Relation status
  bool    IsInTOC();
  bool    IsInIndex();

private:
  CString m_fileName;     // Where the page resides
  bool    m_payload;      // HTML is not payload, Payload: JPG, BMP, VB, JS etc
  bool    m_didSweep;     // Document file is already sweeped
  // Meta Data
  bool    m_metadataRead; // Metadata already ready
  // Actual META DATA
  CString m_title;        // Title of the page
  CString m_template;     // Template page is based on
  CString m_author;       // Original author of the page
  CString m_comment;      // First page comment
  int     m_status;       // 0=progress 1= review 2=complete
  int     m_priority;     // 0=high 1=medium 2=low
  int     m_timeSpent;    // hour/min/sec
  int     m_todo;         // To do bits
  // Editor decides on these via <BODY>
  int     m_width;        // Width of the text
  int     m_height;       // Height of the text

  // STYLE SHEETS
  // StyleSheets m_styleSheets;

  // Linking of pages, counting the links
  typedef struct _linkCount
  {
    DocumentFile* m_doc;
    int           m_number;
  }
  LinkCount;
  typedef std::map<CString,LinkCount> PageLinks;

  PageLinks m_linkFrom; // From this page to another
  PageLinks m_linkToMe; // From other pages ToMe
  // Mid-page anchors
  AnchorMap m_anchors;
  // Keywords
  IndexMap  m_keywords;
  // TOC Entries
  TOCMap    m_tocEntries;
};

inline bool
DocumentFile::GetPayload()
{
  return m_payload;
}

inline bool    
DocumentFile::MetaDataRead()
{
  return m_metadataRead;
}

inline void
DocumentFile::DidReadMetaData(bool set)
{
  m_metadataRead = set;
}

inline void    
DocumentFile::SetFilename(CString pFilename)
{
  m_fileName = pFilename;
}

inline CString 
DocumentFile::GetFilename()
{
  return m_fileName;
}

inline void  
DocumentFile::SetTitle(CString pTitle)
{
  m_title = pTitle;
}

inline CString 
DocumentFile::GetTitle()
{
  return m_title;
}

inline void    
DocumentFile::SetTemplate(CString pTemplate)
{
  m_template = pTemplate;
}

inline CString 
DocumentFile::GetTemplate()
{
  return m_template;
}

inline void    
DocumentFile::SetAuthor(CString pAuthor)
{
  m_author = pAuthor;
}

inline CString 
DocumentFile::GetAuthor()
{
  return m_author;
}

inline void    
DocumentFile::SetComment(CString pComment)
{
  m_comment = pComment;
}

inline CString 
DocumentFile::GetComment()
{
  return m_comment;
}

inline void    
DocumentFile::SetStatus(int pStatus)
{
  m_status = pStatus;
}

inline int     
DocumentFile::GetStatus()
{
  return m_status;
}

inline void    
DocumentFile::SetPriority(int pPriority)
{
  m_priority = pPriority;
}

inline int     
DocumentFile::GetPriority()
{
  return m_priority;
}

inline void    
DocumentFile::SetTimeSpent(int pTimeSpent)
{
  m_timeSpent = pTimeSpent;
}

inline int     
DocumentFile::GetTimeSpent()
{
  return m_timeSpent;
}

inline void    
DocumentFile::SetToDo(int pToDo)
{
  m_todo = pToDo;
}

inline int     
DocumentFile::GetToDo()
{
  return m_todo;
}

inline void    
DocumentFile::SetWidth(int pWidth)
{
  m_width = pWidth;
}

inline int     
DocumentFile::GetWidth()
{
  return m_width;
}

inline void    
DocumentFile::SetHeight(int pHeight)
{
  m_height = pHeight;
}

inline int     
DocumentFile::GetHeight()
{
  return m_height;
}

inline bool
DocumentFile::IsInTOC()
{
  return (m_tocEntries.size() > 0);
}

inline bool
DocumentFile::IsInIndex()
{
  return (m_keywords.size() > 0);
}

inline void
DocumentFile::SetSweeped(bool p_sweeped)
{
  m_didSweep = p_sweeped;
}

inline bool
DocumentFile::GetSweeped()
{
  return m_didSweep;
}
