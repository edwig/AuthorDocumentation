#include "stdafx.h"
#include "AuthorHTML.h"
#include "ScintillaScriptDoc.h"
#include "ScintillaScriptView.h"
#include "resource.h"
#include "Misc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const TCHAR jsKeyWords[] = 
_T("if else function switch case default break for do while in catch try finally ")
_T("new Boolean Number String toString loop this prototype isPrototypeOf constructor ")
_T("apply typeof undefined void ininstanceof Math abs max min random ceil round floor ")
_T("LN10 LN2 LOG10E LOG2E PI SQRT1_2 SQRT2 acos asin atan atan2 cos exp log pow sin ")
_T("sqrt tan parseInt toString toFixed toPrecision toExponential toLocaleString ")
_T("length charAt charCodeAt substring fromCharCode slice substr toUpperCase toLowerCase ")
_T("toLocaleUpperCase toLocaleLowerCase concat indexOf lastIndexOf test search replace ")
_T("split join match escape unescape encodeURI encodeURIcomponent Array localeCompare ")
_T("reverse pop push shift unshift slice splice Date Time toGMTString toDateString ")
_T("toTimeString toLocaleDateString toLocaleTimeString getTimezoneOffset ")
_T("setFullYear getFullYear setMonth getMonth setDate getDate setHours getHours ")
_T("setMinutes getMinutes setSeconds getSeconds setMiliseconds getMiliseconds ")
_T("getDay setYear getYear setUTCFullYear getUTCFullYear setUTCMonth getUTCMonth ")
_T("getUTCDate setUTCDate setUTCHours getUTCHours setUTCMinutes getUTCMinutes ")
_T("getUTCSeconds setUTCSeconds setUTCMiliseconds getUTCMiliseconds getUTCDay ")
_T("var parse");

const TCHAR vbKeyWords[] = 
_T("addhandler addressof alias and andalso appactivate as assembly ")
_T("attribute auto base beep begin binary boolean byref byte byval ")
_T("call case catch cbool cbyte cchar cdate cdbl cdec char chdir chdrive ")
_T("cint class clng close cobj compare const cshort csng cstr ctype ")
_T("currency date decimal declare default defbool defbyte defint deflng ")
_T("defcur defsng defdbl defdec defdate defstr defobj defvar delegate ")
_T("dim do double each else elseif empty end enum eqv erase error  ")
_T("event exit explicit false for filecopy finally friend function get ")
_T("gettype gosub goto handles if imp implements imports in inherits ")
_T("input integer interface is kill len let lib like line load lock ")
_T("long loop lset me mid midb mkdir mod mustinherit mustoverride mybase ")
_T("myclass name namespace new next not nothing notinheritable notoverridable ")
_T("null object on open option optional or orelse overloads overridable overrides ")
_T("paramarray preserve print private property public put protected ")
_T("raiseevent randomize readonly redim rem removehandler resume reset ")
_T("return rmdir rset savepicture savesetting seek select sendkeys set ")
_T("setattr shadows shared short single static step stop string structure ")
_T("sub synclock then throw time to try true type typeof unicode unload ")
_T("unlock until variant wend when while width with withevents write writeonly xor ");


IMPLEMENT_DYNCREATE(CScintillaScriptView, CScintillaView)

BEGIN_MESSAGE_MAP(CScintillaScriptView, CScintillaView)
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

CScintillaScriptView::CScintillaScriptView()
{
  LoadMarginSettings();
}

void CScintillaScriptView::OnDraw(CDC* /*pDC*/)
{
	CScintillaScriptDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
}

#ifdef _DEBUG
void CScintillaScriptView::AssertValid() const
{
	CScintillaView::AssertValid();
}

void CScintillaScriptView::Dump(CDumpContext& dc) const
{
	CScintillaView::Dump(dc);
}

CScintillaScriptDoc* CScintillaScriptView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CScintillaScriptDoc)));
	return static_cast<CScintillaScriptDoc*>(m_pDocument);
}
#endif //_DEBUG

