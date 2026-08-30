//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    TopicPropPage3.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for all topic properties page 3 of 5
//
#include "stdafx.h"
#include "AuthorHTML.h"
#include "TopicPropPage3.h"
#include "KeywordDlg.h"
#include "IndexFile.h"
#include "HTMLScript.h"
#include "ScriptDlg.h"

// TopicPropPage3 dialog
IMPLEMENT_DYNAMIC(TopicPropPage3Dlg, CDialog)

TopicPropPage3Dlg::TopicPropPage3Dlg(CWnd* pParent
                                    ,CComPtr<IHTMLDocument2> pDoc
                                    ,CString base
                                    ,DocumentFile* doc)
	                :CDialog(TopicPropPage3Dlg::IDD, pParent)
                  ,m_htmlDoc(pDoc)
                  ,m_base(base)
                  ,m_doc(doc)
{
}

TopicPropPage3Dlg::~TopicPropPage3Dlg()
{
  ResetScripts();
}

void
TopicPropPage3Dlg::ResetScripts()
{
  for(unsigned int ind = 0;ind < m_keywords.size(); ++ind)
  {
    KeywordDef* def = &(m_keywords[ind]);
    def->keyword->Release();
  }
  m_keywords.clear();
}

void TopicPropPage3Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
  DDX_Control(pDX,IDC_LIST1,    m_list);
  DDX_Control(pDX,IDC_UP,       m_buttonUp);
  DDX_Control(pDX,IDC_EDIT,     m_buttonEdit);
  DDX_Control(pDX,IDC_NEW,      m_buttonNew);
  DDX_Control(pDX,IDC_DELINDEX, m_buttonDelete);
  DDX_Control(pDX,IDC_DOWN,     m_buttonDown);

  if(pDX->m_bSaveAndValidate == Data2Controls)
  {
    int num = m_list.GetItemCount();
    int now = m_list.GetNextItem(-1,LVNI_SELECTED);

    m_buttonUp  .EnableWindow(now > 0);
    m_buttonDown.EnableWindow(now < (num -1) && (now >= 0));
    // Only Edit/delete button if we ar at a line
    m_buttonEdit  .EnableWindow(now >= 0);
    m_buttonDelete.EnableWindow(now >= 0);
  }
}

BEGIN_MESSAGE_MAP(TopicPropPage3Dlg, CDialog)
  ON_NOTIFY(LVN_ITEMACTIVATE,IDC_LIST1, OnLvnItemDouble)
  ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, OnLvnChanged)
  ON_BN_CLICKED(IDC_UP,       OnBnClickedUp)
  ON_BN_CLICKED(IDC_EDIT,     OnBnClickedEdit)
  ON_BN_CLICKED(IDC_NEW,      OnBnClickedNew)
  ON_BN_CLICKED(IDC_DELINDEX, OnBnClickedDelete)
  ON_BN_CLICKED(IDC_DOWN,     OnBnClickedDown)
END_MESSAGE_MAP()

BOOL
TopicPropPage3Dlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EDITLABELS);
  m_list.InsertColumn(0,"Type",   LVCFMT_LEFT,80);
  m_list.InsertColumn(1,"Level1", LVCFMT_LEFT,100);
  m_list.InsertColumn(2,"Level2", LVCFMT_LEFT,100);
  m_list.InsertColumn(3,"Level3", LVCFMT_LEFT,100);
  m_list.InsertColumn(4,"Level4", LVCFMT_LEFT,100);
  m_list.InsertColumn(5,"Level5", LVCFMT_LEFT,100);

  FillPage();
  UpdateData(Data2Controls);
  return FALSE;
}

void
TopicPropPage3Dlg::FillPage()
{
  ResetScripts();
  GetHeadKeywords();
  ScriptsToList();
}

void
TopicPropPage3Dlg::UpdateProperties()
{
}

void
TopicPropPage3Dlg::ScriptsToList()
{
  m_list.DeleteAllItems();
  for(unsigned int ind = 0; ind < m_keywords.size(); ++ind)
  {
    KeywordDef* def = &(m_keywords[ind]);
    m_list.InsertItem(LVIF_TEXT|LVIF_STATE, ind, def->m_type, 0, 0, 0, 0);
    m_list.SetItemText(ind,1,def->m_level1);
    m_list.SetItemText(ind,2,def->m_level2);
    m_list.SetItemText(ind,3,def->m_level3);
    m_list.SetItemText(ind,4,def->m_level4);
    m_list.SetItemText(ind,5,def->m_level5);
  }
  if(m_keywords.size())
  {
    m_list.SetItemState(0,LVNI_SELECTED,LVNI_SELECTED);
    m_list.SetFocus();
  }
}

