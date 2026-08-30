//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    OptionDlg.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for managing the <OPTION> tag in HTML
//
#include "stdafx.h"
#include "AuthorHTML.h"
#include "OptionDlg.h"
#include "afxdialogex.h"

// OptionDlg dialog

IMPLEMENT_DYNAMIC(OptionDlg, CDialog)

OptionDlg::OptionDlg(CWnd* p_parent,FormOption* p_option)
          :CDialog(OptionDlg::IDD, p_parent)
          ,m_option(p_option)
{
  m_display   = p_option->m_display;
  m_value     = p_option->m_value;
  m_selected  = p_option->m_selected;
}

OptionDlg::~OptionDlg()
{
}

void 
OptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
  DDX_Text   (pDX,IDC_DISPLAY, m_display);
  DDX_Text   (pDX,IDC_VALUE,   m_value);
  DDX_Control(pDX,IDC_SELECTED,m_checkSelected);
  DDX_Control(pDX,IDHELP,      m_buttonHelp);
  DDX_Control(pDX,IDOK,        m_buttonOK);
  DDX_Control(pDX,IDCANCEL,    m_buttonCancel);

  if(pDX->m_bSaveAndValidate == Data2Controls)
  {
    m_checkSelected.SetCheck(m_selected);
  }
}

BEGIN_MESSAGE_MAP(OptionDlg, CDialog)
  ON_EN_CHANGE(IDC_DISPLAY,   &OptionDlg::OnEnChangeDisplay)
  ON_EN_CHANGE(IDC_VALUE,     &OptionDlg::OnEnChangeValue)
  ON_BN_CLICKED(IDC_SELECTED, &OptionDlg::OnBnClickedSelected)
  ON_BN_CLICKED(IDHELP,       &OptionDlg::OnBnClickedHelp)
  ON_BN_CLICKED(IDOK,         &OptionDlg::OnBnClickedOk)
  ON_BN_CLICKED(IDCANCEL,     &OptionDlg::OnBnClickedCancel)
END_MESSAGE_MAP()

BOOL
OptionDlg::OnInitDialog()
{
  CDialog::OnInitDialog();
  UpdateData(Data2Controls);
  return FALSE;
}

// OptionDlg message handlers

void 
OptionDlg::OnEnChangeDisplay()
{
  UpdateData();
}


void 
OptionDlg::OnEnChangeValue()
{
  UpdateData();
}

void 
OptionDlg::OnBnClickedSelected()
{
  m_selected = (m_checkSelected.GetCheck() == TRUE);
}

void 
OptionDlg::OnBnClickedHelp()
{
  // TODO: Add your control notification handler code here
}

void 
OptionDlg::OnBnClickedOk()
{
  m_option->m_display  = m_display;
  m_option->m_value    = m_value;
  m_option->m_selected = m_selected;

  CDialog::OnOK();
}

void 
OptionDlg::OnBnClickedCancel()
{
  CDialog::OnCancel();
}
