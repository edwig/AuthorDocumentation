//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    HTMLArea.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Handles the <AREA> tag of the HTML
//
#include "StdAfx.h"
#include "HtmlArea.h"

// Constructor
HtmlArea::HtmlArea(CComPtr<IHTMLElement> i_Elem) 
         :HtmlElement(i_Elem)
{
  mi_area = i_Elem;
}

BOOL 
HtmlArea::Valid()
{
  return (mi_area != NULL);
}

void    
HtmlArea::SetProperty(eAction e_Action, CString s_Value)
{
  switch(e_Action)
  {
    case E_Href:    // Referenced page
                    SetAttribute("href",s_Value);
                    break;
    case E_Coords:  // Set coordiantes of shape
                    SetAttribute("coords",s_Value);
                    break;
    case E_Shape:   // Set shape of the area
                    SetAttribute("shape",s_Value);
                    break;
    case E_NoHref:  SetAttribute("nohref",s_Value,EMPTYREMOVE);
                    break;
    case E_Title:   SetAttribute("title",s_Value);
                    break;
  }
}

CString 
HtmlArea::GetProperty(eAction e_Action)
{
  switch(e_Action)
  {
    case E_Href:  // Referenced page
                  // Caution: Use EXACT_VALUE, otherwise you get an absolute pathname for free
                  return GetAttribute("href",EXACT_VALUE);
    case E_Coords:// Coordinates
                  return GetAttribute("coords");
    case E_Shape: // Shape of the area
                  return GetAttribute("shape");
    case E_NoHref:// No href 
                  return GetAttribute("nohref");
    case E_Title: // Title
                  return GetAttribute("title");
  }
  return "";
}

