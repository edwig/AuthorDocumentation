//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    TablePropPage3.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: property page for the header/footer attributes of the table
//
#include "stdafx.h"
#include "AuthorHTML.h"
#include "TablePropPage3.h"
#include "TableProperties.h"

// CTablePropPage3Dlg dialog
IMPLEMENT_DYNAMIC(CTablePropPage3Dlg, CDialog)

CTablePropPage3Dlg::CTablePropPage3Dlg(CWnd* pParent
                              ,CString base
                              ,HtmlTable table)
               :CDialog(CTablePropPage3Dlg::IDD, pParent)
               ,m_base(base)
               ,m_table(table)
               ,m_hasCaption(false)
               ,m_hasHeader(false)
               ,m_hasFooter(false)
               ,m_caption(NULL)
               ,m_header(NULL)
               ,m_footer(NULL)
               ,m_initDone(false)
{
}

CTablePropPage3Dlg::~CTablePropPage3Dlg()
{
}

void CTablePropPage3Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
  // Caption
  DDX_Control(pDX,IDC_HAS_CAPTION, m_buttonHasCaption);
  DDX_Control(pDX,IDC_CAPTION_POS, m_comboAlign);
  DDX_Text   (pDX,IDC_CAPTION_TEXT,m_text);
  DDX_Control(pDX,IDC_BUTT_CAPTION,m_buttonCaption);
  // Header
  DDX_Control(pDX,IDC_HAS_HEADER,  m_buttonHasHeader);
  DDX_Text   (pDX,IDC_HEAD_ROWS,   m_hRows);
  DDX_Text   (pDX,IDC_HEAD_COLS,   m_columns);
  DDX_Control(pDX,IDC_BUTT_HEADER, m_buttonHeader);
  // FOOTER
  DDX_Control(pDX,IDC_HAS_FOOTER,  m_buttonHasFooter);
  DDX_Text   (pDX,IDC_FOOT_ROWS,   m_fRows);
  DDX_Text   (pDX,IDC_FOOT_COLS,   m_columns);
  DDX_Control(pDX,IDC_BUTT_FOOTER, m_buttonFooter);

  if(pDX->m_bSaveAndValidate == Data2Controls)
  {
    // CAPTION
    int ind;
    ind = m_comboAlign.FindString(-1,m_vAlign);
          m_comboAlign.SetCurSel(ind);

    if(m_caption.Valid())
    {
      m_buttonCaption.SetWindowText("Remove &caption");
    }
    else
    {
      m_buttonCaption.SetWindowText("Make &caption");
    }
    m_buttonHasCaption.SetCheck(m_hasCaption);
    CWnd* w = GetDlgItem(IDC_CAPTION_TEXT);
    w->EnableWindow(!m_caption.Valid());

    m_buttonCaption.EnableWindow(m_table.Valid());

    // HEADER
    if(m_header.Valid())
    {
      m_buttonHeader.SetWindowText("Remove &header");
    }
    else
    {
      m_buttonHeader.SetWindowText("Make &header");
    }
    m_buttonHasHeader.SetCheck(m_hasHeader);
    m_buttonHeader.EnableWindow(m_table.Valid());

    // FOOTER
    if(m_footer.Valid())
    {
      m_buttonFooter.SetWindowText("Remove &footer");
    }
    else
    {
      m_buttonFooter.SetWindowText("Make &footer");
    }
    m_buttonHasFooter.SetCheck(m_hasFooter);
    m_buttonFooter.EnableWindow(m_table.Valid());

    // Visibility caption
    CWnd* t = GetDlgItem(IDC_CAPTION_TEXT);
    m_comboAlign.EnableWindow(m_hasCaption);
    m_buttonCaption.EnableWindow(m_hasCaption);
    t->EnableWindow(m_hasCaption);
    // Visibility header
    CWnd* r = GetDlgItem(IDC_HEAD_ROWS);
    CWnd* c = GetDlgItem(IDC_HEAD_COLS);
    m_buttonHeader.EnableWindow(m_hasHeader);
    r->EnableWindow(m_hasHeader);
    c->EnableWindow(m_hasHeader);
    // Visibility footer
    r = GetDlgItem(IDC_FOOT_ROWS);
    c = GetDlgItem(IDC_FOOT_COLS);
    m_buttonFooter.EnableWindow(m_hasFooter);
    r->EnableWindow(m_hasFooter);
    c->EnableWindow(m_hasFooter);
  }
}

