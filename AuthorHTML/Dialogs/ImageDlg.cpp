//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    ImageDlg.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for managing the <IMG> tag in HTML,
//
#include "stdafx.h"
#include "AuthorHTML.h"
#include "ImageDlg.h"
#include "FileDialog.h"
#include "TableProperties.h"
#include "GeneralIDDlg.h"
#include "StyleSheetDlg.h"
#include "TagEventsDlg.h"
#include <ExDispid.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ImageDlg dialog


ImageDlg::ImageDlg(CWnd*    pParent
                  ,HtmlImg* img
                  ,CString  base)
         :CDialog(ImageDlg::IDD, pParent)
         ,m_img(img)
         ,m_base(base)
         ,m_initDone(false)
         ,m_ratio(1.0)
{
}

void ImageDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  // Source
  DDX_Control(pDX,IDC_IMAGECOMBO,      m_comboSrc);
  DDX_Control(pDX,IDC_IMG_VIDEO,       m_comboDynSrc);
  DDX_Control(pDX,IDC_BUTTON_OPEN,     m_buttonSrc);
  DDX_Control(pDX,IDC_BUTTON_VIDEO,    m_buttonDynSrc);
  DDX_Control(pDX,IDC_IMG_LOWRES,      m_comboLowSrc);
  DDX_Control(pDX,IDC_BUTTON_LOWRES,   m_buttonLowSrc);
  // Attributes
  DDX_Text   (pDX,IDC_EDIT_IMGINFO,    m_title);
  DDX_Control(pDX,IDC_KEEPRATIO,       m_buttonRatio); 
  DDX_Text   (pDX,IDC_IMG_WIDTH,       m_width);
  DDX_Text   (pDX,IDC_IMG_HEIGHT,      m_height);
  DDX_Control(pDX,IDC_SPIN1,           m_spinWidth);
  DDX_Control(pDX,IDC_SPIN2,           m_spinHeight);
  DDX_Text   (pDX,IDC_IMG_HPAD,        m_hpad);
  DDX_Text   (pDX,IDC_IMG_VPAD,        m_vpad);
  DDX_Control(pDX,IDC_SPIN_HPAD,       m_spinHpad);
  DDX_Control(pDX,IDC_SPIN_VPAD,       m_spinVpad);
  DDX_Control(pDX,IDC_ALIGNMENT,       m_comboAlign);
  DDX_Text   (pDX,IDC_IMG_BORDERWIDTH, m_border);
  DDX_Control(pDX,IDC_SPIN_BORDER,     m_spinBorder);
  // Standard controls
  DDX_Control(pDX,IDC_RESET_IMAGE,     m_buttonReset);
  DDX_Control(pDX,IDC_ID,              m_buttonID);
  DDX_Control(pDX,IDC_EVENTS,          m_buttonEvents);
  DDX_Control(pDX,IDC_STYLE,           m_buttonStyle);
  DDX_Control(pDX,IDOK,                m_buttonOK);
  DDX_Control(pDX,IDCANCEL,            m_buttonCancel);
  DDX_Control(pDX,ID_HELP_IMAGE,       m_buttonHelp);
  DDX_Control(pDX,IDC_IMG_STARTVIDEO,  m_comboStart);
  DDX_Control(pDX,IDC_IMG_LOOP,        m_comboLoop);

  if(pDX->m_bSaveAndValidate == Data2Controls)
  {
    CString def,text;

    m_buttonID.GetWindowText(def);
    text = m_img->HasIdentity() ? "[ &ID ]" : "&ID";
    if(def != text) m_buttonID.SetWindowText(text);

    m_buttonStyle.GetWindowText(def);
    text = m_img->HasStyle() ? "[ &Style ]" : "&Style";
    if(def != text) m_buttonStyle.SetWindowText(text);

    m_buttonEvents.GetWindowText(def);
    text = m_img->HasEvents() ? "[ &Events ]" : "&Events";
    if(def != text) m_buttonEvents.SetWindowText(text);

    m_comboSrc    .EnableWindow(TRUE);
    m_buttonSrc   .EnableWindow(TRUE);
    m_comboDynSrc .EnableWindow(TRUE);
    m_buttonDynSrc.EnableWindow(TRUE);
    m_comboStart  .EnableWindow(TRUE);
    m_comboLoop   .EnableWindow(TRUE);
    if(!m_src.IsEmpty())
    {
      m_comboDynSrc .EnableWindow(FALSE);
      m_buttonDynSrc.EnableWindow(FALSE);
      m_comboStart  .EnableWindow(FALSE);
      m_comboLoop   .EnableWindow(FALSE);
    }
    else if(!m_dynsrc.IsEmpty())
    {
      m_comboSrc .EnableWindow(FALSE);
      m_buttonSrc.EnableWindow(FALSE);
    }
    int ind = m_comboAlign.FindString(-1,m_align);
    m_comboAlign.SetCurSel(ind);
    ind = m_comboStart.FindString(-1,m_start);
    m_comboStart.SetCurSel(ind);
    if(m_loop.CompareNoCase("Infinite"))
    {
      m_comboLoop.SetCurSel(0);
    }
  }
}

