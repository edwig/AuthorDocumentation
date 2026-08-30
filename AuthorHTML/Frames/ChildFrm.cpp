//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    ChildFrm.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// The Child frame for a documentation HTML file
//
#include "stdafx.h"
#include "AuthorHTML.h"
#include "ChildFrm.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMDIChildFrame

IMPLEMENT_DYNCREATE(CMDIChildFrame,CMDIChildWndEx)

BEGIN_MESSAGE_MAP(CMDIChildFrame,CMDIChildWndEx)
	ON_WM_DESTROY()
	ON_WM_MDIACTIVATE()
  ON_WM_SYSCOMMAND()
	
	ON_COMMAND_RANGE          (ID_COLOR_BLUE, ID_COLOR_MAGENTA, OnSelectColor)
	ON_UPDATE_COMMAND_UI_RANGE(ID_COLOR_BLUE, ID_COLOR_MAGENTA, OnUpdateSelectColor)

	ON_COMMAND          (ID_COLOR_NONE,         OnSelectColorNone)
	ON_UPDATE_COMMAND_UI(ID_COLOR_NONE,         OnUpdateSelectColorNone)
  ON_MESSAGE(WM_SETTEXT,                      OnSetText)
  ON_COMMAND(ID_VIEW_SOURCE,                  OnViewSource)
  ON_COMMAND(ID_VIEW_WEB,                     OnViewWeb)
  ON_UPDATE_COMMAND_UI(ID_VIEW_WEB,           OnUpdateViewWeb)
  ON_UPDATE_COMMAND_UI(ID_VIEW_SOURCE,        OnUpdateViewSource)
  ON_UPDATE_COMMAND_UI(ID_FMTBAR_CLASSNAME,   OnUpdateClassName)
  ON_UPDATE_COMMAND_UI(ID_FMTBAR_FONTSIZE,    OnUpdateFontSize)
  ON_UPDATE_COMMAND_UI(ID_FMTBAR_FONTNAME,    OnUpdateFontName)
END_MESSAGE_MAP()

// CMDIChildFrame construction/destruction

CMDIChildFrame::CMDIChildFrame()
               :m_pSrcView(NULL)
               ,m_pWebView(NULL)
               ,m_dwCurrentView(ID_VIEW_WEB)
{
// 	m_nColor = nColor++;
// 	if (nColor > xtpTabColorMagenta)
//   {
//     nColor = xtpTabColorBlue;
//   }
}

CMDIChildFrame::~CMDIChildFrame()
{
}

LRESULT 
CMDIChildFrame::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_NCPAINT)
	{
		// prevent caption blinking
		return TRUE;
	}
	return CMDIChildWndEx::WindowProc(message, wParam, lParam);
}

BOOL 
CMDIChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIChildWndEx::PreCreateWindow(cs) )
  {
    return FALSE;
  }
  cs.style = WS_CHILD | WS_VISIBLE | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU |
             FWS_ADDTOTITLE | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_MAXIMIZE;
  return TRUE;
}

int 
CMDIChildFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  if (CMDIChildWndEx::OnCreate(lpCreateStruct) == -1)
  {
    return -1;
  }
  return 0;
}

BOOL 
CMDIChildFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{	
  CDocument *pDoc=NULL;
  POSITION pos;
  BOOL bRet = FALSE;
  //create the source view for this document at this time
  CRuntimeClass *prc = RUNTIME_CLASS(ScintillaHTMLView);
  ASSERT(prc);
  m_pSrcView = (ScintillaHTMLView*)prc->CreateObject();
  if(m_pSrcView)
  {
    VERIFY(m_pSrcView->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
    CRect(0,0,0,0), this, AFX_IDW_PANE_LAST, NULL));
  }
  //create the doc-template view and save off a pointer to it
  // SDI-MDI
  bRet = CMDIChildWndEx::OnCreateClient(lpcs, pContext);
  pDoc = pContext->m_pCurrentDoc;
  if(bRet && pDoc)
  {
    pos = pDoc->GetFirstViewPosition();
    pDoc->AddView(m_pSrcView);
  }
  else
  {
    return FALSE;
  }
  CView *pView = pDoc->GetNextView(pos);
  ASSERT(pView->IsKindOf(RUNTIME_CLASS(CHTMLEdView)));
  if(pView)
  {
    m_pWebView = (CHTMLEdView*) pView;
  }
  else
  {
    return FALSE;
  }
  // Workbook implementation
  ASSERT_KINDOF(MainFrame, GetMDIFrame());
  ((MainFrame*)GetMDIFrame())->OnCreateChild(this);

  return bRet;
}
 
