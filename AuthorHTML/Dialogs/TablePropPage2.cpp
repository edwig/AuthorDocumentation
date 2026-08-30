//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    TablePropPage2.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: property page for the cell attributes of the table
//
#include <stdafx.h>
#include "AuthorHTML.h"
#include "TablePropPage2.h"
#include "TableProperties.h"
#include "GeneralIDDlg.h"
#include "StyleSheetDlg.h"
#include "TagEventsDlg.h"

#define COLOR_DEFAULT_CELLBACKGROUND 0x0ffffff // White

// Forcing true/false to BOOL (performance warning)
#pragma warning(disable: 4800)

CTablePropPage2Dlg::CTablePropPage2Dlg(CWnd*            pParent /*=NULL*/
                                       ,CString         base
                                       ,bool            bNewTable
                                       ,HtmlTable       pTable
                                       ,HtmlTableRow    pRow
                                       ,HtmlTableCell   pCell
                                       ,CComPtr<IHTMLDocument2> doc)
                                       : CDialog(CTablePropPage2Dlg::IDD, pParent)
                                       ,m_bNewTable(bNewTable)
                                       ,m_pTable(pTable)
                                       ,m_pRow(pRow)
                                       ,m_pCell(pCell)
                                       ,m_doc(doc)
                                       ,m_base(base)
{
  // Always start in cell-mode
  m_mode = TP_CellMode;
  m_bInitDone = false;

  m_heightEnable            =
  m_widthEnable             =
  m_verticalEnable          =
  m_horizontalEnable        =
  m_backColorEnable         =
  m_backImageEnable         =
  m_borderColorEnable       =
  m_borderLightColorEnable  =
  m_borderDarkColorEnable   = false;
}

void            
CTablePropPage2Dlg::InitTabForCell(HtmlTableCell cell)
{
  m_heightInPixels = true;
  m_widthInPixels  = true;
  if(m_bNewTable)
  {
    m_CellColumnNum = 0;
    m_maxColumn     = 0;
    m_Height        = 0;
    m_Width         = 0;
  }
  else
  {
    m_maxColumn     = m_pTable.GetRowCount();
    m_CellColumnNum = cell.GetCellIndex() + 1;
    CString height  = cell.GetAttribute("height");
    CString width   = cell.GetAttribute("width");
    m_Height = atoi(height);
    m_Width  = atoi(width);
    if(height.Find("%") >= 0)
    {
      m_heightInPixels = false;
    }
    if(width.Find("%") >= 0)
    {
      m_widthInPixels = false;
    }
    m_vAlign            = cell.GetProperty(HtmlTableCell::E_AlignVert);
    m_hAlign            = cell.GetProperty(HtmlTableCell::E_AlignHorz);
    m_backgroundImage   = cell.GetProperty(HtmlTableCell::E_Background);
    m_backgroundColor   = cell.GetProperty(HtmlTableCell::E_BgColor);
    m_borderColor       = cell.GetProperty(HtmlTableCell::E_BorderColor);
    m_borderColorLight  = cell.GetProperty(HtmlTableCell::E_BorderColorLight);
    m_borderColorDark   = cell.GetProperty(HtmlTableCell::E_BorderColorDark);
    // Set background
    if(m_backgroundColor.IsEmpty())
    {
      m_backColorEnable = false;
      m_buttonBGColor.SetColor(RGB(0xFF,0xFF,0xFF));
    }
    else
    {
      int red,green,blue;
      m_backColorEnable = true;
      Misc::DecodeColor(m_backgroundColor,red,green,blue);
      m_buttonBGColor.SetColor(RGB(red,green,blue));
    }
    // set bordercolor
    if(m_borderColor.IsEmpty())
    {
      m_borderColorEnable = false;
      m_buttonBorderColor.SetColor(RGB(0xFF,0xFF,0xFF));
    }
    else
    {
      int red,green,blue;
      m_borderColorEnable = true;
      Misc::DecodeColor(m_borderColor,red,green,blue);
      m_buttonBorderColor.SetColor(RGB(red,green,blue));
    }
    // Set light bordercolor
    if(m_borderColorLight.IsEmpty())
    {
      m_borderLightColorEnable = false;
      m_buttonBorderLightColor.SetColor(RGB(0xFF,0xFF,0xFF));
    }
    else
    {
      int red,green,blue;
      m_borderLightColorEnable = true;
      Misc::DecodeColor(m_borderColorLight,red,green,blue);
      m_buttonBorderLightColor.SetColor(RGB(red,green,blue));
    }
    // Set dark bordercolor
    if(m_borderColorDark.IsEmpty())
    {
      m_borderDarkColorEnable = false;
      m_buttonBorderDarkColor.SetColor(RGB(0xFF,0xFF,0xFF));
    }
    else
    {
      int red,green,blue;
      m_borderDarkColorEnable = true;
      Misc::DecodeColor(m_borderColorDark,red,green,blue);
      m_buttonBorderDarkColor.SetColor(RGB(red,green,blue));
    }
  }
}

