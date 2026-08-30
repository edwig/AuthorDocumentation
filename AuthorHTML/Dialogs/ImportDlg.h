//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    ImportDlg.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for importing a CHM file into a project
//              by decompiling a CHM file and creating a project with the contents
//
#pragma once
#include "resource.h"

// ImportDlg dialog

class ImportDlg : public CDialog
{
	DECLARE_DYNAMIC(ImportDlg)

public:
	ImportDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~ImportDlg();
  BOOL    OnInitDialog();
  CString GetHHPFilename();
  bool    GetDoSweep();

// Dialog Data
	enum { IDD = IDD_IMPORT };

protected:
	void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  void CheckInput();

  CString       m_filename;
  CString       m_directory;
  bool          m_doSweep;

  CMFCButton    m_buttonSearchCHM;
  CMFCButton    m_buttonSearchProject;
  CButton       m_buttonDoSweep;
  CMFCButton    m_buttonHelp;
  CMFCButton    m_buttonOK;
  CMFCButton    m_buttonCancel;

	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnEnChangeChm();
  afx_msg void OnBnClickedSearchChm();
  afx_msg void OnEnChangeProject();
  afx_msg void OnBnClickedSearchPro();
  afx_msg void OnBnClickedHelp();
  afx_msg void OnBnClickedOk();
  afx_msg void OnBnClickedCancel();
  afx_msg void OnBnClickedDosweep();
};

inline bool
ImportDlg::GetDoSweep()
{
  return m_doSweep;
}
