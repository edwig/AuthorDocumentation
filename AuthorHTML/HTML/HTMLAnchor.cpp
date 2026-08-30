//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    HTMLAnchor.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Handles the <A> tag of the HTML
//
#include "StdAfx.h"
#include "HtmlAnchor.h"

// Constructor
HtmlAnchor::HtmlAnchor(CComPtr<IHTMLElement> i_Elem) : HtmlElement(i_Elem)
{
  mi_anchor = i_Elem;
}

BOOL 
HtmlAnchor::Valid()
{
  return (mi_anchor != NULL);
}

void    
HtmlAnchor::SetProperty(eAction e_Action, CString s_Value)
{
  switch(e_Action)
  {
    case E_Href:    // Referenced page
                    SetAttribute("href",s_Value);
                    break;
    case E_Target:  // Target frame
                    SetAttribute("target",s_Value);
                    break;
    case E_Title:   // Title of the link (tooltip text)
                    SetAttribute("title",s_Value);
                    break;
    case E_Rel:     // Relation to the link
                    SetAttribute("rel",s_Value);
                    break;
    case E_Rev:     // Reverse relation of link to us 
                    SetAttribute("rev",s_Value);
                    break;
  }
}

CString 
HtmlAnchor::GetProperty(eAction e_Action)
{
  switch(e_Action)
  {
    case E_Href:  // Referenced page
                  // Caution: Use EXACT_VALUE, otherwise you get an absolute pathname for free
                  return GetAttribute("href",EXACT_VALUE);
    case E_Target:// Target frame
                  return GetAttribute("target");
    case E_Title: // Title of the link (tooltip text)
                  return GetAttribute("title");
    case E_Rel:   // Relation to the link
                  return GetAttribute("rel");
    case E_Rev:   // Reverse relation of link to us 
                  return GetAttribute("rev");
  }
  return "";
}
