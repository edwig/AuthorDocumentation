//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    AnchorDlg.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog to edit the <A> anchor with bookmark, so we can link to it
//
#include "stdafx.h"
#include "AuthorHTML.h"
#include "AnchorDlg.h"
#include "FileDialog.h"
#include "TableProperties.h"
#include "GeneralIDDlg.h"
#include "TagEventsDlg.h"
#include "StyleSheetDlg.h"
#include <vector>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// AnchorDlg dialog


AnchorDlg::AnchorDlg(CWnd* pParent
                    ,CString base
                    ,CString basePage
                    ,HtmlAnchor* elem
                    ,int unique)
          :CDialog(AnchorDlg::IDD, pParent)
          ,m_base(base)
          ,m_basePage(basePage)
          ,m_elem(elem)
          ,m_unique(unique)
          ,m_doPopup(false)
          ,m_doBSSCpopup(false)
          ,m_doADHpopup(false)
{
}

AnchorDlg::~AnchorDlg()
{
}

void AnchorDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  if(::IsWindow(m_AddrCombo.m_hWnd))
  {
    DDX_Text(pDX, IDC_ADDRCOMBO, m_href);
  }
  DDX_Text   (pDX,IDC_EDIT_HYPINFO,    m_title);
  DDX_Control(pDX,IDC_FRAME_TARGET,    m_targetCombo);
  DDX_Control(pDX,IDC_HYPERLINK_FRAME, m_buttonFrame);
  DDX_Control(pDX,IDC_HYPERLINK_POPUP, m_buttonPopUp);
  DDX_Control(pDX,IDC_HYPERLINK_REL,   m_relCombo);
  DDX_Control(pDX,IDC_HYPERLINK_REV,   m_revCombo);
  DDX_Control(pDX,IDC_BUTTON_OPEN,     m_buttonOpen);
  DDX_Control(pDX,IDC_BTNGO,           m_buttonGo);
  DDX_Control(pDX,IDC_BOOKMARKS,       m_comboBM);
  DDX_Control(pDX,IDC_ID,              m_buttonID);
  DDX_Control(pDX,IDC_EVENTS,          m_buttonEvents);
  DDX_Control(pDX,IDC_STYLE,           m_buttonStyle);
  DDX_Control(pDX,IDOK,                m_buttonOK);
  DDX_Control(pDX,IDCANCEL,            m_buttonCancel);
  DDX_Control(pDX,ID_HELP_URL,         m_buttonHelp);

  if(pDX->m_bSaveAndValidate == Data2Controls)
  {
    int ind;
    CString desc = Misc::GetAttributeDisplayname("target",m_target);
    ind = m_targetCombo.FindString(-1,desc);
          m_targetCombo.SetCurSel(ind);
    ind = m_relCombo.FindString(-1,m_rel);
          m_relCombo.SetCurSel(ind);
    ind = m_revCombo.FindString(-1,m_rev);
          m_revCombo.SetCurSel(ind);
    ind = m_comboBM.FindString(-1,m_bookmark);
          m_comboBM.SetCurSel(ind);

    if(m_elem)
    {
      CString def,text;

      m_buttonID.GetWindowText(def);
      text = m_elem->HasIdentity() ? "[ &ID ]" : "&ID";
      if(def != text) m_buttonID.SetWindowText(text);

      m_buttonStyle.GetWindowText(def);
      text = m_elem->HasStyle() ? "[ &Style ]" : "&Style";
      if(def != text) m_buttonStyle.SetWindowText(text);

      m_buttonEvents.GetWindowText(def);
      text = m_elem->HasEvents() ? "[ &Events ]" : "&Events";
      if(def != text) m_buttonEvents.SetWindowText(text);
    }
    else
    {
      // Newly defined tag. No style possible yet.
      m_buttonStyle .EnableWindow(FALSE);
      m_buttonEvents.EnableWindow(FALSE);
      m_buttonID    .EnableWindow(FALSE);
    }

    // Only enable BM combo if mid-page bookmarks found
    m_comboBM.EnableWindow(m_comboBM.GetCount());

    if(m_doPopup)
    {
      m_target = "";
      m_targetCombo.SetCurSel(-1);
      m_targetCombo.EnableWindow(FALSE);
    }
    else
    {
      m_targetCombo.EnableWindow(TRUE);
    }
    m_buttonFrame.SetCheck(m_doPopup == false);
    m_buttonPopUp.SetCheck(m_doPopup == true);
  }
}

