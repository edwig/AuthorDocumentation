//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    WindowDefDlg.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for all window properties pages
//
#include "stdafx.h"
#include "AuthorHTML.h"
#include "WindowDefDlg.h"

// WindowDefDlg dialog

IMPLEMENT_DYNAMIC(WindowDefDlg, CDialog)

WindowDefDlg::WindowDefDlg(CWnd* p_parent
                          ,ProjectFile* p_project
                          ,CString      p_firstWindow)
             :CDialog(WindowDefDlg::IDD, p_parent)
             ,m_project(p_project)
             ,m_currentWindow(p_firstWindow)
{
  m_page1 = new WindowDefPage1(this,p_project);
  m_page2 = new WindowDefPage2(this,p_project);
  m_page3 = new WindowDefPage3(this,p_project);
  m_page4 = new WindowDefPage4(this,p_project);
  m_page5 = new WindowDefPage5(this,p_project);
}

WindowDefDlg::~WindowDefDlg()
{
  delete m_page1;
  delete m_page2;
  delete m_page3;
  delete m_page4;
  delete m_page5;
}

void WindowDefDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
  DDX_Control(pDX,IDC_WINDOWS,    m_comboWindows);
  DDX_Text   (pDX,IDC_TITLE,      m_title);
  DDX_Control(pDX,IDC_NEWWINDEF,  m_buttonNew);
  DDX_Control(pDX,IDC_DELWINDEF,  m_buttonDelete);
  DDX_Control(pDX,IDC_TAB,        m_tab);
  DDX_Control(pDX,IDOK,           m_buttonOK);
  DDX_Control(pDX,IDCANCEL,       m_buttonCancel);
  DDX_Control(pDX,IDC_HELPWINDOW, m_buttonHelp);
}

BEGIN_MESSAGE_MAP(WindowDefDlg, CDialog)
  ON_CBN_SELCHANGE(IDC_WINDOWS,     &WindowDefDlg::OnCbnSelchangeWindows)
  ON_EN_CHANGE    (IDC_TITLE,       &WindowDefDlg::OnEnChangeTitle)
  ON_BN_CLICKED   (IDC_NEWWINDEF,   &WindowDefDlg::OnBnClickedNewwindef)
  ON_BN_CLICKED   (IDC_DELWINDEF,   &WindowDefDlg::OnBnClickedDelwindef)
  ON_BN_CLICKED   (IDOK,            &WindowDefDlg::OnBnClickedOk)
  ON_BN_CLICKED   (IDCANCEL,        &WindowDefDlg::OnBnClickedCancel)
  ON_BN_CLICKED   (IDC_HELPWINDOW,  &WindowDefDlg::OnBnClickedHelpwindow)
  ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, &WindowDefDlg::OnTcnSelchangeTab)
END_MESSAGE_MAP()

BOOL
WindowDefDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  m_page1->Create(IDD_WINDOWDEF_PAGE1,this);
  m_page2->Create(IDD_WINDOWDEF_PAGE2,this);
  m_page3->Create(IDD_WINDOWDEF_PAGE3,this);
  m_page4->Create(IDD_WINDOWDEF_PAGE4,this);
  m_page5->Create(IDD_WINDOWDEF_PAGE5,this);

  // Set text of tabs
  CString s;
  m_page1->GetWindowText(s);
  m_tab.InsertItem(0,s);
  m_page2->GetWindowText(s);
  m_tab.InsertItem(1,s);
  m_page3->GetWindowText(s);
  m_tab.InsertItem(2,s);
  m_page4->GetWindowText(s);
  m_tab.InsertItem(3,s);
  m_page5->GetWindowText(s);
  m_tab.InsertItem(4,s);

  CRect rect;
  m_tab.GetWindowRect(&rect);    // geeft t.o.v. ouder window inclusief titelbar
  ScreenToClient(&rect);
  rect.top    += 21;
  rect.bottom -= 3;
  rect.right  -= 2;
  rect.left   += 1; 

  m_page1->MoveWindow(rect,true);    // zet tov client window.
  m_page1->ShowWindow(SW_SHOW);
  m_page2->MoveWindow(rect,true);    // zet tov client window.
  m_page2->ShowWindow(SW_HIDE);
  m_page3->MoveWindow(rect,true);    // zet tov client window.
  m_page3->ShowWindow(SW_HIDE);
  m_page4->MoveWindow(rect,true);    // zet tov client window.
  m_page4->ShowWindow(SW_HIDE);
  m_page5->MoveWindow(rect,true);    // zet tov client window.
  m_page5->ShowWindow(SW_HIDE);
  TabCtrl_SetCurSel(m_tab.GetSafeHwnd(),0);

  SetWindowNames();
  SetProperties();

  return TRUE;
}

void
WindowDefDlg::SetWindowNames()
{
  WindowNames names;
  m_project->GetWindowNames(names);
  m_comboWindows.ResetContent();
  for(unsigned int ind = 0;ind < names.size(); ++ind)
  {
    m_comboWindows.AddString(names[ind]);
  }
  int ind = m_comboWindows.FindString(0,m_currentWindow);
  if(ind < 0) ind = 0;
  m_comboWindows.SetCurSel(ind);
}

