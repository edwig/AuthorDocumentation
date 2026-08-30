//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    HTMLTable.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Handles the <TABLE> tag of the HTML
//
#include "StdAfx.h"
#include "HTMLTable.h"
#include "HTMLStyle.h"
#include "AuthorHTML.h"

// Constructor
HtmlTable::HtmlTable(CComPtr<IHTMLElement> i_Elem) 
          :HtmlElement(i_Elem)
{
	m_Table = i_Elem;
}

BOOL 
HtmlTable::Valid()
{
	return (m_Table != NULL);
}

void
HtmlTable::SetProperty(eAction e_Action, CString s_Value)
{
  switch(e_Action)
  {
    case E_Alignment:       if(m_Style.Valid())
                            {
                              m_Style.SetProperty(HtmlStyle::P_TextAlign,s_Value);
                            }
                            SetAttribute("align",s_Value);
                            break;
    case E_BgColor:         if(m_Style.Valid())
                            {
                              m_Style.SetProperty(HtmlStyle::P_BackgColor,s_Value);
                            }
                            SetAttribute("bgcolor",s_Value);
                            break;
    case E_BorderColor:     if(m_Style.Valid())
                            {
                              m_Style.SetProperty(HtmlStyle::P_BorderColor,s_Value);
                            }
                            SetAttribute("bordercolor",s_Value);
                            break;
    case E_BorderColorDark: SetAttribute("bordercolordark",s_Value);
                            break;
    case E_BorderColorLight:SetAttribute("bordercolorlight",s_Value);
                            break;
    case E_BorderRules:     SetAttribute("borderrules",s_Value);
                            break;
    case E_BorderWidth:     if(m_Style.Valid())
                            {
                              m_Style.SetProperty(HtmlStyle::P_BorderWidth,s_Value);
                            }
                            SetAttribute("borderwidth",s_Value);
                            break;
    case E_CellPadding:     SetAttribute("cellpadding",s_Value);
                            break;
    case E_CellSpacing:     SetAttribute("cellspacing",s_Value);
                            break;
    case E_Height:          if(m_Style.Valid())
                            {
                              m_Style.SetProperty(HtmlStyle::P_Height,s_Value);
                            }
                            SetAttribute("height",s_Value);
                            break;
    case E_HSpace:          if(m_Style.Valid())
                            {
                              m_Style.SetProperty(HtmlStyle::P_PaddingLeft, s_Value);
                              m_Style.SetProperty(HtmlStyle::P_PaddingRight,s_Value);
                            }
                            SetAttribute("hspace",s_Value);
                            break;
    case E_VSpace:          if(m_Style.Valid())
                            {
                              m_Style.SetProperty(HtmlStyle::P_PaddingTop,   s_Value);
                              m_Style.SetProperty(HtmlStyle::P_PaddingBottom,s_Value);
                            }
                            SetAttribute("vspace",s_Value);
                            break;
    case E_Width:           if(m_Style.Valid())
                            {
                              m_Style.SetProperty(HtmlStyle::P_Width,s_Value);
                            }
                            SetAttribute("width",s_Value);
                            break;
  }
}

