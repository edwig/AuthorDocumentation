//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    ExplorerView.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Shows the directory of the current documentation project
//
#pragma once
#include "DirTreeCtrl.h"

class ExplorerToolBar : public CMFCToolBar
{
  virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
  {
    CMFCToolBar::OnUpdateCmdUI((CFrameWnd*)GetOwner(), bDisableIfNoHndler);
  }

  virtual BOOL AllowShowOnList() const
  {
    return FALSE;
  }
};


/////////////////////////////////////////////////////////////////////////////
// CExplorerView window

class CExplorerView : public CDockablePane
{
// Construction
public:
	CExplorerView();
  virtual ~CExplorerView();
  BOOL  SetSubtree(const CString& path);

  // Attributes
  ExplorerToolBar m_wndToolBar;
  CDirTreeCtrl    m_wndExplorer;

	// Generated message map functions
protected:
  void InitializeTree();
  void SaveTree();
  void AdjustLayout();

  afx_msg void OnSize(UINT nType,int cx,int cy);
  afx_msg void OnExplorerTree_DblClick(NMHDR* hdr, LRESULT* pResult);
  afx_msg void OnExplorerTree_RClick  (NMHDR* hdr, LRESULT* pResult);
  afx_msg void OnAddToProject();
  afx_msg void OnFpwRefresh();
  afx_msg void OnFpwOpen();
  afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnExplorerCommand(UINT);
	
	DECLARE_MESSAGE_MAP()
};

