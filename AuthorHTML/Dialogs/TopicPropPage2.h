//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    TopicPropPage2.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for all topic properties page 2 of 5
//
#pragma once
#include "StdAfx.h"
#include "HtmlBody.h"
#include "resource.h"

// TopicPropPage2Dlg dialog

class TopicPropPage2Dlg : public CDialog
{
	DECLARE_DYNAMIC(TopicPropPage2Dlg)

public:
	TopicPropPage2Dlg(CWnd* pParent
                   ,HtmlBody body
                   ,CString  base);
	virtual ~TopicPropPage2Dlg();
  virtual  BOOL OnInitDialog();
           void UpdateProperties();

// Dialog Data
	enum { IDD = IDD_TOPIC_PROPPAGE2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  void FillPage();

  HtmlBody  m_body;
  CString   m_base;
  CString   m_backgroundImage;
  bool      m_backgroundFixed;
  CString   m_marginTop;    // Bottom margin alsoo set
  CString   m_marginLeft;   // Right  margin alsoo set
  CString   m_textColor;
  CString   m_backgroundColor;
  CString   m_linkColor;
  CString   m_linkVisitedColor;
  CString   m_linkActiveColor;
  CString   m_scroll;
  bool      m_nowrap;

  bool      m_doTextColor;
  bool      m_doBackColor;
  bool      m_doLinkColor;
  bool      m_doVisitColor;
  bool      m_doActivColor;
  bool      m_doTopBottom;
  bool      m_doRightLeft;
  bool      m_doScroll;
  // CONTROLS
  CSpinButtonCtrl   m_spinTop;
  CSpinButtonCtrl   m_spinLeft;
  CMFCButton        m_buttonImage;
  AD_Checkbox       m_buttonFixed;
  AD_Checkbox       m_buttonNoWrap;
  CComboBox         m_comboScroll;

  AD_ColorButton    m_buttonColor;
  AD_ColorButton    m_buttonBackColor;
  AD_ColorButton    m_buttonHyperlink;
  AD_ColorButton    m_buttonVisited;
  AD_ColorButton    m_buttonActive;

  AD_Checkbox       m_buttonTC;
  AD_Checkbox       m_buttonBC;
  AD_Checkbox       m_buttonLC;
  AD_Checkbox       m_buttonVC;
  AD_Checkbox       m_buttonAC;
  AD_Checkbox       m_buttonTB;
  AD_Checkbox       m_buttonLR;
  AD_Checkbox       m_buttonSC;

  AD_Button         m_buttonID;
  AD_Button         m_buttonEvents;
  AD_Button         m_buttonStyle;

  CBitmap           m_findAlpha;

	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnEnChangeBackground();
  afx_msg void OnBnClickedButtonImage();
  afx_msg void OnBnClikcedBodyTextColor();
  afx_msg void OnBnClickedBodyColor();
  afx_msg void OnBnClickedBodyFixed();
  afx_msg void OnBnClickedBodyHyper();
  afx_msg void OnBnClickedBodyVisited();
  afx_msg void OnBnClickedBodyActive();
  afx_msg void OnBnClickedId();
  afx_msg void OnBnClickedBodyStyle();
  afx_msg void OnEnChangeBodyTopbottom();
  afx_msg void OnEnChangeBodyLeftright();
  afx_msg void OnBnClickedEvents();
  afx_msg void OnBnClickedDoTc();
  afx_msg void OnBnClickedDoBc();
  afx_msg void OnBnClickedDoLc();
  afx_msg void OnBnClickedDoVc();
  afx_msg void OnBnClickedDoAc();
  afx_msg void OnBnClickedDoTB();
  afx_msg void OnBnClickedDoLR();
  afx_msg void OnBnClickedDoScroll();
  afx_msg void OnCbnSelchangeScroll();
  afx_msg void OnBnClickedNowrap();
};
