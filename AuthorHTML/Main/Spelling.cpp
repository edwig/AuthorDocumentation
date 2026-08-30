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
  m_nameMainDictionary = settings.GetSetting("MainDictionary","en_us.dict");

  opt = settings.GetSetting("SPELL_Enable",             "1"); m_enable            = (atoi(opt) != 0);
  opt = settings.GetSetting("SPELL_UseCustomDictionary","1"); m_useCustom         = (atoi(opt) != 0);
  opt = settings.GetSetting("SPELL_ShowSpellingErrors" ,"1"); m_showErrors        = (atoi(opt) != 0);
  opt = settings.GetSetting("SPELL_IgnoreAllCapitals",  "1"); m_ignoreAllCapitals = (atoi(opt) != 0);
  opt = settings.GetSetting("SPELL_IgnoreNumberWords",  "1"); m_ignoreNumberWords = (atoi(opt) != 0);
}

void
Spelling::WriteOptions()
{
  CString nameMain = Misc::FilenamePart(m_nameMainDictionary);
  settings.SetSetting("MainDictionary",nameMain);

  settings.SetSetting("SPELL_Enable",             m_enable            ? "1" : "0");
  settings.SetSetting("SPELL_UseCustomDictionary",m_useCustom         ? "1" : "0");
  settings.SetSetting("SPELL_ShowSpellingErrors", m_showErrors        ? "1" : "0");
  settings.SetSetting("SPELL_IgnoreAllCapitals",  m_ignoreAllCapitals ? "1" : "0");
  settings.SetSetting("SPELL_IgnoreNumberWords",  m_ignoreNumberWords ? "1" : "0");
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
    if(isprint(c))
    {
      if(isdigit(c)) foundDigit = true;
      if(isalpha(c)) foundAlpha = true;
      if(isupper(c)) foundUpper = true;
      if(islower(c)) foundLower = true;
      if(ispunct(c)) foundPunct = true;
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
  struct _finddata_t  fileinfo;
  intptr_t            fileHandle;
  CString pattern = "*.dict";
  CString directory = theApp.GetBinDirectory() + "..\\spell\\";

  m_allDictionaries.clear();

  pattern = directory + pattern;
  fileHandle = _findfirst(pattern,&fileinfo);
  if(fileHandle != -1)
  {
    do
    {
      if(!(fileinfo.attrib & _A_SUBDIR)         && 
           stricmp(fileinfo.name,"custom.dict") &&
           stricmp(fileinfo.name,"ignore.dict") )
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
          TRACE("Dictionary: %s : %s\n",name.GetString(),filename.GetString());
        }
      }
    }
    while(_findnext(fileHandle,&fileinfo) == 0);
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
  return "";
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
  settings.SetSetting("MainDictionary",nameDictionary);
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
  text.Format("Loading dictionary for language: %s",language.GetString());
  MainFrame::SetStatusText(text);
}

