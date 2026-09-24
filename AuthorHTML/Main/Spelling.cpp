//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    Spelling.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Check the spelling of the text of a HTML documentation page
//              against the provided language dictionaries
//
#include "StdAfx.h"
#include "Spelling.h"
#include "AuthorHTML.h"
#include "MainFrm.h"
#include "Misc.h"
#include <io.h>

Spelling::Spelling()
         :m_dirty(false)
         ,m_readDict(false)
         ,m_renderStyle(NULL)
{
  InitDiacrites();
  ReadOptions();
  ReadDictionariesList();
}

Spelling::~Spelling()
{
  if(m_dirty)
  {
    WriteDictionaries();
  }
  WriteOptions();
  Reset();
  m_allDictionaries.clear();
  m_renderStyle = NULL;
}

void
Spelling::Reset()
{
  m_mainDictionary  .clear();
  m_mainSpelling    .clear();
  m_customDictionary.clear();
  m_customSpelling  .clear();
  m_ignoreSpelling  .clear();
  m_ignoreDictionary.clear();
  m_readDict = false;
}

void
Spelling::MakeRenderstyle(CComPtr<IHTMLDocument2> pDoc)
{
  if(m_renderStyle)
  {
    return;
  }
  CComQIPtr<IHTMLDocument4,&IID_IHTMLDocument4> doc4 = pDoc;
  HRESULT hr = doc4->createRenderStyle(NULL,&m_renderStyle);
  if(SUCCEEDED(hr))
  {
    CComVariant color;
    V_VT  (&color) = VT_BSTR;

    m_renderStyle->put_defaultTextSelection(L"false");
    m_renderStyle->put_textDecoration(L"underline");
    m_renderStyle->put_textUnderlineStyle(L"wave");
    CComBSTR fore = L"auto";
    V_BSTR(&color) = fore;
    m_renderStyle->put_textColor(color);
    CComBSTR back = L"transparent";
    V_BSTR(&color) = back;
    m_renderStyle->put_textBackgroundColor(color);
    CComBSTR bColor = L"red";
    V_BSTR(&color) = bColor;
    m_renderStyle->put_textDecorationColor(color);
  }
}

void
Spelling::ReadOptions()
{
  CString opt;
  m_nameMainDictionary = settings.GetSetting(_T("MainDictionary"),_T("en_us.dict"));

  opt = settings.GetSetting(_T("SPELL_Enable"),             _T("1")); m_enable            = (_ttoi(opt) != 0);
  opt = settings.GetSetting(_T("SPELL_UseCustomDictionary"),_T("1")); m_useCustom         = (_ttoi(opt) != 0);
  opt = settings.GetSetting(_T("SPELL_ShowSpellingErrors") ,_T("1")); m_showErrors        = (_ttoi(opt) != 0);
  opt = settings.GetSetting(_T("SPELL_IgnoreAllCapitals"),  _T("1")); m_ignoreAllCapitals = (_ttoi(opt) != 0);
  opt = settings.GetSetting(_T("SPELL_IgnoreNumberWords"),  _T("1")); m_ignoreNumberWords = (_ttoi(opt) != 0);
}

void
Spelling::WriteOptions()
{
  CString nameMain = Misc::FilenamePart(m_nameMainDictionary);
  settings.SetSetting(_T("MainDictionary"),nameMain);

  settings.SetSetting(_T("SPELL_Enable"),             m_enable            ? _T("1") : _T("0"));
  settings.SetSetting(_T("SPELL_UseCustomDictionary"),m_useCustom         ? _T("1") : _T("0"));
  settings.SetSetting(_T("SPELL_ShowSpellingErrors"), m_showErrors        ? _T("1") : _T("0"));
  settings.SetSetting(_T("SPELL_IgnoreAllCapitals"),  m_ignoreAllCapitals ? _T("1") : _T("0"));
  settings.SetSetting(_T("SPELL_IgnoreNumberWords"),  m_ignoreNumberWords ? _T("1") : _T("0"));
}

