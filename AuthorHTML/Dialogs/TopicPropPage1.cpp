//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    TopicPropPage1.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for all topic properties page 1 of 5
//
#include "stdafx.h"
#include "AuthorHTML.h"
#include "TopicPropPage1.h"
#include "MainFrm.h"
#include "Misc.h"

IMPLEMENT_DYNAMIC(TopicPropPage1Dlg, CDialog)

TopicPropPage1Dlg::TopicPropPage1Dlg(CWnd* pParent
                                    ,DocumentFile* pDocument
                                    ,CComPtr<IHTMLDocument2> pDoc)
	                :CDialog(TopicPropPage1Dlg::IDD, pParent)
                  ,m_document(pDocument)
                  ,m_htmlDoc(pDoc)
{
  m_title     = pDocument->GetTitle();
  m_template  = pDocument->GetTemplate();
  m_author    = pDocument->GetAuthor();
  m_comment   = pDocument->GetComment();
  m_status    = pDocument->GetStatus();
  m_priority  = pDocument->GetPriority();
  m_status    = pDocument->GetStatus();
  m_todo      = pDocument->GetToDo();
  m_timeSpent = pDocument->GetTimeSpent();

  // Get based-file name
  CString fileName  = pDocument->GetFilename();
  AuthorHTMLApp* app = (AuthorHTMLApp*) AfxGetApp();
  CString base = app->GetBaseDirectory();
  m_fileName = Misc::RemoveBasePart(base,fileName);
}

TopicPropPage1Dlg::~TopicPropPage1Dlg()
{
}

void TopicPropPage1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
  DDX_Text   (pDX,IDC_TOPIC_TITLE,       m_title);
  DDX_Text   (pDX,IDC_TOPIC_FILENAME,    m_fileName);
  DDX_Text   (pDX,IDC_TOPIC_TEMPLATE,    m_template);
  DDX_Control(pDX,IDC_BUTTON_FILE,       m_buttonFile);
  DDX_Control(pDX,IDC_BUTTON_TEMPLATE,   m_buttonTemplate);
  DDX_Text   (pDX,IDC_TOPIC_AUTHOR,      m_author);
  DDX_Text   (pDX,IDC_TOPIC_COMMENT,     m_comment);
  DDX_Control(pDX,IDC_TOPIC_STATUS,      m_comboStatus);
  DDX_Control(pDX,IDC_TOPIC_PRIORITY,    m_comboPriority);
  DDX_Text   (pDX,IDC_TIMESPENT,         m_timeSpent);
  DDX_Control(pDX,IDC_TODO_1,            m_todo1);
  DDX_Control(pDX,IDC_TODO_2,            m_todo2);
  DDX_Control(pDX,IDC_TODO_3,            m_todo3);
  DDX_Control(pDX,IDC_TODO_4,            m_todo4);
  DDX_Control(pDX,IDC_TODO_5,            m_todo5);
  DDX_Control(pDX,IDC_TODO_6,            m_todo6);
  DDX_Control(pDX,IDC_TODO_7,            m_todo7);
}

BEGIN_MESSAGE_MAP(TopicPropPage1Dlg, CDialog)
  ON_EN_KILLFOCUS (IDC_TOPIC_TITLE,   OnEnChangeTitle)
  ON_EN_KILLFOCUS (IDC_TOPIC_FILENAME,OnEnChangeFilename)
  ON_CBN_SELCHANGE(IDC_TOPIC_STATUS,  OnCbnSelchangeTopicStatus)
  ON_CBN_SELCHANGE(IDC_TOPIC_PRIORITY,OnCbnSelchangeTopicPriority)
  ON_BN_CLICKED   (IDC_TODO_1,        OnBnClickedTodo1)
  ON_BN_CLICKED   (IDC_TODO_2,        OnBnClickedTodo2)
  ON_BN_CLICKED   (IDC_TODO_3,        OnBnClickedTodo3)
  ON_BN_CLICKED   (IDC_TODO_4,        OnBnClickedTodo4)
  ON_BN_CLICKED   (IDC_TODO_5,        OnBnClickedTodo5)
  ON_BN_CLICKED   (IDC_TODO_6,        OnBnClickedTodo6)
  ON_BN_CLICKED   (IDC_TODO_7,        OnBnClickedTodo7)
END_MESSAGE_MAP()

BOOL
TopicPropPage1Dlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  // Progress;Review;Ready;
  CString status;
  switch(m_status)
  {
    case STATUS_PROGRESS: status = "Progress"; break;
    case STATUS_REVIEW:   status = "Review";   break;
    case STATUS_READY:    status = "Ready";    break;
  }
  int num = m_comboStatus.FindString(-1,status);
  if(num >= 0)
  {
    m_comboStatus.SetCurSel(num);
  }
  
  // Low;Medium;High;
  CString prio;
  switch(m_priority)
  {
    case PRIO_HIGH:   prio = "High";   break;
    case PRIO_MEDIUM: prio = "Medium"; break;
    case PRIO_LOW:    prio = "Low";    break;
  }
  num = m_comboPriority.FindString(-1,prio);
  if(num >= 0)
  {
    m_comboPriority.SetCurSel(num);
  }
  if(m_todo & TODO_FIRSTDRAFT) m_todo1.SetCheck(TRUE);
  if(m_todo & TODO_ADDTOTOC)   m_todo2.SetCheck(TRUE);
  if(m_todo & TODO_LINKS)      m_todo3.SetCheck(TRUE);
  if(m_todo & TODO_INDEX)      m_todo4.SetCheck(TRUE);
  if(m_todo & TODO_IMAGES)     m_todo5.SetCheck(TRUE);
  if(m_todo & TODO_BROWSESEQ)  m_todo6.SetCheck(TRUE);
  if(m_todo & TODO_REVIEW)     m_todo7.SetCheck(TRUE);

  return TRUE;
}

