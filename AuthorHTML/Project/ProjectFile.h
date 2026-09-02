//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    IndexFile.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// A project file is the *.HHP file that's used in the Microsoft 
// HTML Help compiler from the HTML Workshop Suite to compile seperate
// HTML files to a single CHM (Compiled HTML) document
//
#pragma once
#include "StdAfx.h"
#include "DocumentFile.h"
#include "WindowDefinition.h"
#include "Glossary.h"
#include "tidy.h"
#include <map>
#include <vector>

#define MAX_LINELEN 2048

typedef std::map<CString,DocumentFile*>      DocumentMap;
typedef std::map<CString,WindowDefinition*>  WindowMap;
typedef std::vector<CString>                 WindowNames;
typedef std::multimap<DocumentFile*,CString> BrokenMap;

class ProjectFile
{
public:
   ProjectFile(CString p_filename);
  ~ProjectFile();
   void    SweepProject();
   void    ResetSweeped();
   void    ResetMetadataRead();
   bool    WriteProjectFile();
   bool    ReadProjectFile();
   bool    AddDocumentFile   (CString sHtmlFile);
   bool    AddDocumentFile   (DocumentFile* doc);
   bool    RemoveDocumentFile(CString sHtmlFile);
   bool    RemoveDocumentFile(DocumentFile* doc);
   bool    NeedSaving();
   // Add an existing document (possibly with metadata and keywords)
   // to the project. Tidy up, read metadata and read keywords
   void    AddExistingDocument(CString htmlFile);
   bool    AddWindow(CString windowName);
   bool    RemoveWindow(CString windowName);
   CString GetBaseDirectory();
   DocumentFile*     FindDocumentFile(const CString& p_file);
   WindowDefinition* FindWindowDefinition(CString name);
   WindowDefinition* FirstWindowDefinition();
   void    FindRegular(CString& titleReg,CString& fileReg,DocumentMap& found);
   // Find the length of the compilelog
   int     FindCompileLength();
   // Rename a file across a project
   void    RenameFile(CString& p_old_href,CString& p_new_href);
   // Block sweep for this project
   void    BlockSweep();
   
   // Specialized getters/setters
   CString      GetProjectFilename();
   void         SetProjectFilename(CString p_filename);
   // GENERAL GETTERS
   CString      GetContentsFile();
   CString      GetCompiledName();
   CString      GetIndexFile();
   DocumentMap& GetDocumentMap();
   void         GetWindowNames(WindowNames& p_names);
   CString      GetTitle();
   void         SetTitle(CString p_title);
   CString      GetDefaultTopic();
   CString      GetDefaultFont();
   CString      GetDefaultWindow();
   bool         GetFullTextSearch();
   bool         GetBinaryIndex();
   bool         GetSweepRebuildsIndex();
   bool         GetAutoIndex();
   Glossary*    GetGlossary();
   BrokenMap*   GetBrokenMap();

   // GENERAL SETTERS
   void         SetContentsFile(CString p_content);
   void         SetCompiledName(CString p_compiled);
   void         SetIndexFile   (CString p_index);
   void         SetDefaultTopic(CString p_topic);
   void         SetDefaultFont(CString p_font);
   void         SetDefaultWindow(CString p_window);
   void         SetFullTextSearch(bool p_fullsearch);
   void         SetBinaryIndex(bool p_binaryIndex);
   void         SetAutoIndex(bool p_auto);         
private:
   void    Reset();
   int     TidyFile          (DocumentFile* docfile,CString filename);
   void    GetDocumentHeader (DocumentFile* docfile,TidyDoc& tdoc);
   void    GetDocumentTitle  (DocumentFile* docfile,TidyDoc& tdoc);
   void    GetDocumentBody   (DocumentFile* docfile,TidyDoc& tdoc);
   void    GetDocumentMeta   (TidyNode node,DocumentFile* docfile);
   void    GetDocumentPayload(TidyNode node,DocumentFile* docfile);
   bool    CheckBrokenLink   (CString& p_dir,CString& p_file);
   // Renaming
   int     RenameInOneFile(CString& p_filename,CString& p_old_href,CString& p_new_href);
   void    RenameInHeader (TidyDoc tdoc,CString& p_old_href,CString& p_new_href);
   void    RenameInBody   (TidyDoc tdoc,CString& p_old_href,CString& p_new_href);
   void    RenameInElement(TidyDoc tdoc,TidyNode node,CString& p_old_href,CString& p_new_href);

