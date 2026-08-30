//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    ChildFrm.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// The Child frame for a documentation HTML file
//
#pragma once
#include "StdAfx.h"
#include "HTMLEdView.h"
#include "ScintillaHTMLView.h"
#include <afxmdichildwndex.h>

class CMDIChildFrame : public CMDIChildWndEx
{
	DECLARE_DYNCREATE(CMDIChildFrame)
public:
	CMDIChildFrame();
// Operations
public:
  // Overrides
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
  LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

// Implementation
public:
	virtual ~CMDIChildFrame();
  void          SwapView();
  void          SwapView(int nCmdId);
  CView*        GetSourceView();
  CHTMLEdView*  GetWebView();
  DWORD         GetCurrentView();
  CView*        GetActiveView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
  int OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void  OnSysCommand (UINT nID,LPARAM lParam);
  afx_msg void  OnDestroy ();
  afx_msg void  OnMDIActivate (BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd);
  afx_msg LRESULT OnSetText (WPARAM, LPARAM lParam);
  afx_msg void OnViewSource();
  afx_msg void OnViewWeb();
  afx_msg void OnUpdateViewWeb(CCmdUI* pCmdUI);
  afx_msg void OnUpdateViewSource(CCmdUI* pCmdUI);
  afx_msg void OnUpdateFontSize(CCmdUI* pCmdUI);
  afx_msg void OnUpdateFontName(CCmdUI* pCmdUI);
  afx_msg void OnUpdateClassName(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSelectColor(CCmdUI* pCmdUI);
	afx_msg void OnSelectColor(UINT p_nColor);
	afx_msg void OnUpdateSelectColorNone(CCmdUI* pCmdUI);
	afx_msg void OnSelectColorNone();
	afx_msg LRESULT OnGetTabColor(WPARAM, LPARAM);

  DECLARE_MESSAGE_MAP()

  virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);

	int                m_nColor;    // COlor of the tab
  ScintillaHTMLView* m_pSrcView;
  CHTMLEdView*       m_pWebView;
  DWORD              m_dwCurrentView;
};

inline CHTMLEdView* 
CMDIChildFrame::GetWebView()
{
  return m_pWebView;
}

