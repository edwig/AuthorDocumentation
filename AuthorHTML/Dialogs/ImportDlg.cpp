//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    ImportDlg.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for importing a CHM file into a project
//              by decompiling a CHM file and creating a project with the contents
//
#include "stdafx.h"
#include "AuthorHTML.h"
#include "ImportDlg.h"
#include "FileDialog.h"
#include "MapDialog.h"
#include "MainFrm.h"
#include "Misc.h"
#include "ImportCHM.h"
#include "afxdialogex.h"
#include <io.h>

// ImportDlg dialog

IMPLEMENT_DYNAMIC(ImportDlg, CDialog)

ImportDlg::ImportDlg(CWnd* pParent /*=NULL*/)
          :CDialog(ImportDlg::IDD, pParent)
          ,m_doSweep(true)
{
}

ImportDlg::~ImportDlg()
{
}

void 
ImportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
  DDX_Text   (pDX,IDC_CHM,          m_filename);
  DDX_Text   (pDX,IDC_PROJECT,      m_directory);
  DDX_Control(pDX,IDC_SEARCH_CHM,   m_buttonSearchCHM);
  DDX_Control(pDX,IDC_SEARCH_PRO,   m_buttonSearchProject);
  DDX_Control(pDX,IDC_DOSWEEP,      m_buttonDoSweep);
  DDX_Control(pDX,IDHELP,           m_buttonHelp);
  DDX_Control(pDX,IDOK,             m_buttonOK);
  DDX_Control(pDX,IDCANCEL,         m_buttonCancel);
}

BEGIN_MESSAGE_MAP(ImportDlg, CDialog)
  ON_EN_CHANGE  (IDC_CHM,         &ImportDlg::OnEnChangeChm)
  ON_BN_CLICKED (IDC_SEARCH_CHM,  &ImportDlg::OnBnClickedSearchChm)
  ON_EN_CHANGE  (IDC_PROJECT,     &ImportDlg::OnEnChangeProject)
  ON_BN_CLICKED (IDC_SEARCH_PRO,  &ImportDlg::OnBnClickedSearchPro)
  ON_BN_CLICKED (IDC_DOSWEEP,     &ImportDlg::OnBnClickedDosweep)
  ON_BN_CLICKED (IDHELP,          &ImportDlg::OnBnClickedHelp)
  ON_BN_CLICKED (IDOK,            &ImportDlg::OnBnClickedOk)
  ON_BN_CLICKED (IDCANCEL,        &ImportDlg::OnBnClickedCancel)
END_MESSAGE_MAP()

BOOL
ImportDlg::OnInitDialog()
{
  CDialog::OnInitDialog();
  CheckInput();
  m_buttonDoSweep.SetCheck(TRUE);
  return TRUE;
}

void
ImportDlg::CheckInput()
{
  bool okToImport = false;
  // Check read-access on file
  if(!m_filename.IsEmpty() && _access(m_filename,4) == 0)
  {
    // Check read/write-access on directory
    if(!m_directory.IsEmpty() && _access(m_directory,6) == 0)
    {
      okToImport = true;
    }
  }
  m_buttonOK.EnableWindow(okToImport);
}

CString 
ImportDlg::GetHHPFilename()
{
  // Check for valid situation
  if(m_directory.IsEmpty() || m_filename.IsEmpty())
  {
    return "";
  }
  // Compose .HPP filename
  CString filename = m_directory;
  if(filename.Right(1) != '\\')
  {
    filename += "\\";
  }
  filename += Misc::BasenamePart(m_filename);
  filename += ".hhp";

  return filename;
}


// ImportDlg message handlers

void 
ImportDlg::OnEnChangeChm()
{
  UpdateData();
  CheckInput();
}

void 
ImportDlg::OnBnClickedSearchChm()
{
  DocFileDialog diag(true
                    ,"Open a help file"
                    ,"chm"
                    ,""
                    ,0
                    ,"On-Line help file (*.chm)|*.chm|");
  if(diag.DoModal() == IDOK)
  {
    m_filename = diag.GetChosenFile();
    UpdateData(FALSE);
    CheckInput();
  }  
}

void 
ImportDlg::OnEnChangeProject()
{
  UpdateData();
  CheckInput();
}

void 
ImportDlg::OnBnClickedSearchPro()
{
  MapDialog diag;
  
  if(diag.Browse(GetSafeHwnd(),"Choose a project directory"))
  {
    m_directory = diag.GetPath();
    UpdateData(FALSE);
    CheckInput();
  }
}

void 
ImportDlg::OnBnClickedDosweep()
{
  m_doSweep = (m_buttonDoSweep.GetCheck() == TRUE);
}

void 
ImportDlg::OnBnClickedHelp()
{
  // TODO: Add your control notification handler code here
}

void 
ImportDlg::OnBnClickedOk()
{
  ImportCHM imp;
  if(imp.Import(m_directory,m_filename))
  {
    CDialog::OnOK();
  }
}

void 
ImportDlg::OnBnClickedCancel()
{
  CDialog::OnCancel();
}

