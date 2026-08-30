//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    AutoSizerDlg.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog to make the size of the target CHM visible.
//
#include "stdafx.h"
#include "AuthorHTML.h"
#include "AutosizerDlg.h"


// AutosizerDlg dialog

IMPLEMENT_DYNAMIC(AutosizerDlg, CDialog)

AutosizerDlg::AutosizerDlg(CWnd* p_parent
                          ,int p_left,int p_top,int p_width,int p_height)
	           :CDialog(AutosizerDlg::IDD,p_parent)
             ,m_left(p_left)
             ,m_top(p_top)
             ,m_width(p_width)
             ,m_height(p_height)
{
}

AutosizerDlg::~AutosizerDlg()
{
}

void AutosizerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(AutosizerDlg, CDialog)
  ON_BN_CLICKED(IDC_HELP_AUTOSIZER, &AutosizerDlg::OnBnClickedHelpAutosizer)
  ON_BN_CLICKED(IDOK,               &AutosizerDlg::OnBnClickedOk)
  ON_BN_CLICKED(IDCANCEL,           &AutosizerDlg::OnBnClickedCancel)
END_MESSAGE_MAP()

BOOL
AutosizerDlg::OnInitDialog()
{
  CDialog::OnInitDialog();
  // To get a resizable dialog, add min- and maxbox at creation time
  // and later on (now) remove them again, together with the modalframe
  ModifyStyle(DS_MODALFRAME|WS_MAXIMIZEBOX|WS_MINIMIZEBOX,WS_POPUP|WS_THICKFRAME);
  SetWindowPos(&CWnd::wndNoTopMost,m_left,m_top,m_width,m_height,SWP_SHOWWINDOW|SWP_DRAWFRAME);
  return TRUE;
}

// AutosizerDlg message handlers

void 
AutosizerDlg::OnBnClickedOk()
{
  // Record last size and pos
  CRect rect;
  GetWindowRect(&rect);
  m_left   = rect.left;
  m_top    = rect.top;
  m_width  = rect.Width();
  m_height = rect.Height();

  OnOK();
}

void AutosizerDlg::OnBnClickedCancel()
{
  // TODO: Add your control notification handler code here
  OnCancel();
}

void AutosizerDlg::OnBnClickedHelpAutosizer()
{
  // TODO: Add your control notification handler code here
}

