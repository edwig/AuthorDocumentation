//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    WindowDefPage3.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for all window properties page 3 of 5
//
#include "stdafx.h"
#include "AuthorHTML.h"
#include "WindowDefPage3.h" 

// WindowDefPage3 dialog

IMPLEMENT_DYNAMIC(WindowDefPage3, CDialog)

WindowDefPage3::WindowDefPage3(CWnd*        p_parent
                              ,ProjectFile* p_project)
               :CDialog(WindowDefPage3::IDD,p_parent)
               ,m_project(p_project)
               ,m_window(NULL)
{
  m_maximizeBox   = false;
  m_maximize      = false;
  m_popup         = false;
  m_sysmenu       = false;
  m_clipChildren  = false;
  m_clipSiblings  = false;
  m_minimizeBox   = false;
  m_minimize      = false;
  m_hscroll       = false;
  m_vscroll       = false;
  m_child         = false;
  m_border        = false;
  m_dlgFrame      = false;
  m_visible       = false;
  m_overlapped    = false;
  m_thkFrame      = false;
  m_disabled      = false;
  // Extended
  m_tool          = false;
  m_clientEdge    = false;
  m_staticEdge    = false;
  m_rightAligned  = false;
  m_rtl           = false;
  m_controlParent = false;
  m_acceptFiles   = false;
  m_topmost       = false;
  m_contextHelp   = false;
  m_noNotify      = false;
  m_lscroll       = false;
  m_transparent   = false;
}

WindowDefPage3::~WindowDefPage3()
{
}

void WindowDefPage3::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
  // Styles
  DDX_Control(pDX,IDC_MAXBOX,       m_buttonMaxBox);
  DDX_Control(pDX,IDC_MAXIMIZED,    m_buttonMaximize);
  DDX_Control(pDX,IDC_POPUP,        m_buttonPopup);
  DDX_Control(pDX,IDC_SYSMENU,      m_buttonSysmenu);
  DDX_Control(pDX,IDC_CLIPCHILD,    m_buttonClipChild);
  DDX_Control(pDX,IDC_CLIPSIBLINGS, m_buttonClipSib);
  DDX_Control(pDX,IDC_MINBOX,       m_buttonMinBox);
  DDX_Control(pDX,IDC_MINIMIZED,    m_buttonMinimize);
  DDX_Control(pDX,IDC_HSCROLL,      m_buttonHscroll);
  DDX_Control(pDX,IDC_VSCROLL,      m_buttonVscroll);
  DDX_Control(pDX,IDC_CHILD,        m_buttonChild);
  DDX_Control(pDX,IDC_BORDER,       m_buttonBorder);
  DDX_Control(pDX,IDC_DLGFRAME,     m_buttonDlgFrame);
  DDX_Control(pDX,IDC_VISIBLE,      m_buttonVisible);
  DDX_Control(pDX,IDC_OVERLAPPED,   m_buttonOverlapped);
  DDX_Control(pDX,IDC_THICKFRAME,   m_buttonThkFrame);
  DDX_Control(pDX,IDC_DISABLED,     m_buttonDisabled);
  // Extended
  DDX_Control(pDX,IDC_TOOL,         m_buttonTool);
  DDX_Control(pDX,IDC_CLIENTEDGE,   m_buttonClientEdge);
  DDX_Control(pDX,IDC_STATICEDGE,   m_buttonStaticEdge);
  DDX_Control(pDX,IDC_RIGHTALIGNED, m_buttonRightAligned);
  DDX_Control(pDX,IDC_RTL,          m_buttonRTL);
  DDX_Control(pDX,IDC_CONTROLPARENT,m_buttonCtrlParent);
  DDX_Control(pDX,IDC_ACCEPTFILES,  m_buttonAcceptFiles);
  DDX_Control(pDX,IDC_TOPMOST,      m_buttonTopmost);
  DDX_Control(pDX,IDC_CONTEXTHELP,  m_buttonContextHelp);
  DDX_Control(pDX,IDC_NONOTIFY,     m_buttonNoNotify);
  DDX_Control(pDX,IDC_LSCROLL,      m_buttonLScroll);
  DDX_Control(pDX,IDC_TRANSPARENT,  m_buttonTransparent);
}


