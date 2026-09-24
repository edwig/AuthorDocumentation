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
                     SetAttribute(_T("type"),s_Value);
                     break;
    case E_Name:     // "name"      type = button checkbox file hidden image password radio text
                     SetAttribute(_T("name"),s_Value);
                     break;
    case E_Value:    // "Value"     type = button checkbox file hidden password radio reset submit text
                     SetAttribute(_T("value"),s_Value);
                     break;
    case E_Checked:  // "checked"   type = checkbox radio
                     SetAttribute(_T("checked"),s_Value,EMPTYREMOVE);
                     break;
    case E_Accept:   // "accept"    type = file
                     SetAttribute(_T("accept"),s_Value);
                     break;
    case E_Maxlength:// "maxlength" type = password text
                     SetAttribute(_T("maxlength"),s_Value);
                     break;
    case E_Size:     // "size"      type = password text
                     SetAttribute(_T("size"),s_Value);
                     break;
    case E_Src:      // "src"       type = image
                     if(m_Style.Valid())
                     {
                       m_Style.SetProperty(HtmlStyle::P_BackgImage,s_Value);
                     }
                     SetAttribute(_T("src"),s_Value);
                     break;
    case E_Align:    // "align"     type = image
                     if(m_Style.Valid())
                     {
                       m_Style.SetProperty(HtmlStyle::P_VerticalAlign,s_Value);
                     }
                     SetAttribute(_T("align"),s_Value);
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
                     return GetAttribute(_T("type"));
    case E_Name:     // "name"      type = button checkbox file hidden image password radio text
                     return GetAttribute(_T("name"));
    case E_Value:    // "Value"     type = button checkbox file hidden password radio reset submit text
                     return GetAttribute(_T("value"));
    case E_Checked:  // "checked"   type = checkbox radio
                     return GetAttribute(_T("checked"));
    case E_Accept:   // "accept"    type = file
                     return GetAttribute(_T("accept"));
    case E_Maxlength:// "maxlength" type = password text
                     return GetAttribute(_T("maxlength"));
    case E_Size:     // "size"      type = password text
                     return GetAttribute(_T("size"));
    case E_Src:      // "src"       type = image
                     if(m_Style.Valid())
                     {
                       prop = m_Style.GetProperty(HtmlStyle::P_BackgImage);
                       if(!prop.IsEmpty())
                       {
                         return prop;
                       }
                     }
                     return GetAttribute(_T("src"),EXACT_VALUE);
    case E_Align:    // "align"     type = image
                     if(m_Style.Valid())
                     {
                       prop = m_Style.GetProperty(HtmlStyle::P_VerticalAlign);
                       if(!prop.IsEmpty())
                       {
                         return prop;
                       }
                     }
                     return GetAttribute(_T("align"));
  }
  return _T("");
}
