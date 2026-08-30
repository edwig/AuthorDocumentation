//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    MainFrame.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: The main window frame on the MS-Windows desktop
//
#include "stdafx.h"
#include "AuthorHTML.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "htmleddoc.h"
#include "htmlEdView.h"
#include "misc.h"
#include "GlobalSettingsDlg.h"
#include "BrokenLinksDlg.h"

#include <Shlwapi.h>
#include <windows.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define EDIT_HINT "Change the text class"
#define FONT_HINT "Change the font style of the text"
#define SIZE_HINT "Change the size of the text font"

// HACK: Global status bar pointer to update the status bar.
CMFCStatusBar*    g_statusBar    = nullptr;
LPCSTR MainFrame::m_cszClassName = "AuthorDoc.AuthorHTML";

/////////////////////////////////////////////////////////////////////////////
// MainFrame

IMPLEMENT_DYNAMIC(MainFrame,CMDIFrameWndEx)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(MainFrame,CMDIFrameWndEx)
  ON_WM_CREATE()
  ON_WM_SIZE()
  ON_WM_SYSCOMMAND()
  ON_WM_SHOWWINDOW()
  ON_COMMAND(ID_WINDOW_MANAGER,                OnWindowManager)
  ON_COMMAND(ID_TOOLS_PREFERENCES,             OnPreferences)
  ON_COMMAND     (ID_FMTBAR_CLASSNAME,         OnClassNameChange)
  ON_CBN_SELENDOK(ID_FMTBAR_CLASSNAME,         OnClassNameChange)
  ON_COMMAND     (ID_FMTBAR_FONTNAME,          OnFontNameChange)
  ON_CBN_SELENDOK(ID_FMTBAR_FONTNAME,          OnFontNameChange)
  ON_COMMAND     (ID_FMTBAR_FONTSIZE,          OnFontSizeChange)
  ON_CBN_SELENDOK(ID_FMTBAR_FONTSIZE,          OnFontSizeChange)

  ON_COMMAND(ID_FILE_CLOSEOTHERS,              OnCloseOthers)
  ON_COMMAND(ID_WINDOW_MAXIMIZE,               OnMDIMaximize)
  ON_COMMAND(ID_VIEW_CUSTOMIZE,                OnViewCustomize)
  ON_COMMAND(ID_TOOLS_BROKENLINKS,             OnBrokenLinks)

  // Panes
  ON_COMMAND_RANGE(ID_PANE_TOC,ID_PANE_COMPILE,OnShowPane)
  ON_UPDATE_COMMAND_UI(ID_PANE_TOC,            OnUpdateUIPaneToc)
  ON_UPDATE_COMMAND_UI(ID_PANE_KEYWORDS,       OnUpdateUIPaneIndex)
  ON_UPDATE_COMMAND_UI(ID_PANE_LINKS,          OnUpdateUILinks)
  ON_UPDATE_COMMAND_UI(ID_PANE_EXPLORER,       OnUpdateUIExplorer)
  ON_UPDATE_COMMAND_UI(ID_PANE_DOCUMENTS,      OnUpdateUIDocuments)
  ON_UPDATE_COMMAND_UI(ID_PANE_PROJECT,        OnUpdateUIPaneProject)
  ON_UPDATE_COMMAND_UI(ID_PANE_COMPILE,        OnUpdateUIPaneCompile)

  ON_UPDATE_COMMAND_UI(ID_WINDOW_MAXIMIZE,     OnMDIMaximizeUI)
  ON_UPDATE_COMMAND_UI(ID_INDICATOR_VIEW,      OnUpdatePane)
  ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipText)
  ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipText)

  ON_UPDATE_COMMAND_UI(ID_FMTBAR_CLASSNAME,    OnUpdateUI)
  ON_UPDATE_COMMAND_UI(ID_FMTBAR_FONTSIZE,     OnUpdateUI)

  ON_REGISTERED_MESSAGE(AFX_WM_RESETTOOLBAR,   OnToolbarReset)
  ON_MESSAGE((WM_APP + 1),                     OnRestoreToolbar)

  ON_COMMAND_RANGE          (ID_VIEW_APPLOOK_2007, ID_VIEW_APPLOOK_2007_3, OnAppLook)
  ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_2007, ID_VIEW_APPLOOK_2007_3, OnUpdateAppLook)

END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_VIEW,		// view indicator - Web/Source
  ID_INDICATOR_OVR,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

int IDC_MF_WORKBOOK_BAR = 0;

/////////////////////////////////////////////////////////////////////////////
// MainFrame construction/destruction

MainFrame::MainFrame()
{
  IDC_MF_WORKBOOK_BAR  = (AFX_IDW_CONTROLBAR_LAST - 1);

  m_hActiveChild    = 
  m_hLastChild      = 
  m_hSkipChild      = NULL;
  m_bShowed         = FALSE;
  m_bMDINextSeq     = FALSE;
  m_showWorkbookBar = TRUE;
  m_lockLink        = false;

  m_nAppLook = theApp.GetInt(_T("ApplicationLook"),ID_VIEW_APPLOOK_2007);
}

MainFrame::~MainFrame()
{
  g_statusBar = nullptr;
}

void
MainFrame::CloseAllPanes()
{
  // Close the project panes
  ClosePane(ID_PANE_TOC);
  ClosePane(ID_PANE_KEYWORDS);
  ClosePane(ID_PANE_PROJECT);
  ClosePane(ID_PANE_COMPILE);
  ClosePane(ID_PANE_DOCUMENTS);
  ClosePane(ID_PANE_EXPLORER);
  ClosePane(ID_PANE_LINKS);
}

int 
MainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWndEx::OnCreate(lpCreateStruct) == -1)
  {
    theApp.Panic("Cannot create MDI frame");
  }	

  OnAppLook(m_nAppLook);

//   CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
  CMFCToolBarComboBoxButton::SetFlatMode();
  CMFCToolBar::EnableQuickCustomization();

  // Find default font
  m_font.Attach(GetStockObject(DEFAULT_GUI_FONT));

  // Make the MDI tab manager for documents
  OnCreate_Tabmanager();

  // Make our command bars on top
  OnCreate_CommandBars();

  // Make our statusbar
  OnCreate_StatusBar();

  // Now Dock the command bars to the frame
  OnCreate_DockCommandBars();

  // enable Visual Studio 2005 style docking window behavior
  CDockingManager::SetDockingMode(DT_SMART);
  // enable Visual Studio 2005 style docking window auto-hide behavior
  EnableAutoHidePanes(CBRS_ALIGN_ANY);

  // Make the docking panes (left and bottom)
  OnCreate_DockingPanes();

  OnCreate_DockDockingPanes();

  // set the visual manager and style based on persisted value
  OnApplicationLook(theApp.m_nAppLook);

  // Enable enhanced windows management dialog
  EnableWindowsDialog(ID_WINDOW_MANAGER,_T("&Windows..."),TRUE);

  // Enable toolbar and docking window menu replacement
  EnablePaneMenu(TRUE,ID_VIEW_CUSTOMIZE,_T("Customize"),ID_VIEW_TOOLBAR);

  // enable quick (Alt+drag) toolbar customization
  CMFCToolBar::EnableQuickCustomization();

  if(CMFCToolBar::GetUserImages() == nullptr)
  {
    // load user-defined toolbar images
    if(m_UserImages.Load(_T(".\\UserImages.bmp")))
    {
      CMFCToolBar::SetUserImages(&m_UserImages);
    }
  }

  // enable menu personalization (most-recently used commands)
  // TODO: define your own basic commands, ensuring that each pulldown menu has at least one basic command.
  CList<UINT,UINT> lstBasicCommands;

  lstBasicCommands.AddTail(ID_FILE_NEW);
  lstBasicCommands.AddTail(ID_FILE_OPEN);
  lstBasicCommands.AddTail(ID_FILE_SAVE);
  lstBasicCommands.AddTail(ID_FILE_PRINT);
  lstBasicCommands.AddTail(ID_APP_EXIT);
  lstBasicCommands.AddTail(ID_EDIT_CUT);
  lstBasicCommands.AddTail(ID_EDIT_PASTE);
  lstBasicCommands.AddTail(ID_EDIT_UNDO);
  lstBasicCommands.AddTail(ID_APP_ABOUT);
  lstBasicCommands.AddTail(ID_VIEW_STATUS_BAR);
  lstBasicCommands.AddTail(ID_VIEW_TOOLBAR);

  CMFCToolBar::SetBasicCommands(lstBasicCommands);

  // Switch the order of document name and application name on the window title bar. This
  // improves the usability of the taskbar because the document name is visible with the thumbnail.
  ModifyStyle(0,FWS_PREFIXTITLE);

  theApp.LoadState(this,"Layout");
  PostMessage(WM_APP+1,0,0);

  // Final platform checks
  OnCreate_Platform();
  return 0;
}

