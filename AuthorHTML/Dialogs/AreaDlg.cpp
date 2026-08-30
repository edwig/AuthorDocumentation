//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    AreaDlg.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog to edit the <AREA> tag to be able to make
//              links in an image map. The <AREA> tag is a child of the <MAP> tag
//
#include "stdafx.h"
#include "AuthorHTML.h"
#include "AreaDlg.h"
#include "GeneralIDDlg.h"
#include "TagEventsDlg.h"
#include "FileDialog.h"

// AreaDlg dialog

IMPLEMENT_DYNAMIC(AreaDlg, CDialog)

AreaDlg::AreaDlg(CWnd* pParent
                ,HtmlArea* area
                ,CString   base)
       	:CDialog(AreaDlg::IDD, pParent)
        ,m_area(area)
        ,m_base(base)
        ,m_doPopup(false)
        ,m_doBSSCpopup(false)
        ,m_doADHpopup(false)
        ,m_nohref(false)
        ,m_initDone(false)
{
}

AreaDlg::~AreaDlg()
{
}

void AreaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
  DDX_Text   (pDX,IDC_TITLE,  m_title);
  DDX_Control(pDX,IDC_BUTTON_OPEN,m_buttonOpen);
  DDX_Control(pDX,IDC_GO,     m_buttonGO);
  DDX_Control(pDX,IDC_POPUP,  m_buttonPopup);
  DDX_Control(pDX,IDC_NOHREF, m_buttonNoHref);
  DDX_Control(pDX,IDC_SHAPE,  m_comboShape);
  DDX_Text   (pDX,IDC_LEFT,   m_left);
  DDX_Text   (pDX,IDC_TOP,    m_top);
  DDX_Text   (pDX,IDC_RIGHT,  m_right);
  DDX_Text   (pDX,IDC_BOTTOM, m_bottom);
  DDX_Text   (pDX,IDC_POLYGON,m_polygon);
  DDX_Control(pDX,IDC_SPIN1,  m_spin1);
  DDX_Control(pDX,IDC_SPIN2,  m_spin2);
  DDX_Control(pDX,IDC_SPIN3,  m_spin3);
  DDX_Control(pDX,IDC_SPIN4,  m_spin4);
  DDX_Control(pDX,IDC_BOOKMARKS,m_comboBM);
  DDX_Control(pDX,IDC_ID,       m_buttonID);
  DDX_Control(pDX,IDC_EVENTS,   m_buttonEvents);
  DDX_Control(pDX,IDOK,         m_buttonOK);
  DDX_Control(pDX,IDCANCEL,     m_buttonCancel);
  DDX_Control(pDX,IDC_HELP_AREA,m_buttonHelp);

  if(pDX->m_bSaveAndValidate == Data2Controls)
  {
    CString shape = Misc::GetAttributeDisplayname("shape",m_shape);
    int ind = m_comboShape.FindString(-1,shape);
              m_comboShape.SetCurSel(ind);
    m_buttonNoHref.SetCheck(m_nohref);

    CString def;
    m_buttonEvents.GetWindowText(def);
    CString text = m_area->HasEvents() ? "[ &Events ]" : "&Events";
    if(def != text) m_buttonEvents.SetWindowText(text);

    // Only enable BM combo if mid-page bookmarks found
    m_comboBM.EnableWindow(m_comboBM.GetCount());

    m_buttonPopup.SetCheck(m_doPopup);
    m_buttonNoHref.SetCheck(m_nohref);

    if(m_shape.CompareNoCase("rectangle") == 0 ||
       m_shape.CompareNoCase("rect")      == 0 )
    {
      CWnd* w;
      w = GetDlgItem(IDC_STATIC1); w->SetWindowText("Left offset:"); 
      w = GetDlgItem(IDC_STATIC2); w->SetWindowText("Top offset:");
      w = GetDlgItem(IDC_STATIC3); w->SetWindowText("Right offset:");
      w = GetDlgItem(IDC_STATIC4); w->SetWindowText("Bottom offset:");
      w = GetDlgItem(IDC_LEFT);    w->EnableWindow(TRUE);
      w = GetDlgItem(IDC_RIGHT);   w->EnableWindow(TRUE);
      w = GetDlgItem(IDC_TOP);     w->EnableWindow(TRUE);
      w = GetDlgItem(IDC_BOTTOM);  w->EnableWindow(TRUE);
      w = GetDlgItem(IDC_POLYGON); w->EnableWindow(FALSE);
    }
    else if(m_shape.CompareNoCase("circle") == 0 ||
            m_shape.CompareNoCase("circ")   == 0)
    {
      CWnd* w;
      w = GetDlgItem(IDC_STATIC1); w->SetWindowText("Center X:");
      w = GetDlgItem(IDC_STATIC2); w->SetWindowText("Center Y:");
      w = GetDlgItem(IDC_STATIC3); w->SetWindowText("Radius:");
      w = GetDlgItem(IDC_STATIC4); w->SetWindowText("");
      w = GetDlgItem(IDC_LEFT);    w->EnableWindow(TRUE);
      w = GetDlgItem(IDC_RIGHT);   w->EnableWindow(TRUE);
      w = GetDlgItem(IDC_TOP);     w->EnableWindow(TRUE);
      w = GetDlgItem(IDC_BOTTOM);  w->EnableWindow(FALSE);
      w = GetDlgItem(IDC_POLYGON); w->EnableWindow(FALSE);
    }
    else // Polygon
    {
      CWnd* w;
      w = GetDlgItem(IDC_STATIC1); w->SetWindowText("Left offset:");
      w = GetDlgItem(IDC_STATIC2); w->SetWindowText("Top offset:");
      w = GetDlgItem(IDC_STATIC3); w->SetWindowText("Right offset:");
      w = GetDlgItem(IDC_STATIC4); w->SetWindowText("Bottom offset:");
      w = GetDlgItem(IDC_LEFT);    w->EnableWindow(FALSE);
      w = GetDlgItem(IDC_RIGHT);   w->EnableWindow(FALSE);
      w = GetDlgItem(IDC_TOP);     w->EnableWindow(FALSE);
      w = GetDlgItem(IDC_BOTTOM);  w->EnableWindow(FALSE);
      w = GetDlgItem(IDC_POLYGON); w->EnableWindow(TRUE);
    }
  }
}

