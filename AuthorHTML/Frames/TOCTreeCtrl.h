//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    TOCtreeCtrl.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Specialized tree control for the Table-Of-Contents (TOC)
//
#pragma once 
#include "TOCEntry.h"

class TOCTreeCtrl : public CTreeCtrl
{
public:
  TOCTreeCtrl();
  virtual ~TOCTreeCtrl();
  void RedisplayEntry(HTREEITEM item,TOCEntry* entry);
  int  OpenCloseBook(TOCEntry* entry,HTREEITEM item,UINT state);
  void DeleteChildrenItems(HTREEITEM item);
  void ReSelectItem(HTREEITEM from,TOCEntry* entry);

protected:
  afx_msg void OnItemExpanded (NMHDR* pNMHDR, LRESULT* pResult);

  DECLARE_MESSAGE_MAP()
};

