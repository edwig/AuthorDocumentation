//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    ScriptDlg.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for managing the <SCRIPT> tag in HTML
//
#include "stdafx.h"
#include "AuthorHTML.h"
#include "ScriptDlg.h"
#include "FileDialog.h"
#include "ScintillaScriptView.h"
#include <vector>

// ScriptDlg dialog

IMPLEMENT_DYNAMIC(ScriptDlg, CDialog)

ScriptDlg::ScriptDlg(CWnd*       pParent
                    ,HtmlScript* elem
                    ,CString     base)
          :CDialog(ScriptDlg::IDD, pParent)
          ,m_elem(elem)
          ,m_base(base)
{
  // Scintilla must be created early, to create the WindowsClassname
  // for the dialog create from DoModal
  CRect r;
  m_scintilla.Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP, r, this, 0);
}

ScriptDlg::~ScriptDlg()
{
}

void ScriptDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
  DDX_Control(pDX,IDC_SCRIPTTYPE, m_comboType);
  DDX_Control(pDX,IDC_LANG,       m_comboLang);
  DDX_Text   (pDX,IDC_SRC,        m_src);
  DDX_Text   (pDX,IDC_FORHTML,    m_object);
  DDX_Control(pDX,IDC_EVENT,      m_comboEvent);
  DDX_Control(pDX,IDC_DEFER,      m_buttonDefer);
  DDX_Control(pDX,IDC_CHOOSE,     m_buttonChoose);
  //DDX_Text   (pDX,IDC_SCRIPT,     m_script);
  DDX_Control(pDX,IDC_SCRIPT,     m_scintilla);
  DDX_Control(pDX,IDOK,           m_buttonOK);
  DDX_Control(pDX,IDCANCEL,       m_buttonCancel);
  DDX_Control(pDX,IDC_SCRIPT_HELP,m_buttonHelp);
  DDX_Text   (pDX,IDC_DESCRIPT,   m_description);

  if(pDX->m_bSaveAndValidate == Data2Controls)
  {
    int ind;
    CString typ;
    typ = Misc::GetAttributeDisplayname("script-type",m_type);
    ind = m_comboType.FindString(-1,typ);
          m_comboType.SetCurSel(ind);
    typ = Misc::GetAttributeDisplayname("language",m_language);
    ind = m_comboLang.FindString(-1,typ);
          m_comboLang.SetCurSel(ind);
    ind = m_comboEvent.FindString(-1,m_event);
          m_comboEvent.SetCurSel(ind);

    // src sets script window
    CWnd* s = GetDlgItem(IDC_SCRIPT);
    s->EnableWindow(m_src.IsEmpty());
    s = GetDlgItem(IDC_SRC);
    s->EnableWindow(m_script.IsEmpty());
    m_buttonChoose.EnableWindow(m_script.IsEmpty());
  }
}

BEGIN_MESSAGE_MAP(ScriptDlg, CDialog)
  ON_WM_PAINT()
  ON_CBN_SELCHANGE(IDC_SCRIPTTYPE,  OnCbnSelchangeScripttype)
  ON_CBN_SELCHANGE(IDC_LANG,        OnCbnSelchangeLang)
  ON_EN_KILLFOCUS (IDC_SRC,         OnEnChangeSrc)
  ON_EN_KILLFOCUS (IDC_FORHTML,     OnEnChangeForhtml)
  ON_CBN_SELCHANGE(IDC_EVENT,       OnCbnSelchangeEvent)
  ON_BN_CLICKED   (IDC_DEFER,       OnBnClickedDefer)
  ON_BN_CLICKED   (IDC_CHOOSE,      OnBnClickedChoose)
  ON_EN_KILLFOCUS (IDC_SCRIPT,      OnEnChangeScript)
  ON_BN_CLICKED   (IDOK,            OnBnClickedOk)
  ON_BN_CLICKED   (IDCANCEL,        OnBnClickedCancel)
  ON_BN_CLICKED   (IDC_SCRIPT_HELP, OnBnClickedScriptHelp)
END_MESSAGE_MAP()

