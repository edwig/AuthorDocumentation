//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    StyleSheetsDlg.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Overview of CSS files
//
#include "stdafx.h"
#include "AuthorHTML.h"
#include "StyleSheetDlg.h"
#include "StyleSheetsDlg.h"
#include "FileDialog.h"
#include "CSS.h"
#include "Misc.h"

// StyleSheetsDlg dialog

IMPLEMENT_DYNAMIC(StyleSheetsDlg, CDialog)
StyleSheetsDlg::StyleSheetsDlg(CWnd*         pParent
                              ,CString       typeText
                              ,CSSVector&    list
                              ,CComPtr<IHTMLDocument2> pDoc
                              ,CString       p_base)
	            :CDialog(StyleSheetsDlg::IDD, pParent)
              ,m_typeText(typeText)
              ,m_list(list)
              ,m_doc(pDoc)
              ,m_base(p_base)
{
  m_changes    = false;
  m_didChanges = false;
}

StyleSheetsDlg::~StyleSheetsDlg()
{
}

void 
StyleSheetsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
  DDX_Text   (pDX, IDC_STYLETYPE, m_typeText);
  DDX_Control(pDX, IDC_CSS_LIST,  m_listBox);
  DDX_Control(pDX, IDC_CSS_EDIT,  m_buttonEdit);
  DDX_Control(pDX, IDC_SS_ATTACH, m_buttonAttach);
  DDX_Control(pDX, IDC_CSS_NEW,   m_buttonNew);
  DDX_Control(pDX, IDC_CSS_DELETE,m_buttonDelete);
  DDX_Control(pDX, IDC_CSS_UP,    m_buttonUp);
  DDX_Control(pDX, IDC_CSS_DOWN,  m_buttonDown);
  DDX_Control(pDX, IDOK,          m_buttonOK);
  DDX_Control(pDX, IDCANCEL,      m_buttonCancel);
  DDX_Control(pDX, IDC_CSS_APPLY, m_buttonApply);
  DDX_Control(pDX, IDC_CSS_HELP,  m_buttonHelp);

  // Set UP/DOWN buttons
  int num = m_listBox.GetCount();
  int now = m_listBox.GetCurSel();
  m_buttonUp  .EnableWindow(now > 0);
  m_buttonDown.EnableWindow(now < (num -1) && (now >= 0));
  // Only Edit/delete button if we ar at a line
  m_buttonEdit  .EnableWindow(now >= 0);
  m_buttonDelete.EnableWindow(now >= 0);
  // If there are changes, you can apply.
  m_buttonApply.EnableWindow(m_changes);

  // Only if started for an HTML document
  m_buttonAttach.EnableWindow(m_doc != NULL);
  m_buttonNew.EnableWindow   (m_doc != NULL);
  m_buttonDelete.EnableWindow(m_doc != NULL);
}

BEGIN_MESSAGE_MAP(StyleSheetsDlg, CDialog)
  ON_LBN_SELCHANGE(IDC_CSS_LIST,  OnLbnSelchangeCssList)
  ON_LBN_DBLCLK   (IDC_CSS_LIST,  OnLbnDoubleClickList)
  ON_BN_CLICKED   (IDC_CSS_UP,    OnBnClickedCssUp)
  ON_BN_CLICKED   (IDC_CSS_DOWN,  OnBnClickedCssDown)
  ON_BN_CLICKED   (IDC_CSS_EDIT,  OnBnClickedCssEdit)
  ON_BN_CLICKED   (IDC_SS_ATTACH, OnBnClickedSsAttach)
  ON_BN_CLICKED   (IDC_CSS_NEW,   OnBnClickedCssNew)
  ON_BN_CLICKED   (IDC_CSS_DELETE,OnBnClickedCssDelete)
  ON_BN_CLICKED   (IDOK,          OnBnClickedOk)
  ON_BN_CLICKED   (IDC_CSS_APPLY, OnBnClickedCssApply)
  ON_BN_CLICKED   (IDCANCEL,      OnBnClickedCancel)
  ON_BN_CLICKED   (IDC_CSS_HELP,  OnBnClickedCssHelp)
END_MESSAGE_MAP()

BOOL     
StyleSheetsDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  AddArrayToListbox();
  if(m_listBox.GetCount())
  {
    m_listBox.SetCurSel(0);
  }
  UpdateData(Data2Controls);
  return TRUE;
}

void
StyleSheetsDlg::AddArrayToListbox()
{
  m_listBox.ResetContent();
  //EmptyListbox();
  for(unsigned int n = 0;n < m_list.size(); ++n)
  {
    SheetDef def = m_list[n];
    if(def.filename.IsEmpty())
    {
      m_listBox.AddString("<Internal stylesheet>");
    }
    else
    {
      m_listBox.AddString(def.filename);
    }
  }
}

