//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    HTMLEdView.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Iimplementation of the CHTMLEdView class
// VIEW with the MSHMTL control for a HTML page
//
// This is the main view for a HTML editor
// Contains and calls all the other HTML stuff.
//
#include "stdafx.h"
#include <afxpriv.h>
#include <io.h> // For access
// Includes of main
#include "AuthorHTML.h"
#include "HTMLEdDoc.h"
#include "htmledview.h"
#include "mainfrm.h"
#include "ChildFrm.h"
#include "DocumentFile.h"
#include "ProjectFile.h"
#include "RegExp.h"
#include "Css.h"
// HTML Elements
#include "HTMLAnchor.h"
#include "HTMLArea.h"
#include "HTMLFieldset.h"
#include "HTMLHeading.h"
#include "HTMLIFrame.h"
#include "HTMLScript.h"
#include "HTMLTable.h"
#include "HTMLTableRow.h"
#include "HTMLTableCell.h"
// Dialogs
#include "AnchorDlg.h"
#include "AreaDlg.h"
#include "BookmarkDialog.h"
#include "ButtonDlg.h"
#include "CharacterDialog.h"
#include "CommentDialog.h"
#include "DivTextDlg.h"
#include "ImageDlg.h"
#include "FieldsetDlg.h"
#include "FileDialog.h"
#include "FindReplaceDialog.h"
#include "FontDlg.h"
#include "FormDlg.h"
#include "FormSelectDlg.h"
#include "HeadingDlg.h"
#include "HRDialog.h"
#include "InlineFrameDlg.h"
#include "LayerDlg.h"
#include "MarqueeDialog.h"
#include "NavigateHyperlink.h"
#include "ObjectDlg.h"
#include "ParagraphDlg.h"
#include "ScriptDlg.h"
#include "SnapGridDlg.h"
#include "SpanDivDlg.h"
#include "Spelling.h"
#include "SpellingDlg.h"
#include "SpellOptionsDlg.h"
#include "StyleSheetsDlg.h"
#include "TableCaptionDlg.h"
#include "TableProperties.h"
#include "TopicProperties.h"
#include "AlinkDlg.h"
#include "ALinkCommandDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHTMLEdView
// Arrays for context menu
IMPLEMENT_DYNCREATE(CHTMLEdView, CHtmlEditView)

///////////////////////////////////////////////////////////////////
//
// "special mapping" of MFC -> IHTML commands
//
// CodeJock Framework does not support DHTMLEDITING_CMD_ENTRY
// All the messages are dispatched twice (Quite annoying)!!
//
//BEGIN_DHTMLEDITING_CMDMAP(CHTMLEdView)
  //DHTMLEDITING_CMD_ENTRY(ID_FILE_PRINTPREVIEW,         IDM_PRINTPREVIEW)
  //DHTMLEDITING_CMD_ENTRY(ID_FILE_PAGESETUP,            IDM_PAGESETUP)
  //DHTMLEDITING_CMD_ENTRY(ID_EDIT_CUT,                  IDM_CUT)
  //DHTMLEDITING_CMD_ENTRY(ID_EDIT_COPY,                 IDM_COPY)
  //DHTMLEDITING_CMD_ENTRY(ID_EDIT_PASTE,                IDM_PASTE)
  //DHTMLEDITING_CMD_ENTRY(ID_EDIT_UNDO,                 IDM_UNDO)
  //DHTMLEDITING_CMD_ENTRY(ID_EDIT_REDO,                 IDM_REDO)
  //DHTMLEDITING_CMD_ENTRY(ID_EDIT_SELECT_ALL,           IDM_SELECTALL)
  //DHTMLEDITING_CMD_ENTRY(ID_EDIT_FONT,                 IDM_FONT)
  //DHTMLEDITING_CMD_ENTRY(ID_BUTTON_INDENT,             IDM_INDENT)
  //DHTMLEDITING_CMD_ENTRY(ID_BUTTON_OUTDENT,            IDM_OUTDENT)
  //DHTMLEDITING_CMD_ENTRY(ID_BUTTON_NOFORMAT,           IDM_REMOVEFORMAT)
  //DHTMLEDITING_CMD_ENTRY_FUNC(ID_FORMAT_ABSOLUTEPOSITIONELEMENT, IDM_ABSOLUTE_POSITION, OnAbsolute)
  //DHTMLEDITING_CMD_ENTRY_FUNC(ID_FORMAT_STATICELEMENT, IDM_ABSOLUTE_POSITION, OnStatic)
  //DHTMLEDITING_CMD_ENTRY_FUNC(ID_BUTTON_ABSOLUTE,      IDM_ABSOLUTE_POSITION, OnAbsolute)
  //DHTMLEDITING_CMD_ENTRY_FUNC(ID_BUTTON_STATIC,        IDM_ABSOLUTE_POSITION, OnStatic)
  //DHTMLEDITING_CMD_ENTRY_FUNC(ID_BUTTON_ANCHOR,        IDM_BOOKMARK,          OnBookmark)
  //DHTMLEDITING_CMD_ENTRY_FUNC(ID_BUTTON_HYPERLINK,     IDM_HYPERLINK,         OnAnchor)
  //DHTMLEDITING_CMD_ENTRY_FUNC(ID_BUTTON_IMAGE,         IDM_IMAGE,             OnImage)
  //DHTMLEDITING_CMD_ENTRY_FUNC(ID_MARQUEE,              IDM_MARQUEE,           OnMarquee)
  //DHTMLEDITING_CMD_ENTRY_TYPE(ID_BUTTON_BOLD,          IDM_BOLD,         AFX_UI_ELEMTYPE_CHECBOX)
  //DHTMLEDITING_CMD_ENTRY_TYPE(ID_BUTTON_ITALIC,        IDM_ITALIC,       AFX_UI_ELEMTYPE_CHECBOX)
  //DHTMLEDITING_CMD_ENTRY_TYPE(ID_BUTTON_UNDERLINE,     IDM_UNDERLINE,    AFX_UI_ELEMTYPE_CHECBOX)
  //DHTMLEDITING_CMD_ENTRY_TYPE(ID_BUTTON_BULLETLIST,    IDM_UNORDERLIST,  AFX_UI_ELEMTYPE_CHECBOX)
  //DHTMLEDITING_CMD_ENTRY_TYPE(ID_BUTTON_NUMBERLIST,    IDM_ORDERLIST,    AFX_UI_ELEMTYPE_CHECBOX)
  //DHTMLEDITING_CMD_ENTRY_TYPE(ID_BUTTON_LEFTJUSTIFY,   IDM_JUSTIFYLEFT,  AFX_UI_ELEMTYPE_CHECBOX)
  //DHTMLEDITING_CMD_ENTRY_TYPE(ID_BUTTON_CENTERJUSTIFY, IDM_JUSTIFYCENTER,AFX_UI_ELEMTYPE_CHECBOX)
  //DHTMLEDITING_CMD_ENTRY_TYPE(ID_BUTTON_RIGHTJUSTIFY,  IDM_JUSTIFYRIGHT, AFX_UI_ELEMTYPE_CHECBOX)
  //DHTMLEDITING_CMD_ENTRY_TYPE(ID_BUTTON_JUSTIFY,       IDM_JUSTIFYFULL,  AFX_UI_ELEMTYPE_CHECBOX)
//END_DHTMLEDITING_CMDMAP()

///////////////////////////////////////////////////////////////////
//
// "NORMAL" ATL/MFC message map
//
BEGIN_MESSAGE_MAP(CHTMLEdView, CHtmlEditView)
  ON_WM_CREATE()
  ON_COMMAND(ID_PROPERTIES,                   OnProperties)
  ON_COMMAND(ID_EDIT_REMOVEHYPERLINK,         OnRemoveHyperlink)
  ON_COMMAND(ID_EDIT_FIND,                    OnFind)
  ON_COMMAND(ID_EDIT_REPLACE,                 OnReplace)
  ON_COMMAND(ID_EDIT_CHECKSPELLING,           OnSpellCheckDocument)
  ON_COMMAND(ID_EDIT_SPELLINGOPTIONS,         OnSpellOptions)
  ON_COMMAND(ID_CSSSTYLESHEETS,               OnEditStyleSheets)
  ON_COMMAND(ID_FORMAT_PARAGRAPH,             OnFormatParagraph)
  ON_COMMAND(ID_VIEW_TABLEBORDERS,            OnTableBorders)
  ON_COMMAND(ID_VIEW_TAGS,                    OnViewTags)
  ON_COMMAND(ID_VIEW_BROWSE,                  OnViewBrowse)
  ON_COMMAND(ID_GRID_SHOWGRID,                OnShowGrid)
  ON_COMMAND(ID_GRID_SNAPTOGRID,              OnSnapToGrid)
  ON_COMMAND(ID_GRID_GRIDPROPERTIES,          OnGridProperties)
  ON_COMMAND(ID_TEXTSTYLES_ACRONYM,           OnInsertAcronym)
  ON_COMMAND(ID_TEXTSTYLES_ADDRESS,           OnInsertAddress)
  ON_COMMAND(ID_TEXTSTYLES_BIG,               OnBig)
  ON_COMMAND(ID_TEXTSTYLES_BLOCKQUOTE,        OnBlockQuote)
  ON_COMMAND(ID_TEXTSTYLES_CITATION,          OnCite)
  ON_COMMAND(ID_TEXTSTYLES_CODE,              OnCode)
  ON_COMMAND(ID_TEXTSTYLES_DELETEDTEXT,       OnDeletedText)
  ON_COMMAND(ID_TEXTSTYLES_INSERTEDTEXT,      OnInsertedText)
  ON_COMMAND(ID_TEXTSTYLES_KEYBOARD,          OnKbd)
  ON_COMMAND(ID_TEXTSTYLES_PREFORMATTED,      OnPre)
  ON_COMMAND(ID_TEXTSTYLES_INLINEQUOTATION,   OnQuotation)
  ON_COMMAND(ID_TEXTSTYLES_SAMPLE,            OnSample)
  ON_COMMAND(ID_TEXTSTYLES_SMALLER,           OnSmaller)
  ON_COMMAND(ID_TEXTSTYLES_SUBSCRIPT,         OnSubscript)
  ON_COMMAND(ID_TEXTSTYLES_SUPERSCRIPT,       OnSuperscript)
  ON_COMMAND(ID_TEXTSTYLES_TELETYPETEXT,      OnTeletype)
  ON_COMMAND(ID_PARAGRAPHSTYLES_PARAGRAPH,    OnParaStyleParagraph)
  ON_COMMAND(ID_PARAGRAPHSTYLES_HEADING1,     OnParaStyleHeading1)
  ON_COMMAND(ID_PARAGRAPHSTYLES_HEADING2,     OnParaStyleHeading2)
  ON_COMMAND(ID_PARAGRAPHSTYLES_HEADING3,     OnParaStyleHeading3)
  ON_COMMAND(ID_PARAGRAPHSTYLES_HEADING4,     OnParaStyleHeading4)
  ON_COMMAND(ID_PARAGRAPHSTYLES_HEADING5,     OnParaStyleHeading5)
  ON_COMMAND(ID_PARAGRAPHSTYLES_HEADING6,     OnParaStyleHeading6)
  ON_COMMAND(ID_PARAGRAPHSTYLES_ADDRESS,      OnParaStyleAddress)
  ON_COMMAND(ID_PARAGRAPHSTYLES_PREFORMAT,    OnParaStylePreformat)
  ON_COMMAND(ID_TABLE_INSERT,                 OnTableInsert)
  ON_COMMAND(ID_TABLE_INSERTROWABOVE,         OnTableInsertRowAbove)
  ON_COMMAND(ID_TABLE_INSERTROWBELOW,         OnTableInsertRowBelow)
  ON_COMMAND(ID_TABLE_INSERTCOLUMNLEFT,       OnTableInsertColumnBefore)
  ON_COMMAND(ID_TABLE_INSERTCOLUMNRIGHT,      OnTableInsertColumnAfter)
  ON_COMMAND(ID_TABLE_DELETEROW,              OnTableDeleteRow)
  ON_COMMAND(ID_TABLE_DELETECOLUMN,           OnTableDeleteColumn)
  ON_COMMAND(ID_TABLE_COMBINECELLS,           OnTableCombine)
  ON_COMMAND(ID_TABLE_SPLITCELL,              OnTableSplitCell)
  ON_COMMAND(ID_TABLE_TABLEPROPERTIES,        OnTableProperties)
  ON_COMMAND(ID_TABLE_CELLPROPERTIES,         OnCellProperties)
  ON_COMMAND(ID_CELL_TOP,                     OnCellTop)
  ON_COMMAND(ID_CELL_MIDDLE,                  OnCellMiddle)
  ON_COMMAND(ID_CELL_BOTTOM,                  OnCellBottom)
  ON_COMMAND(ID_VIEW_SPECIALTEST,             OnSpecialTest)
  ON_COMMAND(ID_TOGGLE_INSERT,                OnInsertKey)
  ON_COMMAND(ID_INSERT_BREAK,                 OnInsertBR)
  ON_COMMAND(ID_INSERT_HORIZONTALLINE,        OnInsertHR)
  ON_COMMAND(ID_INSERT_SYMBOL,                OnInsertSymbol)
  ON_COMMAND(ID_EDIT_COMMENT,                 OnEditComment)
  ON_COMMAND(ID_INSERT_COMMENT,               OnInsertComment)
  ON_COMMAND(ID_BUTTON_PARAGRAPH,             OnViewParagraph)
  ON_COMMAND(ID_INSERT_BREAKBELOWIMAGES,      OnBreakBelowImages)
  ON_COMMAND(ID_INSERT_FORM,                  OnInsertForm)
  ON_COMMAND(ID_INSERT_BUTTON,                OnInsertButton)
  ON_COMMAND(ID_FORM_FIELD,                   OnInsertFormField)
  ON_COMMAND(ID_FORM_BUTTON,                  OnInsertFormButton)
  ON_COMMAND(ID_FORM_TEXTAREA,                OnInsertFormTextarea)
  ON_COMMAND(ID_FORM_IMAGE,                   OnInsertFormImage)
  ON_COMMAND(ID_FORM_FIELDSET,                OnInsertFieldset)
  ON_COMMAND(ID_FORM_LABEL,                   OnInsertFormLabel)
  ON_COMMAND(ID_FORM_SELECTIONLIST,           OnInsertFormSelect)
  ON_COMMAND(ID_INSERT_IFRAME,                OnInsertIFrame)
  ON_COMMAND(ID_INSERT_SCRIPT,                OnInsertScript)
  ON_COMMAND(ID_INSERT_OBJECT,                OnInsertObject)
  ON_COMMAND(ID_INSERT_LAYER,                 OnInsertLayer)
  ON_COMMAND(ID_INSERT_ALINK,                 OnInsertALink)
  ON_COMMAND(ID_INSERT_ALINK_COMMAND,         OnInsertALinkCommand)
  ON_COMMAND(ID_AREA_RECTANGLE,               OnInsertAreaRectangle)
  ON_COMMAND(ID_AREA_CIRCLE,                  OnInsertAreaCircle)
  ON_COMMAND(ID_AREA_POLYGON,                 OnInsertAreaPolygon)
  ON_COMMAND(ID_BUTTON_ANCHOR,                OnBookmark)
  ON_COMMAND(ID_BUTTON_HYPERLINK,             OnAnchor)
  ON_COMMAND(ID_BUTTON_IMAGE,                 OnImage)
  ON_COMMAND(ID_MARQUEE,                      OnMarquee)
  ON_COMMAND(ID_BUTTON_BOLD,                  OnBold)
  ON_COMMAND(ID_BUTTON_ITALIC,                OnItalic)
  ON_COMMAND(ID_BUTTON_UNDERLINE,             OnUnderline)
  ON_COMMAND(ID_BUTTON_COLOR,                 PopColorMenu)
  ON_COMMAND(ID_BUTTON_NUMBERLIST,            OnNumberList)
  ON_COMMAND(ID_BUTTON_BULLETLIST,            OnBulletList)
  ON_COMMAND(ID_EDIT_FONT,                    OnFont)
  ON_COMMAND(ID_BUTTON_INDENT,                OnIndent)
  ON_COMMAND(ID_BUTTON_OUTDENT,               OnOutdent)
  ON_COMMAND(ID_FILE_PRINTPREVIEW,            OnPrintPreview)
  ON_COMMAND(ID_FILE_PAGESETUP,               OnPageSetup)
  ON_COMMAND(ID_EDIT_CUT,                     OnCut)
  ON_COMMAND(ID_EDIT_COPY,                    OnCopy)
  ON_COMMAND(ID_EDIT_PASTE,                   OnPaste)
  ON_COMMAND(ID_EDIT_UNDO,                    OnUndo)
  ON_COMMAND(ID_EDIT_REDO,                    OnRedo)
  ON_COMMAND(ID_EDIT_SELECT_ALL,              OnSelectAll)
  ON_COMMAND(ID_BUTTON_NOFORMAT,              OnNoFormat)
  ON_COMMAND(ID_BUTTON_LEFTJUSTIFY,           OnLeftJustify)
  ON_COMMAND(ID_BUTTON_CENTERJUSTIFY,         OnCenterJustify)
  ON_COMMAND(ID_BUTTON_RIGHTJUSTIFY,          OnRightJustify)
  ON_COMMAND(ID_BUTTON_JUSTIFY,               OnJustify)
  ON_COMMAND(ID_FORMAT_ABSOLUTEPOSITIONELEMENT,OnAbsolute)
  ON_COMMAND(ID_BUTTON_ABSOLUTE,              OnAbsolute)
  ON_COMMAND(ID_FORMAT_STATICELEMENT,         OnStatic)
  ON_COMMAND(ID_BUTTON_STATIC,                OnStatic)
  ON_UPDATE_COMMAND_UI(ID_FORMAT_ABSOLUTEPOSITIONELEMENT,OnUpdateAbsolute)
  ON_UPDATE_COMMAND_UI(ID_BUTTON_ABSOLUTE,        OnUpdateAbsolute)
  ON_UPDATE_COMMAND_UI(ID_FORMAT_STATICELEMENT,   OnUpdateStatic)
  ON_UPDATE_COMMAND_UI(ID_BUTTON_STATIC,          OnUpdateStatic)
  ON_UPDATE_COMMAND_UI(ID_BUTTON_LEFTJUSTIFY,     OnUpdateLeftJustify)
  ON_UPDATE_COMMAND_UI(ID_BUTTON_CENTERJUSTIFY,   OnUpdateCenterJustify)
  ON_UPDATE_COMMAND_UI(ID_BUTTON_RIGHTJUSTIFY,    OnUpdateRightJustify)
  ON_UPDATE_COMMAND_UI(ID_BUTTON_JUSTIFY,         OnUpdateJustify)
  ON_UPDATE_COMMAND_UI(ID_BUTTON_NOFORMAT,        OnUpdateNoFormat)
  ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO,              OnUpdateUndo)
  ON_UPDATE_COMMAND_UI(ID_EDIT_REDO,              OnUpdateRedo)
  ON_UPDATE_COMMAND_UI(ID_EDIT_SELECT_ALL,        OnUpdateSelectAll)
  ON_UPDATE_COMMAND_UI(ID_EDIT_CUT,               OnUpdateCut)
  ON_UPDATE_COMMAND_UI(ID_EDIT_COPY,              OnUpdateCopy)
  ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE,             OnUpdatePaste)
  ON_UPDATE_COMMAND_UI(ID_FILE_PAGESETUP,         OnUpdatePageSetup)
  ON_UPDATE_COMMAND_UI(ID_FILE_PRINTPREVIEW,      OnUpdatePrintPreview)
  ON_UPDATE_COMMAND_UI(ID_BUTTON_BOLD,            OnUpdateBold)
  ON_UPDATE_COMMAND_UI(ID_BUTTON_ITALIC,          OnUpdateItalic)
  ON_UPDATE_COMMAND_UI(ID_BUTTON_UNDERLINE,       OnUpdateUnderline)
  ON_UPDATE_COMMAND_UI(ID_BUTTON_ANCHOR,          OnUpdateAnchor)
  ON_UPDATE_COMMAND_UI(ID_BUTTON_HYPERLINK,       OnUpdateHyperlink)
  ON_UPDATE_COMMAND_UI(ID_BUTTON_IMAGE,           OnUpdateUINoSelection)
  ON_UPDATE_COMMAND_UI(ID_MARQUEE,                OnUpdateUISelection)
  ON_UPDATE_COMMAND_UI(ID_BUTTON_NUMBERLIST,      OnUpdateNumberList)
  ON_UPDATE_COMMAND_UI(ID_BUTTON_BULLETLIST,      OnUpdateBulletList)
  ON_UPDATE_COMMAND_UI(ID_BUTTON_INDENT,          OnUpdateIndent)
  ON_UPDATE_COMMAND_UI(ID_BUTTON_OUTDENT,         OnUpdateOutdent)
  ON_UPDATE_COMMAND_UI(ID_EDIT_REMOVEHYPERLINK,   OnUpdateHyperlink)
  ON_UPDATE_COMMAND_UI(ID_EDIT_COMMENT,           OnUpdateComment)
  ON_UPDATE_COMMAND_UI(ID_BUTTON_COLOR,           OnUpdateColor)
  ON_UPDATE_COMMAND_UI(ID_FORMAT_PARAGRAPH,       OnUpdateFormatParagraph)
  ON_UPDATE_COMMAND_UI(ID_VIEW_TABLEBORDERS,      OnUpdateTableBorders)
  ON_UPDATE_COMMAND_UI(ID_VIEW_TAGS,              OnUpdateModeTags)
  ON_UPDATE_COMMAND_UI(ID_VIEW_BROWSE,            OnUpdateModeBrowse)
  ON_UPDATE_COMMAND_UI(ID_GRID_SHOWGRID,          OnUpdateGrid)
  ON_UPDATE_COMMAND_UI(ID_GRID_SNAPTOGRID,        OnUpdateSnap)
  ON_UPDATE_COMMAND_UI(ID_BUTTON_PARAGRAPH,       OnUpdateParagraph)
  ON_UPDATE_COMMAND_UI(ID_TABLE_INSERTROWABOVE,   OnUpdateTableCommands)
  ON_UPDATE_COMMAND_UI(ID_TABLE_INSERTROWBELOW,   OnUpdateTableCommands)
  ON_UPDATE_COMMAND_UI(ID_TABLE_INSERTCOLUMNLEFT, OnUpdateTableCommands)
  ON_UPDATE_COMMAND_UI(ID_TABLE_INSERTCOLUMNRIGHT,OnUpdateTableCommands)
  ON_UPDATE_COMMAND_UI(ID_TABLE_DELETEROW,        OnUpdateTableCommands)
  ON_UPDATE_COMMAND_UI(ID_TABLE_DELETECOLUMN,     OnUpdateTableCommands)
  ON_UPDATE_COMMAND_UI(ID_TABLE_COMBINECELLS,     OnUpdateTableCommands)
  ON_UPDATE_COMMAND_UI(ID_TABLE_SPLITCELL,        OnUpdateTableCommands)
  ON_UPDATE_COMMAND_UI(ID_TABLE_TABLEPROPERTIES,  OnUpdateTableCommands)
  ON_UPDATE_COMMAND_UI(ID_TABLE_CELLPROPERTIES,   OnUpdateTableCommands)
  ON_UPDATE_COMMAND_UI(ID_CELL_TOP,               OnUpdateTableCommands)
  ON_UPDATE_COMMAND_UI(ID_CELL_MIDDLE,            OnUpdateTableCommands)
  ON_UPDATE_COMMAND_UI(ID_CELL_BOTTOM,            OnUpdateTableCommands)
  ON_UPDATE_COMMAND_UI(ID_INSERT_FORM,            OnUpdateForm)
  ON_UPDATE_COMMAND_UI(ID_FORM_FIELD,             OnUpdateFormElements)
  ON_UPDATE_COMMAND_UI(ID_FORM_BUTTON,            OnUpdateFormElements)
  ON_UPDATE_COMMAND_UI(ID_FORM_TEXTAREA,          OnUpdateFormElements)
  ON_UPDATE_COMMAND_UI(ID_FORM_IMAGE,             OnUpdateFormElements)
  ON_UPDATE_COMMAND_UI(ID_FORM_LABEL,             OnUpdateFormElements)
  ON_UPDATE_COMMAND_UI(ID_FORM_SELECTIONLIST,     OnUpdateFormElements)
END_MESSAGE_MAP()

////////////////////////////////////////////////////////////////////
//
// JUMP TABLE: Optimization per tag for double-click action
//
////////////////////////////////////////////////////////////////////

