//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    FindReplaceDialog.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for finding and replacing text.
//
#include "stdafx.h"
#include "AuthorHTML.h"
#include "FindReplaceDialog.h"
#include ".\findreplacedialog.h"

#define MAX_STRINGS 20 // Max strings in find/replace combos

// FindReplaceDialog dialog

IMPLEMENT_DYNAMIC(FindReplaceDialog, CDialog)

FindReplaceDialog::FindReplaceDialog(bool    bFind
                                    ,CString sSearchText
                                    ,CString sReplaceText
                                    ,CWnd*   pParent /*=NULL*/)
                	:CDialog(FindReplaceDialog::IDD, pParent)
                  ,m_bFind(bFind)
                  ,m_findText(sSearchText)
                  ,m_replaceText(sReplaceText)
                  ,m_options(0)
{
}

FindReplaceDialog::~FindReplaceDialog()
{
}

void FindReplaceDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
  DDX_Control(pDX,IDC_COMBO_FIND,     m_comboFind);
  DDX_Control(pDX,IDC_COMBO_REPLACE,  m_comboReplace);
  //
  DDX_Control(pDX,IDC_CHECK_CASE,     m_buttonCase);
  DDX_Control(pDX,IDC_CHECK_WHOLE,    m_buttonWhole);
  DDX_Control(pDX,IDC_CHECK_UP,       m_buttonUp);
  DDX_Control(pDX,IDC_CHECK_REGULAR,  m_buttonRegular);
  //
  DDX_Control(pDX,IDC_RADIO_DOCUMENT, m_buttonDocument);
  DDX_Control(pDX,IDC_RADIO_ALL,      m_buttonAll);
  DDX_Control(pDX,IDC_RADIO_PROJECT,  m_buttonProject);
  DDX_Control(pDX,IDC_RADIO_BLOCK,    m_buttonBlock);
  //
  DDX_Control(pDX,IDC_BUTTON_FINDNEXT,    m_buttonFindNext);
  DDX_Control(pDX,IDC_BUTTON_REPLACEWITH, m_buttonReplaceWith);
  DDX_Control(pDX,IDC_BUTTON_REPLACEALL,  m_buttonReplaceAll);

  DDX_Control(pDX,IDOK,                   m_buttonOK);
  DDX_Control(pDX,IDCANCEL,               m_buttonCancel);
  DDX_Control(pDX,IDC_BUTTON_HELP,        m_buttonHelp);
}

BEGIN_MESSAGE_MAP(FindReplaceDialog, CDialog)
  ON_CBN_SELCHANGE(IDC_COMBO_FIND,      OnCbnSelchangeComboFind)
  ON_CBN_KILLFOCUS(IDC_COMBO_FIND,      OnCbnSelchangeComboFind)
  ON_CBN_SELCHANGE(IDC_COMBO_REPLACE,   OnCbnSelchangeComboReplace)
  ON_CBN_KILLFOCUS(IDC_COMBO_REPLACE,   OnCbnSelchangeComboReplace)
  ON_BN_CLICKED(IDC_BUTTON_FINDNEXT,    OnBnClickedButtonFindnext)
  ON_BN_CLICKED(IDOK,                   OnBnClickedOk)
  ON_BN_CLICKED(IDCANCEL,               OnBnClickedCancel)
  ON_BN_CLICKED(IDC_BUTTON_REPLACEWITH, OnBnClickedButtonReplacewith)
  ON_BN_CLICKED(IDC_CHECK_CASE,         OnBnClickedCheckCase)
  ON_BN_CLICKED(IDC_CHECK_WHOLE,        OnBnClickedCheckWhole)
  ON_BN_CLICKED(IDC_CHECK_UP,           OnBnClickedCheckUp)
  ON_BN_CLICKED(IDC_CHECK_REGULAR,      OnBnClickedCheckRegular)
  ON_BN_CLICKED(IDC_RADIO_DOCUMENT,     OnBnClickedRadioDocument)
  ON_BN_CLICKED(IDC_RADIO_ALL,          OnBnClickedRadioAll)
  ON_BN_CLICKED(IDC_RADIO_PROJECT,      OnBnClickedRadioProject)
  ON_BN_CLICKED(IDC_RADIO_BLOCK,        OnBnClickedRadioBlock)
  ON_BN_CLICKED(IDC_BUTTON_REPLACEALL,  OnBnClickedButtonReplaceall)
END_MESSAGE_MAP()

