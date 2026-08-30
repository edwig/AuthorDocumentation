//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    DivTextDlg.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for editing <DIV> text properties
//
#pragma once
#include "StdAfx.h"
#include "HTMLElement.h"
#include "resource.h"

// DivTextDlg dialog

class DivTextDlg : public CDialog
{
	DECLARE_DYNAMIC(DivTextDlg)

public:
	DivTextDlg(CWnd* pParent
            ,CString p_base
            ,CString tag
            ,CString title
            ,CString text
            ,HtmlElement* elem);
	virtual ~DivTextDlg();
  virtual BOOL OnInitDialog();
  CString GetTitle();
  CString GetText();

  // Dialog Data
	enum { IDD = IDD_DIVTEXT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
  CString       m_base;
  CString       m_tag;
  CString       m_title;
  CString       m_text;
  HtmlElement*  m_elem;
  // Controls
  AD_Button       m_buttonOK;
  AD_Button       m_buttonCancel;
  AD_Button       m_buttonHelp;
  AD_Button       m_buttonID;
  AD_Button       m_buttonEvents;
  AD_Button       m_buttonStyle;
public:
  afx_msg void OnEnChangeTextTitle();
  afx_msg void OnEnChangeTextText();
  afx_msg void OnBnClickedOk();
  afx_msg void OnBnClickedCancel();
  afx_msg void OnBnClickedTextHelp();
  afx_msg void OnBnClickedId();
  afx_msg void OnBnClickedStyle();
  afx_msg void OnBnClickedEvents();
};

inline CString
DivTextDlg::GetTitle()
{
  return m_title;
}

inline CString 
DivTextDlg::GetText()
{
  return m_text;
}
