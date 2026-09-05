//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    ExplorerView.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Shows the directory of the current documentation project
//
#include "stdafx.h"
#include "AuthorHTML.h"
#include "ExplorerView.h"
#include "DirTreeCtrl.h"
#include "resource.h"
#include "ProjectFile.h"
#include "Misc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define ID_FPW_DRIVES       1001
#define ID_FPW_EXPLORER     1002
#define ID_FPW_FILTER       1003

static BOOL 
IsFolder (const CString& path)
{
  // 16.12.2002 bug fix, file manager context menu fails
  DWORD dwFileAttributes = GetFileAttributes(path);
  if (dwFileAttributes != INVALID_FILE_ATTRIBUTES)
  {
    return (dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ? TRUE : FALSE;
  }
  else
  {
    throw (CString("Not a folder: ") + path);
  }
}

/////////////////////////////////////////////////////////////////////////////
// CExplorerView

CExplorerView::CExplorerView()
{
	// m_nToolBarID = IDR_EXPLORER_BAR;
}

CExplorerView::~CExplorerView()
{
  SaveTree();
}


BEGIN_MESSAGE_MAP(CExplorerView, CDockablePane)
  ON_WM_CREATE()
  ON_WM_SIZE()
  ON_NOTIFY(NM_DBLCLK,  ID_FPW_EXPLORER,  OnExplorerTree_DblClick)
  ON_NOTIFY(NM_RCLICK,  ID_FPW_EXPLORER,  OnExplorerTree_RClick)
  ON_COMMAND(ID_EXPLORER_OPEN,            OnFpwOpen)
  ON_COMMAND(ID_EXPLORER_REFRESH,         OnFpwRefresh)
  ON_COMMAND(ID_EXPLORER_ADDTOPROJECT,    OnAddToProject)
  ON_COMMAND_RANGE(ID_EXPLORER_PARENT,ID_EXPLORER_SEARCH,OnExplorerCommand)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CExplorerView message handlers

int
CExplorerView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  CDockablePane::OnCreate(lpCreateStruct);

  if(!m_wndExplorer.Create(WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS,
		                       CRect(0,0,0,0), this, ID_FPW_EXPLORER))
	{
		TRACE0( "Unable to create tree control.\n" );
		return NULL;
	}
  InitializeTree();
  HTREEITEM root = m_wndExplorer.GetRootItem();
  m_wndExplorer.Expand(root,TVE_EXPAND);

  // Create the toolbar
  m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_TOC_BAR);
  m_wndToolBar.LoadToolBar(IDR_EXPLORER_BAR, 0, 0, TRUE /* Is locked */);
  m_wndToolBar.LoadBitmap(IDB_EXPLORER_STATELIST, 0, 0, TRUE /* Locked */);

  m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
  m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
  m_wndToolBar.SetOwner(this);

  // All commands will be routed via this control , not via the parent frame:
  m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

  AdjustLayout();
	return 0;
}

void
CExplorerView::InitializeTree()
{
  CString drivePath = settings.GetSetting("FileManager_CurrentDrive","");
  if (drivePath.IsEmpty())
  {
    m_wndExplorer.DisplayDrives();
  }
  m_wndExplorer.DisplayTree(drivePath,TRUE);
}

void
CExplorerView::SaveTree()
{
}

void CExplorerView::OnExplorerCommand(UINT p_command)
{
  switch (p_command)
  {
    case ID_EXPLORER_PARENT:      break;
    case ID_EXPLORER_NEW:         break;
    case ID_EXPLORER_LIBRARY:     OnAddToProject();
                                  return;
    case ID_EXPLORER_RENAME:      break;
    case ID_EXPLORER_DELETE:      break;
    case ID_EXPLORER_PROPERTIES:  break;
    case ID_EXPLORER_SEARCH:      break;
  }
  theApp.Panic("Not implemented yet!");
}

void
CExplorerView::OnSize(UINT nType, int cx, int cy)
{
  CDockablePane::OnSize(nType, cx, cy);
  AdjustLayout();
}

void 
CExplorerView::OnExplorerTree_DblClick (NMHDR*, LRESULT* pResult)
{
  HTREEITEM hCurSel = m_wndExplorer.GetNextItem(TVI_ROOT, TVGN_CARET);

  if (hCurSel)
  {
    CPoint point;
    ::GetCursorPos(&point);
    m_wndExplorer.ScreenToClient(&point);
    if (m_wndExplorer.HitTest(point, 0) == hCurSel)
    {
      CString path = m_wndExplorer.GetFullPath(hCurSel);
      if (!IsFolder(path) || (0xFF00 & GetKeyState(VK_CONTROL)))
      {
        PostMessage(WM_COMMAND, ID_EXPLORER_OPEN);
        *pResult = 1;
        return;
      }
    }
  }
  *pResult = 0;
}