BEGIN_MESSAGE_MAP(ImageDlg, CDialog)
  // Source
  ON_CBN_CLOSEUP  (IDC_IMAGECOMBO,      OnCloseup)
  ON_CBN_KILLFOCUS(IDC_IMAGECOMBO,      OnCloseup)
  ON_CBN_CLOSEUP  (IDC_IMG_VIDEO,       OnCbnSelchangeImgVideo)
  ON_CBN_KILLFOCUS(IDC_IMG_VIDEO,       OnCbnSelchangeImgVideo)
  ON_CBN_CLOSEUP  (IDC_IMG_LOWRES,      OnCbnSelchangeImgLowres)
  ON_CBN_KILLFOCUS(IDC_IMG_LOWRES,      OnCbnSelchangeImgLowres)
  ON_BN_CLICKED   (IDC_BUTTON_OPEN,     OnBnClickedButtonOpen)
  ON_BN_CLICKED   (IDC_BUTTON_VIDEO,    OnBnClickedButtonVideo)
  ON_BN_CLICKED   (IDC_BUTTON_LOWRES,   OnBnClickedButtonLowres)
  // Hyperlink options
  ON_EN_KILLFOCUS (IDC_EDIT_IMGINFO,    OnEnChangeEditHypinfo)
  ON_BN_CLICKED   (IDC_KEEPRATIO,       OnBnClickedRatio)
  ON_EN_KILLFOCUS (IDC_IMG_WIDTH,       OnEnChangeImgWidth)
  ON_EN_KILLFOCUS (IDC_IMG_HEIGHT,      OnEnChangeImgHeight)
  ON_EN_KILLFOCUS (IDC_IMG_HPAD,        OnEnChangeImgHpad)
  ON_EN_KILLFOCUS (IDC_IMG_VPAD,        OnEnChangeImgVpad)
  ON_BN_CLICKED   (IDC_RESET_IMAGE,     OnBnClickedResetImage)
  ON_CBN_SELCHANGE(IDC_ALIGNMENT,       OnCbnSelchangeAlignment)
  ON_EN_KILLFOCUS (IDC_IMG_BORDERWIDTH, OnEnChangeImgBorderwidth)
  ON_CBN_SELCHANGE(IDC_IMG_STARTVIDEO,  OnCbnSelchangeImgStartvideo)
  ON_CBN_SELCHANGE(IDC_IMG_LOOP,        OnCbnSelchangeImgLoop)
  // Standard controls
  ON_BN_CLICKED   (IDC_ID,              OnBnClickedId)
  ON_BN_CLICKED   (IDC_EVENTS,          OnBnClickedEvents)
  ON_BN_CLICKED   (IDC_STYLE,           OnBnClickedStyle)
  ON_BN_CLICKED   (IDOK,                OnBnClickedOk)
  ON_BN_CLICKED   (IDCANCEL,            OnBnClickedCancel)
  ON_BN_CLICKED   (ID_HELP_IMAGE,       OnBnClickedHelpImage)
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(ImageDlg, CDialog)
  ON_EVENT(ImageDlg, IDC_EXPLORER, DISPID_DOCUMENTCOMPLETE, OnDocumentComplete, VTS_DISPATCH VTS_VARIANT)
END_EVENTSINK_MAP()