void            
CTablePropPage2Dlg::InitTabForRow(HtmlTableRow p_row)
{
  m_heightInPixels = true;
  m_widthInPixels  = true;
  if(m_bNewTable)
  {
    m_CellColumnNum = 0;
    m_maxColumn     = 0;
    m_Height        = 0;
    m_Width         = 0;
  }
  else
  {
    m_maxColumn     = m_pTable.GetRowCount();
    m_CellColumnNum = p_row.GetRowIndex() + 1;
    CString height  = p_row.GetAttribute("height");
    CString width   = p_row.GetAttribute("width");
    m_Height = atoi(height);
    m_Width  = atoi(width);
    if(height.Find("%") >= 0)
    {
      m_heightInPixels = false;
    }
    if(width.Find("%") >= 0)
    {
      m_widthInPixels = false;
    }
    m_vAlign            = p_row.GetProperty(HtmlTableRow::E_AlignVert);
    m_hAlign            = p_row.GetProperty(HtmlTableRow::E_AlignHorz);
    m_backgroundImage   = p_row.GetProperty(HtmlTableRow::E_Background);
    m_backgroundColor   = p_row.GetProperty(HtmlTableRow::E_BgColor);
    m_borderColor       = p_row.GetProperty(HtmlTableRow::E_BorderColor);
    m_borderColorLight  = p_row.GetProperty(HtmlTableRow::E_BorderColorLight);
    m_borderColorDark   = p_row.GetProperty(HtmlTableRow::E_BorderColorDark);
    // Decode background
    if(m_backgroundColor.IsEmpty())
    {
      m_backColorEnable = false;
      m_buttonBGColor.SetColor(RGB(0xFF,0xFF,0xFF));
    }
    else
    {
      int red,green,blue;
      m_backColorEnable = true;
      Misc::DecodeColor(m_backgroundColor,red,green,blue);
      m_buttonBGColor.SetColor(RGB(red,green,blue));
    }
    // set bordercolor
    if(m_borderColor.IsEmpty())
    {
      m_borderColorEnable = false;
      m_buttonBorderColor.SetColor(RGB(0xFF,0xFF,0xFF));
    }
    else
    {
      int red,green,blue;
      m_borderColorEnable = true;
      Misc::DecodeColor(m_borderColor,red,green,blue);
      m_buttonBorderColor.SetColor(RGB(red,green,blue));
    }
    // Set light bordercolor
    if(m_borderColorLight.IsEmpty())
    {
      m_borderLightColorEnable = false;
      m_buttonBorderLightColor.SetColor(RGB(0xFF,0xFF,0xFF));
    }
    else
    {
      int red,green,blue;
      m_borderLightColorEnable = true;
      Misc::DecodeColor(m_borderColorLight,red,green,blue);
      m_buttonBorderLightColor.SetColor(RGB(red,green,blue));
    }
    // Set dark bordercolor
    if(m_borderColorDark.IsEmpty())
    {
      m_borderDarkColorEnable = false;
      m_buttonBorderDarkColor.SetColor(RGB(0xFF,0xFF,0xFF));
    }
    else
    {
      int red,green,blue;
      m_borderDarkColorEnable = true;
      Misc::DecodeColor(m_borderColorDark,red,green,blue);
      m_buttonBorderDarkColor.SetColor(RGB(red,green,blue));
    }
  }
}

