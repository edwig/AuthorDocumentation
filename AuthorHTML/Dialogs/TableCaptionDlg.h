//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    TableCaptionDlg.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for managing the <CAPTION> tag in HTML
//
#pragma once
#include "StdAfx.h"
#include "HTMLCaption.h"
#include "resource.h"

// TableCaptionDlg dialog

class TableCaptionDlg : public CDialog
{
	DECLARE_DYNAMIC(TableCaptionDlg)

public:
	TableCaptionDlg(CWnd* pParent,CString m_base,HtmlCaption* caption);
	virtual ~TableCaptionDlg();
  virtual BOOL OnInitDialog();
  
// Dialog Data
	enum { IDD = IDD_CAPTION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
  void FillPage();
  void UpdateProperties();

  HtmlCaption* m_caption;
  CString      m_base;

  CString     m_valign;
  CString     m_align;
  // Controls
  CComboBox m_comboVAlign;
  CComboBox m_comboAlign;
  // Standard controls
  AD_Button     m_buttonID;
  AD_Button     m_buttonEvents;
  AD_Button     m_buttonStyle;
  AD_Button     m_buttonOK;
  AD_Button     m_buttonCancel;
  AD_Button     m_buttonHelp;
public:
  afx_msg void OnCbnSelchangePosition();
  afx_msg void OnCbnSelchangeAlignment();
  afx_msg void OnBnClickedId();
  afx_msg void OnBnClickedStyle();
  afx_msg void OnBnClickedOk();
  afx_msg void OnBnClickedCancel();
  afx_msg void OnBnClickedHeadHelp();
  afx_msg void OnBnClickedEvents();
};
