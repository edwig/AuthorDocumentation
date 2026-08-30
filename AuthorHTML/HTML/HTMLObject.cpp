//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    HTMLObject.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Handles the <OBJECT> tag of the HTML
//
#include "StdAfx.h"
#include "HtmlObject.h"

// Constructor
HtmlObject::HtmlObject(CComPtr<IHTMLElement> i_Elem) 
           :HtmlElement(i_Elem)
           ,m_paramChanged(false)
{
  mi_object = i_Elem;
  LoadParameters();
}

HtmlObject::~HtmlObject()
{
  if(m_paramChanged)
  {
    SaveParameters();
  }
}

BOOL 
HtmlObject::Valid()
{
  return (mi_object != NULL);
}

void    
HtmlObject::SetProperty(eAction e_Action, CString s_Value)
{
  switch(e_Action)
  {
    case E_Alt:       // Alternative text while loading
                      SetAttribute("alt",s_Value);
                      break;
    case E_Align:     // Alignment of object to surrounding text
                      SetAttribute("align",s_Value);
                      break;
    case E_AltHtml:   // Alternative link if object doesn't want to load
                      SetAttribute("althtml",s_Value);
                      break;
    case E_BaseHref:  // Base URL for page of object (same as BASE)
                      SetAttribute("basehref",s_Value);
                      break;
    case E_Border:    // Reverse relation of link to us 
                      if(m_Style.Valid())
                      {
                        m_Style.SetProperty(HtmlStyle::P_Border,s_Value);
                      }
                      SetAttribute("border",s_Value);
                      break;
    case E_ClassId:   // "clsid:XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX" for registered Microsoft® ActiveX® Controls.
                      SetAttribute("classid",s_Value);
                      break;
    case E_Code:      // JAVA compiled
                      SetAttribute("code",s_Value);
                      break;
    case E_CodeBase:  // BASE of compiled JAVA
                      SetAttribute("codebase",s_Value);
                      break;
    case E_CodeType:  // Type of code (java/vbscript)
                      SetAttribute("codetype",s_Value);
                      break;
    case E_Data:      // Data of the object
                      SetAttribute("data",s_Value);
                      break;
    case E_Height:    // Height of the object
                      if(m_Style.Valid())
                      {
                        m_Style.SetProperty(HtmlStyle::P_Height,s_Value);
                      }
                      SetAttribute("height",s_Value);
                      break;
    case E_HSpace:    // Padding horizontal space
                      if(m_Style.Valid())
                      {
                        m_Style.SetProperty(HtmlStyle::P_PaddingLeft, s_Value);
                        m_Style.SetProperty(HtmlStyle::P_PaddingRight,s_Value);
                      }
                      SetAttribute("hspace",s_Value);
                      break;
    case E_Name:      // Name of the object
                      SetAttribute("name",s_Value);
                      break;
    case E_Standby:   // New standby functionality
                      SetAttribute("standby",s_Value);
                      break;
    case E_Type:      // Mime type of the object
                      SetAttribute("type",s_Value);
                      break;
    case E_UseMap:    // URL of area map to use
                      SetAttribute("usemap",s_Value);
                      break;
    case E_VSpace:    // padding vertical space
                      if(m_Style.Valid())
                      {
                        m_Style.SetProperty(HtmlStyle::P_PaddingBottom,s_Value);
                        m_Style.SetProperty(HtmlStyle::P_PaddingTop,   s_Value);
                      }
                      SetAttribute("vspace",s_Value);
                      break;
    case E_Width:     // Width of the object in the page
                      if(m_Style.Valid())
                      {
                        m_Style.SetProperty(HtmlStyle::P_Width,s_Value);
                      }
                      SetAttribute("width",s_Value);
                      break;

  } 
}

CString 
HtmlObject::GetProperty(eAction e_Action)
{
  CString prop;
  switch(e_Action)
  {
    case E_Alt:       // Alt text
                      return GetAttribute("alt");
    case E_Align:     // Alignment
                      return GetAttribute("align");
    case E_AltHtml:   // Alternative action
                      // Caution: Use EXACT_VALUE, otherwise you get an absolute pathname for free
                      return GetAttribute("althtml",EXACT_VALUE);
    case E_BaseHref:  // BASE URL for the page
                      return GetAttribute("basehref",EXACT_VALUE);
    case E_Border:    // Border around object
                      if(m_Style.Valid())
                      {
                        prop = m_Style.GetProperty(HtmlStyle::P_Border);
                        if(!prop.IsEmpty())
                        {
                          return prop;
                        }
                      }
                      return GetAttribute("border");
    case E_ClassId:   // "clsid:XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX" for registered Microsoft® ActiveX® Controls.
                      return GetAttribute("classid");
    case E_Code:      // Compiled JAVA code
                      return GetAttribute("code",EXACT_VALUE);
    case E_CodeBase:  // BASE of the compiled code
                      return GetAttribute("codebase",EXACT_VALUE);
    case E_CodeType:  // Type of compiled code
                      return GetAttribute("codetype");
    case E_Data:      // Data
                      return GetAttribute("data",EXACT_VALUE);
    case E_Height:    // Height of the object
                      if(m_Style.Valid())
                      {
                        prop = m_Style.GetProperty(HtmlStyle::P_Height);
                        if(!prop.IsEmpty())
                        {
                          return prop;
                        }
                      }
                      return GetAttribute("height");
    case E_HSpace:    // Horizontal padding
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
                      return GetAttribute("hspace");
    case E_Name:      // Name of the object
                      return GetAttribute("name");
    case E_Standby:   // New Standby functionality
                      return GetAttribute("standby",EXACT_VALUE);
    case E_Type:      // MIME type of the object
                      return GetAttribute("type");
    case E_UseMap:    // URL of area map to use
                      return GetAttribute("usemap",EXACT_VALUE);
    case E_VSpace:    // Padding vertical space
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
                      return GetAttribute("vspace");
    case E_Width:     // Width of the object
                      if(m_Style.Valid())
                      {
                        prop = m_Style.GetProperty(HtmlStyle::P_Width);
                        if(!prop.IsEmpty())
                        {
                          return prop;
                        }
                      }
                      return GetAttribute("width");
  }
  return "";
}