void CTablePropPage2Dlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CTablePropPage2Dlg)
  //}}AFX_DATA_MAP
  DDX_Control   (pDX,IDC_CELLAPPLY,   m_cellSelectCtrl);
  DDV_MinMaxUInt(pDX,IDC_CELLNUM,     1, 32000);
  DDX_Control   (pDX,IDC_PREVCELL,    m_previousButton);
  DDX_Control   (pDX,IDC_NEXTCELL,    m_nextButton);
  DDX_Control   (pDX,IDC_SPINCELLNUM, m_cellCtrl);

  DDX_Control   (pDX,IDC_CHECKHEIGHT, m_checkHeight);
  DDX_Control   (pDX,IDC_CHECKWIDTH,  m_checkWidth);

  DDX_Text      (pDX,IDC_COLHEIGHT,   m_Height);
  DDX_Text      (pDX,IDC_COLWIDTH,    m_Width);
  DDV_MinMaxUInt(pDX,IDC_COLHEIGHT,   0,32000);
  DDV_MinMaxUInt(pDX,IDC_COLWIDTH,    0,32000);
  DDX_Control   (pDX,IDC_SPIN1,       m_heightCtrl);
  DDX_Control   (pDX,IDC_SPIN2,       m_widthCtrl);

  DDX_Control   (pDX,IDC_FORMATHEIGHT,              m_formatHeightCtrl);
  DDX_Control   (pDX,IDC_FORMATWIDTH,               m_formatWidthCtrl);

  DDX_Control   (pDX,IDC_CHECKVERTICAL,             m_checkVertical);
  DDX_Control   (pDX,IDC_CHECKHORIZONTAL,           m_checkHorizontal);
  DDX_Control   (pDX,IDC_ALIGNVERT,                 m_alignVertical);
  DDX_Control   (pDX,IDC_ALIGNHORZ,                 m_alignHorizontal);

  DDX_Control   (pDX,IDC_BUTT_BGCOLORCELL,          m_buttonBGColor);
  DDX_Control   (pDX,IDC_CHECKBACKCOLOR,            m_checkBackColor);
  DDX_Control   (pDX,IDC_CHECKBACKIMAGE,            m_checkBackImage);
  DDX_Control   (pDX,IDC_CHOOSE_CELLIMAGE,          m_chooseImage);

  DDX_Control   (pDX,IDC_CHECKBORDERCOLOR,          m_checkBorderColor);
  DDX_Control   (pDX,IDC_BUTT_BORDERCOLORCELL,      m_buttonBorderColor);
  DDX_Control   (pDX,IDC_CHECKBORDERLIGHTCOLOR,     m_checkBorderLightColor);
  DDX_Control   (pDX,IDC_BUTT_BORDERLIGHTCOLORCELL, m_buttonBorderLightColor);
  DDX_Control   (pDX,IDC_CHECKBORDERDARKCOLOR,      m_checkBorderDarkColor);
  DDX_Control   (pDX,IDC_BUTT_BORDERDARKCOLORCELL,  m_buttonBorderDarkColor);

  DDX_Control   (pDX,IDC_ID,                        m_buttonID);
  DDX_Control   (pDX,IDC_EVENTS,                    m_buttonEvents);
  DDX_Control   (pDX,IDC_STYLE,                     m_buttonStyle);

  if(pDX->m_bSaveAndValidate == Data2Controls)
  {
    if(m_pCell.Valid())
    {
      CString def,text;

      m_buttonID.GetWindowText(def);
      text = m_pCell.HasIdentity() ? "[ &ID ]" : "&ID";
      if(def != text) m_buttonID.SetWindowText(text);

      m_buttonStyle.GetWindowText(def);
      text = m_pCell.HasStyle() ? "[ &Style ]" : "&Style";
      if(def != text) m_buttonStyle.SetWindowText(text);

      m_buttonEvents.GetWindowText(def);
      text = m_pCell.HasEvents() ? "[ &Events ]" : "&Events";
      if(def != text) m_buttonEvents.SetWindowText(text);
    }
    else
    {
      // Newly defined tag. No style possible yet.
      m_buttonStyle .EnableWindow(FALSE);
      m_buttonEvents.EnableWindow(FALSE);
      m_buttonID    .EnableWindow(FALSE);
    }
    m_checkBackColor.SetCheck(m_backColorEnable);
    m_buttonBGColor.EnableWindow(m_backColorEnable);

    m_checkBorderColor.SetCheck(m_borderColorEnable);
    m_buttonBorderColor.EnableWindow(m_borderColorEnable);

    m_checkBorderLightColor.SetCheck(m_borderLightColorEnable);
    m_buttonBorderLightColor.EnableWindow(m_borderLightColorEnable);

    m_checkBorderDarkColor.SetCheck(m_borderDarkColorEnable);
    m_buttonBorderDarkColor.EnableWindow(m_borderDarkColorEnable);
  }
}

BEGIN_MESSAGE_MAP(CTablePropPage2Dlg, CDialog)
  //{{AFX_MSG_MAP(CTablePropPage2Dlg)
  //}}AFX_MSG_MAP
  ON_CBN_SELCHANGE  (IDC_CELLAPPLY,        OnCbnSelchangeCellapply)
  ON_EN_CHANGE      (IDC_CELLNUM,          OnEnChangeCellnum)
  ON_BN_CLICKED     (IDC_PREVCELL,         OnBnClickedPrevcell)
  ON_BN_CLICKED     (IDC_NEXTCELL,         OnBnClickedNextcell)
  ON_BN_CLICKED     (IDC_CHECKHEIGHT,      OnBnClickedCheckheight)
  ON_BN_CLICKED     (IDC_CHECKWIDTH,       OnBnClickedCheckwidth)
  ON_CBN_SELCHANGE  (IDC_FORMATHEIGHT,     OnCbnSelchangeFormatheight)
  ON_CBN_SELCHANGE  (IDC_FORMATWIDTH,      OnCbnSelchangeFormatwidth)
  ON_BN_CLICKED     (IDC_CHECKVERTICAL,    OnBnClickedCheckvertical)
  ON_BN_CLICKED     (IDC_CHECKHORIZONTAL,  OnBnClickedCheckhorizontal)
  ON_CBN_SELCHANGE  (IDC_ALIGNVERT,        OnCbnSelchangeAlignvert)
  ON_CBN_SELCHANGE  (IDC_ALIGNHORZ,        OnCbnSelchangeAlignhorz)
  ON_BN_CLICKED     (IDC_BUTT_BGCOLORCELL, OnBnClickedButtBgcolorcell)
  ON_BN_CLICKED     (IDC_CHECKBACKCOLOR,   OnBnClickedCheckbackcolor)
  ON_BN_CLICKED     (IDC_CHECKBACKIMAGE,   OnBnClickedCheckbackimage)
  ON_BN_CLICKED     (IDC_ID,               OnBnClickedId)
  ON_BN_CLICKED     (IDC_EVENTS,           OnBnClickedEvents)
  ON_BN_CLICKED     (IDC_STYLE,            OnBnClickedStyle)

  ON_BN_CLICKED(IDC_CHECKBORDERCOLOR,         OnBnClickedCheckbordercolor)
  ON_BN_CLICKED(IDC_BUTT_BORDERCOLORCELL,     OnBnClickedButtBordercolorcell)
  ON_BN_CLICKED(IDC_CHECKBORDERLIGHTCOLOR,    OnBnClickedCheckborderlightcolor)
  ON_BN_CLICKED(IDC_BUTT_BORDERLIGHTCOLORCELL,OnBnClickedButtBorderlightcolorcell)
  ON_BN_CLICKED(IDC_CHECKBORDERDARKCOLOR,     OnBnClickedCheckborderdarkcolor)
  ON_BN_CLICKED(IDC_BUTT_BORDERDARKCOLORCELL, OnBnClickedButtBorderdarkcolorcell)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTablePropPage2Dlg message handlers

