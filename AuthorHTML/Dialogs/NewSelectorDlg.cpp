//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    NewSelectorDlg.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for creating an new CSS selector
//
#include "stdafx.h"
#include "AuthorHTML.h"
#include "NewSelectorDlg.h"
#include "Misc.h"
#include "Css.h"

// NewSelectorDlg dialog

IMPLEMENT_DYNAMIC(NewSelectorDlg, CDialog)
NewSelectorDlg::NewSelectorDlg(CWnd*   pParent)
	             :CDialog(NewSelectorDlg::IDD, pParent)
{
}

NewSelectorDlg::~NewSelectorDlg()
{
}

void NewSelectorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
  DDX_Text   (pDX,IDC_RESULT_RULE,    m_totalResult);
  DDX_Control(pDX,IDC_NEW_SELECTOR,   m_comboSelector);
  DDX_Control(pDX,IDC_ONLY_ATTRIBUTE, m_comboAttribute);
  DDX_Control(pDX,IDC_ONLY_PSEUDO,    m_comboPseudo);
  DDX_Text   (pDX,IDC_CLASSNAME,      m_classname);
  DDX_Control(pDX,IDC_RADIO1,         m_buttonSel);
  DDX_Control(pDX,IDC_RADIO2,         m_buttonAttr);
  DDX_Control(pDX,IDC_RADIO3,         m_buttonPseudo);
  DDX_Control(pDX,IDC_RADIO4,         m_buttonID);
  DDX_Control(pDX,IDC_NEW_CLASSNAME,  m_buttonClass);
  DDX_Control(pDX,IDC_EXT_SAME,       m_buttonIdentical);
  DDX_Control(pDX,IDC_EXT_DESCENDANT, m_buttonDescendant);
  DDX_Control(pDX,IDC_EXT_DIRECT,     m_buttonDirectChild);
  DDX_Control(pDX,IDC_EXT_ADJACENT,   m_buttonAdjacent);
  DDX_Text   (pDX,IDC_ID,             m_uniqueID);
  DDX_Control(pDX,IDOK,               m_buttonOK);
  DDX_Control(pDX,IDCANCEL,           m_buttonCancel);
  DDX_Control(pDX,IDC_NSR_HELP,       m_buttonHelp);
}

BEGIN_MESSAGE_MAP(NewSelectorDlg, CDialog)
  ON_CBN_SELCHANGE(IDC_NEW_SELECTOR,  OnCbnSelchangeNewSelector)
  ON_CBN_KILLFOCUS(IDC_NEW_SELECTOR,  OnChangeSelector)
  ON_CBN_SELCHANGE(IDC_ONLY_ATTRIBUTE,OnCbnSelchangeOnlyAttribute)
  ON_CBN_KILLFOCUS(IDC_ONLY_ATTRIBUTE,OnCbnSelchangeOnlyAttribute)
  ON_CBN_SELCHANGE(IDC_ONLY_PSEUDO,   OnCbnSelchangeOnlyPseudo)
  ON_BN_CLICKED   (IDC_RADIO1,        OnBnClickedRadio1)
  ON_BN_CLICKED   (IDC_RADIO2,        OnBnClickedRadio2)
  ON_BN_CLICKED   (IDC_RADIO3,        OnBnClickedRadio3)
  ON_BN_CLICKED   (IDC_RADIO4,        OnBnClickedRadio4)
  ON_BN_CLICKED   (IDOK,              OnBnClickedOk)
  ON_BN_CLICKED   (IDCANCEL,          OnBnClickedCancel)
  ON_BN_CLICKED   (IDC_NSR_HELP,      OnBnClickedNsrHelp)
  ON_EN_KILLFOCUS (IDC_CLASSNAME,     OnEnChangeClassname)
  ON_EN_KILLFOCUS (IDC_ID,            OnEnChangeId)
  ON_BN_CLICKED   (IDC_EXT_SAME,      OnBnClickedExtSame)
  ON_BN_CLICKED   (IDC_EXT_DESCENDANT,OnBnClickedExtDescendant)
  ON_BN_CLICKED   (IDC_EXT_DIRECT,    OnBnClickedExtDirect)
  ON_BN_CLICKED   (IDC_EXT_ADJACENT,  OnBnClickedExtAdjacent)
  ON_BN_CLICKED   (IDC_NEW_CLASSNAME, OnBnClickedNewClassname)
END_MESSAGE_MAP()

