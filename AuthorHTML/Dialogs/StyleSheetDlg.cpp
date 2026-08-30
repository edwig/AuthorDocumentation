//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    StyleSheetDlg.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for managing CSS style sheets
//              Holds all 7 pages of CSS properties
//
#include "stdafx.h"
#include "AuthorHTML.h"
#include "StyleSheetDlg.h"
#include "StyleSheetImportsDlg.h"
#include "NewSelectorDlg.h"
#include "css.h"
#include "Misc.h"
#include "CSSPage1Dlg.h"
#include "CSSPage2Dlg.h"
#include "CSSPage3Dlg.h"
#include "CSSPage4Dlg.h"
#include "CssPage5Dlg.h"
#include "CssPage6Dlg.h"
#include "CssPage7Dlg.h"

// Amount to move controls to the left if we startup as 
// an instyle dialog for just one stylerule
#define TOLEFT 170

CString
CSSPropertyGet(CssStyleSheet* css
              ,string& selector
              ,string  property
              ,bool unspec /*=true*/)
{
  string media = "standard";
  CString value;

  value = css->get(media,selector,property).c_str();
  if(unspec && value.IsEmpty())
  {
    value = "unspecified";
  }
  if(isalpha(value.GetAt(0)))
  {
    value.MakeLower();
    value.SetAt(0,(char)toupper(value[0]));
  }
  return value;
}

void
CSSPropertyPut(CssStyleSheet* css
              ,string& selector
              ,string  property
              ,CString value
              ,bool    unspec)
{
  string media = "standard";
  string val   = value;

  if(unspec && value.CompareNoCase("Unspecified") == 0)
  {
    val = "";
  }
  css->put(media,selector,property,val);
}

void
CssSplitValueUnits(CString property,CString& value,CString& units)
{
  value = "";
  units = "";
  if(property.IsEmpty())
  {
    return;
  }
  static char* digitsep = "+-.,";
  if(isdigit(property[0]) || strchr(digitsep,property[0]))
  {
    while(property.GetLength()>0 && 
         (isdigit(property[0]) || strchr(digitsep,property[0])))
    {
      value += property[0];
      property = property.Mid(1);
    }
    units = property;
  }
  else
  {
    // Parse through (smaller,x-etc)
    value = property;
  }
}

CComBSTR
CssCtoB(CString value)
{
  if(value.CompareNoCase("Unspecified") == 0)
  {
    value= "";
  }
  CComBSTR bString = CT2CW(value);
  return bString;
}

// StyleSheetDlg dialog

IMPLEMENT_DYNAMIC(StyleSheetDlg, CDialog)

StyleSheetDlg::StyleSheetDlg(CWnd*          p_Parent 
                            ,CString        p_base
                            ,CString        p_typeText
                            ,CssStyleSheet* p_css
                            ,CString        p_cssText
                            ,bool           p_tagonly /* = true   */
                            ,int            p_tabs    /* = CSSALL */)
	            :CDialog(StyleSheetDlg::IDD, p_Parent)
              ,m_base(p_base)
              ,m_typeText(p_typeText)
              ,m_css(p_css)
              ,m_inlineSheet(p_cssText)
              ,m_tagonly(p_tagonly)
              ,m_doTabs(p_tabs)
              ,m_page1(NULL)
              ,m_page2(NULL)
              ,m_page3(NULL)
              ,m_page4(NULL)
              ,m_page5(NULL)
              ,m_page6(NULL)
              ,m_page7(NULL)
              ,m_canApply(false)
              ,m_didApply(false)
{
  m_inline  = false;
  if(!p_css)
  {
    //USE DIALOG FOR INLINE STYLE
    m_css = new CssStyleSheet();
    string style = (std::string) p_cssText;
    m_css->parse_css(style);
    m_inline = true;

    if(m_tagonly)
    {
      m_tag = p_typeText;
      CString desc = Misc::GetTagDescription(p_typeText);
      p_typeText.MakeUpper();
      m_typeText = CString("Inline style for: ") + p_typeText + " (" + desc + ")";
    }
  }
  m_firstTab = 0;

  if(m_doTabs & CSS1) m_page1 = new CSSPage1Dlg(this,m_css);
  if(m_doTabs & CSS2) m_page2 = new CSSPage2Dlg(this,m_css);
  if(m_doTabs & CSS3) m_page3 = new CSSPage3Dlg(this,m_css);
  if(m_doTabs & CSS4) m_page4 = new CSSPage4Dlg(this,m_css);
  if(m_doTabs & CSS5) m_page5 = new CSSPage5Dlg(this,m_css);
  if(m_doTabs & CSS6) m_page6 = new CSSPage6Dlg(this,m_css);
  if(m_doTabs & CSS7) m_page7 = new CSSPage7Dlg(this,m_css);
}

