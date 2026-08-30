//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    CSSPage1Dlg.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for editing CSS properties
//              Shows an overview list of all properties for a specific selector 
//
#pragma once
#include "StdAfx.h"
#include "CSS.h"
#include "resource.h"

// CSSPage1Dlg dialog

class CSSPage1Dlg : public CDialog
{
	DECLARE_DYNAMIC(CSSPage1Dlg)

public:
	CSSPage1Dlg(CWnd* pParent
             ,CssStyleSheet* p_css);
	virtual ~CSSPage1Dlg();
  BOOL     OnInitDialog();
  void     SetSelector(CString selector,CString descriptor);
  void     UpdateProperties();

// Dialog Data
	enum { IDD = IDD_SS_PAGE1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  void SetProperties(CString selector);

	DECLARE_MESSAGE_MAP()

private:
  CString   m_selector;
  CString   m_descriptor;
  CssStyleSheet* m_css;
  // Controls
  CListBox  m_listBox;
  AD_Button   m_buttonDelete;
public:
  afx_msg void OnBnClickedDelete();
};