bool 
Spelling::CheckWord(CString& p_word)
{
  if(!m_enable)
  {
    // Real quick answer!
    return true;
  }
  CString word = p_word;
  DeDiacrite(word);
  bool foundDigit = false;
  bool foundAlpha = false;
  bool foundUpper = false;
  bool foundLower = false;
  bool foundPunct = false;
  for(int ind = 0; ind < word.GetLength(); ++ind)
  {
    int c = word.GetAt(ind);
    if(_istprint(c))
    {
      if(_istdigit(c)) foundDigit = true;
      if(_istalpha(c)) foundAlpha = true;
      if(_istupper(c)) foundUpper = true;
      if(_istlower(c)) foundLower = true;
      if(_istpunct(c)) foundPunct = true;
    }
  }
  if(m_ignoreNumberWords)
  {
    if(foundDigit&& !foundAlpha)
    {
      return true;
    }
  }
  if(m_ignoreAllCapitals)
  {
    if(foundUpper && !foundLower)
    {
      return true;
    }
  }
  if(foundPunct && !foundAlpha && !foundDigit)
  {
    // Only punctuation found (", ', ," etc)
    return true;
  }
  // If not already: read them in.
  if(!m_readDict)
  {
    ReadDictionaries();
  }
  Dictionary::iterator pos = m_mainDictionary.lower_bound(p_word);
  if(pos != m_mainDictionary.end())
  {
    if(p_word == *pos)
    {
      // OK, Found in main dictionary
      return true;
    }
  }
  if(m_useCustom)
  {
    pos = m_customDictionary.lower_bound(p_word);
    if(pos != m_customDictionary.end())
    {
      if(p_word == *pos)
      {
        // OK, Found in custom dictionary
        return true;
      }
    }
  }
  pos = m_ignoreDictionary.lower_bound(p_word);
  if(pos != m_ignoreDictionary.end())
  {
    if(p_word == *pos)
    {
      // OK, Found in the ignore-words list
      return true;
    }
  }
  // Not found directly
  return false;
}

// Get the alternatives for a word.
int
Spelling::GetAlternatives(CString& p_word,vector<CString>& all)
{
  int number = 0;

  SoundAs sound = CalculateSoundex(p_word);

  if(!m_readDict)
  {
    ReadDictionaries();
  }
  SpellDictionary::iterator bot = m_mainSpelling.lower_bound(sound);
  SpellDictionary::iterator top = m_mainSpelling.upper_bound(sound);

  while(bot != m_mainSpelling.end() && bot!= top)
  {
    all.push_back(bot->second);
    ++number;
    ++bot;
  }

  if(m_useCustom)
  {
    bot = m_customSpelling.lower_bound(sound);
    top = m_customSpelling.upper_bound(sound);

    while(bot != m_customSpelling.end() && bot!= top)
    {
      all.push_back(bot->second);
      ++number;
      ++bot;
    }
  }
  return number;
}

bool    
Spelling::ReadDictionariesList()
{
  struct _tfinddata_t  fileinfo;
  intptr_t            fileHandle;
  CString pattern = _T("*.dict");
  CString directory = theApp.GetBinDirectory() + _T("..\\spell\\");

  m_allDictionaries.clear();

  pattern = directory + pattern;
  fileHandle = _tfindfirst(pattern,&fileinfo);
  if(fileHandle != -1)
  {
    do
    {
      if(!(fileinfo.attrib & _A_SUBDIR)        && 
           _tcsstr(fileinfo.name,_T("custom")) == 0 &&
           _tcsstr(fileinfo.name,_T("ignore")) == 0 )
      {
        // OK, found a dictionary (not the custom/ignore dictionary)
        CString filename = directory + CString(fileinfo.name);
        CString name = ReadDictonaryHeader(filename);
        if(!name.IsEmpty())
        {
          DictFile dict;
          dict.name     = name;
          dict.fileName = fileinfo.name;
          m_allDictionaries.push_back(dict);
          // TRACE("Dictionary: %s : %s\n",name.GetString(),filename.GetString());
        }
      }
    }
    while(_tfindnext(fileHandle,&fileinfo) == 0);
    _findclose(fileHandle);
  }
  return true;
}

