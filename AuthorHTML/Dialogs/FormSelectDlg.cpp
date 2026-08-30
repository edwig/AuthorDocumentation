//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    FormSelectDlg.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for managing the <SELECT> tag in HTML, 
//
#include "stdafx.h"
#include "AuthorHTML.h"
#include "FormSelectDlg.h"
#include "GeneralIDDlg.h"
#include "StyleSheetDlg.h"
#include "TagEventsDlg.h"
#include "OptionDlg.h"

// FormSelectDlg dialog

IMPLEMENT_DYNAMIC(FormSelectDlg, CDialog)

FormSelectDlg::FormSelectDlg(CWnd*         pParent
                             ,CString      p_base
                             ,CString      name
                             ,CString      size
                             ,bool         multiple
                             ,FormOptions* options
                             ,HtmlElement* elem)
              :CDialog(FormSelectDlg::IDD, pParent)
              ,m_name(name)
              ,m_size(size)
              ,m_multiple(multiple)
              ,m_options(options)
              ,m_elem(elem)
              ,m_base(p_base)
{
}

FormSelectDlg::~FormSelectDlg()
{
}

void FormSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
  DDX_Text      (pDX,IDC_FS_NAME,      m_name);
  DDX_Text      (pDX,IDC_FS_SIZE,      m_size);
  DDX_Control   (pDX,IDC_SPIN,         m_spinSize);
  DDX_Control   (pDX,IDC_FS_MULTIPLE,  m_buttonMultiple);
  DDX_Control   (pDX,IDC_FS_LIST,      m_list);
  DDX_Text      (pDX,IDC_FS_NEW_OPTION,m_newOption);
  DDX_Text      (pDX,IDC_FS_NEW_VALUE, m_newValue);
  DDX_Control   (pDX,IDC_FS_NEW_DEF,   m_buttonNewSelected);
  DDX_Control   (pDX,IDC_FS_UP,        m_buttonUp);
  DDX_Control   (pDX,IDC_FS_DOWN,      m_buttonDown);
  DDX_Control   (pDX,IDC_FS_NEW,       m_buttonNew);
  DDX_Control   (pDX,IDC_FS_DELETE,    m_buttonDelete);
  DDX_Control   (pDX,IDC_ID,           m_buttonID);
  DDX_Control   (pDX,IDC_EVENTS,       m_buttonEvents);
  DDX_Control   (pDX,IDC_BUT_STYLE,    m_buttonStyle);
  DDX_Control   (pDX,IDOK,             m_buttonOK);
  DDX_Control   (pDX,IDCANCEL,         m_buttonCancel);
  DDX_Control   (pDX,IDC_FS_HELP,      m_buttonHelp);

  if(pDX->m_bSaveAndValidate == Data2Controls)
  {
    int item = m_list.GetNextItem(-1,LVNI_SELECTED);
    int num  = m_list.GetItemCount();
    m_buttonUp.EnableWindow(item > 0);
    m_buttonDown.EnableWindow(item < (num-1));

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
  }
}

BEGIN_MESSAGE_MAP(FormSelectDlg, CDialog)
  ON_EN_KILLFOCUS (IDC_FS_NAME,           OnEnChangeFsName)
  ON_EN_KILLFOCUS (IDC_FS_SIZE,           OnEnChangeFsSize)
  ON_BN_CLICKED   (IDC_FS_MULTIPLE,       OnBnClickedFsMultiple)
  ON_NOTIFY       (NM_DBLCLK,IDC_FS_LIST, OnLvnItemDblClick)
  ON_NOTIFY       (NM_CLICK, IDC_FS_LIST, OnLvnItemClick)
  ON_EN_KILLFOCUS (IDC_FS_NEW_OPTION,     OnEnChangeFsNewOption)
  ON_EN_KILLFOCUS (IDC_FS_NEW_VALUE,      OnEnChangeFsNewValue)
  ON_BN_CLICKED   (IDC_FS_NEW_DEF,        OnBnClickedFsNewDef)
  ON_BN_CLICKED   (IDC_FS_UP,             OnBnClickedFsUp)
  ON_BN_CLICKED   (IDC_FS_NEW,            OnBnClickedFsNew)
  ON_BN_CLICKED   (IDC_FS_DELETE,         OnBnClickedFsDelete)
  ON_BN_CLICKED   (IDC_FS_DOWN,           OnBnClickedFsDown)
  ON_BN_CLICKED   (IDC_ID,                OnBnClickedId)
  ON_BN_CLICKED   (IDC_EVENTS,            OnBnClickedEvents)
  ON_BN_CLICKED   (IDC_BUT_STYLE,         OnBnClickedButStyle)
  ON_BN_CLICKED   (IDOK,                  OnBnClickedOk)
  ON_BN_CLICKED   (IDCANCEL,              OnBnClickedCancel)
  ON_BN_CLICKED   (IDC_FS_HELP,           OnBnClickedFsHelp)
  ON_NOTIFY       (UDN_DELTAPOS, IDC_SPIN,OnDeltaposSpin)
