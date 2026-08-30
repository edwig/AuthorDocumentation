//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    NewFileDlg.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for creating a new documentation file
//              Probably from a template or from scratch
//
#include "stdafx.h"
#include "AuthorHTML.h"
#include "NewFileDlg.h"
#include "FileDialog.h"
#include "Misc.h"
#include "ProjectFile.h"
#include <io.h>

// NewFileDlg dialog

IMPLEMENT_DYNAMIC(NewFileDlg, CDialog)

NewFileDlg::NewFileDlg(CWnd* pParent
                      ,CString base)
           :CDialog(NewFileDlg::IDD, pParent)
           ,m_base(base)
           ,m_synchronize(true)
           ,m_error(false)
{
}

NewFileDlg::~NewFileDlg()
{
}

void NewFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
  DDX_Text(pDX,IDC_TITLE,     m_title);
  DDX_Text(pDX,IDC_FILENAME,  m_filename);
  DDX_Text(pDX,IDC_TEMPLATE,  m_template);

  DDX_Control(pDX,IDC_BUTTON_FSEARCH, m_buttonFSearch);
  DDX_Control(pDX,IDC_BTNFGO,         m_buttonFGo);
  DDX_Control(pDX,IDC_BUTTON_TSEARCH, m_buttonTSearch);
  DDX_Control(pDX,IDC_BTNTGO,         m_buttonTGo);
  DDX_Control(pDX,IDOK,               m_buttonOK);
  DDX_Control(pDX,IDCANCEL,           m_buttonCancel);
  DDX_Control(pDX,ID_HELP_NEWFILE,    m_buttonHelp);

  if(pDX->m_bSaveAndValidate == Data2Controls)
  {

  }
}

BEGIN_MESSAGE_MAP(NewFileDlg, CDialog)
  ON_EN_CHANGE   (IDC_TITLE,          &NewFileDlg::OnEnChangeTitle)
  ON_EN_KILLFOCUS(IDC_FILENAME,       &NewFileDlg::OnEnChangeFilename)
  ON_EN_KILLFOCUS(IDC_TEMPLATE,       &NewFileDlg::OnEnChangeTemplate)
  ON_BN_CLICKED  (IDC_BUTTON_FSEARCH, &NewFileDlg::OnBnClickedButtonFsearch)
  ON_BN_CLICKED  (IDC_BTNFGO,         &NewFileDlg::OnBnClickedBtnfgo)
  ON_BN_CLICKED  (IDC_BUTTON_TSEARCH, &NewFileDlg::OnBnClickedButtonTsearch)
  ON_BN_CLICKED  (IDC_BTNTGO,         &NewFileDlg::OnBnClickedBtntgo)
  ON_BN_CLICKED  (IDOK,               &NewFileDlg::OnBnClickedOk)
  ON_BN_CLICKED  (IDCANCEL,           &NewFileDlg::OnBnClickedCancel)
  ON_BN_CLICKED  (ID_HELP_NEWFILE,    &NewFileDlg::OnBnClickedHelpNewfile)
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(NewFileDlg, CDialog)
  ON_EVENT(NewFileDlg, IDC_EXPLORER, 0x00000103, OnDocumentComplete, VTS_DISPATCH VTS_VARIANT)
END_EVENTSINK_MAP()

BOOL
NewFileDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  CWnd *pWnd = GetDlgItem(IDC_EXPLORER);
  if(pWnd)
  {
    m_spBrowser = pWnd->GetControlUnknown();
  }

  FillPage();
  UpdateData(Data2Controls);
  return TRUE;
}

void
NewFileDlg::FillPage()
{
}

bool
NewFileDlg::UpdateProperties()
{
  m_error = false;
  if(m_filename.IsEmpty())
  {
    theApp.ErrorMessage("Fill in a filename to add a file to the project");
    m_error = true;
  }
  else if(m_title.IsEmpty())
  {
    theApp.ErrorMessage("Fill in a title to add the file to the project");
    m_error = true;
  }
  else
  {
    OnBnClickedBtnfgo();
  }
  if(!m_error)
  {
    if(m_template.IsEmpty())
    {
      CreateNewFile();
    }
    else
    {
      CopyTemplate();
    }
  }
  return !m_error;
}

void
NewFileDlg::CopyTemplate()
{
  CString templatename = m_base + m_template;
  if(_access(templatename,4) == -1)
  {
    CString mess = "The template [" + m_template + "] does not exist.";
    theApp.ErrorMessage(mess);
    m_error = true;
    return;
  }
  CFile theTemp;
  if(!theTemp.Open(templatename,CFile::modeRead))
  {
    CString mess = "The template [" + m_template + "] cannot be opened for reading.";
    theApp.ErrorMessage(mess);
    m_error = true;
    return;
  }
  CString filename = m_base + m_filename;
  CFile theFile;
  if(!theFile.Open(filename,CFile::modeCreate | CFile::modeReadWrite))
  {
    CString mess = "The file [" + m_filename + "] could not be created. Check your filesystem";
    theApp.ErrorMessage(mess);
    m_error = true;
    return;
  }
  // Now copy the template as a byte stream
  int buf;
  while(theTemp.Read((void*)&buf,1))
  {
    theFile.Write((void*)&buf,1);
  }
  theFile.Close();
  theTemp.Close();
}

