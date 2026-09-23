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

#define TOC_MIN_FONTSIZE       8
#define TOC_MAX_FONTSIZE      36
#define TOC_DEFAULT_FONTSIZE   9

class TOCTreeCtrl : public CTreeCtrl
{
public:
  TOCTreeCtrl();
  virtual ~TOCTreeCtrl();
  void RedisplayEntry(HTREEITEM item,TOCEntry* entry);
  int  OpenCloseBook(TOCEntry* entry,HTREEITEM item,UINT state);
  void DeleteChildrenItems(HTREEITEM item);
  void ReSelectItem(HTREEITEM from,TOCEntry* entry);
  void SetFontSize(int p_size,CString p_fontName = _T("Verdana"));

protected:
  afx_msg void OnItemExpanded (NMHDR* pNMHDR, LRESULT* pResult);

  DECLARE_MESSAGE_MAP()

private:
  CFont m_font;
};

