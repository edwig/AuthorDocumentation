//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    FieldsetDlg.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for editing <FIELDSET> properties
//
#pragma once
#include "resource.h"
#include "HTMLFieldset.h"

// FieldsetDlg dialog

class FieldsetDlg : public CDialog
{
	DECLARE_DYNAMIC(FieldsetDlg)

public:
	FieldsetDlg(CWnd* pParent,CString base,HtmlFieldset* set);
	virtual ~FieldsetDlg();
  virtual BOOL OnInitDialog();

// Dialog Data
	enum { IDD = IDD_FIELDSET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
  void FillPage();
  void UpdateProperties();

  HtmlFieldset* m_fieldset;
  CString       m_base;
  // Attributes
  CString m_legend;
  CString m_alignFieldset;
  CString m_alignLegend;
  // Controls
  CComboBox m_comboAlignFS;
  CComboBox m_comboAlignLE;
  AD_Button m_buttonID;
  AD_Button m_buttonEvents;
  AD_Button m_buttonStyle;
  AD_Button m_buttonOK;
  AD_Button m_buttonCancel;
  AD_Button m_buttonHelp;
public:
  afx_msg void OnEnChangeLegend();
  afx_msg void OnCbnSelchangeFsAlign();
  afx_msg void OnCbnSelchangeLeAlign();
  afx_msg void OnBnClickedId();
  afx_msg void OnBnClickedEvents();
  afx_msg void OnBnClickedStyle();
  afx_msg void OnBnClickedOk();
  afx_msg void OnBnClickedCancel();
  afx_msg void OnBnClickedFieldsetHelp();
};
