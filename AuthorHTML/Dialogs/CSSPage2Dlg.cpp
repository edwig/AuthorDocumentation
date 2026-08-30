//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    CSSPage2Dlg.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for editing CSS properties
//              Handles the text attributes basics
//
#include "stdafx.h"
#include "AuthorHTML.h"
#include "CSSPage2Dlg.h"
#include "StyleSheetDlg.h"
#include "Misc.h"
#include <string>

#define COLOR_DEFAULT_FOREGROUND 0x000000  // BLACK

INT CALLBACK CSSEnumFontNameProc(LOGFONT *plf, TEXTMETRIC* /*ptm*/, INT /*nFontType*/, LPARAM lParam)
{
  CComboBox* comboBox = (CComboBox*) lParam;
  comboBox->AddString(plf->lfFaceName);
  return TRUE;
}

// CSSPage2Dlg dialog

IMPLEMENT_DYNAMIC(CSSPage2Dlg, CDialog)
CSSPage2Dlg::CSSPage2Dlg(CWnd* pParent
                        ,CssStyleSheet* pCss)
	          :CDialog(CSSPage2Dlg::IDD, pParent)
            ,m_css(pCss)
            ,m_initDone(false)
{
}

CSSPage2Dlg::~CSSPage2Dlg()
{
}