BOOL
CTablePropPage2Dlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  // Cell Select
  if(m_bNewTable)
  {
    m_cellSelectCtrl.ResetContent();
    m_cellSelectCtrl.AddString("All cells");
    m_cellSelectCtrl.SetCurSel(0);
    m_cellSelectCtrl.EnableWindow(FALSE);

    m_previousButton.EnableWindow(FALSE);
    m_nextButton.    EnableWindow(FALSE);

    CWnd* cellnum = GetDlgItem(IDC_CELLNUM);
    cellnum->EnableWindow(FALSE);
  }
  else
  {
    // Data = "Cell;Column;Row" -> Set to "Cell"
    m_cellSelectCtrl.SetCurSel(0);
  }
  m_heightCtrl.SetRange(0,32000);
  m_widthCtrl. SetRange(0,32000);
  m_cellCtrl  .SetRange(1,32000);
  m_heightCtrl.SetBase(10);
  m_widthCtrl. SetBase(10);
  m_cellCtrl  .SetBase(10);
  m_heightCtrl.SetPos(m_Height);
  m_widthCtrl. SetPos(m_Width);
  m_cellCtrl.  SetPos(m_CellColumnNum);

  m_formatHeightCtrl.SetCurSel(m_heightInPixels ? 0 : 1);
  m_formatWidthCtrl .SetCurSel(m_widthInPixels  ? 0 : 1);

  OnBnClickedCheckheight();
  OnBnClickedCheckwidth();
  OnBnClickedCheckvertical();
  OnBnClickedCheckhorizontal();
  OnBnClickedCheckbackcolor();
  OnBnClickedCheckbackimage();

  // Top,middle,bottom,baseline
  int valign = 1; // Middle is default
  if(m_vAlign.CompareNoCase("Top")      == 0)  valign = 0;
  if(m_vAlign.CompareNoCase("Middle")   == 0)  valign = 1;
  if(m_vAlign.CompareNoCase("Bottom")   == 0)  valign = 2;
  if(m_vAlign.CompareNoCase("Baseline") == 0)  valign = 3;
  m_alignVertical.SetCurSel(valign); 

  // Get horizontal alignment (Left, center, right, justify)
  int halign = 0;
  if(m_hAlign.CompareNoCase("Left")   == 0) halign = 0;
  if(m_hAlign.CompareNoCase("Center") == 0) halign = 1;
  if(m_hAlign.CompareNoCase("Right")  == 0) halign = 2;
  if(m_hAlign.CompareNoCase("Justify")== 0) halign = 3;
  m_alignHorizontal.SetCurSel(halign);

  m_buttonBGColor.EnableOtherButton(_T("More colors..."));
  m_buttonBGColor.SetColor(COLOR_DEFAULT_CELLBACKGROUND);

  switch(m_mode)
  {
    case TP_CellMode:   InitTabForCell(m_pCell);  break;
    case TP_RowMode:    InitTabForRow(m_pRow);    break;
    case TP_ColumnMode: break; // TODO 
  }
  UpdateData(FALSE);

  m_bInitDone = true;
  return TRUE;
}