CHTMLEdView::TagJumps 
CHTMLEdView::jumps[] =
{
  // SUPPORTED TAGS
  { _T("!"),          &CHTMLEdView::OnDoubleClickComment,      NULL, _T("") },
  { _T("a"),          &CHTMLEdView::OnDoubleClickHyperlink,    NULL, _T("") },
  { _T("acronym"),    &CHTMLEdView::OnDoubleClickAcronym,      NULL, _T("") },
  { _T("address"),    &CHTMLEdView::OnDoubleClickAddress,      NULL, _T("") },
  { _T("area"),       &CHTMLEdView::OnDoubleClickArea,         NULL, _T("") },
  { _T("b"),          &CHTMLEdView::OnDoubleClickInlineTag,    NULL, _T("") },
  { _T("big"),        &CHTMLEdView::OnDoubleClickInlineTag,    NULL, _T("") },
  { _T("blockquote"), &CHTMLEdView::OnDoubleClickInlineTag,    NULL, _T("") },
  { _T("br"),         &CHTMLEdView::OnDoubleClickInlineTag,    NULL, _T("") },
  { _T("button"),     &CHTMLEdView::OnDoubleClickButton,       NULL, _T("") },
  { _T("caption"),    &CHTMLEdView::OnDoubleClickCaption,      NULL, _T("") },
  { _T("cite"),       &CHTMLEdView::OnDoubleClickInlineTag,    NULL, _T("") },
  { _T("code"),       &CHTMLEdView::OnDoubleClickInlineTag,    NULL, _T("") },
  { _T("comment"),    &CHTMLEdView::OnDoubleClickComment,      NULL, _T("") },
  { _T("dd"),         &CHTMLEdView::OnDoubleClickInlineTag,    NULL, _T("") },
  { _T("del"),        &CHTMLEdView::OnDoubleClickInlineTag,    NULL, _T("") },
  { _T("dir"),        &CHTMLEdView::OnDoubleClickInlineTag,    NULL, _T("") },
  { _T("div"),        &CHTMLEdView::OnDoubleClickDiv,          NULL, _T("") },
  { _T("dl"),         &CHTMLEdView::OnDoubleClickInlineTag,    NULL, _T("") },
  { _T("dt"),         &CHTMLEdView::OnDoubleClickInlineTag,    NULL, _T("") },
  { _T("em"),         &CHTMLEdView::OnDoubleClickInlineTag,    NULL, _T("") },
  { _T("fieldset"),   &CHTMLEdView::OnDoubleClickFieldset,     NULL, _T("") },
  { _T("font"),       &CHTMLEdView::OnDoubleClickFont,         NULL, _T("") },
  { _T("form"),       &CHTMLEdView::OnDoubleClickForm,         NULL, _T("") },
  { _T("h1"),         &CHTMLEdView::OnDoubleClickHeading1,     NULL, _T("") },
  { _T("h2"),         &CHTMLEdView::OnDoubleClickHeading2,     NULL, _T("") },
  { _T("h3"),         &CHTMLEdView::OnDoubleClickHeading3,     NULL, _T("") },
  { _T("h4"),         &CHTMLEdView::OnDoubleClickHeading4,     NULL, _T("") },
  { _T("h5"),         &CHTMLEdView::OnDoubleClickHeading5,     NULL, _T("") },
  { _T("h6"),         &CHTMLEdView::OnDoubleClickHeading6,     NULL, _T("") },
  { _T("hr"),         &CHTMLEdView::OnDoubleClickHR,           NULL, _T("") },
  { _T("i"),          &CHTMLEdView::OnDoubleClickInlineTag,    NULL, _T("") },
  { _T("iframe"),     &CHTMLEdView::OnDoubleClickIFrame,       NULL, _T("") },
  { _T("img"),        &CHTMLEdView::OnDoubleClickImage,        NULL, _T("") },
  { _T("input"),      &CHTMLEdView::OnDoubleClickFormInput,    NULL, _T("") },
  { _T("ins"),        &CHTMLEdView::OnDoubleClickInlineTag,    NULL, _T("") },
  { _T("kbd"),        &CHTMLEdView::OnDoubleClickInlineTag,    NULL, _T("") },
  { _T("label"),      &CHTMLEdView::OnDoubleClickFormLabel,    NULL, _T("") },
  { _T("legend"),     &CHTMLEdView::OnDoubleClickLegend,       NULL, _T("") },
  { _T("li"),         &CHTMLEdView::OnDoubleClickInlineTag,    NULL, _T("") },
  { _T("listing"),    &CHTMLEdView::OnDoubleClickInlineTag,    NULL, _T("") },
  { _T("marquee"),    &CHTMLEdView::OnDoubleClickMarquee,      NULL, _T("") },
  { _T("object"),     &CHTMLEdView::OnDoubleClickObject,       NULL, _T("") },
  { _T("ol"),         &CHTMLEdView::OnDoubleClickInlineTag,    NULL, _T("") },
  { _T("p"),          &CHTMLEdView::OnDoubleClickParagraph,    NULL, _T("") },
  { _T("plaintext"),  &CHTMLEdView::OnDoubleClickInlineTag,    NULL, _T("") },
  { _T("pre"),        &CHTMLEdView::OnDoubleClickInlineTag,    NULL, _T("") },
  { _T("q"),          &CHTMLEdView::OnDoubleClickInlineTag,    NULL, _T("") },
  { _T("s"),          &CHTMLEdView::OnDoubleClickInlineTag,    NULL, _T("") },
  { _T("samp"),       &CHTMLEdView::OnDoubleClickInlineTag,    NULL, _T("") },
  { _T("script"),     &CHTMLEdView::OnDoubleClickScript,       NULL, _T("") },
  { _T("select"),     &CHTMLEdView::OnDoubleClickFormSelect,   NULL, _T("") },
  { _T("small"),      &CHTMLEdView::OnDoubleClickInlineTag,    NULL, _T("") },
  { _T("span"),       &CHTMLEdView::OnDoubleClickSpan,         NULL, _T("") },
  { _T("strike"),     &CHTMLEdView::OnDoubleClickInlineTag,    NULL, _T("") },
  { _T("strong"),     &CHTMLEdView::OnDoubleClickInlineTag,    NULL, _T("") },
  { _T("sub"),        &CHTMLEdView::OnDoubleClickInlineTag,    NULL, _T("") },
  { _T("sup"),        &CHTMLEdView::OnDoubleClickInlineTag,    NULL, _T("") },
  { _T("table"),      &CHTMLEdView::OnTableProperties,         NULL, _T("") },
  { _T("tbody"),      &CHTMLEdView::OnTableProperties,         NULL, _T("") },
  { _T("textarea"),   &CHTMLEdView::OnDoubleClickFormTextArea, NULL, _T("") },
  { _T("td"),         &CHTMLEdView::OnCellProperties,          NULL, _T("") },
  { _T("th"),         &CHTMLEdView::OnCellProperties,          NULL, _T("") },
  { _T("thead"),      &CHTMLEdView::OnTableProperties,         NULL, _T("") },
  { _T("tfoot"),      &CHTMLEdView::OnTableProperties,         NULL, _T("") },
  { _T("tr"),         &CHTMLEdView::OnCellProperties,          NULL, _T("") },
  { _T("tt"),         &CHTMLEdView::OnDoubleClickInlineTag,    NULL, _T("") },
  { _T("u"),          &CHTMLEdView::OnDoubleClickInlineTag,    NULL, _T("") },
  { _T("ul"),         &CHTMLEdView::OnDoubleClickInlineTag,    NULL, _T("") },
  { _T("var"),        &CHTMLEdView::OnDoubleClickInlineTag,    NULL, _T("") },
  { _T("wbr"),        &CHTMLEdView::OnDoubleClickInlineTag,    NULL, _T("") },
  { _T("xmp"),        &CHTMLEdView::OnDoubleClickInlineTag,    NULL, _T("") },
  // UNSUPPORTED TAGS
  { _T("abbr"),     NULL,  &CHTMLEdView::OnUnsupportedHTML, _T("ABBR is deprecated HTML")                       },
  { _T("applet"),   NULL,  &CHTMLEdView::OnUnsupportedHTML, _T("APPLET (Use object instead)")                   },
  { _T("base"),     NULL,  &CHTMLEdView::OnUnsupportedHTML, _T("BASE (Cannot be used in CHM documentation)")    },
  { _T("basefont"), NULL,  &CHTMLEdView::OnUnsupportedHTML, _T("BASEFONT (Use Cascading StyleSheets instead)")  },
  { _T("bgsound"),  NULL,  &CHTMLEdView::OnUnsupportedHTML, _T("BGSOUND (Not used in CHM documentation)")       },
  { _T("blink"),    NULL,  &CHTMLEdView::OnUnsupportedHTML, _T("BLINK (Do not use: to restless / unreadable)")  },
  { _T("center"),   NULL,  &CHTMLEdView::OnUnsupportedHTML, _T("CENTER (Use (CSS)styles instead)")              },
  { _T("col"),      NULL,  &CHTMLEdView::OnUnsupportedHTML, _T("COL (Not yet supported by AuthorHTML)")         },
  { _T("colgroup"), NULL,  &CHTMLEdView::OnUnsupportedHTML, _T("COLGROUP (Not yet supported by AuthorHTML)")    },
  { _T("dfn"),      NULL,  &CHTMLEdView::OnUnsupportedHTML, _T("DFN is deprecated HTML (use I instead)")        },
  { _T("embed"),    NULL,  &CHTMLEdView::OnUnsupportedHTML, _T("EMBED is deprecated HTML (Use object instead)") },
  { _T("ilayer"),   NULL,  &CHTMLEdView::OnUnsupportedHTML, _T("ILAYER is deprecated HTML (Use DIV instead)")   },
  { _T("isindex"),  NULL,  &CHTMLEdView::OnUnsupportedHTML, _T("ISINDEX is deprecated HTML (Use INPUT instead)")},
  { _T("keygen"),   NULL,  &CHTMLEdView::OnUnsupportedHTML, _T("KEYGEN (deprecated, only NS 4.x)")              },
  { _T("layer"),    NULL,  &CHTMLEdView::OnUnsupportedHTML, _T("LAYER is deprecated HTML (Use DIV instead)")    },
  { _T("menu"),     NULL,  &CHTMLEdView::OnUnsupportedHTML, _T("MENU (Use lists instead)")                      },
  { _T("multicol"), NULL,  &CHTMLEdView::OnUnsupportedHTML, _T("MULTICOL (Use Cascading StyleSheets instead)")  },
  { _T("nobr"),     NULL,  &CHTMLEdView::OnUnsupportedHTML, _T("NOBR (Use 'break-below-images' instead)")       },
  { _T("noembed"),  NULL,  &CHTMLEdView::OnUnsupportedHTML, _T("NOEMBED (Deprecated HTML)")                     },
  { _T("noframes"), NULL,  &CHTMLEdView::OnUnsupportedHTML, _T("NOFRAMES (Deprecated HTML)")                    },
  { _T("nolayer"),  NULL,  &CHTMLEdView::OnUnsupportedHTML, _T("NOLAYER (Deprecated HTML)")                     },
  { _T("noscript"), NULL,  &CHTMLEdView::OnUnsupportedHTML, _T("NOSCRIPT (Deprecated HTML)")                    },
  { _T("param"),    NULL,  &CHTMLEdView::OnUnsupportedHTML, _T("PARAM (Use object instead)")                    },
  { _T("rt"),       NULL,  &CHTMLEdView::OnUnsupportedHTML, _T("RT - RubyText (not much used IE extension)")    },
  { _T("ruby"),     NULL,  &CHTMLEdView::OnUnsupportedHTML, _T("RUBY at rubytext (Not much used IE extension)") },
  { _T("strike"),   NULL,  &CHTMLEdView::OnUnsupportedHTML, _T("STRIKE (Deprecated strike-through)")            },
  { _T("server"),   NULL,  &CHTMLEdView::OnUnsupportedHTML, _T("SERVER (Deprecated HTML)")                      },
  { _T("spacer"),   NULL,  &CHTMLEdView::OnUnsupportedHTML, _T("SPACER (Deprecated HTML)")                      },
  { _T(""),         NULL,                                   NULL, _T("") }
};

/////////////////////////////////////////////////////////////////////////////
// CHTMLEdView construction/destruction
//
CHTMLEdView::CHTMLEdView()
            :m_bTableBorders(true)
            ,m_bTagsMode(false)
            ,m_bParaMode(false)
            ,m_bDesignMode(true)
            ,m_overwriteKey(false)
            ,m_bookmarkNumber(0)
            ,m_HITxPos(0)
            ,m_HITyPos(0)
            ,m_pEditSite(NULL)
            ,m_dirty(false)
            ,m_complete(false)
{
  m_Doc2.p = NULL;
}

CHTMLEdView::~CHTMLEdView()
{
  // Free spelling highlights
  ReleaseHighlights();
}

int 
CHTMLEdView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  if(CHtmlEditView::OnCreate(lpCreateStruct) == -1)
  {
    return -1;
  }
  return 0;
}

BOOL 
CHTMLEdView::CreateControlSite(COleControlContainer* pContainer
                              ,COleControlSite** ppSite, UINT /* nID */
                              ,REFCLSID /* clsid */)
{
  ASSERT(ppSite != NULL);

  //DEBUG
  *ppSite = m_pEditSite = new CHTMLEditControlSite(pContainer);
  return TRUE;
}

// Catch messages in an early stage to 
// process double clicks and special keys
//
BOOL CHTMLEdView::PreTranslateMessage(MSG* pMsg) 
{
  if (pMsg->message == WM_LBUTTONDOWN)
  {
    // Client area position for hittest-purposes of floating elements
    m_HITxPos = LOWORD(pMsg->lParam); 
    m_HITyPos = HIWORD(pMsg->lParam);
  }
  else if (pMsg->message == WM_LBUTTONDBLCLK)
  {
    // Client area position
    int xPos = LOWORD(pMsg->lParam); 
    int yPos = HIWORD(pMsg->lParam);
    // MK_CONTROL | MK_SHIFT | MK_LBUTTON | MK_RBUTTON | MK_MBUTTON
    //int keys = pMsg->wParam; 
    return OnDoubleClick(xPos,yPos);
  }
  else if ((pMsg->message == WM_KEYDOWN) && m_bDesignMode)
	{
		BOOL b_Ctrl  = (GetKeyState(VK_CONTROL) & 0x8000) > 0;
		BOOL b_Shift = (GetKeyState(VK_SHIFT)   & 0x8000) > 0;
		BOOL b_Alt   = (GetKeyState(VK_MENU)    & 0x8000) > 0;

    if(pMsg->wParam == VK_TAB)
    {
      // Extra TAB-key in a table
      if(!b_Ctrl && !b_Shift && !b_Alt)
      {
        return GotoNextCell();
      }
      if(b_Shift && !b_Ctrl && !b_Alt)
      {
        return GotoPreviousCell();
      }
      return FALSE;
    }
    if(pMsg->wParam == VK_DELETE)
    {
      // Default edit-mode does only backspace key for erasure
      if(!b_Ctrl && !b_Shift && !b_Alt)
      {
 			  return OnDeleteKey();
      }
    }
    if(pMsg->wParam == VK_BACK)
    {
      // Default edit-mode does only backspace key for erasure
      if(!b_Ctrl && !b_Shift && !b_Alt)
      {
        return OnBackspaceKey();
      }
    }
		if (!b_Shift && !b_Alt)
		{
      if (b_Ctrl)
      {
			  switch (pMsg->wParam)
			  {
			    case _T('N'):  // prevent stupid default behavior for CTRL N (opening Windows Explorer)
                    return TRUE;
			    case _T('T'):  // CTRL T: inserts a new table
                    OnTableInsert();
				            return TRUE;
  			  case _T('R'):  // CTRL R: new table row
	            		  OnTableInsertRowBelow();
				            return TRUE;
  			  case _T('O'):  // CTRL O: new table column
				            OnTableInsertColumnBefore();
				            return TRUE;
          case VK_RETURN: // CTRL-ENTER is a soft hyphen
                    OnSoftHyphen();
                    return TRUE;
			  }
      }
      else
      {
        // Not-shift, not-alt, not-ctrl
        // This is an ordinary key-hit
        // Check for SpellChecking in OnIdle() time
        switch(pMsg->wParam)
        {
          case VK_UP:
          case VK_DOWN:
          case VK_HOME:
          case VK_END:
          case VK_NEXT:
          case VK_PRIOR:
          case _T('.'):
          case _T(','):
          case _T(' '): m_dirty = true;
                    break;
        }
        if(pMsg->wParam > 127)
        {
          return OnDiacrite(pMsg->wParam);
        }
      }
		}
	}
	return FALSE;
}

// What? No default DEL key? Then we define it here:
// Delete the current text selection if one exists
// Else delete the next character.
BOOL
CHTMLEdView::OnDeleteKey()
{
  CComPtr<IHTMLSelectionObject> pSelObj;
  CComPtr<IHTMLTxtRange>        pTxtRange;

  HRESULT hr = m_Doc2->get_selection(&pSelObj);
  if (SUCCEEDED(hr)) 
  {
    hr = pSelObj->createRange((IDispatch**)&pTxtRange);
    if (SUCCEEDED(hr)) 
    {
      CComBSTR type;
      hr = pSelObj->get_type(&type);
      // Type must be "Text" or "None"
      if(SUCCEEDED(hr) && wcsicmp(type,L"control"))
      {
        CComBSTR bstrText;
        CString  theText;
        pTxtRange->get_text(&bstrText);
        theText = CW2CT(bstrText);
        if(theText.IsEmpty())
        {
          // Get a selection of one character and delete it
          CComBSTR unit = L"character";
          VARIANT_BOOL suc = VARIANT_FALSE;
          hr = pTxtRange->expand(unit,&suc);
          if(SUCCEEDED(hr) && suc == VARIANT_TRUE)
          {
            pSelObj->clear();
            return TRUE;
          }
        }
        else
        {
          // Clear current text selection
          // TODO: Check on HREF and SRC attributes -> Broken links
          pSelObj->clear();
          return TRUE;
        }
      }
    }
  }
  return FALSE;
}

BOOL
CHTMLEdView::OnBackspaceKey()
{
  CComPtr<IHTMLSelectionObject> pSelObj;
  CComPtr<IHTMLTxtRange>        pTxtRange;

  HRESULT hr = m_Doc2->get_selection(&pSelObj);
  if (SUCCEEDED(hr)) 
  {
    hr = pSelObj->createRange((IDispatch**)&pTxtRange);
    if (SUCCEEDED(hr)) 
    {
      CComBSTR type;
      hr = pSelObj->get_type(&type);
      // Type must be "Text" or "None"
      if(SUCCEEDED(hr) && wcsicmp(type,L"control"))
      {
        CComBSTR bstrText;
        CString  theText;
        pTxtRange->get_text(&bstrText);
        theText = CW2CT(bstrText);
        if(theText.IsEmpty())
        {
          return FALSE;
        }
        else
        {
          // Clear current text selection
          // TODO: Check on SRC and HREF attributes -> leads to broken links
          pSelObj->clear();
          return TRUE;
        }
      }
    }
  }
  return FALSE;
}

void
CHTMLEdView::OnSoftHyphen()
{
  PasteTextHTML(_T("&shy;"));
}

BOOL
CHTMLEdView::OnDiacrite(WPARAM letter)
{
  CString html = Misc::GetDiacriteHtml((uchar) letter);
  if(html.IsEmpty())
  {
    return FALSE;
  }
  PasteTextHTML(html);
  return TRUE;
}

// returns the <BODY> element in the document
HtmlElement 
CHTMLEdView::GetBody()
{
	CComPtr<IHTMLElement> i_Body;
	m_Doc2->get_body(&i_Body);
	return HtmlElement(i_Body);
}

// This function is called after each 
// CHtmlEditor::Navigate(), CHtmlEditor::NavigateToEmbeddedResource(), 
// cHtmlDocument::SetHtml() and cHtmlDocument::SetDesignMode()
void CHTMLEdView::OnNavigateComplete2(LPCTSTR t_URL)
{
  m_URL = t_URL;

  // Test if it is legall and will not result in the error page
  if(!m_URL.IsEmpty() && m_URL.CompareNoCase(_T("about:blank")))
  {
    CString filename;
    CString anchor;
    Misc::SplitMidpageAnchor(m_URL,filename,anchor);
    filename = Misc::StripFileProtocol(filename);

    if(_taccess(filename,4) == -1)
    {
      CString file = GetBase() + filename;
      if(_taccess(file,4) == -1)
      {
        theApp.ErrorMessage(_T("Cannot find or display the document: ") + m_URL);
        m_URL = _T("");
        PostMessage(WM_COMMAND,ID_FILE_CLOSE);
        return;
      }
    }
  }
	// ATTENTION: Always retrieve the document after a navigation !!
	// Internet Explorer generates a new document instance each time you navigate !!
	CComPtr<IDispatch> D_Doc = GetHtmlDocument();
  m_Doc2     = D_Doc;
  m_bCleanUp = false;

  ThemesEnable();
  // Allow editing
  SetDesignMode(TRUE);
  // 
  // Do not protect META name=GENERATOR content="MSHTML 6.x.y.z.p.q"
  ExecHelperSetVal(IDM_PROTECTMETATAGS,   (bool)false);
  SetOurGenerator();
  // Set focus on document
  CComQIPtr<IHTMLDocument4,&IID_IHTMLDocument4> doc4 = m_Doc2;
  doc4->focus();
  
  // Show borders of the tables
  m_bTableBorders = true;
  ShowTableBorders();
}

// Things to do on the document after everything is gotten and rendered
// OnNavigateComplete2 is too early to do this
void
CHTMLEdView::OnDownloadComplete()
{
  if(m_Doc2.p == NULL)
  {
    return;
  }
  // Tell the EditControlSite that we have a new document
  if(m_pEditSite)
  {
    m_pEditSite->InstallGrid(m_Doc2);
    m_pEditSite->Grid(FALSE);
  }
  // Now attach our edit designer
  m_designer.Detach();
  m_designer.Attach(this,m_Doc2,m_pEditSite);

  // All kind of settings on this document that we want in edit mode
  ExecHelperSetVal(IDM_KEEPSELECTION,     (bool)true);
  ExecHelperSetVal(IDM_MULTILEVELREDO,    (bool)true);
  ExecHelperSetVal(IDM_PRESERVEUNDOALWAYS,(bool)true);
  ExecHelperSetVal(IDM_2D_POSITION,       (bool)true);
  ExecHelperSetVal(IDM_LIVERESIZE,        (bool)true);

  ExtractStyles();
  CString noStyle = _T("<No style>");
  UpdateClassCombo(noStyle,noStyle);

  CHTMLEdDoc* document = this->GetDocument();
  CString temp = document->GetFromTemplate();
  if(!temp.IsEmpty())
  {
    Misc::MetaTag(m_Doc2,_T("AuthorHTMLTemplate"),&temp);
  }
  // Check for BASE tag
  CheckBase();

  // Connect drawing behaviors for map/area's on images
  ConnectIMGBehaviors();

  // Kick dirty for spell checking in OnIdle() time of new document
  m_dirty = true;
  // Document is now complete
  if(!m_URL.IsEmpty() && m_URL.CompareNoCase(_T("about:blank")))
  {
    // Document is now fully loaded. and commands can be performed
    m_complete = true;
  }
}

void 
CHTMLEdView::ThemesEnable()
{
  // [TODO]
  // Check for meta tag in the content
  // <META HTTP-EQUIV="MSThemeCompatible" Content="No">
}

BOOL 
CHTMLEdView::GetDesignMode()
{
	CComBSTR bs_Mode;
  if(m_Doc2.p)
  {
  	m_Doc2->get_designMode(&bs_Mode);
  }
	return (wcsicmp(bs_Mode, L"On") == 0);
}

// Overrides CHtmlEditView::Navigate
void
CHTMLEdView::Navigate(LPCTSTR t_URL
                     ,DWORD   dwFlags
                     ,LPCTSTR lpszTargetFrameName
                     ,LPCTSTR lpszHeaders
                     ,LPVOID  lpvPostData
                     ,DWORD   dwPostDataLen
                     )
{
  CHtmlEditView::Navigate(t_URL,dwFlags,lpszTargetFrameName,lpszHeaders,lpvPostData,dwPostDataLen);
}

// You can use this function to control in Browse mode where the user is allowed to browse to
// (in Design mode the user cannot click any links in the document)
void 
CHTMLEdView::OnBeforeNavigate2(LPCTSTR     t_URL
                              ,DWORD       //u32_Flags
                              ,LPCTSTR     //t_TargetFrame
                              ,CByteArray& //i_PostedData
                              ,LPCTSTR     //t_Headers
                              ,BOOL*       pb_Cancel)
{
	// t_URL = "about:blank" must ALWAYS be allowed
	// browsing to embedded resources "res://Path" must ALWAYS be allowed !!
	// browsing to files on the local harddisk should also be allowed
	// security risks are mainly on http and https pages (ftp:// should be no problem)
	if (_tcsnicmp(t_URL, _T("http"), 4) == 0 && (_tcslen(t_URL) > 8))
	{
    theApp.MessageBox(_T("External browsing prohibited"),_T("Warning"),MB_OK|MB_ICONSTOP);
		*pb_Cancel = TRUE;
	}

  // No longer complete
  m_complete = false;
}

void CHTMLEdView::OnUpdateColor(CCmdUI *pCmdUI)
{
	if(QueryStatus(IDM_FORECOLOR) & OLECMDF_ENABLED)
	{
  	pCmdUI->Enable();
	}
}

void
CHTMLEdView::OnRemoveHyperlink()
{
  CComPtr<IHTMLElement> hyperlink;
  if(InsideTag(_T("a"),hyperlink))
  {
    HtmlElement elem(hyperlink);
    CString linktext = elem.GetInnerHtml();
    CString linkto   = elem.GetAttribute(_T("href"));
    CString message;
    message.Format(_T("Remove the hyperlink reference to [%s] ?"),linkto.GetString());
    if(theApp.MessageBox(message,_T("Question"),MB_YESNO|MB_ICONQUESTION) == IDYES)
    {
      // Remove the link by setting innerHTML as outerHTML
      if(!elem.SetOuterHtml(linktext))
      {
        theApp.MessageBox(_T("Could not remove the hyperlink!"),_T("Error"),MB_OK|MB_ICONSTOP);
      }
    }
  }
}

// void
// CHTMLEdView::OnUpdateHyperlink(CCmdUI* pCmdUI)
// {
//   BOOL enab = QueryStatus(IDM_HYPERLINK) & OLECMDF_ENABLED;
//   pCmdUI->Enable(enab);  
// }

void
CHTMLEdView::OnUpdateComment(CCmdUI* pCmdUI)
{
  BOOL enab = TRUE; //QueryStatus(IDM_COMMENT) & OLECMDF_ENABLED;
  pCmdUI->Enable(enab);  
}

void 
CHTMLEdView::OnSpecialTest2()
{
}

void 
CHTMLEdView::OnSpecialTest()
{
}

