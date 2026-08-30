//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    GlobalSettingsDlg.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for the global settings of the AuthorHTML program
//
#pragma once
#include "StdAfx.h"
#include "resource.h"
#include "GlobalSetPage1Dlg.h"
#include "SpellOptionsDlg.h"

// GlobalSettingsDlg dialog

class GlobalSettingsDlg : public CDialog
{
	DECLARE_DYNAMIC(GlobalSettingsDlg)

public:
	GlobalSettingsDlg(CWnd* pParent,Spelling* speller);
	virtual ~GlobalSettingsDlg();
  virtual BOOL OnInitDialog();

// Dialog Data
	enum { IDD = IDD_SETTINGS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
  void FillPage();
  void UpdateProperties();

  bool      m_canApply;
  int       m_firstTab;

  CTabCtrl           m_tabControl;
  GlobalSetPage1Dlg* m_page1;
  SpellOptionsDlg*   m_page2;
  // Common controls
  AD_Button   m_buttonOK;
  AD_Button   m_buttonApply;
  AD_Button   m_buttonCancel;
  AD_Button   m_buttonHelp;
public:
  afx_msg void OnTcnSelchangeTabcontrol(NMHDR *pNMHDR, LRESULT *pResult);
  afx_msg void OnBnClickedOk();
  afx_msg void OnBnClickedCancel();
  afx_msg void OnBnClickedApply();
  afx_msg void OnBnClickedHelp();
};
