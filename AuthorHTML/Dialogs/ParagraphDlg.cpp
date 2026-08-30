//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    ParagraphDlg.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for managing the <P> tag in HTML
//
#include "stdafx.h"
#include "Misc.h"
#include "AuthorHTML.h"
#include "ParagraphDlg.h"
#include "GeneralIDDlg.h"
#include "TagEventsDlg.h"

// ParagraphDlg dialog

IMPLEMENT_DYNAMIC(ParagraphDlg, CDialog)

ParagraphDlg::ParagraphDlg(CWnd* pParent
                          ,CString p_base
                          ,CssStyleSheet* pCss
                          ,CComPtr<IHTMLElement> pElem
                          ,CComPtr<IHTMLStyle> pStyle)
             :CDialog(ParagraphDlg::IDD, pParent)
             ,m_css(pCss)
             ,m_elem(pElem)
             ,m_style(pStyle)
             ,m_canApply(false)
             ,m_mySheet(false)
             ,m_base(p_base)
{
}

ParagraphDlg::~ParagraphDlg()
{
  if(m_mySheet)
  {
    delete m_css;
  }
}

void ParagraphDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
  DDX_Control(pDX,IDC_PARA_ALIGN, m_comboAlign);
  DDX_Text   (pDX,IDC_PARA_BT,    m_marginLeft);
  DDX_Text   (pDX,IDC_PARA_AT,    m_marginRight);
  DDX_Text   (pDX,IDC_PARA_SB,    m_marginTop);
  DDX_Text   (pDX,IDC_PARA_SA,    m_marginBottom);
  DDX_Control(pDX,IDC_PARA_BTU,   m_comboLeft);
  DDX_Control(pDX,IDC_PARA_ATU,   m_comboRight);
  DDX_Control(pDX,IDC_PARA_SBU,   m_comboTop);
  DDX_Control(pDX,IDC_PARA_SAU,   m_comboBottom);
  DDX_Control(pDX,IDC_SPIN_BT,    m_spinLeft);
  DDX_Control(pDX,IDC_SPIN_AT,    m_spinRight);
  DDX_Control(pDX,IDC_SPIN_SB,    m_spinTop);
  DDX_Control(pDX,IDC_SPIN_SA,    m_spinBottom);
  DDX_Control(pDX,IDC_PARA_LINESPACING, m_comboSpacing);
  DDX_Text   (pDX,IDC_PARA_INDENT,      m_textIndent);
  DDX_Control(pDX,IDC_SPIN_INDENT,      m_spinIndent);
  DDX_Control(pDX,IDC_PARA_INDENTU,     m_comboIndent);
  DDX_Text   (pDX,IDC_PARA_WS,    m_wordSpacing);
  DDX_Control(pDX,IDC_SPIN_WS,    m_spinWord);
  DDX_Control(pDX,IDC_PARA_WSU,   m_comboWord);
  DDX_Control(pDX,IDC_ID,         m_buttonID);
  DDX_Control(pDX,IDC_EVENTS,     m_buttonEvents);
  DDX_Control(pDX,IDC_STYLE,      m_buttonStyle);
  DDX_Control(pDX,IDOK,           m_buttonOK);
  DDX_Control(pDX,IDC_PARA_APPLY, m_buttonApply);
  DDX_Control(pDX,IDCANCEL,       m_buttonCancel);
  DDX_Control(pDX,IDC_PARA_HELP,  m_buttonHelp);

  if(pDX->m_bSaveAndValidate == Data2Controls)
  {
    int ind;
    ind = m_comboAlign.FindString(-1,m_align);
          m_comboAlign.SetCurSel(ind);
    ind = m_comboLeft.FindString(-1,m_marginLeftUnit);
          m_comboLeft.SetCurSel(ind);
    ind = m_comboRight.FindString(-1,m_marginRightUnit);
          m_comboRight.SetCurSel(ind);
    ind = m_comboTop.FindString(-1,m_marginTopUnit);
          m_comboTop.SetCurSel(ind);
    ind = m_comboBottom.FindString(-1,m_marginBottomUnit);
          m_comboBottom.SetCurSel(ind);
    ind = m_comboIndent.FindString(-1,m_textIndentUnit);
          m_comboIndent.SetCurSel(ind);
    ind = m_comboWord.FindString(-1,m_wordSpacingUnit);
          m_comboWord.SetCurSel(ind);

    CString desc = Misc::GetAttributeDisplayname("line-height",m_lineHeight);
    ind = m_comboSpacing.FindString(-1,desc);
    if(ind >= 0)
    {
      m_comboSpacing.SetCurSel(ind);
    }
    CString def,text;
    HtmlElement elem(m_elem);

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
  m_buttonApply.EnableWindow(m_canApply);
}

