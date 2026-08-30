//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    HTMLCaption.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Handles the <TABLECAPTION> tag of the HTML
//
#pragma once
#include <atlbase.h>
#include <afxhtml.h>
#include "HTMLElement.h"

class HtmlCaption : public HtmlElement
{
public:
  HtmlCaption(CComPtr<IHTMLElement> i_Elem);

  enum eAction
  {
    E_Align,   // Horizontal text-align
    E_Valign   // Top or bottom of table
  };
  virtual BOOL Valid();
  void    SetProperty(eAction e_Action, CString s_Value);
  CString GetProperty(eAction e_Action);

protected:
  CComQIPtr<IHTMLTableCaption, &IID_IHTMLTableCaption> mi_caption;
};
