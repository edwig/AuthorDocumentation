//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    IndexFile.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// A project file is the *.HHP file that's used in the Microsoft 
// HTML Help compiler from the HTML Workshop Suite to compile seperate
// HTML files to a single CHM (Compiled HTML) document
//
#include "StdAfx.h"
#include "ProjectFile.h"
#include "DocumentFile.h"
#include "IndexFile.h"
#include "TOC.h"
#include "AuthorHTML.h"
#include "SweepDlg.h"
#include "MainFrm.h"
#include "Misc.h"
#include "tidy.h"
#include "buffio.h"
#include "RegExp.h"
#include <WinFile.h>
#include <io.h>

ProjectFile::ProjectFile(CString p_projectfile)
            :m_projectFilename(p_projectfile)
            ,m_displayProgress(false)
            ,m_displayCompileNotes(false)
            ,m_fullTextSearch(false)
            ,m_binaryIndex(false)
            ,m_autoIndex(false)
            ,m_enhancedDecompile(false)
            ,m_binaryTOC(false)
            ,m_flat(false)
            ,m_dbcs(false)
            ,m_needSaving(false)
            ,m_blockSwipeOnce(false)
            ,m_sweeping(NULL)
{
  // Base directory is taken from the project file
  m_baseDir = Misc::DirectoryPart(p_projectfile);
}

ProjectFile::~ProjectFile()
{
  Reset();
}

void
ProjectFile::Reset()
{
  for(WindowMap::iterator it = m_windows.begin();
      it != m_windows.end(); ++it)
  {
    delete it->second;
  }
  m_windows.clear();
  for(DocumentMap::iterator it = m_documents.begin();
    it != m_documents.end(); ++it)
  {
    delete it->second;
  }
  m_documents.clear();
  m_broken.clear();

  //m_baseDir                 = "";
  m_compiledName            = _T("");
  m_title                   = _T("");
  m_contentsFile            = _T("");
  m_indexFile               = _T("");
  m_defaultTopic            = _T("");
  m_defaultWindow           = _T("");
  m_errorLogFile            = _T("");
  m_customTab               = _T("");
  m_compatibility           = _T("");
  m_defaultFont             = _T("");
  m_language                = _T("");
  m_stopFile                = _T("");
  m_displayProgress         = false;
  m_displayCompileNotes     = false;
  m_fullTextSearch          = false;
  m_binaryIndex             = false;
  m_autoIndex               = false;
  m_enhancedDecompile       = false;
  m_binaryTOC               = false;
  m_flat                    = false;
  m_needSaving              = false;
  m_dbcs                    = false;
}

CString 
ProjectFile::GetProjectFilename()
{
  return Misc::FilenamePart(m_projectFilename);
}

void
ProjectFile::SetProjectFilename(CString p_filename,bool p_create /*= false*/)
{
  CString base = theApp.GetBaseDirectory();
  CString org_project = m_projectFilename;
  CString org_index;
  CString org_toc;

  CString file   = Misc::FilenamePart(p_filename);
  CString extens = Misc::ExtensionPart(p_filename);
  if(extens.Compare(_T(".hhp")))
  {
    int pos = file.Find(_T('.'));
    if(pos > 0)
    {
      file = file.Left(pos);
    }
  }
  m_projectFilename = base + file;
  if(extens.Compare(_T(".hhp")))
  {
    m_projectFilename += _T(".hhp");
  }

  // Changing the TOC (*.HHC) and Index-keywords (*.HHK) filenames
  CString tocFile;
  CString idxFile;
  TOC* toc = theApp.GetTOC();
  if(toc)
  {
    org_toc = toc->GetFilename();
    m_contentsFile = file + _T(".hhc");
    tocFile = base + m_contentsFile;
    toc->SetFilename(tocFile);
  }
  IndexFile* idx = theApp.GetIndex();
  if(idx)
  {
    org_index = idx->GetFilename();
    m_indexFile = file + _T(".hhk");
    idxFile = base + m_indexFile;
    idx->SetFilename(idxFile);
  }
  
  // Rewrite HHC/HHK in the window's definitions
  WindowMap::iterator it = m_windows.begin();
  while(it != m_windows.end())
  {
    WindowDefinition* win = it->second;
    win->SetHHCFile(Misc::FilenamePart(tocFile));
    win->SetHHKFile(Misc::FilenamePart(idxFile));
    // Next window
    ++it;
  }

  bool rewritten = true;
  // Write the new files first
  if(!     WriteProjectFile()) rewritten = false;
  if(!idx->WriteIndexFile())   rewritten = false;
  if(!toc->WriteTOCFile())     rewritten = false;

  // Ready creating
  if(p_create)
  {
    return;
  }

  // If successful, remove the old ones
  if(rewritten)
  {
    bool deleted = true;
    if(!DeleteFile(org_project)) deleted = false;
    if(!DeleteFile(org_index))   deleted = false;
    if(!DeleteFile(org_toc))     deleted = false;
    if(!deleted)
    {
      theApp.MessageBox(_T("Not all of the original project files (hhp,hhc,hhk) have been removed"),_T("Warning"),MB_OK|MB_ICONWARNING);
    }
  }
  else
  {
    theApp.MessageBox(_T("The new project files (hhp,hhc,hhk) could not be created.\n")
                      _T("The project renaming proces is aborted")
                     ,_T("Error")
                     ,MB_OK|MB_ICONERROR);
    m_projectFilename = org_project;
    if(idx && !org_index.IsEmpty())
    {
      idx->SetFilename(org_index);
    }
    if(toc && !org_toc.IsEmpty())
    {
      toc->SetFilename(org_toc);
    }
  }
}

void
ProjectFile::SetCompiledName(CString p_compiled)
{
  CString compiled(p_compiled);
  compiled.MakeLower();
  if(compiled.Find(_T(".chm")) < 0)
  {
    p_compiled += _T(".chm");
  }
  m_compiledName = p_compiled;
}

