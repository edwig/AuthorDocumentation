//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    SnapGridDlg.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for managing Snap-to-Grid settings
//
#include "stdafx.h"
#include "AuthorHTML.h"
#include "SnapGridDlg.h"
#include "Misc.h"

#define COLOR_DEFAULT_GRIDCOLOR  0x09999FF   // Dark type of red

// SnapGridDlg dialog

IMPLEMENT_DYNAMIC(SnapGridDlg, CDialog)

SnapGridDlg::SnapGridDlg(CWnd* pParent
                        ,bool    enable
                        ,bool    snaptogrid
                        ,CString gridWidth
                        ,CString gridHeight
                        ,CString snapWidth
                        ,CString snapHeight
                        ,CString lineColor)
	          :CDialog(SnapGridDlg::IDD, pParent)
            ,m_gridEnable(enable)
            ,m_snapEnable(snaptogrid)
            ,m_gridWidth(gridWidth)
            ,m_gridHeight(gridHeight)
            ,m_snapWidth(snapWidth)
            ,m_snapHeight(snapHeight)
            ,m_lineColor(lineColor)
{
  Ratios();
}

SnapGridDlg::~SnapGridDlg()
{
}

// Calculate ratio's and correct for ratio
bool
SnapGridDlg::Ratios()
{
  int gw,gh,sw,sh;
  int ratioW,ratioH;
  gw = atoi(m_gridWidth);
  sw = atoi(m_snapWidth);
  if(gw > sw)
  {
    ratioW = gw / sw;
    CString gridWidth;
    gridWidth.Format("%d",ratioW * sw);
    if(gridWidth != m_gridWidth)
    {
      return false;
    }
  }
  else
  {
    ratioW = sw / gw;
    CString snapWidth;
    snapWidth.Format("%d",ratioW * gw);
    if(snapWidth != m_snapWidth)
    {
      return false;
    }
  }

  gh = atoi(m_gridHeight);
  sh = atoi(m_snapHeight);
  if(gh > sh)
  {
    ratioH = gh / sh;
    CString gridHeight;
    gridHeight.Format("%d",ratioH * sh);
    if(gridHeight != m_gridHeight)
    {
      return false;
    }
  }
  else
  {
    ratioH = sh / gh;
    CString snapHeight;
    snapHeight.Format("%d",ratioH * gh);
    if(snapHeight != m_snapHeight)
    {
      return false;
    }
  }
  return true;
}

void SnapGridDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
  // Check Boxes
  DDX_Control(pDX,IDC_GRID_ENABLE, m_buttonGridEnable);
  DDX_Control(pDX,IDC_GRID_EQUAL,  m_buttonGridEqual);
  DDX_Control(pDX,IDC_SNAP_ENABLE, m_buttonSnapEnable);
  DDX_Control(pDX,IDC_SNAP_EQUAL,  m_buttonSnapEqual);
  DDX_Text   (pDX,IDC_GRID_WIDTH,  m_gridWidth);
  DDX_Text   (pDX,IDC_GRID_HEIGHT, m_gridHeight);
  DDX_Control(pDX,IDC_SPIN_GW,     m_spinGW);
  DDX_Control(pDX,IDC_SPIN_GH,     m_spinGH);
  DDX_Text   (pDX,IDC_SNAP_WIDTH,  m_snapWidth);
  DDX_Text   (pDX,IDC_SNAP_HEIGHT, m_snapHeight);
  DDX_Control(pDX,IDC_SPIN_SW,     m_spinSW);
  DDX_Control(pDX,IDC_SPIN_SH,     m_spinSH);
  DDX_Control(pDX,IDC_GRID_BUTTCOLOR, m_buttonColor);
  DDX_Control(pDX,IDOK,            m_buttonOK);
  DDX_Control(pDX,IDCANCEL,        m_buttonCancel);
  DDX_Control(pDX,IDC_GRID_HELP,   m_buttonHelp);

  if(pDX->m_bSaveAndValidate == Data2Controls)
  {
    CWnd* w;
    // GRID PART
    m_buttonGridEqual .EnableWindow(m_gridEnable);
    w = GetDlgItem(IDC_GRID_WIDTH);  w->EnableWindow(m_gridEnable);
    w = GetDlgItem(IDC_GRID_HEIGHT); w->EnableWindow(m_gridEnable && !m_gridEqual);
    m_spinGW.EnableWindow(m_gridEnable);
    m_spinGH.EnableWindow(m_gridEnable && !m_gridEqual);
    m_buttonColor.EnableWindow(m_gridEnable);
    // SNAP PART
    m_buttonSnapEqual .EnableWindow(m_snapEnable);
    w = GetDlgItem(IDC_SNAP_WIDTH);  w->EnableWindow(m_snapEnable);
    w = GetDlgItem(IDC_SNAP_HEIGHT); w->EnableWindow(m_snapEnable && !m_snapEqual);
    m_spinSW.EnableWindow(m_snapEnable);
    m_spinSH.EnableWindow(m_snapEnable && !m_snapEqual);
  }
}

