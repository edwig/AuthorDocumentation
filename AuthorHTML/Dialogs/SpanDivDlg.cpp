//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    SnapDivDlg.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for managing Snap-to-DIV settings
//
#include "stdafx.h"
#include "AuthorHTML.h"
#include "SpanDivDlg.h"
#include "Misc.h"
#include "GeneralIDDlg.h"
#include "TagEventsDlg.h"
#include "StyleSheetDlg.h"

// SpanDivDlg dialog

IMPLEMENT_DYNAMIC(SpanDivDlg, CDialog)

SpanDivDlg::SpanDivDlg(CWnd*        pParent
                      ,CString      p_base
                      ,CString      p_tag
                      ,HtmlElement* p_elem)
           :CDialog(SpanDivDlg::IDD, pParent)
           ,m_tag(p_tag)
           ,m_elem(p_elem)
           ,m_base(p_base)
{
  CString desc = Misc::GetTagDescription(m_tag);
  p_tag.MakeUpper();
  m_type = p_tag + " : " + desc;
}

SpanDivDlg::~SpanDivDlg()
{
}

void SpanDivDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
  DDX_Text  (pDX, IDC_TYPE,   m_type);
  DDX_Control(pDX,IDC_ID,     m_buttonID);
  DDX_Control(pDX,IDC_EVENTS, m_buttonEvents);
  DDX_Control(pDX,IDC_STYLE,  m_buttonStyle);
  DDX_Control(pDX,IDOK,       m_buttonOK);
  DDX_Control(pDX,IDCANCEL,   m_buttonCancel);
  DDX_Control(pDX,IDC_SD_HELP,m_buttonHelp);

  if(pDX->m_bSaveAndValidate == Data2Controls)
  {
    m_buttonID    .EnableWindow(m_elem != NULL);
    m_buttonEvents.EnableWindow(m_elem != NULL);
    m_buttonStyle .EnableWindow(m_elem != NULL);

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

BEGIN_MESSAGE_MAP(SpanDivDlg, CDialog)
  ON_BN_CLICKED(IDC_ID,     OnBnClickedId)
  ON_BN_CLICKED(IDC_EVENTS, OnBnClickedEvents)
  ON_BN_CLICKED(IDC_STYLE,  OnBnClickedStyle)
  ON_BN_CLICKED(IDOK,       OnBnClickedOk)
  ON_BN_CLICKED(IDCANCEL,   OnBnClickedCancel)
  ON_BN_CLICKED(IDC_SD_HELP,OnBnClickedSdHelp)
END_MESSAGE_MAP()

// SpanDivDlg message handlers

void 
SpanDivDlg::OnBnClickedId()
{
  GeneralIDDlg dlg(this,m_tag,m_elem);
  dlg.DoModal();
}

void SpanDivDlg::OnBnClickedEvents()
{
  TagEventsDlg dlg(this,m_elem,m_tag);
  dlg.DoModal();
  UpdateData(Data2Controls);
}

void 
SpanDivDlg::OnBnClickedStyle()
{
  if(!m_elem->HasStyle())
  {
    m_elem->SetStyle();
  }
  CString style = m_elem->GetInlineStyle();
  style = m_tag + " { " + style + "}";
  StyleSheetDlg dlg(this,m_base,m_tag,NULL,style);
  if(dlg.DoModal() == IDOK)
  {
    style = dlg.GetInlineStylesheet();
    style.TrimRight("}");
    style.TrimLeft(m_tag);
    style.TrimLeft(" {");
    m_elem->SetInlineStyle(style);
  }
}

void 
SpanDivDlg::OnBnClickedOk()
{
  OnOK();
}

void 
SpanDivDlg::OnBnClickedCancel()
{
  OnCancel();
}

void SpanDivDlg::OnBnClickedSdHelp()
{
  // TODO: Add your control notification handler code here
}

