//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    TOCView.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: A TOCView is a view over the table of contents
// it displays the TOC in a Pane, so the user can see and edit it
// and navigate the project bij choosing a topic
//
#include "stdafx.h"
#include "AuthorHTML.h"
#include "TOCView.h"
#include "TOCEntry.h"
#include "TOCDlg.h"
#include "TOC.h"
#include "FindTopicDlg.h"
#include "MainFrm.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define ID_TOC_EXPLORER   1004

/////////////////////////////////////////////////////////////////////////////
// TOCView

TOCView::TOCView()
        :m_imageList(NULL)
{
  // m_nToolBarID = IDR_TOC_BAR;
}

TOCView::~TOCView()
{
  if(m_imageList)
  {
    delete m_imageList;
  }
  SaveTree();
}

BEGIN_MESSAGE_MAP(TOCView, CDockablePane)
  ON_WM_CREATE()
  ON_WM_SIZE()
  ON_COMMAND(ID_TOC_NEWBOOK,      OnNewBook)
  ON_COMMAND(ID_TOC_NEWPAGE,      OnNewPage)
  ON_COMMAND(ID_TOC_BOOKFROMPAGE, OnBookFromPage)
  ON_COMMAND(ID_TOC_PAGEFROMBOOK, OnPageFromBook)
  ON_COMMAND(ID_TOC_EDITPAGE,     OnEditPage)
  ON_COMMAND(ID_TOC_MOVEUP,       OnMoveUp)
  ON_COMMAND(ID_TOC_MOVEDOWN,     OnMoveDown)
  ON_COMMAND(ID_TOC_MOVELEFT,     OnMoveLeft)
  ON_COMMAND(ID_TOC_MOVERIGHT,    OnMoveRight)
  ON_COMMAND(ID_TOC_RENAME,       OnRename)
  ON_COMMAND(ID_TOC_DELETE,       OnDelete)
  ON_COMMAND(ID_TOC_FINDTOPIC,    OnFindTopic)
  ON_COMMAND(ID_TOC_PROPERTIES,   OnProperties)
  ON_NOTIFY(NM_DBLCLK,ID_TOC_EXPLORER,  OnTOC_DblClick)
  ON_NOTIFY(NM_RCLICK,ID_TOC_EXPLORER,  OnTOC_RClick)
  ON_UPDATE_COMMAND_UI(ID_TOC_MOVEUP,   OnUpdateMoveUp)
  ON_UPDATE_COMMAND_UI(ID_TOC_MOVEDOWN, OnUpdateMoveDown)
  ON_UPDATE_COMMAND_UI(ID_TOC_MOVELEFT, OnUpdateMoveLeft)
  ON_UPDATE_COMMAND_UI(ID_TOC_MOVERIGHT,OnUpdateMoveRight)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// TOCView message handlers

int 
TOCView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  CDockablePane::OnCreate(lpCreateStruct);

  if(!m_wndExplorer.Create(WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS,
                           CRect(0,0,0,0), this, ID_TOC_EXPLORER))
  {
    TRACE0( "Unable to create tree control.\n" );
    return NULL;
  }
  InitializeTree();
  HTREEITEM root = m_wndExplorer.GetRootItem();
  m_wndExplorer.Expand(root,TVE_EXPAND);

  m_imageList = new CImageList();
  m_imageList->Create(MAKEINTRESOURCE(IDB_TOC),16,0,RGB(255,255,255));
  m_wndExplorer.SetImageList(m_imageList,TVSIL_NORMAL);

  // Create the toolbar
  m_wndToolBar.Create(this,AFX_DEFAULT_TOOLBAR_STYLE,IDR_TOC_BAR);
  m_wndToolBar.LoadToolBar(IDR_TOC_BAR,0,0,TRUE /* Is locked */);
  m_wndToolBar.LoadBitmap(IDB_TOC,0,0,TRUE /* Locked */);

  m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
  m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
  m_wndToolBar.SetOwner(this);

  // All commands will be routed via this control , not via the parent frame:
  m_wndToolBar.SetRouteCommandsViaFrame(FALSE);
  
  AdjustLayout();
  return 0;
}

void
TOCView::InitializeTree()
{
}

void
TOCView::SaveTree()
{
}

