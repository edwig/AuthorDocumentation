//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    Misc.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Class with miscallaneous functions
//
#pragma once

#if _MSC_VER <= 1200 // Visual Studio 6.0
#error "This code is written for Visual Studio 7 or higher"
#endif

// Remove warning CRegKey::SetValue is deprecated
#pragma warning(disable : 4996)

#include "StdAfx.h"
#include <afxtoolbarcomboboxbutton.h>
#include <atlbase.h>    // CComPtr
#include <afxhtml.h>    // CHtmlEditView
#include <vector>
#include <string>

using namespace std;

// Types of registry keys
#define _HKLM  HKEY_LOCAL_MACHINE
#define _HKCU  HKEY_CURRENT_USER
#define _HKCR  HKEY_CLASSES_ROOT

// Types of special characters in dialog
#define ACC_LOWER 1
#define ACC_UPPER 2
#define OTH_LOWER 3
#define OTH_UPPER 4
#define GRE_LOWER 5 // Greek ulower
#define GRE_UPPER 6 // Greek upper
#define COMMON    7

// Types of tags in TagEvents
#define TAGS_ALL      0x0001
#define TAGS_BODY     0x0002
#define TAGS_INPUT    0x0004
#define TAGS_SELECT   0x0008
#define TAGS_TEXTAREA 0x0010
#define TAGS_TABLE    0x0020
#define TAGS_SCRIPT   0x0040
#define TAGS_STYLE    0x0080
#define TAGS_OBJECT   0x0100
#define TAGS_MARQUEE  0x0200
#define TAGS_IMG      0x0400
#define TAGS_FORM     0x0800

// For execution
#define MAX_COMMANDLINE 1024

// Tokens needed for the parsing of the project definition files
// These are the PRJ/HHC/HHK files of the Microsoft Help Compiler Workshop
//
typedef enum _Token
{
   PF_NOTOKEN = 0
  ,PF_EOF
  ,PF_EQUAL
  ,PF_TYPE
  ,PF_PARAM
  ,PF_NAME
  ,PF_VALUE
  ,PF_STRING
  ,PF_HTML
  ,PF_COMMENT
  ,PF_HEAD
  ,PF_BODY
  ,PF_OBJECT
  ,PF_LIST
  ,PF_LISTITEM
  ,PF_ANCHOR
  ,PF_BREAK
  ,PF_ENDHTML
  ,PF_ENDHEAD
  ,PF_ENDBODY
  ,PF_ENDOBJECT
  ,PF_ENDLIST
  ,PF_ENDLISTITEM
  ,PF_ENDANCHOR
}
TOCToken;

typedef unsigned char uchar;

typedef struct _letters
{
  uchar letter;
  uchar diacrite;
  int   lett_category;
  char* htmlText;
  char* descript;
}
Diacrites;

typedef struct _taginfo 
{
  char *tagname;          // Name of the tag
  char *description;      // English description
  bool  is_blockformat;   // Is a blockformat (settable)
  bool  is_userselect;    // User selectable (from styles dialog)
  bool  is_used;          // Already used in combo
  bool  not_ie_explorer;  // Not in MS-IE-Explorer
}
TagInfo;

typedef struct _attributeInfo
{
  char  *name;    // Name of the attribute
  char  *value;   // Value of the attribute
  char  *display; // Display in combobox
}
AttributeInfo;

typedef struct _internet_colors
{
  char* name;   // Internet safe name
  char* color;  // #RRGGBB color code
}
InternetColors;

typedef struct _tagevents
{
  bool    standard;
  CString event;
  CString description;
  UINT    tags;
}
TagEvents;

// Static arrays with various definitions
extern Diacrites      DiacriteLetters[];
extern TagInfo        taginfo[];
extern AttributeInfo  attributes[];
extern InternetColors internet_colors[];
extern TagEvents      tagEvents[];