/////////////////////////////////////////////////////////////////////////
//
// Check the spelling of text in your favourite language
//
/////////////////////////////////////////////////////////////////////////

void
CHTMLEdView::OnSpellCheckDocument()
{
  // Get colletion of all body elements to iterate over
  CComPtr<IHTMLElement> body;
  HRESULT hr = m_Doc2->get_body(&body);
  if(SUCCEEDED(hr) && body.p)
  {
    // TRACE("Start spell checking of document\n");
    int errors  = 0;
    int changed = 0;
    if(SpellCheck(body,errors,changed))
    {
      CString msg;
      msg.Format(_T("Ready spell-checking the document. Errors: %d Changed: %d"),errors,changed);
      theApp.MessageBox(msg,_T("Spell checker"),MB_OK|MB_ICONINFORMATION);
    }
    // Kick idle time spell checking
    m_dirty = true;
  }
}

bool
CHTMLEdView::SpellCheck(CComPtr<IHTMLElement> pElement
                       ,int& errors
                       ,int &changed)
{
  HRESULT hr;
  Spelling* spell = theApp.GetSpeller();
  if(!spell->GetOption(CHECKING_ENABLE))
  {
    CString language = spell->GetLanguageName();
    CString ask;
    ask.Format(_T("Checking of your spelling is not currently enabled.\n")
               _T("WOULD YOU LIKE TO ENABLE THIS FEATURE NOW?\n\n")
               _T("Your language will then be: %s\n")
               _T("(Change it via menu 'Edit/Spelling options...')"),language.GetString());
    if(theApp.MessageBox(ask,_T("Question"),MB_YESNO|MB_ICONQUESTION) == IDNO)
    {
      // Already ready
      return false;
    }
    spell->SetOption(CHECKING_ENABLE,true);
  }
  // Get a selection object to work on
  CComPtr<IHTMLSelectionObject> sel;
  m_Doc2->get_selection(&sel);

  // Create a text range object to work on
  CComPtr<IDispatch> disp1;
  sel->createRange(&disp1);
  CComQIPtr<IHTMLTxtRange,&IID_IHTMLTxtRange> total = disp1;

  // Put at beginning
  long actual1 = 0;
  long actual2 = 1;
  CComBSTR how;
  CComBSTR unit = CT2CW(_T("word"));
  CComBSTR sentence;
  total->moveToElementText(pElement);
  total->get_text(&sentence);
  if(sentence.m_str)
  {
    CComPtr<IDispatch> disp2;
    sel->createRange(&disp2);
    CComQIPtr<IHTMLTxtRange,&IID_IHTMLTxtRange> txt = disp2;

    txt->moveToElementText(pElement);
    how = CT2CW(_T("StartToStart"));
    hr = txt->setEndPoint(how,total);
    how = CT2CW(_T("EndToStart"));
    hr = txt->setEndPoint(how,total);
    hr = txt->moveEnd(unit,1,&actual1);
    while(actual1 == 1 && actual2 == 1)
    {
      CComBSTR bText;
      hr = txt->get_text(&bText);
      if(bText.m_str)
      {
        CString cText = CW2CT(bText);
        if(!cText.IsEmpty())
        {
          CString word = cText;
          word.TrimRight(_T(' '));
          if(word.GetLength() > 1)
          {
            // Spell check
            if(!spell->CheckWord(word))
            {
              // Make correct TxtRange
              CComBSTR bWord;
              CComPtr<IHTMLTxtRange> range;
              long actual;
              long numSpaces = word.GetLength() - cText.GetLength();
              txt->duplicate(&range);
              range->moveEnd(L"character",numSpaces,&actual);

              // Scroll into view before showing the dialog
              range->scrollIntoView();
              range->select();

              ++errors;
              SpellingDlg dialog(this,word,spell);
              switch(dialog.DoModal())
              {
                case IDOK:      word  = dialog.GetCorrectedWord();
                                bWord = CT2CW(word);
                                range->put_text(bWord);
                                ReleaseHighlight(word);
                                ++changed;
                                break;
                case IDCANCEL:  range->collapse();
                                return false;
                case IDCONTINUE:ReleaseHighlight(word);
                                break;
              }
              range->collapse();
            }
          }
        }
      }
      hr = txt->moveStart(unit,1,&actual2);
      hr = txt->moveEnd  (unit,1,&actual1);
    }
  }
  return true;
}

void
CHTMLEdView::OnSpellOptions()
{
  Spelling* spell = theApp.GetSpeller();
  CString orglang = spell->GetLanguageName();
  SpellOptionsDlg dialog(this,spell,true);
  if(dialog.DoModal() == IDOK)
  {
    CString language = dialog.GetLanguage();
    if(orglang.CompareNoCase(language))
    {
      spell->ChangeDictionary(language);
      // Kick idle time Spelling 
      m_dirty = true;
    }
  }
}

void
CHTMLEdView::SpellCheckOnIdle()
{
  if(m_dirty)
  {
    HighLightSpellErrors();
    m_dirty = false;
  }
}

void
CHTMLEdView::HighLightSpellErrors()
{
  // Remove previous highlights
  ReleaseHighlights();

  Spelling* spell = theApp.GetSpeller();
  if(!spell->GetOption(CHECKING_ENABLE) ||
     !spell->GetOption(SHOW_SPELLERRORS) )
  {
    // Not enabled or a no-show case. Do nothing
    return;
  }
  spell->MakeRenderstyle(m_Doc2);
  // Check now that a render style exists (otherwise IE crashes later-on)
  if(!spell->GetRenderStyle().p)
  {
    // No render style
    MainFrame::SetStatusText(_T("No render styles available to show spelling errors."));
    return;
  }
  HRESULT hr;

  CComPtr<IHTMLElement> body;
  hr = m_Doc2->get_body(&body);
  if(!SUCCEEDED(hr) || !body.p)
  {
    // No body of the document to check on errors
    return;
  }
  // Get a selection object to work on
  CComPtr<IHTMLSelectionObject> sel;
  m_Doc2->get_selection(&sel);

  // Create a text range object to work on
  CComPtr<IDispatch> disp1;
  sel->createRange(&disp1);
  CComQIPtr<IHTMLTxtRange,&IID_IHTMLTxtRange> total = disp1;

  // Get Display services
  CComPtr<IDisplayServices> disp;
  m_Doc2->QueryInterface(IID_IDisplayServices,(void **)&disp);
  // Get Markup services
  CComPtr<IMarkupServices> mark;
  m_Doc2->QueryInterface(IID_IMarkupServices,(void **)&mark);
  // Get rendering services
  CComPtr<IHighlightRenderingServices> render;
  m_Doc2->QueryInterface(IID_IHighlightRenderingServices,(void **)&render);

  // Put at beginning
  long actual1 = 0;
  long actual2 = 1;
  CComBSTR how;
  CComBSTR unit = CT2CW(_T("word"));
  CComBSTR sentence;
  total->moveToElementText(body);
  total->get_text(&sentence);
  if(sentence.m_str)
  {
    CComPtr<IDispatch> disp2;
    sel->createRange(&disp2);
    CComQIPtr<IHTMLTxtRange,&IID_IHTMLTxtRange> txt = disp2;

    txt->moveToElementText(body);
    how = CT2CW(_T("StartToStart"));
    txt->setEndPoint(how,total);
    how = CT2CW(_T("EndToStart"));
    txt->setEndPoint(how,total);
    txt->moveEnd(unit,1,&actual1);
    while(actual1 == 1 && actual2 == 1)
    {
      CComBSTR bText;
      hr = txt->get_text(&bText);
      if(bText.m_str)
      {
        CString cText = CW2CT(bText);
        if(!cText.IsEmpty())
        {
          CString word = cText;
          word.TrimRight(_T(' '));
          if(word.GetLength() > 1)
          {
            // Spell check
            if(!spell->CheckWord(word))
            {
              // Highlight it!!

              // Create a duplicate TxtRange to solve the problem
              // of trailing space getting wavy underlines!
              CComPtr<IHTMLTxtRange> range;
              long actual;
              long numSpaces = word.GetLength() - cText.GetLength();
              txt->duplicate(&range);
              range->moveEnd(L"character",numSpaces,&actual);
              // Create 2 DisplayPointers and 2 Markup Pointers
              CComPtr<IDisplayPointer> disp_left;
              CComPtr<IDisplayPointer> disp_right;
              CComPtr<IMarkupPointer>  mark_left;
              CComPtr<IMarkupPointer>  mark_right;
              disp->CreateDisplayPointer(&disp_left);
              disp->CreateDisplayPointer(&disp_right);
              mark->CreateMarkupPointer (&mark_left);
              mark->CreateMarkupPointer (&mark_right);

              // Set markup pointers to textrange
              mark->MovePointersToRange(range,mark_left,mark_right);
              // Move display pointers to markup pointers
              disp_left ->MoveToMarkupPointer(mark_left,NULL);
              disp_right->MoveToMarkupPointer(mark_right,NULL);
              // create segement
              IHighlightSegment* segment;
              hr = render->AddSegment(disp_left
                                     ,disp_right
                                     ,spell->GetRenderStyle()
                                     ,&segment);
              if(SUCCEEDED(hr))
              {
                // Save highlight segment in map
                segment->AddRef();
                m_highLights.insert(std::make_pair(word,segment));
              }
            }
          }
        }
      }
      // Move on to the next word...
      hr = txt->moveStart(unit,1,&actual2);
      hr = txt->moveEnd  (unit,1,&actual1);
    }
  }
}

// Release all highlights from the text
// To be done on destruction time or when we
// turn the checking off.
void
CHTMLEdView::ReleaseHighlights()
{
  if(m_Doc2)
  {
    CComPtr<IHighlightRenderingServices> serv;
    m_Doc2->QueryInterface(IID_IHighlightRenderingServices,(void **)&serv);

    HighLights::iterator it;
    for(it = m_highLights.begin();it != m_highLights.end(); ++it)
    {
      IHighlightSegment* seg = it->second;
      serv->RemoveSegment(seg);
      seg->Release();
    }
  }
  m_highLights.clear();
}

// Release one word from the highlights
// To be done on the add/ignore of a word
void
CHTMLEdView::ReleaseHighlight(CString& word)
{
  if(m_Doc2)
  {
    CComPtr<IHighlightRenderingServices> serv;
    m_Doc2->QueryInterface(IID_IHighlightRenderingServices,(void **)&serv);

    HighLights::iterator start;
    HighLights::iterator stop;
    start = m_highLights.lower_bound(word);
    stop  = m_highLights.upper_bound(word);

    while(start != stop && start != m_highLights.end())
    {
      IHighlightSegment* seg = start->second;
      serv->RemoveSegment(seg);
      seg->Release();
      // Next same word
      ++start;
    }
  }
}

////////////////////////////////////////////////////////////////
//
// Finding and replacing text
//
////////////////////////////////////////////////////////////////

void
CHTMLEdView::OnFind()
{
  OnSearchText(true);
}

void
CHTMLEdView::OnReplace()
{
  OnSearchText(false);
}

void
CHTMLEdView::OnSearchText(bool findOnly)
{
  HRESULT hr = S_FALSE;

  // Get colletion of all body elements to iterate over
  CComPtr<IHTMLElement> body;
  CComPtr<IDispatch> dispchild;
  m_Doc2->get_body(&body);
  hr = body->get_children(&dispchild);
  CComQIPtr<IHTMLElementCollection,&IID_IHTMLElementCollection> coll = dispchild;
  if(SUCCEEDED(hr) && coll.p)
  {
    FindReplaceDialog diag(findOnly,_T(""),_T(""),this);
    INT_PTR action = diag.DoModal();
    if(action == IDOK || 
       action == IDC_BUTTON_FINDNEXT    ||
       action == IDC_BUTTON_REPLACEWITH ||
       action == IDC_BUTTON_REPLACEALL  )
    {
      if(action == IDC_BUTTON_REPLACEALL || action == IDC_BUTTON_REPLACEWITH)
      {
          findOnly = false;
      }
      bool    didNothing    = true;
      CString textToFind    = diag.GetFindText();
      CString textToReplace = diag.GetReplaceText();
      UINT    iOptions      = diag.GetOptions();
      SearchInCollection(coll,findOnly,textToFind,textToReplace,didNothing,iOptions,action);

      if(didNothing)
      {
        CString message;
        message.Format(_T("Cannot find the text \"%s\" in your document(s)"),textToFind.GetString());
        theApp.MessageBox(message,_T("Find result"),MB_OK|MB_ICONINFORMATION);
      }
    }
  }
}

int
CHTMLEdView::SearchInCollection(CComPtr<IHTMLElementCollection> coll
                               ,bool&    bFindOnly
                               ,CString& textToFind
                               ,CString& textToReplace
                               ,bool&    bDidNothing
                               ,UINT&    iOptions
                               ,INT_PTR& iActions)
{
  long count    = 0;
  HRESULT hr    = S_FALSE;
  bool bForeward = !(iOptions & FRD_SEARCHUP);

  // Get a selection
  CComPtr<IHTMLSelectionObject> selection;
  m_Doc2->get_selection(&selection);

  // Get a TXTRange
  CComPtr<IDispatch> disp;
  selection->createRange(&disp);
  CComQIPtr<IHTMLTxtRange,&IID_IHTMLTxtRange> range = disp;
  
  coll->get_length(&count);
  int ind = bForeward ? 0 : (count - 1);
  while(( bForeward && ind < count) ||
        (!bForeward && ind >= 0))
  {
    CComPtr<IDispatch> disp2;
    CComVariant number((long)ind);
//     V_VT(&number) = VT_I4;
//     V_I4(&number) = ind;

    hr = coll->item(number,number,&disp2);
    CComQIPtr<IHTMLElement,&IID_IHTMLElement> pElement = disp2;
    if(SUCCEEDED(hr) && pElement.p)
    {
      // Get Tag name
      CComBSTR bTagName;
      pElement->get_tagName(&bTagName);
      CString tagName = bTagName;
      if((tagName.CompareNoCase(_T("TR"))       == 0) ||
         (tagName.CompareNoCase(_T("TABLE"))    == 0) ||
         (tagName.CompareNoCase(_T("THEAD"))    == 0) ||
         (tagName.CollateNoCase(_T("TBODY"))    == 0) ||
         (tagName.CompareNoCase(_T("TFOOT"))    == 0) ||
         (tagName.CompareNoCase(_T("FORM"))     == 0) ||
         (tagName.CompareNoCase(_T("FIELDSET")) == 0) )
      {
        // See if we have children
        CComPtr<IDispatch> elemDisp;
        pElement->get_children(&elemDisp);
        CComQIPtr<IHTMLElementCollection,&IID_IHTMLElementCollection> coll2 = elemDisp;
        long childCount = 0;
        coll2->get_length(&childCount);
        if(childCount > 0)
        {
          // recurse
          if(!SearchInCollection(coll2,bFindOnly,textToFind,textToReplace,bDidNothing,iOptions,iActions))
          {
            return false;
          }
        }
      }
      else
      {
        // Iterate over this element
        CComBSTR curSentence;
        range->moveToElementText(pElement);
        range->get_text(&curSentence);
        CString sCurSentence = curSentence;
        // Do with this sentence
        if(!sCurSentence.IsEmpty())
        {
          long startPos = bForeward ? 0 : sCurSentence.GetLength();
          do
          {
            long pos    = 0;
            long actual = 0;
            if(iOptions & FRD_USEREGULAR)
            {
              RegExp expr(textToFind);
              pos = expr.Match(sCurSentence);
            }
            else
            {
              if(iOptions & FRD_MATCHCASE)
              {
                if(bForeward)
                {
                  pos = sCurSentence.Find(textToFind,startPos);
                }
                else
                {
                  pos = Misc::FindReverse(sCurSentence,textToFind,startPos) - textToFind.GetLength();
                }
              }
              else
              { 
                if(bForeward)
                {
                  pos = Misc::FindNoCase(sCurSentence,textToFind,startPos);
                }
                else
                {
                  pos = Misc::FindReverseNoCase(sCurSentence,textToFind,startPos) - textToFind.GetLength();
                }
              }
              if(pos >= 0 && (iOptions & FRD_MATCHWORD))
              {
                // Only a match if we have a whole word found.
                if(pos > 0 && _istalnum(sCurSentence.GetAt(pos - 1)))
                {
                  // Not at the beginning of a word/sentence
                  pos = -1;
                }
                else if((pos + textToFind.GetLength() < sCurSentence.GetLength()) &&  
                         _istalnum(sCurSentence.GetAt(pos + textToFind.GetLength())))
                {
                  // Not at the end of a word/sentence
                  pos = -1;
                }
              }
            }
            if(pos >= 0)
            {
              // Did at least something....
              bDidNothing = false;

              CComBSTR character = L"character";
              int newlines = Misc::CountNL(sCurSentence,pos);
              range->moveStart(character,(pos - newlines),&actual);
              range->setEndPoint(L"EndToStart",range);
              range->moveEnd  (character,textToFind.GetLength(),&actual);
              range->select();

              // CARET MOVE
              // Now move our caret to the beginning of the range
              CComPtr<IDisplayServices> pServ;
              CComPtr<IDisplayPointer>  pDisp;
              CComPtr<IMarkupServices>  spMS;
              CComPtr<IMarkupPointer>   pMup;
              CComPtr<IHTMLCaret>       pCaret;
              hr = m_Doc2->QueryInterface(IID_IMarkupServices, (void**)&spMS);
              hr = m_Doc2->QueryInterface(IID_IDisplayServices,(void**)&pServ);
              hr = pServ->GetCaret(&pCaret);
              hr = pServ->CreateDisplayPointer(&pDisp);
              hr = spMS->CreateMarkupPointer(&pMup);

              pMup->MoveAdjacentToElement(pElement,ELEM_ADJ_AfterBegin);
              for(int mvind = 0; mvind < (pos - newlines); ++mvind)
              {
                pMup->MoveUnit(MOVEUNIT_NEXTCHAR);
              }
              pDisp->MoveToMarkupPointer(pMup,NULL);
              pCaret->MoveCaretToPointer(pDisp,TRUE,CARET_DIRECTION_INDETERMINATE);
              // END CARET MOVE

              if(iActions != IDC_BUTTON_REPLACEALL)
              {
                // Only ask again if not silently do all!
                FindReplaceDialog diag(bFindOnly,textToFind,textToReplace,this);
                iActions      = diag.DoModal();
                textToFind    = diag.GetFindText();
                textToReplace = diag.GetReplaceText();
                iOptions      = diag.GetOptions();
                bForeward     = !(iOptions & FRD_SEARCHUP);
              }
              if(iActions == IDC_BUTTON_REPLACEWITH || iActions == IDC_BUTTON_REPLACEALL)
              {
                bFindOnly = false;
                CComBSTR replace = CT2CW(textToReplace);
                range->put_text(replace); 
                range->moveToElementText(pElement);
                range->get_text(&curSentence);
                sCurSentence = curSentence;
              }
              else
              {
                // Again on BUTTON_FIND
                bFindOnly = true;
              }
              if(iActions == IDCANCEL || iActions == IDC_BUTTON_FINDNEXT)
              {
                // Clear the selection
                range->collapse();
                selection->empty();
              }
              if(iActions == IDOK || iActions == IDCANCEL)
              {
                // end (with or without selection)
                return false;
              }
            }
            range->moveToElementText(pElement);

            startPos = pos + (bForeward ? 1 : -1);
          }
          while(startPos > 0);
        }
      }
    }
    // Next object in the hierarchy
    // Foreward or backward
    ind += bForeward ? 1 : -1;
  }
  return true;
}

void 
CHTMLEdView::ExtractStyles()
{
	MainFrame* _tmain = (MainFrame*) theApp.m_pMainWnd;
	ASSERT_VALID(_tmain);
	CMFCToolBarComboBoxButton* combo = _tmain->GetClassNameCombo();
  if(m_Doc2 && combo)
  {
    Misc::ExtractStylesList(m_Doc2,combo);
  }
}

// Handle the VK_INSERT key
//
void CHTMLEdView::OnInsertKey()
{
  HRESULT hr;
  CComVariant var;
  m_overwriteKey  = !m_overwriteKey;
  V_VT(&var)   = VT_BOOL;
  V_BOOL(&var) = m_overwriteKey ? VARIANT_TRUE : VARIANT_FALSE;
  hr = ExecCommand(&CGID_MSHTML
                  ,IDM_OVERWRITE
                  ,MSOCMDEXECOPT_DODEFAULT
                  ,&var
                  ,NULL);
  OnUpdateInsertKey();
}

////////////////////////////////////////////////////////////////
//
// BASIC HANDLERS
//
////////////////////////////////////////////////////////////////


extern CMFCStatusBar* g_statusBar;

void CHTMLEdView::OnUpdateInsertKey()
{
  if(g_statusBar)
  {
    CString text = m_overwriteKey ? _T("OVR") : _T("");
    g_statusBar->SetPaneText(2,text); // , TRUE);
  }
}

void
CHTMLEdView::OnPrintPreview()
{
  ExecHelperNN(IDM_PRINTPREVIEW);
}

void
CHTMLEdView::OnPageSetup()
{
  ExecHelperNN(IDM_PAGESETUP);
}

void
CHTMLEdView::OnCut()
{
  ExecHelperNN(IDM_CUT);
}

void
CHTMLEdView::OnCopy()
{
  ExecHelperNN(IDM_COPY);
}

void
CHTMLEdView::OnPaste()
{
  ExecHelperNN(IDM_PASTE);
}

void
CHTMLEdView::OnUndo()
{
  ExecHelperNN(IDM_UNDO);
}

void
CHTMLEdView::OnRedo()
{
  ExecHelperNN(IDM_REDO);
}

void
CHTMLEdView::OnSelectAll()
{
  ExecHelperNN(IDM_SELECTALL);
}

void
CHTMLEdView::OnNoFormat()
{
  ExecHelperNN(IDM_REMOVEFORMAT);
}

void
CHTMLEdView::OnLeftJustify()
{
  ExecHelperNN(IDM_JUSTIFYLEFT);
}

void
CHTMLEdView::OnCenterJustify()
{
  ExecHelperNN(IDM_JUSTIFYCENTER);
}

void
CHTMLEdView::OnRightJustify()
{
  ExecHelperNN(IDM_JUSTIFYRIGHT);
}

void
CHTMLEdView::OnJustify()
{
  ExecHelperNN(IDM_JUSTIFYFULL);
}

void 
CHTMLEdView::OnAbsolute()
{
  SetAbsolutePosition(true);
}

void 
CHTMLEdView::OnStatic()
{
  SetAbsolutePosition(false);
}

void 
CHTMLEdView::OnBold()
{
  ExecHelperNN(IDM_BOLD);
}

void
CHTMLEdView::OnItalic()
{
  ExecHelperNN(IDM_ITALIC);
}

void
CHTMLEdView::OnUnderline()
{
  ExecHelperNN(IDM_UNDERLINE);
}

void 
CHTMLEdView::OnBulletList()
{
  ExecHelperNN(IDM_UNORDERLIST);
}

void
CHTMLEdView::OnFont()
{
  ExecHelperNN(IDM_FONT);
}

void 
CHTMLEdView::OnNumberList()
{
  ExecHelperNN(IDM_ORDERLIST);
}

void 
CHTMLEdView::OnIndent()
{
  ExecHelperNN(IDM_INDENT);
}

void 
CHTMLEdView::OnOutdent()
{
  ExecHelperNN(IDM_OUTDENT);
}

#pragma warning(disable: 4800)

void 
CHTMLEdView::OnUpdateAbsolute(CCmdUI* pCmdUI)
{
  bool check  = (bool)(QueryStatus(IDM_2D_ELEMENT)        & OLECMDF_LATCHED);
  bool enab   = (bool)(QueryStatus(IDM_ABSOLUTE_POSITION) & OLECMDF_ENABLED);
  pCmdUI->SetCheck(check);
  pCmdUI->Enable(enab);
}

void 
CHTMLEdView::OnUpdateStatic(CCmdUI* pCmdUI)
{
  bool check  = (bool)(QueryStatus(IDM_1D_ELEMENT)        & OLECMDF_LATCHED);
  bool enab   = (bool)(QueryStatus(IDM_ABSOLUTE_POSITION) & OLECMDF_ENABLED);
  pCmdUI->SetCheck(check);
  pCmdUI->Enable(enab);
}

void
CHTMLEdView::OnUpdateLeftJustify(CCmdUI* pCmdUI)
{
  // Latched is Toggle & ON
  bool check  = (bool)(QueryStatus(IDM_JUSTIFYLEFT) & OLECMDF_LATCHED);
  pCmdUI->SetCheck(check);
}

void
CHTMLEdView::OnUpdateCenterJustify(CCmdUI* pCmdUI)
{
  // Latched is Toggle & ON
  bool check  = (bool)(QueryStatus(IDM_JUSTIFYCENTER) & OLECMDF_LATCHED);
  pCmdUI->SetCheck(check);
}

void
CHTMLEdView::OnUpdateRightJustify(CCmdUI* pCmdUI)
{
  // Latched is Toggle & ON
  bool check  = (bool)(QueryStatus(IDM_JUSTIFYRIGHT) & OLECMDF_LATCHED);
  pCmdUI->SetCheck(check);
}

void
CHTMLEdView::OnUpdateJustify(CCmdUI* pCmdUI)
{
  // Latched is Toggle & ON
  bool check  = (bool)(QueryStatus(IDM_JUSTIFYFULL) & OLECMDF_LATCHED);
  pCmdUI->SetCheck(check);
}

void
CHTMLEdView::OnUpdateNoFormat(CCmdUI* pCmdUI)
{
  bool check  = (bool)(QueryStatus(IDM_REMOVEFORMAT) & OLECMDF_ENABLED);
  pCmdUI->Enable(check);
}

