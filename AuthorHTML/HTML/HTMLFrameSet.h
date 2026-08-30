// HtmlBody.h
//
#pragma once
#include <atlbase.h>
#include <afxhtml.h>
#include "HTMLElement.h"

class HtmlFrameSet: public HtmlElement
{
public:
  HtmlFrameSet(CComPtr<IHTMLElement> i_Elem);

  enum eAction
  {
    E_Border,       // Border thickness
    E_BorderColor,  // Color of the border
    E_FrameBorder,  // Has/has no frame border
    E_FrameSpacing, // Number of pixels between frames
  };
  virtual BOOL Valid();
  void    SetProperty(eAction e_Action, CString s_Value);
  CString GetProperty(eAction e_Action);

  void    SetCols(CString& p_cols);
  void    SetRows(CString& p_rows);
  CString GetCols();
  CString GetRows();

protected:
  CComQIPtr<IHTMLFrameSetElement, &IID_IHTMLFrameSetElement> mi_frameset;
};