BEGIN_MESSAGE_MAP(WindowDefPage3, CDialog)
  ON_BN_CLICKED(IDC_MAXBOX,       &WindowDefPage3::OnBnClickedMaxbox)
  ON_BN_CLICKED(IDC_MAXIMIZED,    &WindowDefPage3::OnBnClickedMaximized)
  ON_BN_CLICKED(IDC_POPUP,        &WindowDefPage3::OnBnClickedPopup)
  ON_BN_CLICKED(IDC_SYSMENU,      &WindowDefPage3::OnBnClickedSysmenu)
  ON_BN_CLICKED(IDC_CLIPCHILD,    &WindowDefPage3::OnBnClickedClipchild)
  ON_BN_CLICKED(IDC_CLIPSIBLINGS, &WindowDefPage3::OnBnClickedClipsiblings)
  ON_BN_CLICKED(IDC_MINBOX,       &WindowDefPage3::OnBnClickedMinbox)
  ON_BN_CLICKED(IDC_MINIMIZED,    &WindowDefPage3::OnBnClickedMinimized)
  ON_BN_CLICKED(IDC_HSCROLL,      &WindowDefPage3::OnBnClickedHscroll)
  ON_BN_CLICKED(IDC_VSCROLL,      &WindowDefPage3::OnBnClickedVscroll)
  ON_BN_CLICKED(IDC_CHILD,        &WindowDefPage3::OnBnClickedChild)
  ON_BN_CLICKED(IDC_BORDER,       &WindowDefPage3::OnBnClickedBorder)
  ON_BN_CLICKED(IDC_DLGFRAME,     &WindowDefPage3::OnBnClickedDlgframe)
  ON_BN_CLICKED(IDC_VISIBLE,      &WindowDefPage3::OnBnClickedVisible)
  ON_BN_CLICKED(IDC_OVERLAPPED,   &WindowDefPage3::OnBnClickedOverlapped)
  ON_BN_CLICKED(IDC_THICKFRAME,   &WindowDefPage3::OnBnClickedThickframe)
  ON_BN_CLICKED(IDC_DISABLED,     &WindowDefPage3::OnBnClickedDisabled)
  // Extended
  ON_BN_CLICKED(IDC_TOOL,         &WindowDefPage3::OnBnClickedTool)
  ON_BN_CLICKED(IDC_CLIENTEDGE,   &WindowDefPage3::OnBnClickedClientedge)
  ON_BN_CLICKED(IDC_STATICEDGE,   &WindowDefPage3::OnBnClickedStaticedge)
  ON_BN_CLICKED(IDC_RIGHTALIGNED, &WindowDefPage3::OnBnClickedRightaligned)
  ON_BN_CLICKED(IDC_RTL,          &WindowDefPage3::OnBnClickedRtl)
  ON_BN_CLICKED(IDC_CONTROLPARENT,&WindowDefPage3::OnBnClickedControlparent)
  ON_BN_CLICKED(IDC_ACCEPTFILES,  &WindowDefPage3::OnBnClickedAcceptfiles)
  ON_BN_CLICKED(IDC_TOPMOST,      &WindowDefPage3::OnBnClickedTopmost)
  ON_BN_CLICKED(IDC_CONTEXTHELP,  &WindowDefPage3::OnBnClickedContexthelp)
  ON_BN_CLICKED(IDC_NONOTIFY,     &WindowDefPage3::OnBnClickedNonotify)
  ON_BN_CLICKED(IDC_LSCROLL,      &WindowDefPage3::OnBnClickedLscroll)
  ON_BN_CLICKED(IDC_TRANSPARENT,  &WindowDefPage3::OnBnClickedTransparent)
END_MESSAGE_MAP()

BOOL
WindowDefPage3::OnInitDialog()
{
  CDialog::OnInitDialog();

  return TRUE;
}

