//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    CSSPage5Dlg.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for editing CSS properties
//              Handles the margin, padding, and offset attributes
//
#include "stdafx.h"
#include "AuthorHTML.h"
#include "StyleSheetDlg.h"
#include "css.h"
#include "CSSPage5Dlg.h"

// CSSPage5Dlg dialog

IMPLEMENT_DYNAMIC(CSSPage5Dlg, CDialog)

CSSPage5Dlg::CSSPage5Dlg(CWnd*          pParent
                        ,CssStyleSheet* pCss)
	          :CDialog(CSSPage5Dlg::IDD, pParent)
            ,m_css(pCss)
{
}

CSSPage5Dlg::~CSSPage5Dlg()
{
}

void CSSPage5Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
  // MARGINS
  DDX_Text   (pDX,IDC_MARGIN_T, m_marginTop);
  DDX_Text   (pDX,IDC_MARGIN_L, m_marginLeft);
  DDX_Text   (pDX,IDC_MARGIN_R, m_marginRight);
  DDX_Text   (pDX,IDC_MARGIN_B, m_marginBottom);
  DDX_Control(pDX,IDC_SM_T,     m_spinMT);
  DDX_Control(pDX,IDC_SM_L,     m_spinML);
  DDX_Control(pDX,IDC_SM_R,     m_spinMR);
  DDX_Control(pDX,IDC_SM_B,     m_spinMB);
  DDX_Control(pDX,IDC_MU_T,     m_comboMT);
  DDX_Control(pDX,IDC_MU_L,     m_comboML);
  DDX_Control(pDX,IDC_MU_R,     m_comboMR);
  DDX_Control(pDX,IDC_MU_B,     m_comboMB);
  // PADDINGS
  DDX_Text   (pDX,IDC_PAD_T,    m_paddingTop);
  DDX_Text   (pDX,IDC_PAD_L,    m_paddingLeft);
  DDX_Text   (pDX,IDC_PAD_R,    m_paddingRight);
  DDX_Text   (pDX,IDC_PAD_B,    m_paddingBottom);
  DDX_Control(pDX,IDC_SP_T,     m_spinPT);
  DDX_Control(pDX,IDC_SP_L,     m_spinPL);
  DDX_Control(pDX,IDC_SP_R,     m_spinPR);
  DDX_Control(pDX,IDC_SP_B,     m_spinPB);
  DDX_Control(pDX,IDC_PU_T,     m_comboPT);
  DDX_Control(pDX,IDC_PU_L,     m_comboPL);
  DDX_Control(pDX,IDC_PU_R,     m_comboPR);
  DDX_Control(pDX,IDC_PU_B,     m_comboPB);
  // OFFSETS
  DDX_Text   (pDX,IDC_OFFSET_T, m_offsetTop);
  DDX_Text   (pDX,IDC_OFFSET_L, m_offsetLeft);
  DDX_Text   (pDX,IDC_OFFSET_R, m_offsetRight);
  DDX_Text   (pDX,IDC_OFFSET_B, m_offsetBottom);
  DDX_Control(pDX,IDC_SO_T,     m_spinOT);
  DDX_Control(pDX,IDC_SO_L,     m_spinOL);
  DDX_Control(pDX,IDC_SO_R,     m_spinOR);
  DDX_Control(pDX,IDC_SO_B,     m_spinOB);
  DDX_Control(pDX,IDC_OU_T,     m_comboOT);
  DDX_Control(pDX,IDC_OU_L,     m_comboOL);
  DDX_Control(pDX,IDC_OU_R,     m_comboOR);
  DDX_Control(pDX,IDC_OU_B,     m_comboOB);
  // Width/Height
  DDX_Text   (pDX,IDC_WIDTH,    m_width);
  DDX_Text   (pDX,IDC_HEIGHT,   m_height);
  DDX_Control(pDX,IDC_SW,       m_spinW);
  DDX_Control(pDX,IDC_SH,       m_spinH);
  DDX_Control(pDX,IDC_WIDTH_UNIT, m_comboW);
  DDX_Control(pDX,IDC_HEIGHT_UNIT,m_comboH);

  if(pDX->m_bSaveAndValidate == Data2Controls)
  {
    int ind;
    // MARGINS
    ind = m_comboMT.FindString(-1,m_marginTUnit);
    m_comboMT.SetCurSel(ind);
    ind = m_comboML.FindString(-1,m_marginLUnit);
    m_comboML.SetCurSel(ind);
    ind = m_comboMR.FindString(-1,m_marginRUnit);
    m_comboMR.SetCurSel(ind);
    ind = m_comboMB.FindString(-1,m_marginBUnit);
    m_comboMB.SetCurSel(ind);
    // PADDINGS
    ind = m_comboPT.FindString(-1,m_paddingTUnit);
    m_comboPT.SetCurSel(ind);
    ind = m_comboPL.FindString(-1,m_paddingLUnit);
    m_comboPL.SetCurSel(ind);
    ind = m_comboPR.FindString(-1,m_paddingRUnit);
    m_comboPR.SetCurSel(ind);
    ind = m_comboPB.FindString(-1,m_paddingBUnit);
    m_comboPB.SetCurSel(ind);
    // OFFSETS
    ind = m_comboOT.FindString(-1,m_offsetTUnit);
    m_comboOT.SetCurSel(ind);
    ind = m_comboOL.FindString(-1,m_offsetLUnit);
    m_comboOL.SetCurSel(ind);
    ind = m_comboOR.FindString(-1,m_offsetRUnit);
    m_comboOR.SetCurSel(ind);
    ind = m_comboOB.FindString(-1,m_offsetBUnit);
    m_comboOB.SetCurSel(ind);
    // Width/Height
    ind = m_comboW.FindString(-1,m_widthUnit);
    m_comboW.SetCurSel(ind);
    ind = m_comboH.FindString(-1,m_heightUnit);
    m_comboH.SetCurSel(ind);
  }
}

