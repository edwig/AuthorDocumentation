//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    PageLink2.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Shows the links on THIS page to other pages 
//              AND shows the links from OTHER pages to this page.
//
#include "stdafx.h"
#include <Shlwapi.h>
#include <string>
#include "OpenFiles.h"
#include "MainFrm.h"
#include "AuthorHTML.h"
#include "HTMLEdView.h"
#include "ProjectFile.h"

#define ID_LINK_FROM_FILES   1001
#define ID_LINK_TOME_FILES   1002

#define ACTIVATE_FILE_TIMER_FROM 800
#define ACTIVATE_FILE_TIMER_TOME 801
#define ACTIVATE_FILE_TIMER_SET  802

#define ACTIVATE_FILE_RETRY        4 // 4 times
#define ACTIVATE_FILE_DELAY      250 // Miliseconds

PageLink2::PageLink2()
          :m_htmlView(NULL)
{
  m_imgList = NULL;
  m_retry   = -1;
}

PageLink2::~PageLink2()
{
  RemoveLinks();
}

void 
PageLink2::ActivateOpenFile(CListCtrl* list)
{
  POSITION pos = list->GetFirstSelectedItemPosition();

  if (pos)
  {
    LVITEM item;
    memset(&item, 0, sizeof(item));
    item.mask = LVIF_PARAM;
    item.iItem = list->GetNextSelectedItem(pos);

    VERIFY(list->GetItem(&item));
    DocumentFile* doc = (DocumentFile*)item.lParam;
    if(doc)
    {
      CString path = doc->GetFilename();
      CString base = theApp.GetBaseDirectory();
      CString docfile = base + path;
      theApp.OpenTypedDocumentFile(docfile);
    }
  }
}

BEGIN_MESSAGE_MAP(PageLink2, CDockablePane)
  ON_WM_SIZE()
  ON_WM_TIMER()
  ON_WM_CREATE()
  ON_NOTIFY(NM_DBLCLK,  ID_LINK_FROM_FILES,OnOpenFiles_Click_From)
  ON_NOTIFY(NM_DBLCLK,  ID_LINK_TOME_FILES,OnOpenFiles_Click_ToMe)
  ON_NOTIFY(NM_RCLICK,  ID_LINK_FROM_FILES,OnOpenFiles_RClick_From)
  ON_NOTIFY(NM_RCLICK,  ID_LINK_TOME_FILES,OnOpenFiles_RClick_ToMe)
  ON_NOTIFY(LVN_KEYDOWN,ID_LINK_FROM_FILES,OnOpenFiles_KeyDown_From)
  ON_NOTIFY(LVN_KEYDOWN,ID_LINK_TOME_FILES,OnOpenFiles_KeyDown_ToMe)
END_MESSAGE_MAP()

int
PageLink2::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  CDockablePane::OnCreate(lpCreateStruct);

  CRect rect;
  if (!m_listFrom.Create(WS_CHILD|WS_TABSTOP|WS_VISIBLE|
                         LVS_SORTASCENDING|LVS_SHOWSELALWAYS|LVS_REPORT|
                         LVS_SINGLESEL // | LVS_NOCOLUMNHEADER
                        ,rect
                        ,this
                        ,ID_LINK_FROM_FILES))
  {
    return NULL;
  }
  if (!m_listToMe.Create(WS_CHILD|WS_TABSTOP|WS_VISIBLE|
                         LVS_SORTASCENDING|LVS_SHOWSELALWAYS|LVS_REPORT|
                         LVS_SINGLESEL // | LVS_NOCOLUMNHEADER
                        ,rect
                        ,this
                        ,ID_LINK_TOME_FILES))
  {
    return NULL;
  }
  static char* text1 = "Links to other pages";
  static char* text2 = "Links to this page";

  m_listFrom.InsertColumn(0, text1);
  m_listToMe.InsertColumn(0, text2);
  m_listFrom.SetColumnWidth(0,300);
  m_listToMe.SetColumnWidth(0,300);
  m_listFrom.SetExtendedStyle(m_listFrom.GetExtendedStyle()|LVS_EX_FULLROWSELECT);
  m_listToMe.SetExtendedStyle(m_listToMe.GetExtendedStyle()|LVS_EX_FULLROWSELECT);

  SHFILEINFO shFinfo;
  HIMAGELIST hImgList = (HIMAGELIST)SHGetFileInfo("", 0, &shFinfo, sizeof(shFinfo), SHGFI_SMALLICON|SHGFI_SYSICONINDEX);
  // Record the image list as 'permanent'
  ::SendMessage(m_listFrom.m_hWnd,LVM_SETIMAGELIST, (LPARAM)hImgList, (UINT)LVSIL_SMALL);
  ::SendMessage(m_listToMe.m_hWnd,LVM_SETIMAGELIST, (LPARAM)hImgList, (UINT)LVSIL_SMALL);
  if (m_imgList && m_imgList->m_hImageList) 
  {
    m_imgList->Detach();
  }
  m_imgList = CImageList::FromHandle(hImgList);
  m_listFrom.SetImageList(m_imgList, LVSIL_SMALL);
  m_listToMe.SetImageList(m_imgList, LVSIL_SMALL);
  m_listFrom.ShowWindow(SW_SHOW);
  m_listFrom.ShowWindow(SW_SHOW);

  AdjustLayout();
  return 0;
}