CString
Spelling::GetLanguageName()
{
  CString currentMain = Misc::FilenamePart(m_nameMainDictionary);
  for(unsigned int ind=0;ind < m_allDictionaries.size(); ++ind)
  {
    DictFile df = m_allDictionaries[ind];
    if(currentMain == df.fileName)
    {
      return df.name;
    }
  }
  return _T("");
}

bool
Spelling::ChangeDictionary(CString language)
{
  CString nameDictionary;
  CString currentMain = Misc::FilenamePart(m_nameMainDictionary);
  for(unsigned int ind = 0; ind < m_allDictionaries.size(); ++ ind)
  {
    DictFile df = m_allDictionaries[ind];
    if(df.name == language)
    {
      nameDictionary = df.fileName;
      if(nameDictionary.CompareNoCase(currentMain) == 0)
      {
        // No change of language
        return true;
      }
      break;
    }
  }
  if(nameDictionary.IsEmpty())
  {
    return false;
  }
  settings.SetSetting(_T("MainDictionary"),nameDictionary);
  m_nameMainDictionary = nameDictionary;
  Reset();
  return ReadDictionaries();
}

void
Spelling::StatusText(CString name)
{
  CString language;
  for(unsigned int ind = 0; ind < m_allDictionaries.size(); ++ ind)
  {
    DictFile df = m_allDictionaries[ind];
    if(df.fileName == name)
    {
      language = df.name;
      break;
    }
  }
  CString text;
  text.Format(_T("Loading dictionary for language: %s"),language.GetString());
  MainFrame::SetStatusText(text);
}

bool    
Spelling::ReadDictionaries()
{
  CString errors;
  extern AuthorHTMLApp theApp;
  CString binDir = theApp.GetBinDirectory();
  CString dictionaryName = binDir + _T("..\\spell\\") + m_nameMainDictionary;
  CString basefile   = Misc::FilenamePart(dictionaryName);
  CString dictCustom = binDir + _T("..\\spell\\custom_") + basefile;
  CString dictIgnore = binDir + _T("..\\spell\\ignore_") + basefile;

  if(!m_enable)
  {
    // Why bother?
    return true;
  }
  CWaitCursor take_a_deep_breath;
  StatusText(m_nameMainDictionary);

  FILE* input = _tfopen(dictionaryName,_T("r"));
  if(input)
  {
    if(ReadDictionary(input,m_mainSpelling,m_mainDictionary))
    {
      fclose(input);
    }
    else
    {
      // Could not read main spelling dictionary: %s
      errors.Format(_T("Could not read main spelling dictionary: %s"),dictionaryName.GetString());
    }
  }
  else
  {
    // Could not open or find the main spelling dictionary: %s
    errors.Format(_T("Could not open or find the main spelling dictionary: %s"),dictionaryName.GetString());
  }
  input = _tfopen(dictCustom,_T("r"));
  if(input)
  {
    if(ReadDictionary(input,m_customSpelling,m_customDictionary))
    {
      fclose(input);
    }
    else
    {
      // Could not read custom spelling dictionary: %s
      CString error;
      error.Format(_T("Could not read custom spelling dictionary: %s"),dictCustom.GetString());
      errors += error;
    }
  }
  else
  {
    // Could not open or find the custom spelling dictionary: %s
    CString error;
    error.Format(_T("Could not open or find the custom spelling dictionary: %s"),dictCustom.GetString());
    errors += error;
  }
  input = _tfopen(dictIgnore,_T("r"));
  if(input)
  {
    if(ReadDictionary(input,m_ignoreSpelling,m_ignoreDictionary))
    {
      fclose(input);
    }
    else
    {
      // Could not read ignore dictionary: %s
      CString error;
      error.Format(_T("Could not read ignore spelling dictionary: %s"),dictIgnore.GetString());
      errors += error;
    }
  }
  else
  {
    // Could not open or find the ignore spelling dictionary: %s
    CString error;
    error.Format(_T("Could not open or find the ignore spelling dictionary: %s"),dictIgnore.GetString());
    errors += error;
  }
  
  MainFrame::SetStatusText(_T(""));
  if(!errors.IsEmpty())
  {
    theApp.ErrorMessage(errors);
    return false;
  }
  // OK, We did read it.
  m_readDict = true;
  return true;
}