bool 
ProjectFile::WriteProjectFile()
{
  if(m_projectFilename.IsEmpty())
  {
    return false;
  }
  MainFrame::SetStatusText(_T("Writing project file") + m_projectFilename);

  m_glossary.WriteToFile();

  // See if really necessary
  if(!m_needSaving)
  {
    return true;
  }
  WinFile file(m_projectFilename.GetString());
  if(m_dbcs)
  {
    file.SetDBCSMode(true,Encoding::LE_UTF16);
  }
  if(!file.Open(winfile_write | open_trans_text,attrib_normal,m_dbcs ? Encoding::LE_UTF16 : Encoding::EN_ACP))
  {
    return false;
  }
  // 1) WRITE ALL THE OPTIONS
  file.Write(_T("[OPTIONS]\n"));
  file.Format(_T("Compiled file=%s\n"),   m_compiledName.GetString());
  file.Format(_T("Compatibility=%s\n"),   m_compatibility.GetString());
  file.Format(_T("Title=%s\n"),           m_title.GetString());
  file.Format(_T("Contents File=%s\n"),   m_contentsFile.GetString());
  file.Format(_T("Index File=%s\n"),      m_indexFile.GetString());
  file.Format(_T("Default Topic=%s\n"),   m_defaultTopic.GetString());
  file.Format(_T("Default Window=%s\n"),  m_defaultWindow.GetString());
  file.Format(_T("Default Font=%s\n"),    m_defaultFont.GetString());
  file.Format(_T("Error log file=%s\n"),  m_errorLogFile.GetString());
  file.Format(_T("Custom tab=%s\n"),      m_customTab.GetString());
  file.Format(_T("Language=%s\n"),        m_language.GetString());
  file.Format(_T("Full text search stop list file=%s\n"), m_stopFile.GetString());
  file.Format(_T("Display compile progress=%s\n"), m_displayProgress  ? _T("yes") : _T("no"));
  file.Format(_T("Display compile notes=%s\n"), m_displayCompileNotes ? _T("yes") : _T("no"));
  file.Format(_T("Full-text search=%s\n"),      m_fullTextSearch      ? _T("yes") : _T("no"));
  file.Format(_T("Binary index=%s\n"),          m_binaryIndex         ? _T("yes") : _T("no"));
  file.Format(_T("Binary TOC=%s\n"),            m_binaryTOC           ? _T("yes") : _T("no"));
  file.Format(_T("Auto index=%s\n"),            m_autoIndex           ? _T("yes") : _T("no"));
  file.Format(_T("Enhanced decompilation=%s\n"),m_enhancedDecompile   ? _T("yes") : _T("no"));
  file.Format(_T("Flat=%s\n"),                  m_flat                ? _T("yes") : _T("no"));
  file.Format(_T("DBCS=%s\n"),                  m_dbcs                ? _T("yes") : _T("no"));

  if(m_glossary.HasEntries())
  {
    file.Write(_T("Custom tab=\"&Glossary\",HHActiveX.GlossaryPane\n"));
  }
  file.Write(_T("\n"));

  // 2) WRITE ALL WINDOWS
  file.Write(_T("[WINDOWS]\n"));
  WindowMap::iterator wit;
  for(wit = m_windows.begin(); wit != m_windows.end(); ++wit)
  {
    WindowDefinition* win = wit->second;
    file.Format(_T("%s\n"),win->DefinitionString().GetString());
  }
  file.Write(_T("\n"));

  // 3) WRITE ALL FILES
  file.Write(_T("[FILES]\n"));
  DocumentMap::iterator it;
  for(it = m_documents.begin(); it != m_documents.end();++it)
  {
    DocumentFile* doc = it->second;
    file.Format(_T("%s\n"),doc->GetFilename().GetString());
  }
  // Glossary goes in the files section
  if(m_glossary.HasEntries())
  {
    CString fileName = m_glossary.GetFilename();
    fileName = Misc::FilenamePart(fileName);
    file.Format(_T("%s\n"), fileName.GetString());
  }
  file.Write(_T("\n"));

  // 4) WRITE ALIASSES
  file.Write(_T("[ALIAS]\n"));

  // 5) WRITE MAP
  file.Write(_T("[MAP]\n"));

  // 6) WRITE TEXT POPUPS
  file.Write(_T("[TEXT POPUPS]\n"));

  // 7) MERGE FILES
  file.Close();

  // Reset saving
  m_needSaving = false;

  return true;
}

void
ProjectFile::DetectDBCSMode()
{
  WinFile file(m_projectFilename.GetString());
  if(file.Open(winfile_read | open_trans_binary, attrib_normal))
  {
    BYTE buffer[40];
    int didread = 0;
    file.Read(buffer,40,didread);

    wchar_t* options_utf16 = L"[OPTIONS]";

    if(memcmp(buffer, options_utf16, 18) == 0)
    {
      m_dbcs = true;
    }
    file.Close();
  }
}

