//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    WindowDefPage3.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for all window properties page 3 of 5
//
#pragma once
#include "resource.h"
#include "ProjectFile.h"
#include "WindowDefinition.h"

// WindowDefPage3 dialog

class WindowDefPage3 : public CDialog
{
	DECLARE_DYNAMIC(WindowDefPage3)

public:
	WindowDefPage3(CWnd* p_parent,ProjectFile* p_project);
	virtual ~WindowDefPage3();
  BOOL    OnInitDialog();

  void    SetProperties(WindowDefinition* p_window);
  void    UpdateProperties();

// Dialog Data
	enum { IDD = IDD_WINDOWDEF_PAGE3 };

private:
  void    ShowProperties();

  ProjectFile*      m_project;
  WindowDefinition* m_window;

  // Window styles
  bool        m_maximizeBox;
  bool        m_maximize;
  bool        m_popup;
  bool        m_sysmenu;
  bool        m_clipChildren;
  bool        m_clipSiblings;
  bool        m_minimizeBox;
  bool        m_minimize;
  bool        m_hscroll;
  bool        m_vscroll;
  bool        m_child;
  bool        m_border;
  bool        m_dlgFrame;
  bool        m_visible;
  bool        m_overlapped;
  bool        m_thkFrame;
  bool        m_disabled;
  // Extended styles
  bool        m_tool;
  bool        m_clientEdge;
  bool        m_staticEdge;
  bool        m_rightAligned;
  bool        m_rtl;
  bool        m_controlParent;
  bool        m_acceptFiles;
  bool        m_topmost;
  bool        m_contextHelp;
  bool        m_noNotify;
  bool        m_lscroll;
  bool        m_transparent;

  // Window styles
  AD_Checkbox m_buttonMaxBox;
  AD_Checkbox m_buttonMaximize;
  AD_Checkbox m_buttonPopup;
  AD_Checkbox m_buttonSysmenu;
  AD_Checkbox m_buttonClipChild;
  AD_Checkbox m_buttonClipSib;
  AD_Checkbox m_buttonMinBox;
  AD_Checkbox m_buttonMinimize;
  AD_Checkbox m_buttonHscroll;
  AD_Checkbox m_buttonVscroll;
  AD_Checkbox m_buttonChild;
  AD_Checkbox m_buttonBorder;
  AD_Checkbox m_buttonDlgFrame;
  AD_Checkbox m_buttonVisible;
  AD_Checkbox m_buttonOverlapped;
  AD_Checkbox m_buttonThkFrame;
  AD_Checkbox m_buttonDisabled;
  // Extended styles
  AD_Checkbox m_buttonTool;
  AD_Checkbox m_buttonClientEdge;
  AD_Checkbox m_buttonStaticEdge;
  AD_Checkbox m_buttonRightAligned;
  AD_Checkbox m_buttonRTL;
  AD_Checkbox m_buttonCtrlParent;
  AD_Checkbox m_buttonAcceptFiles;
  AD_Checkbox m_buttonTopmost;
  AD_Checkbox m_buttonContextHelp;
  AD_Checkbox m_buttonNoNotify;
  AD_Checkbox m_buttonLScroll;
  AD_Checkbox m_buttonTransparent;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnBnClickedMaxbox();
  afx_msg void OnBnClickedMaximized();
  afx_msg void OnBnClickedPopup();
  afx_msg void OnBnClickedSysmenu();
  afx_msg void OnBnClickedClipchild();
  afx_msg void OnBnClickedClipsiblings();
  afx_msg void OnBnClickedMinbox();
  afx_msg void OnBnClickedMinimized();
  afx_msg void OnBnClickedHscroll();
  afx_msg void OnBnClickedVscroll();
  afx_msg void OnBnClickedChild();
  afx_msg void OnBnClickedBorder();
  afx_msg void OnBnClickedDlgframe();
  afx_msg void OnBnClickedVisible();
  afx_msg void OnBnClickedOverlapped();
  afx_msg void OnBnClickedThickframe();
  afx_msg void OnBnClickedDisabled();
  afx_msg void OnBnClickedTool();
  afx_msg void OnBnClickedClientedge();
  afx_msg void OnBnClickedStaticedge();
  afx_msg void OnBnClickedRightaligned();
  afx_msg void OnBnClickedRtl();
  afx_msg void OnBnClickedControlparent();
  afx_msg void OnBnClickedAcceptfiles();
  afx_msg void OnBnClickedTopmost();
  afx_msg void OnBnClickedContexthelp();
  afx_msg void OnBnClickedNonotify();
  afx_msg void OnBnClickedLscroll();
  afx_msg void OnBnClickedTransparent();
};