bool    
Spelling::WriteDictionaries()
{
  if(m_dirty)
  {
    CString errors;
    FILE*   output;
    CString binDir = theApp.GetBinDirectory();
    CString dictCustom = binDir + _T("..\\spell\\custom.dict");
    CString dictIgnore = binDir + _T("..\\spell\\ignore.dict");

    CString nameMain = Misc::FilenamePart(m_nameMainDictionary);

    /*
    // TODO: Re-Writing the main dictionary????

    output = fopen(m_nameMainDictionary,"w");
    if(output)
    {
      if(WriteDictionary(output,m_mainDictionary))
      {
        fclose(output);
      }
      else
      {
        // Error while re-writing the main dictionary: %s
        errors.Format("Error while re-writing the main dictionary: %s",m_nameMainDictionary);
      }
    }
    else
    {
      // Could not open the main dictionary '%s' for writing
      errors.Format("Could not open the main dictionary '%s' for writing",m_nameMainDictionary);
    }
    */
    output = _tfopen(dictCustom,_T("w"));
    if(output)
    {
      _ftprintf(output,_T("# Custom Dictionary\n"));
      _ftprintf(output,_T("# Author Dictionary\n"));
      if(WriteDictionary(output,m_customDictionary))
      {
        fclose(output);
      }
      else
      {
        // Error while re-writing the custom dictionary: %s
        CString error;
        error.Format(_T("Error while re-writing the custom dictionary: %s"),dictCustom.GetString());
        errors += error;
      }
    }
    else
    {
      // Could not open the custom dictionary '%s' for writing
      CString error;
      error.Format(_T("Could not open the custom dictionary '%s' for writing"),dictCustom.GetString());
      errors += error;
    }
    output = _tfopen(dictIgnore,_T("w"));
    if(output)
    {
      _ftprintf(output,_T("# Ignore Dictionary\n"));
      _ftprintf(output,_T("# Author Dictionary\n"));
      if(WriteDictionary(output,m_ignoreDictionary))
      {
        fclose(output);
      }
      else
      {
        CString error;
        error.Format(_T("Error while re-writing the ignore-words dictionary: %s"),dictIgnore.GetString());
        errors += error;
      }
    }
    else
    {
      CString error;
      error.Format(_T("Could not open the ignore-words dictionary: %s"),dictIgnore.GetString());
      errors += error;
    }
    if(errors.IsEmpty())
    {
      // rewrite OK
      m_dirty = false;
    }
    else
    {
      theApp.ErrorMessage(errors);
      return false;
    }
  }
  return true;
}