void
WindowDefDlg::SetProperties()
{
  m_title  = "";
  m_window = m_project->FindWindowDefinition(m_currentWindow);
  if(m_window == NULL)
  {
    m_window = m_project->FirstWindowDefinition();
  }
  if(m_window == NULL)
  {
    m_currentWindow = "main";
    m_project->AddWindow(m_currentWindow);
    m_window = m_project->FindWindowDefinition(m_currentWindow);
    SetWindowNames();
  }
  if(m_window == NULL)
  {
    theApp.Panic("Cannot create a new main window definition for a CHM help file.");
    OnCancel();
  }
  m_title = m_window->GetTitle();
  UpdateData(FALSE);
  m_page1->SetProperties(m_window);
  m_page2->SetProperties(m_window);
  m_page3->SetProperties(m_window);
  m_page4->SetProperties(m_window);
  m_page5->SetProperties(m_window);
}

void
WindowDefDlg::UpdateProperties()
{
  m_window->SetTitle(m_title);
  m_page1->UpdateProperties();
  m_page2->UpdateProperties();
  m_page3->UpdateProperties();
  m_page4->UpdateProperties();
  m_page5->UpdateProperties();
}


// WindowDefDlg message handlers

void WindowDefDlg::OnCbnSelchangeWindows()
{
  int ind = m_comboWindows.GetCurSel();
  if(ind >= 0)
  {
    CString newwin;
    m_comboWindows.GetLBText(ind,newwin);
    WindowDefinition* win = m_project->FindWindowDefinition(newwin);
    if(win)
    {
      m_window = win;
      m_currentWindow = newwin;
      SetProperties();
    }
    else
    {
      CString msg;
      msg.Format("Panic: Could not find the window definition with the name: %s",newwin.GetString());
      theApp.MessageBox(msg,"Error",MB_OK|MB_ICONERROR);
    }
  }
}

void 
WindowDefDlg::OnEnChangeTitle()
{
  UpdateData(TRUE);
}

void 
WindowDefDlg::OnBnClickedNewwindef()
{
  CString newName;
  m_comboWindows.GetWindowText(newName);
  if(m_project->FindWindowDefinition(newName))
  {
    CString msg;
    msg.Format("The main CHM window with the name '%s' does already exist.\n"
               "Define another name first, then push the 'New' button.",newName.GetString());
    theApp.MessageBox(msg,"Warning",MB_OK|MB_ICONWARNING);
  }
  else
  {
    // Add a new window
    m_project->AddWindow(newName);
    m_window = m_project->FindWindowDefinition(newName);
    m_currentWindow = newName;
    SetWindowNames();
    SetProperties();
  }
}

void 
WindowDefDlg::OnBnClickedDelwindef()
{
  CString winName;
  m_comboWindows.GetWindowText(winName);
  CString msg;
  msg.Format("Do you want to delete the window definition '%s'?",winName.GetString());
  if(theApp.MessageBox(msg,"Question",MB_YESNO|MB_ICONQUESTION) == IDYES)
  {
    if(m_project->RemoveWindow(winName))
    {
      int ind = m_comboWindows.FindString(0,winName);
      if(ind >= 0)
      {
        m_comboWindows.DeleteString(ind);
        m_comboWindows.SetCurSel(0);
      }
      m_comboWindows.GetWindowText(m_currentWindow);
      SetProperties();
    }
    else
    {
      CString error;
      error.Format("Could not find the window with name '%s' to be deleted",winName.GetString());
      theApp.MessageBox(error,"Error",MB_ICONERROR|MB_OK);
    }
  }
}

void 
WindowDefDlg::OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
  int num = TabCtrl_GetCurSel(pNMHDR->hwndFrom);
  switch(num)
  {
    case 0:  m_page1->ShowWindow(SW_SHOW);
             m_page2->ShowWindow(SW_HIDE);
             m_page3->ShowWindow(SW_HIDE);
             m_page4->ShowWindow(SW_HIDE);
             m_page5->ShowWindow(SW_HIDE);
             break;
    case 1:  m_page1->ShowWindow(SW_HIDE);
             m_page2->ShowWindow(SW_SHOW);
             m_page3->ShowWindow(SW_HIDE);
             m_page4->ShowWindow(SW_HIDE);
             m_page5->ShowWindow(SW_HIDE);
             break;
    case 2:  m_page1->ShowWindow(SW_HIDE);
             m_page2->ShowWindow(SW_HIDE);
             m_page3->ShowWindow(SW_SHOW);
             m_page4->ShowWindow(SW_HIDE);
             m_page5->ShowWindow(SW_HIDE);
             break;
    case 3:  m_page1->ShowWindow(SW_HIDE);
             m_page2->ShowWindow(SW_HIDE);
             m_page3->ShowWindow(SW_HIDE);
             m_page4->ShowWindow(SW_SHOW);
             m_page5->ShowWindow(SW_HIDE);
             break;
    case 4:  m_page1->ShowWindow(SW_HIDE);
             m_page2->ShowWindow(SW_HIDE);
             m_page3->ShowWindow(SW_HIDE);
             m_page4->ShowWindow(SW_HIDE);
             m_page5->ShowWindow(SW_SHOW);
             break;
  }
  *pResult = 0;
}

void 
WindowDefDlg::OnBnClickedOk()
{
  UpdateProperties();
  OnOK();
}

void 
WindowDefDlg::OnBnClickedCancel()
{
  OnCancel();
}

void 
WindowDefDlg::OnBnClickedHelpwindow()
{
  // TODO: Add your control notification handler code here
}

