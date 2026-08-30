//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    WindowDefPage2.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for all window properties page 2 of 5
//
#include "stdafx.h"
#include "AuthorHTML.h"
#include "WindowDefPage2.h"
#include "FindTopicDlg.h" 

// WindowDefPage2 dialog

IMPLEMENT_DYNAMIC(WindowDefPage2, CDialog)

WindowDefPage2::WindowDefPage2(CWnd*        p_parent
                              ,ProjectFile* p_project)
               :CDialog(WindowDefPage2::IDD,p_parent)
               ,m_project(p_project)
               ,m_window(NULL)
{
  m_hideShow  = false;
  m_back      = false;
  m_forward   = false;
  m_stop      = false;
  m_refresh   = false;
  m_home      = false;
  m_options   = false;
  m_print     = false;
  m_locate    = false;
  m_user1     = false;
  m_user2     = false;
  m_label1    = "";
  m_label2    = "";
  m_file1     = "";
  m_file2     = "";
}

WindowDefPage2::~WindowDefPage2()
{
}

void WindowDefPage2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
  DDX_Control(pDX,IDC_HIDESHOW, m_buttonHideShow);
  DDX_Control(pDX,IDC_BACK,     m_buttonBack);
  DDX_Control(pDX,IDC_FORWARD,  m_buttonForward);
  DDX_Control(pDX,IDC_STOP,     m_buttonStop);
  DDX_Control(pDX,IDC_REFRESH,  m_buttonRefresh);
  DDX_Control(pDX,IDC_HOME,     m_buttonHome);
  DDX_Control(pDX,IDC_OPTIONS,  m_buttonOptions);
  DDX_Control(pDX,IDC_PRINT,    m_buttonPrint);
  DDX_Control(pDX,IDC_LOCATE,   m_buttonLocate);
  DDX_Control(pDX,IDC_USER1,    m_buttonUser1);
  DDX_Control(pDX,IDC_USER2,    m_buttonUser2);
  DDX_Text   (pDX,IDC_LABEL1,   m_label1);
  DDX_Text   (pDX,IDC_LABEL2,   m_label2);
  DDX_Text   (pDX,IDC_FILE1,    m_file1);
  DDX_Text   (pDX,IDC_FILE2,    m_file2);
  DDX_Control(pDX,IDC_BUTTON_FILE1, m_buttonFile1);
  DDX_Control(pDX,IDC_BUTTON_FILE2, m_buttonFile2);
}


BEGIN_MESSAGE_MAP(WindowDefPage2, CDialog)
  ON_BN_CLICKED(IDC_HIDESHOW, &WindowDefPage2::OnBnClickedHideshow)
  ON_BN_CLICKED(IDC_BACK,     &WindowDefPage2::OnBnClickedBack)
  ON_BN_CLICKED(IDC_FORWARD,  &WindowDefPage2::OnBnClickedForward)
  ON_BN_CLICKED(IDC_STOP,     &WindowDefPage2::OnBnClickedStop)
  ON_BN_CLICKED(IDC_REFRESH,  &WindowDefPage2::OnBnClickedRefresh)
  ON_BN_CLICKED(IDC_HOME,     &WindowDefPage2::OnBnClickedHome)
  ON_BN_CLICKED(IDC_OPTIONS,  &WindowDefPage2::OnBnClickedOptions)
  ON_BN_CLICKED(IDC_PRINT,    &WindowDefPage2::OnBnClickedPrint)
  ON_BN_CLICKED(IDC_LOCATE,   &WindowDefPage2::OnBnClickedLocate)
  ON_BN_CLICKED(IDC_USER1,    &WindowDefPage2::OnBnClickedUser1)
  ON_BN_CLICKED(IDC_USER2,    &WindowDefPage2::OnBnClickedUser2)
  ON_EN_CHANGE(IDC_LABEL1,    &WindowDefPage2::OnEnChangeLabel1)
  ON_EN_CHANGE(IDC_FILE1,     &WindowDefPage2::OnEnChangeFile1)
  ON_BN_CLICKED(IDC_BUTTON_FILE1, &WindowDefPage2::OnBnClickedButtonFile1)
  ON_EN_CHANGE(IDC_LABEL2,    &WindowDefPage2::OnEnChangeLabel2)
  ON_EN_CHANGE(IDC_FILE2,     &WindowDefPage2::OnEnChangeFile2)
  ON_BN_CLICKED(IDC_BUTTON_FILE2, &WindowDefPage2::OnBnClickedButtonFile2)
