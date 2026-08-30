//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    DivTextDlg.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for editing <DIV> text properties
//
#include "stdafx.h"
#include "AuthorHTML.h"
#include "DivTextDlg.h"
#include "StyleSheetDlg.h"
#include "GeneralIDDlg.h"
#include "TagEventsDlg.h"

// DivTextDlg dialog

IMPLEMENT_DYNAMIC(DivTextDlg, CDialog)

DivTextDlg::DivTextDlg(CWnd* pParent
                      ,CString p_base
                      ,CString tag
                      ,CString title
                      ,CString text
                      ,HtmlElement* elem)
	         :CDialog(DivTextDlg::IDD, pParent)
           ,m_tag(tag)
           ,m_title(title)
           ,m_text(text)
           ,m_elem(elem)
           ,m_base(p_base)
{
}

DivTextDlg::~DivTextDlg()
{
}

void DivTextDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
  DDX_Text   (pDX,IDC_TEXT_TITLE,m_title);
  DDX_Text   (pDX,IDC_TEXT_TEXT, m_text);
  DDX_Control(pDX,IDC_ID,        m_buttonID);
  DDX_Control(pDX,IDC_EVENTS,    m_buttonEvents);
  DDX_Control(pDX,IDC_STYLE,     m_buttonStyle);
  DDX_Control(pDX,IDOK,          m_buttonOK);
  DDX_Control(pDX,IDCANCEL,      m_buttonCancel);
  DDX_Control(pDX,IDC_TEXT_HELP, m_buttonHelp);

  if(pDX->m_bSaveAndValidate == Data2Controls)
  {
    if(m_elem)
    {
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
    else
    {
      // Newly defined tag. No style possible yet.
      m_buttonStyle.EnableWindow(FALSE);
      m_buttonID   .EnableWindow(FALSE);
    }
  }
}

BEGIN_MESSAGE_MAP(DivTextDlg, CDialog)
  ON_EN_KILLFOCUS (IDC_TEXT_TITLE,OnEnChangeTextTitle)
  ON_EN_KILLFOCUS (IDC_TEXT_TEXT, OnEnChangeTextText)
  ON_BN_CLICKED   (IDC_ID,        OnBnClickedId)
  ON_BN_CLICKED   (IDC_EVENTS,    OnBnClickedEvents)
  ON_BN_CLICKED   (IDC_STYLE,     OnBnClickedStyle)
  ON_BN_CLICKED   (IDOK,          OnBnClickedOk)
  ON_BN_CLICKED   (IDCANCEL,      OnBnClickedCancel)
  ON_BN_CLICKED   (IDC_TEXT_HELP, OnBnClickedTextHelp)
END_MESSAGE_MAP()

BOOL
DivTextDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  if(m_tag.CompareNoCase("abbr") == 0)
  {
    SetWindowText("Abbreviation");
  }
  else if(m_tag.CompareNoCase("acronym") == 0)
  {
    SetWindowText("Acronym");
  }
  else if(m_tag.CompareNoCase("address") == 0)
  {
    SetWindowText("Address");
    CWnd* w = GetDlgItem(IDC_TEXT_TITLE);
    w->EnableWindow(FALSE);
  }
  return TRUE;
}

// DivTextDlg message handlers

void DivTextDlg::OnEnChangeTextTitle()
{
  CWnd* w = GetDlgItem(IDC_TEXT_TITLE);
  w->GetWindowText(m_title);
}

void DivTextDlg::OnEnChangeTextText()
{
  CWnd* w = GetDlgItem(IDC_TEXT_TEXT);
  w->GetWindowText(m_text);
}

void DivTextDlg::OnBnClickedOk()
{
  OnOK();
}

void DivTextDlg::OnBnClickedCancel()
{
  OnCancel();
}

void DivTextDlg::OnBnClickedId()
{
  GeneralIDDlg dlg(this,m_tag,m_elem);
  dlg.DoModal();
}

void 
DivTextDlg::OnBnClickedEvents()
{
  TagEventsDlg dlg(this,m_elem,m_tag);
  dlg.DoModal();
  UpdateData(Data2Controls);
}

void DivTextDlg::OnBnClickedStyle()
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

void DivTextDlg::OnBnClickedTextHelp()
{
  // TODO: Add your control notification handler code here
}


