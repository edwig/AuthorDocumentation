//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    StartupDlg.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Choose an action upon startup of the program. 
//              Either open a recent project or file, or create a new one.
//
#include "stdafx.h"
#include "AuthorHTML.h"
#include "StartupDlg.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "ImportDlg.h"
#include "ProjectFile.h"
#include <io.h>

// StartupDlg dialog

IMPLEMENT_DYNAMIC(StartupDlg, CDialog)

StartupDlg::StartupDlg(CWnd* pParent /*=NULL*/)
	         :CDialog(StartupDlg::IDD, pParent)
           ,m_listsProjects(true)
{
}

StartupDlg::~StartupDlg()
{
}

void 
StartupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
  DDX_Control(pDX,IDC_OPENPROJECT,    m_buttonOpenProject);
  DDX_Control(pDX,IDC_OPENFILE,       m_buttonOpenFile);
  DDX_Control(pDX,IDC_NEWPROJECT,     m_buttonNewProject);
  DDX_Control(pDX,IDC_NEWFILE,        m_buttonNewFile);
  DDX_Control(pDX,IDC_IMPORT,         m_buttonImport);
  DDX_Control(pDX,IDC_LIST,           m_list);
  DDX_Control(pDX,IDHELP,             m_buttonHelp);
  DDX_Control(pDX,IDCANCEL,           m_buttonCancel);
}

BEGIN_MESSAGE_MAP(StartupDlg, CDialog)
  ON_BN_CLICKED(IDC_OPENPROJECT,  &StartupDlg::OnBnClickedOpenproject)
  ON_BN_CLICKED(IDC_OPENFILE,     &StartupDlg::OnBnClickedOpenfile)
  ON_BN_CLICKED(IDC_NEWPROJECT,   &StartupDlg::OnBnClickedNewproject)
  ON_BN_CLICKED(IDC_NEWFILE,      &StartupDlg::OnBnClickedNewfile)
  ON_BN_CLICKED(IDC_IMPORT,       &StartupDlg::OnBnClickedImport)
  ON_NOTIFY(NM_DBLCLK,IDC_LIST,   &StartupDlg::OnLvnItemchangedList)
  ON_BN_CLICKED(IDHELP,           &StartupDlg::OnBnClickedHelp)
  ON_BN_CLICKED(IDCANCEL,         &StartupDlg::OnBnClickedCancel)
END_MESSAGE_MAP()

BOOL
StartupDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EDITLABELS);
  m_list.InsertColumn(0,"Projects / Files",LVCFMT_LEFT,280);

  SetButtonImages();
  GetRecentProjectList();

  // Don't do the focus by way of the CDialog
  return FALSE;
}

void
StartupDlg::SetButtonImages()
{
  m_buttonOpenProject.SetImage(IDB_CHM_PROJECT);
  m_buttonOpenFile   .SetImage(IDB_HTML_FILE);
  m_buttonNewProject .SetImage(IDB_CHM_NEWPROJECT);
  m_buttonNewFile    .SetImage(IDB_HTML_NEWFILE);
  m_buttonImport     .SetImage(IDB_CHM_FILE);
}

void
StartupDlg::SetButtonText(bool p_project)
{
  if(p_project)
  {
    // Projects displayed
    m_buttonOpenProject.SetWindowText("\n&OPEN\nProject by dialog");
    m_buttonOpenFile.SetWindowText("\nGet the list\nof recent files");
  }
  else
  {
    // Files displayed
    m_buttonOpenProject.SetWindowText("\nGet the list\nof recent projects");
    m_buttonOpenFile.SetWindowText("\nOPEN\n&File by dialog");
  }
}

void
StartupDlg::GetRecentProjectList()
{
  CRecentFileList* recent = theApp.GetRecentProjectList();

  m_list.DeleteAllItems();
  int size = recent->GetSize();
  for(int ind = 0; ind < size; ++ind)
  {
    CString filename;
    recent->GetDisplayName(filename,ind,"",0,TRUE);
    m_list.InsertItem(LVIF_TEXT|LVIF_STATE, ind, filename, 0, 0, 0, 0);
  }
  m_list.SetFocus();
  m_list.SetItemState(0,LVIS_SELECTED,LVIS_SELECTED);

  m_listsProjects = true;
  SetButtonText(true);
}