// As a result of ID_EXPLORER OPEN in DblClick
void 
CExplorerView::OnFpwOpen ()
{
  HTREEITEM hCurSel = m_wndExplorer.GetNextItem(TVI_ROOT, TVGN_CARET);
  if (hCurSel)
  {
    CString path = m_wndExplorer.GetFullPath(hCurSel);
    if (!IsFolder(path))
    {
      theApp.OpenTypedDocumentFile(path);
    }
    else
    {
      theApp.OnCmdMsg(ID_FILE_OPEN, 0, 0, 0);
    }
  }
}

void
CExplorerView::OnAddToProject()
{
  HTREEITEM hCurSel = m_wndExplorer.GetNextItem(TVI_ROOT, TVGN_CARET);
  if(hCurSel)
  {
    CString path = m_wndExplorer.GetFullPath(hCurSel);
    ProjectFile* project = theApp.GetProjectFile();
    project->AddDocumentFile(path);
  }
}

void 
CExplorerView::OnExplorerTree_RClick (NMHDR*, LRESULT* pResult)
{
  UINT uFlags;
  CPoint point;
  ::GetCursorPos(&point);
  m_wndExplorer.ScreenToClient(&point);
  HTREEITEM hItem = m_wndExplorer.HitTest(point, &uFlags);

  if (hItem && TVHT_ONITEM & uFlags)
  {
    if (m_wndExplorer.GetNextItem(TVI_ROOT, TVGN_CARET) != hItem)
    {
      m_wndExplorer.SelectItem(hItem);
    }
    CMenu menu;
    VERIFY(menu.LoadMenu(IDR_EXPLORER_POPUP));
    CMenu* pPopup = menu.GetSubMenu(0);

    CString fullPath = m_wndExplorer.GetFullPath(hItem);
    BOOL isFolder = IsFolder(fullPath);
    if (!isFolder) 
    {
      pPopup->SetDefaultItem(ID_EXPLORER_OPEN);
    }
    // See if file is already in project
    ProjectFile* project = theApp.GetProjectFile();
    if(project->FindDocumentFile(Misc::FilenamePart(fullPath)) == NULL)
    {
      // Enable
      pPopup->EnableMenuItem(ID_EXPLORER_ADDTOPROJECT,MF_BYCOMMAND|MF_ENABLED);
    }
    pPopup->ModifyMenu(ID_EXPLORER_OPEN, MF_BYCOMMAND, ID_EXPLORER_OPEN, !isFolder ? "Open File\tDblClick" : "Open File Dalog...\tCtrl+DblClick");
    pPopup->EnableMenuItem(ID_EXPLORER_REFRESH, isFolder ? MF_BYCOMMAND|MF_ENABLED : MF_BYCOMMAND|MF_GRAYED);

    pPopup->RemoveMenu(3, MF_BYPOSITION);
    pPopup->RemoveMenu(3, MF_BYPOSITION);
    pPopup->RemoveMenu(3, MF_BYPOSITION);
    //pPopup->EnableMenuItem(ID_FPW_GOTO_DEF_DIR, isFolder ? MF_BYCOMMAND|MF_ENABLED : MF_BYCOMMAND|MF_GRAYED);
    //pPopup->EnableMenuItem(ID_FPW_SET_DEF_DIR, isFolder ? MF_BYCOMMAND|MF_ENABLED : MF_BYCOMMAND|MF_GRAYED);

    ASSERT(pPopup != NULL);
    m_wndExplorer.ClientToScreen(&point);
    TrackPopupMenu(menu,TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, 0,GetSafeHwnd(), nullptr);

    *pResult = 1;
  }
  else
  {
    *pResult = 0;
  }
}

// As a result of ID_EXPLORER_REFRESH
void 
CExplorerView::OnFpwRefresh ()
{
  HTREEITEM hCurSel = m_wndExplorer.GetNextItem(TVI_ROOT, TVGN_CARET);
  if (hCurSel)
  {
    CString path = m_wndExplorer.GetFullPath(hCurSel);
    if (IsFolder(path))
    {
      m_wndExplorer.RefreshFolder(hCurSel);
    }
    else
    {
      MessageBeep((UINT)-1);
    }
  }
}

BOOL
CExplorerView::SetSubtree(const CString& path)
{
  if (m_wndExplorer.DisplayTree(path,TRUE))
    return TRUE;
  return FALSE;
}

void
CExplorerView::AdjustLayout()
{
  if(GetSafeHwnd() == nullptr)
  {
    return;
  }

  CRect rectClient;
  GetClientRect(rectClient);

  int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

  m_wndToolBar .SetWindowPos(nullptr, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
  m_wndExplorer.SetWindowPos(nullptr, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}

