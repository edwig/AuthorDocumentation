//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    CSS_utils.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Utility definitions and functions for CSS manipulations
//
#pragma once
#include "umap.hpp"
#include <string>

class CssStyleSheet;

typedef umap<string, string>   pstore;
typedef umap<string, pstore >  sstore;
typedef umap<string, sstore>   css_struct;

enum parse_status
{
   is     // In a selector
  ,ip     // In a property
  ,iv     // In a value
  ,instr  // In a string
  ,ic     // In a comment
  ,at     // In an @ at-block
};

enum message_type
{
   Information
  ,Warning
  ,Error
};

enum token_type
{
   AT_START
  ,AT_END
  ,SEL_START
  ,SEL_END
  ,PROPERTY
  ,VALUE
  ,COMMENT
};

struct token
{
  token_type type;
  string     data;
};

struct message
{
  string       m;
  message_type t;
};

CString
CSSPropertyGet(CssStyleSheet* css
              ,string& selector
              ,string  property
              ,bool unspec /*=true*/);

void
CSSPropertyPut(CssStyleSheet* css
              ,string& selector
              ,string  property
              ,CString value
              ,bool    unspec);

void     CssSplitValueUnits(CString property,CString& value,CString& units);

CComBSTR CssCtoB(CString value);

// Possibly convert CSS values to a different unit
CString  CssConvertToUnit(CString  p_value                  // Input value
                         ,CString  p_units                  // Convert to these units
                         ,CString& p_origUnit               // Save resulting units
                         ,bool     p_warning  = false);     // Give absolute/relative warning

// Setting all the size options in the combobox for a measurement
void CSSComboBoxUnits(CComboBox& p_combo,CString p_init = "px");
