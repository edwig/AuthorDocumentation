//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    TableCaptionDlg.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for managing the <CAPTION> tag in HTML
//
#include "stdafx.h"
#include "AuthorHTML.h"
#include "TableCaptionDlg.h"
#include "GeneralIDDlg.h"
#include "StyleSheetDlg.h"
#include "TagEventsDlg.h"

// TableCaptionDlg dialog
IMPLEMENT_DYNAMIC(TableCaptionDlg, CDialog)

TableCaptionDlg::TableCaptionDlg(CWnd*        pParent
                                ,CString      base
                                ,HtmlCaption* caption)
               :CDialog(TableCaptionDlg::IDD, pParent)
               ,m_caption(caption)
               ,m_base(base)
{
}

TableCaptionDlg::~TableCaptionDlg()
{
}

void 
TableCaptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
  DDX_Control(pDX,IDC_POSITION,  m_comboVAlign);
  DDX_Control(pDX,IDC_ALIGNMENT, m_comboAlign);
  DDX_Control(pDX,IDC_ID,        m_buttonID);
  DDX_Control(pDX,IDC_EVENTS,    m_buttonEvents);
  DDX_Control(pDX,IDC_STYLE,     m_buttonStyle);
  DDX_Control(pDX,IDOK,          m_buttonOK);
  DDX_Control(pDX,IDCANCEL,      m_buttonCancel);
  DDX_Control(pDX,IDC_HEAD_HELP, m_buttonHelp);

  if(pDX->m_bSaveAndValidate == Data2Controls)
  {
    int ind;
    ind = m_comboVAlign.FindString(-1,m_valign);
          m_comboVAlign.SetCurSel(ind);
    ind = m_comboAlign.FindString(-1,m_align);
          m_comboAlign.SetCurSel(ind);

    CString def,text;

    m_buttonID.GetWindowText(def);
    text = m_caption->HasIdentity() ? "[ &ID ]" : "&ID";
    if(def != text) m_buttonID.SetWindowText(text);

    m_buttonStyle.GetWindowText(def);
    text = m_caption->HasStyle() ? "[ &Style ]" : "&Style";
    if(def != text) m_buttonStyle.SetWindowText(text);

    m_buttonEvents.GetWindowText(def);
    text = m_caption->HasEvents() ? "[ &Events ]" : "&Events";
    if(def != text) m_buttonEvents.SetWindowText(text);
  }
}

BEGIN_MESSAGE_MAP(TableCaptionDlg, CDialog)
  ON_CBN_SELCHANGE(IDC_POSITION,  OnCbnSelchangePosition)
  ON_CBN_SELCHANGE(IDC_ALIGNMENT, OnCbnSelchangeAlignment)
  ON_BN_CLICKED   (IDC_ID,        OnBnClickedId)
  ON_BN_CLICKED   (IDC_EVENTS,    OnBnClickedEvents)
  ON_BN_CLICKED   (IDC_STYLE,     OnBnClickedStyle)
  ON_BN_CLICKED   (IDOK,          OnBnClickedOk)
  ON_BN_CLICKED   (IDCANCEL,      OnBnClickedCancel)
  ON_BN_CLICKED   (IDC_HEAD_HELP, OnBnClickedHeadHelp)
END_MESSAGE_MAP()

BOOL
TableCaptionDlg::OnInitDialog()
{
  CDialog::OnInitDialog();
  FillPage();
  UpdateData(Data2Controls);
  return TRUE;
}

void
TableCaptionDlg::FillPage()
{
  m_valign = m_caption->GetProperty(HtmlCaption::E_Valign);
  m_align  = m_caption->GetProperty(HtmlCaption::E_Align);
}

void
TableCaptionDlg::UpdateProperties()
{
  m_caption->SetProperty(HtmlCaption::E_Valign,m_valign);
  m_caption->SetProperty(HtmlCaption::E_Align, m_align);
}

// TableCaptionDlg message handlers

void 
TableCaptionDlg::OnCbnSelchangePosition()
{
  int ind = m_comboVAlign.GetCurSel();
  if(ind >= 0)
  {
    m_comboVAlign.GetLBText(ind,m_valign);
  }
}

void 
TableCaptionDlg::OnCbnSelchangeAlignment()
{
  int ind = m_comboAlign.GetCurSel();
  if(ind >= 0)
  {
    m_comboAlign.GetLBText(ind,m_align);
  }
}

void 
TableCaptionDlg::OnBnClickedId()
{
  HtmlElement* elem = (HtmlElement*)(m_caption);
  GeneralIDDlg dlg(this,"caption",elem);
  dlg.DoModal();
}

void 
TableCaptionDlg::OnBnClickedEvents()
{
  TagEventsDlg dlg(this,m_caption,"caption");
  dlg.DoModal();
  UpdateData(Data2Controls);
}

void 
TableCaptionDlg::OnBnClickedStyle()
{
  if(!m_caption->HasStyle())
  {
    m_caption->SetStyle();
  }
  UpdateProperties();
  CString style = m_caption->GetInlineStyle();
  style = CString("caption { ") + style + "}";
  StyleSheetDlg dlg(this,m_base,"caption",NULL,style);
  if(dlg.DoModal() == IDOK)
  {
    style = dlg.GetInlineStylesheet();
    style.TrimRight("}");
    style.TrimLeft("caption {");
    m_caption->SetInlineStyle(style);
    FillPage();
    UpdateData(Data2Controls);
  }
}

void 
TableCaptionDlg::OnBnClickedOk()
{
  UpdateProperties();
  OnOK();
}

void 
TableCaptionDlg::OnBnClickedCancel()
{
  OnCancel();
}

void 
TableCaptionDlg::OnBnClickedHeadHelp()
{
  // TODO: Add your control notification handler code here
}

