//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    CharacterDialog.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog to insert a special character in the HTML file.
//
#include "stdafx.h"
#include "AuthorHTML.h"
#include "CharacterDialog.h"
#include "HTMLEdDoc.h"
#include "HTMLEdView.h" 
#include "Misc.h"
#include ".\characterdialog.h"

// CharacterDialog dialog

IMPLEMENT_DYNAMIC(CharacterDialog, CDialog)
CharacterDialog::CharacterDialog(CWnd* pParent /*=NULL*/)
	              :CDialog(CharacterDialog::IDD, pParent)
	              ,m_parent(pParent)
{
}

CharacterDialog::~CharacterDialog()
{
}

void CharacterDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
  DDX_Control(pDX,IDC_RADIO_ACC_LOWER,   m_butAccentLower);
  DDX_Control(pDX,IDC_RADIO_ACC_UPPER,   m_butAccentUpper);
  DDX_Control(pDX,IDC_RADIO_OHTER_LOWER, m_butOtherLower);
  DDX_Control(pDX,IDC_RADIO_OTHER_UPPER, m_butOtherUpper);
  DDX_Control(pDX,IDC_GREEK_LOWER,       m_butGreekLower);
  DDX_Control(pDX,IDC_GREEK_UPPER,       m_butGreekUpper);
  DDX_Control(pDX,IDC_RADIO_COMMON,      m_butCommon);
  DDX_Control(pDX,IDC_LETTER_COMBO,      m_letter);
  DDX_Control(pDX,IDC_CHAR_COMBO,        m_character);
  DDX_Control(pDX,IDOK,                  m_buttonOK);
  DDX_Control(pDX,IDCANCEL,              m_buttonCancel);
  DDX_Control(pDX,IDC_HELP_CHARACTER,    m_buttonHelp);
  DDX_Control(pDX,IDC_INSERT_CHAR,       m_buttonInsert);
}

BEGIN_MESSAGE_MAP(CharacterDialog, CDialog)
  ON_BN_CLICKED(IDC_RADIO_ACC_LOWER,   OnBnClickedRadioAccLower)
  ON_BN_CLICKED(IDC_RADIO_ACC_UPPER,   OnBnClickedRadioAccUpper)
  ON_CBN_SELCHANGE(IDC_LETTER_COMBO,   OnCbnSelchangeLetterCombo)
  ON_CBN_SELCHANGE(IDC_CHAR_COMBO,     OnCbnSelchangeCharCombo)
  ON_BN_CLICKED(IDC_RADIO_OHTER_LOWER, OnBnClickedRadioOhterLower)
  ON_BN_CLICKED(IDC_RADIO_OTHER_UPPER, OnBnClickedRadioOtherUpper)
  ON_BN_CLICKED(IDC_RADIO_COMMON,      OnBnClickedRadioCommon)
  ON_BN_CLICKED(IDC_INSERT_CHAR,       OnBnClickedInsertChar)
  ON_BN_CLICKED(IDC_GREEK_LOWER,       OnBnClickedGreekLower)
  ON_BN_CLICKED(IDC_GREEK_UPPER,       OnBnClickedGreekUpper)
END_MESSAGE_MAP()

BOOL
CharacterDialog::OnInitDialog()
{
  CDialog::OnInitDialog();
  OnBnClickedRadioAccLower();
  OnCbnSelchangeCharCombo();
  m_butAccentLower.SetCheck(TRUE);
  return TRUE;
}

// CharacterDialog message handlers

void CharacterDialog::OnBnClickedRadioAccLower()
{
  m_letter.EnableWindow(TRUE);
  AddToComboByCategory(ACC_LOWER);
  AddToCombo(ACC_LOWER,'a');
  m_letter.SetCurSel(0);
  m_character.SetCurSel(0);
}

void CharacterDialog::OnBnClickedRadioAccUpper()
{
  m_letter.EnableWindow(TRUE);
  AddToComboByCategory(ACC_UPPER);
  AddToCombo(ACC_UPPER,'A');
  m_letter.SetCurSel(0);
  m_character.SetCurSel(0);
}

