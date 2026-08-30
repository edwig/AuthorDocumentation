//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    WindowDefPage4.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for all window properties page 4 of 5
//
#pragma once
#include "resource.h"
#include "ProjectFile.h"
#include "WindowDefinition.h"

// WindowDefPage4 dialog

class WindowDefPage4 : public CDialog
{
	DECLARE_DYNAMIC(WindowDefPage4)

public:
	WindowDefPage4(CWnd* p_parent,ProjectFile* p_project);
	virtual ~WindowDefPage4();
  BOOL OnInitDialog();
  void SetProperties(WindowDefinition* p_window);
  void UpdateProperties();

// Dialog Data
	enum { IDD = IDD_WINDOWDEF_PAGE4 };
private:
  void ShowProperties();

  ProjectFile*      m_project;
  WindowDefinition* m_window;

  bool  m_remember;
  int   m_left;
  int   m_top;
  int   m_width;
  int   m_height;
  int   m_right;
  int   m_bottom;

  AD_Checkbox m_buttonRemember;
  AD_Button   m_buttonAutosizer;
  AD_Button   m_buttonDefaults;
  AD_Button   m_buttonReset;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnBnClickedRemember();
  afx_msg void OnEnChangeLeft();
  afx_msg void OnEnChangeTop();
  afx_msg void OnEnChangeWidth();
  afx_msg void OnEnChangeHeight();
  afx_msg void OnBnClickedAutosizer();
  afx_msg void OnBnClickedDefaults();
  afx_msg void OnBnClickedReset();
};
