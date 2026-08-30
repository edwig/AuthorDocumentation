//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    HTMLTableRow.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Handles the <ROW> tag of the HTML
//
#include <StdAfx.h>
#include "HtmlTableRow.h"
#include "HtmlTableCell.h"
#include "HtmlTable.h"

// Constructor
HtmlTableRow::HtmlTableRow(CComPtr<IHTMLElement> i_Elem) : HtmlElement(i_Elem)
{
	m_Row = i_Elem;
}

BOOL 
HtmlTableRow::Valid()
{
	return (m_Row != NULL);
}

void
HtmlTableRow::SetProperty(eAction e_Action, CString s_Value)
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
HtmlTableRow::GetProperty(eAction e_Action)
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

// zero based
UINT 
HtmlTableRow::GetRowIndex()
{
	long Index;
	m_Row->get_rowIndex(&Index);
	return Index;
}

// deletes the n-th cell in the row 
// The index is always the n_th cell no matter which column span the cells have
// zero based index
BOOL 
HtmlTableRow::DeleteCell(UINT Index)
{
	return (SUCCEEDED(m_Row->deleteCell(Index)));
}

// deletes the n-th column in the row 
// If the first cell spans about 3 columns, DeleteColumn(3) will delete the second cell
// If the cell to be deleted spans about more columns, only the cellspan will be decreased
// zero based index
BOOL 
HtmlTableRow::DeleteColumn(UINT Index)
{
	UINT u32_Col = 0;
	HtmlTableCell i_Cell = GetCell(0);
	while (TRUE)
	{
		if (!i_Cell.Valid())
			break;

		UINT u32_Span = i_Cell.GetColSpan();
		if  (u32_Span == 1)
		{
			if (u32_Col == Index)
			{
				BOOL b_Ret = i_Cell.Remove(); // remove cell
				// if this was the last column -> remove complete table
				if (!GetCellCount()) GetParentTable().Remove();
				return b_Ret;
			}
		}
		else if (u32_Span > 1) // cell spans about multiple columns
		{
			if (Index >= u32_Col && Index < u32_Col + u32_Span)
				return i_Cell.SetColSpan(u32_Span -1); // reduce cellspan
		}
		u32_Col += u32_Span;
		i_Cell = HtmlTableCell(i_Cell.NextSibling());
	}
	return FALSE;	
}

// zero based
// inserts a new cell at the given cell position 
// no matter which colspans the precedings cells have
// returns the new cell
HtmlTableCell 
HtmlTableRow::InsertCell(UINT Index)
{
	CComPtr<IDispatch> D_Cell = 0;
	m_Row->insertCell(Index, &D_Cell);

	CComQIPtr<IHTMLElement, &IID_IHTMLElement> i_Cell = D_Cell;
	HtmlTableCell i_NewCell = HtmlTableCell((CComPtr<IHTMLElement>)i_Cell);

	// Set new cell's content = &nbsp; otherwise the border may be displayed wrong and
	// the cell appears different in Design mode and Browse mode if it is completely empty
	// SetInnerHtml() will add <DIV></DIV>
	// i_NewCell.SetInnerHtml(_T("&nbsp;"));
  i_NewCell.SetInnerHtml(_T("<P>&nbsp;</P>"));
	return i_NewCell;
}

// zero based
// inserts a new cell at the given column position
// respecting the colspans of the preceding cells
// returns the new cell or the cell which was made wider
HtmlTableCell 
HtmlTableRow::InsertColumn(UINT Index)
{
	UINT u32_Col = 0;
	for (UINT i=0; TRUE; i++)
	{
		HtmlTableCell i_Cell = GetCell(i);
		if (!i_Cell.Valid())
			return InsertCell(i); // insert behind the rightmost cell

		UINT u32_Span = i_Cell.GetColSpan();
		if (u32_Col == Index) // insert at the left of the specified cell
			return InsertCell(i);
		
		if (Index > u32_Col && Index < u32_Col + u32_Span)
		{
			i_Cell.SetColSpan(u32_Span +1);
			return i_Cell; // increase cellspan of multi column cell
		}
		u32_Col += u32_Span;
	}
}

