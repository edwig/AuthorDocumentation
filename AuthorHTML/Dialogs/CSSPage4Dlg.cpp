//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    CSSPage4Dlg.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for editing CSS properties
//              Handles the border attributes
//
#include "stdafx.h"
#include "AuthorHTML.h"
#include "StyleSheetDlg.h"
#include "CSSPage4Dlg.h"
#include "Css.h"
#include "Misc.h"

#define COLOR_DEFAULT_BACKGROUNDCOLOR 0x0ffffff // WHITE

// CSSPage4Dlg dialog

IMPLEMENT_DYNAMIC(CSSPage4Dlg, CDialog)
CSSPage4Dlg::CSSPage4Dlg(CWnd*          pParent
                        ,CssStyleSheet* pCss)
	          :CDialog(CSSPage4Dlg::IDD, pParent)
            ,m_css(pCss)
            ,m_allsides(true)
            ,m_useTop  (false)
            ,m_useLeft (false)
            ,m_useRight(false)
            ,m_useBottom(false)
{
}

CSSPage4Dlg::~CSSPage4Dlg()
{
}

void CSSPage4Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
  DDX_Control(pDX,IDC_BORDER_ALL, m_buttonAllSides);
  // LineStyles
  DDX_Control(pDX,IDC_LS_TOP,     m_comboLSTop);
  DDX_Control(pDX,IDC_LS_LEFT,    m_comboLSLeft);
  DDX_Control(pDX,IDC_LS_RIGHT,   m_comboLSRight);
  DDX_Control(pDX,IDC_LS_BOTTOM,  m_comboLSBottom);
  // LineWidth
  DDX_Text   (pDX,IDC_LW_TOP,     m_borderTWidth);
  DDX_Text   (pDX,IDC_LW_LEFT,    m_borderLWidth);
  DDX_Text   (pDX,IDC_LW_RIGHT,   m_borderRWidth);
  DDX_Text   (pDX,IDC_LW_BOTTOM,  m_borderBWidth);
  DDX_Control(pDX,IDC_LWS_TOP,    m_spinTop);
  DDX_Control(pDX,IDC_LWS_LEFT,   m_spinLeft);
  DDX_Control(pDX,IDC_LWS_RIGHT,  m_spinRight);
  DDX_Control(pDX,IDC_LWS_BOTTOM, m_spinBottom);
  DDX_Control(pDX,IDC_LWU_TOP,    m_comboWUTop);
  DDX_Control(pDX,IDC_LWU_LEFT,   m_comboWULeft);
  DDX_Control(pDX,IDC_LWU_RIGHT,  m_comboWURight);
  DDX_Control(pDX,IDC_LWU_BOTTOM, m_comboWUBottom);

  DDX_Control(pDX,IDC_BUT_LC_TOP,   m_buttonTColor);
  DDX_Control(pDX,IDC_BUT_LC_LEFT,  m_buttonLColor);
  DDX_Control(pDX,IDC_BUT_LC_RIGHT, m_buttonRColor);
  DDX_Control(pDX,IDC_BUT_LC_BOTTOM,m_buttonBColor);

  DDX_Control(pDX,IDC_HAS_TC,       m_buttonHasTC);
  DDX_Control(pDX,IDC_HAS_LC,       m_buttonHasLC);
  DDX_Control(pDX,IDC_HAS_RC,       m_buttonHasRC);
  DDX_Control(pDX,IDC_HAS_BC,       m_buttonHasBC);

  DDX_Control(pDX,IDC_USE_TOPLINE,   m_buttonTUse);
  DDX_Control(pDX,IDC_USE_LEFTLINE,  m_buttonLUse);
  DDX_Control(pDX,IDC_USE_RIGHTLINE, m_buttonRUse);
  DDX_Control(pDX,IDC_USE_BOTTOMLINE,m_buttonBUse);

  if(pDX->m_bSaveAndValidate == Data2Controls)
  {
    m_comboLSTop   .EnableWindow(m_useTop || m_allsides);
    m_comboLSLeft  .EnableWindow(m_useLeft);
    m_comboLSRight .EnableWindow(m_useRight);
    m_comboLSBottom.EnableWindow(m_useBottom);

    CWnd* t = GetDlgItem(IDC_LW_TOP);
    CWnd* l = GetDlgItem(IDC_LW_LEFT);
    CWnd* r = GetDlgItem(IDC_LW_RIGHT);
    CWnd* b = GetDlgItem(IDC_LW_BOTTOM);
    t->EnableWindow(m_useTop || m_allsides);
    l->EnableWindow(m_useLeft);
    r->EnableWindow(m_useRight);
    b->EnableWindow(m_useBottom);

    m_spinTop      .EnableWindow(m_useTop || m_allsides);
    m_spinLeft     .EnableWindow(m_useLeft);
    m_spinRight    .EnableWindow(m_useRight);
    m_spinBottom   .EnableWindow(m_useBottom);

    m_comboWUTop   .EnableWindow(m_useTop || m_allsides);
    m_comboWULeft  .EnableWindow(m_useLeft);
    m_comboWURight .EnableWindow(m_useRight);
    m_comboWUBottom.EnableWindow(m_useBottom);

    m_buttonTColor.EnableWindow((m_useTop && m_hasTColor) || (m_allsides && m_hasAColor));
    m_buttonLColor.EnableWindow(m_useLeft   && m_hasLColor);
    m_buttonRColor.EnableWindow(m_useRight  && m_hasRColor);
    m_buttonBColor.EnableWindow(m_useBottom && m_hasBColor);

    m_buttonHasTC.EnableWindow(m_useTop || m_allsides);
    m_buttonHasLC.EnableWindow(m_useLeft);
    m_buttonHasRC.EnableWindow(m_useRight);
    m_buttonHasBC.EnableWindow(m_useBottom);

    m_buttonHasTC.SetCheck(m_hasTColor || m_hasAColor);
    m_buttonHasLC.SetCheck(m_hasLColor);
    m_buttonHasRC.SetCheck(m_hasRColor);
    m_buttonHasBC.SetCheck(m_hasBColor);

    int ind = m_comboLSTop.FindString(-1,m_borderTStyle);
    m_comboLSTop.SetCurSel(ind);
    ind = m_comboLSLeft.FindString(-1,m_borderLStyle);
    m_comboLSLeft.SetCurSel(ind);
    ind = m_comboLSRight.FindString(-1,m_borderRStyle);
    m_comboLSRight.SetCurSel(ind);
    ind = m_comboLSBottom.FindString(-1,m_borderBStyle);
    m_comboLSBottom.SetCurSel(ind);

    ind = m_comboWUTop.FindString(-1,m_borderTUnits);
    m_comboWUTop.SetCurSel(ind);
    ind = m_comboWULeft.FindString(-1,m_borderLUnits);
    m_comboWULeft.SetCurSel(ind);
    ind = m_comboWURight.FindString(-1,m_borderRUnits);
    m_comboWURight.SetCurSel(ind);
    ind = m_comboWUBottom.FindString(-1,m_borderBUnits);
    m_comboWUBottom.SetCurSel(ind);

    m_buttonAllSides.SetCheck(m_allsides);
    m_buttonTUse.SetCheck(m_useTop);
    m_buttonLUse.SetCheck(m_useLeft);
    m_buttonRUse.SetCheck(m_useRight);
    m_buttonBUse.SetCheck(m_useBottom);
  }
}