void 
TOCView::OnTOC_DblClick (NMHDR*, LRESULT* pResult)
{
  HTREEITEM hCurSel = m_wndExplorer.GetNextItem(TVI_ROOT, TVGN_CARET);

  if (hCurSel)
  {
    CPoint point;
    ::GetCursorPos(&point);
    m_wndExplorer.ScreenToClient(&point);
    if (m_wndExplorer.HitTest(point, 0) == hCurSel)
    {
      CString tocString = m_wndExplorer.GetItemText(hCurSel);
      TOCEntry* entry   = (TOCEntry*) m_wndExplorer.GetItemData(hCurSel);
      CString path = entry->GetDocumentFilename();
      if(!path.IsEmpty())
      {
        // Probably a topic document
        path = theApp.GetBaseDirectory() + path;
        theApp.OpenTypedDocumentFile(path);
      }
      else if(m_wndExplorer.ItemHasChildren(hCurSel))
      {
        m_wndExplorer.Expand(hCurSel,TVE_TOGGLE);
        int state = m_wndExplorer.GetItemState(hCurSel,TVIS_EXPANDED);
        m_wndExplorer.OpenCloseBook(entry,hCurSel,state);
      }
      *pResult = 1;
      return;
    }
  }
  *pResult = 0;
}

void 
TOCView::OnTOC_RClick (NMHDR*, LRESULT* pResult)
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
    VERIFY(menu.LoadMenu(IDR_TOC_POPUP));
    CMenu* pPopup = menu.GetSubMenu(0);

    ASSERT(pPopup != NULL);
    m_wndExplorer.ClientToScreen(&point);
    //CXTPCommandBars::TrackPopupMenu(pPopup,TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
    TrackPopupMenu(menu,TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y,0,GetSafeHwnd(),nullptr);

    *pResult = 1;
  }
  else
  {
    *pResult = 0;
  }
}

void 
TOCView::OnNewBook()
{
  TOCEntry* old = NULL;
  TOCEntry* entry = new TOCEntry();
  HTREEITEM hCurSel = m_wndExplorer.GetNextItem(TVI_ROOT, TVGN_CARET);
  HTREEITEM parent  = NULL;

  if (hCurSel)
  {
    parent = m_wndExplorer.GetParentItem(hCurSel);
    old = (TOCEntry*) m_wndExplorer.GetItemData(hCurSel);
  }
  else
  {
    parent = hCurSel = m_wndExplorer.GetRootItem();
  }
  entry->SetTitle("New book");
  entry->MakeBook();
  // Add the entry
  TOC* toc = theApp.GetTOC();
  if(toc)
  {
    toc->AddEntry(old,entry);
    // Add to tree
    HTREEITEM newItem = m_wndExplorer.InsertItem(entry->GetTitle(),parent,hCurSel);
    m_wndExplorer.SetItemData(newItem,(DWORD_PTR)entry);
    m_wndExplorer.RedisplayEntry(newItem,entry);

    // Reselect the new entry
    m_wndExplorer.ReSelectItem(parent,entry);
  }
}

void 
TOCView::OnNewPage()
{
  TOCEntry* old = NULL;
  TOCEntry* entry = new TOCEntry();
  HTREEITEM hCurSel = m_wndExplorer.GetNextItem(TVI_ROOT, TVGN_CARET);
  HTREEITEM parent  = NULL;

  if (hCurSel)
  {
    parent = m_wndExplorer.GetParentItem(hCurSel);
    old = (TOCEntry*) m_wndExplorer.GetItemData(hCurSel);
  }
  else
  {
    parent = hCurSel = m_wndExplorer.GetRootItem();
  }
  CString base = theApp.GetBaseDirectory();
  TOCDlg dlg(this,base,entry);
  if(dlg.DoModal() == IDOK)
  {
    TOC* toc = theApp.GetTOC();
    toc->AddEntry(old,entry);

    HTREEITEM newItem = m_wndExplorer.InsertItem(entry->GetTitle(),parent,hCurSel);
    m_wndExplorer.SetItemData(newItem,(DWORD_PTR)entry);
    m_wndExplorer.RedisplayEntry(newItem,entry);

    // Reselect the new entry
    m_wndExplorer.ReSelectItem(parent,entry);

    // Probably a topic document
    CString path = entry->GetDocumentFilename();
    if(!path.IsEmpty())
    {
      CString fileref = base + path;
      theApp.OpenTypedDocumentFile(fileref);
    }
    // refocus the TOC
    MainFrame* main = (MainFrame*) theApp.GetMainWnd();
    if(main)
    {
      main->ShowPane(ID_PANE_TOC);
    }
  }
  else
  {
    delete entry;
  }
}

