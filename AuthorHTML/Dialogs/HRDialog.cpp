//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    HRDialog.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for managing the <HR> tag in HTML,
//
#include "stdafx.h"
#include "AuthorHTML.h"
#include "HRDialog.h"
#include "HTMLHR.h"
#include "TableProperties.h"
#include "StyleSheetDlg.h"
#include "GeneralIDDlg.h"
#include "TagEventsDlg.h"

#define COLOR_DEFAULT_RULER 0x0666666  // Dark gray

// HRDialog dialog

IMPLEMENT_DYNAMIC(HRDialog, CDialog)

HRDialog::HRDialog(CWnd* pParent,CString p_base,HtmlHR pHr)
 	       :CDialog(HRDialog::IDD, pParent)
         ,m_base(p_base)
         ,m_hr(pHr)
         ,m_canApply(false)
{
}

void HRDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
  DDX_Text   (pDX,IDC_HRSIZE,     m_size);
  DDX_Text   (pDX,IDC_HRWIDTH,    m_width);
  DDX_Control(pDX,IDC_SPIN1,      m_sizeCtrl);
  DDX_Control(pDX,IDC_SPIN2,      m_widthCtrl);
  DDX_Control(pDX,IDC_HRALIGN,    m_alignCtrl);
  DDX_Control(pDX,IDC_FORMATWIDTH,m_formatCtrl);
  DDX_Control(pDX,IDC_HRNOSHADE,  m_noShade);
  DDX_Control(pDX,IDC_HRSHADE,    m_Shade);
  DDX_Control(pDX,IDC_HRCOLOR,    m_buttonColor);
  DDX_Control(pDX,IDC_CHECK1,     m_buttonHasColor);

  DDX_Control(pDX,IDC_HR_ID,      m_buttonID);
  DDX_Control(pDX,IDC_EVENTS,     m_buttonEvents);
  DDX_Control(pDX,IDC_STYLE,      m_buttonStyle);
  DDX_Control(pDX,IDOK,           m_buttonOK);
  DDX_Control(pDX,IDCANCEL,       m_buttonCancel);
  DDX_Control(pDX,IDC_APPLY,      m_buttonApply);
  DDX_Control(pDX,IDHELP,         m_buttonHelp);

  if(pDX->m_bSaveAndValidate == Data2Controls)
  {
    CString def,text;

    m_buttonID.GetWindowText(def);
    text = m_hr.HasIdentity() ? "[ &ID ]" : "&ID";
    if(def != text) m_buttonID.SetWindowText(text);

    m_buttonStyle.GetWindowText(def);
    text = m_hr.HasStyle() ? "[ &Style ]" : "&Style";
    if(def != text) m_buttonStyle.SetWindowText(text);

    m_buttonEvents.GetWindowText(def);
    text = m_hr.HasEvents() ? "[ &Events ]" : "&Events";
    if(def != text) m_buttonEvents.SetWindowText(text);

    // Align
    int ind = m_alignCtrl.FindString(-1,m_align);
    m_alignCtrl.SetCurSel(ind);
    // Shade
    m_noShade.SetCheck(!m_shade);
    m_Shade  .SetCheck(m_shade);
    // Apply button
    m_buttonApply.EnableWindow(m_canApply);

    m_buttonHasColor.SetCheck(m_hasColor);
    m_buttonColor.EnableWindow(m_hasColor);
  }
}

