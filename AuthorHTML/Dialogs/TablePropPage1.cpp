//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    TablePropPage1.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: property page for general attributes of the table
//
#include <stdafx.h>
#include "AuthorHTML.h"
#include "TablePropPage1.h"
#include "TableProperties.h"
#include "GeneralIDDlg.h"
#include "StyleSheetDlg.h"
#include "TagEventsDlg.h"

#define COLOR_DEFAULT_TABLEBACKGROUND   0x0ffffff      // White
#define COLOR_DEFAULT_TABLEBORDER       0x0000000      // Black
#define COLOR_DEFAULT_TABLEDARKBORDER   0x0333333      // Dark gray
#define COLOR_DEFAULT_TABLELIGHTBORDER  0x0cccccc      // Light gray

CTablePropPage1Dlg::CTablePropPage1Dlg(CWnd*     pParent /*=NULL*/
                                      ,CString   base
                                      ,HtmlTable pTable)
                   :CDialog(CTablePropPage1Dlg::IDD, pParent)
                   ,m_pTable(pTable)
                   ,m_base(base)
                   ,m_initDone(false)
{
}

void CTablePropPage1Dlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CTablePropPage1Dlg)
  //}}AFX_DATA_MAP
  DDX_Text      (pDX, IDC_TAB_NUMROWS, m_NumRows);
  DDV_MinMaxUInt(pDX, m_NumRows,       1, 32000);
  DDX_Text      (pDX, IDC_TAB_NUMCOLS, m_NumCols);
  DDV_MinMaxUInt(pDX, m_NumCols,       1, 32000);

  DDX_Text      (pDX, IDC_TAB_HEIGHT,  m_TabHeight);
  DDV_MinMaxUInt(pDX, m_TabHeight,     0, 32000);
  DDX_Text      (pDX, IDC_TAB_WIDTH,   m_TabWidth);
  DDV_MinMaxUInt(pDX, m_TabWidth,      0, 32000);

  DDX_Text      (pDX, IDC_TAB_BORDER, m_TabBorder);
  DDV_MinMaxUInt(pDX, m_TabBorder,    0, 32000);
  DDX_Text      (pDX, IDC_TAB_SPACING,m_TabSpacing);
  DDV_MinMaxUInt(pDX, m_TabSpacing,   0, 32000);
  DDX_Text      (pDX, IDC_TAB_PADDING,m_TabPadding);
  DDV_MinMaxUInt(pDX, m_TabPadding,   0, 32000);

  DDX_Text      (pDX, IDC_HSPACE,     m_HSpace);
  DDV_MinMaxUInt(pDX, IDC_HSPACE,     0, 32000);
  DDX_Text      (pDX, IDC_VSPACE,     m_VSpace);
  DDV_MinMaxUInt(pDX, IDC_VSPACE,     0, 32000);

  DDX_Control(pDX, IDC_SPIN1, m_TabRowsCtrl);
  DDX_Control(pDX, IDC_SPIN2, m_TabColumnsCtrl);
  DDX_Control(pDX, IDC_SPIN3, m_TabHeightCtrl);
  DDX_Control(pDX, IDC_SPIN4, m_TabWidthCtrl);
  DDX_Control(pDX, IDC_SPIN5, m_TabBorderCtrl);
  DDX_Control(pDX, IDC_SPIN6, m_TabSpacingCtrl);
  DDX_Control(pDX, IDC_SPIN7, m_TabPaddingCtrl);
  DDX_Control(pDX, IDC_SPIN8, m_HSpaceCtrl);
  DDX_Control(pDX, IDC_SPIN9, m_VSpaceCtrl);

  DDX_Control(pDX, IDC_UNITS1,      m_formatHeight);
  DDX_Control(pDX, IDC_UNITS2,      m_formatWidht);
  DDX_Control(pDX, IDC_TEXTJUSTIFY, m_textJustify);

  DDX_Control(pDX, IDC_BUTT_BORDERCOLOR,m_buttonBorderColor);
  DDX_Control(pDX, IDC_BUTT_DARKBORDER, m_buttonDarkBorderColor);
  DDX_Control(pDX, IDC_BUTT_LIGHTBORDER,m_buttonLightBorderColor);
  DDX_Control(pDX, IDC_BUTTONBACKCOLOR, m_buttonBackColor);

  DDX_Control(pDX, IDC_CHECK1,          m_buttonCanBorder);
  DDX_Control(pDX, IDC_CHECK2,          m_buttonCanDarkBorder);
  DDX_Control(pDX, IDC_CHECK3,          m_buttonCanLightBorder);
  DDX_Control(pDX, IDC_CHECK4,          m_buttonCanBackColor);

  DDX_Control(pDX, IDC_ID,              m_buttonID);
  DDX_Control(pDX, IDC_EVENTS,          m_buttonEvents);
  DDX_Control(pDX, IDC_STYLE,           m_buttonStyle);

  if(pDX->m_bSaveAndValidate == Data2Controls)
  {
    if(m_pTable.Valid())
    {
      CString def,text;

      m_buttonID.GetWindowText(def);
      text = m_pTable.HasIdentity() ? "[ &ID ]" : "&ID";
      if(def != text) m_buttonID.SetWindowText(text);

      m_buttonStyle.GetWindowText(def);
      text = m_pTable.HasStyle() ? "[ &Style ]" : "&Style";
      if(def != text) m_buttonStyle.SetWindowText(text);

      m_buttonEvents.GetWindowText(def);
      text = m_pTable.HasEvents() ? "[ &Events ]" : "&Events";
      if(def != text) m_buttonEvents.SetWindowText(text);
    }
    else
    {
      // Newly defined tag. No style possible yet.
      m_buttonStyle .EnableWindow(FALSE);
      m_buttonEvents.EnableWindow(FALSE);
      m_buttonID    .EnableWindow(FALSE);
    }
    m_buttonBorderColor     .EnableWindow(m_hasBorderColor);
    m_buttonDarkBorderColor .EnableWindow(m_hasDarkBorderColor);
    m_buttonLightBorderColor.EnableWindow(m_hasLightBorderColor);
    m_buttonBackColor       .EnableWindow(m_hasBackgroundColor);
  }
  CTablePropertiesDlg* dlg = dynamic_cast<CTablePropertiesDlg*> (GetParent());
  if(dlg)
  {
    dlg->UpdateColumns(m_NumCols);
  }
}

