//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    TopicPropPage3.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for all topic properties page 3 of 5
//
#pragma once
#include "HtmlElement.h"
#include "resource.h"
#include "DocumentFile.h"
#include "Keyword.h"

// TopicPropPage3 dialog
class TopicPropPage3Dlg : public CDialog
{
	DECLARE_DYNAMIC(TopicPropPage3Dlg)

public:
	TopicPropPage3Dlg(CWnd* pParent
                   ,CComPtr<IHTMLDocument2> pDoc
                   ,CString base
                   ,DocumentFile* doc);
	virtual ~TopicPropPage3Dlg();
  virtual BOOL OnInitDialog();
  void UpdateProperties();
  void ShowFocus();
  bool GetChanged();

// Dialog Data
	enum { IDD = IDD_TOPIC_PROPPAGE3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
  void FillPage();
  void GetHeadKeywords();
  void RemoveHeadKeywords();
  void RewriteHeadKeywords();
  void KeywordsToList();
  void AddKeywords(KeywordType p_type,CString p_keywords);
  void AddMapID(unsigned p_mapID,CString p_alias);

  CComPtr<IHTMLDocument2> m_htmlDoc;
  DocumentFile*           m_doc;
  CListCtrl               m_list;
  KeywordVector           m_keywords;
  CString                 m_base;
  bool                    m_changed;
  // Controls
  AD_Button   m_buttonUp;
  AD_Button   m_buttonEdit;
  AD_Button   m_buttonNew;
  AD_Button   m_buttonDelete;
  AD_Button   m_buttonDown;
public:
  afx_msg void OnLvnChanged   (NMHDR* pNMHDR, LRESULT* pResult);
  afx_msg void OnLvnItemDouble(NMHDR* pNMHDR, LRESULT* pResult);
  afx_msg void OnBnClickedUp();
  afx_msg void OnBnClickedEdit();
  afx_msg void OnBnClickedNew();
  afx_msg void OnBnClickedDelete();
  afx_msg void OnBnClickedDown();
};