CString
HtmlTable::GetProperty(eAction e_Action)
{
  CString prop;
  switch(e_Action)
  {
    case E_Alignment:       if(m_Style.Valid())
                            {
                              prop = m_Style.GetProperty(HtmlStyle::P_TextAlign);
                              if(!prop.IsEmpty())
                              {
                                return prop;
                              }
                            }
                            return GetAttribute("align");
    case E_BgColor:         if(m_Style.Valid())
                            {
                              prop = m_Style.GetProperty(HtmlStyle::P_BackgColor);
                              if(!prop.IsEmpty())
                              {
                                return prop;
                              }
                            }
                            return GetAttribute("bgcolor");
    case E_BorderColor:     if(m_Style.Valid())
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
    case E_BorderRules:     return GetAttribute("borderrules");
    case E_BorderWidth:     if(m_Style.Valid())
                            {
                              prop = m_Style.GetProperty(HtmlStyle::P_BorderWidth);
                              if(!prop.IsEmpty())
                              {
                                return prop;
                              }
                            }
                            return GetAttribute("borderwidth");
    case E_CellPadding:     return GetAttribute("cellpadding");
    case E_CellSpacing:     return GetAttribute("cellspacing");
    case E_Height:          if(m_Style.Valid())
                            {
                              prop = m_Style.GetProperty(HtmlStyle::P_Height);
                              if(!prop.IsEmpty())
                              {
                                return prop;
                              }
                            }
                            return GetAttribute("height");
    case E_HSpace:          if(m_Style.Valid())
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
    case E_VSpace:          if(m_Style.Valid())
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
    case E_Width:           if(m_Style.Valid())
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

// zero based
BOOL 
HtmlTable::DeleteRow(UINT Index)
{
	BOOL b_OK = SUCCEEDED(m_Table->deleteRow(Index));

	// If the table has no rows anymore -> delete it completely !
	if (!GetRowCount()) this->Remove();
	else CleanUp();
	return b_OK;
}

// zero based
BOOL 
HtmlTable::DeleteColumn(UINT Index)
{
	BOOL b_Ret = TRUE;
	for (UINT i=0; TRUE; i++)
	{
		HtmlTableRow i_Row = GetRow(i);
		if (!i_Row.Valid())
			break;

		if (!i_Row.DeleteColumn(Index))
			b_Ret = FALSE;
	}
	return b_Ret;
}

// zero based Index
// if CellCount  > 0 -> inserts CellCount new cells into the new row
// if CellCount == 0 -> inserts no cells, caller must insert new cells
// if CellCount  < 0 -> inserts as many new cells as row 0 has
HtmlTableRow 
HtmlTable::InsertRow(UINT Index, int CellCount)
{
	if (CellCount < 0) CellCount = GetColumnCount(); // get cols of row 0

	CComPtr<IDispatch> D_Row = 0;
	m_Table->insertRow(Index, &D_Row);

	CComQIPtr<IHTMLElement, &IID_IHTMLElement> i_Row = D_Row;
	HtmlTableRow i_NewRow = HtmlTableRow((CComPtr<IHTMLElement>)i_Row);

	for (int i=0; i<CellCount; i++)
	{
		HtmlTableCell i_NewCell = i_NewRow.InsertCell(0);
	}
	return i_NewRow;
}

// zero based Index
BOOL 
HtmlTable::InsertColumn(UINT Index)
{
	BOOL b_Ret = TRUE;
	for (UINT i=0; TRUE; i++)
	{
		HtmlTableRow i_Row = GetRow(i);
		if (!i_Row.Valid())
    {
      break;
    }
		HtmlTableCell i_Cell = i_Row.InsertColumn(Index);
		if (!i_Cell.Valid())
    {
      b_Ret = FALSE;
    }
	}
	return b_Ret;
}

// zero based
// This function may return an invalid element !!!! (Check Ret.Valid() !)
HtmlTableRow 
HtmlTable::GetRow(UINT Index)
{
  if(!m_Table)
  {
    return HtmlTableRow(0);
  }
	CComPtr<IHTMLElementCollection> i_Collect = 0;
	m_Table->get_rows(&i_Collect);
	if (!i_Collect)
  {
    return HtmlTableRow(0);
  }
	CComVariant v_Index((int)Index);
	CComPtr<IDispatch> D_Row = 0;
	i_Collect->item(v_Index, v_Index, &D_Row);

	CComQIPtr<IHTMLElement, &IID_IHTMLElement> i_Row = D_Row;
	return HtmlTableRow((CComPtr<IHTMLElement>)i_Row);
}

UINT 
HtmlTable::GetRowCount()
{
	CComPtr<IHTMLElementCollection> i_Collect = 0;
  if(!m_Table.p)
  {
    return 0;
  }
	m_Table->get_rows(&i_Collect);
	if (!i_Collect)
  {
    return 0;
  }
	long Count = 0;
	i_Collect->get_length(&Count);
	return Count;
}

// retrieves the column count od row 0
UINT 
HtmlTable::GetColumnCount()
{
	HtmlTableRow i_First = GetRow(0);
	if (!i_First.Valid())
	{
    return 0;
  }
	UINT i = 0, Cols = 0;
	while (true)
	{
		HtmlTableCell i_Cell = i_First.GetCell(i++);
		if (!i_Cell.Valid())
			break;
		Cols += i_Cell.GetColSpan();
	}
	return Cols;
}

// returns the cell at the given row and column index
// If there is no cell at the given column index because the left neighbour cell spans over
// into this column and b_ReturnPrevious=FALSE, an invalid cell will be returned! 
// If b_ReturnPrevious=TRUE the previous cell (left neighbour) will be returned in this case
// (Check Ret.Valid() !)
HtmlTableCell 
HtmlTable::GetCell(UINT u32_Row, UINT u32_Column, BOOL b_ReturnPrevious) // =FALSE
{
	HtmlTableRow i_Row = GetRow(u32_Row);
	if (!i_Row.Valid())
  {
    return HtmlTableCell(0);
  }
	return i_Row.GetColumn(u32_Column, b_ReturnPrevious);
}

// returns a map with the colspans of each cell
// pu32_Span[Row][Col] will receive the colspan for each cell
// <TR><TD colspan=2>..</TD><TD>..</TD></TR> will result in:
// pu32_Map[Row][0] = 2  // cell 0 spans over to next cell
// pu32_Map[Row][1] = 0  // cell 1 does not exist
// pu32_Map[Row][2] = 1
// The caller must delete the returned pointer pu32_Map !!
BOOL 
HtmlTable::GetSpanMap(UINT *pu32_Rows, UINT *pu32_Cols, UINT **ppu32_Map)
{
	UINT u32_RowCount = GetRowCount();
	UINT u32_ColCount = GetColumnCount(); // retrieves column count of row 0
	if (!u32_RowCount || !u32_ColCount)
		return FALSE;

	UINT Size = u32_RowCount * u32_ColCount;
	UINT  *pu32_Span = new UINT[Size];
	memset(pu32_Span, 0, Size *4);

	for (UINT R=0; R<u32_RowCount; R++)
	{
		HtmlTableRow i_Row = GetRow(R);
		
		UINT C = 0;
		for (UINT i=0; TRUE; i++)
		{
			HtmlTableCell i_Cell = i_Row.GetCell(i);
			if (!i_Cell.Valid())
				break;

// 			if (i_Cell.GetRowSpan() > 1)
// 			{
//         theApp.MessageBox("Cells which span over multiple rows are NOT supported !", "Error", MB_OK|MB_ICONSTOP);
// 				goto _Error;
// 			}
			UINT u32_Span = i_Cell.GetColSpan();

			pu32_Span[R *u32_ColCount +C] = u32_Span;

			C += u32_Span;
		}

// 		if (C != u32_ColCount) // All rows must contain the same amount of columns as Row 0!
// 		{
//       theApp.MessageBox("All rows must contain the same amount of columns!","Error", MB_OK|MB_ICONSTOP);
// 			goto _Error;
// 		}
	}

	*pu32_Rows =  u32_RowCount;
	*pu32_Cols =  u32_ColCount;
	*ppu32_Map = pu32_Span;
	return TRUE;

// 	_Error:
// 	delete pu32_Span;
// 	return FALSE;
}

// Checks if the cells of a column have a colspan > 1 in EVERY row. 
// If so, the colspan is decreased as much as possible in every row.
// <Table>
// <TR><td>..</td><td colspan=3>..</td></TR>
// <TR><td>..</td><td colspan=3>..</td></TR>
// </Table>
// In this table CleanUp() would remove the useless "colspan=3"
void 
HtmlTable::CleanUp()
{
	UINT u32_Rows, u32_Cols, *pu32_Map;
	if (!GetSpanMap(&u32_Rows, &u32_Cols, &pu32_Map))
  {
    return;
  }
	// IMPORTANT: start with the LAST column !!
	for (int C=u32_Cols-1; C>=0; C--)
	{
		// Check if the whole column can be deleted (all map entries of all rows == 0)
		BOOL b_Empty = TRUE;
		for (UINT R=0; R<u32_Rows; R++)
		{
			if (pu32_Map[R * u32_Cols + C] > 0)
			{
				b_Empty = FALSE;
				break;
			}
		}
		if (b_Empty) DeleteColumn(C);
	}
	delete pu32_Map;
}

BOOL
HtmlTable::AlterColumnWidth(UINT Index,int delta_width,int total)
{
	BOOL b_Ret = TRUE;

  // Check not last column
  UINT cols = GetColumnCount();
  if(Index >= (cols -1))
  {
    // Cannot set column width on last column
    // This column's width should be set by the table width
    return FALSE;
  }
  // Set cell widths on all rows
	for (UINT i=0; TRUE; i++)
	{
		HtmlTableRow i_Row = GetRow(i);
		if (!i_Row.Valid())
    {
      break;
    }
		if(i_Row.AlterColumnWidth(Index,delta_width,total) == FALSE)
    {
      b_Ret = FALSE;
    }
	}
	return b_Ret;
}

HtmlCaption   
HtmlTable::GetCaption(bool p_make)
{
  if(Valid())
  {
    CComPtr<IHTMLTableCaption> caption;
    m_Table->get_caption(&caption);
    if(caption.p)
    {
      // Already existing caption
      CComQIPtr<IHTMLElement,&IID_IHTMLElement> elem = caption;
      return HtmlCaption(elem);
    }
    if(p_make)
    {
      HRESULT hr = m_Table->createCaption(&caption);
      if(SUCCEEDED(hr))
      {
        CComQIPtr<IHTMLElement,&IID_IHTMLElement> elem = caption;
        return HtmlCaption(elem);
      }
    }
  }
  // No caption in document on this table
  return HtmlCaption(NULL);
}

HtmlTableSection
HtmlTable::GetHeader(bool p_make)
{
  if(Valid())
  {
    CComPtr<IHTMLTableSection> head;
    m_Table->get_tHead(&head);
    if(head.p)
    {
      CComQIPtr<IHTMLElement,&IID_IHTMLElement> elem = head;
      return HtmlTableSection(elem);
    }
    if(p_make)
    {
      IDispatch* disp;
      HRESULT hr = m_Table->createTHead(&disp);
      if(SUCCEEDED(hr))
      {
        CComQIPtr<IHTMLElement,&IID_IHTMLElement> thead = disp;
        return HtmlTableSection(thead);
      }
    }
  }
  // No Header in document
  return HtmlTableSection(0);
}

HtmlTableSection
HtmlTable::GetFooter(bool p_make)
{
  if(Valid())
  {
    CComPtr<IHTMLTableSection> head;
    m_Table->get_tFoot(&head);
    if(head.p)
    {
      CComQIPtr<IHTMLElement,&IID_IHTMLElement> elem = head;
      return HtmlTableSection(elem);
    }
    if(p_make)
    {
      IDispatch* disp;
      HRESULT hr = m_Table->createTFoot(&disp);
      if(SUCCEEDED(hr))
      {
        CComQIPtr<IHTMLElement,&IID_IHTMLElement> thead = disp;
        return HtmlTableSection(thead);
      }
    }
  }
  // No footer in document
  return HtmlTableSection(NULL);
}

BOOL
HtmlTable::DeleteCaption()
{
  if(Valid())
  {
    HRESULT hr = m_Table->deleteCaption();
    return SUCCEEDED(hr);
  }
  return TRUE;
}

BOOL
HtmlTable::DeleteHeader()
{
  if(Valid())
  {
    HRESULT hr = m_Table->deleteTHead();
    return SUCCEEDED(hr);
  }
  return TRUE;
}

BOOL
HtmlTable::DeleteFooter()
{
  if(Valid())
  {
    HRESULT hr = m_Table->deleteTFoot();
    return SUCCEEDED(hr);
  }
  return TRUE;
}

