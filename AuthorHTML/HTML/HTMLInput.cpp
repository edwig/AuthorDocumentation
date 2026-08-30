//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    HTMLInput.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Handles the <INPUT> tag of the HTML
//
#include "StdAfx.h"
#include "HtmlInput.h"

// Constructor
HtmlInput::HtmlInput(CComPtr<IHTMLElement> i_Elem) : HtmlElement(i_Elem)
{
  mi_input = i_Elem;
}

BOOL 
HtmlInput::Valid()
{
  return (mi_input != NULL);
}

void    
HtmlInput::SetProperty(eAction e_Action, CString s_Value)
{
  switch(e_Action)
  {
    case E_Type:     // "type"      TYPE OF INPUT FIELD
                     SetAttribute("type",s_Value);
                     break;
    case E_Name:     // "name"      type = button checkbox file hidden image password radio text
                     SetAttribute("name",s_Value);
                     break;
    case E_Value:    // "Value"     type = button checkbox file hidden password radio reset submit text
                     SetAttribute("value",s_Value);
                     break;
    case E_Checked:  // "checked"   type = checkbox radio
                     SetAttribute("checked",s_Value,EMPTYREMOVE);
                     break;
    case E_Accept:   // "accept"    type = file
                     SetAttribute("accept",s_Value);
                     break;
    case E_Maxlength:// "maxlength" type = password text
                     SetAttribute("maxlength",s_Value);
                     break;
    case E_Size:     // "size"      type = password text
                     SetAttribute("size",s_Value);
                     break;
    case E_Src:      // "src"       type = image
                     if(m_Style.Valid())
                     {
                       m_Style.SetProperty(HtmlStyle::P_BackgImage,s_Value);
                     }
                     SetAttribute("src",s_Value);
                     break;
    case E_Align:    // "align"     type = image
                     if(m_Style.Valid())
                     {
                       m_Style.SetProperty(HtmlStyle::P_VerticalAlign,s_Value);
                     }
                     SetAttribute("align",s_Value);
                     break;
  }
}

CString 
HtmlInput::GetProperty(eAction e_Action)
{
  CString prop;
  switch(e_Action)
  {
    case E_Type:     // "type"      TYPE OF INPUT FIELD
                     return GetAttribute("type");
    case E_Name:     // "name"      type = button checkbox file hidden image password radio text
                     return GetAttribute("name");
    case E_Value:    // "Value"     type = button checkbox file hidden password radio reset submit text
                     return GetAttribute("value");
    case E_Checked:  // "checked"   type = checkbox radio
                     return GetAttribute("checked");
    case E_Accept:   // "accept"    type = file
                     return GetAttribute("accept");
    case E_Maxlength:// "maxlength" type = password text
                     return GetAttribute("maxlength");
    case E_Size:     // "size"      type = password text
                     return GetAttribute("size");
    case E_Src:      // "src"       type = image
                     if(m_Style.Valid())
                     {
                       prop = m_Style.GetProperty(HtmlStyle::P_BackgImage);
                       if(!prop.IsEmpty())
                       {
                         return prop;
                       }
                     }
                     return GetAttribute("src",EXACT_VALUE);
    case E_Align:    // "align"     type = image
                     if(m_Style.Valid())
                     {
                       prop = m_Style.GetProperty(HtmlStyle::P_VerticalAlign);
                       if(!prop.IsEmpty())
                       {
                         return prop;
                       }
                     }
                     return GetAttribute("align");
  }
  return "";
}
