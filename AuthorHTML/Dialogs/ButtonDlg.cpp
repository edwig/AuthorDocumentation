//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    ButtonDlg.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog to edit the <BUTTON> tag to be able to make buttons in a form
//
#include "stdafx.h"
#include "AuthorHTML.h"
#include "ButtonDlg.h"
#include "GeneralIDDlg.h"
#include "StyleSheetDlg.h"
#include "TagEventsDlg.h"

// ButtonDlg dialog

IMPLEMENT_DYNAMIC(ButtonDlg, CDialog)

ButtonDlg::ButtonDlg(CWnd* pParent
                    ,CString      base
                    ,HtmlElement* elem)
          :CDialog(ButtonDlg::IDD, pParent)
          ,m_elem(elem)
          ,m_base(base)
{
}

ButtonDlg::~ButtonDlg()
{
}

void ButtonDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
  DDX_Text   (pDX,IDC_BUTTON_NAME,  m_name);
  DDX_Text   (pDX,IDC_BUTTON_VALUE, m_value);
  DDX_Control(pDX,IDC_BUTTON_TYPE,  m_comboType);
  DDX_Control(pDX,IDC_ID,           m_buttonID);
  DDX_Control(pDX,IDC_EVENTS,       m_buttonEvents);
  DDX_Control(pDX,IDC_STYLE,        m_buttonStyle);
  DDX_Control(pDX,IDOK,             m_buttonOK);
  DDX_Control(pDX,IDCANCEL,         m_buttonCancel);
  DDX_Control(pDX,IDC_BUTTON_HELP,  m_buttonHelp);
  
  if(pDX->m_bSaveAndValidate == Data2Controls)
  {
    int ind = m_comboType.FindString(-1,m_type);
    m_comboType.SetCurSel(ind);

    CString def,text;

    m_buttonID.GetWindowText(def);
    text = m_elem->HasIdentity() ? "[ &ID ]" : "&ID";
    if(def != text) m_buttonID.SetWindowText(text);

    m_buttonStyle.GetWindowText(def);
    text = m_elem->HasStyle() ? "[ &Style ]" : "&Style";
    if(def != text) m_buttonStyle.SetWindowText(text);

    m_buttonEvents.GetWindowText(def);
    text = m_elem->HasEvents() ? "[ &Events ]" : "&Events";
    if(def != text) m_buttonEvents.SetWindowText(text);
  }
}

BEGIN_MESSAGE_MAP(ButtonDlg, CDialog)
  ON_EN_KILLFOCUS (IDC_BUTTON_NAME, OnEnChangeButtonName)
  ON_EN_KILLFOCUS (IDC_BUTTON_VALUE,OnEnChangeButtonValue)
  ON_CBN_SELCHANGE(IDC_BUTTON_TYPE, OnCbnSelchangeButtonType)
  ON_BN_CLICKED   (IDC_ID,          OnBnClickedId)
  ON_BN_CLICKED   (IDC_EVENTS,      OnBnClickedEvents)
  ON_BN_CLICKED   (IDC_STYLE,       OnBnClickedStyle)
  ON_BN_CLICKED   (IDOK,            OnBnClickedOk)
  ON_BN_CLICKED   (IDCANCEL,        OnBnClickedCancel)
  ON_BN_CLICKED   (IDC_BUTTON_HELP, OnBnClickedButtonHelp)
END_MESSAGE_MAP()

BOOL
ButtonDlg::OnInitDialog()
{
  CDialog::OnInitDialog();
  FillPage();
  UpdateData(Data2Controls);
  return TRUE;
}

void
ButtonDlg::FillPage()
{
  m_name  = m_elem->GetAttribute("name");
  m_value = m_elem->GetAttribute("value");
  m_type  = m_elem->GetAttribute("type");
}

void
ButtonDlg::UpdateProperties()
{
  m_elem->SetAttribute("name", m_name);
  m_elem->SetAttribute("value",m_value);
  m_elem->SetAttribute("type" ,m_type);
}

// ButtonDlg message handlers

void 
ButtonDlg::OnEnChangeButtonName()
{
  CWnd* w = GetDlgItem(IDC_BUTTON_NAME);
  w->GetWindowText(m_name);
}

void 
ButtonDlg::OnEnChangeButtonValue()
{
  CWnd* w = GetDlgItem(IDC_BUTTON_VALUE);
  w->GetWindowText(m_value);
}

void 
ButtonDlg::OnCbnSelchangeButtonType()
{
  int ind = m_comboType.GetCurSel();
  if(ind >= 0)
  {
    m_comboType.GetLBText(ind,m_type);
  }
}

void 
ButtonDlg::OnBnClickedId()
{
  GeneralIDDlg dlg(this,"button",m_elem);
  dlg.DoModal();
}

void 
ButtonDlg::OnBnClickedEvents()
{
  TagEventsDlg dlg(this,m_elem,"BUTTON");
  dlg.DoModal();
  UpdateData(Data2Controls);
}

void 
ButtonDlg::OnBnClickedStyle()
{
  if(!m_elem->HasStyle())
  {
    m_elem->SetStyle();
  }
  UpdateProperties();
  CString style = m_elem->GetInlineStyle();
  style = CString("button { ") + style + "}";
  StyleSheetDlg dlg(this,m_base,"button",NULL,style);
  if(dlg.DoModal() == IDOK)
  {
    style = dlg.GetInlineStylesheet();
    style.TrimRight("}");
    style.TrimLeft("button {");
    m_elem->SetInlineStyle(style);
    FillPage();
  }
  UpdateData(Data2Controls);
}

void 
ButtonDlg::OnBnClickedOk()
{
  UpdateProperties();
  OnOK();
}

void 
ButtonDlg::OnBnClickedCancel()
{
  OnCancel();
}

void 
ButtonDlg::OnBnClickedButtonHelp()
{
}

