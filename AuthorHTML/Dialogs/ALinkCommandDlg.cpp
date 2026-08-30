//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    ALinkCommandDlg.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog to edit the associative link command <A> in the HTML file
//
#include "stdafx.h"
#include "AuthorHTML.h"
#include "ALinkCommandDlg.h"
#include "ProjectFile.h"
#include "FindTopicDlg.h"
#include "FileDialog.h"
#include "GeneralIDDlg.h"
#include "StyleSheetDlg.h"
#include "afxdialogex.h"

// ALinkCommandDlg dialog

IMPLEMENT_DYNAMIC(ALinkCommandDlg, CDialog)

ALinkCommandDlg::ALinkCommandDlg(CWnd* p_parent,CString& p_base,HtmlObject* p_object)
	              :CDialog(ALinkCommandDlg::IDD, p_parent)
                ,m_base(p_base)
                ,m_object(p_object)
                ,m_changed(false)
                ,m_maxItemInObject(0)
{
}

ALinkCommandDlg::~ALinkCommandDlg()
{
}

void 
ALinkCommandDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
  DDX_Control(pDX,IDC_INDEXTYPE,   m_comboType);
  DDX_Text   (pDX,IDC_TEXT,        m_text);
  DDX_Text   (pDX,IDC_CHMFILE,     m_chmFilename);
  DDX_Control(pDX,IDC_CHM_SEARCH,  m_buttonCHM);
  DDX_Text   (pDX,IDC_DEFAULT,     m_defaultTopic);
  DDX_Control(pDX,IDC_DEF_SEARCH,  m_buttonDefTopic);
  DDX_Control(pDX,IDC_BUTTONTYPE,  m_comboButton);
  DDX_Text   (pDX,IDC_ICONBITMAP,  m_iconbitmap);
  DDX_Control(pDX,IDC_BMP_SEARCH,  m_buttonBMPSearch);
  DDX_Control(pDX,IDC_MENU,        m_comboMenu);
  DDX_Text   (pDX,IDC_FRAMETITLE,  m_frame);
  DDX_Text   (pDX,IDC_HEIGHT,      m_height);
  DDX_Control(pDX,IDC_SPINH,       m_spinHeight);
  DDX_Text   (pDX,IDC_WIDTH,       m_width);
  DDX_Control(pDX,IDC_SPINW,       m_spinWidth);
  DDX_Control(pDX,IDC_LIST,        m_list);
  DDX_Control(pDX,IDC_ID,          m_buttonID);
  DDX_Control(pDX,IDC_STYLE,       m_buttonStyle);
  DDX_Control(pDX,IDOK,            m_buttonOK);
  DDX_Control(pDX,IDC_BUTAPPLY,    m_buttonApply);
  DDX_Control(pDX,IDCANCEL,        m_buttonCancel);
  DDX_Control(pDX,IDC_HELPACOMMAND,m_buttonHelp);

  if(pDX->m_bSaveAndValidate == Data2Controls)
  {
    CString def,text;

    m_buttonID.GetWindowText(def);
    text = m_object->HasIdentity() ? "[ &ID ]" : "&ID";
    if(def != text) m_buttonID.SetWindowText(text);

    m_buttonStyle.GetWindowText(def);
    text = m_object->HasStyle() ? "[ &Style ]" : "&Style";
    if(def != text) m_buttonStyle.SetWindowText(text);
  }

  m_buttonApply.EnableWindow(m_changed);
  // Text only in use with text links or text buttons
  CWnd* t = GetDlgItem(IDC_TEXT);
  t->EnableWindow(m_button < BUTTON_CHICLET);

  // Icon or bitmap on button
  CWnd* b = GetDlgItem(IDC_ICONBITMAP);
  b->EnableWindow(m_button >= BUTTON_BITMAP);
  m_buttonBMPSearch.EnableWindow(m_button >= BUTTON_BITMAP);
}

