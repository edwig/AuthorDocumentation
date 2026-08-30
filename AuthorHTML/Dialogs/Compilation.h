//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    Compilation.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Small gauge dialog to show the progress of a compilation
//
#pragma  once
#include "stdafx.h"
#include "resource.h"

class Compilation : public CDialog
{
public:
  Compilation(CWnd* pParent,bool show);
  ~Compilation();
  // Dialog Data
  enum { IDD = IDD_COMPILE };

public:
  void DoTheInit();
  BOOL OnInitDialog();
  void Reset();

  void SetText(CString p_text);    // Set text under the gauge
  void SetCurrent(int p_current);  // Set current position
  void SetMax(int p_max);          // Set max positions
  int  GetMax();
  void Increment();                // Increment current position
  void SetAtEnd();                 // Set gauge at end

  void DoDataExchange(CDataExchange* pDX);

  // Implementation
protected:
  DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnBnClickedCancel();
private:
  void Repaint();
  void MakeTextToShow();

  CProgressCtrl m_gauge;     // The gauge progress control
  CString       m_text;      // Text as incomming
  CString       m_showText;  // Text as shown
  int           m_current;   // Current position
  int           m_max;       // Max positions
  int           m_show;      // Show / Hide dialog
};

inline int
Compilation::GetMax()
{
  return m_max;
}