// Loosely based on D.Knuth's SoundEx function
SoundAs 
Spelling::CalculateSoundex(CString& word)
{
  TCHAR* pnt;
  int   len = (word.GetLength() < 9) ? 9 : word.GetLength() + 1;
  TCHAR* theWord      = (TCHAR *)malloc(len);
  TCHAR* resultstring = (TCHAR *)malloc(len);
  CString noDiacrite = word;
  DeDiacrite(noDiacrite);

  // Local copy of the word
  _tcscpy(theWord,noDiacrite.GetString());
  if(_tcslen(theWord) > 1)
  {
    // STEP 1: Retain first letter en remove vowel-like letters
    pnt = &theWord[1];
    while(*pnt)
    {
      if(_totlower(*pnt)==_T('a') || _totlower(*pnt)==_T('e') || _totlower(*pnt)==_T('i') || 
         _totlower(*pnt)==_T('o') || _totlower(*pnt)==_T('u') || _totlower(*pnt)==_T('y') )
      {
        memmove(pnt,(pnt+1),_tcslen(pnt) + 1);
      }
      else
      {
        ++pnt;
      }
    }
    // STEP 2: Remove intervening h and w: retain first letter
    pnt = &theWord[1];
    while(*pnt)
    {
      if(_totlower(*pnt)==_T('h') || _totlower(*pnt)==_T('w'))
      {
        memmove(pnt,(pnt+1),_tcslen(pnt) + 1);
      }
      else
      {
        ++pnt;
      }
    }
    // STEP 3: assign sounding numbers
    pnt = theWord;
    while(*pnt)
    {
      // PLOFFING sound
      if(_totlower(*pnt)==_T('b') || _totlower(*pnt)==_T('f') || 
         _totlower(*pnt)==_T('p') || _totlower(*pnt)==_T('v') )
      {
        *pnt = _T('1');
      }
      // SSSS sound
      else if(_totlower(*pnt)==_T('c') || _totlower(*pnt)==_T('g') || _totlower(*pnt)==_T('j') || 
              _totlower(*pnt)==_T('k') || _totlower(*pnt)==_T('q') || _totlower(*pnt)==_T('s') || 
              _totlower(*pnt)==_T('x') || _totlower(*pnt)==_T('z') )
      {
        *pnt = _T('2');
      }
      // DTDT sound
      else if(_totlower(*pnt)==_T('d') || _totlower(*pnt)==_T('t'))
      {
        *pnt = _T('3');
      }
      // LLLLL sound
      else if(_totlower(*pnt)==_T('l'))
      {
        *pnt = _T('4');
      }
      // MMMNNN sound
      else if(_totlower(*pnt)==_T('m') || _totlower(*pnt)==_T('n'))
      {
        *pnt = _T('5');
      }
      // ROLLING sound
      else if(_totlower(*pnt)==_T('r'))
      {
        *pnt = _T('6');
      }
      ++pnt;
    }
    // STEP 4: Strip adjacent sounding chars
    pnt = theWord;
    while(*pnt && *(pnt+1))
    {
      if(*pnt == *(pnt+1))
      {
        memmove(pnt,(pnt+1),_tcslen(pnt) + 1);
      }
      else
      {
        ++pnt;
      }
    }
  }
  // Make resultstring and pad until 8 chars
  int x;
  _tcsnccpy(resultstring,theWord,len);
  for(x=0;x<8;++x)
  {
    if(resultstring[x] == 0)
    {
      resultstring[x  ] = _T('0');
      resultstring[x+1] =  0;
    }
  }
  SoundAs sound = 0;
  for(x=0;x<8;++x)
  {
    sound <<= 8;
    sound  += (int) (resultstring[x] - _T('0'));
  }
  free(theWord);
  free(resultstring);
  return sound;
}

CString
Spelling::ReadDictonaryHeader(CString& filename)
{
  FILE* file = _tfopen(filename,_T("r"));
  if(!file)
  {
    return _T("");
  }
  TCHAR buffer[256];
  CString dictName;

  while(_fgetts(buffer,256,file))
  {
    int len = (int)_tcslen(buffer);
    if(!len) continue;
    if(buffer[len-1] == _T('\n'))
    {
      buffer[--len] = 0;
    }
    if(!len) continue;
    if(buffer[0] == _T('#'))
    {
      if(dictName.IsEmpty())
      {
        dictName = &buffer[2];
        continue;
      }
      if(_tcscmp(buffer,_T("# Author Dictionary")))
      {
        // Error
        dictName = _T("");
        break;
      }
    }
    break;
  }
  fclose(file);
  return dictName;
}