END_MESSAGE_MAP()

BOOL
WindowDefPage2::OnInitDialog()
{
  CDialog::OnInitDialog();
  return TRUE;
}

void
WindowDefPage2::SetProperties(WindowDefinition* p_window)
{
  m_window = p_window;

  m_hideShow  = m_window->GetButton(BUTTONS_HIDESHOW);
  m_back      = m_window->GetButton(BUTTONS_BACK);
  m_forward   = m_window->GetButton(BUTTONS_FORWARD);
  m_stop      = m_window->GetButton(BUTTONS_STOP);
  m_refresh   = m_window->GetButton(BUTTONS_REFRESH);
  m_home      = m_window->GetButton(BUTTONS_HOME);
  m_options   = m_window->GetButton(BUTTONS_OPTIONS);
  m_print     = m_window->GetButton(BUTTONS_PRINT);
  m_locate    = m_window->GetButton(BUTTONS_LOCATE);
  m_user1     = m_window->GetButton(BUTTONS_JUMP1);
  m_user2     = m_window->GetButton(BUTTONS_JUMP2);
  m_label1    = m_window->GetJump1Text();
  m_label2    = m_window->GetJump2Text();
  m_file1     = m_window->GetJump1Topic();
  m_file2     = m_window->GetJump2Topic();
  ShowProperties();
}

void
WindowDefPage2::ShowProperties()
{
  m_buttonHideShow.SetCheck(m_hideShow);
  m_buttonBack    .SetCheck(m_back);
  m_buttonForward .SetCheck(m_forward);
  m_buttonStop    .SetCheck(m_stop);
  m_buttonRefresh .SetCheck(m_refresh);
  m_buttonHome    .SetCheck(m_home);
  m_buttonOptions .SetCheck(m_options);
  m_buttonPrint   .SetCheck(m_print);
  m_buttonLocate  .SetCheck(m_locate);
  m_buttonUser1   .SetCheck(m_user1);
  m_buttonUser2   .SetCheck(m_user2);

  CWnd* w1 = GetDlgItem(IDC_LABEL1);
  CWnd* w2 = GetDlgItem(IDC_FILE1);
  w1->SetWindowText(m_label1);
  w2->SetWindowText(m_file1);

  w1->EnableWindow(m_user1);
  w2->EnableWindow(m_user1);
  m_buttonFile1.EnableWindow(m_user1);
  
  w1 = GetDlgItem(IDC_LABEL2);
  w2 = GetDlgItem(IDC_FILE2);
  w1->SetWindowText(m_label2);
  w2->SetWindowText(m_file2);

  w1->EnableWindow(m_user2);
  w2->EnableWindow(m_user2);
  m_buttonFile2.EnableWindow(m_user2);
  
}

void
WindowDefPage2::UpdateProperties()
{
  m_window->SetButton(BUTTONS_HIDESHOW, m_hideShow);
  m_window->SetButton(BUTTONS_BACK,     m_back);
  m_window->SetButton(BUTTONS_FORWARD,  m_forward);
  m_window->SetButton(BUTTONS_STOP,     m_stop);
  m_window->SetButton(BUTTONS_REFRESH,  m_refresh);
  m_window->SetButton(BUTTONS_HOME,     m_home);
  m_window->SetButton(BUTTONS_OPTIONS,  m_options);
  m_window->SetButton(BUTTONS_PRINT,    m_print);
  m_window->SetButton(BUTTONS_LOCATE,   m_locate);
  m_window->SetButton(BUTTONS_JUMP1,    m_user1);
  m_window->SetButton(BUTTONS_JUMP2,    m_user2);
  m_window->SetJump1Text(m_label1);
  m_window->SetJump2Text(m_label2);
  m_window->SetJump1Topic(m_file1);
  m_window->SetJump2Topic(m_file2);
}