bool 
ProjectFile::ReadProjectFile()
{
  if(m_projectFilename.IsEmpty())
  {
    return false;
  }
  // Reset everything except the project filename
  Reset();
  MainFrame::SetStatusText(_T("Reading project file: ") + m_projectFilename);

  DetectDBCSMode();
  WinFile file(m_projectFilename.GetString());
  if(m_dbcs)
  {
    file.SetDBCSMode(true,Encoding::LE_UTF16);
  }
  file.Open(winfile_read|open_trans_text);
  if(!file.GetIsOpen())
  {
    return false;
  }
  Misc::ResetTokenizer();

  XString buffer;
  bool options = false;
  bool windows = false;
  bool files   = false;
  bool aliases = false;
  bool maps    = false;
  bool popups  = false;

  while(file.Read(buffer))
  {
    int len = buffer.GetLength();
    if((len > 0) && (buffer.GetAt(len - 1) == _T('\n')))
    {
      buffer = buffer.Left(buffer.GetLength() - 1);
    }
    if(len < 2)
    {
      // Guards against empty lines
      continue;
    }
    // RECOGNIZE OUR SECTION !!
    if(buffer.Left(9).Compare(_T("[OPTIONS]")) == 0)
    {
      options = true;
      windows = files = aliases = maps = popups = false;
      continue;
    }
    if(buffer.Left(9).Compare(_T("[WINDOWS]")) == 0)
    {
      windows = true;
      options = files = aliases = maps = popups = false;
      continue;
    }
    if(buffer.Left(7).Compare(_T("[FILES]")) == 0)
    {
      files = true;
      options = windows = aliases = maps = popups = false;
      continue;
    }
    if(buffer.Left(7).Compare(_T("[ALIAS]")) == 0)
    {
      aliases = true;
      options = windows = files = maps = popups = false;
      continue;
    }
    if(buffer.Left(5).Compare(_T("[MAP]")) == 0)
    {
      maps = true;
      options = windows = files = aliases = popups = false;
      continue;
    }
    if(buffer.Left(13).Compare(_T("[TEXT POPUPS]")) == 0)
    {
      popups = true;
      options = windows = files = aliases = maps = false;
      continue;
    }
    // HANDLE OUR SECTIONS
    if(options)
    {
      // OPTIONS IN THE HEADER OF THE PROJECT FILE
      CString value;
      int assign = buffer.Find(_T('='));
      if (assign > 0)
      {
        value  = buffer.Mid(assign + 1);
        buffer = buffer.Left(assign);
      }
      CString theOption(buffer);
      CString theValue (value);
      int     unsupported = 0;

           if(theOption.CompareNoCase(_T("compiled file"))            == 0) m_compiledName        = theValue;
      else if(theOption.CompareNoCase(_T("compatibility"))            == 0) m_compatibility       = theValue;
      else if(theOption.CompareNoCase(_T("title"))                    == 0) m_title               = theValue;
      else if(theOption.CompareNoCase(_T("contents file"))            == 0) m_contentsFile        = theValue;
      else if(theOption.CompareNoCase(_T("index file"))               == 0) m_indexFile           = theValue;
      else if(theOption.CompareNoCase(_T("default topic"))            == 0) m_defaultTopic        = theValue;
      else if(theOption.CompareNoCase(_T("default window"))           == 0) m_defaultWindow       = theValue; 
      else if(theOption.CompareNoCase(_T("default font"))             == 0) m_defaultFont         = theValue;
      else if(theOption.CompareNoCase(_T("error log file"))           == 0) m_errorLogFile        = theValue;
      else if(theOption.CompareNoCase(_T("custom tab"))               == 0) m_customTab           = theValue;
      else if(theOption.CompareNoCase(_T("language"))                 == 0) m_language            = theValue;
      else if(theOption.CompareNoCase(_T("full text search stop list file")) == 0) m_stopFile     = theValue;
      else if(theOption.CompareNoCase(_T("display compile progress")) == 0) m_displayProgress     = (theValue.CompareNoCase(_T("yes"))==0 ? true : false);
      else if(theOption.CompareNoCase(_T("display compile notes"))    == 0) m_displayCompileNotes = (theValue.CompareNoCase(_T("yes"))==0 ? true : false);
      else if(theOption.CompareNoCase(_T("full-text search"))         == 0) m_fullTextSearch      = (theValue.CompareNoCase(_T("yes"))==0 ? true : false);
      else if(theOption.CompareNoCase(_T("binary index"))             == 0) m_binaryIndex         = (theValue.CompareNoCase(_T("yes"))==0 ? true : false);
      else if(theOption.CompareNoCase(_T("auto index"))               == 0) m_autoIndex           = (theValue.CompareNoCase(_T("yes"))==0 ? true : false);
      else if(theOption.CompareNoCase(_T("Enhanced decompilation"))   == 0) m_enhancedDecompile   = (theValue.CompareNoCase(_T("yes"))==0 ? true : false);
      else if(theOption.CompareNoCase(_T("binary toc"))               == 0) m_binaryTOC           = (theValue.CompareNoCase(_T("yes"))==0 ? true : false);
      else if(theOption.CompareNoCase(_T("flat"))                     == 0) m_flat                = (theValue.CompareNoCase(_T("yes"))==0 ? true : false);
      else if(theOption.CompareNoCase(_T("Auto Index"))               == 0) unsupported++;
      else if(theOption.CompareNoCase(_T("Auto TOC"))                 == 0) unsupported++;
      else if(theOption.CompareNoCase(_T("citation"))                 == 0) unsupported++;
      else if(theOption.CompareNoCase(_T("compress"))                 == 0) unsupported++;
      else if(theOption.CompareNoCase(_T("copyright"))                == 0) unsupported++;
      else if(theOption.CompareNoCase(_T("create chi file"))          == 0) unsupported++;
      else if(theOption.CompareNoCase(_T("DBCS"))                     == 0) unsupported++;
      else if(theOption.CompareNoCase(_T("ignore"))                   == 0) unsupported++;
      else if(theOption.CompareNoCase(_T("prefix"))                   == 0) unsupported++;
      else if(theOption.CompareNoCase(_T("sample staging path"))      == 0) unsupported++;
      else if(theOption.CompareNoCase(_T("sample list file"))         == 0) unsupported++;
      else if(theOption.CompareNoCase(_T("tmpdir"))                   == 0) unsupported++;
      else 
      {
        CString message;
        message.Format(_T("Unknown option in HHP file: %s\n")
                       _T("We will continue reading the project file, but you must repair it by hand!")
                      ,theOption.GetString());
        theApp.ErrorMessage(message);
      }
    }
    else if(windows)
    {
      WindowDefinition* win = new WindowDefinition(this);
      if(!win->ParseDefinitionString(buffer.GetString()))
      {
        delete win;
      }
      else
      {
        CString winName = win->GetName();
        winName.MakeLower();
        m_windows.insert(std::make_pair(winName,win));
      }
    }
    else if(files)
    {
      // Every line now refers to a file
      // ADD A FILE REFERENCE to a HTM(L) FILE or a payload file
      AddDocumentFile(_T(""),buffer.GetString());
    }
    else if(aliases)
    {
      // TODO
    }
    else if(maps)
    {
      // TODO
    }
    else if(popups)
    {
      // TODO
    }
    else
    {
      // ERROR
      ASSERT(FALSE);
    }
  }
  file.Close();
  m_needSaving = false;

  // Try the glossary file from the project filename
  m_glossary.ReadFromFile(m_projectFilename);

  // General checks for AuthorDocumentation
  if(m_flat)
  {
    theApp.MessageBox(_T("The 'flat' compile option was set but can never be used by AuthorDocumentation.\n")
                      _T("This is an option that removes the folder structures from a compiled help file.\n")
                      _T("AuthorDocumentation cannot cope with this option in inter-file jumps,\n")
                      _T("therefore this option will now be reset to false")
                     ,_T("Warning")
                     ,MB_OK|MB_ICONWARNING|MB_TASKMODAL);
    m_flat = false;
  }
  if(m_compatibility.IsEmpty() || m_compatibility.CompareNoCase(_T("1.1 or later")))
  {
    m_compatibility = _T("1.1 or later");
    m_needSaving = true;
  }
  if(m_errorLogFile.IsEmpty())
  {
    m_errorLogFile = _T("CompileErrors.log");
    m_needSaving = true;
  }
  m_displayProgress     = true;
  m_displayCompileNotes = true;

  return true;
}