BEGIN_MESSAGE_MAP(ParagraphDlg, CDialog)
  ON_CBN_SELCHANGE(IDC_PARA_ALIGN,      OnCbnSelchangeParaAlign)
  ON_CBN_SELCHANGE(IDC_PARA_LINESPACING,OnCbnSelchangeParaLinespacing)
  ON_CBN_KILLFOCUS(IDC_PARA_LINESPACING,OnCbnSelchangeParaLinespacing)
  ON_EN_KILLFOCUS (IDC_PARA_BT,         OnEnChangeParaBt)
  ON_EN_KILLFOCUS (IDC_PARA_AT,         OnEnChangeParaAt)
  ON_EN_KILLFOCUS (IDC_PARA_SB,         OnEnChangeParaSb)
  ON_EN_KILLFOCUS (IDC_PARA_SA,         OnEnChangeParaSa)
  ON_EN_KILLFOCUS (IDC_PARA_INDENT,     OnEnChangeParaIndent)
  ON_EN_KILLFOCUS (IDC_PARA_WS,         OnEnChangeParaWs)
  ON_CBN_SELCHANGE(IDC_PARA_BTU,        OnCbnSelchangeParaBtu)
  ON_CBN_SELCHANGE(IDC_PARA_ATU,        OnCbnSelchangeParaAtu)
  ON_CBN_SELCHANGE(IDC_PARA_SBU,        OnCbnSelchangeParaSbu)
  ON_CBN_SELCHANGE(IDC_PARA_SAU,        OnCbnSelchangeParaSau)
  ON_CBN_SELCHANGE(IDC_PARA_INDENTU,    OnCbnSelchangeParaIndentu)
  ON_CBN_SELCHANGE(IDC_PARA_WSU,        OnCbnSelchangeParaWsu)
  ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_BT,  OnDeltaposSpinBt)
  ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_AT,  OnDeltaposSpinAt)
  ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_SB,  OnDeltaposSpinSb)
  ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_SA,  OnDeltaposSpinSa)
  ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_INDENT,OnDeltaposSpinIndent)
  ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_WS,  OnDeltaposSpinWs)
  ON_BN_CLICKED   (IDC_ID,              OnBnClickedId)
  ON_BN_CLICKED   (IDC_EVENTS,          OnBnClickedEvents)
  ON_BN_CLICKED   (IDC_STYLE,           OnBnClickedStyle)
  ON_BN_CLICKED   (IDOK,                OnBnClickedOk)
  ON_BN_CLICKED   (IDC_PARA_APPLY,      OnBnClickedParaApply)
  ON_BN_CLICKED   (IDCANCEL,            OnBnClickedCancel)
  ON_BN_CLICKED   (IDC_PARA_HELP,       OnBnClickedParaHelp)
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(ParagraphDlg, CDialog)
  ON_EVENT(ParagraphDlg, IDC_EXPLORER, 0x00000103, OnDocumentComplete, VTS_DISPATCH VTS_VARIANT)
END_EVENTSINK_MAP()

BOOL
ParagraphDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  // Start up our display preview of fonts
  CWnd *pWnd = GetDlgItem(IDC_EXPLORER);
  if(pWnd)
  {
    m_spBrowser = pWnd->GetControlUnknown();
  }
  FillPage();

  m_spinLeft.SetBase(10);
  m_spinLeft.SetRange(-32000,32000);
  m_spinLeft.SetPos(atoi(m_marginLeft));
  m_spinRight.SetBase(10);
  m_spinRight.SetRange(-32000,32000);
  m_spinRight.SetPos(atoi(m_marginRight));
  m_spinTop.SetBase(10);
  m_spinTop.SetRange(-32000,32000);
  m_spinTop.SetPos(atoi(m_marginTop));
  m_spinBottom.SetBase(10);
  m_spinBottom.SetRange(-32000,32000);
  m_spinBottom.SetPos(atoi(m_marginBottom));
  m_spinIndent.SetBase(10);
  m_spinIndent.SetRange(-32000,32000);
  m_spinIndent.SetPos(atoi(m_textIndent));
  m_spinWord.SetBase(10);
  m_spinWord.SetRange(-32000,32000);
  m_spinWord.SetPos(atoi(m_wordSpacing));

  Redisplay();
  return TRUE;
}