CString
CTablePropPage2Dlg::CreateOneCell(int rows,int columns)
{
  CString heightString;
  CString widthString;
  CString horizString;
  CString vertiString;
  CString background;
  CString image;
  CString border;
  CString borderLight;
  CString borderDark;
  
  // Make sure we have everything
  UpdateData(Controls2Data);

  // Caculate default height/width on basis of rows/columns
  UINT defHeight = 100 / rows;
  UINT defWidth  = 100 / columns;

  if(m_heightEnable)
  {
    if(m_heightInPixels)
    {
      int height;
      height = min(m_Height,defHeight);
      heightString.Format("%d",height);
    }
    else
    {
      heightString.Format("%d%%",m_Height);
    }
  }
  else
  {
    // Default height in % of table by num-of-rows
    heightString.Format("%d%%",defHeight);
  }
  heightString = " height=\"" + heightString + "\"";
  if(m_widthEnable)
  {
    if(m_widthInPixels)
    {
      int width;
      width = min(m_Width,defWidth);
      widthString.Format("%d",width);
    }
    else
    {
      widthString.Format("%d%%",m_Width);
    }
  }
  else
  {
    // Default width in % of table by num-of-columns
    widthString.Format("%d%%",defWidth);
  }
  widthString = " width=\"" + widthString + "\"";

  // Horizontal / Vertical justification
  if(m_horizontalEnable)
  {
    horizString = " align=\"" + m_vAlign + "\"";
  }
  if(m_verticalEnable)
  {
    vertiString = " valign=\"" + m_hAlign + "\"";
  }
  if(m_backColorEnable)
  {
    background = " bgcolor=\"" + m_backgroundColor + "\"";
  }
  if(m_backImageEnable)
  {
    image = " background=\"" + m_backgroundImage + "\"";
  }
  if(m_borderColorEnable)
  {
    border = " bordercolor=\"" + m_borderColor + "\"";
  }
  if(m_borderLightColorEnable)
  {
    borderLight = " bordercolorlight=\"" + m_borderColorLight + "\"";
  }
  if(m_borderDarkColorEnable)
  {
    borderDark = " bordercolordark=\"" + m_borderColorDark + "\"";
  }
  // Now build the single table data (TD) string
  CString colDef = "<TD" + 
                   heightString + widthString + 
                   vertiString  + horizString + 
                   background   + image + 
                   border       + borderLight + 
                   borderDark   + 
                   "><P></P></TD>";
  return colDef;
}

// Comes from the APPLY button on the TableProperties dialog
void
CTablePropPage2Dlg::UpdateProperties()
{
  switch(m_mode)
  {
    case TP_CellMode:  UpdateCellProperties(m_pCell); break;
    case TP_RowMode:   UpdateRowProperties(m_pRow);   break;
  }
}

void
CTablePropPage2Dlg::UpdateCellProperties(HtmlTableCell pCell)
{
  int red,green,blue;

  // Height update
  CString heightString;
  if(m_heightEnable)
  {
    if(m_heightInPixels)
    {
      heightString.Format("%d",m_Height);
    }
    else
    {
      heightString.Format("%d%%",m_Height);
    }
  }
  pCell.SetAttribute("height",heightString);

  // Width update
  CString widthString;
  if(m_widthEnable)
  {
    if(m_widthInPixels)
    {
      widthString.Format("%d",m_Width);
    }
    else
    {
      widthString.Format("%d%%",m_Width);
    }
  }
  pCell.SetAttribute("width",widthString);

  // Horizontal / Vertical justification
  if(!m_horizontalEnable)
  {
    m_hAlign = "";
  }
  pCell.SetProperty(HtmlTableCell::E_AlignHorz,m_hAlign);
  if(!m_verticalEnable)
  {
    m_vAlign = "";
  }
  pCell.SetProperty(HtmlTableCell::E_AlignVert,m_vAlign);

  // Set or reset the background color
  if(m_backColorEnable)
  {
    Misc::DecodeColor(m_backgroundColor,red,green,blue);
    m_backgroundColor.Format("#%02x%02x%02x",red,green,blue);
  }
  else
  {
    m_backgroundColor = "";
  }
  pCell.SetProperty(HtmlTableCell::E_BgColor,m_backgroundColor);

  // Backgorund image
  if(!m_backImageEnable)
  {
    m_backgroundImage = "";
  }
  pCell.SetProperty(HtmlTableCell::E_Background,m_backgroundImage);

  // Set or reset the border color
  if(m_borderColorEnable)
  {
    Misc::DecodeColor(m_borderColor,red,green,blue);
    m_borderColor.Format("#%02x%02x%02x",red,green,blue);
  }
  else
  {
    m_borderColor = "";
  }
  pCell.SetProperty(HtmlTableCell::E_BorderColor,m_borderColor);

  // Set or reset the bordercolorLight
  if(m_borderLightColorEnable)
  {
    Misc::DecodeColor(m_borderColorLight,red,green,blue);
    m_borderColorLight.Format("#%02x%02x%02x",red,green,blue);
  }
  else
  {
    m_borderColorLight = "";
  }
  pCell.SetProperty(HtmlTableCell::E_BorderColorLight,m_borderColorLight);

  // Set or reset the bordercolorDark
  if(m_borderDarkColorEnable)
  {
    Misc::DecodeColor(m_borderColorDark,red,green,blue);
    m_borderColorDark.Format("#%02x%02x%02x",red,green,blue);
  }
  else
  {
    m_borderColorDark = "";
  }
  pCell.SetProperty(HtmlTableCell::E_BorderColorDark,m_borderColorDark);
}

