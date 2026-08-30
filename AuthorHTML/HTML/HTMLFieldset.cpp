//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    HTMLFieldSet.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Handles the <FIELDSET> tag of the HTML
//
#include "StdAfx.h"
#include "HTMLFieldset.h"
#include "HTMLStyle.h"

HtmlFieldset::HtmlFieldset(CComPtr<IHTMLElement> i_Elem) 
             :HtmlElement(i_Elem)
             ,m_legend(NULL)
{
  mi_Fieldset = i_Elem;
  FindLegend();
}

BOOL 
HtmlFieldset::Valid()
{
  return (mi_Fieldset != NULL);
}

void 
HtmlFieldset::SetProperty(eAction e_Action, CString s_Value)
{
  CComVariant v_Value(s_Value);
  switch (e_Action)
  {
    case E_Legend:if(m_legend.Valid())
                  {
                    if(s_Value.IsEmpty())
                    {
                      m_legend.Remove();
                    }
                    else
                    {
                      m_legend.SetInnerHtml(s_Value);
                    }
                  }
                  else if(!s_Value.IsEmpty())
                  {
                    SetLegend(s_Value);
                  }
                  break;
    case E_LegendAlign:
                  if(!m_legend.Valid())
                  {
                    SetLegend("&nbrsp;");
                  }
                  m_legend.SetAttribute("align",s_Value);
                  break;
    case E_Align: if(m_Style.Valid())
                  {
                    m_Style.SetProperty(HtmlStyle::P_TextAlign,s_Value);
                  }
                  CComBSTR align = s_Value;
                  mi_Fieldset->put_align(align);  
                  break;
  }
}

CString 
HtmlFieldset::GetProperty(eAction e_Action)
{
  CString prop;
  switch (e_Action)
  {
    case E_Align: if(m_Style.Valid())
                  {
                    prop = m_Style.GetProperty(HtmlStyle::P_TextAlign);
                    if(!prop.IsEmpty())
                    {
                      return prop;
                    }
                  }
                  return GetAttribute("align");
    case E_Legend:if(m_legend.Valid())
                  {
                    return m_legend.GetInnerText();
                  }
                  break;
    case E_LegendAlign:
                  if(m_legend.Valid())
                  {
                    return m_legend.GetAttribute("align");
                  }
                  break;
  }
  return "";
}

void
HtmlFieldset::FindLegend()
{
  CComQIPtr<IHTMLElementCollection> coll;
  UINT length = GetChildCollection(coll);
  for(UINT num = 0;num < length; ++num)
  {
    HtmlElement elem = GetElementFromCollection(num,coll);
    CString tag = elem.GetTagName();
    if(tag.CompareNoCase("legend") == 0)
    {
      m_legend = elem;
      return;
    }
  }
}

void
HtmlFieldset::SetLegend(CString text)
{
  if(!m_legend.Valid())
  {
    CString elem;
    elem.Format("<LEGEND>%s</LEGEND>",text.GetString());
    InsertHtml(elem,TRUE,TRUE);
    FindLegend();
  }
}
