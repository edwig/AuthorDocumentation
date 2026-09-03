//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    IndexPropDlg.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for managing the properties of an index entry
//
#pragma once
#include "IndexEntry.h"
#include "resource.h"

// IndexPropDlg dialog

class IndexPropDlg : public CDialog
{
	DECLARE_DYNAMIC(IndexPropDlg)

public:
	IndexPropDlg(CWnd* pParent
              ,CString base
              ,IndexEntry* entry);
	virtual ~IndexPropDlg();
  virtual BOOL OnInitDialog();

// Dialog Data
	enum { IDD = IDD_INDEX_PROPERTIES };

protected:
	void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  void OnDocumentComplete(LPDISPATCH pDisp,LPVARIANT pURL);
  bool ScrollIntoView(CString bookmark);

	DECLARE_MESSAGE_MAP()
  DECLARE_EVENTSINK_MAP()

private:
  void FillPage();
  void UpdateProperties();
  void RedisplayPages();
  CString     m_base;
  IndexEntry* m_entry;
  CComQIPtr<IWebBrowser2> m_spBrowser;

  CString     m_keyword;
  CString     m_frame;
  CString     m_window;
  CString     m_comment;
  CString     m_displayFile;
  CString     m_bookmark;
  bool        m_inHHK;
  AD_Button   m_buttonDelete;
  AD_Button   m_buttonFind;
  CButton     m_buttonInHHK;
  CButton     m_buttonInTopic;
  CComboBox   m_comboWindow;
  CComboBox   m_comboFrame;
  CListCtrl   m_list;
  CComboBox   m_comboBM;
  AD_Button   m_buttonOK;
  AD_Button   m_buttonCancel;
  AD_Button   m_buttonHelp;
public:
  afx_msg void OnEnChangeKeyword();
  afx_msg void OnBnClickedDelete();
  afx_msg void OnBnClickedFind();
  afx_msg void OnCbnSelchangeWindow();
  afx_msg void OnCbnSelchangeFrame();
  afx_msg void OnEnChangeComment();
  afx_msg void OnBnClickedIntopic();
  afx_msg void OnBnClickedInfile();
  afx_msg void OnLvnItemchangedList(NMHDR *pNMHDR, LRESULT *pResult);
  afx_msg void OnCbnSelchangeBookmarks();
  afx_msg void OnBnClickedOk();
  afx_msg void OnBnClickedCancel();
  afx_msg void OnBnClickedHelpIndexprop();
};
