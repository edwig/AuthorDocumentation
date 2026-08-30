//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    HTMLImage.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Handles the <IMG> tag of the HTML
//
#pragma once
#include <atlbase.h>
#include <afxhtml.h>
#include "HTMLElement.h"

class HtmlImg : public HtmlElement
{
public:
	HtmlImg(CComPtr<IHTMLElement> i_Elem);

  enum eAction
  {
    E_Border,  // Border in pixels
    E_Width,   // Width  in pixels
    E_Height,  // Height in pixels
    E_HorPad,  // Horizontal padding
    E_VerPad,  // Vertical   padding
    E_Align,   // Vertical-align

  };
  virtual BOOL Valid();
  void    SetProperty(eAction e_Action, CString s_Value);
  CString GetProperty(eAction e_Action);
  
  CString GetSrc();
	BOOL    SetSrc(CString s_Src);
  CString GetLowSrc();
  BOOL    SetLowSrc(CString s_Src);
  CString GetDynSrc();
  BOOL    SetDynSrc(CString s_Src);
  CString GetAlt();
  BOOL    SetAlt(CString s_ALt);
  CString GetStart();
  BOOL    SetStart(CString s_ALt);
  CString GetLoop();
  BOOL    SetLoop(CString s_ALt);

protected:
	CComQIPtr<IHTMLImgElement, &IID_IHTMLImgElement> mi_Img;
};
