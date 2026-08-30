//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    WindowDefPage4.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for all window properties page 4 of 5
//
#include "stdafx.h"
#include "AuthorHTML.h"
#include "WindowDefPage4.h"
#include "AutosizerDlg.h"
#include <io.h>
#include <shlobj.h>

// WindowDefPage4 dialog

IMPLEMENT_DYNAMIC(WindowDefPage4, CDialog)

WindowDefPage4::WindowDefPage4(CWnd*        p_parent
                              ,ProjectFile* p_project)
               :CDialog(WindowDefPage4::IDD,p_parent)
               ,m_project(p_project)
               ,m_window(NULL)
{
  m_remember = false;
  m_left     = 0;
  m_top      = 0;
  m_width    = 0;
  m_height   = 0;
  m_right    = 0;
  m_bottom   = 0;
}

WindowDefPage4::~WindowDefPage4()
{
}

void WindowDefPage4::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
  DDX_Control(pDX,IDC_REMEMBER, m_buttonRemember);
  DDX_Text   (pDX,IDC_LEFT,     m_left);
  DDX_Text   (pDX,IDC_TOP,      m_top);
  DDX_Text   (pDX,IDC_WIDTH,    m_width);
  DDX_Text   (pDX,IDC_HEIGHT,   m_height);
  DDX_Text   (pDX,IDC_RIGHT,    m_right);
  DDX_Text   (pDX,IDC_BOTTOM,   m_bottom);
  DDX_Control(pDX,IDC_AUTOSIZER,m_buttonAutosizer);
  DDX_Control(pDX,IDC_DEFAULTS, m_buttonDefaults);
  DDX_Control(pDX,IDC_RESET,    m_buttonReset);
}

BEGIN_MESSAGE_MAP(WindowDefPage4, CDialog)
  ON_BN_CLICKED  (IDC_REMEMBER, &WindowDefPage4::OnBnClickedRemember)
  ON_EN_KILLFOCUS(IDC_LEFT,     &WindowDefPage4::OnEnChangeLeft)
  ON_EN_KILLFOCUS(IDC_TOP,      &WindowDefPage4::OnEnChangeTop)
  ON_EN_KILLFOCUS(IDC_WIDTH,    &WindowDefPage4::OnEnChangeWidth)
  ON_EN_KILLFOCUS(IDC_HEIGHT,   &WindowDefPage4::OnEnChangeHeight)
  ON_BN_CLICKED  (IDC_AUTOSIZER,&WindowDefPage4::OnBnClickedAutosizer)
  ON_BN_CLICKED  (IDC_DEFAULTS, &WindowDefPage4::OnBnClickedDefaults)
  ON_BN_CLICKED  (IDC_RESET,    &WindowDefPage4::OnBnClickedReset)
END_MESSAGE_MAP()

BOOL
WindowDefPage4::OnInitDialog()
{
  CDialog::OnInitDialog();
  return TRUE;
}

void
WindowDefPage4::SetProperties(WindowDefinition* p_window)
{
  m_window = p_window;

  m_remember = p_window->GetNavPaneOption(NAVPANE_SIZE_POS);
  p_window->GetWindowSizePos(m_left,m_top,m_right,m_bottom);
  m_width  = m_right  - m_left + 1;
  m_height = m_bottom - m_top  + 1;

  ShowProperties();
}

void
WindowDefPage4::ShowProperties()
{
  m_buttonRemember.SetCheck(m_remember);
  UpdateData(FALSE);  
}

void
WindowDefPage4::UpdateProperties()
{
  m_window->SetNavPaneOption(NAVPANE_SIZE_POS,m_remember);
  m_window->SetWindowSizePos(m_left,m_top,m_right,m_bottom);
}

// WindowDefPage4 message handlers

void 
WindowDefPage4::OnBnClickedRemember()
{
  m_remember = m_buttonRemember.GetCheck() == TRUE;
  ShowProperties();
}

void 
WindowDefPage4::OnEnChangeLeft()
{
  UpdateData(TRUE);
  m_right = m_left + m_width - 1;
  UpdateData(FALSE);
}

void 
WindowDefPage4::OnEnChangeTop()
{
  UpdateData(TRUE);
  m_bottom = m_top + m_height - 1;
  UpdateData(FALSE);
}

void 
WindowDefPage4::OnEnChangeWidth()
{
  UpdateData(TRUE);
  m_right = m_left + m_width - 1;
  UpdateData(FALSE);
}

void 
WindowDefPage4::OnEnChangeHeight()
{
  UpdateData(TRUE);
  m_bottom = m_top + m_height - 1;
  UpdateData(FALSE);
}

void 
WindowDefPage4::OnBnClickedAutosizer()
{
  AutosizerDlg dlg(this,m_left,m_top,m_width,m_height);
  if(dlg.DoModal() == IDOK)
  {
    // Repositioned
    m_left   = dlg.GetLeft();
    m_top    = dlg.GetTop();
    m_width  = dlg.GetWidth();
    m_height = dlg.GetHeight();
    m_right  = m_left + m_width - 1;
    m_bottom = m_top + m_height - 1;
    UpdateData(FALSE);
  }
}

void WindowDefPage4::OnBnClickedDefaults()
{
  int width = GetSystemMetrics (SM_CXFULLSCREEN);
  int height = GetSystemMetrics(SM_CYFULLSCREEN);
  m_width  = width / 4;
  m_height = height / 4;
  m_left   = m_width;
  m_top    = m_height;
  m_right  = m_left + m_width;
  m_bottom = m_top  + m_height;
  UpdateData(FALSE);
}

void 
WindowDefPage4::OnBnClickedReset()
{
  char szPath[MAX_PATH];
  if(SHGetFolderPath(m_hWnd         // For UI remote connections
                    ,CSIDL_APPDATA  // Typically "C:\Documents and Settings\%Username\Application data\"
                    ,NULL           // Default user (no impersonations)
                    ,0
                    ,(LPSTR)szPath))
  {
    // Cannot find this directory
    return;
  }
  strcat(szPath,"\\Microsoft\\HTML Help\\hh.dat");
  if(_access(szPath,0) == 0)
  {
    if(unlink(szPath))
    {
      theApp.ErrorMessage("Cannot erase the system-wide 'hh.dat' file with HTML window positions");
    }
  }
}