bool
Spelling::ReadDictionary(FILE* input
                        ,SpellDictionary& dict
                        ,Dictionary& list)
{
  TCHAR buffer [256];
  TCHAR already[256];
  int  point;

  while(_fgetts(buffer,256,input))
  {
    int len = (int)_tcslen(buffer);
    if(!len) continue;
    if(buffer[len-1] == _T('\n'))
    {
      buffer[--len] = 0;
    }
    if(!len) continue;
    if(buffer[0] == _T('#'))
    {
      // This is a remark
      continue;
    }
    point = buffer[0] - _T('1') + 1;
    _tcscpy(&already[point],&buffer[1]);

    CString theWord(already);
    SoundAs sound = CalculateSoundex(theWord);

    dict.insert(std::make_pair(sound,theWord));
    list.insert(theWord);
  }
  return true;
}

void
Spelling::AddToCustom(CString& word)
{
  if(m_useCustom)
  {
    SoundAs sound = CalculateSoundex(word);

    m_customSpelling  .insert(std::make_pair(sound,word));
    m_customDictionary.insert(word);
    m_dirty = true;
  }
}

void
Spelling::AddToIgnore(CString& word)
{
  SoundAs sound = CalculateSoundex(word);

  m_ignoreSpelling.insert(std::make_pair(sound,word));
  m_ignoreDictionary.insert(word);
  m_dirty = true;
}

bool
Spelling::WriteDictionary(FILE* output,Dictionary& list)
{
  TCHAR buffer [256];
  TCHAR already[256] = _T("");
  int point = 0;

  for(Dictionary::iterator it = list.begin();it != list.end();++it)
  {
    CString word = *it;
    _tcscpy(buffer,word.GetString());
    int len = (int) _tcslen(buffer);
    if(!len) continue;
    // Buffer now contains only a word
    for(point = 0; point < len; ++point)
    {
      if(already[point] && buffer[point])
      {
        if(already[point] != buffer[point])
        {
          break;
        }
      }
      else
      {
        break;
      }
    }
    TCHAR num = (TCHAR)(_T('1') + point - 1);
    _ftprintf(output,_T("%c%s\n"),num,&buffer[point]);
    _tcscpy(already,buffer);
  }
  return true;
}

bool
Spelling::GetOption(SpelOption p_option)
{
  switch(p_option)
  {
    case CHECKING_ENABLE:   return m_enable;
    case SHOW_SPELLERRORS:  return m_showErrors;
    case USE_CUSTOMDICT:    return m_useCustom;
    case IGNORE_ALLUPPER:   return m_ignoreAllCapitals;
    case IGNORE_NUMBERWORDS:return m_ignoreNumberWords;
  }
  return false;
}

void
Spelling::SetOption(SpelOption p_option,bool p_value)
{
  switch(p_option)
  {
    case CHECKING_ENABLE:     m_enable            = p_value; break;
    case SHOW_SPELLERRORS:    m_showErrors        = p_value; break;
    case USE_CUSTOMDICT:      m_useCustom         = p_value; break;
    case IGNORE_ALLUPPER:     m_ignoreAllCapitals = p_value; break;
    case IGNORE_NUMBERWORDS:  m_ignoreNumberWords = p_value; break;
  }
}

static int _diacrites[256];
 
void
Spelling::InitDiacrites()
{
  for(int ind=0; ind<128; ++ind)
  {
    _diacrites[ind] = ind;
  }
  for(int ind=128; ind<256; ++ind)
  {
    _diacrites[ind] = _T(' ');
  }
  Diacrites* diac = DiacriteLetters;
  while(diac->letter && (diac->lett_category <= OTH_UPPER))
  {
    int ind = diac->diacrite;
    _diacrites[ind] = diac->letter;
    ++diac;
  }
}

void
Spelling::DeDiacrite(CString& word)
{
  for(int ind=0;ind<word.GetLength();++ind)
  {
    uchar c = word.GetAt(ind);
    uchar r = (uchar) _diacrites[(int)c];
    word.SetAt(ind,r);
  }
}