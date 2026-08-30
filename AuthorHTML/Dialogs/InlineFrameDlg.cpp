//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    InlineFrameDlg.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for managing the <IFRAME> tag in HTML
//
#include "stdafx.h"
#include "AuthorHTML.h"
#include "InlineFrameDlg.h"
#include "StyleSheetDlg.h"
#include "FileDialog.h"
#include "GeneralIDDlg.h"
#include "TagEventsDlg.h"

IMPLEMENT_DYNAMIC(InlineFrameDlg, CDialog)

InlineFrameDlg::InlineFrameDlg(CWnd* pParent
                              ,CString p_base
                              ,HtmlIFrame* frame)
               :CDialog(InlineFrameDlg::IDD, pParent)
               ,m_iframe(frame)
               ,m_base(p_base)
{
}

InlineFrameDlg::~InlineFrameDlg()
{
}

void InlineFrameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
  // Source
  DDX_Text   (pDX,IDC_IF_NAME,    m_name);
  DDX_Text   (pDX,IDC_IF_SRC,     m_src);
  DDX_Control(pDX,IDC_IF_CHOOSE,  m_buttonChoose);
  // Size
  DDX_Text   (pDX,IDC_IF_WIDTH,   m_width);
  DDX_Text   (pDX,IDC_IF_HEIGHT,  m_height);
  DDX_Control(pDX,IDC_COMBO_WU,   m_comboWUnits);
  DDX_Control(pDX,IDC_COMBO_HU,   m_comboHUnits);
  DDX_Control(pDX,IDC_SPIN_W,     m_spinWidth);
  DDX_Control(pDX,IDC_SPIN_H,     m_spinHeight);
  DDX_Control(pDX,IDC_IF_NORESIZE,m_buttonNoResize);
  // Margins
  DDX_Text   (pDX,IDC_IF_HMARGIN, m_hspace);
  DDX_Text   (pDX,IDC_IF_VMARGIN, m_vspace);
  DDX_Control(pDX,IDC_COMBO_HMU,  m_comboHSU);
  DDX_Control(pDX,IDC_COMBO_VMU,  m_comboVSU);
  DDX_Control(pDX,IDC_SPIN_HM,    m_spinHM);
  DDX_Control(pDX,IDC_SPIN_VM,    m_spinVM);
  // Paddings
  DDX_Text   (pDX,IDC_IF_HPAD,    m_marginw);
  DDX_Text   (pDX,IDC_IF_VPAD,    m_marginh);
  DDX_Control(pDX,IDC_COMBO_HPU,  m_comboHPU);
  DDX_Control(pDX,IDC_COMBO_VPU,  m_comboVPU);
  DDX_Control(pDX,IDC_SPIN_HP,    m_spinHP);
  DDX_Control(pDX,IDC_SPIN_VP,    m_spinVP);
  // Appearance
  DDX_Control(pDX,IDC_IF_ALIGN,   m_comboAlign);
  DDX_Control(pDX,IDC_IF_SCROLL,  m_comboScroll);
  DDX_Control(pDX,IDC_IF_BORDER,  m_buttonBorder);
  // Standard Controls
  DDX_Control(pDX,IDC_ID,         m_buttonID);
  DDX_Control(pDX,IDC_EVENTS,     m_buttonEvents);
  DDX_Control(pDX,IDC_STYLE,      m_buttonStyle);
  DDX_Control(pDX,IDOK,           m_buttonOK);
  DDX_Control(pDX,IDCANCEL,       m_buttonCancel);
  DDX_Control(pDX,IDC_IF_APPLY,   m_buttonApply);
  DDX_Control(pDX,IDC_IF_HELP,    m_buttonHelp);

  if(pDX->m_bSaveAndValidate == Data2Controls)
  {
    int ind;
    ind = m_comboWUnits.FindString(-1,m_wunits);
          m_comboWUnits.SetCurSel(ind);
    ind = m_comboHUnits.FindString(-1,m_hunits);
          m_comboHUnits.SetCurSel(ind);
    ind = m_comboHSU.FindString(-1,m_hsunits);
          m_comboHSU.SetCurSel(ind);
    ind = m_comboVSU.FindString(-1,m_vsunits);
          m_comboVSU.SetCurSel(ind);
    ind = m_comboHPU.FindString(-1,m_mwunits);
          m_comboHPU.SetCurSel(ind);
    ind = m_comboVPU.FindString(-1,m_mhunits);
          m_comboVPU.SetCurSel(ind);
    ind = m_comboAlign.FindString(-1,m_align);
          m_comboAlign.SetCurSel(ind);
    ind = m_comboScroll.FindString(-1,m_scrolling);
          m_comboScroll.SetCurSel(ind);

    m_buttonNoResize.SetCheck(atoi(m_noresize)    ? TRUE : FALSE);
    m_buttonBorder  .SetCheck(atoi(m_frameBorder) ? TRUE : FALSE);

    CString def,text;

    m_buttonID.GetWindowText(def);
    text = m_iframe->HasIdentity() ? "[ &ID ]" : "&ID";
    if(def != text) m_buttonID.SetWindowText(text);

    m_buttonStyle.GetWindowText(def);
    text = m_iframe->HasStyle() ? "[ &Style ]" : "&Style";
    if(def != text) m_buttonStyle.SetWindowText(text);

    m_buttonEvents.GetWindowText(def);
    text = m_iframe->HasEvents() ? "[ &Events ]" : "&Events";
    if(def != text) m_buttonEvents.SetWindowText(text);
  }
}

