//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    HTMLStyle.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Handles the various style attributes of a tag in the HTML
//
#pragma once
#include <atlbase.h>    // CComPtr
#include <afxhtml.h>    // CHtmlEditView
#include "misc.h"

// These definitions must have the same order as enum eProp !
#define __StyleNames {_T("background:"),            \
                      _T("background-attachment:"), \
                      _T("background-color:"),      \
                      _T("background-image:"),      \
                      _T("background-position:"),   \
                      _T("background-repeat:"),     \
                      _T("border:"),                \
                      _T("border-bottom:"),         \
                      _T("border-bottom-color:"),   \
                      _T("border-bottom-style:"),   \
                      _T("border-bottom-width:"),   \
                      _T("border-color:"),          \
                      _T("border-left:"),           \
                      _T("border-left-color:"),     \
                      _T("border-left-style:"),     \
                      _T("border-left-width:"),     \
                      _T("border-right:"),          \
                      _T("border-right-color:"),    \
                      _T("border-right-style:"),    \
                      _T("border-right-width:"),    \
                      _T("border-style:"),          \
                      _T("border-top:"),            \
                      _T("border-top-color:"),      \
                      _T("border-top-style:"),      \
                      _T("border-top-width:"),      \
                      _T("border-width:"),          \
                      _T("bottom:"),                \
                      _T("clear:"),                 \
                      _T("clip:"),                  \
                      _T("color:"),                 \
                      _T("cursor:"),                \
                      _T("display:"),               \
                      _T("float:"),                 \
                      _T("font:"),                  \
                      _T("font-family:"),           \
                      _T("font-size:"),             \
                      _T("font-style:"),            \
                      _T("font-variant:"),          \
                      _T("font-weight:"),           \
                      _T("height:"),                \
                      _T("left:"),                  \
                      _T("letter-spacing:"),        \
                      _T("line-height:"),           \
                      _T("list-style:"),            \
                      _T("list-style-image:"),      \
                      _T("list-style-position:"),   \
                      _T("list-style-type:"),       \
                      _T("margin:"),                \
                      _T("margin-bottom:"),         \
                      _T("margin-left:"),           \
                      _T("margin-right:"),          \
                      _T("margin-top:"),            \
                      _T("overflow:"),              \
                      _T("padding:"),               \
                      _T("padding-bottom"),         \
                      _T("padding-left:"),          \
                      _T("padding-right:"),         \
                      _T("padding-top:"),           \
                      _T("page-break-after:"),      \
                      _T("page-break-before:"),     \
                      _T("position:"),              \
                      _T("right:"),                 \
                      _T("text-align:"),            \
                      _T("text-decoration:"),       \
                      _T("text-indent:"),           \
                      _T("text-transform:"),        \
                      _T("top:"),                   \
                      _T("vertical-align:"),        \
                      _T("visibility:"),            \
                      _T("width:"),                 \
                      _T("z-index:"),               \
                      _T("zoom:")                   \
                      }


class HtmlStyle : public Misc
{
public:
	enum eProp
	{
    P_Background = 0,
    P_BackAttach,
		P_BackgColor,
    P_BackgImage,
    P_BackgPosition,
    P_BackgRepeat,
    P_Border,
    P_BorderBottom,
    P_BorderBotColor,
    P_BorderBotStyle,
    P_BorderBotWidth,
    P_BorderColor,
    P_BorderLeft,
    P_BorderLftColor,
    P_BorderLftStyle,
    P_BorderLftWidth,
    P_BorderRight,
    P_BorderRigColor,
    P_BorderRigStyle,
    P_BorderRigWidth,
		P_BorderStyle,
    P_BorderTop,
    P_BorderTopColor,
    P_BorderTopStyle,
    P_BorderTopWidth,
		P_BorderWidth,
    P_Bottom,
    P_Clear,
    P_Clip,
		P_Color,
    P_Cursor,
    P_Display,
    P_Float,
    P_Font,
    P_FontFamily,
		P_FontSize,
    P_FontStyle, 
    P_FontVariant,
    P_FontWeight,
    P_Height,
    P_Left,
    P_LetterSpacing,
    P_LineHeight,
    P_ListStyle,
    P_ListStyleImage,
    P_ListStylePos,
    P_ListStyleType,
    P_Margin,
    P_MarginBottom,
    P_MarginLeft,
    P_MarginRight,
    P_MarginTop,
    P_Overflow,
    P_Padding,
    P_PaddingBottom,
    P_PaddingLeft,
    P_PaddingRight,
    P_PaddingTop,
    P_PageBreakAfter,
    P_PageBreakBefore,
    P_Position,
    P_Right,
    P_TextAlign,
    P_TextDecoration,
    P_TextIndent,
    P_TextTransform,
    P_Top,
    P_VerticalAlign,
    P_Visibility,
		P_Width,
    P_ZIndex,
    P_Zoom
	};

	HtmlStyle(CComPtr<IHTMLStyle> i_Style);
  HtmlStyle();

  virtual BOOL    Valid();
          CString GetStylesheetText();
          void    SetStylesheetText(CString& text);
	        CString GetProperty(eProp e_Prop);
	        BOOL    SetProperty(eProp e_Prop, CString s_Value);
          void    SetStyle(CComPtr<IHTMLStyle> i_Style);

protected:
	CComPtr<IHTMLStyle>     mi_Style;
};