void CSSPage2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
  DDX_Control(pDX,IDC_SS_COLORBUTTON,   m_buttonColor);
  DDX_Control(pDX,IDC_CHECK_COLOR,      m_buttonHasColor);
  DDX_Control(pDX,IDC_SS_FONTWEIGHT,    m_comboFontWeight);
  DDX_Control(pDX,IDC_SS_FONTSTYLE,     m_comboFontStyle);
  DDX_Control(pDX,IDC_SS_FONTCASE,      m_comboFontTransform);
  DDX_Control(pDX,IDC_SS_FONTALIGN,     m_comboFontAlign);
  DDX_Control(pDX,IDC_SS_JUSTIFY,       m_comboFontJustify);
  DDX_Control(pDX,IDC_SS_DECORATION,    m_comboFontDecoration);
  DDX_Control(pDX,IDC_SS_WHITESPACE,    m_comboFontWhitespace);
  DDX_Control(pDX,IDC_SS_WRITINGMODE,   m_comboFontWritingMode);
  DDX_Control(pDX,IDC_FF_UNSPEC,        m_buttonFontUnspec);
  DDX_Control(pDX,IDC_FF_PREDEF,        m_buttonFontPredef);
  DDX_Control(pDX,IDC_FF_CUSTOM,        m_buttonFontSpecify);
  DDX_Control(pDX,IDC_SS_PREDEFFONT,    m_comboFontPredefined);
  DDX_Control(pDX,IDC_FONTFAMILY,       m_comboFontSpecify);
  DDX_Control(pDX,IDC_SS_FONTSIZE,      m_comboFontSize);
  DDX_Control(pDX,IDC_FS_UNITS,         m_comboFontSizeUnits);
  DDX_Control(pDX,IDC_SS_LINEHEIGHT,    m_comboLineHeigth);
  DDX_Control(pDX,IDC_LH_UNITS,         m_comboLineHeightUnits);
  DDX_Text   (pDX,IDC_SS_INDENT,        m_lineIndent);
  DDX_Control(pDX,IDC_LI_UNITS,         m_comboLineIndentUnits);
  DDX_Text   (pDX,IDC_SS_LETTERSPACING, m_letterSpacing);
  DDX_Control(pDX,IDC_LS_UNITS,         m_comboLetterSpacingUnits);

  if(pDX->m_bSaveAndValidate == Data2Controls)
  {
    int ind = m_comboFontWeight.FindString(-1,m_fontWeight);
    if(ind >= 0)
    {
      m_comboFontWeight.SetCurSel(ind);
    }
    ind = m_comboFontStyle.FindString(-1,m_fontStyle);
    if(ind >= 0)
    {
      m_comboFontStyle.SetCurSel(ind);
    }
    ind = m_comboFontTransform.FindString(-1,m_fontTransform);
    if(ind >= 0)
    {
      m_comboFontTransform.SetCurSel(ind);
    }
    ind = m_comboFontAlign.FindString(-1,m_fontAlign);
    if(ind >= 0)
    {
      m_comboFontAlign.SetCurSel(ind);
    }
    ind = m_comboFontJustify.FindString(-1,m_fontJustify);
    if(ind >= 0)
    {
      m_comboFontJustify.SetCurSel(ind);
    }
    ind = m_comboFontDecoration.FindString(-1,m_fontDecoration);
    if(ind >= 0)
    {
      m_comboFontDecoration.SetCurSel(ind);
    }
    ind = m_comboFontWhitespace.FindString(-1,m_fontWhitespace);
    if(ind >= 0)
    {
      m_comboFontWhitespace.SetCurSel(ind);
    }
    ind = m_comboFontWritingMode.FindString(-1,m_fontWritingMode);
    if(ind >= 0)
    {
      m_comboFontWritingMode.SetCurSel(ind);
    }

    if(m_fontFamily.IsEmpty())
    {
      m_buttonFontUnspec .SetCheck(TRUE);
      m_buttonFontPredef .SetCheck(FALSE);
      m_buttonFontSpecify.SetCheck(FALSE);
      m_comboFontPredefined.SetCurSel(-1);
      m_comboFontPredefined.EnableWindow(FALSE);
      m_comboFontSpecify.SetCurSel(-1);
      m_comboFontSpecify.EnableWindow(FALSE);
    }
    else
    {
      //Arial, Helvetica, Sans-serif;
      // Times New Roman, Times, Serif;
      // Courier New, Courier, Monospace;
      if((Misc::FindNoCase(m_fontFamily,"Arial")     >= 0) ||
         (Misc::FindNoCase(m_fontFamily,"Helvetica") >= 0) )
      {
        m_buttonFontUnspec .SetCheck(FALSE);
        m_buttonFontPredef .SetCheck(TRUE);
        m_buttonFontSpecify.SetCheck(FALSE);
        m_comboFontPredefined.EnableWindow(TRUE);
        m_comboFontPredefined.SetCurSel(0);
        m_comboFontSpecify.SetCurSel(-1);
        m_comboFontSpecify.EnableWindow(FALSE);
      }
      else if((Misc::FindNoCase(m_fontFamily,"Times New Roman") >= 0) ||
              (m_fontFamily.CompareNoCase("Times") == 0))
      {
        m_buttonFontUnspec .SetCheck(FALSE);
        m_buttonFontPredef .SetCheck(TRUE);
        m_buttonFontSpecify.SetCheck(FALSE);
        m_comboFontPredefined.EnableWindow(TRUE);
        m_comboFontPredefined.SetCurSel(1);
        m_comboFontSpecify.SetCurSel(-1);
        m_comboFontSpecify.EnableWindow(FALSE);
      }
      else if((Misc::FindNoCase(m_fontFamily,"Courier New") >= 0) ||
              (m_fontFamily.CompareNoCase("Courier") == 0))
      {
        m_buttonFontUnspec .SetCheck(FALSE);
        m_buttonFontPredef .SetCheck(TRUE);
        m_buttonFontSpecify.SetCheck(FALSE);
        m_comboFontPredefined.EnableWindow(TRUE);
        m_comboFontPredefined.SetCurSel(2);
        m_comboFontSpecify.SetCurSel(-1);
        m_comboFontSpecify.EnableWindow(FALSE);
      }
      else
      {
        m_buttonFontUnspec .SetCheck(FALSE);
        m_buttonFontPredef .SetCheck(FALSE);
        m_buttonFontSpecify.SetCheck(TRUE);
        m_comboFontPredefined.SetCurSel(-1);
        m_comboFontPredefined.EnableWindow(FALSE);
        m_comboFontSpecify.EnableWindow(TRUE);
        ind = m_comboFontSpecify.FindString(-1,m_fontFamily);
        if(ind >= 0)
        {
          // Preselected font
          m_comboFontSpecify.SetCurSel(ind);
        }
        else
        {
          // Specify our own
          m_comboFontSpecify.SetWindowText(m_fontFamily);
        }
      }
    }
    ind = m_comboFontSize.FindString(-1,m_fontSize);
    if(ind >= 0)
    {
      m_comboFontSize.SetCurSel(ind);
    }
    else
    {
      m_comboFontSize.SetWindowText(m_fontSize);
    }
    ind = m_comboFontSizeUnits.FindString(-1,m_fontSizeUnits);
    if(ind >= 0)
    {
      m_comboFontSizeUnits.SetCurSel(ind);
    }
    else
    {
      m_comboFontSizeUnits.SetWindowText(m_fontSizeUnits);
    }
    ind = m_comboLineIndentUnits.FindString(-1,m_lineIndentUnits);
    if(ind >= 0)
    {
      m_comboLineIndentUnits.SetCurSel(ind);
    }
    else
    {
      m_comboLineIndentUnits.SetWindowText(m_lineIndentUnits);
    }
    ind = m_comboLetterSpacingUnits.FindString(-1,m_letterSpacingUnits);
    if(ind >= 0)
    {
      m_comboLetterSpacingUnits.SetCurSel(ind);
    }
    else
    {
      m_comboLetterSpacingUnits.SetWindowText(m_letterSpacingUnits);
    }


    m_buttonColor.EnableWindow(m_hasFontColor);
    m_buttonHasColor.SetCheck(m_hasFontColor);
  }
}

