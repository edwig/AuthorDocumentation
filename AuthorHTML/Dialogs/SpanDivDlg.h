//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    SnapDivDlg.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for managing Snap-to-DIV settings
//
#pragma once
#include "StdAfx.h"
#include "GeneralIDDlg.h"
#include "StyleSheetDlg.h"
#include "HtmlElement.h"
#include "resource.h"

// SpanDivDlg dialog

class SpanDivDlg : public CDialog
{
	DECLARE_DYNAMIC(SpanDivDlg)

public:
	SpanDivDlg(CWnd*        pParent
            ,CString      p_base
            ,CString      p_tag
            ,HtmlElement* p_elem);
	virtual ~SpanDivDlg();

// Dialog Data
	enum { IDD = IDD_SPANDIV };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
  HtmlElement* m_elem;
  CString m_base;
  CString m_tag;
  CString m_type;
  // Controls
  AD_Button m_buttonID;
  AD_Button m_buttonEvents;
  AD_Button m_buttonStyle;
  AD_Button m_buttonOK;
  AD_Button m_buttonCancel;
  AD_Button m_buttonHelp;
public:
  afx_msg void OnBnClickedId();
  afx_msg void OnBnClickedStyle();
  afx_msg void OnBnClickedOk();
  afx_msg void OnBnClickedCancel();
  afx_msg void OnBnClickedSdHelp();
  afx_msg void OnBnClickedEvents();
};
