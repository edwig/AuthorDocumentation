//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    BookmarkDialog.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog to edit the bookmarks in the HTML file. 
//              Bookmarks are used to link to a specific location in a document
//
#include "stdafx.h"
#include "AuthorHTML.h"
#include "BookmarkDialog.h"
#include "Misc.h"
#include "TableProperties.h"

// BookmarkDialog dialog

IMPLEMENT_DYNAMIC(BookmarkDialog, CDialog)
BookmarkDialog::BookmarkDialog(CString currentMarker
                              ,CString newMarker
                              ,CComPtr<IHTMLDocument2> doc
                              ,CWnd*   pParent /*=NULL*/)
	             :CDialog(BookmarkDialog::IDD, pParent)
               ,m_currentMarker(currentMarker)
               ,m_newBookmark(newMarker)
               ,m_doc(doc)
               ,m_pParent((CHTMLEdView*)pParent)
{
}

BookmarkDialog::~BookmarkDialog()
{
}

void BookmarkDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
  DDX_Text   (pDX,IDC_EDIT_BOOKMARK,  m_newBookmark);
  DDX_Control(pDX,IDC_LIST_BOOKMARKS, m_listBookmarks);
  DDX_Control(pDX,IDC_SET_BOOKMARK,   m_buttonSetMark);
  DDX_Control(pDX,IDC_MOVETO_BOOKMARK,m_buttonMoveTo);
  DDX_Control(pDX,IDC_REMOVE_BOOKMARK,m_buttonRemove);
  DDX_Control(pDX,IDOK,               m_buttonOK);
  DDX_Control(pDX,IDCANCEL,           m_buttonCancel);
  DDX_Control(pDX,IDC_HELP_BOOKMARK,  m_buttonHelp);
}

BEGIN_MESSAGE_MAP(BookmarkDialog, CDialog)
  ON_EN_KILLFOCUS(IDC_EDIT_BOOKMARK,    OnEnChangeEditBookmark)
  ON_LBN_SELCHANGE(IDC_LIST_BOOKMARKS,  OnLbnSelchangeListBookmarks)
  ON_BN_CLICKED(IDC_SET_BOOKMARK,       OnBnClickedSetBookmark)
  ON_BN_CLICKED(IDC_MOVETOBOOKMARK,     OnBnClickedMovetobookmark)
  ON_BN_CLICKED(IDC_REMOVE_BOOKMARK,    OnBnClickedRemoveBookmark)
  ON_BN_CLICKED(IDOK,                   OnBnClickedOk)
  ON_BN_CLICKED(IDC_HELP_BOOKMARK,      OnBnClickedHelp)
END_MESSAGE_MAP()

BOOL
BookmarkDialog::OnInitDialog()
{
  CDialog::OnInitDialog();
  InitList();
  return TRUE;
}

void
BookmarkDialog::InitList()
{
  int listIndex = 0;
  int currIndex = 0;

  m_listBookmarks.ResetContent();
  CComPtr<IHTMLElementCollection> coll;
  HRESULT hr = m_doc->get_anchors(&coll);
  if(SUCCEEDED(hr))
  {
    long num = 0;
    coll->get_length(&num);
    for(int index = 0; index < num; ++index)
    {
      CComVariant object;
      V_VT(&object) = VT_I4;
      V_I4(&object) = index;
      CComPtr<IDispatch> disp;
      hr = coll->item(object,object,&disp);
      if(SUCCEEDED(hr))
      {
        CComBSTR attrib = L"name";
        CComVariant  value;
        V_VT(&value) = VT_BSTR;
        CComQIPtr<IHTMLElement,&IID_IHTMLElement> elem = disp;
        elem->getAttribute(attrib,0,&value);
        CString anchorName = V_BSTR(&value);
        if(!anchorName.IsEmpty())
        {
          m_listBookmarks.AddString(anchorName);
          if(anchorName.CompareNoCase(m_currentMarker) == 0)
          {
            currIndex = listIndex;
          }
          ++listIndex;
        }
      }
    }
  }
  // Find it in the list
  if(!m_currentMarker.IsEmpty())
  {
    m_listBookmarks.SetCurSel(currIndex);

    // No new bookmark
    m_buttonSetMark.EnableWindow(FALSE);
    CWnd* wnd = GetDlgItem(IDC_EDIT_BOOKMARK);
    wnd->EnableWindow(FALSE);
  }
}

