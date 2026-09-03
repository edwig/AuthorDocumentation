//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    KeywordDlg.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for managing the 5-level keywords in HTML
//
#include "StdAfx.h"
#include "AuthorHTML.h"
#include "KeywordDlg.h"

IMPLEMENT_DYNAMIC(KeywordDlg, CDialog)

KeywordDlg::KeywordDlg(CWnd*        pParent
                      ,HtmlElement* elem
                      ,CString      p_base
                      ,KeywordDef*  def)
           :CDialog(KeywordDlg::IDD, pParent)
           ,m_elem(elem)
           ,m_base(p_base)
           ,m_def(def)
{
}

KeywordDlg::~KeywordDlg()
{
}

void 
KeywordDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Control(pDX,IDC_LINKTYPE,  m_comboType);
  DDX_Text   (pDX,IDC_COMPOSITE, m_composite);
  DDX_Text   (pDX,IDC_LEVEL1,    m_level1);
  DDX_Text   (pDX,IDC_LEVEL2,    m_level2);
  DDX_Text   (pDX,IDC_LEVEL3,    m_level3);
  DDX_Text   (pDX,IDC_LEVEL4,    m_level4);
  DDX_Text   (pDX,IDC_LEVEL5,    m_level5);

  if(pDX->m_bSaveAndValidate == FALSE)
  {
    CWnd* w2 = GetDlgItem(IDC_LEVEL2);
    CWnd* w3 = GetDlgItem(IDC_LEVEL3);
    CWnd* w4 = GetDlgItem(IDC_LEVEL4);
    CWnd* w5 = GetDlgItem(IDC_LEVEL5);
    bool key = m_type == "Index";
    w2->EnableWindow(!m_level1.IsEmpty() && key);
    w3->EnableWindow(!m_level2.IsEmpty() && key);
    w4->EnableWindow(!m_level3.IsEmpty() && key);
    w5->EnableWindow(!m_level4.IsEmpty() && key);
  }
}

BEGIN_MESSAGE_MAP(KeywordDlg, CDialog)
  ON_CBN_SELCHANGE(IDC_LINKTYPE,    &KeywordDlg::OnCbnSelchangeLinktype)
  ON_EN_CHANGE    (IDC_COMPOSITE,   &KeywordDlg::OnEnChangeComposite)
  ON_EN_CHANGE    (IDC_LEVEL1,      &KeywordDlg::OnEnChangeLevel1)
  ON_EN_CHANGE    (IDC_LEVEL2,      &KeywordDlg::OnEnChangeLevel2)
  ON_EN_CHANGE    (IDC_LEVEL3,      &KeywordDlg::OnEnChangeLevel3)
  ON_EN_CHANGE    (IDC_LEVEL4,      &KeywordDlg::OnEnChangeLevel4)
  ON_EN_CHANGE    (IDC_LEVEL5,      &KeywordDlg::OnEnChangeLevel5)
  ON_BN_CLICKED   (IDOK,            &KeywordDlg::OnBnClickedOk)
  ON_BN_CLICKED   (IDCANCEL,        &KeywordDlg::OnBnClickedCancel)
  ON_BN_CLICKED   (IDC_HELP_KEYWORD,&KeywordDlg::OnBnClickedHelpKeyword)
END_MESSAGE_MAP()

BOOL
KeywordDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  m_comboType.AddString("Composite index keyword");
  m_comboType.AddString("Associative link keyword");

  FillPage();

  return TRUE;
}

void
KeywordDlg::FillPage()
{
  // Fill combobox
  m_type  = m_def->m_type;
  int ind = m_def->m_type == "Index" ? 0 : 1;
  m_comboType.SetCurSel(ind);

  // Fill text fields
  m_composite = m_def->m_composite;
  m_level1    = m_def->m_level1;
  m_level2    = m_def->m_level2;
  m_level3    = m_def->m_level3;
  m_level4    = m_def->m_level4;
  m_level5    = m_def->m_level5;
  ReComposite();
}

void
KeywordDlg::CheckType()
{
  if(m_type != "Index")
  {
    m_level2.Empty();
    m_level3.Empty();
    m_level4.Empty();
    m_level5.Empty();
    ReComposite();
    UpdateData(Data2Controls);
  }
}

void
KeywordDlg::CheckWord(CString& p_word)
{
  // Check for illegal characters in the keyword
  int pos = p_word.Find(',');
  if (pos >= 0)
  {
    CString message;
    message.Format("Illegal character ',' in keyword '%s'.\n"
                   "Please remove the comma and try again.", p_word);
    theApp.Panic(message);
    p_word.Empty();
  }
}

void
KeywordDlg::UpdateProperties()
{
  // Set Keyword type
  if(m_def->m_type == "Index")
  {
    m_elem->SetAttribute("name","MS-HKWD",ALWAYS);
  }
  else
  {
    m_elem->SetAttribute("name","MS-HAID",ALWAYS);
  }
  // Set content
  m_def->m_composite = m_composite;
  m_def->m_level1    = m_level1;
  m_def->m_level2    = m_level2;
  m_def->m_level3    = m_level3;
  m_def->m_level4    = m_level4;
  m_def->m_level5    = m_level5;
  m_elem->SetAttribute("content",m_composite,ALWAYS);
}

void
KeywordDlg::ReComposite()
{
  CString composite;
  composite = m_level1;
  if(!m_level1.IsEmpty())
  {
    if(!m_level2.IsEmpty())
    {
      composite += "," + m_level2;
      if(!m_level3.IsEmpty())
      {
        composite += "," + m_level3;
        if(!m_level4.IsEmpty())
        {
          composite += "," + m_level4;
          if(!m_level5.IsEmpty())
          {
            composite += "," + m_level5;
          }
        }
        else
        {
          m_level5.Empty();
        }
      }
      else
      {
        m_level4.Empty();
        m_level5.Empty();
      }
    }
    else
    {
      m_level3.Empty();
      m_level4.Empty();
      m_level5.Empty();
    }
  }
  else
  {
    m_level2.Empty();
    m_level3.Empty();
    m_level4.Empty();
    m_level5.Empty();
  }
  m_composite = composite;
  UpdateData(Data2Controls);
}

void 
KeywordDlg::OnCbnSelchangeLinktype()
{
  int ind = m_comboType.GetCurSel();
  if(ind >= 0)
  {
    CString type = (ind == 0) ? "Index" : "Association";
    m_def->m_type = type;
    m_type = type;
    CheckType();
    UpdateData(Data2Controls);
  }
}

void 
KeywordDlg::OnEnChangeComposite()
{
}

void 
KeywordDlg::OnEnChangeLevel1()
{
  UpdateData();
  CheckWord(m_level1);
  ReComposite();
}

void 
KeywordDlg::OnEnChangeLevel2()
{
  UpdateData();
  CheckWord(m_level2);
  ReComposite();
}

void 
KeywordDlg::OnEnChangeLevel3()
{
  UpdateData();
  CheckWord(m_level3);
  ReComposite();
}

void 
KeywordDlg::OnEnChangeLevel4()
{
  UpdateData();
  CheckWord(m_level4);
  ReComposite();
}

void 
KeywordDlg::OnEnChangeLevel5()
{
  UpdateData();
  CheckWord(m_level5);
  ReComposite();
}

void 
KeywordDlg::OnBnClickedOk()
{
  ReComposite();
  UpdateProperties();
  OnOK();
}

void 
KeywordDlg::OnBnClickedCancel()
{
  OnCancel();
}

void 
KeywordDlg::OnBnClickedHelpKeyword()
{
}

