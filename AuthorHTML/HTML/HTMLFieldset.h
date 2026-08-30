//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    HTMLFieldSet.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Handles the <FIELDSET> tag of the HTML
//
#pragma once
#include <atlbase.h>
#include <afxhtml.h>
#include "HTMLElement.h"

class HtmlFieldset : public HtmlElement
{
public:
  HtmlFieldset(CComPtr<IHTMLElement> i_Elem);

  enum eAction
  {
     E_Align        // left,center,right alignment
    ,E_Legend       // Legend text
    ,E_LegendAlign  // ALignment of legend text
  };
  virtual BOOL Valid();
  void    SetProperty(eAction e_Action, CString s_Value);
  CString GetProperty(eAction e_Action);

protected:
  // Find the legend of the FIELDSET
  void    FindLegend();
  // If no legend yet, set it with this text
  void    SetLegend(CString text);

  CComQIPtr<IHTMLFieldSetElement, &IID_IHTMLFieldSetElement> mi_Fieldset;
  HtmlElement m_legend;
};

