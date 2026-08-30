//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    FindTopicDlg.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for finding topics.
//
#pragma once
#include "resource.h"

// FindTopicDlg dialog

class FindTopicDlg : public CDialog
{
	DECLARE_DYNAMIC(FindTopicDlg)

public:
	FindTopicDlg(CWnd* pParent,CString base,bool p_allowBookmarks = true);
	virtual ~FindTopicDlg();
  int      OnInitDialog();
  CString  GetFoundTopic();
  CString  GetFoundTitle();

// Dialog Data
	enum { IDD = IDD_FIND_TOPIC };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  void ScrollIntoView(CString bookmark);

  DECLARE_EVENTSINK_MAP()
	DECLARE_MESSAGE_MAP()

  CString m_base;
  CString m_foundTopic;
  CString m_foundTitle;
  CString m_findTitle;
  CString m_findFile;
  CString m_bookmark;
  bool    m_allowBookmarks;

  CComQIPtr<IWebBrowser2> m_spBrowser;
  CComboBox   m_comboBM;
  CListCtrl   m_list;
  AD_Button   m_buttonFind;
  AD_Button   m_buttonOK;
  AD_Button   m_buttonCancel;
  AD_Button   m_buttonHelp;
public:
  afx_msg void OnDocumentComplete(LPDISPATCH pDisp,LPVARIANT pURL);
  afx_msg void OnEnChangeTitle();
  afx_msg void OnEnChangeFilename();
  afx_msg void OnBnClickedFind();
  afx_msg void OnLvnItemchangedList(NMHDR *pNMHDR, LRESULT *pResult);
  afx_msg void OnCbnSelchangeBookmarks();
  afx_msg void OnBnClickedOk();
  afx_msg void OnBnClickedCancel();
  afx_msg void OnBnClickedHelpFindtopic();
};

inline CString
FindTopicDlg::GetFoundTopic()
{
  return m_foundTopic;
}

inline CString
FindTopicDlg::GetFoundTitle()
{
  return m_foundTitle;
}