BEGIN_MESSAGE_MAP(CSSPage2Dlg, CDialog)
  ON_BN_CLICKED     (IDC_SS_COLORBUTTON,  OnBnClickedSsColorbutton)
  ON_BN_CLICKED     (IDC_CHECK_COLOR,     OnBnClickedCheckColor)
  ON_CBN_SELCHANGE  (IDC_SS_FONTWEIGHT,   OnCbnSelchangeSsFontweight)
  ON_CBN_SELCHANGE  (IDC_SS_FONTSTYLE,    OnCbnSelchangeSsFontstyle)
  ON_CBN_SELCHANGE  (IDC_SS_FONTCASE,     OnCbnSelchangeSsFontcase)
  ON_CBN_SELCHANGE  (IDC_SS_FONTALIGN,    OnCbnSelchangeSsFontalign)
  ON_CBN_SELCHANGE  (IDC_SS_JUSTIFY,      OnCbnSelchangeSsFontJustify)
  ON_CBN_SELCHANGE  (IDC_SS_DECORATION,   OnCbnSelchangeSsFontDecoration)
  ON_CBN_SELCHANGE  (IDC_SS_WHITESPACE,   OnCbnSelchangeSsFontWhitespace)
  ON_BN_CLICKED     (IDC_FF_UNSPEC,       OnBnClickedFfUnspec)
  ON_BN_CLICKED     (IDC_FF_PREDEF,       OnBnClickedFfPredef)
  ON_BN_CLICKED     (IDC_FF_CUSTOM,       OnBnClickedFfCustom)
  ON_CBN_SELCHANGE  (IDC_SS_PREDEFFONT,   OnCbnSelchangeSsPredeffont)
  ON_CBN_SELCHANGE  (IDC_FONTFAMILY,      OnCbnSelchangeFontfamily)
  ON_CBN_SELCHANGE  (IDC_SS_FONTSIZE,     OnCbnSelchangeSsFontsize)
  ON_CBN_KILLFOCUS  (IDC_SS_FONTSIZE,     OnCbnSelchangeSsFontsize)
  ON_CBN_SELCHANGE  (IDC_FS_UNITS,        OnCbnSelchangeFsUnits)
  ON_CBN_SELCHANGE  (IDC_SS_LINEHEIGHT,   OnCbnSelchangeSsLineheight)
  ON_CBN_SELCHANGE  (IDC_LH_UNITS,        OnCbnSelchangeLhUnits)
  ON_EN_KILLFOCUS   (IDC_SS_INDENT,       OnEnChangeIndent)
  ON_CBN_SELCHANGE  (IDC_LI_UNITS,        OnCbnSelchangeLiUnits)
  ON_EN_KILLFOCUS   (IDC_SS_LETTERSPACING,OnEnChangeSsLetterspacing)
  ON_CBN_SELCHANGE  (IDC_LS_UNITS,        OnCbnSelchangeLsUnits)
  ON_CBN_SELCHANGE  (IDC_SS_WRITINGMODE,  OnCbnSelchangeWritingmode)
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CSSPage2Dlg, CDialog)
  ON_EVENT(CSSPage2Dlg, IDC_EXPLORER, 0x00000103, OnDocumentComplete, VTS_DISPATCH VTS_VARIANT)
