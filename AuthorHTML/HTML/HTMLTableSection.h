// HtmlTableSection.h
//
#pragma once
#include <atlbase.h>
#include <afxhtml.h>
#include "HTMLElement.h"
#include "HTMLTableRow.h"

class HtmlTableSection : public HtmlElement
{
public:
  HtmlTableSection(CComPtr<IHTMLElement> i_Elem);

  enum eAction
  {
    E_Align,  // Horizontal text-align
    E_BgColor,
    E_Valign
  };
  virtual BOOL  Valid();
  void          SetProperty(eAction e_Action, CString s_Value);
  CString       GetProperty(eAction e_Action);
  HtmlTableRow  InsertRow(int Index, int CellCount);
  UINT          GetRows();
protected:
  CComQIPtr<IHTMLTableSection, &IID_IHTMLTableSection> mi_section;
};
