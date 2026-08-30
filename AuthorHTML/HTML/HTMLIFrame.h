//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    HTMLIFrame.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Handles the <IFRAME> tag of the HTML
//
#pragma once
#include <atlbase.h>
#include <afxhtml.h>
#include "HTMLElement.h"

class HtmlIFrame : public HtmlElement
{
public:
  HtmlIFrame(CComPtr<IHTMLElement> i_Elem);

  enum eAction
  {
    // Properties alsoo in CSS
    E_Align,        // Vertical-align / Text-align
    E_Width,        // Width  in pixels
    E_Height,       // Height in pixels
    E_HSpace,       // Horizontal margin  (margin-left  / margin-right)
    E_VSpace,       // Vertical   margin  (margin-top   / margin-bottom)
    E_MarginHeight, // Vertical   padding (padding-top  / padding-bottom)
    E_MarginWidth,  // Horizontal padding (padding-left / padding-right)

    // Properties only for IFrame
    E_FrameBorder,  // Border in pixels
    E_Name,         // Name of the frame
    E_NoResize,     // Resizing the frame is not posible for reader
    E_Scrolling,    // Scrolling in the frame
    E_Src           // Source URL of the frame
  };
  virtual BOOL Valid();
  void    SetProperty(eAction e_Action, CString s_Value);
  CString GetProperty(eAction e_Action);

protected:
  CComQIPtr<IHTMLIFrameElement, &IID_IHTMLIFrameElement> mi_IFrame;
};
