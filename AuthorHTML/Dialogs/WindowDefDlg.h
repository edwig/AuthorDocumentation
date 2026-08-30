//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    WindowDefDlg.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for all window properties pages
//
#pragma once
#include "resource.h"
#include "ProjectFile.h"
#include "WindowDefinition.h"
#include "WindowDefPage1.h"
#include "WindowDefPage2.h"
#include "WindowDefPage3.h"
#include "WindowDefPage4.h"
#include "WindowDefPage5.h"

// WindowDefDlg dialog

class WindowDefDlg : public CDialog
{
	DECLARE_DYNAMIC(WindowDefDlg)

public:
	WindowDefDlg(CWnd* p_parent
              ,ProjectFile* p_project
              ,CString      p_firstWindow);
	virtual ~WindowDefDlg();
  BOOL     OnInitDialog();

// Dialog Data
	enum { IDD = IDD_WINDOWDEF };
private:
  ProjectFile*      m_project;
  WindowDefinition* m_window;
  CString           m_currentWindow;

  WindowDefPage1*   m_page1;
  WindowDefPage2*   m_page2;
  WindowDefPage3*   m_page3;
  WindowDefPage4*   m_page4;
  WindowDefPage5*   m_page5;

  CComboBox     m_comboWindows;
  CString       m_title;
  CTabCtrl      m_tab;
  AD_Button     m_buttonNew;
  AD_Button     m_buttonDelete;
  AD_Button     m_buttonOK;
  AD_Button     m_buttonCancel;
  AD_Button     m_buttonHelp;

protected:
  void SetWindowNames();
  void SetProperties();
  void UpdateProperties();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnCbnSelchangeWindows();
  afx_msg void OnEnChangeTitle();
  afx_msg void OnBnClickedNewwindef();
  afx_msg void OnBnClickedDelwindef();
  afx_msg void OnBnClickedOk();
  afx_msg void OnBnClickedCancel();
  afx_msg void OnBnClickedHelpwindow();
  afx_msg void OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult);
};
