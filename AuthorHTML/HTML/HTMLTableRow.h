//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    HTMLTableRow.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Handles the <ROW> tag of the HTML
//
#pragma once
#include "HtmlElement.h"

class HtmlTable;
class HtmlTableCell;

class HtmlTableRow : public HtmlElement
{
public:
	HtmlTableRow(CComPtr<IHTMLElement> i_Elem);

  virtual BOOL  Valid();
  enum eAction
  {
    E_AlignHorz,  // "align"  Horizontal alignment of text in cell
    E_AlignVert,  // "valign" Alignment of text in cell
    E_Background, // Background
    E_BgColor,    // Background color
    E_Height,     // Height of cell
    E_Width,      // Width  of cell
    E_BorderColor,
    E_BorderColorLight,
    E_BorderColorDark
  };
  void          SetProperty(eAction e_Action, CString s_Value);
  CString       GetProperty(eAction e_Action);

	UINT          GetCellCount();
	UINT          GetRowIndex();
	BOOL          DeleteCell(UINT Index);
	BOOL          DeleteColumn(UINT Index);
	HtmlTableCell InsertColumn(UINT Index);
	HtmlTableCell GetCell     (UINT Index);
	HtmlTableCell GetColumn   (UINT Index, BOOL b_ReturnPrevious =FALSE);
	HtmlTableCell InsertCell  (UINT Index);
	HtmlTable     GetParentTable();
  BOOL          AlterColumnWidth(UINT Index,int delta_width,int total);

protected:
	CComQIPtr<IHTMLTableRow, &IID_IHTMLTableRow> m_Row;
};