void
TopicPropPage3Dlg::ShowFocus()
{
  m_list.SetFocus();
}

// <meta content="Invalid,Credit card number" name="MS-HKWD">
void
TopicPropPage3Dlg::GetHeadKeywords()
{
  CComPtr<IHTMLElementCollection> coll;

  HRESULT hr = m_htmlDoc->get_all(&coll);
  if(SUCCEEDED(hr))
  {
    CComBSTR name = L"head";
    CComVariant selector;
    V_VT(&selector) = VT_BSTR;
    V_BSTR(&selector) = name;
    CComPtr<IDispatch> tdisp;
    hr = coll->tags(selector,&tdisp);
    CComQIPtr<IHTMLElementCollection,&IID_IHTMLElementCollection> tagscol = tdisp;
    if(SUCCEEDED(hr))
    {
      CComVariant ask;
      V_VT(&ask) = VT_I4;
      V_I4(&ask) = 0;
      CComPtr<IDispatch> item;
      hr = tagscol->item(ask,ask,&item);
      CComQIPtr<IHTMLHeadElement,&IID_IHTMLHeadElement> head = item;
      if(SUCCEEDED(hr) && head.p)
      {
        IDispatch* disp;
        CComQIPtr<IHTMLElement,&IID_IHTMLElement> pElem = item;
        hr = pElem->get_children(&disp);
        CComQIPtr<IHTMLElementCollection,&IID_IHTMLElementCollection> headcol = disp;
        if(SUCCEEDED(hr) && headcol.p)
        {
          name = L"meta";
          V_BSTR(&selector) = name;
          CComPtr<IDispatch> sdisp;
          hr = headcol->tags(selector,&sdisp);
          CComQIPtr<IHTMLElementCollection,&IID_IHTMLElementCollection> scriptcol = sdisp;
          if(SUCCEEDED(hr) && scriptcol.p)
          {
            long count = 0;
            scriptcol->get_length(&count);
            for(int ind = 0; ind < count; ++ind)
            {
              V_I4(&ask) = ind;
              IDispatch* idisp;
              hr = scriptcol->item(ask,ask,&idisp);
              CComQIPtr<IHTMLMetaElement,&IID_IHTMLMetaElement> meta = idisp;
              if(SUCCEEDED(hr) && meta.p)
              {
                // Yoepieee, its a meta tag in the head
                CComQIPtr<IHTMLElement,&IID_IHTMLElement> elem = idisp;
                HtmlElement element(elem);

                CString key = element.GetAttribute("name");
                if((key.CompareNoCase("MS-HKWD") == 0) ||
                   (key.CompareNoCase("MS-HAID") == 0)  )
                {
                  CString keywords = element.GetAttribute("content");
                  KeywordDef def;

                  def.keyword = meta.p;
                  def.m_type  = key.CompareNoCase("MS-HKWD") == 0 ? "Index" : "Association";
                  def.m_composite = keywords;
                  AddKeywords(&def,keywords);
                  def.keyword->AddRef();

                  m_keywords.push_back(def);
                }
              }
            }
          }
        }
      }
    }
  }
}

void
TopicPropPage3Dlg::AddKeywords(KeywordDef* def,CString keywords)
{
  CString keyword;
  int level = 0;

  // Break into seperate strings.
  while(keywords.GetLength() > 0) 
  {
    int pos = keywords.Find(',');
    if(pos >= 0)
    {
      keyword  = keywords.Left(pos);
      keywords = keywords.Mid(pos);
      keywords.TrimLeft(',');
    }
    else
    {
      keyword  = keywords;
      keywords = "";
    }
    switch(level)
    {
      case 0: def->m_level1 = keyword; break;
      case 1: def->m_level2 = keyword; break;
      case 2: def->m_level3 = keyword; break;
      case 3: def->m_level4 = keyword; break;
      case 4: def->m_level5 = keyword; break;
      default:break; 
    }
    ++level;
  } 
  if(level > 4)
  {
    CString message;
    message.Format("Composite index keyword truncated after 5 levels.\n"
                   "Remaining keywords: %s %s",keyword.GetString(),keywords.GetString());
    theApp.MessageBox(message,"Index keywords",MB_OK|MB_ICONERROR);
  }
}

