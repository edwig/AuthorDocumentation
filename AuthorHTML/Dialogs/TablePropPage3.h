//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    TablePropPage3.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: property page for the header/footer attributes of the table
//
#pragma once
#include "StdAfx.h"
#include "HTMLTable.h"
#include "HTMLCaption.h"
#include "resource.h"

// CTablePropPage3Dlg dialog

class CTablePropPage3Dlg : public CDialog
{
	DECLARE_DYNAMIC(CTablePropPage3Dlg)

public:
	CTablePropPage3Dlg(CWnd* pParent
                ,CString base
                ,HtmlTable table);
	virtual ~CTablePropPage3Dlg();
  virtual BOOL OnInitDialog();
  void    UpdateProperties();
  void    SetColumns(UINT cols);
  CString CreateCaption();
  CString CreateHeader(UINT cols,CString one_cell);
  CString CreateFooter(UINT cols,CString one_cell);

// Dialog Data
	enum { IDD = IDD_TABPROP_PAGE3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
  void    FillPage();

  HtmlTable   m_table;
  CString     m_base;
  bool        m_initDone;

  // Attributes for caption
  HtmlCaption m_caption;
  bool        m_hasCaption;
  CString     m_vAlign;
  CString     m_text;
  // Attributes for header
  HtmlTableSection  m_header;
  bool              m_hasHeader;
  CString           m_hRows;
  CString           m_columns;
  // Attributes for footer
  HtmlTableSection  m_footer;
  bool              m_hasFooter;
  CString           m_fRows;

  // Controls
  AD_Checkbox   m_buttonHasCaption;
  CComboBox     m_comboAlign;
  AD_Button     m_buttonCaption;
  AD_Checkbox   m_buttonHasHeader;
  AD_Button     m_buttonHeader;
  AD_Checkbox   m_buttonHasFooter;
  AD_Button     m_buttonFooter;
public:
  afx_msg void OnBnClickedHasCaption();
  afx_msg void OnCbnSelchangeCaptionPos();
  afx_msg void OnEnChangeCaptionText();
  afx_msg void OnBnClickedButtCaption();
  afx_msg void OnBnClickedHasHeader();
  afx_msg void OnEnChangeHeadCols();
  afx_msg void OnBnClickedButtHeader();
  afx_msg void OnEnChangeHeadRows();
  afx_msg void OnBnClickedHasFooter();
  afx_msg void OnEnChangeFootRows();
  afx_msg void OnEnChangeFootCols();
  afx_msg void OnBnClickedButtFooter();
};