BEGIN_MESSAGE_MAP(CTablePropPage1Dlg, CDialog)
  ON_CBN_SELCHANGE  (IDC_UNITS1,           OnCbnSelchangeUnits1)
  ON_CBN_SELCHANGE  (IDC_UNITS2,           OnCbnSelchangeUnits2)
  ON_CBN_SELCHANGE  (IDC_TEXTJUSTIFY,      OnCbnSelchangeTextjustify)
  ON_BN_CLICKED     (IDC_BUTT_BORDERCOLOR, OnBnClickedButtBordercolor)
  ON_BN_CLICKED     (IDC_BUTT_DARKBORDER,  OnBnClickedButtDarkborder)
  ON_BN_CLICKED     (IDC_BUTT_LIGHTBORDER, OnBnClickedButtLightborder)
  ON_BN_CLICKED     (IDC_BUTTONBACKCOLOR,  OnBnClickedButtonbackcolor)
  ON_BN_CLICKED     (IDC_ID,               OnBnClickedId)
  ON_BN_CLICKED     (IDC_EVENTS,           OnBnClickedEvents)
  ON_BN_CLICKED     (IDC_STYLE,            OnBnClickedStyle)
  ON_BN_CLICKED     (IDC_CHECK1,           OnBnClickedCheck1)
  ON_BN_CLICKED     (IDC_CHECK2,           OnBnClickedCheck2)
  ON_BN_CLICKED     (IDC_CHECK3,           OnBnClickedCheck3)
  ON_BN_CLICKED     (IDC_CHECK4,           OnBnClickedCheck4)
