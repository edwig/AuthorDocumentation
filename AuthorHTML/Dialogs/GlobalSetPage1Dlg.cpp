//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    GlobalSetPage1Dlg.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for the 1th global settings page
//
#include "stdafx.h"
#include "AuthorHTML.h"
#include "GlobalSetPage1Dlg.h"

// GlobalSetPage1Dlg dialog

IMPLEMENT_DYNAMIC(GlobalSetPage1Dlg, CDialog)
GlobalSetPage1Dlg::GlobalSetPage1Dlg(CWnd* pParent /*=NULL*/)
	                :CDialog(GlobalSetPage1Dlg::IDD, pParent)
{
}

GlobalSetPage1Dlg::~GlobalSetPage1Dlg()
{
}

void GlobalSetPage1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
  DDX_Control(pDX,IDC_EDIT_LINK,    m_buttonNavigate);
  DDX_Control(pDX,IDC_DCLICK_DIALOG,m_buttonShowIt);

  if(pDX->m_bSaveAndValidate == Data2Controls)
  {
    m_buttonNavigate.SetCheck(m_defNavigate == TRUE);
    m_buttonShowIt  .SetCheck(m_defShowIt   == TRUE);
  }
}

BEGIN_MESSAGE_MAP(GlobalSetPage1Dlg, CDialog)
  ON_BN_CLICKED(IDC_EDIT_LINK,     OnBnClickedEditLink)
  ON_BN_CLICKED(IDC_DCLICK_DIALOG, OnBnClickedDclickDialog)
END_MESSAGE_MAP()

BOOL
GlobalSetPage1Dlg::OnInitDialog()
{
  CDialog::OnInitDialog();
  FillPage();
  UpdateData(Data2Controls);
  return TRUE;
}

void
GlobalSetPage1Dlg::FillPage()
{
  m_defNavigate = (settings.GetSetting("NavigateDCHyperlink","yes")   == "yes");
  m_defShowIt   = (settings.GetSetting("ShowNavigateHyperlink","yes") == "yes");
}

void
GlobalSetPage1Dlg::UpdateProperties()
{
  settings.SetSetting("NavigateDCHyperlink",m_defNavigate ? "yes" : "no");
  settings.SetSetting("ShowNavigateHyperlink",m_defShowIt ? "yes" : "no");
}

// GlobalSetPage1Dlg message handlers

void 
GlobalSetPage1Dlg::OnBnClickedEditLink()
{
  m_defNavigate = !m_defNavigate;
}

void 
GlobalSetPage1Dlg::OnBnClickedDclickDialog()
{
  m_defShowIt = !m_defShowIt;
}
