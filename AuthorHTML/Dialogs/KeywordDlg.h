//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    KeywordDlg.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for managing the 5-level keywords in HTML
//
#pragma once
#include "resource.h"
#include "HTMLElement.h"
#include "TopicPropPage3.h"

// KeywordDlg dialog

class KeywordDlg : public CDialog
{
	DECLARE_DYNAMIC(KeywordDlg)

public:
	KeywordDlg(CWnd*       p_parent
            ,KeywordDef* p_keyword); 
	virtual ~KeywordDlg();
  BOOL OnInitDialog();

// Dialog Data
	enum { IDD = IDD_KEYWORDS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
  void FillPage();
  void SyncFocus();
  void UpdateProperties();
  void ReComposite();
  void CheckType();
  void CheckWord(CString& p_word);

  KeywordDef*  m_keyword;
  CComboBox    m_comboType;

  KeywordType  m_type;
  CString      m_composite;
  CString      m_level1;
  CString      m_level2;
  CString      m_level3;
  CString      m_level4;
  CString      m_level5;
public:
  afx_msg void OnEnChangeComposite();
  afx_msg void OnEnChangeLevel1();
  afx_msg void OnEnChangeLevel2();
  afx_msg void OnEnChangeLevel3();
  afx_msg void OnEnChangeLevel4();
  afx_msg void OnEnChangeLevel5();
  afx_msg void OnBnClickedOk();
  afx_msg void OnBnClickedCancel();
  afx_msg void OnBnClickedHelpKeyword();
  afx_msg void OnCbnSelchangeLinktype();
};
