//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    FormSelectDlg.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for managing the <SELECT> tag in HTML, 
//
#pragma once
#include "StdAfx.h"
#include "HTMLElement.h"
#include <vector>
#include "resource.h"
#include "OptionDlg.h"

// FormOption

typedef std::vector<FormOption> FormOptions;

// FormSelectDlg dialog

class FormSelectDlg : public CDialog
{
	DECLARE_DYNAMIC(FormSelectDlg)

public:
	FormSelectDlg(CWnd*   pParent
               ,CString p_base
               ,CString name
               ,CString size
               ,bool    multiple
               ,FormOptions* options
               ,HtmlElement* elem);
  virtual ~FormSelectDlg();
  virtual BOOL OnInitDialog();
  CString GetName();
  CString GetSize();
  bool    GetMultiple();

// Dialog Data
	enum { IDD = IDD_FORMSELECT };

protected:
	void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
private:
  void CheckSizeError();
  void DisplayList(int p_focusRow);

  // Attributes
  CString         m_base;
  CString         m_name;
  CString         m_size;
  bool            m_multiple;
  FormOptions*    m_options;
  CString         m_newOption;
  CString         m_newValue;
  bool            m_newSelected;
  HtmlElement*    m_elem;
  // Controls
  CSpinButtonCtrl m_spinSize;
  AD_Checkbox     m_buttonMultiple;
  CListCtrl       m_list;
  AD_Checkbox     m_buttonNewSelected;
  // Side buttons
  AD_Button         m_buttonUp;
  AD_Button         m_buttonDown;
  AD_Button         m_buttonNew;
  AD_Button         m_buttonDelete;
  // Standard controls
  AD_Button         m_buttonID;
  AD_Button         m_buttonEvents;
  AD_Button         m_buttonStyle;
  AD_Button         m_buttonOK;
  AD_Button         m_buttonCancel;
  AD_Button         m_buttonHelp;
public:
  afx_msg void OnEnChangeFsName();
  afx_msg void OnEnChangeFsSize();
  afx_msg void OnBnClickedFsMultiple();
  afx_msg void OnDeltaposSpin   (NMHDR *pNMHDR, LRESULT *pResult);
  afx_msg void OnLvnItemClick   (NMHDR *pNMHDR, LRESULT *pResult);
  afx_msg void OnLvnItemDblClick(NMHDR *pNMHDR, LRESULT *pResult);
  afx_msg void OnBnClickedOk();
  afx_msg void OnBnClickedCancel();
  afx_msg void OnBnClickedFsHelp();
  afx_msg void OnEnChangeFsNewOption();
  afx_msg void OnEnChangeFsNewValue();
  afx_msg void OnBnClickedFsNewDef();
  afx_msg void OnBnClickedFsUp();
  afx_msg void OnBnClickedFsNew();
  afx_msg void OnBnClickedFsDelete();
  afx_msg void OnBnClickedFsDown();
  afx_msg void OnBnClickedId();
  afx_msg void OnBnClickedButStyle();
  afx_msg void OnBnClickedEvents();
};

inline CString 
FormSelectDlg::GetName()
{
  return m_name;
}

inline CString 
FormSelectDlg::GetSize()
{
  return m_size;
}

inline bool    
FormSelectDlg::GetMultiple()
{
  return m_multiple;
}
