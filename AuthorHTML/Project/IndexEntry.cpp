//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    ImportCHM.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Import a CHM file as a project directory
// by way of the standard "hhc.exe -decompile" command
//
#include "StdAfx.h"
#include "AuthorHTML.h"
#include "ProjectFile.h"
#include "IndexFile.h"
#include "IndexEntry.h"

IndexEntry::IndexEntry()
{
}

IndexEntry::IndexEntry(CString title)
           :m_title(title)
           ,m_inHHK(true)
           ,m_deleteMarker(false)
{
}

IndexEntry::~IndexEntry()
{
}

void
IndexEntry::Reset()
{
  // Delete all children
  for(unsigned int ind = 0;ind < m_children.size(); ++ind)
  {
    IndexEntry* index = m_children[ind];
    index->Reset();
    delete index;
  }
  m_children.clear();

  // Delete all documents
  for(unsigned int ind = 0;ind < m_documents.size(); ++ind)
  {
    IndexDocument* doc = m_documents[ind];
    delete doc;
  }
  m_documents.clear();
}

IndexLeafType
IndexEntry::EntryType()
{
  if(m_documents.size() == 0) return Index_Alias;
  if(m_documents.size() == 1) return Index_Topic;
  // m_documents.size() > 1
  return Index_Multi;
}

void
IndexEntry::AddChildIndex(IndexEntry* entry,bool sorted /*=false*/)
{
  // Set parent
  entry->SetParent(this);

  // Add between (sorted)
  if(sorted)
  {
    IndexMap::iterator it = m_children.begin();
    CString title = entry->GetTitle();

    while(it != m_children.end())
    {
      IndexEntry* point = *it;
      CString tit = point->GetTitle();
      if(tit.CompareNoCase(title) >= 0)
      {
        m_children.insert(it,entry);
        return;
      }
      ++it;
    }
  }
  m_children.push_back(entry);
}

void
IndexEntry::AddDocument(CString title,CString filename)
{
  IndexDocument* doc = new IndexDocument();
  doc->m_title    = title;
  doc->m_document = theApp.GetProjectFile()->FindDocumentFile(filename);
  if(doc->m_document)
  {
    // Keep this as a reference as indexentries for this document
    doc->m_document->RelateToIndexEntry(this);
  }
  m_documents.push_back(doc);
  m_deleteMarker = false;
}

int
IndexEntry::DeleteDocument(CString filename)
{
  int deleted = 0;

  IndDocMap::iterator it = m_documents.begin();
  while(it != m_documents.end())
  {
    IndexDocument* inddoc = *it;
    if(filename.CompareNoCase(inddoc->m_document->GetFilename()) == 0)
    {
      ++deleted;
      m_documents.erase(it);
      if(m_documents.size() == 0)
      {
        m_deleteMarker = true;
      }
      return deleted;
    }
    ++it;
  }
  return 0;
}

// Delete child (freeit = true)
// Detach child (freeit = false)
void
IndexEntry::DeleteChild(IndexEntry* entry,bool freeit /*=true*/)
{
  IndexMap::iterator it = m_children.begin();
  while(it != m_children.end())
  {
    if(*it == entry)
    {
      m_children.erase(it);
      if(freeit)
      {
        delete entry;
      }
      else
      {
        // Re-parent the child
        entry->m_parent = m_parent;
      }
      return;
    }
    // Next child in map
    ++it;
  }
}

void
IndexEntry::DetachChild(IndexEntry* child)
{
  DeleteChild(child,false);
}

IndexEntry*
IndexEntry::FindEntry(CString title)
{
  IndexMap::iterator it = m_children.begin();
  while(it != m_children.end())
  {
    if((*it)->GetTitle().CompareNoCase(title) == 0)
    {
      return *it;
    }
    ++it;
  }
  return NULL;
}

bool
IndexEntry::HasDocument(CString filename)
{
  IndDocMap::iterator it = m_documents.begin();

  while(it != m_documents.end())
  {
    IndexDocument* inddoc = *it;
    DocumentFile*  doc = inddoc->m_document;
    if(filename.CompareNoCase(doc->GetFilename()) == 0)
    {
      return true;
    }
    ++it;
  }
  return false;
}

IndexEntry* 
IndexEntry::HasKeyword(CString keyword)
{
  IndexMap::iterator it = m_children.begin();
  while(it != m_children.end())
  {
    if((*it)->GetTitle().CompareNoCase(keyword) == 0)
    {
      return (*it);
    }
    ++it;
  }
  return NULL;
}

int
IndexEntry::DeleteDocumentRecursive(CString& filename)
{
  int num = DeleteDocument(filename);
  IndexMap::iterator it = m_children.begin();
  while(it != m_children.end())
  {
    IndexEntry* child = *it;
    num += child->DeleteDocumentRecursive(filename);
    ++it;
  }
  return num;
}


//////////////////////////////////////////////////////////////////////////
//
// Sorting an index
//
//////////////////////////////////////////////////////////////////////////

static bool
IndexEntryCompare(IndexEntry* p_left,IndexEntry* p_right)
{
  return p_left->GetTitle().CompareNoCase(p_right->GetTitle()) < 0;
}

void
IndexEntry::SortEntries()
{
  // Sort this entry
  std::sort(m_children.begin(),m_children.end(),IndexEntryCompare);

  // Sort all my children
  for(unsigned int ind = 0;ind < m_children.size(); ++ind)
  {
    m_children[ind]->SortEntries();
  }
}