// <PARAM> is not exposed by the <OBJECT> tag
// This seems to be a bug in MS-HTML. Posts exist on the internet
// about this bug. Generic get_children or get_all of IHTMLElement
// does also NOT work.
// Parameters **MUST** go trough the DomNode interface
//
CString
HtmlObject::GetParameter(CString p_name)
{
  CString value;
  if(Valid())
  {
    ParameterMap::iterator it = m_parameters.find(p_name);
    if(it != m_parameters.end())
    {
      value = it->second;
    }
  }
  return value;
}

// NEW parameters cannot even go through the DOM interface.
// The whole <OBJECT> must be re-evaluated by the set_outerHTML interface
// IE wants to re-examine the object and all of its attributes and parameters!!
// Sigh: Checked all other interfaces took me more than 6 hours!!
bool
HtmlObject::SetParameter(CString p_name,CString p_value)
{
  bool result = false;
  if(Valid())
  {
//     HtmlElement elem = FirstChild();
//     while(elem.Valid())
//     {
//       CString name = elem.GetAttribute("name",EXACT_VALUE);
//       if(name.CompareNoCase(p_name) == 0)
//       {
//         result = elem.SetAttribute("value",p_value) > 0;
//         break;
//       }
//       // Next parameter
//       elem = elem.NextSibling();
//     }
//     if(result == false)
//     {
      // 1) Not directly found. 
      // 2) SetAttribute returned false
      // 3) FirstChild/NextSibling didn't work
      // Put in parameters map for later saving to <PARAM>
      ParameterMap::iterator it = m_parameters.find(p_name);
      if(it == m_parameters.end())
      {
        // Put new parameter in map
        m_parameters.insert(std::make_pair(p_name,p_value));
        m_paramChanged = true;
      }
      else
      {
        if(p_value.Compare(it->second) != 0)
        {
          // Found and value changed also
          it->second = p_value;
          m_paramChanged = true;
        }
      }
//  }
  }
  return result;
}

// Removing a parameter is different from removing an attribute
// setting a parameter to "" does not remove it. This is an extra
// method to explicitly remove the parameter.
bool
HtmlObject::RemoveParameter(CString p_name)
{
  bool result = false;
  if(Valid())
  {
    ParameterMap::iterator it = m_parameters.find(p_name);
    if(it != m_parameters.end())
    {
      m_parameters.erase(it);
      result = true;
    }
  }
  return result;
}

// <PARAM> is not exposed by the <OBJECT> tag
// This seems to be a bug in MSHTML. Posts exist on the internet
// about this bug. Generic get_children or get_all of IHTMLElement
// does also NOT work.
// Parameters **MUST** go trough the DomNode interface
//
// Parameters are cached here until we save everything at once
// at a re-evaluation of the <OBJECT>. 
// See comments at SaveParameters
void
HtmlObject::LoadParameters()
{
  if(Valid())
  {
    HtmlElement elem = FirstChild();
    while(elem.Valid())
    {
      CString name  = elem.GetAttribute("name", EXACT_VALUE);
      CString value = elem.GetAttribute("value",EXACT_VALUE);
      m_parameters.insert(std::make_pair(name,value));
      // Next parameter
      elem = elem.NextSibling();
    }
  }
}

// NEW parameters cannot even go through the DOM interface.
// The whole <OBJECT> must be re-evaluated by the set_outerHTML interface
// (even set_innerHTML() doesnn't work in IHTMLElement)
// IE wants to re-examine the object and all of its attributes and parameters!!
// Sigh: Checked all other interfaces took me more than 6 hours!!
void
HtmlObject::SaveParameters()
{
  if(m_parameters.size() == 0 || m_paramChanged == false)
  {
    // Nothing to do, or nothing changed
    return;
  }
  CComBSTR bOuter;
  CComQIPtr<IHTMLElement,&IID_IHTMLElement> elem = mi_object;
  HRESULT hr = elem->get_outerHTML(&bOuter);
  if(SUCCEEDED(hr))
  {
    CString outer = CW2CT(bOuter);
    int pos = outer.Find('>'); // find end of "<object attribs....;>"
    if(pos > 0)
    {
      // Get object part
      outer = outer.Left(pos+1);

      // Do all the parameters
      ParameterMap::iterator it = m_parameters.begin();
      while(it != m_parameters.end())
      {
        CString param;
        param.Format("\n<param name=\"%s\" value=\"%s\">",it->first.GetString(),it->second.GetString());
        outer += param;
        // next parameter
        ++it;
      }
      outer += "\n</object>\n";
      
      // Put object back and re-evaluate
      // By caching everything in m_parameters this does now occur just
      // once and not for the setting of every parameter.
      CComBSTR newOuter(outer);
      hr = elem->put_outerHTML(newOuter);
      if(SUCCEEDED(hr))
      {
        m_paramChanged = false;
      }
    }
  }
}