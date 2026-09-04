//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    MainFrame.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: The main window frame on the MS-Windows desktop
//
#pragma once

#include "ScintillaHTMLView.h"	// Added by ClassView
#include "MainFrm.h"
#include "OpenFiles.h"
#include "PageLink2.h"
#include "ExplorerView.h"
#include "TOCView.h"
#include "IndexView.h"
#include "ProjectPane.h"
#include "CompilePane.h"
#include <afxcontrolbars.h>
#include <afxtoolbar.h>

#define WIDHT_CLASSNAME 200     // Width of the classname dialog
#define WIDTH_FONTNAME  200     // width of the fontname  dialog
#define WIDTH_FONTSIZE   60     // width of the fontsize  dialog


class ScintillaHTMLView;
class CHTMLEdView;

class MainFrame : public CMDIFrameWndEx
{
  friend class ScintillaChildFrame;
  //protected: // create from serialization only
  DECLARE_DYNAMIC(MainFrame)
public:
	MainFrame();
  virtual ~MainFrame();
  virtual BOOL    PreTranslateMessage(MSG* pMsg);
  void    ClosePane(int nID);
  void    ShowPane(int nID);
  void    CloseAllPanes();
  void    RedrawFrame(CWnd* pWnd);
  CMFCToolBar* GetEditBar();
  void    LockLinkPages(bool p_lock);

  // Attributes
public:
  CMFCToolBarComboBoxButton* GetClassNameCombo();
  CMFCToolBarFontComboBox*   GetFontNameCombo();
  CMFCToolBarComboBoxButton* GetFontSizeCombo();

  // Overrides
	// ClassWizard generated virtual function overrides
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

  // Implementation
  static void SetStatusText(CString text);

  // Children and directory tree
  void OnCreateChild   (CMDIChildWnd*);
  void OnDestroyChild  (CMDIChildWnd*);
  void OnActivateChild (CMDIChildWnd*);
  void ActivateChild   (CMDIChildWnd*);
  int  GetImageByDocument (const CDocument*);
  void SetMainMenu(bool full);
  void OnRenameChild(CMDIChildWnd*, LPCTSTR);
  BOOL SetSubtree(const CString& path);

  static LPCSTR m_cszClassName;
  void ReDockControlBar();
  void ReDockLeftOf(CMFCToolBar* p_bar);

  // Renaming all files for one renaming
  bool RenameFile       (CString& p_old_href,CString& p_new_href);
  bool CopyFileForRename(CString& p_old_href,CString& p_new_href);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

private:
  void MainFrame::OnApplicationLook(UINT id);
  void MainFrame::OnWindowManager();
  CMDIChildWndEx* CreateDocumentWindow(LPCTSTR lpcszDocName,CObject* pObj);
  
  HWND                m_hActiveChild;
  HWND                m_hLastChild;
  HWND                m_hSkipChild;
  BOOL                m_bMDINextSeq;
  BOOL                m_showWorkbookBar;

  BOOL                m_bShowed;
  CString             m_strProfileName;
  CFont	              m_font;          // default gui font for bar controls
  int                 m_nTheme;        // Current Theme
  bool                m_lockLink;      // Lock link pages recalculation
  UINT                m_nAppLook;      // Current application look

  CMFCMenuBar   m_wndMenuBar;
  CMFCStatusBar m_wndStatusBar;
  CMFCToolBar   m_wndToolBar;
  CMFCToolBar   m_wndEditBar;
  CMFCToolBar   m_wndTableBar;
  CMFCToolBar   m_wndWindowBar;
  CMFCToolBar   m_wndLinkBar;
  CMFCToolBarImages m_UserImages;
public:
  TOCView       m_wndTOCView;
  CExplorerView m_wndExplorerView;
  OpenFiles     m_openFiles;
  IndexView     m_wndIndexView;
  ProjectPane   m_wndProjectView;
  CompilePane   m_wndCompileView;
  PageLink2     m_wndPageLink2View;

  ProjectView*  GetProjectView(); 

  // Generated message map functions
public:
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnClassNameChange();
	afx_msg void OnFontNameChange ();
	afx_msg void OnFontSizeChange ();
	afx_msg void OnUpdatePane (CCmdUI *pCmdUI);
  afx_msg void OnPreferences();
  afx_msg void OnSize(UINT nType,int cx,int cy);

  afx_msg void OnCloseOthers();
  afx_msg BOOL OnToolTipText(UINT nID, NMHDR* pNMHDR, LRESULT* pResult);
  afx_msg void OnMDIMaximize();
  afx_msg void OnMDIMaximizeUI(CCmdUI* pCmdUI);
  afx_msg void OnUpdateUIPaneToc(CCmdUI* pCmdUI);
  afx_msg void OnUpdateUIPaneIndex(CCmdUI* pCmdUI);
  afx_msg void OnUpdateUILinks(CCmdUI* pCmdUI);
  afx_msg void OnUpdateUIExplorer(CCmdUI* pCmdUI);
  afx_msg void OnUpdateUIDocuments(CCmdUI* pCmdUI);
  afx_msg void OnUpdateUIPaneProject(CCmdUI* pCmdUI);
  afx_msg void OnUpdateUIPaneCompile(CCmdUI* pCmdUI);

  afx_msg void OnSysCommand(UINT nID,LPARAM lParam);
  afx_msg void OnLastWindow();
  afx_msg void OnShowPane(UINT nID);
  afx_msg void OnBrokenLinks();
  afx_msg void OnUpdateUI(CCmdUI* pCmdUI);
  afx_msg LRESULT OnToolbarReset  (WPARAM wp, LPARAM);
  afx_msg LRESULT OnRestoreToolbar(WPARAM wp,LPARAM);
  afx_msg void    OnAppLook(UINT id);
  afx_msg void    OnUpdateAppLook(CCmdUI* pCmdUI);
  afx_msg void    OnViewCustomize();

  DECLARE_MESSAGE_MAP()

private:
  void    OnCreate_Platform();
  void    OnCreate_Tabmanager();
  void    OnCreate_DockingPanes();
  void    OnCreate_CommandBars();
  void    OnCreate_StatusBar();
  void    OnCreate_DockCommandBars();
  void    OnCreate_DockDockingPanes();
  void    OnCreate_SetDockingIcons();
  void    OnCreate_MenuPersonalization();

  UINT    GetMsieVersion();
  BOOL    IsPlatformNT();
  void    ResetMsieSettings();
  void    ShowPane(int nID,BOOL focus);
};

inline ProjectView*  
MainFrame::GetProjectView() 
{ 
  return m_wndProjectView.m_proView; 
}

inline CMFCToolBar* 
MainFrame::GetEditBar()
{
  return &m_wndEditBar;
}

inline void
MainFrame::LockLinkPages(bool p_lock)
{
  m_lockLink = p_lock;
}