// TopicPropPage3 message handlers

void TopicPropPage3Dlg::OnLvnChanged(NMHDR* /*pNMHDR*/, LRESULT *pResult)
{
  UpdateData(Data2Controls);
  *pResult = 0;
}

void TopicPropPage3Dlg::OnLvnItemDouble(NMHDR* /*pNMHDR*/, LRESULT *pResult)
{
  UpdateData(Data2Controls);
  OnBnClickedEdit();
  *pResult = 0;
}

void 
TopicPropPage3Dlg::OnBnClickedUp()
{
  int now = m_list.GetNextItem(-1,LVNI_SELECTED);
  if(now > 0)
  {
    std::swap(m_keywords[now],m_keywords[now-1]);
    ScriptsToList();
    UpdateData(Data2Controls);
    m_list.SetItemState(now-1,LVNI_SELECTED,LVNI_SELECTED);
  }
  m_list.SetFocus();
}

void 
TopicPropPage3Dlg::OnBnClickedEdit()
{
  int num = m_list.GetItemCount();
  int now = m_list.GetNextItem(-1,LVNI_SELECTED);
  if(now < num)
  {
    KeywordDef* def = &(m_keywords[now]);
    CComQIPtr<IHTMLElement,&IID_IHTMLElement> elem = def->keyword;
    HtmlElement keyword(elem);
    KeywordDlg dlg(this,&keyword,m_base,def);
    if(dlg.DoModal() == IDOK)
    {
      FillPage();
      UpdateData(Data2Controls);
    }
  }
  m_list.SetFocus();
}

void 
TopicPropPage3Dlg::OnBnClickedNew()
{
  CComPtr<IHTMLElement> elem = Misc::CreateHeadElement(m_htmlDoc,TAGID_META);
  CComQIPtr<IHTMLMetaElement,&IID_IHTMLMetaElement> meta = elem;
  KeywordDef def;
  def.keyword = meta;
  def.m_type  = "Index";
  meta.p->AddRef();
  HtmlElement keyword(elem);
  keyword.SetAttribute("name","MS-HKWD");
  KeywordDlg dlg(this,&keyword,m_base,&def);
  if(dlg.DoModal() == IDOK)
  {
    m_keywords.push_back(def);
    ScriptsToList();
    UpdateData(Data2Controls);
  }
  else
  {
    keyword.Remove();
  }
  m_list.SetFocus();
}

void 
TopicPropPage3Dlg::OnBnClickedDelete()
{
  int num = m_list.GetItemCount();
  int now = m_list.GetNextItem(-1,LVNI_SELECTED);
  if(now < num)
  {
    CString mess;
    KeywordDef* def = &(m_keywords[now]);
    CString keyword = def->m_composite;
    CString type    = def->m_type == "Index" ? "Index keyword" : "Associative link";
    mess.Format("Do you want to delete the %s [%s] ?",type.GetString(),keyword.GetString());
    if(theApp.MessageBox(mess,"Delete?",MB_YESNO|MB_ICONQUESTION) == IDYES)
    {
      KeywordDef* kdef = &(m_keywords[now]);
      CComQIPtr<IHTMLDOMNode,&IID_IHTMLDOMNode> dom = kdef->keyword;
      HRESULT hr = dom->removeNode(VARIANT_TRUE,NULL);
      if(SUCCEEDED(hr))
      {
        KeywordVector::iterator it = m_keywords.begin();
        while(now)
        {
          ++it;
          --now;
        }
        // Release the DOM pointer
        kdef->keyword->Release();
        // Erase from vector scripts
        m_keywords.erase(it);
        // Rebuild visual list
        ScriptsToList();
        UpdateData(Data2Controls);

        CString filename = m_doc->GetFilename();
        IndexFile* index = theApp.GetIndex();
        index->DeleteEntry(keyword,filename);
      }
    }
  }
  m_list.SetFocus();
}

void 
TopicPropPage3Dlg::OnBnClickedDown()
{
  int num = m_list.GetItemCount();
  int now = m_list.GetNextItem(-1,LVNI_SELECTED);
  if(now < (num -1))
  {
    std::swap(m_keywords[now],m_keywords[now+1]);
    ScriptsToList();
    UpdateData(Data2Controls);
    m_list.SetItemState(now+1,LVNI_SELECTED,LVNI_SELECTED);
  }
  m_list.SetFocus();
}
