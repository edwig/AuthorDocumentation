//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    NaviageHyperlink.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for choosing an action on a hyperlink in HTML
//              Follow it, or edit it, or don't show this dialog again
//
#include "stdafx.h"
#include "AuthorHTML.h"
#include "NavigateHyperlink.h"

// NavigateHyperlink dialog

IMPLEMENT_DYNAMIC(NavigateHyperlink, CDialog)
NavigateHyperlink::NavigateHyperlink(bool    bNavigate
                                    ,bool    bShowIt
                                    ,CString sRelation
                                    ,CWnd*   pParent /*=NULL*/)
                	:CDialog(NavigateHyperlink::IDD, pParent)
                	,m_bNavigate(bNavigate)
                	,m_bShowIt(bShowIt)
                	,m_sRelation(sRelation)
{
}

NavigateHyperlink::~NavigateHyperlink()
{
}

void NavigateHyperlink::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_NAV_NAVIGATE, m_buttonNavigate);
	DDX_Control(pDX,IDC_NAV_EDIT,     m_buttonEdit);
	DDX_Control(pDX,IDC_HYPERLINK_REL,m_relationCombo);
	DDX_Control(pDX,IDC_NAV_SHOWAGAIN,m_dontShowAgain);

  DDX_Control(pDX,IDOK,             m_buttonOK);
  DDX_Control(pDX,IDCANCEL,         m_buttonCancel);
  DDX_Control(pDX,IDC_NAV_HELP,     m_buttonHelp);
}

BEGIN_MESSAGE_MAP(NavigateHyperlink, CDialog)
  ON_BN_CLICKED(IDC_NAV_HELP,     OnBnClickedNavHelp)
  ON_BN_CLICKED(IDC_NAV_NAVIGATE, OnBnClickedNavNavigate)
  ON_BN_CLICKED(IDC_NAV_EDIT,     OnBnClickedNavEdit)
  ON_BN_CLICKED(IDC_NAV_SHOWAGAIN,OnBnClickedNavShowagain)
END_MESSAGE_MAP()

BOOL
NavigateHyperlink::OnInitDialog()
{
  CDialog::OnInitDialog();
  int ind = GetRelationIndex(m_sRelation);
  if(ind >= 0)
  {
    m_relationCombo.SetCurSel(ind);
  }
  m_buttonNavigate.SetCheck(m_bNavigate == TRUE);
  m_buttonEdit    .SetCheck(m_bNavigate == FALSE);
  m_dontShowAgain .SetCheck(m_bShowIt   == FALSE);
  return TRUE;
}

// Appendix;Chapter;Contents;Copyright;Glossary;Help;Index;Next;Prev;Section;Start;Subsection
int
NavigateHyperlink::GetRelationIndex(CString& rel)
{
  int index = -1;
       if(rel == "Appendix")     index = 0;
  else if(rel == "Chapter")      index = 1;
  else if(rel == "Contents")     index = 2;
  else if(rel == "Copyright")    index = 3;
  else if(rel == "Glossary")     index = 4;
  else if(rel == "Help")         index = 5;
  else if(rel == "Index")        index = 6;
  else if(rel == "Next")         index = 7;
  else if(rel == "Prev")         index = 8;
  else if(rel == "Section")      index = 9;
  else if(rel == "Start")        index = 10;
  else if(rel == "Subsection")   index = 11;
  return index;
}

// NavigateHyperlink message handlers

void NavigateHyperlink::OnBnClickedNavHelp()
{
  // TODO: Add your control notification handler code here
}

void NavigateHyperlink::OnBnClickedNavNavigate()
{
  m_bNavigate = m_buttonNavigate.GetCheck() > 0;
}

void NavigateHyperlink::OnBnClickedNavEdit()
{
  m_bNavigate = ! m_buttonEdit.GetCheck();
}

void NavigateHyperlink::OnBnClickedNavShowagain()
{
  m_bShowIt = ! m_dontShowAgain.GetCheck();
}
