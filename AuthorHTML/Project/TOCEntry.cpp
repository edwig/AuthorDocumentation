//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    TOCEntry.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Interface of the Table Of Contents (TOC)
//
#include "StdAfx.h"
#include "TOCEntry.h"
#include "AuthorHTML.h"
#include "ProjectFile.h"
#include "Misc.h"

// Default static constructor
TOCEntry::TOCEntry()
         :m_document(NULL)
         ,m_parent(NULL)
         ,m_type(PF_Topic)
{
}

// Paragraph constructor
TOCEntry::TOCEntry(CString title)
         :m_title(title)
         ,m_document(NULL)
         ,m_parent(NULL)
         ,m_type(PF_Topic)
{
}

// Topic constructor
TOCEntry::TOCEntry(CString title
                  ,CString filename
                  ,int     imageNumber
                  ,CString comment
                  ,CString frameName
                  ,CString windowName
                  )
         :m_title(title)
         ,m_imageNumber(imageNumber)
         ,m_comment(comment)
         ,m_frameName(frameName)
         ,m_windowName(windowName)
         ,m_document(NULL)
         ,m_parent(NULL)
         ,m_type(PF_Topic)
{
  Misc::SplitMidpageAnchor(filename,filename,m_bookmark);

  m_document = theApp.GetProjectFile()->FindDocumentFile(filename);
  if(m_document)
  {
    m_document->RelateToTOCEntry(this);
  }
}

TOCEntry::~TOCEntry()
{
  // Loose coupling with document
  if(m_document)
  {
    m_document->UnRelateTOCEntry(this);
  }
  // Cascade the entries under me
  for(unsigned int ind = 0; ind < m_children.size(); ++ind)
  {
    TOCEntry* entry = m_children[ind];
    delete entry;
  }
  // Clear the childrens map
  m_children.clear();
}

void
TOCEntry::SetDocumentFilename(CString filename)
{
  CString anchor;
  CString file;
  Misc::SplitMidpageAnchor(filename,file,anchor);

  DocumentFile* document = theApp.GetProjectFile()->FindDocumentFile(file);
  if(document != m_document)
  {
    if(m_document)
    {
      m_document->UnRelateTOCEntry(this);
    }
    m_document = document;
    m_document->RelateToTOCEntry(this);
  }
}

void
TOCEntry::AddTopic(TOCEntry* entry)
{
  entry->m_parent = this;
  m_children.push_back(entry);
}

void
TOCEntry::AddBetween(TOCEntry* here,TOCEntry* toinsert)
{
  TOCMap::iterator it = m_children.begin();

  toinsert->m_parent = this;
  while(it != m_children.end())
  {
    if(*it == here)
    {
      m_children.insert(++it,toinsert);
      return;
    }
    // Next child in map
    ++it;
  }
  // Not found? Simply add at the end
  m_children.push_back(toinsert);
}

CString     
TOCEntry::GetDocumentFilename()
{
  CString filename;
  if(m_document)
  {
    filename = m_document->GetFilename();
    if(!m_bookmark.IsEmpty())
    {
      filename += "#" + m_bookmark;
    }
  }
  return filename;
}

// Delete child (freeit = true)
// Detach child (freeit = false)
void
TOCEntry::DeleteChild(TOCEntry* entry,bool freeit /*=true*/)
{
  TOCMap::iterator it = m_children.begin();
  while(it != m_children.end())
  {
    if(*it == entry)
    {
      m_children.erase(it);
      if(freeit)
      {
        delete entry;
      }
      return;
    }
    // Next child in map
    ++it;
  }
}

void
TOCEntry::DetachChild(TOCEntry* entry)
{
  DeleteChild(entry,false);
}

void
TOCEntry::MakeBook()
{
  if(!HasChildren())
  {
    m_type = PF_Book;
  }
}

void
TOCEntry::MakePage()
{
  if(!HasChildren())
  {
    m_type = PF_Topic;
  }
}
