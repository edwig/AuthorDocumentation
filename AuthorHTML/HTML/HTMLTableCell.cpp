//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    HTMLTableCell.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Handles the <CELL> tag of the HTML
//
#include <stdafx.h>
#include "HTMLTableCell.h"
#include "HTMLTableRow.h"
#include "HTMLTable.h"

// Constructor
HtmlTableCell::HtmlTableCell(CComPtr<IHTMLElement> i_Elem) 
              :HtmlElement(i_Elem)
{
	m_Cell = i_Elem;
  if(m_Cell != NULL)
  {
    CComPtr<IHTMLStyle> style;
    i_Elem->get_style(&style);
    m_Style.SetStyle(style);
  }
}

BOOL 
HtmlTableCell::Valid()
{
	return (m_Cell != NULL);
}

void
HtmlTableCell::SetProperty(eAction e_Action, CString s_Value)
{
  switch(e_Action)
  {
    case E_AlignHorz:   // "align"  Horizontal alignment of text in cell
                        if(m_Style.Valid())
                        {
                          m_Style.SetProperty(HtmlStyle::P_TextAlign,s_Value);
                        }
                        SetAttribute("align",s_Value);
                        break;
    case E_AlignVert:   // "valign" Alignment of text in cell
                        if(m_Style.Valid())
                        {
                          m_Style.SetProperty(HtmlStyle::P_VerticalAlign,s_Value);
                        }
                        SetAttribute("valign",s_Value);
                        break;
    case E_Background:  // Background
                        if(m_Style.Valid())
                        {
                          m_Style.SetProperty(HtmlStyle::P_BackgImage,s_Value);
                        }
                        SetAttribute("background",s_Value);
                        break;
    case E_BgColor:     // Background color
                        if(m_Style.Valid())
                        {
                          m_Style.SetProperty(HtmlStyle::P_BackgColor,s_Value);
                        }
                        SetAttribute("bgcolor",s_Value);
                        break;
    case E_Height:      // Height of cell
                        if(m_Style.Valid())
                        {
                          m_Style.SetProperty(HtmlStyle::P_Height,s_Value);
                        }
                        SetAttribute("height",s_Value);
                        break;
    case E_Width:       // Width  of cell
                        if(m_Style.Valid())
                        {
                          m_Style.SetProperty(HtmlStyle::P_Width,s_Value);
                        }
                        SetAttribute("width",s_Value);
                        break;
    case E_BorderColor: if(m_Style.Valid())
                        {
                          m_Style.SetProperty(HtmlStyle::P_BorderColor,s_Value);
                        }
                        SetAttribute("bordercolor",s_Value);
                        break;

    case E_BorderColorDark: SetAttribute("bordercolordark",s_Value);
                            break;
    case E_BorderColorLight:SetAttribute("bordercolorlight",s_Value);
                            break;
  }
}

CString
HtmlTableCell::GetProperty(eAction e_Action)
{
  CString prop;
  switch(e_Action)
  {
    case E_AlignHorz:   // "align"  Horizontal alignment of text in cell
                        if(m_Style.Valid())
                        {
                          prop = m_Style.GetProperty(HtmlStyle::P_TextAlign);
                          if(!prop.IsEmpty())
                          {
                            return prop;
                          }
                        }
                        return GetAttribute("align");
    case E_AlignVert:   // "valing" Alignment of text in cell
                        if(m_Style.Valid())
                        {
                          prop = m_Style.GetProperty(HtmlStyle::P_VerticalAlign);
                          if(!prop.IsEmpty())
                          {
                            return prop;
                          }
                        }
                        return GetAttribute("valign");
    case E_Background:  // Background
                        if(m_Style.Valid())
                        {
                          prop = m_Style.GetProperty(HtmlStyle::P_BackgImage);
                          if(!prop.IsEmpty())
                          {
                            return prop;
                          }
                        }
                        return GetAttribute("background");
    case E_BgColor:     // Background color
                        if(m_Style.Valid())
                        {
                          prop = m_Style.GetProperty(HtmlStyle::P_BackgColor);
                          if(!prop.IsEmpty())
                          {
                            return prop;
                          }
                        }
                        return GetAttribute("bgcolor");
    case E_Height:      // Height of cell
                        if(m_Style.Valid())
                        {
                          prop = m_Style.GetProperty(HtmlStyle::P_Height);
                          if(!prop.IsEmpty())
                          {
                            return prop;
                          }
                        }
                        return GetAttribute("height");
    case E_Width:       // Width  of cell
                        if(m_Style.Valid())
                        {
                          prop = m_Style.GetProperty(HtmlStyle::P_Width);
                          if(!prop.IsEmpty())
                          {
                            return prop;
                          }
                        }
                        return GetAttribute("width");
    case E_BorderColor: if(m_Style.Valid())
                        {
                          prop = m_Style.GetProperty(HtmlStyle::P_BorderColor);
                          if(!prop.IsEmpty())
                          {
                            return prop;
                          }
                        }
                        return GetAttribute("bordercolor");

    case E_BorderColorDark: return GetAttribute("bordercolordark");
    case E_BorderColorLight:return GetAttribute("bordercolorlight");
  }
  return "";
}

