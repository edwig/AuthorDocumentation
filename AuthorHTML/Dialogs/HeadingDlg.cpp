//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    HeadingDlg.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for headings (H1..H6) in HTML
//
#include "stdafx.h"
#include "AuthorHTML.h"
#include "HeadingDlg.h"
#include "GeneralIDDlg.h"
#include "StyleSheetDlg.h"
#include "TagEventsDlg.h"

// HeadingDlg dialog

IMPLEMENT_DYNAMIC(HeadingDlg, CDialog)

HeadingDlg::HeadingDlg(CWnd* pParent
                      ,CString p_base
                      ,CString tag
                      ,HtmlHeading* head)
           :CDialog(HeadingDlg::IDD, pParent)
           ,m_head(head)
           ,m_tag(tag)
           ,m_base(p_base)
{
  CString desc = Misc::GetTagDescription(tag);
  tag.MakeUpper();
  m_type = tag + " : " + desc;
}

HeadingDlg::~HeadingDlg()
{
}

void 
HeadingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
  DDX_Text   (pDX,IDC_TYPE,     m_type);
  DDX_Control(pDX,IDC_ALIGNMENT,m_comboAlign);
  DDX_Control(pDX,IDC_ID,       m_buttonID);
  DDX_Control(pDX,IDC_EVENTS,   m_buttonEvents);
  DDX_Control(pDX,IDC_STYLE,    m_buttonStyle);
  DDX_Control(pDX,IDOK,         m_buttonOK);
  DDX_Control(pDX,IDC_HEAD_HELP,m_buttonHelp);

  if(pDX->m_bSaveAndValidate == Data2Controls)
  {
    int ind = m_comboAlign.FindString(-1,m_align);
    m_comboAlign.SetCurSel(ind);

    CString def,text;

    m_buttonID.GetWindowText(def);
    text = m_head->HasIdentity() ? "[ &ID ]" : "&ID";
    if(def != text) m_buttonID.SetWindowText(text);

    m_buttonStyle.GetWindowText(def);
    text = m_head->HasStyle() ? "[ &Style ]" : "&Style";
    if(def != text) m_buttonStyle.SetWindowText(text);

    m_buttonEvents.GetWindowText(def);
    text = m_head->HasEvents() ? "[ &Events ]" : "&Events";
    if(def != text) m_buttonEvents.SetWindowText(text);
  }
}

BEGIN_MESSAGE_MAP(HeadingDlg, CDialog)
  ON_CBN_SELCHANGE(IDC_ALIGNMENT, OnCbnSelchangeAlignment)
  ON_BN_CLICKED   (IDC_ID,        OnBnClickedId)
  ON_BN_CLICKED   (IDC_EVENTS,    OnBnClickedEvents)
  ON_BN_CLICKED   (IDC_STYLE,     OnBnClickedStyle)
  ON_BN_CLICKED   (IDOK,          OnBnClickedOk)
  ON_BN_CLICKED   (ID_HELP,       OnBnClickedHelp)
END_MESSAGE_MAP()

BOOL
HeadingDlg::OnInitDialog()
{
  CDialog::OnInitDialog();
  FillPage();
  UpdateData(Data2Controls);
  return TRUE;
}

void
HeadingDlg::FillPage()
{
  m_align = m_head->GetProperty(HtmlHeading::E_Align);
}

void
HeadingDlg::UpdatePropterties()
{
  m_head->SetProperty(HtmlHeading::E_Align,m_align);
}

// HeadingDlg message handlers

void 
HeadingDlg::OnCbnSelchangeAlignment()
{
  int ind = m_comboAlign.GetCurSel();
  if(ind >= 0)
  {
    m_comboAlign.GetLBText(ind,m_align);
  }
}

void 
HeadingDlg::OnBnClickedId()
{
  HtmlElement* elem = (HtmlElement*) m_head;
  GeneralIDDlg dlg(this,m_tag,elem);
  dlg.DoModal();
}

void 
HeadingDlg::OnBnClickedEvents()
{
  TagEventsDlg dlg(this,m_head,m_tag);
  dlg.DoModal();
  UpdateData(Data2Controls);
}

void 
HeadingDlg::OnBnClickedStyle()
{
  if(!m_head->HasStyle())
  {
    m_head->SetStyle();
  }
  CString style = m_head->GetInlineStyle();
  style = m_tag + " { " + style + "}";
  StyleSheetDlg dlg(this,m_base,m_tag,NULL,style);
  if(dlg.DoModal() == IDOK)
  {
    style = dlg.GetInlineStylesheet();
    style.TrimRight("}");
    style.TrimLeft(m_tag);
    style.TrimLeft(" {");
    m_head->SetInlineStyle(style);
  }
}

void 
HeadingDlg::OnBnClickedOk()
{
  UpdatePropterties();
  EndDialog(IDOK);
}

void 
HeadingDlg::OnBnClickedHelp()
{
  // TODO: Add your control notification handler code here
}

