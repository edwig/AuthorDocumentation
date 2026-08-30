//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    FormTextareaDlg.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for managing the <TEXTAREA> tag in HTML, 
//
#pragma once
#include "Stdafx.h"
#include "HtmlELement.h"
#include "resource.h"

// FormTextareaDlg dialog

class FormTextareaDlg : public CDialog
{
	DECLARE_DYNAMIC(FormTextareaDlg)

public:
	FormTextareaDlg(CWnd* pParent
                 ,CString p_base
                 ,CString name
                 ,CString cols
                 ,CString rows
                 ,CString wrap
                 ,HtmlElement* p_elem);
	virtual ~FormTextareaDlg();
  BOOL OnInitDialog();
  CString GetName();
  CString GetCols();
  CString GetRows();
  CString GetWrap();

// Dialog Data
	enum { IDD = IDD_TEXTAREA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
  HtmlElement* m_elem;
  CString      m_base;
  // Attributes
  CString m_name;
  CString m_cols;
  CString m_rows;
  CString m_wrap;
  // Controls
  CComboBox     m_comboWrap;
  CSpinButtonCtrl m_spinCols;
  CSpinButtonCtrl m_spinRows;
  AD_Button         m_buttonID;
  AD_Button         m_buttonEvents;
  AD_Button         m_buttonStyle;
  AD_Button         m_buttonOK;
  AD_Button         m_buttonCancel;
  AD_Button         m_buttonHelp;
public:
  afx_msg void OnEnChangeTaName();
  afx_msg void OnEnChangeTaWidth();
  afx_msg void OnEnChangeTaRows();
  afx_msg void OnCbnSelchangeTaWordwrap();
  afx_msg void OnBnClickedOk();
  afx_msg void OnBnClickedCancel();
  afx_msg void OnBnClickedTaHelp();
  afx_msg void OnBnClickedId();
  afx_msg void OnBnClickedStyle();
  afx_msg void OnBnClickedEvents();
};

inline CString 
FormTextareaDlg::GetName()
{
  return m_name;
}

inline CString 
FormTextareaDlg::GetCols()
{
  return m_cols;
}

inline CString 
FormTextareaDlg::GetRows()
{
  return m_rows;
}

inline CString 
FormTextareaDlg::GetWrap()
{
  return m_wrap;
}
