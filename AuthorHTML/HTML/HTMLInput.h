//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    HTMLInput.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Handles the <INPUT> tag of the HTML
//
#pragma once
#include <atlbase.h>
#include <afxhtml.h>
#include "HTMLElement.h"

class HtmlInput : public HtmlElement
{
public:
  HtmlInput(CComPtr<IHTMLElement> i_Elem);

  enum eAction
  {
    E_Type,     // "type"      TYPE OF INPUT FIELD
    E_Name,     // "name"      type = button checkbox file hidden image password radio              text
    E_Value,    // "Value"     type = button checkbox file hidden       password radio reset submit text
    E_Checked,  // "checked"   type =        checkbox                            radio
    E_Accept,   // "accept"    type =                 file
    E_Maxlength,// "maxlength" type =                                   password                    text
    E_Size,     // "size"      type =                                   password                    text
    E_Src,      // "src"       type =                             image
    E_Align     // "align"     type =                             image
  };
  virtual BOOL Valid();
  void    SetProperty(eAction e_Action, CString s_Value);
  CString GetProperty(eAction e_Action);

protected:
  CComQIPtr<IHTMLInputElement, &IID_IHTMLInputElement> mi_input;
};