void 
CharacterDialog::OnBnClickedRadioOhterLower()
{
  m_letter.ResetContent();
  m_letter.EnableWindow(FALSE);
  AddToCombo(OTH_LOWER,0);
  m_letter.SetCurSel(0);
  m_character.SetCurSel(0);
}

void 
CharacterDialog::OnBnClickedRadioOtherUpper()
{
  m_letter.ResetContent();
  m_letter.EnableWindow(FALSE);
  AddToCombo(OTH_UPPER,0);
  m_letter.SetCurSel(0);
  m_character.SetCurSel(0);
}

void 
CharacterDialog::OnBnClickedGreekLower()
{
  m_letter.ResetContent();
  m_letter.EnableWindow(FALSE);
  AddToCombo(GRE_LOWER,0);
  m_letter.SetCurSel(0);
  m_character.SetCurSel(0);
}

void 
CharacterDialog::OnBnClickedGreekUpper()
{
  m_letter.ResetContent();
  m_letter.EnableWindow(FALSE);
  AddToCombo(GRE_UPPER,0);
  m_letter.SetCurSel(0);
  m_character.SetCurSel(0);
}

void 
CharacterDialog::OnBnClickedRadioCommon()
{
  AddToCombo(COMMON,0);
  m_letter.ResetContent();
  m_letter.EnableWindow(FALSE);
  m_character.SetCurSel(0);
}

void CharacterDialog::OnCbnSelchangeLetterCombo()
{
  CString sel;
  int n = m_letter.GetCurSel();
  m_letter.GetLBText(n,sel);
  int lett_category = FindCategory(sel.GetAt(0));

  AddToCombo(lett_category,sel.GetAt(0));
  m_character.SetCurSel(0);
}

void CharacterDialog::OnCbnSelchangeCharCombo()
{
  CString sel;
  int n = m_character.GetCurSel();
  m_character.GetLBText(n,sel);
  unsigned char c = sel.GetAt(0);
  m_result = FindHTML((int)c);
}

//////////////////////////
//
// INTERNALS
//
//////////////////////////

void
CharacterDialog::AddToComboByCategory(int letter_cat)
{
  m_letter.ResetContent();
  int lastLetter = 0;
  for(int index = 0; DiacriteLetters[index].letter; ++index)
  {
    if(DiacriteLetters[index].lett_category == letter_cat)
    {
      if(DiacriteLetters[index].letter != lastLetter)
      {
        lastLetter = DiacriteLetters[index].letter;

        char string1[2];
        string1[0]     = (char)DiacriteLetters[index].letter;
        string1[1]     = 0;
        CString letter = string1;
        m_letter.AddString(letter);
      }
    }
  }
}

void
CharacterDialog::AddToCombo(int cat,int letter)
{
  m_character.ResetContent();
  for(int index = 0; DiacriteLetters[index].letter; ++index)
  {
    if((DiacriteLetters[index].lett_category == cat   ) && ((letter == 0) ||
       (DiacriteLetters[index].letter        == letter) ))
    {
      char string2[2];
      string2[0]       = (char)DiacriteLetters[index].diacrite;
      string2[1]       = 0;
      CString descript = string2;
      descript += "  (";
      descript += DiacriteLetters[index].descript;
      descript += ")";
      m_character.AddString(descript);
    }
  }
}

int
CharacterDialog::FindCategory(int letter)
{
  for(int index=0; DiacriteLetters[index].letter; ++index)
  {
    if(DiacriteLetters[index].letter == letter)
    {
      return DiacriteLetters[index].lett_category;
    }
  }
  return 0;
}

CString
CharacterDialog::FindHTML(int letter)
{
  CString html;
  for(int index=0; DiacriteLetters[index].letter; ++index)
  {
    if(DiacriteLetters[index].diacrite == letter)
    {
      html = DiacriteLetters[index].htmlText;
      break;
    }
  }
  return html;
}

void 
CharacterDialog::OnBnClickedInsertChar()
{
  OnCbnSelchangeCharCombo();
  if(!m_result.IsEmpty())
  {
    CHTMLEdView* view = (CHTMLEdView*)m_parent;
    view->PasteTextHTML(m_result);
  }
}

