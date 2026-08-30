//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    HTMLCaption.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Handles the <TABLECAPTION> tag of the HTML
//
#include "StdAfx.h"
#include "HtmlCaption.h"

// Constructor
HtmlCaption::HtmlCaption(CComPtr<IHTMLElement> i_Elem) : HtmlElement(i_Elem)
{
  mi_caption = i_Elem;
}

BOOL 
HtmlCaption::Valid()
{
  return (mi_caption != NULL);
}

void    
HtmlCaption::SetProperty(eAction e_Action, CString s_Value)
{
  switch(e_Action)
  {
    case E_Align:   if(m_Style.Valid())
                    {
                      m_Style.SetProperty(HtmlStyle::P_TextAlign,s_Value);
                    }
                    SetAttribute("align",s_Value);
                    break;
    case E_Valign:  if(m_Style.Valid())
                    {
                      m_Style.SetProperty(HtmlStyle::P_VerticalAlign,s_Value);
                    }
                    SetAttribute("valign",s_Value);
                    break;
  }
}

CString 
HtmlCaption::GetProperty(eAction e_Action)
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
    case E_Valign:  if(m_Style.Valid())
                    {
                      CString prop = m_Style.GetProperty(HtmlStyle::P_VerticalAlign);
                      if(!prop.IsEmpty())
                      {
                        return prop;
                      }
                    }
                    return GetAttribute("valign");
  }
  return "";
}
