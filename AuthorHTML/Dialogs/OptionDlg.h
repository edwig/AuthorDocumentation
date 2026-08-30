//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    OptionDlg.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for managing the <OPTION> tag in HTML
//
#pragma once
#include "resource.h"

struct FormOption
{
  CString m_display;
  CString m_value;
  bool    m_selected;
};

// OptionDlg dialog

class OptionDlg : public CDialog
{
	DECLARE_DYNAMIC(OptionDlg)

public:
	OptionDlg(CWnd* p_parent,FormOption* p_option);
	virtual ~OptionDlg();
  BOOL OnInitDialog();

// Dialog Data
	enum { IDD = IDD_OPTION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  FormOption* m_option;
  CString     m_display;
  CString     m_value;
  bool        m_selected;

  AD_Checkbox m_checkSelected;
  AD_Button   m_buttonHelp;
  AD_Button   m_buttonOK;
  AD_Button   m_buttonCancel;
	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnEnChangeDisplay();
  afx_msg void OnEnChangeValue();
  afx_msg void OnBnClickedSelected();
  afx_msg void OnBnClickedHelp();
  afx_msg void OnBnClickedOk();
  afx_msg void OnBnClickedCancel();
};