BEGIN_MESSAGE_MAP(CSSPage5Dlg, CDialog)
  // Margins
  ON_EN_KILLFOCUS (IDC_MARGIN_T, OnEnChangeMarginT)
  ON_EN_KILLFOCUS (IDC_MARGIN_L, OnEnChangeMarginL)
  ON_EN_KILLFOCUS (IDC_MARGIN_R, OnEnChangeMarginR)
  ON_EN_KILLFOCUS (IDC_MARGIN_B, OnEnChangeMarginB)
  ON_CBN_SELCHANGE(IDC_MU_T,     OnCbnSelchangeMuT)
  ON_CBN_SELCHANGE(IDC_MU_L,     OnCbnSelchangeMuL)
  ON_CBN_SELCHANGE(IDC_MU_R,     OnCbnSelchangeMuR)
  ON_CBN_SELCHANGE(IDC_MU_B,     OnCbnSelchangeMuB)
  // Paddings
  ON_EN_KILLFOCUS (IDC_PAD_T,    OnEnChangePadT)
  ON_EN_KILLFOCUS (IDC_PAD_L,    OnEnChangePadL)
  ON_EN_KILLFOCUS (IDC_PAD_R,    OnEnChangePadR)
  ON_EN_KILLFOCUS (IDC_PAD_B,    OnEnChangePadB)
  ON_CBN_SELCHANGE(IDC_PU_T,     OnCbnSelchangePuT)
  ON_CBN_SELCHANGE(IDC_PU_L,     OnCbnSelchangePuL)
  ON_CBN_SELCHANGE(IDC_PU_R,     OnCbnSelchangePuR)
  ON_CBN_SELCHANGE(IDC_PU_B,     OnCbnSelchangePuB)
  // Offsets
  ON_EN_KILLFOCUS (IDC_OFFSET_T, OnEnChangeOffsetT)
  ON_EN_KILLFOCUS (IDC_OFFSET_L, OnEnChangeOffsetL)
  ON_EN_KILLFOCUS (IDC_OFFSET_R, OnEnChangeOffsetR)
  ON_EN_KILLFOCUS (IDC_OFFSET_B, OnEnChangeOffsetB)
  ON_CBN_SELCHANGE(IDC_OU_T,     OnCbnSelchangeOuT)
  ON_CBN_SELCHANGE(IDC_OU_L,     OnCbnSelchangeOuL)
  ON_CBN_SELCHANGE(IDC_OU_R,     OnCbnSelchangeOuR)
  ON_CBN_SELCHANGE(IDC_OU_B,     OnCbnSelchangeOuB)
  // Width / Height
  ON_EN_KILLFOCUS (IDC_WIDTH,      OnEnChangeWidth)
  ON_EN_KILLFOCUS (IDC_HEIGHT,     OnEnChangeHeight)
  ON_CBN_SELCHANGE(IDC_WIDTH_UNIT, OnCbnSelchangeWidthUnit)
  ON_CBN_SELCHANGE(IDC_HEIGHT_UNIT,OnCbnSelchangeHeightUnit)
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CSSPage5Dlg, CDialog)
  ON_EVENT(CSSPage5Dlg, IDC_EXPLORER2, 0x00000103, OnDocumentComplete, VTS_DISPATCH VTS_VARIANT)
