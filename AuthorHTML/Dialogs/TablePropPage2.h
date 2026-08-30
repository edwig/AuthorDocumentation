//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    TablePropPage2.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: property page for the cell attributes of the table
//
#pragma once
#include "StdAfx.h"
#include <afxdlgs.h>
#include "resource.h"
#include "HTMLTable.h"
#include "HTMLTableRow.h"
#include "HTMLTableCell.h"
#include "resource.h"

class CTablePropertiesDlg;

class CTablePropPage2Dlg : public CDialog
{
  // Construction
  enum _cellMode
  {
    TP_CellMode
   ,TP_ColumnMode
   ,TP_RowMode
  };
public:
  CTablePropPage2Dlg(CWnd*         pParent
                    ,CString       base
                    ,bool          bNewTable
                    ,HtmlTable     pTable
                    ,HtmlTableRow  pRow
                    ,HtmlTableCell pCell
                    ,CComPtr<IHTMLDocument2> doc);

  CString CreateOneCell(int rows,int columns);
  void    UpdateProperties();
  void    UpdateCellProperties(HtmlTableCell pCell);
  void    UpdateRowProperties (HtmlTableRow pRow);
  // Dialog Data
  //{{AFX_DATA(CTablePropertiesDlg)
  enum { IDD = IDD_TABPROP_PAGE2 };
  //}}AFX_DATA
  UINT    m_CellColumnNum;
  UINT    m_maxColumn;
  UINT    m_Height;
  UINT    m_Width;
  bool    m_heightInPixels;
  bool    m_widthInPixels;
  CString m_vAlign;
  CString m_hAlign;
  CString m_backgroundColor;
  CString m_backgroundImage;
  CString m_borderColor;
  CString m_borderColorLight;
  CString m_borderColorDark;

  bool    m_heightEnable;
  bool    m_widthEnable;
  bool    m_verticalEnable;
  bool    m_horizontalEnable;
  bool    m_backColorEnable;
  bool    m_backImageEnable;
  bool    m_borderColorEnable;
  bool    m_borderLightColorEnable;
  bool    m_borderDarkColorEnable;

  enum _cellMode m_mode;
  bool    m_bInitDone;

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CTablePropertiesDlg)
protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
          BOOL OnInitDialog();
  //}}AFX_VIRTUAL

  // Implementation
private:
  void            InitTabForCell(HtmlTableCell cell);
  void            InitTabForRow(HtmlTableRow p_row);
  void            CommitUpdates();

  CComPtr<IHTMLDocument2> m_doc;
  bool            m_bNewTable;
  HtmlTable       m_pTable;
  HtmlTableRow    m_pRow;
  HtmlTableCell   m_pCell;
  CString         m_base;
  // Controls
  CComboBox       m_cellSelectCtrl;
  AD_Button       m_previousButton;
  AD_Button       m_nextButton;
  CSpinButtonCtrl m_cellCtrl;
  CSpinButtonCtrl m_heightCtrl;
  CSpinButtonCtrl m_widthCtrl;
  CComboBox       m_formatHeightCtrl;
  CComboBox       m_formatWidthCtrl;
  CComboBox       m_alignVertical;
  CComboBox       m_alignHorizontal;

  AD_Checkbox     m_checkHeight;
  AD_Checkbox     m_checkWidth;
  AD_Checkbox     m_checkVertical;
  AD_Checkbox     m_checkHorizontal;
  AD_ColorButton  m_buttonBGColor;
  AD_Checkbox     m_checkBackColor;
  AD_Checkbox     m_checkBackImage;
  AD_Button       m_chooseImage;
  AD_Button       m_buttonID;
  AD_Button       m_buttonEvents;
  AD_Button       m_buttonStyle;

  AD_Checkbox     m_checkBorderColor;
  AD_ColorButton  m_buttonBorderColor;
  AD_Checkbox     m_checkBorderLightColor;
  AD_ColorButton  m_buttonBorderLightColor;
  AD_Checkbox     m_checkBorderDarkColor;
  AD_ColorButton  m_buttonBorderDarkColor;

  // Generated message map functions
  //{{AFX_MSG(CTablePropertiesDlg)
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnCbnSelchangeCellapply();
  afx_msg void OnEnChangeCellnum();
  afx_msg void OnBnClickedPrevcell();
  afx_msg void OnBnClickedNextcell();
  afx_msg void OnBnClickedCheckheight();
  afx_msg void OnBnClickedCheckwidth();
  afx_msg void OnCbnSelchangeFormatheight();
  afx_msg void OnCbnSelchangeFormatwidth();
  afx_msg void OnBnClickedCheckvertical();
  afx_msg void OnBnClickedCheckhorizontal();
  afx_msg void OnCbnSelchangeAlignvert();
  afx_msg void OnCbnSelchangeAlignhorz();
  afx_msg void OnBnClickedButtBgcolorcell();
  afx_msg void OnBnClickedCheckbackcolor();
  afx_msg void OnBnClickedCheckbackimage();
  afx_msg void OnBnClickedStyle();
  afx_msg void OnBnClickedId();
  afx_msg void OnBnClickedEvents();
  afx_msg void OnBnClickedCheckbordercolor();
  afx_msg void OnBnClickedButtBordercolorcell();
  afx_msg void OnBnClickedCheckborderlightcolor();
  afx_msg void OnBnClickedButtBorderlightcolorcell();
  afx_msg void OnBnClickedCheckborderdarkcolor();
  afx_msg void OnBnClickedButtBorderdarkcolorcell();
};