bool    
Spelling::ReadDictionaries()
{
  CString errors;
  extern AuthorHTMLApp theApp;
  CString binDir = theApp.GetBinDirectory();
  CString dictionaryName = binDir + "..\\spell\\" + m_nameMainDictionary;
  CString dictCustom = binDir + "..\\spell\\custom.dict";
  CString dictIgnore = binDir + "..\\spell\\ignore.dict";

  if(!m_enable)
  {
    // Why bother?
    return true;
  }
  CWaitCursor take_a_deep_breath;
  StatusText(m_nameMainDictionary);

  FILE* input = fopen(dictionaryName,"r");
  if(input)
  {
    if(ReadDictionary(input,m_mainSpelling,m_mainDictionary))
    {
      fclose(input);
    }
    else
    {
      // Could not read main spelling dictionary: %s
      errors.Format("Could not read main spelling dictionary: %s",dictionaryName.GetString());
    }
  }
  else
  {
    // Could not open or find the main spelling dictionary: %s
    errors.Format("Could not open or find the main spelling dictionary: %s",dictionaryName.GetString());
  }
  input = fopen(dictCustom,"r");
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
      error.Format("Could not read custom spelling dictionary: %s",dictCustom.GetString());
      errors += error;
    }
  }
  else
  {
    // Could not open or find the custom spelling dictionary: %s
    CString error;
    error.Format("Could not open or find the custom spelling dictionary: %s",dictCustom.GetString());
    errors += error;
  }
  input = fopen(dictIgnore,"r");
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
      error.Format("Could not read ignore spelling dictionary: %s",dictIgnore.GetString());
      errors += error;
    }
  }
  else
  {
    // Could not open or find the ignore spelling dictionary: %s
    CString error;
    error.Format("Could not open or find the ignore spelling dictionary: %s",dictIgnore.GetString());
    errors += error;
  }
  
  MainFrame::SetStatusText("");
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
    CString dictCustom = binDir + "..\\spell\\custom.dict";
    CString dictIgnore = binDir + "..\\spell\\ignore.dict";

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
    output = fopen(dictCustom,"w");
    if(output)
    {
      fprintf(output,"# Custom Dictionary\n");
      fprintf(output,"# Author Dictionary\n");
      if(WriteDictionary(output,m_customDictionary))
      {
        fclose(output);
      }
      else
      {
        // Error while re-writing the custom dictionary: %s
        CString error;
        error.Format("Error while re-writing the custom dictionary: %s",dictCustom.GetString());
        errors += error;
      }
    }
    else
    {
      // Could not open the custom dictionary '%s' for writing
      CString error;
      error.Format("Could not open the custom dictionary '%s' for writing",dictCustom.GetString());
      errors += error;
    }
    output = fopen(dictIgnore,"w");
    if(output)
    {
      fprintf(output,"# Ignore Dictionary\n");
      fprintf(output,"# Author Dictionary\n");
      if(WriteDictionary(output,m_ignoreDictionary))
      {
        fclose(output);
      }
      else
      {
        CString error;
        error.Format("Error while re-writing the ignore-words dictionary: %s",dictIgnore.GetString());
        errors += error;
      }
    }
    else
    {
      CString error;
      error.Format("Could not open the ignore-words dictionary: %s",dictIgnore.GetString());
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
  char* pnt;
  int   len = (word.GetLength() < 9) ? 9 : word.GetLength() + 1;
  char* theWord      = (char *)malloc(len);
  char* resultstring = (char *)malloc(len);
  CString noDiacrite = word;
  DeDiacrite(noDiacrite);

  // Local copy of the word
  strcpy(theWord,noDiacrite.GetString());
  if(strlen(theWord) > 1)
  {
    // STEP 1: Retain first letter en remove vowel-like letters
    pnt = &theWord[1];
    while(*pnt)
    {
      if(tolower(*pnt)=='a' || tolower(*pnt)=='e' || tolower(*pnt)=='i' || 
         tolower(*pnt)=='o' || tolower(*pnt)=='u' || tolower(*pnt)=='y' )
      {
        memmove(pnt,(pnt+1),strlen(pnt) + 1);
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
      if(tolower(*pnt)=='h' || tolower(*pnt)=='w')
      {
        memmove(pnt,(pnt+1),strlen(pnt) + 1);
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
      if(tolower(*pnt)=='b' || tolower(*pnt)=='f' || 
         tolower(*pnt)=='p' || tolower(*pnt)=='v' )
      {
        *pnt = '1';
      }
      // SSSS sound
      else if(tolower(*pnt)=='c' || tolower(*pnt)=='g' || tolower(*pnt)=='j' || 
              tolower(*pnt)=='k' || tolower(*pnt)=='q' || tolower(*pnt)=='s' || 
              tolower(*pnt)=='x' || tolower(*pnt)=='z' )
      {
        *pnt = '2';
      }
      // DTDT sound
      else if(tolower(*pnt)=='d' || tolower(*pnt)=='t')
      {
        *pnt = '3';
      }
      // LLLLL sound
      else if(tolower(*pnt)=='l')
      {
        *pnt = '4';
      }
      // MMMNNN sound
      else if(tolower(*pnt)=='m' || tolower(*pnt)=='n')
      {
        *pnt = '5';
      }
      // ROLLING sound
      else if(tolower(*pnt)=='r')
      {
        *pnt = '6';
      }
      ++pnt;
    }
    // STEP 4: Strip adjacent sounding chars
    pnt = theWord;
    while(*pnt && *(pnt+1))
    {
      if(*pnt == *(pnt+1))
      {
        memmove(pnt,(pnt+1),strlen(pnt) + 1);
      }
      else
      {
        ++pnt;
      }
    }
  }
  // Make resultstring and pad until 8 chars
  int x;
  strncpy(resultstring,theWord,len);
  for(x=0;x<8;++x)
  {
    if(resultstring[x] == 0)
    {
      resultstring[x  ] = '0';
      resultstring[x+1] =  0;
    }
  }
  SoundAs sound = 0;
  for(x=0;x<8;++x)
  {
    sound <<= 8;
    sound  += (int) (resultstring[x] - '0');
  }
  free(theWord);
  free(resultstring);
  return sound;
}

CString
Spelling::ReadDictonaryHeader(CString& filename)
{
  FILE* file = fopen(filename,"r");
  if(!file)
  {
    return "";
  }
  char buffer[256];
  CString dictName;

  while(fgets(buffer,256,file))
  {
    int len = (int)strlen(buffer);
    if(!len) continue;
    if(buffer[len-1] == '\n')
    {
      buffer[--len] = 0;
    }
    if(!len) continue;
    if(buffer[0] == '#')
    {
      if(dictName.IsEmpty())
      {
        dictName = &buffer[2];
        continue;
      }
      if(strcmp(buffer,"# Author Dictionary"))
      {
        // Error
        dictName = "";
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
  char buffer [256];
  char already[256];
  int  point;

  while(fgets(buffer,256,input))
  {
    int len = (int)strlen(buffer);
    if(!len) continue;
    if(buffer[len-1] == '\n')
    {
      buffer[--len] = 0;
    }
    if(!len) continue;
    if(buffer[0] == '#')
    {
      // This is a remark
      continue;
    }
    point = buffer[0] - '1' + 1;
    strcpy(&already[point],&buffer[1]);

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
  char buffer [256];
  char already[256] = "";
  int point = 0;

  for(Dictionary::iterator it = list.begin();it != list.end();++it)
  {
    CString word = *it;
    strcpy(buffer,word.GetString());
    int len = (int) strlen(buffer);
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
    char num = (char)('1' + point - 1);
    fprintf(output,"%c%s\n",num,&buffer[point]);
    strcpy(already,buffer);
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
    _diacrites[ind] = ' ';
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