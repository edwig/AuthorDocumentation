//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    TOC.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Interface of the Table Of Contents (TOC)
//
#pragma once
#include "TOCEntry.h"
#include "TOCTreeCtrl.h"
#include "Misc.h"

// This is a general safty precaution. There is no real technical limit
// to the number of levels in a TOC tree. But if there are more than 100
// there is likely to be a programming error. 
// If users ever report this, make sure they really, ***really** want more
// than this number of nodes in there TOC tree!!
#define RECURSION_MAX_LEVEL 100

class TOC
{
public:
  TOC(CString tocFilename);
 ~TOC();
  bool ReadTOCFile();
  bool WriteTOCFile();
  bool NeedSaving();
  void DisplayTOCInTree(TOCTreeCtrl* tree);
  void InsertTOC(TOCTreeCtrl* tree,HTREEITEM item,TOCEntry* entry,int level);
  void AddEntry(TOCEntry* here,TOCEntry* toinsert);
  void AddChild(TOCEntry* here,TOCEntry* toinsert);
  void DetachEntry(TOCEntry* entry);
  void DeleteEntry(TOCEntry* entry);
  void MoveEntry(TOCEntry* here,TOCEntry* tomove);

  CString GetFilename();
  void    SetFilename(CString p_filename);

private:
  void     Reset();
  void     ReadProperties(FILE* file);
  void     ReadComment(FILE* file);
  bool     ReadList(FILE* file,TOCEntry* list,int level);
  void     WriteProperties(FILE* file);
  void     WriteList(FILE* file,TOCEntry* list,int level);
  void     WriteParameter(FILE* file,CString& levelString,const char* name,CString value);
  TOCToken GetTOCParameter(FILE* file,int num,CString& name,CString& value);
  void     ParameterError(const char* error,int num);

  CString       m_tocFilename;
  long          m_linenumber;
  bool          m_needSaving;
  // TOC site properties
  CString       m_siteType;
  int           m_imageWidth;
  unsigned long m_windowStyles;
  unsigned long m_ExWindowStyles;

  // This is the TOC!!
  TOCEntry      m_list;
};

inline CString 
TOC::GetFilename()
{
  return m_tocFilename;
}

inline void
TOC::SetFilename(CString p_filename)
{
  m_tocFilename = p_filename;
}

inline bool
TOC::NeedSaving()
{
  return m_needSaving;
}