END_MESSAGE_MAP()

BOOL
FormSelectDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EDITLABELS);

  m_list.InsertColumn(0,"Display",  LVCFMT_LEFT,100);
  m_list.InsertColumn(1,"Value",    LVCFMT_LEFT,100);
  m_list.InsertColumn(2,"Selected" ,LVCFMT_LEFT,60);
  DisplayList(0);
  // Set multiple
  m_buttonMultiple.SetCheck(m_multiple);

  return FALSE;
}

void
FormSelectDlg::DisplayList(int p_focusRow)
{
  m_list.DeleteAllItems();

  for(unsigned int ind = 0; ind < m_options->size(); ++ind)
  {
    FormOption opt = (*m_options)[ind];
    m_list.InsertItem(LVIF_TEXT|LVIF_STATE, ind, opt.m_display, 0, 0, 0, 0);
    m_list.SetItemText(ind,1,opt.m_value);
    m_list.SetItemText(ind,2,opt.m_selected ? "1" : "0");
  }
  m_list.SetFocus();
  m_list.SetItemState(p_focusRow,LVIS_SELECTED,LVIS_SELECTED);
  // Update must be later to set up/down buttons
  UpdateData(Data2Controls);
}

void
FormSelectDlg::CheckSizeError()
{
  if(atoi(m_size) == 1 && m_multiple)
  {
    theApp.MessageBox("Selection dialogs with a size of 1, cannot be multiple-select","Select size",MB_OK|MB_ICONERROR);
    m_multiple = false;
    m_buttonMultiple.SetCheck(FALSE);
  }
}

// FormSelectDlg message handlers

void 
FormSelectDlg::OnEnChangeFsName()
{
  CWnd* w = GetDlgItem(IDC_FS_NAME);
  w->GetWindowText(m_name);
}

void 
FormSelectDlg::OnEnChangeFsSize()
{
  UpdateData(Controls2Data);
  if(atoi(m_size) < 1)
  {
    m_size = "1";
    UpdateData(Data2Controls);
  }
  CheckSizeError();
}

void FormSelectDlg::OnDeltaposSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
  LPNMUPDOWN pUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
  int size = atoi(m_size);
  if((pUpDown->iPos - pUpDown->iDelta) > 0)
  {
    size = pUpDown->iPos - pUpDown->iDelta;
  }
  m_size.Format("%d",size);
  CheckSizeError();
  UpdateData(Data2Controls);
  *pResult = 1;
}

void 
FormSelectDlg::OnBnClickedFsMultiple()
{
  m_multiple = (m_buttonMultiple.GetCheck() == TRUE);
  CheckSizeError();
}

void 
FormSelectDlg::OnLvnItemClick(NMHDR* /*pNMHDR*/, LRESULT *pResult)
{
  //LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
  // TODO: Add your control notification handler code here
  //CEdit* pmyEdit = m_list.EditLabel(pNMLV->iItem);
  *pResult = 0;
  UpdateData(Data2Controls);
}

void
FormSelectDlg::OnLvnItemDblClick(NMHDR* /*pNMHDR*/, LRESULT *pResult)
{
  int item  = m_list.GetNextItem(-1,LVNI_SELECTED);
  int num   = m_list.GetItemCount();
  if(item < num)
  {
    int count = item;
    FormOptions::iterator it = m_options->begin();
    while(count--) ++it;
    FormOption& option = *(it);

    OptionDlg dlg(this,&option);
    if(dlg.DoModal() == IDOK)
    {
      // Redisplay this item.
      DisplayList(item);
    }
  }
  *pResult = 0;
  UpdateData(Data2Controls);
}

