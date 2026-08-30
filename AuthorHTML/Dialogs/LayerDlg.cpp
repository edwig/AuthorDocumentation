//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    LayerDlg.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for managing the <LAYER> tag in HTML
//
#include "stdafx.h"
#include "AuthorHTML.h"
#include "LayerDlg.h"
#include "GeneralIDDlg.h"
#include "StyleSheetDlg.h"
#include "TagEventsDlg.h"

// LayerDlg dialog
IMPLEMENT_DYNAMIC(LayerDlg, CDialog)

LayerDlg::LayerDlg(CWnd* pParent
                  ,CString base
                  ,HtmlElement* elem
                  ,CString newUnique)
         :CDialog(LayerDlg::IDD, pParent)
         ,m_base(base)
         ,m_elem(elem)
         ,m_newUniqueID(newUnique)
         ,m_css(NULL)
{
}

LayerDlg::~LayerDlg()
{
  if(m_css)
  {
    delete m_css;
    m_css = NULL;
  }
}

void LayerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
  DDX_Text   (pDX,IDC_LAYER_NAME,   m_id);
  DDX_Text   (pDX,IDC_LAYER_ZINDEX, m_zindex);
  DDX_Control(pDX,IDC_VISIBILITY,   m_comboVisibility);
  DDX_Text   (pDX,IDC_WIDTH,        m_width);
  DDX_Text   (pDX,IDC_HEIGHT,       m_height);
  DDX_Control(pDX,IDC_ID,           m_buttonID);
  DDX_Control(pDX,IDC_EVENTS,       m_buttonEvents);
  DDX_Control(pDX,IDC_STYLE,        m_buttonStyle);
  DDX_Control(pDX,IDOK,             m_buttonOK);
  DDX_Control(pDX,IDCANCEL,         m_buttonCancel);
  DDX_Control(pDX,IDC_LAYER_HELP,   m_buttonHelp);
  DDX_Control(pDX,IDC_SPIN_ZINDEX,  m_spinZindex);
  DDX_Control(pDX,IDC_SPIN_WIDTH,   m_spinWidth);
  DDX_Control(pDX,IDC_SPIN_HEIGHT,  m_spinHeight);

  if(pDX->m_bSaveAndValidate == Data2Controls)
  {
    int ind = m_comboVisibility.FindString(-1,m_visibility);
              m_comboVisibility.SetCurSel(ind);

    CString def,text;

    m_buttonID.GetWindowText(def);
    text = m_elem->HasIdentity() ? "[ &ID ]" : "&ID";
    if(def != text) m_buttonID.SetWindowText(text);

    m_buttonStyle.GetWindowText(def);
    text = m_elem->HasStyle() ? "[ &Style ]" : "&Style";
    if(def != text) m_buttonStyle.SetWindowText(text);

    m_buttonEvents.GetWindowText(def);
    text = m_elem->HasEvents() ? "[ &Events ]" : "&Events";
    if(def != text) m_buttonEvents.SetWindowText(text);
  }
}

BEGIN_MESSAGE_MAP(LayerDlg, CDialog)
  ON_EN_KILLFOCUS (IDC_LAYER_NAME,  OnEnChangeLayerName)
  ON_EN_KILLFOCUS (IDC_LAYER_ZINDEX,OnEnChangeLayerZindex)
  ON_CBN_SELCHANGE(IDC_VISIBILITY,  OnCbnSelchangeVisibility)
  ON_EN_KILLFOCUS (IDC_WIDTH,       OnEnChangeWidth)
  ON_EN_KILLFOCUS (IDC_HEIGHT,      OnEnChangeHeight)
  ON_BN_CLICKED   (IDC_ID,          OnBnClickedId)
  ON_BN_CLICKED   (IDC_EVENTS,      OnBnClickedEvents)
  ON_BN_CLICKED   (IDC_STYLE,       OnBnClickedStyle)
  ON_BN_CLICKED   (IDOK,            OnBnClickedOk)
  ON_BN_CLICKED   (IDCANCEL,        OnBnClickedCancel)
  ON_BN_CLICKED   (IDC_LAYER_HELP,  OnBnClickedLayerHelp)
END_MESSAGE_MAP()

BOOL
LayerDlg::OnInitDialog()
{
  CDialog::OnInitDialog();
  FillPage();

  m_spinZindex.SetBase(10);
  m_spinZindex.SetRange(1,32000);
  m_spinZindex.SetPos(atoi(m_zindex));
  m_spinWidth.SetBase(10);
  m_spinWidth.SetRange(0,32000);
  m_spinWidth.SetPos(atoi(m_width));
  m_spinHeight.SetBase(10);
  m_spinHeight.SetRange(0,32000);
  m_spinHeight.SetPos(atoi(m_height));

  UpdateData(Data2Controls);
  return TRUE;
}

