//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    NaviageHyperlink.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for choosing an action on a hyperlink in HTML
//              Follow it, or edit it, or don't show this dialog again
//
#pragma once
#include "StdAfx.h"
#include "resource.h"

// NavigateHyperlink dialog

class NavigateHyperlink : public CDialog
{
	DECLARE_DYNAMIC(NavigateHyperlink)

public:
	NavigateHyperlink(bool    bNavigate
	                 ,bool    bShowIt
	                 ,CString sRelation
	                 ,CWnd*   pParent = NULL);   // standard constructor
	virtual ~NavigateHyperlink();
	BOOL    OnInitDialog();
	bool    GetNavigate();
	bool    GetShowIt();

// Dialog Data
	enum { IDD = IDD_DONAVIGATE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  int GetRelationIndex(CString& rel);

  bool          m_bNavigate;
  bool          m_bShowIt;  
  CString       m_sRelation;
  CComboBox     m_relationCombo;
  AD_Checkbox   m_buttonNavigate;
  AD_Checkbox   m_buttonEdit;
  AD_Checkbox   m_dontShowAgain;

  AD_Button     m_buttonOK;
  AD_Button     m_buttonCancel;
  AD_Button     m_buttonHelp;
	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnBnClickedNavHelp();
  afx_msg void OnBnClickedNavNavigate();
  afx_msg void OnBnClickedNavEdit();
  afx_msg void OnBnClickedNavShowagain();
};

inline bool    
NavigateHyperlink::GetNavigate()
{
  return m_bNavigate;
}

inline bool   
NavigateHyperlink::GetShowIt()
{
  return m_bShowIt;
}