BOOL
NewSelectorDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  // Fill all tags as selectors
  vector<string> tags;
  Misc::GetAllTags(&tags);
  for(unsigned int ind = 0; ind < tags.size(); ++ind)
  {
    CString tag = tags[ind].c_str();
    m_comboSelector.AddString(tag);
  }
  // Fill all attributes
  vector<string> all;
  CssStyleSheet sheet;
  sheet.GetAllAttributes(&all);
  for(unsigned int i = 0;i < all.size(); ++i)
  {
    CString attribute = all[i].c_str();
    m_comboAttribute.AddString(attribute);
  }
  CWnd* edit = GetDlgItem(IDC_CLASSNAME);
  edit->EnableWindow(FALSE);
  edit = GetDlgItem(IDC_ID);
  edit->EnableWindow(FALSE);
  m_comboAttribute.EnableWindow(FALSE);
  m_comboPseudo.EnableWindow(FALSE);
  m_buttonSel.SetCheck(TRUE);
  m_buttonAttr.SetCheck(FALSE);
  m_buttonPseudo.SetCheck(FALSE);
  m_buttonClass.SetCheck(FALSE);
  m_buttonID.SetCheck(FALSE);
  return TRUE;
}

CString
NewSelectorDlg::GetSelector()
{
  return m_totalResult;
}

CString
NewSelectorDlg::GetAddedSelector()
{
  CString totalSelector = m_selector;
  if(!m_classname.IsEmpty())
  {
    totalSelector += ".";
    totalSelector += m_classname;
  }
  if(!m_attribute.IsEmpty())
  {
    totalSelector += "[";
    totalSelector += m_attribute;
    totalSelector += "]";
  }
  if(!m_pseudo.IsEmpty())
  {
    totalSelector += m_pseudo;
  }
  if(!m_uniqueID.IsEmpty())
  {
    totalSelector += "#";
    totalSelector += m_uniqueID;
  }
  return m_totalResult + totalSelector;
}

// NewSelectorDlg message handlers

void NewSelectorDlg::OnCbnSelchangeNewSelector()
{
  int ind = m_comboSelector.GetCurSel();
  if(ind >= 0)
  {
    m_comboSelector.GetLBText(ind,m_selector);
  }
}

void
NewSelectorDlg::OnChangeSelector()
{
  int ind = m_comboSelector.GetCurSel();
  if(ind >= 0)
  {
    m_comboSelector.GetLBText(ind,m_selector);
  }
  else
  {
    m_comboSelector.GetWindowText(m_selector);
    ind = m_comboSelector.FindString(-1,m_selector);
    if(ind >= 0)
    {
      m_comboSelector.SetCurSel(ind);
    }
  }
}

void 
NewSelectorDlg::OnCbnSelchangeOnlyAttribute()
{
  int ind = m_comboAttribute.GetCurSel();
  if(ind >= 0)
  {
    m_comboAttribute.GetLBText(ind,m_attribute);
  }
  else
  {
    m_comboAttribute.GetWindowText(m_attribute);
    ind = m_comboAttribute.FindString(-1,m_attribute);
    if(ind >= 0)
    {
      m_comboAttribute.SetCurSel(ind);
    }
  }
}

void 
NewSelectorDlg::OnCbnSelchangeOnlyPseudo()
{
  int ind = m_comboPseudo.GetCurSel();
  if(ind >= 0)
  {
    m_comboPseudo.GetLBText(ind,m_pseudo);
  }
  else
  {
    m_comboPseudo.GetWindowText(m_pseudo);
    ind = m_comboPseudo.FindString(-1,m_pseudo);
    if(ind >= 0)
    {
      m_comboPseudo.SetCurSel(ind);
    }
  }
}

// Standard tag only
void 
NewSelectorDlg::OnBnClickedRadio1()
{
  m_comboAttribute.SetCurSel(-1);
  m_comboAttribute.EnableWindow(FALSE);
  m_comboPseudo.SetCurSel(-1);
  m_comboPseudo.EnableWindow(FALSE);
  CWnd* u = GetDlgItem(IDC_ID);
  u->EnableWindow(FALSE);
  m_attribute = "";
  m_pseudo    = "";
  m_uniqueID  = "";
  UpdateData(Data2Controls);
}

// Exclusivly attribute
void 
NewSelectorDlg::OnBnClickedRadio2()
{
  m_comboAttribute.EnableWindow(TRUE);
  m_comboPseudo.SetCurSel(-1);
  m_comboPseudo.EnableWindow(FALSE);
  CWnd* u = GetDlgItem(IDC_ID);
  u->EnableWindow(FALSE);
  m_pseudo   = "";
  m_uniqueID = "";
  UpdateData(Data2Controls);
}

