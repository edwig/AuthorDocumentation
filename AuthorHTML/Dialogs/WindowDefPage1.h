//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    WindowDefPage1.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for all window properties page 1 of 5
//
#pragma once
#include "resource.h"
#include "ProjectFile.h"
#include "WindowDefinition.h"

// WindowDefPage1 dialog

class WindowDefPage1 : public CDialog
{
	DECLARE_DYNAMIC(WindowDefPage1)

public:
	WindowDefPage1(CWnd* p_parent
                ,ProjectFile* p_project);
	virtual ~WindowDefPage1();
  BOOL OnInitDialog();
  void SetProperties(WindowDefinition* p_window);
  void UpdateProperties();

// Dialog Data
	enum { IDD = IDD_WINDOWDEF_PAGE1 };

protected:
  void ShowProperties();
	void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

private:
  ProjectFile*      m_project;
  WindowDefinition* m_window;

  bool         m_navigationPane;
  bool         m_textSearch;
  bool         m_advancedSearch;
  bool         m_favourites;
  bool         m_glossary;
  bool         m_browse;
  bool         m_autoSync;
  bool         m_openClosed;
  bool         m_autoShow;
  bool         m_remember;
  int          m_defaultTab;
  int          m_tabPosition;
  long         m_width;

  AD_Checkbox  m_buttonNavPane;
  AD_Checkbox  m_buttonTextSearch;
  AD_Checkbox  m_buttonAdvTSearch;
  AD_Checkbox  m_buttonFavourites;
  AD_Checkbox  m_buttonGlossary;
  AD_Checkbox  m_buttonBrowse;
  AD_Checkbox  m_buttonAutoSync;
  AD_Checkbox  m_buttonOpenClosed;
  AD_Checkbox  m_buttonAutoShow;
  AD_Checkbox  m_buttonRemember;
  CComboBox    m_comboDefTab;
  CComboBox    m_comboTabPos;
  CSpinButtonCtrl m_spin;

	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnBnClickedNavpane();
  afx_msg void OnBnClickedTextsearch();
  afx_msg void OnBnClickedAdvanced();
  afx_msg void OnBnClickedFavourites();
  afx_msg void OnBnClickedGlossary();
  afx_msg void OnBnClickedBrowse();
  afx_msg void OnBnClickedAutosync();
  afx_msg void OnBnClickedOpenclosed();
  afx_msg void OnBnClickedAutoshow();
  afx_msg void OnBnClickedRemember();
  afx_msg void OnCbnSelchangeDefaultTab();
  afx_msg void OnCbnSelchangeTabposition();
  afx_msg void OnEnChangeWidth();
  afx_msg void OnDeltaposSpin(NMHDR *pNMHDR, LRESULT *pResult);
};
