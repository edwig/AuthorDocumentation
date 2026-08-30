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
  if(m_directory.Right(1) != '\\')
  {
    m_directory += "\\";
  }
  // Register original CWD (Current Working Directory)
  char original[MAX_PATH];
  GetCurrentDirectory(MAX_PATH,original);
  if(!m_directory.IsEmpty())
  {
    // Change to starting directory
    SetCurrentDirectory(m_directory);
  }
  // Copy the file to the directory
  CString file = Misc::FilenamePart(m_filename);
  CString newFile = m_directory + file;
  if(CopyFile(m_filename,newFile,TRUE) == FALSE)
  {
    CString message;
    message.Format("Cannot copy the file [%s] to the directory [%s]",m_filename.GetString(),m_directory.GetString());
    theApp.MessageBoxA(message,"Decompile error",MB_OK|MB_ICONERROR);
    return false;
  }
  // Build arguments
  char buffer[MAX_PATH];
  GetWindowsDirectory(buffer,MAX_PATH);
  CString program;
  program.Format("%s\\hh.exe",buffer);
  CString arguments = "-decompile . " + file;

  // Decompile the CHM file
  int ret = Misc::StartProgramma(program,arguments,false,true,false);
  if(ret < 0)
  {
    CString message = "Cannot start the decompile program in the default Windows directory (hh.exe)";
    theApp.MessageBox(message,"Decompile error",MB_OK|MB_ICONERROR);
    return false;
  }
  else
  {
    retval = true;
  }
  CString relative;
  m_firstTOCfile = FindFirstWithExtension(".hhc",m_directory,relative);
  m_firstKEYfile = FindFirstWithExtension(".hhk",m_directory,relative);
  m_firstHHPfile = FindFirstWithExtension(".hhp",m_directory,relative);

  // Create a default project file
  if(m_firstHHPfile.IsEmpty())
  {
    WriteDefaultProjectfile();
  }
  // Restore the original CWD
  SetCurrentDirectory(original);
  return retval;
}

bool
ImportCHM::WriteDefaultProjectfile()
{
  CString projectName = Misc::BasenamePart(m_filename);
  CString projectFile = m_directory + projectName + ".hhp";

  FILE* fout = fopen(projectFile,"w");
  if(fout == NULL)
  {
    CString message;
    message.Format("Cannot open a default project file: %s",projectFile.GetString());
    theApp.MessageBox(message,"File error",MB_OK|MB_ICONERROR);
    return false;
  }
  fprintf(fout,"[OPTIONS]\n");
  fprintf(fout,"Compiled file=%s\n",  (LPCTSTR)projectName);
  fprintf(fout,"Compatibility=\n");
  fprintf(fout,"Title=%s\n",          (LPCTSTR)projectName);
  fprintf(fout,"Contents File=%s\n",  (LPCTSTR)m_firstTOCfile);
  fprintf(fout,"Index File=%s\n",     (LPCTSTR)m_firstKEYfile);
  fprintf(fout,"Default Topic=\n");
  fprintf(fout,"Default Window=main\n");
  fprintf(fout,"Default Font=\n");
  fprintf(fout,"Error log file=\n");
  fprintf(fout,"Custom tab=\n");
  fprintf(fout,"Language=\n");
  fprintf(fout,"Full text search stop list file=\n");
  fprintf(fout,"Display compile progress=yes\n");
  fprintf(fout,"Display compile notes=yes\n");
  fprintf(fout,"Full-text search=no\n");
  fprintf(fout,"Binary index=no\n");
  fprintf(fout,"Binary TOC=no\n");
  fprintf(fout,"Auto index=no\n");
  fprintf(fout,"Enhanced decompilation=yes\n");
  fprintf(fout,"Flat=no\n");
  fprintf(fout,"\n");
  fprintf(fout,"[WINDOWS]\n");
  fprintf(fout,"main="","","","","","","","","",0x420,0,0x0,[0,0,0,0],0x0,0x0,,0,0,0\n");
  fprintf(fout,"\n");
  fprintf(fout,"[FILES]\n");

  CString relative;
  GetProjectFiles(m_directory,relative,fout);

  if(fclose(fout) == EOF)
  {
    CString message;
    message.Format("Cannot write the default project file: %s",projectFile.GetString());
    theApp.MessageBox(message,"File error",MB_OK|MB_ICONERROR);
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
  if(!p_relative.IsEmpty() && p_relative.Right(1) != '\\')
  {
    p_relative += "\\";
  }
  // Make search pattern
  CString pattern = p_directory + p_relative  + "*.*";

  if ((SearchHandle = FindFirstFile((LPCSTR)pattern, &FindData)) != INVALID_HANDLE_VALUE)
  {
    do 
    {
      if(FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
      {
        if(strcmp(FindData.cFileName,"..") &&
           strcmp(FindData.cFileName,"."))
        {
          // Recurse into directory for files
          CString relative = p_relative + FindData.cFileName;
          GetProjectFiles(p_directory,relative,fout);
        }
      }
      else
      {
        // Print out to the project file
        fprintf(fout,"%s%s\n",(LPCTSTR)p_relative,(LPCTSTR)FindData.cFileName);
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
  if(!p_relative.IsEmpty() && p_relative.Right(1) != '\\')
  {
    p_relative += "\\";
  }
  // Make search pattern
  CString pattern = p_directory + p_relative  + "*" + p_extension;

  if ((SearchHandle = FindFirstFile((LPCSTR)pattern, &FindData)) != INVALID_HANDLE_VALUE)
  {
    do 
    {
      if(FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
      {
        if(strcmp(FindData.cFileName,"..") &&
           strcmp(FindData.cFileName,"."))
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
  return CString("");
}

