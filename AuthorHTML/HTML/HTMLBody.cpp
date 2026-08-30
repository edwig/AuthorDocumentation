// HtmlBody.cpp
//
#include "StdAfx.h"
#include "HtmlBody.h"

// Constructor
HtmlBody::HtmlBody(CComPtr<IHTMLElement> i_Elem) 
         :HtmlElement(i_Elem)
{
  mi_body = i_Elem;
}

BOOL 
HtmlBody::Valid()
{
  return (mi_body != NULL);
}

void    
HtmlBody::SetProperty(eAction e_Action, CString s_Value)
{
  switch(e_Action)
  {
    case E_Link:         // Color of the links
                         SetAttribute("link",s_Value);
                         break;
    case E_Alink:        // Color of the active link
                         SetAttribute("alink",s_Value);
                         break;
    case E_Vlink:        // Color of the visited links
                         SetAttribute("vlink",s_Value);
                         break;
    case E_Background:   // Background picture
                         if(m_Style.Valid())
                         {
                           m_Style.SetProperty(HtmlStyle::P_BackgImage,s_Value);
                         }
                         SetAttribute("background",s_Value);
                         break;
    case E_BgColor:      // Color of the background
                         if(m_Style.Valid())
                         {
                           m_Style.SetProperty(HtmlStyle::P_BackgColor,s_Value);
                         }
                         SetAttribute("bgcolor",s_Value);
                         break;
    case E_BgProperties: // Scrolling of the background image
                         if(m_Style.Valid())
                         {
                           m_Style.SetProperty(HtmlStyle::P_BackAttach,s_Value);
                         }
                         SetAttribute("bgproperties",s_Value);
                         break;
    case E_BottomMargin: // Margin at the bottom in pixels
                         if(m_Style.Valid())
                         {
                           m_Style.SetProperty(HtmlStyle::P_MarginBottom,s_Value);
                         }
                         SetAttribute("bottommargin",s_Value);
                         break;
    case E_LeftMargin:   // Margin at the leftside
                         if(m_Style.Valid())
                         {
                           m_Style.SetProperty(HtmlStyle::P_MarginLeft,s_Value);
                         }
                         SetAttribute("leftmargin",s_Value);
                         break;
    case E_RightMargin:  // Margin at the rightside
                         if(m_Style.Valid())
                         {
                           m_Style.SetProperty(HtmlStyle::P_MarginRight,s_Value);
                         }
                         SetAttribute("rightmargin",s_Value);
                         break;
    case E_Scroll:       // Turn scrollbars on/off
                         SetAttribute("scroll",s_Value);
                         break;
    case E_TextColor:    // Color of the text on the page
                         if(m_Style.Valid())
                         {
                           m_Style.SetProperty(HtmlStyle::P_Color,s_Value);
                         }
                         SetAttribute("text",s_Value);
                         break;
    case E_Title:        // Advisory title of the body
                         SetAttribute("title",s_Value);
                         break;
    case E_TopMargin:    // Margin at the top
                         if(m_Style.Valid())
                         {
                           m_Style.SetProperty(HtmlStyle::P_MarginTop,s_Value);
                         }
                         SetAttribute("topmargin",s_Value);
                         break;
  }
}

CString 
HtmlBody::GetProperty(eAction e_Action)
{
  CString prop;
  switch(e_Action)
  {
    case E_Link:         // Color of the links
                         return GetAttribute("link");
    case E_Alink:        // Color of the active link
                         return GetAttribute("alink");
    case E_Vlink:        // Color of the visited links
                         return GetAttribute("vlink");
                         break;
    case E_Background:   // Background picture
                         if(m_Style.Valid())
                         {
                           prop = m_Style.GetProperty(HtmlStyle::P_BackgImage);
                           if(!prop.IsEmpty())
                           {
                             return prop;
                           }
                         }
                         return GetAttribute("background",EXACT_VALUE);
    case E_BgColor:      // Color of the background
                         if(m_Style.Valid())
                         {
                           prop = m_Style.GetProperty(HtmlStyle::P_BackgColor);
                           if(!prop.IsEmpty())
                           {
                             return prop;
                           }
                         }
                         return GetAttribute("bgcolor");
    case E_BgProperties: // Scrolling of the background image
                         if(m_Style.Valid())
                         {
                           prop = m_Style.GetProperty(HtmlStyle::P_BackAttach);
                           if(!prop.IsEmpty())
                           {
                             return prop;
                           }
                         }
                         return GetAttribute("bgproperties");
    case E_BottomMargin: // Margin at the bottom in pixels
                         if(m_Style.Valid())
                         {
                           prop = m_Style.GetProperty(HtmlStyle::P_MarginBottom);
                           if(!prop.IsEmpty())
                           {
                             return prop;
                           }
                         }
                         return GetAttribute("bottommargin");
    case E_LeftMargin:   // Margin at the leftside
                         if(m_Style.Valid())
                         {
                           prop = m_Style.GetProperty(HtmlStyle::P_MarginLeft);
                           if(!prop.IsEmpty())
                           {
                             return prop;
                           }
                         }
                         return GetAttribute("leftmargin");
    case E_RightMargin:  // Margin at the rightside
                         if(m_Style.Valid())
                         {
                           prop = m_Style.GetProperty(HtmlStyle::P_MarginRight);
                           if(!prop.IsEmpty())
                           {
                             return prop;
                           }
                         }
                         return GetAttribute("rightmargin");
    case E_Scroll:       // Turn scrollbars on/off
                         return GetAttribute("scroll");
                         break;
    case E_TextColor:    // Color of the text on the page
                         if(m_Style.Valid())
                         {
                           prop = m_Style.GetProperty(HtmlStyle::P_Color);
                           if(!prop.IsEmpty())
                           {
                             return prop;
                           }
                         }
                         return GetAttribute("text");
    case E_Title:        // Advisory title of the body
                         return GetAttribute("title");
                         break;
    case E_TopMargin:    // Margin at the top
                         if(m_Style.Valid())
                         {
                           prop = m_Style.GetProperty(HtmlStyle::P_MarginTop);
                           if(!prop.IsEmpty())
                           {
                             return prop;
                           }
                         }
                         return GetAttribute("topmargin");
  }
  return "";
}

bool
HtmlBody::GetNoWrap()
{
  VARIANT_BOOL wrap;
  mi_body->get_noWrap(&wrap);
  return (wrap == VARIANT_TRUE);
}

void
HtmlBody::SetNoWrap(bool nowrap)
{
  VARIANT_BOOL wrap = nowrap ? VARIANT_TRUE : VARIANT_FALSE;
  mi_body->put_noWrap(wrap);
}