BEGIN_MESSAGE_MAP(AreaDlg, CDialog)
  ON_CONTROL      (CBN_CLOSEUP,IDC_ADDRCOMBO,OnCloseup)
  ON_BN_CLICKED   (IDC_BUTTON_OPEN,OnButtonOpen)
  ON_BN_CLICKED   (IDC_GO,        OnButtonGo)
  ON_BN_CLICKED   (IDC_POPUP,     OnBnClickedPopup)
  ON_BN_CLICKED   (IDC_NOHREF,    OnBnClickedNohref)
  ON_EN_CHANGE    (IDC_TITLE,     OnEnChangeTitle)
  ON_CBN_SELCHANGE(IDC_SHAPE,     OnCbnSelchangeShape)
  ON_EN_CHANGE    (IDC_LEFT,      OnEnChangeLeft)
  ON_EN_CHANGE    (IDC_TOP,       OnEnChangeTop)
  ON_EN_CHANGE    (IDC_RIGHT,     OnEnChangeRight)
  ON_EN_CHANGE    (IDC_BOTTOM,    OnEnChangeBottom)
  ON_EN_CHANGE    (IDC_POLYGON,   OnEnChangePolygon)
  ON_CBN_SELCHANGE(IDC_BOOKMARKS, OnCbnSelchangeBookmarks)
  ON_BN_CLICKED   (IDC_ID,        OnBnClickedId)
  ON_BN_CLICKED   (IDC_EVENTS,    OnBnClickedEvents)
  ON_BN_CLICKED   (IDOK,          OnBnClickedOk)
  ON_BN_CLICKED   (IDCANCEL,      OnBnClickedCancel)
  ON_BN_CLICKED   (IDC_HELP_AREA, OnBnClickedHelpArea)
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(AreaDlg, CDialog)
  ON_EVENT(AreaDlg, IDC_EXPLORER, 0x00000103, OnDocumentComplete, VTS_DISPATCH VTS_VARIANT)
