//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    TableProperties.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for managing the properties of a <TABLE> tag in HTML
//              Holds three tabs for the table, the rows and the cells
//
#include "stdafx.h"
#include "AuthorHTML.h"
#include "TableProperties.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTablePropertiesDlg dialog

CTablePropertiesDlg::CTablePropertiesDlg(CWnd*         pParent
                                        ,CString       base
                                        ,bool          bNewTable
                                        ,HtmlTable     pTable
                                        ,HtmlTableRow  pRow
                                        ,HtmlTableCell pCell
                                        ,CComPtr<IHTMLDocument2> doc)
                    :CDialog(CTablePropertiesDlg::IDD, pParent)
                    ,m_bNewTable(bNewTable)
                    ,m_pTable(pTable)
                    ,m_pRow(pRow)
                    ,m_pTableCell(pCell)
                    ,m_doc(doc)
                    ,m_firstTab(0)
{
  // m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
  m_tab1Dlg = new CTablePropPage1Dlg(this,base,pTable);
  m_tab2Dlg = new CTablePropPage2Dlg(this,base,bNewTable,pTable,pRow,pCell,doc);
  m_tab3Dlg = new CTablePropPage3Dlg(this,base,pTable);
}

CTablePropertiesDlg::~CTablePropertiesDlg()
{
  delete m_tab1Dlg;
  delete m_tab2Dlg;
  delete m_tab3Dlg;
}

void CTablePropertiesDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_TABCONTROL, m_tabControl);
  DDX_Control(pDX, IDOK,           m_buttonOK);
  DDX_Control(pDX, IDAPPLY,        m_buttonApply);
  DDX_Control(pDX, IDCANCEL,       m_buttonCancel);
  DDX_Control(pDX, IDHELP,         m_buttonHelp);
}

BEGIN_MESSAGE_MAP(CTablePropertiesDlg, CDialog)
  ON_BN_CLICKED(IDOK,     OnBnClickedOk)
  ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
  ON_BN_CLICKED(IDAPPLY,  OnBnClickedApply)
  ON_BN_CLICKED(IDHELP,   OnBnClickedHelp)
  ON_NOTIFY(TCN_SELCHANGE, IDC_TABCONTROL, OnTcnSelchangeTabcontrol)
END_MESSAGE_MAP()

BOOL
CTablePropertiesDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  m_tab1Dlg->Create(IDD_TABPROP_PAGE1,this);
  m_tab2Dlg->Create(IDD_TABPROP_PAGE2,this);
  m_tab3Dlg->Create(IDD_TABPROP_PAGE3,this);

  if(m_bNewTable)
  {
    m_buttonApply.EnableWindow(FALSE);
  }

  CString s;

  // Hier kan de TAB-volgorde veranderd worden:
  m_tab1Dlg->GetWindowText(s);
  m_tabControl.InsertItem(0,s);

  m_tab2Dlg->GetWindowText(s);
  m_tabControl.InsertItem(1,s);

  m_tab3Dlg->GetWindowText(s);
  m_tabControl.InsertItem(2,s);

  CRect rect;
  m_tabControl.GetWindowRect(&rect);    // geeft t.o.v. ouder window inclusief titelbar
  ScreenToClient(&rect);
  rect.top    += 21;   // 22
  rect.bottom -= 2;
  rect.right  -= 2;
  rect.left   += 1;    // 6

  m_tab1Dlg->MoveWindow(rect,true);    // zet tov client window.
  m_tab2Dlg->MoveWindow(rect,false);
  m_tab3Dlg->MoveWindow(rect,false);

  m_tab1Dlg->ShowWindow(m_firstTab == 0 ? SW_SHOW : SW_HIDE);
  m_tab2Dlg->ShowWindow(m_firstTab == 1 ? SW_SHOW : SW_HIDE);
  m_tab3Dlg->ShowWindow(m_firstTab == 2 ? SW_SHOW : SW_HIDE);
  TabCtrl_SetCurSel(m_tabControl.GetSafeHwnd(),m_firstTab);

  return TRUE;
}

INT_PTR
CTablePropertiesDlg::StartDialog(int tab)
{
  m_firstTab = tab;
  return CDialog::DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CTablePropertiesDlg message handlers


void CTablePropertiesDlg::OnBnClickedOk()
{
  if(m_bNewTable)
  {
    CreateNewTable();
  }
  else
  {
    UpdateProperties();
  }
  OnOK();
}

void CTablePropertiesDlg::OnBnClickedCancel()
{
  OnCancel();
}

void CTablePropertiesDlg::OnBnClickedApply()
{
  if(!m_bNewTable)
  {
    UpdateProperties();
  }
}

void CTablePropertiesDlg::OnBnClickedHelp()
{
  // TODO: Add your control notification handler code here
}

void
CTablePropertiesDlg::UpdateProperties()
{
  m_tab1Dlg->UpdateProperties();
  m_tab2Dlg->UpdateProperties();
  m_tab3Dlg->UpdateProperties();
}

bool
CTablePropertiesDlg::CreateNewTable()
{
  UINT rows = m_tab1Dlg->m_NumRows;
  UINT cols = m_tab1Dlg->m_NumCols;
  CString oneCell  = m_tab2Dlg->CreateOneCell(rows,cols);
  CString caption  = m_tab3Dlg->CreateCaption();
  CString header   = m_tab3Dlg->CreateHeader(cols,oneCell);
  m_newTableString = m_tab1Dlg->CreateTableString(caption,oneCell);
  return true;
}

void 
CTablePropertiesDlg::OnTcnSelchangeTabcontrol(NMHDR *pNMHDR, LRESULT *pResult)
{
  int num = TabCtrl_GetCurSel(pNMHDR->hwndFrom);
  switch(num)
  {
    case 0: m_tab1Dlg->ShowWindow(TRUE);
            m_tab2Dlg->ShowWindow(FALSE);
            m_tab3Dlg->ShowWindow(FALSE);
            break;
    case 1: m_tab2Dlg->ShowWindow(TRUE);
            m_tab1Dlg->ShowWindow(FALSE);
            m_tab3Dlg->ShowWindow(FALSE);
            break;
    case 2: m_tab3Dlg->ShowWindow(TRUE);
            m_tab1Dlg->ShowWindow(FALSE);
            m_tab2Dlg->ShowWindow(FALSE);
            break;
  }
  *pResult = 0;
}

void    
CTablePropertiesDlg::UpdateColumns(int cols)
{
  // Broadcast to all pages?
  m_tab1Dlg->SetColumns(cols);
  m_tab3Dlg->SetColumns(cols);
}

void
CTablePropertiesDlg::UpdateRows(int rows)
{
  // Broadcast to all pages?
  m_tab1Dlg->SetRows(rows);
}
