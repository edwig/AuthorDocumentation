//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    ProjectPane.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: A projectpane is a view on a project's files 
// The pane is neccesary to display a toolbar above the project
//
#pragma once
#include "ProjectView.h"
#include "resource.h"

class PRJViewToolBar: public CMFCToolBar
{
  virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/,BOOL bDisableIfNoHndler)
  {
    CMFCToolBar::OnUpdateCmdUI((CFrameWnd*)GetOwner(),bDisableIfNoHndler);
  }

  virtual BOOL AllowShowOnList() const
  {
    return FALSE;
  }
};


class ProjectPane : public CDockablePane
{
public:
   ProjectPane();
  ~ProjectPane();

  void AdjustLayout();

  // Implementation
  PRJViewToolBar m_wndToolBar;
  ProjectView*   m_proView;

  DECLARE_MESSAGE_MAP()

  afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnProjectCommand(UINT nCommand);
  afx_msg void OnSize(UINT nType,int cx,int cy);

};
