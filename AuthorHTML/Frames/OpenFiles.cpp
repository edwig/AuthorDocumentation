//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    OpenFiles.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Shows the currently opened files in the main window frame
//
#include "stdafx.h"
#include <Shlwapi.h>
#include <string>
#include "OpenFiles.h"
#include "MainFrm.h"
#include "AuthorHTML.h"

#define ID_FPW_OPEN_FILES   1000

#define ACTIVATE_FILE_TIMER 777
#define ACTIVATE_FILE_DELAY 500

OpenFiles::OpenFiles()
{
  m_imgList = NULL;
}

OpenFiles::~OpenFiles()
{
}

void 
OpenFiles::ActivateOpenFile ()
{
  POSITION pos = m_openFilesList.GetFirstSelectedItemPosition();

  if (pos)
  {
    LVITEM item;
    memset(&item, 0, sizeof(item));
    item.mask = LVIF_PARAM;
    item.iItem = m_openFilesList.GetNextSelectedItem(pos);

    VERIFY(m_openFilesList.GetItem(&item));
    _ASSERT(AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(MainFrame)));
    _ASSERT(((CMDIChildWnd*)item.lParam)->IsKindOf(RUNTIME_CLASS(CMDIChildWnd)));

    ((MainFrame*)AfxGetMainWnd())->ActivateChild((CMDIChildWnd*)item.lParam);
  }
}

BEGIN_MESSAGE_MAP(OpenFiles, CDockablePane)
    ON_WM_SIZE()
    ON_WM_CREATE()
    ON_NOTIFY(NM_CLICK,   ID_FPW_OPEN_FILES,OnOpenFiles_Click)
    ON_NOTIFY(NM_RCLICK,  ID_FPW_OPEN_FILES,OnOpenFiles_RClick)
    ON_NOTIFY(LVN_KEYDOWN,ID_FPW_OPEN_FILES,OnOpenFiles_KeyDown)
END_MESSAGE_MAP()

int
OpenFiles::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  CDockablePane::OnCreate(lpCreateStruct);

  CRect empty;
  if(!m_openFilesList.Create(WS_CHILD|WS_TABSTOP|WS_VISIBLE|
                             LVS_SORTASCENDING|LVS_SHOWSELALWAYS|LVS_REPORT|
                             LVS_NOCOLUMNHEADER|LVS_SINGLESEL
                            ,empty
                            ,this
                            ,ID_FPW_OPEN_FILES))
  {
    return NULL;
  }
  m_openFilesList.InsertColumn(0, (LPCSTR)NULL);
  m_openFilesList.SetColumnWidth(0,300);
  m_openFilesList.SetExtendedStyle(m_openFilesList.GetExtendedStyle()|LVS_EX_FULLROWSELECT);
  //m_openFilesList.ModifyStyleEx(0, WS_EX_CLIENTEDGE, 0);

  SHFILEINFO shFinfo;
  HIMAGELIST hImgList = (HIMAGELIST)SHGetFileInfo("", 0, &shFinfo, sizeof(shFinfo), SHGFI_SMALLICON|SHGFI_SYSICONINDEX);
  // Record the image list as 'permanent'
  ::SendMessage(m_openFilesList.m_hWnd,LVM_SETIMAGELIST, (LPARAM)hImgList, (UINT)LVSIL_SMALL);
  if (m_imgList && m_imgList->m_hImageList) 
  {
    m_imgList->Detach();
  }
  m_imgList = CImageList::FromHandle(hImgList);
  m_openFilesList.SetImageList(m_imgList, LVSIL_SMALL);
  m_openFilesList.ShowWindow(SW_SHOW);

  return 0;
}

void 
OpenFiles::OnSize(UINT nType, int cx, int cy)
{
  CDockablePane::OnSize(nType, cx, cy);

  if (cx > 0 && cy > 0 && nType != SIZE_MAXHIDE && nType != SIZE_MINIMIZED) 
  {
    CRect rect;
    GetClientRect(&rect);
    rect.InflateRect(3, 0);

    HDWP hdwp = ::BeginDeferWindowPos(2);
    ::DeferWindowPos(hdwp, m_openFilesList, 0, rect.left, rect.top, 
      rect.Width(), rect.Height(), SWP_NOZORDER);
    ::EndDeferWindowPos(hdwp);

    if(m_openFilesList.GetSafeHwnd())
    {
      m_openFilesList.GetClientRect(rect);
      m_openFilesList.SetColumnWidth(0,rect.Width());
    }
  }
}

void
OpenFiles::OpenFilesStarted()
{
  MainFrame *pFrm = (MainFrame*)AfxGetMainWnd();
  pFrm->ShowPane(ID_PANE_DOCUMENTS);
}

