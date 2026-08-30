//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    ObjectDlg.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for managing the <OBJECT> tag in HTML
//
#include "stdafx.h"
#include "AuthorHTML.h"
#include "ObjectDlg.h"
#include "GeneralIDDlg.h"
#include "StyleSheetDlg.h"
#include "TagEventsDlg.h"
#include "ParametersDlg.h"
#include "FileDialog.h"
#include <string.h>

// ObjectDlg dialog

IMPLEMENT_DYNAMIC(ObjectDlg, CDialog)
ObjectDlg::ObjectDlg(CWnd* pParent
                    ,CString base
                    ,HtmlObject* object)
          :CDialog(ObjectDlg::IDD, pParent)
          ,m_base(base)
          ,m_object(object)
{
}

ObjectDlg::~ObjectDlg()
{
}

void ObjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
  DDX_Text   (pDX,IDC_NAME,         m_name);
  DDX_Control(pDX,IDC_OBJ_TYPE,     m_comboType);
  DDX_Control(pDX,IDC_CODETYPE,     m_comboCodeType);
  DDX_Text   (pDX,IDC_DATA,         m_data);
  DDX_Text   (pDX,IDC_CODE,         m_code);
  DDX_Control(pDX,IDC_CLASSID,      m_comboClassID);
  DDX_Text   (pDX,IDC_BASEHREF,     m_baseHref);
  DDX_Text   (pDX,IDC_CODEBASE,     m_codeBase);
  DDX_Text   (pDX,IDC_ALT,          m_alt);
  DDX_Text   (pDX,IDC_ALTHTML,      m_altHtml);
  DDX_Text   (pDX,IDC_STANDBY,      m_standby);
  DDX_Text   (pDX,IDC_USEMAP,       m_useMap);
  DDX_Control(pDX,IDC_ALIGN,        m_comboAlign);
  DDX_Text   (pDX,IDC_BORDERWIDTH,  m_border);
  DDX_Text   (pDX,IDC_WIDTH,        m_width);
  DDX_Text   (pDX,IDC_HEIGHT,       m_height);
  DDX_Text   (pDX,IDC_OBJ_HSPACE,   m_hspace);
  DDX_Text   (pDX,IDC_OBJ_VSPACE,   m_vspace);
  DDX_Control(pDX,IDC_BUTT_DATA,    m_buttonData);
  DDX_Control(pDX,IDC_BUTT_CODE,    m_buttonCode);
  DDX_Control(pDX,IDC_BUTT_BHREF,   m_buttonBHref);
  DDX_Control(pDX,IDC_BUTT_CODEBASE,m_buttonCodeBase);
  DDX_Control(pDX,IDC_BUTT_ALTHTML, m_buttonAltHtml);
  DDX_Control(pDX,IDC_BUTT_STANDBY, m_buttonStandby);
  DDX_Control(pDX,IDC_BUTT_USEMAP,  m_buttonUseMap);
  DDX_Control(pDX,IDC_PARAMETERS,   m_buttonParameters);
  DDX_Control(pDX,IDC_ID,           m_buttonID);
  DDX_Control(pDX,IDC_EVENTS,       m_buttonEvents);
  DDX_Control(pDX,IDC_STYLE,        m_buttonStyle);
  DDX_Control(pDX,IDOK,             m_buttonOK);
  DDX_Control(pDX,IDCANCEL,         m_buttonCancel);
  DDX_Control(pDX,IDC_OBJ_HELP,     m_buttonHelp);

  if(pDX->m_bSaveAndValidate == Data2Controls)
  {
    int ind;
    ind = m_comboType.FindString(-1,m_type);
          m_comboType.SetCurSel(ind);
    ind = m_comboCodeType.FindString(-1,m_codeType);
          m_comboCodeType.SetCurSel(ind);
    ind = m_comboClassID.FindString(-1,m_classID);
          m_comboClassID.SetCurSel(ind);
    ind = m_comboAlign.FindString(-1,m_align);
          m_comboAlign.SetCurSel(ind);

    CString def,text;

    m_buttonID.GetWindowText(def);
    text = m_object->HasIdentity() ? "[ &ID ]" : "&ID";
    if(def != text) m_buttonID.SetWindowText(text);

    m_buttonStyle.GetWindowText(def);
    text = m_object->HasStyle() ? "[ &Style ]" : "&Style";
    if(def != text) m_buttonStyle.SetWindowText(text);

    m_buttonEvents.GetWindowText(def);
    text = m_object->HasEvents() ? "[ &Events ]" : "&Events";
    if(def != text) m_buttonEvents.SetWindowText(text);

    // Parameter button
    m_buttonParameters.GetWindowText(def);
    text = m_object->GetNumberOfParameters() ? "[ &Parameters ]" : "&Parameters";
    if(def != text) m_buttonParameters.SetWindowText(text);
  }
}


