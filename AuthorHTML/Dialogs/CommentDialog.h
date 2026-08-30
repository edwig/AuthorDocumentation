//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    CommentDialog.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog to edit the <COMMENT> tag in the HTML file. 
//              The comment is not visible in the browser, 
//              but it is visible in the HTML source code
//
#pragma once
#include "StdAfx.h"
#include "resource.h"

// CommentDialog dialog

class CommentDialog : public CDialog
{
	DECLARE_DYNAMIC(CommentDialog)

public:
	CommentDialog(CString p_text
               ,CWnd* pParent = NULL);   // standard constructor
	virtual ~CommentDialog();
  CString  GetText();

// Dialog Data
	enum { IDD = IDD_COMMENT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  CEdit   m_editBox;
  CString m_text;
  CWnd*   m_parent;

  AD_Button m_buttonOK;
  AD_Button m_buttonCancel;
  AD_Button m_buttonHelp;
	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnBnClickedOk();
};

inline CString
CommentDialog::GetText()
{
  return m_text;
}