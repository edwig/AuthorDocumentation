//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    GeneralIDDlg.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for managing the ID attribute in HTML, 
//
#pragma once
#include "StdAfx.h"
#include "HTMLElement.h"
#include "resource.h"

// GeneralIDDlg dialog

class GeneralIDDlg : public CDialog
{
	DECLARE_DYNAMIC(GeneralIDDlg)

public:
	GeneralIDDlg(CWnd* pParent
              ,CString type
              ,HtmlElement* elem);
	virtual ~GeneralIDDlg();

// Dialog Data
	enum { IDD = IDD_GENERAL_ID };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
  bool CheckID();
  void UpdateProperties();
  // Attributes
  CString m_type;
  CString m_id;
  CString m_className;
  HtmlElement* m_elem;
  // Controls
  AD_Button m_buttonOK;
  AD_Button m_buttonCancel;
  AD_Button m_buttonHelp;
public:
  afx_msg void OnEnChangeId();
  afx_msg void OnEnChangeClassname();
  afx_msg void OnBnClickedOk();
  afx_msg void OnBnClickedCancel();
  afx_msg void OnBnClickedHelp();
};