bool
ProjectFile::CheckBrokenLink(CString& p_dir,CString& p_reldir, CString& p_file)
{
  CString fullpath = p_dir + p_reldir + p_file;
  if(_taccess(fullpath,04) == 0)
  {
    // File exists. No problems
    return false;
  }
  // Link to an external source
  if((p_file.Find(_T("http")) == 0) || (p_file.Find(_T("mailto:")) == 0))
  {
    return true;
  }
  // Register as a broken link
  m_broken.insert(std::make_pair(m_sweeping,p_file));

  // Broken link
  return true;
}

// Add document purely by filename
bool 
ProjectFile::AddDocumentFile(CString p_relativeDirectory,CString sHtmlFile,bool p_newdoc /* = false */)
{
  bool payload = true;
  CString file = Misc::RemoveBasePart(m_baseDir,sHtmlFile);
  file = Misc::StripFileProtocol(file);

  TCHAR extension[_MAX_EXT+1];
  _tsplitpath(sHtmlFile.GetString(),NULL,NULL,NULL,extension);
  if((_tcsicmp(extension,_T(".htm"))  == 0) ||
     (_tcsicmp(extension,_T(".html")) == 0) )
  {
    payload = false;
  }
  if(_tcsicmp(extension,_T(".glo")) == 0)
  {
    // Glossary does not goes with the payloads
    return false;
  }
  bool brokenLink = CheckBrokenLink(m_baseDir,p_relativeDirectory,file);

  if(p_newdoc || !brokenLink)
  {
    CString totalfilename = m_baseDir + p_relativeDirectory + file;
    CString projectfile   = Misc::ReduceDirectoryPath(totalfilename);
    CString filename      = projectfile.Mid(m_baseDir.GetLength());

    CString searchName = filename;
    searchName.MakeLower();
    if(m_documents.find(searchName) == m_documents.end())
    {
      DocumentFile* doc = new DocumentFile(filename,payload);
      // Place in htmlFiles map on lower-case name
      m_documents.insert(std::make_pair(searchName,doc));
      m_needSaving = true;
      return true;
    }
  }
  return false;
}

// Add existing documentfile
bool
ProjectFile::AddDocumentFile(DocumentFile* doc)
{
  CString file = doc->GetFilename();
  file.MakeLower();
  if(m_documents.find(file) == m_documents.end())
  {
    m_documents.insert(std::make_pair(file,doc));
    m_needSaving = true;
    return true;
  }
  return false;
}

bool 
ProjectFile::RemoveDocumentFile(CString sHtmlFile)
{
  CString file = Misc::RemoveBasePart(m_baseDir,sHtmlFile);
  file = Misc::StripFileProtocol(file);
  file.MakeLower();
  DocumentMap::iterator it = m_documents.find(file);
  if(it != m_documents.end())
  {
    DocumentFile* doc = it->second;
    doc->CascadeDelete();
    delete doc;
    m_documents.erase(it);
    m_needSaving = true;
    return true;
  }
  return false;
}

bool 
ProjectFile::RemoveDocumentFile(DocumentFile* doc)
{
  if(!doc)
  {
    // All OK, No document to remove
    return TRUE;
  }
  CString filename = doc->GetFilename();
  filename.MakeLower();
  DocumentMap::iterator it = m_documents.find(filename);
  if(it != m_documents.end())
  {
    doc->CascadeDelete();
    delete doc;
    m_documents.erase(it);
    m_needSaving = true;
    return true;
  }
  return false;
}

DocumentFile*
ProjectFile::FindDocumentFile(const CString& p_file)
{
  CString anchor;
  CString filename;
  Misc::SplitMidpageAnchor(p_file,filename,anchor);
  filename = Misc::StripFileProtocol(filename);

  filename.MakeLower();
  DocumentMap::iterator it = m_documents.find(filename);
  if(it != m_documents.end())
  {
    return it->second;
  }
  return NULL;
}

bool
ProjectFile::AddWindow(CString windowName)
{
  CString name = windowName;
  name.MakeLower();
  WindowMap::iterator it = m_windows.find(name);
  if(it == m_windows.end())
  {
    WindowDefinition* window = new WindowDefinition(this);
    window->SetName(windowName);
    m_windows.insert(std::make_pair(name,window));
    m_needSaving = true;
  }
  return true;
}

bool
ProjectFile::RemoveWindow(CString windowName)
{
  windowName.MakeLower();
  WindowMap::iterator it = m_windows.find(windowName);
  if(it != m_windows.end())
  {
    delete it->second;
    m_windows.erase(it);
    m_needSaving = true;
    return true;
  }
  return false;
}

WindowDefinition* 
ProjectFile::FindWindowDefinition(CString name)
{
  name.MakeLower();
  WindowMap::iterator it = m_windows.find(name);
  if(it != m_windows.end())
  {
    return it->second;
  }
  return NULL;
}

WindowDefinition*
ProjectFile::FirstWindowDefinition()
{
  WindowMap::iterator it = m_windows.begin();
  if(it != m_windows.end())
  {
    return it->second;
  }
  return NULL;
}