BOOL
ScriptDlg::OnInitDialog()
{
  CDialog::OnInitDialog();
  FillPage();
  std::vector<string> all;
  Misc::GetAllAttributeDisplaynames("script-type",&all);
  for(unsigned int ind = 0; ind < all.size(); ++ ind)
  {
    m_comboType.AddString(all[ind].c_str());
  }
  all.clear();
  Misc::GetAllAttributeDisplaynames("language",&all);
  for(unsigned int ind = 0; ind < all.size(); ++ind)
  {
    m_comboLang.AddString(all[ind].c_str());
  }
  std::vector<CString> events;
  m_comboEvent.AddString(""); // Empty string to empty event.
  Misc::GetAllEvents(events,0,0);
  for(unsigned int ind = 0; ind < events.size(); ++ind)
  {
    m_comboEvent.AddString(events[ind]);
  }

  // Create controls
  UpdateData(FALSE);

  // Further setup of the Scintilla control
  m_scintilla.SetupDirectAccess();
  // Set font
  SetAStyle(STYLE_DEFAULT, RGB(0, 0, 0), RGB(0xff, 0xff, 0xff), 10, "Courier new");
  // Set language styles
  SetEditorToScript();

  //Setup folding
  m_scintilla.SetMarginWidthN(2, 16);
  m_scintilla.SetMarginSensitiveN(2, TRUE);
  m_scintilla.SetMarginTypeN(2, SC_MARGIN_SYMBOL);
  m_scintilla.SetMarginMaskN(2, SC_MASK_FOLDERS);
  m_scintilla.SetIndentationGuides(TRUE,TRUE);
  m_scintilla.StyleSetFore(STYLE_INDENTGUIDE,RGB(0,0,0x80),TRUE);
  m_scintilla.SetProperty(_T("fold"), _T("1"));

  //Setup markers
  DefineMarker(SC_MARKNUM_FOLDEROPEN,     SC_MARK_MINUS, RGB(0xff, 0xff, 0xff), RGB(0, 0, 0xFF));
  DefineMarker(SC_MARKNUM_FOLDER,         SC_MARK_PLUS,  RGB(0xff, 0xff, 0xff), RGB(0, 0, 0));
  DefineMarker(SC_MARKNUM_FOLDERSUB,      SC_MARK_EMPTY, RGB(0xff, 0xff, 0xff), RGB(0, 0, 0));
  DefineMarker(SC_MARKNUM_FOLDERTAIL,     SC_MARK_EMPTY, RGB(0xff, 0xff, 0xff), RGB(0, 0, 0));
  DefineMarker(SC_MARKNUM_FOLDEREND,      SC_MARK_EMPTY, RGB(0xff, 0xff, 0xff), RGB(0, 0, 0));
  DefineMarker(SC_MARKNUM_FOLDEROPENMID,  SC_MARK_EMPTY, RGB(0xff, 0xff, 0xff), RGB(0, 0, 0));
  DefineMarker(SC_MARKNUM_FOLDERMIDTAIL,  SC_MARK_EMPTY, RGB(0xff, 0xff, 0xff), RGB(0, 0, 0));

  //Setup auto completion
  m_scintilla.AutoCSetSeparator(10); //Use a separator of line feed

  //Setup call tips
  m_scintilla.SetMouseDwellTime(1000);

  // Set script in control
  m_scintilla.SetText(m_script);

  UpdateData(Data2Controls);
  return TRUE;
}

