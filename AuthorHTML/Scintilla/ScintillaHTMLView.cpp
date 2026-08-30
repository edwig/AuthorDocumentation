// ScintillaHTMLView.cpp
//
#include "stdafx.h"
#include "AuthorHTML.h"
#include "ScintillaScriptDoc.h"
#include "ScintillaHTMLView.h"
#include "HTMLEdView.h"
#include "HTMLEdDoc.h"
#include "ChildFrm.h"

#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const TCHAR keyWords[] = 
_T("a abbr acronym address applet area b base basefont ")
_T("bdo big blockquote body br button caption center ")
_T("cite code col colgroup dd del dfn dir div dl dt em ")
_T("fieldset font form frame frameset h1 h2 h3 h4 h5 h6 ")
_T("head hr html i iframe img input ins isindex kbd label ")
_T("legend li link map marquee menu meta noframes noscript ")
_T("object ol optgroup option p param pre q s samp ")
_T("script select small span strike strong style sub sup ")
_T("table tbody td textarea tfoot th thead title tr tt u ul ")
_T("var xml xmlns");

IMPLEMENT_DYNCREATE(ScintillaHTMLView, CScintillaView)

BEGIN_MESSAGE_MAP(ScintillaHTMLView, CScintillaView)
  ON_COMMAND(ID_OPTIONS_ADDMARKER, OnOptionsAddmarker)
  ON_COMMAND(ID_OPTIONS_DELETEMARKER, OnOptionsDeletemarker)
  ON_UPDATE_COMMAND_UI(ID_OPTIONS_DELETEMARKER, OnUpdateOptionsDeletemarker)
  ON_COMMAND(ID_OPTIONS_FIND_NEXTMARKER, OnOptionsFindNextmarker)
  ON_COMMAND(ID_OPTIONS_FIND_PREVMARKER, OnOptionsFindPrevmarker)
  ON_COMMAND(ID_OPTIONS_FOLD_MARGIN, OnOptionsFoldMargin)
  ON_COMMAND(ID_OPTIONS_SELECTION_MARGIN, OnOptionsSelectionMargin)
  ON_UPDATE_COMMAND_UI(ID_OPTIONS_SELECTION_MARGIN, OnUpdateOptionsSelectionMargin)
  ON_COMMAND(ID_OPTIONS_VIEW_LINENUMBERS, OnOptionsViewLinenumbers)
  ON_UPDATE_COMMAND_UI(ID_OPTIONS_VIEW_LINENUMBERS, OnUpdateOptionsViewLinenumbers)
  ON_UPDATE_COMMAND_UI(ID_OPTIONS_ADDMARKER, OnUpdateOptionsAddmarker)
  ON_UPDATE_COMMAND_UI(ID_OPTIONS_FOLD_MARGIN, OnUpdateOptionsFoldMargin)
  ON_UPDATE_COMMAND_UI(ID_INDICATOR_LINE, OnUpdateLine) 
  ON_UPDATE_COMMAND_UI(ID_INDICATOR_STYLE, OnUpdateStyle) 
  ON_UPDATE_COMMAND_UI(ID_INDICATOR_FOLD, OnUpdateFold) 
  ON_UPDATE_COMMAND_UI(ID_INDICATOR_OVR, OnUpdateInsert)
  ON_WM_ACTIVATE()
END_MESSAGE_MAP()

ScintillaHTMLView::ScintillaHTMLView()
{
  LoadMarginSettings();
}

void ScintillaHTMLView::OnDraw(CDC* /*pDC*/)
{
  CHTMLEdDoc* pDoc = GetDocument();
  ASSERT_VALID(pDoc);
}

#ifdef _DEBUG
void ScintillaHTMLView::AssertValid() const
{
  CScintillaView::AssertValid();
}

void ScintillaHTMLView::Dump(CDumpContext& dc) const
{
  CScintillaView::Dump(dc);
}

