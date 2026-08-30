//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    TopicPropPage4.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for all topic properties page 4 of 5
//
#pragma once
#include "HtmlElement.h"
#include "resource.h"

#undef ScriptDef
#undef _ScriptDef

typedef struct _ScriptDef
{
  IHTMLScriptElement* script;
  CString language;
  CString type;
  bool    deferred;
  CString src;
  CString forHtml;
  CString event;
  CString code;
}
ScriptDef;

typedef std::vector<ScriptDef> ScriptVector;

// TopicPropPage4 dialog
class TopicPropPage4Dlg : public CDialog
{
  DECLARE_DYNAMIC(TopicPropPage4Dlg)

public:
  TopicPropPage4Dlg(CWnd* pParent
                   ,CComPtr<IHTMLDocument2> pDoc
                   ,CString base);
  virtual ~TopicPropPage4Dlg();
  virtual BOOL OnInitDialog();
  void UpdateProperties();
  void ResetScripts();
  void ShowFocus();

  // Dialog Data
  enum { IDD = IDD_TOPIC_PROPPAGE4 };

protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  DECLARE_MESSAGE_MAP()
private:
  void FillPage();
  void GetHeadScripts();
  void ScriptsToList();

  CComPtr<IHTMLDocument2> m_htmlDoc;
  CListCtrl               m_list;
  ScriptVector            m_scripts;
  CString                 m_base;
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