// BookmarkDialog message handlers

void BookmarkDialog::OnEnChangeEditBookmark()
{
  UpdateData(Controls2Data);
  m_newBookmark.Remove(' ');
  UpdateData(Data2Controls);
}

void BookmarkDialog::OnLbnSelchangeListBookmarks()
{
  CString current;
  int index = m_listBookmarks.GetCurSel();
  m_listBookmarks.GetText(index,current);
  if(!current.IsEmpty())
  {
    m_currentMarker = current;
  }
}

void BookmarkDialog::OnBnClickedRemoveBookmark()
{
  if(!m_currentMarker.IsEmpty())
  {
    // Move to the bookmark
    CComPtr<IHTMLElement> pElem;
    FindBookmarkElement(m_currentMarker,pElem);
    Misc::MoveCaretToElement(m_doc
                            ,pElem
                            ,CARET_DIRECTION_INDETERMINATE
                            ,true);
    // Friendly ask our user
    CString mess;
    mess.Format("Remove bookmark [#%s] from text?",m_currentMarker.GetString());
    if(theApp.MessageBox(mess,"Question",MB_YESNO|MB_ICONQUESTION) == IDYES)
    {
      // Find in the list
      int index = m_listBookmarks.FindString(-1,m_currentMarker);
      if(index >= 0)
      {
        m_listBookmarks.DeleteString(index);
      }
      // Remove from document
      m_pParent->ExecCommand(&CGID_MSHTML,IDM_UNBOOKMARK,NULL,NULL);
    }
  }
}

void 
BookmarkDialog::OnBnClickedMovetobookmark()
{
  if(!m_currentMarker.IsEmpty())
  {
    CComPtr<IHTMLElement> pElem;
    FindBookmarkElement(m_currentMarker,pElem);
    Misc::MoveCaretToElement(m_doc
                            ,pElem
                            ,CARET_DIRECTION_INDETERMINATE
                            ,true);
  }
}

void 
BookmarkDialog::OnBnClickedSetBookmark()
{
  CComPtr<IHTMLElement> elem;
  FindBookmarkElement(m_newBookmark,elem);
  if(elem.p)
  {
    // Found it. It already exists
    CString text;
    text.Format("The bookmark [#%s] does already exist on this page. Change the bookmark name",m_newBookmark.GetString());
    theApp.MessageBox(text,"Warning",MB_OK|MB_ICONEXCLAMATION);
    return;
  }
  CComVariant  var;
  CComBSTR mark = m_newBookmark;
	V_VT(&var)   = VT_BSTR;
	V_BSTR(&var) = mark;
  m_pParent->ExecCommand(&CGID_MSHTML
                        ,IDM_BOOKMARK
                        ,MSOCMDEXECOPT_DODEFAULT
                        ,&var
                        ,NULL);
  // Set only once
  m_currentMarker = m_newBookmark;
  m_newBookmark = "";
  // Set it in the list
  InitList();
}

void BookmarkDialog::OnBnClickedOk()
{
  if(!m_newBookmark.IsEmpty())
  {
    OnBnClickedSetBookmark();
  }
  OnOK();
}

void
BookmarkDialog::OnBnClickedHelp()
{
  OnHelp();
}

/////////////////////////////////////////
//
// INTERNALS
//
/////////////////////////////////////////

void 
BookmarkDialog::FindBookmarkElement(CString pName,CComPtr<IHTMLElement> &pElem)
{
  CComPtr<IHTMLElementCollection> coll;
  HRESULT hr = m_doc->get_anchors(&coll);
  if(SUCCEEDED(hr))
  {
    long num = 0;
    coll->get_length(&num);
    for(int index = 0; index < num; ++index)
    {
      CComVariant object;
      V_VT(&object) = VT_I4;
      V_I4(&object) = index;
      CComPtr<IDispatch> disp;
      hr = coll->item(object,object,&disp);
      if(SUCCEEDED(hr))
      {
        CComBSTR attrib = L"name";
        CComVariant  value;
        V_VT(&value) = VT_BSTR;
        CComQIPtr<IHTMLElement,&IID_IHTMLElement> elem = disp;
        elem->getAttribute(attrib,0,&value);
        CString anchorName = V_BSTR(&value);
        if(!anchorName.IsEmpty())
        {
          if(anchorName.CompareNoCase(pName) == 0)
          {
            pElem = elem;
          }
        }
      }
    }
  }
}