BEGIN_MESSAGE_MAP(ObjectDlg, CDialog)
  ON_EN_KILLFOCUS (IDC_NAME,          OnEnChangeName)
  ON_CBN_SELCHANGE(IDC_OBJ_TYPE,      OnCbnSelchangeObjType)
  ON_CBN_SELCHANGE(IDC_CODETYPE,      OnCbnSelchangeCodetype)
  ON_EN_KILLFOCUS (IDC_DATA,          OnEnChangeData)
  ON_EN_KILLFOCUS (IDC_CODE,          OnEnChangeCode)
  ON_CBN_SELCHANGE(IDC_CLASSID,       OnCbnSelchangeClassid)
  ON_EN_KILLFOCUS (IDC_BASEHREF,      OnEnChangeBasehref)
  ON_EN_KILLFOCUS (IDC_CODEBASE,      OnEnChangeCodebase)
  ON_EN_KILLFOCUS (IDC_ALT,           OnEnChangeAlt)
  ON_EN_KILLFOCUS (IDC_ALTHTML,       OnEnChangeAlthtml)
  ON_EN_KILLFOCUS (IDC_STANDBY,       OnEnChangeStandby)
  ON_EN_KILLFOCUS (IDC_USEMAP,        OnEnChangeUsemap)
  ON_CBN_SELCHANGE(IDC_ALIGN,         OnCbnSelchangeAlign)
  ON_EN_KILLFOCUS (IDC_BORDERWIDTH,   OnEnChangeBorderwidth)
  ON_EN_KILLFOCUS (IDC_WIDTH,         OnEnChangeWidth)
  ON_EN_KILLFOCUS (IDC_HEIGHT,        OnEnChangeHeight)
  ON_EN_KILLFOCUS (IDC_OBJ_HSPACE,    OnEnChangeObjHspace)
  ON_EN_KILLFOCUS (IDC_OBJ_VSPACE,    OnEnChangeObjVspace)
  ON_BN_CLICKED   (IDC_BUTT_DATA,     OnBnClickedButtData)
  ON_BN_CLICKED   (IDC_BUTT_CODE,     OnBnClickedButtCode)
  ON_BN_CLICKED   (IDC_BUTT_BHREF,    OnBnClickedButtBhref)
  ON_BN_CLICKED   (IDC_BUTT_CODEBASE, OnBnClickedButtCodebase)
  ON_BN_CLICKED   (IDC_BUTT_ALTHTML,  OnBnClickedButtAlthtml)
  ON_BN_CLICKED   (IDC_BUTT_STANDBY,  OnBnClickedButtStandby)
  ON_BN_CLICKED   (IDC_BUTT_USEMAP,   OnBnClickedButtUsemap)
  ON_BN_CLICKED   (IDC_PARAMETERS,    OnBnClickedButtParameters)
  ON_BN_CLICKED   (IDC_ID,            OnBnClickedId)
  ON_BN_CLICKED   (IDC_EVENTS,        OnBnClickedEvents)
  ON_BN_CLICKED   (IDC_STYLE,         OnBnClickedStyle)
  ON_BN_CLICKED   (IDOK,              OnBnClickedOk)
  ON_BN_CLICKED   (IDCANCEL,          OnBnClickedCancel)
  ON_BN_CLICKED   (IDC_OBJ_HELP,      OnBnClickedObjHelp)
END_MESSAGE_MAP()

BOOL
ObjectDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  m_comboAlign.AddString("");
  m_comboAlign.AddString("Baseline");
  m_comboAlign.AddString("Center");
  m_comboAlign.AddString("Left");
  m_comboAlign.AddString("Middle");
  m_comboAlign.AddString("Right");
  m_comboAlign.AddString("Textbottom");
  m_comboAlign.AddString("Textmiddle");
  m_comboAlign.AddString("Texttop");

  FillPage();


  UpdateData(Data2Controls);
  return TRUE;
}

