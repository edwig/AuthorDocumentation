//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    TOCDlg.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for properties of a single TOC entry.
//
#include "stdafx.h"
#include "AuthorHTML.h"
#include "TOCDlg.h"
#include "ProjectFile.h"
#include "DocumentFile.h"
#include "FileDialog.h"
#include "FindTopicDlg.h"
#include "Misc.h"
#include <vector>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CIconImage::CIconImage()
{
}
CIconImage::~CIconImage()
{
}
BEGIN_MESSAGE_MAP(CIconImage,CStatic)
  ON_WM_PAINT()
END_MESSAGE_MAP()
void
CIconImage::OnPaint()
{
  SetIcon(m_hicon);
  CStatic::OnPaint();
}

/////////////////////////////////////////////////////////////////////////////
// TOCDlg dialog


TOCDlg::TOCDlg(CWnd*     pParent
              ,CString   base
              ,TOCEntry* entry)
              :CDialog(TOCDlg::IDD, pParent)
              ,m_base(base)
              ,m_entry(entry)
{
  m_image = "Default";
}

TOCDlg::~TOCDlg()
{
}

void TOCDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Text   (pDX,IDC_TOC_TITLE,       m_title);
  if(::IsWindow(m_AddrCombo.m_hWnd))
  {
    DDX_Text(pDX, IDC_ADDRCOMBO, m_href);
  }
  DDX_Text   (pDX,IDC_EDIT_HYPINFO,    m_comment);
  DDX_Control(pDX,IDC_WINDOW_TARGET,   m_comboWindow);
  DDX_Control(pDX,IDC_FRAME_TARGET,    m_comboFrame);
  DDX_Control(pDX,IDC_IMAGENUMBER,     m_comboImage);
  DDX_Control(pDX,IDC_BUTTON_OPEN,     m_buttonOpen);
  DDX_Control(pDX,IDC_BTNGO,           m_buttonGo);
  DDX_Control(pDX,IDC_BOOKMARKS,       m_comboBM);
  DDX_Control(pDX,IDC_ICONIMAGE,       m_iconImage);
  DDX_Control(pDX,IDOK,                m_buttonOK);
  DDX_Control(pDX,IDCANCEL,            m_buttonCancel);
  DDX_Control(pDX,IDC_BUTTON_NEWPAGE,  m_buttonNew);
  DDX_Control(pDX,IDC_BUTTON_FINDPAGE, m_buttonFind);
  DDX_Control(pDX,ID_HELP_URL,         m_buttonHelp);

  if(pDX->m_bSaveAndValidate == Data2Controls)
  {
    int ind;
    CString desc = Misc::GetAttributeDisplayname("target",m_target);
    ind = m_comboFrame.FindString(-1,desc);
    m_comboFrame.SetCurSel(ind);
    ind = m_comboWindow.FindString(-1,m_window);
    m_comboWindow.SetCurSel(ind);
    ind = m_comboImage.FindString(-1,m_image);
    m_comboImage.SetCurSel(ind);

    ind = m_comboBM.FindString(-1,m_bookmark);
    m_comboBM.SetCurSel(ind);

    // Only enable BM combo if mid-page bookmarks found
    m_comboBM.EnableWindow(m_comboBM.GetCount());

    OnPaintIcon();
  }
}

BEGIN_MESSAGE_MAP(TOCDlg, CDialog)
  ON_EN_CHANGE    (IDC_TOC_TITLE,      OnEnChangeTitle)
  ON_CONTROL      (CBN_CLOSEUP,IDC_ADDRCOMBO,OnCloseup)
  ON_CONTROL      (BN_CLICKED,IDC_BTNGO,     OnGo)
  ON_BN_CLICKED   (IDC_BUTTON_OPEN,    OnBnClickedButtonOpen)
  ON_EN_KILLFOCUS (IDC_EDIT_HYPINFO,   OnEnChangeEditHypinfo)
  ON_CBN_SELCHANGE(IDC_WINDOW_TARGET,  OnCbnSelchangeWindowTarget)
  ON_CBN_SELCHANGE(IDC_FRAME_TARGET,   OnCbnSelchangeFrameTarget)
  ON_CBN_SELCHANGE(IDC_IMAGENUMBER,    OnCbnSelchangeImagenumber)
  ON_CBN_SELCHANGE(IDC_BOOKMARKS,      OnCbnSelchangeBookmarks)
  ON_BN_CLICKED   (IDC_BUTTON_NEWPAGE, OnBnClickedButtonNewpage)
  ON_BN_CLICKED   (IDC_BUTTON_FINDPAGE,OnBnClickedButtonFindpage)
  ON_BN_CLICKED   (ID_HELP_URL,        OnBnClickedHelpTOC)
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(TOCDlg, CDialog)
  ON_EVENT(TOCDlg, IDC_EXPLORER, 0x00000103, OnDocumentComplete, VTS_DISPATCH VTS_VARIANT)
