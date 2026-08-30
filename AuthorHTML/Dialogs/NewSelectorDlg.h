//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    NewSelectorDlg.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for creating an new CSS selector
//
#pragma once
#include "StdAfx.h"
#include "resource.h"

// NewSelectorDlg dialog

class NewSelectorDlg : public CDialog
{
	DECLARE_DYNAMIC(NewSelectorDlg)

public:
	NewSelectorDlg(CWnd* pParent); 
	virtual ~NewSelectorDlg();
  BOOL     OnInitDialog();
  CString  GetSelector();

// Dialog Data
	enum { IDD = IDD_NEW_STYLERULE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
  void    ResetDialog();
  void    ExtraSelector(char extra);
  CString GetAddedSelector();

  CString     m_totalResult;
  CString     m_selector;
  CString     m_attribute;
  CString     m_pseudo;
  CString     m_classname;
  CString     m_uniqueID;
  // CONTROLS
  CComboBox m_comboSelector;
  CComboBox m_comboAttribute;
  CComboBox m_comboPseudo;
  // Radios
  AD_Radiobox m_buttonSel;
  AD_Radiobox m_buttonAttr;
  AD_Radiobox m_buttonPseudo;
  AD_Radiobox m_buttonID;
  AD_Checkbox m_buttonClass;
  // Extra selector buttons
  AD_Button   m_buttonIdentical;
  AD_Button   m_buttonDescendant;
  AD_Button   m_buttonDirectChild;
  AD_Button   m_buttonAdjacent;
  // Standard controls
  AD_Button   m_buttonOK;
  AD_Button   m_buttonCancel;
  AD_Button   m_buttonHelp;
public:
  afx_msg void OnCbnSelchangeNewSelector();
  afx_msg void OnCbnSelchangeOnlyAttribute();
  afx_msg void OnCbnSelchangeOnlyPseudo();
  afx_msg void OnBnClickedRadio1();
  afx_msg void OnBnClickedRadio2();
  afx_msg void OnBnClickedRadio3();
  afx_msg void OnBnClickedOk();
  afx_msg void OnBnClickedCancel();
  afx_msg void OnBnClickedNsrHelp();
  afx_msg void OnChangeSelector();
  afx_msg void OnEnChangeClassname();
  afx_msg void OnBnClickedExtSame();
  afx_msg void OnBnClickedExtDescendant();
  afx_msg void OnBnClickedExtDirect();
  afx_msg void OnBnClickedExtAdjacent();
  afx_msg void OnBnClickedNewClassname();
  afx_msg void OnEnChangeId();
  afx_msg void OnBnClickedRadio4();
};
