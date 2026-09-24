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
                    SetAttribute(_T("href"),s_Value);
                    break;
    case E_Coords:  // Set coordiantes of shape
                    SetAttribute(_T("coords"),s_Value);
                    break;
    case E_Shape:   // Set shape of the area
                    SetAttribute(_T("shape"),s_Value);
                    break;
    case E_NoHref:  SetAttribute(_T("nohref"),s_Value,EMPTYREMOVE);
                    break;
    case E_Title:   SetAttribute(_T("title"),s_Value);
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
                  return GetAttribute(_T("href"),EXACT_VALUE);
    case E_Coords:// Coordinates
                  return GetAttribute(_T("coords"));
    case E_Shape: // Shape of the area
                  return GetAttribute(_T("shape"));
    case E_NoHref:// No href 
                  return GetAttribute(_T("nohref"));
    case E_Title: // Title
                  return GetAttribute(_T("title"));
  }
  return _T("");
}

