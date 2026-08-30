//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    HTMLStyle.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Handles the various style attributes of a tag in the HTML
//
#include <StdAfx.h>
#include "HTmlStyle.h"

// ###########################  STYLE  #################################
// This class may contain a complete style definition like
// { FONT-SIZE: 17px; FONT-FAMILY: Arial; color:red; }

// Constructor1  takes IHTMLStyle
HtmlStyle::HtmlStyle(CComPtr<IHTMLStyle> i_Style)
{
	mi_Style = i_Style;
}

// Constructor3 NULL, Call SetStyle !!!
HtmlStyle::HtmlStyle()
{
  mi_Style     = NULL;
}

void
HtmlStyle::SetStyle(CComPtr<IHTMLStyle> i_Style)
{
  mi_Style = i_Style;
}

BOOL 
HtmlStyle::Valid()
{
	return (mi_Style != NULL);
}

CString
HtmlStyle::GetStylesheetText()
{
  CComBSTR bText;
  CString  text;
  if(mi_Style != NULL)
  {
    mi_Style->get_cssText(&bText);
    text = CW2CT(bText);
  }
  return text;
}

void    
HtmlStyle::SetStylesheetText(CString& text)
{
  CComBSTR bText = CT2CW(text);
  if(mi_Style != NULL)
  {
    mi_Style->put_cssText(bText);
  }
}

