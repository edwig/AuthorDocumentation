//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    CharacterDialog.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog to insert a special character in the HTML file.
//
#pragma once
#include "StdAfx.h"
#include "resource.h"

// CharacterDialog dialog

class CharacterDialog : public CDialog
{
	DECLARE_DYNAMIC(CharacterDialog)

public:
	CharacterDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CharacterDialog();
  BOOL     OnInitDialog();
  CString  GetHTML();

// Dialog Data
	enum { IDD = IDD_INSERT_CHARACTER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  void    AddToComboByCategory(int letter_cat);
  void    AddToCombo(int cat,int letter);
  int     FindCategory(int letter);
  CString FindHTML(int letter);

  AD_Checkbox     m_butAccentLower;
  AD_Checkbox     m_butAccentUpper;
  AD_Checkbox     m_butOtherLower;
  AD_Checkbox     m_butOtherUpper;
  AD_Checkbox     m_butGreekLower;
  AD_Checkbox     m_butGreekUpper;
  AD_Checkbox     m_butCommon;
  CComboBox       m_letter;
  CComboBox       m_character;
  AD_Button       m_buttonInsert;
  AD_Button       m_buttonOK;
  AD_Button       m_buttonCancel;
  AD_Button       m_buttonHelp;

  CString   m_result;
  CWnd*     m_parent;
	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnBnClickedRadioAccLower();
  afx_msg void OnBnClickedRadioAccUpper();
  afx_msg void OnCbnSelchangeLetterCombo();
  afx_msg void OnCbnSelchangeCharCombo();
  afx_msg void OnBnClickedRadioOhterLower();
  afx_msg void OnBnClickedRadioOtherUpper();
  afx_msg void OnBnClickedRadioCommon();
  afx_msg void OnBnClickedInsertChar();
  afx_msg void OnBnClickedGreekLower();
  afx_msg void OnBnClickedGreekUpper();
};

inline CString
CharacterDialog::GetHTML()
{
  return m_result;
}