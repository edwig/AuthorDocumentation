//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    FindTopicDlg.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for finding topics.
//
#include "stdafx.h"
#include "AuthorHTML.h"
#include "FindTopicDlg.h"
#include "DocumentFile.h"
#include "ProjectFile.h"

// FindTopicDlg dialog

IMPLEMENT_DYNAMIC(FindTopicDlg, CDialog)

FindTopicDlg::FindTopicDlg(CWnd* pParent
                          ,CString base
                          ,bool    p_allowBookmarks)
	           :CDialog(FindTopicDlg::IDD, pParent)
             ,m_base(base)
             ,m_allowBookmarks(p_allowBookmarks)
{
}

FindTopicDlg::~FindTopicDlg()
{
}

void FindTopicDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
  DDX_Text   (pDX,IDC_TITLE,    m_findTitle);
  DDX_Text   (pDX,IDC_FILENAME, m_findFile);
  DDX_Control(pDX,IDC_FIND,     m_buttonFind);
  DDX_Control(pDX,IDC_LIST,     m_list);
  DDX_Control(pDX,IDC_BOOKMARKS,m_comboBM);
  DDX_Control(pDX,IDOK,         m_buttonOK);
  DDX_Control(pDX,IDCANCEL,     m_buttonCancel);
  DDX_Control(pDX,ID_HELP_FINDTOPIC, m_buttonHelp);
}

BEGIN_MESSAGE_MAP(FindTopicDlg, CDialog)
  ON_EN_KILLFOCUS (IDC_TITLE,         OnEnChangeTitle)
  ON_EN_KILLFOCUS (IDC_FILENAME,      OnEnChangeFilename)
  ON_BN_CLICKED   (IDC_FIND,          OnBnClickedFind)
  ON_CBN_SELCHANGE(IDC_BOOKMARKS,     OnCbnSelchangeBookmarks)
  ON_BN_CLICKED   (IDOK,              OnBnClickedOk)
  ON_BN_CLICKED   (IDCANCEL,          OnBnClickedCancel)
  ON_BN_CLICKED   (ID_HELP_FINDTOPIC, OnBnClickedHelpFindtopic)
  ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST,OnLvnItemchangedList)
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(FindTopicDlg, CDialog)
  ON_EVENT(FindTopicDlg, IDC_EXPLORER, 0x00000103, OnDocumentComplete, VTS_DISPATCH VTS_VARIANT)
END_EVENTSINK_MAP()

int
FindTopicDlg::OnInitDialog() 
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

  m_comboBM.EnableWindow(m_allowBookmarks);
  return TRUE;
}

// FindTopicDlg message handlers

void 
FindTopicDlg::OnDocumentComplete(LPDISPATCH /*pDisp*/, LPVARIANT /*pURL*/)
{
  bool found = false;
  m_comboBM.ResetContent();
  m_comboBM.AddString("");

  if(m_allowBookmarks == false)
  {
    return;
  }
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
FindTopicDlg::ScrollIntoView(CString bookmark)
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
FindTopicDlg::OnEnChangeTitle()
{
  CWnd* title = GetDlgItem(IDC_TITLE);
  if(title)
  {
    title->GetWindowText(m_findTitle);
  }
}

void 
FindTopicDlg::OnEnChangeFilename()
{
  CWnd* file = GetDlgItem(IDC_FILENAME);
  if(file)
  {
    file->GetWindowText(m_findFile);
  }
}

void 
FindTopicDlg::OnBnClickedFind()
{
  // Get find data
  UpdateData();

  // Regular find in the projectfile's documents
  DocumentMap found;
  theApp.GetProjectFile()->FindRegular(m_findTitle,m_findFile,found);

  // Clear the list
  m_list.DeleteAllItems();

  // Fill the list again
  int ind = 0;
  DocumentMap::iterator it = found.begin();
  while(it != found.end())
  {
    DocumentFile* doc = it->second;
    CString title = doc->GetTitle();
    CString file  = doc->GetFilename();
    CString auth  = doc->GetAuthor();
  
    if(!ind && m_spBrowser)
    {
      CString URL = m_base + file;
      m_spBrowser->Navigate(URL.AllocSysString(),NULL,NULL,NULL,NULL);
      m_foundTopic = file;
      m_foundTitle = title;
    }
    m_list.InsertItem(LVIF_TEXT|LVIF_STATE, ind, title, 0, 0, 0, 0);
    m_list.SetItemText(ind,1,file);
    m_list.SetItemText(ind,2,auth);
    ++ind;
    ++it;
  }
}

void 
FindTopicDlg::OnLvnItemchangedList(NMHDR* pNMHDR, LRESULT *pResult)
{
  LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
  char filename[256];
  char title   [256];
  m_list.GetItemText(pNMLV->iItem,0,title   ,256);
  m_list.GetItemText(pNMLV->iItem,1,filename,256);
  if(m_spBrowser)
  {
    CString URL = m_base + filename;
    m_spBrowser->Navigate(URL.AllocSysString(),NULL,NULL,NULL,NULL);
    m_foundTopic = filename;
    m_foundTitle = title;
  }
  *pResult = 0;
}

void 
FindTopicDlg::OnCbnSelchangeBookmarks()
{
  int ind = m_comboBM.GetCurSel();
  if(ind >= 0)
  {
    m_comboBM.GetLBText(ind,m_bookmark);
    if(!m_bookmark.IsEmpty())
    {
      ScrollIntoView(m_bookmark);
    }
  }
}

void 
FindTopicDlg::OnBnClickedOk()
{
  OnOK();
}

void 
FindTopicDlg::OnBnClickedCancel()
{
  OnCancel();
}

void 
FindTopicDlg::OnBnClickedHelpFindtopic()
{
  // TODO: Add your control notification handler code here
}