   CString m_projectFilename;
   CString m_baseDir;
   bool    m_needSaving;
   bool    m_blockSweepOnce;
   // Options
   CString m_compiledName;
   CString m_title;
   CString m_contentsFile;
   CString m_indexFile;
   CString m_defaultTopic;
   CString m_defaultWindow;
   CString m_errorLogFile;
   CString m_compatibility;
   CString m_defaultFont;
   CString m_language;
   CString m_stopFile;
   bool    m_displayProgress;
   bool    m_displayCompileNotes;
   bool    m_fullTextSearch;  
   bool    m_binaryIndex;
   bool    m_autoIndex;
   bool    m_enhancedDecompile;
   bool    m_binaryTOC;
   bool    m_flat;
   CString m_customTab;
   bool    m_sweepRebuildsIndex;
   // Windows
   WindowMap      m_windows;
   // Files
   DocumentMap    m_documents;
   // Map with broken links
   DocumentFile*  m_sweeping;
   BrokenMap      m_broken;
   // Glossary
   Glossary       m_glossary;
   // Alias files
   // MAP files
   // Text popup files
};

inline CString 
ProjectFile::GetBaseDirectory()
{
  return m_baseDir;
}

inline CString  
ProjectFile::GetContentsFile()
{
  return m_contentsFile;
}

inline CString  
ProjectFile::GetIndexFile()
{
  return m_indexFile;
}

inline DocumentMap& 
ProjectFile::GetDocumentMap()
{
  return m_documents;
}

inline CString
ProjectFile::GetCompiledName()
{
  return m_compiledName;
}

inline CString      
ProjectFile::GetTitle()
{
  return m_title;
}

inline void
ProjectFile::SetTitle(CString p_title)
{
  m_title = p_title;
}

inline CString
ProjectFile::GetDefaultTopic()
{
  return m_defaultTopic;
}

inline void
ProjectFile::SetDefaultTopic(CString p_topic)
{
  m_defaultTopic = p_topic;
}

inline CString
ProjectFile::GetDefaultFont()
{
  return m_defaultFont;
}

inline void
ProjectFile::SetDefaultFont(CString p_font)
{
  m_defaultFont = p_font;
}

inline CString
ProjectFile::GetDefaultWindow()
{
  return m_defaultWindow;
}

inline void
ProjectFile::SetDefaultWindow(CString p_window)
{
  m_defaultWindow = p_window;
}

inline bool
ProjectFile::GetFullTextSearch()
{
  return m_fullTextSearch;
}

inline void
ProjectFile::SetFullTextSearch(bool p_fullsearch)
{
  m_fullTextSearch = p_fullsearch;
}

inline bool
ProjectFile::GetBinaryIndex()
{
  return m_binaryIndex;
}

inline void
ProjectFile::SetBinaryIndex(bool p_binaryIndex)
{
  m_binaryIndex = p_binaryIndex;
}

inline bool
ProjectFile::GetSweepRebuildsIndex()
{
  return m_sweepRebuildsIndex;
}

inline bool
ProjectFile::GetAutoIndex()
{
  return m_autoIndex;
}

inline void
ProjectFile::SetAutoIndex(bool p_auto)
{
  m_autoIndex = p_auto;
}

inline Glossary*
ProjectFile::GetGlossary()
{
  return &m_glossary;
}

inline void         
ProjectFile::SetContentsFile(CString p_content)
{
  m_contentsFile = p_content;
}

inline void
ProjectFile::SetIndexFile(CString p_index)
{
  m_indexFile = p_index;
}

inline bool
ProjectFile::NeedSaving()
{
  return m_needSaving;
}

inline BrokenMap*
ProjectFile::GetBrokenMap()
{
  return &m_broken;
}

inline void
ProjectFile::BlockSweep()
{
  m_blockSweepOnce = true;
}

