//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    IndexTreeCtrl.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Shows the keyword index of the documentation project
//
#pragma once 
#include "IndexEntry.h"

class IndexTreeCtrl : public CTreeCtrl
{
public:
  IndexTreeCtrl();
  virtual ~IndexTreeCtrl();
  void RedisplayEntry(HTREEITEM item,IndexEntry* entry);
  int  OpenCloseBook(IndexEntry* entry,HTREEITEM item,UINT state);
  void DeleteChildrenItems(HTREEITEM item);
  void ReSelectItem(HTREEITEM from,IndexEntry* entry);

protected:
  afx_msg void OnItemExpanded (NMHDR* pNMHDR, LRESULT* pResult);

  DECLARE_MESSAGE_MAP()
};

