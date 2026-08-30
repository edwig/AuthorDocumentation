//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    Sweep.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Sweep dialog for showing progress of a long process
//              e.g. for initial reading the project, or compiling the project
//
#include "stdafx.h"
#include "AuthorHTML.h"
#include "SweepDlg.h"
#include "resource.h"

BEGIN_MESSAGE_MAP(SweepDlg,CDialog)
END_MESSAGE_MAP()

SweepDlg::SweepDlg(CWnd* pParent,bool show)
         :CDialog(SweepDlg::IDD, pParent)
         ,m_max(100)
         ,m_current(0)
{
  m_show = show ? SW_SHOW : SW_HIDE;
}

SweepDlg::~SweepDlg()
{
  Reset();
}

void
SweepDlg::DoTheInit()
{
  Create(IDD_SWEEP,NULL);
  ShowWindow(m_show);
  CenterWindow(theApp.m_pMainWnd);
  SetIcon(AfxGetApp()->LoadIcon(MAKEINTRESOURCE(IDR_MAINFRAME)),FALSE);
}

BOOL
SweepDlg::OnInitDialog()
{
  CDialog::OnInitDialog();
  UpdateData(false);
  return TRUE;
}

void 
SweepDlg::Reset()
{
  m_gauge.SetPos(0);
  SetText("");
  UpdateData(false);
  ShowWindow(m_show);
  Repaint();
  // Now hide it explicitly
  ShowWindow(SW_HIDE);
}

void 
SweepDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
  if(!pDX->m_bSaveAndValidate)
  {
    DDX_Text   (pDX,IDC_RELAY_TEXT,m_showText);
    DDX_Control(pDX,IDC_PROGRESS,  m_gauge);
  }
}

void
SweepDlg::SetCurrent(int p_current)
{
  if(p_current > m_current && p_current <= m_max)
  {
    m_gauge.SetPos(m_current = p_current);
  }
  MakeTextToShow();
  UpdateData(false);
  ShowWindow(m_show);
  Repaint();
}

void
SweepDlg::SetMax(int p_max)
{
  if(p_max > 5 && p_max < 32000)
  {
    m_max = p_max;
  }
  // Set the gauge's maximum and reset it
  m_gauge.SetRange(0,m_max);
  m_gauge.SetPos(0);
  UpdateData(false);
  ShowWindow(m_show);
  Repaint();
}

void
SweepDlg::Increment()
{
  if(m_current < m_max)
  {
    m_gauge.SetPos(++m_current);
  }
  MakeTextToShow();
  UpdateData(false);
  ShowWindow(m_show);
  Repaint();
}

void 
SweepDlg::SetAtEnd()
{
  m_current = m_max;
  MakeTextToShow();
  m_gauge.SetPos(m_max);
  UpdateData(false);
  ShowWindow(m_show);
  Repaint();
}

void
SweepDlg::SetText(CString p_text)
{
  m_text = p_text;
  MakeTextToShow();
  UpdateData(false);
  CWnd* tt = GetDlgItem(IDC_RELAY_TEXT);
  tt->ShowWindow(m_show);
  ShowWindow(m_show);
  Repaint();
}

void 
SweepDlg::MakeTextToShow()
{
  int percent = 0;
  if(m_max > 0)
  {
    percent = (100 * m_current) / m_max;
  }
  // Now processing "[22/201] = 10% text.html"
  m_showText.Format("[%d/%d = %d%%] %s",m_current,m_max,percent,m_text.GetString());
}

void
SweepDlg::Repaint()
{
  // Only the paint messages for the big processes
  // where the application must be visible (again)
  // Potentially it's an endless loop, so while its non critical
  // we add a timing limit on the loop
  MSG msg;
  UINT ticks = GetTickCount();
  while(GetTickCount() - ticks < 100 && (
        PeekMessage(&msg, NULL, WM_PAINT,     WM_PAINT,     PM_REMOVE) ||
        PeekMessage(&msg, NULL, WM_SYSCOMMAND,WM_SYSCOMMAND,PM_REMOVE) ))
  {
    try
    {
      ::TranslateMessage(&msg);
      ::DispatchMessage(&msg);
    }
    catch(...)
    {
      // What???
    }
  }
}