StyleSheetDlg::~StyleSheetDlg()
{
  if(m_inline)
  {
    delete m_css;
  }
  if(m_page1) delete m_page1;
  if(m_page2) delete m_page2;
  if(m_page3) delete m_page3;
  if(m_page4) delete m_page4;
  if(m_page5) delete m_page5;
  if(m_page6) delete m_page6;
  if(m_page7) delete m_page7;
}

void StyleSheetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
  DDX_Text   (pDX,IDC_SS_TYPE,      m_typeText);
  DDX_Control(pDX,IDC_SS_SELECTORS, m_listBox);
  DDX_Control(pDX,IDC_SS_NEW,       m_buttonNew);
  DDX_Control(pDX,IDC_SS_DELETE,    m_buttonDelete);
  DDX_Control(pDX,IDC_IMPORTS,      m_buttonImports);
  DDX_Control(pDX,IDOK,             m_buttonOK);
  DDX_Control(pDX,IDCANCEL,         m_buttonCancel);
  DDX_Control(pDX,IDC_SS_APPLY,     m_buttonApply);
  DDX_Control(pDX,IDC_SS_HELP,      m_buttonHelp);
  DDX_Control(pDX,IDC_SS_TABS,      m_tabs);

  m_buttonApply.EnableWindow(m_canApply);
}

BEGIN_MESSAGE_MAP(StyleSheetDlg, CDialog)
  ON_LBN_SELCHANGE(IDC_SS_SELECTORS,OnLbnSelchangeSsSelectors)
  ON_BN_CLICKED   (IDC_SS_NEW,      OnBnClickedSsNew)
  ON_BN_CLICKED   (IDC_SS_DELETE,   OnBnClickedSsDelete)
  ON_BN_CLICKED   (IDC_IMPORTS,     OnBnClickedImports)
  ON_BN_CLICKED   (IDOK,            OnBnClickedOk)
  ON_BN_CLICKED   (IDCANCEL,        OnBnClickedCancel)
  ON_BN_CLICKED   (IDC_SS_APPLY,    OnBnClickedSsApply)
  ON_BN_CLICKED   (IDC_SS_HELP,     OnBnClickedSsHelp)
  ON_NOTIFY(TCN_SELCHANGE,IDC_SS_TABS,OnTcnSelchangeTabcontrol)
END_MESSAGE_MAP()