BEGIN_MESSAGE_MAP(ALinkCommandDlg, CDialog)
  ON_CBN_SELCHANGE(IDC_INDEXTYPE,               OnCbnSelchangeIndextype)
  ON_EN_CHANGE    (IDC_TEXT,                    OnEnChangeText)
  ON_EN_CHANGE    (IDC_CHMFILE,                 OnEnChangeChmfile)
  ON_BN_CLICKED   (IDC_CHM_SEARCH,              OnBnClickedChmSearch)
  ON_EN_CHANGE    (IDC_DEFAULT,                 OnEnChangeDefault)
  ON_BN_CLICKED   (IDC_DEF_SEARCH,              OnBnClickedDefSearch)
  ON_CBN_SELCHANGE(IDC_BUTTONTYPE,              OnCbnSelchangeButtontype)
  ON_EN_CHANGE    (IDC_ICONBITMAP,              OnEnChangeIconbitmap)
  ON_BN_CLICKED   (IDC_BMP_SEARCH,              OnBnClickedBmpSearch)
  ON_CBN_SELCHANGE(IDC_MENU,                    OnCbnSelchangeMenu)
  ON_EN_CHANGE    (IDC_FRAMETITLE,              OnEnChangeFrametitle)
  ON_EN_CHANGE    (IDC_HEIGHT,                  OnEnChangeHeight)
  ON_NOTIFY       (UDN_DELTAPOS,      IDC_SPINH,OnDeltaposSpinh)
  ON_EN_CHANGE    (IDC_WIDTH,                   OnEnChangeWidth)
  ON_NOTIFY       (UDN_DELTAPOS,      IDC_SPINW,OnDeltaposSpinw)
  ON_NOTIFY       (LVN_ITEMCHANGED,   IDC_LIST, OnLvnItemchangedList)
  ON_NOTIFY       (GVN_ENDLABELEDIT,  IDC_LIST, OnEndInPlaceEdit)
  ON_NOTIFY       (GVN_BEGINLABELEDIT,IDC_LIST, OnBeginEdit)
  ON_BN_CLICKED   (IDC_KEYWORD,                 OnBnClickedKeyword)
  ON_BN_CLICKED   (IDC_DELETE,                  OnBnClickedDelete)
  ON_BN_CLICKED   (IDC_ID,                      OnBnClickedId)
  ON_BN_CLICKED   (IDC_STYLE,                   OnBnClickedStyle)
  ON_BN_CLICKED   (IDOK,                        OnBnClickedOk)
  ON_BN_CLICKED   (IDC_BUTAPPLY,                OnBnClickedButapply)
  ON_BN_CLICKED   (IDCANCEL,                    OnBnClickedCancel)
  ON_BN_CLICKED   (IDC_HELPACOMMAND,            OnBnClickedHelpacommand)
END_MESSAGE_MAP()

BOOL
ALinkCommandDlg::OnInitDialog()
{
  CDialog::OnInitDialog();


  // Prepare keyword list
  CString labelText("Associative keyword links");
  m_list.InsertColumn(labelText);
  m_list.SetEditable(TRUE);
  m_list.SetListMode(TRUE);
  m_list.EnableDragAndDrop(FALSE);
  m_list.SetSingleRowSelection(TRUE);
  m_list.SetSingleColSelection(TRUE);
  m_list.SetRowCount(1);
  m_list.SetFixedRowCount(1);
  m_list.InsertItem(0,0,labelText,DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS|DT_NOPREFIX);
  m_list.SetColumnWidth(0,500);

  FillPage();

  // Set type combo
  m_comboType.AddString("Composite index keyword");
  m_comboType.AddString("Associative link keyword");
  m_comboType.SetCurSel(m_type == "A") ? 1 : 0;

  // Fill the button combo
  m_comboButton.AddString("No button");       // 0
  m_comboButton.AddString("Text button");     // 1
  m_comboButton.AddString("Chiclet button");  // 2
  m_comboButton.AddString("Bitmap button");   // 3
  m_comboButton.AddString("Icon button");     // 4
//m_comboButton.AddString("Shortcut button"); // 5
  m_comboButton.SetCurSel(m_button);

  m_comboMenu.AddString("Default");
  m_comboMenu.AddString("Menu");
  m_comboMenu.AddString("Dialog");
  m_comboMenu.SetCurSel(m_menu);

  UpdateData(false);

  return TRUE;
}

void
ALinkCommandDlg::EnsureCHM()
{
  if(m_chmFilename.IsEmpty())
  {
    ProjectFile* project = theApp.GetProjectFile();
    if(project)
    {
      m_chmFilename = project->GetCompiledName();
    }
  }
}

