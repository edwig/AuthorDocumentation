//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    TopicProperties.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for all topic properties pages
//
#pragma once
#include "StdAfx.h"
#include "DocumentFile.h"
#include "TopicPropPage1.h"
#include "TopicPropPage2.h"
#include "TopicPropPage3.h"
#include "TopicPropPage4.h"
#include "TopicPropPage5.h"
#include "resource.h"

// TopicPropertiesDlg dialog

class TopicPropertiesDlg : public CDialog
{
	DECLARE_DYNAMIC(TopicPropertiesDlg)

public:
	TopicPropertiesDlg(CWnd*         pParent
                    ,DocumentFile* pDocument
                    ,CComPtr<IHTMLDocument2> pDoc
                    ,CString       base);
	virtual ~TopicPropertiesDlg();
  virtual BOOL    OnInitDialog();
          bool    GetRenamed();

// Dialog Data
	enum { IDD = IDD_TOPIC_PROPERTIES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

private:
  void UpdateProperties();

  bool                    m_renamed;
  bool                    m_hasBody;    // Otherwise: frameset!
  CTabCtrl	              m_tabControl;
  TopicPropPage1Dlg*      m_page1;
  TopicPropPage2Dlg*      m_page2;
  TopicPropPage3Dlg*      m_page3;
  TopicPropPage4Dlg*      m_page4;
  TopicPropPage5Dlg*      m_page5;
  int                     m_firstTab;
  // Common controls
  AD_Button     m_buttonOK;
  AD_Button     m_buttonApply;
  AD_Button     m_buttonCancel;
  AD_Button     m_buttonHelp;

	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnBnClickedOk();
  afx_msg void OnBnClickedCancel();
  afx_msg void OnBnClickedApply();
  afx_msg void OnBnClickedHelp();
  afx_msg void OnTcnSelchangeTabcontrol(NMHDR *pNMHDR, LRESULT *pResult);
};

inline bool
TopicPropertiesDlg::GetRenamed()
{
  return m_renamed;
}