END_EVENTSINK_MAP()

BOOL 
TOCDlg::OnInitDialog() 
{
  CDialog::OnInitDialog();
  IUnknown *pUnk;

  CWnd *pWnd = GetDlgItem(IDC_EXPLORER);
  pUnk = NULL;
  if(pWnd)
  {
    m_spBrowser = pWnd->GetControlUnknown();
  }
  FillPage();

  pWnd = NULL;
  CRect rcItem;
  pWnd = GetDlgItem(IDC_PHSTATIC);
  if(pWnd)
  {
    pWnd->GetClientRect(rcItem);
    pWnd->ClientToScreen(rcItem);
    pWnd->DestroyWindow();
    ScreenToClient(rcItem);
    rcItem.bottom += 150;
    if(m_AddrCombo.Create(WS_VSCROLL|WS_CHILD|WS_VISIBLE|CBS_DROPDOWN|CBS_AUTOHSCROLL,rcItem,this,IDC_ADDRCOMBO))
    {
      if(m_href.IsEmpty())
      {
        // TOC could be for a #named TOC on same page
        m_AddrCombo.SetCurSel(-1);
        //if(m_spBrowser)
        //{
        //  m_spBrowser->Navigate(m_base.AllocSysString(),NULL,NULL,NULL,NULL);
        //}
      }
      else
      {
        // TOC is not empty
        int ind = m_AddrCombo.AddString(m_href);
        m_AddrCombo.SetCurSel(ind);
        if(m_spBrowser)
        {
          CString URL = m_base + m_href;
          m_spBrowser->Navigate(URL.AllocSysString(),NULL,NULL,NULL,NULL);
        }
      }
    }
  }
  // All target descriptions
  vector<string> all;
  Misc::GetAllAttributeDisplaynames("target",&all);
  for(unsigned int ind=0; ind<all.size(); ++ind)
  {
    m_comboFrame.AddString(all[ind].c_str());
  }
  // Set image numbers;
  m_comboImage.AddString("Default");
  for(int ind = 1; ind < 43; ++ind)
  {
    CString number;
    number.Format("%d",ind);
    m_comboImage.AddString(number);
  }
  // CImagelist
  m_imageList.Create(MAKEINTRESOURCE(IDB_TOC),16,0,RGB(255,255,255));

  UpdateData(Data2Controls);
  return TRUE;
}

void
TOCDlg::OnPaintIcon()
{
  if(!m_imageList)
  {
    // Not sofar in initialisation
    return;
  }
  int image = atoi(m_image);
  if(image <= 0)
  {
    image = (m_entry->EntryType() == PF_Book) ? 0 : 8;
  }
  else
  { 
    --image;
  }
  HICON hicon = m_imageList.ExtractIcon(image);
  m_iconImage.SetTOCIcon(hicon);
  m_iconImage.RedrawWindow();
}

void
TOCDlg::FillPage()
{
  m_href    = m_entry->GetDocumentFilename();
  m_title   = m_entry->GetTitle();
  m_comment = m_entry->GetComment();
  int image = m_entry->GetImageNumber();
  m_image = "Default";
  if(image >= 0)
  {
    m_image.Format("%d",image + 1);
  }
  // Test for bookmark
  int pos = m_href.Find('#');
  if(pos >= 0)
  {
    m_bookmark = m_href.Mid(pos + 1);
    m_href     = m_href.Left(pos);
  }
}

bool
TOCDlg::UpdateProperties()
{
  CString href = m_href;
  if(Misc::MakeRelativePathname(m_base,m_href,href) == false)
  {
    if(href.IsEmpty())
    {
      href = m_href;
    }
  }
  if(!CheckDocument(href))
  {
    return false;
  }
  m_entry->SetDocumentFilename(href);
  m_entry->SetBookmark(m_bookmark);
  m_entry->SetTitle(m_title);
  m_entry->SetComment(m_comment);
  // Set image
  int image = -1;
  if(m_image.CompareNoCase("Default") != 0)
  {
    image = atoi(m_image) - 1;
  }
  m_entry->SetImageNumber(image);
  return true;
}

