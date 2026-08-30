//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    GetURL.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for managing the URL
//
#pragma once

/////////////////////////////////////////////////////////////////////////////
// CGetURL dialog
#include "addresscombo.h"
#include "resource.h"

class CGetURL : public CDialog
{
public:
	CGetURL(CWnd* pParent = NULL);   // standard constructor
	CString m_URL;

	enum { IDD = IDD_URLDIALOG };

// Overrides
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	void OnGo();
	void OnCloseup();
	CString m_szCurAddr;
	CComQIPtr<IWebBrowser2> m_spBrowser;
	CAddressCombo m_AddrCombo;

	virtual void OnOK();
	virtual BOOL OnInitDialog();

	void OnDocumentComplete(LPDISPATCH pDisp, LPVARIANT pURL);
	DECLARE_MESSAGE_MAP()
	DECLARE_EVENTSINK_MAP()
public:
  afx_msg void OnBnClickedButtonOpen();
};