void 
PageLink2::OnSize(UINT nType, int cx, int cy)
{
  CDockablePane::OnSize(nType, cx, cy);

  if (cx > 0 && cy > 0 && nType != SIZE_MAXHIDE && nType != SIZE_MINIMIZED) 
  {
    CRect rect;
    GetClientRect(&rect);
    rect.InflateRect(3, 0);

    int halfUp = (rect.bottom / 2) - 1;
    int halfDn = (rect.bottom / 2) + 1;

    HDWP hdwp = ::BeginDeferWindowPos(2);
    ::DeferWindowPos(hdwp, m_listFrom, 0, rect.left, rect.top,rect.Width(), (halfUp - rect.top),    SWP_NOZORDER);
    ::DeferWindowPos(hdwp, m_listToMe, 0, rect.left, halfDn,  rect.Width(), (rect.bottom - halfDn), SWP_NOZORDER);
    ::EndDeferWindowPos(hdwp);

    if(m_listFrom.GetSafeHwnd())
    {
      m_listFrom.GetClientRect(rect);
      m_listFrom.SetColumnWidth(0,rect.Width());
      m_listToMe.GetClientRect(rect);
      m_listToMe.SetColumnWidth(0,rect.Width());
    }
  }
}

void
PageLink2::LinksStarted()
{
  MainFrame *pFrm = (MainFrame*)AfxGetMainWnd();
  pFrm->ShowPane(ID_PANE_LINKS);
}

void
PageLink2::ActivatePage(CHTMLEdView* p_view)
{
  if(theApp.GetProjectFile() == NULL)
  {
    return;
  }
  // Be sure the pane is activated
  LinksStarted();
  // Remove previous links
  RemoveLinks();

  m_retry = ACTIVATE_FILE_RETRY;
  SetTimer(ACTIVATE_FILE_TIMER_SET, ACTIVATE_FILE_DELAY, 0);
  m_htmlView = p_view;
  SetPageLinks();
}

void
PageLink2::SetPageLinks()
{
  if(m_htmlView)
  {
    if(m_htmlView->IsComplete())
    {
      CString path = m_htmlView->RegisteredURL();
      ProjectFile* pro = theApp.GetProjectFile();
      if(pro)
      {
        CString docfile;
        CString base = pro->GetBaseDirectory();
        Misc::MakeRelativePathname(base,path,docfile);
        DocumentFile* doc = pro->FindDocumentFile(docfile);
        if(doc)
        {
          doc->DisplayPageLinks(m_listFrom,m_listToMe);
        }
      }
      m_htmlView = NULL;
    }
    else
    {
      // Retry ACTIVATE_FILE_RETRY times ACTIVATE_FILE_DELAY times to re-read
      // Files with scripts can be slow!!
      while(m_retry-- >= 0)
      {
        SetTimer(ACTIVATE_FILE_TIMER_SET, ACTIVATE_FILE_DELAY, 0);
      }
    }
  }
}

void
PageLink2::RemoveLinks()
{
  if(m_listFrom)
  {
    m_listFrom.DeleteAllItems();
  }
  if(m_listToMe)
  {
    m_listToMe.DeleteAllItems();
  }
}

