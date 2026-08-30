//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    ProjectDlg.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Manages the project (*.hpp) file settings
//
#pragma once
#include "ProjectFile.h"
#include "resource.h"


// ProjectDlg dialog

class ProjectDlg : public CDialog
{
	DECLARE_DYNAMIC(ProjectDlg)

public:
	ProjectDlg(CWnd* p_parent,ProjectFile* p_project);
	virtual ~ProjectDlg();
  BOOL OnInitDialog();

// Dialog Data
	enum { IDD = IDD_PROJECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
  void UpdateProject();
  void SetWindowNames();
  ProjectFile* m_project;

  CString      m_projectName;
  CString      m_compiledName;
  CString      m_title;
  CString      m_defaultTopic;
  AD_Button    m_buttonDefTopic;
  CString      m_defaultFont;
  AD_Button    m_buttonDefFont;
  CComboBox    m_comboWindow;
  CString      m_defaultWindow;
  AD_Button    m_buttonEditWindows;
  bool         m_fullTextSearch;
  AD_Checkbox  m_buttonFullSearch;
  bool         m_binaryIndex;
  AD_Checkbox  m_buttonBinIndex;
  bool         m_autoIndex;
  AD_Checkbox  m_buttonAutoIndex;

  AD_Button    m_buttonOK;
  AD_Button    m_buttonCancel;
  AD_Button    m_buttonHelp;
public:
  afx_msg void OnEnChangeCompiledname();
  afx_msg void OnEnChangeTitle();
  afx_msg void OnEnChangeDefaulttopic();
  afx_msg void OnBnClickedButtotopic();
  afx_msg void OnEnChangeDefaultfont();
  afx_msg void OnBnClickedButtonfont();
  afx_msg void OnCbnSelchangeDefwindow();
  afx_msg void OnBnClickedTextsearch();
  afx_msg void OnBnClickedBinaryindex();
  afx_msg void OnBnClickedAutoindex();
  afx_msg void OnBnClickedOk();
  afx_msg void OnBnClickedCancel();
  afx_msg void OnBnClickedHelpproject();
  afx_msg void OnBnClickedEditwindows();
};
