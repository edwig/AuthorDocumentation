//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    IndexPropDlg.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for managing the properties of an index entry
//
#include "stdafx.h"
#include "AuthorHTML.h"
#include "IndexPropDlg.h"
#include "FileDialog.h"
#include "ProjectFile.h"
#include "DocumentFile.h"
#include "Misc.h"

// IndexPropDlg dialog
IMPLEMENT_DYNAMIC(IndexPropDlg, CDialog)

IndexPropDlg::IndexPropDlg(CWnd* pParent
                          ,CString base
                          ,IndexEntry* entry)
	           :CDialog(IndexPropDlg::IDD, pParent)
             ,m_base(base)
             ,m_entry(entry)
{
}

IndexPropDlg::~IndexPropDlg()
{
}

void IndexPropDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
  DDX_Text   (pDX,IDC_KEYWORD,   m_keyword);
  DDX_Control(pDX,IDC_DELETE,    m_buttonDelete);
  DDX_Control(pDX,IDC_FIND,      m_buttonFind);
  DDX_Control(pDX,IDC_WINDOW,    m_comboWindow);
  DDX_Control(pDX,IDC_FRAME,     m_comboFrame);
  DDX_Text   (pDX,IDC_COMMENT,   m_comment);
  DDX_Control(pDX,IDC_INTOPIC,   m_buttonInTopic);
  DDX_Control(pDX,IDC_INFILE,    m_buttonInHHK);
  DDX_Control(pDX,IDC_LIST,      m_list);
  DDX_Control(pDX,IDC_BOOKMARKS, m_comboBM);
  DDX_Control(pDX,IDOK,          m_buttonOK);
  DDX_Control(pDX,IDCANCEL,      m_buttonCancel);
  DDX_Control(pDX,ID_HELP_INDEXPROP, m_buttonHelp);

  if(pDX->m_bSaveAndValidate == Data2Controls)
  {
    int ind;
    CString desc = Misc::GetAttributeDisplayname("target",m_frame);
    ind = m_comboFrame.FindString(-1,desc);
    m_comboFrame.SetCurSel(ind);

    m_buttonInHHK  .SetCheck( m_inHHK);
    m_buttonInTopic.SetCheck(!m_inHHK);
  }
}

BEGIN_MESSAGE_MAP(IndexPropDlg, CDialog)
  ON_EN_KILLFOCUS (IDC_KEYWORD,         OnEnChangeKeyword)
  ON_BN_CLICKED   (IDC_DELETE,          OnBnClickedDelete)
  ON_BN_CLICKED   (IDC_FIND,            OnBnClickedFind)
  ON_CBN_SELCHANGE(IDC_WINDOW,          OnCbnSelchangeWindow)
  ON_CBN_SELCHANGE(IDC_FRAME,           OnCbnSelchangeFrame)
  ON_EN_KILLFOCUS (IDC_COMMENT,         OnEnChangeComment)
  ON_BN_CLICKED   (IDC_INTOPIC,         OnBnClickedIntopic)
  ON_BN_CLICKED   (IDC_INFILE,          OnBnClickedInfile)
  ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST,  OnLvnItemchangedList)
  ON_CBN_SELCHANGE(IDC_BOOKMARKS,       OnCbnSelchangeBookmarks)
  ON_BN_CLICKED   (IDOK,                OnBnClickedOk)
  ON_BN_CLICKED   (IDCANCEL,            OnBnClickedCancel)
  ON_BN_CLICKED   (ID_HELP_INDEXPROP,   OnBnClickedHelpIndexprop)
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(IndexPropDlg, CDialog)
  ON_EVENT(IndexPropDlg, IDC_EXPLORER, 0x00000103, OnDocumentComplete, VTS_DISPATCH VTS_VARIANT)
END_EVENTSINK_MAP()

BOOL
IndexPropDlg::OnInitDialog()
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

  // All target descriptions
  vector<string> all;
  Misc::GetAllAttributeDisplaynames("target",&all);
  for(unsigned int ind=0; ind<all.size(); ++ind)
  {
    m_comboFrame.AddString(all[ind].c_str());
  }

  FillPage();
  UpdateData(Data2Controls);
  return TRUE;
}

