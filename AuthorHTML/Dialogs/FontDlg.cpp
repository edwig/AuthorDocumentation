//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    FontDlg.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for managing the <FONT> tag in HTML, 
//              to set the font size, color and face
//
#include "stdafx.h"
#include "AuthorHTML.h"
#include "FontDlg.h"
#include "TagEventsDlg.h"
#include "StyleSheetDlg.h"
#include "GeneralIDDlg.h"

#define COLOR_DEFAULT_FONTCOLOR 0x0000000  // BLACK

// FontDlg dialog

IMPLEMENT_DYNAMIC(FontDlg, CDialog)

FontDlg::FontDlg(CWnd* pParent
                ,CString base
                ,HtmlElement* elem)
        :CDialog(FontDlg::IDD, pParent)
        ,m_elem(elem)
        ,m_base(base)
{
}

FontDlg::~FontDlg()
{
}

void FontDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
  DDX_Control(pDX,IDC_FONTSIZE,     m_comboSize);
  DDX_Control(pDX,IDC_COLOR_BUTTON, m_buttonColor);
  DDX_Control(pDX,IDC_CHECK1,       m_buttonHasColor);
  DDX_Control(pDX,IDC_FACE,         m_comboFace);
  DDX_Control(pDX,IDC_ID,           m_buttonID);
  DDX_Control(pDX,IDC_EVENTS,       m_buttonEvents);
  DDX_Control(pDX,IDC_STYLE,        m_buttonStyle);
  DDX_Control(pDX,IDOK,             m_buttonOK);
  DDX_Control(pDX,IDCANCEL,         m_buttonCancel);
  DDX_Control(pDX,IDC_HELP_FONTSIZE,m_buttonHelp);
  
  if(pDX->m_bSaveAndValidate == Data2Controls)
  {
    CString def,text;

    m_buttonID.GetWindowText(def);
    text = m_elem->HasIdentity() ? _T("[ &ID ]") : _T("&ID");
    if(def != text) m_buttonID.SetWindowText(text);

    m_buttonStyle.GetWindowText(def);
    text = m_elem->HasStyle() ? _T("[ &Style ]") : _T("&Style");
    if(def != text) m_buttonStyle.SetWindowText(text);

    m_buttonEvents.GetWindowText(def);
    text = m_elem->HasEvents() ? _T("[ &Events ]") : _T("&Events");
    if(def != text) m_buttonEvents.SetWindowText(text);

    int ind;
    ind = m_comboSize.FindStringExact(-1,m_size);
          m_comboSize.SetCurSel(ind);
    ind = m_comboFace.FindStringExact(-1,m_face);
          m_comboFace.SetCurSel(ind);

    m_buttonColor.EnableWindow(m_hasColor);
    m_buttonHasColor.SetCheck(m_hasColor);
  }
}

BEGIN_MESSAGE_MAP(FontDlg, CDialog)
  ON_CBN_SELCHANGE  (IDC_FONTSIZE,      OnCbnSelchangeSize)
  ON_BN_CLICKED     (IDC_COLOR_BUTTON,  OnBnClickedColorButton)
  ON_BN_CLICKED     (IDC_CHECK1,        OnBnClickedCheck1)
  ON_CBN_SELCHANGE  (IDC_FACE,          OnCbnSelchangeFace)
  ON_BN_CLICKED     (IDC_ID,            OnBnClickedId)
  ON_BN_CLICKED     (IDC_EVENTS,        OnBnClickedEvents)
  ON_BN_CLICKED     (IDC_STYLE,         OnBnClickedStyle)
  ON_BN_CLICKED     (IDOK,              OnBnClickedOk)
  ON_BN_CLICKED     (IDCANCEL,          OnBnClickedCancel)
  ON_BN_CLICKED     (IDC_HELP_FONTSIZE, OnBnClickedHelpFontsize)
END_MESSAGE_MAP()

INT CALLBACK NEnumFontNameProc(LOGFONT *plf, TEXTMETRIC* /*ptm*/, INT /*nFontType*/, LPARAM lParam)
{
 CComboBox* comboBox = (CComboBox*) lParam;
 comboBox->AddString(plf->lfFaceName);
 return TRUE;
}