BEGIN_MESSAGE_MAP(SnapGridDlg, CDialog)
  // Check boxes
  ON_BN_CLICKED   (IDC_GRID_ENABLE, OnBnClickedGridEnable)
  ON_BN_CLICKED   (IDC_GRID_EQUAL,  OnBnClickedGridEqual)
  ON_BN_CLICKED   (IDC_SNAP_ENABLE, OnBnClickedSnapEnable)
  ON_BN_CLICKED   (IDC_SNAP_EQUAL,  OnBnClickedSnapEqual)
  // Edit fields
  ON_EN_KILLFOCUS (IDC_GRID_WIDTH,  OnEnChangeGridWidth)
  ON_EN_KILLFOCUS (IDC_GRID_HEIGHT, OnEnChangeGridHeight)
  ON_EN_KILLFOCUS (IDC_SNAP_WIDTH,  OnEnChangeSnapWidth)
  ON_EN_KILLFOCUS (IDC_SNAP_HEIGHT, OnEnChangeSnapHeight)
  // Buttons
  ON_BN_CLICKED   (IDC_GRID_BUTTCOLOR,OnBnClickedGridButtcolor)
  ON_BN_CLICKED   (IDOK,              OnBnClickedOk)
  ON_BN_CLICKED   (IDCANCEL,          OnBnClickedCancel)
  ON_BN_CLICKED   (IDC_GRID_HELP,     OnBnClickedGridHelp)
  // Spin controls
  ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_GW, OnDeltaposSpinGw)
  ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_SW, OnDeltaposSpinSw)
END_MESSAGE_MAP()

BOOL
SnapGridDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  m_gridEqual = (m_gridWidth == m_gridHeight);
  m_snapEqual = (m_snapWidth == m_snapHeight);
  m_buttonGridEnable.SetCheck(m_gridEnable);
  m_buttonSnapEnable.SetCheck(m_snapEnable);
  m_buttonGridEqual .SetCheck(m_gridEqual);
  m_buttonSnapEqual .SetCheck(m_snapEqual);

  m_spinGW.SetBase(10);
  m_spinGH.SetBase(10);
  m_spinSW.SetBase(10);
  m_spinSH.SetBase(10);
  m_spinGW.SetRange(0,32000);
  m_spinGH.SetRange(0,32000);
  m_spinSW.SetRange(0,32000);
  m_spinSH.SetRange(0,32000);
  m_spinGW.SetPos(atoi(m_gridWidth));
  m_spinGH.SetPos(atoi(m_gridHeight));
  m_spinSW.SetPos(atoi(m_snapWidth));
  m_spinSH.SetPos(atoi(m_snapHeight));

  m_buttonColor.EnableOtherButton(_T("More colors..."));
  m_buttonColor.SetColor(COLOR_DEFAULT_GRIDCOLOR);

  int red,green,blue;
  Misc::DecodeColor(m_lineColor,red,green,blue);
  m_buttonColor.SetColor(RGB(red,green,blue));

  UpdateData(Data2Controls);

  return TRUE;
}

void
SnapGridDlg::EnableGrid(bool enable)
{
  m_gridEnable = enable;
  if(!enable)
  {
    EnableSnap(enable);
  }
  UpdateData(Data2Controls);
}

