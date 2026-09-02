//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    TopicProperties.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for all topic properties pages
//
#include "stdafx.h"
#include "AuthorHTML.h"
#include "TopicProperties.h"
#include "MainFrm.h"
#include "HTMLBody.h"
#include "HTMLFrameSet.h"

// TopicProperties dialog

IMPLEMENT_DYNAMIC(TopicPropertiesDlg, CDialog)

TopicPropertiesDlg::TopicPropertiesDlg(CWnd*         pParent
                                      ,DocumentFile* pDocument
                                      ,CComPtr<IHTMLDocument2> pDoc
                                      ,CString       base)
                   :CDialog(TopicPropertiesDlg::IDD, pParent)
                   ,m_document(pDocument)
                   ,m_firstTab(0)
                   ,m_hasBody(false)
{
  // Get Body from document
  CComPtr<IHTMLElement> elem;
  pDoc->get_body(&elem);
  // Test to see if it's a body or a frameset
  HtmlBody     body(elem);
  HtmlFrameSet frame(elem);
  if(body.Valid())
  {
    m_hasBody = true;
    m_page2 = new TopicPropPage2Dlg(this,body,base);
    m_page5 = NULL;
  }
  else
  {
    m_hasBody = false;
    m_page5 = new TopicPropPage5Dlg(this,frame,base);
    m_page2 = NULL;
  }
  m_page1 = new TopicPropPage1Dlg(this,pDocument,pDoc);
  m_page3 = new TopicPropPage3Dlg(this,pDoc,base,pDocument);
  m_page4 = new TopicPropPage4Dlg(this,pDoc,base);
}

TopicPropertiesDlg::~TopicPropertiesDlg()
{
  delete m_page1;
  delete m_page2;
  delete m_page3;
  delete m_page4;
  delete m_page5;
}

void TopicPropertiesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_TOPIC_TABS,  m_tabControl);
  DDX_Control(pDX, IDOK,            m_buttonOK);
  DDX_Control(pDX, IDC_PROP_APPLY,  m_buttonApply);
  DDX_Control(pDX, IDCANCEL,        m_buttonCancel);
  DDX_Control(pDX, IDC_TOPIC_HELP,  m_buttonHelp);
}

BEGIN_MESSAGE_MAP(TopicPropertiesDlg, CDialog)
  ON_BN_CLICKED(IDOK,           OnBnClickedOk)
  ON_BN_CLICKED(IDCANCEL,       OnBnClickedCancel)
  ON_BN_CLICKED(IDC_PROP_APPLY, OnBnClickedApply)
  ON_BN_CLICKED(IDHELP,         OnBnClickedHelp)
  ON_NOTIFY(TCN_SELCHANGE, IDC_TOPIC_TABS, OnTcnSelchangeTabcontrol)
END_MESSAGE_MAP()

BOOL
TopicPropertiesDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  m_page1->Create(IDD_TOPIC_PROPPAGE1,this);
  m_page3->Create(IDD_TOPIC_PROPPAGE3,this);
  m_page4->Create(IDD_TOPIC_PROPPAGE4,this);
  if(m_hasBody)
  {
    m_page2->Create(IDD_TOPIC_PROPPAGE2,this);
  }
  else
  {
    m_page5->Create(IDD_TOPIC_PROPPAGE5,this);
  }

  // Set text of tabs
  CString s;
  m_page1->GetWindowText(s);
  m_tabControl.InsertItem(0,s);
  if(m_hasBody)
  {
    m_page2->GetWindowText(s);
    m_tabControl.InsertItem(1,s);
  }
  else
  {
    m_page5->GetWindowTextA(s);
    m_tabControl.InsertItem(1,s);
  }
  m_page3->GetWindowText(s);
  m_tabControl.InsertItem(2,s);
  m_page4->GetWindowText(s);
  m_tabControl.InsertItem(3,s);

  CRect rect;
  m_tabControl.GetWindowRect(&rect);    // geeft t.o.v. ouder window inclusief titelbar
  ScreenToClient(&rect);
  rect.top    += 21;
  rect.bottom -= 3;
  rect.right  -= 2;
  rect.left   += 1; 

  m_page1->MoveWindow(rect,true);    // zet tov client window.
  m_page3->MoveWindow(rect,false);
  m_page4->MoveWindow(rect,false);
  if(m_hasBody)
  {
    m_page2->MoveWindow(rect,false);
  }
  else
  {
    m_page5->MoveWindow(rect,false);
  }
  m_page1->ShowWindow(m_firstTab == 0 ? SW_SHOW : SW_HIDE);
  m_page3->ShowWindow(m_firstTab == 2 ? SW_SHOW : SW_HIDE);
  m_page4->ShowWindow(m_firstTab == 2 ? SW_SHOW : SW_HIDE);
  if(m_hasBody)
  {
    m_page2->ShowWindow(m_firstTab == 1 ? SW_SHOW : SW_HIDE);
  }
  else
  {
    m_page5->ShowWindow(m_firstTab == 1 ? SW_SHOW : SW_HIDE);
  }
  TabCtrl_SetCurSel(m_tabControl.GetSafeHwnd(),m_firstTab);

  return TRUE;
}