void
ParagraphDlg::FillPage()
{
  string selector = "p";
  m_align        = CSSPropertyGet(m_css,selector,"text-align",   true);
  m_marginLeft   = CSSPropertyGet(m_css,selector,"margin-left",  false);
  m_marginRight  = CSSPropertyGet(m_css,selector,"margin-right", false);
  m_marginTop    = CSSPropertyGet(m_css,selector,"margin-top",   false); 
  m_marginBottom = CSSPropertyGet(m_css,selector,"margin-bottom",false); 
  CssSplitValueUnits(m_marginLeft  ,m_marginLeft  ,m_marginLeftUnit);
  CssSplitValueUnits(m_marginRight ,m_marginRight ,m_marginRightUnit);
  CssSplitValueUnits(m_marginTop   ,m_marginTop   ,m_marginTopUnit);
  CssSplitValueUnits(m_marginBottom,m_marginBottom,m_marginBottomUnit);
  m_lineHeight   = CSSPropertyGet(m_css,selector,"line-height",false);
  m_textIndent   = CSSPropertyGet(m_css,selector,"text-indent",false);
  CssSplitValueUnits(m_textIndent,m_textIndent,m_textIndentUnit);
  m_wordSpacing  = CSSPropertyGet(m_css,selector,"word-spacing",false);
  CssSplitValueUnits(m_wordSpacing,m_wordSpacing,m_wordSpacingUnit);

  if(m_align.IsEmpty() || m_align.CompareNoCase("Unspecified") == 0)
  {
    HtmlElement elem(m_elem);
    m_align = elem.GetAttribute("align");
  }
}

void
ParagraphDlg::UpdateProperties()
{
  string selector = "p";
  CString left   = m_marginLeft   + m_marginLeftUnit;
  CString right  = m_marginRight  + m_marginRightUnit;
  CString top    = m_marginTop    + m_marginTopUnit;
  CString bottom = m_marginBottom + m_marginBottomUnit;
  CString indent = m_textIndent   + m_textIndentUnit;
  CString word   = m_wordSpacing  + m_wordSpacingUnit;
  CSSPropertyPut(m_css,selector,"margin-left",  left,   false);
  CSSPropertyPut(m_css,selector,"margin-right", right,  false);
  CSSPropertyPut(m_css,selector,"margin-top",   top,    false);
  CSSPropertyPut(m_css,selector,"margin-bottom",bottom, false);
  CSSPropertyPut(m_css,selector,"line-height",  m_lineHeight,false);
  CSSPropertyPut(m_css,selector,"text-indent",  indent, false);
  CSSPropertyPut(m_css,selector,"word-spacing", word,   false);
  CSSPropertyPut(m_css,selector,"text-align",   m_align,true);
  if(!m_align.IsEmpty() && m_align.CompareNoCase("Unspecified"))
  {
    HtmlElement elem(m_elem);
    elem.SetAttribute("align",m_align);
  }
}

// ParagraphDlg message handlers
void 
ParagraphDlg::OnDocumentComplete(LPDISPATCH /*pDisp*/, LPVARIANT /*pURL*/)
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
    VARIANT findIndex;
    CComPtr<IDispatch> disp2;
    V_VT(&findIndex) = VT_I4;
    V_I4(&findIndex) = ind;
    hr = coll->item(findIndex,findIndex,&disp2);
    CComQIPtr<IHTMLParaElement,&IID_IHTMLParaElement> para = disp2;
    if(para.p)
    {
      CComQIPtr<IHTMLElement,&IID_IHTMLElement> elem = disp2;
      CComBSTR bID;
      hr = elem->get_id(&bID);
      if(SUCCEEDED(hr))
      {
        CString cID = CW2CT(bID);
        if(cID == "para")
        {
          CComPtr<IHTMLStyle> style;
          hr = elem->get_style(&style);
          if(SUCCEEDED(hr))
          {
            CComBSTR bString;
            // TEXT-ALIGN
            bString = CT2CW(m_align);
            style->put_textAlign(bString);
            // MARGIN-LEFT
            VARIANT value;
            bString = CT2CW(m_marginLeft + m_marginLeftUnit);
            V_VT(&value) = VT_BSTR;
            V_BSTR(&value) = bString;
            style->put_marginLeft(value);
            // MARGIN-RIGHT
            bString = CT2CW(m_marginRight + m_marginRightUnit);
            V_BSTR(&value) = bString;
            style->put_marginRight(value);
            // MARGIN-TOP
            bString = CT2CW(m_marginTop + m_marginTopUnit);
            V_BSTR(&value) = bString;
            style->put_marginTop(value);
            // MARGIN-BOTTOM
            bString = CT2CW(m_marginBottom + m_marginBottomUnit);
            V_BSTR(&value) = bString;
            style->put_marginBottom(value);
            // LINE-HEIGHT
            bString = CT2CW(m_lineHeight);
            V_BSTR(&value) = bString;
            style->put_lineHeight(value);
            // TEXT-IDENT
            bString = CT2CW(m_textIndent + m_textIndentUnit);
            V_BSTR(&value) = bString;
            style->put_textIndent(value);
            // WORD-SPACING
            bString = CT2CW(m_wordSpacing + m_wordSpacingUnit);
            V_BSTR(&value) = bString;
            style->put_wordSpacing(value);
          }
        }
      }
    }
  }
}

