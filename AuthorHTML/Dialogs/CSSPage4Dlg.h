//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    CSSPage4Dlg.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for editing CSS properties
//              Handles the border attributes
//
#pragma once
#include "StdAfx.h"
#include "Css.h"
#include "resource.h"

// CSSPage4Dlg dialog

class CSSPage4Dlg : public CDialog
{
	DECLARE_DYNAMIC(CSSPage4Dlg);

public:
	CSSPage4Dlg(CWnd* pParen,CssStyleSheet* pCss);
	virtual ~CSSPage4Dlg();
  virtual BOOL OnInitDialog();
  void UpdateProperties();
  void SetSelector(string selector);
  // Dialog Data
	enum { IDD = IDD_SS_PAGE4 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  void OnDocumentComplete(LPDISPATCH /*pDisp*/, LPVARIANT /*pURL*/);

	DECLARE_MESSAGE_MAP()
  DECLARE_EVENTSINK_MAP()

private:
  void Redisplay();
  void FillPage();
  void SetAllSides();
  void SplitBorderProperty(CString  property
                          ,CString& width
                          ,CString& style
                          ,CString& color);
  CString StripUnspecified(CString property,bool replace = true);

  CComQIPtr<IWebBrowser2>   m_spBrowser;
  CComQIPtr<IHTMLDocument2> m_doc;
  CssStyleSheet* m_css;
  string         m_selector;

  // Which to use
  bool           m_allsides; 
  bool           m_useTop;
  bool           m_useLeft;
  bool           m_useRight;
  bool           m_useBottom;

  // Attribute per side
  CString        m_borderTStyle;
  CString        m_borderLStyle;
  CString        m_borderRStyle;
  CString        m_borderBStyle;

  CString        m_borderTWidth;
  CString        m_borderLWidth;
  CString        m_borderRWidth;
  CString        m_borderBWidth;

  CString        m_borderTUnits;
  CString        m_borderLUnits;
  CString        m_borderRUnits;
  CString        m_borderBUnits;

  CString        m_borderTColor;
  CString        m_borderLColor;
  CString        m_borderRColor;
  CString        m_borderBColor;

  bool           m_hasAColor;
  bool           m_hasTColor;
  bool           m_hasLColor;
  bool           m_hasRColor;
  bool           m_hasBColor;

  // CONTROLS
  AD_Checkbox  m_buttonAllSides;
  CComboBox    m_comboLSTop;
  CComboBox    m_comboLSLeft;
  CComboBox    m_comboLSRight;
  CComboBox    m_comboLSBottom;

  CSpinButtonCtrl m_spinTop;
  CSpinButtonCtrl m_spinLeft;
  CSpinButtonCtrl m_spinRight;
  CSpinButtonCtrl m_spinBottom;
  CComboBox       m_comboWUTop;
  CComboBox       m_comboWULeft;
  CComboBox       m_comboWURight;
  CComboBox       m_comboWUBottom;
  AD_ColorButton  m_buttonTColor;
  AD_ColorButton  m_buttonLColor;
  AD_ColorButton  m_buttonRColor;
  AD_ColorButton  m_buttonBColor;
  AD_Checkbox     m_buttonHasTC;
  AD_Checkbox     m_buttonHasLC;
  AD_Checkbox     m_buttonHasRC;
  AD_Checkbox     m_buttonHasBC;
  AD_Checkbox     m_buttonTUse;
  AD_Checkbox     m_buttonLUse;
  AD_Checkbox     m_buttonRUse;
  AD_Checkbox     m_buttonBUse;

public:
  afx_msg void OnBnClickedBorderAll();
  afx_msg void OnCbnSelchangeLsTop();
  afx_msg void OnCbnSelchangeLsLeft();
  afx_msg void OnCbnSelchangeLsRight();
  afx_msg void OnCbnSelchangeLsBottom();
  afx_msg void OnEnChangeLwTop();
  afx_msg void OnCbnSelchangeLwuTop();
  afx_msg void OnEnChangeLwLeft();
  afx_msg void OnCbnSelchangeLwuLeft();
  afx_msg void OnEnChangeLwRight();
  afx_msg void OnCbnSelchangeLwuRight();
  afx_msg void OnEnChangeLwBottom();
  afx_msg void OnCbnSelchangeLwuBottom();
  afx_msg void OnBnClickedButLcTop();
  afx_msg void OnBnClickedButLcLeft();
  afx_msg void OnBnClickedButLcRight();
  afx_msg void OnBnClickedButLcBottom();
  afx_msg void OnBnClickedUseTopline();
  afx_msg void OnBnClickedUseLeftline();
  afx_msg void OnBnClickedUseRightline();
  afx_msg void OnBnClickedUseBottomline();
  afx_msg void OnBnClickedHasTc();
  afx_msg void OnBnClickedHasLc();
  afx_msg void OnBnClickedHasRc();
  afx_msg void OnBnClickedHasBc();
};
