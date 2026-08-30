//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    ALinkDlg.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog to edit the <A> bookmark link name only
//
#pragma once
#include "resource.h"

// ALinkDlg dialog

class ALinkDlg : public CDialog
{
	DECLARE_DYNAMIC(ALinkDlg)

public:
	ALinkDlg(CWnd* p_parent,CString& p_keyword);
	virtual ~ALinkDlg();
  CString  GetKeyword();

// Dialog Data
	enum { IDD = IDD_ALINK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

  CString   m_keyword;
  AD_Button m_buttonHelp;
  AD_Button m_buttonOK;
  AD_Button m_buttonCancel;
public:
  afx_msg void OnEnChangeKeyword();
  afx_msg void OnBnClickedHelp();
  afx_msg void OnBnClickedOk();
  afx_msg void OnBnClickedCancel();
};

inline CString
ALinkDlg::GetKeyword()
{
  return m_keyword;
}