void
CTablePropPage2Dlg::UpdateRowProperties(HtmlTableRow pRow)
{
  int red,green,blue;

  // Height update
  CString heightString;
  if(m_heightEnable)
  {
    if(m_heightInPixels)
    {
      heightString.Format("%d",m_Height);
    }
    else
    {
      heightString.Format("%d%%",m_Height);
    }
  }
  pRow.SetAttribute("height",heightString);

  // Width update
  CString widthString;
  if(m_widthEnable)
  {
    if(m_widthInPixels)
    {
      widthString.Format("%d",m_Width);
    }
    else
    {
      widthString.Format("%d%%",m_Width);
    }
  }
  pRow.SetAttribute("width",widthString);

  // Horizontal / Vertical justification
  if(!m_horizontalEnable)
  {
    m_hAlign = "";
  }
  pRow.SetProperty(HtmlTableRow::E_AlignHorz,m_hAlign);
  if(!m_verticalEnable)
  {
    m_vAlign = "";
  }
  pRow.SetProperty(HtmlTableRow::E_AlignVert,m_vAlign);

  // Set or reset the background color
  if(m_backColorEnable)
  {
    Misc::DecodeColor(m_backgroundColor,red,green,blue);
    m_backgroundColor.Format("#%02x%02x%02x",red,green,blue);
  }
  else
  {
    m_backgroundColor = "";
  }
  pRow.SetProperty(HtmlTableRow::E_BgColor,m_backgroundColor);

  // Backgorund image
  if(!m_backImageEnable)
  {
    m_backgroundImage = "";
  }
  pRow.SetProperty(HtmlTableRow::E_Background,m_backgroundImage);

  // Set or reset the border color
  if(m_borderColorEnable)
  {
    Misc::DecodeColor(m_borderColor,red,green,blue);
    m_borderColor.Format("#%02x%02x%02x",red,green,blue);
  }
  else
  {
    m_borderColor = "";
  }
  pRow.SetProperty(HtmlTableRow::E_BorderColor,m_borderColor);

  // Set or reset the bordercolorLight
  if(m_borderLightColorEnable)
  {
    Misc::DecodeColor(m_borderColorLight,red,green,blue);
    m_borderColorLight.Format("#%02x%02x%02x",red,green,blue);
  }
  else
  {
    m_borderColorLight = "";
  }
  pRow.SetProperty(HtmlTableRow::E_BorderColorLight,m_borderColorLight);

  // Set or reset the bordercolorDark
  if(m_borderDarkColorEnable)
  {
    Misc::DecodeColor(m_borderColorDark,red,green,blue);
    m_borderColorDark.Format("#%02x%02x%02x",red,green,blue);
  }
  else
  {
    m_borderColorDark = "";
  }
  pRow.SetProperty(HtmlTableRow::E_BorderColorDark,m_borderColorDark);
}

// Commits al changes from the property page
// to the table. Use before changing row/column/cell
void
CTablePropPage2Dlg::CommitUpdates()
{
  if(m_mode == TP_CellMode)
  {
    // Update current cell
    UpdateCellProperties(m_pCell);
  }
  if(m_mode == TP_RowMode)
  {
    // Update row properties
    UpdateRowProperties(m_pRow);
  }
  if(m_mode == TP_ColumnMode)
  {
    // Update the column
    int rows = m_pTable.GetRowCount();
    int colm = m_pCell.GetColIndex();
    for(int ind = 0;ind < rows; ++ind)
    {
      HtmlTableRow  row = m_pTable.GetRow(ind);
      HtmlTableCell cell = row.GetColumn(colm);
      if(cell.Valid())
      {
        UpdateCellProperties(cell);
      }
    }
  }
}

// SELECT cell/row/column mode
//
void CTablePropPage2Dlg::OnCbnSelchangeCellapply()
{
  CString units;
  int ind = m_cellSelectCtrl.GetCurSel();
  m_cellSelectCtrl.GetLBText(ind,units);

  // Commit latest changes
  CommitUpdates();
  if(units == "Cell")   
  {
    m_mode = TP_CellMode;
    m_CellColumnNum = m_pCell.GetCellIndex() + 1;
  }
  if(units == "Column") 
  {
    m_mode = TP_ColumnMode;
    m_CellColumnNum = m_pCell.GetColIndex() + 1;
  }
  if(units == "Row")    
  {
    m_mode = TP_RowMode;
    m_CellColumnNum = m_pRow.GetRowIndex() + 1;
  }
  CString sNum;
  CWnd* wnd = GetDlgItem(IDC_CELLNUM);
  sNum.Format("%d",m_CellColumnNum);
  wnd->SetWindowText(sNum);
}