// Create new file. Filename and title are garantueed 
// to be filled and valid.
void
NewFileDlg::CreateNewFile()
{
  CString filename = m_base + m_filename;
  CFile theFile;

  if(_access(filename,4) == 0)
  {
    CString mess = "The file [" + m_filename + "] does already exist.\n"
                   "Do you want to overwrite this file?";
    if(theApp.MessageBox(mess,"Overwerite file",MB_YESNO|MB_ICONEXCLAMATION) == IDNO)
    {
      m_error = true;
      return;
    }
    CFile::Remove(filename);
  }
  if(!theFile.Open(filename,CFile::modeCreate | CFile::modeWrite))
  {
    CString mess = "The file [" + m_filename + "] could not be created. Check your filesystem";
    theApp.ErrorMessage(mess);
    m_error = true;
    return;
  }
  CString content = "<html>\n"
                    "  <head>\n"
                    "    <title>" + m_title + "</title>\n"
                    "  </head>\n"
                    "  <body>\n"
                    "  </body>\n"
                    "</html>\n";
  theFile.Write(content.GetString(),content.GetLength());
  theFile.Close();
}

// NewFileDlg message handlers

void 
NewFileDlg::OnDocumentComplete(LPDISPATCH /*pDisp*/, LPVARIANT /*pURL*/)
{
  // Now read the bookmarks from the document (if any)
  CComPtr<IDispatch> disp;
  HRESULT hr = m_spBrowser->get_Document(&disp);
  CComQIPtr<IHTMLDocument2,&IID_IHTMLDocument2> doc = disp;
  if(SUCCEEDED(hr) && doc.p)
  {
  }
}

void 
NewFileDlg::OnEnChangeTitle()
{
  CWnd* w = GetDlgItem(IDC_TITLE);
  w->GetWindowText(m_title);
  if(m_synchronize)
  {
    CString file = m_title;
    file.Replace(' ','_');
    file.Replace('.','_');
    file.Replace('/','_');
    file.Replace('\\','_');
    file += ".htm";
    m_filename = file;
    w = GetDlgItem(IDC_FILENAME);
    w->SetWindowText(m_filename);
  }
}

void 
NewFileDlg::OnEnChangeFilename()
{
  CWnd* w = GetDlgItem(IDC_FILENAME);
  w->GetWindowText(m_filename);

  // No more synchronising
  m_synchronize = false;
}

void 
NewFileDlg::OnEnChangeTemplate()
{
  CWnd* w = GetDlgItem(IDC_TEMPLATE);
  w->GetWindowText(m_template);
  // And navigate
  OnBnClickedBtntgo();
}

void 
NewFileDlg::OnBnClickedButtonFsearch()
{
  DocFileDialog diag(true
                    ,"Search for a page to add to the project"
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
    m_filename = href;
    UpdateData(Data2Controls);
    OnBnClickedBtnfgo();
  }
}

void 
NewFileDlg::OnBnClickedBtnfgo()
{
  CString filename = m_base + m_filename;
  if(_access(filename,4) == -1)
  {
    return;
  }
  m_spBrowser->Navigate(filename.AllocSysString(),NULL,NULL,NULL,NULL);
  if(theApp.GetProjectFile()->FindDocumentFile(m_filename) != NULL)
  {
    CString mess = "The file [" + m_filename + "] is already part of this project!\n"
                   "Choose another filename or review your current files.";
    theApp.ErrorMessage(mess);
    m_error = true;
  }
}

void 
NewFileDlg::OnBnClickedButtonTsearch()
{
  DocFileDialog diag(true
                    ,"Search for a template to add a page to the project"
                    ,"htt"
                    ,""
                    ,0
                    ,"HTML Template document (*.htt)|*htt|");
  if(diag.DoModal() == IDOK)
  {
    CString href = diag.GetChosenFile();
    CString relative;
    if(Misc::MakeRelativePathname(m_base,href,relative))
    {
      href = relative;
    }
    m_template = href;
    UpdateData(Data2Controls);
    OnBnClickedBtntgo();
  }
}

void 
NewFileDlg::OnBnClickedBtntgo()
{
  CString templatename = m_base + m_template;
  if(_access(templatename,4) == -1)
  {
    CString mess = "The template [" + m_template + "] does not exist.";
    theApp.ErrorMessage(mess);
    return;
  }
  m_spBrowser->Navigate(templatename.AllocSysString(),NULL,NULL,NULL,NULL);
}

void 
NewFileDlg::OnBnClickedOk()
{
  if(!UpdateProperties())
  {
    return;
  }
  OnOK();
}

void 
NewFileDlg::OnBnClickedCancel()
{
  OnCancel();
}

void 
NewFileDlg::OnBnClickedHelpNewfile()
{
  // TODO: Add your control notification handler code here
}