END_MESSAGE_MAP()

BOOL
CTablePropPage1Dlg::OnInitDialog()
{
  CDialog::OnInitDialog();
  ReadProperties();

  m_TabRowsCtrl.   SetRange(1,32000);
  m_TabColumnsCtrl.SetRange(1,32000);
  m_TabHeightCtrl. SetRange(0,32000);
  m_TabWidthCtrl.  SetRange(0,32000);
  m_TabBorderCtrl. SetRange(0,32000);
  m_TabSpacingCtrl.SetRange(0,32000);
  m_TabPaddingCtrl.SetRange(0,32000);
  m_HSpaceCtrl.    SetRange(0,32000);
  m_VSpaceCtrl.    SetRange(0,32000);

  m_TabRowsCtrl.   SetBase(10);
  m_TabColumnsCtrl.SetBase(10);
  m_TabHeightCtrl. SetBase(10);
  m_TabWidthCtrl.  SetBase(10);
  m_TabBorderCtrl. SetBase(10);
  m_TabSpacingCtrl.SetBase(10);
  m_TabPaddingCtrl.SetBase(10);
  m_HSpaceCtrl.    SetBase(10);
  m_VSpaceCtrl.    SetBase(10);
  m_TabRowsCtrl.   SetPos(m_NumRows);
  m_TabColumnsCtrl.SetPos(m_NumCols);
  m_TabHeightCtrl. SetPos(m_TabHeight);
  m_TabWidthCtrl.  SetPos(m_TabWidth);
  m_TabBorderCtrl. SetPos(m_TabBorder);
  m_TabSpacingCtrl.SetPos(m_TabSpacing);
  m_TabPaddingCtrl.SetPos(m_TabPadding);
  m_HSpaceCtrl.    SetPos(m_HSpace);
  m_VSpaceCtrl.    SetPos(m_VSpace);

  m_formatHeight.SetCurSel(m_tabHeightInPix ? 1 : 0);
  m_formatWidht. SetCurSel(m_tabWidthInPix  ? 1 : 0);

  int ind = 0;
  if (m_align.CompareNoCase("No text around") == 0) ind = 0;
  if (m_align.CompareNoCase("Left")   == 0)         ind = 1;
  if (m_align.CompareNoCase("Center") == 0)         ind = 2;
  if (m_align.CompareNoCase("Right")  == 0)         ind = 3;
  m_textJustify.SetCurSel(ind);

  m_buttonBorderColor     .SetColor(COLOR_DEFAULT_TABLEBORDER);
  m_buttonDarkBorderColor .SetColor(COLOR_DEFAULT_TABLEDARKBORDER);
  m_buttonLightBorderColor.SetColor(COLOR_DEFAULT_TABLELIGHTBORDER);
  m_buttonBackColor       .SetColor(COLOR_DEFAULT_TABLEBACKGROUND);

  m_buttonBorderColor     .EnableOtherButton(_T("More colors..."));
  m_buttonDarkBorderColor .EnableOtherButton(_T("More colors..."));
  m_buttonLightBorderColor.EnableOtherButton(_T("More colors..."));
  m_buttonBackColor       .EnableOtherButton(_T("More colors..."));

  UpdateData(Data2Controls);
  m_initDone = true;
  return TRUE;
}

