//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    TopicPropPage4.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for all topic properties page 4 of 5
//
#include "stdafx.h"
#include "AuthorHTML.h"
#include "TopicPropPage4.h"
#include "HTMLScript.h"
#include "ScriptDlg.h"

// TopicPropPage4 dialog
IMPLEMENT_DYNAMIC(TopicPropPage4Dlg, CDialog)

TopicPropPage4Dlg::TopicPropPage4Dlg(CWnd* pParent
                                     ,CComPtr<IHTMLDocument2> pDoc
                                     ,CString base)
                                     :CDialog(TopicPropPage4Dlg::IDD, pParent)
                                     ,m_htmlDoc(pDoc)
                                     ,m_base(base)
{
}

TopicPropPage4Dlg::~TopicPropPage4Dlg()
{
  ResetScripts();
}

void
TopicPropPage4Dlg::ResetScripts()
{
  for(unsigned int ind = 0;ind < m_scripts.size(); ++ind)
  {
    ScriptDef* def = &(m_scripts[ind]);
    def->script->Release();
  }
  m_scripts.clear();
}

void TopicPropPage4Dlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Control(pDX,IDC_LIST1,    m_list);
  DDX_Control(pDX,IDC_UP,       m_buttonUp);
  DDX_Control(pDX,IDC_EDIT,     m_buttonEdit);
  DDX_Control(pDX,IDC_NEW,      m_buttonNew);
  DDX_Control(pDX,IDC_DELSCRIPT, m_buttonDelete);
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

BEGIN_MESSAGE_MAP(TopicPropPage4Dlg, CDialog)
  ON_NOTIFY(LVN_ITEMACTIVATE,IDC_LIST1, OnLvnItemDouble)
  ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, OnLvnChanged)
  ON_BN_CLICKED(IDC_UP,       OnBnClickedUp)
  ON_BN_CLICKED(IDC_EDIT,     OnBnClickedEdit)
  ON_BN_CLICKED(IDC_NEW,      OnBnClickedNew)
  ON_BN_CLICKED(IDC_DELSCRIPT,OnBnClickedDelete)
  ON_BN_CLICKED(IDC_DOWN,     OnBnClickedDown)
END_MESSAGE_MAP()

BOOL
TopicPropPage4Dlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EDITLABELS);
  m_list.InsertColumn(0,"Type",     LVCFMT_LEFT,100);
  m_list.InsertColumn(1,"Language", LVCFMT_LEFT,100);
  m_list.InsertColumn(2,"Deferred", LVCFMT_LEFT,80);
  m_list.InsertColumn(3,"Script",   LVCFMT_LEFT,400);

  UpdateData(Data2Controls);
  FillPage();
  return FALSE;
}

void
TopicPropPage4Dlg::FillPage()
{
  GetHeadScripts();
  ScriptsToList();
}

void
TopicPropPage4Dlg::UpdateProperties()
{
}

void
TopicPropPage4Dlg::ScriptsToList()
{
  m_list.DeleteAllItems();
  for(unsigned int ind = 0; ind < m_scripts.size(); ++ind)
  {
    ScriptDef* def = &(m_scripts[ind]);
    m_list.InsertItem(LVIF_TEXT|LVIF_STATE, ind, def->type, 0, 0, 0, 0);
    m_list.SetItemText(ind,1,def->language);
    m_list.SetItemText(ind,2,def->deferred ? "Deferred" : "Immediate");
    m_list.SetItemText(ind,3,def->src.IsEmpty() ? def->code : def->src);
  }
  if(m_scripts.size())
  {
    m_list.SetItemState(0,LVNI_SELECTED,LVNI_SELECTED);
    m_list.SetFocus();
  }
}

void
TopicPropPage4Dlg::ShowFocus()
{
  m_list.SetFocus();
}

