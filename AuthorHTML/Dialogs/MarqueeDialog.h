//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    MarqueeDialog.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for managing the <MARQUEE> tag in HTML
//
#pragma once
#include "StdAfx.h"
#include "resource.h"

// MarqueeDialog dialog

class MarqueeDialog : public CDialog
{
	DECLARE_DYNAMIC(MarqueeDialog)

public:
	MarqueeDialog(CWnd* pParent
               ,CComPtr<IHTMLMarqueeElement> pMarquee
               ,CString base);
	virtual ~MarqueeDialog();
  virtual BOOL OnInitDialog();

// Dialog Data
	enum { IDD = IDD_MARQUEE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  void UpdateDocument();

  // HTML ELement
  CComPtr<IHTMLMarqueeElement> m_marquee;
  CString m_base;
  // Info
  CString m_direction;
  long    m_speedAmount;
  long    m_speedDelay;
  CString m_behaviour;
  bool    m_continuously;
  long    m_repeatTimes;
  bool    m_maintainAspect;
  bool    m_usePreferredWidth;
  CString m_preferredWidth;
  bool    m_usePreferredHeight;
  CString m_preferredHeight;
  // Controls
  AD_Radiobox         m_buttonLeft;
  AD_Radiobox         m_buttonRight;
  AD_Radiobox         m_buttonUp;
  AD_Radiobox         m_buttonDown;
  CSpinButtonCtrl     m_spinAmount;
  CSpinButtonCtrl     m_spinDelay;
  AD_Radiobox         m_buttonScroll;
  AD_Radiobox         m_buttonSlide;
  AD_Radiobox         m_buttonAlternate;
  AD_Checkbox         m_buttonContinuously;
  CSpinButtonCtrl     m_spinTimes;
  AD_Checkbox         m_buttonAspect;
  AD_Checkbox         m_buttonUseWidth;
  AD_Checkbox         m_buttonUseHeight;
  CSpinButtonCtrl     m_spinWidth;
  CSpinButtonCtrl     m_spinHeight;
  AD_Button         m_buttonID;
  AD_Button         m_buttonEvents;
  AD_Button         m_buttonStyle;
  AD_Button         m_buttonOK;
  AD_Button         m_buttonCancel;
  AD_Button         m_buttonApply;
  AD_Button         m_buttonHelp;

	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnBnClickedRadioLr();
  afx_msg void OnBnClickedRadioRl();
  afx_msg void OnBnClickedRadioUp();
  afx_msg void OnBnClickedRadioDown();
  afx_msg void OnBnClickedRadioScroll();
  afx_msg void OnBnClickedRadioSlide();
  afx_msg void OnBnClickedRadioAlternate();
  afx_msg void OnBnClickedRepeatCont();
  afx_msg void OnBnClickedMarqUsew();
  afx_msg void OnBnClickedMarqUseh();
  afx_msg void OnBnClickedOk();
  afx_msg void OnBnClickedMarqueeApply();
  afx_msg void OnBnClickedId();
  afx_msg void OnBnClickedEvents();
  afx_msg void OnBnClickedStyle();
};