void
CHTMLEdView::OnUpdateUndo(CCmdUI* pCmdUI)
{
  bool check  = (bool)(QueryStatus(IDM_UNDO) & OLECMDF_ENABLED);
  pCmdUI->Enable(check);
}

void
CHTMLEdView::OnUpdateRedo(CCmdUI* pCmdUI)
{
  bool check  = (bool)(QueryStatus(IDM_REDO) & OLECMDF_ENABLED);
  pCmdUI->Enable(check);
}

void
CHTMLEdView::OnUpdateSelectAll(CCmdUI* pCmdUI)
{
  bool check  = (bool)(QueryStatus(IDM_SELECTALL) & OLECMDF_ENABLED);
  pCmdUI->Enable(check);
}

void
CHTMLEdView::OnUpdateCut(CCmdUI* pCmdUI)
{
  bool check  = (bool)(QueryStatus(IDM_CUT) & OLECMDF_ENABLED);
  pCmdUI->Enable(check);
}

void
CHTMLEdView::OnUpdateCopy(CCmdUI* pCmdUI)
{
  bool check  = (bool)(QueryStatus(IDM_COPY) & OLECMDF_ENABLED);
  pCmdUI->Enable(check);
}

void
CHTMLEdView::OnUpdatePaste(CCmdUI* pCmdUI)
{
  bool check  = (bool)(QueryStatus(IDM_PASTE) & OLECMDF_ENABLED);
  pCmdUI->Enable(check);
}

void
CHTMLEdView::OnUpdatePageSetup(CCmdUI* pCmdUI)
{
  bool check  = (bool)(QueryStatus(IDM_PAGESETUP) & OLECMDF_ENABLED);
  pCmdUI->Enable(check);
}

void 
CHTMLEdView::OnUpdatePrintPreview(CCmdUI* pCmdUI)
{
  bool check  = (bool)(QueryStatus(IDM_PRINTPREVIEW) & OLECMDF_ENABLED);
  pCmdUI->Enable(check);
}

void
CHTMLEdView::OnUpdateBold(CCmdUI* pCmdUI)
{
  // Latched is Toggle & ON
  bool check  = (bool)(QueryStatus(IDM_BOLD) & OLECMDF_LATCHED);
  pCmdUI->SetCheck(check);
}

void 
CHTMLEdView::OnUpdateItalic(CCmdUI* pCmdUI)
{
  // Latched is Toggle & ON
  bool check  = (bool)(QueryStatus(IDM_ITALIC) & OLECMDF_LATCHED);
  pCmdUI->SetCheck(check);
}

void 
CHTMLEdView::OnUpdateUnderline(CCmdUI* pCmdUI)
{
  // Latched is Toggle & ON
  bool check  = (bool)(QueryStatus(IDM_UNDERLINE) & OLECMDF_LATCHED);
  pCmdUI->SetCheck(check);
}

void 
CHTMLEdView::OnUpdateBulletList(CCmdUI* pCmdUI)
{
  // Latched is Toggle & ON
  bool check  = (bool)(QueryStatus(IDM_UNORDERLIST) & OLECMDF_LATCHED);
  pCmdUI->SetCheck(check);
}

void 
CHTMLEdView::OnUpdateNumberList(CCmdUI* pCmdUI)
{
  // Latched is Toggle & ON
  bool check  = (bool)(QueryStatus(IDM_ORDERLIST) & OLECMDF_LATCHED);
  pCmdUI->SetCheck(check);
}

void 
CHTMLEdView::OnUpdateIndent(CCmdUI* pCmdUI)
{
  bool check  = (bool)(QueryStatus(IDM_INDENT) & OLECMDF_ENABLED);
  pCmdUI->Enable(check);
}

void 
CHTMLEdView::OnUpdateOutdent(CCmdUI* pCmdUI)
{
  bool check  = (bool)(QueryStatus(IDM_OUTDENT) & OLECMDF_ENABLED);
  pCmdUI->Enable(check);
}

void 
CHTMLEdView::OnBookmark()
{
  bool    inside;
  CString oldMark;
  CComPtr<IHTMLElement> pElement;

  // Check if there was already a bookmark.
  // Works only by retrieving the attribute "name" from the element
  // anchor->name doesn't work after IE 6.0, so IDM_BOOKMARK (get name) does not work
  inside = InsideTag(_T("a"),pElement);
  if(inside)
  {
    oldMark = GetAttribute(pElement,_T("name"));
  }
  if(!oldMark.IsEmpty())
  {
    // Mark as current and do the dialog
    BookmarkDialog diag(oldMark,_T(""),m_Doc2,this);
    diag.DoModal();

  }
  else if(!inside)
  {
    // Get default text for bookmark. Protect against empty tags 
    // or malformed ?????? tags from empty COM strings.
    CString marker = GetCurrentSelectionText();
    if(marker.IsEmpty() || !_istalpha(marker[0]))
    {
      marker.Format(_T("Bookmark%d"),++m_bookmarkNumber);
    }
    marker.Replace(_T(" "),_T(""));
    // Choose name for a marker.
    BookmarkDialog diag(_T(""),marker,m_Doc2,this);
    diag.DoModal();
  }
  else // Inside and no anchor tag
  {
    // We are inside a hyperlink. Don't remove a bookmark or set one
  }
}

// See if we are inside a bookmark (not a hyperlink!!)
void
CHTMLEdView::OnUpdateAnchor(CCmdUI* pCmdUI)
{
  CComPtr<IHTMLElement> pElement;
  bool inBookmark = false;
  bool inside = InsideTag(_T("a"),pElement);
  if(inside)
  {
    CString marker = GetAttribute(pElement,_T("name"));
    CString href   = GetAttribute(pElement,_T("href"));
    if(href.IsEmpty() && !marker.IsEmpty())
    {
      inBookmark = true;
    }
  }
  else
  {
    // Possibly convert current selection to a bookmark
    CString marker = GetCurrentSelectionText();
    if(!marker.IsEmpty())
    {
      inBookmark = true;
    }
  }
  pCmdUI->Enable(inBookmark);
}

// See if we are inside a bookmark (not a hyperlink!!)
void
CHTMLEdView::OnUpdateHyperlink(CCmdUI* pCmdUI)
{
  CComPtr<IHTMLElement> pElement;
  bool inHyper = false;
  bool inside = InsideTag(_T("a"),pElement);
  if(inside)
  {
    CString marker = GetAttribute(pElement,_T("name"));
    CString href   = GetAttribute(pElement,_T("href"));
    if(!href.IsEmpty() && marker.IsEmpty())
    {
      inHyper = true;
    }
  }
  else
  {
    // Possibly convert current selection to a hyperlink
    CString marker = GetCurrentSelectionText();
    if (!marker.IsEmpty())
    {
      inHyper= true;
    }
  }
  pCmdUI->Enable(inHyper);
}

void
CHTMLEdView::OnUpdateUISelection(CCmdUI* pCmdUI)
{
  pCmdUI->Enable(!GetCurrentSelectionText().IsEmpty());
}

void
CHTMLEdView::OnUpdateUINoSelection(CCmdUI* pCmdUI)
{
  pCmdUI->Enable(GetCurrentSelectionText().IsEmpty());
}

/////////////////////////////////////////////////////////////////////////////
// CHTMLEdView diagnostics

#ifdef _DEBUG
void CHTMLEdView::AssertValid() const
{
	//CHtmlEditView::AssertValid();
}

void CHTMLEdView::Dump(CDumpContext& dc) const
{
	CHtmlEditView::Dump(dc);
}

CHTMLEdDoc* CHTMLEdView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CHTMLEdDoc)));
	return (CHTMLEdDoc*)m_pDocument;
}
#endif //_DEBUG

HRESULT CHTMLEdView::OnUpdateUI()
{
	long id = GetDlgCtrlID();
	if(id==AFX_IDW_PANE_LAST) //we change the ID if we hide this window
  {
    return S_OK;
  }
	if(S_OK == GetIsDirty())
  {
    GetDocument()->SetModifiedFlag();
  }
  CString tag,classname;
  UpdateStructure(tag,classname);
  UpdateClassCombo(tag,classname);
	UpdateFontCombos();
	AfxGetMainWnd()->PostMessage(WM_KICKIDLE);
	return S_OK;
}

#pragma warning (disable : 4311)

HRESULT 
CHTMLEdView::OnShowContextMenu(DWORD      /*dwID*/
							                ,LPPOINT    ppt
							                ,LPUNKNOWN  /*pcmdtReserved*/
							                ,LPDISPATCH /*pdispReserved*/)
{
  CComPtr<IHTMLElement> pElem;
  CComPtr<IHTMLElement> pImage;
  CComPtr<IHTMLElement> pForm;
  CComPtr<IHTMLElement> pField;
  CComPtr<IHTMLElement> pText;
  CComPtr<IHTMLElement> pLabel;
  CComPtr<IHTMLElement> pSelect;
  CComPtr<IHTMLElement> pCaption;
  CComPtr<IHTMLElement> pLayer;
  CComPtr<IHTMLElement> pArea;
    
  // OnContextmenu works only as expected by moving the caret
  // to the position of the context menu.
  POINT piv;
  piv.x = ppt->x;
  piv.y = ppt->y;
  ScreenToClient(&piv);
  Misc::MoveCaretToPoint(m_Doc2,&piv);
    
  bool inTable = InTable();
  int  firstPosition = 4; // Paragraph

  // Load the menu
  CMenu htmlMenu;
  VERIFY(htmlMenu.LoadMenu(IDR_HTML_POPUP));
  CMenu* popup = htmlMenu.GetSubMenu(0);

  if(InsideTag(_T("P")))
  {
    ++firstPosition;
  }
  else
  {
    popup->RemoveMenu(firstPosition,MF_BYPOSITION);
  }
  firstPosition += 4;

  // ALL TAGS
  MENUITEMINFO info;
  vector<IHTMLElement*> allTags;
  info.cbSize = sizeof(MENUITEMINFO);
  info.fMask = MIIM_SUBMENU;
  VERIFY(popup->GetMenuItemInfo(firstPosition,&info,TRUE));
  CMenu tagsPopup;
  if(info.hSubMenu)
  {
    tagsPopup.Attach(info.hSubMenu);
    GetTagsMenu(&tagsPopup,allTags);
  }
  //
  firstPosition += 2;
  // TABLE PART
  if(!inTable)
  {
    for(int n = 0; n < 6; ++n)
    {
      popup->RemoveMenu(firstPosition,MF_BYPOSITION);
    }
  }
  else
  {
    if(InsideTag(_T("caption"),pCaption))
    {
      ++firstPosition;
      for(int n = 0; n < 4; ++n)
      {
        popup->RemoveMenu(firstPosition,MF_BYPOSITION);
      }
    }
    else
    {
      // Not in caption but in row-groups
      popup->RemoveMenu(firstPosition,MF_BYPOSITION);
      firstPosition += 5;
    }
  }
  if(InsideTag(_T("A"),pElem))
  {
    CComBSTR href = L"href";
    CComVariant  value;
    V_VT(&value) = VT_BSTR;
    pElem->getAttribute(href,0,&value);
    CString hyperlink = V_BSTR(&value);

    if(hyperlink.IsEmpty())
    {
      // Leave bookmark....
      ++firstPosition;
      popup->RemoveMenu(firstPosition,MF_BYPOSITION);
      popup->RemoveMenu(firstPosition,MF_BYPOSITION);
    }
    else
    {
      // Leave Hyperlink and EditHyperlink
      popup->RemoveMenu(firstPosition,MF_BYPOSITION);
      firstPosition += 2;
    }
    ++firstPosition;
  }
  else
  {
    for(int n = 0; n < 4; ++n)
    {
      popup->RemoveMenu(firstPosition,MF_BYPOSITION);
    }
  }
  bool appendImage = false;
  bool removeSep   = true;

  if(InsideTag(_T("IMG"),pImage))
  {
    appendImage = true;
  }
  else
  {
    // Hittest from document
    HRESULT hr = m_Doc2->elementFromPoint(piv.x,piv.y,&pImage);
    if(SUCCEEDED(hr) && pImage.p)
    {
      if(InsideTag(pImage,_T("img"),pImage))
      {
        appendImage = true;
      }
      else
      {
        // Advanced hittest for floating objects
        if(Misc::GetElementByPositionAndTag(m_Doc2,pImage,piv.x,piv.y,_T("img")))
        {
          appendImage = true;
        }
      }
    }
  }
  if(appendImage)
  {
    removeSep = false;
    ++firstPosition;
  }
  else
  {
    popup->RemoveMenu(firstPosition,MF_BYPOSITION);
  }

  bool appendArea = false;
  if(InsideTag(_T("AREA"),pArea))
  {
    appendArea = true;
  }
  else
  {
    // Hittest from document
//  HRESULT hr = m_Doc2->elementFromPoint(m_HITxPos,m_HITyPos,&pArea);
    HRESULT hr = m_Doc2->elementFromPoint(piv.x,piv.y,&pArea);
    if(SUCCEEDED(hr) && pArea.p)
    {
      if(InsideTag(pArea,_T("area"),pArea))
      {
        appendArea = true;
      }
      else
      {
        // Advanced hittest for floating objects
        if(Misc::GetElementByPositionAndTag(m_Doc2,pArea,piv.x,piv.x,_T("area")))
        {
          appendArea = true;
        }
      }
    }
  }
  if(appendArea)
  {
    firstPosition += 2;
    removeSep = false;
  }
  else
  {
    popup->RemoveMenu(firstPosition,MF_BYPOSITION);
    popup->RemoveMenu(firstPosition,MF_BYPOSITION);
  }

  if(InsideTag(_T("!")) || InsideTag(_T("COMMENT")))
  {
    removeSep = false;
    ++firstPosition;
  }
  else
  {
    popup->RemoveMenu(firstPosition,MF_BYPOSITION);
  }
  if(InsideTag(_T("marquee")))
  {
    removeSep = false;
    ++firstPosition;
  }
  else
  {
    popup->RemoveMenu(firstPosition,MF_BYPOSITION);
  }
  if(removeSep)
  {
    // Remove last double seperator
    popup->RemoveMenu(firstPosition,MF_BYPOSITION);
  }
  // LAYER PART
  removeSep = true;
  bool noLayer = true;
  if(InsideTag(_T("div"),pLayer))
  {
    HtmlElement elem(pLayer);
    if(elem.HasStyle())
    {
      XString style = elem.GetInlineStyle();
      style = XString(_T("div {")) + style + _T("}");
      CssStyleSheet css;
      css.parse_css(style);
      XString media  = _T("standard");
      XString select = _T("div");
      if(css.get(_T("standard"),_T("div"),_T("position")).c_str() == XString(_T("absolute")))
      {
        noLayer   = false;
        removeSep = false;
        firstPosition += 2;
      }
    }
  }
  if(noLayer)
  {
    popup->RemoveMenu(firstPosition,MF_BYPOSITION);
    popup->RemoveMenu(firstPosition,MF_BYPOSITION);
  }
  if(removeSep)
  {
    // Remove last double seperator
    popup->RemoveMenu(firstPosition,MF_BYPOSITION);
  }
  // FORM PART
  removeSep = true;
  if(InsideTag(_T("form"),pForm))
  {
    removeSep = false;
    ++firstPosition;
  }
  else
  {
    popup->RemoveMenu(firstPosition,MF_BYPOSITION);
  }
  if(InsideTag(_T("input"),pField))
  {
    removeSep = false;
    ++firstPosition;
  }
  else
  {
    popup->RemoveMenu(firstPosition,MF_BYPOSITION);
  }
  if(InsideTag(_T("label"),pLabel))
  {
    removeSep = false;
    ++firstPosition;
  }
  else
  {
    popup->RemoveMenu(firstPosition,MF_BYPOSITION);
  }
  if(InsideTag(_T("select"),pSelect))
  {
    removeSep = false;
    ++firstPosition;
  }
  else
  {
    popup->RemoveMenu(firstPosition,MF_BYPOSITION);
  }

  if(InsideTag(_T("textarea"),pText))
  {
    removeSep = false;
    ++firstPosition;
  }
  else
  {
    popup->RemoveMenu(firstPosition,MF_BYPOSITION);
  }
  if(removeSep)
  {
    // Remove last double seperator
    popup->RemoveMenu(firstPosition++,MF_BYPOSITION);
  }


	// enable/disable items based on the status of the command
 	UINT nCount = popup->GetMenuItemCount();
	for (UINT i = 0; i< nCount; i++)
	{
		int nID = (int)popup->GetMenuItemID(i);
		if (nID != 0 && nID != -1)
		{
			BOOL bHasFunc;
			UINT uiElemType;
			UINT dhtmlCmdID = GetDHtmlCommandMapping(nID, bHasFunc, uiElemType);
      if(dhtmlCmdID && (dhtmlCmdID != 0xffffffff))
      {
			  long nStatus = QueryStatus(dhtmlCmdID);
			  if ((nStatus & OLECMDF_ENABLED) || inTable)
        {
          popup->EnableMenuItem(nID, MF_ENABLED);
        }
        else
        {
  			  popup->EnableMenuItem(nID, MF_DISABLED|MF_GRAYED);
        }
      }
      else
      {
        popup->EnableMenuItem(nID, MF_ENABLED);
      }
    }
	}
  UINT item = TrackPopupMenu(*popup
                            ,TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_RETURNCMD 
                            ,ppt->x
                            ,ppt->y
                            ,0
                            ,GetSafeHwnd()
                            ,nullptr);
  switch(item)
  {
    case ID_EDIT_CUT:         CHtmlEditView::Cut();
                              break;
    case ID_EDIT_COPY:        CHtmlEditView::Copy();
                              break;
    case ID_EDIT_PASTE:       CHtmlEditView::Paste();
                              break;
    case ID_CONTEXTMENU_SELECTALL: 
                              CHtmlEditView::SelectAll();
                              break;
    case ID_FORMAT_PARAGRAPH: OnFormatParagraph();
                              break;
    case ID_EDIT_FIND:        OnFind();
                              break;
    case ID_EDIT_REPLACE_AA:  OnReplace();
                              break;
    case ID_PROPERTIES:       OnProperties();
                              break;
    case ID_EDIT_CAPTION:     OnDoubleClickCaption(pCaption);
                              break;
    case ID_TABLE_TABLEPROPERTIES:
                              OnTableProperties();
                              break;
    case ID_TABLE_CELLPROPERTIES:
                              OnCellProperties();
                              break;
    case ID_TABLE_INSERTROWABOVE:
                              OnTableInsertRowAbove();
                              break;
    case ID_TABLE_INSERTROWBELOW:
                              OnTableInsertRowBelow();
                              break;
    case ID_TABLE_INSERTCOLUMNLEFT:
                              OnTableInsertColumnBefore();
                              break;
    case ID_TABLE_INSERTCOLUMNRIGHT:
                              OnTableInsertColumnAfter();
                              break;
    case ID_TABLE_DELETEROW:
                              OnTableDeleteRow();
                              break;
    case ID_TABLE_DELETECOLUMN:
                              OnTableDeleteColumn();
                              break;
    case IDM_HYPERLINK:       OnAnchor();
                              break;
    case ID_EDIT_REMOVEHYPERLINK:
                              OnRemoveHyperlink();
                              break;
    case ID_BUTTON_IMAGE:     OnDoubleClickImage(pImage);
                              break;
    case ID_HTMLPOPUP_AREAPROPERTIES:
                              OnDoubleClickArea(pArea);
                              break;
    case ID_HTMLPOPUP_REMOVEAREA:
                              OnRemoveArea(pArea);
                              break;
    case ID_EDIT_COMMENT:     OnEditComment();
                              break;
    case ID_BUTTON_ANCHOR:    OnBookmark();
                              break;
    case ID_CONTEXT_MARQUEE:  OnMarquee();
                              break;
    case ID_CONTEXT_FORM:     OnDoubleClickForm(pForm);
                              break;
    case ID_CONTEXT_FIELD:    OnDoubleClickFormInput(pField);
                              break;
    case ID_FORM_LABEL:       OnDoubleClickFormLabel(pLabel);
                              break;
    case ID_CONTEXT_SELECT:   OnDoubleClickFormSelect(pSelect);
                              break;
    case ID_CONTEXT_TEXTAREA: OnDoubleClickFormTextArea(pText);
                              break;
    case ID_HTMLPOPUP_LAYERPROPERTIES:
                              OnDoubleClickDiv(pLayer);
                              break;
    case ID_HTMLPOPUP_REMOVELAYER:
                              OnRemoveLayer(pLayer);
                              break;
    case ID_CSSSTYLESHEETS:   OnEditStyleSheets();
                              break;
  }
  if((ID_ALLTAGS_FIRSTTAG < item) && (item <= ID_ALLTAGS_LASTTAG))
  {
    unsigned ind = item - ID_ALLTAGS_FIRSTTAG - 1;
    CComPtr<IHTMLElement> elem = allTags[ind];
    if(elem)
    {
      OnEditElement(elem);
    }
  }
  // Clear all tags
  for(unsigned int ind = 0;ind < allTags.size(); ++ind)
  {
    try
    {
      IHTMLElement* elem = allTags[ind];
      elem->Release();
    }
    catch(...)
    {
      // Elements could be removed by the actions in the popup menu.
      // IGNORE THIS ERROR and continue to the next. We are in the
      // process of clearing up things anyway
    }
  }
  allTags.clear();

  return S_OK;
}
#pragma warning (default : 4311)

void
CHTMLEdView::GetTagsMenu(CMenu* menu,vector<IHTMLElement*>& allTags)
{
  HRESULT       hr       = S_FALSE;
  IHTMLElement* pElement = NULL;
  IHTMLElement* pParent  = NULL;
  CComBSTR      pStr     = NULL;
  CString       text;
  int x = 1;

  // First item deleted
  menu->DeleteMenu(0,MF_BYPOSITION);

  hr = GetCurrentElement(&pElement);
  if (SUCCEEDED(hr) && pElement)
  {
    CString tag;
    CString className;
    do 
    {
      hr = pElement->get_tagName(&pStr);
      if(SUCCEEDED(hr))
      {
        tag = CW2CT(pStr);
        if(tag == _T("BODY"))
        {
          break;
        }
        text = CString(_T("Tag: ")) + tag;
      }
      hr = pElement->get_className(&pStr);
      if(SUCCEEDED(hr))
      {
        className = CW2CT(pStr);
        if(!className.IsEmpty())
        {
          text += CString(_T(" (")) + className + _T(")");
        }
      }
      // Save this element
      pElement->AddRef();
      allTags.push_back(pElement);

      menu->AppendMenu(MF_STRING,ID_ALLTAGS_FIRSTTAG + x++,text);
      hr = pElement->get_parentElement(&pParent);
      if(SUCCEEDED(hr))
      {
        pElement = pParent;
      }
    } 
    while(pElement && SUCCEEDED(hr));
  }
}

SIZE
CHTMLEdView::GetDocumentSize()
{
  SIZE size;
  size.cx = 0;
  size.cy = 0;
  if(!m_Doc2.p)
  {
    return size;
  }
  long w,h;
  CComPtr<IHTMLElement> bod;
  m_Doc2->get_body(&bod);
  CComQIPtr<IHTMLElement2,&IID_IHTMLElement2> body = bod;

  // Get client size (including padding, excluding border and margin)
  body->get_clientHeight(&h);
  body->get_clientWidth (&w);
  size.cx = w;
  size.cy = h;
  return size;
}

/////////////////////////////////////////////////////////////////////////////
// MainFrame message handlers
//INT CALLBACK NEnumFontNameProc(LOGFONT *plf, TEXTMETRIC* /*ptm*/, INT /*nFontType*/, LPARAM lParam)
//{
//  CXTPControlComboBox* comboBox = (CXTPControlComboBox*) lParam;
//  comboBox->AddString(plf->lfFaceName);
//  return TRUE;
//}

void CHTMLEdView::OnInitialUpdate() 
{
  CHtmlEditView::OnInitialUpdate();
  m_bTableBorders  = TRUE;
  m_bDesignMode    = TRUE;
  m_overwriteKey   = FALSE;
  m_bTagsMode      = false;
  m_bParaMode      = false;
  m_bookmarkNumber = 0;
  ShowTableBorders();
  OnUpdateInsertKey();

  // Set all font names
	MainFrame *pFrame = (MainFrame*) AfxGetMainWnd();
	ASSERT_VALID(pFrame);
  // Set all font sizes
  CMFCToolBarComboBoxButton* pCombo = pFrame->GetFontSizeCombo();
  if(!pCombo)
  {
    return;
  }
  pCombo->RemoveAllItems();

  static int nFontSizes[] = {8, 10, 12, 14, 18, 24, 36};
  CString str;
  for (int i = 0; i < sizeof(nFontSizes)/sizeof(int); i++)
  {
    str.Format(_T("%d"), nFontSizes[i]);
    pCombo->AddItem(str);
  }
}

void
CHTMLEdView::UpdateClassCombo(CString& p_tagname,CString& p_classname)
{
  CString szCurFmt;
  int nSel = 0;

  if(p_tagname.IsEmpty() && p_classname.IsEmpty())
  {
    // Nothing to do
    return;
  }
	MainFrame *pFrame = (MainFrame*) AfxGetMainWnd();
	ASSERT_VALID(pFrame);
	CMFCToolBarComboBoxButton *pCombo = pFrame->GetClassNameCombo();
  if(!pCombo || pCombo->IsDroppedDown())
  {
    // Combo is in dropped state. Don't do processing
    return;
  }
  szCurFmt = pCombo->m_strText; //  GetWindowText();
  bool userselect;
  CString szResult = Misc::CombinedClassName(p_tagname,p_classname,userselect);
  if(szCurFmt != szResult)
	{
		nSel = pCombo->FindItem(szResult);

    if(nSel == CB_ERR)
    {
      CString empty;
      CString tagOnly = Misc::CombinedClassName(p_tagname,empty,userselect);
      if(szCurFmt == tagOnly)
      {
        nSel = pCombo->GetCurSel();
      }
      else
      {
        nSel = pCombo->FindItem(tagOnly);
      }
    }
	  if(nSel == CB_ERR)
    {
      // Defaults to "<No Style>"
      nSel = 0;
    }
    // Set the combo
    if(nSel != pCombo->GetCurSel())
    {
      pCombo->SelectItem(nSel);
    }
  }
}

