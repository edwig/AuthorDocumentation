//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    CSSPage3Dlg.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for editing CSS properties
//              Handles the background attributes
//
#pragma once
#include "StdAfx.h"
#include "Css.h"
#include "resource.h"

// CSSPage3Dlg dialog

class CSSPage3Dlg : public CDialog
{
	DECLARE_DYNAMIC(CSSPage3Dlg)

public:
	CSSPage3Dlg(CWnd* pParent
             ,CssStyleSheet* pCss);
	virtual ~CSSPage3Dlg();
  BOOL OnInitDialog();
  void UpdateProperties();
  void SetSelector(string selector);

// Dialog Data
	enum { IDD = IDD_SS_PAGE3 };

protected:
	void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  void OnDocumentComplete(LPDISPATCH /*pDisp*/, LPVARIANT /*pURL*/);

	DECLARE_MESSAGE_MAP()
  DECLARE_EVENTSINK_MAP()

private:
  void Redisplay();
  void FillPage();

  CComQIPtr<IWebBrowser2>   m_spBrowser;
  CComQIPtr<IHTMLDocument2> m_doc;
  CssStyleSheet* m_css;
  string         m_selector;
  CString        m_backgroundColor;
  CString        m_backgroundImage;
  CString        m_backgroundRepeat;
  CString        m_backgroundAttach;
  CString        m_backgroundPosH;
  CString        m_backgroundPosV;
  bool           m_hasBackColor;
  // CONTROLS
  AD_ColorButton m_buttonBC;
  AD_Checkbox    m_buttonHasBC;
  AD_Button      m_buttonBI;
  CComboBox      m_comboRepeat;
  AD_Checkbox    m_buttonAttach;
  // Postions
  AD_Radiobox    m_buttonVTop;
  AD_Radiobox    m_buttonVCenter;
  AD_Radiobox    m_buttonVBottom;
  AD_Radiobox    m_buttonHLeft;
  AD_Radiobox    m_buttonHMiddle;
  AD_Radiobox    m_buttonHRight;
public:
  afx_msg void OnBnClickedBcChoose();
  afx_msg void OnBnClickedBiChoose();
  afx_msg void OnEnChangeBackgroundimage();
  afx_msg void OnCbnSelchangeBiTile();
  afx_msg void OnBnClickedBiScroll();
  afx_msg void OnBnClickedBiTop();
  afx_msg void OnBnClickedBiCenter();
  afx_msg void OnBnClickedBiBottom();
  afx_msg void OnBnClickedBiLeft();
  afx_msg void OnBnClickedBiMiddle();
  afx_msg void OnBnClickedBiRight();
  afx_msg void OnBnClickedHasColor();
};