bool
ALinkCommandDlg::CheckCommand()
{
  CString title("Associative link error");
  CString errors;

  if(m_chmFilename.IsEmpty())
  {
    errors += "Associative link must have a CHM file name to refer to\n";
  }
  if(!errors.IsEmpty())
  {
    theApp.MessageBox(errors,title,MB_OK|MB_ICONERROR);
    return false;
  }
  return true;
}

void
ALinkCommandDlg::FillPage()
{
  // Reset everything
  m_text.Empty();
  m_button = BUTTON_NOBUTTON;
  m_menu   = MENU_DEFAULT;

  // Get Command
  CString command = m_object->GetParameter("Command");
  int pos = command.Find(',');
  if(pos >= 0)
  {
    CString menu = command.Mid(pos + 1);
    command = command.Left(pos);
    if(menu.CompareNoCase("menu") == 0)
    {
      m_menu = MENU_MENU;
    }
    if(menu.CompareNoCase("dialog") == 0)
    {
      m_menu = MENU_DIALOG;
    }
  }
  // Alink or KLink
  m_type = (command.CompareNoCase("ALink") == 0) ? "A" : "K";

  // Get button and text
  CString button = m_object->GetParameter("Button");
  if(button.IsEmpty())
  {
    m_button = BUTTON_CHICLET;
  }
// Does not work due to a bug in IE8
//else if(button.CompareNoCase("Bitmap:shortcut") == 0)
//{
//  m_button = BUTTON_SHORTCUT;
//}
  else if(button.Left(7).CompareNoCase("Bitmap:") == 0)
  {
    m_iconbitmap = button.Mid(7);
    m_button = BUTTON_BITMAP;
  }
  else if(button.Left(5).CompareNoCase("Icon:") == 0)
  {
    m_iconbitmap = button.Mid(5);
    m_button = BUTTON_ICON;
  }
  else
  {
    // Non empty text, not beeing icon/bitmap/shortcut
    m_button = BUTTON_TEXTBUTTON;
    m_text   = button;
  }
  // Get CHM file
  m_chmFilename = m_object->GetParameter("Item1");
  // Default topic
  m_defaultTopic = m_object->GetParameter("Default Topic");
  // Text with no button
  CString text = m_object->GetParameter("Text");
  if(!text.IsEmpty() && m_text.IsEmpty())
  {
    m_text = text;
  }
  // frame
  m_frame = m_object->GetParameter("Frame");
  // width & height
  // DO NOT USE GETPROPERTY. IGNORE STYLESHEETS
  m_width  = m_object->GetAttribute("width");
  m_height = m_object->GetAttribute("Height");

  // Get keyword items in the list
  ParameterMap* map = m_object->GetParameterMap();
  ParameterMap::iterator it = map->begin();
  while(it != map->end())
  {
    CString name  = it->first;
    CString value = it->second;
    if((name.Left(4).CompareNoCase("Item") == 0) &&
        name.CompareNoCase("Item1"))
    {
      int row = m_list.GetRowCount();
      m_list.InsertRow("");
      m_list.InsertItem(row,0,value,DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS|DT_NOPREFIX);
      ++m_maxItemInObject;
    }
    // Next parameter
    ++it;
  }
  m_list.InsertRow("");
  m_list.ExpandLastColumn();
  m_list.Refresh();

  // Make sure we have an "Item1"
  EnsureCHM();
}


