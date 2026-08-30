//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    FormLabelDlg.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for managing the <LABEL> tag in HTML, 
//
#include "stdafx.h"
#include "AuthorHTML.h"
#include "FormLabelDlg.h"

// FormLabelDlg dialog

IMPLEMENT_DYNAMIC(FormLabelDlg, CDialog)

FormLabelDlg::FormLabelDlg(CWnd* pParent,CString cfor)
             :CDialog(FormLabelDlg::IDD, pParent)
             ,m_for(cfor)
{
}

FormLabelDlg::~FormLabelDlg()
{
}

void FormLabelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
  DDX_Text(pDX,IDC_FL_FOR,      m_for);
  DDX_Control(pDX,IDOK,         m_buttonOK);
  DDX_Control(pDX,IDCANCEL,     m_buttonCancel);
  DDX_Control(pDX,IDC_FL_HELP,  m_buttonHelp);
}

BEGIN_MESSAGE_MAP(FormLabelDlg, CDialog)
  ON_EN_KILLFOCUS(IDC_FL_FOR, OnEnChangeFlFor)
  ON_BN_CLICKED  (IDOK,       OnBnClickedOk)
  ON_BN_CLICKED  (IDCANCEL,   OnBnClickedCancel)
  ON_BN_CLICKED  (IDC_FL_HELP,OnBnClickedFlHelp)
END_MESSAGE_MAP()

// FormLabelDlg message handlers

void FormLabelDlg::OnEnChangeFlFor()
{
  CWnd* w = GetDlgItem(IDC_FL_FOR);
  w->GetWindowText(m_for);
}

void FormLabelDlg::OnBnClickedOk()
{
  if(m_for.IsEmpty())
  {
    theApp.ErrorMessage("Fill in an edit field ID first");
    return;
  }
  OnOK();
}

void FormLabelDlg::OnBnClickedCancel()
{
  OnCancel();
}

void FormLabelDlg::OnBnClickedFlHelp()
{
  // TODO: Add your control notification handler code here
}