void
CTablePropPage1Dlg::ReadProperties()
{
  if(m_pTable.Valid())
  {
    m_NumCols    = m_pTable.GetColumnCount();
    m_NumRows    = m_pTable.GetRowCount();
    m_TabBorder  = atoi(m_pTable.GetProperty(HtmlTable::E_BorderWidth));
    m_TabSpacing = atoi(m_pTable.GetProperty(HtmlTable::E_CellSpacing));
    m_TabPadding = atoi(m_pTable.GetProperty(HtmlTable::E_CellPadding));
    m_tabHeightInPix = true;
    m_tabWidthInPix  = true;
    CString height = m_pTable.GetProperty(HtmlTable::E_Height);
    CString width  = m_pTable.GetProperty(HtmlTable::E_Width);
    if((height.Find("%") >= 0))
    {
      m_tabHeightInPix = false;
    }
    if((width.Find("%") >= 0))
    {
      m_tabWidthInPix = false;
    }
    m_TabHeight = atoi(height);
    m_TabWidth  = atoi(width);
    m_VSpace = atoi(m_pTable.GetProperty(HtmlTable::E_VSpace));
    m_HSpace = atoi(m_pTable.GetProperty(HtmlTable::E_HSpace));
    m_align  = m_pTable.GetProperty(HtmlTable::E_Alignment);
    if(m_align.IsEmpty())
    {
      m_align = "No text around";
    }
    m_backgroundColor  = m_pTable.GetProperty(HtmlTable::E_BgColor);
    m_borderColor      = m_pTable.GetProperty(HtmlTable::E_BorderColor);
    m_darkBorderColor  = m_pTable.GetProperty(HtmlTable::E_BorderColorDark);
    m_lightBorderColor = m_pTable.GetProperty(HtmlTable::E_BorderColorLight);

    int red,green,blue;
    if(m_backgroundColor.IsEmpty())
    {
      m_hasBackgroundColor = false;
    }
    else
    {
      m_hasBackgroundColor = true;
      Misc::DecodeColor(m_backgroundColor,red,green,blue);
      m_buttonBackColor.SetColor(RGB(red,green,blue));
    }
    if(m_borderColor.IsEmpty())
    {
      m_hasBorderColor = false;
    }
    else
    {
      m_hasBorderColor = true;
      Misc::DecodeColor(m_borderColor,red,green,blue);
      m_buttonBorderColor.SetColor(RGB(red,green,blue));
    }
    if(m_darkBorderColor.IsEmpty())
    {
      m_hasDarkBorderColor = false;
    }
    else
    {
      m_hasDarkBorderColor = true;
      Misc::DecodeColor(m_darkBorderColor,red,green,blue);
      m_buttonDarkBorderColor.SetColor(RGB(red,green,blue));
    }
    if(m_lightBorderColor.IsEmpty())
    {
      m_hasLightBorderColor = false;
    }
    else
    {
      m_hasLightBorderColor = true;
      Misc::DecodeColor(m_lightBorderColor,red,green,blue);
      m_buttonLightBorderColor.SetColor(RGB(red,green,blue));
    }
  }
  else
  {
    // Defaults for a new table
    m_NumCols        = 3;
    m_NumRows        = 2;
    m_TabBorder      = 1;
    m_TabSpacing     = 2;
    m_TabPadding     = 1;
    m_TabHeight      = 0;
    m_TabWidth       = 100;
    m_tabHeightInPix = true;
    m_tabWidthInPix  = false;
    m_VSpace         = 0;
    m_HSpace         = 0;
    m_align          = "No text around";

    m_hasBorderColor      = false;
    m_hasLightBorderColor = false;
    m_hasDarkBorderColor  = false;
    m_hasBackgroundColor  = false;
  }
}

