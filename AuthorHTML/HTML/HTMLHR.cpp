// HTMLHR.cpp
//
// Constructor
#include "StdAfx.h"
#include "HTMLHR.h"
#include "HTMLStyle.h"

HtmlHR::HtmlHR(CComPtr<IHTMLElement> i_Elem) 
       :HtmlElement(i_Elem)
{
	mi_HR = i_Elem;
}

BOOL 
HtmlHR::Valid()
{
	return (mi_HR != NULL);
}

void 
HtmlHR::SetProperty(eAction e_Action, CString s_Value)
{
	CComVariant v_Value(s_Value);
	switch (e_Action)
	{
		case E_Color: if(m_Style.Valid())
                  {
                    m_Style.SetProperty(HtmlStyle::P_Color,s_Value);
                  }
                  SetAttribute("color",s_Value);
                  break;
		case E_Size:  if(m_Style.Valid())
                  {
                    m_Style.SetProperty(HtmlStyle::P_Height,s_Value);
                  }
                  SetAttribute("size",s_Value);
                  break;
		case E_Width: if(m_Style.Valid())
                  {
                    m_Style.SetProperty(HtmlStyle::P_Width,s_Value);
                  }
                  SetAttribute("width",s_Value);
                  break;
    case E_Align: if(m_Style.Valid())
                  {
                    m_Style.SetProperty(HtmlStyle::P_TextAlign,s_Value);
                  }
                  SetAttribute("align",s_Value);
                  break;
	}
}

CString 
HtmlHR::GetProperty(eAction e_Action)
{
  CString prop;
	switch (e_Action)
	{
    case E_Color: if(m_Style.Valid())
                  {
                    prop = m_Style.GetProperty(HtmlStyle::P_Color);
                    if(!prop.IsEmpty())
                    {
                      return prop;
                    }
                  }
                  return GetAttribute("color");
		case E_Size:  if(m_Style.Valid())
                  {
                    prop = m_Style.GetProperty(HtmlStyle::P_Height);
                    if(!prop.IsEmpty())
                    {
                      return prop;
                    }
                  }
                  return GetAttribute("size");
		case E_Width: if(m_Style.Valid())
                  {
                    prop = m_Style.GetProperty(HtmlStyle::P_Width);
                    if(!prop.IsEmpty())
                    {
                      return prop;
                    }
                  }
                  return GetAttribute("width");
    case E_Align: if(m_Style.Valid())
                  {
                    prop = m_Style.GetProperty(HtmlStyle::P_TextAlign);
                    if(!prop.IsEmpty())
                    {
                      return prop;
                    }
                  }
                  return GetAttribute("align");
	}
  return "";
}

// return TRUE if the <HR NOSHADE> property is not set
BOOL 
HtmlHR::GetShade()
{
	VARIANT_BOOL v_NoShade(false);
	mi_HR->get_noShade(&v_NoShade);
	return (v_NoShade == FALSE);
}

// turn on / off the <HR NOSHADE> property (default = off)
void 
HtmlHR::SetShade(BOOL b_Shade)
{
	VARIANT_BOOL v_NoShade(b_Shade == FALSE);
	mi_HR->put_noShade(v_NoShade);
}

