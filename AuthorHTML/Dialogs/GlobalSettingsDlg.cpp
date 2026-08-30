//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    GlobalSettingsDlg.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for the global settings of the AuthorHTML program
//
#include "stdafx.h"
#include "AuthorHTML.h"
#include "GlobalSettingsDlg.h"
#include "GlobalSetPage1Dlg.h"
#include "Spelling.h"
#include "SpellOptionsDlg.h"

// GlobalSettingsDlg dialog

IMPLEMENT_DYNAMIC(GlobalSettingsDlg, CDialog)
GlobalSettingsDlg::GlobalSettingsDlg(CWnd* pParent,Spelling* speller)
                  :CDialog(GlobalSettingsDlg::IDD, pParent)
                  ,m_firstTab(0)
                  ,m_canApply(false)
{
  m_page1 = new GlobalSetPage1Dlg(this);
  m_page2 = new SpellOptionsDlg(this,speller,true,true);
}

GlobalSettingsDlg::~GlobalSettingsDlg()
{
  delete m_page1;
  delete m_page2;
}

void GlobalSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_TABCONTROL, m_tabControl);
  DDX_Control(pDX, IDOK,           m_buttonOK);
  DDX_Control(pDX, IDAPPLY,        m_buttonApply);
  DDX_Control(pDX, IDCANCEL,       m_buttonCancel);
  DDX_Control(pDX, IDHELP,         m_buttonHelp);
}


BEGIN_MESSAGE_MAP(GlobalSettingsDlg, CDialog)
  ON_NOTIFY     (TCN_SELCHANGE, IDC_TABCONTROL, OnTcnSelchangeTabcontrol)
  ON_BN_CLICKED(IDOK,     OnBnClickedOk)
  ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
  ON_BN_CLICKED(IDAPPLY,  OnBnClickedApply)
  ON_BN_CLICKED(IDHELP,   OnBnClickedHelp)
END_MESSAGE_MAP()

BOOL
GlobalSettingsDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  CRect rect1(0,0,100,100);
  m_page1->Create(IDD_SETTINGS_PAGE1,this);
  m_page2->Create(IDD_SPELL_OPTIONS,this);

  // Make page 2 a control-child
  m_page2->ModifyStyle(WS_POPUP|WS_SYSMENU|DS_MODALFRAME|WS_CAPTION|WS_BORDER
                      ,DS_SETFONT|DS_CONTROL|WS_CHILD
                      ,SWP_NOSIZE);
  m_page2->SetParent(this);

  // Hier kan de TAB-volgorde veranderd worden:
  CString s;
  m_page1->GetWindowText(s);
  m_tabControl.InsertItem(0,s);

  m_page2->GetWindowText(s);
  m_tabControl.InsertItem(1,s);

  CRect rect;
  m_tabControl.GetWindowRect(&rect);    // geeft t.o.v. ouder window inclusief titelbar
  ScreenToClient(&rect);
  rect.top    += 21;   // 22
  rect.bottom -= 2;
  rect.right  -= 2;
  rect.left   += 1;    // 6

  m_page1->MoveWindow(rect,true);    // zet tov client window.
  rect.top    -= 1;
  rect.bottom += 2;
  rect.right  += 2;
  rect.left   -= 1;
  m_page2->MoveWindow(rect,true);

  m_page1->ShowWindow(m_firstTab == 0 ? SW_SHOW : SW_HIDE);
  m_page2->ShowWindow(m_firstTab == 1 ? SW_SHOW : SW_HIDE);
  TabCtrl_SetCurSel(m_tabControl.GetSafeHwnd(),m_firstTab);

  UpdateData(Data2Controls);
  return TRUE;
}

void
GlobalSettingsDlg::FillPage()
{
}

void
GlobalSettingsDlg::UpdateProperties()
{
  m_page1->UpdateProperties();
  m_page2->UpdateProperties();
}

// GlobalSettingsDlg message handlers

/////////////////////////////////////////////////////////////////////////////
// CTablePropertiesDlg message handlers


void 
GlobalSettingsDlg::OnBnClickedOk()
{
  UpdateProperties();
  OnOK();
}

void 
GlobalSettingsDlg::OnBnClickedCancel()
{
  OnCancel();
}

void 
GlobalSettingsDlg::OnBnClickedApply()
{
  UpdateProperties();
}

void 
GlobalSettingsDlg::OnBnClickedHelp()
{
  // TODO: Add your control notification handler code here
}

void 
GlobalSettingsDlg::OnTcnSelchangeTabcontrol(NMHDR *pNMHDR, LRESULT *pResult)
{
  int num = TabCtrl_GetCurSel(pNMHDR->hwndFrom);
  switch(num)
  {
    case 0: m_page1->ShowWindow(TRUE);
            m_page2->ShowWindow(FALSE);
            break;
    case 1: m_page1->ShowWindow(FALSE);
            m_page2->ShowWindow(TRUE);
            break;
  }
  *pResult = 0;
}
