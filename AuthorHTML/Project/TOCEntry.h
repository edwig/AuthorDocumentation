//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    TOCEntry.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Interface of the Table Of Contents (TOC)
//
#pragma once
#include "DocumentFile.h"
#include <vector>

class TOCEntry;

typedef std::vector<TOCEntry*> TOCMap;
typedef enum _TOCLeaf
{
  PF_Book = 1
 ,PF_Topic
}
TOCLeafType;

class TOCEntry
{
public:
  TOCEntry();
  TOCEntry(CString title);
  TOCEntry(CString title
          ,CString filename
          ,int imageNumber
          ,CString comment
          ,CString frameName
          ,CString windowName);
 ~TOCEntry();

  void        AddTopic(TOCEntry* entry);
  void        AddBetween(TOCEntry* here,TOCEntry* toinsert);
  void        DetachChild(TOCEntry* entry);
  void        DeleteChild(TOCEntry* entry,bool freeit = true);
  TOCLeafType EntryType();
  bool        HasChildren();
  void        MakeBook();
  void        MakePage();

  // Getters
  CString     GetTitle();
  CString     GetDocumentFilename();
  int         GetImageNumber();
  CString     GetComment();
  CString     GetFrameName();
  CString     GetWindowName();
  TOCEntry*   GetParent();
  TOCMap&     GetChildren();
  CString     GetBookmark();
  // Setters
  void        SetTitle(CString title);
  void        SetDocumentFilename(CString filename);
  void        SetImageNumber(int image);
  void        SetComment(CString comment);
  void        SetFrameName(CString frame);
  void        SetWindowName(CString window);
  void        SetBookmark(CString& p_bookmark);

private:
  TOCLeafType   m_type;
  CString       m_title;
  int           m_imageNumber;
  CString       m_comment;
  CString       m_frameName;
  CString       m_windowName;
  DocumentFile* m_document;
  CString       m_bookmark;
  // The TOCEntry tree definitions
  TOCEntry*     m_parent;
  TOCMap        m_children;
};

inline TOCLeafType
TOCEntry::EntryType()
{
  if(HasChildren() || (m_type == PF_Book))
  {
    return PF_Book;
  }
  return PF_Topic;
}

inline CString
TOCEntry::GetTitle()
{
  return m_title;
}

inline TOCMap&
TOCEntry::GetChildren()
{
  return m_children;
}

inline int
TOCEntry::GetImageNumber()
{
  return m_imageNumber;
}

inline CString     
TOCEntry::GetComment()
{
  return m_comment;
}

inline CString
TOCEntry::GetFrameName()
{
  return m_frameName;
}

inline CString     
TOCEntry::GetWindowName()
{
  return m_windowName;
}

inline bool
TOCEntry::HasChildren()
{
  return (m_children.size() > 0);
}

inline void
TOCEntry::SetTitle(CString title)
{
  m_title = title;
}

inline void
TOCEntry::SetImageNumber(int image)
{
  m_imageNumber = image;
}

inline void
TOCEntry::SetComment(CString comment)
{
  m_comment = comment;
}

inline void
TOCEntry::SetFrameName(CString frame)
{
  m_frameName = frame;
}

inline void
TOCEntry::SetWindowName(CString window)
{
  m_windowName = window;
}

inline TOCEntry*
TOCEntry::GetParent()
{
  return m_parent;
}

inline void
TOCEntry::SetBookmark(CString& p_bookmark)
{
  m_bookmark = p_bookmark;
}

inline CString
TOCEntry::GetBookmark()
{
  return m_bookmark;
}