BEGIN_MESSAGE_MAP(CTablePropPage3Dlg, CDialog)
  // Caption
  ON_BN_CLICKED   (IDC_HAS_CAPTION,   OnBnClickedHasCaption)
  ON_CBN_SELCHANGE(IDC_CAPTION_POS,   OnCbnSelchangeCaptionPos)
  ON_EN_KILLFOCUS (IDC_CAPTION_TEXT,  OnEnChangeCaptionText)
  ON_BN_CLICKED   (IDC_BUTT_CAPTION,  OnBnClickedButtCaption)
  // HEADER
  ON_BN_CLICKED   (IDC_HAS_HEADER,    OnBnClickedHasHeader)
  ON_EN_KILLFOCUS (IDC_HEAD_ROWS,     OnEnChangeHeadRows)
  ON_EN_KILLFOCUS (IDC_HEAD_COLS,     OnEnChangeHeadCols)
  ON_BN_CLICKED   (IDC_BUTT_HEADER,   OnBnClickedButtHeader)
  // FOOTER
  ON_BN_CLICKED   (IDC_HAS_FOOTER,    OnBnClickedHasFooter)
  ON_EN_KILLFOCUS (IDC_FOOT_ROWS,     OnEnChangeFootRows)
  ON_EN_KILLFOCUS (IDC_FOOT_COLS,     OnEnChangeFootCols)
  ON_BN_CLICKED   (IDC_BUTT_FOOTER,   OnBnClickedButtFooter)
END_MESSAGE_MAP()

BOOL
CTablePropPage3Dlg::OnInitDialog()
{
  CDialog::OnInitDialog();
  FillPage();
  UpdateData(Data2Controls);
  m_initDone = true;
  return TRUE;
}

void
CTablePropPage3Dlg::FillPage()
{
  if(m_table.Valid())
  {
    // CAPTION
    m_caption = m_table.GetCaption();
    if(m_caption.Valid())
    {
      m_hasCaption = true;
      m_vAlign = m_caption.GetProperty(HtmlCaption::E_Valign);
    }
    else
    {
      m_hasCaption = false;
      m_vAlign = "top";
    }
    // HEADER
    m_header = m_table.GetHeader();
    if(m_header.Valid())
    {
      m_hasHeader = true;
      int rows = m_header.GetRows();
      m_hRows.Format("%d",rows);
    }
    else
    {
      m_hRows = "1"; // Default
    }
    // FOOTER
    m_footer = m_table.GetFooter();
    if(m_footer.Valid())
    {
      m_hasFooter = true;
      int rows = m_footer.GetRows();
      m_fRows.Format("%d",rows);
    }
    else
    {
      m_fRows = "1"; // Default
    }
    UpdateData(Data2Controls);
  }
}

void
CTablePropPage3Dlg::UpdateProperties()
{
  if(m_table.Valid())
  {
    if(m_caption.Valid())
    {
      m_caption.SetProperty(HtmlCaption::E_Valign,m_vAlign);
    }
  }
}

void
CTablePropPage3Dlg::SetColumns(UINT cols)
{
  m_columns.Format("%d",cols);
  if(m_initDone)
  {
    UpdateData(Data2Controls);
  }
}


////////////////////////////////////////////////////////////////////
//
// CAPTION
//
////////////////////////////////////////////////////////////////////

CString
CTablePropPage3Dlg::CreateCaption()
{
  if(!m_table.Valid() && !m_caption.Valid())
  {
    if(m_hasCaption)
    {
      CString caption = "<CAPTION";
      if(!m_vAlign.IsEmpty())
      {
        caption += CString(" valign=\"") + m_vAlign + "\"";
      }
      caption += ">";
      caption += m_text.IsEmpty() ? "Caption for this table" : m_text;
      caption += "</CAPTION>\n";

      return caption;
    }
  }
  return "";
}