void
LayerDlg::FillPage()
{
  if(!m_css)
  {
    m_css = new CssStyleSheet();
    // Timestamp not permitted. Stripping of selector will fail otherwise
    m_css->m_settings["timestamp"] = 0;
  }
  if(!m_elem->HasStyle())
  {
    m_elem->SetStyle();
    // INIT LAYER CODE
    m_id.Format("Layer%s",m_newUniqueID.GetString());
    m_zindex     = "1";
    m_visibility = "Unspecified";
    m_width      = "100";
    m_height     = "100";
    m_left       = "100";
    m_top        = "100";
    UpdateProperties();
    return;
  }
  CString style = m_elem->GetInlineStyle();
  style = CString("div { ") + style + "}";
  m_css->parse_css((string)style);

  string selector = "div";
  m_id = m_elem->GetAttribute("id");
  m_zindex     = CSSPropertyGet(m_css,selector,"z-index",   false);
  m_visibility = CSSPropertyGet(m_css,selector,"visibility",true);
  m_width      = CSSPropertyGet(m_css,selector,"width",     false);
  m_height     = CSSPropertyGet(m_css,selector,"height",    false);
  m_left       = CSSPropertyGet(m_css,selector,"left",      false);
  m_top        = CSSPropertyGet(m_css,selector,"top",       false);
}

void
LayerDlg::UpdateProperties()
{
  m_elem->SetAttribute("id",m_id);
  string selector = "div";
  CSSPropertyPut(m_css,selector,"z-index",   m_zindex,    false);
  CSSPropertyPut(m_css,selector,"visibility",m_visibility,true);
  CSSPropertyPut(m_css,selector,"width",     m_width,     false);
  CSSPropertyPut(m_css,selector,"height",    m_height,    false);
  CSSPropertyPut(m_css,selector,"left",      m_left,      false);
  CSSPropertyPut(m_css,selector,"top",       m_top,       false);
  CSSPropertyPut(m_css,selector,"position",  "absolute",  false);
  if(m_css->print_css())
  {
    CString inlineSheet = m_css->GetTheSheet().c_str();
    inlineSheet.TrimRight("}");
    inlineSheet.TrimLeft("div {");
    m_elem->SetInlineStyle(inlineSheet);
  }
}

// LayerDlg message handlers

void 
LayerDlg::OnEnChangeLayerName()
{
  CWnd* w = GetDlgItem(IDC_LAYER_NAME);
  w->GetWindowText(m_id);
}

void 
LayerDlg::OnEnChangeLayerZindex()
{
  CWnd* w = GetDlgItem(IDC_LAYER_ZINDEX);
  w->GetWindowText(m_zindex);
}

void 
LayerDlg::OnCbnSelchangeVisibility()
{
  int ind = m_comboVisibility.GetCurSel();
  if(ind >= 0)
  {
    m_comboVisibility.GetLBText(ind,m_visibility);
  }
}

void LayerDlg::OnEnChangeWidth()
{
  CWnd* w = GetDlgItem(IDC_WIDTH);
  w->GetWindowText(m_width);
}

void LayerDlg::OnEnChangeHeight()
{
  CWnd* w = GetDlgItem(IDC_HEIGHT);
  w->GetWindowText(m_height);
}

void 
LayerDlg::OnBnClickedId()
{
  GeneralIDDlg dlg(this,"div",m_elem);
  dlg.DoModal();
}

void 
LayerDlg::OnBnClickedEvents()
{
  TagEventsDlg dlg(this,m_elem,"div");
  dlg.DoModal();
  UpdateData(Data2Controls);
}

void 
LayerDlg::OnBnClickedStyle()
{
  UpdateProperties();
  CString style = m_elem->GetInlineStyle();
  style = CString("div { ") + style + "}";
  StyleSheetDlg dlg(this,m_base,"div",NULL,style);
  if(dlg.DoModal() == IDOK)
  {
    style = dlg.GetInlineStylesheet();
    style.TrimRight("}");
    style.TrimLeft("div {");
    m_elem->SetInlineStyle(style);
    FillPage();
    UpdateData(Data2Controls);
  }
}

void 
LayerDlg::OnBnClickedOk()
{
  UpdateProperties();
  OnOK();
}

void 
LayerDlg::OnBnClickedCancel()
{
  OnCancel();
}

void 
LayerDlg::OnBnClickedLayerHelp()
{
  // TODO: Add your control notification handler code here
}

