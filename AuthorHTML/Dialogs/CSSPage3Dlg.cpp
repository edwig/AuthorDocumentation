//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    CSSPage3Dlg.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for editing CSS properties
//              Handles the background attributes
//
#include "stdafx.h"
#include "AuthorHTML.h"
#include "StyleSheetDlg.h"
#include "CSSPage3Dlg.h"
#include "FileDialog.h"
#include "Misc.h"

#define COLOR_DEFAULT_BACKGROUNDCOLOR 0x0ffffff   // WHITE

// CSSPage3Dlg dialog

IMPLEMENT_DYNAMIC(CSSPage3Dlg, CDialog)

CSSPage3Dlg::CSSPage3Dlg(CWnd* pParent
                        ,CssStyleSheet* pCss)
           	:CDialog(CSSPage3Dlg::IDD, pParent)
            ,m_css(pCss)
{
}

CSSPage3Dlg::~CSSPage3Dlg()
{
}

void CSSPage3Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
  DDX_Control(pDX,IDC_BC_CHOOSE,      m_buttonBC);
  DDX_Control(pDX,IDC_HAS_COLOR,      m_buttonHasBC);
  DDX_Text   (pDX,IDC_BACKGROUNDIMAGE,m_backgroundImage);
  DDX_Control(pDX,IDC_BI_CHOOSE,      m_buttonBI);
  DDX_Control(pDX,IDC_BI_TILE,        m_comboRepeat);
  DDX_Control(pDX,IDC_BI_SCROLL,      m_buttonAttach);
  DDX_Control(pDX,IDC_BI_TOP,         m_buttonVTop);
  DDX_Control(pDX,IDC_BI_CENTER,      m_buttonVCenter);
  DDX_Control(pDX,IDC_BI_BOTTOM,      m_buttonVBottom);
  DDX_Control(pDX,IDC_BI_LEFT,        m_buttonHLeft);
  DDX_Control(pDX,IDC_BI_MIDDLE,      m_buttonHMiddle);
  DDX_Control(pDX,IDC_BI_RIGHT,       m_buttonHRight);

  if(pDX->m_bSaveAndValidate == Data2Controls)
  {
    CString repeat = Misc::GetAttributeDisplayname("background-repeat",m_backgroundRepeat);
    int ind = m_comboRepeat.FindString(-1,repeat);
    if(ind >= 0)
    {
      m_comboRepeat.SetCurSel(ind);
    }
    if(m_backgroundAttach == "" ||
       m_backgroundAttach.CompareNoCase("scroll") == 0)
    {
      m_buttonAttach.SetCheck(TRUE);
    }
    else
    {
      m_buttonAttach.SetCheck(FALSE);
    }
    bool enable = !m_backgroundImage.IsEmpty();
    m_comboRepeat  .EnableWindow(enable);
    m_buttonAttach .EnableWindow(enable);
    m_buttonVTop   .EnableWindow(enable);
    m_buttonVCenter.EnableWindow(enable);
    m_buttonVBottom.EnableWindow(enable);
    m_buttonHLeft  .EnableWindow(enable);
    m_buttonHMiddle.EnableWindow(enable);
    m_buttonHRight .EnableWindow(enable);

    int toCheck = IDC_BI_TOP;
    if(m_backgroundPosV.CompareNoCase("top")    == 0) toCheck = IDC_BI_TOP;
    if(m_backgroundPosV.CompareNoCase("center") == 0) toCheck = IDC_BI_CENTER;
    if(m_backgroundPosV.CompareNoCase("bottom") == 0) toCheck = IDC_BI_BOTTOM;
    CheckRadioButton(IDC_BI_TOP,IDC_BI_BOTTOM,toCheck);

    toCheck= IDC_BI_LEFT;
    if(m_backgroundPosH.CompareNoCase("left")   == 0) toCheck = IDC_BI_LEFT;
    if(m_backgroundPosH.CompareNoCase("center") == 0) toCheck = IDC_BI_MIDDLE;
    if(m_backgroundPosH.CompareNoCase("right")  == 0) toCheck = IDC_BI_RIGHT;
    CheckRadioButton(IDC_BI_LEFT,IDC_BI_RIGHT,toCheck);

    ((StyleSheetDlg*)GetParent())->SetBackgroundColor(m_backgroundColor);

    m_buttonBC.EnableWindow(m_hasBackColor);
    m_buttonHasBC.SetCheck(m_hasBackColor);
  }
}

