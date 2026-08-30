//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    Spelling.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Check the spelling of the text of a HTML documentation page
//              against the provided language dictionaries
//
#pragma once
#include "StdAfx.h"
#include <map>
#include <vector>
#include <set>

typedef struct _dictionaryFile
{
  CString name;       // Language
  CString fileName;   // Filname part
}
DictFile;

typedef enum _SpelOption
{
  CHECKING_ENABLE
 ,SHOW_SPELLERRORS
 ,USE_CUSTOMDICT
 ,IGNORE_ALLUPPER
 ,IGNORE_NUMBERWORDS
}
SpelOption;

typedef unsigned __int64                SoundAs;
typedef std::multimap<SoundAs,CString>  SpellDictionary;
typedef std::set<CString>               Dictionary;
typedef std::vector<DictFile>           DictionaryFiles;

class Spelling
{
public:
   Spelling();
  ~Spelling();
   // General functions
   bool     CheckWord(CString& word);
   int      GetAlternatives(CString& word,std::vector<CString>& all);
   void     AddToCustom(CString& word);
   void     AddToIgnore(CString& word);
   bool     ChangeDictionary(CString language);
   // General getters/setters
   CString          GetDictionaryName();
   DictionaryFiles* GetDictionaryFiles();
   CString          GetLanguageName();
   bool             GetOption(SpelOption p_option);
   void             SetOption(SpelOption p_option,bool p_value);
   // Rendering of spelling errors
   void MakeRenderstyle(CComPtr<IHTMLDocument2> pDoc);
   CComPtr<IHTMLRenderStyle> GetRenderStyle();

private:
   void     Reset();
   void     InitDiacrites();
   bool     ReadDictionaries();
   bool     WriteDictionaries();
   bool     ReadDictionariesList();
   CString  ReadDictonaryHeader(CString& name);
   void     ReadOptions();
   void     WriteOptions();
   SoundAs  CalculateSoundex(CString& word);
   bool     ReadDictionary(FILE* input
                          ,SpellDictionary& dict
                          ,Dictionary& list);
   bool     WriteDictionary(FILE* output,Dictionary& list);
   void     StatusText(CString text);
   void     DeDiacrite(CString& word);
   // Should re-write dictionaries?
   bool             m_dirty;
   // Should read dictionaries?
   bool             m_readDict;
   // Main spelling wordlist
   SpellDictionary  m_mainSpelling;
   Dictionary       m_mainDictionary;
   // Custom users wordlist
   SpellDictionary  m_customSpelling;
   Dictionary       m_customDictionary;
   // Ignore list
   SpellDictionary  m_ignoreSpelling;
   Dictionary       m_ignoreDictionary;

   // Options
   bool             m_enable;             // CHecking is enabled
   bool             m_ignoreAllCapitals;  // All CAPITALIZED words 
   bool             m_ignoreNumberWords;  // Words with numbers in them
   bool             m_showErrors;         // Show errors in document
   bool             m_useCustom;          // Use/Ignore custom wordlist
   // All the dictionaries / Current
   DictionaryFiles  m_allDictionaries;
   CString          m_nameMainDictionary; // Name current main dictionary

   // Text rendering
   IHTMLRenderStyle* m_renderStyle;
};

inline CString
Spelling::GetDictionaryName()
{
  return m_nameMainDictionary;
}

inline DictionaryFiles* 
Spelling::GetDictionaryFiles()
{
  return &m_allDictionaries;
}

inline CComPtr<IHTMLRenderStyle> 
Spelling::GetRenderStyle()
{
  return m_renderStyle;
}
