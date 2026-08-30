//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    TopicPropPage2.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for all topic properties page 2 of 5
//
#include "stdafx.h"
#include "AuthorHTML.h"
#include "TopicPropPage2.h"
#include "FileDialog.h"
#include "GeneralIDDlg.h"
#include "StyleSheetDlg.h"
#include "Misc.h"
#include "TagEventsDlg.h"

#define COLOR_DEFAULT_TEXTCOLOR   RGB(0,  0,  0  )     // Normal text color = BLACK
#define COLOR_DEFAULT_BACKGROUND  RGB(255,255,255)    // Body background = WHITE
#define COLOR_DEFAULT_LINK        RGB(0,  0,  255)     // Normal links    = BLUE
#define COLOR_DEFAULT_ALINK       RGB(255,0,  0  )     // Active links    = RED
#define COLOR_DEFAULT_VLINK       RGB(255,0,  255)     // Visited lnks    = FUCHSIA

// TopicPropPage2Dlg dialog

IMPLEMENT_DYNAMIC(TopicPropPage2Dlg, CDialog)
TopicPropPage2Dlg::TopicPropPage2Dlg(CWnd*    pParent
                                    ,HtmlBody body
                                    ,CString  base)
                 	:CDialog(TopicPropPage2Dlg::IDD, pParent)
                  ,m_body(body)
                  ,m_base(base)
{
}

TopicPropPage2Dlg::~TopicPropPage2Dlg()
{
}

void TopicPropPage2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
  DDX_Text   (pDX,IDC_BODY_BACKGROUND, m_backgroundImage);
  DDX_Text   (pDX,IDC_BODY_TOPBOTTOM,  m_marginTop);
  DDX_Text   (pDX,IDC_BODY_LEFTRIGHT,  m_marginLeft);
  DDX_Control(pDX,IDC_SPIN1,           m_spinTop);
  DDX_Control(pDX,IDC_SPIN2,           m_spinLeft);
  DDX_Control(pDX,IDC_BUTTON_IMAGE,    m_buttonImage);
  DDX_Control(pDX,IDC_SCROLL,          m_comboScroll);
  DDX_Control(pDX,IDC_NOWRAP,          m_buttonNoWrap);

  DDX_Control(pDX,IDC_BODY_TEXTCOLOR,  m_buttonColor);
  DDX_Control(pDX,IDC_BODY_COLOR,      m_buttonBackColor);
  DDX_Control(pDX,IDC_BODY_FIXED,      m_buttonFixed);
  DDX_Control(pDX,IDC_BODY_HYPER,      m_buttonHyperlink);
  DDX_Control(pDX,IDC_BODY_VISITED,    m_buttonVisited);
  DDX_Control(pDX,IDC_BODY_ACTIVE,     m_buttonActive);

  DDX_Control(pDX,IDC_DO_TC,           m_buttonTC);
  DDX_Control(pDX,IDC_DO_BC,           m_buttonBC);
  DDX_Control(pDX,IDC_DO_LC,           m_buttonLC);
  DDX_Control(pDX,IDC_DO_VC,           m_buttonVC);
  DDX_Control(pDX,IDC_DO_AC,           m_buttonAC);
  DDX_Control(pDX,IDC_DO_LR,           m_buttonLR);
  DDX_Control(pDX,IDC_DO_TB,           m_buttonTB);
  DDX_Control(pDX,IDC_DO_SCROLL,       m_buttonSC);

  DDX_Control(pDX,IDC_ID,              m_buttonID);
  DDX_Control(pDX,IDC_EVENTS,          m_buttonEvents);
  DDX_Control(pDX,IDC_BODY_STYLE,      m_buttonStyle);

  if(pDX->m_bSaveAndValidate == Data2Controls)
  {
    CString def,text;

    m_buttonID.GetWindowText(def);
    text = m_body.HasIdentity() ? "[ &ID ]" : "&ID";
    if(def != text) m_buttonID.SetWindowText(text);

    m_buttonStyle.GetWindowText(def);
    text = m_body.HasStyle() ? "[ &Style ]" : "&Style";
    if(def != text) m_buttonStyle.SetWindowText(text);

    m_buttonEvents.GetWindowText(def);
    text = m_body.HasEvents() ? "[ &Events ]" : "&Events";
    if(def != text) m_buttonEvents.SetWindowText(text);

    m_buttonColor    .EnableWindow(m_doTextColor);
    m_buttonBackColor.EnableWindow(m_doBackColor);
    m_buttonHyperlink.EnableWindow(m_doLinkColor);
    m_buttonVisited  .EnableWindow(m_doVisitColor);
    m_buttonActive   .EnableWindow(m_doActivColor);

    m_comboScroll.EnableWindow(m_doScroll);
    CWnd* w = GetDlgItem(IDC_BODY_TOPBOTTOM);
    w->EnableWindow(m_doTopBottom);
    w = GetDlgItem(IDC_BODY_LEFTRIGHT);
    w->EnableWindow(m_doRightLeft);

    int ind = m_comboScroll.FindString(0,m_scroll);
    m_comboScroll.SetCurSel(ind);

    m_buttonNoWrap.SetCheck(m_nowrap);
  }
}