BEGIN_MESSAGE_MAP(CSSPage3Dlg, CDialog)
  ON_BN_CLICKED     (IDC_BC_CHOOSE,      OnBnClickedBcChoose)
  ON_BN_CLICKED     (IDC_HAS_COLOR,      OnBnClickedHasColor)
  ON_BN_CLICKED     (IDC_BI_CHOOSE,      OnBnClickedBiChoose)
  ON_EN_KILLFOCUS   (IDC_BACKGROUNDIMAGE,OnEnChangeBackgroundimage)
  ON_CBN_SELCHANGE  (IDC_BI_TILE,        OnCbnSelchangeBiTile)
  ON_BN_CLICKED     (IDC_BI_SCROLL,      OnBnClickedBiScroll)
  ON_BN_CLICKED     (IDC_BI_TOP,         OnBnClickedBiTop)
  ON_BN_CLICKED     (IDC_BI_CENTER,      OnBnClickedBiCenter)
  ON_BN_CLICKED     (IDC_BI_BOTTOM,      OnBnClickedBiBottom)
  ON_BN_CLICKED     (IDC_BI_LEFT,        OnBnClickedBiLeft)
  ON_BN_CLICKED     (IDC_BI_MIDDLE,      OnBnClickedBiMiddle)
  ON_BN_CLICKED     (IDC_BI_RIGHT,       OnBnClickedBiRight)
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CSSPage3Dlg, CDialog)
  ON_EVENT(CSSPage3Dlg, IDC_EXPLORER, 0x00000103, OnDocumentComplete, VTS_DISPATCH VTS_VARIANT)
END_EVENTSINK_MAP()

BOOL
CSSPage3Dlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  // Start up our display preview of fonts
  CWnd *pWnd = GetDlgItem(IDC_EXPLORER);
  if(pWnd)
  {
    m_spBrowser = pWnd->GetControlUnknown();
  }
  // Fill combo repeat
  vector<string> all;
  Misc::GetAllAttributeDisplaynames("background-repeat",&all);
  for(unsigned int ind=0; ind<all.size(); ++ind)
  {
    m_comboRepeat.AddString(all[ind].c_str());
  }
  m_buttonBC.SetColor(COLOR_DEFAULT_BACKGROUNDCOLOR);
  m_buttonBC.EnableOtherButton(_T("More colors"));
  // get all attributes
  FillPage();
  UpdateData(Data2Controls);
  return TRUE;
}

void
CSSPage3Dlg::SetSelector(string selector)
{
  m_selector = selector;
  FillPage();
}

void
CSSPage3Dlg::FillPage()
{
  m_backgroundColor  = CSSPropertyGet(m_css,m_selector,"background-color", false);
  m_backgroundImage  = CSSPropertyGet(m_css,m_selector,"background-image", false);
  m_backgroundRepeat = CSSPropertyGet(m_css,m_selector,"background-repeat",false);
  m_backgroundAttach = CSSPropertyGet(m_css,m_selector,"background-attachment",false);
  CString backPos    = CSSPropertyGet(m_css,m_selector,"background-position",  false);

  if(m_backgroundColor.IsEmpty())
  {
    m_hasBackColor = false;
  }
  else
  {
    m_hasBackColor = true;
    int red,green,blue;
    Misc::DecodeColor(m_backgroundColor,red,green,blue);
    m_buttonBC.SetColor(RGB(red,green,blue));
  }
  if(!m_backgroundImage.IsEmpty())
  {
    if(m_backgroundImage.Left(3).CompareNoCase("url") == 0)
    {
      m_backgroundImage = m_backgroundImage.Mid(3);
      m_backgroundImage.TrimLeft ("( \"");
      m_backgroundImage.TrimRight(") \"");
    }
  }
  // Find background positions
  int pos = backPos.Find(" ");
  if(pos >= 0)
  {
    m_backgroundPosH = backPos.Left(pos);
    m_backgroundPosV = backPos.Mid(pos+1);
  }
  else
  {
    m_backgroundPosH = backPos;
    m_backgroundPosV = "";
  }
  Redisplay();
}

void 
CSSPage3Dlg::UpdateProperties()
{
  CString image = m_backgroundImage;
  if(!image.IsEmpty())
  {
    image = "url(\"" + m_backgroundImage + ")";
  }
  if(m_hasBackColor)
  {
    if(!m_backgroundColor.IsEmpty())
    {
      int red,green,blue;
      Misc::DecodeColor(m_backgroundColor,red,green,blue);
      m_backgroundColor.Format("#%02x%02x%02x",red,green,blue);
    }
  }
  else
  {
    m_backgroundColor = "";
  }
  CString backPos = m_backgroundPosH;
  if(!m_backgroundPosV.IsEmpty())
  {
    backPos += " " + m_backgroundPosV;
  }
  CSSPropertyPut(m_css,m_selector,"background-color", m_backgroundColor, false);
  CSSPropertyPut(m_css,m_selector,"background-image", image, false);
  CSSPropertyPut(m_css,m_selector,"background-repeat",m_backgroundRepeat,false);
  CSSPropertyPut(m_css,m_selector,"background-attachment",m_backgroundAttach,false);
  CSSPropertyPut(m_css,m_selector,"background-position",  backPos,false);

  StyleSheetDlg* dlg = (StyleSheetDlg*)GetParent();
  dlg->SetCanApply();
}

// CSSPage3Dlg message handlers