BOOL ImageDlg::OnInitDialog() 
{
  CDialog::OnInitDialog();

  CWnd *pWnd = GetDlgItem(IDC_EXPLORER);
  if(pWnd)
  {
    m_spBrowser = pWnd->GetControlUnknown();
  }
  pWnd = NULL;

  // Get all attributes
  FillPage();

  // Load image page for display of an image
  m_spBrowser->Navigate(L"res://authorhtml.exe/image.html", NULL, NULL, NULL, NULL);

  // Path to the image
  if(!m_src.IsEmpty())
  {
    m_comboSrc.AddString(m_src);
    m_comboSrc.SetCurSel(0);
  }
  // Path to the video
  if(!m_dynsrc.IsEmpty())
  {
    m_comboDynSrc.AddString(m_dynsrc);
    m_comboDynSrc.SetCurSel(0);
  }
  if(!m_lowsrc.IsEmpty())
  {
    m_comboLowSrc.AddString(m_lowsrc);
    m_comboLowSrc.SetCurSel(0);
  }
  m_buttonRatio.SetCheck(TRUE);
  CWnd* h = GetDlgItem(IDC_IMG_HEIGHT);
  h->EnableWindow(FALSE);

  m_spinWidth .SetBase(10);
  m_spinHeight.SetBase(10);
  m_spinHpad  .SetBase(10);
  m_spinVpad  .SetBase(10);
  m_spinWidth .SetRange(0,32000);
  m_spinHeight.SetRange(0,32000);
  m_spinHpad  .SetRange(0,32000);
  m_spinVpad  .SetRange(0,32000);
  m_spinWidth .SetPos(atoi(m_width));
  m_spinHeight.SetPos(atoi(m_height));
  m_spinBorder.SetBase(10);
  m_spinBorder.SetRange(0,32000);
  m_spinBorder.SetPos(atoi(m_border));

  UpdateData(Data2Controls);
  m_initDone = true;
  return TRUE;
}


void
ImageDlg::FillPage()
{
  m_src    = m_img->GetSrc();
  m_lowsrc = m_img->GetLowSrc();
  m_dynsrc = m_img->GetDynSrc();
  m_title  = m_img->GetAlt();
  m_start  = m_img->GetStart();
  m_loop   = m_img->GetLoop();
  m_width  = m_img->GetProperty(HtmlImg::E_Width);
  m_height = m_img->GetProperty(HtmlImg::E_Height);
  m_align  = m_img->GetProperty(HtmlImg::E_Align);
  m_border = m_img->GetProperty(HtmlImg::E_Border);
  m_hpad   = m_img->GetProperty(HtmlImg::E_HorPad);
  m_vpad   = m_img->GetProperty(HtmlImg::E_VerPad);

  CssSplitValueUnits(m_width, m_width, m_wunits);
  CssSplitValueUnits(m_height,m_height,m_hunits);
  CssSplitValueUnits(m_hpad,  m_hpad,  m_hpunits);
  CssSplitValueUnits(m_vpad,  m_vpad,  m_vpunits);
  CssSplitValueUnits(m_border,m_border,m_bunits);
  if(atoi(m_loop) == -1)
  {
    m_loop = "Infinite";
  }
}

void
ImageDlg::UpdateProperties()
{
  CString looping(m_loop);
  if(looping.CompareNoCase("Infinite") == 0)
  {
    looping = "-1";
  }
  m_img->SetSrc(m_src);
  m_img->SetDynSrc(m_dynsrc);
  m_img->SetLowSrc(m_lowsrc);
  m_img->SetAlt(m_title);
  m_img->SetStart(m_start);
  m_img->SetLoop(looping);
  m_img->SetProperty(HtmlImg::E_Width,  m_width  + m_wunits);
  m_img->SetProperty(HtmlImg::E_Height, m_height + m_hunits);
  m_img->SetProperty(HtmlImg::E_Border, m_border + m_bunits);
  m_img->SetProperty(HtmlImg::E_Align,  m_align);
  m_img->SetProperty(HtmlImg::E_HorPad, m_hpad + m_hpunits);
  m_img->SetProperty(HtmlImg::E_VerPad, m_vpad + m_vpunits);
}

#pragma warning (disable:4244) // Double to int conversion
void
ImageDlg::ReCalculateRatio()
{
  double cur_ratio = (double)(atoi(m_height)) / (double)(atoi(m_width));
  if(abs((double)(cur_ratio - m_ratio)) < (double)0.1)
  {
    m_buttonRatio.SetCheck(TRUE);
  }
  else
  {
    m_buttonRatio.SetCheck(FALSE);
  }
  OnBnClickedRatio();
}