void
ScriptDlg::SetEditorToScript()
{
  // Get text out of control
  CString script;
  int len = m_scintilla.GetTextLength() + 1;
  m_scintilla.GetText(len,script.GetBufferSetLength(len));
  script.ReleaseBuffer();
  m_scintilla.SetText("");

  CString stype = Misc::GetAttributeDisplayname("script-type",m_type);
  int type = m_comboType.FindStringExact(0,stype);
  if(type == 0 || type == 1) // Javascript
  {
    // Ok, its a Javascript like file
    m_scintilla.SetLexer(SCLEX_CPP);
    m_scintilla.SetKeyWords(0, jsKeyWords);

    //Setup styles
    m_scintilla.StyleClearAll();

    SetAStyle(SCE_C_DEFAULT,          RGB(0, 0,    0));
    SetAStyle(SCE_C_COMMENT,          RGB(0, 0x80, 0));
    SetAStyle(SCE_C_COMMENTLINE,      RGB(0, 0x80, 0));
    SetAStyle(SCE_C_COMMENTDOC,       RGB(0, 0x80, 0));
    SetAStyle(SCE_C_NUMBER,           RGB(0, 0x80, 0x80));
    SetAStyle(SCE_C_WORD,             RGB(0, 0,    0x80));
    m_scintilla.StyleSetBold(SCE_C_WORD, 1);
    SetAStyle(SCE_C_STRING,           RGB(0x80, 0, 0));
    SetAStyle(SCE_C_CHARACTER,        RGB(0xFF, 0, 0));
    SetAStyle(SCE_C_PREPROCESSOR,     RGB(0x80, 0, 0));
    SetAStyle(SCE_C_OPERATOR,         RGB(0x80, 0x80, 0));
    SetAStyle(SCE_C_IDENTIFIER,       RGB(0, 0,    0));
    SetAStyle(SCE_C_STRINGEOL,        RGB(0, 0,    0));
    SetAStyle(SCE_C_REGEX,            RGB(0, 0x80, 0));
  }
  else if(type == 2 || type == 3 || type == 4) // VBScript
  {
    //Setup the Lexer
    m_scintilla.SetLexer(SCLEX_VBSCRIPT);
    m_scintilla.SetKeyWords(0, vbKeyWords);

    //Setup styles
    m_scintilla.StyleClearAll();
    SetAStyle(SCE_V_DEFAULT,          RGB(0, 0,    0));
    SetAStyle(SCE_V_COMMENT,          RGB(0, 0x80, 0));
    SetAStyle(SCE_V_COMMENTLINE,      RGB(0, 0x80, 0));
    SetAStyle(SCE_V_COMMENTLINEBANG,  RGB(0, 0x80, 0));
    SetAStyle(SCE_V_NUMBER,           RGB(0, 0x80, 0x80));
    SetAStyle(SCE_V_WORD,             RGB(0, 0,    0x80));
    m_scintilla.StyleSetBold(SCE_V_WORD, 1);
    SetAStyle(SCE_V_STRING,           RGB(0x80, 0, 0x80));

    SetAStyle(SCE_V_WORD2,            RGB(0, 0,    0x80));
    SetAStyle(SCE_V_WORD3,            RGB(0, 0,    0x80));
    SetAStyle(SCE_V_PREPROCESSOR,     RGB(0x80, 0, 0));
    SetAStyle(SCE_V_OPERATOR,         RGB(0x80, 0x80, 0));
    SetAStyle(SCE_V_IDENTIFIER,       RGB(0, 0,    0));
    SetAStyle(SCE_V_STRINGEOL,        RGB(0, 0,    0));
    SetAStyle(SCE_V_USER,             RGB(0x66, 0x66,0x66));
  }
  else if(!m_type.IsEmpty())
  {
    // Error
    theApp.MessageBox("Cannot recognize this type of script!","Script error",MB_OK|MB_ICONHAND);
  }
  // Reset the same text
  m_scintilla.SetText(script);
}

void 
ScriptDlg::DefineMarker(int marker, int markerType, COLORREF fore, COLORREF back) 
{
  m_scintilla.MarkerDefine(marker, markerType);
  m_scintilla.MarkerSetFore(marker, fore);
  m_scintilla.MarkerSetBack(marker, back);
}

void 
ScriptDlg::SetAStyle(int style, COLORREF fore, COLORREF back, int size, const char* face) 
{
  m_scintilla.StyleSetFore(style, fore);
  m_scintilla.StyleSetBack(style, back);
  if (size >= 1)
  {
    m_scintilla.StyleSetSize(style, size);
  }
  if (face) 
  {
    m_scintilla.StyleSetFont(style, face);
  }
}

void
ScriptDlg::FillPage()
{
  m_type      = m_elem->GetAttribute("type");
  m_language  = m_elem->GetAttribute("language");
  m_src       = m_elem->GetAttribute("src",EXACT_VALUE);
  m_defer     = atoi(m_elem->GetAttribute("defer")) == 1;
  m_object    = m_elem->GetAttribute("for");
  m_event     = m_elem->GetAttribute("event");
  m_script    = m_elem->GetText();
  m_script.Trim();
  m_script.TrimLeft("<!--");
  m_script.TrimRight("-->");
  m_script.TrimRight("//");
}

