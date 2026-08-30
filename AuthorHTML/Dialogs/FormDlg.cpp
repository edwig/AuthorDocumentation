//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    Form.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for managing the <FORM> tag in HTML, 
//
#include "stdafx.h"
#include "AuthorHTML.h"
#include "FormDlg.h"
#include "GeneralIDDlg.h"
#include "StyleSheetDlg.h"
#include "TagEventsDlg.h"

// FormDlg dialog

IMPLEMENT_DYNAMIC(FormDlg, CDialog)

FormDlg::	FormDlg(CWnd*   pParent
                 ,CString p_base
                 ,CString name
                 ,CString action
                 ,CString method
                 ,CString encoding
                 ,CString target
                 ,HtmlElement* elem)
	      :CDialog(FormDlg::IDD, pParent)
        ,m_name(name)
        ,m_actionUrl(action)
        ,m_method(method)
        ,m_encoding(encoding)
        ,m_target(target)
        ,m_elem(elem)
        ,m_base(p_base)
{
}

FormDlg::~FormDlg()
{
}

void FormDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
  DDX_Text   (pDX,IDC_FORM_NAME,     m_name);
  DDX_Text   (pDX,IDC_FORM_ACTION,   m_actionUrl);
  DDX_Control(pDX,IDC_FORM_METHOD,   m_comboMethod);
  DDX_Control(pDX,IDC_FORM_ENCODING, m_comboEncoding);
  DDX_Control(pDX,IDC_FORM_TARGET,   m_comboTarget);
  DDX_Control(pDX,IDC_ID,            m_buttonID);
  DDX_Control(pDX,IDC_EVENTS,        m_buttonEvents);
  DDX_Control(pDX,IDC_STYLE,         m_buttonStyle);
  DDX_Control(pDX,IDOK,              m_buttonOK);
  DDX_Control(pDX,IDCANCEL,          m_buttonCancel);
  DDX_Control(pDX,IDC_FORM_HELP,     m_buttonHelp);

  if(pDX->m_bSaveAndValidate == Data2Controls)
  {
    int ind;
    ind = m_comboMethod.FindString(-1,m_method);
          m_comboMethod.SetCurSel(ind);
    ind = m_comboEncoding.FindString(-1,m_encoding);
          m_comboEncoding.SetCurSel(ind);
    ind = m_comboTarget.FindString(-1,m_target);
          m_comboTarget.SetCurSel(ind);

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

BEGIN_MESSAGE_MAP(FormDlg, CDialog)
  ON_EN_KILLFOCUS (IDC_FORM_NAME,     OnEnChangeFormName)
  ON_EN_KILLFOCUS (IDC_FORM_ACTION,   OnEnChangeFormAction)
  ON_CBN_SELCHANGE(IDC_FORM_METHOD,   OnCbnSelchangeFormMethod)
  ON_CBN_SELCHANGE(IDC_FORM_ENCODING, OnCbnSelchangeFormEncoding)
  ON_CBN_SELCHANGE(IDC_FORM_TARGET,   OnCbnSelchangeFormTarget)
  ON_BN_CLICKED   (IDC_ID,            OnBnClickedId)
  ON_BN_CLICKED   (IDC_STYLE,         OnBnClickedStyle)
  ON_BN_CLICKED   (IDC_EVENTS,        OnBnClickedEvents)
  ON_BN_CLICKED   (IDOK,              OnBnClickedOk)
  ON_BN_CLICKED   (IDCANCEL,          OnBnClickedCancel)
  ON_BN_CLICKED   (IDC_FORM_HELP,     OnBnClickedFormHelp)
END_MESSAGE_MAP()

BOOL
FormDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  return TRUE;
}

// FormDlg message handlers

void FormDlg::OnEnChangeFormName()
{
  CWnd* w = GetDlgItem(IDC_FORM_NAME);
  w->GetWindowText(m_name);
}

void FormDlg::OnEnChangeFormAction()
{
  CWnd* w = GetDlgItem(IDC_FORM_ACTION);
  w->GetWindowText(m_actionUrl);
}

void FormDlg::OnCbnSelchangeFormMethod()
{
  int ind = m_comboMethod.GetCurSel();
  if(ind >= 0)
  {
    m_comboMethod.GetLBText(ind,m_method);
  }
}

void FormDlg::OnCbnSelchangeFormEncoding()
{
  int ind = m_comboEncoding.GetCurSel();
  if(ind >= 0)
  {
    m_comboEncoding.GetLBText(ind,m_encoding);
  }
}

void FormDlg::OnCbnSelchangeFormTarget()
{
  int ind = m_comboTarget.GetCurSel();
  if(ind >= 0)
  {
    m_comboTarget.GetLBText(ind,m_target);
  }
}

void FormDlg::OnBnClickedOk()
{
  CString msg1,msg2,msg3;
  if(m_name.IsEmpty())
  {
    msg1 = "\nThe name of the form was not filled in yet.";
  }
  if(m_method.IsEmpty())
  {
    msg2 = "\nThe form method (get/post) was not filled in yet";
  }
  if(m_actionUrl.IsEmpty())
  {
    msg3 = "\nThe URL the form is acting upon was not filled in yet";
  }
  CString msg = msg1 + msg2 + msg3;
  if(!msg.IsEmpty())
  {
    msg = CString("Minimal requirements for this forms attributes are not yet met!\n") + msg;
    theApp.ErrorMessage(msg);
    return;
  }
  OnOK();
}

void FormDlg::OnBnClickedCancel()
{
  OnCancel();
}

void FormDlg::OnBnClickedId()
{
  GeneralIDDlg dlg(this,"form",m_elem);
  dlg.DoModal();
}

void 
FormDlg::OnBnClickedEvents()
{
  TagEventsDlg dlg(this,m_elem,"form");
  dlg.DoModal();
  UpdateData(Data2Controls);
}

void FormDlg::OnBnClickedStyle()
{
  if(!m_elem->HasStyle())
  {
    m_elem->SetStyle();
  }
  CString style = m_elem->GetInlineStyle();
  style = CString("form { ") + style + "}";
  StyleSheetDlg dlg(this,m_base,"form",NULL,style);
  if(dlg.DoModal() == IDOK)
  {
    style = dlg.GetInlineStylesheet();
    style.TrimRight("}");
    style.TrimLeft("form {");
    m_elem->SetInlineStyle(style);
  }
}

void FormDlg::OnBnClickedFormHelp()
{
  // TODO: Add your control notification handler code here
}


