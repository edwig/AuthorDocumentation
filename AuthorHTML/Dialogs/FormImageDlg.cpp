//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    FormImageDlg.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for managing the <IMG> tag in HTML, 
//
#include "stdafx.h"
#include "AuthorHTML.h"
#include "FormImageDlg.h"
#include "FileDialog.h"
#include "GeneralIDDlg.h"
#include "StyleSheetDlg.h"
#include "TagEventsDlg.h"

// FormImageDlg dialog

IMPLEMENT_DYNAMIC(FormImageDlg, CDialog)
FormImageDlg::FormImageDlg(CWnd*      pParent
                          ,CString    base
                          ,HtmlInput* elem)
             :CDialog(FormImageDlg::IDD, pParent)
             ,m_elem(elem)
             ,m_base(base)
{
}

FormImageDlg::~FormImageDlg()
{
}

void 
FormImageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
  DDX_Text   (pDX,IDC_FI_NAME,  m_name);
  DDX_Text   (pDX,IDC_FI_URL,   m_url);
  DDX_Control(pDX,IDC_FI_CHOOSE,m_buttonChoose);
  DDX_Control(pDX,IDC_FI_ALIGN, m_comboAlign);
  DDX_Control(pDX,IDC_ID,       m_buttonID);
  DDX_Control(pDX,IDC_EVENTS,   m_buttonEvents);
  DDX_Control(pDX,IDC_STYLE,    m_buttonStyle);
  DDX_Control(pDX,IDOK,         m_buttonOK);
  DDX_Control(pDX,IDCANCEL,     m_buttonCancel);
  DDX_Control(pDX,IDC_FI_HELP,  m_buttonHelp);

  if(pDX->m_bSaveAndValidate == Data2Controls)
  {
    int ind = m_comboAlign.FindString(-1,m_align);
    m_comboAlign.SetCurSel(ind);
  }
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

BEGIN_MESSAGE_MAP(FormImageDlg, CDialog)
  ON_EN_KILLFOCUS (IDC_FI_NAME,   OnEnChangeFiName)
  ON_EN_KILLFOCUS (IDC_FI_URL,    OnEnChangeFiUrl)
  ON_BN_CLICKED   (IDC_FI_CHOOSE, OnBnClickedFiChoose)
  ON_CBN_SELCHANGE(IDC_FI_ALIGN,  OnCbnSelchangeFiAlign)
  ON_BN_CLICKED   (IDC_ID,        OnBnClickedId)
  ON_BN_CLICKED   (IDC_EVENTS,    OnBnClickedEvents)
  ON_BN_CLICKED   (IDC_STYLE,     OnBnClickedStyle)
  ON_BN_CLICKED   (IDOK,          OnBnClickedOk)
  ON_BN_CLICKED   (IDCANCEL,      OnBnClickedCancel)
  ON_BN_CLICKED   (IDC_FI_HELP,   OnBnClickedFiHelp)
END_MESSAGE_MAP()

BOOL
FormImageDlg::OnInitDialog()
{
  CDialog::OnInitDialog();
  FillPage();
  UpdateData(Data2Controls);
  return TRUE;
}

void
FormImageDlg::FillPage()
{
  m_name  = m_elem->GetProperty(HtmlInput::E_Name);
  m_url   = m_elem->GetProperty(HtmlInput::E_Src);
  m_align = m_elem->GetProperty(HtmlInput::E_Align);
}

void
FormImageDlg::UpdateProperties()
{
  m_elem->SetProperty(HtmlInput::E_Name, m_name);
  m_elem->SetProperty(HtmlInput::E_Src,  m_url);
  m_elem->SetProperty(HtmlInput::E_Align,m_align);
}

// FormImageDlg message handlers

void 
FormImageDlg::OnEnChangeFiName()
{
  CWnd* w = GetDlgItem(IDC_FI_NAME);
  w->GetWindowText(m_name);
}

void 
FormImageDlg::OnEnChangeFiUrl()
{
  CWnd* w = GetDlgItem(IDC_FI_URL);
  w->GetWindowText(m_url);
}

void 
FormImageDlg::OnBnClickedFiChoose()
{
  DocFileDialog diag(true               // true = open
                    ,"Select an image"  // title
                    ,""                 // Extension
                    ,""                 // Default file
                    ,0                  // flags
                    ,"All images (jpg,gif,bmp,png)|*.jpg;*.jpeg;*.gif;*.bmp|"
                    "Joint Photogroup files (jpg)|*.jpg;*.jpeg|"
                    "Graphics Information File (gif)|*.gif|"
                    "Portable Network Graphics (png)|*.png|"
                    "Windows bitmaps (bmp)|*.bmp|"
                    "All files|*.*");
  if(diag.DoModal() == IDOK)
  {
    m_url = diag.GetChosenFile();
    CString relative;
    if(Misc::MakeRelativePathname(m_base,m_url,relative))
    {
      m_url = relative;
    }
    UpdateData(Data2Controls);
  }
}

void 
FormImageDlg::OnCbnSelchangeFiAlign()
{
  int ind = m_comboAlign.GetCurSel();
  if(ind >= 0)
  {
    m_comboAlign.GetLBText(ind,m_align);
  }
}

void 
FormImageDlg::OnBnClickedOk()
{
  if(m_url.IsEmpty())
  {
    theApp.ErrorMessage("A form image submit button must have at least an image-url!");
    return;
  }
  UpdateProperties();
  OnOK();
}

void 
FormImageDlg::OnBnClickedCancel()
{
  OnCancel();
}

void FormImageDlg::OnBnClickedId()
{
  GeneralIDDlg dlg(this,"input",m_elem);
  dlg.DoModal();
}

void 
FormImageDlg::OnBnClickedEvents()
{
  TagEventsDlg dlg(this,m_elem,"input");
  dlg.DoModal();
  UpdateData(Data2Controls);
}

void FormImageDlg::OnBnClickedStyle()
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
FormImageDlg::OnBnClickedFiHelp()
{
  // TODO: Add your control notification handler code here
}