END_EVENTSINK_MAP()

BOOL
CSSPage2Dlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  // Start up our display preview of fonts
  CWnd *pWnd = GetDlgItem(IDC_EXPLORER);
  if(pWnd)
  {
    m_spBrowser = pWnd->GetControlUnknown();
  }
  // FILL FONT FAMILY
	::EnumFontFamilies(GetDC()->m_hDC
                   ,(LPTSTR) NULL
                   ,(FONTENUMPROC)CSSEnumFontNameProc
                   ,(LPARAM)&(m_comboFontSpecify));

  m_buttonColor.SetColor(COLOR_DEFAULT_FOREGROUND);
  m_buttonColor.EnableOtherButton(_T("More colors"));

  // Read in all the attributes
  m_initDone = true;
  FillPage();
  return TRUE;
}

void
CSSPage2Dlg::SetSelector(string selector)
{
  m_selector = selector;
  FillPage();
}

void
CSSPage2Dlg::Redisplay()
{
  // Display all gotten properties
  UpdateData(Data2Controls);

  UpdateProperties();
  // (Re-)Load image page for display the text
  m_spBrowser->Navigate(L"res://authorhtml.exe/csstext.html", NULL, NULL, NULL, NULL);
}

void
CSSPage2Dlg::FillPage()
{
  CString value;
  m_fontColor       = CSSPropertyGet(m_css,m_selector,"color",          false);
  m_fontWeight      = CSSPropertyGet(m_css,m_selector,"font-weight",    true);
  m_fontStyle       = CSSPropertyGet(m_css,m_selector,"font-style",     true);
  m_fontTransform   = CSSPropertyGet(m_css,m_selector,"text-transform", true);
  m_fontAlign       = CSSPropertyGet(m_css,m_selector,"text-align",     true);
  m_fontFamily      = CSSPropertyGet(m_css,m_selector,"font-family",    false);
  m_fontJustify     = CSSPropertyGet(m_css,m_selector,"text-justify",   true);
  m_fontDecoration  = CSSPropertyGet(m_css,m_selector,"text-decoration",true);
  m_fontWhitespace  = CSSPropertyGet(m_css,m_selector,"white-space",    true);
  m_fontWritingMode = CSSPropertyGet(m_css,m_selector,"writing-mode",   true);
  value             = CSSPropertyGet(m_css,m_selector,"font-size",      false);
  CssSplitValueUnits(value,m_fontSize,m_fontSizeUnits);
  value             = CSSPropertyGet(m_css,m_selector,"line-height",    false);
  CssSplitValueUnits(value,m_lineHeigth,m_lineHeigthUnits);
  value             = CSSPropertyGet(m_css,m_selector,"text-indent",    false);
  CssSplitValueUnits(value,m_lineIndent,m_lineIndentUnits);
  value             = CSSPropertyGet(m_css,m_selector,"letter-spacing", false);
  CssSplitValueUnits(value,m_letterSpacing,m_letterSpacingUnits);

  if(m_fontColor.IsEmpty())
  {
    m_hasFontColor = false;
  }
  else
  {
    m_hasFontColor = true;
    int red,green,blue;
    Misc::DecodeColor(m_fontColor,red,green,blue);
    m_buttonColor.SetColor(RGB(red,green,blue));
  }
  // Redisplay on tab-screen
  Redisplay();
}

