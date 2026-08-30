#include "stdafx.h"
#include "AuthorHTML.h"
#include "ScintillaChildFrm.h"
#include "ScintillaScriptDoc.h"
#include "ScintillaScriptView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(ScintillaChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(ScintillaChildFrame, CMDIChildWnd)
  ON_WM_DESTROY()
  ON_WM_MDIACTIVATE()
  ON_WM_SYSCOMMAND()
  ON_MESSAGE(WM_SETTEXT,OnSetText)
	ON_WM_MOVE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

ScintillaChildFrame::ScintillaChildFrame()
{
}

ScintillaChildFrame::~ScintillaChildFrame()
{
}

BOOL ScintillaChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIChildWnd::PreCreateWindow(cs))
		return FALSE;

  cs.style = WS_CHILD | WS_VISIBLE | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU |
    FWS_ADDTOTITLE | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_MAXIMIZE;

	return TRUE;
}

int 
ScintillaChildFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
  {
    return -1;
  }
  return 0;
}

BOOL 
ScintillaChildFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
  BOOL bRet = CMDIChildWnd::OnCreateClient(lpcs,pContext);

  // Workbook implementation
  ASSERT_KINDOF(MainFrame, GetMDIFrame());
  ((MainFrame*)GetMDIFrame())->OnCreateChild(this);

  return bRet;
}

void 
ScintillaChildFrame::OnDestroy ()
{
  CMDIChildWnd::OnDestroy();
  ((MainFrame*)GetMDIFrame())->OnDestroyChild(this);
}

// This handler remove Workbook Tab resizing bug on maximize/restore
//
void 
ScintillaChildFrame::OnSysCommand(UINT nID, LPARAM lParam)
{
  CMDIChildWnd::OnSysCommand(nID, lParam);
  UINT nItemID = (nID & 0xFFF0);
  // Bug fix, restore workbookbar on normal maximize instead of MDImaximize
  if ((nItemID == SC_MAXIMIZE) && !m_bHelpMode)
  {
    ((MainFrame*)GetMDIFrame())->OnMDIMaximize();
  }
}

void 
ScintillaChildFrame::OnMDIActivate (BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd)
{
  CMDIChildWnd::OnMDIActivate(bActivate, pActivateWnd, pDeactivateWnd);
  if (bActivate && this == pActivateWnd)
  {
    ((MainFrame*)GetMDIFrame())->OnActivateChild(this);
  }
}

LRESULT 
ScintillaChildFrame::OnSetText (WPARAM, LPARAM lParam)
{
  Default();
  ((MainFrame*)GetMDIFrame())->OnRenameChild(this, (LPCTSTR)lParam);
  return TRUE;
}


#ifdef _DEBUG
void ScintillaChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void ScintillaChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

void ScintillaChildFrame::OnMove(int x, int y) 
{
  //Let the base class do its thing
	CMDIChildWnd::OnMove(x, y);
	
	CScintillaScriptView* pView = (CScintillaScriptView*) GetActiveView();
  if (pView && pView->IsKindOf(RUNTIME_CLASS(CScintillaScriptView)))
  {
    CScintillaCtrl& rCtrl = pView->GetCtrl();

    //Cancel any outstanding call tip
	  if (rCtrl.CallTipActive())
      rCtrl.CallTipCancel();
  }
}

void ScintillaChildFrame::OnSize(UINT nType, int cx, int cy) 
{
  //Let the base class do its thing
	CMDIChildWnd::OnSize(nType, cx, cy);
	
	CScintillaScriptView* pView = static_cast<CScintillaScriptView*>(GetActiveView());
  if (pView && pView->IsKindOf(RUNTIME_CLASS(CScintillaScriptView)))
  {
    CScintillaCtrl& rCtrl = pView->GetCtrl();

    //Cancel any outstanding call tip
	  if (rCtrl.CallTipActive())
      rCtrl.CallTipCancel();
  }
}