BEGIN_MESSAGE_MAP(AnchorDlg, CDialog)
  ON_CONTROL      (CBN_CLOSEUP,IDC_ADDRCOMBO,OnCloseup)
  ON_CONTROL      (BN_CLICKED,IDC_BTNGO,     OnGo)
  ON_BN_CLICKED   (IDC_BUTTON_OPEN,    OnBnClickedButtonOpen)
  ON_EN_KILLFOCUS (IDC_EDIT_HYPINFO,   OnEnChangeEditHypinfo)
  ON_CBN_SELCHANGE(IDC_FRAME_TARGET,   OnCbnSelchangeFrameTarget)
  ON_BN_CLICKED   (IDC_HYPERLINK_FRAME,OnBnClickedRadioFrame)
  ON_BN_CLICKED   (IDC_HYPERLINK_POPUP,OnBnClickedRadioPopup)
  ON_CBN_SELCHANGE(IDC_HYPERLINK_REL,  OnCbnSelChangeRel)
  ON_CBN_SELCHANGE(IDC_HYPERLINK_REV,  OnCbnSelChangeRev)
  ON_CBN_KILLFOCUS(IDC_HYPERLINK_REL,  OnCbnSelChangeRel)
  ON_CBN_KILLFOCUS(IDC_HYPERLINK_REV,  OnCbnSelChangeRev)
  ON_CBN_SELCHANGE(IDC_BOOKMARKS,      OnCbnSelchangeBookmarks)
  ON_BN_CLICKED   (IDC_ID,             OnBnClickedId)
  ON_BN_CLICKED   (IDC_EVENTS,         OnBnClickedEvents)
  ON_BN_CLICKED   (IDC_STYLE,          OnBnClickedStyle)
  ON_BN_CLICKED   (ID_HELP_URL,        OnBnClickedHelpUrl)
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(AnchorDlg, CDialog)
  ON_EVENT(AnchorDlg, IDC_EXPLORER, 0x00000103, OnDocumentComplete, VTS_DISPATCH VTS_VARIANT)
END_EVENTSINK_MAP()

BOOL 
AnchorDlg::OnInitDialog() 
{
  CDialog::OnInitDialog();

  CWnd *pWnd = GetDlgItem(IDC_EXPLORER);
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
        // Anchor could be for a #named anchor on same page
        m_AddrCombo.SetCurSel(-1);
        if(m_spBrowser)
        {
          m_spBrowser->Navigate(m_basePage.AllocSysString(),NULL,NULL,NULL,NULL);
        }
      }
      else
      {
        // Anchor is not empty
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
    m_targetCombo.AddString(all[ind].c_str());
  }
  m_buttonFrame.SetCheck(TRUE);
  UpdateData(Data2Controls);
  return TRUE;
}

void
AnchorDlg::FillPage()
{
  m_href   = m_elem->GetProperty(HtmlAnchor::E_Href);
  m_target = m_elem->GetProperty(HtmlAnchor::E_Target);
  m_title  = m_elem->GetProperty(HtmlAnchor::E_Title);
  m_rel    = m_elem->GetProperty(HtmlAnchor::E_Rel);
  m_rev    = m_elem->GetProperty(HtmlAnchor::E_Rev);

  int pos = m_href.Find('#');
  if(pos >= 0)
  {
    m_bookmark = m_href.Mid(pos + 1);
    m_href     = m_href.Left(pos);
  }
  CString cID = m_elem->GetAttribute("id");
  if(!cID.IsEmpty())
  {
    for(int num=0;num < cID.GetLength(); ++num)
    {
      if(isdigit(cID.GetAt(num)))
      {
        m_unique = atoi(&((cID.GetString())[num]));
        break;
      }
    }
  }
  if(m_href.Find("javascript") >= 0)
  {
    m_doPopup = true;
    m_href = m_href.TrimLeft("javascript:");
    if(m_href.Left(12) == "ADHShowPopup")
    {
      m_doADHpopup = true;
      m_href = m_href.Mid(14);
    }
    if(m_href.Left(9) == "BSSCPopup")
    {
      m_doBSSCpopup = true;
      m_href = m_href.Mid(11);
    }
    m_href = m_href.TrimLeft("'");
    int pos2 = m_href.Find('\'');
    if(pos2 > 0)
    {
      m_href = m_href.Left(pos2);
    }
  }
}

void 
AnchorDlg::UpdateProperties()
{
  CString href;
  if(m_doPopup)
  {
    CString id;
    id.Format("a%d",m_unique);
    m_elem->SetAttribute("id",id);

    if(m_doPopup)
    {
      if(m_doADHpopup || m_doBSSCpopup == false)
      {
        href = "javascript:ADHShowPopup('" + m_href + "'," + id + ");";
        m_doADHpopup = true;
      }
      if(m_doBSSCpopup)
      {
        href = "javascript:BSSCPopup('" + m_href + "'," + id + ");";
      }
    }
  }
  else
  {
    href = m_href;
    if(!m_bookmark.IsEmpty())
    {
      href += "#";
      href += m_bookmark;
    }
  }
  m_elem->SetProperty(HtmlAnchor::E_Href,  href);
  m_elem->SetProperty(HtmlAnchor::E_Target,m_target);
  m_elem->SetProperty(HtmlAnchor::E_Title, m_title);
  m_elem->SetProperty(HtmlAnchor::E_Rel,   m_rel);
  m_elem->SetProperty(HtmlAnchor::E_Rev,   m_rev);
}