void
MainFrame::OnCreate_Tabmanager()
{
  CMDITabInfo mdiTabParams;
  mdiTabParams.m_style = CMFCTabCtrl::STYLE_3D_ONENOTE; // other styles available...
  mdiTabParams.m_bActiveTabCloseButton = TRUE;          // set to FALSE to place close button at right of tab area
  mdiTabParams.m_bTabIcons     = FALSE;                 // set to TRUE to enable document icons on MDI taba
  mdiTabParams.m_bAutoColor    = TRUE;                  // set to FALSE to disable auto-coloring of MDI tabs
  mdiTabParams.m_bDocumentMenu = TRUE;                  // enable the document menu at the right edge of the tab area
  EnableMDITabbedGroups(TRUE,mdiTabParams);
}

void
MainFrame::OnCreate_CommandBars()
{
  // Add the menu bar
  if(!m_wndMenuBar.Create(this))
  {
    theApp.Panic("Failed to create menu bar.");
    return;
  }
  m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);
  CMFCPopupMenu::SetForceMenuFocus(FALSE);

  // CREATE TOOLBARS

  // Standard toolbar
  if(!m_wndToolBar.CreateEx(this,TBSTYLE_FLAT,WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC,CRect(1,1,1,1),IDR_MAINFRAME) ||
     !m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
  {
    theApp.Panic("Failed to create toolbar STANDARD");
    return;
  }
  m_wndToolBar.SetWindowText(_T("Standard toolbar"));
  m_wndToolBar.EnableCustomizeButton(TRUE,ID_VIEW_CUSTOMIZE,_T("Customize"));

  // Tables toolbar
  if(!m_wndTableBar.CreateEx(this,TBSTYLE_FLAT,WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC,CRect(1,1,1,1),IDR_TABLEBAR) ||
     !m_wndTableBar.LoadToolBar(IDR_TABLEBAR))
  {
    theApp.Panic("Failed to create toolbar TABLEBAR");
    return;
  }
  m_wndTableBar.SetWindowText(_T("Table toolbar"));
  m_wndTableBar.EnableCustomizeButton(TRUE,ID_VIEW_CUSTOMIZE,_T("Customize"));

  // Window Toolbar
  if(!m_wndWindowBar.CreateEx(this,TBSTYLE_FLAT,WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC,CRect(1,1,1,1),IDR_WINDOWBAR) ||
     !m_wndWindowBar.LoadToolBar(IDR_WINDOWBAR))
  {
    theApp.Panic("Failed to create toolbar WINDOWBAR");
    return;
  }
  m_wndWindowBar.SetWindowText(_T("Window toolbar"));
  m_wndWindowBar.EnableCustomizeButton(TRUE,ID_VIEW_CUSTOMIZE,_T("Customize"));

  // Edit toolbar
  if(!m_wndEditBar.CreateEx(this,TBSTYLE_FLAT,WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC,CRect(1,1,1,1),ID_TOOLBAR_EDITING) ||
     !m_wndEditBar.LoadToolBar(ID_TOOLBAR_EDITING))
     {
       theApp.Panic("Failed to create toolbar EDITBAR");
       return;
  }
  m_wndEditBar.SetWindowText(_T("Edit toolbar"));
  m_wndEditBar.EnableCustomizeButton(TRUE,ID_VIEW_CUSTOMIZE,_T("Customize"));

  // Link toolbar
  if(!m_wndLinkBar.CreateEx(this,TBSTYLE_FLAT,WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC,CRect(1,1,1,1),IDR_LINKBAR) ||
     !m_wndLinkBar.LoadToolBar(IDR_LINKBAR))
  {
    theApp.Panic("Failed to create toolbar LINKBAR");
    return;
  }
  m_wndLinkBar.SetWindowText(_T("Link toolbar"));
  m_wndLinkBar.EnableCustomizeButton(TRUE,ID_VIEW_CUSTOMIZE,_T("Customize"));

  // Allow user-defined toolbars operations:
  InitUserToolbars(nullptr,uiFirstUserToolBarId,uiLastUserToolBarId);
}

void
MainFrame::OnCreate_StatusBar()
{
  // STATUS BAR
  if(!m_wndStatusBar.Create(this) ||
     !m_wndStatusBar.SetIndicators(indicators,sizeof(indicators) / sizeof(UINT)))
  {
    theApp.Panic("Failed to create status bar");
  }
  // Save global status bar pointer
  g_statusBar = &m_wndStatusBar;
}

void
MainFrame::OnCreate_DockCommandBars()
{
  // Set all toolbars to be dockable
  m_wndMenuBar  .EnableDocking(CBRS_ALIGN_ANY);
  m_wndToolBar  .EnableDocking(CBRS_ALIGN_ANY);
  m_wndEditBar  .EnableDocking(CBRS_ALIGN_ANY);
  m_wndTableBar .EnableDocking(CBRS_ALIGN_ANY);
  m_wndWindowBar.EnableDocking(CBRS_ALIGN_ANY);
  m_wndLinkBar  .EnableDocking(CBRS_ALIGN_ANY);

  EnableDocking(CBRS_ALIGN_ANY);
  DockPane(&m_wndMenuBar);
  DockPane(&m_wndToolBar);
  DockPane(&m_wndEditBar);
  DockPane(&m_wndTableBar);
  DockPane(&m_wndWindowBar);
  DockPane(&m_wndLinkBar);
}

