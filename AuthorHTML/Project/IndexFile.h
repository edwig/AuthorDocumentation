//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    IndexFile.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// A index file is the *.HHK file that's used in the Microsoft 
// HTML Help compiler from the HTML Workshop Suite that contains
// all the index keywords and file references to HTML pages
//
#pragma once
#include "IndexEntry.h"
#include "IndexTreeCtrl.h"
#include "DocumentFile.h"
#include "Misc.h"

// This is a general saftye precaution. There is no real technical limit
// to the number of levels in a index tree. But if there are more than 100
// there is likely to be a programming error. 
// If users ever report this, make sure they really, ***really** want more
// than this number of nodes in there index tree!!
#define RECURSION_MAX_LEVEL 100

class IndexFile
{
public:
  IndexFile(CString indexFilename);
 ~IndexFile();
  bool ReadIndexFile();
  bool WriteIndexFile();
  bool NeedSaving();
  void RemovePageIndexes();
  void RemovePageIndexes(IndexEntry& p_entry);
  void DisplayIndexInTree(IndexTreeCtrl* tree);
  void InsertIndex(IndexTreeCtrl * tree,HTREEITEM item,IndexEntry* entry,int level);
  void AddEntry(IndexEntry* here,IndexEntry* toinsert);
  void AddChild(IndexEntry* here,IndexEntry* toinsert);
  void DetachEntry(IndexEntry* entry);
  void DeleteEntry(IndexEntry* entry);
  void DeleteEntry(CString keywords,CString filename);
  void AddKeywords(CString keywords,DocumentFile* doc);
  void SortIndex();
  int  DeleteDocumentFromTree(CString filename);

  CString GetFilename();
  void    SetFilename(CString p_filename);

private:
  void Reset();
  void ReadComment(FILE* file);
  void ReadProperties(FILE* file);
  bool ReadList(FILE* file,IndexEntry* list,int level);
  void WriteProperties(FILE* file);
  void WriteList(FILE* file,IndexEntry* list,int level);
  TOCToken GetIndexParameter(FILE* file,int num,CString& name,CString& value);
  void ParameterError(const char* error,int num);
  void WriteParameter(FILE* file,CString& levelString,const char* name,CString value);

  CString m_indexFilename;
  long    m_linenumber;
  bool    m_needSaving;
  // Index site properties
  CString m_siteType;
  CString m_frameName;

  // This is the INDEX !!
  IndexEntry m_list;
};

inline CString 
IndexFile::GetFilename()
{
  return m_indexFilename;
}

inline void
IndexFile::SetFilename(CString p_filename)
{
  m_indexFilename = p_filename;
}

inline bool
IndexFile::NeedSaving()
{
  return m_needSaving;
}

