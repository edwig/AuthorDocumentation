//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    HTMLArea.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Handles the <AREA> tag of the HTML
//
#pragma once
#include <atlbase.h>
#include <afxhtml.h>
#include "HTMLElement.h"

class HtmlArea : public HtmlElement
{
public:
  HtmlArea(CComPtr<IHTMLElement> i_Elem);

  enum eAction
  {
    E_Href,    // Referenced page
    E_Coords,  // Coordinates of the aarea
    E_Shape,   // Shape of the area
    E_NoHref,  // Special mouse shape
    E_Title
  };
  virtual BOOL Valid();
  void    SetProperty(eAction e_Action, CString s_Value);
  CString GetProperty(eAction e_Action);

protected:
  CComQIPtr<IHTMLAreaElement, &IID_IHTMLAreaElement> mi_area;
};