void 
TOCView::OnBookFromPage()
{
  HTREEITEM hCurSel = m_wndExplorer.GetNextItem(TVI_ROOT, TVGN_CARET);
  TOCEntry* entry   = (TOCEntry*) m_wndExplorer.GetItemData(hCurSel);
  if(entry->HasChildren())
  {
    theApp.MessageBox("This is already a book. Conversion not possible","TOC",MB_OK|MB_ICONINFORMATION);
    return;
  }
  entry->MakeBook();
  m_wndExplorer.RedisplayEntry(hCurSel,entry);
}

void 
TOCView::OnPageFromBook()
{
  HTREEITEM hCurSel = m_wndExplorer.GetNextItem(TVI_ROOT, TVGN_CARET);
  TOCEntry* entry   = (TOCEntry*) m_wndExplorer.GetItemData(hCurSel);
  if(entry->HasChildren())
  {
    theApp.MessageBox("This book still has pages. Conversion not possible","TOC",MB_OK|MB_ICONINFORMATION);
    return;
  }
  entry->MakePage();
  m_wndExplorer.RedisplayEntry(hCurSel,entry);
}

void 
TOCView::OnEditPage()
{
  HTREEITEM hCurSel = m_wndExplorer.GetNextItem(TVI_ROOT, TVGN_CARET);
  TOCEntry* entry   = (TOCEntry*) m_wndExplorer.GetItemData(hCurSel);
  CString path = entry->GetDocumentFilename();
  if(!path.IsEmpty())
  {
    path = theApp.GetBaseDirectory() + path;
    theApp.OpenTypedDocumentFile(path);
  }
  else
  {
    theApp.MessageBox("This entry doesn't have a page associated with it (yet)","Edit",MB_OK|MB_ICONASTERISK);
  }
}

void 
TOCView::OnMoveUp()
{
  HTREEITEM hCurSel = m_wndExplorer.GetNextItem(TVI_ROOT, TVGN_CARET);
  if (hCurSel)
  {
    HTREEITEM prevSib = m_wndExplorer.GetPrevSiblingItem(hCurSel);
    if(prevSib)
    {
      TOCEntry* thisEntry = (TOCEntry*) m_wndExplorer.GetItemData(hCurSel);
      TOCEntry* prevEntry = (TOCEntry*) m_wndExplorer.GetItemData(prevSib);
      TOC* toc = theApp.GetTOC();
      toc->DetachEntry(prevEntry);
      toc->AddEntry(thisEntry,prevEntry);
      TOCEntry* theirParent = thisEntry->GetParent();
      HTREEITEM parent = m_wndExplorer.GetParentItem(hCurSel);
      // Delete childs of parent
      m_wndExplorer.DeleteChildrenItems(parent);
      // Redisplay from this point on
      toc->InsertTOC(&m_wndExplorer,parent,theirParent,0);

      // Reselect our entry
      m_wndExplorer.ReSelectItem(parent,thisEntry);
    }
  }
}

void
TOCView::OnMoveDown()
{
  HTREEITEM hCurSel = m_wndExplorer.GetNextItem(TVI_ROOT, TVGN_CARET);
  if (hCurSel)
  {
    HTREEITEM nextSib = m_wndExplorer.GetNextSiblingItem(hCurSel);
    if(nextSib)
    {
      TOCEntry* thisEntry = (TOCEntry*) m_wndExplorer.GetItemData(hCurSel);
      TOCEntry* nextEntry = (TOCEntry*) m_wndExplorer.GetItemData(nextSib);
      TOC* toc = theApp.GetTOC();
      toc->DetachEntry(thisEntry);
      toc->AddEntry(nextEntry,thisEntry);
      TOCEntry* theirParent = thisEntry->GetParent();
      HTREEITEM parent = m_wndExplorer.GetParentItem(hCurSel);
      // Delete childs of parent
      m_wndExplorer.DeleteChildrenItems(parent);
      // Redisplay from this point on
      toc->InsertTOC(&m_wndExplorer,parent,theirParent,0);

      // Reselect our entry
      m_wndExplorer.ReSelectItem(parent,thisEntry);
    }
  }
}