void
TopicPropPage4Dlg::GetHeadScripts()
{
  ResetScripts();

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
          name = L"script";
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
              CComQIPtr<IHTMLScriptElement,&IID_IHTMLScriptElement> script = idisp;
              if(SUCCEEDED(hr) && script.p)
              {
                // Yoepieee, its a script tag in the head
                CComQIPtr<IHTMLElement,&IID_IHTMLElement> elem = idisp;
                HtmlScript selem(elem);
                ScriptDef def;

                def.script   = script.p;
                def.language = selem.GetAttribute("language");
                def.type     = selem.GetAttribute("type");
                def.deferred = atoi(selem.GetAttribute("defer")) == 1;
                def.src      = selem.GetAttribute("src");
                def.forHtml  = selem.GetAttribute("for");
                def.event    = selem.GetAttribute("event");
                def.code     = selem.GetText();

                def.script->AddRef();

                m_scripts.push_back(def);
              }
            }
          }
        }
      }
    }
  }
}

// TopicPropPage4 message handlers

void TopicPropPage4Dlg::OnLvnChanged(NMHDR* /*pNMHDR*/, LRESULT *pResult)
{
  UpdateData(Data2Controls);
  *pResult = 0;
}

void TopicPropPage4Dlg::OnLvnItemDouble(NMHDR* /*pNMHDR*/, LRESULT *pResult)
{
  UpdateData(Data2Controls);
  OnBnClickedEdit();
  *pResult = 0;
}

void 
TopicPropPage4Dlg::OnBnClickedUp()
{
  int now = m_list.GetNextItem(-1,LVNI_SELECTED);
  if(now > 0)
  {
    std::swap(m_scripts[now],m_scripts[now-1]);
    ScriptsToList();
    UpdateData(Data2Controls);
    m_list.SetItemState(now-1,LVNI_SELECTED,LVNI_SELECTED);
  }
  m_list.SetFocus();
}

void 
TopicPropPage4Dlg::OnBnClickedEdit()
{
  int num = m_list.GetItemCount();
  int now = m_list.GetNextItem(-1,LVNI_SELECTED);
  if(now < num)
  {
    ScriptDef* def = &(m_scripts[now]);
    CComQIPtr<IHTMLElement,&IID_IHTMLElement> elem = def->script;
    HtmlScript script(elem);
    ScriptDlg dlg(this,&script,m_base);
    if(dlg.DoModal() == IDOK)
    {
      FillPage();
      UpdateData(Data2Controls);
    }
  }
  m_list.SetFocus();
}

void 
TopicPropPage4Dlg::OnBnClickedNew()
{
  HtmlScript script(Misc::CreateHeadElement(m_htmlDoc,TAGID_SCRIPT));
  ScriptDlg dlg(this,&script,m_base);
  if(dlg.DoModal() == IDOK)
  {
    FillPage();
    UpdateData(Data2Controls);
  }
  else
  {
    script.Remove();
  }
  m_list.SetFocus();
}

void 
TopicPropPage4Dlg::OnBnClickedDelete()
{
  int num = m_list.GetItemCount();
  int now = m_list.GetNextItem(-1,LVNI_SELECTED);
  if(now < num)
  {
    CString mess;
    CString script = m_list.GetItemText(now,3);
    mess.Format("Do you want to delete the following script?\n\n%s",script.GetString());
    if(theApp.MessageBox(mess,"Delete?",MB_YESNO|MB_ICONQUESTION) == IDYES)
    {
      ScriptDef* def = &(m_scripts[now]);
      CComQIPtr<IHTMLDOMNode,&IID_IHTMLDOMNode> dom = def->script;
      HRESULT hr = dom->removeNode(VARIANT_TRUE,NULL);
      if(SUCCEEDED(hr))
      {
        ScriptVector::iterator it = m_scripts.begin();
        while(now)
        {
          ++it;
          --now;
        }
        // Release the DOM pointer
        def->script->Release();
        // Erase from vector scripts
        m_scripts.erase(it);
        // Rebuild visual list
        ScriptsToList();
        UpdateData(Data2Controls);
      }
    }
  }
  m_list.SetFocus();
}

void 
TopicPropPage4Dlg::OnBnClickedDown()
{
  int num = m_list.GetItemCount();
  int now = m_list.GetNextItem(-1,LVNI_SELECTED);
  if(now < (num -1))
  {
    std::swap(m_scripts[now],m_scripts[now+1]);
    ScriptsToList();
    UpdateData(Data2Controls);
    m_list.SetItemState(now+1,LVNI_SELECTED,LVNI_SELECTED);
  }
  m_list.SetFocus();
}
