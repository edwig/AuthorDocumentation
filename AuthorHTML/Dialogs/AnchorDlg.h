//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    AnchorDlg.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog to edit the <A> anchor with bookmark, so we can link to it
//
#pragma once
#include "StdAfx.h"
#include "addresscombo.h"
#include "HTMLAnchor.h"
#include "resource.h"


/////////////////////////////////////////////////////////////////////////////
// AnchorDlg dialog

class AnchorDlg : public CDialog
{
  // Construction
public:
  AnchorDlg(CWnd*       pParent
           ,CString     base
           ,CString     basePage
           ,HtmlAnchor* elem
           ,int         unique);
  ~AnchorDlg();
  virtual BOOL OnInitDialog();
          bool GetDoPopup();
  // Dialog Data
  enum { IDD = IDD_HYPERLINK };

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
  void UpdateProperties();

  void OnGo();
  void OnCloseup();
  void OnDocumentComplete(LPDISPATCH pDisp, LPVARIANT pURL);
  int  GetRelationIndex(CString& rel);
  void ScrollIntoView(CString bookmark);

  // Attributes
  CComQIPtr<IWebBrowser2> m_spBrowser;
  HtmlAnchor* m_elem;
  CString m_base;
  CString m_basePage;
  CString m_href;
  CString m_target;
  CString m_title;
  CString m_rel;
  CString m_rev;
  CString m_bookmark;
  int     m_unique;   // Unique id voor "ID" (popup)
  bool    m_doPopup;
  bool    m_doBSSCpopup;
  bool    m_doADHpopup;

  CAddressCombo   m_AddrCombo;
  CComboBox       m_targetCombo;
  CComboBox       m_relCombo;
  CComboBox       m_revCombo;
  AD_Radiobox     m_buttonFrame;
  AD_Radiobox     m_buttonPopUp;
  AD_Button       m_buttonOpen;
  AD_Button       m_buttonGo;
  CComboBox       m_comboBM;
  // General Buttons
  AD_Button         m_buttonOK;
  AD_Button         m_buttonCancel;
  AD_Button         m_buttonHelp;
  AD_Button         m_buttonID;
  AD_Button         m_buttonEvents;
  AD_Button         m_buttonStyle;

public:
  afx_msg void OnBnClickedButtonOpen();
  afx_msg void OnEnChangeEditHypinfo();
  afx_msg void OnCbnSelchangeFrameTarget();
  afx_msg void OnBnClickedRadioFrame();
  afx_msg void OnBnClickedRadioPopup();
  afx_msg void OnCbnSelChangeRel();
  afx_msg void OnCbnSelChangeRev();
  afx_msg void OnBnClickedId();
  afx_msg void OnBnClickedStyle();
  afx_msg void OnBnClickedHelpUrl();
  afx_msg void OnBnClickedEvents();
  afx_msg void OnCbnSelchangeBookmarks();
};

inline bool 
AnchorDlg::GetDoPopup()
{
  return m_doPopup;
}
