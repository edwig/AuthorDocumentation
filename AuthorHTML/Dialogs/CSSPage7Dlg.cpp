//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    CSSPage7Dlg.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for editing CSS properties
//              Handles the basic box attributes
//
#include "stdafx.h"
#include "AuthorHTML.h"
#include "StyleSheetDlg.h"
#include "CSSPage7Dlg.h"

// CSSPage7Dlg dialog

IMPLEMENT_DYNAMIC(CSSPage7Dlg, CDialog)
CSSPage7Dlg::CSSPage7Dlg(CWnd* pParent
                       ,CssStyleSheet* pCss)
          	:CDialog(CSSPage7Dlg::IDD, pParent)
            ,m_css(pCss)
{
}

CSSPage7Dlg::~CSSPage7Dlg()
{
}

void CSSPage7Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
  DDX_Control(pDX,IDC_HORZ_ALIGN, m_comboHAlign);
  DDX_Control(pDX,IDC_VERT_ALIGN, m_comboVAlign);
  DDX_Control(pDX,IDC_DISPLAY,    m_comboDisplay);
  DDX_Control(pDX,IDC_FLOAT,      m_comboFloat);
  DDX_Control(pDX,IDC_POSITION,   m_comboPosition);
  DDX_Control(pDX,IDC_ZINDEX,     m_comboZIndex);
  DDX_Control(pDX,IDC_SPIN_Z,     m_spinZIndex);
  DDX_Control(pDX,IDC_VISIBILITY, m_comboVisibility);
  DDX_Control(pDX,IDC_CLEAR,      m_comboClear);
  DDX_Control(pDX,IDC_OVERFLOW,   m_comboOverflow);
  DDX_Control(pDX,IDC_ZOOM,       m_comboZoom);
  DDX_Control(pDX,IDC_SPIN_ZOOM,  m_spinZoom);

  if(pDX->m_bSaveAndValidate == Data2Controls)
  {
    int ind;
    ind = m_comboHAlign.FindString(-1,m_hAlign);
    if(ind >= 0) m_comboHAlign.SetCurSel(ind);
    else         m_comboHAlign.SetWindowText(m_hAlign);
    ind = m_comboVAlign.FindString(-1,m_vAlign);
    if(ind >= 0) m_comboVAlign.SetCurSel(ind);
    else         m_comboVAlign.SetWindowText(m_vAlign);
    ind = m_comboZIndex.FindString(-1,m_zindex);
    if(ind >= 0) m_comboZIndex.SetCurSel(ind);
    else         m_comboZIndex.SetWindowText(m_zindex);
    ind = m_comboZoom.FindString(-1,m_zoom);
    if(ind >= 0) m_comboZoom.SetCurSel(ind);
    else         m_comboZoom.SetWindowText(m_zoom);
    ind = m_comboDisplay.FindString(-1,m_display);
          m_comboDisplay.SetCurSel(ind);
    ind = m_comboFloat.FindString(-1,m_float);
          m_comboFloat.SetCurSel(ind);
    ind = m_comboPosition.FindString(-1,m_position);
          m_comboPosition.SetCurSel(ind);
    ind = m_comboVisibility.FindString(-1,m_visibility);
          m_comboVisibility.SetCurSel(ind);
    ind = m_comboClear.FindString(-1,m_clear);
          m_comboClear.SetCurSel(ind);
    ind = m_comboOverflow.FindString(-1,m_overflow);
          m_comboOverflow.SetCurSel(ind);
  }
}

BEGIN_MESSAGE_MAP(CSSPage7Dlg, CDialog)
  ON_CBN_SELCHANGE(IDC_DISPLAY,       OnCbnSelchangeDisplay)
  ON_CBN_SELCHANGE(IDC_FLOAT,         OnCbnSelchangeFloat)
  ON_CBN_SELCHANGE(IDC_POSITION,      OnCbnSelchangePosition)
  ON_CBN_SELCHANGE(IDC_ZINDEX,        OnCbnSelchangeZindex)
  ON_CBN_KILLFOCUS(IDC_ZINDEX,        OnCbnSelchangeZindex)
  ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_Z, OnDeltaposSpinZ)
  ON_CBN_SELCHANGE(IDC_VISIBILITY,    OnCbnSelchangeVisibility)
  ON_CBN_SELCHANGE(IDC_CLEAR,         OnCbnSelchangeClear)
  ON_CBN_SELCHANGE(IDC_OVERFLOW,      OnCbnSelchangeOverflow)
  ON_CBN_SELCHANGE(IDC_ZOOM,          OnCbnSelchangeZoom)
  ON_CBN_KILLFOCUS(IDC_ZOOM,          OnCbnSelchangeZoom)
  ON_CBN_SELCHANGE(IDC_HORZ_ALIGN,    OnCbnSelchangeHorzAlign)
  ON_CBN_SELCHANGE(IDC_VERT_ALIGN,    OnCbnSelchangeVertAlign)
END_MESSAGE_MAP()

BOOL
CSSPage7Dlg::OnInitDialog()
{
  CDialog::OnInitDialog();
  m_spinZIndex.SetBase(10);
  m_spinZIndex.SetRange(0,32000);
  m_spinZoom.SetBase(10);
  m_spinZoom.SetRange(0,32000);
  return TRUE;
}

void 
CSSPage7Dlg::SetSelector(string selector)
{
  m_selector = selector;
  FillPage();
}

