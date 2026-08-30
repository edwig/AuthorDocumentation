//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    CSSPage7Dlg.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for editing CSS properties
//              Handles the basic box attributes
//
#pragma once
#include "StdAfx.h"
#include "Css.h"
#include "resource.h"

// CSSPage7Dlg dialog

class CSSPage7Dlg : public CDialog
{
	DECLARE_DYNAMIC(CSSPage7Dlg)

public:
	CSSPage7Dlg(CWnd* pParent
             ,CssStyleSheet* pCss);
	virtual ~CSSPage7Dlg();
  virtual BOOL OnInitDialog();
  void UpdateProperties();
  void SetSelector(string selector);

// Dialog Data
	enum { IDD = IDD_SS_PAGE7 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
  void Redisplay();
  void FillPage();
  CssStyleSheet*  m_css;
  string          m_selector;

  // Attributes
  CString         m_hAlign;
  CString         m_vAlign;
  CString         m_display;
  CString         m_float;
  CString         m_position;
  CString         m_zindex;
  CString         m_visibility;
  CString         m_clear;
  CString         m_overflow;
  CString         m_zoom;

  // Controls
  CComboBox     m_comboHAlign;
  CComboBox     m_comboVAlign;
  CComboBox     m_comboDisplay;
  CComboBox     m_comboFloat;
  CComboBox     m_comboPosition;
  CComboBox     m_comboZIndex;
  CSpinButtonCtrl m_spinZIndex;
  CComboBox     m_comboVisibility;
  CComboBox     m_comboClear;
  CComboBox     m_comboOverflow;
  CComboBox     m_comboZoom;
  CSpinButtonCtrl m_spinZoom;

public:
  afx_msg void OnCbnSelchangeDisplay();
  afx_msg void OnCbnSelchangeFloat();
  afx_msg void OnCbnSelchangePosition();
  afx_msg void OnCbnSelchangeZindex();
  afx_msg void OnDeltaposSpinZ(NMHDR *pNMHDR, LRESULT *pResult);
  afx_msg void OnCbnSelchangeVisibility();
  afx_msg void OnCbnSelchangeClear();
  afx_msg void OnCbnSelchangeOverflow();
  afx_msg void OnCbnSelchangeZoom();
  afx_msg void OnCbnSelchangeHorzAlign();
  afx_msg void OnCbnSelchangeVertAlign();
};
