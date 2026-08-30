//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    WindowDefPage1.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for all window properties page 1 of 5
//
#include "stdafx.h"
#include "AuthorHTML.h"
#include "WindowDefPage1.h"

// WindowDefPage1 dialog

IMPLEMENT_DYNAMIC(WindowDefPage1, CDialog)

WindowDefPage1::WindowDefPage1(CWnd*        p_parent
                              ,ProjectFile* p_project)
	             :CDialog(WindowDefPage1::IDD,p_parent)
               ,m_project(p_project)
               ,m_window(NULL)
{
  m_navigationPane  = false;
  m_textSearch      = false;
  m_advancedSearch  = false;
  m_favourites      = false;
  m_glossary        = false;
  m_browse          = false;
  m_autoSync        = false;
  m_openClosed      = false;
  m_autoShow        = false;
  m_remember        = false;
  m_defaultTab      = 0;
  m_tabPosition     = 0;
  m_width           = 0;
}

WindowDefPage1::~WindowDefPage1()
{
}

void WindowDefPage1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
  DDX_Control(pDX,IDC_NAVPANE,    m_buttonNavPane);
  DDX_Control(pDX,IDC_TEXTSEARCH, m_buttonTextSearch);
  DDX_Control(pDX,IDC_ADVANCED,   m_buttonAdvTSearch);
  DDX_Control(pDX,IDC_FAVOURITES, m_buttonFavourites);
  DDX_Control(pDX,IDC_GLOSSARY,   m_buttonGlossary);
  DDX_Control(pDX,IDC_BROWSE,     m_buttonBrowse);
  DDX_Control(pDX,IDC_AUTOSYNC,   m_buttonAutoSync);
  DDX_Control(pDX,IDC_OPENCLOSED, m_buttonOpenClosed);
  DDX_Control(pDX,IDC_AUTOSHOW,   m_buttonAutoShow);
  DDX_Control(pDX,IDC_REMEMBER,   m_buttonRemember);
  DDX_Control(pDX,IDC_DEFAULT_TAB,m_comboDefTab);
  DDX_Control(pDX,IDC_TABPOSITION,m_comboTabPos);
  DDX_Text   (pDX,IDC_WIDTH,      m_width);
  DDX_Control(pDX,IDC_SPIN,       m_spin);

  if(pDX->m_bSaveAndValidate)
  {
    OnCbnSelchangeDefaultTab();
    OnCbnSelchangeTabposition();
  }
}

BEGIN_MESSAGE_MAP(WindowDefPage1, CDialog)
  ON_BN_CLICKED(IDC_NAVPANE,    &WindowDefPage1::OnBnClickedNavpane)
  ON_BN_CLICKED(IDC_TEXTSEARCH, &WindowDefPage1::OnBnClickedTextsearch)
  ON_BN_CLICKED(IDC_ADVANCED,   &WindowDefPage1::OnBnClickedAdvanced)
  ON_BN_CLICKED(IDC_FAVOURITES, &WindowDefPage1::OnBnClickedFavourites)
  ON_BN_CLICKED(IDC_GLOSSARY,   &WindowDefPage1::OnBnClickedGlossary)
  ON_BN_CLICKED(IDC_BROWSE,     &WindowDefPage1::OnBnClickedBrowse)
  ON_BN_CLICKED(IDC_AUTOSYNC,   &WindowDefPage1::OnBnClickedAutosync)
  ON_BN_CLICKED(IDC_OPENCLOSED, &WindowDefPage1::OnBnClickedOpenclosed)
  ON_BN_CLICKED(IDC_AUTOSHOW,   &WindowDefPage1::OnBnClickedAutoshow)
  ON_BN_CLICKED(IDC_REMEMBER,   &WindowDefPage1::OnBnClickedRemember)
  ON_EN_CHANGE (IDC_WIDTH,      &WindowDefPage1::OnEnChangeWidth)
  ON_CBN_SELCHANGE(IDC_DEFAULT_TAB, &WindowDefPage1::OnCbnSelchangeDefaultTab)
  ON_CBN_SELCHANGE(IDC_TABPOSITION, &WindowDefPage1::OnCbnSelchangeTabposition)
  ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN, &WindowDefPage1::OnDeltaposSpin)
