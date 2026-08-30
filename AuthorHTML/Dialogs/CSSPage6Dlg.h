//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    CSSPage6Dlg.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for editing CSS properties
//              Handles the list-style attributes
//
#pragma once
#include "StdAfx.h"
#include "Css.h"
#include "resource.h"

// CSSPage6Dlg dialog

class CSSPage6Dlg : public CDialog
{
	DECLARE_DYNAMIC(CSSPage6Dlg)

public:
	CSSPage6Dlg(CWnd*          pParent
             ,CssStyleSheet* pCss);
	virtual ~CSSPage6Dlg();
  virtual BOOL OnInitDialog();
  void SetSelector(string selector);
  void UpdateProperties();

  // Dialog Data
	enum { IDD = IDD_SS_PAGE6 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
  void FillPage();
  void Redisplay();

  CssStyleSheet* m_css;
  string         m_selector;

  // Attributes
  CString        m_listStyle;
  CString        m_listPosition;
  CString        m_listImage;

  // CONTROLS
  CComboBox    m_comboStyle;
  CComboBox    m_comboPosition;
  AD_Button        m_buttonImage;
public:
  afx_msg void OnCbnSelchangeListstyle();
  afx_msg void OnCbnSelchangeStylepos();
  afx_msg void OnEnChangeStyleimage();
  afx_msg void OnBnClickedButListimage();
};
