//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    HTMLAnchor.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Handles the <A> tag of the HTML
//
#pragma once
#include <atlbase.h>
#include <afxhtml.h>
#include "HTMLElement.h"

class HtmlAnchor : public HtmlElement
{
public:
  HtmlAnchor(CComPtr<IHTMLElement> i_Elem);

  enum eAction
  {
    E_Href,    // Referenced page
    E_Target,  // Target frame
    E_Title,   // Title of the link (tooltip text)
    E_Rel,     // Relation to the link
    E_Rev,     // Reverse relation of link to us 
  };
  virtual BOOL Valid();
  void    SetProperty(eAction e_Action, CString s_Value);
  CString GetProperty(eAction e_Action);

protected:
  CComQIPtr<IHTMLAnchorElement, &IID_IHTMLAnchorElement> mi_anchor;
};
