//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    NewFileDlg.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for creating a new documentation file
//              Probably from a template or from scratch
//
#pragma once
#include "resource.h"

// NewFileDlg dialog

class NewFileDlg : public CDialog
{
	DECLARE_DYNAMIC(NewFileDlg)

public:
	NewFileDlg(CWnd* pParent,CString base);
	virtual ~NewFileDlg();
  virtual BOOL OnInitDialog();

  CString GetTitle();
  CString GetFilename();
  CString GetTemplate();

// Dialog Data
	enum { IDD = IDD_NEWFILE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
  DECLARE_EVENTSINK_MAP()

  void  FillPage();
  bool  UpdateProperties();
  void  CreateNewFile();
  void  CopyTemplate();
  void  OnDocumentComplete(LPDISPATCH pDisp, LPVARIANT pURL);

  CString m_base;
  CString m_title;
  CString m_filename;
  CString m_template;
  bool    m_synchronize;  // Sync title and filename initially
  bool    m_error;
  // Controls
  CButton m_buttonFSearch;
  CButton m_buttonFGo;
  CButton m_buttonTSearch;
  CButton m_buttonTGo;
  CButton m_buttonOK;
  CButton m_buttonCancel;
  CButton m_buttonHelp;
  CComQIPtr<IWebBrowser2> m_spBrowser;

public:
  afx_msg void OnEnChangeTitle();
  afx_msg void OnEnChangeFilename();
  afx_msg void OnEnChangeTemplate();
  afx_msg void OnBnClickedButtonFsearch();
  afx_msg void OnBnClickedBtnfgo();
  afx_msg void OnBnClickedButtonTsearch();
  afx_msg void OnBnClickedBtntgo();
  afx_msg void OnBnClickedOk();
  afx_msg void OnBnClickedCancel();
  afx_msg void OnBnClickedHelpNewfile();
};

inline CString 
NewFileDlg::GetTitle()
{
  return m_title;
}

inline CString 
NewFileDlg::GetFilename()
{
  return m_filename;
}

inline CString 
NewFileDlg::GetTemplate()
{
  return m_template;
}
