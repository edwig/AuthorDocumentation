//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    StyleSheetsDlg.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Overview of CSS files
//
#pragma once
#include "StdAfx.h"
#include "resource.h"

typedef struct _SheetDef
{
  IHTMLElement* link;     // Linked sheet -> see filename
  IHTMLElement* style;    // Inline style -> see csstext
  CString       filename;
  CString       csstext;
}
SheetDef;

typedef std::vector<SheetDef> CSSVector;

// StyleSheetsDlg dialog

class StyleSheetsDlg : public CDialog
{
	DECLARE_DYNAMIC(StyleSheetsDlg)

public:
	StyleSheetsDlg(CWnd*        pParent 
               ,CString       typeText
               ,CSSVector&    list
               ,CComPtr<IHTMLDocument2> pDoc
               ,CString       p_base);
	virtual ~StyleSheetsDlg();
  BOOL     OnInitDialog();
  void     SetStyleSheets();
  bool     GetDidChanges();

// Dialog Data
	enum { IDD = IDD_STYLESHEETS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
  void  EmptyListbox();
  void  AddArrayToListbox();
  IHTMLElement* SetLink(CString linkURL);
  IHTMLElement* SetStyle(CString cssText);
  void          DeleteNode(IHTMLElement* elem);

  CString       m_typeText;   // Text with information about type of CSS
  // CStringArray* m_list
  CSSVector&    m_list;
  CListBox      m_listBox;    // List of CSS
  CComPtr<IHTMLDocument2> m_doc;
  bool          m_changes;    // Changed state in dialog
  bool          m_didChanges; // Did synchronize at least once
  CString       m_base;       // Base directory

  AD_Button   m_buttonEdit;
  AD_Button   m_buttonNew;
  AD_Button   m_buttonAttach;
  AD_Button   m_buttonDelete;
  AD_Button   m_buttonUp;
  AD_Button   m_buttonDown;

  AD_Button   m_buttonOK;
  AD_Button   m_buttonApply;
  AD_Button   m_buttonCancel;
  AD_Button   m_buttonHelp;

public:
  afx_msg void OnLbnSelchangeCssList();
  afx_msg void OnLbnDoubleClickList();
  afx_msg void OnBnClickedCssUp();
  afx_msg void OnBnClickedCssDown();
  afx_msg void OnBnClickedCssEdit();
  afx_msg void OnBnClickedCssNew();
  afx_msg void OnBnClickedCssDelete();
  afx_msg void OnBnClickedOk();
  afx_msg void OnBnClickedCssApply();
  afx_msg void OnBnClickedCancel();
  afx_msg void OnBnClickedCssHelp();
  afx_msg void OnBnClickedSsAttach();
};

inline bool
StyleSheetsDlg::GetDidChanges()
{
  return m_didChanges;
}