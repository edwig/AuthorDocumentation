// HTMLDomNode
#include <StdAfx.h>
#include "HTMLDomNode.h"
#include "HTmlElement.h"

// Constructor
HtmlDomNode::HtmlDomNode(CComPtr<IHTMLDOMNode> i_Dom)
{
	m_Dom = i_Dom;
}

BOOL 
HtmlDomNode::Valid()
{
	return (m_Dom != NULL);
}

// Removes this element and all it's children from the document
// See also HtmlDomNode::Strip() and cMisc::RemoveTag()
BOOL HtmlDomNode::Remove()
{
	CComPtr<IHTMLDOMNode> i_OldNode;
	VARIANT_BOOL v_RemChilds(true);
	return SUCCEEDED(m_Dom->removeNode(v_RemChilds, &i_OldNode));
}

// While HtmlDomNode::Remove() completely removes a tag with all its content and children,
// this function just strips this element but does not remove its children
// E.g.  "<BIG><b>Hello World</b></BIG>"  ---> strip BIG tag --> "<b>Hello World</b>"
BOOL HtmlDomNode::Strip()
{
	CComPtr<IHTMLDOMNode> i_OldNode;
	VARIANT_BOOL v_RemChilds(false);
	return SUCCEEDED(m_Dom->removeNode(v_RemChilds, &i_OldNode));
}

// returns the right neighbour which is a IHTMLElement in the HTML hierarchy
HtmlElement 
HtmlDomNode::NextSibling()
{
	CComPtr<IHTMLDOMNode> i_Dom = m_Dom;
	while (TRUE)
	{
		CComPtr<IHTMLDOMNode> i_Next;
		i_Dom->get_nextSibling(&i_Next);
		if (!i_Next) // no more neighbours
    {
      break;
    }
		CComQIPtr<IHTMLElement> i_Elem = i_Next;
		if (i_Elem) // Check if i_Next is a IHTMLElement
    {
      return HtmlElement((CComPtr<IHTMLElement>) i_Elem);
    }
		i_Dom = i_Next;
	}
	return HtmlElement(0);
}

// returns the left neighbour which is a IHTMLElement in the HTML hierarchy
HtmlElement 
HtmlDomNode::PreviousSibling()
{
	CComPtr<IHTMLDOMNode> i_Dom = m_Dom;
	while (TRUE)
	{
		CComPtr<IHTMLDOMNode> i_Prev;
		i_Dom->get_previousSibling(&i_Prev);
		if (!i_Prev) // no more neighbours
    {
      break;
    }
		CComQIPtr<IHTMLElement> i_Elem = i_Prev;
		if (i_Elem) // Check if i_Prev is a IHTMLElement
    {
      return HtmlElement((CComPtr<IHTMLElement>)i_Elem);
    }
		i_Dom = i_Prev;
	}
	return HtmlElement(0);
}

// Appends an already existing Element to the child list of this node
// returns the newly added Element
HtmlElement 
HtmlDomNode::AppendChild(HtmlElement i_NewChild)
{
	CComPtr<IHTMLDOMNode> i_NewNode;
	m_Dom->appendChild(i_NewChild.m_Dom, &i_NewNode);

	// cast Domnode back to HtmlElement
	CComQIPtr<IHTMLElement, &IID_IHTMLElement>  i_NewElem = i_NewNode;
	return HtmlElement((CComPtr<IHTMLElement>) i_NewElem);
}

HtmlElement
HtmlDomNode::FirstChild()
{
  CComPtr<IHTMLDOMNode> i_NewNode;
  m_Dom->get_firstChild(&i_NewNode);
  // cast Domnode back to HtmlElement
  CComQIPtr<IHTMLElement, &IID_IHTMLElement> i_NewElem = i_NewNode;
  return HtmlElement((CComPtr<IHTMLElement>) i_NewElem);
}