BEGIN_MESSAGE_MAP(CSSPage4Dlg, CDialog)
  ON_BN_CLICKED   (IDC_BORDER_ALL,  OnBnClickedBorderAll)
  ON_CBN_SELCHANGE(IDC_LS_TOP,      OnCbnSelchangeLsTop)
  ON_CBN_SELCHANGE(IDC_LS_LEFT,     OnCbnSelchangeLsLeft)
  ON_CBN_SELCHANGE(IDC_LS_RIGHT,    OnCbnSelchangeLsRight)
  ON_CBN_SELCHANGE(IDC_LS_BOTTOM,   OnCbnSelchangeLsBottom)
  ON_EN_KILLFOCUS (IDC_LW_TOP,      OnEnChangeLwTop)
  ON_EN_KILLFOCUS (IDC_LW_LEFT,     OnEnChangeLwLeft)
  ON_EN_KILLFOCUS (IDC_LW_RIGHT,    OnEnChangeLwRight)
  ON_EN_KILLFOCUS (IDC_LW_BOTTOM,   OnEnChangeLwBottom)
  ON_CBN_SELCHANGE(IDC_LWU_TOP,     OnCbnSelchangeLwuTop)
  ON_CBN_SELCHANGE(IDC_LWU_LEFT,    OnCbnSelchangeLwuLeft)
  ON_CBN_SELCHANGE(IDC_LWU_RIGHT,   OnCbnSelchangeLwuRight)
  ON_CBN_SELCHANGE(IDC_LWU_BOTTOM,  OnCbnSelchangeLwuBottom)
  ON_BN_CLICKED     (IDC_BUT_LC_TOP,    OnBnClickedButLcTop)
  ON_BN_CLICKED     (IDC_BUT_LC_LEFT,   OnBnClickedButLcLeft)
  ON_BN_CLICKED     (IDC_BUT_LC_RIGHT,  OnBnClickedButLcRight)
  ON_BN_CLICKED     (IDC_BUT_LC_BOTTOM, OnBnClickedButLcBottom)
  ON_BN_CLICKED     (IDC_HAS_TC,        OnBnClickedHasTc)
  ON_BN_CLICKED     (IDC_HAS_LC,        OnBnClickedHasLc)
  ON_BN_CLICKED     (IDC_HAS_RC,        OnBnClickedHasRc)
  ON_BN_CLICKED     (IDC_HAS_BC,        OnBnClickedHasBc)
  ON_BN_CLICKED     (IDC_USE_TOPLINE,   OnBnClickedUseTopline)
  ON_BN_CLICKED     (IDC_USE_LEFTLINE,  OnBnClickedUseLeftline)
  ON_BN_CLICKED     (IDC_USE_RIGHTLINE, OnBnClickedUseRightline)
  ON_BN_CLICKED     (IDC_USE_BOTTOMLINE,OnBnClickedUseBottomline)
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CSSPage4Dlg, CDialog)
  ON_EVENT(CSSPage4Dlg, IDC_EXPLORER, 0x00000103, OnDocumentComplete, VTS_DISPATCH VTS_VARIANT)
END_EVENTSINK_MAP()

BOOL
CSSPage4Dlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  // Start up our display preview of fonts
  CWnd *pWnd = GetDlgItem(IDC_EXPLORER);
  if(pWnd)
  {
    m_spBrowser = pWnd->GetControlUnknown();
  }
  FillPage();
  UpdateData(Data2Controls);
  InitControls();

  return TRUE;
}

