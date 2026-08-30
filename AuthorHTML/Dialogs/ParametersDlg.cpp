//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    ParametersDlg.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for managing the <PARAM> tag of an <OBJECT> in HTML
//
#include "stdafx.h"
#include "AuthorHTML.h"
#include "ParametersDlg.h"

// ParametersDlg dialog

IMPLEMENT_DYNAMIC(ParametersDlg, CDialog)

ParametersDlg::ParametersDlg(CWnd* p_parent,HtmlObject* p_object)
              :CDialog(ParametersDlg::IDD, p_parent)
              ,m_object(p_object)
              ,m_maxParam(0)
{
}

ParametersDlg::~ParametersDlg()
{
}

void ParametersDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
  DDX_Control(pDX,IDC_LIST,         m_list);
  DDX_Control(pDX,IDC_DELETE,       m_buttonDelete);
  DDX_Control(pDX,IDOK,             m_buttonOK);
  DDX_Control(pDX,IDC_BUTAPPLY,     m_buttonApply);
  DDX_Control(pDX,IDC_HELPACOMMAND, m_buttonHelp);
}


BEGIN_MESSAGE_MAP(ParametersDlg, CDialog)
  ON_NOTIFY    (GVN_ENDLABELEDIT,  IDC_LIST,OnEndInPlaceEdit)
  ON_NOTIFY    (GVN_BEGINLABELEDIT,IDC_LIST,OnBeginEdit)
  ON_BN_CLICKED(IDC_DELETE,                 OnBnClickedDelete)
  ON_BN_CLICKED(IDOK,                       OnBnClickedOk)
  ON_BN_CLICKED(IDC_BUTAPPLY,               OnBnClickedButapply)
  ON_BN_CLICKED(IDCANCEL,                   OnBnClickedCancel)
  ON_BN_CLICKED(IDC_HELPACOMMAND,           OnBnClickedHelpacommand)
END_MESSAGE_MAP()

BOOL
ParametersDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  CString name ("Parameter name");
  CString value("Parameter value");
  m_list.InsertColumn(name);
  m_list.InsertColumn(value);
  m_list.SetEditable(TRUE);
  m_list.SetListMode(TRUE);
  m_list.EnableDragAndDrop(FALSE);
  m_list.SetSingleRowSelection(TRUE);
  m_list.SetSingleColSelection(TRUE);
  m_list.SetRowCount(1);
  m_list.SetFixedRowCount(1);
  m_list.InsertItem(0,0,name, DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS|DT_NOPREFIX);
  m_list.InsertItem(0,1,value,DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS|DT_NOPREFIX);
  m_list.SetColumnWidth(0,200);
  m_list.ExpandLastColumn();

  FillPage();

  UpdateData(FALSE);

  return TRUE;
}

void
ParametersDlg::FillPage()
{
  // Get keyword items in the list
  ParameterMap* map = m_object->GetParameterMap();
  ParameterMap::iterator it = map->begin();
  while(it != map->end())
  {
    CString name  = it->first;
    CString value = it->second;
    int row = m_list.GetRowCount();
    m_list.InsertRow("");
    m_list.InsertItem(row,0,name, DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS|DT_NOPREFIX);
    m_list.InsertItem(row,1,value,DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS|DT_NOPREFIX);
    ++m_maxParam;
    // Next parameter
    ++it;
  }
  m_list.InsertRow("");
  m_list.ExpandLastColumn();
  m_list.Refresh();
}

void
ParametersDlg::UpdateObject()
{
  // Read parameter list
  for(int ind = 1; ind < m_list.GetRowCount(); ++ind)
  {
    CString name  = m_list.GetItemText(ind,0);
    CString value = m_list.GetItemText(ind,1);

    CString item;
    if(!name.IsEmpty() && !value.IsEmpty())
    {
      m_object->SetParameter(name,value);
    }
    else if(!name.IsEmpty() && value.IsEmpty())
    {
      m_object->RemoveParameter(name);
    }
  }
  // Nothing changed anymore
  m_changed = false;
}

// ParametersDlg message handlers

void 
ParametersDlg::OnBnClickedDelete()
{
  CString message;
  CCellID cell = m_list.GetFocusCell();
  CString text = m_list.GetItemText(cell.row,0);
  message.Format("Do you want to delete the parameter '%s'?\n",text.GetString());
  if(theApp.MessageBox(message,"Delete",MB_YESNO|MB_ICONQUESTION) == IDYES)
  {
    // Removed immediately. Otherwise parameter name not known!!
    m_object->RemoveParameter(text);

    m_list.DeleteRow(cell.row);
    m_list.Refresh();
    m_changed = true;
    UpdateData(FALSE);
  }
}

void
ParametersDlg::OnBeginEdit(NMHDR *pNMHDR, LRESULT *pResult)
{
  NM_GRIDVIEW *cel = (NM_GRIDVIEW*)pNMHDR;
  int col = cel->iColumn;
  int row = cel->iRow;
  m_saveEdit = m_list.GetItemText(row,col);
  *pResult = 0;
}

void
ParametersDlg::OnEndInPlaceEdit(NMHDR *pNMHDR, LRESULT *pResult)
{
  NM_GRIDVIEW *cel = (NM_GRIDVIEW*)pNMHDR;
  int col = cel->iColumn;
  int row = cel->iRow;

  CString text = m_list.GetItemText(row,col);
  if(text.CompareNoCase(m_saveEdit))
  {
    m_changed = true;
  }
  int max = m_list.GetRowCount() - 1;
  text = m_list.GetItemText(max,0);
  if(!text.IsEmpty())
  {
    m_list.InsertRow("");
    m_list.Refresh();
  }
  m_saveEdit.Empty();
  UpdateData(FALSE);

  *pResult = 0;
}

void 
ParametersDlg::OnBnClickedOk()
{
  UpdateObject();
  CDialog::OnOK();
}

void 
ParametersDlg::OnBnClickedButapply()
{
  UpdateObject();
}

void 
ParametersDlg::OnBnClickedCancel()
{
  CDialog::OnCancel();
}

void 
ParametersDlg::OnBnClickedHelpacommand()
{
  // TODO: Add your control notification handler code here
}
