//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    TagEventDlg.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for the event of a tag.
//
#pragma once
#include "resource.h"
#include "HTMLElement.h"

// TagEventDlg dialog

class TagEventDlg : public CDialog
{
	DECLARE_DYNAMIC(TagEventDlg)

public:
	TagEventDlg(CWnd* pParent
             ,HtmlElement* elem
             ,CString tag
             ,CString event);
	virtual ~TagEventDlg();
  virtual BOOL OnInitDialog();
  CString GetEvent();
  CString GetCode();

// Dialog Data
	enum { IDD = IDD_TAGEVENT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
  void FillPage();
  void UpdateProperties();

  HtmlElement* m_elem;
  CString      m_tag;
  CString      m_tagdescript;
  CString      m_id;
  CString      m_name;
  CString      m_event;
  CString      m_description;
  CString      m_code;
  CComboBox  m_comboEvent;
  // Standard buttons
  AD_Button      m_buttonOK;
  AD_Button      m_buttonCancel;
  AD_Button      m_buttonHelp;
public:
  afx_msg void OnCbnSelchangeEvent();
  afx_msg void OnEnChangeScript();
  afx_msg void OnBnClickedOk();
  afx_msg void OnBnClickedCancel();
  afx_msg void OnBnClickedScriptHelp();
  afx_msg void OnEnChangeForhtml();
  afx_msg void OnEnChangeId();
  afx_msg void OnEnChangeTagname();
  afx_msg void OnEnChangeDescript();
};

inline CString
TagEventDlg::GetEvent()
{
  return m_event;
}

inline CString
TagEventDlg::GetCode()
{
  return m_code;
}