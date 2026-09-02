//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    IndexView.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: A IndexView is a view over the keywords of the 
// keyword file or the keywords in all the documents
// enabeling to edit all these keywords at once
//
#include "stdafx.h"
#include "AuthorHTML.h"
#include "IndexView.h"
#include "IndexFile.h"
#include "IndexEntry.h"
#include "IndexPropDlg.h"
#include "IndexChooseDlg.h"
#include "FindTopicDlg.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define ID_INDEX_EXPLORER   1005

/////////////////////////////////////////////////////////////////////////////
// IndexView

IndexView::IndexView()
          :m_imageList(NULL)
{
  // m_nToolBarID = IDR_KEYWORD_BAR;
}

IndexView::~IndexView()
{
  if(m_imageList)
  {
    delete m_imageList;
  }
  SaveTree();
}

BEGIN_MESSAGE_MAP(IndexView, CDockablePane)
  ON_WM_CREATE()
  ON_WM_SIZE()
  ON_COMMAND(ID_INDEX_NEW,                OnNewKeyword)
  ON_COMMAND(ID_INDEX_SUBKEY,             OnNewSubkey)
  ON_COMMAND(ID_INDEX_EDITPAGE,           OnEditPage)
  ON_COMMAND(ID_INDEX_DELETE,             OnDelete)
  ON_COMMAND(ID_INDEX_MOVELEFT,           OnMoveLeft)
  ON_COMMAND(ID_INDEX_MOVERIGHT,          OnMoveRight)
  ON_COMMAND(ID_INDEX_FIRSTUPPER,         OnKeyUpper)
  ON_COMMAND(ID_INDEX_LOWERCASE,          OnKeyLower)
  ON_COMMAND(ID_INDEX_PROPERTIES,         OnProperties)
  ON_NOTIFY(NM_DBLCLK,ID_INDEX_EXPLORER,  OnIndex_DblClick)
  ON_NOTIFY(NM_RCLICK,ID_INDEX_EXPLORER,  OnIndex_RClick)
  ON_UPDATE_COMMAND_UI(ID_INDEX_MOVELEFT, OnUpdateMoveLeft)
  ON_UPDATE_COMMAND_UI(ID_INDEX_MOVERIGHT,OnUpdateMoveRight)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// IndexView message handlers

int
IndexView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  CDockablePane::OnCreate(lpCreateStruct);

  if(!m_wndExplorer.Create(WS_VISIBLE | TVS_HASLINES /*| TVS_LINESATROOT /*| TVS_HASBUTTONS */| TVS_SHOWSELALWAYS,
                           CRect(0,0,0,0), this, ID_INDEX_EXPLORER))
  {
    TRACE0( "Unable to create tree control.\n" );
    return NULL;
  }
  InitializeTree();
  HTREEITEM root = m_wndExplorer.GetRootItem();
  m_wndExplorer.Expand(root,TVE_EXPAND);

  m_imageList = new CImageList();
  m_imageList->Create(MAKEINTRESOURCE(IDB_KEYWORDS),16,0,RGB(255,255,255));
  m_wndExplorer.SetImageList(m_imageList,TVSIL_NORMAL);

  // Create the toolbar
  m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_TOC_BAR);
  m_wndToolBar.LoadToolBar(IDR_KEYWORD_BAR, 0, 0, TRUE /* Is locked */);
  m_wndToolBar.LoadBitmap (IDB_KEYWORDS,    0, 0, TRUE /* Locked */);

  m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
  m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
  m_wndToolBar.SetOwner(this);

  // All commands will be routed via this control , not via the parent frame:
  m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

  AdjustLayout();
  return 0;
}

void
IndexView::InitializeTree()
{
}

void
IndexView::SaveTree()
{
}

void
IndexView::OnSize(UINT nType, int cx, int cy)
{
  CDockablePane::OnSize(nType, cx, cy);
  AdjustLayout();
}

