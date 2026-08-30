//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    ParagraphDlg.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for managing the <P> tag in HTML
//
#pragma once
#include "StdAfx.h"
#include "Css.h"
#include "StyleSheetDlg.h"
#include "resource.h"

// ParagraphDlg dialog

class ParagraphDlg : public CDialog
{
	DECLARE_DYNAMIC(ParagraphDlg)

public:
	ParagraphDlg(CWnd* pParent
              ,CString p_base
              ,CssStyleSheet* pCss
              ,CComPtr<IHTMLElement> pElem
              ,CComPtr<IHTMLStyle> pStyle);
	virtual ~ParagraphDlg();
  virtual BOOL OnInitDialog();
  void UpdateProperties();

// Dialog Data
	enum { IDD = IDD_PARAGRAPH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  void OnDocumentComplete(LPDISPATCH /*pDisp*/, LPVARIANT /*pURL*/);

	DECLARE_MESSAGE_MAP()
  DECLARE_EVENTSINK_MAP()

private:
  void FillPage();
  void Redisplay();

  CComQIPtr<IWebBrowser2>   m_spBrowser;
  CComQIPtr<IHTMLDocument2> m_doc;
  CComPtr<IHTMLElement>     m_elem;
  CComPtr<IHTMLStyle>       m_style;
  CssStyleSheet*            m_css;
  CString                   m_base;
  bool                      m_canApply;
  bool                      m_mySheet;

  // Properties
  CString     m_align;
  CString     m_marginLeft;
  CString     m_marginLeftUnit;
  CString     m_marginRight;
  CString     m_marginRightUnit;
  CString     m_marginTop;
  CString     m_marginTopUnit;
  CString     m_marginBottom;
  CString     m_marginBottomUnit;
  CString     m_lineHeight;
  CString     m_textIndent;
  CString     m_textIndentUnit;
  CString     m_wordSpacing;
  CString     m_wordSpacingUnit;

  // Controls
  CSpinButtonCtrl m_spinLeft;
  CComboBox     m_comboLeft;
  CSpinButtonCtrl m_spinRight;
  CComboBox     m_comboRight;
  CSpinButtonCtrl m_spinTop;
  CComboBox     m_comboTop;
  CSpinButtonCtrl m_spinBottom;
  CComboBox     m_comboBottom;
  CComboBox     m_comboAlign;
  CComboBox     m_comboSpacing;
  CSpinButtonCtrl m_spinIndent;
  CComboBox     m_comboIndent;
  CSpinButtonCtrl m_spinWord;
  CComboBox     m_comboWord;
  AD_Button         m_buttonID;
  AD_Button         m_buttonEvents;
  AD_Button         m_buttonStyle;
  AD_Button         m_buttonOK;
  AD_Button         m_buttonApply;
  AD_Button         m_buttonCancel;
  AD_Button         m_buttonHelp;
public:

  afx_msg void OnCbnSelchangeParaAlign();
  afx_msg void OnEnChangeParaBt();
  afx_msg void OnEnChangeParaAt();
  afx_msg void OnEnChangeParaSb();
  afx_msg void OnEnChangeParaSa();
  afx_msg void OnCbnSelchangeParaBtu();
  afx_msg void OnCbnSelchangeParaAtu();
  afx_msg void OnCbnSelchangeParaSbu();
  afx_msg void OnCbnSelchangeParaSau();
  afx_msg void OnDeltaposSpinBt(NMHDR *pNMHDR, LRESULT *pResult);
  afx_msg void OnDeltaposSpinAt(NMHDR *pNMHDR, LRESULT *pResult);
  afx_msg void OnDeltaposSpinSb(NMHDR *pNMHDR, LRESULT *pResult);
  afx_msg void OnDeltaposSpinSa(NMHDR *pNMHDR, LRESULT *pResult);
  afx_msg void OnCbnSelchangeParaLinespacing();
  afx_msg void OnEnChangeParaIndent();
  afx_msg void OnCbnSelchangeParaIndentu();
  afx_msg void OnDeltaposSpinIndent(NMHDR *pNMHDR, LRESULT *pResult);
  afx_msg void OnEnChangeParaWs();
  afx_msg void OnCbnSelchangeParaWsu();
  afx_msg void OnDeltaposSpinWs(NMHDR *pNMHDR, LRESULT *pResult);
  afx_msg void OnBnClickedOk();
  afx_msg void OnBnClickedParaApply();
  afx_msg void OnBnClickedCancel();
  afx_msg void OnBnClickedParaHelp();
  afx_msg void OnBnClickedId();
  afx_msg void OnBnClickedStyle();
  afx_msg void OnBnClickedEvents();
};
