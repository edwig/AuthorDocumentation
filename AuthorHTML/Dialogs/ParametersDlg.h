//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    ParametersDlg.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for managing the <PARAM> tag of an <OBJECT> in HTML
//
#pragma once
#include "resource.h"
#include "HtmlObject.h"
#include "GridCtrl.h"

// ParametersDlg dialog

class ParametersDlg : public CDialog
{
	DECLARE_DYNAMIC(ParametersDlg)

public:
	ParametersDlg(CWnd* p_parent,HtmlObject* p_object);
 ~ParametersDlg();
  BOOL OnInitDialog();

// Dialog Data
	enum { IDD = IDD_PARAMETERS };

protected:
	void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  void FillPage();
  void UpdateObject();

  // Internal state
  HtmlObject*  m_object;
  bool         m_changed;
  int          m_maxParam;
  CString      m_saveEdit;

  // Controls
  CGridCtrl    m_list;
  AD_Button    m_buttonDelete;
  AD_Button    m_buttonOK;
  AD_Button    m_buttonApply;
  AD_Button    m_buttonCancel;
  AD_Button    m_buttonHelp;

	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnBeginEdit(NMHDR *pNMHDR, LRESULT *pResult);
  afx_msg void OnEndInPlaceEdit(NMHDR *pNMHDR, LRESULT *pResult);
  afx_msg void OnBnClickedDelete();
  afx_msg void OnBnClickedOk();
  afx_msg void OnBnClickedButapply();
  afx_msg void OnBnClickedCancel();
  afx_msg void OnBnClickedHelpacommand();
};
