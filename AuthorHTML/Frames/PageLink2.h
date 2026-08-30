//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    PageLink2.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Shows the links on THIS page to other pages 
//              AND shows the links from OTHER pages to this page.
//
#pragma once

class CHTMLEdView;

class PageLink2: public CDockablePane
{
public:
  PageLink2();
  virtual ~PageLink2();
  void   ActivatePage(CHTMLEdView* p_view);
  void   RemoveLinks();

protected:
  void  ActivateOpenFile(CListCtrl* list);
  void  LinksStarted();
  void  SetPageLinks();

  CListCtrl    m_listFrom;  // From this page to another
  CListCtrl    m_listToMe;  // From another page ToMe
  CImageList*  m_imgList;
protected:
  DECLARE_MESSAGE_MAP()
private:
  CHTMLEdView* m_htmlView;
  int          m_retry;
public:
  afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnTimer(UINT_PTR nIDEvent);
  afx_msg void OnSize (UINT nType, int cx, int cy);
  afx_msg void OnOpenFiles_Click_From  (NMHDR* pNMHDR, LRESULT* pResult);
  afx_msg void OnOpenFiles_Click_ToMe  (NMHDR* pNMHDR, LRESULT* pResult);
  afx_msg void OnOpenFiles_RClick_From (NMHDR* pNMHDR, LRESULT* pResult);
  afx_msg void OnOpenFiles_RClick_ToMe (NMHDR* pNMHDR, LRESULT* pResult);
  afx_msg void OnOpenFiles_KeyDown_From(NMHDR* pNMHDR, LRESULT* pResult);
  afx_msg void OnOpenFiles_KeyDown_ToMe(NMHDR* pNMHDR, LRESULT* pResult);
};

