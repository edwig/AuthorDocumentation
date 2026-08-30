//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    FormButtonDlg.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for managing the <BUTTON> tag in HTML, 
//
#include "stdafx.h"
#include "AuthorHTML.h"
#include "FormButtonDlg.h"
#include "GeneralIDDlg.h"
#include "StyleSheetDlg.h"
#include "TagEventsDlg.h"

// FormButtonDlg dialog

IMPLEMENT_DYNAMIC(FormButtonDlg, CDialog)
FormButtonDlg::FormButtonDlg(CWnd* pParent
                            ,CString p_base
                            ,CString type
                            ,CString name
                            ,CString value
                            ,bool checked
                            ,HtmlElement* elem)
	            :CDialog(FormButtonDlg::IDD, pParent)
              ,m_type(type)
              ,m_name(name)
              ,m_value(value)
              ,m_checked(checked)
              ,m_elem(elem)
              ,m_base(p_base)
{
}

FormButtonDlg::~FormButtonDlg()
{
}

void 
FormButtonDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
  DDX_Control(pDX,IDC_FB_TYPE,    m_comboType);
  DDX_Text   (pDX,IDC_FB_NAME,    m_name);
  DDX_Text   (pDX,IDC_FB_VALUE,   m_value);
  DDX_Control(pDX,IDC_FB_CHECKED, m_buttonChecked);
  DDX_Control(pDX,IDC_ID,         m_buttonID);
  DDX_Control(pDX,IDC_EVENTS,     m_buttonEvents);
  DDX_Control(pDX,IDC_STYLE,      m_buttonStyle);
  DDX_Control(pDX,IDOK,           m_buttonOK);
  DDX_Control(pDX,IDCANCEL,       m_buttonCancel);
  DDX_Control(pDX,IDC_FB_HELP,    m_buttonHelp);

  if(pDX->m_bSaveAndValidate == (BOOL)Data2Controls)
  {
    int ind = m_comboType.FindString(-1,m_type);
    m_comboType.SetCurSel(ind);

    bool check = true;
    bool name  = true;
    if(m_type.Compare("Button") == 0)
    {
      check = false;
    }
    else if(m_type.CompareNoCase("Reset")  == 0 || 
            m_type.CompareNoCase("Submit") == 0 )
    {
      check = false;
      name  = false;
    }
    m_buttonChecked.EnableWindow(check);
    CWnd* n = GetDlgItem(IDC_FB_NAME);
    n->EnableWindow(name);

    m_buttonChecked.SetCheck(m_checked);

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
      m_buttonStyle .EnableWindow(FALSE);
      m_buttonEvents.EnableWindow(FALSE);
      m_buttonID    .EnableWindow(FALSE);
    }
  }
}

BEGIN_MESSAGE_MAP(FormButtonDlg, CDialog)
  ON_CBN_SELCHANGE(IDC_FB_TYPE,   OnCbnSelchangeFbType)
  ON_EN_KILLFOCUS (IDC_FB_NAME,   OnEnChangeFbName)
  ON_EN_KILLFOCUS (IDC_FB_VALUE,  OnEnChangeFbValue)
  ON_BN_CLICKED   (IDC_FB_CHECKED,OnBnClickedFbChecked)
  ON_BN_CLICKED   (IDC_ID,        OnBnClickedId)
  ON_BN_CLICKED   (IDC_EVENTS,    OnBnClickedEvents)
  ON_BN_CLICKED   (IDC_STYLE,     OnBnClickedStyle)
  ON_BN_CLICKED   (IDOK,          OnBnClickedOk)
  ON_BN_CLICKED   (IDCANCEL,      OnBnClickedCancel)
  ON_BN_CLICKED   (IDC_FB_HELP,   OnBnClickedFbHelp)
END_MESSAGE_MAP()

BOOL
FormButtonDlg::OnInitDialog()
{
  CDialog::OnInitDialog();
  if(m_type.IsEmpty())
  {
    m_type = "Button";
  }
  else
  {
    m_comboType.EnableWindow(FALSE);
  }
  UpdateData(Data2Controls);
  return TRUE;
}

// FormButtonDlg message handlers

void 
FormButtonDlg::OnCbnSelchangeFbType()
{
  int ind = m_comboType.GetCurSel();
  if(ind >= 0)
  {
    m_comboType.GetLBText(ind,m_type);
  }
  UpdateData(Data2Controls);
}

void 
FormButtonDlg::OnEnChangeFbName()
{
  CWnd* w = GetDlgItem(IDC_FB_NAME);
  w->GetWindowText(m_name);
}

void 
FormButtonDlg::OnEnChangeFbValue()
{
  CWnd* w = GetDlgItem(IDC_FB_VALUE);
  w->GetWindowText(m_value);
}

void 
FormButtonDlg::OnBnClickedFbChecked()
{
  m_checked = (m_buttonChecked.GetCheck() == TRUE);
}

void 
FormButtonDlg::OnBnClickedOk()
{
  if(m_type.IsEmpty())
  {
    theApp.ErrorMessage("Type of button not yet set");
    return;
  }
  OnOK();
}

void 
FormButtonDlg::OnBnClickedCancel()
{
  OnCancel();
}

void 
FormButtonDlg::OnBnClickedId()
{
  GeneralIDDlg dlg(this,"input",m_elem);
  dlg.DoModal();
}

void 
FormButtonDlg::OnBnClickedEvents()
{
  TagEventsDlg dlg(this,m_elem,"INPUT");
  dlg.DoModal();
  UpdateData(Data2Controls);
}

void 
FormButtonDlg::OnBnClickedStyle()
{
  if(!m_elem->HasStyle())
  {
    m_elem->SetStyle();
  }
  CString style = m_elem->GetInlineStyle();
  style = CString("input { ") + style + "}";
  StyleSheetDlg dlg(this,m_base,"input",NULL,style);
  if(dlg.DoModal() == IDOK)
  {
    style = dlg.GetInlineStylesheet();
    style.TrimRight("}");
    style.TrimLeft("input {");
    m_elem->SetInlineStyle(style);
  }
}

void 
FormButtonDlg::OnBnClickedFbHelp()
{
  // TODO: Add your control notification handler code here
}

