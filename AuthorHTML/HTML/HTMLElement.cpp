//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    HTMLElement.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Base class for all HTML elements
//
#include <stdafx.h>
#include "HTMLElement.h"

// Constructor
HtmlElement::HtmlElement(CComPtr<IHTMLElement> i_Elem) 
            :HtmlDomNode(0)
{
	m_Elem  = i_Elem;
	m_Dom   = i_Elem;
  if(m_Elem != NULL)
  {
    CComBSTR bText;
    m_Elem->get_outerHTML(&bText);
    CString cText = CW2CT(bText);
    int pos = cText.Find(">");
    if(pos >= 0)
    {
      cText = cText.Left(pos);
      if(cText.Find("style=") >= 0)
      {
        CComPtr<IHTMLStyle> style;
        i_Elem->get_style(&style);
        m_Style.SetStyle(style);
      }
    }
  }
}

const HtmlElement& 
HtmlElement::operator=(const HtmlElement& El)
{
	m_Elem = El.m_Elem;
	m_Dom  = El.m_Dom;
	return *this;
}

BOOL 
HtmlElement::Valid()
{
	return (m_Elem != NULL);
}

BOOL
HtmlElement::HasStyle()
{
  return m_Style.Valid();
}

// Now definitly create a inline-style
void
HtmlElement::SetStyle()
{
  CComPtr<IHTMLStyle> style;
  m_Elem->get_style(&style);
  m_Style.SetStyle(style);
}

CString
HtmlElement::GetInlineStyle()
{
  return m_Style.GetStylesheetText();
}

void
HtmlElement::SetInlineStyle(CString& text)
{
  m_Style.SetStylesheetText(text);
}

// This is needed to be able to easily assign derived classes of HtmlElement to each other
// Example: 
// HtmlElement i_Elem  = i_Doc.GetElementByID("Id1");
// HtmlTable   i_Table = i_Elem;
HtmlElement::operator CComPtr<IHTMLElement>()
{
	return m_Elem;
}

// returns the Html uppercase tag (e.g. "TABLE", "TR", "BODY", "IMG" etc..)
CString 
HtmlElement::GetTagName()
{
	CComBSTR bs_TagName;
	m_Elem->get_tagName(&bs_TagName);
	return CString(bs_TagName);
}

// returns the class name in <Span class="FatText"> (if any)
CString 
HtmlElement::GetClassName()
{
	CComBSTR bs_Class;
	m_Elem->get_className(&bs_Class);
	return CString(bs_Class);
}

BOOL
HtmlElement::SetClassName(CString s_class)
{
  CComBSTR bs_Class = s_class;
  return SUCCEEDED(m_Elem->put_className(bs_Class));
}

CString
HtmlElement::GetID()
{
  CComBSTR id;
  m_Elem->get_id(&id);
  return CString(id);
}

BOOL
HtmlElement::SetID(CString& p_id)
{
  CComBSTR id = p_id;
  return SUCCEEDED(m_Elem->put_id(id));
}

bool
HtmlElement::HasIdentity()
{
  if(Valid())
  {
    CComBSTR id;
    CComBSTR className;

    m_Elem->get_id(&id);
    m_Elem->get_className(&className);
    if(id.Length() || className.Length())
    {
      return true;
    }
  }
  return false;
}

// returns an attribute of an Html tag
// e.g. for <img src="xyz" hspace=3 onmouseover="ShowInfo('Image');"> 
// GetAttribute("hspace")      will return "3"
// GetAttribute("onmouseover") will return "ShowInfo('Image');"
// If you want to retrieve the class attribute, set s_AttrName = "className" !!

// Use SearchType for different types of search
// INTERPOLATED    Returns full path name for "src" eventhough it's a relative path in HTML
// CASE_SENSITIVE  Finds attributes by case-sensitive search if duplicated (ClassName versus className)
// EXACT_VALUE     Returns exact value from HTML, e.g. Relative pathnames for "src" and "href"

