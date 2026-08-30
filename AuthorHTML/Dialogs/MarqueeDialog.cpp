//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    MarqueeDialog.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for managing the <MARQUEE> tag in HTML
//
#include "stdafx.h"
#include "AuthorHTML.h"
#include "MarqueeDialog.h"
#include "TagEventsDlg.h"
#include "GeneralIDDlg.h"
#include "StyleSheetDlg.h"
#include "HtmlElement.h"

// MarqueeDialog dialog

IMPLEMENT_DYNAMIC(MarqueeDialog, CDialog)

MarqueeDialog::MarqueeDialog(CWnd* pParent
                            ,CComPtr<IHTMLMarqueeElement> pMarquee
                            ,CString base)
             	:CDialog(MarqueeDialog::IDD, pParent)
              ,m_marquee(pMarquee)
              ,m_base(base)
{
}

MarqueeDialog::~MarqueeDialog()
{
}

void MarqueeDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

  DDX_Control(pDX,IDC_RADIO_LR,     m_buttonLeft);
  DDX_Control(pDX,IDC_RADIO_RL,     m_buttonRight);
  DDX_Control(pDX,IDC_RADIO_UP,     m_buttonUp);
  DDX_Control(pDX,IDC_RADIO_DOWN,   m_buttonDown);
  DDX_Text   (pDX,IDC_MARQ_AMOUNT,  m_speedAmount);
  DDX_Text   (pDX,IDC_MARQ_DELAY,   m_speedDelay);
  DDX_Control(pDX,IDC_SPIN1,        m_spinAmount);
  DDX_Control(pDX,IDC_SPIN2,        m_spinDelay);
  DDX_Control(pDX,IDC_RADIO_SCROLL, m_buttonScroll);
  DDX_Control(pDX,IDC_RADIO_SLIDE,  m_buttonSlide);
  DDX_Control(pDX,IDC_RADIO_ALTERNATE, m_buttonAlternate);
  DDX_Control(pDX,IDC_REPEAT_CONT,  m_buttonContinuously);
  DDX_Text   (pDX,IDC_REPEAT_TIMES, m_repeatTimes);
  DDX_Control(pDX,IDC_SPIN3,        m_spinTimes);
  DDX_Control(pDX,IDC_MARQ_ASPECT,  m_buttonAspect);
  DDX_Control(pDX,IDC_MARQ_USEW,    m_buttonUseWidth);
  DDX_Control(pDX,IDC_MARQ_USEH,    m_buttonUseHeight);
  DDX_Text   (pDX,IDC_MARQ_WIDTH,   m_preferredWidth);
  DDX_Text   (pDX,IDC_MARQ_HEIGHT,  m_preferredHeight);
  DDX_Control(pDX,IDC_SPIN4,        m_spinWidth);
  DDX_Control(pDX,IDC_SPIN5,        m_spinHeight);
  DDX_Control(pDX,IDOK,             m_buttonOK);
  DDX_Control(pDX,IDCANCEL,         m_buttonCancel);
  DDX_Control(pDX,IDC_MARQUEE_APPLY,m_buttonApply);
  DDX_Control(pDX,IDC_MARQUEE_HELP, m_buttonHelp);
  DDX_Control(pDX,IDC_ID,           m_buttonID);
  DDX_Control(pDX,IDC_EVENTS,       m_buttonEvents);
  DDX_Control(pDX,IDC_STYLE,        m_buttonStyle);

  if(pDX->m_bSaveAndValidate == Data2Controls)
  {
    CString def,text;
    CComQIPtr<IHTMLElement,&IID_IHTMLElement> pElem = m_marquee;
    HtmlElement elem(pElem);

    m_buttonID.GetWindowText(def);
    text = elem.HasIdentity() ? "[ &ID ]" : "&ID";
    if(def != text) m_buttonID.SetWindowText(text);

    m_buttonStyle.GetWindowText(def);
    text = elem.HasStyle() ? "[ &Style ]" : "&Style";
    if(def != text) m_buttonStyle.SetWindowText(text);

    m_buttonEvents.GetWindowText(def);
    text = elem.HasEvents() ? "[ &Events ]" : "&Events";
    if(def != text) m_buttonEvents.SetWindowText(text);
  }
}

BEGIN_MESSAGE_MAP(MarqueeDialog, CDialog)
  ON_BN_CLICKED(IDC_RADIO_LR,        OnBnClickedRadioLr)
  ON_BN_CLICKED(IDC_RADIO_RL,        OnBnClickedRadioRl)
  ON_BN_CLICKED(IDC_RADIO_UP,        OnBnClickedRadioUp)
  ON_BN_CLICKED(IDC_RADIO_DOWN,      OnBnClickedRadioDown)
  ON_BN_CLICKED(IDC_RADIO_SCROLL,    OnBnClickedRadioScroll)
  ON_BN_CLICKED(IDC_RADIO_SLIDE,     OnBnClickedRadioSlide)
  ON_BN_CLICKED(IDC_RADIO_ALTERNATE, OnBnClickedRadioAlternate)
  ON_BN_CLICKED(IDC_REPEAT_CONT,     OnBnClickedRepeatCont)
  ON_BN_CLICKED(IDC_MARQ_USEW,       OnBnClickedMarqUsew)
  ON_BN_CLICKED(IDC_MARQ_USEH,       OnBnClickedMarqUseh)
  ON_BN_CLICKED(IDC_ID,              OnBnClickedId)
  ON_BN_CLICKED(IDC_EVENTS,          OnBnClickedEvents)
  ON_BN_CLICKED(IDC_STYLE,           OnBnClickedStyle)
  ON_BN_CLICKED(IDOK,                OnBnClickedOk)
  ON_BN_CLICKED(IDC_MARQUEE_APPLY,   OnBnClickedMarqueeApply)
