//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    HTMLTableCell.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Handles the <CELL> tag of the HTML
//
#pragma once

#include <atlbase.h>    // CComPtr
#include <afxhtml.h>    // CHtmlEditView
#include "HTMLElement.h"
#include "HTMLStyle.h"

#define RG_HEAD  0x001
#define RG_BODY  0x002
#define RG_FOOT  0x003

class HtmlTable;
class HtmlTableRow;

class HtmlTableCell : public HtmlElement
{
public:
	HtmlTableCell(CComPtr<IHTMLElement> i_Elem);
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
  
  HtmlTableRow  GetParentRow();
	HtmlTable     GetParentTable();
  UINT          GetParentRowGroup();
	BOOL          SetColSpan(UINT Span);
	UINT          GetCellIndex();
	UINT          GetColIndex();
	UINT          GetRowIndex();
	UINT          GetColSpan();
	UINT          GetRowSpan();
  UINT          SetRowSpan(UINT Span);
	BOOL          CombineHorizontal();
  BOOL          CombineVertical();
	void          SplitHorizontal();
  void          SplitVertical();
	BOOL          SetInnerHtml(CString s_Html);

  CComPtr<IHTMLElement> Focus();
  CComPtr<IHTMLElement> FocusNextCell();
  CComPtr<IHTMLElement> FocusPrevCell();

protected:
	CComQIPtr<IHTMLTableCell, &IID_IHTMLTableCell> m_Cell;
  HtmlStyle m_Style;
};
