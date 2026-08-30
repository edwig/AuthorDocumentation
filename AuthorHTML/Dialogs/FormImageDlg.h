//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    FormImageDlg.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for managing the <IMG> tag in HTML, 
//
#pragma once
#include "StdAfx.h"
#include "HTMLInput.h"
#include "resource.h"

// FormImageDlg dialog

class FormImageDlg : public CDialog
{
	DECLARE_DYNAMIC(FormImageDlg)

public:
	FormImageDlg(CWnd*   pParent
              ,CString base
              ,HtmlInput* elem);
	virtual ~FormImageDlg();
  virtual BOOL OnInitDialog();

// Dialog Data
	enum { IDD = IDD_FORMIMAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
  void FillPage();
  void UpdateProperties();

  // Attributes
  HtmlInput*  m_elem;
  CString     m_base;
  CString     m_name;
  CString     m_url;
  CString     m_align;
  // Controls
  AD_Button     m_buttonChoose;
  CComboBox m_comboAlign;
  AD_Button     m_buttonID;
  AD_Button     m_buttonEvents;
  AD_Button     m_buttonStyle;
  AD_Button     m_buttonOK;
  AD_Button     m_buttonCancel;
  AD_Button     m_buttonHelp;
public:
  afx_msg void OnEnChangeFiName();
  afx_msg void OnEnChangeFiUrl();
  afx_msg void OnBnClickedFiChoose();
  afx_msg void OnCbnSelchangeFiAlign();
  afx_msg void OnBnClickedOk();
  afx_msg void OnBnClickedCancel();
  afx_msg void OnBnClickedFiHelp();
  afx_msg void OnBnClickedId();
  afx_msg void OnBnClickedStyle();
  afx_msg void OnBnClickedEvents();
};