HtmlTableRow 
HtmlTableCell::GetParentRow()
{
	return HtmlTableRow(FindParent(_T("TR")));
}

HtmlTable 
HtmlTableCell::GetParentTable()
{
	return HtmlTable(FindParent(_T("TABLE")));
}

UINT
HtmlTableCell::GetParentRowGroup()
{
  HtmlElement elem = FindParent(_T("TBODY"));
  if(elem.Valid()) return RG_BODY;
  elem = FindParent(_T("THEAD"));
  if(elem.Valid()) return RG_HEAD;
  elem = FindParent(_T("TFOOT"));
  if(elem.Valid()) return RG_FOOT;
  return 0;
}

// zero based
UINT 
HtmlTableCell::GetCellIndex()
{
	long Index;
	m_Cell->get_cellIndex(&Index);
	return Index;
}

// get the column of the current cell
// zero based
UINT 
HtmlTableCell::GetColIndex()
{
	UINT u32_Col = 0;
	HtmlTableCell i_Cell = *this;

	while (TRUE)
	{
		// go back until the leftmost is reached
		i_Cell = HtmlTableCell(i_Cell.PreviousSibling());
		if (!i_Cell.Valid())
			break;

		u32_Col += i_Cell.GetColSpan();
	}
	return u32_Col;
}

// zero based
UINT 
HtmlTableCell::GetRowIndex()
{
	return GetParentRow().GetRowIndex();
}

UINT 
HtmlTableCell::GetColSpan()
{
	long Span = 1;
	m_Cell->get_colSpan(&Span);
	return Span;
}

BOOL 
HtmlTableCell::SetColSpan(UINT Span)
{
	return SUCCEEDED(m_Cell->put_colSpan(Span));
}

// overrides HtmlElement::SetInnerHtml(...)
// Assure that hitting the enter key inside any table cell inserts <DIV> instead of <P>
BOOL 
HtmlTableCell::SetInnerHtml(CString s_Html)
{
	//return HtmlElement::SetInnerHtml(_T("<DIV>") +s_Html+ _T("</DIV>")); 
	return HtmlElement::SetInnerHtml(s_Html); 
}

UINT 
HtmlTableCell::GetRowSpan()
{
	long Span = 1;
	m_Cell->get_rowSpan(&Span);
	return Span;
}

UINT
HtmlTableCell::SetRowSpan(UINT Span)
{
  return SUCCEEDED(m_Cell->put_rowSpan(Span));
}

// melts the cell with its right neighbor
BOOL 
HtmlTableCell::CombineHorizontal()
{
	HtmlTableCell i_Right(NextSibling());
	if (!i_Right.Valid())
		return FALSE;

	// the new cell gets the content of both former cells concatenated
	// If the former content is surrounded by "<DIV>..</DIV>" it will be removed
	CString s_Div   = _T("DIV");
  CString s_Left  = Misc::RemoveTag(this->  GetInnerHtml(), s_Div);
  CString s_Right = Misc::RemoveTag(i_Right.GetInnerHtml(), s_Div);
	
	// avoid multiple "&nbsp;&nbsp;" behind each other in the cell
	CString s_Space = _T("&nbsp;");
  s_Left  = Misc::CutString(s_Left,  s_Space, TRUE);
  s_Right = Misc::CutString(s_Right, s_Space, FALSE);
	
	SetInnerHtml(s_Left + s_Space + s_Right);

	SetColSpan(GetColSpan() + i_Right.GetColSpan());
	i_Right.Remove();

	GetParentTable().CleanUp();
	return TRUE;
}

// melts the cell with its lower neighbor
BOOL 
HtmlTableCell::CombineVertical()
{
  UINT u32_Row  = GetRowIndex();
  UINT u32_Col  = GetColIndex();
  HtmlTable i_Table  = GetParentTable();
  HtmlTableRow i_Row = i_Table.GetRow(u32_Row + 1);
  // Check if there is a next row
  if(!i_Row.Valid())
  {
    return FALSE;
  }
  HtmlTableCell i_Under = i_Row.GetCell(u32_Col);
  // Check if there is a cell under this one
  if (!i_Under.Valid())
  {
    return FALSE;
  }
  // the new cell gets the content of both former cells concatenated
  // If the former content is surrounded by "<DIV>..</DIV>" it will be removed
  CString s_Div   = _T("DIV");
  CString s_Upper = Misc::RemoveTag(this->  GetInnerHtml(), s_Div);
  CString s_Lower = Misc::RemoveTag(i_Under.GetInnerHtml(), s_Div);

  // avoid multiple "<br><br>;" behind each other in the cell
  CString s_Space = _T("<br>");
  s_Upper = Misc::CutString(s_Upper,s_Space, TRUE);
  s_Lower = Misc::CutString(s_Lower,s_Space, FALSE);

  SetInnerHtml(s_Upper + s_Space + s_Lower);

  SetRowSpan(GetRowSpan() + i_Under.GetRowSpan());
  i_Under.Remove();

  i_Table.CleanUp();
  return TRUE;
}