void 
CTablePropPage3Dlg::OnBnClickedHasCaption()
{
  m_hasCaption = m_buttonHasCaption.GetCheck() == TRUE;
  UpdateData(Data2Controls);
}

void 
CTablePropPage3Dlg::OnCbnSelchangeCaptionPos()
{
  int ind = m_comboAlign.GetCurSel();
  if(ind >= 0)
  {
    m_comboAlign.GetLBText(ind,m_vAlign);
  }
}

void 
CTablePropPage3Dlg::OnEnChangeCaptionText()
{
  CWnd* w = GetDlgItem(IDC_CAPTION_TEXT);
  w->GetWindowText(m_text);
}

void 
CTablePropPage3Dlg::OnBnClickedButtCaption()
{
  if(m_caption.Valid())
  {
    if(theApp.MessageBox("Really remove the table caption?","Question",MB_YESNO|MB_ICONQUESTION) == IDYES)
    {
      m_table.DeleteCaption();
      m_hasCaption = false;
      m_caption = HtmlCaption(0);
    }
  }
  else
  {
    if(m_text.IsEmpty())
    {
      theApp.MessageBox("Fill-in at least a caption text on the sections-tab","Empty caption",MB_OK|MB_ICONHAND);
      return;
    }
    m_caption = m_table.GetCaption(true); // Create the caption
    m_caption.SetProperty(HtmlCaption::E_Valign,m_vAlign);
    m_caption.SetInnerHtml(m_text);
    m_hasCaption = true;
  }
  UpdateData(Data2Controls);
}

////////////////////////////////////////////////////////////////////
//
// T_HEADER
//
////////////////////////////////////////////////////////////////////

CString
CTablePropPage3Dlg::CreateHeader(UINT cols,CString one_cell)
{
  if(!m_table.Valid())
  {
    if(m_hasHeader)
    {
      // Use "TH" instead of "TD"
      one_cell.Replace("<TD>","<TH>");
      one_cell.Replace("</TD>","</TH>");

      CString header = "<THEAD>\n";
      header += "<TR>";
      for(unsigned int ind = 0; ind < cols; ++ind)
      {
        header += one_cell;
      }
      header += "</TR>\n";
      header += "</THEAD>\n";
      return header;
    }
  }
  return "";
}

void 
CTablePropPage3Dlg::OnBnClickedHasHeader()
{
  m_hasHeader = m_buttonHasHeader.GetCheck() == TRUE;
  UpdateData(Data2Controls);
}

void 
CTablePropPage3Dlg::OnEnChangeHeadRows()
{
  CWnd* w = GetDlgItem(IDC_HEAD_ROWS);
  w->GetWindowText(m_hRows);
}

void 
CTablePropPage3Dlg::OnEnChangeHeadCols()
{
  CWnd* w = GetDlgItem(IDC_HEAD_COLS);
  w->GetWindowText(m_columns);
  UpdateData(Data2Controls);
}

void 
CTablePropPage3Dlg::OnBnClickedButtHeader()
{
  CTablePropertiesDlg* dlg = dynamic_cast<CTablePropertiesDlg*> (GetParent());

  if(m_header.Valid())
  {
    if(theApp.MessageBox("Really remove the table header section?","Question",MB_YESNO|MB_ICONQUESTION) == IDYES)
    {
      int minRows = m_header.GetRows();
      m_table.DeleteHeader();
      m_hasHeader = false;
      m_header = HtmlTableSection(0);

      // Get resulting cols (could be less after deleting header)
      int colsNow = m_table.GetColumnCount();
      if(dlg)
      {
        // Tell the rest of the dialog
        dlg->UpdateRows(-minRows);
        dlg->UpdateColumns(colsNow);
      }
      m_columns.Format("%d",colsNow);
      m_hRows = "1";
    }
  }
  else
  {
    int cols = atoi(m_columns);
    int rows = atoi(m_hRows);
    if(cols <= 0)
    {
      theApp.MessageBox("Fill-in at least a number of columns for the header on the sections-tab","Empty caption",MB_OK|MB_ICONHAND);
      return;
    }
    if(rows <= 0)
    {
      theApp.MessageBox("Fill-in at least a number of rows for the header on the sections-tab","Empty caption",MB_OK|MB_ICONHAND);
      return;
    }
    // Create header and its rows
    m_header = m_table.GetHeader(true); 
    for(int ind = 0; ind < rows; ++ ind)
    {
      m_header.InsertRow(-1,cols);
    }
    m_hasHeader = true;
    // Get really succeeded rows
    int plusRows = m_header.GetRows();
    // Get resulting cols
    int colsNow = m_table.GetColumnCount();
    if(dlg)
    {
      // To rest of the dialog, so the first tab wonn't intervene
      dlg->UpdateRows(plusRows);
      dlg->UpdateColumns(colsNow);
    }
    // Recalibrate after action
    m_hRows  .Format("%d",plusRows);
    m_columns.Format("%d",colsNow);
  }
  UpdateData(Data2Controls);
}

