//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    HTMLHR.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Handles the <HR> (horizontal ruler) tag of the HTML
//
#pragma once
#include <atlbase.h>
#include <afxhtml.h>
#include "HTMLElement.h"

class HtmlHR : public HtmlElement
{
public:
	HtmlHR(CComPtr<IHTMLElement> i_Elem);

	enum eAction
	{
		E_Color,  // foreground color of ruler
		E_Size,   // the height of the line in pixel
		E_Width,  // the length of the line in pixel or percent
		E_Align   // left,center,right alignment
	};
	virtual BOOL Valid();
	void    SetProperty(eAction e_Action, CString s_Value);
	CString GetProperty(eAction e_Action);
	BOOL    GetShade();
	void    SetShade(BOOL b_Shade);

protected:
	CComQIPtr<IHTMLHRElement, &IID_IHTMLHRElement> mi_HR;
};

