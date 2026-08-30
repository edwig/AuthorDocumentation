//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    ImageDlg.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for managing the <IMG> tag in HTML,
//
#pragma once
#include "StdAfx.h"
#include "addresscombo.h"
#include "HTMLImg.h"
#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// ImageDlg dialog

class ImageDlg : public CDialog
{
  // Construction
public:
  ImageDlg(CWnd*    pParent
                 ,HtmlImg* img
                 ,CString  base);
  virtual BOOL OnInitDialog();

  enum { IDD = IDD_IMAGE};

protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  DECLARE_MESSAGE_MAP()
  DECLARE_EVENTSINK_MAP()

  // Implementation
private:
  void OnCloseup();
  void OnDocumentComplete(LPDISPATCH pDisp, LPVARIANT pURL);
  void FillPage();
  void UpdateProperties();
  void ReCalculateRatio();

  CComQIPtr<IWebBrowser2>   m_spBrowser;
  CComQIPtr<IHTMLDocument2> m_doc;
  HtmlImg*                  m_img;

  // Attributes
  CString         m_src;      // Normal image source
  CString         m_dynsrc;   // Dynamic source (video and such)
  CString         m_lowsrc;   // Lowresolution source (whilst loading)
  CString         m_base;
  CString         m_align;
  CString         m_title;
  CString         m_width;        // Size as in image file
  CString         m_height;
  CString         m_docWidth;     // Size as set on the document
  CString         m_docHeight;
  CString         m_hpad;
  CString         m_vpad;
  CString         m_border;
  CString         m_wunits;
  CString         m_hunits;
  CString         m_hpunits;
  CString         m_vpunits;
  CString         m_bunits;
  CString         m_start;
  CString         m_loop;
  bool            m_controls;

  double          m_ratio;
  bool            m_initDone;

  // Controls;
  CComboBox     m_comboSrc;
  CComboBox     m_comboDynSrc;
  CComboBox     m_comboLowSrc;
  AD_Button     m_buttonSrc;
  AD_Button     m_buttonDynSrc;
  AD_Button     m_buttonLowSrc;

  AD_Checkbox     m_buttonRatio;
  CSpinButtonCtrl m_spinWidth;
  CSpinButtonCtrl m_spinHeight;
  CSpinButtonCtrl m_spinHpad;
  CSpinButtonCtrl m_spinVpad;
  CSpinButtonCtrl m_spinBorder;
  CComboBox       m_comboAlign;
  CComboBox       m_comboStart;
  CComboBox       m_comboLoop;
  // Common controls
  AD_Button         m_buttonID;
  AD_Button         m_buttonEvents;
  AD_Button         m_buttonStyle;
  AD_Button         m_buttonReset;
  AD_Button         m_buttonOK;
  AD_Button         m_buttonCancel;
  AD_Button         m_buttonHelp;

public:
  afx_msg void OnBnClickedButtonOpen();
  afx_msg void OnEnChangeEditHypinfo();
  afx_msg void OnBnClickedRatio();
  afx_msg void OnEnChangeImgWidth();
  afx_msg void OnEnChangeImgHeight();
  afx_msg void OnEnChangeImgHpad();
  afx_msg void OnEnChangeImgVpad();
  afx_msg void OnBnClickedResetImage();
  afx_msg void OnCbnSelchangeAlignment();
  afx_msg void OnBnClickedId();
  afx_msg void OnBnClickedStyle();
  afx_msg void OnBnClickedOk();
  afx_msg void OnBnClickedCancel();
  afx_msg void OnBnClickedHelpImage();
  afx_msg void OnCbnSelchangeImgVideo();
  afx_msg void OnBnClickedButtonVideo();
  afx_msg void OnCbnSelchangeImgLowres();
  afx_msg void OnBnClickedButtonLowres();
  afx_msg void OnEnChangeImgBorderwidth();
  afx_msg void OnCbnSelchangeImgStartvideo();
  afx_msg void OnCbnSelchangeImgLoop();
  afx_msg void OnBnClickedEvents();
};