void
CTablePropPage1Dlg::UpdateProperties()
{
  CString val,val2;
  UINT value;
  // Make sure we have the latest version
  UpdateData(Controls2Data);

  UINT rowsNow = m_pTable.GetRowCount();
  UINT colsNow = m_pTable.GetColumnCount();

  // Number of columns changed?
  if(m_NumCols != colsNow)
  {
    if(m_NumCols > colsNow)
    {
      while(m_NumCols > colsNow)
      {
        m_pTable.InsertColumn(colsNow++);
      }
    }
    else
    {
      // Ask for confirmation before deleting columns
      CString message;
      message.Format("Table will be truncated by %d column(s). Continue?",(colsNow - m_NumCols));
      if(theApp.MessageBox(message,"Beware",MB_YESNO|MB_ICONEXCLAMATION) == IDYES)
      {
        while(m_NumCols < colsNow)
        {
          m_pTable.DeleteColumn(--colsNow);
        }
      }
    }
  }
  // Number of rows changed?
  if(m_NumRows != rowsNow)
  {
    if(m_NumRows > rowsNow)
    {
      while(m_NumRows > rowsNow)
      {
        m_pTable.InsertRow(rowsNow++,m_NumCols);
      }
    }
    else
    {
      // Ask for confirmation before deleting rows
      CString message;
      message.Format("Table will be truncated by %d row(s). Continue?",(rowsNow - m_NumRows));
      if(theApp.MessageBox(message,"Beware",MB_YESNO|MB_ICONEXCLAMATION) == IDYES)
      {
        while(m_NumRows < rowsNow)
        {
          m_pTable.DeleteRow(--rowsNow);
        }
      }
    }
  }
  // Height
  val = m_pTable.GetProperty(HtmlTable::E_Height);
  if(m_tabHeightInPix)
  {
    val2.Format("%d", m_TabHeight);
  }
  else
  {
    val2.Format("%d%",m_TabHeight);
  }
  if((val != val2) && (m_TabHeight > 1))
  {
    m_pTable.SetProperty(HtmlTable::E_Height,val2);
  }

  // Width
  val = m_pTable.GetProperty(HtmlTable::E_Width);
  if(m_tabWidthInPix)
  {
    val2.Format("%d",m_TabWidth);
  }
  else
  {
    val2.Format("%d%%",m_TabWidth);
  }
  if((val != val2) && (m_TabWidth > 1))
  {
    m_pTable.SetProperty(HtmlTable::E_Width,val2);
  }
  // Border
  val   = m_pTable.GetProperty(HtmlTable::E_BorderWidth);
  value = atoi(val);
  if(m_TabBorder != value)
  {
    CString width;
    width.Format("%dpx",m_TabBorder);
    m_pTable.SetProperty(HtmlTable::E_BorderWidth,width);
  }
  // Spacing
  val = m_pTable.GetProperty(HtmlTable::E_CellSpacing);
  value = atoi(val);
  if(m_TabSpacing != value)
  {
    CString space;
    space.Format("%dpx",m_TabSpacing);
    m_pTable.SetProperty(HtmlTable::E_CellSpacing,space);
  }
  // Padding
  val = m_pTable.GetProperty(HtmlTable::E_CellPadding);
  value = atoi(val);
  if(m_TabSpacing != value)
  {
    CString pad;
    pad.Format("%dpx",m_TabPadding);
    m_pTable.SetProperty(HtmlTable::E_CellPadding,pad);
  }
  // VSpace
  if(m_VSpace != (unsigned) atoi(m_pTable.GetProperty(HtmlTable::E_VSpace)))
  {
    CString space;
    space.Format("%d",m_VSpace);
    m_pTable.SetProperty(HtmlTable::E_VSpace,space);
  }
  // HSpace
  if(m_HSpace != (unsigned) atoi(m_pTable.GetProperty(HtmlTable::E_HSpace)))
  {
    CString space;
    space.Format("%d",m_HSpace);
    m_pTable.SetProperty(HtmlTable::E_HSpace,space);
  }
  // ALignment / Justify
  CString alignment = m_align;
  if(m_align == "No text around")
  {
    alignment = "";
  }
  if(alignment != m_pTable.GetProperty(HtmlTable::E_Alignment))
  {
    m_pTable.SetProperty(HtmlTable::E_Alignment,alignment);
  }

  int red,green,blue;
  // Background
  if(m_hasBackgroundColor)
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
  m_pTable.SetProperty(HtmlTable::E_BgColor,m_backgroundColor);

  // Bordercolor
  if(m_hasBorderColor)
  {
    if(!m_borderColor.IsEmpty())
    {
      Misc::DecodeColor(m_borderColor,red,green,blue);
      m_borderColor.Format("#%02x%02x%02x",red,green,blue);
    }
  }
  else
  {
    m_borderColor = "";
  }
  m_pTable.SetProperty(HtmlTable::E_BorderColor,m_borderColor);

  // Dark border color
  if(m_hasDarkBorderColor)
  {
    if(!m_darkBorderColor.IsEmpty())
    {
      Misc::DecodeColor(m_darkBorderColor,red,green,blue);
      m_darkBorderColor.Format("#%02x%02x%02x",red,green,blue);
    }
  }
  else
  {
    m_darkBorderColor = "";
  }
  m_pTable.SetProperty(HtmlTable::E_BorderColorDark,m_darkBorderColor);

  // Light border color
  if(m_hasLightBorderColor)
  {
    if(!m_lightBorderColor.IsEmpty())
    {
      Misc::DecodeColor(m_lightBorderColor,red,green,blue);
      m_lightBorderColor.Format("#%02x%02x%02x",red,green,blue);
    }
  }
  else
  {
    m_lightBorderColor = "";
  }
  m_pTable.SetProperty(HtmlTable::E_BorderColorLight,m_lightBorderColor);
}