CString 
HtmlElement::GetAttribute(CString s_AttrName,SearchType search /* = INTERPOLATED */)
{
  if(!Valid())
  {
    return "";
  }
	CComVariant v_AttrValue;
	CComBSTR   bs_AttrName = s_AttrName;
	m_Elem->getAttribute(bs_AttrName, search, &v_AttrValue);
  CString value =  Misc::VarToStr(v_AttrValue);
  if(search == EXACT_VALUE)
  {
    // To resolve issues as %5C in pathnames
    Misc::ResolveSpecialChars(value);
  }
  return value;
}

// see above
BOOL 
HtmlElement::SetAttribute(CString s_AttrName, CString s_Value,SetType setting)
{
  if(s_Value.IsEmpty() || (setting == EMPTYREMOVE && (s_Value == "0")))
  {
    return RemoveAttribute(s_AttrName);
  }
	CComVariant v_AttrValue(s_Value);
	CComBSTR   bs_AttrName = s_AttrName;
	return SUCCEEDED(m_Elem->setAttribute(bs_AttrName, v_AttrValue, 0));
}

// removes the given attribute from the element if it exists (case-insesitive)
BOOL 
HtmlElement::RemoveAttribute(CString s_AttrName)
{
	VARIANT_BOOL vb_Success;
	CComBSTR bs_AttrName = s_AttrName;
	m_Elem->removeAttribute(bs_AttrName, 0, &vb_Success);
	return (vb_Success == VARIANT_TRUE);
}

// For a <Span class="xyz">Hello <b>Test</b></Span> this function 
// will return            "Hello Test"
// Unicode characters are converted to &ABCD; in the MBCS version
CString 
HtmlElement::GetInnerText()
{
 	CComBSTR bs_Text;
	m_Elem->get_innerText(&bs_Text);
  return Misc::EncodeHtml(bs_Text);
}

// return TRUE if the element is empty inside (e.g. <a name="#Pos1"></a>)
BOOL 
HtmlElement::IsEmpty()
{
	// Do NOT use innerText here !
	CComBSTR bs_Html;
	m_Elem->get_innerHTML(&bs_Html);
	CString s_Html(bs_Html);

	s_Html.TrimLeft (_T("\r\n \t"));
	s_Html.TrimRight(_T("\r\n \t"));
	return (!s_Html.GetLength());
}

// For a <Span class="xyz">Hello <b>Test</b></Span> this function 
// will return            "Hello <b>Test</b>"
CString 
HtmlElement::GetInnerHtml()
{
	CComBSTR bs_Html;
	m_Elem->get_innerHTML(&bs_Html);
  return Misc::EncodeHtml(bs_Html);
}

BOOL 
HtmlElement::SetInnerHtml(CString s_Html)
{
	CComBSTR bs_Html = s_Html;
	return SUCCEEDED(m_Elem->put_innerHTML(bs_Html));
}

// For a        <Span class="xyz">Hello <b>Test</b></Span> this function 
// will return "<Span class="xyz">Hello <b>Test</b></Span>"
CString 
HtmlElement::GetOuterHtml()
{
	CComBSTR bs_Html;
	m_Elem->get_outerHTML(&bs_Html);
  return Misc::EncodeHtml(bs_Html);
}

// Wil never succeed for the following Element tags
// caption, col, colgroup, html, head, body, frameset, tbody, td, tfoot, th, thead, tr.
BOOL
HtmlElement::SetOuterHtml(CString s_Html)
{
  CComBSTR bs_Html = s_Html;
  return SUCCEEDED(m_Elem->put_outerHTML(bs_Html));
}

HtmlElement 
HtmlElement::GetParent()
{
	CComPtr<IHTMLElement> i_Parent = 0;
	m_Elem->get_parentElement(&i_Parent);
	return HtmlElement(i_Parent);
}

// retrieves a collection of Html Elements which are childs of this element
// returns the count of childs
// Use HtmlElement::GetElementFromCollection() to retrieve a single child
UINT 
HtmlElement::GetChildCollection(CComQIPtr<IHTMLElementCollection> &i_Collect) // OUT
{
	CComPtr<IDispatch> D_Collect = 0;
	m_Elem->get_children(&D_Collect);

	i_Collect = D_Collect;

	long u32_Len;
	i_Collect->get_length(&u32_Len);
	return u32_Len;
}

