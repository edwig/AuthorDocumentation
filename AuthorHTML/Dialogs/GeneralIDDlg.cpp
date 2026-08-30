//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    GeneralIDDlg.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for managing the ID attribute in HTML, 
//
#include "stdafx.h"
#include "AuthorHTML.h"
#include "GeneralIDDlg.h"

// GeneralIDDlg dialog

IMPLEMENT_DYNAMIC(GeneralIDDlg, CDialog)
GeneralIDDlg::GeneralIDDlg(CWnd* pParent,CString type,HtmlElement* elem)
             :CDialog(GeneralIDDlg::IDD, pParent)
             ,m_elem(elem)
{
  m_id        = m_elem->GetAttribute("id");
  m_className = m_elem->GetAttribute("className");
  CString desc = Misc::GetTagDescription(type);
  type.MakeUpper();
  m_type = type + " : " + desc;
}

GeneralIDDlg::~GeneralIDDlg()
{
}

void GeneralIDDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
  DDX_Text   (pDX,IDC_ID_TYPE,    m_type);
  DDX_Text   (pDX,IDC_ID,         m_id);
  DDX_Text   (pDX,IDC_CLASSNAME,  m_className);
  DDX_Control(pDX,IDOK,           m_buttonOK);
  DDX_Control(pDX,IDCANCEL,       m_buttonCancel);
  DDX_Control(pDX,IDC_ID_HELP,    m_buttonHelp);
}

BEGIN_MESSAGE_MAP(GeneralIDDlg, CDialog)
  ON_EN_KILLFOCUS(IDC_ID,         OnEnChangeId)
  ON_EN_KILLFOCUS(IDC_CLASSNAME,  OnEnChangeClassname)
  ON_BN_CLICKED  (IDOK,           OnBnClickedOk)
  ON_BN_CLICKED  (IDCANCEL,       OnBnClickedCancel)
  ON_BN_CLICKED  (IDC_ID_HELP,    OnBnClickedHelp)
END_MESSAGE_MAP()

void
GeneralIDDlg::UpdateProperties()
{
  m_elem->SetAttribute("id",m_id);
  m_elem->SetAttribute("className",m_className);
}

bool
GeneralIDDlg::CheckID()
{
  bool ret = false;
  if(m_id.IsEmpty())
  {
    ret = true;
  }
  else
  {
    if(isalpha(m_id[0]))
    {
      ret = true;
      for(int n = 1; n < m_id.GetLength(); ++ n)
      {
        if(m_id[n] != '_' && !isalnum(m_id[n]))
        {
          ret = false;
          break;
        }
      }
    }
  }
  if(!ret)
  {
    theApp.MessageBox("Unique ID must be an identifier (alfanumeric characters or '_')\n"
                      "and must begin with a alphabetic character"
                      ,"Error"
                      ,MB_OK|MB_TASKMODAL|MB_ICONHAND);
  }
  return ret;
}

// GeneralIDDlg message handlers

void 
GeneralIDDlg::OnEnChangeId()
{
  CWnd* w = GetDlgItem(IDC_ID);
  w->GetWindowText(m_id);
  if(!m_id.IsEmpty())
  {
    CheckID();
  }
}

void GeneralIDDlg::OnEnChangeClassname()
{
  CWnd* w = GetDlgItem(IDC_CLASSNAME);
  w->GetWindowText(m_className);
}

void GeneralIDDlg::OnBnClickedOk()
{
  if(!CheckID())
  {
    return;
  }
  UpdateProperties();
  OnOK();
}

void GeneralIDDlg::OnBnClickedCancel()
{
  OnCancel();
}

void GeneralIDDlg::OnBnClickedHelp()
{
  // TODO: Add your control notification handler code here
}