void
StyleSheetsDlg::EmptyListbox()
{
  int total = m_listBox.GetCount();
  for(int ind = 0; ind < total; ++ind)
  {
    m_listBox.DeleteString(0);
  }
}

IHTMLElement*
StyleSheetsDlg::SetLink(CString linkURL)
{
  if(m_doc == NULL)
  {
    return NULL;
  }
  CComPtr<IHTMLElement> elem = Misc::CreateHeadElement(m_doc,TAGID_LINK);
  CComQIPtr<IHTMLLinkElement,&IID_IHTMLLinkElement> meta = elem;
  CComBSTR bUrl  = CT2CW(linkURL);
  CComBSTR bRel  = L"Stylesheet";
  CComBSTR bType = L"text/css";
  meta->put_href(bUrl);
  meta->put_rel (bRel);
  meta->put_type(bType);

  elem.p->AddRef();
  return elem.p;
}

IHTMLElement*
StyleSheetsDlg::SetStyle(CString cssText)
{
  if(m_doc == NULL)
  {
    return NULL;
  }
  CComPtr<IHTMLElement> elem = Misc::CreateHeadElement(m_doc,TAGID_STYLE);
  CComQIPtr<IHTMLStyleElement,&IID_IHTMLStyleElement> style = elem;
  CComBSTR bText = CT2CW(cssText);
  CComBSTR bType = L"text/css";
  style->put_type(bType);
  CComPtr<IHTMLStyleSheet> sheet;
  style->get_styleSheet(&sheet);
  sheet->put_cssText(bText);

  elem.p->AddRef();
  return elem.p;
}

void
StyleSheetsDlg::DeleteNode(IHTMLElement* elem)
{
  try
  {
    elem->Release();
    // Now delete it
    CComQIPtr<IHTMLDOMNode,&IID_IHTMLDOMNode> linknode = elem;
    CComPtr<IHTMLDOMNode> removed;
    linknode->removeNode(VARIANT_TRUE,&removed);
  }
  catch (...)
  {
  	// Now what ?
  }
}

void
StyleSheetsDlg::SetStyleSheets()
{
  if(m_changes)
  {
    // Delete the current definitions
    for(unsigned int n = 0;n < m_list.size(); ++n)
    {
      SheetDef def = m_list[n];
      if(def.link)
      {
        DeleteNode(def.link);
      }
      if(def.style)
      {
        DeleteNode(def.style);
      }
      def.link  = NULL;
      def.style = NULL;
    }
    // Re-Add the definitions
    for(unsigned int n = 0;n < m_list.size(); ++n)
    {
      SheetDef def = m_list[n];
      if(!def.filename.IsEmpty())
      {
        def.link =  SetLink(def.filename);
      }
      else
      {
        def.style = SetStyle(def.csstext);
      }
    }
    // Update the APPLY button
    m_changes    = false;
    m_didChanges = true;
    UpdateData(Data2Controls);
  }
}

// StyleSheetsDlg message handlers
void 
StyleSheetsDlg::OnLbnSelchangeCssList()
{
  // To adjust UP/DOWN Buttons
  UpdateData(Data2Controls);
}

void
StyleSheetsDlg::OnLbnDoubleClickList()
{
  int num = m_listBox.GetCurSel();
  if(num >= 0)
  {
    SheetDef def = m_list[num];
    if(!def.filename.IsEmpty())
    {
      CString base = m_base;
      CString cssfile(def.filename);
      cssfile = Misc::StripFileProtocol(cssfile);
      CString fullpath = cssfile;
      if(cssfile[1] != ':')
      {
        // RE-BASE
        base += Misc::DirectoryPart(cssfile);
        base  = Misc::ReduceDirectoryPath(base);
        fullpath = m_base + cssfile;
      }
      CssStyleSheet css;
      // Empty file (true) is alsoo OK, could be a new CSS Stylesheet!
      if(css.SetFile(fullpath,true))
      {
        CString styleText;
        CString type = "Style sheet in external file: " + cssfile;
        StyleSheetDlg sheet(this,base,type,&css,styleText,false);
        if(sheet.DoModal() == IDOK)
        {
          if(sheet.DidApply())
          {
            m_changes = true;
          }
        }
      }
      else
      {
        string logs = css.print_logs();
        theApp.MessageBox(logs.c_str()
                        ,"Errors in CSS Stylesheet"
                        ,MB_OK|MB_ICONSTOP);
      }
    }
    else // link
    {
      CString type = "Document internal";
      CssStyleSheet css;
      css.parse_css((string)def.csstext);
      CString text;
      StyleSheetDlg sheet(this,m_base,type,&css,text,false);
      if(sheet.DoModal() == IDOK)
      {
        //CString text = sheet.GetInlineStylesheet(); 
        css.print_css();
        CString text2 = css.GetTheSheet().c_str();
        text2.TrimLeft("{\n}\n");
        if(def.csstext.CompareNoCase(text2))
        {
          m_list[num].csstext = text2;
          m_changes = true;
        }
      }
    }
  }
  UpdateData(Data2Controls);
}

