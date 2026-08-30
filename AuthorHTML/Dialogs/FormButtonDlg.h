//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    FormButtonDlg.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for managing the <BUTTON> tag in HTML, 
//
#pragma once
#include "StdAfx.h"
#include "HTMLElement.h"
#include "resource.h"

// FormButtonDlg dialog

class FormButtonDlg : public CDialog
{
	DECLARE_DYNAMIC(FormButtonDlg)

public:
	FormButtonDlg(CWnd* pParent
               ,CString p_base
               ,CString type
               ,CString name
               ,CString value
               ,bool checked
               ,HtmlElement* elem);
	virtual ~FormButtonDlg();
  virtual BOOL OnInitDialog();
  CString GetType();
  CString GetName();
  CString GetValue();
  bool    GetChecked();

// Dialog Data
	enum { IDD = IDD_FORMBUTTON };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
  // Attributes
  CString       m_base;
  CString       m_type;
  CString       m_name;
  CString       m_value;
  bool          m_checked;
  HtmlElement*  m_elem;
  // Controls
  CComboBox     m_comboType;
  AD_Checkbox   m_buttonChecked;
  AD_Button     m_buttonID;
  AD_Button     m_buttonEvents;
  AD_Button     m_buttonStyle;
  AD_Button     m_buttonOK;
  AD_Button     m_buttonCancel;
  AD_Button     m_buttonHelp;
public:
  afx_msg void OnCbnSelchangeFbType();
  afx_msg void OnEnChangeFbName();
  afx_msg void OnEnChangeFbValue();
  afx_msg void OnBnClickedFbChecked();
  afx_msg void OnBnClickedOk();
  afx_msg void OnBnClickedCancel();
  afx_msg void OnBnClickedFbHelp();
  afx_msg void OnBnClickedId();
  afx_msg void OnBnClickedStyle();
  afx_msg void OnBnClickedEvents();
};

inline CString 
FormButtonDlg::GetType()
{
  return m_type;
}

inline CString 
FormButtonDlg::GetName()
{
  return m_name;
}

inline CString 
FormButtonDlg::GetValue()
{
  return m_value;
}

inline bool    
FormButtonDlg::GetChecked()
{
  return m_checked;
}