void CHTMLEdView::UpdateFontCombos()
{	
	CString szResult,szCurFmt;
	int nSel;
	short nCurSize;

	MainFrame *pFrame = (MainFrame*) theApp.m_pMainWnd;
	ASSERT_VALID(pFrame);

	//Set font face
	CMFCToolBarFontComboBox *pCombo = pFrame->GetFontNameCombo();
  if(pCombo == nullptr)
  {
    return;
  }
  szCurFmt = pCombo->m_strText; //  GetWindowText();
	GetFontFace(szResult);
	//only update the combo it isn't in the dropped state
	//and the font name has changed
	if(!pCombo->IsDroppedDown() && szCurFmt!=szResult)
	{
		nSel = pCombo->FindItem(szResult);
		if(nSel != CB_ERR)
    {
      pCombo->SelectItem(nSel);
    }
	}

	//Set font size
	CMFCToolBarComboBoxButton* combo = pFrame->GetFontSizeCombo();
	ASSERT_VALID(combo);
	short nSize = 0;
	nCurSize = 0;

	GetFontSize(nSize);
	//only update the size combo if it is not in the
	//dropped down state.
	if(!combo->IsDroppedDown())
	{
		szResult.Empty();
    nSize = Misc::PointSizeToFontSize(nSize);
    _itot(nSize,szResult.GetBuffer(5),10);
		nSel = combo->FindItem(szResult);
		szResult.ReleaseBuffer();
		if(nSel != CB_ERR)
    {
      combo->SelectItem(nSel);
    }
	}
}

UINT_PTR CALLBACK CdlgHook(  HWND hdlg,UINT uiMsg,WPARAM /*wParam*/, LPARAM lParam)
{
	if(uiMsg == WM_INITDIALOG)
	{
		CHOOSECOLOR *pcc = (CHOOSECOLOR*)lParam;
		CRect *rc = (CRect*)pcc->lCustData;
		if(rc)
    {
      SetWindowPos(hdlg,HWND_TOP,rc->right,rc->bottom,0,0,SWP_NOZORDER|SWP_NOSIZE);
    }
		SetWindowText(hdlg, _T("Choose a Foreground Color"));
		delete rc;
	}
	return 0;
}

void 
CHTMLEdView::PopColorMenu()
{
	//get the rect for menu
	MainFrame *pFrame = (MainFrame*) AfxGetMainWnd(); 
	ASSERT_VALID(pFrame);
	CRect *prc = new CRect;
	if(!prc) return;
	pFrame->GetEditBar()->GetClientRect(prc);
	pFrame->GetEditBar()->ClientToScreen(prc);
  prc->right = (prc->right / 2);

  int color = 0;
  GetForeColor(color);
  CColorDialog dlg(color, CC_FULLOPEN | CC_RGBINIT,this);
	dlg.m_cc.Flags |= CC_ENABLEHOOK;
	dlg.m_cc.lpfnHook = CdlgHook;
	dlg.m_cc.lCustData = (LONG_PTR)prc;

	if(dlg.DoModal()==IDOK)
	{
		CString szColor;
		COLORREF cr = dlg.GetColor();

		//change the COLORREF into an RGB.
		szColor.Format(_T("%.2x%.2x%.2x"),GetRValue(cr),GetGValue(cr),GetBValue(cr));
		SetForeColor(szColor);
	}
}

void CHTMLEdView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{

	if(bActivate && pActivateView==this && pDeactiveView != this)
	{
		//we're being activated get the HTML from the WebView
		UpdateView();
	}
  else if(bActivate && pDeactiveView)
  {
    ExtractStyles();
  }
	CHtmlEditView::OnActivateView(bActivate, pActivateView, pDeactiveView);
  // Set styles for current stylesheet of this document
}

void 
CHTMLEdView::UpdateView()
{
  // Beware:
  // Transfer AND saving of the text is now done in "ChildFrm::SwapView(int ID)

  //   CMDIChildFrame* parent   = (CMDIChildFrame*) GetParent();
  //   ScintillaHTMLView* pSrcView = (ScintillaHTMLView*) parent->GetSourceView();
  // 	ASSERT_VALID(pSrcView);
  // 
  //   CString szHTML;
  //   pSrcView->GetWindowText(szHTML);
  // 
  //   CHtmlEditView::SetDocumentHTML(szHTML);

  // Save state and HTML
  // m_pBrowserApp->ExecWB((OLECMDID)OLECMDID_SAVE,MSOCMDEXECOPT_DODEFAULT,NULL,NULL);
  // GetDocument()->InternalSave(szHTML);

  // Re-Navigate to this document
  // This is a hack: SetDocumentHTML does restore the document
  // but does NOT restore the LINK'ed stylesheets.
  CHtmlEditView::Navigate(GetDocument()->GetPathName());
}

bool
CHTMLEdView::DoSave()
{
  SetOurGenerator();
  // Save state and HTML so that the browser think's it's saved an not prompt us for saving afterwards
  HRESULT hr = m_pBrowserApp->ExecWB((OLECMDID)OLECMDID_SAVE,MSOCMDEXECOPT_DODEFAULT,NULL,NULL);
  return SUCCEEDED(hr);
}

CString
CHTMLEdView::RegisteredURL()
{
  CString anchor;
  CString filename;
  Misc::SplitMidpageAnchor(m_URL,filename,anchor);
  return Misc::StripFileProtocol(filename);
}

CString
CHTMLEdView::RebasedURL()
{
  CString anchor;
  CString filename;
  CString url = Misc::StripFileProtocol(m_URL);
  Misc::SplitMidpageAnchor(url,filename,anchor);
  return Misc::RemoveBasePart(m_baseURL,filename);
}

void 
CHTMLEdView::OnTableBorders()
{
  m_bTableBorders = !m_bTableBorders;
  ShowTableBorders();
}

void CHTMLEdView::OnViewTags()
{
  m_bTagsMode = !m_bTagsMode;
  LoadGlyphs(1,m_bTagsMode,m_bParaMode);
  SetShowAllTags(m_bTagsMode || m_bParaMode);
}

void 
CHTMLEdView::OnViewParagraph()
{
  m_bParaMode = !m_bParaMode;
  LoadGlyphs(1,m_bTagsMode,m_bParaMode);
  SetShowAllTags(m_bTagsMode || m_bParaMode);
}

void CHTMLEdView::OnViewBrowse()
{
	m_bDesignMode = !m_bDesignMode;
	SetDesignMode(m_bDesignMode);	
  LoadGlyphs(1,m_bTagsMode,m_bParaMode);
  SetShowAllTags(m_bTagsMode || m_bParaMode);
}

void
CHTMLEdView::OnUpdateParagraph(CCmdUI *pCmdUI)
{
  pCmdUI->SetCheck(m_bParaMode);
}

void CHTMLEdView::OnUpdateModeBrowse(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(!m_bDesignMode);
}

void CHTMLEdView::OnUpdateModeTags(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bTagsMode);
}

void CHTMLEdView::OnUpdateTableBorders(CCmdUI *pCmdUi)
{
  pCmdUi->SetCheck(m_bTableBorders);
}

void 
CHTMLEdView::OnUpdateTableCommands(CCmdUI *pCmdUi)
{
  pCmdUi->Enable(InTableCell());
}

void
CHTMLEdView::ShowTableBorders()
{
  CComVariant var;
  HRESULT hr;
	V_VT(&var)   = VT_BOOL;
  V_BOOL(&var) = m_bTableBorders ? VARIANT_TRUE : VARIANT_FALSE;
  hr = ExecCommand(&CGID_MSHTML
                  ,IDM_SHOWZEROBORDERATDESIGNTIME
                  ,MSOCMDEXECOPT_DODEFAULT
                  ,&var
                  ,NULL);
}

// Get the current element on the HTML document (where the caret is)
// CAVEAT-CAVEAT-CAVEAT CAVEAT-CAVEAT-CAVEAT CAVEAT-CAVEAT-CAVEAT
// This function has previous been implemented with
// IHTMLDocument2->elementFromPoint(&x,&y,&(IHTMLElement))
// This results in a flooding of the OnIdle time with unknown and
// undocumented MS-Windows messages (0x0118 and 0x08002) which in its turn
// results in 100% CPU usage !!! 
// CAVEAT-CAVEAT-CAVEAT CAVEAT-CAVEAT-CAVEAT CAVEAT-CAVEAT-CAVEAT
HRESULT
CHTMLEdView::GetCurrentElement(IHTMLElement** element)
{
  HRESULT hr = S_FALSE;

  if(m_Doc2)
  {
    CComPtr<IDisplayServices> pService;
    CComPtr<IMarkupServices>  spMS;
    CComPtr<IMarkupPointer>   markup;
    CComPtr<IHTMLCaret>       pCaret;
    try
    {
      m_Doc2->QueryInterface(IID_IDisplayServices,(void**)&pService);
      m_Doc2->QueryInterface(IID_IMarkupServices, (void**)&spMS);
      spMS->CreateMarkupPointer(&markup);

      pService->GetCaret(&pCaret);
      pCaret->MoveMarkupPointerToCaret(markup);
      return markup->CurrentScope(element);
    }
    catch(...)
    {
      // In case a QueryInterface or GetCaret does not succeed
      return S_FALSE;
    }
  }
  return hr;
}

// Checks if we are currently inside a 'tag'
// Does **not** return current element (see next method)
// THIS VERSION SHOULD NOT IN ANY FORM POST MESSAGES TO THE
// MS-WINDOWS MESSAGE QUEUE. See the caveat above GetCurrentElement()
bool
CHTMLEdView::InsideTag(CString tag)
{
  CComPtr<IHTMLElement> pCurElement;
  HRESULT hr = S_FALSE;

  hr = GetCurrentElement(&pCurElement);
  if(SUCCEEDED(hr) && pCurElement.p)
  {
    return InsideTag(pCurElement,tag);
  }
  return false;
}

bool    
CHTMLEdView::InsideTag(CComPtr<IHTMLElement> start,CString tag)
{
  CComPtr<IHTMLElement> pCurElement = start;
  CComPtr<IHTMLElement> pParent;
  HRESULT hr = S_FALSE;

  if(pCurElement.p)
  {
    CComBSTR pStr = NULL;
    do 
    {
      hr = pCurElement->get_tagName(&pStr);
      if(SUCCEEDED(hr))
      {
        CString foundTag = CW2CT(pStr);
        if(tag.CompareNoCase(foundTag) == 0)
        {
          return true;;
        }
        if(foundTag.CompareNoCase(_T("BODY")) == 0)
        {
          return false;
        }
        pParent = NULL;
        hr = pCurElement->get_parentElement(&pParent);
        if(SUCCEEDED(hr))
        {
          pCurElement = NULL;
          pCurElement = pParent;
        }
      }
    } 
    while(SUCCEEDED(hr));
  }
  return false;
}

// Checks if we are currently outside a 'tag'
// can return a **element pointer if requested
// so that we can continue to work on it
bool
CHTMLEdView::OutsideTag(CString tag,CComPtr<IHTMLElement>& pElement)
{
  CComPtr<IHTMLElement> pCurElement = NULL;
  HRESULT hr = GetCurrentElement(&pCurElement);
  if(SUCCEEDED(hr) && pCurElement.p)
  {
    return OutsideTag(pCurElement,tag,pElement);
  }
  return false;
}

bool
CHTMLEdView::OutsideTag(CComPtr<IHTMLElement> start,CString tag,CComPtr<IHTMLElement>& pElement)
{
  CComPtr<IHTMLElement> pCurElement = NULL;
  CComPtr<IHTMLElement> pChild      = NULL;
  HRESULT       hr = S_FALSE;

  pCurElement = start;
  CComBSTR pStr = NULL;
  if(pCurElement.p)
  {
    do 
    {
      hr = pCurElement->get_tagName(&pStr);
      if(SUCCEEDED(hr))
      {
        CString foundTag = CW2CT(pStr);
        if(tag.CompareNoCase(foundTag) == 0)
        {
          // Indeed. Found our tag
          // Return the found element, if requested
          // Do not release yet, still needed!
          pElement  = pCurElement;
          return true;;
        }
        pChild = NULL;
        CComQIPtr<IHTMLDOMNode,&IID_IHTMLDOMNode> node = pCurElement;
        CComPtr<IHTMLDOMNode> child;
        hr = node->get_firstChild(&child);
        if(SUCCEEDED(hr))
        {
          if(child.p == NULL)
          {
            return false;
          }
          pCurElement = NULL;
          pCurElement = child;
        }
        else
        {
          return false;
        }
      }
    } 
    while(SUCCEEDED(hr));
  }
  return false;
}


// Checks if we are currently inside a 'tag'
// Can return a **element pointer if requested
// so that we can continue to work on it.
bool
CHTMLEdView::InsideTag(CString tag,CComPtr<IHTMLElement>& pElement)
{
  CComPtr<IHTMLElement> pCurElement = NULL;
  HRESULT hr = S_FALSE;

  hr = GetCurrentElement(&pCurElement);
  if(SUCCEEDED(hr) && pCurElement.p)
  {
    return InsideTag(pCurElement,tag,pElement);
  }
  return false;
}

bool
CHTMLEdView::InsideTag(CComPtr<IHTMLElement> start,CString tag,CComPtr<IHTMLElement>& pElement)
{
  CComPtr<IHTMLElement> pCurElement = NULL;
  CComPtr<IHTMLElement> pParent     = NULL;
  HRESULT       hr = S_FALSE;

  pCurElement = start;
  CComBSTR pStr = NULL;
  if(pCurElement.p)
  {
    do 
    {
      hr = pCurElement->get_tagName(&pStr);
      if(SUCCEEDED(hr))
      {
        CString foundTag = CW2CT(pStr);
        if(tag.CompareNoCase(foundTag) == 0)
        {
          // Indeed. Found our tag
          // Return the found element, if requested
          // Do not release yet, still needed!
          pElement  = pCurElement;
          return true;;
        }
        if(foundTag.CompareNoCase(_T("HTML")) == 0)
        {
          return false;
        }
        pParent = NULL;
        hr = pCurElement->get_parentElement(&pParent);
        if(SUCCEEDED(hr))
        {
          pCurElement = NULL;
          pCurElement = pParent;
        }
      }
    } 
    while(SUCCEEDED(hr));
  }
  return false;
}

CString
CHTMLEdView::GetCurrentSelectionText()
{
  CComPtr<IHTMLSelectionObject> pSelObj;
  CComPtr<IHTMLTxtRange>        pTxtRange;
  CComBSTR bstrType;
  CComBSTR bstrText;
  CString  theType;
  CString  theText;

  if(m_Doc2.p == NULL)
  {
    // UI Interface can be before OnDocumentComplete
    return CString();
  }
  HRESULT hr = m_Doc2->get_selection(&pSelObj);
  if (SUCCEEDED(hr)) 
  {
    hr = pSelObj->get_type(&bstrType);
    if(SUCCEEDED(hr))
    {
      theType = CW2CT(bstrType);
      if(theType.CompareNoCase(_T("text")) == 0)
      {
        hr = pSelObj->createRange((IDispatch**)&pTxtRange);
        if (SUCCEEDED(hr)) 
        {
          pTxtRange->get_text(&bstrText);
          theText = CW2CT(bstrText);
        }
      }
    }
  }
  return theText;
}

// Get a generic attribute from the IHTMLElement
CString
CHTMLEdView::GetAttribute(CComPtr<IHTMLElement> pElement
                         ,CString attrib_name)
{
  CComBSTR attrib;
  CComBSTR atValue;
  CComVariant  value;
  CString  csValue;
  attrib = CT2CW(attrib_name);

  if(SUCCEEDED(pElement->getAttribute(attrib,0,&value)))
  {
    atValue = V_BSTR(&value);
    csValue = CW2CT(atValue);
    return csValue;
  }
  return _T("");
}

// Called whenever UI repaints (OnUpdateUI)
void
CHTMLEdView::UpdateStructure(CString& p_tagname,CString& p_classname)
{
  IHTMLElement* pElement = NULL;
  IHTMLElement* pParent  = NULL;
  CComBSTR      pStr     = NULL;
  HRESULT       hr       = S_FALSE;

  hr = GetCurrentElement(&pElement);
  if (SUCCEEDED(hr) && pElement)
  {
    CString tag;
    CString className;
    do 
    {
      hr = pElement->get_tagName(&pStr);
      if(SUCCEEDED(hr))
      {
        tag = CW2CT(pStr);
        if(tag == _T("BODY"))
        {
          return;
        }
      }
      hr = pElement->get_className(&pStr);
      if(SUCCEEDED(hr))
      {
        className = CW2CT(pStr);
      }
      if(Misc::IsUserSelectableTag(tag))
      {
        p_tagname   = tag;
        p_classname = className;
        return;
      }
      hr = pElement->get_parentElement(&pParent);
      if(SUCCEEDED(hr))
      {
        pElement = pParent;
      }
    } 
    while(pElement && SUCCEEDED(hr));
  }
}


void 
CHTMLEdView::OnTableInsert()
{
  CTablePropertiesDlg tabProp(this,GetBase(),true,HtmlTable(NULL),HtmlTableRow(NULL),HtmlTableCell(NULL),m_Doc2);
  if(tabProp.StartDialog(0) == IDOK)
  {
    PasteTextHTML(tabProp.GetNewTable());
  }
}

// Paste a piece of text directly into the document
void
CHTMLEdView::PasteTextHTML(CString p_text)
{
  CComBSTR b_text = p_text;
  CComPtr<IHTMLSelectionObject> pSelObj;
  CComPtr<IHTMLTxtRange>        pTxtRange;

  HRESULT  hr = m_Doc2->get_selection(&pSelObj);
  if (SUCCEEDED(hr)) 
  {
    hr = pSelObj->createRange((IDispatch**)&pTxtRange);
    if (SUCCEEDED(hr)) 
    {
      hr = pTxtRange->pasteHTML(b_text);
    }
  }
}

void
CHTMLEdView::PasteText(CString p_text)
{
  CComBSTR b_text = p_text;
  CComPtr<IHTMLSelectionObject> pSelObj;
  CComPtr<IHTMLTxtRange>        pTxtRange;

  HRESULT  hr = m_Doc2->get_selection(&pSelObj);
  if (SUCCEEDED(hr)) 
  {
    hr = pSelObj->createRange((IDispatch**)&pTxtRange);
    if (SUCCEEDED(hr)) 
    {
      pTxtRange->put_text(b_text);
    }
  }
}

// returns the table cell which has the cursor
// This function may return an invaild cell !! (check with Cell.Valid()!)
// Displays a messagbox if cursor position / selection is invalid
HtmlTableCell 
CHTMLEdView::GetSelectedCell()
{
	// get the selected object or if only text is selected: get the parent of the selection
  CString s_Sel;
  CComPtr<IHTMLElement> sel;

  HRESULT hr = GetCurrentElement(&sel);
  if(!SUCCEEDED(hr))
  {
    return HtmlTableCell(0);
  }
  HtmlElement i_Sel(sel);
	s_Sel = i_Sel.GetTagName();

	if (s_Sel == _T("TR") || s_Sel == _T("TBODY") || s_Sel == _T("TABLE"))
	{
    theApp.MessageBox(_T("A selection spanning over multiple cells is not allowed."),_T("Error"), MB_OK|MB_ICONSTOP);
		return HtmlTableCell(0);
	}
	// search upward in hierarchy for the next table cell (TD)
	HtmlElement i_TD = i_Sel.FindParent(_T("TD"));
	if (!i_TD.Valid())
	{
    i_TD = i_Sel.FindParent(_T("TH"));
    if(!i_TD.Valid())
    {
      theApp.MessageBox(_T("Please set the cursor into a table cell!"),_T("Warning"), MB_OK|MB_ICONEXCLAMATION);
      return HtmlTableCell(0);
    }
	}
	// convert cHtmlElement to cHtmlTableCell using Ptr()
	return HtmlTableCell(i_TD);
}

// Returns the table row which has the cursor
// This function may return an invalid row!! (Check with Row.Valid())
// Displays a messageBox if cursor position /selection is invalid
HtmlTableRow
CHTMLEdView::GetSelectedRow()
{
  // get the selected object or if only text is selected: get the parent of the selection
  CString s_Sel;
  CComPtr<IHTMLElement> sel;

  HRESULT hr = GetCurrentElement(&sel);
  if(!SUCCEEDED(hr))
  {
    return HtmlTableRow(0);
  }
  HtmlElement i_Sel(sel);
  s_Sel = i_Sel.GetTagName();

  if (s_Sel == _T("TBODY") || s_Sel == _T("TABLE"))
  {
    theApp.MessageBox(_T("A selection spanning over multiple rows is not allowed."),_T("Error"), MB_OK|MB_ICONSTOP);
    return HtmlTableRow(0);
  }
  // search upward in hierarchy for the next table row (TR)
  HtmlElement i_TR = i_Sel.FindParent(_T("TR"));
  if (!i_TR.Valid())
  {
    theApp.MessageBox(_T("Please set the cursor into a table row!"),_T("Warning"), MB_OK|MB_ICONEXCLAMATION);
    return HtmlTableRow(0);
  }
  // convert cHtmlElement to cHtmlTableRow using Ptr()
  return HtmlTableRow(i_TR);
}

bool
CHTMLEdView::InTableCell()
{
  return InsideTag(_T("TD")) || InsideTag(_T("TH"));
}

bool
CHTMLEdView::InTable()
{
  return InsideTag(_T("TABLE"));
}

bool 
CHTMLEdView::InTableRowGroup()
{
  return InsideTag(_T("TBODY")) || 
         InsideTag(_T("THEAD")) || 
         InsideTag(_T("TFOOT"));
}

// Inserts a new table row above (b_Below = FALSE) or below (b_Below = TRUE)
// the table cell which currently has the cursor
// Displays a messagbox if cursor position / selection is invalid
void 
CHTMLEdView::OnTableInsertRow(BOOL b_Below)
{
	HtmlTableCell i_Cell = GetSelectedCell();
	if (i_Cell.Valid())
  {
	  HtmlTable i_Table = i_Cell.GetParentTable();

	  UINT u32_Position = (b_Below) ? 1 : 0;

	  // insert as many cells as row 0 has
	  i_Table.InsertRow(i_Cell.GetRowIndex()+u32_Position, -1);
  }
}

// Inserts a new table column at the left (b_Right = FALSE) or right (b_Right = TRUE)
// of the table cell which currently has the cursor
// Displays a messagbox if cursor position / selection is invalid
void 
CHTMLEdView::OnTableInsertColumn(BOOL b_Right)
{
	HtmlTableCell i_Cell = GetSelectedCell();
	if (i_Cell.Valid())
  {
	  UINT u32_Position = (b_Right) ? i_Cell.GetColSpan() : 0;

	  HtmlTable i_Table = i_Cell.GetParentTable();
	  i_Table.InsertColumn(i_Cell.GetColIndex()+u32_Position);
  }
}

void
CHTMLEdView::OnTableDeleteRow()
{
	HtmlTableCell i_Cell  = GetSelectedCell(); // outputs error
  if(i_Cell.Valid())
  {
  	HtmlTable     i_Table = i_Cell.GetParentTable();
  	if (i_Table.Valid())
    {
      i_Table.DeleteRow(i_Cell.GetRowIndex());
    }
  }
}

void
CHTMLEdView::OnTableDeleteColumn()
{
	HtmlTableCell i_Cell  = GetSelectedCell(); // outputs error
  if(i_Cell.Valid())
  {
  	HtmlTable     i_Table = i_Cell.GetParentTable();
	  if (i_Table.Valid())
    { 
      i_Table.DeleteColumn(i_Cell.GetColIndex());
    }
  }
}

void
CHTMLEdView::OnTableInsertRowBelow()
{
  OnTableInsertRow(TRUE);
}

void
CHTMLEdView::OnTableInsertRowAbove()
{
  OnTableInsertRow(FALSE);
}

void 
CHTMLEdView::OnTableInsertColumnBefore()
{
  OnTableInsertColumn(FALSE);
}

void
CHTMLEdView::OnTableInsertColumnAfter()
{
  OnTableInsertColumn(TRUE);
}

void
CHTMLEdView::OnTableCombine()
{
  bool horizontal = true;

  if(theApp.MessageBox(_T("Combine the cell with it's right neighbor or its lower neighbor?"),_T("Which direction?"),_T("?Right Lower")) == _T("lower"))
  {
    horizontal = false;
  }
	HtmlTableCell i_Cell = GetSelectedCell(); // outputs error
	if (i_Cell.Valid())
  {
    if(horizontal)
    {
	    if(!i_Cell.CombineHorizontal())
      {
        theApp.MessageBox(_T("You can only combine cells which have a right neighbor."),_T("Error"),MB_OK|MB_ICONSTOP);
      }
    }
    else
    {
      if(!i_Cell.CombineVertical())
      {
        theApp.MessageBox(_T("You can only combine cells which have a neighbor under them."),_T("Error"),MB_OK|MB_ICONSTOP);
      }
    }
  }
}

void
CHTMLEdView::OnTableSplitCell()
{
  bool horizontal = true;

  if(theApp.MessageBox(_T("Split the cell horizontal or vertical?"),_T("Which direction?"),_T("?Horizontal Vertical")) == _T("vertical"))
  {
    horizontal = false;
  }
  HtmlTableCell i_Cell = GetSelectedCell(); // outputs error
	if (i_Cell.Valid())
  {
    if(horizontal)
    {
      i_Cell.SplitHorizontal();
    }
    else
    {
      i_Cell.SplitVertical();
    }
  }
}

