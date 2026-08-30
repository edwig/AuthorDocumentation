//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    ALinkDlg.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog to edit the <A> bookmark link name only
//
#include "stdafx.h"
#include "AuthorHTML.h"
#include "Dialogs\ALinkDlg.h"
#include "afxdialogex.h"

// ALinkDlg dialog

IMPLEMENT_DYNAMIC(ALinkDlg, CDialog)

ALinkDlg::ALinkDlg(CWnd* p_parent,CString& p_keyword)
         :CDialog(ALinkDlg::IDD, p_parent)
         ,m_keyword(p_keyword)
{
}

ALinkDlg::~ALinkDlg()
{
}

void ALinkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
  DDX_Text   (pDX,IDC_KEYWORD,  m_keyword);
  DDX_Control(pDX,IDHELP,       m_buttonHelp);
  DDX_Control(pDX,IDOK,         m_buttonOK);
  DDX_Control(pDX,IDCANCEL,     m_buttonCancel);
}

BEGIN_MESSAGE_MAP(ALinkDlg, CDialog)
  ON_EN_CHANGE (IDC_KEYWORD, &ALinkDlg::OnEnChangeKeyword)
  ON_BN_CLICKED(IDHELP,      &ALinkDlg::OnBnClickedHelp)
  ON_BN_CLICKED(IDOK,        &ALinkDlg::OnBnClickedOk)
  ON_BN_CLICKED(IDCANCEL,    &ALinkDlg::OnBnClickedCancel)
END_MESSAGE_MAP()

// ALinkDlg message handlers

void 
ALinkDlg::OnEnChangeKeyword()
{
  UpdateData();
}

void 
ALinkDlg::OnBnClickedHelp()
{
  // TODO: Add your control notification handler code here
}

void 
ALinkDlg::OnBnClickedOk()
{
  CDialog::OnOK();
}

void 
ALinkDlg::OnBnClickedCancel()
{
  CDialog::OnCancel();
}
