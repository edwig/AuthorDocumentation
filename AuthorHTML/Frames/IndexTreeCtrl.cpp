//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    IndexTreeCtrl.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Shows the keyword index of the documentation project
//
#include "stdafx.h"
#include "AuthorHTML.h"
#include "IndexTreeCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(IndexTreeCtrl, CTreeCtrl)
  ON_NOTIFY_REFLECT(TVN_ITEMEXPANDED, OnItemExpanded)
END_MESSAGE_MAP()

IndexTreeCtrl::IndexTreeCtrl()
{
}

IndexTreeCtrl::~IndexTreeCtrl()
{
}

void 
IndexTreeCtrl::OnItemExpanded (NMHDR* pNMHDR, LRESULT* pResult)
{
  NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
  HTREEITEM item = pNMTreeView->itemNew.hItem;
  IndexEntry* entry = (IndexEntry*) GetItemData(item);

  *pResult = OpenCloseBook(entry,item,pNMTreeView->itemNew.state & TVIS_EXPANDED);
};

int
IndexTreeCtrl::OpenCloseBook(IndexEntry* /*entry*/,HTREEITEM /*item*/,UINT /*state*/)
{
  // Index tree's are always expanded
  return 0;
}

void
IndexTreeCtrl::RedisplayEntry(HTREEITEM item,IndexEntry* entry)
{
  int state = 0; // Not bold
  // Index tree's are always expanded
  Expand(item,TVE_EXPAND);
  // Get number of documents
  IndDocMap& docmap = entry->GetDocuments();
  if(docmap.size() == 0)
  {
    state = TVIS_BOLD;
  }
  SetItemState(item,state,TVIS_BOLD);

  // Set type of keyword image
  if(entry->GetInHHK())
  {
    SetItemImage(item,0,0);
  }
  else
  {
    SetItemImage(item,2,2);
  }
  // Redisplay changed text
  SetItemText(item,entry->GetTitle());
}

void
IndexTreeCtrl::DeleteChildrenItems(HTREEITEM item)
{
  HTREEITEM child;
  do 
  {
    child = GetNextItem(item,TVGN_CHILD);
    if(child)
    {
      DeleteItem(child);
    }
  } 
  while(child);
}

void
IndexTreeCtrl::ReSelectItem(HTREEITEM from,IndexEntry* entry)
{
  do
  {
    HTREEITEM child = GetNextItem(from,TVGN_CHILD);
    while(child) 
    {
      IndexEntry* here = (IndexEntry*) GetItemData(child);
      if(here == entry)
      {
        CTreeCtrl::SelectItem(child);
        return;
      }
      ReSelectItem(child,entry);
      child = GetNextSiblingItem(child);
    } 
    from = GetNextSiblingItem(from);
  }
  while(from);
}
