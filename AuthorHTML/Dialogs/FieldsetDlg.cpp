//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    FieldsetDlg.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for editing <FIELDSET> properties
//
#include "stdafx.h"
#include "AuthorHTML.h"
#include "FieldsetDlg.h"
#include "GeneralIDDlg.h"
#include "TagEventsDlg.h"
#include "StyleSheetDlg.h"

// FieldsetDlg dialog

IMPLEMENT_DYNAMIC(FieldsetDlg, CDialog)

FieldsetDlg::FieldsetDlg(CWnd* pParent
                        ,CString base
                        ,HtmlFieldset* set)
            :CDialog(FieldsetDlg::IDD, pParent)
            ,m_base(base)
            ,m_fieldset(set)
{
}

FieldsetDlg::~FieldsetDlg()
{
}

void FieldsetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
  DDX_Text   (pDX,IDC_LEGEND,   m_legend);
  DDX_Control(pDX,IDC_FS_ALIGN, m_comboAlignFS);
  DDX_Control(pDX,IDC_LE_ALIGN, m_comboAlignLE);
  DDX_Control(pDX,IDC_ID,       m_buttonID);
  DDX_Control(pDX,IDC_EVENTS,   m_buttonEvents);
  DDX_Control(pDX,IDC_STYLE,    m_buttonStyle);
  DDX_Control(pDX,IDOK,         m_buttonOK);
  DDX_Control(pDX,IDCANCEL,     m_buttonCancel);
  DDX_Control(pDX,IDC_FIELDSET_HELP, m_buttonHelp);

  if(pDX->m_bSaveAndValidate == Data2Controls)
  {
    CString def,text;

    m_buttonID.GetWindowText(def);
    text = m_fieldset->HasIdentity() ? "[ &ID ]" : "&ID";
    if(def != text) m_buttonID.SetWindowText(text);

    m_buttonStyle.GetWindowText(def);
    text = m_fieldset->HasStyle() ? "[ &Style ]" : "&Style";
    if(def != text) m_buttonStyle.SetWindowText(text);

    m_buttonEvents.GetWindowText(def);
    text = m_fieldset->HasEvents() ? "[ &Events ]" : "&Events";
    if(def != text) m_buttonEvents.SetWindowText(text);

    int ind;
    ind = m_comboAlignFS.FindString(-1,m_alignFieldset);
          m_comboAlignFS.SetCurSel(ind);
    ind = m_comboAlignLE.FindString(-1,m_alignLegend);
          m_comboAlignLE.SetCurSel(ind);
  }
}

BEGIN_MESSAGE_MAP(FieldsetDlg, CDialog)
  ON_EN_KILLFOCUS (IDC_LEGEND,        OnEnChangeLegend)
  ON_CBN_SELCHANGE(IDC_FS_ALIGN,      OnCbnSelchangeFsAlign)
  ON_CBN_SELCHANGE(IDC_LE_ALIGN,      OnCbnSelchangeLeAlign)
  ON_BN_CLICKED   (IDC_ID,            OnBnClickedId)
  ON_BN_CLICKED   (IDC_EVENTS,        OnBnClickedEvents)
  ON_BN_CLICKED   (IDC_STYLE,         OnBnClickedStyle)
  ON_BN_CLICKED   (IDOK,              OnBnClickedOk)
  ON_BN_CLICKED   (IDCANCEL,          OnBnClickedCancel)
  ON_BN_CLICKED   (IDC_FIELDSET_HELP, OnBnClickedFieldsetHelp)
END_MESSAGE_MAP()

BOOL
FieldsetDlg::OnInitDialog()
{
  CDialog::OnInitDialog();
  FillPage();
  UpdateData(Data2Controls);
  return TRUE;
}

void
FieldsetDlg::FillPage()
{
  m_legend        = m_fieldset->GetProperty(HtmlFieldset::E_Legend);
  m_alignFieldset = m_fieldset->GetProperty(HtmlFieldset::E_Align);
  m_alignLegend   = m_fieldset->GetProperty(HtmlFieldset::E_LegendAlign);
}

void
FieldsetDlg::UpdateProperties()
{
  m_fieldset->SetProperty(HtmlFieldset::E_Legend,     m_legend);
  m_fieldset->SetProperty(HtmlFieldset::E_Align,      m_alignFieldset);
  m_fieldset->SetProperty(HtmlFieldset::E_LegendAlign,m_alignLegend);
}

// FieldsetDlg message handlers

void 
FieldsetDlg::OnEnChangeLegend()
{
  CWnd* w = GetDlgItem(IDC_LEGEND);
  w->GetWindowText(m_legend);
}

void 
FieldsetDlg::OnCbnSelchangeFsAlign()
{
  int ind = m_comboAlignFS.GetCurSel();
  if(ind >= 0)
  {
    m_comboAlignFS.GetLBText(ind,m_alignFieldset);
  }
}

void 
FieldsetDlg::OnCbnSelchangeLeAlign()
{
  int ind = m_comboAlignLE.GetCurSel();
  if(ind >= 0)
  {
    m_comboAlignLE.GetLBText(ind,m_alignLegend);
  }
}

void 
FieldsetDlg::OnBnClickedId()
{
  HtmlElement* elem = (HtmlElement*) m_fieldset;
  GeneralIDDlg dlg(this,"fieldset",elem);
  dlg.DoModal();
}

void 
FieldsetDlg::OnBnClickedEvents()
{
  HtmlElement* elem = (HtmlElement*) m_fieldset;
  TagEventsDlg dlg(this,elem,"fieldset");
  dlg.DoModal();
  UpdateData(Data2Controls);
}

void 
FieldsetDlg::OnBnClickedStyle()
{
  if(!m_fieldset->HasStyle())
  {
    m_fieldset->SetStyle();
  }
  CString style = m_fieldset->GetInlineStyle();
  style = CString("fieldset { ") + style + "}";
  StyleSheetDlg dlg(this,m_base,"fieldset",NULL,style);
  if(dlg.DoModal() == IDOK)
  {
    style = dlg.GetInlineStylesheet();
    style.TrimRight("}");
    style.TrimLeft("fieldset");
    style.TrimLeft(" {");
    m_fieldset->SetInlineStyle(style);
  }
}

void 
FieldsetDlg::OnBnClickedOk()
{
  UpdateProperties();
  OnOK();
}

void 
FieldsetDlg::OnBnClickedCancel()
{
  OnCancel();
}

void 
FieldsetDlg::OnBnClickedFieldsetHelp()
{
}