CHTMLEdDoc* 
ScintillaHTMLView::GetDocument() // non-debug version is inline
{
  ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CHTMLEdDoc)));
  return static_cast<CHTMLEdDoc*>(m_pDocument);
}
#endif //_DEBUG

void ScintillaHTMLView::SetAStyle(int style, COLORREF fore, COLORREF back, int size, const char* face) 
{
  CScintillaCtrl& rCtrl = GetCtrl();

  rCtrl.StyleSetFore(style, fore);
  rCtrl.StyleSetBack(style, back);
  if (size >= 1)
    rCtrl.StyleSetSize(style, size);
  if (face) 
    rCtrl.StyleSetFont(style, face);
}

void ScintillaHTMLView::DefineMarker(int marker, int markerType, COLORREF fore, COLORREF back) 
{
  CScintillaCtrl& rCtrl = GetCtrl();

  rCtrl.MarkerDefine(marker, markerType);
  rCtrl.MarkerSetFore(marker, fore);
  rCtrl.MarkerSetBack(marker, back);
}

void ScintillaHTMLView::OnInitialUpdate() 
{
  //Let the base class do its thing
  CScintillaView::OnInitialUpdate();

  CScintillaCtrl& rCtrl = GetCtrl();

  //Setup the Lexer
  rCtrl.SetLexer(SCLEX_HTML);
  rCtrl.SetKeyWords(0, keyWords);

  //Setup styles
  SetAStyle(STYLE_DEFAULT, RGB(0, 0, 0), RGB(0xff, 0xff, 0xff), 10, "Courier new");
  rCtrl.StyleClearAll();
  rCtrl.StyleSetBold(SCE_H_TAG,1);
  SetAStyle(SCE_H_TAG,              RGB(0,   0,   0x80));   // Blue
  SetAStyle(SCE_H_TAGUNKNOWN,       RGB(0x80,0,   0));      // RED
  SetAStyle(SCE_H_ATTRIBUTE,        RGB(0x80,0,   0xff));   // Purple
  SetAStyle(SCE_H_ATTRIBUTEUNKNOWN, RGB(0x80,0,   0));      // RED
  SetAStyle(SCE_H_NUMBER,           RGB(119, 119, 187));    // Dark gray
  SetAStyle(SCE_H_DOUBLESTRING,     RGB(0xff,0,   0));      // Light red
  SetAStyle(SCE_H_SINGLESTRING,     RGB(0,   0x80,192));    // Blueish
  SetAStyle(SCE_H_OTHER,            RGB(0,   0,   0));      // Black
  SetAStyle(SCE_H_COMMENT,          RGB(0,   0x80,0));      // Dark Green
  SetAStyle(SCE_H_ENTITY,           RGB(0xff,0x80,0x40));   // Orange
  SetAStyle(SCE_H_TAGEND,           RGB(0,   0,   0xff));   // bright blue

  // Use identation guide-lines
  rCtrl.SetIndentationGuides(TRUE,TRUE);
  rCtrl.StyleSetFore(STYLE_INDENTGUIDE,RGB(0,0,0x80),TRUE);
  rCtrl.SetProperty(_T("fold"), _T("1"));

  //Setup folding
  rCtrl.SetMarginWidthN(2, 16);
  rCtrl.SetMarginSensitiveN(2, TRUE);
  rCtrl.SetMarginTypeN(2, SC_MARGIN_SYMBOL);
  rCtrl.SetMarginMaskN(2, SC_MASK_FOLDERS);
  rCtrl.SetProperty(_T("fold"), _T("1"));

  //Setup markers
  DefineMarker(SC_MARKNUM_FOLDEROPEN,    SC_MARK_MINUS, RGB(0xff, 0xff, 0xff), RGB(0, 0, 0xFF));
  DefineMarker(SC_MARKNUM_FOLDER,        SC_MARK_PLUS,  RGB(0xff, 0xff, 0xff), RGB(0, 0, 0));
  DefineMarker(SC_MARKNUM_FOLDERSUB,     SC_MARK_EMPTY, RGB(0xff, 0xff, 0xff), RGB(0, 0, 0));
  DefineMarker(SC_MARKNUM_FOLDERTAIL,    SC_MARK_EMPTY, RGB(0xff, 0xff, 0xff), RGB(0, 0, 0));
  DefineMarker(SC_MARKNUM_FOLDEREND,     SC_MARK_EMPTY, RGB(0xff, 0xff, 0xff), RGB(0, 0, 0));
  DefineMarker(SC_MARKNUM_FOLDEROPENMID, SC_MARK_EMPTY, RGB(0xff, 0xff, 0xff), RGB(0, 0, 0));
  DefineMarker(SC_MARKNUM_FOLDERMIDTAIL, SC_MARK_EMPTY, RGB(0xff, 0xff, 0xff), RGB(0, 0, 0));

  //Setup auto completion
  rCtrl.AutoCSetSeparator(10); //Use a separator of line feed

  //Setup call tips
  rCtrl.SetMouseDwellTime(1000);

#ifdef _DEBUG
  AfxDump(this);
#endif
}

