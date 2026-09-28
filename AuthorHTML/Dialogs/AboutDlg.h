//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    AboutDlg.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Showing the about information of the program
//
#pragma once
#include "StdAfx.h"
#include "StyleRichEdit.h"
#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class AboutDlg : public CDialog
{
public:
  AboutDlg();
  virtual BOOL OnInitDialog() override;
  // Dialog Data
  enum { IDD = IDD_ABOUTBOX };
  // ClassWizard generated virtual function overrides
protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  CString       m_text;
  StyleRichEdit m_edit;
  // Implementation
protected:
  DECLARE_MESSAGE_MAP()
};