void
ProjectFile::GetWindowNames(WindowNames& p_names)
{
  WindowMap::iterator it = m_windows.begin();
  while(it != m_windows.end())
  {
    CString wname = it->first;
    p_names.push_back(wname);
    m_needSaving = true;
    ++it;
  }
}

// Add an existing document (possibly with metadata and keywords)
// to the project. Tidy up, read metadata and read keywords
void
ProjectFile::AddExistingDocument(CString htmlFile)
{
  bool payload = true;
  CString file = Misc::RemoveBasePart(m_baseDir,htmlFile);
  file = Misc::StripFileProtocol(file);

  TCHAR extension[_MAX_EXT+1];
  _tsplitpath(htmlFile.GetString(),NULL,NULL,NULL,extension);
  if((_tcsicmp(extension,_T(".htm"))  == 0) ||
     (_tcsicmp(extension,_T(".html")) == 0) )
  {
    payload = false;
  }
  DocumentFile* doc = new DocumentFile(file,payload);
  file.MakeLower();
  // Place in htmlFiles map on lower-case name
  m_documents.insert(std::make_pair(file,doc));
  // Sweep for keywords and metadata
  TidyFile(doc,htmlFile);

  // Add to keywords Pane
  theApp.RedisplayIndex();

  // Needs saving
  m_needSaving = true;
}

// Reset, so we can read the keywords again
void
ProjectFile::ResetMetadataRead()
{
  for(auto& doc : m_documents)
  {
    doc.second->DidReadMetaData(false);
  }
}

void
ProjectFile::ResetSweeped()
{
  // Remove the swiped indicator
  // so we can sweep the documents again
  DocumentMap::iterator it = m_documents.begin();
  while(it != m_documents.end())
  {
    DocumentFile* docfile = it->second;
    docfile->SetSwiped(false);
    // Next document
    ++it;
  }
  // Remove broken links. Will recalculate
  m_broken.clear();
}

void    
ProjectFile::SwipeProject()
{
  // Block once after an import of a CHM file
  if(m_blockSwipeOnce)
  {
    m_blockSwipeOnce = false;
    return;
  }
  MainFrame* _tmain = (MainFrame*) theApp.m_pMainWnd;
  MainFrame::SetStatusText(_T("Swiping the project"));

  DocumentMap::iterator it = m_documents.begin();
  int count = m_documents.size();
  SweepDlg dlg(theApp.m_pMainWnd,true);
  dlg.DoTheInit();
  dlg.SetMax(count);

  int errors   = 0;
  int warnings = 0;
  m_sweepRebuildsIndex = false;

  while(it != m_documents.end())
  {
    DocumentFile* docfile = it->second;

    // Get document name
    CString fileName = m_baseDir + docfile->GetFilename();
    CString text = _T("Scanning: ");
    text += fileName;
    // Show in sweep dialog
    dlg.Increment();
    dlg.SetText(text);

    if(!docfile->GetPayload() && !docfile->GetSweeped())
    {
      m_sweeping = docfile;
      switch(TidyFile(docfile,fileName))
      {
        case 1: ++warnings; break;
        case 2: ++errors;   break;
        case 0: 
        default: break; /* AOK */
      }
      m_sweeping = NULL;
    }
    docfile->SetSwiped(true);
    // Next document
    ++it;
  }
  if(m_sweepRebuildsIndex)
  {
    theApp.RedisplayIndex();
    // Should reconsider HTML pages on compile for auto index
    if(!m_binaryIndex)
    {
      theApp.MessageBox(_T("This project was not set to do have a binary index for keyword-lookup.\n")
                        _T("However: topic index keywords were found in the project on at least one page.\n")
                        _T("The project is therefore reset to have a binary index")
                       ,_T("Warning")
                       ,MB_OK|MB_ICONWARNING|MB_TASKMODAL);
      m_binaryIndex = true;
      m_needSaving = true;
    }
    if(!m_autoIndex)
    {
      theApp.MessageBox(_T("This project was not set to reconsider auto indexing in a compile.\n")
                        _T("However: topic index keywords were found in the project on at least one page.\n")
                        _T("The project is therefore reset to do auto indexing in a help compile.\n")
                       ,_T("Warning")
                       ,MB_OK|MB_ICONWARNING|MB_TASKMODAL);
      m_autoIndex   = true;
      m_needSaving = true;
    }
  }
  // Display the results of the sweep in the project file
  // This must be done after reading of all HHP, HHC and HHK and 
  // the sweeping of all HTML files. (so we know all TOC and index dispositions)
  if(_tmain->m_wndProjectView)
  {
    _tmain->GetProjectView()->LoadProjectFile();
  }
  MainFrame::SetStatusText(_T(""));
  /*
  if(errors || warnings)
  {
    CString mess;
    mess.Format("Project sweep: %d errors and %d warnings",errors,warnings);
    theApp.MessageBox(mess,"Sweep",MB_OK|MB_ICONWARNING);
  }
  */
}