void ScintillaHTMLView::OnOptionsAddmarker() 
{
  CScintillaCtrl& rCtrl = GetCtrl();
  int nPos = rCtrl.GetCurrentPos();
  int nLine = rCtrl.LineFromPosition(nPos);
  rCtrl.MarkerAdd(nLine, 0);
}

void ScintillaHTMLView::OnUpdateOptionsAddmarker(CCmdUI* pCmdUI) 
{
  CScintillaCtrl& rCtrl = GetCtrl();
  int nPos = rCtrl.GetCurrentPos();
  int nLine = rCtrl.LineFromPosition(nPos);
  int nBits = rCtrl.MarkerGet(nLine);
  pCmdUI->Enable((nBits & 0x1) == 0);	
}

void ScintillaHTMLView::OnOptionsDeletemarker() 
{
  CScintillaCtrl& rCtrl = GetCtrl();
  int nPos = rCtrl.GetCurrentPos();
  int nLine = rCtrl.LineFromPosition(nPos);
  rCtrl.MarkerDelete(nLine, 0);
}

void ScintillaHTMLView::OnUpdateOptionsDeletemarker(CCmdUI* pCmdUI) 
{
  CScintillaCtrl& rCtrl = GetCtrl();
  int nPos = rCtrl.GetCurrentPos();
  int nLine = rCtrl.LineFromPosition(nPos);
  int nBits = rCtrl.MarkerGet(nLine);
  pCmdUI->Enable(nBits & 0x1);	
}

void ScintillaHTMLView::OnOptionsFindNextmarker() 
{
  CScintillaCtrl& rCtrl = GetCtrl();
  int nPos = rCtrl.GetCurrentPos();
  int nLine = rCtrl.LineFromPosition(nPos);
  int nFoundLine = rCtrl.MarkerNext(nLine + 1, 0x1);
  if (nFoundLine >= 0)
    rCtrl.GotoLine(nFoundLine);
  else
    MessageBeep(MB_ICONHAND);
}

void ScintillaHTMLView::OnOptionsFindPrevmarker() 
{
  CScintillaCtrl& rCtrl = GetCtrl();
  int nPos = rCtrl.GetCurrentPos();
  int nLine = rCtrl.LineFromPosition(nPos);
  int nFoundLine = rCtrl.MarkerPrevious(nLine - 1, 0x1);
  if (nFoundLine >= 0)
    rCtrl.GotoLine(nFoundLine);
  else
    MessageBeep(MB_ICONHAND);
}

void ScintillaHTMLView::OnOptionsFoldMargin() 
{
  CScintillaCtrl& rCtrl = GetCtrl();
  int nMarginWidth = rCtrl.GetMarginWidthN(2);
  if (nMarginWidth)
    rCtrl.SetMarginWidthN(2, 0);
  else
    rCtrl.SetMarginWidthN(2, 16);
}

