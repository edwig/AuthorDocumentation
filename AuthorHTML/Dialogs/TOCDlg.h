//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    TOCDlg.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for properties of a single TOC entry.
//
#pragma once
#include "StdAfx.h"
#include "addresscombo.h"
#include "TOCEntry.h"
#include "resource.h"

class CIconImage : public CStatic
{
public:
  CIconImage();
  ~CIconImage();
  void OnPaint();
  void SetTOCIcon(HICON icon) { m_hicon = icon; }
  DECLARE_MESSAGE_MAP()
private:
  HICON m_hicon;
};

/////////////////////////////////////////////////////////////////////////////
// TOCDlg dialog

class TOCDlg : public CDialog
{
  // Construction
public:
  TOCDlg(CWnd*       pParent
        ,CString     base
        ,TOCEntry*   entry);
  ~TOCDlg();
  virtual BOOL OnInitDialog();
  // Dialog Data
  enum { IDD = IDD_TOC_PROPERTIES };

  // Overrides
  // ClassWizard generated virtual function overrides
protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  virtual void OnOK();

  DECLARE_MESSAGE_MAP()
  DECLARE_EVENTSINK_MAP()

  // Implementation

private:
  void FillPage();
  bool UpdateProperties();
  void OnPaintIcon();

  void OnGo();
  void OnCloseup();
  void OnDocumentComplete(LPDISPATCH pDisp, LPVARIANT pURL);
  int  GetRelationIndex(CString& rel);
  void ScrollIntoView(CString bookmark);
  bool CheckDocument(CString& href);

  // Attributes
  CComQIPtr<IWebBrowser2> m_spBrowser;
  CString m_base;
  TOCEntry* m_entry;
  CString m_href;
  CString m_comment;
  CString m_window;
  CString m_target;
  CString m_title;
  CString m_bookmark;
  CString m_image;

  CAddressCombo   m_AddrCombo;
  CComboBox       m_comboWindow;
  CComboBox       m_comboFrame; // m_targetCombo;
  CComboBox       m_comboImage;
  CImageList      m_imageList;
  AD_Radiobox     m_buttonFrame;
  AD_Radiobox     m_buttonWindow;
  AD_Button       m_buttonOpen;
  AD_Button       m_buttonGo;
  AD_Button       m_buttonNew;
  AD_Button       m_buttonFind;
  CComboBox       m_comboBM;
  CIconImage      m_iconImage;
  // General Buttons
  AD_Button         m_buttonOK;
  AD_Button         m_buttonCancel;
  AD_Button         m_buttonHelp;

public:
  afx_msg void OnEnChangeTitle();
  afx_msg void OnBnClickedButtonOpen();
  afx_msg void OnEnChangeEditHypinfo();
  afx_msg void OnCbnSelchangeWindowTarget();
  afx_msg void OnCbnSelchangeFrameTarget();
  afx_msg void OnCbnSelchangeImagenumber();
  afx_msg void OnBnClickedRadioFrame();
  afx_msg void OnCbnSelchangeBookmarks();
  afx_msg void OnBnClickedHelpTOC();
  afx_msg void OnBnClickedButtonNewpage();
  afx_msg void OnBnClickedButtonFindpage();
};

