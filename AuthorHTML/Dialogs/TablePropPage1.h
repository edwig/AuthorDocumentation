//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    TablePropPage1.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: property page for general attributes of the table
//
#pragma once
#include "StdAfx.h"
#include <afxdlgs.h>
#include "resource.h"
#include "HTMLTable.h"
#include "resource.h"

class CTablePropertiesDlg;

class CTablePropPage1Dlg : public CDialog
{
  // Construction
 public:
    CTablePropPage1Dlg(CWnd*     pParent
                      ,CString   base
                      ,HtmlTable pTable);   // standard constructor
    BOOL    OnInitDialog();
    void    UpdateProperties();
    void    SetColumns(int cols);
    void    SetRows   (int rows);
    CString CreateTableString(CString p_caption,CString p_oneCell); 

    // Dialog Data
    //{{AFX_DATA(CTablePropertiesDlg)
    enum { IDD = IDD_TABPROP_PAGE1 };
    UINT	m_NumCols;
    UINT	m_NumRows;
    UINT  m_TabHeight;
    UINT  m_TabWidth;
    UINT  m_TabBorder;
    UINT  m_TabSpacing;
    UINT  m_TabPadding;
    UINT  m_VSpace;
    UINT  m_HSpace;
    CString m_align;

    bool  m_tabHeightInPix;  // Px or %
    bool  m_tabWidthInPix;   // Px or %

    CString m_backgroundColor;
    CString m_borderColor;
    CString m_darkBorderColor;
    CString m_lightBorderColor;
    //}}AFX_DATA

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CTablePropertiesDlg)
  protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
  protected:
    void ReadProperties();

    HtmlTable            m_pTable;
    CString              m_base;
    bool                 m_initDone;

    bool                 m_hasBorderColor;
    bool                 m_hasDarkBorderColor;
    bool                 m_hasLightBorderColor;
    bool                 m_hasBackgroundColor;
    // Controls
    CSpinButtonCtrl      m_TabRowsCtrl;
    CSpinButtonCtrl      m_TabColumnsCtrl;
    CSpinButtonCtrl      m_TabHeightCtrl;
    CSpinButtonCtrl      m_TabWidthCtrl;
    CSpinButtonCtrl      m_TabBorderCtrl;
    CSpinButtonCtrl      m_TabSpacingCtrl;
    CSpinButtonCtrl      m_TabPaddingCtrl;
    CSpinButtonCtrl      m_VSpaceCtrl;
    CSpinButtonCtrl      m_HSpaceCtrl;
    CComboBox            m_formatHeight;
    CComboBox            m_formatWidht;
    CComboBox            m_textJustify;
    AD_ColorButton       m_buttonBorderColor;
    AD_ColorButton       m_buttonDarkBorderColor;
    AD_ColorButton       m_buttonLightBorderColor;
    AD_ColorButton       m_buttonBackColor;
  
    AD_Checkbox          m_buttonCanBorder;
    AD_Checkbox          m_buttonCanDarkBorder;
    AD_Checkbox          m_buttonCanLightBorder;
    AD_Checkbox          m_buttonCanBackColor;

    AD_Button            m_buttonID;
    AD_Button            m_buttonEvents;
    AD_Button            m_buttonStyle;

    // Generated message map functions
    //{{AFX_MSG(CTablePropertiesDlg)
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnCbnSelchangeUnits1();
  afx_msg void OnCbnSelchangeUnits2();
  afx_msg void OnCbnSelchangeTextjustify();
  afx_msg void OnBnClickedButtBordercolor();
  afx_msg void OnBnClickedButtDarkborder();
  afx_msg void OnBnClickedButtLightborder();
  afx_msg void OnBnClickedButtonbackcolor();
  afx_msg void OnBnClickedId();
  afx_msg void OnBnClickedStyle();
  afx_msg void OnBnClickedEvents();
  afx_msg void OnBnClickedCheck1();
  afx_msg void OnBnClickedCheck2();
  afx_msg void OnBnClickedCheck3();
  afx_msg void OnBnClickedCheck4();
};