void ScintillaHTMLView::OnUpdateOptionsFoldMargin(CCmdUI* pCmdUI) 
{
  pCmdUI->SetCheck(GetCtrl().GetMarginWidthN(2) != 0);
}

void ScintillaHTMLView::OnOptionsSelectionMargin() 
{
  CScintillaCtrl& rCtrl = GetCtrl();
  int nMarginWidth = rCtrl.GetMarginWidthN(1);
  if (nMarginWidth)
    rCtrl.SetMarginWidthN(1, 0);
  else
    rCtrl.SetMarginWidthN(1, 16);
}

void ScintillaHTMLView::OnUpdateOptionsSelectionMargin(CCmdUI* pCmdUI) 
{
  pCmdUI->SetCheck(GetCtrl().GetMarginWidthN(1) != 0);
}

void ScintillaHTMLView::OnOptionsViewLinenumbers() 
{
  CScintillaCtrl& rCtrl = GetCtrl();
  int nMarginWidth = rCtrl.GetMarginWidthN(0);
  if (nMarginWidth)
    rCtrl.SetMarginWidthN(0, 0);
  else
    rCtrl.SetMarginWidthN(0, 32);
}

void ScintillaHTMLView::OnUpdateOptionsViewLinenumbers(CCmdUI* pCmdUI) 
{
  pCmdUI->SetCheck(GetCtrl().GetMarginWidthN(0) != 0);
}

void ScintillaHTMLView::OnUpdateInsert(CCmdUI* pCmdUI)
{
  CString sText;
  if (GetCtrl().GetOvertype())
    sText.LoadString(ID_INDICATOR_OVR);
  pCmdUI->SetText(sText);
}

void ScintillaHTMLView::OnUpdateStyle(CCmdUI* pCmdUI)
{
  CScintillaCtrl& rCtrl = GetCtrl();
  int nPos = rCtrl.GetCurrentPos();
  int nStyle = rCtrl.GetStyleAt(nPos);
  CString sLine;
  sLine.Format(IDS_STYLE_INDICATOR, nStyle);
  pCmdUI->SetText(sLine);
} 

void ScintillaHTMLView::OnUpdateFold(CCmdUI* pCmdUI)
{
  CScintillaCtrl& rCtrl = GetCtrl();
  int nPos = rCtrl.GetCurrentPos();
  int nLine = rCtrl.LineFromPosition(nPos);
  int nLevel = rCtrl.GetFoldLevel(nLine) & SC_FOLDLEVELNUMBERMASK;
  nLevel -= 1024;

  CString sLine;
  sLine.Format(IDS_FOLD_INDICATOR, nLevel);
  pCmdUI->SetText(sLine);
} 

void ScintillaHTMLView::OnUpdateLine(CCmdUI* pCmdUI)
{
  CScintillaCtrl& rCtrl = GetCtrl();
  int nPos = rCtrl.GetCurrentPos();
  int nLine = rCtrl.LineFromPosition(nPos);
  int nColumn = rCtrl.GetColumn(nPos);

  CString sLine;
  sLine.Format(IDS_LINE_INDICATOR, nLine, nColumn, nPos);
  pCmdUI->SetText(sLine);
} 

//A simple example of auto completion
void ScintillaHTMLView::OnCharAdded(SCNotification* /*pSCNotification*/)
{
  /*
  CScintillaCtrl& rCtrl = GetCtrl();

  //Get the previous 12 characters and if it is "scintilla is" case insensitive
  //then display a list which allows "very cool", "easy" or the "way cool!!"
  int nStartSel = rCtrl.GetSelectionStart();
  int nEndSel = rCtrl.GetSelectionEnd();
  if ((nStartSel == nEndSel) && (nStartSel > 12))
  {
    TextRange tr;
    tr.chrg.cpMin = nStartSel - 13;
    tr.chrg.cpMax = nEndSel - 1;
    char sText[13];
    tr.lpstrText = sText;
    rCtrl.GetTextRange(&tr);

    //Display the auto completion list
    if (_strcmpi(sText, "scintilla is") == 0)
    {
      //Display the auto completion list
      rCtrl.AutoCShow(0, _T("very cool\neasy to use\nway cool!!"));
    }
  }
  */
}