void CScintillaScriptView::SetAStyle(int style, COLORREF fore, COLORREF back, int size, const char* face) 
{
  CScintillaCtrl& rCtrl = GetCtrl();

	rCtrl.StyleSetFore(style, fore);
	rCtrl.StyleSetBack(style, back);
	if (size >= 1)
		rCtrl.StyleSetSize(style, size);
	if (face) 
		rCtrl.StyleSetFont(style, face);
}

void CScintillaScriptView::DefineMarker(int marker, int markerType, COLORREF fore, COLORREF back) 
{
  CScintillaCtrl& rCtrl = GetCtrl();

  rCtrl.MarkerDefine(marker, markerType);
  rCtrl.MarkerSetFore(marker, fore);
  rCtrl.MarkerSetBack(marker, back);
}

void CScintillaScriptView::OnInitialUpdate() 
{
  //Let the base class do its thing
	CScintillaView::OnInitialUpdate();
	
  CScintillaCtrl& rCtrl = GetCtrl();

  CString fileName = GetDocument()->GetPathName();
  CString ext = Misc::ExtensionPart(fileName);

  if(ext.CompareNoCase(".js") == 0)
  {
    // Ok, its a Javascript like file
    rCtrl.SetLexer(SCLEX_CPP);
    rCtrl.SetKeyWords(0, jsKeyWords);

    //Setup styles
    rCtrl.StyleClearAll();

    SetAStyle(SCE_C_DEFAULT,          RGB(0, 0,    0));
    SetAStyle(SCE_C_COMMENT,          RGB(0, 0x80, 0));
    SetAStyle(SCE_C_COMMENTLINE,      RGB(0, 0x80, 0));
    SetAStyle(SCE_C_COMMENTDOC,       RGB(0, 0x80, 0));
    SetAStyle(SCE_C_NUMBER,           RGB(0, 0x80, 0x80));
    SetAStyle(SCE_C_WORD,             RGB(0, 0,    0x80));
    rCtrl.StyleSetBold(SCE_C_WORD, 1);
    SetAStyle(SCE_C_STRING,           RGB(0x80, 0, 0));
    SetAStyle(SCE_C_CHARACTER,        RGB(0xFF, 0, 0));
    SetAStyle(SCE_C_PREPROCESSOR,     RGB(0x80, 0, 0));
    SetAStyle(SCE_C_OPERATOR,         RGB(0x80, 0x80, 0));
    SetAStyle(SCE_C_IDENTIFIER,       RGB(0, 0,    0));
    SetAStyle(SCE_C_STRINGEOL,        RGB(0, 0,    0));
    SetAStyle(SCE_C_REGEX,            RGB(0, 0x80, 0));
//#define SCE_C_UUID 8
//#define SCE_C_VERBATIM 13
//#define SCE_C_COMMENTLINEDOC 15
//#define SCE_C_WORD2 16
//#define SCE_C_COMMENTDOCKEYWORD 17
//#define SCE_C_COMMENTDOCKEYWORDERROR 18
//#define SCE_C_GLOBALCLASS 19
  }
  else if(ext.CompareNoCase(".vbs") == 0)
  {
    //Setup the Lexer
    rCtrl.SetLexer(SCLEX_VBSCRIPT);
    rCtrl.SetKeyWords(0, vbKeyWords);

    //Setup styles
    rCtrl.StyleClearAll();
    SetAStyle(SCE_V_DEFAULT,          RGB(0, 0,    0));
    SetAStyle(SCE_V_COMMENT,          RGB(0, 0x80, 0));
    SetAStyle(SCE_V_COMMENTLINE,      RGB(0, 0x80, 0));
    SetAStyle(SCE_V_COMMENTLINEBANG,  RGB(0, 0x80, 0));
    SetAStyle(SCE_V_NUMBER,           RGB(0, 0x80, 0x80));
    SetAStyle(SCE_V_WORD,             RGB(0, 0,    0x80));
    rCtrl.StyleSetBold(SCE_V_WORD, 1);
    SetAStyle(SCE_V_STRING,           RGB(0x80, 0, 0x80));

    SetAStyle(SCE_V_WORD2,            RGB(0, 0,    0x80));
    SetAStyle(SCE_V_WORD3,            RGB(0, 0,    0x80));
    SetAStyle(SCE_V_PREPROCESSOR,     RGB(0x80, 0, 0));
    SetAStyle(SCE_V_OPERATOR,         RGB(0x80, 0x80, 0));
    SetAStyle(SCE_V_IDENTIFIER,       RGB(0, 0,    0));
    SetAStyle(SCE_V_STRINGEOL,        RGB(0, 0,    0));
    SetAStyle(SCE_V_USER,             RGB(0x66, 0x66,0x66));
  }
  else
  {
    // Error
    theApp.MessageBox("Cannot recognize this type of script!","Script error",MB_OK|MB_ICONHAND);
  }
  // Set font
  SetAStyle(STYLE_DEFAULT, RGB(0, 0, 0), RGB(0xff, 0xff, 0xff), 10, "Courier new");
  //Setup folding
  rCtrl.SetMarginWidthN(2, 16);
	rCtrl.SetMarginSensitiveN(2, TRUE);
	rCtrl.SetMarginTypeN(2, SC_MARGIN_SYMBOL);
	rCtrl.SetMarginMaskN(2, SC_MASK_FOLDERS);
  rCtrl.SetIndentationGuides(TRUE,TRUE);
  rCtrl.StyleSetFore(STYLE_INDENTGUIDE,RGB(0,0,0x80),TRUE);
  rCtrl.SetProperty(_T("fold"), _T("1"));

  //Setup markers
	DefineMarker(SC_MARKNUM_FOLDEROPEN,     SC_MARK_MINUS, RGB(0xff, 0xff, 0xff), RGB(0, 0, 0xFF));
	DefineMarker(SC_MARKNUM_FOLDER,         SC_MARK_PLUS,  RGB(0xff, 0xff, 0xff), RGB(0, 0, 0));
	DefineMarker(SC_MARKNUM_FOLDERSUB,      SC_MARK_EMPTY, RGB(0xff, 0xff, 0xff), RGB(0, 0, 0));
	DefineMarker(SC_MARKNUM_FOLDERTAIL,     SC_MARK_EMPTY, RGB(0xff, 0xff, 0xff), RGB(0, 0, 0));
	DefineMarker(SC_MARKNUM_FOLDEREND,      SC_MARK_EMPTY, RGB(0xff, 0xff, 0xff), RGB(0, 0, 0));
	DefineMarker(SC_MARKNUM_FOLDEROPENMID,  SC_MARK_EMPTY, RGB(0xff, 0xff, 0xff), RGB(0, 0, 0));
	DefineMarker(SC_MARKNUM_FOLDERMIDTAIL,  SC_MARK_EMPTY, RGB(0xff, 0xff, 0xff), RGB(0, 0, 0));

  //Setup auto completion
  rCtrl.AutoCSetSeparator(10); //Use a separator of line feed

  //Setup call tips
  rCtrl.SetMouseDwellTime(1000);

#ifdef _DEBUG
  AfxDump(this);
#endif
}