bool
TOCDlg::CheckDocument(CString& href)
{
  ProjectFile* project   = theApp.GetProjectFile();
  DocumentFile* document = project->FindDocumentFile(href);
  if(!document)
  {
    if(theApp.MessageBox("This document is not a part of this project!\n"
                         "Would you like to try to add it to the project for future use?"
                        ,"Warning"
                        ,MB_YESNO|MB_DEFBUTTON1|MB_ICONWARNING) == IDYES)
    {
      project->AddDocumentFile(href);
    }
    return false;
  }
  return true;
}

/////////////////////////////////////////////////////////////////////////////
// TOCDlg message handlers

void 
TOCDlg::OnDocumentComplete(LPDISPATCH /*pDisp*/, LPVARIANT /*pURL*/)
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
    // If no title yet, get it from the document
    if(m_title.IsEmpty())
    {
      CComBSTR title;
      doc->get_title(&title);
      m_title = title;
      if(!m_title.IsEmpty())
      {
        found = true;
      }
    }
    // Read the list of document anchors
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
TOCDlg::ScrollIntoView(CString bookmark)
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
        CString TOCName = V_BSTR(&value);
        if(TOCName.CompareNoCase(bookmark) == 0)
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
TOCDlg::OnEnChangeTitle()
{
  UpdateData(Controls2Data);
}

void 
TOCDlg::OnCloseup()
{	
  int nSel = m_AddrCombo.GetCurSel();
  if(CB_ERR != nSel)
  {
    m_AddrCombo.GetLBText(nSel,m_href);
    if(m_spBrowser)
    {
      CString URL = m_base + m_href;
      m_spBrowser->Navigate(URL.AllocSysString(),NULL,NULL,NULL,NULL);
    }
  }
}

void 
TOCDlg::OnGo()
{
  m_AddrCombo.GetWindowText(m_href);
  if(m_spBrowser)
  {
    CString URL = m_base + m_href;
    m_spBrowser->Navigate(URL.AllocSysString(),NULL,NULL,NULL,NULL);
  }
}

void TOCDlg::OnBnClickedButtonOpen()
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
    CheckDocument(href);
    int pos = m_AddrCombo.FindString(-1,href);
    if(pos == CB_ERR)
    {
      pos = m_AddrCombo.AddString(href);
    }
    m_AddrCombo.SetCurSel(pos);
    OnGo();
  }
}

void
TOCDlg::OnEnChangeEditHypinfo()
{
  UpdateData(Controls2Data);
}

void
TOCDlg::OnCbnSelchangeWindowTarget()
{
  int ind = m_comboWindow.GetCurSel();
  if(ind >= 0)
  {
    CString target;
    m_comboWindow.GetLBText(ind,target);
    m_window = target;
    UpdateData(Data2Controls);
  }
}

void
TOCDlg::OnCbnSelchangeFrameTarget()
{
  int ind = m_comboFrame.GetCurSel();
  if(ind >= 0)
  {
    CString target;
    m_comboFrame.GetLBText(ind,target);
    m_target = Misc::GetAttributeValue("target",target);
    UpdateData(Data2Controls);
  }
}

void
TOCDlg::OnCbnSelchangeImagenumber()
{
  int ind = m_comboImage.GetCurSel();
  if(ind >= 0)
  {
    m_comboImage.GetLBText(ind,m_image);
    UpdateData(Data2Controls);
  }
}

void 
TOCDlg::OnCbnSelchangeBookmarks()
{
  int ind = m_comboBM.GetCurSel();
  if(ind >= 0)
  {
    m_comboBM.GetLBText(ind,m_bookmark);
    if(m_bookmark.IsEmpty())
    {
      m_spBrowser->Navigate(m_href.AllocSysString(),NULL,NULL,NULL,NULL);
    }
    else
    {
      ScrollIntoView(m_bookmark);
    }
  }
}

void 
TOCDlg::OnBnClickedButtonNewpage()
{
  // TODO: Add your control notification handler code here
}

void 
TOCDlg::OnBnClickedButtonFindpage()
{
  FindTopicDlg find(this,m_base);
  if(find.DoModal() == IDOK)
  {
    m_title = find.GetFoundTitle();
    m_href  = find.GetFoundTopic();
    UpdateData(Data2Controls);
    OnGo();
    UpdateProperties();
  }
}

void 
TOCDlg::OnOK() 
{
  UpdateData(Controls2Data);
  if(UpdateProperties())
  {
    EndDialog(IDOK);
  }
}

void 
TOCDlg::OnBnClickedHelpTOC()
{
  // TODO: Add your control notification handler code here
}