END_MESSAGE_MAP()

BOOL
WindowDefPage1::OnInitDialog()
{
  CDialog::OnInitDialog();
  m_comboTabPos.AddString("Top");
  m_comboTabPos.AddString("Left");
  m_comboTabPos.AddString("Bottom");
  m_spin.SetBase(10);
  m_spin.SetRange(0,10000);
  return TRUE;
}

void
WindowDefPage1::SetProperties(WindowDefinition* p_window)
{
  m_window = p_window;

  m_navigationPane = p_window->GetNavPaneOption(NAVPANE_TOC_INDEX);
  m_textSearch     = p_window->GetNavPaneOption(NAVPANE_SEARCH);
  m_advancedSearch = p_window->GetNavPaneOption(NAVPANE_ADV_SEARCH);
  m_favourites     = p_window->GetNavPaneOption(NAVPANE_FAVOURITES);
  m_glossary       = p_window->GetNavPaneOption(NAVPANE_GLOSSARY);
  m_browse         = p_window->GetNavPaneOption(NAVPANE_BROWSE_SEQ);
  m_defaultTab     = p_window->GetDefaultPane();
  m_tabPosition    = p_window->GetTabPosition();

  m_autoSync       = p_window->GetNavPaneOption(NAVPANE_AUTOSYNCH);
  m_openClosed     = p_window->GetHidePane();
  m_autoShow       = p_window->GetNavPaneOption(NAVPANE_SHOW_HIDE);
  m_remember       = p_window->GetNavPaneOption(NAVPANE_SIZE_POS);
  m_width          = p_window->GetPaneWidth();
  ShowProperties();
}

void
WindowDefPage1::ShowProperties()
{
  m_buttonNavPane   .SetCheck(m_navigationPane);
  m_buttonTextSearch.SetCheck(m_textSearch);
  m_buttonAdvTSearch.SetCheck(m_advancedSearch);
  m_buttonFavourites.SetCheck(m_favourites);
  m_buttonGlossary  .SetCheck(m_glossary);
  m_buttonBrowse    .SetCheck(m_browse);
  m_buttonAutoSync  .SetCheck(m_autoSync);
  m_buttonOpenClosed.SetCheck(m_openClosed);
  m_buttonAutoShow  .SetCheck(m_autoShow);
  m_buttonRemember  .SetCheck(m_remember);

  // Default tab
  m_comboDefTab.ResetContent();
  m_comboDefTab.AddString("Contents");
  m_comboDefTab.AddString("Index");
  if(m_textSearch)
  {
    m_comboDefTab.AddString("Text search");
  }
  if(m_favourites)
  {
    m_comboDefTab.AddString("Favourites");
  }
  if(m_glossary)
  {
    m_comboDefTab.AddString("Glossary");
  }
  m_comboDefTab.SetCurSel(m_defaultTab);

  // Tab position
  m_comboTabPos.SetCurSel(m_tabPosition);

  m_buttonTextSearch.EnableWindow(m_navigationPane);
  m_buttonAdvTSearch.EnableWindow(m_navigationPane);
  m_buttonFavourites.EnableWindow(m_navigationPane);
  m_buttonGlossary  .EnableWindow(m_navigationPane);
  m_buttonBrowse    .EnableWindow(m_navigationPane);
  m_buttonAutoSync  .EnableWindow(m_navigationPane);
  m_buttonOpenClosed.EnableWindow(m_navigationPane);
  m_buttonAutoShow  .EnableWindow(m_navigationPane);
  m_buttonRemember  .EnableWindow(m_navigationPane);
  m_comboDefTab     .EnableWindow(m_navigationPane);
  m_comboTabPos     .EnableWindow(m_navigationPane);
  if(m_navigationPane)
  {
    m_buttonAdvTSearch.EnableWindow(m_textSearch);
  }
  CWnd* w = GetDlgItem(IDC_WIDTH);
  w->EnableWindow(m_navigationPane);
  UpdateData(FALSE);
}