// Update all the properties in the stylesheet
// and signal the general dialog to apply changes
void
CSSPage2Dlg::UpdateProperties()
{
  int red,green,blue;

  if(m_hasFontColor)
  {
    if(!m_fontColor.IsEmpty())
    {
      Misc::DecodeColor(m_fontColor,red,green,blue);
      m_fontColor.Format("#%02x%02x%02x",red,green,blue);
    }
  }
  else
  {
    m_fontColor = "";
  }
  CSSPropertyPut(m_css,m_selector,"color",          m_fontColor,      false);
  CSSPropertyPut(m_css,m_selector,"font-weight",    m_fontWeight,     true);
  CSSPropertyPut(m_css,m_selector,"font-style",     m_fontStyle,      true);
  CSSPropertyPut(m_css,m_selector,"text-transform", m_fontTransform,  true);
  CSSPropertyPut(m_css,m_selector,"text-align",     m_fontAlign,      true);
  CSSPropertyPut(m_css,m_selector,"font-family",    m_fontFamily,     false);
  CSSPropertyPut(m_css,m_selector,"text-decoration",m_fontDecoration, true);
  CSSPropertyPut(m_css,m_selector,"text-justify",   m_fontJustify,    true);
  CSSPropertyPut(m_css,m_selector,"white-space",    m_fontWhitespace, true);
  CSSPropertyPut(m_css,m_selector,"writing-mode",   m_fontWritingMode,true);

  CString value;
  value = m_fontSize + m_fontSizeUnits;
  CSSPropertyPut(m_css,m_selector,"font-size",      value,false);
  value = m_lineHeigth + m_lineHeigthUnits;
  CSSPropertyPut(m_css,m_selector,"line-height",    value,false);
  value = m_lineIndent + m_lineIndentUnits;
  CSSPropertyPut(m_css,m_selector,"text-indent",    value,false);
  value = m_letterSpacing + m_letterSpacingUnits;
  CSSPropertyPut(m_css,m_selector,"letter-spacing", value,false);

  StyleSheetDlg* dlg = (StyleSheetDlg*)GetParent();
  dlg->SetCanApply();
}

// CSSPage2Dlg message handlers

void 
CSSPage2Dlg::OnDocumentComplete(LPDISPATCH /*pDisp*/, LPVARIANT /*pURL*/)
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
    CComPtr<IDispatch> dispPara;
    V_VT(&findIndex) = VT_I4;
    V_I4(&findIndex) = ind;
    hr = coll->item(findIndex,findIndex,&dispPara);
    CComQIPtr<IHTMLBodyElement,&IID_IHTMLBodyElement> body = dispPara;
    if(body.p)
    {
      // Change paragraph attributes
      CComQIPtr<IHTMLElement,&IID_IHTMLElement> elem = dispPara;
      CComPtr<IHTMLStyle> style;
      hr = elem->get_style(&style);
      if(SUCCEEDED(hr))
      {
        // SET BACKGROUND COLOR
        CComVariant  value;
        CComBSTR bString = CT2CW(m_backgroundColor);
        V_VT(&value) = VT_BSTR;
        V_BSTR(&value) = bString;
        style->put_backgroundColor(value);
      }
    }
    CComQIPtr<IHTMLParaElement,&IID_IHTMLParaElement> para = dispPara;
    if(para.p)
    {
      // Change paragraph attributes
      CComQIPtr<IHTMLElement,&IID_IHTMLElement> elem = dispPara;
      CComPtr<IHTMLStyle> style;
      hr = elem->get_style(&style);
      if(SUCCEEDED(hr))
      {
        // FONT COLOR
        CComVariant  value;
        CComBSTR bString = CT2CW(m_fontColor);
        V_VT(&value) = VT_BSTR;
        V_BSTR(&value) = bString;
        style->put_color(value);
        // SET BACKGROUND COLOR
        bString = CT2CW(m_backgroundColor);
        V_BSTR(&value) = bString;
        style->put_backgroundColor(value);

        // FONT WEIGHT
        style->put_fontWeight(CssCtoB(m_fontWeight));
        // FONT STYLE
        style->put_fontStyle(CssCtoB(m_fontStyle));
        // TEXT-TRANSFORM
        style->put_textTransform(CssCtoB(m_fontTransform));
        // TEXT-ALIGN
        style->put_textAlign(CssCtoB(m_fontAlign));
        // FONT-FAMILY
        style->put_fontFamily(CssCtoB(m_fontFamily));
        // FONT-SIZE
        V_BSTR(&value) = CssCtoB(m_fontSize + m_fontSizeUnits);
        style->put_fontSize(value);
        // LINE-HEIGHT
        V_BSTR(&value) = CssCtoB(m_lineHeigth);
        style->put_lineHeight(value);
        // TEXT-DECORATION
        style->put_textDecoration(CssCtoB(m_fontDecoration));
      }
    }
  }
}