void CScintillaScriptView::OnOptionsAddmarker() 
{
  CScintillaCtrl& rCtrl = GetCtrl();
  int nPos = rCtrl.GetCurrentPos();
  int nLine = rCtrl.LineFromPosition(nPos);
	rCtrl.MarkerAdd(nLine, 0);
}

void CScintillaScriptView::OnUpdateOptionsAddmarker(CCmdUI* pCmdUI) 
{
  CScintillaCtrl& rCtrl = GetCtrl();
  int nPos = rCtrl.GetCurrentPos();
  int nLine = rCtrl.LineFromPosition(nPos);
  int nBits = rCtrl.MarkerGet(nLine);
  pCmdUI->Enable((nBits & 0x1) == 0);	
}

void CScintillaScriptView::OnOptionsDeletemarker() 
{
  CScintillaCtrl& rCtrl = GetCtrl();
  int nPos = rCtrl.GetCurrentPos();
  int nLine = rCtrl.LineFromPosition(nPos);
  rCtrl.MarkerDelete(nLine, 0);
}

void CScintillaScriptView::OnUpdateOptionsDeletemarker(CCmdUI* pCmdUI) 
{
  CScintillaCtrl& rCtrl = GetCtrl();
  int nPos = rCtrl.GetCurrentPos();
  int nLine = rCtrl.LineFromPosition(nPos);
  int nBits = rCtrl.MarkerGet(nLine);
  pCmdUI->Enable(nBits & 0x1);	
}