void
SnapGridDlg::EnableSnap(bool enable)
{
  m_snapEnable = enable;
  UpdateData(Data2Controls);
}

// SnapGridDlg message handlers
#pragma warning(disable: 4800)

void 
SnapGridDlg::OnBnClickedGridEnable()
{
  m_gridEnable = (bool) m_buttonGridEnable.GetCheck();
  UpdateData(Data2Controls);
}

void 
SnapGridDlg::OnBnClickedGridEqual()
{
  m_gridEqual = (bool) m_buttonGridEqual.GetCheck();
  if(m_gridEqual)
  {
    m_gridHeight = m_gridWidth;
  }
  UpdateData(Data2Controls);
}

void 
SnapGridDlg::OnBnClickedSnapEnable()
{
  m_snapEnable = (bool) m_buttonSnapEnable.GetCheck();
  UpdateData(Data2Controls);
}

void 
SnapGridDlg::OnBnClickedSnapEqual()
{
  m_snapEqual = (bool) m_buttonSnapEqual.GetCheck();
  if(m_snapEqual)
  {
    m_snapHeight = m_snapWidth;
  }
  UpdateData(Data2Controls);
}

void 
SnapGridDlg::OnEnChangeGridWidth()
{
  CWnd* w = GetDlgItem(IDC_GRID_WIDTH);
  w->GetWindowText(m_gridWidth);
  if(m_gridEqual)
  {
    m_gridHeight = m_gridWidth;
  }
  Ratios();
  UpdateData(Data2Controls);
}

void 
SnapGridDlg::OnEnChangeGridHeight()
{
  CWnd* w = GetDlgItem(IDC_GRID_HEIGHT);
  w->GetWindowText(m_gridHeight);
  Ratios();
  UpdateData(Data2Controls);
}

void 
SnapGridDlg::OnEnChangeSnapWidth()
{
  CWnd* w = GetDlgItem(IDC_SNAP_WIDTH);
  w->GetWindowText(m_snapWidth);
  if(m_snapEqual)
  {
    m_snapHeight = m_snapWidth;
  }
  Ratios();
  UpdateData(Data2Controls);
}

void 
SnapGridDlg::OnEnChangeSnapHeight()
{
  CWnd* w = GetDlgItem(IDC_SNAP_HEIGHT);
  w->GetWindowText(m_snapHeight);
  Ratios();
  UpdateData(Data2Controls);
}

void 
SnapGridDlg::OnBnClickedGridButtcolor()
{
  COLORREF col = m_buttonColor.GetColor();
  int red   = GetRValue(col);
  int green = GetGValue(col);
  int blue  = GetBValue(col);
  m_lineColor.Format("#%02x%02x%02x",red,green,blue);
}

void 
SnapGridDlg::OnBnClickedOk()
{
  if(!Ratios())
  {
    theApp.MessageBox("Grid and snap width's and height's must be a multiple of each other"
                      ,"Error"
                      ,MB_OK|MB_ICONEXCLAMATION);
    return;
  }
  OnOK();
}

void 
SnapGridDlg::OnBnClickedCancel()
{
  OnCancel();
}

void SnapGridDlg::OnDeltaposSpinGw(NMHDR *pNMHDR, LRESULT *pResult)
{
  LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
  m_gridWidth.Format("%d",m_spinGW.GetPos() + pNMUpDown->iDelta);
  if(m_gridEqual)
  {
    m_gridHeight = m_gridWidth;
  }
  UpdateData(Data2Controls);
  *pResult = 0;
}

void SnapGridDlg::OnDeltaposSpinSw(NMHDR *pNMHDR, LRESULT *pResult)
{
  LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
  m_snapWidth.Format("%d",m_spinSW.GetPos() + pNMUpDown->iDelta);
  if(m_snapEqual)
  {
    m_snapHeight = m_snapWidth;
  }
  UpdateData(Data2Controls);
  *pResult = 0;
}

void 
SnapGridDlg::OnBnClickedGridHelp()
{
  // TODO: Add your control notification handler code here
}