BEGIN_MESSAGE_MAP(TopicPropPage2Dlg, CDialog)
  ON_EN_CHANGE      (IDC_BODY_BACKGROUND, OnEnChangeBackground)
  ON_BN_CLICKED     (IDC_BUTTON_IMAGE,    OnBnClickedButtonImage)
  ON_BN_CLICKED     (IDC_BODY_FIXED,      OnBnClickedBodyFixed)
  ON_BN_CLICKED     (IDC_BODY_TEXTCOLOR,  OnBnClikcedBodyTextColor)
  ON_BN_CLICKED     (IDC_BODY_COLOR,      OnBnClickedBodyColor)
  ON_BN_CLICKED     (IDC_BODY_HYPER,      OnBnClickedBodyHyper)
  ON_BN_CLICKED     (IDC_BODY_VISITED,    OnBnClickedBodyVisited)
  ON_BN_CLICKED     (IDC_BODY_ACTIVE,     OnBnClickedBodyActive)
  ON_BN_CLICKED     (IDC_DO_TC,           OnBnClickedDoTc)
  ON_BN_CLICKED     (IDC_DO_BC,           OnBnClickedDoBc)
  ON_BN_CLICKED     (IDC_DO_LC,           OnBnClickedDoLc)
  ON_BN_CLICKED     (IDC_DO_VC,           OnBnClickedDoVc)
  ON_BN_CLICKED     (IDC_DO_AC,           OnBnClickedDoAc)
  ON_BN_CLICKED     (IDC_DO_TB,           OnBnClickedDoTB)
  ON_BN_CLICKED     (IDC_DO_LR,           OnBnClickedDoLR)
  ON_BN_CLICKED     (IDC_DO_SCROLL,       OnBnClickedDoScroll)
  ON_EN_KILLFOCUS   (IDC_BODY_TOPBOTTOM,  OnEnChangeBodyTopbottom)
  ON_EN_KILLFOCUS   (IDC_BODY_LEFTRIGHT,  OnEnChangeBodyLeftright)
  ON_BN_CLICKED     (IDC_ID,              OnBnClickedId)
  ON_BN_CLICKED     (IDC_EVENTS,          OnBnClickedEvents)
  ON_BN_CLICKED     (IDC_BODY_STYLE,      OnBnClickedBodyStyle)
  ON_CBN_KILLFOCUS  (IDC_SCROLL,          OnCbnSelchangeScroll)
  ON_BN_CLICKED     (IDC_NOWRAP,          OnBnClickedNowrap)
END_MESSAGE_MAP()

BOOL
TopicPropPage2Dlg::OnInitDialog()
{
  CDialog::OnInitDialog();
  FillPage();
  m_spinTop .SetBase(10);
  m_spinLeft.SetBase(10);
  m_spinTop .SetRange(0,32000);
  m_spinLeft.SetRange(0,32000);
  m_buttonFixed.SetCheck(m_backgroundFixed);

  m_findAlpha.LoadBitmap(MAKEINTRESOURCE(IDB_ALPHA_FIND));
  m_buttonImage.SetBitmap(m_findAlpha);

  m_buttonColor    .SetColor(COLOR_DEFAULT_TEXTCOLOR);
  m_buttonBackColor.SetColor(COLOR_DEFAULT_BACKGROUND);
  m_buttonHyperlink.SetColor(COLOR_DEFAULT_LINK);
  m_buttonVisited  .SetColor(COLOR_DEFAULT_VLINK);
  m_buttonActive   .SetColor(COLOR_DEFAULT_ALINK);

  m_buttonColor    .EnableOtherButton(_T("More colors..."));
  m_buttonBackColor.EnableOtherButton(_T("More colors..."));
  m_buttonHyperlink.EnableOtherButton(_T("More colors..."));
  m_buttonVisited  .EnableOtherButton(_T("More colors..."));
  m_buttonActive   .EnableOtherButton(_T("More colors..."));

  UpdateData(Data2Controls);

//   m_buttonImage.SetXButtonStyle(BS_XT_SHOWFOCUS | BS_XT_HILITEPRESSED | BS_XT_WINXP_COMPAT);
//   m_buttonImage.SetTheme(xtThemeOffice2003);
  return TRUE;
}