void 
CSSPage3Dlg::OnDocumentComplete(LPDISPATCH /*pDisp*/, LPVARIANT /*pURL*/)
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
    VARIANT findIndex;
    CComPtr<IDispatch> dispImg;
    V_VT(&findIndex) = VT_I4;
    V_I4(&findIndex) = ind;
    hr = coll->item(findIndex,findIndex,&dispImg);
    CComQIPtr<IHTMLBodyElement,&IID_IHTMLBodyElement> body = dispImg;
    if(body.p)
    {
      CComQIPtr<IHTMLElement,&IID_IHTMLElement> elem = body;
      CComPtr<IHTMLStyle> style;
      hr = elem->get_style(&style);
      if(SUCCEEDED(hr))
      {
        // SET BACKGROUND COLOR
        VARIANT value;
        V_VT(&value) = VT_BSTR;
        V_BSTR(&value) = CT2CW(m_backgroundColor);
        style->put_backgroundColor(value);

        // SET BACKGROUND-IMAGE
        CComBSTR bString = CT2CW( "url(" + m_backgroundImage + ")");
        hr = style->put_backgroundImage(bString);

        // SET BACKGROUND-REPEAT
        bString = CT2CW(m_backgroundRepeat);
        hr = style->put_backgroundRepeat(bString);

        // SET BACKGROUND-ATTACHMENT
        bString = CT2CW(m_backgroundAttach);
        style->put_backgroundAttachment(bString);

        // BACKGROUND POSITION
        CString backPos = m_backgroundPosH;
        if(!m_backgroundPosV.IsEmpty())
        {
          backPos += " " + m_backgroundPosV;
        }
        bString = CT2CW(backPos);
        style->put_backgroundPosition(bString);
      }
    }
  }
}

void 
CSSPage3Dlg::Redisplay()
{
  // Display all gotten properties
  UpdateData(Data2Controls);
  // (Re-)Load image page for display the text
  m_spBrowser->Navigate(L"res://authorhtml.exe/cssbi.html", NULL, NULL, NULL, NULL);
}

void 
CSSPage3Dlg::OnBnClickedBcChoose()
{
  COLORREF col = m_buttonBC.GetColor();
  int red   = GetRValue(col);
  int green = GetGValue(col);
  int blue  = GetBValue(col);
  m_backgroundColor.Format("#%02x%02x%02x",red,green,blue);
  UpdateProperties();
  Redisplay();
}

void 
CSSPage3Dlg::OnBnClickedBiChoose()
{
  DocFileDialog diag(true                         // true = open
                    ,"Select a background image"  // title
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
    m_backgroundImage = diag.GetChosenFile();
    CString base = ((StyleSheetDlg*)GetParent())->GetBaseDirectory();
    CString relative;
    if(Misc::MakeRelativePathname(base,m_backgroundImage,relative))
    {
      m_backgroundImage = relative;
    }
    UpdateProperties();
    Redisplay();
  }
}

void CSSPage3Dlg::OnEnChangeBackgroundimage()
{
  CWnd* wnd = GetDlgItem(IDC_BACKGROUNDIMAGE);
  wnd->GetWindowText(m_backgroundImage);
  if(m_backgroundImage.IsEmpty())
  {
    // Image removed
    m_backgroundRepeat = "";
    m_backgroundPosV   = "";
    m_backgroundPosH   = "";
    m_backgroundAttach = "";
  }
  UpdateProperties();
  Redisplay();
}

void CSSPage3Dlg::OnCbnSelchangeBiTile()
{
  int ind = m_comboRepeat.GetCurSel();
  if(ind >=0)
  {
    CString text;
    m_comboRepeat.GetLBText(ind,text);
    m_backgroundRepeat = Misc::GetAttributeValue("background-repeat",text);
    UpdateProperties();
    Redisplay();
  }
}

void CSSPage3Dlg::OnBnClickedBiScroll()
{
  if(m_buttonAttach.GetCheck())
  {
    m_backgroundAttach = "scroll";
  }
  else
  {
    m_backgroundAttach = "fixed";
  }
  UpdateProperties();
  Redisplay();
}

void CSSPage3Dlg::OnBnClickedBiTop()
{
  m_backgroundPosV = "top";
  UpdateProperties();
  Redisplay();
}

void CSSPage3Dlg::OnBnClickedBiCenter()
{
  m_backgroundPosV = "center";
  UpdateProperties();
  Redisplay();
}

void CSSPage3Dlg::OnBnClickedBiBottom()
{
  m_backgroundPosV = "bottom";
  UpdateProperties();
  Redisplay();
}

void CSSPage3Dlg::OnBnClickedBiLeft()
{
  m_backgroundPosH = "left";
  UpdateProperties();
  Redisplay();
}

void 
CSSPage3Dlg::OnBnClickedBiMiddle()
{
  m_backgroundPosH = "center";
  UpdateProperties();
  Redisplay();
}

void 
CSSPage3Dlg::OnBnClickedBiRight()
{
  m_backgroundPosH = "right";
  UpdateProperties();
  Redisplay();
}

void 
CSSPage3Dlg::OnBnClickedHasColor()
{
  m_hasBackColor = m_buttonHasBC.GetCheck() == TRUE;
  UpdateProperties();
  Redisplay();
}