void
CSSPage4Dlg::InitControls()
{
  m_spinTop.SetBase(10);
  m_spinTop.SetRange(0,32000);
  m_spinTop.SetPos(_ttoi(m_borderTWidth));
  m_spinLeft.SetBase(10);
  m_spinLeft.SetRange(0,32000);
  m_spinLeft.SetPos(_ttoi(m_borderLWidth));
  m_spinRight.SetBase(10);
  m_spinRight.SetRange(0,32000);
  m_spinRight.SetPos(_ttoi(m_borderRWidth));
  m_spinBottom.SetBase(10);
  m_spinBottom.SetRange(0,32000);
  m_spinBottom.SetPos(_ttoi(m_borderBWidth));

  m_buttonAllSides.SetCheck(m_allsides);

  m_buttonTColor.SetColor(COLOR_DEFAULT_BACKGROUNDCOLOR);
  m_buttonLColor.SetColor(COLOR_DEFAULT_BACKGROUNDCOLOR);
  m_buttonRColor.SetColor(COLOR_DEFAULT_BACKGROUNDCOLOR);
  m_buttonBColor.SetColor(COLOR_DEFAULT_BACKGROUNDCOLOR);
  m_buttonTColor.EnableOtherButton(_T("More colors"));
  m_buttonLColor.EnableOtherButton(_T("More colors"));
  m_buttonRColor.EnableOtherButton(_T("More colors"));
  m_buttonBColor.EnableOtherButton(_T("More colors"));

  CSSComboBoxUnits(m_comboWUTop,   m_borderTUnits);
  CSSComboBoxUnits(m_comboWULeft,  m_borderLUnits);
  CSSComboBoxUnits(m_comboWURight, m_borderRUnits);
  CSSComboBoxUnits(m_comboWUBottom,m_borderBUnits);
}

void
CSSPage4Dlg::SetSelector(XString selector)
{
  m_selector = selector;
  FillPage();
}

void
CSSPage4Dlg::SplitBorderProperty(CString  property
                                ,CString& width
                                ,CString& style
                                ,CString& color)
{
  width = _T("");
  style = _T("");
  color = _T("");

  CString een,twee,drie;
  int pos = property.Find(_T(' '));
  if(pos >= 0)
  {
    een  = property.Left(pos);
    twee = property.Mid(pos+1);
    pos = twee.Find(_T(' '));
    if(pos >= 0)
    {
      drie = twee.Mid(pos+1);
      twee = twee.Left(pos);
    }
  }
  else
  {
    een = property;
  }
  // Search the style
  CString test1 = Misc::GetAttributeDisplayname(_T("border-style"),een);
  CString test2 = Misc::GetAttributeDisplayname(_T("border-style"),twee);
  CString test3 = Misc::GetAttributeDisplayname(_T("border-style"),drie);
  if(!test1.IsEmpty()) { style = een;  een  = _T(""); }
  if(!test2.IsEmpty()) { style = twee; twee = _T(""); }
  if(!test3.IsEmpty()) { style = drie; drie = _T(""); }
  
  // Search the color
  test1 = Misc::GetInternetColor(een);
  test2 = Misc::GetInternetColor(twee);
  test3 = Misc::GetInternetColor(drie);
  if(test1.GetAt(0) == _T('#') || een .GetAt(0) == _T('#')) { color = een;  een  = _T(""); }
  if(test2.GetAt(0) == _T('#') || twee.GetAt(0) == _T('#')) { color = twee; twee = _T(""); }
  if(test3.GetAt(0) == _T('#') || drie.GetAt(0) == _T('#')) { color = drie; drie = _T(""); }

  // What's left must now be the width parameter
  if(!een .IsEmpty()) width = een;
  if(!twee.IsEmpty()) width = twee;
  if(!drie.IsEmpty()) width = drie;
}

CString
CSSPage4Dlg::StripUnspecified(CString property,bool replace/*=true*/)
{
  if(property.CompareNoCase(_T("unspecified")) == 0)
  {
    if(replace)
    {
      return _T("solid");
    }
    return _T("");
  }
  return property;
}

