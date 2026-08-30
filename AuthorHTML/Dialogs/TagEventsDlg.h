//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    TagEventsDlg.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for all the events of a tag.
//
#pragma once
#include "resource.h"
#include "HTMLElement.h"

// TagEventsDlg dialog

class TagEventsDlg : public CDialog
{
	DECLARE_DYNAMIC(TagEventsDlg)

public:
	TagEventsDlg(CWnd* pParent,HtmlElement* elem,CString tag);
	virtual ~TagEventsDlg();
  virtual BOOL OnInitDialog();

// Dialog Data
	enum { IDD = IDD_TAGEVENTS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
  void DisplayList();
  void UpdateProperties();

  HtmlElement* m_elem;
  CString      m_tag;
  CString      m_descript;
  CString      m_id;
  CString      m_name;
  std::vector<CString> m_allEvents;
  // Controls
  CListCtrl m_list;
  AD_Button   m_buttonEdit;
  AD_Button   m_buttonNew;
  AD_Button   m_buttonDelete;
  // Standard controls
  AD_Button   m_buttonOK;
  AD_Button   m_buttonCancel;
  AD_Button   m_buttonHelp;
public:
  afx_msg void OnLvnItemchangedList(NMHDR *pNMHDR, LRESULT *pResult);
  afx_msg void OnBnClickedEdit();
  afx_msg void OnBnClickedNew();
  afx_msg void OnBnClickedDelete();
  afx_msg void OnBnClickedOk();
  afx_msg void OnBnClickedCancel();
  afx_msg void OnBnClickedEventsHelp();
};
