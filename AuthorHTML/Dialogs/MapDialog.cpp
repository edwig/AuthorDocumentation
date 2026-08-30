//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    MapDialog.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for selecting a folder in the file system
//
#include "StdAfx.h"
#include "AuthorHTML.h"
#include "MapDialog.h"
#include <direct.h>
#include <SHLOBJ.H>

#ifndef BIF_NEWDIALOGSTYLE
#define BIF_NEWDIALOGSTYLE 0x0040
#endif

LPITEMIDLIST PathToPidl(CString const& path)
{
  LPITEMIDLIST  pidl = NULL;
  LPSHELLFOLDER pDesktopFolder;
  OLECHAR       olePath[MAX_PATH];
  ULONG         chEaten;
  ULONG         dwAttributes;
  HRESULT       hr;

  if (SUCCEEDED(SHGetDesktopFolder(&pDesktopFolder)))
  {
    MultiByteToWideChar(CP_ACP
                       ,MB_PRECOMPOSED
                       ,path
                       ,-1
                       ,olePath
                       ,MAX_PATH);

    hr = pDesktopFolder->ParseDisplayName(NULL
                                         ,NULL
                                         ,olePath
                                         ,&chEaten
                                         ,&pidl
                                         ,&dwAttributes);
    if (FAILED(hr))
    {
      return 0;
    }
    pDesktopFolder->Release();
  }
  return pidl;
}


//=============================================================================

CString PidlToPath(LPITEMIDLIST pidl, bool deletePidl = false)
{
  //  Convert the pidl to a path
  char szPath[MAX_PATH] = "";
  BOOL cvtResult = SHGetPathFromIDList(pidl, szPath);
  if(!cvtResult) 
  {
    //throw BasisExceptie(152,"Kan geen PIDL vertalen naar padnaam");
  }
  //  Store in string
  CString result = szPath;

  //  Free the result pidl
  if(deletePidl)
  {
    CoTaskMemFree(pidl);
  }
  //  Done
  return result;
}

//=============================================================================

MapDialog::MapDialog()
{
  // Oorspronkelijke CWD (Current Working Directory) registreren
  m_originalMap[0] = 0;
  _getcwd(m_originalMap,MAX_PATH);
}

//=============================================================================

MapDialog::~MapDialog()
{
  // Ga terug naar de oorspronkelijke directory
  _chdir((LPCSTR) m_originalMap);
}

//=============================================================================

bool MapDialog::Browse(HWND            hwndParent, 
                        CString const&  title, 
                        CString const&  initdir, 
                        CString const&  rootdir, 
                        bool            showFiles,
                        bool            showStatus)
{
  hwndParent = AfxGetApp()->GetMainWnd()->GetSafeHwnd();
  //  Store initial settings
  m_root = rootdir;
  m_init = initdir;

  //  Init the struct
  BROWSEINFO bi;
  memset(&bi, 0, sizeof(bi));

  //  Dialog parent
  bi.hwndOwner = hwndParent;

  //  Allow root to be set
  if(rootdir != "") 
  {
    bi.pidlRoot = PathToPidl(m_root);
  }

  //  We want to get callbacks
  bi.lpfn = CallbackProcS;
  bi.lParam = (LPARAM)this;

  //  Buffer in which the display name is returned
  char szDisplayName[MAX_PATH];
  bi.pszDisplayName = szDisplayName;

  //  Title
  bi.lpszTitle = title;

  //  Misc options
  if(showFiles)   bi.ulFlags |= BIF_BROWSEINCLUDEFILES;
  if(showStatus)  bi.ulFlags |= BIF_STATUSTEXT;

  bi.ulFlags |= BIF_EDITBOX | BIF_VALIDATE | BIF_NEWDIALOGSTYLE;

  //  Show the dialog
  LPITEMIDLIST pidlResult = SHBrowseForFolder(&bi);

  //  Free the root path pidl
  if(bi.pidlRoot) CoTaskMemFree((LPITEMIDLIST)bi.pidlRoot);
   
  //  Check result
  if(pidlResult == 0) return false;

  //  Store the resulting info
  m_disp = szDisplayName;
  m_path = PidlToPath(pidlResult, true);

  //  Done
  return true;
}

//=============================================================================
void MapDialog::EnableOk(bool bEnable)
{
  //  Should only be called when called from within OnSelChange
  if(m_hwnd == 0 || IsWindow(m_hwnd) == false)
  {
    ::MessageBox(NULL,"Call from invalid context","Error",MB_OK|MB_ICONERROR);
  }
  //  Set the ok button state
  ::SendMessage(m_hwnd, BFFM_ENABLEOK, 0, bEnable);
}

//=============================================================================
void MapDialog::SetSelection(CString const& path)
{
  //  Should only be called when called from within OnSelChange
  if(m_hwnd == 0 || IsWindow(m_hwnd) == false)
  {
    ::MessageBox(NULL,"Call from invalid context","Error",MB_OK|MB_ICONERROR);
  }
  //  Set the current path in the tree
  ::SendMessage(m_hwnd, BFFM_SETSELECTION, TRUE, (LPARAM)(char const*)path);  
}

//=============================================================================
void MapDialog::SetStatusText(CString const& text)
{
  //  Should only be called when called from within OnSelChange
  if(m_hwnd == 0 || IsWindow(m_hwnd) == false)
  {
    ::MessageBox(NULL,"Call from invalid context","Error",MB_OK|MB_ICONERROR);
  }
  //  Set the current path in the tree
  ::SendMessage(m_hwnd, BFFM_SETSTATUSTEXT, 0, (LPARAM)(char const*)text);  
}

//=============================================================================

void MapDialog::OnInitialized()
{
  //  Only meant for derived classes
}

//=============================================================================
void MapDialog::OnSelChange(CString const&)
{
  //  Only meant for derived classes
}

//=============================================================================
int MapDialog::CallbackProc(    
    HWND hwnd, 
    UINT uMsg, 
    LPARAM lParam)
{
  try
  {
    m_hwnd = hwnd;
    switch(uMsg)
    {
    case BFFM_INITIALIZED:
      if(m_init != "") SetSelection(m_init);
      OnInitialized();
      break;
    case BFFM_SELCHANGED:
      OnSelChange(PidlToPath((LPITEMIDLIST)lParam));
      break;
    case BFFM_VALIDATEFAILED:
      ::MessageBox(NULL,"Pathname is incorrect, give the correct path.","Error",MB_OK|MB_ICONERROR);
      break;
    }
    m_hwnd = 0;
  }
  catch(...)
  {
    m_hwnd = 0;
  }
  return 0;
}


//=============================================================================

int CALLBACK MapDialog::CallbackProcS(
    HWND hwnd, 
    UINT uMsg, 
    LPARAM lParam, 
    LPARAM lpData
    )
{
  return ((MapDialog*)lpData)->CallbackProc(hwnd, uMsg, lParam);
}


//=============================================================================
