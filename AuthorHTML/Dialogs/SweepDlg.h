//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    Sweep.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Sweep dialog for showing progress of a long process
//              e.g. for initial reading the project, or compiling the project
//
#pragma  once
#include "stdafx.h"
#include "resource.h"

class SweepDlg : CDialog
{
public:
   SweepDlg(CWnd* pParent,bool show);
  ~SweepDlg();
   // Dialog Data
	 enum { IDD = IDD_SWEEP };
  
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
SweepDlg::GetMax()
{
  return m_max;
}