// Here comes the table properties
void
CHTMLEdView::OnTableProperties()
{
  HtmlTableCell i_Cell = GetSelectedCell(); // outputs error
  if(i_Cell.Valid())
  {
    HtmlTable i_Table  = i_Cell.GetParentTable();
    HtmlTableRow i_Row = i_Cell.GetParentRow();
    CTablePropertiesDlg tabProp(this,GetBase(),false,i_Table,i_Row,i_Cell,m_Doc2);
    tabProp.StartDialog(0);
  }
}

void
CHTMLEdView::OnCellProperties()
{
  HtmlTableCell i_Cell = GetSelectedCell(); // outputs error
  if(i_Cell.Valid())
  {
    HtmlTable  i_Table = i_Cell.GetParentTable();
    HtmlTableRow i_Row = i_Cell.GetParentRow();
    CTablePropertiesDlg tabProp(this,GetBase(),false,i_Table,i_Row,i_Cell,m_Doc2);
    tabProp.StartDialog(1);
  }
}

void
CHTMLEdView::OnTableProperties(CComPtr<IHTMLElement> /*pElement*/)
{
  OnTableProperties();
}

void
CHTMLEdView::OnCellProperties(CComPtr<IHTMLElement> /*pElement*/)
{
  OnCellProperties();
}

// We double click somewhere on the view
// See what for an element we are on and perform action
// or return FALSE to let default dclick perform
BOOL
CHTMLEdView::OnDoubleClick(int xPos,int yPos)
{
  typedef std::map<CString,int> JumpMap;
  static JumpMap jumpMap;
  CComPtr<IHTMLElement> pElement;
  HRESULT hr = S_FALSE;
  CString tag;

  if(!jumpMap.size())
  {
    // Initialize jumpmap the first time
    int ind = 0;
    while(jumps[ind].tagname.GetLength())
    {
      jumpMap.insert(std::make_pair(jumps[ind].tagname,ind));
      ++ind;
    }
  }
  try
  {
    hr = m_Doc2->elementFromPoint(xPos,yPos,&pElement);
    if(SUCCEEDED(hr))
    {
      CComBSTR pStr;
      pElement->get_tagName(&pStr);
      tag = CW2CT(pStr);
      tag.MakeLower();

      JumpMap::iterator it = jumpMap.find(tag);
      if(it != jumpMap.end())
      {
        int ind = it->second;
        dclickFunction dblClick = jumps[ind].DoubleClick;
        if(jumps[ind].message.GetLength())
        {
          // Unsupported message
          unsuppFunction unsupp = jumps[ind].Unsupported;
          (this->*unsupp)(jumps[ind].message);
        }
        else
        {
          // ACTIVE ELEMENT TO DO
          (this->*dblClick)(pElement);
        }
        return TRUE;
      }
      // Unknown tag
    }
  }
  catch(...)
  {
    // Unknown feature in IE-Explorer. No Document, or no Element
    CString msg;
    msg.Format(_T("%s: Unimplemented feature of MS-IExplorer. (Tag = %s)"),REGISTER_APP,tag.GetString());
    theApp.MessageBox(msg,_T("Error"),MB_OK|MB_ICONSTOP);
  }
  // Double click not 'eaten'
  return FALSE;
}

// We already found the element, now we want it's 
// corresponding dialog box
BOOL
CHTMLEdView::OnEditElement(CComPtr<IHTMLElement> pElement)
{
  typedef std::map<CString,int> JumpMap;
  static JumpMap jumpMap;
  CString tag;

  if(!jumpMap.size())
  {
    // Initialize jumpmap the first time
    int ind = 0;
    while(jumps[ind].tagname.GetLength())
    {
      jumpMap.insert(std::make_pair(jumps[ind].tagname,ind));
      ++ind;
    }
  }
  try
  {
    CComBSTR pStr;
    pElement->get_tagName(&pStr);
    tag = CW2CT(pStr);
    tag.MakeLower();

    JumpMap::iterator it = jumpMap.find(tag);
    if(it != jumpMap.end())
    {
      int ind = it->second;
      dclickFunction dblClick = jumps[ind].DoubleClick;
      if(jumps[ind].message.GetLength())
      {
        // Unsupported message
        unsuppFunction unsupp = jumps[ind].Unsupported;
        (this->*unsupp)(jumps[ind].message);
      }
      else
      {
        // ACTIVE ELEMENT TO DO
        (this->*dblClick)(pElement);
      }
      return TRUE;
    }
    else
    {
      CString msg;
      msg.Format(_T("This tag [%s] is not yet supported in 'OnEditElement'"),tag.GetString());
      theApp.MessageBox(msg,_T("Impelement"),MB_OK|MB_ICONINFORMATION);
    }
  }
  catch(...)
  {
    // Unknown feature in IE-Explorer. No Document, or no Element
    CString msg;
    msg.Format(_T("%s: Unimplemented feature of MS-IExplorer. (Tag = %s)"),REGISTER_APP,tag.GetString());
    theApp.MessageBox(msg,_T("Error"),MB_OK|MB_ICONSTOP);
  }
  // Double click not 'eaten'
  return FALSE;
}

void
CHTMLEdView::OnDoubleClickHyperlink(CComPtr<IHTMLElement> pElement)
{
  CComBSTR bRel,bHref;
  CString  relation,href;
  BOOL     bCtrlKey = (GetKeyState(VK_CONTROL) & 0x8000) > 0;
  BOOL     bShftKey = (GetKeyState(VK_SHIFT)   & 0x8000) > 0;

  CComQIPtr<IHTMLAnchorElement,&IID_IHTMLAnchorElement> anchor = pElement;

  anchor->get_href(&bHref);
  href = CW2CT(bHref);
  if(href.IsEmpty())
  {
    // Oeps. Not a link, but an bookmark
    OnBookmark();
    return;
  }
  // Strip file protocol/popups
  CString documentFile = Misc::StripFileProtocol(href);
  documentFile = Misc::StripPopup(documentFile);
  // href can contain %XX characters
  Misc::ResolveSpecialChars(href);
  // Add base of this document
  if(documentFile.GetLength()>2 && documentFile.GetAt(1) != _T(':'))
  {
    documentFile = GetBase() + documentFile;
  }
  anchor->get_rel(&bRel);
  relation = CW2CT(bRel);

  // If Control key, navigate directly and ready
  if(bCtrlKey)
  {
    theApp.OpenTypedDocumentFile(documentFile);
    return;
  }
  bool defNavigate = (settings.GetSetting(_T("NavigateDCHyperlink"),_T("yes"))   == _T("yes"));
  bool defShowIt   = (settings.GetSetting(_T("ShowNavigateHyperlink"),_T("yes")) == _T("yes"));
  if(bShftKey)
  {
    defShowIt = true;
  }
  NavigateHyperlink diag(defNavigate,defShowIt,relation,this);
  if(!defShowIt || diag.DoModal() == IDOK)
  {
    defShowIt   = diag.GetShowIt();
    defNavigate = diag.GetNavigate();

    settings.SetSetting(_T("NavigateDCHyperlink"),defNavigate ? _T("yes") : _T("no"));
    settings.SetSetting(_T("ShowNavigateHyperlink"),defShowIt ? _T("yes") : _T("no"));

    if(defNavigate)
    {
      theApp.OpenTypedDocumentFile(documentFile);
    }
    else
    {
      // Just edit it
      OnDoubleClickAnchor(pElement);
    }
  }
}

void
CHTMLEdView::OnAnchor()
{
  bool    inside = true;
  CComPtr<IHTMLElement> pElement;
  CString text = GetCurrentSelectionText();
  int unique = Misc::GetUniqueID(m_Doc2,_T("a"));

  if(!InsideTag (_T("A"),pElement) && 
     !OutsideTag(_T("A"),pElement)  )
  {
    inside = false;
    // Place or replace the hyperlink by ExecCommmand.
    CComVariant vHref(L"");
    ExecCommand(IDM_HYPERLINK
               ,MSOCMDEXECOPT_DONTPROMPTUSER
               ,&vHref
               ,NULL);
    GetCurrentElement(&pElement);
  }
  HtmlAnchor anchor(pElement);
  AnchorDlg diag(this,GetBase(),m_URL,&anchor,unique);
  if(diag.DoModal() == IDOK)
  {
    if(diag.GetDoPopup())
    {
      // Check for Popup SCRIPT section in head
      CheckPopupSCRIPT();
    }
  }
  else if(!inside)
  {
    // Anchor element should be removed again
    anchor.Remove();
    PasteTextHTML(text);
  }
}

void 
CHTMLEdView::OnDoubleClickAnchor(CComPtr<IHTMLElement> pElement)
{
  HtmlAnchor anchor(pElement);
  int unique = Misc::GetUniqueID(m_Doc2,_T("a"));
  AnchorDlg diag(this,GetBase(),m_URL,&anchor,unique);
  if(diag.DoModal() == IDOK)
  {
    if(diag.GetDoPopup())
    {
      // Check for Popup SCRIPT section in head
      CheckPopupSCRIPT();
    }
  }
}

void
CHTMLEdView::CheckPopupSCRIPT()
{
  CString popupFunctions = 
          _T("var oPopup = window.createPopup();\r\n")
          _T("var wide = 400;\r\n")
          _T("var high = 300;\r\n")
          _T("function onDone(src)\r\n")
          _T("{\r\n")
          _T("  var tot = src;\r\n")
          _T("  tot = src.replace('/<html>/','')\r\n")
          _T("  tot = tot.replace('/</html>/','')\r\n")
          _T("  tot = tot.replace('/<body>/','');\r\n")
          _T("  tot = '<body>' + tot;\r\n")
          _T("  tot = \"<DIV STYLE='border-style:solid; border-width=2; width:\" + wide + \"; height:\" + high + \";'>\" + tot + \"</DIV>\";\r\n")
          _T("  oPopup.document.body.innerHTML = tot;\r\n")
          _T("}\r\n")
          _T("function ADHShowPopup(link,theID,w,h)\r\n")
          _T("{\r\n")
          _T("  if(w != null) wide = w;\r\n")
          _T("  if(h != null) high = h;\r\n")
          _T("  dwn.startDownload(link,onDone);\r\n")
          _T("  oPopup.show(50, 50, wide, high, theID);\r\n")
          _T("}\r\n");

  CComPtr<IHTMLElementCollection> col;
  HRESULT hr = m_Doc2->get_all(&col);
  if(SUCCEEDED(hr))
  {
    CComBSTR tagname = L"head";
    CComVariant selector(tagname);
    CComPtr<IDispatch> tdisp;
    hr = col->tags(selector,&tdisp);
    CComQIPtr<IHTMLElementCollection,&IID_IHTMLElementCollection> tagscol = tdisp;
    if(SUCCEEDED(hr))
    {
      CComVariant ask((long)0);
      CComPtr<IDispatch> item = NULL;
      hr = tagscol->item(ask,ask,&item);
      CComQIPtr<IHTMLElement,&IID_IHTMLElement> elem = item;
      if(SUCCEEDED(hr) && elem.p)
      {
        CComBSTR bText;
        elem->get_innerHTML(&bText);
        CString cText = CW2CT(bText);
        if(cText.Find(_T("ADHShowPopup")) > 0)
        {
          // Has already popup functions
          return;
        }
      }
    }
  }
  // Add popup functions in header
  CComPtr<IHTMLStyle> style;
  CComPtr<IHTMLElement> elemD = Misc::CreateHeadElement(m_Doc2,TAGID_META);
  elemD->put_id(L"dwn");
  elemD->get_style(&style);
  style->put_cssText(L"behavior:url(#default#download)");
  CComPtr<IHTMLElement> elemS = Misc::CreateHeadElement(m_Doc2,TAGID_SCRIPT);
  HtmlScript script(elemS);
  script.SetText(popupFunctions);
  script.SetType(_T("text/javascript"));
}

void
CHTMLEdView::OnDoubleClickImage(CComPtr<IHTMLElement> pElement)
{
  CComPtr<IHTMLElement> image = Misc::AreaToImg(m_Doc2,pElement);
  HtmlImg img(image);
  ImageDlg diag(this,&img,GetBase());
  diag.DoModal();
}

void
CHTMLEdView::OnImage()
{
  CString cTag;
  INT_PTR result = 0;
  CComPtr<IHTMLElement> pElement;

  if(!InsideTag(_T("img"),pElement))
  {
    HRESULT hr = m_Doc2->elementFromPoint(m_HITxPos,m_HITyPos,&pElement);
    if(!(SUCCEEDED(hr) && pElement.p))
    {
      // Advanced hittest for floating objects
      Misc::GetElementByPositionAndTag(m_Doc2,pElement,m_HITxPos,m_HITyPos,_T("img"));
    }
  }
  if(pElement.p)
  {
    CComBSTR tag;
    pElement->get_tagName(&tag);
    cTag = CW2CT(tag);
  }
  if(cTag.CompareNoCase(_T("area")) == 0)
  {
    CComPtr<IHTMLElement> image = Misc::AreaToImg(m_Doc2,pElement);
    HtmlImg img(image);
    ImageDlg diag(this,&img,GetBase());
    result = diag.DoModal();
  }
  else
  {
    pElement = CreateNewElement(TAGID_IMG,NULL);
    result = IDOK;
    {
      HtmlImg img(pElement);
      ImageDlg diag(this,&img,GetBase());
      result = diag.DoModal();
    }
    if(result != IDOK)
    {
      // No image made. remove the element
      CComQIPtr<IHTMLDOMNode,&IID_IHTMLDOMNode> node = pElement;
      node->removeNode(VARIANT_FALSE,NULL);
    }
  }
}

BOOL
CHTMLEdView::GotoNextCell()
{
  if(InTableCell())
  {
    HtmlTableCell cell = GetSelectedCell();
    if(cell.Valid())
    {
      CComPtr<IHTMLElement> elem = cell.FocusNextCell();
      if(elem)
      {
        //return MoveCaretToElement(elem,CARET_DIRECTION_FORWARD);
        return Misc::MoveCaretToElement(m_Doc2,elem,CARET_DIRECTION_INDETERMINATE);
      }
    }
  }
  return FALSE;
}

BOOL
CHTMLEdView::GotoPreviousCell()
{
  if(InTableCell())
  {
    HtmlTableCell cell = GetSelectedCell();
    if(cell.Valid())
    {
      CComPtr<IHTMLElement> elem = cell.FocusPrevCell();
      if(elem)
      {
        return Misc::MoveCaretToElement(m_Doc2,elem,CARET_DIRECTION_INDETERMINATE);
      }
    }
  }
  return FALSE;
}

/////////////////////////////////////////////////////////////////////////

CString
CHTMLEdView::GetMetaTag(CString p_name)
{
  // <META content="MSHTML 6.00.2800.1522" name=GENERATOR>
  CString tag_content;

  if(m_Doc2 == NULL)
  {
    return _T("");
  }
  CComPtr<IHTMLElementCollection> col;
  HRESULT hr = m_Doc2->get_all(&col);
  if(SUCCEEDED(hr))
  {
    CComBSTR name = L"meta";
    CComVariant selector(name);
//     V_VT(&selector) = VT_BSTR;
//     V_BSTR(&selector) = name;
    CComPtr<IDispatch> tdisp;
    hr = col->tags(selector,&tdisp);
    CComQIPtr<IHTMLElementCollection,&IID_IHTMLElementCollection> tagscol = tdisp;
    if(SUCCEEDED(hr))
    {
      long num = 0;
      hr = tagscol->get_length(&num);
      if(SUCCEEDED(hr) && num > 0)
      {
        // Loop through all the meta tags
        for(int metas = 0; metas < num; ++metas)
        {
          CComVariant ask((long)metas);
//           V_VT(&ask) = VT_I4;
//           V_I4(&ask) = metas;
          CComPtr<IDispatch> item = NULL;
          hr = tagscol->item(ask,ask,&item);
          if(SUCCEEDED(hr))
          {
            CComQIPtr<IHTMLMetaElement,&IID_IHTMLMetaElement> meta = item;
            CComBSTR meta_name;
            CComBSTR meta_content;
            meta->get_name(&meta_name);
            meta->get_content(&meta_content);
            CString mname = CW2CT(meta_name);
            CString content = CW2CT(meta_content);
            // TRACE("Meta tag. Name [%s] Content [%s]\n",mname.GetString(),content.GetString());
            if(mname.CompareNoCase(p_name) == 0)
            {
              tag_content = content;
            }
            if(mname.CompareNoCase(_T("generator")) == 0)
            {
              //CComQIPtr<IHTMLDOMNode,&IID_IHTMLDOMNode> dom = meta;
              //CComQIPtr<IHTMLDOMNode,&IID_IHTMLDOMNode> rem;
              //hr = dom->removeNode(TRUE,&rem);
              //break;
              CComBSTR our_generator = L"AuthorDocumentation";
              hr = meta->put_content(our_generator);
            }
          }
        }
      }
    }
  }
  return tag_content;
}

void
CHTMLEdView::SetOurGenerator()
{
  // Only set after calling twice!!
  GetMetaTag(_T("generator"));
}

void
CHTMLEdView::SetClassName(CString name)
{
  CString tag;
  CString classname;
  CString combotext = name;

  Misc::ReconstructTagClass(combotext,tag,classname);
  TCHAR type = combotext.GetAt(0); // B/C/S

  if(tag.IsEmpty() && classname.IsEmpty())
  {
    // Nothing to set. Happens at "<No style>"
    return;
  }   
  CComPtr<IHTMLSelectionObject> pSelObj;
  CComPtr<IHTMLTxtRange>        pTxtRange;
  CComBSTR bstrText;
  CString  theText;

  // See if we have a selection pending
  // if yes, apply these changes to the selection
  HRESULT hr = m_Doc2->get_selection(&pSelObj);
  if (SUCCEEDED(hr)) 
  {
    hr = pSelObj->createRange((IDispatch**)&pTxtRange);
    if (SUCCEEDED(hr)) 
    {
      pTxtRange->get_htmlText(&bstrText);
      if(bstrText.ByteLength() > 0)
      {
        CString text = CW2CT(bstrText);
        CComBSTR newText = CT2CW(Misc::AddTagClass(text,tag,classname));
        pTxtRange->pasteHTML(newText);

        // OK, Ready: We pasted this tag/class
        return;
      }
    }
  }

  // Search highest block formatter
  CComPtr<IHTMLElement> pHighBlock;
  GetCurrentElement(&pHighBlock);
  HtmlElement elem(pHighBlock);
  do 
  {
    CString elemTag = elem.GetTagName();
    if(elemTag == _T("BODY"))
    {
      // Don't go higher than the 'body' in the hierarchy
      break;
    }
    if(Misc::IsBlockTag(elemTag))
    {
      CString replace;

      if(type == _T('S'))
      {
        // Adds simple selector to outer element
        elem.SetClassName(classname);
        return;
      }
      if(Misc::IsUserSelectableTag(elemTag) && Misc::IsBlockTag(tag))
      {
        // Tag is normaly accessible in Styles Combo
        replace = elem.GetOuterHtml();
        replace = Misc::RemoveTag(replace,elemTag);
        replace = Misc::AddTagClass(replace,tag,classname);
        // Can only succeed on user-selectable tag-types
        elem.SetOuterHtml(replace);
        return;
      }
      if(type == _T('C') || (!tag.IsEmpty() && Misc::IsBlockTag(tag)))
      {
        // Tag is a block tag, but not user-settable (TD)
        // Place tag/style at inner html
        // Adds SPAN/DIV to inner part
        replace = elem.GetInnerHtml();
        replace = Misc::AddTagClass(replace,tag,classname);
        elem.SetInnerHtml(replace);
        return;
      }
      break;
    }
    elem = elem.GetParent();
  } 
  while(elem.Valid());
  
  CString message;
  message.Format(_T("Cannot set the style [%s.%s]"),tag.GetString(),classname.GetString());
  theApp.ErrorMessage(message);
}

void
CHTMLEdView::OnInsertHR()
{
  ExecHelperNN(IDM_HORIZONTALLINE); 
}

void
CHTMLEdView::OnDoubleClickHR(CComPtr<IHTMLElement> pElement)
{
  HRDialog diag(NULL,GetBase(),pElement);
  diag.DoModal();
}

void
CHTMLEdView::OnInsertBR()
{
  PasteTextHTML(_T("<br>"));
}

// Create a new HTML element in the text flow at this point
CComPtr<IHTMLElement>
CHTMLEdView::CreateNewElement(ELEMENT_TAG_ID tag,CString* p_attributes)
{
  CComPtr<IDisplayServices> spDS;
  CComPtr<IMarkupServices>  spMS;
  CComPtr<IDisplayPointer>  spDP;
  CComPtr<IMarkupPointer>   spMP;
  CComPtr<IHTMLCaret>       spCaret;
  CComPtr<IHTMLElement>     spFElem;
  CComPtr<IHTMLElement>     spNewElement;

  m_Doc2->QueryInterface(IID_IDisplayServices, (void**)&spDS);
  spDS->CreateDisplayPointer(&spDP);
  spDS->GetCaret(&spCaret);
  spCaret->MoveDisplayPointerToCaret(spDP);
  spDP->GetFlowElement(&spFElem);

  m_Doc2->QueryInterface(IID_IMarkupServices, (void**)&spMS);
  if(p_attributes)
  {
    CComBSTR attrib(*p_attributes);
    spMS->CreateElement(tag,attrib,&spNewElement);
  }
  else
  {
    spMS->CreateElement(tag,NULL,&spNewElement);
  }
  spMS->CreateMarkupPointer(&spMP);
  spDP->PositionMarkupPointer(spMP);
  spMS->InsertElement(spNewElement, spMP, NULL);

  return spNewElement;
}

// Insert a new comment in between a text.
// Rather complicated display.
// Comes from the 'Annotator' example in MSDN
void
CHTMLEdView::OnInsertComment()
{
  CommentDialog diag(_T(""),this);
  if(diag.DoModal() == IDOK)
  {
    CString comment = diag.GetText();
    CComBSTR bText  = CT2CW(comment);
    CComPtr<IHTMLElement> elem = CreateNewElement(TAGID_COMMENT,NULL);

    CComQIPtr<IHTMLCommentElement,&IID_IHTMLCommentElement> comm = elem;
    comm->put_text(bText);
  }
}

void
CHTMLEdView::OnDoubleClickComment(CComPtr<IHTMLElement> pElem)
{
  bool rawComment = false;
  CComBSTR bText;
  CComQIPtr<IHTMLCommentElement,&IID_IHTMLCommentElement> comm = pElem;
  if(!comm)
  {
    return;
  }
  comm->get_text(&bText);
  CString sText = bText;
  if(sText.Left(4) == _T("<!--"))
  {
    sText = sText.Mid(4);
    rawComment = true;
  }
  if(sText.Right(3) == _T("-->"))
  {
    sText = sText.Left(sText.GetLength() - 3);
    rawComment = true;
  }
  CommentDialog diag(sText,this);
  if(diag.DoModal() == IDOK)
  {
    CString comment = diag.GetText();
    if(rawComment)
    {
      comment = CString(_T("<!--")) + comment + _T("-->");
    }
    bText = comment;
    comm->put_text(bText);
  }
}

void
CHTMLEdView::OnEditComment()
{
  CComPtr<IHTMLElement> pElem;
  if(InsideTag(_T("!"),pElem))
  {
    OnDoubleClickComment(pElem);
  }
  else if(InsideTag(_T("COMMENT"),pElem))
  {
    OnDoubleClickComment(pElem);
  }
}

void 
CHTMLEdView::OnCellTop()
{
  HtmlTableCell i_Cell  = GetSelectedCell(); // outputs error
  if(i_Cell.Valid())
  {
    i_Cell.SetProperty(HtmlTableCell::E_AlignVert,_T("top"));
  }
}

void 
CHTMLEdView::OnCellMiddle()
{
  HtmlTableCell i_Cell  = GetSelectedCell(); // outputs error
  if(i_Cell.Valid())
  {
    i_Cell.SetProperty(HtmlTableCell::E_AlignVert,_T("middle"));
  }
}

void 
CHTMLEdView::OnCellBottom()
{
  HtmlTableCell i_Cell  = GetSelectedCell(); // outputs error
  if(i_Cell.Valid())
  {
    i_Cell.SetProperty(HtmlTableCell::E_AlignVert,_T("bottom"));
  }
}

void
CHTMLEdView::OnInsertSymbol()
{
  CharacterDialog diag(this);
  if(diag.DoModal() == IDOK)
  {
    CString text = diag.GetHTML();
    PasteTextHTML(text);
  }
}

