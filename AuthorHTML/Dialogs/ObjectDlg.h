//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    ObjectDlg.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for managing the <OBJECT> tag in HTML
//
#pragma once
#include "StdAfx.h"
#include "HtmlObject.h"
#include "resource.h"

// ObjectDlg dialog

class ObjectDlg : public CDialog
{
	DECLARE_DYNAMIC(ObjectDlg)

public:
	ObjectDlg(CWnd* pParent
           ,CString base
           ,HtmlObject* object);
	virtual ~ObjectDlg();
  virtual BOOL OnInitDialog();

// Dialog Data
	enum { IDD = IDD_OBJECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
  void FillPage();
  void UpdateProperties();

  HtmlObject* m_object;
  CString     m_base;
  // Attributes
  CString     m_name;
  CString     m_type;
  CString     m_codeType;
  CString     m_data;
  CString     m_code;
  CString     m_classID;
  CString     m_baseHref;
  CString     m_codeBase;
  CString     m_alt;
  CString     m_altHtml;
  CString     m_standby;
  CString     m_useMap;
  CString     m_align;
  CString     m_border;
  CString     m_width;
  CString     m_height;
  CString     m_hspace;
  CString     m_vspace;
  // Controls
  CComboBox m_comboType;
  CComboBox m_comboCodeType;
  CComboBox m_comboClassID; 
  CComboBox m_comboAlign;
  // Buttons
  AD_Button     m_buttonData;
  AD_Button     m_buttonCode;
  AD_Button     m_buttonBHref;
  AD_Button     m_buttonCodeBase;
  AD_Button     m_buttonAltHtml;
  AD_Button     m_buttonStandby;
  AD_Button     m_buttonUseMap;
  AD_Button     m_buttonParameters;

  // Standard Controls
  AD_Button     m_buttonID;
  AD_Button     m_buttonEvents;
  AD_Button     m_buttonStyle;
  AD_Button     m_buttonOK;
  AD_Button     m_buttonCancel;
  AD_Button     m_buttonHelp;

public:
  afx_msg void OnEnChangeName();
  afx_msg void OnCbnSelchangeObjType();
  afx_msg void OnCbnSelchangeCodetype();
  afx_msg void OnEnChangeData();
  afx_msg void OnEnChangeCode();
  afx_msg void OnCbnSelchangeClassid();
  afx_msg void OnEnChangeBasehref();
  afx_msg void OnEnChangeCodebase();
  afx_msg void OnEnChangeAlt();
  afx_msg void OnEnChangeAlthtml();
  afx_msg void OnEnChangeStandby();
  afx_msg void OnEnChangeUsemap();
  afx_msg void OnCbnSelchangeAlign();
  afx_msg void OnEnChangeBorderwidth();
  afx_msg void OnEnChangeWidth();
  afx_msg void OnEnChangeHeight();
  afx_msg void OnEnChangeObjHspace();
  afx_msg void OnEnChangeObjVspace();
  afx_msg void OnBnClickedButtData();
  afx_msg void OnBnClickedButtCode();
  afx_msg void OnBnClickedButtBhref();
  afx_msg void OnBnClickedButtCodebase();
  afx_msg void OnBnClickedButtAlthtml();
  afx_msg void OnBnClickedButtStandby();
  afx_msg void OnBnClickedButtUsemap();
  afx_msg void OnBnClickedButtParameters();
  afx_msg void OnBnClickedId();
  afx_msg void OnBnClickedStyle();
  afx_msg void OnBnClickedOk();
  afx_msg void OnBnClickedCancel();
  afx_msg void OnBnClickedObjHelp();
  afx_msg void OnBnClickedEvents();
};
