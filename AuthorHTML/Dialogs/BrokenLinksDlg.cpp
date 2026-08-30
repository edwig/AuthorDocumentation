//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    BrokenLinksDlg.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog to sum-up the broken links in a project and to be able
//              to sweep the project for broken links and repair them
//
#include "stdafx.h"
#include "AuthorHTML.h"
#include "Dialogs\BrokenLinksDlg.h"
#include "DocumentFile.h"

// BrokenLinksDlg dialog

IMPLEMENT_DYNAMIC(BrokenLinksDlg, CDialog)

BrokenLinksDlg::BrokenLinksDlg(CWnd* p_parent,ProjectFile* p_project)
               :CDialog(BrokenLinksDlg::IDD, p_parent)
               ,m_project(p_project)
{
}

BrokenLinksDlg::~BrokenLinksDlg()
{
}

void 
BrokenLinksDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
  DDX_Text   (pDX,IDC_NAME,   m_naam);
  DDX_Control(pDX,IDC_BLINKS, m_list);
  DDX_Control(pDX,IDC_SWEEP,  m_buttonSweep);
}

BEGIN_MESSAGE_MAP(BrokenLinksDlg, CDialog)
  ON_BN_CLICKED(IDC_SWEEP, OnBnClickedSweep)
  //ON_NOTIFY(LVN_ITEMACTIVATE,IDC_LIST1, OnLvnItemDouble)
  ON_NOTIFY(NM_DBLCLK,IDC_BLINKS,OnDblClckLink)
END_MESSAGE_MAP()

BOOL
BrokenLinksDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  m_naam = m_project->GetTitle();

  m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EDITLABELS);
  m_list.InsertColumn(0,"Source",      LVCFMT_LEFT,300);
  m_list.InsertColumn(1,"Broken link", LVCFMT_LEFT,300);

  FillPage();

  return FALSE;
}

void
BrokenLinksDlg::FillPage()
{
  int ind = 0;
  m_list.DeleteAllItems();

  BrokenMap* broken = m_project->GetBrokenMap();
  BrokenMap::iterator it = broken->begin();

  while(it != broken->end())
  {
    DocumentFile* doc  = it->first;
    CString brokenLink = it->second;
    CString source("Project file");
    if(doc)
    {
      source = doc->GetRelativeFilename();
    }
    m_list.InsertItem(LVIF_TEXT|LVIF_STATE, ind, source, 0, 0, 0, 0);
    m_list.SetItemText(ind,1,brokenLink);
    // Volgende
    ++it;
    ++ind;
  }
}

// BrokenLinksDlg message handlers

void
BrokenLinksDlg::OnBnClickedSweep()
{
  // Clear the list
  m_list.DeleteAllItems();
  UpdateData(FALSE);
  // Resweep the project
  m_project->ResetSweeped();
  m_project->SweepProject();
  // Fill broken links again
  FillPage();
  UpdateData(FALSE);
}

void
BrokenLinksDlg::OnDblClckLink(NMHDR* /*pNMHDR*/, LRESULT *pResult)
{
  int num = m_list.GetItemCount();
  int now = m_list.GetNextItem(-1,LVNI_SELECTED);
  if(now < num)
  {
    CString file = m_list.GetItemText(now,0);
    if(file.CompareNoCase("Project file"))
    {
      CString base = m_project->GetBaseDirectory();
      CString path = base + file;
      theApp.OpenDocumentFile(path);
      OnOK();
    }
  }
  *pResult = 0;
}