void
CSSPage4Dlg::FillPage()
{
  int red,green,blue;
  // Reset ALL
  m_borderTStyle = _T("");
  m_borderLStyle = _T("");
  m_borderRStyle = _T("");
  m_borderBStyle = _T("");

  m_borderTWidth = _T("");
  m_borderLWidth = _T("");
  m_borderRWidth = _T("");
  m_borderBWidth = _T("");

  m_borderTUnits = _T("");
  m_borderLUnits = _T("");
  m_borderRUnits = _T("");
  m_borderBUnits = _T("");

  m_borderTColor = _T("");
  m_borderLColor = _T("");
  m_borderRColor = _T("");
  m_borderBColor = _T("");

  // Compound attributes
  // Border
  m_allsides  =
  m_useTop    = 
  m_useLeft   =
  m_useRight  =
  m_useBottom = false;

  m_hasAColor = 
  m_hasTColor = 
  m_hasLColor =
  m_hasRColor = 
  m_hasBColor = false;

  CString border = CSSPropertyGet(m_css,m_selector,_T("border"),false);
  if(!border.IsEmpty())
  {
    // All sides the same
    m_allsides = true;

    CString width,style,color;
    SplitBorderProperty(border,width,style,color);
    CString wval,units;
    CssSplitValueUnits(width,wval,units);
    // rework width/style/color
    m_borderTStyle = style;
    m_borderTWidth = wval;
    m_borderTUnits = units;
    if(color.IsEmpty())
    {
      m_hasAColor = false;
    }
    else
    {
      m_hasAColor    = true;
      m_borderTColor = color;
      Misc::DecodeColor(m_borderTColor,red,green,blue);
      m_buttonTColor.SetColor(RGB(red,green,blue));
    }
  }
  else
  {
    // GENERAL STYLES PER PROPERTY
    m_borderTStyle = CSSPropertyGet(m_css,m_selector,_T("border-style"),false);
    m_borderTWidth = CSSPropertyGet(m_css,m_selector,_T("border-width"),false);
    m_borderTColor = CSSPropertyGet(m_css,m_selector,_T("border-color"),false);
    if(!m_borderTStyle.IsEmpty() ||
       !m_borderTWidth.IsEmpty() ||
       !m_borderTColor.IsEmpty() )
    {
      m_allsides = true;
      if(!m_borderTStyle.IsEmpty())
      {
        m_borderLStyle = 
        m_borderRStyle = 
        m_borderBStyle = m_borderTStyle;
      }
      if(!m_borderTWidth.IsEmpty())
      {
        CString wval,units;
        CssSplitValueUnits(m_borderTWidth,wval,units);
        m_borderTWidth = wval; m_borderTUnits = units;
        m_borderLWidth = wval; m_borderLUnits = units;
        m_borderRWidth = wval; m_borderRUnits = units;
        m_borderBWidth = wval; m_borderBUnits = units;
      }
      if(m_borderTColor.IsEmpty())
      {
        m_hasAColor = false;
      }
      else
      {
        m_hasAColor = true;
        m_borderLColor =
        m_borderRColor =
        m_borderBColor = m_borderTColor;
        Misc::DecodeColor(m_borderTColor,red,green,blue);
        m_buttonTColor.SetColor(RGB(red,green,blue));
        m_buttonLColor.SetColor(RGB(red,green,blue));
        m_buttonRColor.SetColor(RGB(red,green,blue));
        m_buttonBColor.SetColor(RGB(red,green,blue));
      }
    }
  }
  if(!m_allsides)
  {
    // GENERAL STYLES PER SIDE
    CString prop;
    prop = CSSPropertyGet(m_css,m_selector,_T("border-top"),false);
    if(!prop.IsEmpty())
    {
      CString width,style,color;
      SplitBorderProperty(prop,width,style,color);
      CString wval,units;
      CssSplitValueUnits(width,wval,units);
      m_borderTStyle = style;
      m_borderTWidth = wval;
      m_borderTUnits = units;
      m_borderTColor = color;
      m_useTop = true;
      if(m_borderTColor.IsEmpty())
      {
        m_hasTColor = false;
      }
      else
      {
        m_hasTColor = true;
        Misc::DecodeColor(m_borderTColor,red,green,blue);
        m_buttonTColor.SetColor(RGB(red,green,blue));
      }
    }
    prop = CSSPropertyGet(m_css,m_selector,_T("border-left"),false);
    if(!prop.IsEmpty())
    {
      CString width,style,color;
      SplitBorderProperty(prop,width,style,color);
      CString wval,units;
      CssSplitValueUnits(width,wval,units);
      m_borderLStyle = style;
      m_borderLWidth = wval;
      m_borderLUnits = units;
      m_borderLColor = color;
      m_useLeft = true;
      if(m_borderLColor.IsEmpty())
      {
        m_hasLColor = false;
      }
      else
      {
        m_hasLColor = true;
        Misc::DecodeColor(m_borderLColor,red,green,blue);
        m_buttonLColor.SetColor(RGB(red,green,blue));
      }
    }
    prop = CSSPropertyGet(m_css,m_selector,_T("border-right"),false);
    if(!prop.IsEmpty())
    {
      CString width,style,color;
      SplitBorderProperty(prop,width,style,color);
      CString wval,units;
      CssSplitValueUnits(width,wval,units);
      m_borderRStyle = style;
      m_borderRWidth = wval;
      m_borderRUnits = units;
      m_borderRColor = color;
      m_useRight = true;
      if(m_borderRColor.IsEmpty())
      {
        m_hasRColor = false;
      }
      else
      {
        m_hasRColor = true;
        Misc::DecodeColor(m_borderRColor,red,green,blue);
        m_buttonRColor.SetColor(RGB(red,green,blue));
      }
    }
    prop = CSSPropertyGet(m_css,m_selector,_T("border-bottom"),false);
    if(!prop.IsEmpty())
    {
      CString width,style,color;
      SplitBorderProperty(prop,width,style,color);
      CString wval,units;
      CssSplitValueUnits(width,wval,units);
      m_borderBStyle = style;
      m_borderBWidth = wval;
      m_borderBUnits = units;
      m_borderBColor = color;
      m_useBottom = true;
      if(m_borderBColor.IsEmpty())
      {
        m_hasBColor = false;
      }
      else
      {
        m_hasBColor = true;
        Misc::DecodeColor(m_borderBColor,red,green,blue);
        m_buttonBColor.SetColor(RGB(red,green,blue));
      }
    }
  }
  // NOW ALL INDIVIDUAL STYLES PER SIDE
  if(!m_allsides && !m_useTop)
  {
    m_borderTStyle = CSSPropertyGet(m_css,m_selector,_T("border-top-style"),true);
    m_borderTWidth = CSSPropertyGet(m_css,m_selector,_T("border-top-width"),false);
    m_borderTColor = CSSPropertyGet(m_css,m_selector,_T("border-top-color"),false);
    CssSplitValueUnits(m_borderTWidth,m_borderTWidth,m_borderTUnits);
    if( m_borderTStyle.CompareNoCase(_T("Unspecified")) ||
       !m_borderTWidth.IsEmpty() ||
       !m_borderTColor.IsEmpty() )
    {
      m_useTop = true;
    }
    if(m_borderTColor.IsEmpty())
    {
      m_hasTColor = false;
    }
    else
    {
      m_hasTColor = true;
      Misc::DecodeColor(m_borderTColor,red,green,blue);
      m_buttonTColor.SetColor(RGB(red,green,blue));
    }
  }
  if(!m_allsides && !m_useLeft)
  {
    m_borderLStyle = CSSPropertyGet(m_css,m_selector,_T("border-left-style"),true);
    m_borderLWidth = CSSPropertyGet(m_css,m_selector,_T("border-left-width"),false);
    m_borderLColor = CSSPropertyGet(m_css,m_selector,_T("border-left-color"),false);
    CssSplitValueUnits(m_borderLWidth,m_borderLWidth,m_borderLUnits);
    if( m_borderLStyle.CompareNoCase(_T("Unspecified")) ||
       !m_borderLWidth.IsEmpty() ||
       !m_borderLColor.IsEmpty() )
    {
      m_useLeft = true;
    }
    if(m_borderLColor.IsEmpty())
    {
      m_hasLColor = false;
    }
    else
    {
      m_hasLColor = true;
      Misc::DecodeColor(m_borderLColor,red,green,blue);
      m_buttonLColor.SetColor(RGB(red,green,blue));
    }
  }
  if(!m_allsides && !m_useRight)
  {
    m_borderRStyle = CSSPropertyGet(m_css,m_selector,_T("border-right-style"),true);
    m_borderRWidth = CSSPropertyGet(m_css,m_selector,_T("border-right-width"),false);
    m_borderRColor = CSSPropertyGet(m_css,m_selector,_T("border-right-color"),false);
    CssSplitValueUnits(m_borderRWidth,m_borderRWidth,m_borderRUnits);
    if( m_borderRStyle.CompareNoCase(_T("Unspecified")) ||
       !m_borderRWidth.IsEmpty() ||
       !m_borderRColor.IsEmpty() )
    {
      m_useRight = true;
    }
    if(m_borderRColor.IsEmpty())
    {
      m_hasRColor = false;
    }
    else
    {
      m_hasRColor = true;
      Misc::DecodeColor(m_borderRColor,red,green,blue);
      m_buttonRColor.SetColor(RGB(red,green,blue));
    }
  }
  if(!m_allsides && !m_useBottom)
  {
    m_borderBStyle = CSSPropertyGet(m_css,m_selector,_T("border-bottom-style"),true);
    m_borderBWidth = CSSPropertyGet(m_css,m_selector,_T("border-bottom-width"),false);
    m_borderBColor = CSSPropertyGet(m_css,m_selector,_T("border-bottom-color"),false);
    CssSplitValueUnits(m_borderBWidth,m_borderBWidth,m_borderBUnits);
    if( m_borderBStyle.CompareNoCase(_T("Unspecified")) ||
       !m_borderBWidth.IsEmpty() ||
       !m_borderBColor.IsEmpty() )
    {
      m_useBottom = true;
    }
    if(m_borderBColor.IsEmpty())
    {
      m_hasBColor = false;
    }
    else
    {
      m_hasBColor = true;
      Misc::DecodeColor(m_borderBColor,red,green,blue);
      m_buttonBColor.SetColor(RGB(red,green,blue));
    }
  }
  Redisplay();
}

