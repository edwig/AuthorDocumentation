//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    FontDlg.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for managing the <FONT> tag in HTML, 
//              to set the font size, color and face
//
#pragma once
#include "HtmlElement.h"
#include "resource.h"

// FontDlg dialog

class FontDlg : public CDialog
{
	DECLARE_DYNAMIC(FontDlg)

public:
	FontDlg(CWnd* pParent
             ,CString base
             ,HtmlElement* elem);
	virtual ~FontDlg();
  virtual BOOL OnInitDialog();

// Dialog Data
	enum { IDD = IDD_FONT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
  void  FillPage();
  void  UpdateProperties();

  HtmlElement* m_elem;
  CString      m_base;
  CString      m_size;
  CString      m_color;
  CString      m_face;
  bool         m_hasColor;
  // Controls;
  CComboBox       m_comboSize;
  CComboBox       m_comboFace;
  AD_ColorButton  m_buttonColor;
  AD_Checkbox     m_buttonHasColor;
  AD_Button       m_buttonID;
  AD_Button       m_buttonEvents;
  AD_Button       m_buttonStyle;
  AD_Button       m_buttonOK;
  AD_Button       m_buttonCancel;
  AD_Button       m_buttonHelp;
public:
  afx_msg void OnCbnSelchangeSize();
  afx_msg void OnBnClickedColorButton();
  afx_msg void OnCbnSelchangeFace();
  afx_msg void OnBnClickedId();
  afx_msg void OnBnClickedEvents();
  afx_msg void OnBnClickedStyle();
  afx_msg void OnBnClickedOk();
  afx_msg void OnBnClickedCancel();
  afx_msg void OnBnClickedHelpFontsize();
  afx_msg void OnBnClickedCheck1();
};
