//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    AutoSizerDlg.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog to make the size of the target CHM visible.
//
#pragma once
#include "resource.h"

// AutosizerDlg dialog

class AutosizerDlg : public CDialog
{
	DECLARE_DYNAMIC(AutosizerDlg)

public:
	AutosizerDlg(CWnd* p_parent
              ,int p_left,int p_top,int p_width,int p_height);
	virtual ~AutosizerDlg();
  BOOL OnInitDialog();
  int  GetLeft();
  int  GetTop();
  int  GetWidth();
  int  GetHeight();

// Dialog Data
	enum { IDD = IDD_AUTOSIZER };
private:
  int   m_left;
  int   m_top;
  int   m_width;
  int   m_height;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnBnClickedHelpAutosizer();
  afx_msg void OnBnClickedOk();
  afx_msg void OnBnClickedCancel();
};

inline int
AutosizerDlg::GetLeft()
{
  return m_left;
}

inline int
AutosizerDlg::GetTop()
{
  return m_top;
}

inline int
AutosizerDlg::GetWidth()
{
  return m_width;
}

inline int 
AutosizerDlg::GetHeight()
{
  return m_height;
}
