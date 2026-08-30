//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    CSSPage5Dlg.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for editing CSS properties
//              Handles the margin, padding, and offset attributes
//
#pragma once
#include "StdAfx.h"
#include "css.h"
#include "resource.h"

// CSSPage5Dlg dialog

class CSSPage5Dlg : public CDialog
{
	DECLARE_DYNAMIC(CSSPage5Dlg)

public:
	CSSPage5Dlg(CWnd* pParent,CssStyleSheet* pCss);
	virtual ~CSSPage5Dlg();
  virtual BOOL OnInitDialog();
  void UpdateProperties();
  void SetSelector(string selector);

// Dialog Data
	enum { IDD = IDD_SS_PAGE5 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  void OnDocumentComplete(LPDISPATCH /*pDisp*/, LPVARIANT /*pURL*/);

	DECLARE_MESSAGE_MAP()
  DECLARE_EVENTSINK_MAP()

private:
  void Redisplay();
  void FillPage();
  void CSSSizeSplit(CString  property
                   ,CString& top
                   ,CString& left
                   ,CString& right
                   ,CString& bottom);
  bool CSSSizeMerge(CString& property
                   ,CString& top
                   ,CString& left
                   ,CString& right
                   ,CString& bottom);

  CComQIPtr<IWebBrowser2>   m_spBrowser;
  CComQIPtr<IHTMLDocument2> m_doc;
  CssStyleSheet*  m_css;
  string          m_selector;

  // PROPERTIES
  // Margins
  CString         m_marginTop;
  CString         m_marginLeft;
  CString         m_marginRight;
  CString         m_marginBottom;
  CString         m_marginTUnit;
  CString         m_marginLUnit;
  CString         m_marginRUnit;
  CString         m_marginBUnit;
  // Paddings
  CString         m_paddingTop;
  CString         m_paddingLeft;
  CString         m_paddingRight;
  CString         m_paddingBottom;
  CString         m_paddingTUnit;
  CString         m_paddingLUnit;
  CString         m_paddingRUnit;
  CString         m_paddingBUnit;
  // Offsets
  CString         m_offsetTop;
  CString         m_offsetLeft;
  CString         m_offsetRight;
  CString         m_offsetBottom;
  CString         m_offsetTUnit;
  CString         m_offsetLUnit;
  CString         m_offsetRUnit;
  CString         m_offsetBUnit;
  // Width/Height
  CString         m_width;
  CString         m_height;
  CString         m_widthUnit;
  CString         m_heightUnit;

  // CONTROLS
  // Margins
  CSpinButtonCtrl m_spinMT;
  CSpinButtonCtrl m_spinML;
  CSpinButtonCtrl m_spinMR;
  CSpinButtonCtrl m_spinMB;
  CComboBox     m_comboMT;
  CComboBox     m_comboML;
  CComboBox     m_comboMR;
  CComboBox     m_comboMB;
  // Paddings
  CSpinButtonCtrl m_spinPT;
  CSpinButtonCtrl m_spinPL;
  CSpinButtonCtrl m_spinPR;
  CSpinButtonCtrl m_spinPB;
  CComboBox     m_comboPT;
  CComboBox     m_comboPL;
  CComboBox     m_comboPR;
  CComboBox     m_comboPB;
  // Offsets
  CSpinButtonCtrl m_spinOT;
  CSpinButtonCtrl m_spinOL;
  CSpinButtonCtrl m_spinOR;
  CSpinButtonCtrl m_spinOB;
  CComboBox     m_comboOT;
  CComboBox     m_comboOL;
  CComboBox     m_comboOR;
  CComboBox     m_comboOB;
  // Width/Height
  CSpinButtonCtrl m_spinW;
  CSpinButtonCtrl m_spinH;
  CComboBox     m_comboW;
  CComboBox     m_comboH;

public:
  // Margins
  afx_msg void OnEnChangeMarginT();
  afx_msg void OnEnChangeMarginL();
  afx_msg void OnEnChangeMarginR();
  afx_msg void OnEnChangeMarginB();
  afx_msg void OnCbnSelchangeMuT();
  afx_msg void OnCbnSelchangeMuL();
  afx_msg void OnCbnSelchangeMuR();
  afx_msg void OnCbnSelchangeMuB();
  // Paddings
  afx_msg void OnEnChangePadT();
  afx_msg void OnEnChangePadL();
  afx_msg void OnEnChangePadR();
  afx_msg void OnEnChangePadB();
  afx_msg void OnCbnSelchangePuT();
  afx_msg void OnCbnSelchangePuL();
  afx_msg void OnCbnSelchangePuR();
  afx_msg void OnCbnSelchangePuB();
  // Offsets
  afx_msg void OnEnChangeOffsetT();
  afx_msg void OnEnChangeOffsetL();
  afx_msg void OnEnChangeOffsetR();
  afx_msg void OnEnChangeOffsetB();
  afx_msg void OnCbnSelchangeOuT();
  afx_msg void OnCbnSelchangeOuL();
  afx_msg void OnCbnSelchangeOuR();
  afx_msg void OnCbnSelchangeOuB();
  // Width/Height
  afx_msg void OnEnChangeWidth();
  afx_msg void OnEnChangeHeight();
  afx_msg void OnCbnSelchangeWidthUnit();
  afx_msg void OnCbnSelchangeHeightUnit();
};
