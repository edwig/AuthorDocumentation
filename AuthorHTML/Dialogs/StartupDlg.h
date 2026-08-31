//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    StartupDlg.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Choose an action upon startup of the program. 
//              Either open a recent project or file, or create a new one.
//
#pragma once
#include "resource.h"

// StartupDlg dialog

class StartupDlg : public CDialog
{
	DECLARE_DYNAMIC(StartupDlg)

public:
	StartupDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~StartupDlg();
  BOOL OnInitDialog();

// Dialog Data
	enum { IDD = IDD_STARTUP };

protected:
	void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  void GetRecentProjectList();
  void GetRecentFileList();
  void SetButtonText(bool p_project);
  void SetButtonImages();

  bool        m_listsProjects;
  CListCtrl   m_list;
  CMFCButton  m_buttonOpenProject;
  CMFCButton  m_buttonOpenFile;
  CMFCButton  m_buttonNewProject;
  CMFCButton  m_buttonNewFile;
  CMFCButton  m_buttonImport;
  CMFCButton  m_buttonHelp;
  CMFCButton  m_buttonCancel;

  CBitmap     m_project;

	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnBnClickedOpenproject();
  afx_msg void OnBnClickedOpenfile();
  afx_msg void OnBnClickedNewproject();
  afx_msg void OnBnClickedNewfile();
  afx_msg void OnBnClickedImport();
  afx_msg void OnBnClickedHelp();
  afx_msg void OnLvnItemchangedList(NMHDR *pNMHDR, LRESULT *pResult);
  afx_msg void OnBnClickedCancel();
};
