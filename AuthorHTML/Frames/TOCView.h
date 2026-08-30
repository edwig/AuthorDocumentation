//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    TOCView.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: A TOCView is a view over the table of contents
// it displays the TOC in a Pane, so the user can see and edit it
// and navigate the project bij choosing a topic
//
#pragma once
#include "TOCEntry.h"
#include "TOCTreeCtrl.h"

class TOCViewToolBar: public CMFCToolBar
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



class TOCView : public CDockablePane
{
  // Construction
public:
  TOCView();
  virtual ~TOCView();

  // Attributes
  TOCViewToolBar  m_wndToolBar;
  TOCTreeCtrl     m_wndExplorer;
  CImageList*     m_imageList;

  // Generated message map functions
protected:
  void InitializeTree();
  void SaveTree();
  void AdjustLayout();

  void OnTOC_DblClick(NMHDR* hdr, LRESULT* pResult);
  void OnTOC_RClick  (NMHDR* hdr, LRESULT* pResult);

  afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnNewBook();
  afx_msg void OnNewPage();
  afx_msg void OnBookFromPage();
  afx_msg void OnPageFromBook();
  afx_msg void OnEditPage();
  afx_msg void OnMoveUp();
  afx_msg void OnMoveDown();
  afx_msg void OnMoveLeft();
  afx_msg void OnMoveRight();
  afx_msg void OnRename();
  afx_msg void OnDelete();
  afx_msg void OnFindTopic();
  afx_msg void OnProperties();
  afx_msg void OnUpdateMoveUp(CCmdUI* pCmdUI);
  afx_msg void OnUpdateMoveDown(CCmdUI* pCmdUI);
  afx_msg void OnUpdateMoveLeft(CCmdUI* pCmdUI);
  afx_msg void OnUpdateMoveRight(CCmdUI* pCmdUI);
  afx_msg void OnSize(UINT nType,int cx,int cy);


  DECLARE_MESSAGE_MAP()
};