void
IndexView::OnIndex_DblClick (NMHDR*, LRESULT* pResult)
{
  HTREEITEM hCurSel = m_wndExplorer.GetNextItem(TVI_ROOT, TVGN_CARET);

  if (hCurSel)
  {
    CPoint point;
    ::GetCursorPos(&point);
    m_wndExplorer.ScreenToClient(&point);
    if (m_wndExplorer.HitTest(point, 0) == hCurSel)
    {
      // OnProperties();
      OnEditPage();
      *pResult = 1;
      return;
    }
  }
  *pResult = 0;
}

void 
IndexView::OnIndex_RClick (NMHDR*, LRESULT* pResult)
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
    VERIFY(menu.LoadMenu(IDR_INDEX_POPUP));
    CMenu* pPopup = menu.GetSubMenu(0);

    ASSERT(pPopup != NULL);
    m_wndExplorer.ClientToScreen(&point);
    TrackPopupMenu(menu,TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y,0,GetSafeHwnd(),nullptr);

    *pResult = 1;
  }
  else
  {
    *pResult = 0;
  }
}

void 
IndexView::OnNewKeyword()
{
  IndexEntry* old   = nullptr;
  IndexEntry* entry = new IndexEntry();
  HTREEITEM last    = m_wndExplorer.GetNextItem(TVI_ROOT,TVGN_LASTVISIBLE);
  HTREEITEM parent  = nullptr;
  
  CString base = theApp.GetBaseDirectory();
  IndexPropDlg dlg(this,base,entry);
  if(dlg.DoModal() == IDOK)
  {
    // Create new entry
    IndexFile* index = theApp.GetIndex();
    index->AddEntry(old,entry);

    // Add to tree
    HTREEITEM newItem = m_wndExplorer.InsertItem(entry->GetTitle(),parent,last);
    m_wndExplorer.SetItemData(newItem,(DWORD_PTR)entry);
    m_wndExplorer.RedisplayEntry(newItem,entry);

    // Reselect the entry
    m_wndExplorer.ReSelectItem(parent,entry);
  }
  else
  {
    delete entry;
  }
}

void 
IndexView::OnNewSubkey()
{
  IndexEntry* old   = nullptr;
  IndexEntry* entry = new IndexEntry();
  HTREEITEM hCurSel = m_wndExplorer.GetNextItem(TVI_ROOT,TVGN_CARET);
  HTREEITEM parent  = NULL;
  IndexFile* index  = theApp.GetIndex();

  if(hCurSel)
  {
    parent = m_wndExplorer.GetParentItem(hCurSel);
    old = (IndexEntry*) m_wndExplorer.GetItemData(hCurSel);
  }
  else
  {
    parent  = nullptr;
    hCurSel = m_wndExplorer.GetNextItem(TVI_ROOT,TVGN_LASTVISIBLE);
  }
  index->AddChild(old,entry);

  // Add to tree
  HTREEITEM newItem = m_wndExplorer.InsertItem(entry->GetTitle(),hCurSel,TVI_SORT); 
  m_wndExplorer.SetItemData(newItem,(DWORD_PTR)entry);

  m_wndExplorer.RedisplayEntry(hCurSel,old);
  m_wndExplorer.RedisplayEntry(newItem,entry);

  // Reselect the entry
  m_wndExplorer.ReSelectItem(parent,entry);

  CString base = theApp.GetBaseDirectory();
  IndexPropDlg dlg(this,base,entry);
  if(dlg.DoModal() == IDOK)
  {
    m_wndExplorer.RedisplayEntry(newItem,entry);
    m_wndExplorer.ReSelectItem(parent,entry);
  }
  else
  {
    m_wndExplorer.DeleteItem(newItem);
    delete entry;
  }
}

