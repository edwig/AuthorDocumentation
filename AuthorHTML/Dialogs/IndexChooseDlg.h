//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    IndexChooseDlg.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for choosing a reference to an index entry
//
#pragma once
#include "IndexEntry.h"
#include "resource.h"

// IndexChooseDlg dialog

class IndexChooseDlg : public CDialog
{
  DECLARE_DYNAMIC(IndexChooseDlg)

public:
  IndexChooseDlg(CWnd* pParent
                ,CString base
                ,IndexEntry* entry);
  virtual ~IndexChooseDlg();
  virtual BOOL OnInitDialog();
  int GetChosenReference();

  // Dialog Data
  enum { IDD = IDD_INDEX_CHOOSE };

protected:
  void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  void OnDocumentComplete(LPDISPATCH pDisp,LPVARIANT pURL);
  void ScrollIntoView(CString bookmark);

  DECLARE_MESSAGE_MAP()
  DECLARE_EVENTSINK_MAP()

private:
  void RedisplayPages();
  int         m_current;
  CString     m_base;
  IndexEntry* m_entry;
  CComQIPtr<IWebBrowser2> m_spBrowser;

  CListCtrl   m_list;
  CComboBox   m_comboBM;
  AD_Button   m_buttonOK;
  AD_Button   m_buttonCancel;
  AD_Button   m_buttonHelp;
public:
  afx_msg void OnBnClickedIntopic();
  afx_msg void OnBnClickedInfile();
  afx_msg void OnLvnItemchangedList(NMHDR *pNMHDR, LRESULT *pResult);
  afx_msg void OnListDblClick      (NMHDR *pNMHDR, LRESULT *pResult);
  afx_msg void OnCbnSelchangeBookmarks();
  afx_msg void OnBnClickedOk();
  afx_msg void OnBnClickedCancel();
  afx_msg void OnBnClickedHelpIndexprop();
};

inline int 
IndexChooseDlg::GetChosenReference()
{
  return m_current;
}