void 
CSSPage4Dlg::UpdateProperties()
{
  CString prop;
  // Clear all properties
  CSSPropertyPut(m_css,m_selector,_T("border"),              _T(""),false);
  CSSPropertyPut(m_css,m_selector,_T("border-style"),        _T(""),true);
  CSSPropertyPut(m_css,m_selector,_T("border-width"),        _T(""),false);
  CSSPropertyPut(m_css,m_selector,_T("border-color"),        _T(""),false);
  CSSPropertyPut(m_css,m_selector,_T("border-top"),          _T(""),false);
  CSSPropertyPut(m_css,m_selector,_T("border-left"),         _T(""),false);
  CSSPropertyPut(m_css,m_selector,_T("border-right"),        _T(""),false);
  CSSPropertyPut(m_css,m_selector,_T("border-bottom"),       _T(""),false);
  CSSPropertyPut(m_css,m_selector,_T("border-top-style"),    _T(""),true);
  CSSPropertyPut(m_css,m_selector,_T("border-left-style"),   _T(""),true);
  CSSPropertyPut(m_css,m_selector,_T("border-right-style"),  _T(""),true);
  CSSPropertyPut(m_css,m_selector,_T("border-bottom-style"), _T(""),true);
  CSSPropertyPut(m_css,m_selector,_T("border-top-width"),    _T(""),false);
  CSSPropertyPut(m_css,m_selector,_T("border-left-width"),   _T(""),false);
  CSSPropertyPut(m_css,m_selector,_T("border-right-width"),  _T(""),false);
  CSSPropertyPut(m_css,m_selector,_T("border-bottom-width"), _T(""),false);
  CSSPropertyPut(m_css,m_selector,_T("border-top-color"),    _T(""),false);
  CSSPropertyPut(m_css,m_selector,_T("border-left-color"),   _T(""),false);
  CSSPropertyPut(m_css,m_selector,_T("border-right-color"),  _T(""),false);
  CSSPropertyPut(m_css,m_selector,_T("border-bottom-color"), _T(""),false);

  CString color,style,width;
  if(m_allsides)
  {
    color = m_hasAColor ? m_borderTColor : _T("");
    style = StripUnspecified(m_borderTStyle);
    width = m_borderTWidth + m_borderTUnits;
    prop = width;
    if(!style.IsEmpty())
    {
      prop += _T(" ") + style;
    }
    if(!color.IsEmpty())
    {
      prop += _T(" ") + color;
    }
    CSSPropertyPut(m_css,m_selector,_T("border"),prop,false);
  }
  else
  {
    if(m_useTop)
    {
      color = m_hasTColor ? m_borderTColor : _T("");
      prop = m_borderTWidth + m_borderTUnits + _T(" ") + StripUnspecified(m_borderTStyle) + _T(" ") + color;
      CSSPropertyPut(m_css,m_selector,_T("border-top"),prop,false);
    }
    else
    {
      color = m_hasTColor ? m_borderTColor : _T("");
      CSSPropertyPut(m_css,m_selector,_T("border-top-style"),m_borderTStyle,true);
      CSSPropertyPut(m_css,m_selector,_T("border-top-width"),m_borderTWidth+m_borderTUnits,false);
      CSSPropertyPut(m_css,m_selector,_T("border-top-color"),color,false);
    }
    if(m_useLeft)
    {
      color = m_hasLColor ? m_borderLColor : _T("");
      prop = m_borderLWidth + m_borderLUnits + _T(" ") + StripUnspecified(m_borderLStyle) + _T(" ") + color;
      CSSPropertyPut(m_css,m_selector,_T("border-left"),prop,false);
    }
    else
    {
      color = m_hasLColor ? m_borderLColor : _T("");
      CSSPropertyPut(m_css,m_selector,_T("border-left-style"),m_borderLStyle,true);
      CSSPropertyPut(m_css,m_selector,_T("border-left-width"),m_borderLWidth+m_borderLUnits,false);
      CSSPropertyPut(m_css,m_selector,_T("border-left-color"),color,false);
    }
    if(m_useRight)
    {
      color = m_hasRColor ? m_borderRColor : _T("");
      prop = m_borderRWidth + m_borderRUnits + _T(" ") + StripUnspecified(m_borderRStyle) + _T(" ") + color;
      CSSPropertyPut(m_css,m_selector,_T("border-right"),prop,false);
    }
    else
    {
      color = m_hasRColor ? m_borderRColor : _T("");
      CSSPropertyPut(m_css,m_selector,_T("border-right-style"),m_borderRStyle,true);
      CSSPropertyPut(m_css,m_selector,_T("border-right-width"),m_borderRWidth+m_borderRUnits,false);
      CSSPropertyPut(m_css,m_selector,_T("border-right-color"),color,false);
    }
    if(m_useBottom)
    {
      color = m_hasBColor ? m_borderBColor : _T("");
      prop = m_borderBWidth + _T(" ") + m_borderBUnits + StripUnspecified(m_borderBStyle) + _T(" ") + color;
      CSSPropertyPut(m_css,m_selector,_T("border-bottom"),prop,false);
    }
    else
    {
      color = m_hasBColor ? m_borderBColor : _T("");
      CSSPropertyPut(m_css,m_selector,_T("border-bottom-style"), m_borderBStyle,true);
      CSSPropertyPut(m_css,m_selector,_T("border-bottom-width"), m_borderBWidth+m_borderBUnits,false);
      CSSPropertyPut(m_css,m_selector,_T("border-bottom-color"), color,false);
    }
  }
  StyleSheetDlg* dlg = (StyleSheetDlg*)GetParent();
  dlg->SetCanApply();
}

