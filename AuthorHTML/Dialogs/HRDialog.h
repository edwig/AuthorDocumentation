//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    HRDialog.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for managing the <HR> tag in HTML,
//
#pragma once
#include "StdAfx.h"
#include "HTMLHR.h"
#include "resource.h"

// HRDialog dialog

class HRDialog : public CDialog
{
	DECLARE_DYNAMIC(HRDialog)

public:
	HRDialog(CWnd* pParent
          ,CString p_base
          ,HtmlHR pHr);   // standard constructor
  BOOL OnInitDialog();
  void UpdateProperties();

// Dialog Data
	enum { IDD = IDD_HR_PROPERTIES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
  void ReadProperties();

  CString m_base;
  HtmlHR  m_hr;
  UINT    m_size;
  UINT    m_width;
  BOOL    m_shade;
  CString m_align;
  bool    m_widthInPix;
  CString m_color;
  bool    m_canApply;
  bool    m_hasColor;

  CSpinButtonCtrl m_sizeCtrl;
  CSpinButtonCtrl m_widthCtrl;
  CComboBox       m_formatCtrl;
  CComboBox       m_alignCtrl;
  AD_Radiobox       m_noShade;
  AD_Radiobox       m_Shade;
  AD_ColorButton    m_buttonColor;
  AD_Checkbox       m_buttonHasColor;
  AD_Button         m_buttonID;
  AD_Button         m_buttonEvents;
  AD_Button         m_buttonStyle;
  AD_Button         m_buttonOK;
  AD_Button         m_buttonCancel;
  AD_Button         m_buttonApply;
  AD_Button         m_buttonHelp;
public:
  afx_msg void OnBnClickedApply();
  afx_msg void OnBnClickedOk();
  afx_msg void OnBnClickedHrnoshade();
  afx_msg void OnBnClickedHrshade();
  afx_msg void OnBnClickedHrcolor();
  afx_msg void OnCbnSelchangeFormatwidth();
  afx_msg void OnCbnSelchangeHralign();
  afx_msg void OnBnClickedHrId();
  afx_msg void OnBnClickedStyle();
  afx_msg void OnBnClickedCancel();
  afx_msg void OnBnClickedHelp();
  afx_msg void OnEnChangeHrsize();
  afx_msg void OnEnChangeHrwidth();
  afx_msg void OnBnClickedEvents();
  afx_msg void OnBnClickedCheck1();
};
