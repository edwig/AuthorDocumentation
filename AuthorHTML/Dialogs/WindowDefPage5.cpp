//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    WindowDefPage5.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for all window properties page 5 of 5
//
#include "stdafx.h"
#include "AuthorHTML.h"
#include "WindowDefPage5.h"
#include "FileDialog.h"
#include "FindTopicDlg.h"
#include "Misc.h"
#include <io.h>

// WindowDefPage5 dialog

IMPLEMENT_DYNAMIC(WindowDefPage5, CDialog)

WindowDefPage5::WindowDefPage5(CWnd*        p_parent
                              ,ProjectFile* p_project)
               :CDialog(WindowDefPage5::IDD,p_parent)
               ,m_project(p_project)
               ,m_window(NULL)
{
  m_toc     = "";
  m_index   = "";
  m_default = "";
  m_home    = "";
}

WindowDefPage5::~WindowDefPage5()
{
}

void WindowDefPage5::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
  DDX_Text   (pDX,IDC_TOC,            m_toc);
  DDX_Text   (pDX,IDC_INDEX,          m_index);
  DDX_Text   (pDX,IDC_DEFAULT,        m_default);
  DDX_Text   (pDX,IDC_HOME,           m_home);
  DDX_Control(pDX,IDC_BUTTON_TOC,     m_buttonTOC);
  DDX_Control(pDX,IDC_BUTTON_INDEX,   m_buttonIndex);
  DDX_Control(pDX,IDC_BUTTON_DEFAULT, m_buttonDefault);
  DDX_Control(pDX,IDC_BUTTON_HOME,    m_buttonHome);
}

BEGIN_MESSAGE_MAP(WindowDefPage5, CDialog)
  ON_EN_KILLFOCUS(IDC_TOC,            &WindowDefPage5::OnEnChangeToc)
  ON_BN_CLICKED  (IDC_BUTTON_TOC,     &WindowDefPage5::OnBnClickedButtonToc)
  ON_EN_KILLFOCUS(IDC_INDEX,          &WindowDefPage5::OnEnChangeIndex)
  ON_BN_CLICKED  (IDC_BUTTON_INDEX,   &WindowDefPage5::OnBnClickedButtonIndex)
  ON_EN_KILLFOCUS(IDC_DEFAULT,        &WindowDefPage5::OnEnChangeDefault)
  ON_BN_CLICKED  (IDC_BUTTON_DEFAULT, &WindowDefPage5::OnBnClickedButtonDefault)
  ON_EN_KILLFOCUS(IDC_HOME,           &WindowDefPage5::OnEnChangeHome)
  ON_BN_CLICKED  (IDC_BUTTON_HOME,    &WindowDefPage5::OnBnClickedButtonHome)
END_MESSAGE_MAP()

BOOL
WindowDefPage5::OnInitDialog()
{
  CDialog::OnInitDialog();
  return TRUE;
}

void
WindowDefPage5::SetProperties(WindowDefinition* p_window)
{
  m_window = p_window;

  m_toc     = m_window->GetHHCFile();
  m_index   = m_window->GetHHKFile();
  m_default = m_window->GetDefaultTopic();
  m_home    = m_window->GetHomeTopic();

  ShowProperties();
}

void
WindowDefPage5::ShowProperties()
{
  UpdateData(FALSE);
}

void
WindowDefPage5::UpdateProperties()
{
  m_window->SetHHCFile(m_toc);
  m_window->SetHHKFile(m_index);
  m_window->SetDefaultTopic(m_default);
  m_window->SetHomeTopic(m_home);
}

// WindowDefPage5 message handlers

void 
WindowDefPage5::OnEnChangeToc()
{
  CWnd* w = GetDlgItem(IDC_TOC);
  w->GetWindowText(m_toc);
  CString direc = Misc::DirectoryPart(m_toc);
  CString file  = Misc::FilenamePart (m_toc);
  CString exten = Misc::ExtensionPart(m_toc);
  if(exten.CompareNoCase(".hhc"))
  {
    theApp.MessageBox("The table-of-contents (TOC) file must have a '.hhc' extension in order to work properly.\n"
                      "The file name you provided will be automatically corrected to this convention!"
                     ,"Correction"
                     ,MB_OK|MB_ICONEXCLAMATION);
    file = file.TrimRight(exten);
    file += ".hhc";
    m_toc = file;
    w->SetWindowText(m_toc);
  }
  if(!direc.IsEmpty())
  {
    theApp.MessageBox("You cannot choose a table-of-contents (TOC) file from another directory than from the current project base directory!\n"
                      "The file name you provided will be cleared automatically. Choose another TOC file!"
                     ,"Correction"
                     ,MB_OK|MB_ICONEXCLAMATION);
    m_toc = "";
    w->SetWindowText(m_toc);
  }
  int err = 0;
  CString path = theApp.GetBaseDirectory();
  path += m_toc;
  if((err = access(path.GetString(),6)) != 0)
  {
    theApp.ErrorMessage("The Table-Of-Contents file could not be found or has the wrong access rights (read/write)");
  }
}

