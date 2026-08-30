// IndexEntry.h
//
#pragma once
#include "DocumentFile.h"
#include <vector>
#include <map>

class IndexEntry;

typedef struct _IndexDocument
{
  CString       m_title;
  DocumentFile* m_document;
}
IndexDocument;

typedef std::vector<IndexDocument*> IndDocMap;
typedef std::vector<IndexEntry*>    IndexMap;
//typedef std::map<CString,IndexEntry*> IndexMap;

typedef enum _IndexLeaf
{
   Index_Topic    // Exactly one (1) topic jump
  ,Index_Multi    // More than 1 topic to choose from
  ,Index_Alias    // Alias jump to another index key
}
IndexLeafType;

class IndexEntry
{
public:
  IndexEntry();
  IndexEntry(CString title);
  ~IndexEntry();
  void          Reset();

  IndexLeafType EntryType();
  void          AddChildIndex(IndexEntry* entry,bool sorted = false);
  void          AddDocument(CString title,CString filename);
  int           DeleteDocument(CString filename);
  int           DeleteDocumentRecursive(CString& filename);
  bool          GetOneReference();
  IndDocMap&    GetDocuments();
  IndexMap&     GetChildren();
  bool          HasChildren();
  void          DeleteChild(IndexEntry* entry,bool freeit = true);
  void          DetachChild(IndexEntry* child);
  IndexEntry*   FindEntry(CString title);
  bool          HasDocument(CString filename);
  IndexEntry*   HasKeyword(CString keyword);
  void          SortEntries();

  // General GETTERS
  CString       GetTitle();
  CString       GetComment();
  CString       GetFrameName();
  CString       GetWindowName();
  bool          GetInHHK();
  IndexEntry*   GetParent();

  // General SETTERS
  void          SetTitle(CString title);
  void          SetComment(CString comment);
  void          SetFrameName(CString frame);
  void          SetWindowName(CString window);
  void          SetInHHK(bool inHHK);
  void          SetParent(IndexEntry* parent);
private:

  CString       m_title;        // Title of this index entry
  CString       m_alias;        // Foreward reference to future index entry
  CString       m_windowName;   // Window to jump to
  CString       m_frameName;    // Frame to jump to
  CString       m_comment;      // Comment on this keyword
  bool          m_inHHK;        // Index is in HHKFile (true) or topic (false)
  IndexEntry*   m_parent;       // My parent
  IndDocMap     m_documents;    // All referenced documents
  IndexMap      m_children;     // All keyword children of this node
  IndexEntry*   m_aliasEntry;   // Alias entry
  bool          m_deleteMarker; // No documents left?
};

inline CString
IndexEntry::GetTitle()
{
  return m_title;
}

inline CString  
IndexEntry::GetComment()
{
  return m_comment;
}

inline CString
IndexEntry::GetFrameName()
{
  return m_frameName;
}

inline CString
IndexEntry::GetWindowName()
{
  return m_windowName;
}

inline bool
IndexEntry::GetInHHK()
{
  return m_inHHK;
}

inline IndexEntry*
IndexEntry::GetParent()
{
  return m_parent;
}

inline void
IndexEntry::SetTitle(CString title)
{
  m_title = title;
}

inline void
IndexEntry::SetComment(CString comment)
{
  m_comment = comment;
}

inline void
IndexEntry::SetFrameName(CString frame)
{
  m_frameName = frame;
}

inline void
IndexEntry::SetWindowName(CString window)
{
  m_windowName = window;
}

inline void
IndexEntry::SetInHHK(bool inHHK)
{
  m_inHHK = inHHK;
}

inline void
IndexEntry::SetParent(IndexEntry* parent)
{
  m_parent = parent;
}

inline bool
IndexEntry::GetOneReference()
{
  return (EntryType() == Index_Topic);
}

inline IndDocMap&  
IndexEntry::GetDocuments()
{
  return m_documents;
}

inline IndexMap&
IndexEntry::GetChildren()
{
  return m_children;
}

inline bool
IndexEntry::HasChildren()
{

  return (bool) (m_children.size() > 0);
}
