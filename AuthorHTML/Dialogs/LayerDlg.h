//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    LayerDlg.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for managing the <LAYER> tag in HTML
//
#pragma once
#include "StdAfx.h"
#include "HtmlElement.h"
#include "resource.h"
#include "CSS.h"

// LayerDlg dialog

class LayerDlg : public CDialog
{
	DECLARE_DYNAMIC(LayerDlg)

public:
	LayerDlg(CWnd* pParent
           ,CString base
           ,HtmlElement* elem
           ,CString newUnique);
	virtual ~LayerDlg();
  virtual BOOL OnInitDialog();

// Dialog Data
	enum { IDD = IDD_LAYER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
  void FillPage();
  void UpdateProperties();

  HtmlElement*    m_elem;
  CString         m_base;
  CString         m_newUniqueID;
  CssStyleSheet*  m_css;
  // Attributes
  CString         m_id;
  CString         m_visibility;
  CString         m_zindex;
  CString         m_width;
  CString         m_height;
  CString         m_left;
  CString         m_top;
  // Controls
  CComboBox     m_comboVisibility;
  CSpinButtonCtrl m_spinZindex;
  CSpinButtonCtrl m_spinWidth;
  CSpinButtonCtrl m_spinHeight;
  // Common
  AD_Button         m_buttonID;
  AD_Button         m_buttonEvents;
  AD_Button         m_buttonStyle;
  AD_Button         m_buttonOK;
  AD_Button         m_buttonCancel;
  AD_Button         m_buttonHelp;
public:
  afx_msg void OnEnChangeLayerZindex();
  afx_msg void OnCbnSelchangeVisibility();
  afx_msg void OnEnChangeWidth();
  afx_msg void OnEnChangeHeight();
  afx_msg void OnBnClickedId();
  afx_msg void OnBnClickedStyle();
  afx_msg void OnBnClickedOk();
  afx_msg void OnBnClickedCancel();
  afx_msg void OnBnClickedLayerHelp();
  afx_msg void OnEnChangeLayerName();
  afx_msg void OnBnClickedEvents();
};