void
TopicPropPage2Dlg::FillPage()
{
  m_nowrap           = m_body.GetNoWrap();
  m_backgroundImage  = m_body.GetProperty(HtmlBody::E_Background);
  m_textColor        = m_body.GetProperty(HtmlBody::E_TextColor);
  m_backgroundColor  = m_body.GetProperty(HtmlBody::E_BgColor);
  m_linkColor        = m_body.GetProperty(HtmlBody::E_Link);
  m_linkVisitedColor = m_body.GetProperty(HtmlBody::E_Vlink);
  m_linkActiveColor  = m_body.GetProperty(HtmlBody::E_Alink);
  m_marginTop        = m_body.GetProperty(HtmlBody::E_TopMargin);
  m_marginLeft       = m_body.GetProperty(HtmlBody::E_LeftMargin);
  m_scroll           = m_body.GetProperty(HtmlBody::E_Scroll);
  CString prop       = m_body.GetProperty(HtmlBody::E_BgProperties);
  m_backgroundFixed  = false;
  if(prop.CompareNoCase("fixed") == 0)
  {
    m_backgroundFixed = true;
  }
  m_doScroll = !m_scroll.IsEmpty();
  m_doTopBottom = !m_marginTop.IsEmpty();
  m_doRightLeft = !m_marginLeft.IsEmpty();

  int red,green,blue;
  if(m_textColor.IsEmpty())
  {
    m_doTextColor = false;
  }
  else
  {
    m_doTextColor = true;
    Misc::DecodeColor(m_textColor,red,green,blue);
    m_buttonColor.SetColor(RGB(red,green,blue));
  }
  if(m_backgroundColor.IsEmpty())
  {
    m_doBackColor = false;
  }
  else
  {
    m_doBackColor = true;
    Misc::DecodeColor(m_backgroundColor,red,green,blue);
    m_buttonBackColor.SetColor(RGB(red,green,blue));
  }
  if(m_linkColor.IsEmpty())
  {
    m_doLinkColor = false;
  }
  else
  {
    m_doLinkColor = true;
    Misc::DecodeColor(m_linkColor,red,green,blue);
    m_buttonHyperlink.SetColor(RGB(red,green,blue));
  }
  if(m_linkVisitedColor.IsEmpty())
  {
    m_doVisitColor = false;
  }
  else
  {
    m_doVisitColor = true;
    Misc::DecodeColor(m_linkVisitedColor,red,green,blue);
    m_buttonVisited.SetColor(RGB(red,green,blue));
  }
  if(m_linkActiveColor.IsEmpty())
  {
    m_doActivColor = false;
  }
  else
  {
    m_doActivColor = true;
    Misc::DecodeColor(m_linkActiveColor,red,green,blue);
    m_buttonActive.SetColor(RGB(red,green,blue));
  }
  m_buttonTC.SetCheck(m_doTextColor);
  m_buttonBC.SetCheck(m_doBackColor);
  m_buttonLC.SetCheck(m_doLinkColor);
  m_buttonVC.SetCheck(m_doVisitColor);
  m_buttonAC.SetCheck(m_doActivColor);
  m_buttonTB.SetCheck(m_doTopBottom);
  m_buttonLR.SetCheck(m_doRightLeft);
  m_buttonSC.SetCheck(m_doScroll);
}