BEGIN_MESSAGE_MAP(InlineFrameDlg, CDialog)
  // Source
  ON_EN_KILLFOCUS (IDC_IF_NAME,     OnEnChangeIfName)
  ON_EN_KILLFOCUS (IDC_IF_SRC,      OnEnChangeIfSrc)
  ON_BN_CLICKED   (IDC_IF_CHOOSE,   OnBnClickedIfChoose)
  // Size
  ON_EN_KILLFOCUS (IDC_IF_WIDTH,    OnEnChangeIfWidth)
  ON_EN_KILLFOCUS (IDC_IF_HEIGHT,   OnEnChangeIfHeight)
  ON_CBN_SELCHANGE(IDC_COMBO_WU,    OnCbnSelchangeComboWu)
  ON_CBN_SELCHANGE(IDC_COMBO_HU,    OnCbnSelchangeComboHu)
  ON_BN_CLICKED   (IDC_IF_NORESIZE, OnBnClickedIfNoresize)
  // Margins
  ON_EN_KILLFOCUS (IDC_IF_HMARGIN,  OnEnChangeIfHmargin)
  ON_EN_KILLFOCUS (IDC_IF_VMARGIN,  OnEnChangeIfVmargin)
  ON_CBN_SELCHANGE(IDC_COMBO_HMU,   OnCbnSelchangeComboHmu)
  ON_CBN_SELCHANGE(IDC_COMBO_VMU,   OnCbnSelchangeComboVmu)
  // Paddings
  ON_EN_KILLFOCUS (IDC_IF_HPAD,     OnEnChangeIfHpad)
  ON_EN_KILLFOCUS (IDC_IF_VPAD,     OnEnChangeIfVpad)
  ON_CBN_SELCHANGE(IDC_COMBO_HPU,   OnCbnSelchangeComboHpu)
  ON_CBN_SELCHANGE(IDC_COMBO_VPU,   OnCbnSelchangeCombVpu)
  // Appearance
  ON_CBN_SELCHANGE(IDC_IF_ALIGN,    OnCbnSelchangeIfAlign)
  ON_CBN_SELCHANGE(IDC_IF_SCROLL,   OnCbnSelchangeIfScroll)
  ON_BN_CLICKED   (IDC_IF_BORDER,   OnBnClickedIfBorder)
  // Standard controls
  ON_BN_CLICKED   (IDC_ID,          OnBnClickedId)
  ON_BN_CLICKED   (IDC_EVENTS,      OnBnClickedEvents)
  ON_BN_CLICKED   (IDC_STYLE,       OnBnClickedStyle)
  ON_BN_CLICKED   (IDOK,            OnBnClickedOk)
  ON_BN_CLICKED   (IDC_IF_APPLY,    OnBnClickedIfApply)
  ON_BN_CLICKED   (IDCANCEL,        OnBnClickedCancel)
  ON_BN_CLICKED   (IDC_IF_HELP,     OnBnClickedIfHelp)
END_MESSAGE_MAP()

BOOL
InlineFrameDlg::OnInitDialog()
{
  CDialog::OnInitDialog();
  FillPage();

  m_spinWidth.SetBase(10);
  m_spinWidth.SetRange(0,32000);
  m_spinWidth.SetPos(atoi(m_width));
  m_spinHeight.SetBase(10);
  m_spinHeight.SetRange(0,32000);
  m_spinHeight.SetPos(atoi(m_height));
  m_spinHM.SetBase(10);
  m_spinHM.SetRange(0,32000);
  m_spinHM.SetPos(atoi(m_hspace));
  m_spinVM.SetBase(10);
  m_spinVM.SetRange(0,32000);
  m_spinVM.SetPos(atoi(m_vspace));
  m_spinHP.SetBase(10);
  m_spinHP.SetRange(0,32000);
  m_spinHP.SetPos(atoi(m_marginw));
  m_spinVP.SetBase(10);
  m_spinVP.SetRange(0,32000);
  m_spinVP.SetPos(atoi(m_marginh));

  UpdateData(Data2Controls);
  return TRUE;
}