void CScintillaScriptView::OnOptionsFindNextmarker() 
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

void CScintillaScriptView::OnOptionsFindPrevmarker() 
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

void CScintillaScriptView::OnOptionsFoldMargin() 
{
  CScintillaCtrl& rCtrl = GetCtrl();
	int nMarginWidth = rCtrl.GetMarginWidthN(2);
  if (nMarginWidth)
    rCtrl.SetMarginWidthN(2, 0);
  else
	  rCtrl.SetMarginWidthN(2, 16);
}

void CScintillaScriptView::OnUpdateOptionsFoldMargin(CCmdUI* pCmdUI) 
{
  pCmdUI->SetCheck(GetCtrl().GetMarginWidthN(2) != 0);
}

void CScintillaScriptView::OnOptionsSelectionMargin() 
{
  CScintillaCtrl& rCtrl = GetCtrl();
	int nMarginWidth = rCtrl.GetMarginWidthN(1);
  if (nMarginWidth)
    rCtrl.SetMarginWidthN(1, 0);
  else
	  rCtrl.SetMarginWidthN(1, 16);
}

void CScintillaScriptView::OnUpdateOptionsSelectionMargin(CCmdUI* pCmdUI) 
{
  pCmdUI->SetCheck(GetCtrl().GetMarginWidthN(1) != 0);
}

void CScintillaScriptView::OnOptionsViewLinenumbers() 
{
  CScintillaCtrl& rCtrl = GetCtrl();
	int nMarginWidth = rCtrl.GetMarginWidthN(0);
  if (nMarginWidth)
    rCtrl.SetMarginWidthN(0, 0);
  else
	  rCtrl.SetMarginWidthN(0, 32);
}

void CScintillaScriptView::OnUpdateOptionsViewLinenumbers(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(GetCtrl().GetMarginWidthN(0) != 0);
}

void CScintillaScriptView::OnUpdateInsert(CCmdUI* pCmdUI)
{
  CString sText;
  if (GetCtrl().GetOvertype())
    sText.LoadString(ID_INDICATOR_OVR);
	pCmdUI->SetText(sText);
}

void CScintillaScriptView::OnUpdateStyle(CCmdUI* pCmdUI)
{
   CScintillaCtrl& rCtrl = GetCtrl();
   int nPos = rCtrl.GetCurrentPos();
   int nStyle = rCtrl.GetStyleAt(nPos);
   CString sLine;
   sLine.Format(IDS_STYLE_INDICATOR, nStyle);
   pCmdUI->SetText(sLine);
} 

void CScintillaScriptView::OnUpdateFold(CCmdUI* pCmdUI)
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

void CScintillaScriptView::OnUpdateLine(CCmdUI* pCmdUI)
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
void CScintillaScriptView::OnCharAdded(SCNotification* /*pSCNotification*/)
{
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
}

//A simple example of call tips
void CScintillaScriptView::OnDwellStart(SCNotification* pSCNotification)
{
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
}

void CScintillaScriptView::OnDwellEnd(SCNotification* /*pSCNotification*/)
{
  CScintillaCtrl& rCtrl = GetCtrl();

  //Cancel any outstanding call tip
  if (rCtrl.CallTipActive())
    rCtrl.CallTipCancel();
}

void CScintillaScriptView::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
  //Let the base class do its thing
	CScintillaView::OnActivate(nState, pWndOther, bMinimized);

  CScintillaCtrl& rCtrl = GetCtrl();

  //Cancel any outstanding call tip
	if (nState == WA_INACTIVE && rCtrl.CallTipActive())
    rCtrl.CallTipCancel();
}

void CScintillaScriptView::OnModifyAttemptRO(SCNotification* /*pSCNotification*/)
{
  if (AfxMessageBox(IDP_ALLOW_MODIFY_READONLY_FILE, MB_YESNO) == IDYES)
    GetCtrl().SetReadOnly(FALSE);
}
