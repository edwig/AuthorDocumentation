//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    SpellOptionsDlg.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for the settings of the spelling checker
//
#include "stdafx.h"
#include "AuthorHTML.h"
#include "SpellOptionsDlg.h"
#include "Misc.h"

// SpellOptionsDlg dialog

IMPLEMENT_DYNAMIC(SpellOptionsDlg, CDialog)
SpellOptionsDlg::SpellOptionsDlg(CWnd* pParent
                                ,Spelling* speller
                                ,bool canDisable
                                ,bool removeButtons)
                :CDialog(SpellOptionsDlg::IDD, pParent)
                ,m_speller(speller)
                ,m_canDisable(canDisable)
                ,m_removeButtons(removeButtons) 
{
}

SpellOptionsDlg::~SpellOptionsDlg()
{
}

void 
SpellOptionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
  DDX_Control(pDX,IDC_SO_ENABLE,      m_buttonEnable);
  DDX_Control(pDX,IDC_LANGUAGE,       m_comboLanguage);
  DDX_Control(pDX,IDC_SHOWERRORS,     m_buttonShowErrors);
  DDX_Control(pDX,IDC_NUMBERIGNORE,   m_buttonIgnoreNumber);
  DDX_Control(pDX,IDC_IGNORECAPITALS, m_buttonIgnoreCapitals);
  DDX_Control(pDX,IDC_USECUSTOM,      m_buttonUseCustom);
  // Common controls
  DDX_Control(pDX,IDOK,               m_buttonOK);
  DDX_Control(pDX,IDCANCEL,           m_buttonCancel);
  DDX_Control(pDX,IDC_SO_HELP,        m_buttonHelp);

  if(pDX->m_bSaveAndValidate == Data2Controls)
  {
    int ind = m_comboLanguage.FindString(-1,m_language);
    m_comboLanguage.SetCurSel(ind);

    m_comboLanguage       .EnableWindow(m_enable);
    m_buttonShowErrors    .EnableWindow(m_enable);
    m_buttonIgnoreNumber  .EnableWindow(m_enable);
    m_buttonIgnoreCapitals.EnableWindow(m_enable);
    m_buttonUseCustom     .EnableWindow(m_enable);

    m_buttonEnable        .SetCheck(m_enable);
    m_buttonShowErrors    .SetCheck(m_showErrors);
    m_buttonIgnoreNumber  .SetCheck(m_ignoreNumberWOrds);
    m_buttonIgnoreCapitals.SetCheck(m_ignoreAllCapitals);
    m_buttonUseCustom     .SetCheck(m_useCustom);
  }
}

BEGIN_MESSAGE_MAP(SpellOptionsDlg, CDialog)
  ON_BN_CLICKED   (IDC_SO_ENABLE,     OnBnClickedSoEnable)
  ON_CBN_SELCHANGE(IDC_LANGUAGE,      OnCbnSelchangeLanguage)
  ON_BN_CLICKED   (IDC_SHOWERRORS,    OnBnClickedShowerrors)
  ON_BN_CLICKED   (IDC_NUMBERIGNORE,  OnBnClickedNumberignore)
  ON_BN_CLICKED   (IDC_IGNORECAPITALS,OnBnClickedIgnorecapitals)
  ON_BN_CLICKED   (IDC_USECUSTOM,     OnBnClickedUsecustom)
  ON_BN_CLICKED   (IDOK,              OnBnClickedOk)
  ON_BN_CLICKED   (IDCANCEL,          OnBnClickedCancel)
  ON_BN_CLICKED   (IDC_SO_HELP,       OnBnClickedSoHelp)
END_MESSAGE_MAP()

BOOL
SpellOptionsDlg::OnInitDialog()
{
  CDialog::OnInitDialog();
  FillPage();
  FillLanguage();
  UpdateData(Data2Controls);
  if(!m_canDisable)
  {
    m_buttonEnable.SetCheck(TRUE);
  }
  m_buttonEnable.EnableWindow(m_canDisable);
  if(m_removeButtons)
  {
    m_buttonOK    .ShowWindow(SW_HIDE);
    m_buttonCancel.ShowWindow(SW_HIDE);
    m_buttonHelp  .ShowWindow(SW_HIDE);
  }
  return TRUE;
}

void
SpellOptionsDlg::FillLanguage()
{
  // Clear language list
  m_comboLanguage.ResetContent();
  CString dictionary = m_speller->GetDictionaryName();
  // Put all dictionary files in the combo
  DictionaryFiles* m_files = m_speller->GetDictionaryFiles();
  for(DictionaryFiles::iterator it = m_files->begin(); it != m_files->end(); ++it)
  {
    DictFile dict = *it;
    m_comboLanguage.AddString(dict.name);
    CString file(dict.fileName);
    file = Misc::FilenamePart(file);
    if(file.CompareNoCase(dictionary) == 0)
    {
      m_language = dict.name;
    }
  }
}

void
SpellOptionsDlg::FillPage()
{
  m_enable            = m_speller->GetOption(CHECKING_ENABLE);
  m_showErrors        = m_speller->GetOption(SHOW_SPELLERRORS);
  m_useCustom         = m_speller->GetOption(USE_CUSTOMDICT);
  m_ignoreAllCapitals = m_speller->GetOption(IGNORE_ALLUPPER);
  m_ignoreNumberWOrds = m_speller->GetOption(IGNORE_NUMBERWORDS);

}

void
SpellOptionsDlg::UpdateProperties()
{
  m_speller->SetOption(CHECKING_ENABLE,   m_enable);
  m_speller->SetOption(SHOW_SPELLERRORS,  m_showErrors);
  m_speller->SetOption(USE_CUSTOMDICT,    m_useCustom);
  m_speller->SetOption(IGNORE_ALLUPPER,   m_ignoreAllCapitals);
  m_speller->SetOption(IGNORE_NUMBERWORDS,m_ignoreNumberWOrds);
}

// SpellOptionsDlg message handlers

void 
SpellOptionsDlg::OnBnClickedSoEnable()
{
  m_enable = (m_buttonEnable.GetCheck() == TRUE);
  UpdateData(Data2Controls);
}

void 
SpellOptionsDlg::OnCbnSelchangeLanguage()
{
  int ind = m_comboLanguage.GetCurSel();
  if(ind >= 0)
  {
    m_comboLanguage.GetLBText(ind,m_language);
  }
}

void 
SpellOptionsDlg::OnBnClickedShowerrors()
{
  m_showErrors = (m_buttonShowErrors.GetCheck() == TRUE);
}

void 
SpellOptionsDlg::OnBnClickedNumberignore()
{
  m_ignoreNumberWOrds = (m_buttonIgnoreNumber.GetCheck() == TRUE);
}

void 
SpellOptionsDlg::OnBnClickedIgnorecapitals()
{
  m_ignoreAllCapitals = (m_buttonIgnoreCapitals.GetCheck() == TRUE);
}

void 
SpellOptionsDlg::OnBnClickedUsecustom()
{
  m_useCustom = (m_buttonUseCustom.GetCheck() == TRUE);
}

void 
SpellOptionsDlg::OnBnClickedOk()
{
  UpdateProperties();
  OnOK();
}

void 
SpellOptionsDlg::OnBnClickedCancel()
{
  OnCancel();
}

void 
SpellOptionsDlg::OnBnClickedSoHelp()
{
  // TODO: Add your control notification handler code here
}