int
ProjectFile::TidyFile(DocumentFile* docfile,CString filename)
{
  ctmbstr cfgfil = NULL, errfil = NULL, htmlfil = NULL;
  TidyDoc tdoc = tidyCreate();
  int status = 0;
  FILE* errout = NULL;

  uint contentErrors = 0;
  uint contentWarnings = 0;
  uint accessWarnings = 0;

  errout = stderr;  /* initialize to stderr */
  status = 0;

  // Do identations
  tidyOptSetValue( tdoc, TidyDoctype,"loose");
  tidyOptSetInt( tdoc, TidyIndentContent, TidyAutoState );
  tidyOptSetInt( tdoc, TidyIndentSpaces,  4);
  // Write back in same file
  tidyOptSetBool( tdoc, TidyWriteBack, yes );
  // No errors in AuthorHTML, no error file
  tidyOptSetBool( tdoc, TidyQuiet, yes );
  // Wrap length
  tidyOptSetInt( tdoc, TidyWrapLen, 128 );
  // No extra generator
  tidyOptSetBool( tdoc, TidyMark, no);
  // Expect to read UTF8
  tidySetInCharEncoding(tdoc,"utf8");
  // Use UTF-8 with a BOM
  tidySetOutCharEncoding(tdoc,"utf8");
  tidyOptSetInt(tdoc,TidyOutputBOM,1);

  CStringA filenameA(filename);
  status = tidyParseFile(tdoc,filenameA);
  if ( status >= 0 )
  {
    status = tidyCleanAndRepair( tdoc );
  }
  if ( status >= 0 )
  {
    status = tidyRunDiagnostics( tdoc );
  }
  if(!docfile->MetaDataRead())
  {
    // Process the tdoc to docfile attributes only the first time
    // Later on we edit it ourselves, so DocumentFile is the place
    // were we edit it later on.
    GetDocumentTitle (docfile,tdoc);
    GetDocumentHeader(docfile,tdoc);
    docfile->DidReadMetaData(true);
  }
  // Get the payload from the body
  GetDocumentBody(docfile,tdoc);

  // Saving
  status = tidySaveFile(tdoc,filenameA);

  contentErrors   += tidyErrorCount( tdoc );
  contentWarnings += tidyWarningCount( tdoc );
  accessWarnings  += tidyAccessWarningCount( tdoc );

  /* called to free hash tables etc. */
  tidyRelease( tdoc );

  // TRACE("File: %s Status: %d.%d\n",filename.GetString(),contentErrors,contentWarnings);

  /* return status can be used by scripts */
  if ( contentErrors > 0 )
  {
    return 2;
  }
  if ( contentWarnings > 0 )
  {
    return 1;
  }
  /* 0 signifies all is ok */
  return 0;
}

void
ProjectFile::GetDocumentTitle(DocumentFile* docfile,TidyDoc& tdoc)
{
  TidyNode head = tidyGetHead(tdoc);
  if(head == 0)
  {
    // No head (yet) in the HTML file
    return;
  }
  TidyNode node = tidyGetChild(head);

  while(node)
  {
    if(tidyNodeIsTITLE(node))
    {
      TidyBuffer buf;
      tidyBufAlloc(&buf,1024);
      tidyNodeGetText(tdoc,node,&buf);
      if(buf.size)
      {
        CString title = CA2CT((char*)buf.bp);
        title.Remove(_T('\r'));
        title.Remove(_T('\n'));
        title.TrimLeft(_T("<title>"));
        int pos = title.Find(_T("</"));
        if(pos >= 0)
        {
          title = title.Left(pos);
        }
        // TRACE("TITLE in sweep: %s\n",title.GetString());
        docfile->SetTitle(title);
      }
      tidyBufFree(&buf);
      return;
    }
    // Get next node
    node = tidyGetNext(node);
  }
}

void
ProjectFile::GetDocumentHeader(DocumentFile* docfile,TidyDoc& tdoc)
{
  TidyNode head = tidyGetHead(tdoc);
  if(head == 0)
  {
    // No head (yet) in the HTML file
    return;
  }

  TidyNode node = tidyGetChild(head);

  while(node)
  {
    if(tidyNodeIsLINK(node) || tidyNodeIsSCRIPT(node))
    {
      TidyAttr attr = tidyAttrFirst(node);
      while(attr)
      {
        if(tidyAttrIsHREF(attr) || tidyAttrIsSRC(attr))
        {
          ctmbstr name = tidyAttrValue(attr);
          CString css(name);
          // CSS, Script or other payload link
          // TRACE("LINK/SCRIPT HEAD payload file: %s\n",css.GetString());
          AddDocumentFile(docfile->GetRelativeDirectory(),css);
        }
        attr = tidyAttrNext(attr);
      }
    }
    else if(tidyNodeIsMETA(node))
    {
      GetDocumentMeta(node,docfile);
    }
    // Get next node
    node = tidyGetNext(node);
  }
}

void
ProjectFile::GetDocumentBody(DocumentFile* docfile,TidyDoc& tdoc)
{
  TidyNode body = tidyGetBody(tdoc);
  if(body == 0)
  {
    // No body (yet) in the HTML file ?
    return;
  }
  TidyNode node = tidyGetChild(body);
  if(node)
  {
    GetDocumentPayload(node,docfile);
  }
}

void
ProjectFile::GetDocumentMeta(TidyNode node,DocumentFile* docfile)
{
  CString sContent;
  CString sName;

  // Find attributes in META Tag
  TidyAttr attr = tidyAttrFirst(node);
  while(attr) 
  {
    if(tidyAttrIsCONTENT(attr))
    {
      ctmbstr content = tidyAttrValue(attr);
      sContent = content;
    }
    if(tidyAttrIsNAME(attr) || tidyAttrIsHTTP_EQUIV(attr))
    {
      ctmbstr name = tidyAttrValue(attr);
      sName = name;
    }
    attr = tidyAttrNext(attr);
  }
  if(!sContent.IsEmpty() && !sName.IsEmpty())
  {
    // Now we have a content,name pair
    // TRACE("META in sweep: %s: %s\n",sName.GetString(),sContent.GetString());

         if(sName.CompareNoCase(_T("AuthorOriginalAuthor")) == 0) docfile->SetAuthor    (sContent);
    else if(sName.CompareNoCase(_T("AuthorDocComment"))     == 0) docfile->SetComment   (sContent);
    else if(sName.CompareNoCase(_T("AuthorHTMLTemplate"))   == 0) docfile->SetTemplate  (sContent);
    else if(sName.CompareNoCase(_T("X-UA-Compatible"))      == 0) docfile->SetCompatible(sContent);
    else if(sName.CompareNoCase(_T("AuthorStatus"))         == 0) docfile->SetStatus   (_ttoi(sContent));
    else if(sName.CompareNoCase(_T("AuthorPriority"))       == 0) docfile->SetPriority (_ttoi(sContent));
    else if(sName.CompareNoCase(_T("AuthorTimeSpent"))      == 0) docfile->SetTimeSpent(_ttoi(sContent));
    else if(sName.CompareNoCase(_T("AuthorToDo"))           == 0) docfile->SetToDo     (_ttoi(sContent));
    else if(sName.CompareNoCase(_T("AuthorWidth"))          == 0) docfile->SetWidth    (_ttoi(sContent));
    else if(sName.CompareNoCase(_T("AuthorHeight"))         == 0) docfile->SetHeight   (_ttoi(sContent));

    else if(sName.CompareNoCase(_T("MS-HKWD")) == 0) 
    {
      theApp.GetIndex()->AddKeywords(sContent,docfile);
      m_sweepRebuildsIndex = true;
      m_needSaving = true;
    }
  }
}

