//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    StyleSheetImportsDlg.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for importing a CSS Stylesheet into the project
//
#include "stdafx.h"
#include "AuthorHTML.h"
#include "StyleSheetImportsDlg.h"
#include "StyleSheetDlg.h"
#include "FileDialog.h"
#include "Misc.h"

// StyleSheetImportsDlg dialog

IMPLEMENT_DYNAMIC(StyleSheetImportsDlg, CDialog)
StyleSheetImportsDlg::StyleSheetImportsDlg(CWnd* pParent
                                          ,CssStyleSheet* css
                                          ,CString base)
                     :CDialog(StyleSheetImportsDlg::IDD, pParent)
                     ,m_css(css)
                     ,m_base(base)
                     ,m_canApply(false)
{
}

StyleSheetImportsDlg::~StyleSheetImportsDlg()
{
}

void StyleSheetImportsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
  DDX_Control(pDX,IDC_IMPORTS,    m_list);
  DDX_Control(pDX,IDC_IMP_UP,     m_buttonUp);
  DDX_Control(pDX,IDC_EDIT,       m_buttonEdit);
  DDX_Control(pDX,IDC_IMP_NEW,    m_buttonNew);
  DDX_Control(pDX,IDC_IMP_DELETE, m_buttonDelete);
  DDX_Control(pDX,IDC_IMP_DOWN,   m_buttonDown);
  DDX_Control(pDX,IDOK,           m_buttonOK);
  DDX_Control(pDX,IDCANCEL,       m_buttonCancel);
  DDX_Control(pDX,IDC_IMP_HELP,   m_buttonHelp);

  if(pDX->m_bSaveAndValidate == Data2Controls)
  {
    int num = m_list.GetCount();
    int ind = m_list.GetCurSel();
    m_buttonUp    .EnableWindow(ind > 0);
    m_buttonDown  .EnableWindow(ind < (num-1));
    m_buttonEdit  .EnableWindow(ind >= 0);
    m_buttonDelete.EnableWindow(ind >= 0);
  }
}

BEGIN_MESSAGE_MAP(StyleSheetImportsDlg, CDialog)
  ON_LBN_SELCHANGE(IDC_IMPORTS,   OnLbnSelchangeImports)
  ON_LBN_DBLCLK   (IDC_IMPORTS,   OnBnClickedEdit)
  ON_BN_CLICKED   (IDC_IMP_UP,    OnBnClickedImpUp)
  ON_BN_CLICKED   (IDC_EDIT,      OnBnClickedEdit)
  ON_BN_CLICKED   (IDC_IMP_NEW,   OnBnClickedImpNew)
  ON_BN_CLICKED   (IDC_IMP_DELETE,OnBnClickedImpDelete)
  ON_BN_CLICKED   (IDC_IMP_DOWN,  OnBnClickedImpDown)
  ON_BN_CLICKED   (IDOK,          OnBnClickedOk)
  ON_BN_CLICKED   (IDCANCEL,      OnBnClickedCancel)
  ON_BN_CLICKED   (IDC_IMP_HELP,  OnBnClickedImpHelp)
END_MESSAGE_MAP()

BOOL
StyleSheetImportsDlg::OnInitDialog()
{
  CDialog::OnInitDialog();
  FillPage();
  UpdateData(Data2Controls);
  return TRUE;
}

void
StyleSheetImportsDlg::FillPage()
{
  m_list.ResetContent();
  for(unsigned int ind = 0; ind < m_css->m_import.size(); ++ind)
  {
    CString imp = m_css->m_import[ind].c_str();
    imp.TrimLeft("url(");
    imp.TrimRight(")");
    imp.TrimLeft('\"');
    imp.TrimRight('\"');
    m_list.AddString(imp);
  }
}

void
StyleSheetImportsDlg::UpdateProperties()
{
  if(m_canApply)
  {
    m_css->m_import.clear();
    for(int ind = 0; ind < m_list.GetCount(); ++ind)
    {
      CString imp;
      m_list.GetText(ind,imp);
      imp = CString("\"") + imp + "\"";
      m_css->m_import.push_back((string)imp);
    }
  }
}