// Static function !
// retrieves the n-th element of the given collection
// This function may return an invalid element !!!! (Check Ret.Valid() !)
HtmlElement 
HtmlElement::GetElementFromCollection(UINT    u32_Index,   // IN
                                      CComQIPtr<IHTMLElementCollection> &i_Collect) // IN
{
	CComVariant v_Index((int)u32_Index);

	// get the element with the requested index
	CComPtr<IDispatch> D_Elem;
	i_Collect->item(v_Index, v_Index, &D_Elem);

	CComQIPtr<IHTMLElement, &IID_IHTMLElement> i_Elem = D_Elem;
	
	HtmlElement i_HtmlElem = HtmlElement((CComPtr<IHTMLElement>)i_Elem);
	return i_HtmlElem;
}

// Searches for a parent (or grandparent) element with the given tag
// if "this" has already the requested tag -> return "this"
// This function may return an invalid element !!!! (Check Ret.Valid() !)
// s_Tag must be in the form "TD" (uppercase)
// see also SkipParents()
HtmlElement 
HtmlElement::FindParent(CString s_Tag)
{
	HtmlElement i_Elem = *this;

	while (i_Elem.Valid() && i_Elem.GetTagName() != s_Tag)
	{
		i_Elem = i_Elem.GetParent();
		if (!i_Elem.Valid())
    {
      break;
    }
	}
	return i_Elem;
}

// This function is similar to FindParent() but it does the reverse:
// While FindParent searches for an element whose name is s_Tag
// this function searches for an element whose name is NOT in s_Tags
// s_Tags must be in the form "<B><EM><U>" (uppercase)
HtmlElement 
HtmlElement::SkipParents(CString s_Tags)
{
	HtmlElement i_Elem = *this;
	CString s_Tag;
	
	while (i_Elem.Valid())
	{
		s_Tag.Format(_T("<%s>"), i_Elem.GetTagName().GetString());

		if (s_Tags.Find(s_Tag) >= 0)
		{
			i_Elem = i_Elem.GetParent();
			continue;
		}

		break;
	}
	return i_Elem;
}

// Inserts the given HTML code inside or outside of this Html element
// There are 4 possible insert positions:
// Outside-Before<TAG>Inside-Before InnerHTML Inside-After</TAG>Ouside-After
BOOL 
HtmlElement::InsertHtml(CString s_Html, BOOL b_AtBegin, BOOL b_Inside)
{
	CComBSTR bs_Where;
	if (b_Inside)
	{
		if (b_AtBegin) bs_Where = L"afterBegin";
		else           bs_Where = L"beforeEnd";
	}
	else // Outside
	{
		if (b_AtBegin) bs_Where = L"beforeBegin";
		else           bs_Where = L"afterEnd";
	}

	CComBSTR bs_Html = s_Html;
	return SUCCEEDED(m_Elem->insertAdjacentHTML(bs_Where, bs_Html));
}

// creates a new HtmlElement and appends it to the childs of this element
// returns the newly created element
HtmlElement 
HtmlElement::AppendNewChild(CString s_Tag)
{
	// get the parent document
	CComPtr<IDispatch> D_Doc;
	m_Elem->get_document(&D_Doc);
	CComQIPtr<IHTMLDocument2, &IID_IHTMLDocument2> i_Doc = D_Doc;

	CComBSTR bs_Tag = s_Tag;
	CComPtr<IHTMLElement> i_Elem;
	i_Doc->createElement(bs_Tag, &i_Elem);

	return AppendChild(HtmlElement(i_Elem));
}

BOOL
HtmlElement::HasEvents()
{
  BOOL has = FALSE;
  if(Valid())
  {
    CComBSTR bText;
    m_Elem->get_outerHTML(&bText);
    CString cText = CW2CT(bText);
    int pos = cText.Find('>');
    if(pos > 0)
    {
      CString tagText = cText.Left(pos);
      int eventPos = Misc::FindNoCase(tagText," on",0);
      if(eventPos > 0)
      {
        return TRUE;
      }
    }
  }
  return has;
}