void
CHTMLEdView::OnProperties()
{
  if(!m_complete)
  {
    // Place ourselves in the back of the queue
    // This happens if we ask properties from within a ProjectView
    // and the document must be loaded first.
    PostMessage(WM_COMMAND,ID_PROPERTIES,0);
    return;
  }
  bool doNew = false;
  DocumentFile* doc = NULL;
  ProjectFile* project = theApp.GetProjectFile();
  if(project)
  {
    CString path = m_URL;
    CString relative;
    CString base = GetBase();
    if(Misc::MakeRelativePathname(base,m_URL,relative))
    {
      path = relative;
    }
    doc = project->FindDocumentFile(relative);
  }
  if(!doc)
  {
    doNew = true;
    doc   = new DocumentFile(m_URL);
  }
  doc->RetrieveFromDocument(m_Doc2);

  // Test if we get a frameset or a body
  CComPtr<IHTMLElement> elem;
  m_Doc2->get_body(&elem);
  CComQIPtr<IHTMLFrameSetElement,&IID_IHTMLFrameSetElement> frame = elem;
  CComQIPtr<IHTMLBodyElement,    &IID_IHTMLBodyElement>     body  = elem;
  if(frame.p == NULL && body.p == NULL)
  {
    theApp.ErrorMessage(_T("This topic doesn't have a <BODY> nor <FRAMESET> element.")
                        _T("AuthorHTML is unable to display a topic dialog.")
                        _T("Is it a truly correct HTML 4.x page?"));
    return;
  }
  // Start the topic dialog
  TopicPropertiesDlg dlg(this,doc,m_Doc2,GetBase());
  if(dlg.DoModal() == IDOK)
  {
    // Properties dialog can rename the file
    if(dlg.GetRenamed())
    {
      // Reset filename of the document
      // Changes from the dialog might get saved
      CString oldFilename = m_URL;

      m_URL = GetBase() + doc->GetFilename();
      CHTMLEdDoc* document = GetDocument();
      document->SetPathName(m_URL,TRUE);
      document->SaveMyDocument();

      // Now copy it
      MainFrame* _tmain = (MainFrame*)theApp.m_pMainWnd;
      if(_tmain->CopyFileForRename(oldFilename,m_URL))
      {
        // Re-Navigate to the changed file
        Navigate2(m_URL);
      }
    }
    else
    {
      theApp.m_pMainWnd->PostMessage(WM_COMMAND,ID_VIEW_BROWSE,0);
      theApp.m_pMainWnd->PostMessage(WM_COMMAND,ID_VIEW_WEB_REDISPLAY,0);
      theApp.m_pMainWnd->PostMessage(WM_COMMAND,ID_VIEW_BROWSE,0);
    }
  }
  // Delete unneeded dcoumentFile
  if(doNew)
  {
    delete doc;
  }
}

void
CHTMLEdView::OnMarquee()
{
  CComPtr<IHTMLElement> elem;

  if(!InsideTag(_T("marquee"),elem))
  {
    CString text = GetCurrentSelectionText();

    if(!text.IsEmpty())
    {
      OnDeleteKey();
      elem = CreateNewElement(TAGID_MARQUEE,NULL);
      if(elem.p)
      {
        CComBSTR bText = CT2CW(text);
        elem->put_innerHTML(bText);
      }
      else
      {
        PasteText(text);
      }
    }
  }
  else
  {
    OnDoubleClickMarquee(elem);
  }
}

void
CHTMLEdView::OnDoubleClickMarquee(CComPtr<IHTMLElement> pElement)
{
  CComQIPtr<IHTMLMarqueeElement,&IID_IHTMLMarqueeElement> marquee = pElement;
  MarqueeDialog diag(this,marquee,GetBase());
  diag.DoModal();
}

void
CHTMLEdView::OnBreakBelowImages()
{
  CString breakAll = _T("<br clear=\"all\">");
  PasteTextHTML(breakAll);
}

void
CHTMLEdView::GetStyleSheets(CSSVector& list,bool p_link)
{
  CComPtr<IHTMLElementCollection> coll;
  HRESULT hr = m_Doc2->get_all(&coll);
  if(SUCCEEDED(hr))
  {
    CComBSTR name = p_link ? L"link" : L"style";
    CComVariant selector(name);
    CComPtr<IDispatch> tdisp;
    hr = coll->tags(selector,&tdisp);
    CComQIPtr<IHTMLElementCollection,&IID_IHTMLElementCollection> tagscol = tdisp;
    if(SUCCEEDED(hr))
    {
      long num = 0;
      hr = tagscol->get_length(&num);
      if(SUCCEEDED(hr) && num > 0)
      {
        // Loop through all the link tags
        for(int links = 0; links < num; ++links)
        {
          CComVariant ask((long)links);
          CComPtr<IDispatch> item = NULL;
          hr = tagscol->item(ask,ask,&item);
          if(SUCCEEDED(hr))
          {
            CComQIPtr<IHTMLElement,&IID_IHTMLElement> elem = item;
            if(p_link)
            {
              CComQIPtr<IHTMLLinkElement,&IID_IHTMLLinkElement> link = item;
              CComBSTR url,rel;
              link->get_href(&url);
              link->get_rel (&rel);
              CString cRel = CW2CT(rel);
              CString cUrl = CW2CT(url);
              if(cRel.CompareNoCase(_T("Stylesheet")) == 0)
              {
                // To resolve issues as %5C in pathnames
                Misc::ResolveSpecialChars(cUrl);
                SheetDef def;
                def.link  = elem;
                def.style = NULL;
                def.filename = cUrl;
                list.push_back(def);
                // Hold him
                elem.p->AddRef();
              }
            }
            else // style
            {
              CComQIPtr<IHTMLStyleElement,&IID_IHTMLStyleElement> style = item;
              CComBSTR bText;
              style->get_type(&bText);
              CString type = CW2CT(bText);
              if(type.IsEmpty() || type.CompareNoCase(_T("text/css")) == 0)
              {
                SheetDef def;
                def.style = elem;
                def.link  = NULL;
                elem->get_innerHTML(&bText);
                def.csstext = CW2CT(bText);
                list.push_back(def);
                elem.p->AddRef();
              }
            }
          }
        }
      }
    }
  }
}

void
CHTMLEdView::OnEditStyleSheets()
{
  CSSVector list;
  GetStyleSheets(list,true);  // Get LINK's 
  GetStyleSheets(list,false); // Get STYLE's 
  CString type = _T("Style sheets in topic");

  CString filename = GetDocument()->GetPathName();
  CString pathname = Misc::DirectoryPart(filename);
  if(pathname.IsEmpty())
  {
    AuthorHTMLApp* app = (AuthorHTMLApp*)AfxGetApp();
    pathname = app->GetBaseDirectory();
  }
  StyleSheetsDlg dlg(this,type,list,m_Doc2,pathname);
  dlg.DoModal();
  if(dlg.GetDidChanges())
  {
    // Re-Navigate to this document
    // Re-Synch our display in case we did delete/change some stylesheet links
    // This is a hack around an IE bug: removing of a stylesheet does 
    // not render the document. The only way to re-render it, is by reloading it!!
    m_pBrowserApp->ExecWB((OLECMDID)OLECMDID_SAVE,MSOCMDEXECOPT_DODEFAULT,NULL,NULL);
    CHtmlEditView::Navigate2(GetDocument()->GetPathName());  
  }
}

void
CHTMLEdView::OnUpdateFormatParagraph(CCmdUI *pCmdUi)
{
  bool enab = InsideTag(_T("p"));
  pCmdUi->Enable(enab);
}

void
CHTMLEdView::OnFormatParagraph()
{
  CComPtr<IHTMLElement> elem;
  if(InsideTag(_T("p"),elem))
  {
    OnDoubleClickParagraph(elem);
  }
}

void
CHTMLEdView::OnDoubleClickParagraph(CComPtr<IHTMLElement> elem)
{
  CComPtr<IHTMLStyle> style;
  HRESULT hr = elem->get_style(&style);
  if(SUCCEEDED(hr))
  {
    CComBSTR bText;
    style->get_cssText(&bText);
    XString cText = CW2CT(bText);
    cText = XString(_T("p {")) + cText + _T("}");
    CssStyleSheet css;
    css.parse_css(cText);

    // Do the paragraph dialog
    ParagraphDlg dlg(this,GetBase(),&css,elem,style);
    dlg.DoModal();
  }
}

//////////////////////////////////////////////////////////////////////////////
//
// GRID: SHOW GRIDLINES AND SNAP-TO-GRID
//
//////////////////////////////////////////////////////////////////////////////

// Switch the status of the grid
void
CHTMLEdView::OnShowGrid()
{
  if(m_pEditSite)
  {
    bool state = m_pEditSite->Grid();
    m_pEditSite->Grid(!state);
  }
}

// Switch the status of the snap
void
CHTMLEdView::OnSnapToGrid()
{
  if(m_pEditSite)
  {
    bool state = m_pEditSite->Snap();
    m_pEditSite->Snap(!state);
  }
}

void 
CHTMLEdView::OnUpdateGrid(CCmdUI *pCmdUi)
{
  bool state = false;
  if(m_pEditSite)
  {
    state = m_pEditSite->Grid();
  }
  pCmdUi->SetCheck(state);
}

void 
CHTMLEdView::OnUpdateSnap(CCmdUI *pCmdUi)
{
  bool state = false;
  if(m_pEditSite)
  {
    state = m_pEditSite->Snap();
  }
  pCmdUi->SetCheck(state);
}

// Edit Grid/Snap-to-grid properties
void
CHTMLEdView::OnGridProperties()
{
  bool doGrid = false;
  bool doSnap = false;
  CString gW,gH,sW,sH,LC;
  int gw,gh,sw,sh,lc;
  int red,green,blue;
  if(m_pEditSite)
  {
    doGrid = m_pEditSite->Grid();
    doSnap = m_pEditSite->Snap();
    m_pEditSite->GetGridSize(gw,gh);
    m_pEditSite->GetSnapSize(sw,sh);
    lc = m_pEditSite->Color();
    gW.Format(_T("%d"),gw);
    gH.Format(_T("%d"),gh);
    sW.Format(_T("%d"),sw);
    sH.Format(_T("%d"),sh);
    red   = GetRValue(lc);
    green = GetGValue(lc);
    blue  = GetBValue(lc);
    LC.Format(_T("#%02x%02x%02x"),red,green,blue);
    SnapGridDlg dlg(this,doGrid,doSnap,gW,gH,sW,sH,LC);
    if(dlg.DoModal() == IDOK)
    {
      // Get new values
      doGrid = dlg.GetGridEnable();
      doSnap = dlg.GetSnapEnable();
      gw = _ttoi(dlg.GetGridWith());
      gh = _ttoi(dlg.GetGridHeight());
      sw = _ttoi(dlg.GetSnapWith());
      sh = _ttoi(dlg.GetSnapHeight());
      LC = dlg.GetLineColor();
      Misc::DecodeColor(LC,red,green,blue);
      lc = RGB(red,green,blue);
      // Make effective
      m_pEditSite->GridSize(gw,gh);
      m_pEditSite->SnapSize(sw,sh);
      m_pEditSite->Snap(doSnap);
      m_pEditSite->Color(lc);

      // Repaint the grid
      m_pEditSite->Grid(FALSE);
      m_pEditSite->Grid(doGrid);
    }
  }
  else
  {
    theApp.ErrorMessage(_T("No connection to EditConrolSite established, cannot paint grids!"));
  }
}

void
CHTMLEdView::OnUnsupportedHTML(CString tag)
{
  CString msg;
  msg.Format(_T("Sorry: The tag '%s' is currently unsupported by %s\n"),tag.GetString(),REGISTER_APP);
  msg += _T("This is due to the current limitations of Microsoft Internet Explorer\n");
  msg += _T("Use the source-view to edit the HTML source manually.");
  theApp.MessageBox(msg,_T("Unsupported"),MB_OK|MB_ICONEXCLAMATION);
}

//////////////////////////////////////////////////////////////////////////////
//
// DIVERSE TEXT TAGS ABBR/ACRONYM/ADDRESS
//
//////////////////////////////////////////////////////////////////////////////

void
CHTMLEdView::OnInsertAcronym()
{
  CString text = GetCurrentSelectionText();
  DivTextDlg dlg(this,GetBase(),_T("acronym"),_T(""),text,NULL);
  if(dlg.DoModal() == IDOK)
  {
    CComBSTR bString = CT2CW(dlg.GetText());
    CComPtr<IHTMLElement> elem = CreateNewElement(TAGID_ACRONYM,NULL);
    elem->put_innerText(bString);
    bString = CT2CW(dlg.GetTitle());
    elem->put_title(bString);
  }
}

void
CHTMLEdView::OnInsertAddress()
{
  CString text = GetCurrentSelectionText();
  DivTextDlg dlg(this,GetBase(),_T("address"),_T(""),text,NULL);
  if(dlg.DoModal() == IDOK)
  {
    CComBSTR bString = CT2CW(dlg.GetText());
    CComPtr<IHTMLElement> elem = CreateNewElement(TAGID_ADDRESS,NULL);
    elem->put_innerText(bString);
    // Address has no title
  }
}

void 
CHTMLEdView::OnDoubleClickAcronym(CComPtr<IHTMLElement> pElement)
{
  CComBSTR bTitle;
  CComBSTR bText;
  pElement->get_title(&bTitle);
  pElement->get_innerText(&bText);
  CString cTitle = CW2CT(bTitle);
  CString cText  = CW2CT(bText);
  HtmlElement elem(pElement);
  DivTextDlg dlg(this,GetBase(),_T("acronym"),cTitle,cText,&elem);
  if(dlg.DoModal() == IDOK)
  {
    bTitle = CT2CW(dlg.GetTitle());
    bText  = CT2CW(dlg.GetText());
    pElement->put_title(bTitle);
    pElement->put_innerText(bText);
  }
}

void 
CHTMLEdView::OnDoubleClickAddress(CComPtr<IHTMLElement> pElement)
{
  CComBSTR bText;
  pElement->get_innerText(&bText);
  CString cText  = CW2CT(bText);
  HtmlElement elem(pElement);
  DivTextDlg dlg(this,GetBase(),_T("address"),_T(""),cText,&elem);
  if(dlg.DoModal() == IDOK)
  {
    bText  = CT2CW(dlg.GetText());
    pElement->put_innerText(bText);
  }
}

void
CHTMLEdView::OnSelectionWarning(CString forTag)
{
  CString msg;
  msg.Format(_T("Select a piece of text before marking the text as: %s"),forTag.GetString());
  theApp.MessageBox(msg,_T("Missing selection"),MB_OK|MB_ICONEXCLAMATION);
}

void
CHTMLEdView::OnBig()
{
  CString text = GetCurrentSelectionText();
  if(text.IsEmpty())
  {
    OnSelectionWarning(_T("Bigger text"));
    return;
  }
  OnDeleteKey();
  text = CString(_T("<BIG>")) + text + _T("</BIG>");
  PasteTextHTML(text);
}

void
CHTMLEdView::OnBlockQuote()
{
  CString text = GetCurrentSelectionText();
  if(text.IsEmpty())
  {
    OnSelectionWarning(_T("Blockquote"));
    return;
  }
  OnDeleteKey();
  text = CString(_T("<BLOCKQUOTE>")) + text + _T("</BLOCKQUOTE>");
  PasteTextHTML(text);
}

void
CHTMLEdView::OnCite()
{
  CString text = GetCurrentSelectionText();
  if(text.IsEmpty())
  {
    OnSelectionWarning(_T("Paragraph citation"));
    return;
  }
  OnDeleteKey();
  text = CString(_T("<CITE>")) + text + _T("</CITE>");
  PasteTextHTML(text);
}

void
CHTMLEdView::OnCode()
{
  CString text = GetCurrentSelectionText();
  if(text.IsEmpty())
  {
    OnSelectionWarning(_T("Code sample"));
    return;
  }
  OnDeleteKey();
  text = CString(_T("<CODE>")) + text + _T("</CODE>");
  PasteTextHTML(text);
}

void
CHTMLEdView::OnDeletedText()
{
  CString text = GetCurrentSelectionText();
  if(text.IsEmpty())
  {
    OnSelectionWarning(_T("Deleted text"));
    return;
  }
  OnDeleteKey();
  text = CString(_T("<DEL>")) + text + _T("</DEL>");
  PasteTextHTML(text);
}

void
CHTMLEdView::OnInsertedText()
{
  CString text = GetCurrentSelectionText();
  if(text.IsEmpty())
  {
    OnSelectionWarning(_T("Inserted text"));
    return;
  }
  OnDeleteKey();
  text = CString(_T("<INS>")) + text + _T("</INS>");
  PasteTextHTML(text);
}

void
CHTMLEdView::OnKbd()
{
  CString text = GetCurrentSelectionText();
  if(text.IsEmpty())
  {
    OnSelectionWarning(_T("Keyboard-input"));
    return;
  }
  OnDeleteKey();
  text = CString(_T("<KBD>")) + text + _T("</KBD>");
  PasteTextHTML(text);
}

void
CHTMLEdView::OnPre()
{
  CString text = GetCurrentSelectionText();
  if(text.IsEmpty())
  {
    OnSelectionWarning(_T("Preformatted text"));
    return;
  }
  OnDeleteKey();
  text = CString(_T("<PRE>")) + text + _T("</PRE>");
  PasteTextHTML(text);
}

void
CHTMLEdView::OnQuotation()
{
  CString text = GetCurrentSelectionText();
  if(text.IsEmpty())
  {
    OnSelectionWarning(_T("Inline Quotation"));
    return;
  }
  OnDeleteKey();
  text = CString(_T("<Q>")) + text + _T("</Q>");
  PasteTextHTML(text);
}

void
CHTMLEdView::OnSample()
{
  CString text = GetCurrentSelectionText();
  if(text.IsEmpty())
  {
    OnSelectionWarning(_T("Sample"));
    return;
  }
  OnDeleteKey();
  text = CString(_T("<SAMP>")) + text + _T("</SAMP>");
  PasteTextHTML(text);
}

void
CHTMLEdView::OnSmaller()
{
  CString text = GetCurrentSelectionText();
  if(text.IsEmpty())
  {
    OnSelectionWarning(_T("Smaller text"));
    return;
  }
  OnDeleteKey();
  text = CString(_T("<SMALL>")) + text + _T("</SMALL>");
  PasteTextHTML(text);
}

void
CHTMLEdView::OnSubscript()
{
  CString text = GetCurrentSelectionText();
  if(text.IsEmpty())
  {
    OnSelectionWarning(_T("Subscript"));
    return;
  }
  OnDeleteKey();
  text = CString(_T("<SUB>")) + text + _T("</SUB>");
  PasteTextHTML(text);
}

void
CHTMLEdView::OnSuperscript()
{
  CString text = GetCurrentSelectionText();
  if(text.IsEmpty())
  {
    OnSelectionWarning(_T("Superscript"));
    return;
  }
  OnDeleteKey();
  text = CString(_T("<SUP>")) + text + _T("</SUP>");
  PasteTextHTML(text);
}

void
CHTMLEdView::OnTeletype()
{
  CString text = GetCurrentSelectionText();
  if(text.IsEmpty())
  {
    OnSelectionWarning(_T("Teletype text"));
    return;
  }
  OnDeleteKey();
  text = CString(_T("<TT>")) + text + _T("</TT>");
  PasteTextHTML(text);
}

//////////////////////////////////////////////////////////////
//
// PARAGRAPHSTYLES REPRISE
//
//////////////////////////////////////////////////////////////

void
CHTMLEdView::OnParaStyleParagraph()
{
  SetClassName(_T("P"));
}

void
CHTMLEdView::OnParaStyleHeading1()
{
  SetClassName(_T("H1"));
}

void
CHTMLEdView::OnParaStyleHeading2()
{
  SetClassName(_T("H2"));
}

void
CHTMLEdView::OnParaStyleHeading3()
{
  SetClassName(_T("H3"));
}

void
CHTMLEdView::OnParaStyleHeading4()
{
  SetClassName(_T("H4"));
}

void
CHTMLEdView::OnParaStyleHeading5()
{
  SetClassName(_T("H5"));
}

void
CHTMLEdView::OnParaStyleHeading6()
{
  SetClassName(_T("H6"));
}

void
CHTMLEdView::OnParaStyleAddress()
{
  SetClassName(_T("ADDRESS"));
}

void
CHTMLEdView::OnParaStylePreformat()
{
  SetClassName(_T("PRE"));
}

/////////////////////////////////////////////////////////////////
//
// FORMS AND FORM ELEMENTS
//
/////////////////////////////////////////////////////////////////

void
CHTMLEdView::OnUpdateForm(CCmdUI* pCmdUi)
{
  // Only possible to insert a form if outside a form
  pCmdUi->Enable(!InsideTag(_T("form")));
}

void
CHTMLEdView::OnUpdateFormElements(CCmdUI* pCmdUi)
{
  // Only add form elements inside a form
  pCmdUi->Enable(InsideTag(_T("form")));
}

void
CHTMLEdView::OnInsertForm()
{
  if(InsideTag(_T("form")))
  {
    theApp.ErrorMessage(_T("Already inside a FORM. Forms cannot be nested"));
    return;
  }
  FormDlg dlg(this,GetBase(),_T(""),_T(""),_T(""),_T(""),_T(""),NULL);
  if(dlg.DoModal() == IDOK)
  {
    CComPtr<IHTMLElement> elem = CreateNewElement(TAGID_FORM,NULL);
    CComQIPtr<IHTMLFormElement,&IID_IHTMLFormElement> newForm = elem;
    FormDlgToFormElement(dlg,newForm);
  }
}

void
CHTMLEdView::OnDoubleClickForm(CComPtr<IHTMLElement> pElement)
{
  CComQIPtr<IHTMLFormElement,&IID_IHTMLFormElement> form = pElement;
  if(!form.p)
  {
    // Not a FormElement after all
    return;
  }
  CComBSTR bName;
  CComBSTR bAction;
  CComBSTR bMethod;
  CComBSTR bEncoding;
  CComBSTR bTarget;
  form->get_name(&bName);
  form->get_action(&bAction);
  form->get_method(&bMethod);
  form->get_encoding(&bEncoding);
  form->get_target(&bTarget);
  CString name     = CW2CT(bName);
  CString action   = CW2CT(bAction);
  CString method   = CW2CT(bMethod);
  CString encoding = CW2CT(bEncoding);
  CString target   = CW2CT(bTarget);
  HtmlElement elem(pElement);
  FormDlg dlg(this,GetBase(),name,action,method,encoding,target,&elem);
  if(dlg.DoModal() == IDOK)
  {
    FormDlgToFormElement(dlg,form);
  }
}

void
CHTMLEdView::FormDlgToFormElement(FormDlg& dlg,CComPtr<IHTMLFormElement> form)
{
  CString name     = dlg.GetName();
  CString action   = dlg.GetActionUrl();
  CString method   = dlg.GetMethod();
  CString encoding = dlg.GetEncoding();
  CString target   = dlg.GetTarget();
  CComBSTR bName     = CT2CW(name);
  CComBSTR bAction   = CT2CW(action);
  CComBSTR bMethod   = CT2CW(method);
  CComBSTR bEncoding = CT2CW(encoding);
  CComBSTR bTarget   = CT2CW(target);
  form->put_name(bName);
  form->put_action(bAction);
  form->put_method(bMethod);
  form->put_encoding(bEncoding);
  form->put_target(bTarget);
}

void
CHTMLEdView::OnInsertFormField()
{
  FormFieldDlg dlg(this,GetBase(),_T(""),_T(""),_T(""),_T(""),_T(""),NULL);
  if(dlg.DoModal() == IDOK)
  {
    CString type = dlg.GetType();
    type = CString(_T("type=\"")) + type + _T("\"");
    CComPtr<IHTMLElement> elem = CreateNewElement(TAGID_INPUT,&type);
    CComQIPtr<IHTMLInputElement,&IID_IHTMLInputElement> field = elem;
    FormFieldDlgToInputElement(dlg,field);
  }
}

void
CHTMLEdView::OnDoubleClickFormField(CComPtr<IHTMLElement> pElement)
{
  CComQIPtr<IHTMLInputElement,&IID_IHTMLInputElement> field = pElement;
  if(!field.p)
  {
    // Not a InputElement after all
    return;
  }
  CComBSTR bType;
  CComBSTR bName;
  CComBSTR bValue;
  long length,size;
  field->get_type(&bType);
  field->get_name(&bName);
  field->get_value(&bValue);
  field->get_maxLength(&length);
  field->get_size(&size);
  CString type = CW2CT(bType);
  CString name = CW2CT(bName);
  CString value = CW2CT(bValue);
  CString cLength,cSize;
  cLength.Format(_T("%d"),length);
  cSize  .Format(_T("%d"),size);
  HtmlElement elem(pElement);
  FormFieldDlg dlg(this,GetBase(),type,name,value,cLength,cSize,&elem);
  if(dlg.DoModal() == IDOK)
  {
    FormFieldDlgToInputElement(dlg,field);
  }
}

void
CHTMLEdView::FormFieldDlgToInputElement(FormFieldDlg& dlg,CComPtr<IHTMLInputElement> field)
{
  CString name     = dlg.GetName();
  CString value    = dlg.GetValue();
  long    length   = _ttoi(dlg.GetMaxLength());
  long     size    = _ttoi(dlg.GetSize());
  CComBSTR bName   = CT2CW(name);
  CComBSTR bValue  = CT2CW(value);
  field->put_name(bName);
  field->put_value(bValue);
  field->put_maxLength(length);
  field->put_size(size);
}

void
CHTMLEdView::OnInsertFormButton()
{
  FormButtonDlg dlg(this,GetBase(),_T(""),_T(""),_T(""),false,NULL);
  if(dlg.DoModal() == IDOK)
  {
    CString type = dlg.GetType();
    type = CString(_T("type=\"")) + type + _T("\"");
    CComPtr<IHTMLElement> elem = CreateNewElement(TAGID_INPUT,&type);
    CComQIPtr<IHTMLInputElement,&IID_IHTMLInputElement> field = elem;
    FormButtonDlgToInputElement(dlg,field);
  }
}

void 
CHTMLEdView::OnDoubleClickFormButton (CComPtr<IHTMLElement> pElement)
{
  CComQIPtr<IHTMLInputElement,&IID_IHTMLInputElement> button = pElement;
  if(!button.p)
  {
    // Not a InputElement after all
    return;
  }
  CComBSTR bType;
  CComBSTR bName;
  CComBSTR bValue;
  VARIANT_BOOL checked;
  button->get_type(&bType);
  button->get_name(&bName);
  button->get_value(&bValue);
  button->get_checked(&checked);
  CString type  = CW2CT(bType);
  CString name  = CW2CT(bName);
  CString value = CW2CT(bValue);
  HtmlElement elem(pElement);
  FormButtonDlg dlg(this,GetBase(),type,name,value,(checked == VARIANT_TRUE),&elem);
  if(dlg.DoModal() == IDOK)
  {
    FormButtonDlgToInputElement(dlg,button);
  }
}

