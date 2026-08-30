//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    TopicPropPage1.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for all topic properties page 1 of 5
//
#pragma once
#include "StdAfx.h"
#include "DocumentFile.h"
#include "resource.h"
#include "resource.h"

// TopicPropPage1Dlg dialog

class TopicPropPage1Dlg : public CDialog
{
	DECLARE_DYNAMIC(TopicPropPage1Dlg)

public:
	TopicPropPage1Dlg(CWnd* pParent
                   ,DocumentFile* pDocument
                   ,CComPtr<IHTMLDocument2> pDoc);
	virtual ~TopicPropPage1Dlg();
  virtual BOOL    OnInitDialog();
          void    UpdateDocumentFile();
          bool    RenameBaseFile();

  // Dialog Data
	enum { IDD = IDD_TOPIC_PROPPAGE1 };

private:
  // Objects
  DocumentFile*           m_document;
  CComPtr<IHTMLDocument2> m_htmlDoc;
  // Field values
  CString m_title;
  CString m_fileName;
  CString m_template;
  CString m_author;
  CString m_comment;
  int     m_status;
  int     m_priority;
  int     m_timeSpent;
  int     m_todo;
  // Controls
  AD_Button     m_buttonFile;
  AD_Button     m_buttonTemplate;
  CComboBox     m_comboStatus;
  CComboBox     m_comboPriority;
  AD_Checkbox   m_todo1;
  AD_Checkbox   m_todo2;
  AD_Checkbox   m_todo3;
  AD_Checkbox   m_todo4;
  AD_Checkbox   m_todo5;
  AD_Checkbox   m_todo6;
  AD_Checkbox   m_todo7;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnCbnSelchangeTopicStatus();
  afx_msg void OnCbnSelchangeTopicPriority();
  afx_msg void OnBnClickedTodo1();
  afx_msg void OnBnClickedTodo2();
  afx_msg void OnBnClickedTodo3();
  afx_msg void OnBnClickedTodo4();
  afx_msg void OnBnClickedTodo5();
  afx_msg void OnBnClickedTodo6();
  afx_msg void OnBnClickedTodo7();
  afx_msg void OnEnChangeTitle();
  afx_msg void OnEnChangeFilename();

};
