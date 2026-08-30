//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    CSSPage2Dlg.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for editing CSS properties
//              Handles the text attributes basics
//
#pragma once
#include "StdAfx.h"
#include "CSS.h"
#include "resource.h"

// CSSPage2Dlg dialog

class CSSPage2Dlg : public CDialog
{
	DECLARE_DYNAMIC(CSSPage2Dlg)

public:
	CSSPage2Dlg(CWnd*          pParent
             ,CssStyleSheet* pCss);
	virtual ~CSSPage2Dlg();
  BOOL     OnInitDialog();
  void     SetSelector(string selector);
  void     SetBackgroundColor(CString color);
  void     UpdateProperties();

// Dialog Data
	enum { IDD = IDD_SS_PAGE2 };

protected:
	void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  void OnDocumentComplete(LPDISPATCH /*pDisp*/, LPVARIANT /*pURL*/);

	DECLARE_MESSAGE_MAP()
  DECLARE_EVENTSINK_MAP()

private:
  void FillPage();
  void Redisplay();

  CComQIPtr<IWebBrowser2>   m_spBrowser;
  CComQIPtr<IHTMLDocument2> m_doc;
  CssStyleSheet* m_css;
  string         m_selector;
  // Properties
  CString        m_fontColor;
  CString        m_fontWeight;
  CString        m_fontStyle;
  CString        m_fontTransform;
  CString        m_fontAlign;
  CString        m_fontFamily;
  CString        m_fontSize;
  CString        m_fontSizeUnits;
  CString        m_lineHeigth;
  CString        m_lineHeigthUnits;
  CString        m_lineIndent;
  CString        m_lineIndentUnits;
  CString        m_letterSpacing;
  CString        m_letterSpacingUnits;
  CString        m_fontJustify;
  CString        m_fontDecoration;
  CString        m_fontWhitespace;
  CString        m_fontWritingMode;
  CString        m_backgroundColor;
  bool           m_hasFontColor;
  bool           m_initDone;
  // Font Family Controls
  AD_Radiobox    m_buttonFontUnspec;
  AD_Radiobox    m_buttonFontPredef;
  AD_Radiobox    m_buttonFontSpecify;
  CComboBox      m_comboFontPredefined;
  CComboBox      m_comboFontSpecify;
  // Font size controls
  CComboBox    m_comboFontSize;
  CComboBox    m_comboFontSizeUnits;
  CComboBox    m_comboLineHeigth;
  CComboBox    m_comboLineHeightUnits;
  CComboBox    m_comboLineIndentUnits;
  CComboBox    m_comboLetterSpacingUnits;
  // Font Controls
  AD_ColorButton m_buttonColor;
  AD_Checkbox    m_buttonHasColor;
  CComboBox    m_comboFontWeight;
  CComboBox    m_comboFontStyle;
  CComboBox    m_comboFontTransform;
  CComboBox    m_comboFontAlign;
  CComboBox    m_comboFontJustify;
  CComboBox    m_comboFontDecoration;
  CComboBox    m_comboFontWhitespace;
  CComboBox    m_comboFontWritingMode;
public:
  afx_msg void OnBnClickedSsColorbutton();
  afx_msg void OnCbnSelchangeSsFontweight();
  afx_msg void OnCbnSelchangeSsFontstyle();
  afx_msg void OnCbnSelchangeSsFontcase();
  afx_msg void OnCbnSelchangeSsFontalign();
  afx_msg void OnCbnSelchangeSsFontJustify();
  afx_msg void OnCbnSelchangeSsFontDecoration();
  afx_msg void OnCbnSelchangeSsFontWhitespace();
  afx_msg void OnBnClickedFfUnspec();
  afx_msg void OnBnClickedFfPredef();
  afx_msg void OnBnClickedFfCustom();
  afx_msg void OnCbnSelchangeSsPredeffont();
  afx_msg void OnCbnSelchangeFontfamily();
  afx_msg void OnCbnSelchangeSsFontsize();
  afx_msg void OnCbnSelchangeFsUnits();
  afx_msg void OnCbnSelchangeSsLineheight();
  afx_msg void OnCbnSelchangeLhUnits();
  afx_msg void OnCbnSelchangeLiUnits();
  afx_msg void OnEnChangeSsFontcolor();
  afx_msg void OnBnClickedCheckColor();
  afx_msg void OnEnChangeIndent();
  afx_msg void OnEnChangeSsLetterspacing();
  afx_msg void OnCbnSelchangeLsUnits();
  afx_msg void OnCbnSelchangeWritingmode();
};