CString
CTablePropPage1Dlg::CreateTableString(CString p_caption,CString p_oneCell)
{
  CString oneTable = "<TABLE";
  CString extra;

  // Table defintiion
  extra.Format(m_tabWidthInPix ? "%dpx\"" : "%d%%\"", m_TabWidth);
  oneTable += CString("\n width=\"") + extra;
  extra.Format("\n border=\"%d\"",     m_TabBorder);      oneTable += extra;
  extra.Format("\n cellspacing=\"%d\"",m_TabSpacing);     oneTable += extra;
  extra.Format("\n cellpadding=\"%d\"",m_TabPadding);     oneTable += extra;
  if(m_hasBackgroundColor && !m_backgroundColor.IsEmpty())
  {
    oneTable += "\n bgcolor=\"" + m_backgroundColor + "\"";
  }
  if(m_hasBorderColor && !m_borderColor.IsEmpty())
  {
    oneTable += "\n bordercolor=\"" + m_borderColor + "\"";
  }
  if(m_hasDarkBorderColor && !m_darkBorderColor.IsEmpty())
  {
    oneTable += "\n bordercolordark=\"" + m_darkBorderColor + "\"";
  }
  if(m_hasLightBorderColor && !m_lightBorderColor.IsEmpty())
  {
    oneTable += "\n bordercolorlight=\"" + m_lightBorderColor + "\"";
  }
  oneTable += ">";

  // Caption comes next
  oneTable += p_caption;

  // Here comes the table body
  oneTable += "<TBODY>";
  // Build all the cells
  for(UINT r = 0; r < m_NumRows; ++r)
  {
    oneTable += "\n<TR>";
    for(UINT c = 0;c < m_NumCols; ++c)
    {
      oneTable += p_oneCell;
    }
    oneTable += "</TR>";
  }
  oneTable += "\n</TBODY>";
  oneTable += "\n</TABLE>\n";
  return oneTable;
}

/////////////////////////////////////////////////////////////////////////////
// CTablePropPage1Dlg message handlers
void 
CTablePropPage1Dlg::OnCbnSelchangeUnits1()
{
  CString units;
  int ind = m_formatHeight.GetCurSel();
  m_formatHeight.GetLBText(ind,units);
  m_tabHeightInPix = (units == "Pixels") ? true : false;
}

void 
CTablePropPage1Dlg::OnCbnSelchangeUnits2()
{
  CString units;
  int ind = m_formatWidht.GetCurSel();
  m_formatWidht.GetLBText(ind,units);
  m_tabWidthInPix = (units == "Pixels") ? true : false;
}

void 
CTablePropPage1Dlg::OnCbnSelchangeTextjustify()
{
  CString justify;
  int ind = m_textJustify.GetCurSel();
  m_textJustify.GetLBText(ind,justify);
  m_align = justify;
}