// CSSPage4Dlg message handlers

void 
CSSPage4Dlg::OnDocumentComplete(LPDISPATCH /*pDisp*/, LPVARIANT /*pURL*/)
{
  HRESULT hr = S_FALSE;
  CComPtr<IDispatch> disp;
  CComPtr<IHTMLElementCollection> coll;
  if(!m_doc.p)
  {
    m_spBrowser->get_Document(&disp);
    disp->QueryInterface(IID_IHTMLDocument2,(void**)&m_doc);
  }
  // Place control in editmode so we can change the image
  CComBSTR command = L"IDM_EDITMODE";
  VARIANT_BOOL gelukt;
  VARIANT val;
  V_VT(&val) = VT_BSTR;
  m_doc->execCommand(command,VARIANT_FALSE,val,&gelukt);

  // Get a collection of all DomNodes and iterate over it
  hr = m_doc->get_all(&coll);
  long count = 0;
  coll->get_length(&count);
  for(int ind = 0; ind < count; ++ ind)
  {
    CComVariant findIndex;
    CComPtr<IDispatch> disp2;
    V_VT(&findIndex) = VT_I4;
    V_I4(&findIndex) = ind;
    hr = coll->item(findIndex,findIndex,&disp2);
    CComQIPtr<IHTMLTableCell,&IID_IHTMLTableCell> tdata = disp2;
    if(tdata.p)
    {
      CComQIPtr<IHTMLElement,&IID_IHTMLElement> elem = disp2;
      CComPtr<IHTMLStyle> style;
      hr = elem->get_style(&style);
      if(SUCCEEDED(hr))
      {
        // TOP LINE STYLE
        CComBSTR bString = CT2CW(m_borderTStyle);
        style->put_borderTopStyle(bString);
        // LEFT LINE STYLE
        bString = CT2CW(m_allsides ? m_borderTStyle : m_borderLStyle);
        style->put_borderLeftStyle(bString);
        // RIGHT LINE STYLE
        bString = CT2CW(m_allsides ? m_borderTStyle : m_borderRStyle);
        style->put_borderRightStyle(bString);
        // BOTTOM LINE STYLE
        bString = CT2CW(m_allsides ? m_borderTStyle : m_borderBStyle);
        style->put_borderBottomStyle(bString);

        // TOP LINE WIDTH
        CComVariant value;
        V_VT(&value) = VT_BSTR;
        bString = CT2CW(m_borderTWidth + m_borderTUnits);
        V_BSTR(&value) = bString;
        style->put_borderTopWidth(value);
        // LEFT LINE WIDTH
        bString = CT2CW(m_allsides ? m_borderTWidth + m_borderTUnits : m_borderLWidth + m_borderLUnits);
        V_BSTR(&value) = bString;
        style->put_borderLeftWidth(value);
        // RIGHT LINE WIDTH
        bString = CT2CW(m_allsides ? m_borderTWidth + m_borderTUnits : m_borderRWidth + m_borderRUnits);
        V_BSTR(&value) = bString;
        style->put_borderRightWidth(value);
        // BOTTOM LINE WIDTH
        bString = CT2CW(m_allsides ? m_borderTWidth + m_borderTUnits : m_borderBWidth + m_borderBUnits);
        V_BSTR(&value) = bString;
        style->put_borderBottomWidth(value);

        // TOP LINE COLOR
        bString = CT2CW(m_borderTColor);
        V_BSTR(&value) = bString;
        style->put_borderTopColor(value);
        // LEFT LINE COLOR
        bString = CT2CW(m_allsides ? m_borderTColor : m_borderLColor);
        V_BSTR(&value) = bString;
        style->put_borderLeftColor(value);
        // RIGHT LINE COLOR
        bString = CT2CW(m_allsides ? m_borderTColor : m_borderRColor);
        V_BSTR(&value) = bString;
        style->put_borderRightColor(value);
        // BOTTOM LINE COLOR
        bString = CT2CW(m_allsides ? m_borderTColor : m_borderBColor);
        V_BSTR(&value) = bString;
        style->put_borderBottomColor(value);
      }
    }
  }
}

