//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    FormFieldDlg.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for managing the <FIELD> tag in HTML, 
//
#pragma once
#include "StdAfx.h"
#include "HTMLElement.h"
#include "StyleSheetDlg.h"
#include "resource.h"

// FormFieldDlg dialog

class FormFieldDlg : public CDialog
{
	DECLARE_DYNAMIC(FormFieldDlg)

public:
	FormFieldDlg(CWnd* pParent
              ,CString p_base
              ,CString type
              ,CString name
              ,CString value
              ,CString maxLength
              ,CString size
              ,HtmlElement* elem);
	virtual ~FormFieldDlg();
  virtual BOOL OnInitDialog();
  CString GetType();
  CString GetName();
  CString GetValue();
  CString GetMaxLength();
  CString GetSize();

  // Dialog Data
	enum { IDD = IDD_FORMFIELD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
private:
  void FillPage();
  void UpdateProperties();

  // Attributes
  CString         m_base;
  CString         m_type;
  CString         m_name;
  CString         m_value;
  CString         m_maxLength;
  CString         m_size;
  HtmlElement*    m_elem;
  // Controls
  CComboBox       m_comboType;
  CSpinButtonCtrl m_spinMaxlength;
  CSpinButtonCtrl m_spinSize;
  AD_Button         m_buttonID;
  AD_Button         m_buttonEvents;
  AD_Button         m_buttonStyle;
  AD_Button         m_buttonOK;
  AD_Button         m_buttonCancel;
  AD_Button         m_buttonHelp;
	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnCbnSelchangeFfType();
  afx_msg void OnEnChangeFfName();
  afx_msg void OnEnChangeFfValue();
  afx_msg void OnEnChangeFfMaxlength();
  afx_msg void OnEnChangeFfSize();
  afx_msg void OnBnClickedOk();
  afx_msg void OnBnClickedCancel();
  afx_msg void OnBnClickedFfHelp();
  afx_msg void OnBnClickedId();
  afx_msg void OnBnClickedStyle();
  afx_msg void OnBnClickedEvents();
};

inline CString 
FormFieldDlg::GetType()
{
  return m_type;
}

inline CString 
FormFieldDlg::GetName()
{
  return m_name;
}

inline CString FormFieldDlg::GetValue()
{
  return m_value;
}

inline CString 
FormFieldDlg::GetMaxLength()
{
  return m_maxLength;
}

inline CString 
FormFieldDlg::GetSize()
{
  return m_size;
}