// WindowDefPage2 message handlers

void 
WindowDefPage2::OnBnClickedHideshow()
{
  m_hideShow = m_buttonHideShow.GetCheck() == TRUE;
  ShowProperties();
}

void 
WindowDefPage2::OnBnClickedBack()
{
  m_back = m_buttonBack.GetCheck() == TRUE;
  ShowProperties();
}

void 
WindowDefPage2::OnBnClickedForward()
{
  m_forward = m_buttonForward.GetCheck() == TRUE;
  ShowProperties();
}

void 
WindowDefPage2::OnBnClickedStop()
{
  m_stop = m_buttonStop.GetCheck() == TRUE;
  ShowProperties();
}

void 
WindowDefPage2::OnBnClickedRefresh()
{
  m_refresh = m_buttonRefresh.GetCheck() == TRUE;
  ShowProperties();
}

void 
WindowDefPage2::OnBnClickedHome()
{
  m_home = m_buttonHome.GetCheck() == TRUE;
  ShowProperties();
}

void 
WindowDefPage2::OnBnClickedOptions()
{
  m_options = m_buttonOptions.GetCheck() == TRUE;
  ShowProperties();
}

void 
WindowDefPage2::OnBnClickedPrint()
{
  m_print = m_buttonPrint.GetCheck() == TRUE;
  ShowProperties();
}

void 
WindowDefPage2::OnBnClickedLocate()
{
  m_locate = m_buttonLocate.GetCheck() == TRUE;
  ShowProperties();
}

void 
WindowDefPage2::OnBnClickedUser1()
{
  m_user1 = m_buttonUser1.GetCheck() == TRUE;
  ShowProperties();
}

void 
WindowDefPage2::OnBnClickedUser2()
{
  m_user2 = m_buttonUser2.GetCheck() == TRUE;
  ShowProperties();
}

void 
WindowDefPage2::OnEnChangeLabel1()
{
  CWnd* w = GetDlgItem(IDC_LABEL1);
  w->GetWindowText(m_label1);
}

void 
WindowDefPage2::OnEnChangeFile1()
{
  CWnd* w = GetDlgItem(IDC_FILE1);
  w->GetWindowText(m_file1);
}

void 
WindowDefPage2::OnBnClickedButtonFile1()
{
  CString base = theApp.GetBaseDirectory();
  FindTopicDlg dlg(this,base);
  if(dlg.DoModal() == IDOK)
  {
    CString topic = dlg.GetFoundTopic();
    if(!topic.IsEmpty())
    {
      m_file1 = topic;
      UpdateData(FALSE);
    }
  }
}

void 
WindowDefPage2::OnEnChangeLabel2()
{
  CWnd* w = GetDlgItem(IDC_LABEL2);
  w->GetWindowText(m_label2);
}

void 
WindowDefPage2::OnEnChangeFile2()
{
  CWnd* w = GetDlgItem(IDC_FILE2);
  w->GetWindowText(m_file2);
}

void 
WindowDefPage2::OnBnClickedButtonFile2()
{
  CString base = theApp.GetBaseDirectory();
  FindTopicDlg dlg(this,base);
  if(dlg.DoModal() == IDOK)
  {
    CString topic = dlg.GetFoundTopic();
    if(!topic.IsEmpty())
    {
      m_file2 = topic;
      UpdateData(FALSE);
    }
  }
}