void
StartupDlg::GetRecentFileList()
{
  CRecentFileList* recent = theApp.GetRecentFileList();
  
  m_list.DeleteAllItems();
  int size = recent->GetSize();
  for(int ind = 0; ind < size; ++ind)
  {
    CString filename;
    recent->GetDisplayName(filename,ind,"",0,TRUE);
    m_list.InsertItem(LVIF_TEXT|LVIF_STATE, ind, filename, 0, 0, 0, 0);
  }
  m_list.SetFocus();
  m_list.SetItemState(0,LVIS_SELECTED,LVIS_SELECTED);

  m_listsProjects = false;
  SetButtonText(false);
}

// StartupDlg message handlers

void 
StartupDlg::OnBnClickedOpenproject()
{
  if(m_listsProjects == false)
  {
    GetRecentProjectList();
    return;
  }
  // Try for an open project
  theApp.OnProjectOpen();

  // See if we now have an open project
  ProjectFile* project = theApp.GetProjectFile();
  if(project)
  {
    OnOK();
  }
}

void 
StartupDlg::OnBnClickedOpenfile()
{
  if(m_listsProjects == true)
  {
    GetRecentFileList();
    return;
  }
  // Try opening a file
  theApp.OnFileOpen();

  // See if we now have an open file
  BOOL alreadyMax;
  MainFrame* main = (MainFrame*) theApp.m_pMainWnd;
  CMDIChildFrame* first = (CMDIChildFrame*)main->MDIGetActive(&alreadyMax);
  if(first)
  {
    OnOK();
  }
}

void 
StartupDlg::OnBnClickedNewproject()
{
  // Try creating a new project
  theApp.OnNewProject();

  // See if we now have an open project
  ProjectFile* project = theApp.GetProjectFile();
  if(project)
  {
    OnOK();
  }
}

void 
StartupDlg::OnBnClickedNewfile()
{
  // Try opening a new file
  theApp.OnNewFile();

  // See if we now have an open file
  BOOL alreadyMax;
  MainFrame* main = (MainFrame*) theApp.m_pMainWnd;
  CMDIChildFrame* first = (CMDIChildFrame*)main->MDIGetActive(&alreadyMax);
  if(first)
  {
    OnOK();
  }
}

void 
StartupDlg::OnBnClickedImport()
{
  ImportDlg dlg(this);
  if(dlg.DoModal() == IDOK)
  {
    CString filename = dlg.GetHHPFilename();
    theApp.OpenTypedDocumentFile(filename);
    if(dlg.GetDoSweep() == false)
    {
      ProjectFile* project = theApp.GetProjectFile();
      if(project)
      {
        project->BlockSweep();
      }
    }
    OnOK();
  }
}

void 
StartupDlg::OnLvnItemchangedList(NMHDR *pNMHDR, LRESULT *pResult)
{
  //LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
  UNREFERENCED_PARAMETER(pNMHDR);
  CWaitCursor takeAdeepSigh;

  int num = m_list.GetItemCount();
  int now = m_list.GetNextItem(-1,LVNI_SELECTED);
  if(now < num)
  {
    CString filename;
    CRecentFileList* recent = NULL;

    if(m_listsProjects)
    {
      // Lists projects
      recent = theApp.GetRecentProjectList();
    }
    else
    {
      // Lists files
      recent = theApp.GetRecentFileList();
    }
    // Open it if it exists
    filename = (*recent)[now];
    if(_access(filename,0) == 0)
    {
      theApp.OpenTypedDocumentFile(filename);
      OnOK();
      return;
    }
    CString message;
    message.Format("The file [%s] does no longer exists.\n"
                   "Remove it from the recent project/file lists?",filename.GetString());
    if(theApp.MessageBox(message,"File error",MB_YESNO) == IDYES)
    {
      recent->Remove(now);
      // Re-read recent list
      if(m_listsProjects)
      {
        GetRecentProjectList();
      }
      else
      {
        GetRecentFileList();
      }
    }
  }
  *pResult = 0;
}

void 
StartupDlg::OnBnClickedHelp()
{
  // TODO: Add your control notification handler code here
}

void 
StartupDlg::OnBnClickedCancel()
{
  CDialog::OnCancel();
}
