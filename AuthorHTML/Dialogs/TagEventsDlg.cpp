//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    TagEventsDlg.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for all the events of a tag.
//
#include "stdafx.h"
#include "AuthorHTML.h"
#include "TagEventDlg.h"
#include "TagEventsDlg.h"

// TagEventsDlg dialog

IMPLEMENT_DYNAMIC(TagEventsDlg, CDialog)

TagEventsDlg::TagEventsDlg(CWnd* pParent
                          ,HtmlElement* elem
                          ,CString      tag)
             :CDialog(TagEventsDlg::IDD, pParent)
             ,m_elem(elem)
             ,m_tag(tag)
{
}

TagEventsDlg::~TagEventsDlg()
{
}

void TagEventsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
  DDX_Text   (pDX,IDC_FORHTML,m_descript);
  DDX_Text   (pDX,IDC_ID,     m_id);
  DDX_Text   (pDX,IDC_TAGNAME,m_name);
  DDX_Control(pDX,IDC_LIST,   m_list);
}

BEGIN_MESSAGE_MAP(TagEventsDlg, CDialog)
  ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST, OnLvnItemchangedList)
  ON_NOTIFY(LVN_ITEMACTIVATE,IDC_LIST, OnLvnItemchangedList)
  ON_BN_CLICKED(IDC_EDIT,         OnBnClickedEdit)
  ON_BN_CLICKED(IDC_NEW,          OnBnClickedNew)
  ON_BN_CLICKED(IDC_DELETE,       OnBnClickedDelete)
  ON_BN_CLICKED(IDOK,             OnBnClickedOk)
  ON_BN_CLICKED(IDCANCEL,         OnBnClickedCancel)
  ON_BN_CLICKED(IDC_EVENTS_HELP,  OnBnClickedEventsHelp)
END_MESSAGE_MAP()

BOOL
TagEventsDlg::OnInitDialog()
{
  CDialog::OnInitDialog();
  m_id   = m_elem->GetAttribute("id");
  m_name = m_elem->GetAttribute("name");
  m_descript = Misc::GetTagDescription(m_tag);

  // Get all events
  Misc::GetAllEvents(m_allEvents,0,0);

  m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EDITLABELS); // LVS_EX_GRIDLINES|

  m_list.InsertColumn(0,"Event",LVCFMT_LEFT,200);
  m_list.InsertColumn(1,"Code", LVCFMT_LEFT,450);
  DisplayList();

  return TRUE;
}

void
TagEventsDlg::DisplayList()
{
  m_list.DeleteAllItems();

  int listitem = 0;
  for(unsigned int ind = 0; ind < m_allEvents.size(); ++ind)
  {
    CString event = m_allEvents[ind];
    CString code  = m_elem->GetAttribute(event);
    if(!code.IsEmpty())
    {
      m_list.InsertItem(LVIF_TEXT|LVIF_STATE, listitem, event, 0, 0, 0, 0);
      m_list.SetItemText(listitem,1,code);
      ++listitem;
    }
  }
  UpdateData(Data2Controls);
}

void
TagEventsDlg::UpdateProperties()
{
  int num  = m_list.GetItemCount();
  for(int ind = 0;ind < num; ++ind)
  {
    CString event = m_list.GetItemText(ind,0);
    CString code  = m_list.GetItemText(ind,1);
    m_elem->SetAttribute(event,code);
  }
}


// TagEventsDlg message handlers

void 
TagEventsDlg::OnLvnItemchangedList(NMHDR *pNMHDR, LRESULT *pResult)
{
  LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
  CString code = m_list.GetItemText(pNMLV->iItem,1);
  *pResult = 0;
}

void 
TagEventsDlg::OnBnClickedEdit()
{
  int item = m_list.GetNextItem(-1,LVNI_SELECTED);
  int num  = m_list.GetItemCount();
  if(item < num)
  {
    CString event = m_list.GetItemText(item,0);
    TagEventDlg dlg(this,m_elem,m_tag,event);
    if(dlg.DoModal() == IDOK)
    {
      CString res_event = dlg.GetEvent();
      CString res_code  = dlg.GetCode();
      m_list.SetItemText(item,0,res_event);
      m_list.SetItemText(item,1,res_code);
    }
  }
}

void 
TagEventsDlg::OnBnClickedNew()
{
  TagEventDlg dlg(this,m_elem,m_tag,"");
  if(dlg.DoModal() == IDOK)
  {
    CString event = dlg.GetEvent();
    CString code  = dlg.GetCode();
    int num = m_list.GetItemCount();
    m_list.InsertItem(LVIF_TEXT|LVIF_STATE,num,event,0,0,0,0);
    m_list.SetItemText(num,1,code);
  }
}

void 
TagEventsDlg::OnBnClickedDelete()
{
  int item = m_list.GetNextItem(-1,LVNI_SELECTED);
  int num  = m_list.GetItemCount();
  if(item < num)
  {
    CString event = m_list.GetItemText(item,0);
    m_elem->SetAttribute(event,"");
    m_list.DeleteItem(item);
  }
}

void 
TagEventsDlg::OnBnClickedOk()
{
  UpdateProperties();
  OnOK();
}

void 
TagEventsDlg::OnBnClickedCancel()
{
  OnCancel();
}

void 
TagEventsDlg::OnBnClickedEventsHelp()
{
  // TODO: Add your control notification handler code here
}