void 
CMDIChildFrame::OnDestroy ()
{
  CMDIChildWndEx::OnDestroy();
  ((MainFrame*)GetMDIFrame())->OnDestroyChild(this);
}

// This handler remove Workbook Tab resizing bug on maximize/restore
//
void
CMDIChildFrame::OnSysCommand(UINT nID,LPARAM lParam)
{
  CMDIChildWndEx::OnSysCommand(nID, lParam);
	UINT nItemID = (nID & 0xFFF0);
  // Bug fix, restore workbookbar on normal maximize instead of MDImaximize
	if ((nItemID == SC_MAXIMIZE) && !m_bHelpMode)
  {
    ((MainFrame*)GetMDIFrame())->OnMDIMaximize();
  }
}

void 
CMDIChildFrame::OnMDIActivate (BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd)
{
  CMDIChildWndEx::OnMDIActivate(bActivate, pActivateWnd, pDeactivateWnd);
  if (bActivate && this == pActivateWnd)
  {
    ((MainFrame*)GetMDIFrame())->OnActivateChild(this);
  }
}

LRESULT 
CMDIChildFrame::OnSetText (WPARAM, LPARAM lParam)
{
  Default();

  ((MainFrame*)GetMDIFrame())->OnRenameChild(this, (LPCTSTR)lParam);

  return TRUE;
}

extern CMFCStatusBar* g_statusBar;

void 
CMDIChildFrame::SwapView(int nCmdID)
{
  CDocument *pDoc;

  pDoc = GetActiveDocument();

  if(nCmdID == ID_VIEW_SOURCE)
  {
    //swap the view IDs
    m_pWebView->SetDlgCtrlID(AFX_IDW_PANE_LAST);
    m_pSrcView->SetDlgCtrlID(AFX_IDW_PANE_FIRST);

    // Saves the document to disk, so the other document view
    // can handle it as 'non-saved, not altered
    m_pWebView->GetDocument()->SaveMyDocument();

    // show/hide the right view
    m_pWebView->ShowWindow(SW_HIDE);
    m_pSrcView->ShowWindow(SW_SHOW);

    //re-layout everything
    SetActiveView(m_pSrcView);
    RecalcLayout();

    m_dwCurrentView = ID_VIEW_SOURCE;

    g_statusBar->SetPaneText(1, _T("SRC VIEW")); //, TRUE);

    return;
  }
  else if(nCmdID == ID_VIEW_WEB)
  {
    //swap the view IDs
    m_pWebView->SetDlgCtrlID(AFX_IDW_PANE_FIRST);
    m_pSrcView->SetDlgCtrlID(AFX_IDW_PANE_LAST);

    // Saves the document to disk, so the other document view
    // can handle it as 'non-saved, not altered
    m_pSrcView->GetDocument()->SaveMyDocument();

    // show/hide the right view
    m_pWebView->ShowWindow(SW_SHOW);
    m_pSrcView->ShowWindow(SW_HIDE);

    //re-layout everything
    SetActiveView(m_pWebView);
    RecalcLayout();

    m_dwCurrentView = ID_VIEW_WEB;
    g_statusBar->SetPaneText(1, _T("WEB VIEW")); // , TRUE);
    return;
  }
}

void 
CMDIChildFrame::SwapView()
{
  //if we are already showing the right view, bail, nothing to do.
  DWORD nCmdID = LOWORD(GetCurrentMessage()->wParam);   
  if((nCmdID == ID_VIEW_SOURCE) && (m_dwCurrentView == ID_VIEW_SOURCE))
  {
    return;
  }
  if((nCmdID == ID_VIEW_WEB) && (m_dwCurrentView == ID_VIEW_WEB))
  {
    return;
  }
  SwapView(nCmdID);
}

void 
CMDIChildFrame::OnViewSource() 
{
  SwapView();	
}

void 
CMDIChildFrame::OnViewWeb() 
{
  SwapView();
}

CView*
CMDIChildFrame::GetActiveView()
{
  if(m_dwCurrentView == ID_VIEW_WEB)
  {
    return (CView*) m_pWebView;
  }
  return (CView*) m_pSrcView;
}