/////////////////////////////////////////////////////////////////////////////
// AnchorDlg message handlers

void 
AnchorDlg::OnDocumentComplete(LPDISPATCH /*pDisp*/, LPVARIANT /*pURL*/)
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
          CComVariant value;
          V_VT(&value) = VT_BSTR;
          CComQIPtr<IHTMLElement,&IID_IHTMLElement> elem = disp2;
          elem->getAttribute(attrib,0,&value);
          CString anchorName = V_BSTR(&value);
          if(!anchorName.IsEmpty())
          {
            m_comboBM.AddString(anchorName);
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
AnchorDlg::ScrollIntoView(CString bookmark)
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
        CComVariant value;
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
AnchorDlg::OnCloseup()
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
AnchorDlg::OnGo()
{
  m_AddrCombo.GetWindowText(m_href);
  if(m_spBrowser)
  {
    CString URL = m_base + m_href;
    m_spBrowser->Navigate(URL.AllocSysString(),NULL,NULL,NULL,NULL);
  }
}

void AnchorDlg::OnBnClickedButtonOpen()
{
  DocFileDialog diag(true
                    ,"Search for a page to link to"
                    ,"htm"
                    ,""
                    ,0);
  if(diag.DoModal() == IDOK)
  {
    CString relative;
    m_href = diag.GetChosenFile();
    if(Misc::MakeRelativePathname(m_base,m_href,relative))
    {
      m_href = relative;
    }
    int pos = m_AddrCombo.FindString(-1,m_href);
    if(pos == CB_ERR)
    {
      pos = m_AddrCombo.AddString(m_href);
    }
    m_AddrCombo.SetCurSel(pos);
    OnGo();
  }
}

void
AnchorDlg::OnEnChangeEditHypinfo()
{
  UpdateData(Controls2Data);
}

void
AnchorDlg::OnCbnSelchangeFrameTarget()
{
  int ind = m_targetCombo.GetCurSel();
  if(ind >= 0)
  {
    CString target;
    m_targetCombo.GetLBText(ind,target);
    m_target = Misc::GetAttributeValue("target",target);
  }
}

void 
AnchorDlg::OnBnClickedRadioFrame()
{
  bool frame = m_buttonFrame.GetCheck() == TRUE;
  m_doPopup = !frame;
  UpdateData(Data2Controls);
}

void
AnchorDlg::OnBnClickedRadioPopup()
{
  m_doPopup = m_buttonPopUp.GetCheck() == TRUE;
  UpdateData(Data2Controls);
}

void 
AnchorDlg::OnCbnSelChangeRel()
{
  int ind = m_relCombo.GetCurSel();
  if(ind >= 0)
  {
    m_relCombo.GetLBText(ind,m_rel);
  }
}

void
AnchorDlg::OnCbnSelChangeRev()
{
  int ind = m_revCombo.GetCurSel();
  if(ind >= 0)
  {
    m_revCombo.GetLBText(ind,m_rev);
  }
}

void 
AnchorDlg::OnCbnSelchangeBookmarks()
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
AnchorDlg::OnBnClickedId()
{
  GeneralIDDlg dlg(this,"a",m_elem);
  dlg.DoModal();
}

void 
AnchorDlg::OnBnClickedEvents()
{
  TagEventsDlg dlg(this,m_elem,"A");
  dlg.DoModal();
  UpdateData(Data2Controls);
}

void 
AnchorDlg::OnBnClickedStyle()
{
  if(!m_elem->HasStyle())
  {
    m_elem->SetStyle();
  }
  CString style = m_elem->GetInlineStyle();
  style = CString("a { ") + style + "}";
  StyleSheetDlg dlg(this,m_base,"a",NULL,style);
  if(dlg.DoModal() == IDOK)
  {
    style = dlg.GetInlineStylesheet();
    style.TrimRight("}");
    style.TrimLeft("a {");
    m_elem->SetInlineStyle(style);
  }
}

void 
AnchorDlg::OnOK() 
{
  UpdateData(Controls2Data);
  UpdateProperties();
  EndDialog(IDOK);
}

void 
AnchorDlg::OnBnClickedHelpUrl()
{
  // TODO: Add your control notification handler code here
}