END_EVENTSINK_MAP()

BOOL
AreaDlg::OnInitDialog()
{
  CDialog::OnInitDialog();
  IUnknown *pUnk;

  CWnd *pWnd = GetDlgItem(IDC_EXPLORER);
  pUnk = NULL;
  if(pWnd)
  {
    m_spBrowser = pWnd->GetControlUnknown();
  }

  // Get attributes
  FillPage();

  // Fill page in browser
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
    if(m_AddrCombo.Create(WS_VSCROLL|WS_CHILD|WS_VISIBLE|CBS_DROPDOWN|CBS_AUTOHSCROLL
                         ,rcItem,this,IDC_ADDRCOMBO))
    {
      if(m_href.IsEmpty())
      {
        // Anchor could be for a #named anchor on same page
        m_AddrCombo.SetCurSel(-1);
        //if(m_spBrowser)
        //{
        //  m_spBrowser->Navigate(m_basePage.AllocSysString(),NULL,NULL,NULL,NULL);
        //}
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
  Misc::GetAllAttributeDisplaynames("shape",&all);
  for(unsigned int ind=0; ind<all.size(); ++ind)
  {
    m_comboShape.AddString(all[ind].c_str());
  }
  // Spins
  m_spin1.SetBase(10);
  m_spin2.SetBase(10);
  m_spin3.SetBase(10);
  m_spin4.SetBase(10);
  m_spin1.SetRange(0,32000);
  m_spin2.SetRange(0,32000);
  m_spin3.SetRange(0,32000);
  m_spin4.SetRange(0,32000);

  UpdateData(Data2Controls);
  m_initDone = true;
  return TRUE;
}

void
AreaDlg::FillPage()
{
  int pos;
  m_id     = m_area->GetAttribute("id");
  m_href   = m_area->GetProperty(HtmlArea::E_Href);
  m_shape  = m_area->GetProperty(HtmlArea::E_Shape);
  m_title  = m_area->GetProperty(HtmlArea::E_Title);
  m_nohref = atoi(m_area->GetProperty(HtmlArea::E_NoHref)) == 1;
  CString coords = m_area->GetProperty(HtmlArea::E_Coords);
  if(m_shape.CompareNoCase("polygon") == 0 ||
     m_shape.CompareNoCase("poly")    == 0 )
  {
    m_shape = "poly";
    m_polygon = coords;
  }
  else
  {
    pos = coords.Find(',');
    if(pos < 0) return;
    m_left = coords.Left(pos);
    coords = coords.Mid(pos+1);
    pos = coords.Find(',');
    if(pos < 0) return;
    m_top  = coords.Left(pos);
    coords = coords.Mid(pos+1);

    if(m_shape.CompareNoCase("rectangle") == 0 ||
       m_shape.CompareNoCase("rect")      == 0 )
    {
      pos = coords.Find(',');
      if(pos < 0) return;
      m_right = coords.Left(pos);
      m_bottom = coords.Mid(pos+1);
    }
    else // circle
    {
      m_right = coords;
    }
  }
  pos = m_href.Find('#');
  if(pos >= 0)
  {
    m_bookmark = m_href.Mid(pos + 1);
    m_href     = m_href.Left(pos);
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
    int pos2 = m_href.Find('\'');
    if(pos2 > 0)
    {
      m_href = m_href.Left(pos2);
    }
  }
}

void
AreaDlg::UpdateProperties()
{
  CString nohref;
  nohref.Format("%d",m_nohref);

  CString href;
  if(m_doPopup)
  {
    if(m_doADHpopup || m_doBSSCpopup == false)
    {
      href = "javascript:ADHShowPopup('" + m_href + "');";
      m_doADHpopup = true;
    }
    if(m_doBSSCpopup)
    {
      href = "javascript:BSSCPopup('" + m_href + "');";
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
  m_area->SetProperty(HtmlArea::E_Href,href);
  m_area->SetProperty(HtmlArea::E_Shape,m_shape);
  m_area->SetProperty(HtmlArea::E_NoHref,nohref);
  m_area->SetProperty(HtmlArea::E_Title,m_title);

  if(m_shape.CompareNoCase("poly") == 0)
  {
    m_area->SetProperty(HtmlArea::E_Coords,m_polygon);
  }
  else if(m_shape.CompareNoCase("rect") == 0)
  {
    CString coords = m_left + "," + m_top + "," + m_right + "," + m_bottom;
    m_area->SetProperty(HtmlArea::E_Coords,coords);
  }
  else  // Circle
  {
    CString coords = m_left + "," + m_top + "," + m_right; // x1,y1,radius!!
    m_area->SetProperty(HtmlArea::E_Coords,coords);
  }
}

void 
AreaDlg::OnDocumentComplete(LPDISPATCH /*pDisp*/, LPVARIANT /*pURL*/)
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
AreaDlg::ScrollIntoView(CString bookmark)
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

// AreaDlg message handlers

void 
AreaDlg::OnCloseup()
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
AreaDlg::OnButtonOpen()
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
    int pos = m_AddrCombo.FindString(-1,href);
    if(pos == CB_ERR)
    {
      pos = m_AddrCombo.AddString(href);
    }
    m_AddrCombo.SetCurSel(pos);
    OnButtonGo();
  }
}

void 
AreaDlg::OnButtonGo()
{
  m_AddrCombo.GetWindowText(m_href);
  if(m_spBrowser)
  {
    CString URL = m_base + m_href;
    m_spBrowser->Navigate(URL.AllocSysString(),NULL,NULL,NULL,NULL);
  }
}

void 
AreaDlg::OnBnClickedPopup()
{
  m_doPopup = (m_buttonPopup.GetCheck() == TRUE);
}

void 
AreaDlg::OnBnClickedNohref()
{
  m_nohref = (m_buttonNoHref.GetCheck() == TRUE);
}

void 
AreaDlg::OnEnChangeTitle()
{
  if(m_initDone)
  {
    UpdateData();
  }
}

void 
AreaDlg::OnCbnSelchangeShape()
{
  int ind = m_comboShape.GetCurSel();
  if(ind >= 0)
  {
    CString shape;
    m_comboShape.GetLBText(ind,shape);
    m_shape = Misc::GetAttributeValue("shape",shape);
  }
  UpdateData(Data2Controls);
}

void 
AreaDlg::OnEnChangeLeft()
{
  if(m_initDone)
  {
    UpdateData();
  }
}

void 
AreaDlg::OnEnChangeTop()
{
  if(m_initDone)
  {
    UpdateData();
  }
}

void 
AreaDlg::OnEnChangeRight()
{
  if(m_initDone)
  {
    UpdateData();
  }
}

void 
AreaDlg::OnEnChangeBottom()
{
  if(m_initDone)
  {
    UpdateData();
  }
}

void 
AreaDlg::OnEnChangePolygon()
{
  if(m_initDone)
  {
    UpdateData();
  }
}

void AreaDlg::OnCbnSelchangeBookmarks()
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
AreaDlg::OnBnClickedId()
{
  HtmlElement* elem = (HtmlElement*)m_area;
  GeneralIDDlg dlg(this,"area",elem);
  dlg.DoModal();
}

void 
AreaDlg::OnBnClickedEvents()
{
  HtmlElement* elem = (HtmlElement*)m_area;
  TagEventsDlg dlg(this,elem,"area");
  dlg.DoModal();
  UpdateData(Data2Controls);
}

void 
AreaDlg::OnBnClickedOk()
{
  UpdateProperties();
  OnOK();
}

void 
AreaDlg::OnBnClickedCancel()
{
  OnCancel();
}

void
AreaDlg::OnBnClickedHelpArea()
{
  // TODO: Add your control notification handler code here
}


