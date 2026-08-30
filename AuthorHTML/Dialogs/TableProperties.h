//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    TableProperties.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for managing the properties of a <TABLE> tag in HTML
//              Holds three tabs for the table, the rows and the cells
//
#pragma once
#include "TablePropPage1.h"
#include "TablePropPage2.h"
#include "TablePropPage3.h"
#include "HTMLTable.h"
#include "HTMLTableRow.h"
#include "HTMLTableCell.h"
#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CTablePropertiesDlg dialog

class CTablePropertiesDlg : public CDialog
{
  // Construction
public:
  CTablePropertiesDlg(CWnd*         pParent
                     ,CString       base
                     ,bool          bNewTable
                     ,HtmlTable     pTable
                     ,HtmlTableRow  pRow
                     ,HtmlTableCell pCell
                     ,CComPtr<IHTMLDocument2> doc);
  ~CTablePropertiesDlg();
          INT_PTR StartDialog(int tab);
  virtual BOOL    OnInitDialog();
          CString GetNewTable();
          void    UpdateColumns(int cols);
          void    UpdateRows(int rows);

  // Dialog Data
  enum { IDD = IDD_TABLE_PROPERTIES };

  // Overrides
protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  // Implementation
protected:
  bool                m_bNewTable;       // Making a new table
  CString             m_newTableString;
  CTabCtrl	          m_tabControl;
  CTablePropPage1Dlg* m_tab1Dlg;
  CTablePropPage2Dlg* m_tab2Dlg;
  CTablePropPage3Dlg* m_tab3Dlg;
  HtmlTable           m_pTable;
  HtmlTableRow        m_pRow;
  HtmlTableCell       m_pTableCell;
  int                 m_firstTab;
  CComPtr<IHTMLDocument2> m_doc;
  AD_Button             m_buttonOK;
  AD_Button             m_buttonApply;
  AD_Button             m_buttonCancel;
  AD_Button             m_buttonHelp;

  bool    CreateNewTable();
  void    UpdateProperties();
  // Generated message map functions
  DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnBnClickedOk();
  afx_msg void OnBnClickedCancel();
  afx_msg void OnBnClickedApply();
  afx_msg void OnBnClickedHelp();
  afx_msg void OnTcnSelchangeTabcontrol(NMHDR *pNMHDR, LRESULT *pResult);
};

inline CString
CTablePropertiesDlg::GetNewTable()
{
  return m_newTableString;
}