class Misc
{
public:
  static void     GetAllTags(vector<string>* all);
  static CString  DecodeMime(CString s_Text);
  static CString  EncodeHtml(CComBSTR bs_Html);
  static void     EncodeHtml(const WCHAR *u16_In, UINT *pu32_WcharToCopy, char *s8_Out, UINT *pu32_OutSize);
  static CString  VarToStr(CComVariant &v_Variant);
  static CString  RemoveTag(CString s_Html, CString s_Tag);
  static CString  AddTagClass(CString s_Html,CString s_tag,CString s_class);
  static CString  CutString(CString s_In, CString s_Cut, BOOL b_End);
  static void     RegWriteString(HKEY h_Class, CString s_Path, CString s_Key, CString s_Value);
  static void     RegWriteDword (HKEY h_Class, CString s_Path, CString s_Key, DWORD u32_Value);
  static void     DecodeColor(CString p_color,int& red,int& green,int& blue);
  static CString  GetInternetColor(CString& p_color);
  static CString  GetDiacriteHtml(uchar letter);
  static BOOL     MoveCaretToElement(CComPtr<IHTMLDocument2> doc2,CComPtr<IHTMLElement> pElem,CARET_DIRECTION pDir,bool select_all = true);
  static bool     SetCurrentSelectionText(CComPtr<IHTMLDocument2> pDoc,CComPtr<IHTMLElement> pElem,bool select_all = true);
  static CString  CombinedClassName(CString& p_tag,CString& p_class,bool& p_userselect);
  static void     ReconstructTagClass(CString& p_combined,CString& tagname,CString& classname);
  static void     ExtractStylesList(CComPtr<IHTMLDocument2> doc2,CMFCToolBarComboBoxButton* pCombo);
  static bool     IsBlockTag(CString tagname);
  static bool     IsUserSelectableTag(CString tagname);
  static CString  GetTagDescription(CString tagname);
  static void     MoveCaretToPoint(CComPtr<IHTMLDocument2> doc,LPPOINT ppt);
  static BOOL     GetElementByPositionAndTag(CComPtr<IHTMLDocument2> doc
                                            ,CComPtr<IHTMLElement>&  pElem
                                            ,int xHit
                                            ,int yHit
                                            ,CString tag);
  static CComPtr<IHTMLElement> CreateHeadElement(CComPtr<IHTMLDocument2> doc,ELEMENT_TAG_ID tagID);
  static CComPtr<IHTMLElement> CreateChildElement(CComPtr<IHTMLDocument2> p_doc,CComPtr<IHTMLElement> p_parent,ELEMENT_TAG_ID p_tagID);
  static CComPtr<IHTMLElement> AreaToImg(CComPtr<IHTMLDocument2> doc,CComPtr<IHTMLElement> pElement);
  static CComPtr<IHTMLElement> GetByID(CComPtr<IHTMLDocument2>& doc,CString ident);
  static CString  MetaTag(CComPtr<IHTMLDocument2>& doc,CString name,CString* value=NULL);
  static CString  BaseTag(CComPtr<IHTMLDocument2>& doc,CString uri, CString target);
  static long     GetUniqueID(CComPtr<IHTMLDocument2>& doc,CString tagname);
  static CString  CreateNewMap(CComPtr<IHTMLDocument2>& doc);
  static bool     CreateNewArea(CComPtr<IHTMLDocument2>& doc
                               ,CString mapID
                               ,CString areaID
                               ,CString shape);
  // FILES AND STRINGS
  static CString  StripFileProtocol(CString fileref);
  static CString  StripPopup(CString fileref);
  static bool     SplitMidpageAnchor(const CString& p_fileref,CString& p_file,CString& p_anchor);
  static int      FindNoCase (CString line,CString part,int pos = 0);
  static int      FindReverse(CString line,CString part,int pos = -1);
  static int      FindReverseNoCase(CString line,CString part,int pos = -1);
  static CString  SubStr(CString& p_string,char p_Delim,int p_Num);
  static CString  GetParamFromCommandline(CString& commandline,CString p_commando,char p_delim = '=');
  static int      CountNL(CString& sCurSentence,int pos);
  static CString  BasenamePart(CString fullpath);
  static CString  FilenamePart (CString fullpath);
  static CString  ExtensionPart(CString fullpath);
  static CString  DirectoryPart(CString fullpath);
  static CString  RemoveBasePart(CString base,CString fullpath);
  static CString  ReduceDirectoryPath(CString& path);
  static void     ParseNumber(CString& def,unsigned long& number);
  static bool     IsANumber(CString& def,long& number,double& broken);

  // ATTRIBUTES
  static CString  GetAttributeDisplayname(CString attribute,CString value);
  static CString  GetAttributeValue(CString attribute,CString display);
  static void     GetAllAttributeDisplaynames(CString attribute,vector<string>* all);
  static bool     MakeRelativePathname(CString& p_base,CString& p_absolute,CString& p_relative);
  static int      ResolveSpecialChars(CString& value);
  static int      GetAllEvents(vector<CString>& all,bool standard,UINT tags);
  static CString  GetEventDescription(CString event,UINT tags);
  static int      PointSizeToFontSize(int pointsize);
  static int      FontSizeToPointSize(int fontsize);

  // PROJECT HHC HHK type of files parsing
  static TOCToken GetToken (FILE* file,CString& word,  long& linenumber);
  static bool     SkipToken(FILE* file,TOCToken expect,long& linenumber);
  static void     PushToken(CString& word,TOCToken token);
  static void     SkipBOM(FILE* p_file);

  // Renaming file references
  static bool     ReplaceFilenameExtra(CString& p_base
                                      ,CString& p_old_href
                                      ,CString& p_new_href
                                      ,CString& p_input_ref
                                      ,CString& p_output_ref);

  // EXEcution
  static CString  GetExePath();
  static int      StartProgramma(CString& p_program
                                ,CString& p_arguments
                                ,bool     p_currentdir
                                ,bool     p_waitForExit
                                ,bool     p_show = false);

protected:
  static void    InitMaps();
};