void
IndexPropDlg::FillPage()
{
  m_keyword = m_entry->GetTitle();
  m_window  = m_entry->GetWindowName();
  m_frame   = m_entry->GetFrameName();
  m_comment = m_entry->GetComment();
  m_inHHK   = m_entry->GetInHHK();

  RedisplayPages();
}

void
IndexPropDlg::RedisplayPages()
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
}

void
IndexPropDlg::UpdateProperties()
{
  UpdateData(Controls2Data);
  m_entry->SetTitle(m_keyword);
  m_entry->SetWindowName(m_window);
  m_entry->SetFrameName(m_frame);
  m_entry->SetComment(m_comment);
  m_entry->SetInHHK(m_inHHK);
}

// IndexPropDlg message handlers

void 
IndexPropDlg::OnDocumentComplete(LPDISPATCH /*pDisp*/, LPVARIANT /*pURL*/)
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
IndexPropDlg::ScrollIntoView(CString bookmark)
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
IndexPropDlg::OnEnChangeKeyword()
{
  CWnd* w = GetDlgItem(IDC_KEYWORD);
  w->GetWindowText(m_keyword);
}

void
IndexPropDlg::OnBnClickedDelete()
{
  int num = m_list.GetItemCount();
  int now = m_list.GetNextItem(-1,LVNI_SELECTED);
  if(now < num)
  {
    CString mess;
    CString href = m_list.GetItemText(now,1);
    mess.Format("Do you want to delete the following topic?\n%s",href.GetString());
    if(theApp.MessageBox(mess,"Delete?",MB_YESNO|MB_ICONQUESTION) == IDYES)
    {
      m_entry->DeleteDocument(href);
      RedisplayPages();
    }
  }
}

void 
IndexPropDlg::OnBnClickedFind()
{
  DocFileDialog diag(true
                    ,"Search for a page to link to"
                    ,"htm"
                    ,""
                    ,0);
  if(diag.DoModal() == IDOK)
  {
    CString href = diag.GetChosenFile();
    CString relative;
    if(Misc::MakeRelativePathname(m_base,href,relative))
    {
      href = relative;
    }
    CString URL = m_base + href;
    m_spBrowser->Navigate(URL.AllocSysString(),NULL,NULL,NULL,NULL);

    CString title = "No title yet";
    ProjectFile* project = theApp.GetProjectFile();
    DocumentFile* doc = project->FindDocumentFile(href);
    if(doc)
    {
      title = doc->GetTitle();
      m_entry->AddDocument(title,href);
      RedisplayPages();
    }
    else
    {
      CString mess;
      mess.Format("The page [%s] is not a part of the project",href.GetString());
      theApp.ErrorMessage(mess);
    }
  }
}

void 
IndexPropDlg::OnCbnSelchangeWindow()
{
  int ind = m_comboWindow.GetCurSel();
  if(ind >= 0)
  {
    m_comboWindow.GetLBText(ind,m_window);
  }
}

void 
IndexPropDlg::OnCbnSelchangeFrame()
{
  int ind = m_comboFrame.GetCurSel();
  if(ind >= 0)
  {
    CString frame;
    m_comboFrame.GetLBText(ind,frame);
    m_frame = Misc::GetAttributeValue("target",frame);
  }
}

void 
IndexPropDlg::OnEnChangeComment()
{
  CWnd* w = GetDlgItem(IDC_COMMENT);
  w->GetWindowText(m_comment);
}

void 
IndexPropDlg::OnBnClickedIntopic()
{
  m_inHHK = false;
}

void 
IndexPropDlg::OnBnClickedInfile()
{
  m_inHHK = true;
}

void 
IndexPropDlg::OnLvnItemchangedList(NMHDR *pNMHDR, LRESULT *pResult)
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
  }
  *pResult = 0;
}

void 
IndexPropDlg::OnCbnSelchangeBookmarks()
{
}

void 
IndexPropDlg::OnBnClickedOk()
{
  UpdateProperties();
  OnOK();
}

void 
IndexPropDlg::OnBnClickedCancel()
{
  OnCancel();
}

void 
IndexPropDlg::OnBnClickedHelpIndexprop()
{
  // TODO: Add your control notification handler code here
}
