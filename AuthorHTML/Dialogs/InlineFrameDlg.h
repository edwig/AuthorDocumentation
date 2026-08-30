//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    InlineFrameDlg.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for managing the <IFRAME> tag in HTML
//
#pragma once
#include "StdAfx.h"
#include "HTMLIFrame.h"
#include "resource.h"

// InlineFrameDlg dialog

class InlineFrameDlg : public CDialog
{
	DECLARE_DYNAMIC(InlineFrameDlg)

public:
	InlineFrameDlg(CWnd*       pParent
                ,CString     p_base
                ,HtmlIFrame* frame);
	virtual ~InlineFrameDlg();
  virtual BOOL OnInitDialog();

// Dialog Data
	enum { IDD = IDD_IFRAME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
  void FillPage();
  void UpdateProperties();

  // Attributes
  HtmlIFrame* m_iframe;
  CString     m_base;
  CString     m_name;
  CString     m_src;
  CString     m_align;
  CString     m_width;
  CString     m_height;
  CString     m_hspace;
  CString     m_vspace;
  CString     m_marginw;
  CString     m_marginh;
  CString     m_frameBorder;
  CString     m_noresize;
  CString     m_scrolling;

  CString     m_wunits;
  CString     m_hunits;
  CString     m_hsunits;
  CString     m_vsunits;
  CString     m_mwunits;
  CString     m_mhunits;

  // Controls
  AD_Button         m_buttonChoose;
  CComboBox     m_comboWUnits;
  CComboBox     m_comboHUnits;
  CSpinButtonCtrl m_spinWidth;
  CSpinButtonCtrl m_spinHeight;
  AD_Button         m_buttonNoResize;
  CComboBox     m_comboHSU;
  CComboBox     m_comboVSU;
  CSpinButtonCtrl m_spinHM;
  CSpinButtonCtrl m_spinVM;
  CComboBox     m_comboHPU;
  CComboBox     m_comboVPU;
  CSpinButtonCtrl m_spinHP;
  CSpinButtonCtrl m_spinVP;
  CComboBox     m_comboAlign;
  CComboBox     m_comboScroll;
  AD_Checkbox   m_buttonBorder;
  // Standard buttons
  AD_Button     m_buttonID;
  AD_Button     m_buttonEvents;
  AD_Button     m_buttonStyle;
  AD_Button     m_buttonOK;
  AD_Button     m_buttonApply;
  AD_Button     m_buttonCancel;
  AD_Button     m_buttonHelp;
public:
  afx_msg void OnEnChangeIfName();
  afx_msg void OnEnChangeIfSrc();
  afx_msg void OnBnClickedIfChoose();
  afx_msg void OnBnClickedOk();
  afx_msg void OnBnClickedIfApply();
  afx_msg void OnBnClickedCancel();
  afx_msg void OnBnClickedIfHelp();
  afx_msg void OnEnChangeIfWidth();
  afx_msg void OnEnChangeIfHeight();
  afx_msg void OnCbnSelchangeComboWu();
  afx_msg void OnCbnSelchangeComboHu();
  afx_msg void OnBnClickedIfNoresize();
  afx_msg void OnEnChangeIfHmargin();
  afx_msg void OnEnChangeIfVmargin();
  afx_msg void OnCbnSelchangeComboHmu();
  afx_msg void OnCbnSelchangeComboVmu();
  afx_msg void OnEnChangeIfHpad();
  afx_msg void OnEnChangeIfVpad();
  afx_msg void OnCbnSelchangeComboHpu();
  afx_msg void OnCbnSelchangeCombVpu();
  afx_msg void OnCbnSelchangeIfAlign();
  afx_msg void OnCbnSelchangeIfScroll();
  afx_msg void OnBnClickedIfBorder();
  afx_msg void OnBnClickedId();
  afx_msg void OnBnClickedStyle();
  afx_msg void OnBnClickedEvents();
};