void 
PageLink2::OnOpenFiles_Click_From(NMHDR*, LRESULT* pResult)
{
  ActivateOpenFile(&m_listFrom);
  *pResult = 0;
}

void 
PageLink2::OnOpenFiles_Click_ToMe(NMHDR*, LRESULT* pResult)
{
  ActivateOpenFile(&m_listToMe);
  *pResult = 0;
}

void 
PageLink2::OnOpenFiles_KeyDown_From(NMHDR* pnkd, LRESULT* pResult)
{
  NMLVKEYDOWN* nmlvk = (NMLVKEYDOWN*)pnkd;
  if(nmlvk->wVKey == VK_RETURN)
  {
    SetTimer(ACTIVATE_FILE_TIMER_FROM, ACTIVATE_FILE_DELAY, 0);
  }
  *pResult = 0;
}

void 
PageLink2::OnOpenFiles_KeyDown_ToMe(NMHDR* pnkd, LRESULT* pResult)
{
  NMLVKEYDOWN* nmlvk = (NMLVKEYDOWN*)pnkd;
  if(nmlvk->wVKey == VK_RETURN)
  {
    SetTimer(ACTIVATE_FILE_TIMER_TOME, ACTIVATE_FILE_DELAY, 0);
  }
  *pResult = 0;
}

void 
PageLink2::OnOpenFiles_RClick_From (NMHDR* pNMHDR, LRESULT* pResult)
{
  NMITEMACTIVATE* pItem = (NMITEMACTIVATE*)pNMHDR;
  if (pItem && pItem->iItem != -1)
  {
    CPoint point(pItem->ptAction);
    m_listFrom.ClientToScreen(&point);

    CMenu menu;
    VERIFY(menu.LoadMenu(IDR_WORKBOOK_POPUP));
    CMenu* pPopup = menu.GetSubMenu(0);

    ASSERT(pPopup != NULL);
    ASSERT_KINDOF(CFrameWnd, AfxGetMainWnd());
    TrackPopupMenu(menu,TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, 0, GetSafeHwnd(),nullptr);

    *pResult = 1;
  }
  else
  {
    *pResult = 0;
  }
}

void 
PageLink2::OnOpenFiles_RClick_ToMe(NMHDR* pNMHDR, LRESULT* pResult)
{
  NMITEMACTIVATE* pItem = (NMITEMACTIVATE*)pNMHDR;
  if (pItem && pItem->iItem != -1)
  {
    CPoint point(pItem->ptAction);
    m_listToMe.ClientToScreen(&point);

    CMenu menu;
    VERIFY(menu.LoadMenu(IDR_WORKBOOK_POPUP));
    CMenu* pPopup = menu.GetSubMenu(0);

    ASSERT(pPopup != NULL);
    ASSERT_KINDOF(CFrameWnd, AfxGetMainWnd());
    TrackPopupMenu(menu,TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, 0,GetSafeHwnd(),nullptr);

    *pResult = 1;
  }
  else
  {
    *pResult = 0;
  }
}

void 
PageLink2::OnTimer(UINT_PTR nIDEvent)
{
  BOOL active = FALSE;

  switch(nIDEvent)
  {
    case ACTIVATE_FILE_TIMER_SET:  SetPageLinks();
                                   if(m_htmlView == NULL)
                                   {
                                     KillTimer(ACTIVATE_FILE_TIMER_SET);
                                   }
                                   break;
    case ACTIVATE_FILE_TIMER_FROM: active = GetFocus() == &m_listFrom ? TRUE : FALSE;
                                   ActivateOpenFile(&m_listFrom);
                                   if (active) 
                                   {
                                     m_listFrom.SetFocus();
                                   }
                                   KillTimer(ACTIVATE_FILE_TIMER_FROM);
                                   break;
    case ACTIVATE_FILE_TIMER_TOME: active = GetFocus() == &m_listToMe ? TRUE : FALSE;
                                   ActivateOpenFile(&m_listToMe);
                                   if (active) 
                                   {
                                     m_listToMe.SetFocus();
                                   }
                                   KillTimer(ACTIVATE_FILE_TIMER_TOME);
                                   break;
  }
}
