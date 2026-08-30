//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    SpellOptionsDlg.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for the settings of the spelling checker
//
#pragma once
#include "StdAfx.h"
#include "Spelling.h"
#include "resource.h"

// SpellOptionsDlg dialog

class SpellOptionsDlg : public CDialog
{
	DECLARE_DYNAMIC(SpellOptionsDlg)

public:
	SpellOptionsDlg(CWnd* pParent
                 ,Spelling* speller
                 ,bool canDisable
                 ,bool removeButtons = false);
	virtual ~SpellOptionsDlg();
  virtual BOOL OnInitDialog();
  CString      GetLanguage();
  void         UpdateProperties();

// Dialog Data
	enum { IDD = IDD_SPELL_OPTIONS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
  void        FillPage();
  void        FillLanguage();
  Spelling*   m_speller;
  bool        m_removeButtons;
  // Attributes
  bool        m_canDisable;
  bool        m_enable;
  CString     m_language;
  bool        m_showErrors;
  bool        m_ignoreNumberWOrds;
  bool        m_ignoreAllCapitals;
  bool        m_useCustom;
  // Controls
  AD_Checkbox m_buttonEnable;
  CComboBox   m_comboLanguage;
  AD_Checkbox m_buttonShowErrors;
  AD_Checkbox m_buttonIgnoreNumber;
  AD_Checkbox m_buttonIgnoreCapitals;
  AD_Checkbox m_buttonUseCustom;
  // Common buttons
  AD_Button   m_buttonOK;
  AD_Button   m_buttonCancel;
  AD_Button   m_buttonHelp;
public:
  afx_msg void OnBnClickedSoEnable();
  afx_msg void OnBnClickedShowerrors();
  afx_msg void OnBnClickedNumberignore();
  afx_msg void OnBnClickedIgnorecapitals();
  afx_msg void OnBnClickedUsecustom();
  afx_msg void OnBnClickedOk();
  afx_msg void OnBnClickedCancel();
  afx_msg void OnBnClickedSoHelp();
  afx_msg void OnCbnSelchangeLanguage();
};

inline CString
SpellOptionsDlg::GetLanguage()
{
  return m_language;
}