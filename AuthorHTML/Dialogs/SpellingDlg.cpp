//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    SpellingDlg.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for acting on the spelling results of a word
//
#include "stdafx.h"
#include "AuthorHTML.h"
#include "SpellingDlg.h"
#include "SpellOptionsDlg.h"
#include "Misc.h"

// SpellingDlg dialog

IMPLEMENT_DYNAMIC(SpellingDlg, CDialog)

SpellingDlg::SpellingDlg(CWnd* pParent
                        ,CString word
                        ,Spelling* speller)
            :CDialog(SpellingDlg::IDD, pParent)
            ,m_word(word)
            ,m_speller(speller)
{
  SplitWordAndQuotes();
  m_corrected = m_word;
}

SpellingDlg::~SpellingDlg()
{
}

void SpellingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
  DDX_Text   (pDX,IDC_SPEL_WRONG,  m_word);
  DDX_Text   (pDX,IDC_SPEL_CORRECT,m_corrected);
  DDX_Control(pDX,IDC_SPEL_SUGGEST,m_listSuggestions);
  DDX_Control(pDX,IDC_DICTIONARIES,m_comboLanguage);
  DDX_Control(pDX,IDC_SPEL_IGNORE, m_buttonIgnore);
  DDX_Control(pDX,IDC_SPEL_CHANGE, m_buttonChange);
  DDX_Control(pDX,IDC_SPEL_ADD,    m_buttonAdd);
  DDX_Control(pDX,IDC_SPEL_OPTIONS,m_buttonOptions);

  if(pDX->m_bSaveAndValidate == Data2Controls)
  {
    int ind;
    ind = m_comboLanguage.FindString(-1,m_language);
          m_comboLanguage.SetCurSel(ind);
    m_buttonAdd.EnableWindow(m_speller->GetOption(USE_CUSTOMDICT));
  }
}

BEGIN_MESSAGE_MAP(SpellingDlg, CDialog)
  ON_EN_KILLFOCUS (IDC_SPEL_CORRECT,  OnEnChangeSpelCorrect)
  ON_LBN_SELCHANGE(IDC_SPEL_SUGGEST,  OnLbnSelchangeSpelSuggest)
  ON_CBN_SELCHANGE(IDC_DICTIONARIES,  OnCbnSelchangeDictionaries)
  ON_BN_CLICKED   (IDC_SPEL_IGNORE,   OnBnClickedSpelIgnore)
  ON_BN_CLICKED   (IDC_SPEL_CHANGE,   OnBnClickedSpelChange)
  ON_BN_CLICKED   (IDC_SPEL_ADD,      OnBnClickedSpelAdd)
  ON_BN_CLICKED   (IDC_SPEL_OPTIONS,  OnBnClickedSpelOptions)
  ON_BN_CLICKED   (IDCANCEL,          OnBnClickedCancel)
END_MESSAGE_MAP()

BOOL
SpellingDlg::OnInitDialog()
{
  CDialog::OnInitDialog();
  GetLanguage();
  GetAlternatives();
  return TRUE;
}

void
SpellingDlg::GetLanguage()
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
SpellingDlg::GetAlternatives()
{
  // Clear previous Alternatives
  m_listSuggestions.ResetContent();
  // Find alternative wordlist
  std::vector<CString> all;
  int num = m_speller->GetAlternatives(m_word,all);
  for(unsigned int ind = 0;ind < all.size(); ++ind)
  {
    CString word = all[ind];
    if(num > 10 && (tolower(word[0]) != tolower(m_word[0])))
    {
      continue;
    }
    if(num > 30 && (tolower(word[1]) != tolower(m_word[1])))
    {
      continue;
    }
    if(std::abs((int)(word.GetLength() - m_word.GetLength())) > 3)
    {
      continue;
    }
    m_listSuggestions.AddString(all[ind]);
  }
  all.clear();
  UpdateData(Data2Controls);
}

