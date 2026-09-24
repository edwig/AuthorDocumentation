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
#include "ImportCHM.h"
#include "Misc.h"
#include "CheckWorkshop.h"

ImportCHM::ImportCHM()
{
}

ImportCHM::~ImportCHM()
{
}

bool
ImportCHM::Import(CString &p_directory,CString &p_filename)
{
  // Return value
  bool retval = false;
  // Register
  m_directory = p_directory;
  m_filename = p_filename;

  // Check for values
  if(m_filename.IsEmpty() || m_directory.IsEmpty())
  {
    return false;
  }
  // Check directory name
  if(m_directory.Right(1) != _T('\\'))
  {
    m_directory += _T("\\");
  }

  // Copy the file to the directory
  CString file = Misc::FilenamePart(m_filename);
  CString newFile = m_directory + file;

  if(file.Find(_T(' ')) >= 0)
  {
    theApp.Panic(_T("CHM files cannot have a space in the name in order for the decompilation to work!"));
    return false;
  }

  // Do the copy
  if(CopyFile(m_filename,newFile,TRUE) == FALSE)
  {
    CString message;
    message.Format(_T("Cannot copy the file [%s] to the directory [%s]"),m_filename.GetString(),m_directory.GetString());
    theApp.Panic(_T("Decompilation error:\n") + message);
    return false;
  }

  // Register original CWD (Current Working Directory)
  TCHAR original[MAX_PATH];
  GetCurrentDirectory(MAX_PATH,original);
  if(!m_directory.IsEmpty())
  {
    // Change to starting directory
    SetCurrentDirectory(m_directory);
  }

  // Build arguments
  CString program;
  CString arguments = _T("-decompile . ") + file;

  // Find the decompiler
  program.GetEnvironmentVariable(_T("windir"));
#if defined _M_IX86
  program += _T("\\system32\\hh.exe");
#else
  program += _T("\\syswow64\\hh.exe");
#endif

  // Decompile the CHM file
  short ret = (short) Misc::StartProgramma(program,arguments,false,true,false);
  if(ret < 0)
  {
    CString message = _T("Cannot start the decompile program in the default Windows directory (hhc.exe)");
    theApp.MessageBox(message,_T("Decompile error"),MB_OK|MB_ICONERROR);
    retval = false;
  }
  else
  {
    retval = true;

    CString relative;
    m_firstTOCfile = FindFirstWithExtension(_T(".hhc"),m_directory,relative);
    m_firstKEYfile = FindFirstWithExtension(_T(".hhk"),m_directory,relative);
    m_firstHHPfile = FindFirstWithExtension(_T(".hhp"),m_directory,relative);

    m_firstHTMfile = FindFirstWithExtension(_T(".htm"), m_directory,relative);
    if(m_firstHTMfile.IsEmpty())
    {
      m_firstHTMfile = FindFirstWithExtension(_T(".html"),m_directory,relative);
    }

    // Create a default project file
    if(m_firstHHPfile.IsEmpty())
    {
      WriteDefaultProjectfile();
    }
  }
  // Restore the original CWD
  SetCurrentDirectory(original);
  return retval;
}