BOOL
FindReplaceDialog::OnInitDialog()
{
  CDialog::OnInitDialog();

  ReadFindReplaceHistory();
  if(m_findText.IsEmpty())
  {
    if(m_comboFind.GetCount() > 0)
    {
      // If no search text givven, place on first default saved string
      m_comboFind.SetCurSel(0);
    }
  }
  else
  {
    int pos = m_comboFind.FindString(-1,m_findText);
    if(pos != CB_ERR)
    {
      // Search text already in history. show it.
      m_comboFind.SetCurSel(pos);
    }
    else
    {
      // Add search text to combo and select it.
      int addPos = m_comboFind.AddString(m_findText);
      m_comboFind.SetCurSel(addPos);
    }
  }
  if(m_replaceText.IsEmpty())
  {
    if(!m_bFind)
    {
      if(m_comboReplace.GetCount() > 0)
      {
        // If no search text givven, place on first default saved string
        m_comboReplace.SetCurSel(0);
      }
    }
  }
  else
  {
    int pos = m_comboReplace.FindString(-1,m_replaceText);
    if(pos != CB_ERR)
    {
      // Replace text already in history
      m_comboReplace.SetCurSel(pos);
    }
    else
    {
      // Add replace text to history
      int addPos = m_comboReplace.AddString(m_replaceText);
      m_comboReplace.SetCurSel(addPos);
    }
  }

  if(m_bFind)
  {
    m_comboReplace.EnableWindow(FALSE);
    m_buttonReplaceAll.EnableWindow(FALSE);
  }
  else
  {
    SetWindowText("Find & Replace");
  }
  // TODO: Yet to implement
  m_buttonProject.EnableWindow(FALSE);
  m_buttonBlock  .EnableWindow(FALSE);

  // Defaults
  if(m_options & FRD_MATCHCASE)  m_buttonCase    .SetCheck(TRUE);
  if(m_options & FRD_MATCHWORD)  m_buttonWhole   .SetCheck(TRUE);
  if(m_options & FRD_SEARCHUP)   m_buttonUp      .SetCheck(TRUE);
  if(m_options & FRD_USEREGULAR) m_buttonRegular .SetCheck(TRUE);
  if(m_options & FRD_DOCUMENT)   m_buttonDocument.SetCheck(TRUE);
  if(m_options & FRD_ALLOPENDOC) m_buttonAll     .SetCheck(TRUE);
  if(m_options & FRD_PROJECT)    m_buttonProject .SetCheck(TRUE);
  if(m_options & FRD_BLOCKONLY)  m_buttonBlock   .SetCheck(TRUE);

  if(m_options & FRD_USEREGULAR)
  {
    m_buttonCase .EnableWindow(FALSE);
    m_buttonWhole.EnableWindow(FALSE);
    m_buttonUp   .EnableWindow(FALSE);
  }
  return TRUE;
}

// Read/write all find/replace strings
void FindReplaceDialog::ReadFindReplaceHistory()
{
  CString setting;
  CString value;
  for(int ind = 0;ind < MAX_STRINGS;++ind)
  {
    setting.Format("FindString%d",ind + 1);
    value = settings.GetSetting(setting,"");
    if(!value.IsEmpty())
    {
      if(m_comboFind.FindString(-1,value) == CB_ERR)
      {
        m_comboFind.AddString(value);
      }
    }
  }
  for(int ind = 0;ind < MAX_STRINGS;++ind)
  {
    setting.Format("ReplaceString%d",ind + 1);
    value = settings.GetSetting(setting,"");
    if(!value.IsEmpty())
    {
      if(m_comboReplace.FindString(-1,value) == CB_ERR)
      {
        m_comboReplace.AddString(value);
      }
    }
  }
  // get options
  setting = "FindReplaceOptions";
  value = settings.GetSetting(setting,"");
  m_options = atoi(value);
  m_options &= (FRD_MATCHCASE | FRD_MATCHWORD  | FRD_SEARCHUP | FRD_USEREGULAR |
                FRD_DOCUMENT  | FRD_ALLOPENDOC | FRD_PROJECT  | FRD_BLOCKONLY );

  // Last strings
  if(m_findText.IsEmpty())
  {
    setting    = "LastFindString";
    m_findText = settings.GetSetting(setting,"");
  }
  if(m_replaceText.IsEmpty())
  {
    setting       = "LastReplaceString";
    m_replaceText = settings.GetSetting(setting,"");
  }
}

void
FindReplaceDialog::WriteFindReplaceHistory()
{
  CString setting;
  CString value;
  int num = m_comboFind.GetCount();
  for(int ind = 0; ind < num; ++ind)
  {
    m_comboFind.GetLBText(ind,value);
    setting.Format("FindString%d",ind + 1);
    settings.SetSetting(setting,value);
  }
  num = m_comboReplace.GetCount();
  for(int ind = 0; ind < num; ++ind)
  {
    m_comboReplace.GetLBText(ind,value);
    setting.Format("ReplaceString%d",ind + 1);
    settings.SetSetting(setting,value);
  }
  // Save options
  setting = "FindReplaceOptions";
  value.Format("%d",m_options);
  settings.SetSetting(setting,value);

  // Last strings
  setting = "LastFindString";
  value   = m_findText;
  settings.SetSetting(setting,value);
  setting = "LastReplaceString";
  value   = m_replaceText;
  settings.SetSetting(setting,value);
}

// FindReplaceDialog message handlers

