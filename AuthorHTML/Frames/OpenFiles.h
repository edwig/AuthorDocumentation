//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    OpenFiles.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Shows the currently opened files in the main window frame
//
#pragma once

class OpenFiles : public CDockablePane
{
public:
  OpenFiles ();
	virtual ~OpenFiles ();

  CListCtrl&  GetOpenFilesListCtrl () { return m_openFilesList; }

  void   OpenFiles_Append         (LVITEM&);
  void   OpenFiles_UpdateByParam  (LPARAM param, LVITEM&);
  void   OpenFiles_RemoveByParam  (LPARAM param);
  void   OpenFiles_ActivateByParam(LPARAM param);
  LPARAM OpenFiles_GetCurSelParam ();
  LPARAM OpenFiles_FindParamByNumber(int p_number);

protected:
    int OpenFiles_FindByParam (LPARAM param);

protected:
  void  ActivateOpenFile ();
  void  OpenFilesStarted();

  CListCtrl    m_openFilesList;
  CImageList*  m_imgList;
protected:
	DECLARE_MESSAGE_MAP()

public:
  afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);

  afx_msg void OnDestroy();
  afx_msg void OnTimer(UINT nIDEvent);
  afx_msg void OnSize (UINT nType, int cx, int cy);
  afx_msg void OnOpenFiles_Click      (NMHDR* pNMHDR, LRESULT* pResult);
  afx_msg void OnOpenFiles_KeyDown    (NMHDR* pNMHDR, LRESULT* pResult);
  afx_msg void OnOpenFiles_RClick     (NMHDR* pNMHDR, LRESULT* pResult);
};