// Write properties back on the IHTML document
void
TopicPropPage1Dlg::UpdateDocumentFile()
{
  UpdateData(Controls2Data);
  m_document->SetTitle(m_title);
  m_document->SetTemplate(m_template);
  m_document->SetAuthor(m_author);
  m_document->SetComment(m_comment);
  m_document->SetStatus(m_status);
  m_document->SetPriority(m_priority);
  m_document->SetToDo(m_todo);
  m_document->SetTimeSpent(m_timeSpent);

  // Now synchronize
  m_document->SetOnDocument(m_htmlDoc);
}

bool
TopicPropPage1Dlg::RenameBaseFile()
{
  bool renamed = false;
  CString oldFilename = m_document->GetFilename();

  if(m_fileName.CompareNoCase(oldFilename) != 0)
  {
    CString message;
    message.Format("Would you like to change the filename of this topic?\n"
                   "From: %s\n"
                   "To: %s\n"
                   "\n"
                   "This action also will close and reopen the topic in the current view!"
                   ,oldFilename.GetString()
                   ,m_fileName.GetString());
    if(theApp.MessageBox(message,"Changing filename",MB_YESNO|MB_DEFBUTTON2|MB_ICONQUESTION) == IDYES)
    {
      // Really changing the filename
      CWaitCursor takeAdeepSigh;

      MainFrame* main = (MainFrame*) theApp.m_pMainWnd;
      renamed = main->RenameFile(oldFilename,m_fileName);
    }
    else
    {
      // Reverting the filename
      m_fileName = oldFilename;
      UpdateData(FALSE);
    }
  }
  return renamed;
}

// TopicPropPage1Dlg message handlers

void
TopicPropPage1Dlg::OnEnChangeTitle()
{
  UpdateData();
}

void
TopicPropPage1Dlg::OnEnChangeFilename()
{
  UpdateData();
}

void TopicPropPage1Dlg::OnCbnSelchangeTopicStatus()
{
  CString status;
  int num = m_comboStatus.GetCurSel();
  m_comboStatus.GetLBText(num,status);
       if(status == "Progress") m_status = STATUS_PROGRESS;
  else if(status == "Review")   m_status = STATUS_REVIEW;
  else if(status == "Ready")    m_status = STATUS_READY;
  else m_status = 0;
}

void TopicPropPage1Dlg::OnCbnSelchangeTopicPriority()
{
  CString prio;
  int num = m_comboPriority.GetCurSel();
  m_comboPriority.GetLBText(num,prio);
       if(prio == "Low")    m_priority = PRIO_LOW;
  else if(prio == "Medium") m_priority = PRIO_MEDIUM;
  else if(prio == "High")   m_priority = PRIO_HIGH;
  else m_priority = 0;
}

void TopicPropPage1Dlg::OnBnClickedTodo1()
{
  if(m_todo1.GetCheck()) m_todo |=  TODO_FIRSTDRAFT;
  else                   m_todo &= ~TODO_FIRSTDRAFT;
}

void TopicPropPage1Dlg::OnBnClickedTodo2()
{
  if(m_todo2.GetCheck()) m_todo |=  TODO_ADDTOTOC;
  else                   m_todo &= ~TODO_ADDTOTOC;
}

void TopicPropPage1Dlg::OnBnClickedTodo3()
{
  if(m_todo3.GetCheck()) m_todo |=  TODO_LINKS;
  else                   m_todo &= ~TODO_LINKS;
}

void TopicPropPage1Dlg::OnBnClickedTodo4()
{
  if(m_todo4.GetCheck()) m_todo |=  TODO_INDEX;
  else                   m_todo &= ~TODO_INDEX;
}

void TopicPropPage1Dlg::OnBnClickedTodo5()
{
  if(m_todo5.GetCheck()) m_todo |=  TODO_IMAGES;
  else                   m_todo &= ~TODO_IMAGES;
}

void TopicPropPage1Dlg::OnBnClickedTodo6()
{
  if(m_todo6.GetCheck()) m_todo |=  TODO_BROWSESEQ;
  else                   m_todo &= ~TODO_BROWSESEQ;
}

void TopicPropPage1Dlg::OnBnClickedTodo7()
{
  if(m_todo7.GetCheck()) m_todo |=  TODO_REVIEW;
  else                   m_todo &= ~TODO_REVIEW;
}
