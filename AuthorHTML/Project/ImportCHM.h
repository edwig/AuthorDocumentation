//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    ImportCHM.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Import a CHM file as a project directory
// by way of the standard "hhc.exe -decompile" command
//
#pragma once

class ImportCHM
{
public:
  ImportCHM();
 ~ImportCHM();
  bool Import(CString& p_directory,CString& p_filename);

private:
  bool    WriteDefaultProjectfile();
  void    GetProjectFiles(CString& p_directory,CString& p_relative,FILE* fout);
  CString FindFirstWithExtension(CString p_extension,CString& p_directory,CString& p_relative);

  CString   m_directory;
  CString   m_filename;

  CString   m_firstTOCfile;
  CString   m_firstKEYfile;
  CString   m_firstHHPfile;

  CString   m_firstHTMfile;
};