//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    WindowDefPage2.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for all window properties page 2 of 5
//
#pragma once
#include "resource.h"
#include "ProjectFile.h"
#include "WindowDefinition.h"

// WindowDefPage2 dialog

class WindowDefPage2 : public CDialog
{
	DECLARE_DYNAMIC(WindowDefPage2)

public:
	WindowDefPage2(CWnd*        p_parent
                ,ProjectFile* p_project);
	virtual ~WindowDefPage2();
  BOOL     OnInitDialog();
  void     SetProperties(WindowDefinition* p_window);
  void     UpdateProperties();

// Dialog Data
	enum { IDD = IDD_WINDOWDEF_PAGE2 };

private:
  void    ShowProperties();
  ProjectFile*      m_project;
  WindowDefinition* m_window;

  bool    m_hideShow;
  bool    m_back;
  bool    m_forward;
  bool    m_stop;
  bool    m_refresh;
  bool    m_home;
  bool    m_options;
  bool    m_print;
  bool    m_locate;
  bool    m_user1;
  bool    m_user2;
  CString m_label1;
  CString m_label2;
  CString m_file1;
  CString m_file2;

  AD_Checkbox m_buttonHideShow;
  AD_Checkbox m_buttonBack;
  AD_Checkbox m_buttonForward;
  AD_Checkbox m_buttonStop;
  AD_Checkbox m_buttonRefresh;
  AD_Checkbox m_buttonHome;
  AD_Checkbox m_buttonOptions;
  AD_Checkbox m_buttonPrint;
  AD_Checkbox m_buttonLocate;
  AD_Checkbox m_buttonUser1;
  AD_Checkbox m_buttonUser2;
  AD_Button   m_buttonFile1;
  AD_Button   m_buttonFile2;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnBnClickedHideshow();
  afx_msg void OnBnClickedBack();
  afx_msg void OnBnClickedForward();
  afx_msg void OnBnClickedStop();
  afx_msg void OnBnClickedRefresh();
  afx_msg void OnBnClickedHome();
  afx_msg void OnBnClickedOptions();
  afx_msg void OnBnClickedPrint();
  afx_msg void OnBnClickedLocate();
  afx_msg void OnBnClickedUser1();
  afx_msg void OnBnClickedUser2();
  afx_msg void OnEnChangeLabel1();
  afx_msg void OnEnChangeFile1();
  afx_msg void OnBnClickedButtonFile1();
  afx_msg void OnEnChangeLabel2();
  afx_msg void OnEnChangeFile2();
  afx_msg void OnBnClickedButtonFile2();
};
