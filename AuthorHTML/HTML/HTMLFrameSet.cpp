// HtmlFrameSet.cpp
//
#include "StdAfx.h"
#include "HtmlFrameSet.h"

// Constructor
HtmlFrameSet::HtmlFrameSet(CComPtr<IHTMLElement> i_Elem) 
             :HtmlElement(i_Elem)
{
  mi_frameset = i_Elem;
}

BOOL 
HtmlFrameSet::Valid()
{
  return (mi_frameset != NULL);
}

void    
HtmlFrameSet::SetProperty(eAction e_Action, CString s_Value)
{
  switch(e_Action)
  {
    case E_Border:      // number of border pixels
                        SetAttribute(_T("border"),s_Value);
                        break;
    case E_BorderColor: // Color of the border
                        SetAttribute(_T("bordercolor"),s_Value);
                        break;
    case E_FrameBorder: // number of border pixels
                        SetAttribute(_T("frameborder"),s_Value);
                        break;
    case E_FrameSpacing:// Spacing within frames
                        SetAttribute(_T("framespacing"),s_Value);
                        break;
  }
}

CString 
HtmlFrameSet::GetProperty(eAction e_Action)
{
  CString prop;
  switch(e_Action)
  {
    case E_Border:        // Number of border pixels
                          return GetAttribute(_T("border"));
    case E_BorderColor:   // Color of the border
                          return GetAttribute(_T("bordercolor"));
    case E_FrameBorder:   // Number of border pixels
                          return GetAttribute(_T("frameborder"));
    case E_FrameSpacing:  // spacing within frames
                          return GetAttribute(_T("framespacing"));
  }
  return CString(_T(""));
}

void    
HtmlFrameSet::SetCols(CString& p_cols)
{
  CComBSTR cols(p_cols);
  mi_frameset->put_cols(cols);
}

void    
HtmlFrameSet::SetRows(CString& p_rows)
{
  CComBSTR rows(p_rows);
  mi_frameset->put_rows(rows);
}

CString 
HtmlFrameSet::GetCols()
{
  CComBSTR cols;
  mi_frameset->get_cols(&cols);
  CString theCols = CW2CT(cols);
  return theCols;
}

CString 
HtmlFrameSet::GetRows()
{
  CComBSTR rows;
  mi_frameset->get_rows(&rows);
  CString theRows = CW2CT(rows);
  return theRows;
}

