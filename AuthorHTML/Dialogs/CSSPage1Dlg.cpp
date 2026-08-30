//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    CSSPage1Dlg.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for editing CSS properties
//              Shows an overview list of all properties for a specific selector 
//
#include "stdafx.h"
#include "AuthorHTML.h"
#include "CSSPage1Dlg.h"
#include ".\csspage1dlg.h"

// CSSPage1Dlg dialog

IMPLEMENT_DYNAMIC(CSSPage1Dlg, CDialog)

CSSPage1Dlg::CSSPage1Dlg(CWnd* pParent 
                       ,CssStyleSheet* p_css)
	          :CDialog(CSSPage1Dlg::IDD, pParent)
            ,m_css(p_css)
{
}

CSSPage1Dlg::~CSSPage1Dlg()
{
}

void CSSPage1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
  DDX_Control(pDX,IDC_SS_DEF, m_listBox);
  DDX_Control(pDX,IDC_DELETE, m_buttonDelete);
}

BEGIN_MESSAGE_MAP(CSSPage1Dlg, CDialog)
  ON_BN_CLICKED(IDC_DELETE, OnBnClickedDelete)
END_MESSAGE_MAP()

BOOL
CSSPage1Dlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  int tabstops = 100;
  m_listBox.SetTabStops(tabstops);

  return TRUE;
}

void
CSSPage1Dlg::SetSelector(CString selector,CString descriptor)
{
  CString text = selector;
  if(!descriptor.IsEmpty())
  {
    text += CString(" (") + descriptor + ")";
  }
  CWnd* txt = GetDlgItem(IDC_SS_TYPE);
  txt->SetWindowText(text);

  m_selector   = selector;
  m_descriptor = descriptor;

  SetProperties(selector);
  UpdateData(Data2Controls);
}

void
CSSPage1Dlg::SetProperties(CString selector)
{
  string media = "standard";
  string prop  = selector;
  vector<string> properties;
  m_css->GetProperties(media,prop,&properties);

  // Listbox leeg maken
  int total = m_listBox.GetCount();
  for(int ind = 0;ind < total; ++ind)
  {
    m_listBox.DeleteString(0);
  }
  for(unsigned int ind = 0;ind < properties.size(); ++ind)
  {
    string prop2 = properties[ind];
    string value = m_css->get(media,(string)selector,prop2);
    CString text = prop2.c_str();
    if(value.size())
    {
      text += CString("\t: ") + value.c_str();
    }
    m_listBox.AddString(text);
  }
}

// Redisplay properties in list because of changes
// on all other tabs in the dialog
void
CSSPage1Dlg::UpdateProperties()
{
  SetProperties(m_selector);
}

// CSSPage1Dlg message handlers

void 
CSSPage1Dlg::OnBnClickedDelete()
{
  int num = m_listBox.GetCount();
  int ind = m_listBox.GetCurSel();
  if(ind >= 0 && ind < num)
  {
    CString cProp;
    m_listBox.GetText(ind,cProp);
    int pos = cProp.Find('\t');
    if(pos >= 0)
    {
      cProp = cProp.Left(pos);
    }
    string media = "standard";
    string prop  = (string) cProp;

    CString mess;
    mess.Format("Are you sure that you want to delete CSS property [%s]?",cProp.GetString());
    if(theApp.MessageBox(mess,"Delete",MB_YESNO|MB_ICONQUESTION) == IDYES)
    {
      m_css->put(media,(string)m_selector,prop,"");
      UpdateProperties();
      UpdateData(Data2Controls);
    }
  }
}
