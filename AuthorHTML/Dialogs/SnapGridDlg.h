//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    SnapGridDlg.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for managing Snap-to-Grid settings
//
#pragma once
#include "resource.h"

// SnapGridDlg dialog

class SnapGridDlg : public CDialog
{
	DECLARE_DYNAMIC(SnapGridDlg)

public:
	SnapGridDlg(CWnd*   pParent
             ,bool    enable
             ,bool    snaptogrid
             ,CString gridWidth
             ,CString gridHeight
             ,CString snapWidth
             ,CString snapHeight
             ,CString lineColor);
	virtual ~SnapGridDlg();
  virtual BOOL OnInitDialog();
  bool    GetGridEnable();
  bool    GetSnapEnable();
  CString GetGridWith();
  CString GetGridHeight();
  CString GetSnapWith();
  CString GetSnapHeight();
  CString GetLineColor();

// Dialog Data
	enum { IDD = IDD_SNAPGRID };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
  void      EnableGrid(bool enable);
  void      EnableSnap(bool enable);
  bool      Ratios();

  bool      m_gridEnable;
  bool      m_snapEnable;
  bool      m_gridEqual;
  bool      m_snapEqual;
  CString   m_gridWidth;
  CString   m_gridHeight;
  CString   m_snapWidth;
  CString   m_snapHeight;
  CString   m_lineColor;

  // Controls
  AD_Checkbox     m_buttonGridEnable;
  AD_Checkbox     m_buttonGridEqual;
  AD_Checkbox     m_buttonSnapEnable;
  AD_Checkbox     m_buttonSnapEqual;
  CSpinButtonCtrl m_spinGW;
  CSpinButtonCtrl m_spinGH;
  CSpinButtonCtrl m_spinSW;
  CSpinButtonCtrl m_spinSH;
  AD_ColorButton  m_buttonColor;
  AD_Button       m_buttonOK;
  AD_Button       m_buttonCancel;
  AD_Button       m_buttonHelp;
public:
  afx_msg void OnBnClickedGridEnable();
  afx_msg void OnBnClickedGridEqual();
  afx_msg void OnBnClickedSnapEnable();
  afx_msg void OnBnClickedSnapEqual();
  afx_msg void OnEnChangeGridWidth();
  afx_msg void OnEnChangeGridHeight();
  afx_msg void OnEnChangeSnapWidth();
  afx_msg void OnEnChangeSnapHeight();
  afx_msg void OnBnClickedGridButtcolor();
  afx_msg void OnBnClickedOk();
  afx_msg void OnBnClickedCancel();
  afx_msg void OnBnClickedGridHelp();
  afx_msg void OnDeltaposSpinGw(NMHDR *pNMHDR, LRESULT *pResult);
  afx_msg void OnDeltaposSpinSw(NMHDR *pNMHDR, LRESULT *pResult);
};

inline bool    
SnapGridDlg::GetGridEnable()
{
  return m_gridEnable;
}

inline bool    
SnapGridDlg::GetSnapEnable()
{
  return m_snapEnable;
}

inline CString 
SnapGridDlg::GetGridWith()
{
  return m_gridWidth;
}

inline CString 
SnapGridDlg::GetGridHeight()
{
  return m_gridHeight;
}

inline CString 
SnapGridDlg::GetSnapWith()
{
  return m_snapWidth;
}

inline CString 
SnapGridDlg::GetSnapHeight()
{
  return m_snapHeight;
}

inline CString 
SnapGridDlg::GetLineColor()
{
  return m_lineColor;
}