void
ObjectDlg::FillPage()
{
  m_name      = m_object->GetProperty(HtmlObject::E_Name);
  m_type      = m_object->GetProperty(HtmlObject::E_Type);
  m_codeType  = m_object->GetProperty(HtmlObject::E_CodeType);
  m_data      = m_object->GetProperty(HtmlObject::E_Data);
  m_code      = m_object->GetProperty(HtmlObject::E_Code);
  m_classID   = m_object->GetProperty(HtmlObject::E_ClassId);
  m_baseHref  = m_object->GetProperty(HtmlObject::E_BaseHref);
  m_codeBase  = m_object->GetProperty(HtmlObject::E_CodeBase);
  m_alt       = m_object->GetProperty(HtmlObject::E_Alt);
  m_altHtml   = m_object->GetProperty(HtmlObject::E_AltHtml);
  m_standby   = m_object->GetProperty(HtmlObject::E_Standby);
  m_useMap    = m_object->GetProperty(HtmlObject::E_UseMap);
  m_align     = m_object->GetProperty(HtmlObject::E_Align);
  m_border    = m_object->GetProperty(HtmlObject::E_Border);
  m_width     = m_object->GetProperty(HtmlObject::E_Width);
  m_height    = m_object->GetProperty(HtmlObject::E_Height);
  m_vspace    = m_object->GetProperty(HtmlObject::E_VSpace);
  m_hspace    = m_object->GetProperty(HtmlObject::E_HSpace);

  if(!m_align.IsEmpty())
  {
    m_align.SetAt(0,_toupper(m_align.GetAt(0)));
    int ind = m_comboAlign.FindString(-1,m_align);
    if(ind < 0) ind = 0;
    m_comboAlign.SetCurSel(ind);
  }
}

void
ObjectDlg::UpdateProperties()
{
  m_object->SetProperty(HtmlObject::E_Name,       m_name);
  m_object->SetProperty(HtmlObject::E_Data,       m_data);
  m_object->SetProperty(HtmlObject::E_Type,       m_type);
  m_object->SetProperty(HtmlObject::E_CodeType,   m_codeType);
  m_object->SetProperty(HtmlObject::E_Code,       m_code);
  m_object->SetProperty(HtmlObject::E_ClassId,    m_classID);
  m_object->SetProperty(HtmlObject::E_BaseHref,   m_baseHref);
  m_object->SetProperty(HtmlObject::E_CodeBase,   m_codeBase);
  m_object->SetProperty(HtmlObject::E_Alt,        m_alt);
  m_object->SetProperty(HtmlObject::E_AltHtml,    m_altHtml);
  m_object->SetProperty(HtmlObject::E_Standby,    m_standby);
  m_object->SetProperty(HtmlObject::E_UseMap,     m_useMap);
  m_object->SetProperty(HtmlObject::E_Align,      m_align);
  m_object->SetProperty(HtmlObject::E_Border,     m_border);
  m_object->SetProperty(HtmlObject::E_Width,      m_width);
  m_object->SetProperty(HtmlObject::E_Height,     m_height);
  m_object->SetProperty(HtmlObject::E_VSpace,     m_vspace);
  m_object->SetProperty(HtmlObject::E_HSpace,     m_hspace);
}

// ObjectDlg message handlers

void 
ObjectDlg::OnEnChangeName()
{
  UpdateData();
}

void 
ObjectDlg::OnCbnSelchangeObjType()
{
  int ind = m_comboType.GetCurSel();
  if(ind >= 0)
  {
    m_comboType.GetLBText(ind,m_type);
  }
}

void 
ObjectDlg::OnCbnSelchangeCodetype()
{
  int ind = m_comboCodeType.GetCurSel();
  if(ind >= 0)
  {
    m_comboCodeType.GetLBText(ind,m_codeType);
  }
}

void 
ObjectDlg::OnEnChangeData()
{
  UpdateData();
}

void 
ObjectDlg::OnEnChangeCode()
{
  UpdateData();
}

