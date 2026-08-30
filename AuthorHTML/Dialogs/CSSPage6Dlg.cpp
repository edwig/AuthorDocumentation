//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    CSSPage6Dlg.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for editing CSS properties
//              Handles the list-style attributes
//
#include "stdafx.h"
#include "AuthorHTML.h"
#include "FileDialog.h"
#include "StyleSheetDlg.h"
#include "CSSPage6Dlg.h"
#include "CSS.h"
#include "Misc.h"

// CSSPage6Dlg dialog

IMPLEMENT_DYNAMIC(CSSPage6Dlg, CDialog)

CSSPage6Dlg::CSSPage6Dlg(CWnd* pParent,CssStyleSheet* pCss)
           	:CDialog(CSSPage6Dlg::IDD, pParent)
            ,m_css(pCss)
{
}

CSSPage6Dlg::~CSSPage6Dlg()
{
}

void CSSPage6Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
  DDX_Control(pDX,IDC_LISTSTYLE,    m_comboStyle);
  DDX_Control(pDX,IDC_STYLEPOS,     m_comboPosition);
  DDX_Text   (pDX,IDC_STYLEIMAGE,   m_listImage);
  DDX_Control(pDX,IDC_BUT_LISTIMAGE,m_buttonImage);

  if(pDX->m_bSaveAndValidate == Data2Controls)
  {
    int ind;
    CString desc = Misc::GetAttributeDisplayname("list-style-type",m_listStyle);
    ind = m_comboStyle.FindString(-1,desc);
    m_comboStyle.SetCurSel(ind);

    desc = Misc::GetAttributeDisplayname("list-style-position",m_listPosition);
    ind = m_comboPosition.FindString(-1,desc);
    m_comboPosition.SetCurSel(ind);
  }
}

BEGIN_MESSAGE_MAP(CSSPage6Dlg, CDialog)
  ON_CBN_SELCHANGE(IDC_LISTSTYLE,     OnCbnSelchangeListstyle)
  ON_CBN_SELCHANGE(IDC_STYLEPOS,      OnCbnSelchangeStylepos)
  ON_EN_KILLFOCUS (IDC_STYLEIMAGE,    OnEnChangeStyleimage)
  ON_BN_CLICKED   (IDC_BUT_LISTIMAGE, OnBnClickedButListimage)
END_MESSAGE_MAP()

BOOL
CSSPage6Dlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  // Fill combo repeat
  vector<string> all;
  Misc::GetAllAttributeDisplaynames("list-style-type",&all);
  for(unsigned int ind=0; ind<all.size(); ++ind)
  {
    m_comboStyle.AddString(all[ind].c_str());
  }
  all.clear();
  Misc::GetAllAttributeDisplaynames("list-style-position",&all);
  for(unsigned int ind=0; ind<all.size(); ++ind)
  {
    m_comboPosition.AddString(all[ind].c_str());
  }
  FillPage();

  return TRUE;
}

void
CSSPage6Dlg::SetSelector(string selector)
{
  m_selector = selector;
  FillPage();
}

void
CSSPage6Dlg::FillPage()
{
  m_listStyle    = CSSPropertyGet(m_css,m_selector,"list-style-type",    true);
  m_listPosition = CSSPropertyGet(m_css,m_selector,"list-style-position",true);
  m_listImage    = CSSPropertyGet(m_css,m_selector,"list-style-image",   false);

  Redisplay();
}

void 
CSSPage6Dlg::UpdateProperties()
{
  CSSPropertyPut(m_css,m_selector,"list-style-type",    m_listStyle,   true);
  CSSPropertyPut(m_css,m_selector,"list-style-position",m_listPosition,true);
  CSSPropertyPut(m_css,m_selector,"list-style-image",   m_listImage,   false);

  StyleSheetDlg* dlg = (StyleSheetDlg*)GetParent();
  dlg->SetCanApply();
}

void 
CSSPage6Dlg::Redisplay()
{
  // Display all gotten properties
  UpdateData(Data2Controls);
}
  
// CSSPage6Dlg message handlers

void CSSPage6Dlg::OnCbnSelchangeListstyle()
{
  int ind = m_comboStyle.GetCurSel();
  if(ind >= 0)
  {
    CString desc;
    m_comboStyle.GetLBText(ind,desc);
    m_listStyle = Misc::GetAttributeValue("list-style-type",desc);
  }
  UpdateProperties();
  Redisplay();
}

void CSSPage6Dlg::OnCbnSelchangeStylepos()
{
  int ind = m_comboPosition.GetCurSel();
  if(ind >= 0)
  {
    CString desc;
    m_comboPosition.GetLBText(ind,desc);
    m_listPosition = Misc::GetAttributeValue("list-style-position",desc);
  }
  UpdateProperties();
  Redisplay();
}

void CSSPage6Dlg::OnEnChangeStyleimage()
{
  CWnd* wnd = GetDlgItem(IDC_STYLEIMAGE);
  wnd->GetWindowText(m_listImage);
  UpdateProperties();
  Redisplay();
}

void CSSPage6Dlg::OnBnClickedButListimage()
{
  DocFileDialog diag(true                         // true = open
                    ,"Select a list style image"  // title
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
    m_listImage = diag.GetChosenFile();
    CString base = ((StyleSheetDlg*)GetParent())->GetBaseDirectory();
    CString relative;
    if(Misc::MakeRelativePathname(base,m_listImage,relative))
    {
      m_listImage = relative;
    }
    UpdateProperties();
    Redisplay();
  }
}