// Change color with color dialog
void 
CSSPage2Dlg::OnBnClickedSsColorbutton()
{
  COLORREF col = m_buttonColor.GetColor();
  int red   = GetRValue(col);
  int green = GetGValue(col);
  int blue  = GetBValue(col);
  m_fontColor.Format("#%02x%02x%02x",red,green,blue);
  Redisplay();
}

void 
CSSPage2Dlg::OnCbnSelchangeSsFontweight()
{
  int ind = m_comboFontWeight.GetCurSel();
  if(ind >= 0)
  {
    m_comboFontWeight.GetLBText(ind,m_fontWeight);
    Redisplay();
  }
}

void 
CSSPage2Dlg::OnCbnSelchangeSsFontstyle()
{
  int ind = m_comboFontStyle.GetCurSel();
  if(ind >= 0)
  {
    m_comboFontStyle.GetLBText(ind,m_fontStyle);
    Redisplay();
  }
}

void 
CSSPage2Dlg::OnCbnSelchangeSsFontcase()
{
  int ind = m_comboFontTransform.GetCurSel();
  if(ind >= 0)
  {
    m_comboFontTransform.GetLBText(ind,m_fontTransform);
    Redisplay();
  }
}

void 
CSSPage2Dlg::OnCbnSelchangeSsFontalign()
{
  int ind = m_comboFontAlign.GetCurSel();
  if(ind >= 0)
  {
    m_comboFontAlign.GetLBText(ind,m_fontAlign);
    Redisplay();
  }
}

void
CSSPage2Dlg::OnCbnSelchangeSsFontJustify()
{
  int ind = m_comboFontJustify.GetCurSel();
  if(ind >= 0)
  {
    m_comboFontJustify.GetLBText(ind,m_fontJustify);
    Redisplay();
  }
}

void
CSSPage2Dlg::OnCbnSelchangeSsFontDecoration()
{
  int ind = m_comboFontDecoration.GetCurSel();
  if(ind >= 0)
  {
    m_comboFontDecoration.GetLBText(ind,m_fontDecoration);
    Redisplay();
  }
}

void
CSSPage2Dlg::OnCbnSelchangeSsFontWhitespace()
{
  int ind = m_comboFontWhitespace.GetCurSel();
  if(ind >= 0)
  {
    m_comboFontWhitespace.GetLBText(ind,m_fontWhitespace);
    Redisplay();
  }
}

void 
CSSPage2Dlg::OnBnClickedFfUnspec()
{
  m_comboFontPredefined.SetCurSel(-1);
  m_comboFontPredefined.EnableWindow(FALSE);
  m_comboFontSpecify.SetCurSel(-1);
  m_comboFontSpecify.EnableWindow(FALSE);
  m_fontFamily = "";
  Redisplay();
}

void 
CSSPage2Dlg::OnBnClickedFfPredef()
{
  m_comboFontPredefined.EnableWindow(TRUE);
  m_comboFontPredefined.SetCurSel(0);
  m_comboFontPredefined.GetLBText(0,m_fontFamily);
  m_comboFontSpecify.SetCurSel(-1);
  m_comboFontSpecify.EnableWindow(FALSE);
  Redisplay();
}


void 
CSSPage2Dlg::OnBnClickedFfCustom()
{
  m_comboFontPredefined.SetCurSel(-1);
  m_comboFontPredefined.EnableWindow(FALSE);
  m_comboFontSpecify.EnableWindow(TRUE);
  m_comboFontSpecify.SetCurSel(0);
  m_comboFontSpecify.GetLBText(0,m_fontFamily);
}