void 
IndexView::OnEditPage()
{
  HTREEITEM hCurSel = m_wndExplorer.GetNextItem(TVI_ROOT, TVGN_CARET);
  if(hCurSel)
  {
    int ref = -1;
    CString path;
    CString tocString = m_wndExplorer.GetItemText(hCurSel);
    IndexEntry* entry   = (IndexEntry*) m_wndExplorer.GetItemData(hCurSel);
    if(entry->GetOneReference())
    {
      // Always gets the first reference
      ref = 0;
    }
    else
    {
      // Clone IndexPropDlg -> IndexChooseDlg
      IndexEntry* fentry = (IndexEntry*) m_wndExplorer.GetItemData(hCurSel);
      CString base = theApp.GetBaseDirectory();
      IndexChooseDlg dlg(this,base,fentry);
      if(dlg.DoModal() == IDOK)
      {
        ref = dlg.GetChosenReference();
      }
    }
    if(ref >= 0)
    {
      IndDocMap& docmap = entry->GetDocuments();
      IndexDocument* inddoc = docmap[ref];
      if(inddoc)
      {
        path = inddoc->m_document->GetFilename();
      }
    }
    if(!path.IsEmpty())
    {
      // Probably a topic document
      path = theApp.GetBaseDirectory() + path;
      theApp.OpenTypedDocumentFile(path);
    }
    else
    {
      theApp.MessageBox("This index entry doesn't have a page associated with it (yet)","Edit",MB_OK|MB_ICONASTERISK);
    }
  }
}

void
IndexView::OnDelete()
{
  HTREEITEM hCurSel = m_wndExplorer.GetNextItem(TVI_ROOT, TVGN_CARET);
  if(!hCurSel)
  {
    return;
  }
  IndexEntry* entry = (IndexEntry*) m_wndExplorer.GetItemData(hCurSel);
  bool hasChildren = entry->HasChildren();
  CString mess;
  if(hasChildren)
  {
    mess.Format("Are you sure you want to delete the index keyword '%s'\n"
                "and all it's underlying index keywords??",entry->GetTitle().GetString());
  }
  else
  {
    mess.Format("Are you sure you want to delete the index keyword '%s'?",entry->GetTitle().GetString());
  }
  if(theApp.MessageBox(mess,"Delete",MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON2) == IDYES)
  {
    IndexFile* index = theApp.GetIndex();
    index->DeleteEntry(entry);
    m_wndExplorer.DeleteItem(hCurSel);
  }
}

void 
IndexView::OnMoveLeft()
{
  HTREEITEM hCurSel = m_wndExplorer.GetNextItem(TVI_ROOT, TVGN_CARET);
  HTREEITEM parent  = m_wndExplorer.GetParentItem(hCurSel);
  if(parent != NULL)
  {
    IndexEntry* entry       = (IndexEntry*) m_wndExplorer.GetItemData(hCurSel);
    IndexEntry* parentEntry = (IndexEntry*) m_wndExplorer.GetItemData(parent);
    IndexFile* indexFile    = theApp.GetIndex();

    // Re-attach to index HHK
    indexFile->DetachEntry(entry);
    indexFile->AddEntry(parentEntry,entry);

    HTREEITEM top = m_wndExplorer.GetParentItem(parent);
    if(!top)
    {
      top = m_wndExplorer.GetRootItem();
      theApp.RedisplayIndex();
    }
    else
    {
      m_wndExplorer.DeleteChildrenItems(top);
      IndexEntry* theirParent = parentEntry->GetParent();
      indexFile->InsertIndex(&m_wndExplorer,top,theirParent,0);
    }
    // Reselect our entry
    m_wndExplorer.ReSelectItem(parent,entry);
  }
}