void 
FormSelectDlg::OnEnChangeFsNewOption()
{
  CWnd* w = GetDlgItem(IDC_FS_NEW_OPTION);
  w->GetWindowText(m_newOption);
}

void 
FormSelectDlg::OnEnChangeFsNewValue()
{
  CWnd* w = GetDlgItem(IDC_FS_NEW_VALUE);
  w->GetWindowText(m_newValue);
}

void 
FormSelectDlg::OnBnClickedFsNewDef()
{
  m_newSelected = (m_buttonNewSelected.GetCheck() == TRUE);
}

void 
FormSelectDlg::OnBnClickedFsNew()
{
  if(m_newOption.IsEmpty())
  {
    theApp.MessageBox("Fill in a new option value (first field) first","Error",MB_OK|MB_ICONHAND);
    return;
  }
  FormOption opt;
  opt.m_display  = m_newOption;
  opt.m_value    = m_newValue;
  opt.m_selected = m_newSelected;
  m_options->push_back(opt);
  int count = m_options->size();
  DisplayList(count - 1);
}

void 
FormSelectDlg::OnBnClickedFsDelete()
{
  int item  = m_list.GetNextItem(-1,LVNI_SELECTED);
  int num   = m_list.GetItemCount();
  int count = item;
  if(count >= 0 && count < num)
  {
    FormOptions::iterator it = m_options->begin();
    while(count--) ++it;
    m_options->erase(it);
  }
  DisplayList(item);
}

void 
FormSelectDlg::OnBnClickedFsUp()
{
  int item = m_list.GetNextItem(-1,LVNI_SELECTED);
  int num  = m_list.GetItemCount();
  if(item && (item < num))
  {
    int prev = item - 1;
    int row  = prev;
    FormOptions::iterator it1 = m_options->begin();
    FormOptions::iterator it2 = m_options->begin();

    while(prev--) it1++;
    while(item--) it2++;
    CString opt,val;
    bool    sel;
    opt = it1->m_display;
    val = it1->m_value;
    sel = it1->m_selected;
    it1->m_display  = it2->m_display;
    it1->m_value    = it2->m_value;
    it1->m_selected = it2->m_selected;
    it2->m_display  = opt;
    it2->m_value    = val;
    it2->m_selected = sel;

    DisplayList(row);
  }
}

void 
FormSelectDlg::OnBnClickedFsDown()
{
  int item = m_list.GetNextItem(-1,LVNI_SELECTED);
  int num  = m_list.GetItemCount();
  if(item >= 0 && item < (num-1))
  {
    int next = item + 1;
    int row  = next;
    FormOptions::iterator it1 = m_options->begin();
    FormOptions::iterator it2 = m_options->begin();

    while(item--) it1++;
    while(next--) it2++;
    CString opt,val;
    bool    sel;
    opt = it1->m_display;
    val = it1->m_value;
    sel = it1->m_selected;
    it1->m_display  = it2->m_display;
    it1->m_value    = it2->m_value;
    it1->m_selected = it2->m_selected;
    it2->m_display  = opt;
    it2->m_value    = val;
    it2->m_selected = sel;

    DisplayList(row);
  }
}

void 
FormSelectDlg::OnBnClickedOk()
{
  OnOK();
}

void 
FormSelectDlg::OnBnClickedCancel()
{
  OnCancel();
}

void FormSelectDlg::OnBnClickedId()
{
  GeneralIDDlg dlg(this,"select",m_elem);
  dlg.DoModal();
}

void 
FormSelectDlg::OnBnClickedEvents()
{
  TagEventsDlg dlg(this,m_elem,"SELECT");
  dlg.DoModal();
  UpdateData(Data2Controls);
}

void FormSelectDlg::OnBnClickedButStyle()
{
  if(!m_elem->HasStyle())
  {
    m_elem->SetStyle();
  }
  CString style = m_elem->GetInlineStyle();
  style = CString("select { ") + style + "}";
  StyleSheetDlg dlg(this,m_base,"select",NULL,style);
  if(dlg.DoModal() == IDOK)
  {
    style = dlg.GetInlineStylesheet();
    style.TrimRight("}");
    style.TrimLeft("select {");
    m_elem->SetInlineStyle(style);
  }
}

void 
FormSelectDlg::OnBnClickedFsHelp()
{
  // TODO: Add your control notification handler code here
}
