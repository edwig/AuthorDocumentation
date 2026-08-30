//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    ProjectPane.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: A projectpane is a view on a project's files 
// The pane is neccesary to display a toolbar above the project
//
#include "StdAfx.h"
#include "ProjectPane.h"
#include "AuthorHTML.h"

ProjectPane::ProjectPane()
{
}

ProjectPane::~ProjectPane()
{
}

// #define ID_PROJECT_NEWFILE              33154
// #define ID_PROJECT_EDITFILE             33155
// #define ID_PROJECT_DELETE               33156
// #define ID_PROJECT_PROPERTIES           33157

BEGIN_MESSAGE_MAP(ProjectPane,CDockablePane)
  ON_WM_CREATE()
  ON_WM_SIZE()
  ON_COMMAND_RANGE(ID_PROJECT_NEWFILE, ID_PROJECT_PROPERTIES, OnProjectCommand)
END_MESSAGE_MAP()

int
ProjectPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  CDockablePane::OnCreate(lpCreateStruct);

  m_proView = (ProjectView*) ProjectView::CreateObject();
  m_proView->CreateView(this);
  m_proView->ModifyStyle(0,WS_CHILD|WS_VISIBLE);

  // Create the toolbar
  m_wndToolBar.Create(this,AFX_DEFAULT_TOOLBAR_STYLE,IDR_TOC_BAR);
  m_wndToolBar.LoadToolBar(IDR_PROJECTBAR,0,0,TRUE /* Is locked */);
  m_wndToolBar.LoadBitmap (IDB_PROJECT,   0,0,TRUE /* Locked */);

  m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
  m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
  m_wndToolBar.SetOwner(this);

  // All commands will be routed via this control , not via the parent frame:
  m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

  AdjustLayout();
  return 0;
}

void
ProjectPane::OnProjectCommand(UINT nCommand)
{
  switch(nCommand)
  {
    case ID_PROJECT_NEWFILE:    m_proView->OnNewRecord();        break;
    case ID_PROJECT_EDITFILE:   m_proView->OnEditRecord();       break;
    case ID_PROJECT_DELETE:     m_proView->OnDeleteRecord();     break;
    case ID_PROJECT_PROPERTIES: m_proView->OnRecordProperties(); break;
  }
}

void
ProjectPane::OnSize(UINT nType,int cx,int cy)
{
  CDockablePane::OnSize(nType,cx,cy);
  AdjustLayout();
}

void
ProjectPane::AdjustLayout()
{
  if(GetSafeHwnd() == nullptr)
  {
    return;
  }

  CRect rectClient;
  GetClientRect(rectClient);

  int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE,TRUE).cy;

  m_wndToolBar.SetWindowPos(nullptr,rectClient.left,rectClient.top,rectClient.Width(),cyTlb,SWP_NOACTIVATE | SWP_NOZORDER);
  m_proView  ->SetWindowPos(nullptr,rectClient.left + 1,rectClient.top + cyTlb + 1,rectClient.Width() - 2,rectClient.Height() - cyTlb - 2,SWP_NOACTIVATE | SWP_NOZORDER);
}
