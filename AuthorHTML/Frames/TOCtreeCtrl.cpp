//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    TOCtreeCtrl.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Specialized tree control for the Table-Of-Contents (TOC)
//
#include "stdafx.h"
#include "AuthorHTML.h"
#include "TOCTreeCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(TOCTreeCtrl, CTreeCtrl)
  ON_NOTIFY_REFLECT(TVN_ITEMEXPANDED, OnItemExpanded)
END_MESSAGE_MAP()

TOCTreeCtrl::TOCTreeCtrl()
{
}

TOCTreeCtrl::~TOCTreeCtrl()
{
}

void 
TOCTreeCtrl::OnItemExpanded (NMHDR* pNMHDR, LRESULT* pResult)
{
  NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
  HTREEITEM item = pNMTreeView->itemNew.hItem;
  TOCEntry* entry = (TOCEntry*) GetItemData(item);

  *pResult = OpenCloseBook(entry,item,pNMTreeView->itemNew.state & TVIS_EXPANDED);
};

int
TOCTreeCtrl::OpenCloseBook(TOCEntry* entry,HTREEITEM item,UINT state)
{
  int expand = (state & TVIS_EXPANDED) ? 1 : 0;
  int image = entry->GetImageNumber();
  if(image < 0) image = 0;
  if(image < 8)
  {
    // Do open/closing of books
    image += expand;
    SetItemImage(item,image,image);
    return 1; 
  }
  return 0;
}

void
TOCTreeCtrl::RedisplayEntry(HTREEITEM item,TOCEntry* entry)
{
  // Set the image
  int image = entry->GetImageNumber();
  if(image >= 0)
  {
    if(image < 8)
    {
      image &= 0xfff7; // must be even
    }
    SetItemImage(item,image,image);
  }
  else
  {
    if(entry->EntryType() == PF_Book)
    {
      // Standard closed purple-book
      SetItemImage(item,0,0);
    }
    else
    {
      // Standard topic page
      SetItemImage(item,8,8);
    }
  }
  // Set the title
  SetItemText(item,entry->GetTitle());
}

void
TOCTreeCtrl::DeleteChildrenItems(HTREEITEM item)
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
TOCTreeCtrl::ReSelectItem(HTREEITEM from,TOCEntry* entry)
{
  do
  {
    HTREEITEM child = GetNextItem(from,TVGN_CHILD);
    while(child) 
    {
      TOCEntry* here = (TOCEntry*) GetItemData(child);
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