void
WindowDefPage1::UpdateProperties()
{
  m_window->SetNavPaneOption(NAVPANE_TOC_INDEX, m_navigationPane);
  m_window->SetNavPaneOption(NAVPANE_SEARCH,    m_textSearch);
  m_window->SetNavPaneOption(NAVPANE_ADV_SEARCH,m_advancedSearch);
  m_window->SetNavPaneOption(NAVPANE_FAVOURITES,m_favourites);
  m_window->SetNavPaneOption(NAVPANE_GLOSSARY,  m_glossary);
  m_window->SetNavPaneOption(NAVPANE_BROWSE_SEQ,m_browse);
  m_window->SetDefaultPane(m_defaultTab);
  m_window->SetTabPosition(m_tabPosition);
  m_window->SetNavPaneOption(NAVPANE_AUTOSYNCH, m_autoSync);
  m_window->SetHidePane(m_openClosed);
  m_window->SetNavPaneOption(NAVPANE_SHOW_HIDE, m_autoShow);
  m_window->SetNavPaneOption(NAVPANE_SIZE_POS,  m_remember);
  m_window->SetPaneWidth(m_width);
}

// WindowDefPage1 message handlers

void 
WindowDefPage1::OnBnClickedNavpane()
{
  m_navigationPane = m_buttonNavPane.GetCheck() == TRUE;
  ShowProperties();
}

void 
WindowDefPage1::OnBnClickedTextsearch()
{
  m_textSearch = m_buttonTextSearch.GetCheck() == TRUE;
  ShowProperties();
}

void 
WindowDefPage1::OnBnClickedAdvanced()
{
  m_advancedSearch = m_buttonAdvTSearch.GetCheck() == TRUE;
  ShowProperties();
}

void 
WindowDefPage1::OnBnClickedFavourites()
{
  m_favourites = m_buttonFavourites.GetCheck() == TRUE;
  ShowProperties();
}

void 
WindowDefPage1::OnBnClickedGlossary()
{
  m_glossary = m_buttonGlossary.GetCheck() == TRUE;
  ShowProperties();
}

void 
WindowDefPage1::OnBnClickedBrowse()
{
  m_browse = m_buttonBrowse.GetCheck() == TRUE;
  ShowProperties();
}

void 
WindowDefPage1::OnBnClickedAutosync()
{
  m_autoSync = m_buttonAutoSync.GetCheck() == TRUE;
  ShowProperties();
}

void 
WindowDefPage1::OnBnClickedOpenclosed()
{
  m_openClosed = m_buttonOpenClosed.GetCheck() == TRUE;
  ShowProperties();
}

void 
WindowDefPage1::OnBnClickedAutoshow()
{
  m_autoShow = m_buttonAutoShow.GetCheck() == TRUE;
  ShowProperties();
}

void 
WindowDefPage1::OnBnClickedRemember()
{
  m_remember = m_buttonRemember.GetCheck() == TRUE;
  ShowProperties();
}

void 
WindowDefPage1::OnCbnSelchangeDefaultTab()
{
  int ind = m_comboDefTab.GetCurSel();
  if(ind >= 0)
  {
    m_defaultTab = ind;
  }
}

void 
WindowDefPage1::OnCbnSelchangeTabposition()
{
  int ind = m_comboTabPos.GetCurSel();
  if(ind >= 0)
  {
    m_tabPosition = ind;
  }
}

void 
WindowDefPage1::OnEnChangeWidth()
{
  CWnd* w = GetDlgItem(IDC_WIDTH);
  CString wi;
  w->GetWindowText(wi);
  m_width = atoi(wi);
}

void WindowDefPage1::OnDeltaposSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
  LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
  *pResult = 0;
  CString wi;
  CWnd* w = GetDlgItem(IDC_WIDTH);
  w->GetWindowText(wi);
  m_width = atoi(wi);
  m_width += pNMUpDown->iDelta;
  UpdateData(FALSE);
}