void 
OpenFiles::OpenFiles_Append(LVITEM& item)
{
  OpenFilesStarted();
  if(m_openFilesList)
  {
    item.iItem = m_openFilesList.GetItemCount();
    VERIFY(m_openFilesList.InsertItem(&item) != -1);
  }
}


void 
OpenFiles::OpenFiles_UpdateByParam (LPARAM param, LVITEM& item)
{
  int nItem = OpenFiles_FindByParam(param);

  ASSERT(nItem != -1);

  if (nItem != -1)
  {
    // sorting does not work for update so ...
    VERIFY(m_openFilesList.DeleteItem(nItem));
    VERIFY(m_openFilesList.InsertItem(&item) != -1);
  }
}

void 
OpenFiles::OpenFiles_RemoveByParam (LPARAM param)
{
  int nItem = OpenFiles_FindByParam(param);
  ASSERT(nItem != -1);
  if (nItem != -1)
  {
    VERIFY(m_openFilesList.DeleteItem(nItem));
  }
}

void 
OpenFiles::OpenFiles_ActivateByParam (LPARAM param)
{
  int nItem = OpenFiles_FindByParam(param);
  ASSERT(nItem != -1);
  if (nItem != -1)
  {
    VERIFY(m_openFilesList.SetItemState(nItem, LVIS_SELECTED, LVIS_SELECTED));
  }
}


LPARAM 
OpenFiles::OpenFiles_GetCurSelParam ()
{
  int nItem = m_openFilesList.GetNextItem(-1, LVNI_SELECTED);
  ASSERT(nItem != -1);

  if (nItem != -1)
  {
    LVITEM item;
    item.mask = LVIF_PARAM;
    item.iItem = nItem;
    VERIFY(m_openFilesList.GetItem(&item));
    return item.lParam;
  }
  return 0;
}


int 
OpenFiles::OpenFiles_FindByParam (LPARAM param)
{
  LVITEM item;
  memset(&item, 0, sizeof(item));
  item.mask = LVIF_PARAM|LVIF_TEXT;
  int nItems = m_openFilesList.GetItemCount();

  for (item.iItem = 0; item.iItem < nItems; item.iItem++) 
  {
    VERIFY(m_openFilesList.GetItem(&item));

    if (item.lParam == param)
    {
      return item.iItem;
    }
  }
  return -1;
}

LPARAM
OpenFiles::OpenFiles_FindParamByNumber(int number)
{
  LVITEM item;
  memset(&item, 0, sizeof(item));
  item.mask  = LVIF_PARAM;
  item.iItem = number;
  int nItems = m_openFilesList.GetItemCount();

  if(number <= nItems)
  {
    m_openFilesList.GetItem(&item);
    return item.lParam;
  }
  return NULL;
}

void 
OpenFiles::OnOpenFiles_Click (NMHDR*, LRESULT* pResult)
{
  ActivateOpenFile();
  *pResult = 0;
}

void 
OpenFiles::OnOpenFiles_KeyDown (NMHDR*, LRESULT* pResult)
{
  SetTimer(ACTIVATE_FILE_TIMER, ACTIVATE_FILE_DELAY, 0);
  *pResult = 0;
}

void 
OpenFiles::OnOpenFiles_RClick (NMHDR* pNMHDR, LRESULT* pResult)
{
  ActivateOpenFile();

  NMITEMACTIVATE* pItem = (NMITEMACTIVATE*)pNMHDR;
  if (pItem && pItem->iItem != -1)
  {
    CPoint point(pItem->ptAction);
    m_openFilesList.ClientToScreen(&point);

    CMenu menu;
    VERIFY(menu.LoadMenu(IDR_WORKBOOK_POPUP));
    CMenu* pPopup = menu.GetSubMenu(0);

    ASSERT(pPopup != NULL);
    ASSERT_KINDOF(CFrameWnd, AfxGetMainWnd());
    //Common::GUICommandDictionary::AddAccelDescriptionToMenu(pPopup->m_hMenu);
    TrackPopupMenu(menu,TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y,0,GetSafeHwnd(),nullptr);

    *pResult = 1;
  }
  else
  {
    *pResult = 0;
  }
}

void 
OpenFiles::OnTimer(UINT nIDEvent)
{
  if (nIDEvent == ACTIVATE_FILE_TIMER)
  {
    BOOL active = GetFocus() == &m_openFilesList ? TRUE : FALSE;
    ActivateOpenFile();
    if (active) 
    {
      m_openFilesList.SetFocus();
    }
  }
  CDockablePane::OnTimer(nIDEvent);
}