BEGIN_MESSAGE_MAP(HRDialog, CDialog)
  ON_EN_KILLFOCUS   (IDC_HRSIZE,      OnEnChangeHrsize)
  ON_EN_KILLFOCUS   (IDC_HRWIDTH,     OnEnChangeHrwidth)
  ON_BN_CLICKED     (IDC_HRNOSHADE,   OnBnClickedHrnoshade)
  ON_BN_CLICKED     (IDC_HRSHADE,     OnBnClickedHrshade)
  ON_CBN_SELCHANGE  (IDC_FORMATWIDTH, OnCbnSelchangeFormatwidth)
  ON_CBN_SELCHANGE  (IDC_HRALIGN,     OnCbnSelchangeHralign)
  ON_BN_CLICKED     (IDC_HRCOLOR,     OnBnClickedHrcolor)
  ON_BN_CLICKED     (IDC_CHECK1,      OnBnClickedCheck1)

  ON_BN_CLICKED     (IDC_HR_ID,       OnBnClickedHrId)
  ON_BN_CLICKED     (IDC_EVENTS,      OnBnClickedEvents)
  ON_BN_CLICKED     (IDC_STYLE,       OnBnClickedStyle)
  ON_BN_CLICKED     (IDOK,            OnBnClickedOk)
  ON_BN_CLICKED     (IDC_APPLY,       OnBnClickedApply)
  ON_BN_CLICKED     (IDCANCEL,        OnBnClickedCancel)
  ON_BN_CLICKED     (IDHELP,          OnBnClickedHelp)
END_MESSAGE_MAP()

// HRDialog message handlers

BOOL
HRDialog::OnInitDialog()
{
  CDialog::OnInitDialog();

  ReadProperties();
  m_sizeCtrl .SetBase(10);
  m_widthCtrl.SetBase(10);
  m_sizeCtrl .SetRange(0,32000);
  m_widthCtrl.SetRange(0,32000);
  m_sizeCtrl .SetPos(m_size);
  m_widthCtrl.SetPos(m_width);

  m_formatCtrl.SetCurSel(m_widthInPix ? 0 : 1);

  m_buttonColor.SetColor(COLOR_DEFAULT_RULER);
  m_buttonColor.EnableOtherButton(_T("More colors..."));

  UpdateData(Data2Controls);
  return FALSE;
}

void
HRDialog::ReadProperties()
{
  m_widthInPix = true;
  m_size = atoi(m_hr.GetProperty(HtmlHR::E_Size)); // Always pixels
  CString width = m_hr.GetProperty(HtmlHR::E_Width);
  if(width.Find('%') >= 0)
  {
    m_widthInPix = false;
  }
  m_width = atoi(width);
  m_shade = m_hr.GetShade();
  m_align = m_hr.GetProperty(HtmlHR::E_Align);
  m_color = m_hr.GetProperty(HtmlHR::E_Color);
  if(m_color.IsEmpty())
  {
    m_hasColor = false;
  }
  else
  {
    m_hasColor = true;
    int red,green,blue;
    Misc::DecodeColor(m_color,red,green,blue);
    m_buttonColor.SetColor(RGB(red,green,blue));
  }
}

void
HRDialog::UpdateProperties()
{
  int red,green,blue;
  CString s_size,s_width,s_align;

  UpdateData(Controls2Data);
  // SIZE
  if(m_size > 0)
  {
    s_size.Format("%d",m_size);
  }
  m_hr.SetProperty(HtmlHR::E_Size, s_size);
  // WIDTH
  if(m_width > 0)
  {
    s_width.Format("%d",m_width);
    if(!m_widthInPix)
    {
      s_width += CString("%");
    }
  }
  m_hr.SetProperty(HtmlHR::E_Width,s_width);
  // ALIGN
  m_hr.SetProperty(HtmlHR::E_Align,m_align);
  // SHADE / NOSHADE
  if(m_Shade.GetCheck()   == BST_CHECKED) m_hr.SetShade(TRUE);
  if(m_noShade.GetCheck() == BST_CHECKED) m_hr.SetShade(FALSE);
  // COLOR
  if(m_hasColor)
  {
    if(!m_color.IsEmpty())
    {
      Misc::DecodeColor(m_color,red,green,blue);
      m_color.Format("#%02x%02x%02x",red,green,blue);
    }
  }
  else
  {
    m_color = "";
  }
  m_hr.SetProperty(HtmlHR::E_Color,m_color);
}

void 
HRDialog::OnBnClickedApply()
{
  UpdateProperties();
  m_canApply = false;
  UpdateData(Data2Controls);
}