void 
TOCView::OnMoveLeft()
{
  HTREEITEM root    = m_wndExplorer.GetRootItem();
  HTREEITEM hCurSel = m_wndExplorer.GetNextItem(TVI_ROOT, TVGN_CARET);
  HTREEITEM parent  = m_wndExplorer.GetParentItem(hCurSel);
  if(parent && (root != parent))
  {
    TOCEntry* parentEntry = (TOCEntry*) m_wndExplorer.GetItemData(parent);
    TOCEntry* thisEntry   = (TOCEntry*) m_wndExplorer.GetItemData(hCurSel);
    TOCEntry* parentParent = parentEntry->GetParent();

    // Relocate in TOC
    TOC* toc = theApp.GetTOC();
    toc->DetachEntry(thisEntry);
    toc->AddEntry(parentEntry,thisEntry);

    // Relocate in CTree
    HTREEITEM pp = m_wndExplorer.GetParentItem(parent);
    if(!pp)
    {
      pp = root;
      theApp.RedisplayTOC();
    }
    else
    {
      m_wndExplorer.DeleteChildrenItems(pp);
      toc->InsertTOC(&m_wndExplorer,pp,parentParent,0);
    }
    m_wndExplorer.ReSelectItem(pp,thisEntry);
  }
}

void 
TOCView::OnMoveRight()
{
  HTREEITEM hCurSel = m_wndExplorer.GetNextItem(TVI_ROOT, TVGN_CARET);
  HTREEITEM prevSib = NULL;
  if (hCurSel)
  {
    prevSib = m_wndExplorer.GetPrevSiblingItem(hCurSel);
    if(prevSib)
    {
      TOCEntry* prevEntry = (TOCEntry*) m_wndExplorer.GetItemData(prevSib);
      TOCEntry* thisEntry = (TOCEntry*) m_wndExplorer.GetItemData(hCurSel);

      // Relocate in TOC
      TOC* toc = theApp.GetTOC();
      toc->DetachEntry(thisEntry);
      toc->AddChild(prevEntry,thisEntry);

      // Relocate in CTree
      HTREEITEM parentItem  = m_wndExplorer.GetParentItem(hCurSel);
      TOCEntry* parentEntry = prevEntry->GetParent();
      if(!parentItem)
      {
        parentItem = m_wndExplorer.GetRootItem();
        theApp.RedisplayTOC();
      }
      else
      {
        m_wndExplorer.DeleteChildrenItems(parentItem);
        toc->InsertTOC(&m_wndExplorer,parentItem,parentEntry,0);
      }
      m_wndExplorer.ReSelectItem(parentItem,thisEntry);
    }
  }
}

void 
TOCView::OnRename()
{
  OnProperties();
}

void 
TOCView::OnDelete()
{
  HTREEITEM hCurSel = m_wndExplorer.GetNextItem(TVI_ROOT, TVGN_CARET);
  if(!hCurSel)
  {
    return;
  }
  TOCEntry* entry = (TOCEntry*) m_wndExplorer.GetItemData(hCurSel);
  bool hasChildren = entry->HasChildren();
  CString mess;
  if(hasChildren || (entry->EntryType() == PF_Book))
  {
    mess.Format("Are you sure you want to delete the book '%s'\n"
                "and all it's underlying books and pages?",entry->GetTitle().GetString());
  }
  else
  {
    mess.Format("Are you sure you want to delete the page '%s'?",entry->GetTitle().GetString());
  }
  if(theApp.MessageBox(mess,"Delete",MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON2) == IDYES)
  {
    TOC* toc = theApp.GetTOC();
    toc->DeleteEntry(entry);
    m_wndExplorer.DeleteItem(hCurSel);
  }
}