/////////////////////////////////////////////////////////////////////////////
// ImageDlg message handlers

void ImageDlg::OnDocumentComplete(LPDISPATCH /*pDisp*/, LPVARIANT /*pURL*/)
{
  HRESULT hr = S_FALSE;
  CComPtr<IDispatch> disp;
  CComPtr<IHTMLElementCollection> coll;
  if(!m_doc.p)
  {
    m_spBrowser->get_Document(&disp);
    disp->QueryInterface(IID_IHTMLDocument2,(void**)&m_doc);
  }
  // Place control in editmode so we can change the image
  CComBSTR command = L"IDM_EDITMODE";
  VARIANT_BOOL gelukt;
  VARIANT val;
  V_VT(&val) = VT_BSTR;
  m_doc->execCommand(command,VARIANT_FALSE,val,&gelukt);

  // Get a collection of all DomNodes and iterate over it
  hr = m_doc->get_all(&coll);
  long count = 0;
  coll->get_length(&count);
  for(int ind = 0; ind < count; ++ ind)
  {
    CComVariant findIndex;
    CComPtr<IDispatch> dispImg;
    V_VT(&findIndex) = VT_I4;
    V_I4(&findIndex) = ind;
    hr = coll->item(findIndex,findIndex,&dispImg);
    CComQIPtr<IHTMLImgElement,&IID_IHTMLImgElement> image = dispImg;
    if(image.p)
    {
      // This is the first image
      // First reset the source with an empty string
      // If we don't do this first, the height/width properties will not
      // be reset and will be of the previous image!!
      CComBSTR theSource;
      image->put_src(theSource);

      // Now put our picture on the control
      // Construct an absolute pathname as this HTML page is in the resources!!
      if(!m_src.IsEmpty())
      {
        theSource = m_base + m_src;
        hr = image->put_src(theSource);
      }
      else if(!m_dynsrc.IsEmpty())
      {
        theSource = m_base + m_dynsrc;
        hr = image->put_dynsrc(theSource);
      }
      // Now find the actual width/height of the image
      if(m_docWidth.IsEmpty() || m_docHeight.IsEmpty())
      {
        long height = 0;
        long width  = 0;
        image->get_width(&width);
        image->get_height(&height);
        if(width)
        {
          m_docWidth.Format("%d",width);
          if(m_width.IsEmpty() || m_width == "0")
          {
            m_width  = m_docWidth;
          }
        }
        if(height)
        {
          m_docHeight.Format("%d",height);
          if(m_height.IsEmpty() || m_height == "0")
          {
            m_height = m_docHeight;
          }
        }
        if(width != 0)
        {
          m_ratio = (double)height / (double)width;
          ReCalculateRatio();
        }
      }
      // Reset the size on the image
      image->put_width (atoi(m_width));
      image->put_height(atoi(m_height));
    }
  }
}

void 
ImageDlg::OnCloseup()
{	
  CString src;
  int nSel = m_comboSrc.GetCurSel();
  if(CB_ERR != nSel)
  {
    m_comboSrc.GetLBText(nSel,src);
  }
  else
  {
    m_comboSrc.GetWindowText(src);
  }
  if(src != m_src)
  {
    if(!src.IsEmpty())
    {
      m_dynsrc = "";
      int ind = m_comboSrc.AddString(src);
      m_comboSrc.SetCurSel(ind);
    }
    m_src = src;
    // Clear widht/height for measurement
    m_docWidth  = 
    m_docHeight = "";
  }
  if(m_spBrowser)
  {
    OnDocumentComplete(NULL,NULL);
  }
}

void 
ImageDlg::OnCbnSelchangeImgVideo()
{
  int nSel = m_comboDynSrc.GetCurSel();
  if(CB_ERR != nSel)
  {
    m_comboDynSrc.GetLBText(nSel,m_dynsrc);
  }
  else
  {
    CString src;
    m_comboDynSrc.GetWindowText(src);
    if(src != m_dynsrc)
    {
      if(!src.IsEmpty())
      {
        m_src = "";
        int ind = m_comboDynSrc.AddString(src);
        m_comboDynSrc.SetCurSel(ind);
      }
      m_dynsrc = src;
      // Clear widht/height for measurement
      m_docWidth  = 
      m_docHeight = "";
    }
  }
  if(m_spBrowser)
  {
    OnDocumentComplete(NULL,NULL);
  }
}