// StyleSheetImportsDlg message handlers

void 
StyleSheetImportsDlg::OnLbnSelchangeImports()
{
  UpdateData(Data2Controls);
}

void 
StyleSheetImportsDlg::OnBnClickedImpUp()
{
  int ind = m_list.GetCurSel();
  if(ind >= 0)
  {
    --ind;
    CString first,second;
    m_list.GetText(ind,first);
    m_list.GetText(ind+1,second);
    m_list.DeleteString(ind);
    m_list.DeleteString(ind);
    m_list.InsertString(ind,first);
    m_list.InsertString(ind,second);
    UpdateData(Data2Controls);
    m_list.SetCurSel(ind);
    m_canApply = true;
  }
}

void 
StyleSheetImportsDlg::OnBnClickedEdit()
{
  int num = m_list.GetCount();
  int ind = m_list.GetCurSel();
  if((ind >= 0) && (ind < num ))
  {
    CString file;
    m_list.GetText(ind,file);
    CString path = m_base + file;

    CssStyleSheet css;
    // Empty file (true) is alsoo OK, could be a new CSS Stylesheet!
    if(css.SetFile(path,true))
    {
      CString styleText;
      CString type = "Style sheet in imported CSS file: " + path;
      StyleSheetDlg sheet(this,m_base,type,&css,styleText,false);
      if(sheet.DoModal() == IDOK)
      {
        if(sheet.DidApply())
        {
          m_canApply = true;
        }
      }

    }
    else
    {
      string logs = css.print_logs();
      theApp.MessageBox(logs.c_str(),"Errors in CSS Stylesheet",MB_OK|MB_ICONSTOP);
    }
  }
}

void 
StyleSheetImportsDlg::OnBnClickedImpNew()
{
  DocFileDialog diag(true
                    ,"Import a CSS Stylesheet"
                    ,"css"
                    ,""
                    ,0
                    ,"Cascading style sheet (*.css)|*.css|");
  if(diag.DoModal() == IDOK)
  {
    CString file = diag.GetChosenFile();
    CString relative;
    if(Misc::MakeRelativePathname(m_base,file,relative))
    {
      file = relative;
    }
    m_list.AddString(file);
    UpdateData(Data2Controls);
    m_canApply = true;
  }
}

void 
StyleSheetImportsDlg::OnBnClickedImpDelete()
{
  int num = m_list.GetCount();
  int ind = m_list.GetCurSel();
  if((ind >= 0) && (ind < num ))
  {
    CString file;
    m_list.GetText(ind,file);
    CString mess;
    mess.Format("Do you sure that you want to unlink the CSS imported file [%s]?\n"
                "(The file will not be deleted from disk!)",file.GetString());
    if(theApp.MessageBox(mess,"Question",MB_YESNO|MB_ICONQUESTION) == IDYES)
    {
      m_list.DeleteString(ind);
      UpdateData(Data2Controls);
      m_canApply = true;
    }
  }
}

void 
StyleSheetImportsDlg::OnBnClickedImpDown()
{
  int num = m_list.GetCount();
  int ind = m_list.GetCurSel();
  if(ind < (num -1))
  {
    CString first,second;
    m_list.GetText(ind,first);
    m_list.GetText(ind+1,second);
    m_list.DeleteString(ind);
    m_list.DeleteString(ind);
    m_list.InsertString(ind,first);
    m_list.InsertString(ind,second);
    UpdateData(Data2Controls);
    m_list.SetCurSel(ind+1);
    m_canApply = true;
  }
}

void 
StyleSheetImportsDlg::OnBnClickedOk()
{
  UpdateProperties();
  OnOK();
}

void 
StyleSheetImportsDlg::OnBnClickedCancel()
{
  OnCancel();
}

void 
StyleSheetImportsDlg::OnBnClickedImpHelp()
{
  // TODO: Add your control notification handler code here
}
