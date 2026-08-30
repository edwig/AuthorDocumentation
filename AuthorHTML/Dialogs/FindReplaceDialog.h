//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    FindReplaceDialog.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for finding and replacing text.
//
#pragma once
#include "StdAfx.h"
#include "resource.h"

#define FRD_MATCHCASE    0x001
#define FRD_MATCHWORD    0x002
#define FRD_SEARCHUP     0x004
#define FRD_USEREGULAR   0x008
#define FRD_DOCUMENT     0x010
#define FRD_ALLOPENDOC   0x020
#define FRD_PROJECT      0x040
#define FRD_BLOCKONLY    0x080

// FindReplaceDialog dialog

class FindReplaceDialog : public CDialog
{
	DECLARE_DYNAMIC(FindReplaceDialog)

public:
	FindReplaceDialog(bool    bFind
                   ,CString sFindText
                   ,CString sReplaceText
                   ,CWnd*   pParent = NULL);   // standard constructor
	virtual ~FindReplaceDialog();
  CString  GetFindText();
  CString  GetReplaceText();
  int      GetOptions();
  BOOL     OnInitDialog();

  // Dialog Data
	enum { IDD = IDD_FINDREPLACE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
          void ReadFindReplaceHistory();
          void WriteFindReplaceHistory();

  bool      m_bFind;            // Find (true) or Find-and-Replace (false)
  CString   m_findText;         // Primary text to find
  CString   m_replaceText;      // Primary text to replace
  UINT      m_options;
  // Combos with text
  CComboBox m_comboFind;
  CComboBox m_comboReplace;
  // how to find
  AD_Checkbox m_buttonCase;
  AD_Checkbox m_buttonWhole;
  AD_Checkbox m_buttonUp;
  AD_Checkbox m_buttonRegular;
  // where to find
  AD_Checkbox m_buttonDocument;
  AD_Checkbox m_buttonAll;
  AD_Checkbox m_buttonProject;
  AD_Checkbox m_buttonBlock;
  // find/replace actions
  AD_Button   m_buttonFindNext;
  AD_Button   m_buttonReplaceWith;
  AD_Button   m_buttonReplaceAll;
  // Common buttons
  AD_Button   m_buttonOK;
  AD_Button   m_buttonCancel;
  AD_Button   m_buttonHelp;

	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnCbnSelchangeComboFind();
  afx_msg void OnCbnSelchangeComboReplace();
  afx_msg void OnBnClickedButtonFindnext();
  afx_msg void OnBnClickedOk();
  afx_msg void OnBnClickedCancel();
  afx_msg void OnBnClickedButtonReplacewith();
  afx_msg void OnBnClickedCheckCase();
  afx_msg void OnBnClickedCheckWhole();
  afx_msg void OnBnClickedCheckUp();
  afx_msg void OnBnClickedCheckRegular();
  afx_msg void OnBnClickedRadioDocument();
  afx_msg void OnBnClickedRadioAll();
  afx_msg void OnBnClickedRadioProject();
  afx_msg void OnBnClickedRadioBlock();
  afx_msg void OnBnClickedButtonReplaceall();
};

inline CString
FindReplaceDialog::GetFindText()
{
  return m_findText;
}

inline CString
FindReplaceDialog::GetReplaceText()
{
  return m_replaceText;
}

inline int
FindReplaceDialog::GetOptions()
{
  return m_options;
}