void
SpellingDlg::DisplayChanging()
{
  int num = m_listSuggestions.GetCount();
  m_listSuggestions.ResetContent();
  for(int ind=0;ind<num;++ind) m_listSuggestions.AddString("");
  for(int ind=0;ind<num;++ind) m_listSuggestions.SetCurSel(ind);
  m_listSuggestions.SetCurSel(-1);
  m_listSuggestions.ResetContent();
  CString waiting = "<< Changing dictionaries >>";
  int pos = m_listSuggestions.AddString(waiting);
  m_listSuggestions.SetCurSel(pos);
  UpdateData(Data2Controls);
}

void
SpellingDlg::SplitWordAndQuotes()
{
  static char* quotes = "\'\",.:;";
  int ind = 0;

  // Get the before quotes
  for(ind = 0; ind < m_word.GetLength(); ++ind)
  {
    TCHAR ch = m_word.GetAt(ind);
    if(strchr(quotes, ch))
    {
      m_quotesBefore += ch;
    }
    else break;
  }

  // Adjust the word
  if(ind > 0)
  {
    m_word = m_word.Mid(ind);
  }
  if(m_word.IsEmpty())
  {
    return;
  }

  // Get the after quotes
  for(ind = m_word.GetLength() - 1; ind >= 0; --ind)
  {
    TCHAR ch = m_word.GetAt(ind);
    if(strchr(quotes,ch) == nullptr)
    {
      break;
    }
  }
  if(ind < m_word.GetLength() - 1)
  {
    ++ind;
    m_quotesAfter = m_word.Mid(ind);
    m_word = m_word.Left(ind);
  }
}


// Re-add the extra quotes
void
SpellingDlg::ReApplyQuotes()
{
  m_corrected = m_quotesBefore + m_corrected + m_quotesAfter;
}

// SpellingDlg message handlers

void 
SpellingDlg::OnEnChangeSpelCorrect()
{
  CWnd* w = GetDlgItem(IDC_SPEL_CORRECT);
  w->GetWindowText(m_corrected);
}

void 
SpellingDlg::OnLbnSelchangeSpelSuggest()
{
  int ind = m_listSuggestions.GetCurSel();
  if(ind >= 0)
  {
    m_listSuggestions.GetText(ind,m_corrected);
    UpdateData(Data2Controls);
  }
}

void 
SpellingDlg::OnCbnSelchangeDictionaries()
{
  int ind = m_comboLanguage.GetCurSel();
  if(ind >= 0)
  {
    CString lang;
    m_comboLanguage.GetLBText(ind,lang);
    if(lang != m_language)
    {
      m_language = lang;
      DisplayChanging();
      m_speller->ChangeDictionary(m_language);
      GetAlternatives();
    }
  }
}

void 
SpellingDlg::OnBnClickedSpelIgnore()
{
  // Revert to original word
  m_speller->AddToIgnore(m_word);
  
  ReApplyQuotes();
  EndDialog(IDCONTINUE);
}

void 
SpellingDlg::OnBnClickedSpelChange()
{
  // m_corrected will be used
  // Re-add the extra quotes
  ReApplyQuotes();
  EndDialog(IDOK);
}

void 
SpellingDlg::OnBnClickedSpelAdd()
{
  m_speller->AddToCustom(m_word);
  // original word will be used
  ReApplyQuotes();
  EndDialog(IDCONTINUE);
}

void 
SpellingDlg::OnBnClickedSpelOptions()
{
  CString orgLanguage = m_language;
  SpellOptionsDlg dialog(this,m_speller,false);
  if(dialog.DoModal() == IDOK)
  {
    m_language = dialog.GetLanguage();
    if(m_language != orgLanguage)
    {
      DisplayChanging();
      m_speller->ChangeDictionary(m_language);
      GetAlternatives();
    }
  }
}

void SpellingDlg::OnBnClickedCancel()
{
  OnCancel();
}