void 
CSSPage7Dlg::FillPage()
{
  m_hAlign     = CSSPropertyGet(m_css,m_selector,"text-align",    true);
  m_vAlign     = CSSPropertyGet(m_css,m_selector,"vertical-align",true);
  m_display    = CSSPropertyGet(m_css,m_selector,"display",       true);
  m_float      = CSSPropertyGet(m_css,m_selector,"float",         true);
  m_position   = CSSPropertyGet(m_css,m_selector,"position",      true);
  m_zindex     = CSSPropertyGet(m_css,m_selector,"z-index",       true);
  m_visibility = CSSPropertyGet(m_css,m_selector,"visibility",    true);
  m_clear      = CSSPropertyGet(m_css,m_selector,"clear",         true);
  m_overflow   = CSSPropertyGet(m_css,m_selector,"overflow",      true);
  m_zoom       = CSSPropertyGet(m_css,m_selector,"zoom",          true);

  Redisplay();
}

void 
CSSPage7Dlg::UpdateProperties()
{
  CSSPropertyPut(m_css,m_selector,"text-align",     m_hAlign,     true);
  CSSPropertyPut(m_css,m_selector,"vertical-align", m_vAlign,     true);
  CSSPropertyPut(m_css,m_selector,"display",        m_display,    true);
  CSSPropertyPut(m_css,m_selector,"float",          m_float,      true);
  CSSPropertyPut(m_css,m_selector,"position",       m_position,   true);
  CSSPropertyPut(m_css,m_selector,"z-index",        m_zindex,     true);
  CSSPropertyPut(m_css,m_selector,"visibility",     m_visibility, true);
  CSSPropertyPut(m_css,m_selector,"clear",          m_clear,      true);
  CSSPropertyPut(m_css,m_selector,"overflow",       m_overflow,   true);
  CSSPropertyPut(m_css,m_selector,"zoom",           m_zoom,       true);

  StyleSheetDlg* dlg = (StyleSheetDlg*)GetParent();
  dlg->SetCanApply();
}

void 
CSSPage7Dlg::Redisplay()
{
  // Display all gotten properties
  UpdateData(Data2Controls);
}

// CSSPage7Dlg message handlers

void 
CSSPage7Dlg::OnCbnSelchangeDisplay()
{
  int ind = m_comboDisplay.GetCurSel();
  if(ind >= 0)
  {
    m_comboDisplay.GetLBText(ind,m_display);
  }
  UpdateProperties();
  Redisplay();
}

void 
CSSPage7Dlg::OnCbnSelchangeFloat()
{
  int ind = m_comboFloat.GetCurSel();
  if(ind >= 0)
  {
    m_comboFloat.GetLBText(ind,m_float);
  }
  UpdateProperties();
  Redisplay();
}

void 
CSSPage7Dlg::OnCbnSelchangePosition()
{
  int ind = m_comboPosition.GetCurSel();
  if(ind >= 0)
  {
    m_comboPosition.GetLBText(ind,m_position);
  }
  UpdateProperties();
  Redisplay();
}


void 
CSSPage7Dlg::OnCbnSelchangeZindex()
{
  int ind = m_comboZIndex.GetCurSel();
  if(ind >= 0)
  {
    m_comboZIndex.GetLBText(ind,m_zindex);
  }
  else
  {
    m_comboZIndex.GetWindowText(m_zindex);
  }
  UpdateProperties();
  Redisplay();
}

void 
CSSPage7Dlg::OnDeltaposSpinZ(NMHDR *pNMHDR, LRESULT *pResult)
{
  LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
  m_zindex.Format("%d",pNMUpDown->iPos + pNMUpDown->iDelta);
  *pResult = 0;
  UpdateProperties();
  Redisplay();
}


void 
CSSPage7Dlg::OnCbnSelchangeVisibility()
{
  int ind = m_comboVisibility.GetCurSel();
  if(ind >= 0)
  {
    m_comboVisibility.GetLBText(ind,m_visibility);
  }
  UpdateProperties();
  Redisplay();
}

void 
CSSPage7Dlg::OnCbnSelchangeClear()
{
  int ind = m_comboClear.GetCurSel();
  if(ind >= 0)
  {
    m_comboClear.GetLBText(ind,m_clear);
  }
  UpdateProperties();
  Redisplay();
}


void 
CSSPage7Dlg::OnCbnSelchangeOverflow()
{
  int ind = m_comboOverflow.GetCurSel();
  if(ind >= 0)
  {
    m_comboOverflow.GetLBText(ind,m_overflow);
  }
  UpdateProperties();
  Redisplay();
}

void 
CSSPage7Dlg::OnCbnSelchangeZoom()
{
  int ind = m_comboZoom.GetCurSel();
  if(ind >= 0)
  {
    m_comboZoom.GetLBText(ind,m_zoom);
  }
  else
  {
    m_comboZoom.GetWindowText(m_zoom);
  }
  UpdateProperties();
  Redisplay();
}

void 
CSSPage7Dlg::OnCbnSelchangeHorzAlign()
{
  int ind = m_comboHAlign.GetCurSel();
  if(ind >= 0)
  {
    m_comboHAlign.GetLBText(ind,m_hAlign);
  }
  else
  {
    m_comboHAlign.GetWindowText(m_hAlign);
  }
}

void 
CSSPage7Dlg::OnCbnSelchangeVertAlign()
{
  int ind = m_comboVAlign.GetCurSel();
  if(ind >= 0)
  {
    m_comboVAlign.GetLBText(ind,m_vAlign);
  }
  else
  {
    m_comboVAlign.GetWindowText(m_vAlign);
  }
}