void CTablePropPage2Dlg::OnEnChangeCellnum()
{
  CString sNum;
  UINT    max;

  if(!m_bInitDone)
  {
    // Not ready yet
    return;
  }
  CommitUpdates();
  // Get new number from the control
  // m_CellColumnNum not yet set!
  CWnd* wnd = GetDlgItem(IDC_CELLNUM);
  wnd->GetWindowText(sNum);
  UINT num = atoi(sNum);

  switch(m_mode)
  {
    case TP_CellMode:
    case TP_ColumnMode:
         max = m_pCell.GetParentRow().GetCellCount();
         if(num > 0 && num <= max)
         {
           HtmlTableRow row = m_pCell.GetParentRow();
           m_pCell = row.GetCell(num - 1);
           InitTabForCell(m_pCell);
           UpdateData(Data2Controls);
           Misc::MoveCaretToElement(m_doc,m_pCell,CARET_DIRECTION_INDETERMINATE);
         }
         else if(num)
         {
           theApp.ErrorMessage("Number of columns in this table exceeded");
           --num;
           sNum.Format("%d",num);
           wnd->SetWindowText(sNum);
           return;
         }
         else
         {
           theApp.ErrorMessage("Column number 0 not allowed");
           ++num;
           sNum.Format("%d",num);
           wnd->SetWindowText(sNum);
           return;
         }
         break;
    case TP_RowMode:
         max = m_pTable.GetRowCount();
         if(num > 0 && num <= max)
         {
           m_pRow = m_pTable.GetRow(num - 1);
           UINT cellnum = m_pCell.GetCellIndex();
           m_pCell = m_pRow.GetCell(cellnum);
           //InitTabForCell(m_pCell);
           InitTabForRow(m_pRow);
           UpdateData(Data2Controls);
           Misc::MoveCaretToElement(m_doc,m_pCell,CARET_DIRECTION_INDETERMINATE);
         }
         else if(num)
         {
           theApp.ErrorMessage("Number of rows in this table exceeded");
           --num;
           sNum.Format("%d",num);
           wnd->SetWindowText(sNum);
           return;
         }
         else
         {
           theApp.ErrorMessage("Row number 0 not allowed");
           ++num;
           sNum.Format("%d",num);
           wnd->SetWindowText(sNum);
           return;
         }
         break;
  }
}

void CTablePropPage2Dlg::OnBnClickedPrevcell()
{
  CString sNum;
  CWnd* wnd = GetDlgItem(IDC_CELLNUM);
  wnd->GetWindowText(sNum);
  UINT num = atoi(sNum);
  --num;
  sNum.Format("%d",num);
  wnd->SetWindowText(sNum);
  OnEnChangeCellnum();
}

void 
CTablePropPage2Dlg::OnBnClickedNextcell()
{
  CString sNum;
  CWnd* wnd = GetDlgItem(IDC_CELLNUM);
  wnd->GetWindowText(sNum);
  UINT num = atoi(sNum);
  ++num;
  sNum.Format("%d",num);
  wnd->SetWindowText(sNum);
  OnEnChangeCellnum();
}

void 
CTablePropPage2Dlg::OnBnClickedCheckheight()
{
  CWnd* wnd1 = GetDlgItem(IDC_COLHEIGHT);
  CWnd* wnd2 = GetDlgItem(IDC_FORMATHEIGHT);
  BOOL  enab = m_checkHeight.GetState() & 0x003;
  wnd1->EnableWindow(enab);
  wnd2->EnableWindow(enab);
  m_heightEnable = enab;
}

void 
CTablePropPage2Dlg::OnBnClickedCheckwidth()
{
  CWnd* wnd1 = GetDlgItem(IDC_COLWIDTH);
  CWnd* wnd2 = GetDlgItem(IDC_FORMATWIDTH);
  BOOL  enab = m_checkWidth.GetState() & 0x003;
  wnd1->EnableWindow(enab);
  wnd2->EnableWindow(enab);
  m_widthEnable = enab;
}

void 
CTablePropPage2Dlg::OnCbnSelchangeFormatheight()
{
  CString units;
  int ind = m_formatHeightCtrl.GetCurSel();
  m_formatHeightCtrl.GetLBText(ind,units);
  m_heightInPixels = (units == "Pixels") ? true : false;
}

void 
CTablePropPage2Dlg::OnCbnSelchangeFormatwidth()
{
  CString units;
  int ind = m_formatWidthCtrl.GetCurSel();
  m_formatWidthCtrl.GetLBText(ind,units);
  m_widthInPixels = (units == "Pixels") ? true : false;
}

void 
CTablePropPage2Dlg::OnBnClickedCheckvertical()
{
  CWnd* wnd1 = GetDlgItem(IDC_ALIGNVERT);
  BOOL  enab = m_checkVertical.GetState() & 0x003;
  wnd1->EnableWindow(enab);
  m_verticalEnable = enab;
}

void 
CTablePropPage2Dlg::OnBnClickedCheckhorizontal()
{
  CWnd* wnd1 = GetDlgItem(IDC_ALIGNHORZ);
  BOOL  enab = m_checkHorizontal.GetState() & 0x003;
  wnd1->EnableWindow(enab);
  m_horizontalEnable = enab;
}