bool
ImportCHM::WriteDefaultProjectfile()
{
  CString projectName = Misc::BasenamePart(m_filename);
  CString projectFile = m_directory + projectName + _T(".hhp");

  FILE* fout = _tfopen(projectFile,_T("w"));
  if(fout == NULL)
  {
    CString message;
    message.Format(_T("Cannot open a default project file: %s"),projectFile.GetString());
    theApp.MessageBox(message,_T("File error"),MB_OK|MB_ICONERROR);
    return false;
  }
  _ftprintf(fout,_T("[OPTIONS]\n"));
  _ftprintf(fout,_T("Compiled file=%s.chm\n"),(LPCTSTR)projectName);
  _ftprintf(fout,_T("Compatibility=\n"));
  _ftprintf(fout,_T("Title=%s\n"),            (LPCTSTR)projectName);
  _ftprintf(fout,_T("Contents File=%s\n"),    (LPCTSTR)m_firstTOCfile);
  _ftprintf(fout,_T("Index File=%s\n"),       (LPCTSTR)m_firstKEYfile);
  _ftprintf(fout,_T("Default Topic=%s\n"),    (LPCTSTR)m_firstHTMfile);
  _ftprintf(fout,_T("Default Window=main\n"));
  _ftprintf(fout,_T("Default Font=\n"));
  _ftprintf(fout,_T("Error log file=\n"));
  _ftprintf(fout,_T("Custom tab=\n"));
  _ftprintf(fout,_T("Language=\n"));
  _ftprintf(fout,_T("Full text search stop list file=\n"));
  _ftprintf(fout,_T("Display compile progress=yes\n"));
  _ftprintf(fout,_T("Display compile notes=yes\n"));
  _ftprintf(fout,_T("Full-text search=no\n"));
  _ftprintf(fout,_T("Binary index=no\n"));
  _ftprintf(fout,_T("Binary TOC=no\n"));
  _ftprintf(fout,_T("Auto index=no\n"));
  _ftprintf(fout,_T("Enhanced decompilation=yes\n"));
  _ftprintf(fout,_T("Flat=no\n"));
  _ftprintf(fout,_T("\n"));
  _ftprintf(fout,_T("[WINDOWS]\n"));
  _ftprintf(fout,_T("main=\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"\",\"\",\"\",\"\",0x21420,0,0x4204e,[0,0,1024,768],0xb0000,0x0,,0,0,0\n")
              ,projectName.GetString()
              ,m_firstTOCfile.GetString()
              ,m_firstKEYfile.GetString()
              ,m_firstHTMfile.GetString()
              ,m_firstHTMfile.GetString());
  _ftprintf(fout,_T("\n"));
  _ftprintf(fout,_T("[FILES]\n"));

  CString relative;
  GetProjectFiles(m_directory,relative,fout);

  if(fclose(fout) == _TEOF)
  {
    CString message;
    message.Format(_T("Cannot write the default project file: %s"),projectFile.GetString());
    theApp.MessageBox(message,_T("File error"),MB_OK|MB_ICONERROR);
    return false;
  }
  return true;
}

void
ImportCHM::GetProjectFiles(CString& p_directory,CString& p_relative,FILE* fout)
{
  WIN32_FIND_DATA FindData;
  HANDLE SearchHandle = NULL;

  // Make it a directory name
  if(!p_relative.IsEmpty() && p_relative.Right(1) != _T('\\'))
  {
    p_relative += _T("\\");
  }
  // Make search pattern
  CString pattern = p_directory + p_relative  + _T("*.*");

  if ((SearchHandle = FindFirstFile((LPCTSTR)pattern, &FindData)) != INVALID_HANDLE_VALUE)
  {
    do 
    {
      if(FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
      {
        if(_tcscmp(FindData.cFileName,_T("..")) &&
           _tcscmp(FindData.cFileName,_T(".")))
        {
          // Recurse into directory for files
          CString relative = p_relative + FindData.cFileName;
          GetProjectFiles(p_directory,relative,fout);
        }
      }
      else
      {
        // Print out to the project file
        CString file(FindData.cFileName);
        CString extens = Misc::ExtensionPart(file);
        if(extens.Compare(_T(".chm")))
        {
          _ftprintf(fout,_T("%s%s\n"),(LPCTSTR)p_relative,(LPCTSTR)FindData.cFileName);
        }
      }
    } 
    while (FindNextFile(SearchHandle, &FindData));
    FindClose(SearchHandle);
  }
}

CString
ImportCHM::FindFirstWithExtension(CString p_extension,CString& p_directory,CString& p_relative)
{
  WIN32_FIND_DATA FindData;
  HANDLE SearchHandle = NULL;

  // Make it a directory name
  if(!p_relative.IsEmpty() && p_relative.Right(1) != _T('\\'))
  {
    p_relative += _T("\\");
  }
  // Make search pattern
  CString pattern = p_directory + p_relative  + _T("*") + p_extension;

  if ((SearchHandle = FindFirstFile((LPCTSTR)pattern, &FindData)) != INVALID_HANDLE_VALUE)
  {
    do 
    {
      if(FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
      {
        if(_tcscmp(FindData.cFileName,_T("..")) &&
           _tcscmp(FindData.cFileName,_T(".")))
        {
          // Recurse into directory for files
          CString relative = p_relative + FindData.cFileName;
          CString result = FindFirstWithExtension(p_extension,p_directory,relative);
          if(!result.IsEmpty())
          {
            return result;
          }
        }
      }
      else
      {
        // Print out to the project file
        if(Misc::ExtensionPart(FindData.cFileName).CompareNoCase(p_extension) == 0)
        {
          return (p_relative + FindData.cFileName);
        }
      }
    } 
    while (FindNextFile(SearchHandle, &FindData));
    FindClose(SearchHandle);
  }
  return CString(_T(""));
}
