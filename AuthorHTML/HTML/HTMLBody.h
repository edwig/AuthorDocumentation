//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    HTMLBody.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Handles the <BODY> tag of the HTML
//
#pragma once
#include <atlbase.h>
#include <afxhtml.h>
#include "HTMLElement.h"

class HtmlBody : public HtmlElement
{
public:
  HtmlBody(CComPtr<IHTMLElement> i_Elem);

  enum eAction
  {
    E_Link,         // Color of the links
    E_Alink,        // Color of the active link
    E_Vlink,        // Color of the visited links
    E_Background,   // Background picture
    E_BgColor,      // Color of the background
    E_BgProperties, // Scrolling of the background image
    E_BottomMargin, // Margin at the bottom in pixels
    E_LeftMargin,   // Margin at the leftside
    E_RightMargin,  // Margin at the rightside
    E_Scroll,       // Turn scrollbars on/off
    E_TextColor,    // Color of the text on the page
    E_Title,        // Advisory title of the body
    E_TopMargin,    // Margin at the top
    E_NoWrap        // Automatically wordwrap (default = no)
  };
  virtual BOOL Valid();
  void    SetProperty(eAction e_Action, CString s_Value);
  CString GetProperty(eAction e_Action);
  bool    GetNoWrap();
  void    SetNoWrap(bool nowrap);

protected:
  CComQIPtr<IHTMLBodyElement, &IID_IHTMLBodyElement> mi_body;
};