void 
CSSPage4Dlg::Redisplay()
{
  // Display all gotten properties
  UpdateData(Data2Controls);
  // (Re-)Load image page for display the text
  m_spBrowser->Navigate(L"res://authorhtml.exe/cssborders.html", NULL, NULL, NULL, NULL);
}

void 
CSSPage4Dlg::OnBnClickedBorderAll()
{
  m_allsides = m_buttonAllSides.GetCheck() != 0;
  SetAllSides();
}

void
CSSPage4Dlg::SetAllSides()
{
  if(m_allsides)
  {
    m_borderLStyle = 
    m_borderRStyle = 
    m_borderBStyle =
    m_borderLWidth = 
    m_borderRWidth = 
    m_borderBWidth =
    m_borderLUnits = 
    m_borderRUnits = 
    m_borderBUnits =
    m_borderLColor = 
    m_borderRColor = 
    m_borderBColor = _T("");
    m_useTop    = 
    m_useLeft   =
    m_useRight  =
    m_useBottom = false;
  }
  else
  {
    m_borderLStyle = 
    m_borderRStyle = 
    m_borderBStyle = m_borderTStyle;
    m_borderLWidth =
    m_borderRWidth =
    m_borderBWidth = m_borderTWidth;
    m_borderLUnits = 
    m_borderRUnits = 
    m_borderBUnits = m_borderTUnits;
    m_borderLColor =
    m_borderRColor =
    m_borderBColor = m_borderTColor;
    m_useTop    = 
    m_useLeft   =
    m_useRight  =
    m_useBottom = true;
  }
  UpdateProperties();
  Redisplay();
}

void CSSPage4Dlg::OnCbnSelchangeLsTop()
{
  int ind = m_comboLSTop.GetCurSel();
  if(ind >= 0)
  {
    m_comboLSTop.GetLBText(ind,m_borderTStyle);
    UpdateProperties();
    Redisplay();
  }
}

void CSSPage4Dlg::OnCbnSelchangeLsLeft()
{
  int ind = m_comboLSLeft.GetCurSel();
  if(ind >= 0)
  {
    m_comboLSLeft.GetLBText(ind,m_borderLStyle);
    UpdateProperties();
    Redisplay();
  }
}

void CSSPage4Dlg::OnCbnSelchangeLsRight()
{
  int ind = m_comboLSRight.GetCurSel();
  if(ind >= 0)
  {
    m_comboLSRight.GetLBText(ind,m_borderRStyle);
    UpdateProperties();
    Redisplay();
  }
}

void CSSPage4Dlg::OnCbnSelchangeLsBottom()
{
  int ind = m_comboLSBottom.GetCurSel();
  if(ind >= 0)
  {
    m_comboLSBottom.GetLBText(ind,m_borderBStyle);
    UpdateProperties();
    Redisplay();
  }
}

void CSSPage4Dlg::OnEnChangeLwTop()
{
  CWnd* w = GetDlgItem(IDC_LW_TOP);
  w->GetWindowText(m_borderTWidth);
  UpdateProperties();
  Redisplay();
}

void CSSPage4Dlg::OnEnChangeLwLeft()
{
  CWnd* w = GetDlgItem(IDC_LW_LEFT);
  w->GetWindowText(m_borderLWidth);
  UpdateProperties();
  Redisplay();
}

void 
CSSPage4Dlg::OnEnChangeLwRight()
{
  CWnd* w = GetDlgItem(IDC_LW_RIGHT);
  w->GetWindowText(m_borderRWidth);
  UpdateProperties();
  Redisplay();
}

void CSSPage4Dlg::OnEnChangeLwBottom()
{
  CWnd* w = GetDlgItem(IDC_LW_BOTTOM);
  w->GetWindowText(m_borderBWidth);
  UpdateProperties();
  Redisplay();
}

void 
CSSPage4Dlg::OnCbnSelchangeLwuTop()
{
  int ind = m_comboWUTop.GetCurSel();
  if(ind >= 0)
  {
    CString newunits;
    m_comboWUTop.GetLBText(ind,newunits);
    m_borderTWidth = CssConvertToUnit(m_borderTWidth + m_borderTUnits,newunits,m_borderTUnits,true);
    UpdateProperties();
    Redisplay();
  }
}

void CSSPage4Dlg::OnCbnSelchangeLwuLeft()
{
  int ind = m_comboWULeft.GetCurSel();
  if(ind >= 0)
  {
    CString newunits;
    m_comboWULeft.GetLBText(ind,newunits);
    m_borderLWidth = CssConvertToUnit(m_borderLWidth + m_borderLUnits,newunits,m_borderLUnits,true);
    UpdateProperties();
    Redisplay();
  }
}

void CSSPage4Dlg::OnCbnSelchangeLwuRight()
{
  int ind = m_comboWURight.GetCurSel();
  if(ind >= 0)
  {
    CString newunits;
    m_comboWURight.GetLBText(ind,newunits);
    m_borderRWidth = CssConvertToUnit(m_borderRWidth + m_borderRUnits,newunits,m_borderRUnits,true);
    UpdateProperties();
    Redisplay();
  }
}

void CSSPage4Dlg::OnCbnSelchangeLwuBottom()
{
  int ind = m_comboWUBottom.GetCurSel();
  if(ind >= 0)
  {
    CString newunits;
    m_comboWUBottom.GetLBText(ind,newunits);
    m_borderBWidth = CssConvertToUnit(m_borderBWidth + m_borderBUnits,newunits,m_borderBUnits,true);
    UpdateProperties();
    Redisplay();
  }
}