void
TopicPropPage2Dlg::UpdateProperties()
{
  int red,green,blue;

  CString fixed = m_backgroundFixed ? "fixed" : "";
  m_body.SetProperty(HtmlBody::E_BgProperties,fixed);
  m_body.SetProperty(HtmlBody::E_Background,m_backgroundImage);
  m_body.SetNoWrap(m_nowrap);

  // It makes no sense to have a dialog with 4 margin values as:
  // By default. IE sets the left/right margin to the same values
  // By default. IE sets the top/bottom margin to the same values
  if(!m_doTopBottom)
  {
    m_marginTop = "";
  }
  m_body.SetProperty(HtmlBody::E_TopMargin,   m_marginTop);
  m_body.SetProperty(HtmlBody::E_BottomMargin,m_marginTop);
  if(!m_doRightLeft)
  {
    m_marginLeft = "";
  }
  m_body.SetProperty(HtmlBody::E_LeftMargin,  m_marginLeft);
  m_body.SetProperty(HtmlBody::E_RightMargin, m_marginLeft);
  
  if(!m_doScroll)
  {
    m_scroll = "";
  }
  m_body.SetProperty(HtmlBody::E_Scroll,m_scroll);

  if(m_doTextColor)
  {
    if(!m_textColor.IsEmpty())
    {
      Misc::DecodeColor(m_textColor,red,green,blue);
      m_textColor.Format("#%02x%02x%02x",red,green,blue);
    }
  }
  else 
  {
    m_textColor = "";
  }
  m_body.SetProperty(HtmlBody::E_TextColor,m_textColor);

  if(m_doBackColor)
  {
    if(!m_backgroundColor.IsEmpty())
    {
      Misc::DecodeColor(m_backgroundColor,red,green,blue);
      m_backgroundColor.Format("#%02x%02x%02x",red,green,blue);
    }
  }
  else
  {
    m_backgroundColor = "";
  }
  m_body.SetProperty(HtmlBody::E_BgColor,m_backgroundColor);

  if(m_doLinkColor)
  {
    if(!m_linkColor.IsEmpty())
    {
      Misc::DecodeColor(m_linkColor,red,green,blue);
      m_linkColor.Format("#%02x%02x%02x",red,green,blue);
    }
  }
  else
  {
    m_linkColor = "";
  }
  m_body.SetProperty(HtmlBody::E_Link,m_linkColor);

  if(m_doVisitColor)
  {
    if(!m_linkVisitedColor.IsEmpty())
    {
      Misc::DecodeColor(m_linkVisitedColor,red,green,blue);
      m_linkVisitedColor.Format("#%02x%02x%02x",red,green,blue);
    }
  }
  else
  {
    m_linkVisitedColor = "";
  }
  m_body.SetProperty(HtmlBody::E_Vlink,m_linkVisitedColor);

  if(m_doActivColor)
  {
    if(!m_linkActiveColor.IsEmpty())
    {
      Misc::DecodeColor(m_linkActiveColor,red,green,blue);
      m_linkActiveColor.Format("#%02x%02x%02x",red,green,blue);
    }
  }
  else
  {
    m_linkActiveColor = "";
  }
  m_body.SetProperty(HtmlBody::E_Alink,m_linkActiveColor);
}

// TopicPropPage2Dlg message handlers

void
TopicPropPage2Dlg::OnEnChangeBackground()
{
  UpdateData(Controls2Data);
}

void 
TopicPropPage2Dlg::OnBnClickedButtonImage()
{
  DocFileDialog diag(true               // true = open
                    ,"Select an image"  // title
                    ,""                 // Extension
                    ,""                 // Default file
                    ,0                  // flags
                    ,"All images (jpg,gif,bmp,png)|*.jpg;*.jpeg;*.gif;*.bmp|"
                     "Joint Photogroup files (jpg)|*.jpg;*.jpeg|"
                     "Graphics Information File (gif)|*.gif|"
                     "Portable Network Graphics (png)|*.png|"
                     "Windows bitmaps (bmp)|*.bmp|"
                     "All files|*.*");
  if(diag.DoModal() == IDOK)
  {
    CString file = diag.GetChosenFile();
    CString relative;
    if(Misc::MakeRelativePathname(m_base,file,relative))
    {
      file = relative;
    }
    m_backgroundImage = file;
    UpdateData(Data2Controls);
  }
}

void 
TopicPropPage2Dlg::OnBnClikcedBodyTextColor()
{
  // Show the fully opened Color dialog with red as the selected color.
  COLORREF col = m_buttonColor.GetColor();
  int red   = GetRValue(col);
  int green = GetGValue(col);
  int blue  = GetBValue(col);
  m_textColor.Format("#%02x%02x%02x",red,green,blue);
}

void 
TopicPropPage2Dlg::OnBnClickedBodyColor()
{
  // Show the fully opened Color dialog with red as the selected color.
  COLORREF col = m_buttonBackColor.GetColor();
  int red   = GetRValue(col);
  int green = GetGValue(col);
  int blue  = GetBValue(col);
  m_backgroundColor.Format("#%02x%02x%02x",red,green,blue);
}

void 
TopicPropPage2Dlg::OnBnClickedBodyHyper()
{
  COLORREF col = m_buttonHyperlink.GetColor();
  int red   = GetRValue(col);
  int green = GetGValue(col);
  int blue  = GetBValue(col);
  m_linkColor.Format("#%02x%02x%02x",red,green,blue);
}