void
IndexView::OnMoveRight()
{
  HTREEITEM hCurSel = m_wndExplorer.GetNextItem(TVI_ROOT, TVGN_CARET);
  HTREEITEM prevSib = NULL;

  if (hCurSel)
  {
    prevSib = m_wndExplorer.GetPrevSiblingItem(hCurSel);
    if(prevSib)
    {
      IndexEntry* thisEntry = (IndexEntry*) m_wndExplorer.GetItemData(hCurSel);
      IndexEntry* prevEntry = (IndexEntry*) m_wndExplorer.GetItemData(prevSib);
      IndexFile*  index = theApp.GetIndex();

      index->DetachEntry(thisEntry);
      index->AddChild(prevEntry,thisEntry);
      IndexEntry* theirParent = prevEntry->GetParent();
      HTREEITEM parentItem = m_wndExplorer.GetParentItem(hCurSel);

      if(!parentItem)
      {
        parentItem = m_wndExplorer.GetRootItem();
        theApp.RedisplayIndex();
      }
      else
      {
        /// Delete childs of parent
        m_wndExplorer.DeleteChildrenItems(parentItem);
        // Redisplay from this point on
        index->InsertIndex(&m_wndExplorer,parentItem,theirParent,1);
      }
      m_wndExplorer.ReSelectItem(parentItem,thisEntry);
    }
  }
}

void 
IndexView::OnKeyUpper()
{
  HTREEITEM hCurSel = m_wndExplorer.GetNextItem(TVI_ROOT, TVGN_CARET);
  if(hCurSel)
  {
    IndexEntry* entry = (IndexEntry*) m_wndExplorer.GetItemData(hCurSel);
    CString title = entry->GetTitle();
    if(title.GetLength() > 0)
    {
      title.SetAt(0,toupper(title.GetAt(0)));
    }
    entry->SetTitle(title);
    m_wndExplorer.RedisplayEntry(hCurSel,entry);
  }
}

void 
IndexView::OnKeyLower()
{
  HTREEITEM hCurSel = m_wndExplorer.GetNextItem(TVI_ROOT, TVGN_CARET);
  if(hCurSel)
  {
    IndexEntry* entry = (IndexEntry*) m_wndExplorer.GetItemData(hCurSel);
    CString title = entry->GetTitle();
    if(title.GetLength() > 0)
    {
      title.SetAt(0,tolower(title.GetAt(0)));
    }
    entry->SetTitle(title);
    m_wndExplorer.RedisplayEntry(hCurSel,entry);
  }
}

void 
IndexView::OnProperties()
{
  HTREEITEM hCurSel = m_wndExplorer.GetNextItem(TVI_ROOT, TVGN_CARET);

  if (hCurSel)
  {
    IndexEntry* entry = (IndexEntry*) m_wndExplorer.GetItemData(hCurSel);
    CString base = theApp.GetBaseDirectory();
    IndexPropDlg dlg(this,base,entry);
    if(dlg.DoModal() == IDOK)
    {
      m_wndExplorer.RedisplayEntry(hCurSel,entry);
    }
  }
}

void
IndexView::OnUpdateMoveLeft(CCmdUI* pCmdUI)
{
  BOOL enable = TRUE;
  HTREEITEM hCurSel = m_wndExplorer.GetNextItem(TVI_ROOT, TVGN_CARET);
  HTREEITEM parent  = m_wndExplorer.GetParentItem(hCurSel);
  if(parent == NULL)
  {
    enable = FALSE;
  }
  pCmdUI->Enable(enable);
}

void
IndexView::OnUpdateMoveRight(CCmdUI* pCmdUI)
{
  HTREEITEM hCurSel = m_wndExplorer.GetNextItem(TVI_ROOT, TVGN_CARET);
  HTREEITEM prevSib = NULL;
  BOOL enable = FALSE;

  if (hCurSel)
  {
    prevSib = m_wndExplorer.GetPrevSiblingItem(hCurSel);
    if(prevSib)
    {
      enable = TRUE;
    }
  }
  pCmdUI->Enable(enable);
}

void
IndexView::AdjustLayout()
{
  if (GetSafeHwnd() == nullptr)
  {
    return;
  }

  CRect rectClient;
  GetClientRect(rectClient);

  int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

  m_wndToolBar.SetWindowPos(nullptr, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
  m_wndExplorer.SetWindowPos(nullptr, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}


