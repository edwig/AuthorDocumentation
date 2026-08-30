//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    AreaDlg.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog to edit the <AREA> tag to be able to make
//              links in an image map. The <AREA> tag is a child of the <MAP> tag
//
#pragma once
#include "HTMLArea.h"
#include "resource.h"
#include "AddressCombo.h"

// AreaDlg dialog

class AreaDlg : public CDialog
{
	DECLARE_DYNAMIC(AreaDlg)

public:
	AreaDlg(CWnd* pParent
         ,HtmlArea* area
         ,CString   base);
	virtual ~AreaDlg();
  virtual BOOL OnInitDialog();
  bool GetDoPopup();

// Dialog Data
	enum { IDD = IDD_AREA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
  DECLARE_EVENTSINK_MAP()

private:
  void FillPage();
  void UpdateProperties();
  void OnDocumentComplete(LPDISPATCH pDisp, LPVARIANT pURL);
  void ScrollIntoView(CString bookmark);

  bool      m_initDone;
  CComQIPtr<IWebBrowser2> m_spBrowser;
  HtmlArea* m_area;
  CString   m_base;
  // Properties
  CString   m_href;
  CString   m_bookmark;
  CString   m_shape;
  CString   m_left;   // x1
  CString   m_top;    // y1 
  CString   m_right;  // x2 radius
  CString   m_bottom; // y2 
  CString   m_polygon;
  bool      m_nohref;
  bool      m_doPopup;
  bool      m_doBSSCpopup;
  bool      m_doADHpopup;
  CString   m_title;
  CString   m_id;
  // Controls
  CAddressCombo   m_AddrCombo;
  AD_Button       m_buttonOpen;
  AD_Button       m_buttonGO;
  AD_Checkbox     m_buttonPopup;
  AD_Checkbox     m_buttonNoHref;
  CComboBox       m_comboShape;
  CComboBox       m_comboBM;

  CSpinButtonCtrl m_spin1;
  CSpinButtonCtrl m_spin2;
  CSpinButtonCtrl m_spin3;
  CSpinButtonCtrl m_spin4;

  // Common controls
  AD_Button   m_buttonID;
  AD_Button   m_buttonEvents;
  AD_Button   m_buttonOK;
  AD_Button   m_buttonCancel;
  AD_Button   m_buttonHelp;
public:
  afx_msg void OnCloseup();
  afx_msg void OnButtonOpen();
  afx_msg void OnButtonGo();
  afx_msg void OnBnClickedNohref();
  afx_msg void OnEnChangeTitle();
  afx_msg void OnCbnSelchangeShape();
  afx_msg void OnEnChangeLeft();
  afx_msg void OnEnChangeTop();
  afx_msg void OnEnChangeRight();
  afx_msg void OnEnChangeBottom();
  afx_msg void OnEnChangePolygon();
  afx_msg void OnBnClickedEvents();
  afx_msg void OnBnClickedOk();
  afx_msg void OnBnClickedCancel();
  afx_msg void OnBnClickedHelpArea();
  afx_msg void OnCbnSelchangeBookmarks();
  afx_msg void OnBnClickedId();
  afx_msg void OnBnClickedPopup();
};

inline bool
AreaDlg::GetDoPopup()
{
  return m_doPopup;
}