//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    ButtonDlg.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog to edit the <BUTTON> tag to be able to make buttons in a form
//
#pragma once
#include "StdAfx.h"
#include "HtmlElement.h"
#include "resource.h"

// ButtonDlg dialog

class ButtonDlg : public CDialog
{
	DECLARE_DYNAMIC(ButtonDlg)

public:
	ButtonDlg(CWnd* pParent
           ,CString base
           ,HtmlElement* elem);
	virtual ~ButtonDlg();
  virtual BOOL OnInitDialog();

// Dialog Data
	enum { IDD = IDD_BUTTON };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
  void FillPage();
  void UpdateProperties();
  HtmlElement* m_elem;
  CString      m_base;
  // Attributes
  CString m_name;
  CString m_value;
  CString m_type;
  // COntrols
  CComboBox   m_comboType;
  // Standard Controls
  AD_Button     m_buttonID;
  AD_Button     m_buttonEvents;
  AD_Button     m_buttonStyle;
  AD_Button     m_buttonOK;
  AD_Button     m_buttonCancel;
  AD_Button     m_buttonHelp;
public:
  afx_msg void OnEnChangeButtonName();
  afx_msg void OnEnChangeButtonValue();
  afx_msg void OnCbnSelchangeButtonType();
  afx_msg void OnBnClickedId();
  afx_msg void OnBnClickedStyle();
  afx_msg void OnBnClickedOk();
  afx_msg void OnBnClickedCancel();
  afx_msg void OnBnClickedButtonHelp();
  afx_msg void OnBnClickedEvents();
};