void
InlineFrameDlg::FillPage()
{
  m_align       = m_iframe->GetProperty(HtmlIFrame::E_Align);
  m_frameBorder = m_iframe->GetProperty(HtmlIFrame::E_FrameBorder);
  m_name        = m_iframe->GetProperty(HtmlIFrame::E_Name);
  m_src         = m_iframe->GetProperty(HtmlIFrame::E_Src);
  m_width       = m_iframe->GetProperty(HtmlIFrame::E_Width);
  m_height      = m_iframe->GetProperty(HtmlIFrame::E_Height);
  m_hspace      = m_iframe->GetProperty(HtmlIFrame::E_HSpace);
  m_vspace      = m_iframe->GetProperty(HtmlIFrame::E_VSpace);
  m_marginh     = m_iframe->GetProperty(HtmlIFrame::E_MarginHeight);
  m_marginw     = m_iframe->GetProperty(HtmlIFrame::E_MarginWidth);
  m_noresize    = m_iframe->GetProperty(HtmlIFrame::E_NoResize);
  m_scrolling   = m_iframe->GetProperty(HtmlIFrame::E_Scrolling);

  CssSplitValueUnits(m_width,  m_width,  m_wunits);
  CssSplitValueUnits(m_height, m_height, m_hunits);
  CssSplitValueUnits(m_hspace, m_hspace, m_hsunits);
  CssSplitValueUnits(m_vspace, m_vspace, m_vsunits);
  CssSplitValueUnits(m_marginh,m_marginh,m_mhunits);
  CssSplitValueUnits(m_marginw,m_marginw,m_mwunits);
}

void
InlineFrameDlg::UpdateProperties()
{
  m_iframe->SetProperty(HtmlIFrame::E_Align,       m_align);
  m_iframe->SetProperty(HtmlIFrame::E_FrameBorder, m_frameBorder);
  m_iframe->SetProperty(HtmlIFrame::E_Name,        m_name);
  m_iframe->SetProperty(HtmlIFrame::E_Src,         m_src);
  m_iframe->SetProperty(HtmlIFrame::E_Width,       m_width   + m_wunits);
  m_iframe->SetProperty(HtmlIFrame::E_Height,      m_height  + m_hunits);
  m_iframe->SetProperty(HtmlIFrame::E_HSpace,      m_hspace  + m_hsunits);
  m_iframe->SetProperty(HtmlIFrame::E_VSpace,      m_vspace  + m_vsunits);
  m_iframe->SetProperty(HtmlIFrame::E_MarginHeight,m_marginh + m_mhunits);
  m_iframe->SetProperty(HtmlIFrame::E_MarginWidth, m_marginw + m_mwunits);
  m_iframe->SetProperty(HtmlIFrame::E_NoResize,    m_noresize);
  m_iframe->SetProperty(HtmlIFrame::E_Scrolling,   m_scrolling);
}

// InlineFrameDlg message handlers

void 
InlineFrameDlg::OnEnChangeIfName()
{
  CWnd* w = GetDlgItem(IDC_IF_NAME);
  w->GetWindowText(m_name);
}

void 
InlineFrameDlg::OnEnChangeIfSrc()
{
  CWnd* w = GetDlgItem(IDC_IF_SRC);
  w->GetWindowText(m_src);
  CString relative;
  if(Misc::MakeRelativePathname(m_base,m_src,relative))
  {
    m_src = relative;
  }
}

void 
InlineFrameDlg::OnBnClickedIfChoose()
{
  DocFileDialog diag(true
                    ,"Search for a page to link"
                    ,"htm"
                    ,""
                    ,0);
  if(diag.DoModal() == IDOK)
  {
    CString file = diag.GetChosenFile();
    m_src = Misc::FilenamePart(file);
    CString relative;
    if(Misc::MakeRelativePathname(m_base,m_src,relative))
    {
      m_src = relative;
    }
    UpdateData(Data2Controls);
  }
}

void 
InlineFrameDlg::OnEnChangeIfWidth()
{
  CWnd* w = GetDlgItem(IDC_IF_WIDTH);
  w->GetWindowText(m_width);
}

void 
InlineFrameDlg::OnEnChangeIfHeight()
{
  CWnd* w = GetDlgItem(IDC_IF_HEIGHT);
  w->GetWindowText(m_height);
}

void 
InlineFrameDlg::OnCbnSelchangeComboWu()
{
  int ind = m_comboWUnits.GetCurSel();
  if(ind >= 0)
  {
    m_comboWUnits.GetLBText(ind,m_wunits);
  }
}

