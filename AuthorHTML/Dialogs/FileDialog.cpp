//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    FileDialog.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog selecting a file from the OS.
//
#include "StdAfx.h"
#include "FileDialog.h"
#include "AuthorHTML.h"
#include <dlgs.h>

#pragma warning (disable:4312)

DocFileDialog::DocFileDialog(bool    p_open        // true = open, false = SaveAs
                            ,CString p_title       // Title of the dialog
                            ,CString p_defext      // Default extension
                            ,CString p_filename    // Default first file
                            ,int     p_flags       // Default flags
                            ,CString p_filter      // Filter for extensions
                            ,CString p_direct)     
              :m_open(p_open)
{
  if(p_filter.IsEmpty())
  {
    p_filter = "Documentation files (*.htm, *.html)|*.htm;*.html|";
  }
  // Register original CWD (Current Working Directory)
  GetCurrentDirectory(MAX_PATH, m_original);
  if(!p_direct.IsEmpty())
  {
    // Change to starting directory
    SetCurrentDirectory(p_direct.GetString());
  }
  strncpy_s(m_filter,  1024,   p_filter,  1024);
  strncpy_s(m_filename,MAX_PATH,p_filename,MAX_PATH);
  strncpy_s(m_defext,  100,    p_defext,  100);
  strncpy_s(m_title,   100,    p_title,   100);
  FilterString(m_filter);

  // Vul de filename structuur
  p_flags |= OFN_ENABLESIZING | OFN_LONGNAMES | OFN_NOCHANGEDIR |  OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
  p_flags &= ~(OFN_NODEREFERENCELINKS | OFN_NOLONGNAMES | OFN_NOTESTFILECREATE);

  m_ofn.lStructSize       = sizeof(OPENFILENAME);
  m_ofn.hwndOwner         = AfxGetApp()->GetMainWnd()->GetSafeHwnd();
  m_ofn.hInstance         = (HINSTANCE) GetWindowLong(m_ofn.hwndOwner,GWLP_HINSTANCE);
  m_ofn.lpstrFile         = (LPSTR) m_filename;
  m_ofn.lpstrDefExt       = (LPSTR) m_defext;
  m_ofn.lpstrTitle        = (LPSTR) m_title;
  m_ofn.lpstrFilter       = (LPSTR) m_filter;
  m_ofn.Flags             = p_flags;
  m_ofn.nFilterIndex      = 1;    // Use lpstrFilter
  m_ofn.nMaxFile          = MAX_PATH;
  m_ofn.lpstrCustomFilter = NULL; //(LPSTR) buf_filter;
  m_ofn.nMaxCustFilter    = 0;
  m_ofn.lpstrFileTitle    = NULL;
  m_ofn.nMaxFileTitle     = 0;
  m_ofn.lpstrInitialDir   = NULL;
  m_ofn.nFileOffset       = 0;
  m_ofn.lCustData         = NULL;
  m_ofn.lpfnHook          = NULL;
  m_ofn.lpTemplateName    = NULL;
}

DocFileDialog::~DocFileDialog()
{
  // Go back to the original directory
  SetCurrentDirectory(m_original);
}

#pragma warning(disable:4702)
int 
DocFileDialog::DoModal()
{
  int res = IDCANCEL;
  try
  {
    if(m_open)
    {
      res = GetOpenFileName(&m_ofn);
    }
    else
    {
      res = GetSaveFileName(&m_ofn);
    }
  }
  catch(...)
  {
    theApp.MessageBox("Cannot create a file dialog","ERROR",MB_OK|MB_ICONHAND);
  }
  return res;
}

CString 
DocFileDialog::GetChosenFile()
{
  return (CString) m_ofn.lpstrFile;
}

void
DocFileDialog::FilterString(char *filter)
{
  char *pnt = filter;
  while(*pnt)
  {
    if(*pnt == '|')
    {
      *pnt = 0;
    }
    ++pnt;
  }
  *pnt++ = 0;
  *pnt++ = 0;
}