void
CHTMLEdView::FormButtonDlgToInputElement(FormButtonDlg& dlg,CComPtr<IHTMLInputElement> button)
{
  CString name     = dlg.GetName();
  CString value    = dlg.GetValue();
  bool    check    = dlg.GetChecked();
  CComBSTR bName   = CT2CW(name);
  CComBSTR bValue  = CT2CW(value);
  button->put_name(bName);
  button->put_value(bValue);
  button->put_checked((VARIANT_BOOL)check);
}

void
CHTMLEdView::OnInsertFormImage()
{
  CString type = _T("type=\"image\"");
  CComPtr<IHTMLElement> elem = CreateNewElement(TAGID_INPUT,&type);
  HtmlInput input(elem);
  FormImageDlg dlg(this,GetBase(),&input);
  if(dlg.DoModal() != IDOK)
  {
    CComQIPtr<IHTMLDOMNode,&IID_IHTMLDOMNode> dom = elem;
    dom->removeNode(VARIANT_FALSE,NULL);
  }
}

void 
CHTMLEdView::OnDoubleClickFormImage(CComPtr<IHTMLElement> pElement)
{
  HtmlInput input(pElement);
  FormImageDlg dlg(this,GetBase(),&input);
  dlg.DoModal();
}

void
CHTMLEdView::OnDoubleClickFormInput(CComPtr<IHTMLElement> pElement)
{
  CComQIPtr<IHTMLInputElement,&IID_IHTMLInputElement> input = pElement;
  CComBSTR bType;
  input->get_type(&bType);
  CString type = CW2CT(bType);
  if(type == _T("button"))   OnDoubleClickFormButton(pElement);
  if(type == _T("checkbox")) OnDoubleClickFormButton(pElement);
  if(type == _T("file"))     OnDoubleClickFormField(pElement);
  if(type == _T("hidden"))   OnDoubleClickFormField(pElement);
  if(type == _T("image"))    OnDoubleClickFormImage(pElement);
  if(type == _T("password")) OnDoubleClickFormField(pElement);
  if(type == _T("radio"))    OnDoubleClickFormButton(pElement);
  if(type == _T("reset"))    OnDoubleClickFormButton(pElement);
  if(type == _T("submit"))   OnDoubleClickFormButton(pElement);
  if(type == _T("text"))     OnDoubleClickFormField(pElement);
}

void
CHTMLEdView::OnInsertFormTextarea()
{
  FormTextareaDlg dlg(this,GetBase(),_T(""),_T(""),_T(""),_T(""),NULL);
  if(dlg.DoModal() == IDOK)
  {
    CComPtr<IHTMLElement> elem = CreateNewElement(TAGID_TEXTAREA,NULL);
    CComQIPtr<IHTMLTextAreaElement,&IID_IHTMLTextAreaElement> area = elem;
    FormTextareaDlgToTextAreaElement(&dlg,area);
  }
}

void 
CHTMLEdView::OnDoubleClickFormTextArea(CComPtr<IHTMLElement> pElement)
{
  CComQIPtr<IHTMLTextAreaElement,&IID_IHTMLTextAreaElement> area = pElement;
  if(!area.p)
  {
    // Not a InputElement after all
    return;
  }
  CComBSTR bName;
  CComBSTR bWrap;
  long cols,rows;
  CString cCols,cRows;
  area->get_name(&bName);
  area->get_wrap(&bWrap);
  area->get_cols(&cols);
  area->get_rows(&rows);
  CString name = CW2CT(bName);
  CString wrap = CW2CT(bWrap);
  cCols.Format(_T("%d"),cols);
  cRows.Format(_T("%d"),rows);
  HtmlElement elem(pElement);
  FormTextareaDlg dlg(this,GetBase(),name,cCols,cRows,wrap,&elem);
  if(dlg.DoModal() == IDOK)
  {
    FormTextareaDlgToTextAreaElement(&dlg,area);
  }
}

void
CHTMLEdView::FormTextareaDlgToTextAreaElement(FormTextareaDlg* dlg,CComPtr<IHTMLTextAreaElement> area)
{
  CString name = dlg->GetName();
  CString cols = dlg->GetCols();
  CString rows = dlg->GetRows();
  CString wrap = dlg->GetWrap();
  CComBSTR bName = CT2CW(name);
  CComBSTR bWrap = CT2CW(wrap);
  area->put_name(bName);
  area->put_cols(_ttoi(cols));
  area->put_rows(_ttoi(rows));
  area->put_wrap(bWrap);
}

void
CHTMLEdView::OnInsertFieldset()
{
  CString text = GetCurrentSelectionText();
  if(text.IsEmpty())
  {
    CString msg = _T("Make a selection first to apply a FIELDSET around it.");
    theApp.MessageBox(msg,_T("No selection"),MB_OK|MB_ICONWARNING);
    return;
  }
  else
  {
    OnDeleteKey();
  }
  text = CString(_T("<FIELDSET>")) + text + _T("</FIELDSET>");
  PasteTextHTML(text);
}

void
CHTMLEdView::OnInsertFormLabel()
{
  FormLabelDlg dlg(this,_T(""));
  if(dlg.DoModal() == IDOK)
  {
    CComPtr<IHTMLElement> elem = CreateNewElement(TAGID_LABEL,NULL);
    CComQIPtr<IHTMLLabelElement,&IID_IHTMLLabelElement> label = elem;
    FormLabelDlgToLabelElement(dlg,label);
  }
}

void 
CHTMLEdView::OnDoubleClickFormLabel(CComPtr<IHTMLElement> pElement)
{
  CComQIPtr<IHTMLLabelElement,&IID_IHTMLLabelElement> label = pElement;
  if(!label.p)
  {
    // Not a InputElement after all
    return;
  }
  CComBSTR bFor;
  label->get_htmlFor(&bFor);
  CString cFor = CW2CT(bFor);
  FormLabelDlg dlg(this,cFor);
  if(dlg.DoModal() == IDOK)
  {
    FormLabelDlgToLabelElement(dlg,label);
  }
}

void
CHTMLEdView::FormLabelDlgToLabelElement(FormLabelDlg& dlg,CComPtr<IHTMLLabelElement> label)
{
  CString  cFor = dlg.GetFor();
  CComBSTR bFor = CT2CW(cFor);
  label->put_htmlFor(bFor);
}

void
CHTMLEdView::OnInsertFormSelect()
{
  FormOptions options;
  FormSelectDlg dlg(this,GetBase(),_T(""),_T(""),true,&options,NULL);
  if(dlg.DoModal() == IDOK)
  {
    CComPtr<IHTMLElement> elem = CreateNewElement(TAGID_SELECT,NULL);
    CComQIPtr<IHTMLSelectElement,&IID_IHTMLSelectElement> select = elem;
    FormSelectDlgToSelectElement(dlg,options,select);
  }
}

void 
CHTMLEdView::OnDoubleClickFormSelect(CComPtr<IHTMLElement> pElement)
{
  CComQIPtr<IHTMLSelectElement,&IID_IHTMLSelectElement> select = pElement;
  if(!select.p)
  {
    // Not a selectElement after all
    return;
  }
  CComBSTR bName;
  long size;
  VARIANT_BOOL multiple = VARIANT_FALSE;
  select->get_name(&bName);
  select->get_size(&size);
  select->get_multiple(&multiple);
  CString cName = CW2CT(bName);
  CString cSize;
  cSize.Format(_T("%d"),size);

  // Get all the options from the select
  FormOptions options;
  long count;
  select->get_length(&count);
  for(int ind = 0; ind < count; ++ind)
  {
    CComVariant index((long)ind);
//     V_VT(&index) = VT_I4;
//     V_I4(&index) = ind;
    IDispatch* disp;
    HRESULT hr = select->item(index,index,&disp);
    if(SUCCEEDED(hr))
    {
      CComQIPtr<IHTMLOptionElement,&IID_IHTMLOptionElement> option = disp;
      CComBSTR bText;
      CComBSTR bValue;
      VARIANT_BOOL sel;
      option->get_text(&bText);
      option->get_value(&bValue);
      option->get_defaultSelected(&sel);

      FormOption opt;
      opt.m_display = CW2CT(bText);
      opt.m_value   = CW2CT(bValue);
      opt.m_selected = (sel == VARIANT_TRUE);
      options.push_back(opt);
    }
  }
  HtmlElement elem(pElement);
  FormSelectDlg dlg(this,GetBase(),cName,cSize,(multiple == VARIANT_TRUE),&options,&elem);
  if(dlg.DoModal() == IDOK)
  {
    FormSelectDlgToSelectElement(dlg,options,select);
  }
}

void
CHTMLEdView::FormSelectDlgToSelectElement(FormSelectDlg& dlg
                                         ,FormOptions& options
                                         ,CComPtr<IHTMLSelectElement> select)
{
  XString name = dlg.GetName();
  XString size = dlg.GetSize();
  bool    mult = dlg.GetMultiple();
  CComBSTR bName = CT2CW(name);
  long     lSize = _ttoi(size);

  // Reapply the attributes of the SELECT element
  select->put_name(bName);
  select->put_size(lSize);
  select->put_multiple((VARIANT_BOOL) mult);

  // Remove the options collection
  long count;
  select->get_length(&count);
  for(int ind = 0; ind < count; ++ind)
  {
    select->remove(0);
  }

  // Get a factory from the document window
  CComPtr<IHTMLWindow2> win;
  CComPtr<IHTMLOptionElementFactory> factory;
  m_Doc2->get_parentWindow(&win);
  win->get_Option(&factory);
 
  // Reapply the <OPTONS> of the <SELECT>
  for(unsigned int ind = 0; ind < options.size(); ++ind)
  {
    FormOption opt = options[ind];
    VARIANT_BOOL sel = opt.m_selected ? VARIANT_TRUE : VARIANT_FALSE;
    CComVariant vText (opt.m_display);
    CComVariant vValue(opt.m_value);
    CComVariant vDefault(sel);
    CComVariant vSelect(sel);

    // Ask the factory to create an option
    CComPtr<IHTMLOptionElement> option;
    factory->create(vText,vValue,vDefault,vSelect,&option);
    // Cast the option to an element
    CComQIPtr<IHTMLElement,&IID_IHTMLElement> elem = option;
    // Add the element to the option list at the 'ind'icated position
    CComVariant where;
    V_VT(&where) = VT_I4;
    V_I4(&where) = ind;
    select->add(elem,where);
  }
}

void
CHTMLEdView::OnInsertButton()
{
  CComPtr<IHTMLElement> pElement = CreateNewElement(TAGID_BUTTON,NULL);
  HtmlElement elem(pElement);

  ButtonDlg dlg(this,GetBase(),&elem);
  if(dlg.DoModal() != IDOK)
  {
    CComQIPtr<IHTMLDOMNode,&IID_IHTMLDOMNode> dom = pElement;
    dom->removeNode(VARIANT_FALSE,NULL);
  }
}

void 
CHTMLEdView::OnDoubleClickButton(CComPtr<IHTMLElement> pElement)
{
  HtmlElement elem(pElement);
  ButtonDlg dlg(this,GetBase(),&elem);
  dlg.DoModal();
}

///////////////////////////////////////////////////////////////////
//
// INLINE FRAMES - REUSABLE TEXT BLOCKS
//
///////////////////////////////////////////////////////////////////

void
CHTMLEdView::OnDoubleClickHeading(CString tag,CComPtr<IHTMLElement> pElement)
{
  HtmlHeading head(pElement);
  HeadingDlg diag(this,GetBase(),tag,&head);
  diag.DoModal();
}

void
CHTMLEdView::OnDoubleClickHeading1(CComPtr<IHTMLElement> elem)
{
  OnDoubleClickHeading(_T("h1"),elem);
}
void
CHTMLEdView::OnDoubleClickHeading2(CComPtr<IHTMLElement> elem)
{
  OnDoubleClickHeading(_T("h2"),elem);
}
void
CHTMLEdView::OnDoubleClickHeading3(CComPtr<IHTMLElement> elem)
{
  OnDoubleClickHeading(_T("h3"),elem);
}
void
CHTMLEdView::OnDoubleClickHeading4(CComPtr<IHTMLElement> elem)
{
  OnDoubleClickHeading(_T("h4"),elem);
}
void
CHTMLEdView::OnDoubleClickHeading5(CComPtr<IHTMLElement> elem)
{
  OnDoubleClickHeading(_T("h5"),elem);
}
void
CHTMLEdView::OnDoubleClickHeading6(CComPtr<IHTMLElement> elem)
{
  OnDoubleClickHeading(_T("h6"),elem);
}


///////////////////////////////////////////////////////////////////
//
// INLINE FRAMES - REUSABLE TEXT BLOCKS
//
///////////////////////////////////////////////////////////////////

void
CHTMLEdView::OnInsertIFrame()
{
  CComPtr<IHTMLElement> elem = CreateNewElement(TAGID_IFRAME,NULL);
  HtmlIFrame frame(elem);
  
  InlineFrameDlg dlg(this,GetBase(),&frame);
  if(dlg.DoModal() != IDOK)
  {
    CComQIPtr<IHTMLDOMNode,&IID_IHTMLDOMNode> dom = elem;
    dom->removeNode(VARIANT_FALSE,NULL);
  }
}

void
CHTMLEdView::OnDoubleClickIFrame(CComPtr<IHTMLElement> pElement)
{
  HtmlIFrame frame(pElement);
  InlineFrameDlg dlg(this,GetBase(),&frame);
  dlg.DoModal();
}



///////////////////////////////////////////////////////////////////
//
// SCRIPTS
//
///////////////////////////////////////////////////////////////////

void
CHTMLEdView::OnInsertScript()
{
  CComPtr<IHTMLElement> elem = CreateNewElement(TAGID_SCRIPT,NULL);
  HtmlScript script(elem);

  ScriptDlg dlg(this,&script,GetBase());
  if(dlg.DoModal() != IDOK)
  {
    CComQIPtr<IHTMLDOMNode,&IID_IHTMLDOMNode> dom = elem;
    dom->removeNode(VARIANT_FALSE,NULL);
  }
}

void
CHTMLEdView::OnDoubleClickScript(CComPtr<IHTMLElement> pElement)
{
  HtmlScript script(pElement);
  ScriptDlg dlg(this,&script,GetBase());
  dlg.DoModal();
}

///////////////////////////////////////////////////////////////////
//
// OBJECT (ACTIVE/X - COMPILED JAVA)
//
///////////////////////////////////////////////////////////////////

void
CHTMLEdView::OnInsertObject()
{
  CComPtr<IHTMLElement> elem = CreateNewElement(TAGID_OBJECT,NULL);
  HtmlObject object(elem);

  theApp.MessageBox(_T("The <OBJECT> interface is a generic only interface with no HTMLHelp knowledge.\n")
                    _T("To use it you must know what you are doing here. You are on your own!")
                    ,_T("Warning")
                    ,MB_OK|MB_ICONWARNING);

  ObjectDlg dlg(this,GetBase(),&object);
  if(dlg.DoModal() != IDOK)
  {
    CComQIPtr<IHTMLDOMNode,&IID_IHTMLDOMNode> dom = elem;
    dom->removeNode(VARIANT_FALSE,NULL);
  }
}

void
CHTMLEdView::OnDoubleClickObject(CComPtr<IHTMLElement> pElement)
{
  HtmlObject object(pElement);
  XString classid = object.GetProperty(HtmlObject::E_ClassId);
  if(classid.CompareNoCase(CLSID_ALINK) == 0)
  {
    // It's a ALINK definition
    XString value = object.GetParameter(_T("ALink Name"));
    ALinkDlg dlg(this,value);
    if(dlg.DoModal() == IDOK)
    {
      // Set the parameter to this new value
      object.SetParameter(_T("ALink Name"),dlg.GetKeyword());
    }
  }
  else if(classid.CompareNoCase(CLSID_ALINK_COMMAND) == 0)
  {
    // It's a ALINK Command
    CString base = GetBase();
    ALinkCommandDlg dlg(this,base,&object);
    dlg.DoModal();
  }
  else
  {
    // GENERAL object
    ObjectDlg dlg(this,GetBase(),&object);
    dlg.DoModal();
  }
}

// ALink is a kind-of-object
void
CHTMLEdView::OnInsertALink()
{
  CComPtr<IHTMLElement> elem = CreateNewElement(TAGID_OBJECT,NULL);
  HtmlObject object(elem);
  object.SetProperty(HtmlObject::E_Type,_T("application/x-oleobject"));
  object.SetProperty(HtmlObject::E_ClassId,CLSID_ALINK);

  // Title of the document as default link name
  CComBSTR bTitle;
  m_Doc2->get_title(&bTitle);
  CString title = CW2CT(bTitle);
  if(title.IsEmpty())
  {
    title = _T("Associative keyword to this topic");
  }
  object.SetParameter(_T("ALink Name"),title);

  // Get a value for the parameter
  XString value = title;
  ALinkDlg dlg(this,value);
  if(dlg.DoModal() == IDOK)
  {
    // Set the parameter to this new value
    object.SetParameter(_T("ALink Name"),dlg.GetKeyword());
  }
}

// ALinkCommand is a kind-of-object
void
CHTMLEdView::OnInsertALinkCommand()
{
  CComPtr<IHTMLElement> elem = CreateNewElement(TAGID_OBJECT,NULL);
  HtmlObject object(elem);
  
  object.SetProperty(HtmlObject::E_Type,_T("application/x-oleobject"));
  object.SetProperty(HtmlObject::E_ClassId,CLSID_ALINK_COMMAND);
  object.SetProperty(HtmlObject::E_Width,_T("100"));
  object.SetProperty(HtmlObject::E_Height,_T("30"));
  object.SetParameter(_T("Command"),_T("ALink"));
  object.SetParameter(_T("Button"),_T("Related topics"));

  // Get a value for the parameter
  CString base = GetBase();
  ALinkCommandDlg dlg(this,base,&object);
  dlg.DoModal();
}

///////////////////////////////////////////////////////////////////
//
// DIV/SPAN general containers
//
///////////////////////////////////////////////////////////////////

void
CHTMLEdView::OnDoubleClickSpan(CComPtr<IHTMLElement> pElement)
{
  OnDoubleClickSpanDiv(_T("span"),pElement);
}

void
CHTMLEdView::OnDoubleClickDiv(CComPtr<IHTMLElement> pElement)
{
  HtmlElement elem(pElement);
  if(elem.HasStyle())
  {
    XString style = elem.GetInlineStyle();
    style = XString(_T("div {")) + style + _T("}");
    CssStyleSheet css;
    css.parse_css(style);
    XString media = _T("standard");
    XString select = _T("div");
    if(css.get(_T("standard"),_T("div"),_T("position")).c_str() == XString(_T("absolute")))
    {
      LayerDlg dlg(this,GetBase(),&elem,_T("1"));
      dlg.DoModal();
      return;
    }
  }
  OnDoubleClickSpanDiv(_T("div"),pElement);
}

void
CHTMLEdView::OnDoubleClickSpanDiv(CString p_tag,CComPtr<IHTMLElement> pElement)
{
  HtmlElement elem(pElement);
  SpanDivDlg dlg(this,GetBase(),p_tag,&elem);
  dlg.DoModal();
}

void 
CHTMLEdView::OnDoubleClickCaption(CComPtr<IHTMLElement> pElement)
{
  HtmlCaption caption(pElement);
  TableCaptionDlg dialog(this,GetBase(),&caption);
  dialog.DoModal();
}

void 
CHTMLEdView::OnDoubleClickInlineTag(CComPtr<IHTMLElement> pElement)
{
  HtmlElement elem(pElement);
  CString tag = elem.GetTagName();
  SpanDivDlg dlg(this,GetBase(),tag,&elem);
  dlg.DoModal();
}

void
CHTMLEdView::OnDoubleClickFont(CComPtr<IHTMLElement> pElement)
{
  HtmlElement elem(pElement);
  FontDlg dlg(this,GetBase(),&elem);
  dlg.DoModal();
}

void
CHTMLEdView::OnDoubleClickFieldset(CComPtr<IHTMLElement> pElement)
{
  HtmlFieldset set(pElement);
  FieldsetDlg dlg(this,GetBase(),&set);
  dlg.DoModal();
}

void
CHTMLEdView::OnDoubleClickLegend(CComPtr<IHTMLElement> pElement)
{
  CComPtr<IHTMLElement> pField;
  InsideTag(pElement,_T("FIELDSET"),pField);
  if(pField.p)
  {
    OnDoubleClickFieldset(pField);
  }
}

///////////////////////////////////////////////////////////////////
//
// LAYER
//
///////////////////////////////////////////////////////////////////

void
CHTMLEdView::OnInsertLayer()
{
  CComPtr<IHTMLElement> elem = CreateNewElement(TAGID_DIV,NULL);
  HtmlElement division(elem);

  // Make unique layer number
  long num = Misc::GetUniqueID(m_Doc2,_T("div"));
  CString unique;
  unique.Format(_T("%d"),num);

  LayerDlg dlg(this,GetBase(),&division,unique);
  if(dlg.DoModal() != IDOK)
  {
    CComQIPtr<IHTMLDOMNode,&IID_IHTMLDOMNode> dom = elem;
    dom->removeNode(VARIANT_FALSE,NULL);
  }
}

void
CHTMLEdView::OnRemoveLayer(CComPtr<IHTMLElement> pElement)
{
  HtmlElement layer(pElement);
  CString identity = layer.GetAttribute(_T("id")); 
  CString mess;
  mess.Format(_T("Are you sure you want to remove layer [%s]?"),identity.GetString());
  if(theApp.MessageBox(mess,_T("Question"),MB_YESNO|MB_ICONQUESTION) == IDYES)
  {
    layer.Remove();
  }
}

void
CHTMLEdView::CheckBase()
{
  if(m_Doc2.p == NULL)
  {
    // To early in startup of a page
    return;
  }
  // Step 1: Defaults to name of this document
  m_baseURL = Misc::DirectoryPart(m_URL);
  // Step 2: Base of the project file

  // Step 3: Independent HTML page
  // Base is found by way of the BASE tag in the head
  CString base = Misc::BaseTag(m_Doc2,_T(""),_T(""));
  if(!base.IsEmpty())
  {
    m_baseURL = base;
  }
}

//////////////////////////////////////////////////////////////////////////
//
// Rename file references
// 
//////////////////////////////////////////////////////////////////////////

int
CHTMLEdView::RenameReferences(CString& p_old_href,CString& p_new_href)
{
  // Drop out if no content
  if(m_Doc2 == NULL)
  {
    return 0;
  }
  // Check if we are the file that will be renamed
//   CString pathname = RebasedURL();
//   if(pathname.CompareNoCase(p_old_href) == 0)
//   {
//     return 0;
//   }
  int renamed = 0;
  // Attributes 'href' and 'src' from these
  renamed += RenameReferencesTag(p_old_href,p_new_href,_T("link"));
  renamed += RenameReferencesTag(p_old_href,p_new_href,_T("script"));
  renamed += RenameReferencesTag(p_old_href,p_new_href,_T("img"));
  renamed += RenameReferencesTag(p_old_href,p_new_href,_T("a"));
  renamed += RenameReferencesTag(p_old_href,p_new_href,_T("area"));
  renamed += RenameReferencesTag(p_old_href,p_new_href,_T("frame"));
  renamed += RenameReferencesTag(p_old_href,p_new_href,_T("iframe"));

  return renamed;
}

int
CHTMLEdView::RenameReferencesTag(CString& p_old_href,CString& p_new_href,TCHAR* p_tag)
{
  int renamed = 0;
  const int CaseInsesitive = 0;
  CComPtr<IHTMLElementCollection> col;

  HRESULT hr = m_Doc2->get_all(&col);
  if(SUCCEEDED(hr))
  {
    CComBSTR name = CT2CW(p_tag);
    CComVariant selector(name);
//     V_VT(&selector) = VT_BSTR;
//     V_BSTR(&selector) = name;
    CComPtr<IDispatch> tdisp;
    hr = col->tags(selector,&tdisp);
    CComQIPtr<IHTMLElementCollection,&IID_IHTMLElementCollection> tagscol = tdisp;
    if(SUCCEEDED(hr))
    {
      long num = 0;
      hr = tagscol->get_length(&num);
      if(SUCCEEDED(hr) && num > 0)
      {
        // Loop through all the tags
        for(int tag = 0; tag < num; ++tag)
        {
          CComVariant ask((long)tag);
//           V_VT(&ask) = VT_I4;
//           V_I4(&ask) = tag;
          CComPtr<IDispatch> item = NULL;
          hr = tagscol->item(ask,ask,&item);
          if(SUCCEEDED(hr))
          {
            CComQIPtr<IHTMLElement,&IID_IHTMLElement> element = item;
            
            CComBSTR hname = L"href";
            CComBSTR sname = L"src";
            CComVariant href;
            CComVariant src;
            V_VT(&href) = VT_BSTR;
            V_VT(&src)  = VT_BSTR;
            element->getAttribute(hname,CaseInsesitive,&href);
            element->getAttribute(sname,CaseInsesitive,&src);
            if(V_VT(&href) != VT_NULL)
            {
              CString href_string = V_BSTR(&href);
              CString new_href;

              if(Misc::ReplaceFilenameExtra(m_baseURL,p_old_href,p_new_href,href_string,new_href))
              {
                CComVariant nhref(new_href);
                element->setAttribute(hname,nhref,CaseInsesitive);
                ++renamed;
              }
            }
            if(V_VT(&src) != VT_NULL)
            {
              CString src_string = V_BSTR(&src);
              CString new_href;

              if(Misc::ReplaceFilenameExtra(m_baseURL,p_old_href,p_new_href,src_string,new_href))
              {
                CComVariant nhref(new_href);
                element->setAttribute(sname,nhref, CaseInsesitive);
                ++renamed;
              }
            }
          }
        }
      }
    }
  }
  return renamed;
}