void 
CSSPage4Dlg::OnBnClickedButLcTop()
{
  COLORREF col = m_buttonTColor.GetColor();
  int red   = GetRValue(col);
  int green = GetGValue(col);
  int blue  = GetBValue(col);
  m_borderTColor.Format(_T("#%02x%02x%02x"),red,green,blue);
  UpdateProperties();
  Redisplay();
}

void 
CSSPage4Dlg::OnBnClickedButLcLeft()
{
  COLORREF col = m_buttonLColor.GetColor();
  int red   = GetRValue(col);
  int green = GetGValue(col);
  int blue  = GetBValue(col);
  m_borderLColor.Format(_T("#%02x%02x%02x"),red,green,blue);
  UpdateProperties();
  Redisplay();
}

void 
CSSPage4Dlg::OnBnClickedButLcRight()
{
  COLORREF col = m_buttonRColor.GetColor();
  int red   = GetRValue(col);
  int green = GetGValue(col);
  int blue  = GetBValue(col);
  m_borderRColor.Format(_T("#%02x%02x%02x"),red,green,blue);
  UpdateProperties();
  Redisplay();
}

void 
CSSPage4Dlg::OnBnClickedButLcBottom()
{
  COLORREF col = m_buttonBColor.GetColor();
  int red   = GetRValue(col);
  int green = GetGValue(col);
  int blue  = GetBValue(col);
  m_borderBColor.Format(_T("#%02x%02x%02x"),red,green,blue);
  UpdateProperties();
  Redisplay();
}

void 
CSSPage4Dlg::OnBnClickedUseTopline()
{
  if(m_buttonTUse.GetCheck())
  {
    m_allsides = false;
    m_useTop   = true;
    m_borderTStyle = _T("solid");
    m_borderTWidth = _T("1");
    m_borderTUnits = _T("px");
    m_borderTColor = _T("black");
    m_hasTColor    = true;
  }
  else
  {
    m_useTop = false;
    m_borderTStyle = _T("");
    m_borderTWidth = _T("");
    m_borderTUnits = _T("");
    m_borderTColor = _T("");
    m_hasTColor    = false;
  }
  int red,green,blue;
  Misc::DecodeColor(m_borderTColor,red,green,blue);
  m_buttonTColor.SetColor(RGB(red,green,blue));
  UpdateProperties();
  Redisplay();
}

void 
CSSPage4Dlg::OnBnClickedUseLeftline()
{
  if(m_buttonLUse.GetCheck())
  {
    m_allsides = false;
    m_useLeft  = true;
    m_borderLStyle = _T("solid");
    m_borderLWidth = _T("1");
    m_borderLUnits = _T("px");
    m_borderLColor = _T("black");
    m_hasLColor    = true;
  }
  else
  {
    m_useLeft = false;
    m_borderLStyle = _T("");
    m_borderLWidth = _T("");
    m_borderLUnits = _T("");
    m_borderLColor = _T("");
    m_hasLColor    = false;
  }
  int red,green,blue;
  Misc::DecodeColor(m_borderLColor,red,green,blue);
  m_buttonLColor.SetColor(RGB(red,green,blue));

  UpdateProperties();
  Redisplay();
}

void 
CSSPage4Dlg::OnBnClickedUseRightline()
{
  if(m_buttonRUse.GetCheck())
  {
    m_allsides = false;
    m_useRight = true;
    m_borderRStyle = _T("solid");
    m_borderRWidth = _T("1");
    m_borderRUnits = _T("px");
    m_borderRColor = _T("black");
    m_hasRColor    = true;
  }
  else
  {
    m_useRight = false;
    m_borderRStyle = _T("");
    m_borderRWidth = _T("");
    m_borderRUnits = _T("");
    m_borderRColor = _T("");
    m_hasRColor    = false;
  }
  int red,green,blue;
  Misc::DecodeColor(m_borderRColor,red,green,blue);
  m_buttonRColor.SetColor(RGB(red,green,blue));

  UpdateProperties();
  Redisplay();
}

void 
CSSPage4Dlg::OnBnClickedUseBottomline()
{
  if(m_buttonBUse.GetCheck())
  {
    m_allsides  = false;
    m_useBottom = true;
    m_borderBStyle = _T("solid");
    m_borderBWidth = _T("1");
    m_borderBUnits = _T("px");
    m_borderBColor = _T("black");
    m_hasBColor    = true;
  }
  else
  {
    m_useBottom = false;
    m_borderBStyle = _T("");
    m_borderBWidth = _T("");
    m_borderBUnits = _T("");
    m_borderBColor = _T("");
    m_hasBColor    = false;
  }
  int red,green,blue;
  Misc::DecodeColor(m_borderBColor,red,green,blue);
  m_buttonBColor.SetColor(RGB(red,green,blue));

  UpdateProperties();
  Redisplay();
}

void 
CSSPage4Dlg::OnBnClickedHasTc()
{
  bool doit = m_buttonHasTC.GetCheck() == TRUE;
  if(m_allsides)
  {
    m_hasAColor = doit;
    m_hasTColor = false;
  }
  else
  {
    m_hasAColor = false;
    m_hasTColor = doit;
  }
  UpdateProperties();
  Redisplay();
}

void 
CSSPage4Dlg::OnBnClickedHasLc()
{
  m_hasLColor = m_buttonHasLC.GetCheck() == TRUE;
  UpdateProperties();
  Redisplay();
}

void 
CSSPage4Dlg::OnBnClickedHasRc()
{
  m_hasRColor = m_buttonHasRC.GetCheck() == TRUE;
  UpdateProperties();
  Redisplay();
}

void 
CSSPage4Dlg::OnBnClickedHasBc()
{
  m_hasBColor = m_buttonHasBC.GetCheck() == TRUE;
  UpdateProperties();
  Redisplay();
}