// Special effects only
void 
NewSelectorDlg::OnBnClickedRadio3()
{
  m_comboAttribute.SetCurSel(-1);
  m_comboAttribute.EnableWindow(FALSE);
  m_comboPseudo.EnableWindow(TRUE);
  CWnd* u = GetDlgItem(IDC_ID);
  u->EnableWindow(FALSE);
  m_attribute = "";
  m_uniqueID  = "";
  UpdateData(Data2Controls);
}

// Unique ID only
void 
NewSelectorDlg::OnBnClickedRadio4()
{
  m_comboAttribute.SetCurSel(-1);
  m_comboAttribute.EnableWindow(FALSE);
  m_comboPseudo.SetCurSel(-1);
  m_comboPseudo.EnableWindow(FALSE);
  CWnd* u = GetDlgItem(IDC_ID);
  u->EnableWindow(TRUE);
  m_attribute = "";
  m_pseudo    = "";
  UpdateData(Data2Controls);
}

void 
NewSelectorDlg::OnBnClickedOk()
{
  if(m_selector.IsEmpty() && m_classname.IsEmpty() && m_uniqueID.IsEmpty())
  {
    theApp.MessageBox("You must first fill in an selector, classname or unique ID","Error",MB_OK|MB_ICONHAND);
    return;
  }
  CString oldResult = m_totalResult;
  m_totalResult = GetAddedSelector();
  UpdateData(Data2Controls);
  CString mess;
  mess.Format("Would you like to add the selector '%s' to the stylesheet?",m_totalResult.GetString());
  if(theApp.MessageBox(mess,"Question",MB_YESNO|MB_ICONQUESTION) == IDYES)
  {
    OnOK();
  }
  else
  {
    m_totalResult = oldResult;
    UpdateData(Data2Controls);
  }
}

void 
NewSelectorDlg::OnBnClickedCancel()
{
  OnCancel();
}

void 
NewSelectorDlg::OnEnChangeClassname()
{
  CWnd* edit = GetDlgItem(IDC_CLASSNAME);
  edit->GetWindowText(m_classname);
}

void
NewSelectorDlg::ResetDialog()
{
  m_selector  =
  m_attribute = 
  m_pseudo    =
  m_classname = "";
  m_comboSelector .SetCurSel(-1);
  m_comboAttribute.SetCurSel(-1);
  m_comboPseudo   .SetCurSel(-1);
  m_comboAttribute.EnableWindow(FALSE);
  m_comboPseudo   .EnableWindow(FALSE);
  m_buttonSel   .SetCheck(TRUE);
  m_buttonAttr  .SetCheck(FALSE);
  m_buttonPseudo.SetCheck(FALSE);
  m_buttonClass .SetCheck(FALSE);
  m_buttonID    .SetCheck(FALSE);
  CWnd* edit = GetDlgItem(IDC_CLASSNAME);
  edit->EnableWindow(FALSE);
}

void 
NewSelectorDlg::ExtraSelector(char extra)
{
  if(m_selector.IsEmpty() && m_classname.IsEmpty() && m_uniqueID.IsEmpty())
  {
    theApp.ErrorMessage("Fill in a selector, classname or unique ID first");
  }
  else
  {
    m_totalResult = GetAddedSelector();
    m_totalResult += extra;
    ResetDialog();
    UpdateData(Data2Controls);
  }
}

void 
NewSelectorDlg::OnBnClickedExtSame()
{
  ExtraSelector(',');
}

void 
NewSelectorDlg::OnBnClickedExtDescendant()
{
  ExtraSelector(' ');
}

void 
NewSelectorDlg::OnBnClickedExtDirect()
{
  ExtraSelector('>');
}

void 
NewSelectorDlg::OnBnClickedExtAdjacent()
{
  ExtraSelector('+');
}

void 
NewSelectorDlg::OnBnClickedNewClassname()
{
  CWnd *edit = GetDlgItem(IDC_CLASSNAME);
  edit->EnableWindow(m_buttonClass.GetCheck());
}

void 
NewSelectorDlg::OnEnChangeId()
{
  CWnd* w = GetDlgItem(IDC_ID);
  w->GetWindowText(m_uniqueID);
}

void 
NewSelectorDlg::OnBnClickedNsrHelp()
{
  // TODO: Add your control notification handler code here
}


