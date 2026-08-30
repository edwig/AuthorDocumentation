//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    GlobalSetPage1Dlg.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for the 1th global settings page
//
#pragma once
#include "StdAfx.h"
#include "Resource.h"


// GlobalSetPage1Dlg dialog

class GlobalSetPage1Dlg : public CDialog
{
	DECLARE_DYNAMIC(GlobalSetPage1Dlg)

public:
	GlobalSetPage1Dlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~GlobalSetPage1Dlg();
  virtual BOOL OnInitDialog();
          void UpdateProperties();

// Dialog Data
	enum { IDD = IDD_SETTINGS_PAGE1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
  void FillPage();
  bool m_defNavigate;
  bool m_defShowIt;
  AD_Checkbox m_buttonNavigate;
  AD_Checkbox m_buttonShowIt;
public:
  afx_msg void OnBnClickedEditLink();
  afx_msg void OnBnClickedDclickDialog();
};