BOOL
StyleSheetDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  if(m_tagonly)
  {
    //USE DIALOG FOR INLINE STYLE
    SetWindowText("Style editor");    

    m_buttonNew    .ShowWindow(SW_HIDE);
    m_buttonDelete .ShowWindow(SW_HIDE);
    m_buttonImports.ShowWindow(SW_HIDE);
    m_listBox     .ShowWindow(SW_HIDE);
    CWnd* s = GetDlgItem(IDC_SS_STATIC);
    s->ShowWindow(SW_HIDE);
  }

  if(m_doTabs & CSS1) m_page1->Create(IDD_SS_PAGE1,this);
  if(m_doTabs & CSS2) m_page2->Create(IDD_SS_PAGE2,this);
  if(m_doTabs & CSS3) m_page3->Create(IDD_SS_PAGE3,this);
  if(m_doTabs & CSS4) m_page4->Create(IDD_SS_PAGE4,this);
  if(m_doTabs & CSS5) m_page5->Create(IDD_SS_PAGE5,this);
  if(m_doTabs & CSS6) m_page6->Create(IDD_SS_PAGE6,this);
  if(m_doTabs & CSS7) m_page7->Create(IDD_SS_PAGE7,this);

  // Hier kan de TAB-volgorde veranderd worden:
  CString s;
  int tab = 0;
  if(m_doTabs & CSS1)
  {
    m_page1->GetWindowText(s);
    m_tabs.InsertItem(tab++,s);
  }
  if(m_doTabs & CSS2)
  {
    m_page2->GetWindowText(s);
    m_tabs.InsertItem(tab++,s);
  }
  if(m_doTabs & CSS3)
  {
    m_page3->GetWindowText(s);
    m_tabs.InsertItem(tab++,s);
  }
  if(m_doTabs & CSS4)
  {
    m_page4->GetWindowText(s);
    m_tabs.InsertItem(tab++,s);
  }
  if(m_doTabs & CSS5)
  {
    m_page5->GetWindowText(s);
    m_tabs.InsertItem(tab++,s);
  }
  if(m_doTabs & CSS6)
  {
    m_page6->GetWindowText(s);
    m_tabs.InsertItem(tab++,s);
  }
  if(m_doTabs & CSS7)
  {
    m_page7->GetWindowText(s);
    m_tabs.InsertItem(tab++,s);
  }
  CRect rect;
  m_tabs.GetWindowRect(&rect);    // geeft t.o.v. ouder window inclusief titelbar
  ScreenToClient(&rect);
  if(m_tagonly)
  {
    rect.left  -= TOLEFT;
    rect.right -= TOLEFT;
    m_tabs.MoveWindow(rect,true);
  }
  rect.top    += 21;   // 22
  rect.bottom -= 3;
  rect.right  -= 2;
  rect.left   += 1;    // 2

  if(m_doTabs & CSS1) m_page1->MoveWindow(rect,false);    // zet tov client window.
  if(m_doTabs & CSS2) m_page2->MoveWindow(rect,false);
  if(m_doTabs & CSS3) m_page3->MoveWindow(rect,false);
  if(m_doTabs & CSS4) m_page4->MoveWindow(rect,false);
  if(m_doTabs & CSS5) m_page5->MoveWindow(rect,false);
  if(m_doTabs & CSS6) m_page6->MoveWindow(rect,false);
  if(m_doTabs & CSS7) m_page7->MoveWindow(rect,false);

  TabCtrl_SetCurSel(m_tabs.GetSafeHwnd(),m_firstTab);

  if(!m_firstTab) 
  {
    m_firstTab = 1;
  }
  if(m_firstTab == 1) m_page1->ShowWindow(SW_SHOW);
  if(m_firstTab == 2) m_page2->ShowWindow(SW_SHOW);
  if(m_firstTab == 3) m_page3->ShowWindow(SW_SHOW);
  if(m_firstTab == 4) m_page4->ShowWindow(SW_SHOW);
  if(m_firstTab == 5) m_page5->ShowWindow(SW_SHOW);
  if(m_firstTab == 6) m_page6->ShowWindow(SW_SHOW);
  if(m_firstTab == 7) m_page7->ShowWindow(SW_SHOW);

  // Fill selector list and position at first selector
  FillSelectorList();
  if(m_listBox.GetCount() >= 1)
  {
    m_listBox.SetCurSel(0);
    // Show tabs
    OnLbnSelchangeSsSelectors();
  }
  m_canApply = false;
  m_buttonApply.EnableWindow(false);

  if(m_tagonly)
  {
    // Now reajust other fields/buttons and total dialog
    CWnd* t = GetDlgItem(IDC_SS_TYPE);
    t->GetWindowRect(&rect);
    ScreenToClient(&rect);
    rect.right -= TOLEFT;
    t->MoveWindow(&rect,true);

    MoveButton(&m_buttonOK);
    MoveButton(&m_buttonCancel);
    MoveButton(&m_buttonApply);
    MoveButton(&m_buttonHelp);

    // Now the total dialog
    GetWindowRect(&rect);
    rect.right -= TOLEFT;
    MoveWindow(&rect,true);

    //CString descriptor = Misc::GetTagDescription(m_typeText);
    if(m_doTabs & CSS1) m_page1->SetSelector(m_tag,m_typeText);
    if(m_doTabs & CSS2) m_page2->SetSelector((string)m_tag);
    if(m_doTabs & CSS3) m_page3->SetSelector((string)m_tag);
    if(m_doTabs & CSS4) m_page4->SetSelector((string)m_tag);
    if(m_doTabs & CSS5) m_page5->SetSelector((string)m_tag);
    if(m_doTabs & CSS6) m_page6->SetSelector((string)m_tag);
    if(m_doTabs & CSS7) m_page7->SetSelector((string)m_tag);
  }
  return TRUE;
}

