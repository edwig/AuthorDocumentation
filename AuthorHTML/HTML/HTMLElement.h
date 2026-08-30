//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    HTMLElement.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Base class for all HTML elements
//
#pragma once
#include <atlbase.h>    // CComPtr
#include <afxhtml.h>    // CHtmlEditView
#include "HTMLDomNode.h"
#include "HTMLStyle.h"

//class HtmlStyle;

typedef enum _SearchType
{
  INTERPOLATED   = 0
 ,CASE_SENSITIVE = 1
 ,EXACT_VALUE    = 2
}
SearchType;

typedef enum _SetType
{
  ALWAYS      = 0   // Always set the attribute
 ,EMPTYREMOVE = 1   // Empty attribute should be removed (BOOLEANS!)
}
SetType;

class HtmlElement : public HtmlDomNode
{
public:
	HtmlElement(CComPtr<IHTMLElement> i_Elem);
	const HtmlElement& operator=(const HtmlElement& El);
	operator CComPtr<IHTMLElement>();

	virtual BOOL  Valid();
  BOOL          HasStyle();
  void          SetStyle();
  BOOL          HasEvents();
  bool          HasIdentity();
  CString       GetInlineStyle();
  void          SetInlineStyle(CString& text);
	CString       GetTagName();
	CString       GetClassName();
  BOOL          SetClassName(CString s_class);
  CString       GetID();
  BOOL          SetID(CString& p_id);
	HtmlElement   GetParent();
	UINT          GetChildCollection(CComQIPtr<IHTMLElementCollection> &i_Collect);
	HtmlElement   FindParent(CString s_Tag);
	HtmlElement   SkipParents(CString s_Tags);
	CString       GetAttribute(CString s_AttrName,SearchType search = INTERPOLATED);
	BOOL          SetAttribute(CString s_AttrName, CString s_Value,SetType setting = ALWAYS);
	BOOL          RemoveAttribute(CString s_AttrName);
	CString       GetInnerText();
	BOOL          IsEmpty();
	CString       GetInnerHtml();
  BOOL          SetInnerHtml(CString s_Html);
	CString       GetOuterHtml();
  BOOL          SetOuterHtml(CString s_Html);
	BOOL          InsertHtml(CString s_Html, BOOL b_AtBegin, BOOL b_Inside);
	//HtmlStyle GetStyle();
	HtmlElement   AppendNewChild(CString s_Tag);
	static HtmlElement GetElementFromCollection(UINT u32_Index, CComQIPtr<IHTMLElementCollection> &i_Collect);

protected:
	CComQIPtr<IHTMLElement, &IID_IHTMLElement> m_Elem;
  HtmlStyle     m_Style;
};