END_EVENTSINK_MAP()

BOOL
CSSPage5Dlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  // Start up our display preview of fonts
  CWnd *pWnd = GetDlgItem(IDC_EXPLORER2);
  if(pWnd)
  {
    m_spBrowser = pWnd->GetControlUnknown();
  }
  FillPage();
  UpdateData(Data2Controls);

  // Margins
  m_spinMT.SetBase(10);
  m_spinML.SetBase(10);
  m_spinMR.SetBase(10);
  m_spinMB.SetBase(10);
  m_spinMT.SetRange(-32000,32000);
  m_spinML.SetRange(-32000,32000);
  m_spinMR.SetRange(-32000,32000);
  m_spinMB.SetRange(-32000,32000);
  m_spinMT.SetPos(atoi(m_marginTop));
  m_spinML.SetPos(atoi(m_marginLeft));
  m_spinMR.SetPos(atoi(m_marginRight));
  m_spinMB.SetPos(atoi(m_marginBottom));

  // Paddings
  m_spinPT.SetBase(10);
  m_spinPL.SetBase(10);
  m_spinPR.SetBase(10);
  m_spinPB.SetBase(10);
  m_spinPT.SetRange(-32000,32000);
  m_spinPL.SetRange(-32000,32000);
  m_spinPR.SetRange(-32000,32000);
  m_spinPB.SetRange(-32000,32000);
  m_spinPT.SetPos(atoi(m_paddingTop));
  m_spinPL.SetPos(atoi(m_paddingLeft));
  m_spinPR.SetPos(atoi(m_paddingRight));
  m_spinPB.SetPos(atoi(m_paddingBottom));

  // Offsets
  m_spinOT.SetBase(10);
  m_spinOL.SetBase(10);
  m_spinOR.SetBase(10);
  m_spinOB.SetBase(10);
  m_spinOT.SetRange(-32000,32000);
  m_spinOL.SetRange(-32000,32000);
  m_spinOR.SetRange(-32000,32000);
  m_spinOB.SetRange(-32000,32000);
  m_spinOT.SetPos(atoi(m_offsetTop));
  m_spinOL.SetPos(atoi(m_offsetLeft));
  m_spinOR.SetPos(atoi(m_offsetRight));
  m_spinOB.SetPos(atoi(m_offsetBottom));

  // Width/Height
  m_spinW.SetBase(10);
  m_spinH.SetBase(10);
  m_spinW.SetRange(-32000,32000);
  m_spinH.SetRange(-32000,32000);
  m_spinW.SetPos(atoi(m_width));
  m_spinH.SetPos(atoi(m_height));

  return TRUE;
}

void
CSSPage5Dlg::SetSelector(string selector)
{
  m_selector = selector;
  FillPage();
}

void
CSSPage5Dlg::CSSSizeSplit(CString  property
                         ,CString& top
                         ,CString& left
                         ,CString& right
                         ,CString& bottom)
{
  top = left = right = bottom = "";
  int pos = property.Find(' ');
  if(pos < 0)
  {
    // Just one -> all the same
    top = left = right = bottom = property;
    return;
  }
  CString first  = property.Left(pos);
  CString second = property.Mid(pos+1);
  pos = second.Find(' ');
  if(pos < 0)
  {
    // Two values
    top  = bottom = first;
    left = right  = second;
    return;
  }
  CString third  = second.Mid(pos+1);
          second = second.Left(pos);
  pos = third.Find(' ');
  if(pos < 0)
  {
    // Three values
    top    = first;
    left   = right = second;
    bottom = third;
  }
  // Four values
  CString fourth = third.Mid(pos+1);
          third  = third.Left(pos);
  top    = first;
  right  = second;
  bottom = third;
  left   = fourth;
}