void
ALinkCommandDlg::UpdateObject()
{
  if(m_changed)
  {
    // Set type of command (must be set already)
    CString textvalue = (m_type == "A") ? "ALink" : "KLink";
    switch(m_menu)
    {
      case MENU_DEFAULT: break;
      case MENU_MENU:    textvalue += ",menu";   break;
      case MENU_DIALOG:  textvalue += ",dialog"; break;
    }
    m_object->SetParameter("Command",textvalue);
    // Set button text
    switch(m_button)
    {
      case BUTTON_NOBUTTON:   m_object->RemoveParameter("Button");
                              break;
      case BUTTON_TEXTBUTTON: m_object->SetParameter("Button",m_text);
                              break;
      case BUTTON_CHICLET:    m_object->SetParameter("Button","");
                              m_text = "";
                              break;
      case BUTTON_BITMAP:     textvalue = "Bitmap:" + m_iconbitmap;
                              m_object->SetParameter("Button",textvalue);
                              m_text = "";
                              break;
      case BUTTON_ICON:       textvalue = "Icon:" + m_iconbitmap;
                              m_object->SetParameter("Button",textvalue);
                              m_text = "";
                              break;
//    Does not work due to a bug in IE8
//    case BUTTON_SHORTCUT:   textvalue = "Bitmap:shortcut";
//                            m_object->SetParameter("Button",textvalue);
//                            m_text = "";
//                            break;
    }
    // Set CHM filename
    m_object->SetParameter("Item1",m_chmFilename);
    // Set optional default topic
    if(m_defaultTopic.IsEmpty())
    {
      m_object->RemoveParameter("Default Topic");
    }
    else
    {
      m_object->SetParameter("Default Topic",m_defaultTopic);
    }
    // Set text 
    if(!m_text.IsEmpty() && m_button != BUTTON_TEXTBUTTON)
    {
      m_object->SetParameter("Text",m_text);
    }
    else
    {
      m_object->RemoveParameter("Text");
    }
    // Frame title
    if(m_frame.IsEmpty())
    {
      m_object->RemoveParameter("Frame");
    }
    else
    {
      m_object->SetParameter("Frame",m_frame);
    }
    // Width and height
    m_object->SetAttribute("width", m_width);
    m_object->SetAttribute("height",m_height);
    // Read keyword list
    for(int ind = 1; ind < m_list.GetRowCount(); ++ind)
    {
      CString word = m_list.GetItemText(ind,0);
      CString item;
      if(!word.IsEmpty())
      {
        item.Format("Item%d",ind + 1);
        m_object->SetParameter(item,word);
      }
    }
    // In case we have less keywords than originally in object
    for(int ind = m_list.GetRowCount() - 1;ind <= m_maxItemInObject; ++ind)
    {
      CString item;
      item.Format("Item%d",ind);
      m_object->RemoveParameter(item);
    }
    // Nothing changed anymore
    m_changed = false;
  }
}

// ALinkCommandDlg message handlers

void 
ALinkCommandDlg::OnCbnSelchangeIndextype()
{
  int ind = m_comboType.GetCurSel();
  if(ind >= 0)
  {
    m_type = (ind == 0) ? "K" : "A";
    m_changed = true;
    UpdateData(FALSE);
  }
}

void 
ALinkCommandDlg::OnEnChangeText()
{
  m_changed = true;
  UpdateData();
}

void 
ALinkCommandDlg::OnEnChangeChmfile()
{
  m_changed = true;
  UpdateData();
}

void 
ALinkCommandDlg::OnBnClickedChmSearch()
{
  DocFileDialog diag(true
                    ,"Search for a documentation file"
                    ,"chm"
                    ,""
                    ,0);
  if(diag.DoModal() == IDOK)
  {
    CString chmFile = diag.GetChosenFile();
    CString relative;
    if(Misc::MakeRelativePathname(m_base,chmFile,relative))
    {
      chmFile = relative;
    }
    if(m_chmFilename.CompareNoCase(chmFile))
    {
      m_chmFilename = chmFile;
      m_changed = true;
      UpdateData(false);
    }
  }
}

void 
ALinkCommandDlg::OnEnChangeDefault()
{
  m_changed = true;
  UpdateData();
}

void 
ALinkCommandDlg::OnBnClickedDefSearch()
{
  FindTopicDlg dlg(this,m_base,false);
  if(dlg.DoModal() == IDOK)
  {
    CString defTopic = dlg.GetFoundTopic();
    if(m_defaultTopic.CompareNoCase(defTopic))
    {
      m_changed = true;
      m_defaultTopic = defTopic;
      UpdateData(false);
    }
  }
}

void 
ALinkCommandDlg::OnCbnSelchangeButtontype()
{
  int sel = m_comboButton.GetCurSel();
  if(sel >= 0)
  {
    m_changed = true;
    m_button = sel;
    if(sel >= 2)
    {
      m_text.Empty();
    }
    else
    {
      m_iconbitmap.Empty();
    }
    UpdateData(false);
  }
}

void 
ALinkCommandDlg::OnEnChangeIconbitmap()
{
  m_changed = true;
  UpdateData();
}

void 
ALinkCommandDlg::OnBnClickedBmpSearch()
{
  CString title = (m_button == BUTTON_BITMAP)
                    ? "Search for a bitmap file"
                    : "Search for an icon file";
  CString type  = (m_button == BUTTON_BITMAP)
                    ? "bmp"
                    : "ico";

  DocFileDialog diag(true,title,type,"",0);
  if(diag.DoModal() == IDOK)
  {
    CString file = diag.GetChosenFile();
    CString relative;
    if(Misc::MakeRelativePathname(m_base,file,relative))
    {
      m_iconbitmap = relative;
    }
    if(m_iconbitmap.CompareNoCase(file))
    {
      m_changed = true;
      m_iconbitmap = file;
      UpdateData(false);
    }
  }
}