void 
ParagraphDlg::Redisplay()
{
  // Display all gotten properties
  UpdateData(Data2Controls);
  // (Re-)Load image page for display the text
  if(m_spBrowser)
  {
    m_spBrowser->Navigate(L"res://authorhtml.exe/paragraph.html", NULL, NULL, NULL, NULL);
  }
}

void 
ParagraphDlg::OnCbnSelchangeParaAlign()
{
  int ind = m_comboAlign.GetCurSel();
  if(ind >= 0)
  {
    m_comboAlign.GetLBText(ind,m_align);
    m_canApply = true;
  }
  UpdateProperties();
  Redisplay();
}

void 
ParagraphDlg::OnEnChangeParaBt()
{
  CWnd* w = GetDlgItem(IDC_PARA_BT);
  CString newVal;
  w->GetWindowText(newVal);
  if(m_marginLeft != newVal)
  {
    m_marginLeft = newVal;
    m_canApply = true;
  }
  if(!m_marginLeft.IsEmpty() && m_marginLeftUnit.IsEmpty())
  {
    m_marginLeftUnit = "px";
  }
  if(m_marginLeft.IsEmpty() && !m_marginLeftUnit.IsEmpty())
  {
    m_marginLeftUnit = "";
  }
  UpdateProperties();
  Redisplay();
}

void 
ParagraphDlg::OnEnChangeParaAt()
{
  CString value;
  CWnd* w = GetDlgItem(IDC_PARA_AT);
  w->GetWindowText(value);
  if(m_marginRight != value)
  {
    m_marginRight = value;
    m_canApply = true;
  }
  if(!m_marginRight.IsEmpty() && m_marginRightUnit.IsEmpty())
  {
    m_marginRightUnit = "px";
  }
  if(m_marginRight.IsEmpty() && !m_marginRightUnit.IsEmpty())
  {
    m_marginRightUnit = "";
  }
  UpdateProperties();
  Redisplay();
}

void 
ParagraphDlg::OnEnChangeParaSb()
{
  CString value;
  CWnd* w = GetDlgItem(IDC_PARA_SB);
  w->GetWindowText(value);
  if(m_marginTop != value)
  {
    m_marginTop = value;
    m_canApply = true;
  }
  if(!m_marginTop.IsEmpty() && m_marginTopUnit.IsEmpty())
  {
    m_marginTopUnit = "px";
  }
  if(m_marginTop.IsEmpty() && !m_marginTopUnit.IsEmpty())
  {
    m_marginTopUnit = "";
  }
  UpdateProperties();
  Redisplay();
}

void 
ParagraphDlg::OnEnChangeParaSa()
{
  CString value;
  CWnd* w = GetDlgItem(IDC_PARA_SA);
  w->GetWindowText(value);
  if(m_marginBottom != value)
  {
    m_marginBottom = value;
    m_canApply = true;
  }
  if(!m_marginBottom.IsEmpty() && m_marginBottomUnit.IsEmpty())
  {
    m_marginBottomUnit = "px";
  }
  if(m_marginBottom.IsEmpty() && !m_marginBottomUnit.IsEmpty())
  {
    m_marginBottomUnit = "";
  }
  UpdateProperties();
  Redisplay();
}

void 
ParagraphDlg::OnCbnSelchangeParaBtu()
{
  int ind = m_comboLeft.GetCurSel();
  if(ind >= 0)
  {
    m_comboLeft.GetLBText(ind,m_marginLeftUnit);
    m_canApply = true;
  }
  UpdateProperties();
  Redisplay();
}

