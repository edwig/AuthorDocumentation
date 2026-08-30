//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    WindowDefPage5.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for all window properties page 5 of 5
//
#pragma once
#include "resource.h"
#include "ProjectFile.h"
#include "WindowDefinition.h"

// WindowDefPage5 dialog

class WindowDefPage5 : public CDialog
{
	DECLARE_DYNAMIC(WindowDefPage5)

public:
	WindowDefPage5(CWnd* p_parent
                ,ProjectFile* p_project);
	virtual ~WindowDefPage5();
  BOOL OnInitDialog();
  void SetProperties(WindowDefinition* p_window);
  void UpdateProperties();

// Dialog Data
	enum { IDD = IDD_WINDOWDEF_PAGE5 };
private:
  void ShowProperties();

  WindowDefinition* m_window;
  ProjectFile*      m_project;
  CString           m_toc;
  CString           m_index;
  CString           m_default;
  CString           m_home;

  AD_Button         m_buttonTOC;
  AD_Button         m_buttonIndex;
  AD_Button         m_buttonDefault;
  AD_Button         m_buttonHome;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnEnChangeToc();
  afx_msg void OnBnClickedButtonToc();
  afx_msg void OnEnChangeIndex();
  afx_msg void OnBnClickedButtonIndex();
  afx_msg void OnEnChangeDefault();
  afx_msg void OnBnClickedButtonDefault();
  afx_msg void OnEnChangeHome();
  afx_msg void OnBnClickedButtonHome();
};
