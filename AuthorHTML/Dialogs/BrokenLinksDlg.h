//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    BrokenLinksDlg.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog to sum-up the broken links in a project and to be able
//              to sweep the project for broken links and repair them
//
#pragma once
#include "Resource.h"
#include "ProjectFile.h"

// BrokenLinksDlg dialog

class BrokenLinksDlg : public CDialog
{
	DECLARE_DYNAMIC(BrokenLinksDlg)

public:
	BrokenLinksDlg(CWnd* p_parent,ProjectFile* p_project);
	virtual ~BrokenLinksDlg();
  virtual BOOL OnInitDialog();

// Dialog Data
	enum { IDD = IDD_BROKENLINKS };

protected:
	void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  void FillPage();

	DECLARE_MESSAGE_MAP()

  afx_msg void OnBnClickedSweep();
  afx_msg void OnDblClckLink(NMHDR* pNMHDR, LRESULT *pResult);

  CString       m_naam;
  CListCtrl     m_list;
  CButton       m_buttonSweep;
  // Data
  ProjectFile*  m_project;
};
