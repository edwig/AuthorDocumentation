//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    HTMLHeading.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Handles the <H> tag of the HTML
//
#include "StdAfx.h"
#include "HtmlHeading.h"

// Constructor
HtmlHeading::HtmlHeading(CComPtr<IHTMLElement> i_Elem) : HtmlElement(i_Elem)
{
  mi_header = i_Elem;
}

BOOL 
HtmlHeading::Valid()
{
  return (mi_header != NULL);
}

void    
HtmlHeading::SetProperty(eAction e_Action, CString s_Value)
{
  switch(e_Action)
  {
    case E_Align:   if(m_Style.Valid())
                    {
                      m_Style.SetProperty(HtmlStyle::P_TextAlign,s_Value);
                    }
                    SetAttribute("align",s_Value);
                    break;
  }
}

CString 
HtmlHeading::GetProperty(eAction e_Action)
{
  switch(e_Action)
  {
    case E_Align:   if(m_Style.Valid())
                    {
                      CString prop = m_Style.GetProperty(HtmlStyle::P_TextAlign);
                      if(!prop.IsEmpty())
                      {
                        return prop;
                      }
                    }
                    return GetAttribute("align");
  }
  return "";
}