BOOL
FontDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  // Set sizes
  static TCHAR* nFontSizes[] = 
  {
    _T("8"), _T("10"), _T("12"), _T("14"), _T("18"), _T("24"), _T("36"),
    _T("+1"),_T("+2"), _T("+3"), _T("+4"), _T("+5"), _T("+6"), _T("+7"),
    _T("-1"),_T("-2"), _T("-3"), _T("-4"), _T("-5"), _T("-6"), _T("-7")
  };
  for (int i = 0; i < sizeof(nFontSizes)/sizeof(TCHAR*); i++)
  {
    m_comboSize.AddString(nFontSizes[i]);
  }
  // Set Faces
  m_comboFace.ResetContent();
  m_comboFace.AddString(_T(""));
  m_comboFace.AddString(_T("serif"));
  m_comboFace.AddString(_T("sans-serif"));
  m_comboFace.AddString(_T("monospace"));
  ::EnumFontFamilies(GetDC()->m_hDC,(LPTSTR) NULL,(FONTENUMPROC)NEnumFontNameProc,(LPARAM)(&m_comboFace));

  m_buttonColor.SetColor(COLOR_DEFAULT_FONTCOLOR);
  m_buttonColor.EnableOtherButton(_T("More colors"));

  FillPage();
  UpdateData(Data2Controls);
  return TRUE;
}

void
FontDlg::FillPage()
{
  m_color = m_elem->GetAttribute(_T("color"));
  m_size  = m_elem->GetAttribute(_T("size"));
  m_face  = m_elem->GetAttribute(_T("face"));

  m_size.Trim();
  if(!m_size.IsEmpty())
  {
    if(m_size.GetAt(0) != _T('-') && 
       m_size.GetAt(0) != _T('+') )
    {
      int size = _ttoi(m_size);
      size = Misc::PointSizeToFontSize(size);
      m_size.Format(_T("%d"),size);
    }
  }
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
FontDlg::UpdateProperties()
{
  m_size.Trim();
  if(!m_size.IsEmpty())
  {
    if(m_size.GetAt(0) != _T('-') &&
       m_size.GetAt(0) != _T('+') )
    {
      int size = _ttoi(m_size);
      size = Misc::FontSizeToPointSize(size);
      m_size.Format(_T("%d"),size);
    }
  }
  if(m_hasColor)
  {
    if(!m_color.IsEmpty())
    {
      int red,green,blue;
      Misc::DecodeColor(m_color,red,green,blue);
      m_color.Format(_T("#%02x%02x%02x"),red,green,blue);
    }
  }
  else
  {
    m_color = _T("");
  }
  m_elem->SetAttribute(_T("color"),m_color);
  m_elem->SetAttribute(_T("size"), m_size);
  m_elem->SetAttribute(_T("face"), m_face);
}

// FontDlg message handlers

void 
FontDlg::OnCbnSelchangeSize()
{
  int ind = m_comboSize.GetCurSel();
  if(ind >= 0)
  {
    m_comboSize.GetLBText(ind,m_size);
  }
}

void 
FontDlg::OnBnClickedColorButton()
{
  COLORREF col = m_buttonColor.GetColor();
  int red   = GetRValue(col);
  int green = GetGValue(col);
  int blue  = GetBValue(col);
  m_color.Format(_T("#%02x%02x%02x"),red,green,blue);
}

void 
FontDlg::OnCbnSelchangeFace()
{
  int ind = m_comboFace.GetCurSel();
  if(ind >= 0)
  {
    m_comboFace.GetLBText(ind,m_face);
  }
  else
  {
    m_comboFace.GetWindowText(m_face);
  }
}

void 
FontDlg::OnBnClickedId()
{
  GeneralIDDlg dlg(this,_T("FONT"),m_elem);
  dlg.DoModal();
}

void 
FontDlg::OnBnClickedEvents()
{
  TagEventsDlg dlg(this,m_elem,_T("FONT"));
  dlg.DoModal();
  UpdateData(Data2Controls);
}

void 
FontDlg::OnBnClickedStyle()
{
  if(!m_elem->HasStyle())
  {
    m_elem->SetStyle();
  }
  CString style = m_elem->GetInlineStyle();
  style = CString(_T("font { ")) + style + _T("}");
  StyleSheetDlg dlg(this,m_base,_T("font"),NULL,style);
  if(dlg.DoModal() == IDOK)
  {
    style = dlg.GetInlineStylesheet();
    style.TrimRight(_T("}"));
    style.TrimLeft(_T("font {"));
    m_elem->SetInlineStyle(style);
  }
}

void 
FontDlg::OnBnClickedOk()
{
  UpdateProperties();
  OnOK();
}

void 
FontDlg::OnBnClickedCancel()
{
  OnCancel();
}

void 
FontDlg::OnBnClickedHelpFontsize()
{
  // TODO: Add your control notification handler code here
}

void 
FontDlg::OnBnClickedCheck1()
{
  m_hasColor = m_buttonHasColor.GetCheck() == TRUE;
  UpdateData(Data2Controls);
}
