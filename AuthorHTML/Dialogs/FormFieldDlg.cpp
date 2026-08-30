//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    FormFieldDlg.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for managing the <FIELD> tag in HTML, 
//
#include "stdafx.h"
#include "AuthorHTML.h"
#include "FormFieldDlg.h"
#include "GeneralIDDlg.h"
#include "TagEventsDlg.h"

// FormFieldDlg dialog

IMPLEMENT_DYNAMIC(FormFieldDlg, CDialog)

FormFieldDlg::FormFieldDlg(CWnd* pParent
                          ,CString p_base
                          ,CString type
                          ,CString name
                          ,CString value
                          ,CString maxLength
                          ,CString size
                          ,HtmlElement* elem)
             :CDialog(FormFieldDlg::IDD, pParent)
             ,m_type(type)
             ,m_name(name)
             ,m_value(value)
             ,m_maxLength(maxLength)
             ,m_size(size)
             ,m_elem(elem)
             ,m_base(p_base)
{
}

FormFieldDlg::~FormFieldDlg()
{
}

void FormFieldDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
  DDX_Control(pDX,IDC_FF_TYPE,      m_comboType);
  DDX_Text   (pDX,IDC_FF_NAME,      m_name);
  DDX_Text   (pDX,IDC_FF_VALUE,     m_value);
  DDX_Text   (pDX,IDC_FF_MAXLENGTH, m_maxLength);
  DDX_Text   (pDX,IDC_FF_SIZE,      m_size);
  DDX_Control(pDX,IDC_SPIN1,        m_spinMaxlength);
  DDX_Control(pDX,IDC_SPIN2,        m_spinSize);
  DDX_Control(pDX,IDC_ID,           m_buttonID);
  DDX_Control(pDX,IDC_EVENTS,       m_buttonEvents);
  DDX_Control(pDX,IDC_STYLE,        m_buttonStyle);
  DDX_Control(pDX,IDOK,             m_buttonOK);
  DDX_Control(pDX,IDCANCEL,         m_buttonCancel);
  DDX_Control(pDX,IDC_FF_HELP,      m_buttonHelp);

  if(pDX->m_bSaveAndValidate == Data2Controls)
  {
    int ind = m_comboType.FindString(-1,m_type);
    m_comboType.SetCurSel(ind);

    bool notHidden = (m_type != "Hidden");
    m_spinSize.EnableWindow(notHidden);
    m_spinMaxlength.EnableWindow(notHidden);
    CWnd* w1 = GetDlgItem(IDC_FF_MAXLENGTH);
    CWnd* w2 = GetDlgItem(IDC_FF_SIZE);
    w1->EnableWindow(notHidden);
    w2->EnableWindow(notHidden);

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

BEGIN_MESSAGE_MAP(FormFieldDlg, CDialog)
  ON_CBN_SELCHANGE(IDC_FF_TYPE,     OnCbnSelchangeFfType)
  ON_EN_KILLFOCUS (IDC_FF_NAME,     OnEnChangeFfName)
  ON_EN_KILLFOCUS (IDC_FF_VALUE,    OnEnChangeFfValue)
  ON_EN_KILLFOCUS (IDC_FF_MAXLENGTH,OnEnChangeFfMaxlength)
  ON_EN_KILLFOCUS (IDC_FF_SIZE,     OnEnChangeFfSize)
  ON_BN_CLICKED   (IDC_ID,          OnBnClickedId)
  ON_BN_CLICKED   (IDC_EVENTS,      OnBnClickedEvents)
  ON_BN_CLICKED   (IDC_STYLE,       OnBnClickedStyle)
  ON_BN_CLICKED   (IDOK,            OnBnClickedOk)
  ON_BN_CLICKED   (IDCANCEL,        OnBnClickedCancel)
  ON_BN_CLICKED   (IDC_FF_HELP,     OnBnClickedFfHelp)
END_MESSAGE_MAP()

BOOL
FormFieldDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  // Default type is always the textfield type
  if(m_type.IsEmpty())
  {
    m_type = "Text";
    UpdateData(Data2Controls);
  }
  else
  {
    m_comboType.EnableWindow(FALSE);
  }
  // INIT spin controls
  m_spinMaxlength.SetBase(10);
  m_spinMaxlength.SetRange(1,32000);
  m_spinMaxlength.SetPos(atoi(m_maxLength));
  m_spinSize.SetBase(10);
  m_spinSize.SetRange(1,32000);
  m_spinSize.SetPos(atoi(m_size));

  return TRUE;
}

void
FormFieldDlg::FillPage()
{
}

void
FormFieldDlg::UpdateProperties()
{
}

// FormFieldDlg message handlers

void FormFieldDlg::OnCbnSelchangeFfType()
{
  int ind = m_comboType.GetCurSel();
  if(ind >= 0)
  {
    m_comboType.GetLBText(ind,m_type);
  }
}

void FormFieldDlg::OnEnChangeFfName()
{
  CWnd* w = GetDlgItem(IDC_FF_NAME);
  w->GetWindowText(m_name);
}

void FormFieldDlg::OnEnChangeFfValue()
{
  CWnd* w = GetDlgItem(IDC_FF_VALUE);
  w->GetWindowText(m_value);
}

void FormFieldDlg::OnEnChangeFfMaxlength()
{
  CWnd* w = GetDlgItem(IDC_FF_MAXLENGTH);
  w->GetWindowText(m_maxLength);
}

void FormFieldDlg::OnEnChangeFfSize()
{
  CWnd* w = GetDlgItem(IDC_FF_SIZE);
  w->GetWindowText(m_size);
}

void FormFieldDlg::OnBnClickedOk()
{
  CString msg1,msg2,msg3;
  if(m_type.IsEmpty())
  {
    msg1 = "\nThe type of the field is not yet givven.";
  }
  if(m_name.IsEmpty())
  {
    msg2 = "\nThe parameter name of the field is not yet givven.";
  }
  CString msg = msg1 + msg2;
  if(!msg.IsEmpty())
  {
    theApp.ErrorMessage(msg);
    return;
  }
  if(m_type == "Hidden")
  {
    // Never set for hidden fields
    m_size = m_maxLength = "";
  }
  OnOK();
}

void FormFieldDlg::OnBnClickedCancel()
{
  OnCancel();
}

void FormFieldDlg::OnBnClickedId()
{
  GeneralIDDlg dlg(this,"input",m_elem);
  dlg.DoModal();
}

void 
FormFieldDlg::OnBnClickedEvents()
{
  TagEventsDlg dlg(this,m_elem,"input");
  dlg.DoModal();
  UpdateData(Data2Controls);
}

void FormFieldDlg::OnBnClickedStyle()
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
FormFieldDlg::OnBnClickedFfHelp()
{
  // TODO: Add your control notification handler code here
}