////////////////////////////////////////////////////////////////////
//
// T_FOOTER
//
////////////////////////////////////////////////////////////////////

CString
CTablePropPage3Dlg::CreateFooter(UINT cols,CString one_cell)
{
  if(!m_table.Valid())
  {
    if(m_hasHeader)
    {
      CString header = "<TFOOT>\n";
      header += "<TR>";
      for(unsigned int ind = 0; ind < cols; ++ind)
      {
        header += one_cell;
      }
      header += "</TR>\n";
      header += "</TFOOT>\n";
      return header;
    }
  }
  return "";
}

void 
CTablePropPage3Dlg::OnBnClickedHasFooter()
{
  m_hasFooter = m_buttonHasFooter.GetCheck() == TRUE;
  UpdateData(Data2Controls);
}

void 
CTablePropPage3Dlg::OnEnChangeFootRows()
{
  CWnd* w = GetDlgItem(IDC_FOOT_ROWS);
  w->GetWindowText(m_fRows);
}

void 
CTablePropPage3Dlg::OnEnChangeFootCols()
{
  CWnd* w = GetDlgItem(IDC_FOOT_COLS);
  w->GetWindowText(m_columns);
  UpdateData(Data2Controls);
}

void 
CTablePropPage3Dlg::OnBnClickedButtFooter()
{
  CTablePropertiesDlg* dlg = dynamic_cast<CTablePropertiesDlg*> (GetParent());

  if(m_footer.Valid())
  {
    if(theApp.MessageBox("Really remove the table footer section?","Question",MB_YESNO|MB_ICONQUESTION) == IDYES)
    {
      int minRows = m_footer.GetRows();
      m_table.DeleteFooter();
      m_hasFooter = false;
      m_footer = HtmlTableSection(0);

      // Get resulting cols (could be less after deleting header)
      int colsNow = m_table.GetColumnCount();
      if(dlg)
      {
        // Tell the rest of the dialog
        dlg->UpdateRows(-minRows);
        dlg->UpdateColumns(colsNow);
      }
      m_columns.Format("%d",colsNow);
      m_hRows = "1";
    }
  }
  else
  {
    int cols = atoi(m_columns);
    int rows = atoi(m_hRows);
    if(cols <= 0)
    {
      theApp.MessageBox("Fill-in at least a number of columns for the footer on the sections-tab","Empty caption",MB_OK|MB_ICONHAND);
      return;
    }
    if(rows <= 0)
    {
      theApp.MessageBox("Fill-in at least a number of rows for the footer on the sections-tab","Empty caption",MB_OK|MB_ICONHAND);
      return;
    }
    // Create footer and its rows
    m_footer = m_table.GetFooter(true); 
    for(int ind = 0; ind < rows; ++ ind)
    {
      m_footer.InsertRow(-1,cols);
    }
    m_hasFooter = true;
    // Get really succeeded rows
    int plusRows = m_footer.GetRows();
    // Get resulting cols
    int colsNow = m_table.GetColumnCount();
    if(dlg)
    {
      // To rest of the dialog, so the first tab wonn't intervene
      dlg->UpdateRows(plusRows);
      dlg->UpdateColumns(colsNow);
    }
    // Recalibrate after action
    m_hRows  .Format("%d",plusRows);
    m_columns.Format("%d",colsNow);
  }
  UpdateData(Data2Controls);
}