void 
ParagraphDlg::OnCbnSelchangeParaAtu()
{
  int ind = m_comboRight.GetCurSel();
  if(ind >= 0)
  {
    m_comboRight.GetLBText(ind,m_marginRightUnit);
    m_canApply = true;
  }
  UpdateProperties();
  Redisplay();
}

void 
ParagraphDlg::OnCbnSelchangeParaSbu()
{
  int ind = m_comboTop.GetCurSel();
  if(ind >= 0)
  {
    m_comboTop.GetLBText(ind,m_marginTopUnit);
    m_canApply = true;
  }
  UpdateProperties();
  Redisplay();
}

void 
ParagraphDlg::OnCbnSelchangeParaSau()
{
  int ind = m_comboBottom.GetCurSel();
  if(ind >= 0)
  {
    m_comboBottom.GetLBText(ind,m_marginBottomUnit);
    m_canApply = true;
  }
  UpdateProperties();
  Redisplay();
}

void 
ParagraphDlg::OnDeltaposSpinBt(NMHDR *pNMHDR, LRESULT *pResult)
{
  LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
  *pResult = 0;
  CWnd* w = GetDlgItem(IDC_PARA_BT);
  w->GetWindowText(m_marginLeft);
  m_canApply = true;
  double total = atof(m_marginLeft);
  total += pNMUpDown->iDelta;
  m_marginLeft.Format("%f",total);
  m_marginLeft.TrimRight('0');
  m_marginLeft.TrimRight('.');
  w->SetWindowText(m_marginLeft);
  OnEnChangeParaBt();
}

void 
ParagraphDlg::OnDeltaposSpinAt(NMHDR *pNMHDR, LRESULT *pResult)
{
  LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
  *pResult = 0;
  CWnd* w = GetDlgItem(IDC_PARA_AT);
  w->GetWindowText(m_marginRight);
  m_canApply = true;
  double total = atof(m_marginRight);
  total += pNMUpDown->iDelta;
  m_marginRight.Format("%f",total);
  m_marginRight.TrimRight('0');
  m_marginRight.TrimRight('.');
  w->SetWindowText(m_marginRight);
  OnEnChangeParaAt();
}

void 
ParagraphDlg::OnDeltaposSpinSb(NMHDR *pNMHDR, LRESULT *pResult)
{
  LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
  *pResult = 0;
  CWnd* w = GetDlgItem(IDC_PARA_SB);
  w->GetWindowText(m_marginTop);
  m_canApply = true;
  double total = atof(m_marginTop);
  total += pNMUpDown->iDelta;
  m_marginTop.Format("%f",total);
  m_marginTop.TrimRight('0');
  m_marginTop.TrimRight('.');
  w->SetWindowText(m_marginTop);
  OnEnChangeParaSb();
}

void 
ParagraphDlg::OnDeltaposSpinSa(NMHDR *pNMHDR, LRESULT *pResult)
{
  LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
  *pResult = 0;
  CWnd* w = GetDlgItem(IDC_PARA_SA);
  w->GetWindowText(m_marginBottom);
  m_canApply = true;
  double total = atof(m_marginBottom);
  total += pNMUpDown->iDelta;
  m_marginBottom.Format("%f",total);
  m_marginBottom.TrimRight('0');
  m_marginBottom.TrimRight('.');
  w->SetWindowText(m_marginBottom);
  OnEnChangeParaSa();
}

void 
ParagraphDlg::OnCbnSelchangeParaLinespacing()
{
  CString desc,value;
  int ind = m_comboSpacing.GetCurSel();
  if(ind >= 0)
  {
    m_comboSpacing.GetLBText(ind,desc);
  }
  else
  {
    m_comboSpacing.GetWindowText(desc);
  }
  value = Misc::GetAttributeValue("line-height",desc);
  if(m_lineHeight != value)
  {
    m_lineHeight = value;
    m_canApply   = true;
  }
  UpdateProperties();
  Redisplay();
}

void 
ParagraphDlg::OnEnChangeParaIndent()
{
  CString value;
  CWnd* w = GetDlgItem(IDC_PARA_INDENT);
  w->GetWindowText(value);
  if(m_textIndent != value)
  {
    m_textIndent = value;
    m_canApply = true;
  }
  if(!m_textIndent.IsEmpty() && m_textIndentUnit.IsEmpty())
  {
    m_textIndentUnit = "px";
  }
  if(m_textIndent.IsEmpty() && !m_textIndentUnit.IsEmpty())
  {
    m_textIndentUnit = "";
  }
  UpdateProperties();
  Redisplay();
}