void 
ObjectDlg::OnCbnSelchangeClassid()
{
  int ind = m_comboClassID.GetCurSel();
  if(ind >= 0)
  {
    m_comboClassID.GetLBText(ind,m_classID);
  }
}

void 
ObjectDlg::OnEnChangeBasehref()
{
  UpdateData();
}

void 
ObjectDlg::OnEnChangeCodebase()
{
  UpdateData();
}

void 
ObjectDlg::OnEnChangeAlt()
{
  UpdateData();
}

void 
ObjectDlg::OnEnChangeAlthtml()
{
  UpdateData();
}

void 
ObjectDlg::OnEnChangeStandby()
{
  UpdateData();
}

void 
ObjectDlg::OnEnChangeUsemap()
{
  UpdateData();
}

void 
ObjectDlg::OnCbnSelchangeAlign()
{
  int ind = m_comboAlign.GetCurSel();
  if(ind >= 0)
  {
    m_comboAlign.GetLBText(ind,m_align);
  }
}

void 
ObjectDlg::OnEnChangeBorderwidth()
{
  UpdateData();
}

void 
ObjectDlg::OnEnChangeWidth()
{
  UpdateData();
}

void 
ObjectDlg::OnEnChangeHeight()
{
  UpdateData();
}

void 
ObjectDlg::OnEnChangeObjHspace()
{
  UpdateData();
}

void 
ObjectDlg::OnEnChangeObjVspace()
{
  UpdateData();
}

// Search Data (generic URL)
void 
ObjectDlg::OnBnClickedButtData()
{
  DocFileDialog diag(true,"Search for an object","","",0);
  if(diag.DoModal() == IDOK)
  {
    m_data = diag.GetChosenFile();
    UpdateData(false);
  }
}

void 
ObjectDlg::OnBnClickedButtCode()
{
}

void 
ObjectDlg::OnBnClickedButtBhref()
{
  DocFileDialog diag(true,"Search for an object HREF","","",0);
  if(diag.DoModal() == IDOK)
  {
    m_baseHref = diag.GetChosenFile();
    UpdateData(false);
  }
}

void 
ObjectDlg::OnBnClickedButtCodebase()
{
  DocFileDialog diag(true,"Search for an object CODEBASE","","",0);
  if(diag.DoModal() == IDOK)
  {
    m_codeBase = diag.GetChosenFile();
    UpdateData(false);
  }
}

void 
ObjectDlg::OnBnClickedButtAlthtml()
{
  // TODO: Add your control notification handler code here
}

void 
ObjectDlg::OnBnClickedButtStandby()
{
  // TODO: Add your control notification handler code here
}

void 
ObjectDlg::OnBnClickedButtUsemap()
{
  // TODO: Add your control notification handler code here
}

void
ObjectDlg::OnBnClickedButtParameters()
{
  ParametersDlg dlg(this,m_object);
  dlg.DoModal();
  UpdateData(false);
}

void 
ObjectDlg::OnBnClickedId()
{
  HtmlElement* elem = (HtmlElement*)(m_object);
  GeneralIDDlg dlg(this,"object",elem);
  dlg.DoModal();
}

void 
ObjectDlg::OnBnClickedEvents()
{
  HtmlElement* elem = (HtmlElement*)(m_object);
  TagEventsDlg dlg(this,elem,"object");
  dlg.DoModal();
  UpdateData(Data2Controls);
}

void 
ObjectDlg::OnBnClickedStyle()
{
  if(!m_object->HasStyle())
  {
    m_object->SetStyle();
  }
  UpdateProperties();
  CString style = m_object->GetInlineStyle();
  style = CString("object { ") + style + "}";
  StyleSheetDlg dlg(this,m_base,"object",NULL,style);
  if(dlg.DoModal() == IDOK)
  {
    style = dlg.GetInlineStylesheet();
    style.TrimRight("}");
    style.TrimLeft("object {");
    m_object->SetInlineStyle(style);
    FillPage();
    UpdateData(Data2Controls);
  }
}

void 
ObjectDlg::OnBnClickedOk()
{
  UpdateProperties();
  OnOK();
}

void 
ObjectDlg::OnBnClickedCancel()
{
  OnCancel();
}

void 
ObjectDlg::OnBnClickedObjHelp()
{
  // TODO: Add your control notification handler code here
}