// splits a multi-column cell into 2 sub-cells
void 
HtmlTableCell::SplitHorizontal()
{
	UINT u32_Row  = GetRowIndex();
	UINT u32_Col  = GetColIndex();
	UINT u32_Span = GetColSpan();

	// insert new cell behind "this" cell in the same row
	HtmlTableCell i_NewCell = GetParentRow().InsertColumn(u32_Col + u32_Span);

	if (u32_Span == 1) // split single-column cell
	{
		HtmlTable i_Table = GetParentTable();
		UINT u32_Count = i_Table.GetRowCount();
		for (UINT i=0; i<u32_Count; i++)
		{
			if (i == u32_Row) // for this row the job is already done
				continue;
			
			HtmlTableCell i_Cell = i_Table.GetCell(i, u32_Col, TRUE);
			if (i_Cell.Valid()) i_Cell.SetColSpan(i_Cell.GetColSpan() +1);
		}
	}
	else // split multi-column cell
	{
		// split in the middle (e.g. Colspan=5 --> Colspan=2 and Colspan=3)
	UINT u32_Span1 = u32_Span /2;
	UINT u32_Span2 = u32_Span - u32_Span1;

	    this->SetColSpan(u32_Span1);
	i_NewCell.SetColSpan(u32_Span2);
	}
}

// splits a multi-column cell into 2 sub-cells
void 
HtmlTableCell::SplitVertical()
{
  UINT u32_Row  = GetRowIndex();
  UINT u32_Col  = GetColIndex();
  UINT u32_Span = GetRowSpan();

  HtmlTable i_Table  = GetParentTable();
  HtmlTableRow i_Row = i_Table.GetRow(u32_Row + u32_Span - 1);

  // insert new cell behind "this" cell in the same row
  HtmlTableCell i_NewCell = i_Row.InsertColumn(u32_Col);

  if (u32_Span == 1) // split single-column cell
  {
    HtmlTable table = GetParentTable();
    UINT u32_Count = table.GetColumnCount();
    for (UINT i=0; i<u32_Count; i++)
    {
      if (i == u32_Col) // for this col the job is already done
      {
       continue;
      }
      HtmlTableCell i_Cell = table.GetCell(u32_Row, i, TRUE);
      if (i_Cell.Valid()) 
      {
        i_Cell.SetRowSpan(i_Cell.GetRowSpan() +1);
      }
    }
  }
  else // split multi-column cell
  {
    // split in the middle (e.g. Rowspan = 5 --> Rowspan=2 and Rowspan=3)
    UINT u32_Span1 = u32_Span / 2;
    UINT u32_Span2 = u32_Span - u32_Span1;

    this->SetRowSpan(u32_Span1);
    i_NewCell.SetRowSpan(u32_Span2);
  }
}

CComPtr<IHTMLElement>
HtmlTableCell::Focus()
{
  return m_Elem;
}

CComPtr<IHTMLElement>
HtmlTableCell::FocusNextCell()
{
	HtmlTableCell i_Right(NextSibling());
	if (!i_Right.Valid())
  {
    // To the next row?
    HtmlTable table  = GetParentTable();
    HtmlTableRow row = GetParentRow();

    // TODO TAB
    // WORKS ONLY WITHOUT TFOOT
    UINT numRow = table.GetRowCount();
    if(GetRowIndex() == (numRow - 1))
    {
      // Make new row first. Automatically extending the table
      // Most editing programs do this and users expect this!
      table.InsertRow(numRow,-1);
    }
    // TODO TAB
    // Does only work in TBODY (not over THEAD / TFOOT)
    HtmlTableRow nextRow(row.NextSibling());
    if(nextRow.Valid())
    {
      HtmlTableCell first = nextRow.GetCell(0);
      return first.Focus();
    }
  }
  return i_Right.Focus();
}

CComPtr<IHTMLElement>
HtmlTableCell::FocusPrevCell()
{
 	HtmlTableCell i_Right(PreviousSibling());
	if (!i_Right.Valid())
  {
    HtmlTable table = GetParentTable();
    UINT rowCount   = GetRowIndex();
    if(rowCount)
    {
      // Go the the last cell in the previous row
      HtmlTableRow row = GetParentRow();
      HtmlTableRow prev(row.PreviousSibling());
      if(prev.Valid())
      {
        UINT cellCount = prev.GetCellCount();
        HtmlTableCell last = prev.GetCell(cellCount - 1);
        return last.Focus();
      }
    }
    return NULL;
  }
  return i_Right.Focus();
}