//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    TopicPropPage5.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for all topic properties page 5 of 5
//
#pragma once
#include "resource.h"
#include "HtmlFrameSet.h"

class TopicPropPage5Dlg : public CDialog
{
	DECLARE_DYNAMIC(TopicPropPage5Dlg)

public:
	TopicPropPage5Dlg(CWnd* p_parent,HtmlFrameSet p_frame,CString& p_base);
	virtual ~TopicPropPage5Dlg();
  void     UpdateProperties();

// Dialog Data
	enum { IDD = IDD_TOPIC_PROPPAGE5 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  CString      m_base;
  HtmlFrameSet m_frameset;

	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnEnChangeFrameCols();
  afx_msg void OnEnChangeFrameRows();
  afx_msg void OnEnChangeBorder();
  afx_msg void OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult);
  afx_msg void OnBnClickedBorderColor();
  afx_msg void OnBnClickedDoBc();
  afx_msg void OnBnClickedFrameborder();
  afx_msg void OnEnChangeFramespacing();
  afx_msg void OnDeltaposSpin2(NMHDR *pNMHDR, LRESULT *pResult);
  afx_msg void OnBnClickedId();
  afx_msg void OnBnClickedEvents();
  afx_msg void OnBnClickedBodyStyle();
};
