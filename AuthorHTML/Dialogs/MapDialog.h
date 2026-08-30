//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    MapDialog.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for selecting a folder in the file system
//
#pragma once

class MapDialog
{
public:

   MapDialog();
  ~MapDialog();

  bool Browse(HWND            hwndParent, 
              CString const&  title, 
              CString const&  initdir    = "",
              CString const&  rootdir    = "",
              bool            showFiles  = false,
              bool            showStatus = false);

  CString const& GetPath() const { return m_path; }
protected:
  //  These functions can only be called from
  //  within the OnSelChange function, so it
  //  is reasonable to keep them as protected.
  void EnableOk(bool bEnable);
  void SetSelection(CString const& path);
  void SetStatusText(CString const& text);

private:

  virtual void OnInitialized();
  virtual void OnSelChange(CString const& path);
          int  CallbackProc(HWND hwnd,UINT uMsg,LPARAM lParam);
  static  int  CALLBACK CallbackProcS(HWND hwnd,UINT uMsg,LPARAM lParam,LPARAM lpData);

  HWND      m_hwnd;
  char      m_originalMap[MAX_PATH+1];
  CString   m_disp;
  CString   m_path;
  CString   m_root;
  CString   m_init;
};