void
StyleSheetDlg::MoveButton(AD_Button* but)
{
  CRect rect;
  but->GetWindowRect(&rect);
  ScreenToClient(&rect);
  rect.left  -= TOLEFT;
  rect.right -= TOLEFT;
  but->MoveWindow(&rect,true);
}

void
StyleSheetDlg::FillSelectorList()
{
  string media = "standard";
  vector<string> selectors;
  m_css->GetSelectors(media,&selectors);

  // Empty the listbox first
  int total = m_listBox.GetCount();
  for(int ind = 0; ind < total; ++ ind)
  {
    m_listBox.DeleteString(0);
  }
  for(unsigned int num = 0; num < selectors.size(); ++num)
  {
    CString sel = selectors[num].c_str();
    if(!sel.IsEmpty())
    {
      m_listBox.AddString(sel);
    }
  }
  UpdateData(Data2Controls);
}

void
StyleSheetDlg::UpdateProperties()
{
  if(m_canApply)
  {
    m_canApply = false;
    m_didApply = true;
    //m_page2->UpdateProperties(); ???
    // Print back to CSS file
    m_css->print_css();
    if(m_inline)
    {
      m_inlineSheet = m_css->GetTheSheet().c_str();
    }
    // Re-disable the button
    m_buttonApply.EnableWindow(FALSE);
  }
}

void
StyleSheetDlg::SetCanApply()
{
  m_canApply = true;
  // Resemble properties on list at first page at all times
  m_page1->UpdateProperties();
  m_buttonApply.EnableWindow(TRUE);
}

// StyleSheetDlg message handlers

void 
StyleSheetDlg::OnLbnSelchangeSsSelectors()
{
  int num = m_listBox.GetCurSel();
  if(num >= 0)
  {
    CString selector;
    m_listBox.GetText(num,selector);
    CString descriptor = Misc::GetTagDescription(selector);
    if(m_doTabs & CSS1) m_page1->SetSelector(selector,descriptor);
    if(m_doTabs & CSS2) m_page2->SetSelector((string)selector);
    if(m_doTabs & CSS3) m_page3->SetSelector((string)selector);
    if(m_doTabs & CSS4) m_page4->SetSelector((string)selector);
    if(m_doTabs & CSS5) m_page5->SetSelector((string)selector);
    if(m_doTabs & CSS6) m_page6->SetSelector((string)selector);
    if(m_doTabs & CSS7) m_page7->SetSelector((string)selector);
  }
}