void 
CTablePropPage2Dlg::OnCbnSelchangeAlignvert()
{
  CString align;
  int ind = m_alignVertical.GetCurSel();
  m_alignVertical.GetLBText(ind,align);
  m_vAlign = align;
}

void 
CTablePropPage2Dlg::OnCbnSelchangeAlignhorz()
{
  CString align;
  int ind = m_alignHorizontal.GetCurSel();
  m_alignHorizontal.GetLBText(ind,align);
  m_hAlign = align;
}

void 
CTablePropPage2Dlg::OnBnClickedButtBgcolorcell()
{
  COLORREF col = m_buttonBGColor.GetColor();
  int red   = GetRValue(col);
  int green = GetGValue(col);
  int blue  = GetBValue(col);
  m_backgroundColor.Format("#%02x%02x%02x",red,green,blue);
}

void 
CTablePropPage2Dlg::OnBnClickedCheckbackcolor()
{
  CWnd* wnd1 = GetDlgItem(IDC_BUTT_BGCOLORCELL);
  BOOL  enab = m_checkBackColor.GetState() & 0x003;
  wnd1->EnableWindow(enab);
  m_backColorEnable = enab;
}

void CTablePropPage2Dlg::OnBnClickedCheckbackimage()
{
  CWnd* wnd1 = GetDlgItem(IDC_EDITCELLIMAGE);
  CWnd* wnd2 = GetDlgItem(IDC_CHOOSE_CELLIMAGE);
  BOOL  enab = m_checkBackImage.GetState() & 0x003;
  wnd1->EnableWindow(enab);
  wnd2->EnableWindow(enab);
  m_backImageEnable = enab;
}

void 
CTablePropPage2Dlg::OnBnClickedCheckbordercolor()
{
  CWnd* wnd1 = GetDlgItem(IDC_BUTT_BORDERCOLORCELL);
  BOOL  enab = m_checkBorderColor.GetState() & 0x003;
  wnd1->EnableWindow(enab);
  m_borderColorEnable = enab;
}

void 
CTablePropPage2Dlg::OnBnClickedButtBordercolorcell()
{
  COLORREF col = m_buttonBorderColor.GetColor();
  int red   = GetRValue(col);
  int green = GetGValue(col);
  int blue  = GetBValue(col);
  m_borderColor.Format("#%02x%02x%02x",red,green,blue);
}

void 
CTablePropPage2Dlg::OnBnClickedCheckborderlightcolor()
{
  CWnd* wnd1 = GetDlgItem(IDC_BUTT_BORDERLIGHTCOLORCELL);
  BOOL  enab = m_checkBorderLightColor.GetState() & 0x003;
  wnd1->EnableWindow(enab);
  m_borderLightColorEnable = enab;
}

void 
CTablePropPage2Dlg::OnBnClickedButtBorderlightcolorcell()
{
  COLORREF col = m_buttonBorderLightColor.GetColor();
  int red   = GetRValue(col);
  int green = GetGValue(col);
  int blue  = GetBValue(col);
  m_borderColorLight.Format("#%02x%02x%02x",red,green,blue);
}

void 
CTablePropPage2Dlg::OnBnClickedCheckborderdarkcolor()
{
  CWnd* wnd1 = GetDlgItem(IDC_BUTT_BORDERDARKCOLORCELL);
  BOOL  enab = m_checkBorderDarkColor.GetState() & 0x003;
  wnd1->EnableWindow(enab);
  m_borderDarkColorEnable = enab;
}

void 
CTablePropPage2Dlg::OnBnClickedButtBorderdarkcolorcell()
{
  COLORREF col = m_buttonBorderDarkColor.GetColor();
  int red   = GetRValue(col);
  int green = GetGValue(col);
  int blue  = GetBValue(col);
  m_borderColorDark.Format("#%02x%02x%02x",red,green,blue);
}

void 
CTablePropPage2Dlg::OnBnClickedId()
{
  HtmlElement* elem = (HtmlElement*)(&m_pCell);
  GeneralIDDlg dlg(this,"td",elem);
  dlg.DoModal();
}

void 
CTablePropPage2Dlg::OnBnClickedEvents()
{
  HtmlElement* elem = (HtmlElement*)(&m_pCell);
  TagEventsDlg dlg(this,elem,"TD");
  dlg.DoModal();
  UpdateData(Data2Controls);
}

void 
CTablePropPage2Dlg::OnBnClickedStyle()
{
  if(!m_pCell.HasStyle())
  {
    m_pCell.SetStyle();
  }
  UpdateProperties();
  CString style = m_pCell.GetInlineStyle();

  // debug
  style.MakeLower();

  style = CString("td { ") + style + "}";
  StyleSheetDlg dlg(this,m_base,"td",NULL,style);
  if(dlg.DoModal() == IDOK)
  {
    style = dlg.GetInlineStylesheet();
    style.TrimRight("}");
    style.TrimLeft("td {");
    m_pCell.SetInlineStyle(style);
    InitTabForCell(m_pCell);
    UpdateData(Data2Controls);
  }
}