void
TopicPropertiesDlg::UpdateProperties()
{
  m_page1->UpdateDocumentFile();
  m_page3->UpdateProperties();
  m_page4->UpdateProperties();
  if(m_hasBody)
  {
    m_page2->UpdateProperties();
  }
  else
  {
    m_page5->UpdateProperties();
  }

  if(m_page3->GetChanged())
  {
    AuthorHTMLApp* app = reinterpret_cast<AuthorHTMLApp*>(AfxGetApp());
    if(app)
    {
      app->ReSweepIndex();
    }
  }
}

void
TopicPropertiesDlg::SaveDocument()
{
  MainFrame* main = reinterpret_cast<MainFrame*>(theApp.m_pMainWnd);
  main->SendMessage(WM_COMMAND,ID_FILE_SAVE);
}

void
TopicPropertiesDlg::OnBnClickedOk()
{
  UpdateProperties();
  SaveDocument();
  m_renamed = m_page1->RenameBaseFile();
  OnOK();
}

void
TopicPropertiesDlg::OnBnClickedCancel()
{
  OnCancel();
}

void
TopicPropertiesDlg::OnBnClickedApply()
{
  UpdateProperties();
  SaveDocument();
}

void 
TopicPropertiesDlg::OnTcnSelchangeTabcontrol(NMHDR *pNMHDR, LRESULT *pResult)
{
  int num = TabCtrl_GetCurSel(pNMHDR->hwndFrom);

  if(m_hasBody)
  {
    switch(num)
    {
      case 0: m_page1->ShowWindow(TRUE);
              m_page2->ShowWindow(FALSE);
              m_page3->ShowWindow(FALSE);
              m_page4->ShowWindow(FALSE);
              break;
      case 1: m_page1->ShowWindow(FALSE);
              m_page2->ShowWindow(TRUE);
              m_page3->ShowWindow(FALSE);
              m_page4->ShowWindow(FALSE);
              break;
      case 2: m_page1->ShowWindow(FALSE);
              m_page2->ShowWindow(FALSE);
              m_page3->ShowWindow(TRUE);
              m_page3->ShowFocus();
              m_page4->ShowWindow(FALSE);
              break;
      case 3: m_page1->ShowWindow(FALSE);
              m_page2->ShowWindow(FALSE);
              m_page3->ShowWindow(FALSE);
              m_page4->ShowWindow(TRUE);
              m_page4->ShowFocus();
              break;
    }
  }
  else
  {
    // We have a frameset
    switch(num)
    {
      case 0: m_page1->ShowWindow(TRUE);
              m_page5->ShowWindow(FALSE);
              m_page3->ShowWindow(FALSE);
              m_page4->ShowWindow(FALSE);
              break;
      case 1: m_page1->ShowWindow(FALSE);
              m_page5->ShowWindow(TRUE);
              m_page3->ShowWindow(FALSE);
              m_page4->ShowWindow(FALSE);
              break;
      case 2: m_page1->ShowWindow(FALSE);
              m_page5->ShowWindow(FALSE);
              m_page3->ShowWindow(TRUE);
              m_page3->ShowFocus();
              m_page4->ShowWindow(FALSE);
              break;
      case 3: m_page1->ShowWindow(FALSE);
              m_page5->ShowWindow(FALSE);
              m_page3->ShowWindow(FALSE);
              m_page4->ShowWindow(TRUE);
              m_page4->ShowFocus();
              break;
    }
  }
  *pResult = 0;
}

void
TopicPropertiesDlg::OnBnClickedHelp()
{
  // TODO
}