// zero based
// returns the cell at the given CELL index
// This function may return an invalid element !!!! (Check Ret.Valid() !)
HtmlTableCell 
HtmlTableRow::GetCell(UINT Index)
{
	CComPtr<IHTMLElementCollection> i_Collect = 0;
	m_Row->get_cells(&i_Collect);
	if (!i_Collect)
		return HtmlTableCell(0);

	CComVariant v_Index((int)Index);
	CComPtr<IDispatch> D_Cell = 0;
	i_Collect->item(v_Index, v_Index, &D_Cell);

	CComQIPtr<IHTMLElement, &IID_IHTMLElement> i_Cell = D_Cell;
	return HtmlTableCell((CComPtr<IHTMLElement>)i_Cell);
}

// zero based
// returns the cell at the given COLUMN index
// If there is no cell at the given column index because the left neighbour cell spans over
// into this column and b_ReturnPrevious=FALSE, an invalid cell will be returned! 
// If b_ReturnPrevious=TRUE the previous cell (left neighbour) will be returned in this case
// (Check Ret.Valid() !)
HtmlTableCell 
HtmlTableRow::GetColumn(UINT Index, BOOL b_ReturnPrevious) // =FALSE
{
	UINT u32_Col = 0;
	HtmlTableCell i_PrevCell = HtmlTableCell(0);

	for (UINT i=0; TRUE; i++) // i = CELL Index !
	{
		HtmlTableCell i_Cell = GetCell(i);
		if (!i_Cell.Valid() || u32_Col > Index) // not found
		{
			if (b_ReturnPrevious) return i_PrevCell;        // return left neighbour
			else                  return HtmlTableCell(0); // return invalid cell
		}

		if (u32_Col == Index)
			return i_Cell; // found !

		i_PrevCell = i_Cell;

		u32_Col += i_Cell.GetColSpan(); // u32_Col = COLUMN Index !
	}
}

// returns the number of cells in a row
UINT 
HtmlTableRow::GetCellCount()
{
	CComPtr<IHTMLElementCollection> i_Collect = 0;
	m_Row->get_cells(&i_Collect);
	if (!i_Collect)
		return 0;

	long Len = 0;
	i_Collect->get_length(&Len);
	return Len;
}

HtmlTable 
HtmlTableRow::GetParentTable()
{
	return HtmlTable(FindParent(_T("TABLE")));
}

BOOL
HtmlTableRow::AlterColumnWidth(UINT Index,int delta_width,int total)
{
	UINT u32_Col = 0;

	for (UINT i=0; TRUE; i++) // i = CELL Index !
	{
		HtmlTableCell i_Cell = GetCell(i);
		if (!i_Cell.Valid() || u32_Col > Index) // not found
		{
      return FALSE;;
		}
		if (u32_Col == Index)
    {
      int w;
      CString sWidth = i_Cell.GetProperty(HtmlTableCell::E_Width);
      w = atoi(sWidth);
      if(sWidth.Find('%') >= 0)
      {
        // Oeps, it's a percentage
        int delta = (w * delta_width) / total;
        w += delta;
        if(w >= 0)
        {
          sWidth.Format("%d%%",w);
          i_Cell.SetProperty(HtmlTableCell::E_Width,sWidth);
          return TRUE;
        }
      }
      else
      {
        w += delta_width;
        if(w >= 0)
        {
          sWidth.Format("%d",w);
          i_Cell.SetProperty(HtmlTableCell::E_Width,sWidth);
          return TRUE;
        }
      }
      return FALSE;
    }
		u32_Col += i_Cell.GetColSpan(); // u32_Col = COLUMN Index !
	}
  return FALSE;
}