void
WindowDefPage3::SetProperties(WindowDefinition* p_window)
{
  m_window = p_window;

  m_maximizeBox   = m_window->GetWinStyle(STYLES_MAXIMIZEBOX);
  m_maximize      = m_window->GetWinStyle(STYLES_MAXIMIZE);
  m_popup         = m_window->GetWinStyle(STYLES_POPUP);
  m_sysmenu       = m_window->GetWinStyle(STYLES_SYSTEMMENU);
  m_clipChildren  = m_window->GetWinStyle(STYLES_CLIPCHILDREN);
  m_clipSiblings  = m_window->GetWinStyle(STYLES_CLIPSIBLINGS);
  m_minimizeBox   = m_window->GetWinStyle(STYLES_MINIMIZEBOX);
  m_minimize      = m_window->GetWinStyle(STYLES_MINIMIZE);
  m_hscroll       = m_window->GetWinStyle(STYLES_HORZSCROLL);
  m_vscroll       = m_window->GetWinStyle(STYLES_VERTSCROLL);
  m_child         = m_window->GetWinStyle(STYLES_CHILD);
  m_border        = m_window->GetWinStyle(STYLES_BORDER);
  m_dlgFrame      = m_window->GetWinStyle(STYLES_DLGFRAME);
  m_visible       = m_window->GetWinStyle(STYLES_VISIBLE);
  m_overlapped    = m_window->GetWinStyle(STYLES_OVERLAPPED);
  m_thkFrame      = m_window->GetWinStyle(STYLES_THICK);
  m_disabled      = m_window->GetWinStyle(STYLES_DISABLED);
  // Extended
  m_tool          = m_window->GetWinExStyle(STYLES_TOOLWIN);
  m_clientEdge    = m_window->GetWinExStyle(STYLES_CLIENTEDGE);
  m_staticEdge    = m_window->GetWinExStyle(STYLES_STATICEDGE);
  m_rightAligned  = m_window->GetWinExStyle(STYLES_RIGHTALIGNED);
  m_rtl           = m_window->GetWinExStyle(STYLES_RTL);
  m_controlParent = m_window->GetWinExStyle(STYLES_CONTROLPAR);
  m_acceptFiles   = m_window->GetWinExStyle(STYLES_ACCEPTFILES);
  m_topmost       = m_window->GetWinExStyle(STYLES_TOPMOST);
  m_contextHelp   = m_window->GetWinExStyle(STYLES_CONTEXTHELP);
  m_noNotify      = m_window->GetWinExStyle(STYLES_NOPARNOTIFY);
  m_lscroll       = m_window->GetWinExStyle(STYLES_LEFTSCROLL);
  m_transparent   = m_window->GetWinExStyle(STYLES_TRANSPARENT);

  ShowProperties();
}

void
WindowDefPage3::ShowProperties()
{
  m_buttonMaxBox    .SetCheck(m_maximizeBox);
  m_buttonMaximize  .SetCheck(m_maximize);
  m_buttonPopup     .SetCheck(m_popup);
  m_buttonSysmenu   .SetCheck(m_sysmenu);
  m_buttonClipChild .SetCheck(m_clipChildren);
  m_buttonClipSib   .SetCheck(m_clipSiblings);
  m_buttonMinBox    .SetCheck(m_minimizeBox);
  m_buttonMinimize  .SetCheck(m_minimize);
  m_buttonHscroll   .SetCheck(m_hscroll);
  m_buttonVscroll   .SetCheck(m_vscroll);
  m_buttonChild     .SetCheck(m_child);
  m_buttonBorder    .SetCheck(m_border);
  m_buttonDlgFrame  .SetCheck(m_dlgFrame);
  m_buttonVisible   .SetCheck(m_visible);
  m_buttonOverlapped.SetCheck(m_overlapped);
  m_buttonThkFrame  .SetCheck(m_thkFrame);
  m_buttonDisabled  .SetCheck(m_disabled);
  //Extended
  m_buttonTool        .SetCheck(m_tool);
  m_buttonClientEdge  .SetCheck(m_clientEdge);
  m_buttonStaticEdge  .SetCheck(m_staticEdge);
  m_buttonRightAligned.SetCheck(m_rightAligned);
  m_buttonRTL         .SetCheck(m_rtl);
  m_buttonCtrlParent  .SetCheck(m_controlParent);
  m_buttonAcceptFiles .SetCheck(m_acceptFiles);
  m_buttonTopmost     .SetCheck(m_topmost);
  m_buttonContextHelp .SetCheck(m_contextHelp);
  m_buttonNoNotify    .SetCheck(m_noNotify);
  m_buttonLScroll     .SetCheck(m_lscroll);
  m_buttonTransparent .SetCheck(m_transparent);
}