void 
InlineFrameDlg::OnCbnSelchangeComboHu()
{
  int ind = m_comboHUnits.GetCurSel();
  if(ind >= 0)
  {
    m_comboHUnits.GetLBText(ind,m_hunits);
  }
}

void 
InlineFrameDlg::OnBnClickedIfNoresize()
{
  m_noresize = m_buttonNoResize.GetCheck() ? "-1" : "0";
}

void InlineFrameDlg::OnEnChangeIfHmargin()
{
  CWnd* w = GetDlgItem(IDC_IF_HMARGIN);
  w->GetWindowText(m_hspace);
}

void InlineFrameDlg::OnEnChangeIfVmargin()
{
  CWnd* w = GetDlgItem(IDC_IF_VMARGIN);
  w->GetWindowText(m_vspace);
}

void InlineFrameDlg::OnCbnSelchangeComboHmu()
{
  int ind = m_comboHSU.GetCurSel();
  if(ind >= 0)
  {
    m_comboHSU.GetLBText(ind,m_hsunits);
  }
}

void InlineFrameDlg::OnCbnSelchangeComboVmu()
{
  int ind = m_comboVSU.GetCurSel();
  if(ind >= 0)
  {
    m_comboVSU.GetLBText(ind,m_vsunits);
  }
}

void 
InlineFrameDlg::OnEnChangeIfHpad()
{
  CWnd* w = GetDlgItem(IDC_IF_HPAD);
  w->GetWindowText(m_marginw);
}

void 
InlineFrameDlg::OnEnChangeIfVpad()
{
  CWnd* w = GetDlgItem(IDC_IF_VPAD);
  w->GetWindowText(m_marginh);
}

void 
InlineFrameDlg::OnCbnSelchangeComboHpu()
{
  int ind = m_comboHPU.GetCurSel();
  if(ind >= 0)
  {
    m_comboHPU.GetLBText(ind,m_mwunits);
  }
}

void 
InlineFrameDlg::OnCbnSelchangeCombVpu()
{
  int ind = m_comboVPU.GetCurSel();
  if(ind >= 0)
  {
    m_comboVPU.GetLBText(ind,m_mhunits);
  }
}

void 
InlineFrameDlg::OnCbnSelchangeIfAlign()
{
  int ind = m_comboAlign.GetCurSel();
  if(ind >= 0)
  {
    m_comboAlign.GetLBText(ind,m_align);
  }
}

void 
InlineFrameDlg::OnCbnSelchangeIfScroll()
{
  int ind = m_comboScroll.GetCurSel();
  if(ind >= 0)
  {
    m_comboScroll.GetLBText(ind,m_scrolling);
  }
}

void 
InlineFrameDlg::OnBnClickedIfBorder()
{
  m_frameBorder= m_buttonBorder.GetCheck() ? "-1" : "0";
}

void 
InlineFrameDlg::OnBnClickedId()
{
  HtmlElement* elem = (HtmlElement*)(m_iframe);
  GeneralIDDlg dlg(this,"iframe",elem);
  dlg.DoModal();
}

void 
InlineFrameDlg::OnBnClickedEvents()
{
  TagEventsDlg dlg(this,m_iframe,"iframe");
  dlg.DoModal();
  UpdateData(Data2Controls);
}

void 
InlineFrameDlg::OnBnClickedStyle()
{
  if(!m_iframe->HasStyle())
  {
    m_iframe->SetStyle();
  }
  UpdateProperties();
  CString style = m_iframe->GetInlineStyle();
  style = CString("iframe { ") + style + "}";
  // Special case of the stylesheet dialog
  // Only overview / borders / sizes and box
  StyleSheetDlg dlg(this,m_base,"iframe",NULL,style,true,CSS1|CSS4|CSS5|CSS7);
  if(dlg.DoModal() == IDOK)
  {
    style = dlg.GetInlineStylesheet();
    style.TrimRight("}");
    style.TrimLeft("iframe {");
    m_iframe->SetInlineStyle(style);
    FillPage();
    UpdateData(Data2Controls);
  }
}

void 
InlineFrameDlg::OnBnClickedOk()
{
  if(m_src.IsEmpty())
  {
    theApp.ErrorMessage("You must at least supply a source page for the frames content");
    return;
  }
  UpdateProperties();
  OnOK();
}

void 
InlineFrameDlg::OnBnClickedIfApply()
{
  UpdateProperties();
}

void 
InlineFrameDlg::OnBnClickedCancel()
{
  OnCancel();
}

void 
InlineFrameDlg::OnBnClickedIfHelp()
{
  // TODO: Add your control notification handler code here
}

