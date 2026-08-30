//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    IndexView.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: A IndexView is a view over the keywords of the 
// keyword file or the keywords in all the documents
// enabeling to edit all these keywords at once
//
#pragma once
#include "IndexEntry.h"
#include "IndexTreeCtrl.h"

class IndexViewToolBar : public CMFCToolBar
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


class IndexView : public CDockablePane
{
  // Construction
public:
  IndexView();
  virtual ~IndexView();

  // Attributes
  IndexViewToolBar m_wndToolBar;
  IndexTreeCtrl    m_wndExplorer;
  CImageList*      m_imageList;

  // Generated message map functions
protected:
  void InitializeTree();
  void SaveTree();
  void AdjustLayout();

  void OnIndex_DblClick(NMHDR* hdr, LRESULT* pResult);
  void OnIndex_RClick  (NMHDR* hdr, LRESULT* pResult);

  afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnNewKeyword();
  afx_msg void OnNewSubkey();
  afx_msg void OnEditPage();
  afx_msg void OnMoveLeft();
  afx_msg void OnMoveRight();
  afx_msg void OnKeyUpper();
  afx_msg void OnKeyLower();
  afx_msg void OnDelete();
  afx_msg void OnProperties();
  afx_msg void OnUpdateMoveLeft(CCmdUI* pCmdUI);
  afx_msg void OnUpdateMoveRight(CCmdUI* pCmdUI);
  afx_msg void OnSize(UINT nType,int cx,int cy);


  DECLARE_MESSAGE_MAP()
};
