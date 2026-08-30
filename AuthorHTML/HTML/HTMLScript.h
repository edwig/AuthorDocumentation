//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    HTMLScript.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Handles the <SCRIPT> tag of the HTML
//
#pragma once
#include <atlbase.h>
#include <afxhtml.h>
#include "HTMLElement.h"

class HtmlScript : public HtmlElement
{
public:
  HtmlScript(CComPtr<IHTMLElement> i_Elem);
  virtual BOOL Valid();
  CString GetText();
  void    SetText(CString cText);
  CString GetType();
  void    SetType(CString type);

protected:
  CComQIPtr<IHTMLScriptElement, &IID_IHTMLScriptElement> mi_script;
};
