//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    FormTextareaDlg.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for managing the <TEXTAREA> tag in HTML, 
//
#include "stdafx.h"
#include "AuthorHTML.h"
#include "FormTextareaDlg.h"
#include "Misc.h"
#include "GeneralIDDlg.h"
#include "StyleSheetDlg.h"
#include "TagEventsDlg.h"

// FormTextareaDlg dialog

IMPLEMENT_DYNAMIC(FormTextareaDlg, CDialog)

FormTextareaDlg::FormTextareaDlg(CWnd* pParent
                                 ,CString p_base
                                 ,CString name
                                 ,CString cols
                                 ,CString rows
                                 ,CString wrap
                                 ,HtmlElement* p_elem)
                :CDialog(FormTextareaDlg::IDD, pParent)
                ,m_name(name)
                ,m_cols(cols)
                ,m_rows(rows)
                ,m_wrap(wrap)
                ,m_elem(p_elem)
                ,m_base(p_base)
{
}

FormTextareaDlg::~FormTextareaDlg()
{
}

void 
FormTextareaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
  DDX_Text   (pDX,IDC_TA_NAME,    m_name);
  DDX_Text   (pDX,IDC_TA_WIDTH,   m_cols);
  DDX_Text   (pDX,IDC_TA_ROWS,    m_rows);
  DDX_Control(pDX,IDC_TA_WORDWRAP,m_comboWrap);
  DDX_Control(pDX,IDC_SPIN1,      m_spinCols);
  DDX_Control(pDX,IDC_SPIN2,      m_spinRows);
  DDX_Control(pDX,IDC_ID,         m_buttonID);
  DDX_Control(pDX,IDC_EVENTS,     m_buttonEvents);
  DDX_Control(pDX,IDC_STYLE,      m_buttonStyle);
  DDX_Control(pDX,IDOK,           m_buttonOK);
  DDX_Control(pDX,IDCANCEL,       m_buttonCancel);
  DDX_Control(pDX,IDC_TA_HELP,    m_buttonHelp);

  if(pDX->m_bSaveAndValidate == Data2Controls)
  {
    CString disp = Misc::GetAttributeDisplayname("wrap",m_wrap);
    int ind = m_comboWrap.FindString(-1,disp);
    m_comboWrap.SetCurSel(ind);

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

BEGIN_MESSAGE_MAP(FormTextareaDlg, CDialog)
  ON_EN_KILLFOCUS (IDC_TA_NAME,     OnEnChangeTaName)
  ON_EN_KILLFOCUS (IDC_TA_WIDTH,    OnEnChangeTaWidth)
  ON_EN_KILLFOCUS (IDC_TA_ROWS,     OnEnChangeTaRows)
  ON_CBN_SELCHANGE(IDC_TA_WORDWRAP, OnCbnSelchangeTaWordwrap)
  ON_BN_CLICKED   (IDC_ID,          OnBnClickedId)
  ON_BN_CLICKED   (IDC_EVENTS,      OnBnClickedEvents)
  ON_BN_CLICKED   (IDC_STYLE,       OnBnClickedStyle)
  ON_BN_CLICKED   (IDOK,            OnBnClickedOk)
  ON_BN_CLICKED   (IDCANCEL,        OnBnClickedCancel)
  ON_BN_CLICKED   (IDC_TA_HELP,     OnBnClickedTaHelp)
END_MESSAGE_MAP()

BOOL
FormTextareaDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  vector<string> all;
  Misc::GetAllAttributeDisplaynames("wrap",&all);
  for(unsigned int ind = 0; ind < all.size(); ++ind)
  {
    CString disp = all[ind].c_str();
    if(m_comboWrap.FindString(-1,disp) < 0)
    {
      m_comboWrap.AddString(disp);
    }
  }
  m_spinCols.SetBase(10);
  m_spinRows.SetBase(10);
  m_spinCols.SetRange(0,32000);
  m_spinRows.SetRange(0,32000);
  m_spinCols.SetPos(atoi(m_cols));
  m_spinRows.SetPos(atoi(m_rows));
  UpdateData(Data2Controls);
  return TRUE;
}

// FormTextareaDlg message handlers

void 
FormTextareaDlg::OnEnChangeTaName()
{
  CWnd* w = GetDlgItem(IDC_TA_NAME);
  w->GetWindowText(m_name);
}

void 
FormTextareaDlg::OnEnChangeTaWidth()
{
  CWnd* w = GetDlgItem(IDC_TA_WIDTH);
  w->GetWindowText(m_cols);
}

void 
FormTextareaDlg::OnEnChangeTaRows()
{
  CWnd* w = GetDlgItem(IDC_TA_ROWS);
  w->GetWindowText(m_rows);
}

void 
FormTextareaDlg::OnCbnSelchangeTaWordwrap()
{
  int ind = m_comboWrap.GetCurSel();
  if(ind >= 0)
  {
    CString disp;
    m_comboWrap.GetLBText(ind,disp);
    m_wrap = Misc::GetAttributeValue("wrap",disp);
  }
}

void 
FormTextareaDlg::OnBnClickedId()
{
  GeneralIDDlg dlg(this,"textarea",m_elem);
  dlg.DoModal();
}

void 
FormTextareaDlg::OnBnClickedEvents()
{
  TagEventsDlg dlg(this,m_elem,"TEXTAREA");
  dlg.DoModal();
  UpdateData(Data2Controls);
}

void 
FormTextareaDlg::OnBnClickedStyle()
{
  if(!m_elem->HasStyle())
  {
    m_elem->SetStyle();
  }
  CString style = m_elem->GetInlineStyle();
  style = CString("textarea { ") + style + "}";
  StyleSheetDlg dlg(this,m_base,"textarea",NULL,style);
  if(dlg.DoModal() == IDOK)
  {
    style = dlg.GetInlineStylesheet();
    style.TrimRight("}");
    style.TrimLeft("textarea {");
    m_elem->SetInlineStyle(style);
  }
}

void 
FormTextareaDlg::OnBnClickedOk()
{
  CString msg1,msg2,msg3;
  if(m_name.IsEmpty())
  {
    msg1 = "\nParameter name for textarea not yet givven.";
  }
  if(atoi(m_rows) == 0)
  {
    msg2 = "\nNumber of textlines for textarea not yet givven.";
  }
  if(atoi(m_cols) == 0)
  {
    msg3 = "\nWidth in columns for textarea not yet givven.";
  }
  CString msg = msg1 + msg2 + msg3;
  if(!msg.IsEmpty())
  {
    msg = CString("The definition of the textarea is incomplete:")  + msg;
    theApp.ErrorMessage(msg);
    return;
  }
  OnOK();
}

void 
FormTextareaDlg::OnBnClickedCancel()
{
  OnCancel();
}

void 
FormTextareaDlg::OnBnClickedTaHelp()
{
  // TODO: Add your control notification handler code here
}