END_MESSAGE_MAP()

BOOL
MarqueeDialog::OnInitDialog()
{
  CDialog::OnInitDialog();

  // DIRECTION
  CComBSTR bDirection;
  m_marquee->get_direction(&bDirection);
  m_direction = bDirection;

       if(m_direction.CompareNoCase("left")  == 0) m_buttonLeft .SetCheck(TRUE);
  else if(m_direction.CompareNoCase("right") == 0) m_buttonRight.SetCheck(TRUE);
  else if(m_direction.CompareNoCase("up")    == 0) m_buttonUp   .SetCheck(TRUE);
  else if(m_direction.CompareNoCase("down")  == 0) m_buttonDown .SetCheck(TRUE);
  else
  {
    m_direction = "Left";
    m_buttonLeft.SetCheck(TRUE);
  }

  // SPEED AMOUNT
  m_marquee->get_scrollAmount(&m_speedAmount);
  m_spinAmount.SetBase(10);
  m_spinAmount.SetRange(0,32000);
  m_spinAmount.SetPos(m_speedAmount);

  // SPEED DELAY
  m_marquee->get_scrollDelay(&m_speedDelay);
  m_spinDelay.SetBase(10);
  m_spinDelay.SetRange(0,32000);
  m_spinDelay.SetPos(m_speedDelay);

  // BEHAVIOUR
  CComBSTR bBehaviour;
  m_marquee->get_behavior(&bBehaviour);
  m_behaviour = bBehaviour;
       if(m_behaviour.CompareNoCase("scroll")    == 0) m_buttonScroll.SetCheck(TRUE);
  else if(m_behaviour.CompareNoCase("slide")     == 0) m_buttonSlide .SetCheck(TRUE);
  else if(m_behaviour.CompareNoCase("alternate") == 0) m_buttonAlternate.SetCheck(TRUE);
  else 
  {
    m_behaviour = "Scroll";
    m_buttonScroll.SetCheck(TRUE);
  }

  // REPEAT CONTINUOUSLY / TIMES
  m_marquee->get_loop(&m_repeatTimes);
  if(m_repeatTimes <= 0)
  {
    m_buttonContinuously.SetCheck(TRUE);
    m_spinTimes.EnableWindow(FALSE);
    CWnd* times = GetDlgItem(IDC_REPEAT_TIMES);
    times->EnableWindow(FALSE);
  }
  m_spinTimes.SetBase(10);
  m_spinTimes.SetRange(0,32000);
  m_spinTimes.SetPos(m_repeatTimes);

  // ASPECT RATIO

  // PREFERRED WIDTH
  CComVariant width;
  V_VT(&width) = VT_BSTR;
  m_marquee->get_width(&width);
  m_preferredWidth = V_BSTR(&width);
  long wid = atoi(m_preferredWidth);
  if(wid)
  {
    m_buttonUseWidth.SetCheck(TRUE);
  }
  else
  {
    m_spinWidth.EnableWindow(FALSE);
    CWnd* w = GetDlgItem(IDC_MARQ_WIDTH);
    w->EnableWindow(FALSE);
  }
  m_spinWidth.SetBase(10);
  m_spinWidth.SetRange(0,32000);
  m_spinWidth.SetPos(wid);

  // PREFERRED HEIGHT
  CComVariant height;
  V_VT(&height) = VT_BSTR;
  m_marquee->get_height(&height);
  m_preferredHeight = V_BSTR(&height);
  long high = atoi(m_preferredHeight);
  if(high)
  {
    m_buttonUseHeight.SetCheck(TRUE);
  }
  else
  {
    m_spinHeight.EnableWindow(FALSE);
    CWnd* h = GetDlgItem(IDC_MARQ_HEIGHT);
    h->EnableWindow(FALSE);
  }
  m_spinHeight.SetBase(10);
  m_spinHeight.SetRange(0,32000);
  m_spinHeight.SetPos(high);

  return TRUE;
}