bool
CSSPage5Dlg::CSSSizeMerge(CString& property
                         ,CString& top
                         ,CString& left
                         ,CString& right
                         ,CString& bottom)
{
  property = "";

  if(top.IsEmpty() && left.IsEmpty() && right.IsEmpty() && bottom.IsEmpty())
  {
    return true;
  }
  if(top == bottom && top == left && top == right)
  {
    // All sides the same
    property = top;
    return true;
  }
  if(!top.IsEmpty() && !left.IsEmpty() && !right.IsEmpty() && !bottom.IsEmpty())
  {
    if(top == bottom && right == left)
    {
      // Two values
      property = top + " " + right;
      return true;
    }
    if(right == left)
    {
      // Three values
      property = top + " " + left + " " + bottom;
    }
    // Four values is possible
    property = top + " " + right + " " + bottom + " " + left;
    return true;
  }
  // INDIVIDUAL VALUES NEEDED
  return false;
}

void
CSSPage5Dlg::FillPage()
{
  // GET PROPERTIES
  // Margins
  CString margin = CSSPropertyGet(m_css,m_selector,"margin",false);
  if(margin.IsEmpty())
  {
    m_marginTop    = CSSPropertyGet(m_css,m_selector,"margin-top",   false);
    m_marginLeft   = CSSPropertyGet(m_css,m_selector,"margin-left",  false);
    m_marginRight  = CSSPropertyGet(m_css,m_selector,"margin-right", false);
    m_marginBottom = CSSPropertyGet(m_css,m_selector,"margin-bottom",false);
  }
  else
  {
    CSSSizeSplit(margin,m_marginTop,m_marginLeft,m_marginRight,m_marginBottom);
  }
  CssSplitValueUnits(m_marginTop,   m_marginTop,   m_marginTUnit);
  CssSplitValueUnits(m_marginLeft,  m_marginLeft,  m_marginLUnit);
  CssSplitValueUnits(m_marginRight, m_marginRight, m_marginRUnit);
  CssSplitValueUnits(m_marginBottom,m_marginBottom,m_marginBUnit);
  // Paddings
  CString padding = CSSPropertyGet(m_css,m_selector,"padding",false);
  if(padding.IsEmpty())
  {
    m_paddingTop    = CSSPropertyGet(m_css,m_selector,"padding-top",   false);
    m_paddingLeft   = CSSPropertyGet(m_css,m_selector,"padding-left",  false);
    m_paddingRight  = CSSPropertyGet(m_css,m_selector,"padding-right", false);
    m_paddingBottom = CSSPropertyGet(m_css,m_selector,"padding-bottom",false);
  }
  else
  {
    CSSSizeSplit(padding,m_paddingTop,m_paddingLeft,m_paddingRight,m_paddingBottom);
  }
  CssSplitValueUnits(m_paddingTop,   m_paddingTop,   m_paddingTUnit);
  CssSplitValueUnits(m_paddingLeft,  m_paddingLeft,  m_paddingLUnit);
  CssSplitValueUnits(m_paddingRight, m_paddingRight, m_paddingRUnit);
  CssSplitValueUnits(m_paddingBottom,m_paddingBottom,m_paddingBUnit);
  // Offsets
  m_offsetTop    = CSSPropertyGet(m_css,m_selector,"top",   false);
  m_offsetLeft   = CSSPropertyGet(m_css,m_selector,"left",  false);
  m_offsetRight  = CSSPropertyGet(m_css,m_selector,"right", false);
  m_offsetBottom = CSSPropertyGet(m_css,m_selector,"bottom",false);

  CssSplitValueUnits(m_offsetTop,   m_offsetTop,   m_offsetTUnit);
  CssSplitValueUnits(m_offsetLeft,  m_offsetLeft,  m_offsetLUnit);
  CssSplitValueUnits(m_offsetRight, m_offsetRight, m_offsetRUnit);
  CssSplitValueUnits(m_offsetBottom,m_offsetBottom,m_offsetBUnit);
  // Width/Height
  m_width  = CSSPropertyGet(m_css,m_selector,"width", false);
  m_height = CSSPropertyGet(m_css,m_selector,"height",false);
  CssSplitValueUnits(m_width, m_width, m_widthUnit);
  CssSplitValueUnits(m_height,m_height,m_heightUnit);

  Redisplay();
}

