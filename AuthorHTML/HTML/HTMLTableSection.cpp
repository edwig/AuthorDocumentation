//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    HTMLTableSection.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Handles the <TABLESECTION> tag of the HTML
//
#include "StdAfx.h"
#include "HtmlTableSection.h"
#include "HTMLTableCell.h"

// Constructor
HtmlTableSection::HtmlTableSection(CComPtr<IHTMLElement> i_Elem) : HtmlElement(i_Elem)
{
  mi_section = i_Elem;
}

BOOL 
HtmlTableSection::Valid()
{
  return (mi_section != NULL);
}

void    
HtmlTableSection::SetProperty(eAction e_Action, CString s_Value)
{
  switch(e_Action)
  {
    case E_Align:   if(m_Style.Valid())
                    {
                      m_Style.SetProperty(HtmlStyle::P_TextAlign,s_Value);
                    }
                    SetAttribute("align",s_Value);
                    break;
    case E_BgColor: if(m_Style.Valid())
                    {
                      m_Style.SetProperty(HtmlStyle::P_BackgColor,s_Value);
                    }
                    SetAttribute("bgcolor",s_Value);
                    break;
    case E_Valign:  if(m_Style.Valid())
                    {
                      m_Style.SetProperty(HtmlStyle::P_VerticalAlign,s_Value);
                    }
                    SetAttribute("valign",s_Value);
                    break;
  }
}

CString 
HtmlTableSection::GetProperty(eAction e_Action)
{
  switch(e_Action)
  {
    case E_Align:   if(m_Style.Valid())
                    {
                      CString prop = m_Style.GetProperty(HtmlStyle::P_TextAlign);
                      if(!prop.IsEmpty())
                      {
                        return prop;
                      }
                    }
                    return GetAttribute("align");
    case E_BgColor: if(m_Style.Valid())
                    {
                      CString prop = m_Style.GetProperty(HtmlStyle::P_BackgColor);
                      if(!prop.IsEmpty())
                      {
                        return prop;
                      }
                    }
                    return GetAttribute("bgcolor");
    case E_Valign:  if(m_Style.Valid())
                    {
                      CString prop = m_Style.GetProperty(HtmlStyle::P_VerticalAlign);
                      if(!prop.IsEmpty())
                      {
                        return prop;
                      }
                    }
                    return GetAttribute("valign");
  }
  return "";
}

// Looks like InsertRow in HtmlTable but is different!!!!!!
// The "insertRow" interface acts on a different interface!
// "IHTMLTableSection" instead of "IHTMLTable" !!!!!!!!!!!!
HtmlTableRow 
HtmlTableSection::InsertRow(int Index, int CellCount)
{
	CComPtr<IDispatch> D_Row = 0;
	mi_section->insertRow(Index, &D_Row);

	CComQIPtr<IHTMLElement, &IID_IHTMLElement> i_Row = D_Row;
	HtmlTableRow i_NewRow = HtmlTableRow((CComPtr<IHTMLElement>)i_Row);

	for (int i=0; i<CellCount; i++)
	{
		HtmlTableCell i_NewCell = i_NewRow.InsertCell(0);
	}
	return i_NewRow;
}

UINT
HtmlTableSection::GetRows()
{
  CComPtr<IHTMLElementCollection> col;
  HRESULT hr = mi_section->get_rows(&col);
  if(SUCCEEDED(hr))
  {
    long num;
    col->get_length(&num);
    return (UINT)num;
  }
  return 0;
}