void 
MarqueeDialog::UpdateDocument()
{
  // SYNCHRONIZE
  UpdateData(Controls2Data);

  // DIRECTION
  CComBSTR bDirection = m_direction;
  HRESULT hr = m_marquee->put_direction(bDirection);

  // SPEED AMOUNT/DELAY
  hr = m_marquee->put_scrollAmount(m_speedAmount);
  hr = m_marquee->put_scrollDelay (m_speedDelay);

  // BEHAVIOUR
  CComBSTR bBehaviour = m_behaviour;
  hr = m_marquee->put_behavior(bBehaviour);
  m_behaviour = bBehaviour;

  // REPEAT CONTINUOUSLY / TIMES
  hr = m_marquee->put_loop(m_repeatTimes);

  // PREFERRED WIDTH
  CComVariant width;
  V_VT(&width) = VT_I4;
  V_I4(&width) = atoi(m_preferredWidth);
  hr = m_marquee->put_width(width);

  // PREFERRED HEIGHT
  CComVariant height;
  V_VT(&height) = VT_I4;
  V_I4(&height) = atoi(m_preferredHeight);
  hr = m_marquee->put_height(height);
}

// MarqueeDialog message handlers

void 
MarqueeDialog::OnBnClickedRadioLr()
{
  if(m_buttonLeft.GetCheck())
  {
    m_direction = "Left";
  }
}

void 
MarqueeDialog::OnBnClickedRadioRl()
{
  if(m_buttonRight.GetCheck())
  {
    m_direction = "Right";
  }
}

void 
MarqueeDialog::OnBnClickedRadioUp()
{
  if(m_buttonUp.GetCheck())
  {
    m_direction = "Up";
  }
}

void 
MarqueeDialog::OnBnClickedRadioDown()
{
  if(m_buttonDown.GetCheck())
  {
    m_direction = "Down";
  }
}

void 
MarqueeDialog::OnBnClickedRadioScroll()
{
  if(m_buttonScroll.GetCheck())
  {
    m_behaviour = "Scroll";
  }
}

void 
MarqueeDialog::OnBnClickedRadioSlide()
{
  if(m_buttonSlide.GetCheck())
  {
    m_behaviour = "Slide";
  }
}


void 
MarqueeDialog::OnBnClickedRadioAlternate()
{
  if(m_buttonAlternate.GetCheck())
  {
    m_behaviour = "Alternate";
  }
}

void 
MarqueeDialog::OnBnClickedRepeatCont()
{
  CWnd* times = GetDlgItem(IDC_REPEAT_TIMES);
  if(m_buttonContinuously.GetCheck())
  {
    m_repeatTimes = 0;
    m_spinTimes.EnableWindow(FALSE);
    times->EnableWindow(FALSE);
  }
  else
  {
    m_repeatTimes = 1;
    m_spinTimes.EnableWindow(TRUE);
    times->EnableWindow(TRUE);
  }
  m_spinTimes.SetPos(m_repeatTimes);
}

void 
MarqueeDialog::OnBnClickedMarqUsew()
{
  CWnd* width = GetDlgItem(IDC_MARQ_WIDTH);
  if(m_buttonUseWidth.GetCheck())
  {
    m_spinWidth.EnableWindow(TRUE);
    width->EnableWindow(TRUE);
  }
  else
  {
    m_spinWidth.EnableWindow(FALSE);
    width->EnableWindow(FALSE);
    m_preferredWidth = "";
  }
  m_spinWidth.SetPos(atoi(m_preferredWidth));
}

void 
MarqueeDialog::OnBnClickedMarqUseh()
{
  CWnd* height = GetDlgItem(IDC_MARQ_HEIGHT);
  if(m_buttonUseHeight.GetCheck())
  {
    m_spinHeight.EnableWindow(TRUE);
    height->EnableWindow(TRUE);
  }
  else
  {
    m_spinHeight.EnableWindow(FALSE);
    height->EnableWindow(FALSE);
    m_preferredHeight = "";
  }
  m_spinHeight.SetPos(atoi(m_preferredHeight));
}

void MarqueeDialog::OnBnClickedOk()
{
  UpdateDocument();
  OnOK();
}

void MarqueeDialog::OnBnClickedMarqueeApply()
{
  UpdateDocument();
}

void MarqueeDialog::OnBnClickedId()
{
  CComQIPtr<IHTMLElement,&IID_IHTMLElement> pElem = m_marquee;
  HtmlElement elem(pElem);
  GeneralIDDlg dlg(this,"marquee",&elem);
  dlg.DoModal();
}

void MarqueeDialog::OnBnClickedEvents()
{
  CComQIPtr<IHTMLElement,&IID_IHTMLElement> pElem = m_marquee;
  HtmlElement elem(pElem);
  TagEventsDlg dlg(this,&elem,"marquee");
  dlg.DoModal();
  UpdateData(Data2Controls);
}

void MarqueeDialog::OnBnClickedStyle()
{
  CComQIPtr<IHTMLElement,&IID_IHTMLElement> pElem = m_marquee;
  HtmlElement elem(pElem);
  if(!elem.HasStyle())
  {
    elem.SetStyle();
  }
  CString style = elem.GetInlineStyle();
  style = CString("marquee { ") + style + "}";
  StyleSheetDlg dlg(this,m_base,"marquee",NULL,style);
  if(dlg.DoModal() == IDOK)
  {
    style = dlg.GetInlineStylesheet();
    style.TrimRight("}");
    style.TrimLeft("a {");
    elem.SetInlineStyle(style);
  }
}