void 
CSSPage5Dlg::UpdateProperties()
{
  // REAPPLY PROPERTIES
  // Margins
  CString property;
  CString top    = m_marginTop    + m_marginTUnit;
  CString left   = m_marginLeft   + m_marginLUnit;
  CString right  = m_marginRight  + m_marginRUnit;
  CString bottom = m_marginBottom + m_marginBUnit;

  if(CSSSizeMerge(property,top,left,right,bottom))
  {
    CSSPropertyPut(m_css,m_selector,"margin",property,false);
  }
  else
  {
    CSSPropertyPut(m_css,m_selector,"margin-top",   top,   false);
    CSSPropertyPut(m_css,m_selector,"margin-left",  left,  false);
    CSSPropertyPut(m_css,m_selector,"margin-right", right, false);
    CSSPropertyPut(m_css,m_selector,"margin-bottom",bottom,false);
  }
  // Paddings
  top    = m_paddingTop    + m_paddingTUnit;
  left   = m_paddingLeft   + m_paddingLUnit;
  right  = m_paddingRight  + m_paddingRUnit;
  bottom = m_paddingBottom + m_paddingBUnit;

  if(CSSSizeMerge(property,top,left,right,bottom))
  {
    CSSPropertyPut(m_css,m_selector,"padding",property,false);
  }
  else
  {
    CSSPropertyPut(m_css,m_selector,"padding-top",   top,   false);
    CSSPropertyPut(m_css,m_selector,"padding-left",  left,  false);
    CSSPropertyPut(m_css,m_selector,"padding-right", right, false);
    CSSPropertyPut(m_css,m_selector,"padding-bottom",bottom,false);
  }
  // Offsets
  top    = m_offsetTop    + m_offsetTUnit;
  left   = m_offsetLeft   + m_offsetLUnit;
  right  = m_offsetRight  + m_offsetRUnit;
  bottom = m_offsetBottom + m_offsetBUnit;

  if(CSSSizeMerge(property,top,left,right,bottom))
  {
    CSSPropertyPut(m_css,m_selector,"offset",property,false);
  }
  else
  {
    CSSPropertyPut(m_css,m_selector,"offset-top",   top,   false);
    CSSPropertyPut(m_css,m_selector,"offset-left",  left,  false);
    CSSPropertyPut(m_css,m_selector,"offset-right", right, false);
    CSSPropertyPut(m_css,m_selector,"offset-bottom",bottom,false);
  }
  // Width / Height
  CString w = m_width  + m_widthUnit;
  CString h = m_height + m_heightUnit;
  CSSPropertyPut(m_css,m_selector,"width", w,false);
  CSSPropertyPut(m_css,m_selector,"height",h,false);

  StyleSheetDlg* dlg = (StyleSheetDlg*)GetParent();
  dlg->SetCanApply();
}

// CSSPage5Dlg message handlers

void 
CSSPage5Dlg::OnDocumentComplete(LPDISPATCH /*pDisp*/, LPVARIANT /*pURL*/)
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
    CComQIPtr<IHTMLFontElement,&IID_IHTMLFontElement> font = disp2;
    if(font.p)
    {
      CComQIPtr<IHTMLElement,&IID_IHTMLElement> elem = disp2;
      CComBSTR bID;
      hr = elem->get_id(&bID);
      if(SUCCEEDED(hr))
      {
        bool blackColor = true;
        bool doColor    = false;
        CString cID = CW2CT(bID);
        if(cID == "tm")
        {
          doColor    = true;
          blackColor = m_marginTop.IsEmpty();
        }
        if(cID == "lm")
        {
          doColor    = true;
          blackColor = m_marginLeft.IsEmpty();
        }
        if(cID == "rm")
        {
          doColor    = true;
          blackColor = m_marginRight.IsEmpty();
        }
        if(cID == "bm")
        {
          doColor    = true;
          blackColor = m_marginBottom.IsEmpty();
        }
        if(cID == "tp")
        {
          doColor    = true;
          blackColor = m_paddingTop.IsEmpty();
        }
        if(cID == "lp")
        {
          doColor    = true;
          blackColor = m_paddingLeft.IsEmpty();
        }
        if(cID == "rp")
        {
          doColor    = true;
          blackColor = m_paddingRight.IsEmpty();
        }
        if(cID == "bp")
        {
          doColor    = true;
          blackColor = m_paddingBottom.IsEmpty();
        }
        if(cID == "to")
        {
          doColor    = true;
          blackColor = m_offsetTop.IsEmpty();
        }
        if(cID == "lo")
        {
          doColor    = true;
          blackColor = m_offsetLeft.IsEmpty();
        }
        if(cID == "ro")
        {
          doColor    = true;
          blackColor = m_offsetRight.IsEmpty();
        }
        if(cID == "bo")
        {
          doColor    = true;
          blackColor = m_offsetBottom.IsEmpty();
        }
        if(cID == "w")
        {
          doColor = true;
          blackColor = m_width.IsEmpty();
        }
        if(cID == "h")
        {
          doColor    = true;
          blackColor = m_height.IsEmpty();
        }
        if(doColor)
        {
          CComVariant value;
          CComBSTR color = blackColor ? L"#000000" : L"#FF0000";
          V_VT(&value) = VT_BSTR;
          V_BSTR(&value) = color;
          font->put_color(value);
        }
      }
    }
  }
}