// Gets one of the uncountable properties in a Style attribute
// If this class is invalid (mi_Style = 0) an empty string is returned
CString 
HtmlStyle::GetProperty(eProp e_Prop)
{
	CComVariant v_Value;
	v_Value.vt      = VT_BSTR; // to be overridden by the called function below
	v_Value.bstrVal = 0;       // to be overridden by the called function below

	if (mi_Style != NULL)
	{
		switch (e_Prop)
		{
      case P_Background:    mi_Style->get_background          (&v_Value.bstrVal); break;
      case P_BackAttach:    mi_Style->get_backgroundAttachment(&v_Value.bstrVal); break;
		  case P_BackgColor:    mi_Style->get_backgroundColor     (&v_Value);         break;
      case P_BackgImage:    mi_Style->get_backgroundImage     (&v_Value.bstrVal); break;
      case P_BackgPosition: mi_Style->get_backgroundPosition  (&v_Value.bstrVal); break;
      case P_BackgRepeat:   mi_Style->get_backgroundRepeat    (&v_Value.bstrVal); break;
      case P_Border:        mi_Style->get_border              (&v_Value.bstrVal); break;
      case P_BorderBottom:  mi_Style->get_borderBottom        (&v_Value.bstrVal); break;
      case P_BorderBotColor:mi_Style->get_borderBottomColor   (&v_Value);         break;
      case P_BorderBotStyle:mi_Style->get_borderBottomStyle   (&v_Value.bstrVal); break;
      case P_BorderBotWidth:mi_Style->get_borderBottomWidth   (&v_Value);         break;
		  case P_BorderColor:   mi_Style->get_borderColor         (&v_Value.bstrVal); break;
      case P_BorderLeft:    mi_Style->get_borderLeft          (&v_Value.bstrVal); break;
      case P_BorderLftColor:mi_Style->get_borderLeftColor     (&v_Value);         break;
      case P_BorderLftStyle:mi_Style->get_borderLeftStyle     (&v_Value.bstrVal); break;
      case P_BorderLftWidth:mi_Style->get_borderLeftWidth     (&v_Value);         break;
      case P_BorderRight:   mi_Style->get_borderRight         (&v_Value.bstrVal); break;
      case P_BorderRigColor:mi_Style->get_borderRightColor    (&v_Value);         break;
      case P_BorderRigStyle:mi_Style->get_borderRightStyle    (&v_Value.bstrVal); break;
      case P_BorderRigWidth:mi_Style->get_borderRightWidth    (&v_Value);         break;
		  case P_BorderStyle:   mi_Style->get_borderStyle         (&v_Value.bstrVal); break;
      case P_BorderTop:     mi_Style->get_borderTop           (&v_Value.bstrVal); break;
      case P_BorderTopColor:mi_Style->get_borderTopColor      (&v_Value);         break;
      case P_BorderTopStyle:mi_Style->get_borderTopStyle      (&v_Value.bstrVal); break;
      case P_BorderTopWidth:mi_Style->get_borderTopWidth      (&v_Value);         break;
		  case P_BorderWidth:   mi_Style->get_borderWidth         (&v_Value.bstrVal); break;
      case P_Clear:         mi_Style->get_clear               (&v_Value.bstrVal); break;
      case P_Clip:          mi_Style->get_clip                (&v_Value.bstrVal); break;
		  case P_Color:         mi_Style->get_color               (&v_Value);         break;
		  case P_Cursor:        mi_Style->get_cursor              (&v_Value.bstrVal); break;
		  case P_Display:       mi_Style->get_display             (&v_Value.bstrVal); break;
		  case P_Float:         mi_Style->get_styleFloat          (&v_Value.bstrVal); break;
		  case P_Font:          mi_Style->get_font                (&v_Value.bstrVal); break;
      case P_FontFamily:    mi_Style->get_fontFamily          (&v_Value.bstrVal); break;
		  case P_FontSize:      mi_Style->get_fontSize            (&v_Value);         break;
      case P_FontStyle:     mi_Style->get_fontStyle           (&v_Value.bstrVal); break;
      case P_FontVariant:   mi_Style->get_fontVariant         (&v_Value.bstrVal); break;
      case P_FontWeight:    mi_Style->get_fontWeight          (&v_Value.bstrVal); break;
      case P_Height:        mi_Style->get_height              (&v_Value);         break;
      case P_Left:          mi_Style->get_left                (&v_Value);         break;
      case P_LetterSpacing: mi_Style->get_letterSpacing       (&v_Value);         break;
      case P_LineHeight:    mi_Style->get_lineHeight          (&v_Value);         break;
      case P_ListStyle:     mi_Style->get_listStyle           (&v_Value.bstrVal); break;
      case P_ListStyleImage:mi_Style->get_listStyleImage      (&v_Value.bstrVal); break;
      case P_ListStylePos:  mi_Style->get_listStylePosition   (&v_Value.bstrVal); break;
      case P_ListStyleType: mi_Style->get_listStyleType       (&v_Value.bstrVal); break;
      case P_Margin:        mi_Style->get_margin              (&v_Value.bstrVal); break;
      case P_MarginBottom:  mi_Style->get_marginBottom        (&v_Value);         break;
      case P_MarginLeft:    mi_Style->get_marginLeft          (&v_Value);         break;
      case P_MarginRight:   mi_Style->get_marginRight         (&v_Value);         break;
      case P_MarginTop:     mi_Style->get_marginTop           (&v_Value);         break;
      case P_Overflow:      mi_Style->get_overflow            (&v_Value.bstrVal); break;
      case P_Padding:       mi_Style->get_padding             (&v_Value.bstrVal); break;
      case P_PaddingBottom: mi_Style->get_paddingBottom       (&v_Value);         break;
      case P_PaddingLeft:   mi_Style->get_paddingLeft         (&v_Value);         break;
      case P_PaddingRight:  mi_Style->get_paddingRight        (&v_Value);         break;
      case P_PaddingTop:    mi_Style->get_paddingTop          (&v_Value);         break;
      case P_PageBreakAfter:mi_Style->get_pageBreakAfter      (&v_Value.bstrVal); break;
      case P_PageBreakBefore:mi_Style->get_pageBreakBefore    (&v_Value.bstrVal); break;
      case P_Position:      mi_Style->get_position            (&v_Value.bstrVal); break;
      case P_TextAlign:     mi_Style->get_textAlign           (&v_Value.bstrVal); break;
      case P_TextDecoration:mi_Style->get_textDecoration      (&v_Value.bstrVal); break;
      case P_TextIndent:    mi_Style->get_textIndent          (&v_Value);         break;
      case P_TextTransform: mi_Style->get_textTransform       (&v_Value.bstrVal); break;
      case P_Top:           mi_Style->get_top                 (&v_Value);         break;
      case P_VerticalAlign: mi_Style->get_verticalAlign       (&v_Value);         break;
      case P_Visibility:    mi_Style->get_visibility          (&v_Value.bstrVal); break;
		  case P_Width:         mi_Style->get_width               (&v_Value);         break;
      case P_ZIndex:        mi_Style->get_zIndex              (&v_Value);         break;
      case P_Bottom:        {
                              CComQIPtr<IHTMLStyle2,&IID_IHTMLStyle2> style2 = mi_Style;
                              style2->get_bottom(&v_Value);
                            }
                            break;
      case P_Right:         {
                              CComQIPtr<IHTMLStyle2,&IID_IHTMLStyle2> style2 = mi_Style;
                              style2->get_right(&v_Value);
                            }
                            break;
      case P_Zoom:          {
                              CComQIPtr<IHTMLStyle3,&IID_IHTMLStyle3> style3 = mi_Style;
                              style3->get_zoom(&v_Value);
                            }
                            break;
		}
	}
	return VarToStr(v_Value);
}

