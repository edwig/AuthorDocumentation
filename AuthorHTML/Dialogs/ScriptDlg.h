//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    ScriptDlg.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for managing the <SCRIPT> tag in HTML
//
#pragma once
#include "StdAfx.h"
#include "HTMLScript.h"
#include "Resource.h"
#include "ScintillaCtrl.h"
#include "ScintillaScriptView.h"

// ScriptDlg dialog

class ScriptDlg : public CDialog
{
	DECLARE_DYNAMIC(ScriptDlg)

public:
	ScriptDlg(CWnd*       pParent
           ,HtmlScript* elem
           ,CString     base);
	virtual ~ScriptDlg();
  virtual BOOL OnInitDialog();

  // Dialog Data
	enum { IDD = IDD_SCRIPT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
  void FillPage();
  void UpdateProperties();
  void SetEditorToScript();
  void DefineMarker(int marker, int markerType, COLORREF fore, COLORREF back);
  void SetAStyle(int style, COLORREF fore, COLORREF back=RGB(0xff, 0xff, 0xff), int size=-1, const char *face=0);

  HtmlScript*   m_elem;
  CString       m_base;
  CString       m_type;
  CString       m_language;
  CString       m_src;
  bool          m_defer;
  CString       m_object;
  CString       m_event;
  CString       m_description;
  CString       m_script;
  // Controls
  CComboBox       m_comboType;
  CComboBox       m_comboLang;
  CComboBox       m_comboEvent;
  AD_Checkbox     m_buttonDefer;
  AD_Button       m_buttonChoose;
  CScintillaCtrl  m_scintilla;
  // Common butons
  AD_Button       m_buttonOK;
  AD_Button       m_buttonCancel;
  AD_Button       m_buttonHelp;
public:
  afx_msg void OnCbnSelchangeScripttype();
  afx_msg void OnEnChangeSrc();
  afx_msg void OnEnChangeForhtml();
  afx_msg void OnCbnSelchangeEvent();
  afx_msg void OnBnClickedDefer();
  afx_msg void OnBnClickedChoose();
  afx_msg void OnEnChangeScript();
  afx_msg void OnBnClickedOk();
  afx_msg void OnBnClickedCancel();
  afx_msg void OnBnClickedScriptHelp();
  afx_msg void OnCbnSelchangeLang();
  afx_msg void OnPaint();
};