void 
StyleSheetDlg::OnBnClickedSsNew()
{
  NewSelectorDlg dlg(this);
  if(dlg.DoModal() == IDOK)
  {
    CString selector = dlg.GetSelector();
    if(!selector.IsEmpty())
    {
      int ind = m_listBox.AddString(selector);
      m_listBox.SetCurSel(ind);
      UpdateData(Data2Controls);
      OnLbnSelchangeSsSelectors();
    }
  }
}

void 
StyleSheetDlg::OnBnClickedSsDelete()
{
  theApp.MessageBox(m_css->print_logs().c_str(),"CSS Logs",MB_OK);


  int num = m_listBox.GetCurSel();
  if(num >= 0)
  {
    CString selector;
    m_listBox.GetText(num,selector);
    CString descriptor = Misc::GetTagDescription(selector);
    CString ask;
    ask.Format("Would you like to delete selector \"%s\"",selector.GetString());
    if(!descriptor.IsEmpty())
    {
      ask += CString(" (") + descriptor + ")";
    }
    ask += " ?";
    if(theApp.MessageBox(ask
                        ,"DELETE?"
                        ,MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON2) == IDYES)
    {
      string media = "standard";
      string sel   = selector;
      m_css->del_selector(media,sel);
      // Redisplay the selector list
      FillSelectorList();
      // Reselect about where we were.
      if(num >= m_listBox.GetCount()) 
      {
        num = m_listBox.GetCount() - 1;
      }
      if(num >= 0)
      {
        m_listBox.SetCurSel(num);
        OnLbnSelchangeSsSelectors();
      }
      SetCanApply();
    }
  }
}

void 
StyleSheetDlg::OnBnClickedImports()
{
  StyleSheetImportsDlg dialog(this,m_css,m_base);
  if(dialog.DoModal() == IDOK)
  {
    if(dialog.CanApply())
    {
      SetCanApply();
    }
  }
}

void 
StyleSheetDlg::OnBnClickedOk()
{
  UpdateProperties();
  OnOK();
}

void 
StyleSheetDlg::OnBnClickedSsApply()
{
  UpdateProperties();
}

void 
StyleSheetDlg::OnBnClickedCancel()
{
  OnCancel();
}

void 
StyleSheetDlg::OnTcnSelchangeTabcontrol(NMHDR *pNMHDR, LRESULT *pResult)
{
  int page = 0;
  int find = CSS1;
  int num  = TabCtrl_GetCurSel(pNMHDR->hwndFrom);

  if(m_doTabs & find)
  {
    --num;
  }
  while(num >= 0 && find < CSS7)
  {
    ++page;
    find = (find << 1);
    if(m_doTabs & find)
    {
      --num;
    }
  }
  if(m_doTabs & CSS1) m_page1->ShowWindow(FALSE);
  if(m_doTabs & CSS2) m_page2->ShowWindow(FALSE);
  if(m_doTabs & CSS3) m_page3->ShowWindow(FALSE);
  if(m_doTabs & CSS4) m_page4->ShowWindow(FALSE);
  if(m_doTabs & CSS5) m_page5->ShowWindow(FALSE);
  if(m_doTabs & CSS6) m_page6->ShowWindow(FALSE);
  if(m_doTabs & CSS7) m_page7->ShowWindow(FALSE);

  switch(page)
  {
    case 0: m_page1->ShowWindow(TRUE);  break;
    case 1: m_page2->ShowWindow(TRUE);  break;
    case 2: m_page3->ShowWindow(TRUE);  break;
    case 3: m_page4->ShowWindow(TRUE);  break;
    case 4: m_page5->ShowWindow(TRUE);  break;
    case 5: m_page6->ShowWindow(TRUE);  break;
    case 6: m_page7->ShowWindow(TRUE);  break;
  }
  *pResult = 0;
}

void    
StyleSheetDlg::SetBackgroundColor(CString color)
{
  m_page2->SetBackgroundColor(color);
}

void 
StyleSheetDlg::OnBnClickedSsHelp()
{
  // TODO: Add your control notification handler code here
}