void 
CSSPage5Dlg::Redisplay()
{
  // Display all gotten properties
  UpdateData(Data2Controls);
  // (Re-)Load image page for display the text
  if(m_spBrowser)
  {
    m_spBrowser->Navigate(L"res://authorhtml.exe/cssbox.html", NULL, NULL, NULL, NULL);
  }
}

/// MARGINS

void 
CSSPage5Dlg::OnEnChangeMarginT()
{
  CWnd* w = GetDlgItem(IDC_MARGIN_T);
  CString newMargin;
  w->GetWindowText(newMargin);
  if(m_marginTop != newMargin)
  {
    m_marginTop = newMargin;
    UpdateProperties();
    Redisplay();
  }
}

void 
CSSPage5Dlg::OnEnChangeMarginL()
{
  CWnd* w = GetDlgItem(IDC_MARGIN_L);
  w->GetWindowText(m_marginLeft);
  UpdateProperties();
  Redisplay();
}

void 
CSSPage5Dlg::OnEnChangeMarginR()
{
  CWnd* w = GetDlgItem(IDC_MARGIN_R);
  w->GetWindowText(m_marginRight);
  UpdateProperties();
  Redisplay();
}

void 
CSSPage5Dlg::OnEnChangeMarginB()
{
  CWnd* w = GetDlgItem(IDC_MARGIN_B);
  w->GetWindowText(m_marginBottom);
  UpdateProperties();
  Redisplay();
}

void 
CSSPage5Dlg::OnCbnSelchangeMuT()
{
  int ind = m_comboMT.GetCurSel();
  if(ind >= 0)
  {
    m_comboMT.GetLBText(ind,m_marginTUnit);
  }
  UpdateProperties();
  Redisplay();
}

void 
CSSPage5Dlg::OnCbnSelchangeMuL()
{
  int ind = m_comboML.GetCurSel();
  if(ind >= 0)
  {
    m_comboML.GetLBText(ind,m_marginLUnit);
  }
  UpdateProperties();
  Redisplay();
}

void 
CSSPage5Dlg::OnCbnSelchangeMuR()
{
  int ind = m_comboMR.GetCurSel();
  if(ind >= 0)
  {
    m_comboMR.GetLBText(ind,m_marginRUnit);
  }
  UpdateProperties();
  Redisplay();
}

void 
CSSPage5Dlg::OnCbnSelchangeMuB()
{
  int ind = m_comboMR.GetCurSel();
  if(ind >= 0)
  {
    m_comboMR.GetLBText(ind,m_marginRUnit);
  }
  UpdateProperties();
  Redisplay();
}

// PADDINGS

void CSSPage5Dlg::OnEnChangePadT()
{
  CWnd* w = GetDlgItem(IDC_PAD_T);
  w->GetWindowText(m_paddingTop);
  UpdateProperties();
  Redisplay();
}

void CSSPage5Dlg::OnEnChangePadL()
{
  CWnd* w = GetDlgItem(IDC_PAD_L);
  w->GetWindowText(m_paddingLeft);
  UpdateProperties();
  Redisplay();
}

void CSSPage5Dlg::OnEnChangePadR()
{
  CWnd* w = GetDlgItem(IDC_PAD_R);
  w->GetWindowText(m_paddingRight);
  UpdateProperties();
  Redisplay();
}

void CSSPage5Dlg::OnEnChangePadB()
{
  CWnd* w = GetDlgItem(IDC_PAD_B);
  w->GetWindowText(m_paddingBottom);
  UpdateProperties();
  Redisplay();
}

