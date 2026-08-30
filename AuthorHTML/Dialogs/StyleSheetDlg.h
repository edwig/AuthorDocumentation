//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    StyleSheetDlg.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for managing CSS style sheets
//              Holds all 7 pages of CSS properties
//
#pragma once
#include "StdAfx.h"
#include "css.h"
#include "resource.h"

// Macros for displaying CSS pages on the tab control
#define CSS1   0x001
#define CSS2   0x002
#define CSS3   0x004
#define CSS4   0x008
#define CSS5   0x010
#define CSS6   0x020
#define CSS7   0x040
#define CSSALL (CSS1 | CSS2 | CSS3 | CSS4 | CSS5 | CSS6 | CSS7)

// GENERAL STATIC FUNCTIONS FOR ALL STYLESHEET PAGES/DIALOGS

class CSSPage1Dlg;
class CSSPage2Dlg;
class CSSPage3Dlg;
class CSSPage4Dlg;
class CSSPage5Dlg;
class CSSPage6Dlg;
class CSSPage7Dlg;

extern CString
CSSPropertyGet(CssStyleSheet* css
              ,string& selector
              ,string  property
              ,bool unspec /*=true*/);
extern void
CSSPropertyPut(CssStyleSheet* css
              ,string& selector
              ,string  property
              ,CString value
              ,bool    unspec);
extern void
CssSplitValueUnits(CString property,CString& value,CString& units);
extern CComBSTR
CssCtoB(CString value);

// StyleSheetDlg dialog

class StyleSheetDlg : public CDialog
{
	DECLARE_DYNAMIC(StyleSheetDlg)

public:
	StyleSheetDlg(CWnd*           p_Parent
               ,CString         p_base
               ,CString         p_typeText
               ,CssStyleSheet*  p_css
               ,CString         p_cssText
               ,bool            p_tagonly = true
               ,int             p_tabs = CSSALL);
	virtual ~StyleSheetDlg();
  BOOL    OnInitDialog();
  void    UpdateProperties();
  bool    DidApply();
  void    SetCanApply();
  CString GetInlineStylesheet();
  CString GetBaseDirectory();
  void    SetBackgroundColor(CString color);

  // Dialog Data
	enum { IDD = IDD_STYLESHEET };

protected:
	void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  void OnTcnSelchangeTabcontrol(NMHDR *pNMHDR, LRESULT *pResult);
  void FillSelectorList();

	DECLARE_MESSAGE_MAP()

private:
  void MoveButton(AD_Button* but);

  CString   m_base;               // Base directory for files
  CString   m_typeText;           // Type text to display
  int       m_firstTab;           // First tab to show
  int       m_doTabs;             // Which tabs to display (See CSS* macros)
  bool      m_inline;             // Started with a inlne stylesheet
  bool      m_tagonly;            // Sheet for **one** tag
  CString   m_inlineSheet;        // Inline representation of a style sheet
  CString   m_tag;                // Tagonly tag 
  CComPtr<IHTMLStyle> m_style;    // DHTML  representation of a style sheet
  CssStyleSheet*      m_css;      // Parsed stylesheet
  bool      m_canApply;
  bool      m_didApply;
  // MAIN CONTROLS
  CListBox  m_listBox;
  CTabCtrl  m_tabs;
  // Selector butons
  AD_Button   m_buttonNew;
  AD_Button   m_buttonDelete;
  AD_Button   m_buttonImports;
  // Standard buttons
  AD_Button   m_buttonOK;
  AD_Button   m_buttonApply;
  AD_Button   m_buttonCancel;
  AD_Button   m_buttonHelp;
  // Pages
  CSSPage1Dlg* m_page1;
  CSSPage2Dlg* m_page2;
  CSSPage3Dlg* m_page3;
  CSSPage4Dlg* m_page4;
  CSSPage5Dlg* m_page5;
  CSSPage6Dlg* m_page6;
  CSSPage7Dlg* m_page7;
public:
  afx_msg void OnLbnSelchangeSsSelectors();
  afx_msg void OnBnClickedSsNew();
  afx_msg void OnBnClickedSsDelete();
  afx_msg void OnBnClickedOk();
  afx_msg void OnBnClickedSsApply();
  afx_msg void OnBnClickedCancel();
  afx_msg void OnBnClickedSsHelp();
  afx_msg void OnBnClickedImports();
};

inline CString 
StyleSheetDlg::GetInlineStylesheet()
{
  return m_inlineSheet;
}

inline CString 
StyleSheetDlg::GetBaseDirectory()
{
  return m_base;
}

inline bool
StyleSheetDlg::DidApply()
{
  return m_didApply;
}