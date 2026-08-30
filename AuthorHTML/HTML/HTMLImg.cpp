//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    HTMLImage.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Handles the <IMG> tag of the HTML
//
#include "StdAfx.h"
#include "HTMLImg.h"

// Constructor
HtmlImg::HtmlImg(CComPtr<IHTMLElement> i_Elem) : HtmlElement(i_Elem)
{
	mi_Img = i_Elem;
}

BOOL 
HtmlImg::Valid()
{
	return (mi_Img != NULL);
}

void    
HtmlImg::SetProperty(eAction e_Action, CString s_Value)
{
  CComVariant v_Value(s_Value);
  switch(e_Action)
  {
    case E_Border:  if(m_Style.Valid())
                    {
                      m_Style.SetProperty(HtmlStyle::P_Border,s_Value);
                    }
                    SetAttribute("border",s_Value);
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
    case E_HorPad:  if(m_Style.Valid())
                    {
                      m_Style.SetProperty(HtmlStyle::P_PaddingLeft, s_Value);
                      m_Style.SetProperty(HtmlStyle::P_PaddingRight,s_Value);
                    }
                    SetAttribute("hspace",s_Value);
                    break;
    case E_VerPad:  if(m_Style.Valid())
                    {
                      m_Style.SetProperty(HtmlStyle::P_PaddingTop,   s_Value);
                      m_Style.SetProperty(HtmlStyle::P_PaddingBottom,s_Value);
                    }
                    SetAttribute("vspace",s_Value);
                    break;
    case E_Align:   if(m_Style.Valid())
                    {
                      m_Style.SetProperty(HtmlStyle::P_VerticalAlign,s_Value);
                    }
                    SetAttribute("align",s_Value);
                    break;
  }
}

CString 
HtmlImg::GetProperty(eAction e_Action)
{
  CString prop;
  switch(e_Action)
  {
    case E_Border:  if(m_Style.Valid())
                    {
                      prop = m_Style.GetProperty(HtmlStyle::P_Border);
                      if(!prop.IsEmpty())
                      {
                        return prop;
                      }
                    }
                    return GetAttribute("border");
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
    case E_HorPad:  if(m_Style.Valid())
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
                    return GetAttribute("hspace");
    case E_VerPad:  if(m_Style.Valid())
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
                    return GetAttribute("vspace");
    case E_Align:   if(m_Style.Valid())
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

CString 
HtmlImg::GetSrc()
{
  // Get the source of the image with an exact value scan
  // Otherwise it will get an ABSOLUTE path for free, which we don't want.
  // ->get_src and ->get_href alsoo are getting ABSOLUTE path translations
  return GetAttribute("src",EXACT_VALUE);
}

CString 
HtmlImg::GetDynSrc()
{
  // Get the source of the image with an exact value scan
  // Otherwise it will get an ABSOLUTE path for free, which we don't want.
  // ->get_src and ->get_href alsoo are getting ABSOLUTE path translations
  return GetAttribute("dynsrc",EXACT_VALUE);
}

CString 
HtmlImg::GetLowSrc()
{
  // Get the source of the image with an exact value scan
  // Otherwise it will get an ABSOLUTE path for free, which we don't want.
  // ->get_src and ->get_href alsoo are getting ABSOLUTE path translations
  return GetAttribute("lowsrc",EXACT_VALUE);
}

BOOL 
HtmlImg::SetSrc(CString s_Src)
{
  // Set the source of an image with a set-attribute and not with the
  // ->set_src or ->set_href methods. These latter will do ABSOLUTE path translations
  // which we cannot use!!!
  return SetAttribute("src",s_Src);
}

BOOL 
HtmlImg::SetLowSrc(CString s_Src)
{
  // Set the source of an image with a set-attribute and not with the
  // ->set_src or ->set_href methods. These latter will do ABSOLUTE path translations
  // which we cannot use!!!
  return SetAttribute("lowsrc",s_Src);
}

BOOL 
HtmlImg::SetDynSrc(CString s_Src)
{
  // Set the source of an image with a set-attribute and not with the
  // ->set_src or ->set_href methods. These latter will do ABSOLUTE path translations
  // which we cannot use!!!
  return SetAttribute("dynsrc",s_Src);
}

CString 
HtmlImg::GetAlt()
{
  return GetAttribute("alt");
}

BOOL    
HtmlImg::SetAlt(CString s_ALt)
{
  return SetAttribute("alt",s_ALt);
}

CString 
HtmlImg::GetStart()
{
  return GetAttribute("start");
}

BOOL
HtmlImg::SetStart(CString sStart)
{
  return SetAttribute("start",sStart);
}

CString 
HtmlImg::GetLoop()
{
  return GetAttribute("loop");
}

BOOL    
HtmlImg::SetLoop(CString sLoop)
{
  return SetAttribute("loop",sLoop);
}