void FindReplaceDialog::OnCbnSelchangeComboFind()
{
  int ind = m_comboFind.GetCurSel();
  if(ind >=0)
  {
    m_comboFind.GetLBText(ind,m_findText);
  }
  else
  {
    m_comboFind.GetWindowText(m_findText);
    if(m_comboFind.FindStringExact(-1,m_findText) == CB_ERR)
    {
      // add to the top of the list if not found
      m_comboFind.InsertString(0,m_findText);
    }
  }
}

void FindReplaceDialog::OnCbnSelchangeComboReplace()
{
  int ind = m_comboReplace.GetCurSel();
  if (ind >= 0)
  {
    m_comboReplace.GetLBText(ind,m_replaceText);
  }
  else
  {
    m_comboReplace.GetWindowText(m_replaceText);
    if(m_comboReplace.FindStringExact(-1,m_replaceText) == CB_ERR)
    {
      // Add to top of the list
      m_comboReplace.InsertString(0,m_replaceText);
    }
  }
}

void FindReplaceDialog::OnBnClickedButtonFindnext()
{
  if(m_findText.IsEmpty())
  {
    theApp.MessageBox("Specify a text to find first!","Finding what?",MB_OK|MB_ICONQUESTION);
    return;
  }
  WriteFindReplaceHistory();
  EndDialog(IDC_BUTTON_FINDNEXT);
}


void FindReplaceDialog::OnBnClickedOk()
{
  WriteFindReplaceHistory();
  OnOK();
}

void FindReplaceDialog::OnBnClickedCancel()
{
  WriteFindReplaceHistory();
  OnCancel();
}

void FindReplaceDialog::OnBnClickedButtonReplacewith()
{
  if(m_bFind)
  {
    // Dialog was still in find state
    m_comboReplace.EnableWindow(TRUE);
    m_buttonReplaceAll.EnableWindow(TRUE);
    SetWindowText("Find & Replace");
    m_comboReplace.GetWindowText(m_replaceText);
    m_bFind = false;
    return;
  }
  WriteFindReplaceHistory();
  EndDialog(IDC_BUTTON_REPLACEWITH);
}

void FindReplaceDialog::OnBnClickedButtonReplaceall()
{
  WriteFindReplaceHistory();
  EndDialog(IDC_BUTTON_REPLACEALL);
}

void FindReplaceDialog::OnBnClickedCheckCase()
{
  if(m_buttonCase.GetCheck())
  {
    m_options |= FRD_MATCHCASE;
  }
  else
  {
    m_options &= ~FRD_MATCHCASE;
  }
}

void FindReplaceDialog::OnBnClickedCheckWhole()
{
  if(m_buttonWhole.GetCheck())
  {
    m_options |= FRD_MATCHWORD;
  }
  else
  {
    m_options &= ~FRD_MATCHWORD;
  }
}

void FindReplaceDialog::OnBnClickedCheckUp()
{
  if(m_buttonUp.GetCheck())
  {
    m_options |= FRD_SEARCHUP;
  }
  else
  {
    m_options &= ~FRD_SEARCHUP;
  }
}

void FindReplaceDialog::OnBnClickedCheckRegular()
{
  if(m_buttonRegular.GetCheck())
  {
    m_options |= FRD_USEREGULAR;
    m_options &= ~(FRD_MATCHCASE | FRD_MATCHWORD);
    m_buttonCase .SetCheck(FALSE);
    m_buttonWhole.SetCheck(FALSE);
    m_buttonUp   .SetCheck(FALSE);
    m_buttonCase .EnableWindow(FALSE);
    m_buttonWhole.EnableWindow(FALSE);
    m_buttonUp   .EnableWindow(FALSE);
  }
  else
  {
    m_options &= ~FRD_USEREGULAR;
    m_buttonCase .EnableWindow(TRUE);
    m_buttonWhole.EnableWindow(TRUE);
    m_buttonUp   .EnableWindow(TRUE);
  }
}

void FindReplaceDialog::OnBnClickedRadioDocument()
{
  if(m_buttonDocument.GetCheck())
  {
    m_options |= FRD_DOCUMENT;
    m_options &= ~(FRD_ALLOPENDOC | FRD_PROJECT | FRD_BLOCKONLY);
  }
}

void FindReplaceDialog::OnBnClickedRadioProject()
{
  if(m_buttonProject.GetCheck())
  {
    m_options |= FRD_PROJECT;
    m_options &= ~(FRD_DOCUMENT | FRD_ALLOPENDOC | FRD_BLOCKONLY);
  }
}

void FindReplaceDialog::OnBnClickedRadioAll()
{
  if(m_buttonAll.GetCheck())
  {
    m_options |= FRD_ALLOPENDOC;
    m_options &= ~(FRD_DOCUMENT | FRD_PROJECT | FRD_BLOCKONLY);
  }
}

void FindReplaceDialog::OnBnClickedRadioBlock()
{
  if(m_buttonBlock.GetCheck())
  {
    m_options |= FRD_BLOCKONLY;
    m_options &= ~(FRD_DOCUMENT | FRD_PROJECT | FRD_ALLOPENDOC);
  }
}