void 
ParagraphDlg::OnCbnSelchangeParaIndentu()
{
  int ind = m_comboIndent.GetCurSel();
  if(ind >= 0)
  {
    m_comboIndent.GetLBText(ind,m_textIndentUnit);
    m_canApply = true;
  }
  UpdateProperties();
  Redisplay();
}

void 
ParagraphDlg::OnDeltaposSpinIndent(NMHDR *pNMHDR, LRESULT *pResult)
{
  LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
  *pResult = 0;
  CWnd* w = GetDlgItem(IDC_PARA_INDENT);
  w->GetWindowText(m_textIndent);
  m_canApply = true;
  double total = atof(m_textIndent);
  total += pNMUpDown->iDelta;
  m_textIndent.Format("%f",total);
  m_textIndent.TrimRight('0');
  m_textIndent.TrimRight('.');
  w->SetWindowText(m_textIndent);
  OnEnChangeParaIndent();
}

void 
ParagraphDlg::OnEnChangeParaWs()
{
  CString value;
  CWnd* w = GetDlgItem(IDC_PARA_WS);
  w->GetWindowText(value);
  if(m_wordSpacing != value)
  {
    m_wordSpacing = value;
    m_canApply = true;
  }
  if(!m_wordSpacing.IsEmpty() && m_wordSpacingUnit.IsEmpty())
  {
    m_wordSpacingUnit = "px";
  }
  if(m_wordSpacing.IsEmpty() && !m_wordSpacingUnit.IsEmpty())
  {
    m_wordSpacingUnit= "";
  }
  UpdateProperties();
  Redisplay();
}

void 
ParagraphDlg::OnCbnSelchangeParaWsu()
{
  int ind = m_comboWord.GetCurSel();
  if(ind >= 0)
  {
    m_comboWord.GetLBText(ind,m_wordSpacingUnit);
    m_canApply = true;
  }
  UpdateProperties();
  Redisplay();
}

void 
ParagraphDlg::OnDeltaposSpinWs(NMHDR *pNMHDR, LRESULT *pResult)
{
  LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
  *pResult = 0;
  CWnd* w = GetDlgItem(IDC_PARA_WS);
  w->GetWindowText(m_wordSpacing);
  m_canApply = true;
  double total = atof(m_wordSpacing);
  total += pNMUpDown->iDelta;
  m_wordSpacing.Format("%f",total);
  m_wordSpacing.TrimRight('0');
  m_wordSpacing.TrimRight('.');
  w->SetWindowText(m_wordSpacing);
  OnEnChangeParaWs();
}

void ParagraphDlg::OnBnClickedOk()
{
  OnBnClickedParaApply();
  OnOK();
}

void ParagraphDlg::OnBnClickedParaApply()
{
  if(m_css->print_css())
  {
    CComBSTR bText;
    CString sheet = m_css->GetTheSheet().c_str();
    sheet.TrimLeft("p {");
    sheet.TrimRight("}");
    bText = CT2CW(sheet);
    m_style->put_cssText(bText);

    m_canApply = false;
    UpdateData(Data2Controls);
  }
}

void ParagraphDlg::OnBnClickedCancel()
{
  OnCancel();
}

void ParagraphDlg::OnBnClickedId()
{
  HtmlElement elem(m_elem);
  GeneralIDDlg dlg(this,"p",&elem);
  if(dlg.DoModal() == IDOK)
  {
    m_canApply = true;
    UpdateData(Data2Controls);
  }
}

void 
ParagraphDlg::OnBnClickedEvents()
{
  HtmlElement elem(m_elem);
  TagEventsDlg dlg(this,&elem,"P");
  dlg.DoModal();
  UpdateData(Data2Controls);}

void 
ParagraphDlg::OnBnClickedStyle()
{
  CComBSTR bText;

  UpdateProperties();
  if(m_css->print_css())
  {
    CString sheet = m_css->GetTheSheet().c_str();
    StyleSheetDlg dlg(this,m_base,"p",NULL,sheet);
    if(dlg.DoModal() == IDOK)
    {
      sheet = dlg.GetInlineStylesheet();
      if(m_mySheet)
      {
        delete m_css;
      }
      m_css = new CssStyleSheet();
      m_mySheet = true;
      m_css->parse_css((string)sheet);
      FillPage();

      m_canApply = true;
      UpdateData(Data2Controls);
    }
  }
}


void ParagraphDlg::OnBnClickedParaHelp()
{
  // TODO: Add your control notification handler code here
}