void 
HRDialog::OnBnClickedOk()
{
  OnBnClickedApply();
  OnOK();
}

void 
HRDialog::OnBnClickedHrnoshade()
{
  m_shade = FALSE;
  m_canApply = true;
  UpdateData(Data2Controls);
}

void 
HRDialog::OnBnClickedHrshade()
{
   m_shade = TRUE;
   m_canApply = true;
   UpdateData(Data2Controls);
}

void 
HRDialog::OnBnClickedHrcolor()
{
  COLORREF col = m_buttonColor.GetColor();
  int red   = GetRValue(col);
  int green = GetGValue(col);
  int blue  = GetBValue(col);
  m_color.Format("#%02x%02x%02x",red,green,blue);
  m_canApply = true;
}

void 
HRDialog::OnBnClickedCheck1()
{
  m_canApply = true;
  m_hasColor = m_buttonHasColor.GetCheck() == TRUE;
  UpdateData(Data2Controls);
}

void 
HRDialog::OnCbnSelchangeFormatwidth()
{
  CString units;
  int ind = m_formatCtrl.GetCurSel();
  if(ind >= 0)
  {
    m_formatCtrl.GetLBText(ind,units);
    m_widthInPix = (units == "Pixels") ? true : false;
    m_canApply = true;
    UpdateData(Data2Controls);
  }
}

void 
HRDialog::OnCbnSelchangeHralign()
{
  int ind = m_alignCtrl.GetCurSel();
  if(ind >= 0)
  {
    m_alignCtrl.GetLBText(ind,m_align);
    m_canApply = true;
    UpdateData(Data2Controls);
  }
}

void 
HRDialog::OnBnClickedHrId()
{
  HtmlElement* elem = (HtmlElement*)(&m_hr);
  GeneralIDDlg dlg(this,"hr",elem);
  dlg.DoModal();
}

void 
HRDialog::OnBnClickedEvents()
{
  TagEventsDlg dlg(this,&m_hr,"hr");
  dlg.DoModal();
  UpdateData(Data2Controls);
}

void 
HRDialog::OnBnClickedStyle()
{
  if(!m_hr.HasStyle())
  {
    m_hr.SetStyle();
  }
  UpdateProperties();
  CString style = m_hr.GetInlineStyle();
  style = CString("hr { ") + style + "}";
  StyleSheetDlg dlg(this,m_base,"hr",NULL,style);
  if(dlg.DoModal() == IDOK)
  {
    style = dlg.GetInlineStylesheet();
    style.TrimRight("}");
    style.TrimLeft("hr {");
    m_hr.SetInlineStyle(style);
    ReadProperties();
    m_canApply = true;
    UpdateData(Data2Controls);
  }
}

void 
HRDialog::OnBnClickedCancel()
{
  if(m_canApply)
  {
    CString msg;
    msg.Format("Changes where made to the %s tag. Would you like to cancel your changes?","HR");
    if(theApp.MessageBox(msg,"Warning",MB_YESNO|MB_ICONQUESTION) == IDNO)
    {
      return;
    }
  }
  OnCancel();
}

void 
HRDialog::OnEnChangeHrsize()
{
  CString size;
  CWnd* w = GetDlgItem(IDC_HRSIZE);
  w->GetWindowText(size);
  if(m_size != (unsigned)atoi(size))
  {
    m_canApply = true;
    m_size     = atoi(size);
    UpdateData(Data2Controls);
  }
}

void 
HRDialog::OnEnChangeHrwidth()
{
  CString width;
  CWnd* w = GetDlgItem(IDC_HRWIDTH);
  w->GetWindowText(width);
  if(m_width != (unsigned)atoi(width))
  {
    m_canApply = true;
    m_width    = atoi(width);
    UpdateData(Data2Controls);
  }
}

void 
HRDialog::OnBnClickedHelp()
{
  // TODO: Add your control notification handler code here
}