void
WindowDefPage3::UpdateProperties()
{
  m_window->SetWinStyle(STYLES_MAXIMIZEBOX, m_maximizeBox);
  m_window->SetWinStyle(STYLES_MAXIMIZE,    m_maximize);
  m_window->SetWinStyle(STYLES_POPUP,       m_popup);
  m_window->SetWinStyle(STYLES_SYSTEMMENU,  m_sysmenu);
  m_window->SetWinStyle(STYLES_CLIPCHILDREN,m_clipChildren);
  m_window->SetWinStyle(STYLES_CLIPSIBLINGS,m_clipSiblings);
  m_window->SetWinStyle(STYLES_MINIMIZEBOX, m_minimizeBox);
  m_window->SetWinStyle(STYLES_MINIMIZE,    m_minimize);
  m_window->SetWinStyle(STYLES_HORZSCROLL,  m_hscroll);
  m_window->SetWinStyle(STYLES_VERTSCROLL,  m_vscroll);
  m_window->SetWinStyle(STYLES_CHILD,       m_child);
  m_window->SetWinStyle(STYLES_BORDER,      m_border);
  m_window->SetWinStyle(STYLES_DLGFRAME,    m_dlgFrame);
  m_window->SetWinStyle(STYLES_VISIBLE,     m_visible);
  m_window->SetWinStyle(STYLES_OVERLAPPED,  m_overlapped);
  m_window->SetWinStyle(STYLES_THICK,       m_thkFrame);
  m_window->SetWinStyle(STYLES_DISABLED,    m_disabled);
  // Extended
  m_window->SetWinExStyle(STYLES_TOOLWIN,     m_tool);
  m_window->SetWinExStyle(STYLES_CLIENTEDGE,  m_clientEdge);
  m_window->SetWinExStyle(STYLES_STATICEDGE,  m_staticEdge);
  m_window->SetWinExStyle(STYLES_RIGHTALIGNED,m_rightAligned);
  m_window->SetWinExStyle(STYLES_RTL,         m_rtl);
  m_window->SetWinExStyle(STYLES_CONTROLPAR,  m_controlParent);
  m_window->SetWinExStyle(STYLES_ACCEPTFILES, m_acceptFiles);
  m_window->SetWinExStyle(STYLES_TOPMOST,     m_topmost);
  m_window->SetWinExStyle(STYLES_CONTEXTHELP, m_contextHelp);
  m_window->SetWinExStyle(STYLES_NOPARNOTIFY, m_noNotify);
  m_window->SetWinExStyle(STYLES_LEFTSCROLL,  m_lscroll);
  m_window->SetWinExStyle(STYLES_TRANSPARENT, m_transparent);
}

// WindowDefPage3 message handlers

void 
WindowDefPage3::OnBnClickedMaxbox()
{
  m_maximizeBox = m_buttonMaxBox.GetCheck() == TRUE;
  ShowProperties();
}

void 
WindowDefPage3::OnBnClickedMaximized()
{
  m_maximize = m_buttonMaximize.GetCheck() == TRUE;
  ShowProperties();
}

void 
WindowDefPage3::OnBnClickedPopup()
{
  m_popup = m_buttonPopup.GetCheck() == TRUE;
  ShowProperties();
}

void 
WindowDefPage3::OnBnClickedSysmenu()
{
  m_sysmenu = m_buttonSysmenu.GetCheck() == TRUE;
  ShowProperties();
}

void 
WindowDefPage3::OnBnClickedClipchild()
{
  m_clipChildren = m_buttonClipChild.GetCheck() == TRUE;
  ShowProperties();
}

void 
WindowDefPage3::OnBnClickedClipsiblings()
{
  m_clipSiblings = m_buttonClipSib.GetCheck() == TRUE;
  ShowProperties();
}