//A simple example of call tips
void 
ScintillaHTMLView::OnDwellStart(SCNotification* /*pSCNotification*/)
{
  /*
  CScintillaCtrl& rCtrl = GetCtrl();

  //Only display the call tip if the scintilla window has focus
  CWnd* pFocusWnd = GetFocus();
  if (pFocusWnd && (pFocusWnd->GetSafeHwnd() == rCtrl.GetSafeHwnd()))
  {
    //Get the previous 7 characters and next 7 characters arround
    //the current dwell position and if it is "author " case insensitive
    //then display "PJ Naughter" as a call tip
    TextRange tr;
    tr.chrg.cpMin = max(0, pSCNotification->position - 7);
    tr.chrg.cpMax = min(pSCNotification->position + 7, rCtrl.GetLength());
    char sText[15];
    memset(sText, 0, sizeof(sText));
    tr.lpstrText = sText;
    rCtrl.GetTextRange(&tr);

    //Display the call tip
#if (_MSC_VER >= 1400)
    _strupr_s(sText, sizeof(sText));
#else  
    _strupr(sText);
#endif
    if (strstr(sText, "AUTHOR "))
      rCtrl.CallTipShow(pSCNotification->position, _T("PJ Naughter"));
  }
  */
}

void ScintillaHTMLView::OnDwellEnd(SCNotification* /*pSCNotification*/)
{
  CScintillaCtrl& rCtrl = GetCtrl();

  //Cancel any outstanding call tip
  if (rCtrl.CallTipActive())
    rCtrl.CallTipCancel();
}

void ScintillaHTMLView::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
  //Let the base class do its thing
  CScintillaView::OnActivate(nState, pWndOther, bMinimized);

  CScintillaCtrl& rCtrl = GetCtrl();

  //Cancel any outstanding call tip
  if (nState == WA_INACTIVE && rCtrl.CallTipActive())
    rCtrl.CallTipCancel();
}

void ScintillaHTMLView::OnModifyAttemptRO(SCNotification* /*pSCNotification*/)
{
  if (AfxMessageBox(IDP_ALLOW_MODIFY_READONLY_FILE, MB_YESNO) == IDYES)
    GetCtrl().SetReadOnly(FALSE);
}

// WORK TOGETHER WITH HTMLEdView and HTMLEdDoc
void 
ScintillaHTMLView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
  if(bActivate && pActivateView==this && pDeactiveView != this)
  {
    //we're being activated get the HTML from the WebView
    UpdateView();
  }
  CScintillaView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

void 
ScintillaHTMLView::UpdateView()
{
  CString strHTML;
  //CMDIChildFrame* parent = (CMDIChildFrame*)GetParent();

  //CHTMLEdView* pWebView = (CHTMLEdView*)parent->GetWebView();
  //ASSERT_VALID(pWebView);
  //pWebView->GetDocumentHTML(strHTML);

  CString doc = GetDocument()->GetPathName();
  GetDocument()->OnSaveDocument(doc);
  GetDocument()->GetFile(strHTML);

  CScintillaCtrl& scontrol = GetCtrl();
  scontrol.SetText(strHTML,TRUE);
}

void
ScintillaHTMLView::GetWindowText(CString& text)
{
  CScintillaCtrl& scontrol = GetCtrl();
  int len = scontrol.GetTextLength(TRUE);
  len += 2; // to be sure
  scontrol.GetText(len,text.GetBuffer(len),TRUE);
  text.ReleaseBuffer();
}

void
ScintillaHTMLView::NewDocument()
{
  ASSERT_VALID(this);
  ASSERT(m_hWnd != NULL);
  SetWindowText(NULL);
  ASSERT_VALID(this);
}