void 
TopicPropPage2Dlg::OnBnClickedBodyVisited()
{
  COLORREF col = m_buttonVisited.GetColor();
  int red   = GetRValue(col);
  int green = GetGValue(col);
  int blue  = GetBValue(col);
  m_linkVisitedColor.Format("#%02x%02x%02x",red,green,blue);
}

void
TopicPropPage2Dlg::OnBnClickedBodyActive()
{
  COLORREF col = m_buttonActive.GetColor();
  int red   = GetRValue(col);
  int green = GetGValue(col);
  int blue  = GetBValue(col);
  m_linkActiveColor.Format("#%02x%02x%02x",red,green,blue);
}

void 
TopicPropPage2Dlg::OnBnClickedBodyFixed()
{
  m_backgroundFixed = m_buttonFixed.GetCheck() ? true : false;
}

void 
TopicPropPage2Dlg::OnBnClickedId()
{
  HtmlElement* elem = (HtmlElement*) &m_body;
  GeneralIDDlg dlg(this,"body",elem);
  dlg.DoModal();
}

void 
TopicPropPage2Dlg::OnBnClickedEvents()
{
  HtmlElement* elem = (HtmlElement*) &m_body;
  TagEventsDlg dlg(this,elem,"body");
  dlg.DoModal();
  UpdateData(Data2Controls);
}

void 
TopicPropPage2Dlg::OnBnClickedBodyStyle()
{
  if(!m_body.HasStyle())
  {
    m_body.SetStyle();
  }
  CString style = m_body.GetInlineStyle();
  style = "body { " + style + "}";
  StyleSheetDlg dlg(this,m_base,"body",NULL,style);
  if(dlg.DoModal() == IDOK)
  {
    style = dlg.GetInlineStylesheet();
    style.TrimRight("}");
    style.TrimLeft("body {");
    m_body.SetInlineStyle(style);
    FillPage();
  }
  UpdateData(Data2Controls);
}

void 
TopicPropPage2Dlg::OnEnChangeBodyTopbottom()
{
  CWnd* w = GetDlgItem(IDC_BODY_TOPBOTTOM);
  w->GetWindowText(m_marginTop);
}

void 
TopicPropPage2Dlg::OnEnChangeBodyLeftright()
{
  CWnd* w = GetDlgItem(IDC_BODY_LEFTRIGHT);
  w->GetWindowText(m_marginLeft);
}

void 
TopicPropPage2Dlg::OnBnClickedDoTc()
{
  m_doTextColor = m_buttonTC.GetCheck() == TRUE;
  UpdateData(Data2Controls);
}

void 
TopicPropPage2Dlg::OnBnClickedDoBc()
{
  m_doBackColor = m_buttonBC.GetCheck() == TRUE;
  UpdateData(Data2Controls);
}

void 
TopicPropPage2Dlg::OnBnClickedDoLc()
{
  m_doLinkColor = m_buttonLC.GetCheck() == TRUE;
  UpdateData(Data2Controls);
}

void 
TopicPropPage2Dlg::OnBnClickedDoVc()
{
  m_doVisitColor = m_buttonVC.GetCheck() == TRUE;
  UpdateData(Data2Controls);
}

void 
TopicPropPage2Dlg::OnBnClickedDoAc()
{
  m_doActivColor = m_buttonAC.GetCheck() == TRUE;
  UpdateData(Data2Controls);
}

void 
TopicPropPage2Dlg::OnCbnSelchangeScroll()
{
  int ind = m_comboScroll.GetCurSel();
  if(ind >= 0)
  {
    m_comboScroll.GetLBText(ind,m_scroll);
  }
}

void 
TopicPropPage2Dlg::OnBnClickedDoLR()
{
  m_doRightLeft = m_buttonLR.GetCheck() == TRUE;
  UpdateData(Data2Controls);
}

void 
TopicPropPage2Dlg::OnBnClickedDoTB()
{
  m_doTopBottom= m_buttonTB.GetCheck() == TRUE;
  UpdateData(Data2Controls);
}

void 
TopicPropPage2Dlg::OnBnClickedDoScroll()
{
  m_doScroll = m_buttonSC.GetCheck() == TRUE;
  UpdateData(Data2Controls);
}

void 
TopicPropPage2Dlg::OnBnClickedNowrap()
{
  m_nowrap = m_buttonNoWrap.GetCheck() == TRUE;
}
