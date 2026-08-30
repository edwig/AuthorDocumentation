//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    IndexChooseDlg.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for choosing a reference to an index entry
//
#include "stdafx.h"
#include "AuthorHTML.h"
#include "IndexChooseDlg.h"
#include "FileDialog.h"
#include "ProjectFile.h"
#include "DocumentFile.h"
#include "Misc.h"

// IndexChooseDlg dialog
IMPLEMENT_DYNAMIC(IndexChooseDlg, CDialog)

IndexChooseDlg::IndexChooseDlg(CWnd* pParent
                              ,CString base
                              ,IndexEntry* entry)
               :CDialog(IndexChooseDlg::IDD, pParent)
               ,m_base(base)
               ,m_entry(entry)
               ,m_current(-1)
{
}

IndexChooseDlg::~IndexChooseDlg()
{
}

void IndexChooseDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Control(pDX,IDC_LIST,      m_list);
  DDX_Control(pDX,IDC_BOOKMARKS, m_comboBM);
  DDX_Control(pDX,IDOK,          m_buttonOK);
  DDX_Control(pDX,IDCANCEL,      m_buttonCancel);
  DDX_Control(pDX,ID_HELP_INDEXPROP, m_buttonHelp);
}

BEGIN_MESSAGE_MAP(IndexChooseDlg, CDialog)
  ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST,  OnLvnItemchangedList)
  ON_NOTIFY(NM_DBLCLK,       IDC_LIST,  OnListDblClick)
  ON_BN_CLICKED   (IDOK,                OnBnClickedOk)
  ON_BN_CLICKED   (IDCANCEL,            OnBnClickedCancel)
  ON_BN_CLICKED   (ID_HELP_INDEXPROP,   OnBnClickedHelpIndexprop)
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(IndexChooseDlg, CDialog)
  ON_EVENT(IndexChooseDlg, IDC_EXPLORER, 0x00000103, OnDocumentComplete, VTS_DISPATCH VTS_VARIANT)
END_EVENTSINK_MAP()

BOOL
  IndexChooseDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  CWnd *pWnd = GetDlgItem(IDC_EXPLORER);
  if(pWnd)
  {
    m_spBrowser = pWnd->GetControlUnknown();
  }
  m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EDITLABELS);
  m_list.InsertColumn(0,"Title",    LVCFMT_LEFT,300);
  m_list.InsertColumn(1,"Filename", LVCFMT_LEFT,300);
  m_list.InsertColumn(2,"Author",   LVCFMT_LEFT,200);

  RedisplayPages();
  UpdateData(Data2Controls);
  return TRUE;
}

void
IndexChooseDlg::RedisplayPages()
{
  int ind = 0;
  m_list.DeleteAllItems();
  IndDocMap& docmap = m_entry->GetDocuments();
  IndDocMap::iterator it = docmap.begin();
  while(it != docmap.end())
  {
    IndexDocument* id = *it;
    DocumentFile* doc = id->m_document;
    if(doc)
    {
      CString title = id->m_title;
      CString file  = doc->GetFilename();
      CString auth  = doc->GetAuthor();
      if(!ind && m_spBrowser)
      {
        CString URL = m_base + file;
        m_spBrowser->Navigate(URL.AllocSysString(),NULL,NULL,NULL,NULL);
      }
      m_list.InsertItem(LVIF_TEXT|LVIF_STATE, ind, title, 0, 0, 0, 0);
      m_list.SetItemText(ind,1,file);
      m_list.SetItemText(ind,2,auth);
    }
    ++ind;
    ++it;
  }
  if(ind > 0)
  {
    m_list.SetItemState(0,LVNI_SELECTED,LVNI_SELECTED);
    m_list.SetFocus();
  }
}

// IndexChooseDlg message handlers