void
ProjectFile::GetDocumentPayload(TidyNode node,DocumentFile* docfile)
{
  while(node)
  {
    if(tidyNodeIsLINK(node)   || 
       tidyNodeIsSCRIPT(node) || 
       tidyNodeIsIMG(node)    ||
       tidyNodeIsA(node)      ||
       tidyNodeIsAREA(node)   ||   
       tidyNodeIsFRAME(node)  ||
       tidyNodeIsIFRAME(node)  )
    {
      TidyAttr attr = tidyAttrFirst(node);
      while(attr)
      {
        if(tidyAttrIsHREF(attr) || tidyAttrIsSRC(attr))
        {
          ctmbstr name = tidyAttrValue(attr);
          CString fileref(name);
          // Strip "javascript:BSSCPopup('file');"
          // Strip "javascript:ADHShowPopup('file');"
          fileref = Misc::StripPopup(fileref);
          if(!fileref.IsEmpty())
          {
            CString file;
            CString anchor;
            bool hasAnchor = Misc::SplitMidpageAnchor(fileref,file,anchor);

            // File link
            // TRACE("ANCHOR/LINK/SCRIPT/IMG/AREA/(I)FRAME payload file: %s\n",file.GetString());
            DocumentFile* other = FindDocumentFile(file);
            if(other == NULL && !file.IsEmpty())
            {
              if(AddDocumentFile(docfile->GetRelativeDirectory(),file))
              {
                theApp.ReSweepProject();
                other = FindDocumentFile(file);
              }
            }
            if(other)
            {
              docfile->AddLinkFrom(other,anchor);
              other->AddLinkToMe(docfile,anchor);
            }
          }
        }
        attr = tidyAttrNext(attr);
      }
    }
    // Recurse through the document
    TidyNode child = tidyGetChild(node);
    if(child)
    {
      GetDocumentPayload(child,docfile);
    }
    node = tidyGetNext(node);
  }
}

void
ProjectFile::FindRegular(CString& titleReg,CString& fileReg,DocumentMap& found)
{
  found.clear();

  RegExp title(titleReg);
  RegExp file(fileReg);

  DocumentMap::iterator it = m_documents.begin();
  while(it != m_documents.end())
  {
    DocumentFile* doc = it->second;
    CString docTitle = doc->GetTitle();
    CString docFile  = doc->GetFilename();

    if((!titleReg.IsEmpty() && title.Match(docTitle) >= 0) || 
       (! fileReg.IsEmpty() &&  file.Match(docFile)  >= 0) )
    {
      if(!doc->GetPayload())
      {
        found.insert(make_pair(docFile,doc));
      }
    }
    ++it;
  }
}

// Compile length is the number of files we think that the
// HHC compiler will emit in it's log on a successful compile
int     
ProjectFile::FindCompileLength()
{
  // All HTML documents, and payload files
  int len = m_documents.size();

  // Now add the numbers from the following types
  // Alias files
  // MAP files
  // Text popup files

  // HHC compiler adds 15 lines of header/footer text
  len += 15;
  return len;
}

//////////////////////////////////////////////////////////////////////////
//
// Renaming of files
// 
//////////////////////////////////////////////////////////////////////////

// Rename a file across a project

void
ProjectFile::RenameFile(CString& p_old_href,CString& p_new_href)
{
  // Rename in all documents on the disk
  DocumentMap::iterator it = m_documents.begin();
  while(it != m_documents.end())
  {
    DocumentFile* doc = it->second;
    CString filename = m_baseDir + doc->GetFilename();
    if(doc->GetPayload() == false)
    {
      // Only true .HTML or .HTM files can be redefined
      // Tidy cannot recognize images and scripts
      RenameInOneFile(filename,p_old_href,p_new_href);
    }
    // Next document
    ++it;
  }
  // Now do the documents themselves
  it = m_documents.begin();
  while(it != m_documents.end())
  {
    DocumentFile* doc = it->second;
    CString filename = doc->GetFilename();
    if(filename.CompareNoCase(p_old_href) == 0)
    {
      // Erase from mapping
      CString fileref(filename);
      fileref.MakeLower();
      DocumentMap::iterator iter = m_documents.find(fileref);
      if(iter != m_documents.end())
      {
        m_documents.erase(iter);
      }
      // Reset in the documents mapping
      fileref = p_new_href;
      fileref.MakeLower();
      m_documents.insert(std::make_pair(fileref,doc));
      // Set new filename in document
      doc->SetFilename(p_new_href);
      break;
    }
    // next document
    ++it;
  }
}

int
ProjectFile::RenameInOneFile(CString& p_filename,CString& p_old_href,CString& p_new_href)
{
  ctmbstr cfgfil = NULL, errfil = NULL, htmlfil = NULL;
  TidyDoc tdoc = tidyCreate();
  int     status = 0;
  FILE*   errout = NULL;

  uint contentErrors = 0;
  uint contentWarnings = 0;
  uint accessWarnings = 0;

  errout = stderr;  /* initialize to stderr */
  status = 0;

  // Do indentations
  tidyOptSetInt( tdoc, TidyIndentContent, TidyAutoState );
  tidyOptSetInt( tdoc, TidyIndentSpaces,  4);
  // Write back in same file
  tidyOptSetBool( tdoc, TidyWriteBack, yes );
  // No errors in AuthorHTML, no error file
  tidyOptSetBool( tdoc, TidyQuiet, yes );
  // Wrap length
  tidyOptSetInt( tdoc, TidyWrapLen, 128 );
  // No extra generator
  tidyOptSetBool( tdoc, TidyMark, no);

  CStringA filenameA(p_filename);
  status = tidyParseFile(tdoc,filenameA);
  if ( status >= 0 )
  {
    status = tidyCleanAndRepair(tdoc);
  }
  if ( status >= 0 )
  {
    status = tidyRunDiagnostics(tdoc);
  }
  RenameInHeader(tdoc,p_old_href,p_new_href);
  RenameInBody  (tdoc,p_old_href,p_new_href);

  status = tidySaveFile(tdoc,filenameA);

  contentErrors   += tidyErrorCount( tdoc );
  contentWarnings += tidyWarningCount( tdoc );
  accessWarnings  += tidyAccessWarningCount( tdoc );

  /* called to free hash tables etc. */
  tidyRelease(tdoc);

  // TRACE("File: %s Status: %d.%d\n",p_filename.GetString(),contentErrors,contentWarnings);

  /* return status can be used by scripts */
  if(contentErrors > 0)
  {
    return 2;
  }
  if(contentWarnings > 0)
  {
    return 1;
  }
  /* 0 signifies all is ok */
  return 0;
}

