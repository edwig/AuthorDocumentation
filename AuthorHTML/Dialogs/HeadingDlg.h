//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    HeadingDlg.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for headings (H1..H6) in HTML
//
#pragma once
#include "Stdafx.h"
#include "HtmlHeading.h"
#include "resource.h"

// HeadingDlg dialog

class HeadingDlg : public CDialog
{
	DECLARE_DYNAMIC(HeadingDlg)

public:
	HeadingDlg(CWnd* pParent
            ,CString p_base
            ,CString tag
            ,HtmlHeading* head);
	virtual ~HeadingDlg();
  virtual BOOL OnInitDialog();

// Dialog Data
	enum { IDD = IDD_HEADING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
  void FillPage();
  void UpdatePropterties();

  HtmlHeading* m_head;
  CString m_base;
  CString m_type;
  CString m_tag;
  CString m_align;
  // Controls
  CComboBox m_comboAlign;
  AD_Button     m_buttonID;
  AD_Button     m_buttonEvents;
  AD_Button     m_buttonStyle;
  AD_Button     m_buttonOK;
  AD_Button     m_buttonCancel;
  AD_Button     m_buttonHelp;
public:
  afx_msg void OnCbnSelchangeAlignment();
  afx_msg void OnBnClickedId();
  afx_msg void OnBnClickedStyle();
  afx_msg void OnBnClickedOk();
  afx_msg void OnBnClickedHelp();
  afx_msg void OnBnClickedEvents();
};
