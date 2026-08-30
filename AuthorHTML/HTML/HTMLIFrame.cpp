//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    HTMLIFrame.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Handles the <IFRAME> tag of the HTML
//
#include "StdAfx.h"
#include "HtmlIFrame.h"

// Constructor
HtmlIFrame::HtmlIFrame(CComPtr<IHTMLElement> i_Elem) : HtmlElement(i_Elem)
{
  mi_IFrame = i_Elem;
}

BOOL 
HtmlIFrame::Valid()
{
  return (mi_IFrame != NULL);
}

void    
HtmlIFrame::SetProperty(eAction e_Action, CString s_Value)
{
  CComVariant v_Value(s_Value);
  switch(e_Action)
  {
    case E_Align:   if(m_Style.Valid())
                    {
                      if((s_Value.CompareNoCase("Left")   == 0) ||
                         (s_Value.CompareNoCase("Center") == 0) ||
                         (s_Value.CompareNoCase("Right")  == 0) )
                      {
                        m_Style.SetProperty(HtmlStyle::P_TextAlign,s_Value);
                      }
                      else
                      {
                        // Top/Middle/bottom
                        m_Style.SetProperty(HtmlStyle::P_VerticalAlign,s_Value);
                      }
                    }
                    SetAttribute("align",s_Value);
                    break;
    case E_Width:   if(m_Style.Valid())
                    {
                      m_Style.SetProperty(HtmlStyle::P_Width,s_Value);
                    }
                    SetAttribute("width",s_Value);
                    break;
    case E_Height:  if(m_Style.Valid())
                    {
                      m_Style.SetProperty(HtmlStyle::P_Height,s_Value);
                    }
                    SetAttribute("height",s_Value);
                    break;
    case E_HSpace:  if(m_Style.Valid())
                    {
                      m_Style.SetProperty(HtmlStyle::P_MarginLeft, s_Value);
                      m_Style.SetProperty(HtmlStyle::P_MarginRight,s_Value);
                    }
                    SetAttribute("hspace",s_Value);
                    break;
    case E_VSpace:  if(m_Style.Valid())
                    {
                      m_Style.SetProperty(HtmlStyle::P_MarginTop,   s_Value);
                      m_Style.SetProperty(HtmlStyle::P_MarginBottom,s_Value);
                    }
                    SetAttribute("vspace",s_Value);
                    break;
    case E_MarginHeight:
                    if(m_Style.Valid())
                    {
                      m_Style.SetProperty(HtmlStyle::P_PaddingTop,   s_Value);
                      m_Style.SetProperty(HtmlStyle::P_PaddingBottom,s_Value);
                    }
                    SetAttribute("marginheight",s_Value);
                    break;
    case E_MarginWidth:
                    if(m_Style.Valid())
                    {
                      m_Style.SetProperty(HtmlStyle::P_PaddingLeft, s_Value);
                      m_Style.SetProperty(HtmlStyle::P_PaddingRight,s_Value);
                    }
                    SetAttribute("marginwidth",s_Value);
                    break;
    case E_FrameBorder:
                    SetAttribute("frameborder",s_Value);
                    break;
    case E_Name:    SetAttribute("name",s_Value);
                    break;
    case E_NoResize:SetAttribute("noresize",s_Value,EMPTYREMOVE);
                    break;
    case E_Scrolling:
                    SetAttribute("scrolling",s_Value);
                    break;
    case E_Src:     SetAttribute("src",s_Value);
                    break;
  }
}

CString 
HtmlIFrame::GetProperty(eAction e_Action)
{
  CString prop;
  switch(e_Action)
  {
    case E_Align:   if(m_Style.Valid())
                    {
                      CString value = m_Style.GetProperty(HtmlStyle::P_VerticalAlign);
                      if(value.IsEmpty())
                      {
                        value = m_Style.GetProperty(HtmlStyle::P_TextAlign);
                        if(!value.IsEmpty())
                        {
                          return value;
                        }
                      }
                    }
                    return GetAttribute("align");
    case E_Width:   if(m_Style.Valid())
                    {
                      prop = m_Style.GetProperty(HtmlStyle::P_Width);
                      if(!prop.IsEmpty())
                      {
                        return prop;
                      }
                    }
                    return GetAttribute("width");
    case E_Height:  if(m_Style.Valid())
                    {
                      prop = m_Style.GetProperty(HtmlStyle::P_Height);
                      if(!prop.IsEmpty())
                      {
                        return prop;
                      }
                    }
                    return GetAttribute("height");
    case E_HSpace:  if(m_Style.Valid())
                    {
                      prop = m_Style.GetProperty(HtmlStyle::P_MarginLeft);
                      if(!prop.IsEmpty())
                      {
                        return prop;
                      }
                      prop = m_Style.GetProperty(HtmlStyle::P_MarginRight);
                      if(!prop.IsEmpty())
                      {
                        return prop;
                      }
                    }
                    return GetAttribute("hspace");
    case E_VSpace:  if(m_Style.Valid())
                    {
                      prop = m_Style.GetProperty(HtmlStyle::P_MarginTop);
                      if(!prop.IsEmpty())
                      {
                        return prop;
                      }
                      prop = m_Style.GetProperty(HtmlStyle::P_MarginBottom);
                      if(!prop.IsEmpty())
                      {
                        return prop;
                      }
                    }
                    return GetAttribute("vspace");
    case E_MarginHeight:
                    if(m_Style.Valid())
                    {
                      prop = m_Style.GetProperty(HtmlStyle::P_PaddingTop);
                      if(!prop.IsEmpty())
                      {
                        return prop;
                      }
                      prop = m_Style.GetProperty(HtmlStyle::P_PaddingBottom);
                      if(!prop.IsEmpty())
                      {
                        return prop;
                      }
                    }
                    return GetAttribute("marginheight");
    case E_MarginWidth:
                    if(m_Style.Valid())
                    {
                      prop = m_Style.GetProperty(HtmlStyle::P_PaddingLeft);
                      if(!prop.IsEmpty())
                      {
                        return prop;
                      }
                      prop = m_Style.GetProperty(HtmlStyle::P_PaddingRight);
                      if(!prop.IsEmpty())
                      {
                        return prop;
                      }
                    }
                    return GetAttribute("marginwidth");
    case E_FrameBorder:
                    return GetAttribute("frameborder");
                    break;
    case E_Name:    return GetAttribute("name");
                    break;
    case E_NoResize:return GetAttribute("noresize");
                    break;
    case E_Scrolling:
                    return GetAttribute("scrolling");
                    break;
    case E_Src:     // Get the source of the image with an exact value scan
                    // Otherwise it will get an ABSOLUTE path for free, which we don't want.
                    // ->get_src and ->get_href alsoo are getting ABSOLUTE path translations
                    return GetAttribute("src",EXACT_VALUE);
                    break;
  }
  return "";
}
