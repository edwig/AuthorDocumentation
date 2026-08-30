//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    HTMLObject.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Handles the <OBJECT> tag of the HTML
//
#pragma once
#include <atlbase.h>
#include <afxhtml.h>
#include "HTMLElement.h"
#include <map>

#define CLSID_ALINK         "clsid:1e2a7bd0-dab9-11d0-b93a-00c04fc99f9e"
#define CLSID_ALINK_COMMAND "clsid:adb880a6-d8ff-11cf-9377-00aa003b7a11"

typedef std::map<CString,CString> ParameterMap;

class HtmlObject : public HtmlElement
{
public:
  HtmlObject(CComPtr<IHTMLElement> i_Elem);
 ~HtmlObject();

  enum eAction
  {
    E_Alt       // Alternative text while loading (standby?)
   ,E_Align
   ,E_AltHtml
   ,E_BaseHref
   ,E_Border
   ,E_ClassId   // "clsid:XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX" for registered Microsoft® ActiveX® Controls.
   ,E_Code      // For compiled JAVA
   ,E_CodeBase  // Base of compiled JAVA
   ,E_CodeType
   ,E_Data
   ,E_Height
   ,E_HSpace
   ,E_Name
   ,E_Standby   // Standby functionality
   ,E_Type
   ,E_UseMap
   ,E_VSpace
   ,E_Width
  };
  virtual BOOL Valid();

  void    SetProperty(eAction e_Action, CString s_Value);
  CString GetProperty(eAction e_Action);

  CString GetParameter(CString p_name);
  bool    SetParameter(CString p_name,CString p_value);
  int     GetNumberOfParameters();
  bool    RemoveParameter(CString p_name);


  ParameterMap* GetParameterMap();

protected:
  void    LoadParameters();
  void    SaveParameters();

  CComQIPtr<IHTMLObjectElement3, &IID_IHTMLObjectElement3> mi_object;
  ParameterMap  m_parameters;
  bool          m_paramChanged;
};

inline ParameterMap* 
HtmlObject::GetParameterMap()
{
  return &m_parameters;
}

inline int
HtmlObject::GetNumberOfParameters()
{
  return (int)m_parameters.size();
}

