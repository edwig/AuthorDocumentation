//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    HTMLDomNode.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Handles the a general NODE in the HTML DOM (Document-Object-Model)
//
#pragma once
#include <atlbase.h>    // CComPtr
#include <afxhtml.h>    // CHtmlEditView
#include "misc.h"

class HtmlElement;

class HtmlDomNode 
{
public:
	HtmlDomNode(CComPtr<IHTMLDOMNode> i_Dom);

	virtual BOOL Valid();
	HtmlElement  NextSibling();
	HtmlElement  PreviousSibling();
  HtmlElement  FirstChild();

	BOOL         Remove();
	BOOL         Strip();
	HtmlElement  AppendChild(HtmlElement i_NewChild);

protected:
	CComQIPtr<IHTMLDOMNode, &IID_IHTMLDOMNode> m_Dom;
};