void
MainFrame::OnCreate_DockingPanes()
{
  // TOC
  if(!m_wndTOCView.Create(_T("TOC"),this,CRect(0,0,200,200),TRUE,ID_PANE_TOC,WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
  {
    theApp.Panic("Cannot create pane TOC");
    return;
  }

  // File Explorer
  if(!m_wndExplorerView.Create(_T("Explorer"),this,CRect(0,0,200,200),TRUE,ID_PANE_EXPLORER,WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
  {
    theApp.Panic("Cannot create pane EXPLORER");
    return;
  }

  // Documents
  if(!m_openFiles.Create(_T("Documents"),this,CRect(0,0,200,200),TRUE,ID_PANE_DOCUMENTS,WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
  {
    theApp.Panic("Cannot create pane DOCUMENTS");
    return;
  }

  // Indices
  if(!m_wndIndexView.Create(_T("Indices"),this,CRect(0,0,200,200),TRUE,ID_PANE_KEYWORDS,WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
  {
    theApp.Panic("Cannot create pane INDEX");
    return;
  }

  // Page links
  if(!m_wndPageLink2View.Create(_T("Links"),this,CRect(0,0,200,200),TRUE,ID_PANE_LINKS,WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
  {
    theApp.Panic("Cannot create pane LINKS");
    return;
  }

  // Project view
  if(!m_wndProjectView.Create(_T("Project"),this,CRect(0,0,200,200),TRUE,ID_PANE_PROJECT,WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
  {
    theApp.Panic("Cannot create pane PROJECT");
    return;
  }

  // Compile output window
  if(!m_wndCompileView.Create(_T("Compilation"),this,CRect(0,0,100,100),TRUE,ID_PANE_COMPILE,WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
  {
    theApp.Panic("Cannot create pane COMPILE");
    return;
  }

  // Set the icons for the docking pane tabs.
  OnCreate_SetDockingIcons();

  // FIELD CHOOSER en FILTER EDITTING

  EnableDocking(CBRS_ALIGN_ANY);
}

void
MainFrame::OnCreate_SetDockingIcons()
{
  HICON hTOCViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(),MAKEINTRESOURCE(IDI_TOC_VIEW),IMAGE_ICON,::GetSystemMetrics(SM_CXSMICON),::GetSystemMetrics(SM_CYSMICON),0);
  m_wndTOCView.SetIcon(hTOCViewIcon,FALSE);

  HICON hKEYViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(),MAKEINTRESOURCE(IDI_KEYWORD_VIEW),IMAGE_ICON,::GetSystemMetrics(SM_CXSMICON),::GetSystemMetrics(SM_CYSMICON),0);
  m_wndIndexView.SetIcon(hKEYViewIcon,FALSE);

  HICON hEXPViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(),MAKEINTRESOURCE(IDI_EXPLORER_VIEW),IMAGE_ICON,::GetSystemMetrics(SM_CXSMICON),::GetSystemMetrics(SM_CYSMICON),0);
  m_wndExplorerView.SetIcon(hEXPViewIcon,FALSE);

  HICON hDOCViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(),MAKEINTRESOURCE(IDI_WINDOW_VIEW),IMAGE_ICON,::GetSystemMetrics(SM_CXSMICON),::GetSystemMetrics(SM_CYSMICON),0);
  m_openFiles.SetIcon(hDOCViewIcon,FALSE);

  HICON hPRJViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(),MAKEINTRESOURCE(IDI_PROJECT_VIEW),IMAGE_ICON,::GetSystemMetrics(SM_CXSMICON),::GetSystemMetrics(SM_CYSMICON),0);
  m_wndProjectView.SetIcon(hPRJViewIcon,FALSE);

  HICON hLNKViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(),MAKEINTRESOURCE(IDI_LINKS_VIEW),IMAGE_ICON,::GetSystemMetrics(SM_CXSMICON),::GetSystemMetrics(SM_CYSMICON),0);
  m_wndPageLink2View.SetIcon(hLNKViewIcon,FALSE);

  HICON hCMPViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(),MAKEINTRESOURCE(IDI_COMPILE_VIEW),IMAGE_ICON,::GetSystemMetrics(SM_CXSMICON),::GetSystemMetrics(SM_CYSMICON),0);
  m_wndCompileView.SetIcon(hCMPViewIcon,FALSE);

  UpdateMDITabbedBarsIcons();
}

void
MainFrame::OnCreate_DockDockingPanes()
{
  CDockablePane* bar(nullptr);

  // Enable docking
  m_wndTOCView      .EnableDocking(CBRS_ALIGN_ANY);
  m_wndExplorerView .EnableDocking(CBRS_ALIGN_ANY);
  m_openFiles       .EnableDocking(CBRS_ALIGN_ANY);
  m_wndIndexView    .EnableDocking(CBRS_ALIGN_ANY);
  m_wndProjectView  .EnableDocking(CBRS_ALIGN_ANY);
  m_wndPageLink2View.EnableDocking(CBRS_ALIGN_ANY);

  // Dock the panes
  DockPane(&m_wndTOCView);
  m_wndExplorerView .AttachToTabWnd(&m_wndTOCView,      DM_SHOW,TRUE,&bar);
  m_openFiles       .AttachToTabWnd(&m_wndExplorerView, DM_SHOW,TRUE,&bar);
  m_wndIndexView    .AttachToTabWnd(&m_openFiles,       DM_SHOW,TRUE,&bar);
  m_wndPageLink2View.AttachToTabWnd(&m_wndIndexView,    DM_SHOW,TRUE,&bar);
  m_wndProjectView  .AttachToTabWnd(&m_wndPageLink2View,DM_SHOW,TRUE,&bar);

  // Dock the output compile window at the bottom
  m_wndCompileView.EnableDocking(CBRS_ALIGN_ANY);
  DockPane(&m_wndCompileView);
}

void
MainFrame::OnCreate_Platform()
{
  // General checks
  if(!IsPlatformNT())
  {
    theApp.ErrorMessage("AuthorDocumentation cannot run on MS-Windows 95/98/ME Platforms!. Sorry");
    exit(3);
  }
  UINT version = GetMsieVersion();
  if(version < 60)
  {
    // Nummerous IDispatch interfaces are different per MS-IE version.
    // Must be tested at every change of the checked version. (See HTMLEdView, Misc, Dialogs)
    // Look for "IHTML*" and "IID_IHTML*" interfaces.
    theApp.ErrorMessage("AuthorDocumentation cannot run if MS-Internet-Explorer version < 6.0! Sorry");
    exit(3);
  }
  // Be sure it looks as intended!!
  ResetMsieSettings();
}

static LPCTSTR g_registration = "Software\\" REGISTER_APP "\\" APPLICATION_NAME "\\" _T("Workspace\\Layout");

// This function is important to assure that Internet Explorer will work as expected
// If the following Registry Keys have the wrong value, Internet Explorer
// will display wrong colors, wrong font sizes and no images at all !!!
void 
MainFrame::ResetMsieSettings()
{
  Misc::RegWriteDword (_HKCU, _T("Software\\Microsoft\\Internet Explorer\\Styles"),   _T("Use My Stylesheet"),       0);
  Misc::RegWriteDword (_HKCU, _T("Software\\Microsoft\\Internet Explorer\\Settings"), _T("Always Use My Colors"),    0);
  Misc::RegWriteDword (_HKCU, _T("Software\\Microsoft\\Internet Explorer\\Settings"), _T("Always Use My Font Face"), 0);
  Misc::RegWriteDword (_HKCU, _T("Software\\Microsoft\\Internet Explorer\\Settings"), _T("Always Use My Font Size"), 0);
  Misc::RegWriteString(_HKCU, _T("Software\\Microsoft\\Internet Explorer\\Main"),     _T("Display Inline Images"),   _T("yes"));
}

// int 
// MainFrame::OnCreateControl(LPCREATECONTROLSTRUCT lpCreateControl)
// {
//   // This function is called by the Extreme Tool Kit application framework.
//   // This function handles the creation of the controls that are placed
//   // on the CommandBars.  The LPCREATECONTROLSTRUCT structure contains information
//   // about the control to create.
//   //const int nDropHeight = 600;
// 
//   // Create the Style combo box
//   if (lpCreateControl->nID == ID_FMTBAR_CLASSNAME)
//   {
//     CXTPControlComboBox* classNameCombo = GetClassNameCombo();
//     if(!classNameCombo)
//     {
//       classNameCombo = (CXTPControlComboBox*)CXTPControlComboBox::CreateObject();
//       if(!classNameCombo)
//       {
//         TRACE0("Failed to create class-name combo-box\n");
//         return FALSE;
//       }
//       CString hint = EDIT_HINT;
//       classNameCombo->SetWidth(WIDHT_CLASSNAME);
//       classNameCombo->SetDropDownListStyle();
//       classNameCombo->GetListBoxCtrl()->ModifyStyle(0,CBS_SORT,0);
//       classNameCombo->SetEditHint(hint);
//       classNameCombo->SetTooltip (hint);
//       //m_classNameCombo->ModifyListBoxStyle(0,CBS_SORT);
//       //m_classNameCombo->SetEditStyle(CBS_SORT);
//       // TODO naar Update_ui handler per view
//       classNameCombo->SetFlags(xtpFlagManualUpdate);
//       classNameCombo->SetCurSel(-1);
// 
//       lpCreateControl->pControl = classNameCombo;
//       return TRUE;
//     }
//   }
// 
//   if(lpCreateControl->nID == ID_FMTBAR_FONTNAME)
//   {
//     CXTPControlFontComboBox* fontNameCombo = GetFontNameCombo();
//     if(!fontNameCombo)
//     {
//       fontNameCombo = new CXTPControlFontComboBox();
//       if(!fontNameCombo)
//       {
//         TRACE0("Failed to create font-name combo-box\n");
//         return FALSE;
//       }
//       CString hint = FONT_HINT;
//       fontNameCombo->SetWidth(WIDTH_FONTNAME);
//       fontNameCombo->SetDropDownListStyle();
//       fontNameCombo->SetEditHint(hint);
//       fontNameCombo->SetTooltip (hint);
//       // TODO naar Update_ui handler per view
//       fontNameCombo->SetFlags(xtpFlagManualUpdate);
//       fontNameCombo->SetCurSel(-1);
//       lpCreateControl->pControl = fontNameCombo;
//       return TRUE;
//     }
//   }
//   if(lpCreateControl->nID == ID_FMTBAR_FONTSIZE)
//   {
//     CXTPControlComboBox* fontSizeCombo = GetFontSizeCombo();
//     if(!fontSizeCombo)
//     {
//       fontSizeCombo = (CXTPControlComboBox*)CXTPControlComboBox::CreateObject();
//       if(!fontSizeCombo)
//       {
//         TRACE0("Failed to create font-size combo-box\n");
//         return FALSE;
//       }
//       CString hint = SIZE_HINT;
//       fontSizeCombo->SetWidth(WIDTH_FONTSIZE);
//       fontSizeCombo->SetDropDownListStyle();
//       fontSizeCombo->ModifyListBoxStyle(0,CBS_SORT);
//       fontSizeCombo->SetFlags(xtpFlagManualUpdate);
//       fontSizeCombo->SetEditHint(hint);
//       fontSizeCombo->SetTooltip (hint);
//       //m_fontSizeCombo->GetEditCtrl()->SetFont(&m_font);
//       fontSizeCombo->SetCurSel(-1);
// 
//       lpCreateControl->pControl = fontSizeCombo;
//       return TRUE;
//     }
//   }
//   return FALSE;
// }

CMFCToolBarComboBoxButton*
MainFrame::GetClassNameCombo()
{
  CMFCToolBarComboBoxButton* pFindCombo = nullptr;

  CObList listButtons;
  if(CMFCToolBar::GetCommandButtons(ID_FMTBAR_CLASSNAME, listButtons) > 0)
  {
    for(POSITION posCombo = listButtons.GetHeadPosition(); pFindCombo == nullptr && posCombo != nullptr;)
    {
      CMFCToolBarComboBoxButton* pCombo = DYNAMIC_DOWNCAST(CMFCToolBarComboBoxButton, listButtons.GetNext(posCombo));
      if(pCombo != nullptr)
      {
        return pCombo;
      }
    }
  }
  return nullptr;
}

CMFCToolBarFontComboBox*
MainFrame::GetFontNameCombo() 
{
  CMFCToolBarFontComboBox* pFindCombo = nullptr;

  CObList listButtons;
  if(CMFCToolBar::GetCommandButtons(ID_FMTBAR_FONTNAME, listButtons) > 0)
  {
    for(POSITION posCombo = listButtons.GetHeadPosition(); pFindCombo == nullptr && posCombo != nullptr;)
    {
      CMFCToolBarFontComboBox* pCombo = DYNAMIC_DOWNCAST(CMFCToolBarFontComboBox,listButtons.GetNext(posCombo));
      if(pCombo != nullptr)
      {
        return pCombo;
      }
    }
  }
  return nullptr;
}

CMFCToolBarComboBoxButton*
MainFrame::GetFontSizeCombo() 
{
  CMFCToolBarComboBoxButton* pFindCombo = nullptr;

  CObList listButtons;
  if(CMFCToolBar::GetCommandButtons(ID_FMTBAR_FONTSIZE, listButtons) > 0)
  {
    for(POSITION posCombo = listButtons.GetHeadPosition(); pFindCombo == nullptr && posCombo != nullptr;)
    {
      CMFCToolBarComboBoxButton* pCombo = DYNAMIC_DOWNCAST(CMFCToolBarComboBoxButton, listButtons.GetNext(posCombo));
      if(pCombo != nullptr)
      {
        return pCombo;
      }
    }
  }
  return nullptr;
}

BOOL 
MainFrame::PreTranslateMessage (MSG* pMsg)
{
  if (pMsg->message == WM_KEYUP && pMsg->wParam == VK_CONTROL)
  {
    m_bMDINextSeq = FALSE;
    m_hSkipChild  = NULL;
  }
  return CMDIFrameWnd::PreTranslateMessage(pMsg);
}

// LRESULT 
// MainFrame::OnDockingPaneNotify(WPARAM wParam, LPARAM lParam)
// {
//   if (wParam == XTP_DPN_SHOWWINDOW)
//   {
//     // get a pointer to the docking pane being shown.
//     CXTPDockingPane* pPane = (CXTPDockingPane*)lParam;
//     int nIndex = pPane->GetID();
//     if (!pPane->IsValid())
//     {
//       if(nIndex == ID_PANE_DOCUMENTS)
//       {
//         if(!m_openFiles.m_hWnd)
//         {
//           m_openFiles.CreatePane(this);
//         }
//         pPane->Attach(&m_openFiles);
//         return TRUE;
//       }
//       if(nIndex == ID_PANE_EXPLORER)
//       {
//         if(!m_wndExplorerView.m_hWnd)
//         {
//           m_wndExplorerView.CreatePane(this);
//         }
//         pPane->Attach(&m_wndExplorerView);
//         return TRUE;
//       }
//       if(nIndex == ID_PANE_TOC)
//       {
//         if(!m_wndTOCView.m_hWnd)
//         {
//           m_wndTOCView.CreatePane(this);
//         }
//         pPane->Attach(&m_wndTOCView);
//         return TRUE;
//       }
//       if(nIndex == ID_PANE_KEYWORDS)
//       {
//         if(!m_wndIndexView.m_hWnd)
//         {
//           m_wndIndexView.CreatePane(this);
//         }
//         pPane->Attach(&m_wndIndexView);
//         return TRUE;
//       }
//       if(nIndex == ID_PANE_PROJECT)
//       {
//         if(!m_wndProjectView)
//         {
//           if(!m_wndProjectView.m_hWnd)
//           {
//             m_wndProjectView.CreatePane(this);
//           }
//           pPane->Attach(&m_wndProjectView);
//         }
//         return TRUE;
//       }
//       if(nIndex == ID_PANE_COMPILE)
//       {
//         if(!m_wndCompileView)
//         {
//           if(!m_wndCompileView.m_hWnd)
//           {
//             m_wndCompileView.CreatePane(this);
//           }
//           pPane->Attach(&m_wndCompileView);
//         }
//         return TRUE;
//       }
//       if(nIndex == ID_PANE_LINKS)
//       {
//         if(!m_wndPageLink2View)
//         {
//           if(!m_wndPageLink2View.m_hWnd)
//           {
//             m_wndPageLink2View.CreatePane(this);
//           }
//           pPane->Attach(&m_wndPageLink2View);
//         }
//         return TRUE;
//       }
//     }
//   }
//   if (wParam == XTP_DPN_RCLICK)
//   {
//     CPoint pos;
//     GetCursorPos(&pos);
// 
//     CMenu menu;
//     VERIFY(menu.LoadMenu(IDR_PANES));
//     CMenu* pPopup = menu.GetSubMenu(0);
//     CXTPCommandBars::TrackPopupMenu(pPopup
//                                    ,TPM_RIGHTBUTTON
//                                    ,pos.x
//                                    ,pos.y
//                                    ,this);
//     return TRUE;
//   }
//   return FALSE;
// }

// void 
// MainFrame::OnCustomize()
// {
//   // Get a pointer to the command bars object.
//   CXTPCommandBars* pCommandBars = GetCommandBars();
//   if(pCommandBars != NULL)
//   {
//     // Instanciate the customize dialog object.
//     CXTPCustomizeSheet dlg(pCommandBars);
// 
//     // Add the options page to the customize dialog.
//     CXTPCustomizeOptionsPage pageOptions(&dlg);
//     dlg.AddPage(&pageOptions);
// 
//     // Add the commands page to the customize dialog.
//     CXTPCustomizeCommandsPage* pCommands = dlg.GetCommandsPage();
//     pCommands->AddCategories(IDR_MAINFRAME);
// 
//     // Use the command bar manager to initialize the
//     // customize dialog.
//     pCommands->InsertAllCommandsCategory();
//     pCommands->InsertBuiltInMenus(IDR_MAINFRAME);
//     pCommands->InsertNewMenuCategory();
// 
//     // Dispaly the dialog.
//     dlg.DoModal();
//   }
// }

void MainFrame::OnWindowManager()
{
  ShowWindowsDialog();
}

BOOL 
MainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
  WNDCLASS wndClass;
  BOOL bRes = CMDIFrameWndEx::PreCreateWindow(cs);
  HINSTANCE hInstance = AfxGetInstanceHandle();

  // see if the class already exists
  if (!::GetClassInfo(hInstance, m_cszClassName, &wndClass)) 
  {
    // get default stuff
    ::GetClassInfo(hInstance, cs.lpszClass, &wndClass);
    wndClass.lpszClassName = m_cszClassName;
    wndClass.hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    // register a new class
    if (!AfxRegisterClass(&wndClass))
    {
      AfxThrowResourceException();
    }
  }
  cs.lpszClass = m_cszClassName;

  return bRes;
}

/////////////////////////////////////////////////////////////////////////////
// MainFrame diagnostics

#ifdef _DEBUG
void MainFrame::AssertValid() const
{
	//CMDIFrameWnd::AssertValid();
}

void MainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

void 
MainFrame::OnClassNameChange()
{
  CMFCToolBarComboBoxButton* classNameCombo = GetClassNameCombo();
	int ind = classNameCombo->GetCurSel();
  if(ind >= 0)
  {
    CString szText = classNameCombo->GetItem(ind);
    CMDIChildFrame* frame = (CMDIChildFrame*) MDIGetActive();
    if(frame)
    {
	    CHTMLEdView *pView = frame->GetWebView();
	    if(pView)
      {
	      pView->SetClassName(szText);
      }
    }
  }
}

void 
MainFrame::OnFontNameChange()
{
  CMFCToolBarFontComboBox* fontNameCombo = GetFontNameCombo();
	int ind = fontNameCombo->GetCurSel();
  if(ind >= 0)
  {
    CString szText = fontNameCombo->GetItem(ind);
    CMDIChildFrame* frame = (CMDIChildFrame*) MDIGetActive();
    if(frame)
    {
      CHTMLEdView* pView = frame->GetWebView();
      if(pView)
      {
  	    pView->SetFontFace(szText);
      }
    }
  }
}

void 
MainFrame::OnFontSizeChange()
{
	CString szText;
  CMFCToolBarComboBoxButton* fontSizeCombo = GetFontSizeCombo();
	szText = fontSizeCombo->GetItem(fontSizeCombo->GetCurSel());

  CMDIChildFrame* frame = (CMDIChildFrame*) MDIGetActive();
  if(frame)
  {
	  CHTMLEdView *pView = frame->GetWebView();
	  ASSERT_VALID(pView);

    int fontsize  = _ttoi(szText);
    int pointsize = Misc::FontSizeToPointSize(fontsize);
	  pView->SetFontSize((unsigned short)pointsize);
  }
}

// WEB-VIEW / SOURCE-VIEW on the statusbar
void 
MainFrame::OnUpdatePane(CCmdUI *pCmdUI)
{
	pCmdUI->Enable ();
}

void
MainFrame::SetStatusText(CString text)
{
  if(g_statusBar)
  {
    g_statusBar->SetWindowText(text.GetString());
    g_statusBar->UpdateWindow();
  }
}

// returns the Version of Internet Explorer by reading the file version of MSHTML.DLL
// returns 50 or 55 or 60 (or 0 on error)
// requires Version.lib to be included into the project
UINT 
MainFrame::GetMsieVersion()
{
	static UINT u32_Version = 0;
	if (!u32_Version) // Determine MSIE Version only at the first call
	{
		// read whole file infos into buffer s8_Info
		char s8_Info[3000];
		if (!GetFileVersionInfoA("MSHTML.DLL", 0, sizeof(s8_Info), s8_Info)) 
    {
      return 0;
    }
		UINT u32_Len;
		VS_FIXEDFILEINFO *pk_Info;
		if (!VerQueryValueA(s8_Info, "\\", (void**) &pk_Info, &u32_Len))
    {
      return 0;
    }
		u32_Version = HIWORD(pk_Info->dwFileVersionMS) * 10 + LOWORD(pk_Info->dwFileVersionMS);
	}
	return u32_Version;
}

// returns FALSE for Windows 95, 98, ME and TRUE otherwise
BOOL 
MainFrame::IsPlatformNT()
{
	OSVERSIONINFO k_Os;
	k_Os.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&k_Os);

  switch(k_Os.dwMajorVersion)
  {
    case 4: TRACE("Windows 98\n");    break;
    case 5: switch(k_Os.dwMinorVersion)
            {
              case 0: TRACE("Windows 2000\n");  break;
              case 1: TRACE("Windows XP\n");    break;
            }
            break;
    case 6: TRACE("Windows Vista\n"); break;
  }
	return (k_Os.dwPlatformId == VER_PLATFORM_WIN32_NT);
}

void
MainFrame::OnPreferences()
{
  Spelling* spell = theApp.GetSpeller();
  GlobalSettingsDlg dialog(this,spell);
  dialog.DoModal();
}

void 
MainFrame::OnCreateChild (CMDIChildWnd* pWnd)
{
  static CString strTitle;
  pWnd->GetWindowText(strTitle);
  LVITEM item;
  memset(&item, 0, sizeof(item));
  item.mask    = LVIF_TEXT|LVIF_PARAM;
  item.pszText = (LPSTR)(LPCSTR)strTitle;
  item.lParam  = (LPARAM)pWnd;
  m_openFiles.OpenFiles_Append(item);
}

void 
MainFrame::OnDestroyChild (CMDIChildWnd* pWnd)
{
  m_openFiles.OpenFiles_RemoveByParam((LPARAM)pWnd);

  // 16.03.2003 bug fix, a woorkbook tab toltip shows a wrong path if any left tab has been closed
  CWnd* pMDIClient = GetWindow(GW_CHILD);
  CWnd* pMDIChild = (pMDIClient) ? pMDIClient->GetWindow(GW_CHILD) : 0;
  int   count = 0;

  while (pMDIChild)
  {
    ++count;
    if (pMDIChild->IsKindOf(RUNTIME_CLASS(CMDIChildWnd)))
    {
      CString title;
      pMDIChild->GetWindowText(title);

      if (pMDIChild != pWnd)
      {
        OnRenameChild((CMDIChildWnd*)pMDIChild, title);
      }
    }
    pMDIChild = pMDIChild->GetNextWindow();
  }
  if(count <= 1)
  {
    // LASTMDI
    // Last MDI Child was closed
    // regardless of m_showWorkbookBar
    SetMainMenu(false);
    // No other MDI window will refill the links, so clear this window
    m_wndPageLink2View.RemoveLinks();

    // Reset the classname combo
    CString hint = EDIT_HINT;

//     CMFCToolBarComboBoxButton* classNameCombo = GetClassNameCombo();
//     classNameCombo->ResetContent();
//     classNameCombo->SetCurSel(0);
//     // classNameCombo->SetEditHint(hint);
//     // classNameCombo->SetTooltip (hint);
// 
//     // Reset the font combo
//     CString font_hint = FONT_HINT;
//     CMFCToolBarFontComboBox* fontNameCombo = GetFontNameCombo();
//     // Font combo box must **NOT** reset it's content.
//     // fontNameCombo->ResetContent();
//     fontNameCombo->SetCurSel(-1);
//     // fontNameCombo->SetEditHint(font_hint);
//     // fontNameCombo->SetTooltip (font_hint);
//     
//     // Reset the font size combo
//     CString size_hint = SIZE_HINT;
//     CMFCToolBarComboBoxButton* fontSizeCombo = GetFontSizeCombo();
//     fontSizeCombo->ResetContent();
//     fontSizeCombo->SetCurSel(0);
//     // fontSizeCombo->SetEditHint(size_hint);
//     // fontSizeCombo->SetTooltip (size_hint);

    // See if we were the last action
    ProjectFile* project = theApp.GetProjectFile();
    if(project == NULL)
    {
      CloseAllPanes();
      // Repost startup dialog after closing of last 
      ::PostMessage(theApp.m_pMainWnd->GetSafeHwnd(),WM_COMMAND,ID_STARTUP,0);
    }
  }
}

void
MainFrame::SetMainMenu(bool full)
{
  CWnd*  mainWnd  = AfxGetMainWnd();
  CMenu* mainMenu = AfxGetApp()->GetMainWnd()->GetMenu();
  if(mainMenu)
  {
    if(!full)
    {
      // REDUCE MAIN MENU
      if(mainMenu->GetMenuItemCount() > 8)
      {
        mainMenu->DeleteMenu(3,MF_BYPOSITION);
        mainMenu->DeleteMenu(3,MF_BYPOSITION);
        mainMenu->DeleteMenu(3,MF_BYPOSITION);
        mainMenu->DeleteMenu(3,MF_BYPOSITION);
        mainWnd->DrawMenuBar();
      }
    }
  }
}

void
MainFrame::OnMDIMaximize()
{
  CWnd* pMDIClient = GetWindow(GW_CHILD);
  CWnd* pMDIChild = (pMDIClient) ? pMDIClient->GetWindow(GW_CHILD) : 0;

  if(pMDIChild)
  {
    CMDIFrameWnd::MDIMaximize(pMDIChild);
  }
}

void
MainFrame::OnMDIMaximizeUI(CCmdUI* pCmdUI)
{
  CWnd* pMDIClient = GetWindow(GW_CHILD);
  CWnd* pMDIChild = (pMDIClient) ? pMDIClient->GetWindow(GW_CHILD) : 0;

  pCmdUI->Enable(pMDIChild != NULL);
}

void MainFrame::OnActivateChild(CMDIChildWnd* pWnd)
{
  m_hLastChild = m_hActiveChild;
  m_hActiveChild = pWnd ? pWnd->m_hWnd : NULL;
  m_openFiles.OpenFiles_ActivateByParam((LPARAM)pWnd);

  CMDIChildFrame* frame = dynamic_cast<CMDIChildFrame*>(pWnd);
  if(frame && frame->GetWebView())
  {
    CHTMLEdView* view = frame->GetWebView();
    if(view)
    {
      if(m_lockLink == false)
      {
        m_wndPageLink2View.ActivatePage(view);
      }
    }
  }
}

void MainFrame::OnRenameChild (CMDIChildWnd* pWnd, LPCTSTR szTitle)
{
  int iImage = GetImageByDocument(pWnd->GetActiveDocument());
  LVITEM item;  
  memset(&item, 0, sizeof(item));
  item.mask    = LVIF_TEXT|LVIF_PARAM;
  item.pszText = (LPTSTR)szTitle;
  item.lParam  = (LPARAM)pWnd;
  //if (iImage != -1) 
  //{
  item.iImage = iImage;
  item.mask |= LVIF_IMAGE;
  //}
  m_openFiles.OpenFiles_UpdateByParam((LPARAM)pWnd, item);
}

int MainFrame::GetImageByDocument (const CDocument* pDoc)
{
  if (pDoc)
  {
    CString path = pDoc->GetPathName();

    if (path.IsEmpty()) path   = "*.html"; // "*." + m_defaultFileExtension;

    BOOL newDoc = !PathFileExists(path);

    SHFILEINFO shFinfo;
    if (SHGetFileInfo(path, FILE_ATTRIBUTE_NORMAL, &shFinfo, sizeof(shFinfo), 
      (newDoc ? SHGFI_ICON|SHGFI_SMALLICON|SHGFI_USEFILEATTRIBUTES : SHGFI_ICON|SHGFI_SMALLICON)))
    {
      DestroyIcon(shFinfo.hIcon);
      return shFinfo.iIcon;
    }
  }
  return -1;
}


void 
MainFrame::ActivateChild (CMDIChildWnd* child)
{
  BOOL bMaximized;
  MDIGetActive(&bMaximized);

  if (!bMaximized  && child->IsIconic())
  {
    child->MDIRestore();
  }
  else
  {
    child->MDIActivate();
  }
}

// This handler remove Workbook Tab resizing bug on maximize/restore
//
void MainFrame::OnSysCommand(UINT nID,LPARAM lParam)
{
  if ((nID & 0xFFF0) == SC_CLOSE)
  {
    if (!theApp.CanExitInstance()) 
    {
      return;
    }
  }
  CMDIFrameWndEx::OnSysCommand(nID, lParam);

  UINT nItemID = (nID & 0xFFF0);
  // 16.03.2003 bug fix, violetion in debug mode when the application is closing
  if ((nItemID == SC_MAXIMIZE || nItemID == SC_RESTORE) && !m_bHelpMode)
  {
    RecalcLayout(TRUE/*bNotify*/);
  }
}

#undef  _countof
#define _countof(array)         (sizeof(array)/sizeof(array[0]))
#define _AfxGetDlgCtrlID(hWnd)  ((UINT)(WORD)::GetDlgCtrlID(hWnd))

BOOL MainFrame::OnToolTipText (UINT, NMHDR* pNMHDR, LRESULT* pResult)
{
  ASSERT(pNMHDR->code == TTN_NEEDTEXTA || pNMHDR->code == TTN_NEEDTEXTW);

  // need to handle both ANSI and UNICODE versions of the message
  TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
  TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
  TCHAR szFullText[256];
  CString strTipText;
  UINT_PTR nID = pNMHDR->idFrom;
  if (pNMHDR->code == TTN_NEEDTEXTA && (pTTTA->uFlags & TTF_IDISHWND) ||
    pNMHDR->code == TTN_NEEDTEXTW && (pTTTW->uFlags & TTF_IDISHWND))
  {
    // idFrom is actually the HWND of the tool
    nID = _AfxGetDlgCtrlID((HWND)nID);
  }

  if (nID != 0) // will be zero on a separator
  {
    // don't handle the message if no string resource found
    if (AfxLoadString((UINT)nID, szFullText) == 0)
      return FALSE;

    // this is the command id, not the button index
    AfxExtractSubString(strTipText, szFullText, 1, '\n');

    //std::string accLabel;
    //if (Common::GUICommandDictionary::GetCommandAccelLabel(static_cast<Common::Command>(nID), accLabel))
    //{
    //    strTipText += " (";
    //    strTipText += accLabel.c_str();
    //    strTipText +=  ")";
    //}
  }

#ifndef _UNICODE
  if (pNMHDR->code == TTN_NEEDTEXTA)
    lstrcpyn(pTTTA->szText, strTipText, _countof(pTTTA->szText));
  else
    _mbstowcsz(pTTTW->szText, strTipText, _countof(pTTTW->szText));
#else
  if (pNMHDR->code == TTN_NEEDTEXTA)
    _wcstombsz(pTTTA->szText, strTipText, _countof(pTTTA->szText));
  else
    lstrcpyn(pTTTW->szText, strTipText, _countof(pTTTW->szText));
#endif
  *pResult = 0;

  // bring the tooltip window above other popup windows
  ::SetWindowPos(pNMHDR->hwndFrom, HWND_TOP, 0, 0, 0, 0,
    SWP_NOACTIVATE|SWP_NOSIZE|SWP_NOMOVE|SWP_NOOWNERZORDER);

  return TRUE;    // message was handled
}

void MainFrame::OnLastWindow ()
{
  if (!m_bMDINextSeq && m_hLastChild && ::IsWindow(m_hLastChild) )
  {
    CMDIChildWnd* active = MDIGetActive();
    m_hSkipChild = active ? active->m_hWnd : NULL;

    MDIActivate(FromHandle(m_hLastChild));
    m_bMDINextSeq = TRUE;
  }
  else
  {
    MDINext();
    CMDIChildWnd* active = MDIGetActive();

    if (active && active->m_hWnd == m_hSkipChild)
    {
      MDINext();
    }
    m_hSkipChild = NULL;
  }
}

void
MainFrame::OnCloseOthers()
{
  CMDIChildWnd* wnd = MDIGetActive();
  CMDIChildWnd* pnt = wnd;
  if(!pnt)
  {
    // Make sure we have an active document
    return;
  }
  do
  {
    if(pnt != wnd)
    {
      CDocument* doc = pnt->GetActiveDocument();
      doc->OnCloseDocument();
    }
    pnt = (CMDIChildWnd*)wnd->GetNextWindow();
  }
  while(pnt && (pnt != wnd));
}

BOOL
MainFrame::SetSubtree(const CString& path)
{
  return m_wndExplorerView.SetSubtree(path);
}

void
MainFrame::ClosePane(int nID)
{
  CBasePane* pane = GetPane(nID);
  if(pane)
  {
    CMDIFrameWndEx::ShowPane(pane,FALSE,FALSE,FALSE);
  }
}

void
MainFrame::ShowPane(int nID)
{
  CBasePane* pane = GetPane(nID);
  if(pane)
  {
    CMDIFrameWndEx::ShowPane(pane,TRUE,FALSE,FALSE);
  }
}

void
MainFrame::OnShowPane(UINT nID)
{
  CBasePane* pane = GetPane(nID);
  if(pane)
  {
    ShowPane(nID,!pane->IsVisible());
  }
}

void
MainFrame::ShowPane(int nID,BOOL focus)
{
  TOC*           toc     = nullptr;
  IndexFile*     index   = nullptr;
  ProjectFile*   project = nullptr;
  CDockablePane* pane    = nullptr;

  // Check if we may show the pane
  switch(nID)
  {
    case ID_PANE_TOC:       toc = theApp.GetTOC();
                            if(!toc)
                            {
                              return;
                            }
                            pane = &m_wndTOCView;
                            break;
    case ID_PANE_KEYWORDS:  index = theApp.GetIndex();
                            if(!index)
                            {
                              return;
                            }
                            pane = &m_wndIndexView;
                            break;
    case ID_PANE_LINKS:     pane = &m_wndPageLink2View;
                            break;
    case ID_PANE_COMPILE:   pane = &m_wndCompileView;
                            break;
    case ID_PANE_PROJECT:   project = theApp.GetProjectFile();
                            if(!project)
                            {
                              return;
                            }
                            pane = &m_wndProjectView;
                            break;
    case ID_PANE_DOCUMENTS: pane = &m_openFiles;
                            break;
    case ID_PANE_EXPLORER:  pane = &m_wndExplorerView;
                            break;
  }
  if(pane)
  {
    CMDIFrameWndEx::ShowPane(pane,TRUE,FALSE,focus);
  }
}

void
MainFrame::OnUpdateUIPaneToc(CCmdUI* pCmdUI)
{
  pCmdUI->SetCheck(m_wndTOCView.IsVisible());
}

void
MainFrame::OnUpdateUIPaneIndex(CCmdUI* pCmdUI)
{
  pCmdUI->SetCheck(m_wndIndexView.IsVisible());
}

void MainFrame::OnUpdateUILinks(CCmdUI* pCmdUI)
{
  pCmdUI->SetCheck(m_wndPageLink2View.IsVisible());

}

void MainFrame::OnUpdateUIExplorer(CCmdUI* pCmdUI)
{
  pCmdUI->SetCheck(m_wndExplorerView.IsVisible());
}

void MainFrame::OnUpdateUIDocuments(CCmdUI* pCmdUI)
{
  pCmdUI->SetCheck(m_openFiles.IsVisible());
}

void
MainFrame::OnUpdateUIPaneProject(CCmdUI* pCmdUI)
{
  pCmdUI->SetCheck(m_wndProjectView.IsVisible());
}

void
MainFrame::OnUpdateUIPaneCompile(CCmdUI* pCmdUI)
{
  pCmdUI->SetCheck(m_wndCompileView.IsVisible());
}

void
MainFrame::OnUpdateUI(CCmdUI* pCmdUI)
{
  pCmdUI->Enable(TRUE);
}

//////////////////////////////////////////////////////////////////////////////
//
// THEMES 
//
//////////////////////////////////////////////////////////////////////////////


void 
MainFrame::RedrawFrame(CWnd* pWnd)
{
  pWnd->RedrawWindow(0, 0, RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ALLCHILDREN);		
}

void MainFrame::OnApplicationLook(UINT id)
{
  CWaitCursor wait;

  theApp.m_nAppLook = id;

  switch(theApp.m_nAppLook)
  {
    case ID_VIEW_APPLOOK_WIN_2000:  CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
                                    break;
    case ID_VIEW_APPLOOK_OFF_XP:    CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
                                    break;
    case ID_VIEW_APPLOOK_WIN_XP:    CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
                                    CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
                                    break;
    case ID_VIEW_APPLOOK_OFF_2003:  CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
                                    CDockingManager::SetDockingMode(DT_SMART);
                                    break;
    case ID_VIEW_APPLOOK_VS_2005:   CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
                                    CDockingManager::SetDockingMode(DT_SMART);
                                    break;
    case ID_VIEW_APPLOOK_VS_2008:   CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
                                    CDockingManager::SetDockingMode(DT_SMART);
                                    break;
    case ID_VIEW_APPLOOK_WINDOWS_7: CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
                                    CDockingManager::SetDockingMode(DT_SMART);
                                    break;

    default:
      switch(theApp.m_nAppLook)
      {
        case ID_VIEW_APPLOOK_OFF_2007_BLUE:   CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
                                              break;
        case ID_VIEW_APPLOOK_OFF_2007_BLACK:  CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
                                              break;
        case ID_VIEW_APPLOOK_OFF_2007_SILVER: CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
                                              break;
        case ID_VIEW_APPLOOK_OFF_2007_AQUA:   CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
                                              break;
      }
      CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
      CDockingManager::SetDockingMode(DT_SMART);
  }

  CTabbedPane::ResetTabs();

  RecalcLayout();
  RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE);

  theApp.WriteInt(_T("ApplicationLook"), m_nAppLook);
}

void
MainFrame::OnBrokenLinks()
{
  ProjectFile* project = theApp.GetProjectFile();
  if(project)
  {
    BrokenLinksDlg dlg(this,project);
    dlg.DoModal();
  }
  else
  {
    theApp.MessageBox("Currently no project loaded. Sorry","Error",MB_OK|MB_ICONERROR);
  }
}


//////////////////////////////////////////////////////////////////////////
//
// Renaming of files
// 
//////////////////////////////////////////////////////////////////////////

bool
MainFrame::RenameFile(CString& p_old_href,CString& p_new_href)
{
  // Try opening file for rename-copy
  CString newFileName = theApp.GetBaseDirectory() + p_new_href;

  FILE* fpOutput = fopen(newFileName,"w");
  if(fpOutput == NULL)
  {
    CString message;
    message.Format("Cannot create the new file [%s] for renaming the current topic file.",newFileName.GetString());
    theApp.MessageBox(message,"ERROR Renaming file",MB_OK|MB_ICONERROR);
    return false;
  }
  fclose(fpOutput);

  // Do all files in the Project 
  // Do it before opening the old filename. It's handle is needed for renaming
  // it's due to be rewritten by the tidy sweep process.
  ProjectFile* project = theApp.GetProjectFile();
  if(project)
  {
    // Rename across all files
    project->RenameFile(p_old_href,p_new_href);
  }

  // Do the rename in all views listed in the OpenFiles member
  int number = 0;
  CMDIChildFrame* child = (CMDIChildFrame*) m_openFiles.OpenFiles_FindParamByNumber(number);
  while(child)
  {
    // Find next child window
    child = (CMDIChildFrame*) m_openFiles.OpenFiles_FindParamByNumber(number++);
    if(child)
    {
      CHTMLEdView* view = child->GetWebView();
      view->RenameReferences(p_old_href,p_new_href);
    }
  }
  return true;
}

bool
MainFrame::CopyFileForRename(CString& p_old_href,CString& p_new_href)
{
  // Try opening file for rename-copy
//   CString oldFileName = theApp.GetBaseDirectory() + p_old_href;
//   CString newFileName = theApp.GetBaseDirectory() + p_new_href;

  FILE* fpOutput = fopen(p_new_href,"w");
  if(fpOutput == NULL)
  {
    CString message;
    message.Format("Cannot create the new file [%s] for renaming the current topic file.",p_new_href.GetString());
    theApp.MessageBox(message,"ERROR Renaming file",MB_OK|MB_ICONERROR);
    return false;
  }
  FILE* fpInput = fopen(p_old_href,"r");
  if(fpInput)
  {
    int ch = 0;
    while((ch = fgetc(fpInput)) != EOF)
    {
      fputc(ch,fpOutput);
    }
    bool fileError = false;
    if(fclose(fpInput) == EOF)
    {
      fileError = true;
    }
    if(fclose(fpOutput) == EOF)
    {
      fileError = true;
    }
    if(fileError)
    {
      CString message;
      message.Format("File error re-writing file to: %s",p_new_href.GetString());
      theApp.MessageBox(message,"ERROR rewriting",MB_OK|MB_ICONERROR);
      return false;
    }
  }
  else
  {
    CString message;
    message.Format("File error opening file for reading: %s",p_old_href.GetString());
    theApp.MessageBox(message,"ERROR rewriting",MB_OK|MB_ICONERROR);
    return false;
  }

  // Delete the original
  if(DeleteFile(p_old_href) == 0)
  {
    CString message;
    message.Format("Could not delete the original file: %s\n"
                   "Since everything else went fine, we will continue the renaming process.",p_old_href.GetString());
    theApp.MessageBox(message,"ERROR deleting file",MB_OK|MB_ICONERROR);
  }
  return true;
}

LRESULT 
MainFrame::OnRestoreToolbar(WPARAM wp,LPARAM)
{
  OnToolbarReset(ID_TOOLBAR_EDITING,0);

  GetDockingManager()->RecalcLayout();
  GetDockingManager()->AdjustDockingLayout();

  // ReDockControlBar();

  return 0;
}

LRESULT
MainFrame::OnToolbarReset(WPARAM wp,LPARAM /*lp*/)
{
  UINT uiToolBarId = (UINT)wp;
  switch(uiToolBarId)
  {
    case ID_TOOLBAR_EDITING:
    {
      // Replace "ClassName" button for a combo box 
      CMFCToolBarComboBoxButton nameButton(ID_FMTBAR_CLASSNAME,-1,CBS_DROPDOWNLIST,300);
      BOOL result = m_wndEditBar.ReplaceButton(ID_FMTBAR_CLASSNAME,nameButton);

      // Replace "FontName" button for a combo box 
      int fontType = DEVICE_FONTTYPE | RASTER_FONTTYPE | TRUETYPE_FONTTYPE;
      CMFCToolBarFontComboBox fontButton(ID_FMTBAR_FONTNAME,-1,fontType,DEFAULT_CHARSET,CBS_DROPDOWN,300,DEFAULT_PITCH);
      result = m_wndEditBar.ReplaceButton(ID_FMTBAR_FONTNAME,fontButton);

      // Replace "FonSize" button for a combo box 
      // CMFCToolBarComboBoxButton sizeButton(ID_FMTBAR_FONTSIZE, GetCmdMgr()->GetCmdImage(ID_FMTBAR_FONTSIZE, FALSE), CBS_DROPDOWNLIST,75);
      CMFCToolBarComboBoxButton sizeButton(ID_FMTBAR_FONTSIZE, -1, CBS_DROPDOWNLIST,75);
      result = m_wndEditBar.ReplaceButton(ID_FMTBAR_FONTSIZE, sizeButton);

      m_wndEditBar.AdjustLayout();
      m_wndEditBar.Invalidate();
      m_wndEditBar.UpdateWindow();
    }
    break;
  }
  return 0;
}

void
MainFrame::ReDockControlBar()
{
  if(m_wndToolBar.GetSafeHwnd() == NULL)
  {
    return;
  }
  SendMessage(WM_SETREDRAW,FALSE,0);

  m_wndToolBar  .UndockPane();
  m_wndEditBar  .UndockPane();
  m_wndTableBar .UndockPane();
  m_wndWindowBar.UndockPane();
  m_wndLinkBar  .UndockPane();

  if(m_wndToolBar.IsVisible())    DockPane(&m_wndToolBar);
  if(m_wndEditBar.IsVisible())    DockPane(&m_wndEditBar);
  if(m_wndTableBar.IsVisible())   DockPane(&m_wndTableBar);
  if(m_wndWindowBar.IsVisible())  DockPane(&m_wndWindowBar);
  if(m_wndLinkBar.IsVisible())    DockPane(&m_wndLinkBar);

  ReDockLeftOf(nullptr);
  ReDockLeftOf(&m_wndTableBar);
  ReDockLeftOf(&m_wndEditBar);

  ReDockLeftOf(nullptr);  // RESET
  ReDockLeftOf(&m_wndWindowBar);
  ReDockLeftOf(&m_wndLinkBar);
  ReDockLeftOf(&m_wndToolBar);


  SendMessage(WM_SETREDRAW,TRUE,0);
  RedrawWindow(NULL,NULL,RDW_INVALIDATE | RDW_FRAME | RDW_ALLCHILDREN);
}

// Shift register to dock toolbars
void
MainFrame::ReDockLeftOf(CMFCToolBar* p_bar)
{
  static CMFCToolBar* last = nullptr;

  if(p_bar == nullptr)
  {
    last = nullptr;
    return;
  }
  if(last == nullptr)
  {
    if(p_bar->IsVisible())
    {
      last = p_bar;
    }
    return;
  }
  if(p_bar->IsVisible())
  {
    DockPaneLeftOf(p_bar,last);
    last = p_bar;
  }
}

void
MainFrame::OnSize(UINT nType,int cx,int cy)
{
  CMDIFrameWndEx::OnSize(nType,cx,cy);
  // ReDockControlBar();
}

void
MainFrame::OnViewCustomize()
{
  // Create a customize toolbars dialog:
  CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* Automatic menus scaning */
                                                                         ,AFX_CUSTOMIZE_MENU_SHADOWS | AFX_CUSTOMIZE_TEXT_LABELS | AFX_CUSTOMIZE_MENU_ANIMATIONS);

  CMFCToolBarComboBoxButton nameButton(ID_FMTBAR_CLASSNAME, GetCmdMgr()->GetCmdImage(ID_FMTBAR_CLASSNAME, FALSE), CBS_DROPDOWNLIST, 300);

  // Replace "FontName" button for a combo box 
  int fontType = DEVICE_FONTTYPE | RASTER_FONTTYPE | TRUETYPE_FONTTYPE;
  CMFCToolBarFontComboBox fontButton(ID_FMTBAR_FONTNAME, GetCmdMgr()->GetCmdImage(ID_FMTBAR_FONTNAME, FALSE)
                                    ,fontType, DEFAULT_CHARSET, CBS_DROPDOWN, 300, DEFAULT_PITCH);

  // Replace "FonSize" button for a combo box 
  CMFCToolBarComboBoxButton sizeButton(ID_FMTBAR_FONTSIZE, GetCmdMgr()->GetCmdImage(ID_FMTBAR_FONTSIZE, FALSE), CBS_DROPDOWNLIST, 75);

  pDlgCust->EnableUserDefinedToolbars();
  pDlgCust->ReplaceButton(ID_FMTBAR_CLASSNAME,nameButton);
  pDlgCust->ReplaceButton(ID_FMTBAR_FONTNAME, fontButton);
  pDlgCust->ReplaceButton(ID_FMTBAR_FONTSIZE, sizeButton);

  pDlgCust->Create();
}

void 
MainFrame::OnAppLook(UINT id)
{
  CDockingManager::SetDockingMode(DT_SMART);

  m_nAppLook = id;

  CTabbedPane::m_StyleTabWnd = CMFCTabCtrl::STYLE_3D;

  switch (m_nAppLook)
  {
  case ID_VIEW_APPLOOK_2007:
  case ID_VIEW_APPLOOK_2007_1:
  case ID_VIEW_APPLOOK_2007_2:
  case ID_VIEW_APPLOOK_2007_3:
    // enable Office 2007 look:

    switch (m_nAppLook)
    {
    case ID_VIEW_APPLOOK_2007:
      CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
      break;

    case ID_VIEW_APPLOOK_2007_1:
      CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
      break;

    case ID_VIEW_APPLOOK_2007_2:
      CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
      break;

    case ID_VIEW_APPLOOK_2007_3:
      CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
      break;
    }

    CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
    CDockingManager::SetDockingMode(DT_SMART);
    break;

  default:
    ASSERT(FALSE);
  }

  CDockingManager* pDockManager = GetDockingManager();
  if (pDockManager != NULL)
  {
    ASSERT_VALID(pDockManager);
    pDockManager->AdjustPaneFrames();
  }

  CTabbedPane::ResetTabs();

  RecalcLayout();
  RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE);

  theApp.WriteInt(_T("ApplicationLook"), m_nAppLook);
}

void
MainFrame::OnUpdateAppLook(CCmdUI* pCmdUI)
{
  pCmdUI->SetRadio(m_nAppLook == pCmdUI->m_nID);
}

CMDIChildWndEx* 
MainFrame::CreateDocumentWindow(LPCTSTR /*lpcszDocName*/, CObject* /*pObj*/)
{
  return nullptr;
}