void 
CTablePropPage1Dlg::OnBnClickedButtBordercolor()
{
  COLORREF col = m_buttonBorderColor.GetColor();
  int red   = GetRValue(col);
  int green = GetGValue(col);
  int blue  = GetBValue(col);
  m_borderColor.Format("#%02x%02x%02x",red,green,blue);
}

void 
CTablePropPage1Dlg::OnBnClickedButtDarkborder()
{
  COLORREF col = m_buttonDarkBorderColor.GetColor();
  int red   = GetRValue(col);
  int green = GetGValue(col);
  int blue  = GetBValue(col);
  m_darkBorderColor.Format("#%02x%02x%02x",red,green,blue);
}

void 
CTablePropPage1Dlg::OnBnClickedButtLightborder()
{
  COLORREF col = m_buttonLightBorderColor.GetColor();
  int red   = GetRValue(col);
  int green = GetGValue(col);
  int blue  = GetBValue(col);
  m_lightBorderColor.Format("#%02x%02x%02x",red,green,blue);
}

void 
CTablePropPage1Dlg::OnBnClickedButtonbackcolor()
{
  COLORREF col = m_buttonBackColor.GetColor();
  int red   = GetRValue(col);
  int green = GetGValue(col);
  int blue  = GetBValue(col);
  m_backgroundColor.Format("#%02x%02x%02x",red,green,blue);
}

void 
CTablePropPage1Dlg::OnBnClickedId()
{
  HtmlElement* elem = (HtmlElement*)(&m_pTable);
  GeneralIDDlg dlg(this,"table",elem);
  dlg.DoModal();
}

void 
CTablePropPage1Dlg::OnBnClickedEvents()
{
  HtmlElement* elem = (HtmlElement*)(&m_pTable);
  TagEventsDlg dlg(this,elem,"table");
  dlg.DoModal();
  UpdateData(Data2Controls);}

void 
CTablePropPage1Dlg::OnBnClickedStyle()
{
  if(!m_pTable.HasStyle())
  {
    m_pTable.SetStyle();
  }
  UpdateProperties();
  CString style = m_pTable.GetInlineStyle();
  style = CString("table { ") + style + "}";
  StyleSheetDlg dlg(this,m_base,"table",NULL,style);
  if(dlg.DoModal() == IDOK)
  {
    style = dlg.GetInlineStylesheet();
    style.TrimRight("}");
    style.TrimLeft("table {");
    m_pTable.SetInlineStyle(style);
    ReadProperties();
    UpdateData(Data2Controls);
  }
}

void
CTablePropPage1Dlg::SetRows(int rows)
{
  m_NumRows += rows;
  if(m_initDone)
  {
    UpdateData(Data2Controls);
  }
}

void
CTablePropPage1Dlg::SetColumns(int cols)
{
  m_NumCols = cols;
  if(m_initDone)
  {
    CString ccols;
    ccols.Format("%d",cols);
    CWnd* w = GetDlgItem(IDC_TAB_NUMCOLS);
    w->SetWindowText(ccols);
  }
}

void 
CTablePropPage1Dlg::OnBnClickedCheck1()
{
  m_hasBorderColor = m_buttonCanBorder.GetCheck() == TRUE;
  UpdateData(Data2Controls);
}

void 
CTablePropPage1Dlg::OnBnClickedCheck2()
{
  m_hasDarkBorderColor = m_buttonCanDarkBorder.GetCheck() == TRUE;
  UpdateData(Data2Controls);
}

void 
CTablePropPage1Dlg::OnBnClickedCheck3()
{
  m_hasLightBorderColor = m_buttonCanLightBorder.GetCheck() == TRUE;
  UpdateData(Data2Controls);
}

void 
CTablePropPage1Dlg::OnBnClickedCheck4()
{
  m_hasBackgroundColor = m_buttonCanBackColor.GetCheck() == TRUE;
  UpdateData(Data2Controls);
}