void 
WindowDefPage3::OnBnClickedMinbox()
{
  m_minimizeBox = m_buttonMinBox.GetCheck() == TRUE;
  ShowProperties();
}

void 
WindowDefPage3::OnBnClickedMinimized()
{
  m_minimize = m_buttonMinimize.GetCheck() == TRUE;
  ShowProperties();
}

void 
WindowDefPage3::OnBnClickedHscroll()
{
  m_hscroll = m_buttonHscroll.GetCheck() == TRUE;
  ShowProperties();
}

void 
WindowDefPage3::OnBnClickedVscroll()
{
  m_vscroll = m_buttonVscroll.GetCheck() == TRUE;
  ShowProperties();
}

void 
WindowDefPage3::OnBnClickedChild()
{
  m_child = m_buttonChild.GetCheck() == TRUE;
  ShowProperties();
}

void 
WindowDefPage3::OnBnClickedBorder()
{
  m_border = m_buttonBorder.GetCheck() == TRUE;
  ShowProperties();
}

void 
WindowDefPage3::OnBnClickedDlgframe()
{
  m_dlgFrame = m_buttonDlgFrame.GetCheck() == TRUE;
  ShowProperties();
}

void 
WindowDefPage3::OnBnClickedVisible()
{
  m_visible = m_buttonVisible.GetCheck() == TRUE;
  ShowProperties();
}

void 
WindowDefPage3::OnBnClickedOverlapped()
{
  m_overlapped = m_buttonOverlapped.GetCheck() == TRUE;
  ShowProperties();
}

void 
WindowDefPage3::OnBnClickedThickframe()
{
  m_thkFrame = m_buttonThkFrame.GetCheck() == TRUE;
  ShowProperties();
}

void 
WindowDefPage3::OnBnClickedDisabled()
{
  m_disabled = m_buttonDisabled.GetCheck() == TRUE;
  ShowProperties();
}

//////////////////////////////////////////////////////////////////////////
// EXTENDED

void 
WindowDefPage3::OnBnClickedTool()
{
  m_tool = m_buttonTool.GetCheck() == TRUE;
  ShowProperties();
}

void 
WindowDefPage3::OnBnClickedClientedge()
{
  m_clientEdge = m_buttonClientEdge.GetCheck() == TRUE;
  ShowProperties();
}

void 
WindowDefPage3::OnBnClickedStaticedge()
{
  m_staticEdge = m_buttonStaticEdge.GetCheck() == TRUE;
  ShowProperties();
}

void 
WindowDefPage3::OnBnClickedRightaligned()
{
  m_rightAligned = m_buttonRightAligned.GetCheck() == TRUE;
  ShowProperties();
}

void 
WindowDefPage3::OnBnClickedRtl()
{
  m_rtl = m_buttonRTL.GetCheck() == TRUE;
  ShowProperties();
}

void 
WindowDefPage3::OnBnClickedControlparent()
{
  m_controlParent = m_buttonCtrlParent.GetCheck() == TRUE;
  ShowProperties();
}

void 
WindowDefPage3::OnBnClickedAcceptfiles()
{
  m_acceptFiles = m_buttonAcceptFiles.GetCheck() == TRUE;
  ShowProperties();
}

void 
WindowDefPage3::OnBnClickedTopmost()
{
  m_topmost = m_buttonTopmost.GetCheck() == TRUE;
  ShowProperties();
}

void 
WindowDefPage3::OnBnClickedContexthelp()
{
  m_contextHelp = m_buttonContextHelp.GetCheck() == TRUE;
  ShowProperties();
}

void 
WindowDefPage3::OnBnClickedNonotify()
{
  m_noNotify = m_buttonNoNotify.GetCheck() == TRUE;
  ShowProperties();
}

void 
WindowDefPage3::OnBnClickedLscroll()
{
  m_lscroll = m_buttonLScroll.GetCheck() == TRUE;
  ShowProperties();
}

void 
WindowDefPage3::OnBnClickedTransparent()
{
  m_transparent = m_buttonTransparent.GetCheck() == TRUE;
  ShowProperties();
}
