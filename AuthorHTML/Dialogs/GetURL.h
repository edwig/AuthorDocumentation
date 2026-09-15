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
#include "addresscombo.h"
#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CGetURL dialog

class CGetURL : public CDialog
{
public:
  CGetURL(CWnd* pParent = NULL);
  CString GetURL() { return m_URL; }

  enum { IDD = IDD_URLDIALOG };

protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  void OnGo();
  void OnCloseup();
  void FillAddressAndBrowser();

  CString                 m_URL;
  CString                 m_szCurAddr;
  CComQIPtr<IWebBrowser2> m_spBrowser;
  AddressCombo            m_addressCombo;

  virtual void OnOK();
  virtual BOOL OnInitDialog();

  void OnDocumentComplete(LPDISPATCH pDisp, LPVARIANT pURL);

  DECLARE_MESSAGE_MAP()
  DECLARE_EVENTSINK_MAP()

public:
  afx_msg void OnBnClickedButtonOpen();
};

