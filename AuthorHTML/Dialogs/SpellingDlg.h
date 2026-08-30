//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    SpellingDlg.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for acting on the spelling results of a word
//
#pragma once
#include "Spelling.h"
#include "resource.h"

// SpellingDlg dialog

class SpellingDlg : public CDialog
{
	DECLARE_DYNAMIC(SpellingDlg)

public:
	SpellingDlg(CWnd* pParent,CString word,Spelling* speller);
	virtual ~SpellingDlg();
  virtual BOOL OnInitDialog();
  CString GetCorrectedWord();

// Dialog Data
	enum { IDD = IDD_SPELLING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
  void      GetLanguage();
  void      GetAlternatives();
  void      DisplayChanging();

  CString   m_word;       // Word to be checked
  CString   m_corrected;  // Corrected word
  CString   m_language;   // Current language
  Spelling* m_speller;    // Speller interface with dictionaries
  // Controls
  CListBox    m_listSuggestions;
  CComboBox m_comboLanguage;
  AD_Button     m_buttonIgnore;
  AD_Button     m_buttonChange;
  AD_Button     m_buttonAdd;
  AD_Button     m_buttonOptions;
public:
  afx_msg void OnEnChangeSpelCorrect();
  afx_msg void OnLbnSelchangeSpelSuggest();
  afx_msg void OnCbnSelchangeDictionaries();
  afx_msg void OnBnClickedSpelIgnore();
  afx_msg void OnBnClickedSpelChange();
  afx_msg void OnBnClickedSpelAdd();
  afx_msg void OnBnClickedSpelOptions();
  afx_msg void OnBnClickedCancel();
};

inline CString 
SpellingDlg::GetCorrectedWord()
{
  return m_corrected;
}