void
ScriptDlg::UpdateProperties()
{
  CString defer;
  defer.Format("%d",m_defer);
  m_elem->SetAttribute("type",    m_type);
  m_elem->SetAttribute("language",m_language);
  m_elem->SetAttribute("src",     m_src);
  m_elem->SetAttribute("defer",   defer,EMPTYREMOVE);
  m_elem->SetAttribute("for",     m_object);
  m_elem->SetAttribute("event",   m_event);

  int len = m_scintilla.GetTextLength() + 1;
  m_scintilla.GetText(len,m_script.GetBufferSetLength(len));
  m_script.ReleaseBuffer();
  if(m_script.Find("<!--") < 0)
  {
    m_script = CString("<!--") + m_script;
  }
  if(m_script.Find("-->") < 0)
  {
    m_script += CString("//-->");
  }
  m_elem->SetText(m_script);
}

// ScriptDlg message handlers

void 
ScriptDlg::OnCbnSelchangeScripttype()
{
  int ind = m_comboType.GetCurSel();
  if(ind >= 0)
  {
    CString type;
    m_comboType.GetLBText(ind,type);
    m_type = Misc::GetAttributeValue("script-type",type);

    SetEditorToScript();
    m_language = "";
    UpdateData(Data2Controls);
  }
}

void 
ScriptDlg::OnCbnSelchangeLang()
{
  int ind = m_comboLang.GetCurSel();
  if(ind >= 0)
  {
    CString lang;
    m_comboLang.GetLBText(ind,lang);
    m_language = Misc::GetAttributeValue("language",lang);
  }
}

void
ScriptDlg::OnEnChangeSrc()
{
  CWnd* w = GetDlgItem(IDC_SRC);
  w->GetWindowText(m_src);
  UpdateData(Data2Controls);
}

void 
ScriptDlg::OnBnClickedChoose()
{
  DocFileDialog diag(true               // true = open
                    ,"Select a script"  // title
                    ,""                 // Extension
                    ,""                 // Default file
                    ,0                  // flags
                    ,"Java scripts (js)|*.js|"
                     "Visual basic scripts (vbs)|*.vbs|"
                     "Ecmascript|*.cs|"
                     "All files|*.*");
  if(diag.DoModal() == IDOK)
  {
    CString file = diag.GetChosenFile();
    CString relative;
    if(Misc::MakeRelativePathname(m_base,file,relative))
    {
      file = relative;
    }
    m_src = file;
    UpdateData(Data2Controls);
  }
}

void 
ScriptDlg::OnEnChangeForhtml()
{
  CWnd* w = GetDlgItem(IDC_FORHTML);
  w->GetWindowText(m_object);
}

void 
ScriptDlg::OnCbnSelchangeEvent()
{
  int ind = m_comboEvent.GetCurSel();
  if(ind >= 0)
  {
    m_comboEvent.GetLBText(ind,m_event);
    m_description = Misc::GetEventDescription(m_event,0);
    UpdateData(Data2Controls);
  }
}

void 
ScriptDlg::OnBnClickedDefer()
{
  m_defer = (m_buttonDefer.GetCheck() == TRUE);
}

void 
ScriptDlg::OnEnChangeScript()
{
  CWnd* w = GetDlgItem(IDC_SCRIPT);
  w->GetWindowText(m_script);
  UpdateData(Data2Controls);
}

void 
ScriptDlg::OnBnClickedOk()
{
  UpdateProperties();
  OnOK();
}

void 
ScriptDlg::OnBnClickedCancel()
{
  OnCancel();
}

void 
ScriptDlg::OnBnClickedScriptHelp()
{
  // TODO: Add your control notification handler code here
}

void
ScriptDlg::OnPaint()
{
  CDialog::OnPaint();

  // Calc the outer edge of the scintilla control
  CRect rect;
  m_scintilla.GetWindowRect(&rect);
  ScreenToClient(&rect);
  rect.left--;
  rect.top--;

  // Get a DC and a proper pen
  CDC* cdc = GetDC();
  CPen color(0,1,RGB(127,157,185));
  CPen* old = (CPen*) cdc->SelectObject(color);

  //Draw the edge around the Scintilla edit control
  cdc->MoveTo(rect.left, rect.top);
  cdc->LineTo(rect.right,rect.top);
  cdc->LineTo(rect.right,rect.bottom);
  cdc->LineTo(rect.left, rect.bottom);
  cdc->LineTo(rect.left, rect.top);

  // Restore previous pen and DC
  cdc->SelectObject(old);
  ReleaseDC(cdc);
}