void
ProjectFile::RenameInHeader(TidyDoc tdoc,CString& p_old_href,CString& p_new_href)
{
  TidyNode head = tidyGetHead(tdoc);
  if(head == 0)
  {
    // No head (yet) in the HTML file
    return;
  }
  TidyNode node = tidyGetChild(head);

  while(node)
  {
    if(tidyNodeIsLINK(node) || tidyNodeIsSCRIPT(node))
    {
      CString name = tidyNodeGetName(node);
      TidyAttr attr = tidyAttrFirst(node);
      while(attr)
      {
        if(tidyAttrIsHREF(attr) || tidyAttrIsSRC(attr))
        {
          CString new_href;
          CString href = tidyAttrValue(attr);
          // TRACE("Rename in HEAD LINK/SCRIPT payload file: %s\n",href.GetString());

          if(Misc::ReplaceFilenameExtra(m_baseDir,p_old_href,p_new_href,href,new_href))
          {
            CStringA new_hrefA(new_href);
            tidyAttrSetValue(tdoc,attr,(char*) new_hrefA.GetString());
          }
        }
        attr = tidyAttrNext(attr);
      }
    }
    node = tidyGetNext(node);
  }
}

void
ProjectFile::RenameInBody(TidyDoc tdoc,CString& p_old_href,CString& p_new_href)
{
  TidyNode body = tidyGetBody(tdoc);
  if(body == 0)
  {
    // No body (yet) in the HTML file ?
    return;
  }
  TidyNode node = tidyGetChild(body);
  if(node)
  {
    RenameInElement(tdoc,node,p_old_href,p_new_href);
  }
}

void
ProjectFile::RenameInElement(TidyDoc tdoc,TidyNode node,CString& p_old_href,CString& p_new_href)
{
  while(node)
  {
    if(tidyNodeIsLINK(node)   || 
       tidyNodeIsSCRIPT(node) || 
       tidyNodeIsIMG(node)    ||
       tidyNodeIsA(node)      ||
       tidyNodeIsAREA(node)   ||   
       tidyNodeIsFRAME(node)  ||
       tidyNodeIsIFRAME(node)  )
    {
      CString name = tidyNodeGetName(node);
      TidyAttr attr = tidyAttrFirst(node);
      while(attr)
      {
        if(tidyAttrIsHREF(attr) || tidyAttrIsSRC(attr))
        {
          CString new_href;
          CString href = tidyAttrValue(attr);
          // TRACE("Rename in HEAD LINK/SCRIPT payload file: %s\n",href.GetString());

          if(Misc::ReplaceFilenameExtra(m_baseDir,p_old_href,p_new_href,href,new_href))
          {
            CStringA new_hrefA(new_href);
            tidyAttrSetValue(tdoc,attr,(char*)new_hrefA.GetString());
          }
        }
       attr = tidyAttrNext(attr);
      }
    }
    // Recurse through the document
    TidyNode child = tidyGetChild(node);
    if(child)
    {
      RenameInElement(tdoc,child,p_old_href,p_new_href);
    }
    node = tidyGetNext(node);
  }
}


bool
ProjectFile::CreateNewDefaultProject(CString p_name)
{
  p_name = Misc::BasenamePart(p_name);

  m_title               = p_name;
  m_compiledName        = p_name + _T(".chm");
  m_contentsFile        = p_name + _T(".hhc");
  m_indexFile           = p_name + _T(".hhk");
  m_defaultTopic        = _T("Intro.html");
  m_defaultWindow       = _T("Main");
  m_defaultFont         = _T("Verdana,9,0");
  m_errorLogFile        = _T("CompileErrors.log");
  m_customTab           = _T("");
  m_language            = _T("0x409 English (United States)");
  m_stopFile            = _T("StopWords.lng");
  m_compatibility       = _T("1.1 or later");
  m_displayProgress     = true;
  m_displayCompileNotes = true;
  m_fullTextSearch      = true;
  m_binaryIndex         = true;
  m_autoIndex           = true;
  m_enhancedDecompile   = true;
  m_binaryTOC           = true;
  m_flat                = false;
  
  // Add a default window
  AddWindow(m_defaultWindow);
  WindowDefinition* window =  FindWindowDefinition(m_defaultWindow);
  if(window)
  {
    window->CreateNewDefaultWindow(p_name,m_defaultTopic);
  }

  // Create the first topic file, which is the default topic file
  CString newfile = m_baseDir + m_defaultTopic;
  CString newtitle = _T("Welcome to ") + p_name;
  if(CHTMLEdDoc::CreateNewDocumentFile(newfile,newtitle))
  {
    AddDocumentFile(_T(""),newfile,true);
  }

  // Save everyting!
  SetProjectFilename(p_name,true);

  // Default TOC entry, which is the default topic file
  TOCEntry* entry = new TOCEntry(m_title,m_defaultTopic,1,_T(""),_T(""),_T(""));

  // Write the TOC for the first time
  TOC* toc = theApp.GetTOC();
  toc->AddEntry(nullptr,entry);
  toc->SetNeedSaving();
  toc->WriteTOCFile();

  // Write the Index for the first time
  IndexFile* index = theApp.GetIndex(); 
  index->SetNeedSaving(true);
  index->WriteIndexFile();

  // Show the new first file
  theApp.OpenTypedDocumentFile(newfile);
  return true;
}
