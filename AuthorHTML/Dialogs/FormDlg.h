//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    Form.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for managing the <FORM> tag in HTML, 
//
#pragma once
#include "StdAfx.h"
#include "HTMLElement.h"
#include "resource.h"

// FormDlg dialog

class FormDlg : public CDialog
{
	DECLARE_DYNAMIC(FormDlg)

public:
	FormDlg(CWnd*   pParent
         ,CString p_base
         ,CString name
         ,CString action
         ,CString method
         ,CString encoding
         ,CString target
         ,HtmlElement* elem);
	virtual ~FormDlg();
  virtual BOOL OnInitDialog();
  CString GetName();
  CString GetActionUrl();
  CString GetMethod();
  CString GetEncoding();
  CString GetTarget();

// Dialog Data
	enum { IDD = IDD_FORM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  DECLARE_MESSAGE_MAP()
private:
  // Attributes
  CString       m_name;
  CString       m_actionUrl;
  CString       m_method;
  CString       m_encoding;
  CString       m_target;
  HtmlElement*  m_elem;
  CString       m_base;
  // Controls
  CComboBox     m_comboMethod;
  CComboBox     m_comboEncoding;
  CComboBox     m_comboTarget;
  AD_Button       m_buttonID;
  AD_Button       m_buttonEvents;
  AD_Button       m_buttonStyle;
  AD_Button       m_buttonOK;
  AD_Button       m_buttonCancel;
  AD_Button       m_buttonHelp;
public:
  afx_msg void OnEnChangeFormName();
  afx_msg void OnEnChangeFormAction();
  afx_msg void OnCbnSelchangeFormMethod();
  afx_msg void OnCbnSelchangeFormEncoding();
  afx_msg void OnCbnSelchangeFormTarget();
  afx_msg void OnBnClickedOk();
  afx_msg void OnBnClickedCancel();
  afx_msg void OnBnClickedFormHelp();
  afx_msg void OnBnClickedId();
  afx_msg void OnBnClickedStyle();
  afx_msg void OnBnClickedEvents();
};

inline CString 
FormDlg::GetName()
{
  return m_name;
}

inline CString 
FormDlg::GetActionUrl()
{
  return m_actionUrl;
}

inline CString 
FormDlg::GetMethod()
{
  return m_method;
}

inline CString 
FormDlg::GetEncoding()
{
  return m_encoding;
}

inline CString 
FormDlg::GetTarget()
{
  return m_target;
}
