//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    HTMLHeading.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Handles the <H> tag of the HTML
//
#pragma once
#include <atlbase.h>
#include <afxhtml.h>
#include "HTMLElement.h"

class HtmlHeading : public HtmlElement
{
public:
  HtmlHeading(CComPtr<IHTMLElement> i_Elem);

  enum eAction
  {
    E_Align,   // Horizontal text-align
  };
  virtual BOOL Valid();
  void    SetProperty(eAction e_Action, CString s_Value);
  CString GetProperty(eAction e_Action);

protected:
  CComQIPtr<IHTMLHeaderElement, &IID_IHTMLHeaderElement> mi_header;
};