void 
ALinkCommandDlg::OnCbnSelchangeMenu()
{
  m_menu = m_comboMenu.GetCurSel();
  m_changed = true;
  UpdateData(false);
}

void 
ALinkCommandDlg::OnEnChangeFrametitle()
{
  m_changed = true;
  UpdateData();
}

void 
ALinkCommandDlg::OnEnChangeHeight()
{
  m_changed = true;
  UpdateData();
}

void 
ALinkCommandDlg::OnDeltaposSpinh(NMHDR *pNMHDR, LRESULT *pResult)
{
  LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
  int high = atoi(m_height);
  high -= pNMUpDown->iDelta;
  m_height.Format("%d",high);
  m_changed = true;
  UpdateData(false);

  *pResult = 0;
}

void 
ALinkCommandDlg::OnEnChangeWidth()
{
  m_changed = true;
  UpdateData();
}

void
ALinkCommandDlg::OnDeltaposSpinw(NMHDR *pNMHDR, LRESULT *pResult)
{
  LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
  int wide = atoi(m_width);
  wide -= pNMUpDown->iDelta;
  m_width.Format("%d",wide);
  m_changed = true;
  UpdateData(false);

  *pResult = 0;
}

void 
ALinkCommandDlg::OnLvnItemchangedList(NMHDR *pNMHDR, LRESULT *pResult)
{
  LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
  // TODO: Add your control notification handler code here
  *pResult = 0;
}

void
ALinkCommandDlg::OnBeginEdit(NMHDR *pNMHDR, LRESULT *pResult)
{
  NM_GRIDVIEW *cel = (NM_GRIDVIEW*)pNMHDR;
  int col = cel->iColumn;
  int row = cel->iRow;
  m_saveEdit = m_list.GetItemText(row,col);
  *pResult = 0;
}

void
ALinkCommandDlg::OnEndInPlaceEdit(NMHDR *pNMHDR, LRESULT *pResult)
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
ALinkCommandDlg::OnBnClickedKeyword()
{
  // TODO: Add your control notification handler code here
}

void 
ALinkCommandDlg::OnBnClickedDelete()
{
  CString message;
  CCellID cell = m_list.GetFocusCell();
  CString text = m_list.GetItemText(cell.row,0);
  message.Format("Do you want to delete the associative keyword '%s'?",text.GetString());
  if(theApp.MessageBox(message,"Delete keyword?",MB_YESNO|MB_ICONQUESTION) == IDYES)
  {
    m_list.DeleteRow(cell.row);
    m_list.Refresh();
    m_changed = true;
    UpdateData(FALSE);
  }
}

void 
ALinkCommandDlg::OnBnClickedId()
{
  CString tag("object");
  HtmlElement* elem = (HtmlElement*) m_object;
  GeneralIDDlg dlg(this,tag,elem);
  dlg.DoModal();
}

void 
ALinkCommandDlg::OnBnClickedStyle()
{
  if(!m_object->HasStyle())
  {
    m_object->SetStyle();
  }
  CString tag("object");

  CString style = m_object->GetInlineStyle();
  style = tag + " { " + style + "}";
  StyleSheetDlg dlg(this,m_base,tag,NULL,style);
  if(dlg.DoModal() == IDOK)
  {
    style = dlg.GetInlineStylesheet();
    style.TrimRight("}");
    style.TrimLeft(tag);
    style.TrimLeft(" {");
    m_object->SetInlineStyle(style);
  }
}

void 
ALinkCommandDlg::OnBnClickedOk()
{
  if(CheckCommand())
  {
    UpdateObject();
    CDialog::OnOK();
  }
}

void 
ALinkCommandDlg::OnBnClickedButapply()
{
  if(CheckCommand())
  {
    UpdateObject();
    UpdateData(false);
  }
}

void 
ALinkCommandDlg::OnBnClickedCancel()
{
  CDialog::OnCancel();
}

void 
ALinkCommandDlg::OnBnClickedHelpacommand()
{
  // TODO: Add your control notification handler code here
}