void 
ImageDlg::OnCbnSelchangeImgLowres()
{
  int nSel = m_comboLowSrc.GetCurSel();
  if(CB_ERR != nSel)
  {
    m_comboLowSrc.GetLBText(nSel,m_lowsrc);
  }
  else
  {
    CString src;
    m_comboLowSrc.GetWindowText(src);
    if(src != m_lowsrc)
    {
      m_comboLowSrc.AddString(src);
      m_lowsrc = src;
    }
  }
  if(m_spBrowser)
  {
    OnDocumentComplete(NULL,NULL);
  }
}

void 
ImageDlg::OnBnClickedButtonOpen()
{
  DocFileDialog diag(true               // true = open
                    ,"Select an image"  // title
                    ,""                 // Extension
                    ,""                 // Default file
                    ,0                  // flags
                    ,"All images (jpg,gif,bmp,png)|*.jpg;*.jpeg;*.gif;*.bmp|"
                     "Joint Photogroup files (jpg)|*.jpg;*.jpeg|"
                     "Graphics Information File (gif)|*.gif|"
                     "Portable Network Graphics (png)|*.png|"
                     "Windows bitmaps (bmp)|*.bmp|"
                     "All files|*.*");
  if(diag.DoModal() == IDOK)
  {
    CString file = diag.GetChosenFile();
    CString relative;
    if(Misc::MakeRelativePathname(m_base,file,relative))
    {
      file = relative;
    }
    int num = m_comboSrc.AddString(file);
    m_comboSrc.SetCurSel(num);
    m_src = file;
    OnDocumentComplete(NULL,NULL);
  }
}

void 
ImageDlg::OnBnClickedButtonVideo()
{
  DocFileDialog diag(true               // true = open
                    ,"Select a video"   // title
                    ,""                 // Extension
                    ,""                 // Default file
                    ,0                  // flags
                    ,"All movies (wmv,mpeg,)|*.wmv;*.mpeg;*.avi;*.wav|"
                    "Windows video movie (wmv)|*.wmv|"
                    "MPEG 1,2 Movie (mpeg)|*.mpeg;*.mpeg2|"
                    "VID/X Video movie (avi)|*.avi|"
                    "Windows sound (wav)|*.wav|"
                    "All files|*.*");
  if(diag.DoModal() == IDOK)
  {
    CString file = diag.GetChosenFile();
    CString relative;
    if(Misc::MakeRelativePathname(m_base,file,relative))
    {
      file = relative;
    }
    int num = m_comboDynSrc.AddString(file);
    m_comboDynSrc.SetCurSel(num);
    m_dynsrc = file;
    OnDocumentComplete(NULL,NULL);
  }
}

void 
ImageDlg::OnBnClickedButtonLowres()
{
  DocFileDialog diag(true               // true = open
                    ,"Select an low resolution image"  // title
                    ,""                 // Extension
                    ,""                 // Default file
                    ,0                  // flags
                    ,"All images (jpg,gif,bmp,png)|*.jpg;*.jpeg;*.gif;*.bmp|"
                    "Joint Photogroup files (jpg)|*.jpg;*.jpeg|"
                    "Graphics Information File (gif)|*.gif|"
                    "Portable Network Graphics (png)|*.png|"
                    "Windows bitmaps (bmp)|*.bmp|"
                    "All files|*.*");
  if(diag.DoModal() == IDOK)
  {
    CString file = diag.GetChosenFile();
    CString relative;
    if(Misc::MakeRelativePathname(m_base,file,relative))
    {
      file = relative;
    }
    int num = m_comboLowSrc.AddString(file);
    m_comboLowSrc.SetCurSel(num);
    m_lowsrc = file;
    OnDocumentComplete(NULL,NULL);
  }
}

void
ImageDlg::OnEnChangeEditHypinfo()
{
  UpdateData(Controls2Data);
}

void
ImageDlg::OnBnClickedRatio()
{
  int check = m_buttonRatio.GetCheck();
  CWnd* h = GetDlgItem(IDC_IMG_HEIGHT);
  h->EnableWindow(check == 0);
}