// Sets one of the uncountable properties in a Style attribute
// If s_Value == "" the property is removed from the styles collection
BOOL 
HtmlStyle::SetProperty(eProp e_Prop, CString s_Value)
{
	CComVariant v_Value(s_Value);
	BOOL b_Ret = FALSE;

	if (mi_Style != NULL)
	{
		switch (e_Prop)
		{
      case P_Background:    b_Ret = SUCCEEDED(mi_Style->put_background          (v_Value.bstrVal)); break;
      case P_BackAttach:    b_Ret = SUCCEEDED(mi_Style->put_backgroundAttachment(v_Value.bstrVal)); break;
		  case P_BackgColor:    b_Ret = SUCCEEDED(mi_Style->put_backgroundColor     (v_Value));         break;
      case P_BackgImage:    b_Ret = SUCCEEDED(mi_Style->put_backgroundImage     (v_Value.bstrVal)); break;
      case P_BackgPosition: b_Ret = SUCCEEDED(mi_Style->put_backgroundPosition  (v_Value.bstrVal)); break;
      case P_BackgRepeat:   b_Ret = SUCCEEDED(mi_Style->put_backgroundRepeat    (v_Value.bstrVal)); break;
      case P_Border:        b_Ret = SUCCEEDED(mi_Style->put_border              (v_Value.bstrVal)); break;
      case P_BorderBottom:  b_Ret = SUCCEEDED(mi_Style->put_borderBottom        (v_Value.bstrVal)); break;
      case P_BorderBotColor:b_Ret = SUCCEEDED(mi_Style->put_borderBottomColor   (v_Value));         break;
      case P_BorderBotStyle:b_Ret = SUCCEEDED(mi_Style->put_borderBottomStyle   (v_Value.bstrVal)); break;
      case P_BorderBotWidth:b_Ret = SUCCEEDED(mi_Style->put_borderBottomWidth   (v_Value));         break;
		  case P_BorderColor:   b_Ret = SUCCEEDED(mi_Style->put_borderColor         (v_Value.bstrVal)); break;
      case P_BorderLeft:    b_Ret = SUCCEEDED(mi_Style->put_borderLeft          (v_Value.bstrVal)); break;
      case P_BorderLftColor:b_Ret = SUCCEEDED(mi_Style->put_borderLeftColor     (v_Value));         break;
      case P_BorderLftStyle:b_Ret = SUCCEEDED(mi_Style->put_borderLeftStyle     (v_Value.bstrVal)); break;
      case P_BorderLftWidth:b_Ret = SUCCEEDED(mi_Style->put_borderLeftWidth     (v_Value));         break;
      case P_BorderRight:   b_Ret = SUCCEEDED(mi_Style->put_borderRight         (v_Value.bstrVal)); break;
      case P_BorderRigColor:b_Ret = SUCCEEDED(mi_Style->put_borderRightColor    (v_Value));         break;
      case P_BorderRigStyle:b_Ret = SUCCEEDED(mi_Style->put_borderRightStyle    (v_Value.bstrVal)); break;
      case P_BorderRigWidth:b_Ret = SUCCEEDED(mi_Style->put_borderRightWidth    (v_Value));         break;
		  case P_BorderStyle:   b_Ret = SUCCEEDED(mi_Style->put_borderStyle         (v_Value.bstrVal)); break;
      case P_BorderTop:     b_Ret = SUCCEEDED(mi_Style->put_borderTop           (v_Value.bstrVal)); break;
      case P_BorderTopColor:b_Ret = SUCCEEDED(mi_Style->put_borderTopColor      (v_Value));         break;
      case P_BorderTopStyle:b_Ret = SUCCEEDED(mi_Style->put_borderTopStyle      (v_Value.bstrVal)); break;
      case P_BorderTopWidth:b_Ret = SUCCEEDED(mi_Style->put_borderTopWidth      (v_Value));         break;
		  case P_BorderWidth:   b_Ret = SUCCEEDED(mi_Style->put_borderWidth         (v_Value.bstrVal)); break;
      case P_Clear:         b_Ret = SUCCEEDED(mi_Style->put_clear               (v_Value.bstrVal)); break;
      case P_Clip:          b_Ret = SUCCEEDED(mi_Style->put_clip                (v_Value.bstrVal)); break;
		  case P_Color:         b_Ret = SUCCEEDED(mi_Style->put_color               (v_Value));         break;
		  case P_Cursor:        b_Ret = SUCCEEDED(mi_Style->put_cursor              (v_Value.bstrVal)); break;
		  case P_Display:       b_Ret = SUCCEEDED(mi_Style->put_display             (v_Value.bstrVal)); break;
		  case P_Float:         b_Ret = SUCCEEDED(mi_Style->put_styleFloat          (v_Value.bstrVal)); break;
		  case P_Font:          b_Ret = SUCCEEDED(mi_Style->put_font                (v_Value.bstrVal)); break;
      case P_FontFamily:    b_Ret = SUCCEEDED(mi_Style->put_fontFamily          (v_Value.bstrVal)); break;
		  case P_FontSize:      b_Ret = SUCCEEDED(mi_Style->put_fontSize            (v_Value));         break;
      case P_FontStyle:     b_Ret = SUCCEEDED(mi_Style->put_fontStyle           (v_Value.bstrVal)); break;
      case P_FontVariant:   b_Ret = SUCCEEDED(mi_Style->put_fontVariant         (v_Value.bstrVal)); break;
      case P_FontWeight:    b_Ret = SUCCEEDED(mi_Style->put_fontWeight          (v_Value.bstrVal)); break;
      case P_Height:        b_Ret = SUCCEEDED(mi_Style->put_height              (v_Value));         break;
      case P_Left:          b_Ret = SUCCEEDED(mi_Style->put_left                (v_Value));         break;
      case P_LetterSpacing: b_Ret = SUCCEEDED(mi_Style->put_letterSpacing       (v_Value));         break;
      case P_LineHeight:    b_Ret = SUCCEEDED(mi_Style->put_lineHeight          (v_Value));         break;
      case P_ListStyle:     b_Ret = SUCCEEDED(mi_Style->put_listStyle           (v_Value.bstrVal)); break;
      case P_ListStyleImage:b_Ret = SUCCEEDED(mi_Style->put_listStyleImage      (v_Value.bstrVal)); break;
      case P_ListStylePos:  b_Ret = SUCCEEDED(mi_Style->put_listStylePosition   (v_Value.bstrVal)); break;
      case P_ListStyleType: b_Ret = SUCCEEDED(mi_Style->put_listStyleType       (v_Value.bstrVal)); break;
      case P_Margin:        b_Ret = SUCCEEDED(mi_Style->put_margin              (v_Value.bstrVal)); break;
      case P_MarginBottom:  b_Ret = SUCCEEDED(mi_Style->put_marginBottom        (v_Value));         break;
      case P_MarginLeft:    b_Ret = SUCCEEDED(mi_Style->put_marginLeft          (v_Value));         break;
      case P_MarginRight:   b_Ret = SUCCEEDED(mi_Style->put_marginRight         (v_Value));         break;
      case P_MarginTop:     b_Ret = SUCCEEDED(mi_Style->put_marginTop           (v_Value));         break;
      case P_Overflow:      b_Ret = SUCCEEDED(mi_Style->put_overflow            (v_Value.bstrVal)); break;
      case P_Padding:       b_Ret = SUCCEEDED(mi_Style->put_padding             (v_Value.bstrVal)); break;
      case P_PaddingBottom: b_Ret = SUCCEEDED(mi_Style->put_paddingBottom       (v_Value));         break;
      case P_PaddingLeft:   b_Ret = SUCCEEDED(mi_Style->put_paddingLeft         (v_Value));         break;
      case P_PaddingRight:  b_Ret = SUCCEEDED(mi_Style->put_paddingRight        (v_Value));         break;
      case P_PaddingTop:    b_Ret = SUCCEEDED(mi_Style->put_paddingTop          (v_Value));         break;
      case P_PageBreakAfter:b_Ret = SUCCEEDED(mi_Style->put_pageBreakAfter      (v_Value.bstrVal)); break;
      case P_PageBreakBefore:b_Ret= SUCCEEDED(mi_Style->put_pageBreakBefore     (v_Value.bstrVal)); break;
      case P_Position:      //Not supported
                            //b_Ret = SUCCEEDED(mi_Style->put_position            (v_Value.bstrVal)); 
                            break;
      case P_TextAlign:     b_Ret = SUCCEEDED(mi_Style->put_textAlign           (v_Value.bstrVal)); break;
      case P_TextDecoration:b_Ret = SUCCEEDED(mi_Style->put_textDecoration      (v_Value.bstrVal)); break;
      case P_TextIndent:    b_Ret = SUCCEEDED(mi_Style->put_textIndent          (v_Value));         break;
      case P_TextTransform: b_Ret = SUCCEEDED(mi_Style->put_textTransform       (v_Value.bstrVal)); break;
      case P_Top:           b_Ret = SUCCEEDED(mi_Style->put_top                 (v_Value));         break;
      case P_VerticalAlign: b_Ret = SUCCEEDED(mi_Style->put_verticalAlign       (v_Value));         break;
      case P_Visibility:    b_Ret = SUCCEEDED(mi_Style->put_visibility          (v_Value.bstrVal)); break;
      case P_Width:         b_Ret = SUCCEEDED(mi_Style->put_width               (v_Value));         break;
      case P_ZIndex:        b_Ret = SUCCEEDED(mi_Style->put_zIndex              (v_Value));         break;
      case P_Bottom:        {
                              CComQIPtr<IHTMLStyle2,&IID_IHTMLStyle2> style2 =mi_Style;
                              b_Ret = SUCCEEDED(style2->put_bottom(v_Value));
                            }
                            break;
      case P_Right:         {
                              CComQIPtr<IHTMLStyle2,&IID_IHTMLStyle2> style2 =mi_Style;
                              b_Ret = SUCCEEDED(style2->put_right(v_Value));
                            }
                            break;
      case P_Zoom:          {
                              CComQIPtr<IHTMLStyle3,&IID_IHTMLStyle3> style3 =mi_Style;
                              b_Ret = SUCCEEDED(style3->put_zoom(v_Value));
                            }
                            break;
		}
	}
	return b_Ret;
}

