//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    HTMLTable.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Handles the <TABLE> tag of the HTML
//
#pragma once
#include "HTMLElement.h"
#include "HTMLTableRow.h"
#include "HTMLTableCell.h"
#include "HTMLTableSection.h"
#include "HTMLCaption.h"
#include "HTMLStyle.h"

class HtmlTable : public HtmlElement
{
public:
	HtmlTable(CComPtr<IHTMLElement> i_Elem);
	virtual BOOL  Valid();

  enum eRules // Table border
	{
		E_RulesAll  = 0,  // Horizontal lines, vertical lines and box
		E_RulesHor  = 1,  // Horizontal lines and box
		E_RulesVert = 2,  // Vertikal   lines and box
		E_RulesBox  = 3   // Only Box (border around)
	};
  enum eAction
  {
    E_Alignment,      // alignment
    E_BgColor,        // bgcolor
    E_BorderColor,    // bordercolor
    E_BorderColorDark,
    E_BorderColorLight,
    E_BorderRules,
    E_BorderWidth,
    E_CellPadding,
    E_CellSpacing,
    E_Height,
    E_HSpace,
    E_VSpace,
    E_Width,
  };
  void          SetProperty(eAction e_Action, CString s_Value);
  CString       GetProperty(eAction e_Action);

	BOOL              DeleteRow(UINT Index);
	BOOL              DeleteColumn(UINT Index);
	HtmlTableRow      GetRow   (UINT Index);
	HtmlTableRow      InsertRow(UINT Index, int CellCount);
	BOOL              InsertColumn(UINT Index);
	BOOL              GetSpanMap(UINT *pu32_Rows, UINT *pu32_Cols, UINT **ppu32_Map);
	void              CleanUp();
	UINT              GetColumnCount();
	UINT              GetRowCount();
	HtmlTableCell     GetCell(UINT u32_Row, UINT u32_Column, BOOL b_ReturnPrevious=FALSE);
  BOOL              AlterColumnWidth(UINT Index,int delta_width,int total);
  HtmlCaption       GetCaption(bool make = false);
  HtmlTableSection  GetHeader (bool make = false);
  HtmlTableSection  GetFooter (bool make = false);
  BOOL              DeleteCaption();
  BOOL              DeleteHeader();
  BOOL              DeleteFooter();

protected:
	CComQIPtr<IHTMLTable, &IID_IHTMLTable> m_Table;
};
