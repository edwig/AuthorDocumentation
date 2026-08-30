//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    ALinkCommandDlg.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog to edit the associative link command <A> in the HTML file
//
#pragma once
#include "resource.h"
#include "HtmlObject.h"
#include "GridCtrl.h"

// Button types for the ALINK command
// Keep button types in this order for combo and logic
#define BUTTON_NOBUTTON   0
#define BUTTON_TEXTBUTTON 1
#define BUTTON_CHICLET    2
#define BUTTON_BITMAP     3
#define BUTTON_ICON       4
// Shortcut doesn't work: bug in IE6-8
//#define BUTTON_SHORTCUT   5

// Menu types
// Keep menu typs in this order
#define MENU_DEFAULT  0
#define MENU_MENU     1
#define MENU_DIALOG   2

// ALinkCommandDlg dialog

class ALinkCommandDlg : public CDialog
{
	DECLARE_DYNAMIC(ALinkCommandDlg)

public:
	ALinkCommandDlg(CWnd* p_parent,CString& p_base,HtmlObject* p_object);
	virtual ~ALinkCommandDlg();
  virtual BOOL OnInitDialog();

// Dialog Data
	enum { IDD = IDD_ALINK_COMMAND };

protected:
	void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  void FillPage();
  void UpdateObject();
  void EnsureCHM();
  bool CheckCommand();

	DECLARE_MESSAGE_MAP()

  // The object state
  HtmlObject* m_object;
  bool        m_changed;
  CString     m_base;
  // in the object
  CString     m_type;
  CString     m_text;
  CString     m_chmFilename;
  CString     m_defaultTopic;
  int         m_button;     // button in m_comboButton
  CString     m_iconbitmap;
  int         m_menu;       // button in m_comboMenu
  CString     m_frame;
  CString     m_width;
  CString     m_height;
  CString     m_saveEdit;
  int         m_maxItemInObject;

  // Controls
  CComboBox       m_comboType;
  AD_Button       m_buttonCHM;
  AD_Button       m_buttonDefTopic;
  CComboBox       m_comboButton;
  AD_Button       m_buttonBMPSearch;
  CComboBox       m_comboMenu;
  CSpinButtonCtrl m_spinHeight;
  CSpinButtonCtrl m_spinWidth;
  CGridCtrl       m_list;
  AD_Button       m_buttonID;
  AD_Button       m_buttonStyle;
  AD_Button       m_buttonOK;
  AD_Button       m_buttonApply;
  AD_Button       m_buttonCancel;
  AD_Button       m_buttonHelp;
public:
  afx_msg void OnEnChangeText();
  afx_msg void OnEnChangeChmfile();
  afx_msg void OnBnClickedChmSearch();
  afx_msg void OnEnChangeDefault();
  afx_msg void OnBnClickedDefSearch();
  afx_msg void OnCbnSelchangeButtontype();
  afx_msg void OnEnChangeIconbitmap();
  afx_msg void OnBnClickedBmpSearch();
  afx_msg void OnCbnSelchangeMenu();
  afx_msg void OnEnChangeFrametitle();
  afx_msg void OnLvnItemchangedList(NMHDR *pNMHDR, LRESULT *pResult);
  afx_msg void OnBeginEdit         (NMHDR *pNMHDR, LRESULT *pResult);
  afx_msg void OnEndInPlaceEdit    (NMHDR *pNMHDR, LRESULT *pResult);
  afx_msg void OnBnClickedKeyword();
  afx_msg void OnBnClickedDelete();
  afx_msg void OnBnClickedId();
  afx_msg void OnBnClickedStyle();
  afx_msg void OnBnClickedOk();
  afx_msg void OnBnClickedButapply();
  afx_msg void OnBnClickedCancel();
  afx_msg void OnBnClickedHelpacommand();
  afx_msg void OnEnChangeHeight();
  afx_msg void OnDeltaposSpinh(NMHDR *pNMHDR, LRESULT *pResult);
  afx_msg void OnEnChangeWidth();
  afx_msg void OnDeltaposSpinw(NMHDR *pNMHDR, LRESULT *pResult);
  afx_msg void OnCbnSelchangeIndextype();
};