void 
WindowDefPage5::OnBnClickedButtonToc()
{
  CString base = theApp.GetBaseDirectory();
  DocFileDialog diag(true
                    ,"Search for a table-of-contents file (TOC)"
                    ,"hhc"
                    ,""
                    ,OFN_FILEMUSTEXIST
                    ,"Table-Of-Contents files (*.hhc)|*.hhc|"
                    ,base);
  if(diag.DoModal() == IDOK)
  {
    CString href = diag.GetChosenFile();
    CString relative;
    if(Misc::MakeRelativePathname(base,href,relative))
    {
      href = relative;
    }
    m_toc = href;
    CWnd* w = GetDlgItem(IDC_TOC);
    w->SetWindowText(m_toc);
    OnEnChangeToc();
  }
}

void 
WindowDefPage5::OnEnChangeIndex()
{
  CWnd* w = GetDlgItem(IDC_INDEX);
  w->GetWindowText(m_index);
  CString direc = Misc::DirectoryPart(m_index);
  CString file  = Misc::FilenamePart (m_index);
  CString exten = Misc::ExtensionPart(m_index);
  if(exten.CompareNoCase(".hhk"))
  {
    theApp.MessageBox("The keyword index file must have a '.hhk' extension in order to work properly.\n"
                      "The file name you provided will be automatically corrected to this convention!"
                      ,"Correction"
                      ,MB_OK|MB_ICONEXCLAMATION);
    file    = file.TrimRight(exten);
    file   += ".hhk";
    m_index = file;
    w->SetWindowText(m_index);
  }
  if(!direc.IsEmpty())
  {
    theApp.MessageBox("You cannot choose a keyword index file from another directory than from the current project base directory!\n"
                      "The file name you provided will be cleared automatically. Choose another HHK file!"
                      ,"Correction"
                      ,MB_OK|MB_ICONEXCLAMATION);
    m_index = "";
    w->SetWindowText(m_index);
  }
  int err = 0;
  CString path = theApp.GetBaseDirectory();
  path += m_index;
  if((err = access(path.GetString(),6)) != 0)
  {
    theApp.ErrorMessage("The keyword file could not be found or has the wrong access rights (read/write)");
  }
}

void 
WindowDefPage5::OnBnClickedButtonIndex()
{
  CString base = theApp.GetBaseDirectory();
  DocFileDialog diag(true
                    ,"Search for a keyword index file"
                    ,"hhk"
                    ,""
                    ,OFN_FILEMUSTEXIST
                    ,"Keyword index files (*.hhk)|*.hhk|"
                    ,base);
  if(diag.DoModal() == IDOK)
  {
    CString href = diag.GetChosenFile();
    CString relative;
    if(Misc::MakeRelativePathname(base,href,relative))
    {
      href = relative;
    }
    m_index = href;
    CWnd* w = GetDlgItem(IDC_INDEX);
    w->SetWindowText(m_index);
    OnEnChangeIndex();
  }
}

void 
WindowDefPage5::OnEnChangeDefault()
{
  CWnd* w = GetDlgItem(IDC_DEFAULT);
  w->GetWindowText(m_default);
  DocumentFile* doc = m_project->FindDocumentFile(m_default);
  if(doc == NULL)
  {
    theApp.ErrorMessage("The selected topic file is not part of the current project file");
  }
}

void 
WindowDefPage5::OnBnClickedButtonDefault()
{
  CString base = theApp.GetBaseDirectory();
  FindTopicDlg find(this,base,false);
  if(find.DoModal() == IDOK)
  {
    CString href = find.GetFoundTopic();
    m_default = href;
    CWnd* w = GetDlgItem(IDC_DEFAULT);
    w->SetWindowText(m_default);
    OnEnChangeDefault();
  }
}

void 
WindowDefPage5::OnEnChangeHome()
{
  CWnd* w = GetDlgItem(IDC_HOME);
  w->GetWindowText(m_home);
  DocumentFile* doc = m_project->FindDocumentFile(m_home);
  if(doc == NULL)
  {
    theApp.ErrorMessage("The selected topic file is not part of the current project file");
  }
}

void 
WindowDefPage5::OnBnClickedButtonHome()
{
  CString base = theApp.GetBaseDirectory();
  FindTopicDlg find(this,base,false);
  if(find.DoModal() == IDOK)
  {
    CString href = find.GetFoundTopic();
    m_home = href;
    CWnd* w = GetDlgItem(IDC_HOME);
    w->SetWindowText(m_home);
    OnEnChangeHome();
  }
}