void 
CSSPage2Dlg::OnCbnSelchangeSsPredeffont()
{
  int ind = m_comboFontPredefined.GetCurSel();
  if(ind >= 0)
  {
    m_comboFontPredefined.GetLBText(ind,m_fontFamily);
    Redisplay();
  }
}

void 
CSSPage2Dlg::OnCbnSelchangeFontfamily()
{
  int ind = m_comboFontSpecify.GetCurSel();
  if(ind >= 0)
  {
    m_comboFontSpecify.GetLBText(ind,m_fontFamily);
    Redisplay();
  }
}

void 
CSSPage2Dlg::OnCbnSelchangeSsFontsize()
{
  int ind = m_comboFontSize.GetCurSel();
  if(ind >= 0)
  {
    m_comboFontSize.GetLBText(ind,m_fontSize);
  }
  else
  {
    m_comboFontSize.GetWindowText(m_fontSize);
  }
  Redisplay();
}

void 
CSSPage2Dlg::OnCbnSelchangeFsUnits()
{
  int ind = m_comboFontSizeUnits.GetCurSel();
  if(ind >= 0)
  {
    m_comboFontSizeUnits.GetLBText(ind,m_fontSizeUnits);
  }
  else
  {
    m_fontSizeUnits = "";
  }
  Redisplay();
}

void 
CSSPage2Dlg::OnCbnSelchangeSsLineheight()
{
  int ind = m_comboLineHeigth.GetCurSel();
  if(ind >= 0)
  {
    m_comboLineHeigth.GetLBText(ind,m_lineHeigth);
  }
  else
  {
    m_comboLineHeigth.GetWindowText(m_lineHeigth);
  }
  Redisplay();
}

void 
CSSPage2Dlg::OnCbnSelchangeLhUnits()
{
  int ind = m_comboLineHeightUnits.GetCurSel();
  if(ind >= 0)
  {
    m_comboLineHeightUnits.GetLBText(ind,m_lineHeigthUnits);
  }
  else
  {
    m_lineHeigthUnits = "";
  }
  Redisplay();
}

void
CSSPage2Dlg::OnEnChangeIndent()
{
  UpdateData();
  Redisplay();
}

void
CSSPage2Dlg::OnCbnSelchangeLiUnits()
{
  int ind = m_comboLineIndentUnits.GetCurSel();
  if(ind >= 0)
  {
    m_comboLineIndentUnits.GetLBText(ind,m_lineIndentUnits);
  }
  else
  {
    m_lineIndentUnits = "";
  }
  Redisplay();
}

void
CSSPage2Dlg::OnEnChangeSsLetterspacing()
{
  UpdateData();
  Redisplay();
}

void
CSSPage2Dlg::OnCbnSelchangeLsUnits()
{
  int ind = m_comboLetterSpacingUnits.GetCurSel();
  if(ind >= 0)
  {
    m_comboLetterSpacingUnits.GetLBText(ind,m_letterSpacingUnits);
  }
  else
  {
    m_letterSpacingUnits = "";
  }
  Redisplay();
}

void
CSSPage2Dlg::OnCbnSelchangeWritingmode()
{
  int ind = m_comboFontWritingMode.GetCurSel();
  if(ind >= 0)
  {
    m_comboFontWritingMode.GetLBText(ind,m_fontWritingMode);
  }
  else
  {
    m_fontWritingMode = "";
  }
  Redisplay();
}

void 
CSSPage2Dlg::OnEnChangeSsFontcolor()
{
  // Show the fully opened Color dialog with red as the selected color.
  UpdateData(Controls2Data);
  int red,green,blue;
  Misc::DecodeColor(m_fontColor,red,green,blue);
  m_fontColor.Format("#%02x%02x%02x",red,green,blue);
  Redisplay();
}

void     
CSSPage2Dlg::SetBackgroundColor(CString color)
{
  m_backgroundColor = color;
  if(m_initDone)
  {
    Redisplay();
  }
}

void 
CSSPage2Dlg::OnBnClickedCheckColor()
{
  m_hasFontColor = m_buttonHasColor.GetCheck() == TRUE;
  Redisplay();
}
