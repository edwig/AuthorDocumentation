//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    StyleSheetImportsDlg.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for importing a CSS Stylesheet into the project
//
#pragma once
#include "CSS.h"
#include "resource.h"

// StyleSheetImportsDlg dialog

class StyleSheetImportsDlg : public CDialog
{
	DECLARE_DYNAMIC(StyleSheetImportsDlg)

public:
	StyleSheetImportsDlg(CWnd*          pParent 
                      ,CssStyleSheet* css
                      ,CString        base);
	virtual ~StyleSheetImportsDlg();
  virtual BOOL OnInitDialog();
  bool CanApply();

  // Dialog Data
	enum { IDD = IDD_CSSIMPORTS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
  void FillPage();
  void UpdateProperties();

  CssStyleSheet* m_css;
  CString        m_base;
  bool           m_canApply;
  // Controls
  CListBox   m_list;
  AD_Button    m_buttonUp;
  AD_Button    m_buttonEdit;
  AD_Button    m_buttonNew;
  AD_Button    m_buttonDelete;
  AD_Button    m_buttonDown;
  // Common
  AD_Button    m_buttonOK;
  AD_Button    m_buttonCancel;
  AD_Button    m_buttonHelp;
public:
  afx_msg void OnLbnSelchangeImports();
  afx_msg void OnBnClickedImpUp();
  afx_msg void OnBnClickedImpNew();
  afx_msg void OnBnClickedImpDelete();
  afx_msg void OnBnClickedImpDown();
  afx_msg void OnBnClickedOk();
  afx_msg void OnBnClickedCancel();
  afx_msg void OnBnClickedImpHelp();
  afx_msg void OnBnClickedEdit();
};

inline bool
StyleSheetImportsDlg::CanApply()
{
  return m_canApply;
}