void 
CMDIChildFrame::OnUpdateViewWeb(CCmdUI* pCmdUI) 
{
  pCmdUI->SetRadio(m_dwCurrentView==ID_VIEW_WEB);

}

void 
CMDIChildFrame::OnUpdateViewSource(CCmdUI* pCmdUI) 
{
  pCmdUI->SetRadio(m_dwCurrentView==ID_VIEW_SOURCE);
}

CView* 
CMDIChildFrame::GetSourceView()
{
  return DYNAMIC_DOWNCAST(CView,m_pSrcView);
}

DWORD 
CMDIChildFrame::GetCurrentView()
{
  return m_dwCurrentView;
}

#pragma warning (disable:4838)
void 
CMDIChildFrame::OnUpdateFontSize(CCmdUI* /*pCmdUI*/)
{
  MainFrame* mFrame = (MainFrame*) AfxGetMainWnd();
  CMFCToolBarComboBoxButton *pCombo = mFrame->GetFontSizeCombo();
  ASSERT_VALID(pCombo);

  //If Source View, don't display the font size combobox
  if(m_dwCurrentView == ID_VIEW_SOURCE)
  {
    pCombo->EnableWindow(FALSE);
    return;
  }
  else
  {
    if(m_pWebView && m_pWebView->QueryStatus((long)IDM_BLOCKFMT) & OLECMDF_ENABLED)
    {
      pCombo->EnableWindow();
    }
    else
    {
      pCombo->EnableWindow(FALSE);
    }
  }
}
#pragma warning (error: 4838)

void
CMDIChildFrame::OnUpdateClassName(CCmdUI* /*pCmdUI*/)
{
  MainFrame* mFrame = (MainFrame*) AfxGetMainWnd();
  CMFCToolBarComboBoxButton *pCombo = mFrame->GetClassNameCombo();
  ASSERT_VALID(pCombo);

  if(m_dwCurrentView == ID_VIEW_SOURCE)
  {
    pCombo->EnableWindow(FALSE);
    return;
  }
  else
  {
    if(m_pWebView && m_pWebView->QueryStatus((long)IDM_BLOCKFMT) & OLECMDF_ENABLED)
    {
      pCombo->EnableWindow();
    }
    else
    {
      pCombo->EnableWindow(FALSE);
    }
  }
}

void 
CMDIChildFrame::OnUpdateFontName(CCmdUI* /*pCmdUI*/)
{
  MainFrame* mFrame = (MainFrame*) AfxGetMainWnd();
  CMFCToolBarFontComboBox *pCombo = mFrame->GetFontNameCombo();
  ASSERT_VALID(pCombo);

  //If Source View, don't display the font name combobox
  if(m_dwCurrentView == ID_VIEW_SOURCE)
  {
    pCombo->EnableWindow(FALSE);
    return;
  }
  else
  {
    if(m_pWebView && m_pWebView->QueryStatus(IDM_BLOCKFMT) & OLECMDF_ENABLED)
    {
      pCombo->EnableWindow();
    }
    else
    {
      pCombo->EnableWindow(FALSE);
    }
  }
}

// CMDIChildFrame diagnostics

#ifdef _DEBUG
void CMDIChildFrame::AssertValid() const
{
	CMDIChildWndEx::AssertValid();
}

void CMDIChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWndEx::Dump(dc);
}

#endif //_DEBUG

LRESULT 
CMDIChildFrame::OnGetTabColor(WPARAM, LPARAM)
{
	// Can return any one of the XTPTabOneNoteColor RGB color values or any
	// valid COLORREF value for example:
	//
	// return CXTPTabPaintManager::GetOneNoteColor(xtpTabColorOrange);
	return m_nColor;
}

void 
CMDIChildFrame::OnSelectColor(UINT p_nColor)
{
  m_nColor = p_nColor - ID_COLOR_BLUE; // +xtpTabColorBlue;
}

void 
CMDIChildFrame::OnUpdateSelectColor(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck((int)pCmdUI->m_nID - ID_COLOR_BLUE == m_nColor/* - xtpTabColorBlue*/);

}

void 
CMDIChildFrame::OnSelectColorNone()
{
	m_nColor = RGB(255, 255, 255);
}

void 
CMDIChildFrame::OnUpdateSelectColorNone(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_nColor == RGB(255, 255, 255));
}