void 
StyleSheetsDlg::OnBnClickedCssUp()
{
  int num = m_listBox.GetCurSel();
  if(num >= 1)
  {
    // Switch two items
    std::swap(m_list[num-1],m_list[num]);
    m_changes = true;
    // Set again to listbox
    AddArrayToListbox();
    m_listBox.SetCurSel(num -1);
    // Display them.
    UpdateData(Data2Controls);
  }
}

void 
StyleSheetsDlg::OnBnClickedCssDown()
{
  int total = m_listBox.GetCount();
  int num   = m_listBox.GetCurSel();
  if(num < (total -1))
  {
    // Switch two items
    std::swap(m_list[num],m_list[num+1]);
    m_changes = true;
    // Set again to listbox
    AddArrayToListbox();
    m_listBox.SetCurSel(num + 1);
    // Display them.
    UpdateData(Data2Controls);
  }
}

void 
StyleSheetsDlg::OnBnClickedCssEdit()
{
  OnLbnDoubleClickList();
}

void 
StyleSheetsDlg::OnBnClickedCssNew()
{
  CString question = "What type of stylesheet do you want to create?\n"
                     "An external linked stylesheet (file of type CSS)\n"
                     "An internal stylesheet in a TAG\n";
  CString type = theApp.MessageBox(question,"Question","!External_sheet @Internal_sheet");
  if(type == "external sheet")
  {
    DocFileDialog diag(false
                      ,"Create a new CSS Stylesheet"
                      ,"css"
                      ,""
                      ,0
                      ,"Cascading style sheet (*.css)|*.css|");
    if(diag.DoModal() == IDOK)
    {
      CString file = diag.GetChosenFile();
      CString relative;
      if(Misc::MakeRelativePathname(m_base,file,relative))
      {
        file = relative;
      }
      SheetDef def;
      def.filename = file;
      def.link     = NULL;
      def.style    = NULL;
      m_list.push_back(def);
      int num = m_list.size()-1;

      m_changes = true;
      AddArrayToListbox();
      m_listBox.SetCurSel(num);
    }
  }
  if(type == "internal sheet")
  {
    SheetDef def;
    def.link = NULL;
    def.style = NULL;
    m_list.push_back(def);
    int num = m_list.size()-1;

    m_changes = true;
    AddArrayToListbox();
    m_listBox.SetCurSel(num);
  }
  // Display them.
  UpdateData(Data2Controls);
}

void 
StyleSheetsDlg::OnBnClickedSsAttach()
{
  DocFileDialog diag(true
                     ,"Attach an existing CSS Stylesheet"
                     ,"css"
                     ,""
                     ,OFN_FILEMUSTEXIST
                     ,"Cascading style sheet (*.css)|*.css|");
  if(diag.DoModal() == IDOK)
  {
    CString file = diag.GetChosenFile();
    CString relative;
    if(Misc::MakeRelativePathname(m_base,file,relative))
    {
      file = relative;
    }
    SheetDef def;
    def.filename = file;
    def.link     = NULL;
    def.style    = NULL;
    m_list.push_back(def);
    int num = m_list.size() -1;

    m_changes = true;
    AddArrayToListbox();
    m_listBox.SetCurSel(num);
    // Display them.
    UpdateData(Data2Controls);
  }
}

void 
StyleSheetsDlg::OnBnClickedCssDelete()
{
  int ind = m_listBox.GetCurSel();
  if(ind >= 0)
  {
    CString toDelete;
    m_listBox.GetText(ind,toDelete);

    CString ask;
    ask.Format("Do you really want to delete the link to stylesheet '%s'?",toDelete.GetString());
    if(theApp.MessageBox(ask
                        ,"Question"
                        ,MB_YESNO|MB_ICONQUESTION) == IDYES)
    {
      // Remove from list
      CSSVector::iterator it = m_list.begin();
      int n = ind;
      while(n--) ++it;

      if(it->link) DeleteNode(it->link);
      if(it->style)DeleteNode(it->style);
      m_list.erase(it);

      m_changes = true;
      // Redisplay the list box
      AddArrayToListbox();
      m_listBox.SetCurSel(ind);
      // Display them.
      UpdateData(Data2Controls);
    }
  }
}

void 
StyleSheetsDlg::OnBnClickedOk()
{
  SetStyleSheets();
  OnOK();
}

void 
StyleSheetsDlg::OnBnClickedCssApply()
{
  SetStyleSheets();
}

void 
StyleSheetsDlg::OnBnClickedCancel()
{
  OnCancel();
}

void StyleSheetsDlg::OnBnClickedCssHelp()
{
  // TODO: HELP
}