void 
IndexChooseDlg::OnDocumentComplete(LPDISPATCH /*pDisp*/, LPVARIANT /*pURL*/)
{
  bool found = false;
  m_comboBM.ResetContent();
  m_comboBM.AddString("");

  // Now read the bookmarks from the document (if any)
  CComPtr<IDispatch> disp;
  HRESULT hr = m_spBrowser->get_Document(&disp);
  CComQIPtr<IHTMLDocument2,&IID_IHTMLDocument2> doc = disp;
  if(SUCCEEDED(hr) && doc.p)
  {
    CComPtr<IHTMLElementCollection> coll;
    hr = doc->get_anchors(&coll);
    if(SUCCEEDED(hr))
    {
      long num = 0;
      coll->get_length(&num);
      for(int index = 0; index < num; ++index)
      {
        CComVariant object;
        V_VT(&object) = VT_I4;
        V_I4(&object) = index;
        CComPtr<IDispatch> disp2;
        hr = coll->item(object,object,&disp2);
        if(SUCCEEDED(hr))
        {
          CComBSTR attrib = L"name";
          CComVariant  value;
          V_VT(&value) = VT_BSTR;
          CComQIPtr<IHTMLElement,&IID_IHTMLElement> elem = disp2;
          elem->getAttribute(attrib,0,&value);
          CString TOCName = V_BSTR(&value);
          if(!TOCName.IsEmpty())
          {
            m_comboBM.AddString(TOCName);
            found = true;

            CComQIPtr<IHTMLElement2,&IID_IHTMLElement2> elem2 = elem;
            CComPtr<IHTMLStyle> style;
            CComVariant color;
            V_VT(&color) = VT_I4;
            V_I4(&color) = RGB(0,0,255);
            elem2->get_runtimeStyle(&style);
            style->put_backgroundColor(color);
          }
        }
      }
    }
  }
  if(found)
  {
    UpdateData(Data2Controls);
  }
}

void
IndexChooseDlg::ScrollIntoView(CString bookmark)
{
  // Now read the bookmarks from the document (if any)
  CComPtr<IDispatch> disp;
  m_spBrowser->get_Document(&disp);
  CComQIPtr<IHTMLDocument2,&IID_IHTMLDocument2> doc = disp;

  CComPtr<IHTMLElementCollection> coll;
  HRESULT hr = doc->get_anchors(&coll);
  if(SUCCEEDED(hr))
  {
    long num = 0;
    coll->get_length(&num);
    for(int index = 0; index < num; ++index)
    {
      CComVariant object;
      V_VT(&object) = VT_I4;
      V_I4(&object) = index;
      CComPtr<IDispatch> disp2;
      hr = coll->item(object,object,&disp2);
      if(SUCCEEDED(hr))
      {
        CComBSTR attrib = L"name";
        CComVariant  value;
        V_VT(&value) = VT_BSTR;
        CComQIPtr<IHTMLElement,&IID_IHTMLElement> elem = disp2;
        elem->getAttribute(attrib,0,&value);
        CString anchorName = V_BSTR(&value);
        if(anchorName.CompareNoCase(bookmark) == 0)
        {
          CComVariant top;
          V_VT(&top) = VT_BOOL;
          V_I4(&top) = VARIANT_TRUE;
          elem->scrollIntoView(top);
          return;
        }
      }
    }
  }
}

void 
IndexChooseDlg::OnLvnItemchangedList(NMHDR *pNMHDR, LRESULT *pResult)
{
  LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
  char filename[256];
  char title   [256];

  m_current = pNMLV->iItem;
  m_list.GetItemText(m_current,0,title   ,256);
  m_list.GetItemText(m_current,1,filename,256);
  if(m_spBrowser)
  {
    CString URL = m_base + filename;
    m_spBrowser->Navigate(URL.AllocSysString(),NULL,NULL,NULL,NULL);
  }
  *pResult = 0;
}

void 
IndexChooseDlg::OnListDblClick(NMHDR *pNMHDR, LRESULT *pResult)
{
  LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
  m_current = pNMLV->iItem;
  OnOK();
}

void 
IndexChooseDlg::OnCbnSelchangeBookmarks()
{
}

void 
IndexChooseDlg::OnBnClickedOk()
{
  OnOK();
}

void 
IndexChooseDlg::OnBnClickedCancel()
{
  OnCancel();
}

void 
IndexChooseDlg::OnBnClickedHelpIndexprop()
{
  // TODO: Add your control notification handler code here
}