void 
ImageDlg::OnEnChangeImgWidth()
{
  if(!m_initDone)
  {
    return;
  }
  CString newWidth;
  CWnd* w = GetDlgItem(IDC_IMG_WIDTH);
  w->GetWindowText(newWidth);
  if(newWidth != m_width)
  {
    m_width = newWidth;
    if(m_buttonRatio.GetCheck())
    {
      // m_ratio = h / w
      long width = atoi(m_width);
      long height = (long)(width * m_ratio);
      m_height.Format("%d",height);
    }
    UpdateData(Data2Controls);
    OnDocumentComplete(NULL,NULL);
  }
}

void 
ImageDlg::OnEnChangeImgHeight()
{
  if(!m_initDone)
  {
    return;
  }
  CString newHeight;
  CWnd* h = GetDlgItem(IDC_IMG_HEIGHT);
  h->GetWindowText(newHeight);
  if(m_height.CompareNoCase(newHeight))
  {
    m_height = newHeight;
    UpdateData(Data2Controls);
    OnDocumentComplete(NULL,NULL);
  }
}

void 
ImageDlg::OnEnChangeImgHpad()
{
  CString newHPad;
  CWnd* h = GetDlgItem(IDC_IMG_HPAD);
  h->GetWindowText(newHPad);
  if(m_hpad.CompareNoCase(newHPad))
  {
    m_hpad = newHPad;
  }
}

void 
ImageDlg::OnEnChangeImgVpad()
{
  CString newVPad;
  CWnd* v = GetDlgItem(IDC_IMG_VPAD);
  v->GetWindowText(newVPad);
  if(m_vpad.CompareNoCase(newVPad))
  {
    m_vpad = newVPad;
  }
}

void 
ImageDlg::OnBnClickedResetImage()
{
  m_width =  m_docWidth;
  m_height = m_docHeight;
  ReCalculateRatio();
  UpdateData(Data2Controls);
  OnDocumentComplete(NULL,NULL);
}

void 
ImageDlg::OnCbnSelchangeAlignment()
{
  int ind = m_comboAlign.GetCurSel();
  if(ind >= 0)
  {
    m_comboAlign.GetLBText(ind,m_align);
  }
}

void 
ImageDlg::OnEnChangeImgBorderwidth()
{
  CWnd* w = GetDlgItem(IDC_IMG_BORDERWIDTH);
  w->GetWindowText(m_border);
}

void 
ImageDlg::OnCbnSelchangeImgStartvideo()
{
  int ind = m_comboStart.GetCurSel();
  if(ind >= 0)
  {
    m_comboStart.GetLBText(ind,m_start);
  }
}

void 
ImageDlg::OnCbnSelchangeImgLoop()
{
  int ind = m_comboLoop.GetCurSel();
  if(ind >= 0)
  {
    m_comboLoop.GetLBText(ind,m_loop);
  }
}

void 
ImageDlg::OnBnClickedId()
{
  HtmlElement* elem = (HtmlElement*)(m_img);
  GeneralIDDlg dlg(this,"img",elem);
  dlg.DoModal();
}

void 
ImageDlg::OnBnClickedEvents()
{
  TagEventsDlg dlg(this,m_img,"IMG");
  dlg.DoModal();
  UpdateData(Data2Controls);
}

void 
ImageDlg::OnBnClickedStyle()
{
  if(!m_img->HasStyle())
  {
    m_img->SetStyle();
  }
  UINT tabs = CSS1 | // Overview
              CSS4 | // Borders
              CSS5 | // Sizes
              CSS7 ; // Box

  UpdateProperties();
  CString style = m_img->GetInlineStyle();
  style = CString("img { ") + style + "}";
  StyleSheetDlg dlg(this,m_base,"img",NULL,style,true,tabs);
  if(dlg.DoModal() == IDOK)
  {
    style = dlg.GetInlineStylesheet();
    style.TrimRight("}");
    style.TrimLeft("img {");
    m_img->SetInlineStyle(style);
    FillPage();
    UpdateData(Data2Controls);
    OnDocumentComplete(NULL,NULL);
  }
}

void 
ImageDlg::OnBnClickedOk()
{
  UpdateProperties();
  OnOK();
}

void 
ImageDlg::OnBnClickedCancel()
{
  OnCancel();
}

void 
ImageDlg::OnBnClickedHelpImage()
{
  // TODO: Add your control notification handler code here
}
