//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    TagEventDlg.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for the event of a tag.
//
#include "stdafx.h"
#include "AuthorHTML.h"
#include "TagEventDlg.h"

// TagEventDlg dialog

IMPLEMENT_DYNAMIC(TagEventDlg, CDialog)

TagEventDlg::TagEventDlg(CWnd* pParent
                        ,HtmlElement* elem
                        ,CString      tag
                        ,CString      event)
	          :CDialog(TagEventDlg::IDD, pParent)
            ,m_elem(elem)
            ,m_tag(tag)
            ,m_event(event)
{
}

TagEventDlg::~TagEventDlg()
{
}

void TagEventDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
  DDX_Control(pDX,IDC_EVENT,      m_comboEvent);
  DDX_Text   (pDX,IDC_SCRIPT,     m_code);
  DDX_Control(pDX,IDOK,           m_buttonOK);
  DDX_Control(pDX,IDCANCEL,       m_buttonCancel);
  DDX_Control(pDX,IDC_SCRIPT_HELP,m_buttonHelp);
  DDX_Text   (pDX,IDC_FORHTML,    m_tagdescript);
  DDX_Text   (pDX,IDC_ID,         m_id);
  DDX_Text   (pDX,IDC_TAGNAME,    m_name);
  DDX_Text   (pDX,IDC_DESCRIPT,   m_description);

  if(pDX->m_bSaveAndValidate == Data2Controls)
  {
    int ind = m_comboEvent.FindString(-1,m_event);
              m_comboEvent.SetCurSel(ind);
  }
}

BEGIN_MESSAGE_MAP(TagEventDlg, CDialog)
  ON_CBN_SELCHANGE(IDC_EVENT,       OnCbnSelchangeEvent)
  ON_EN_KILLFOCUS (IDC_SCRIPT,      OnEnChangeScript)
  ON_BN_CLICKED   (IDOK,            OnBnClickedOk)
  ON_BN_CLICKED   (IDCANCEL,        OnBnClickedCancel)
  ON_BN_CLICKED   (IDC_SCRIPT_HELP, OnBnClickedScriptHelp)
END_MESSAGE_MAP()

BOOL
TagEventDlg::OnInitDialog()
{
  CDialog::OnInitDialog();
  FillPage();
  UpdateData(Data2Controls);
  return TRUE;
}

void
TagEventDlg::FillPage()
{
  m_name = m_elem->GetAttribute("name");
  m_id   = m_elem->GetAttribute("id");
  m_code = m_elem->GetAttribute(m_event);
  m_tagdescript = Misc::GetTagDescription(m_tag);
  m_description = Misc::GetEventDescription(m_event,0);

  int gettag = 0;
  if(m_tag.CompareNoCase("img")      == 0) gettag = TAGS_IMG;
  if(m_tag.CompareNoCase("body")     == 0) gettag = TAGS_BODY;
  if(m_tag.CompareNoCase("form")     == 0) gettag = TAGS_FORM;
  if(m_tag.CompareNoCase("input")    == 0) gettag = TAGS_INPUT;
  if(m_tag.CompareNoCase("select")   == 0) gettag = TAGS_SELECT;
  if(m_tag.CompareNoCase("table")    == 0) gettag = TAGS_TABLE;
  if(m_tag.CompareNoCase("script")   == 0) gettag = TAGS_SCRIPT;
  if(m_tag.CompareNoCase("style")    == 0) gettag = TAGS_STYLE;
  if(m_tag.CompareNoCase("object")   == 0) gettag = TAGS_OBJECT;
  if(m_tag.CompareNoCase("marquee")  == 0) gettag = TAGS_MARQUEE;
  if(m_tag.CompareNoCase("textarea") == 0) gettag = TAGS_TEXTAREA;

  std::vector<CString> all;
  Misc::GetAllEvents(all,0,gettag);   // Specific tags first
  Misc::GetAllEvents(all,0,TAGS_ALL); // All other tag events
  for(unsigned int ind = 0;ind < all.size(); ++ind)
  {
    m_comboEvent.AddString(all[ind]);
  }
}

void
TagEventDlg::UpdateProperties()
{
  //if(m_code.IsEmpty())
  //{
  //  m_elem->SetAttribute(m_event,"");
  //}
  //m_elem->SetAttribute(m_event,m_code);
}

// TagEventDlg message handlers

void 
TagEventDlg::OnCbnSelchangeEvent()
{
  int ind = m_comboEvent.GetCurSel();
  if(ind >= 0)
  {
    m_comboEvent.GetLBText(ind,m_event);
    m_description = Misc::GetEventDescription(m_event,0);
    UpdateData(Data2Controls);
  }
}

void 
TagEventDlg::OnEnChangeScript()
{
  CWnd* w = GetDlgItem(IDC_SCRIPT);
  w->GetWindowText(m_code);
}

void 
TagEventDlg::OnBnClickedOk()
{
  UpdateProperties();
  OnOK();
}

void 
TagEventDlg::OnBnClickedCancel()
{
  OnCancel();
}

void 
TagEventDlg::OnBnClickedScriptHelp()
{
  // TODO: Add your control notification handler code here
}