void CSSPage5Dlg::OnCbnSelchangePuT()
{
  int ind = m_comboPT.GetCurSel();
  if(ind >= 0)
  {
    m_comboPT.GetLBText(ind,m_paddingTUnit);
  }
  UpdateProperties();
  Redisplay();
}

void CSSPage5Dlg::OnCbnSelchangePuL()
{
  int ind = m_comboPL.GetCurSel();
  if(ind >= 0)
  {
    m_comboPL.GetLBText(ind,m_paddingLUnit);
  }
  UpdateProperties();
  Redisplay();
}

void CSSPage5Dlg::OnCbnSelchangePuR()
{
  int ind = m_comboPR.GetCurSel();
  if(ind >= 0)
  {
    m_comboPR.GetLBText(ind,m_paddingRUnit);
  }
  UpdateProperties();
  Redisplay();
}

void CSSPage5Dlg::OnCbnSelchangePuB()
{
  int ind = m_comboPB.GetCurSel();
  if(ind >= 0)
  {
    m_comboPB.GetLBText(ind,m_paddingBUnit);
  }
  UpdateProperties();
  Redisplay();
}

// OFFSETS

void 
CSSPage5Dlg::OnEnChangeOffsetT()
{
  CWnd* w = GetDlgItem(IDC_OFFSET_T);
  w->GetWindowText(m_offsetTop);
  UpdateProperties();
  Redisplay();
}

void CSSPage5Dlg::OnEnChangeOffsetL()
{
  CWnd* w = GetDlgItem(IDC_OFFSET_L);
  w->GetWindowText(m_offsetLeft);
  UpdateProperties();
  Redisplay();
}

void CSSPage5Dlg::OnEnChangeOffsetR()
{
  CWnd* w = GetDlgItem(IDC_OFFSET_R);
  w->GetWindowText(m_offsetRight);
  UpdateProperties();
  Redisplay();
}

void CSSPage5Dlg::OnEnChangeOffsetB()
{
  CWnd* w = GetDlgItem(IDC_OFFSET_B);
  w->GetWindowText(m_offsetBottom);
  UpdateProperties();
  Redisplay();
}

void CSSPage5Dlg::OnCbnSelchangeOuT()
{
  int ind = m_comboOT.GetCurSel();
  if(ind >= 0)
  {
    m_comboOT.GetLBText(ind,m_offsetTUnit);
  }
  UpdateProperties();
  Redisplay();
}

void CSSPage5Dlg::OnCbnSelchangeOuL()
{
  int ind = m_comboOL.GetCurSel();
  if(ind >= 0)
  {
    m_comboOL.GetLBText(ind,m_offsetLUnit);
  }
  UpdateProperties();
  Redisplay();
}

void CSSPage5Dlg::OnCbnSelchangeOuR()
{
  int ind = m_comboOR.GetCurSel();
  if(ind >= 0)
  {
    m_comboOR.GetLBText(ind,m_offsetRUnit);
  }
  UpdateProperties();
  Redisplay();
}

void CSSPage5Dlg::OnCbnSelchangeOuB()
{
  int ind = m_comboOB.GetCurSel();
  if(ind >= 0)
  {
    m_comboOB.GetLBText(ind,m_offsetBUnit);
  }
  UpdateProperties();
  Redisplay();
}

void CSSPage5Dlg::OnEnChangeWidth()
{
  CWnd* w = GetDlgItem(IDC_WIDTH);
  w->GetWindowText(m_width);
  UpdateProperties();
  Redisplay();
}

void CSSPage5Dlg::OnEnChangeHeight()
{
  CWnd* w = GetDlgItem(IDC_HEIGHT);
  w->GetWindowText(m_height);
  UpdateProperties();
  Redisplay();
}

void CSSPage5Dlg::OnCbnSelchangeWidthUnit()
{
  int ind = m_comboW.GetCurSel();
  if(ind >= 0)
  {
    m_comboW.GetLBText(ind,m_widthUnit);
  }
  UpdateProperties();
  Redisplay();
}

void CSSPage5Dlg::OnCbnSelchangeHeightUnit()
{
  int ind = m_comboH.GetCurSel();
  if(ind >= 0)
  {
    m_comboH.GetLBText(ind,m_heightUnit);
  }
  UpdateProperties();
  Redisplay();
}
