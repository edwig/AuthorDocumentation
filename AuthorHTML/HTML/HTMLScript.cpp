//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    HTMLScript.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Handles the <SCRIPT> tag of the HTML
//
#include "StdAfx.h"
#include "HtmlScript.h"

// Constructor
HtmlScript::HtmlScript(CComPtr<IHTMLElement> i_Elem) 
           :HtmlElement(i_Elem)
{
  mi_script = i_Elem;
}

BOOL 
HtmlScript::Valid()
{
  return (mi_script != NULL);
}

CString
HtmlScript::GetText()
{
  CComBSTR bText;
  mi_script->get_text(&bText);
  CString cText = CW2CT(bText);
  return cText;
}

void
HtmlScript::SetText(CString cText)
{
  USES_CONVERSION;
  CComBSTR bText = CT2CW(cText);
  mi_script->put_text(bText);
}

CString 
HtmlScript::GetType()
{
  CComBSTR bType;
  mi_script->get_type(&bType);
  return CString(CW2CT(bType));
}

void
HtmlScript::SetType(CString type)
{
  CComBSTR bType = CT2CW(type);
  mi_script->put_type(bType);
}