void 
TOCView::OnFindTopic()
{
  CString base = theApp.GetBaseDirectory();
  FindTopicDlg dlg(this,base);
  if(dlg.DoModal() == IDOK)
  {
    CString title = dlg.GetFoundTitle();
    CString topic = dlg.GetFoundTopic();

    // Put new page in the TOCEntry
    TOCEntry* entry = new TOCEntry();
    entry->SetTitle(title);
    entry->SetDocumentFilename(topic);

    // Put new entry in the tree
    HTREEITEM hCurSel = m_wndExplorer.GetNextItem(TVI_ROOT, TVGN_CARET);
    HTREEITEM parent = m_wndExplorer.GetParentItem(hCurSel);
    if(!parent)
    {
      parent = m_wndExplorer.GetRootItem();
    }
    // Put new entry in the TOC
    TOCEntry* old = (TOCEntry*) m_wndExplorer.GetItemData(hCurSel);
    TOC* toc = theApp.GetTOC();
    toc->AddEntry(old,entry);

    // Display in the tree
    HTREEITEM newItem = m_wndExplorer.InsertItem(title,parent,hCurSel);
    m_wndExplorer.SetItemData(newItem,(DWORD_PTR)entry);
    m_wndExplorer.RedisplayEntry(newItem,entry);
  }
}

void 
TOCView::OnProperties()
{
  HTREEITEM hCurSel = m_wndExplorer.GetNextItem(TVI_ROOT, TVGN_CARET);

  if (hCurSel)
  {
    TOCEntry* entry = (TOCEntry*) m_wndExplorer.GetItemData(hCurSel);
    CString base = theApp.GetBaseDirectory();
    TOCDlg dlg(this,base,entry);
    if(dlg.DoModal() == IDOK)
    {
      m_wndExplorer.RedisplayEntry(hCurSel,entry);
    }
  }
}

void
TOCView::OnUpdateMoveUp(CCmdUI* pCmdUI)
{
  BOOL enable = FALSE;
  HTREEITEM hCurSel = m_wndExplorer.GetNextItem(TVI_ROOT, TVGN_CARET);
  if (hCurSel)
  {
    HTREEITEM prevSib = m_wndExplorer.GetPrevSiblingItem(hCurSel);
    if(prevSib)
    {
      enable = TRUE;
    }
  }
  pCmdUI->Enable(enable);
}

void
TOCView::OnUpdateMoveDown(CCmdUI* pCmdUI)
{
  BOOL enable = FALSE;
  HTREEITEM hCurSel = m_wndExplorer.GetNextItem(TVI_ROOT, TVGN_CARET);
  if (hCurSel)
  {
    HTREEITEM nextSib = m_wndExplorer.GetNextSiblingItem(hCurSel);
    if(nextSib)
    {
      enable = TRUE;
    }
  }
  pCmdUI->Enable(enable);
}

void
TOCView::OnUpdateMoveLeft(CCmdUI* pCmdUI)
{
  BOOL enable = TRUE;
  HTREEITEM root    = m_wndExplorer.GetRootItem();
  HTREEITEM hCurSel = m_wndExplorer.GetNextItem(TVI_ROOT, TVGN_CARET);
  HTREEITEM parent  = m_wndExplorer.GetParentItem(hCurSel);
  if(root == parent || parent == NULL)
  {
    enable = FALSE;
  }
  pCmdUI->Enable(enable);
}

void
TOCView::OnUpdateMoveRight(CCmdUI* pCmdUI)
{
  HTREEITEM hCurSel = m_wndExplorer.GetNextItem(TVI_ROOT, TVGN_CARET);
  HTREEITEM prevSib = NULL;
  BOOL enable = FALSE;

  if (hCurSel)
  {
    prevSib = m_wndExplorer.GetPrevSiblingItem(hCurSel);
    if(prevSib)
    {
      TOCEntry* entry = (TOCEntry*) m_wndExplorer.GetItemData(prevSib);
      if(entry->EntryType() == PF_Book)
      {
        enable = TRUE;
      }
    }
  }
  pCmdUI->Enable(enable);
}

void
TOCView::OnSize(UINT nType,int cx,int cy)
{
  CDockablePane::OnSize(nType,cx,cy);
  AdjustLayout();
}

void
TOCView::AdjustLayout()
{
  if(GetSafeHwnd() == nullptr)
  {
    return;
  }

  CRect rectClient;
  GetClientRect(rectClient);

  int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE,TRUE).cy;

  m_wndToolBar.SetWindowPos(nullptr,rectClient.left,rectClient.top,rectClient.Width(),cyTlb,SWP_NOACTIVATE | SWP_NOZORDER);
  m_wndExplorer.SetWindowPos(nullptr,rectClient.left + 1,rectClient.top + cyTlb + 1,rectClient.Width() - 2,rectClient.Height() - cyTlb - 2,SWP_NOACTIVATE | SWP_NOZORDER);
}

