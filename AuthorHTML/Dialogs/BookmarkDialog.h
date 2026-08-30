//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    BookmarkDialog.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog to edit the bookmarks in the HTML file. 
//              Bookmarks are used to link to a specific location in a document
//
#pragma once
#include "StdAfx.h"
#include "HTMLEdView.h"
#include "HTMLEdDoc.h"
#include "resource.h"

// BookmarkDialog dialog

class BookmarkDialog : public CDialog
{
	DECLARE_DYNAMIC(BookmarkDialog)

public:
	BookmarkDialog(CString currentMarker
                ,CString newMarker
                ,CComPtr<IHTMLDocument2> doc
                ,CWnd* pParent = NULL);
	virtual ~BookmarkDialog();
	BOOL     OnInitDialog();

  // Dialog Data
	enum { IDD = IDD_BOOKMARKS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  void FindBookmarkElement(CString pName,CComPtr<IHTMLElement> &pElem);
  void InitList();

  CHTMLEdView* m_pParent;
  CString   m_currentMarker;   // Started for this marker
  CString   m_newBookmark;     // Edit box newBookmark
  CListBox  m_listBookmarks;   
  AD_Button   m_buttonSetMark;
  AD_Button   m_buttonRemove;
  AD_Button   m_buttonMoveTo;
  AD_Button   m_buttonOK;
  AD_Button   m_buttonCancel;
  AD_Button   m_buttonHelp;
  CComPtr<IHTMLDocument2> m_doc;
	DECLARE_MESSAGE_MAP()

public:
  afx_msg void OnEnChangeEditBookmark();
  afx_msg void OnLbnSelchangeListBookmarks();
  afx_msg void OnBnClickedMovetobookmark();
  afx_msg void OnBnClickedOk();
  afx_msg void OnBnClickedRemoveBookmark();
  afx_msg void OnBnClickedSetBookmark();
  afx_msg void OnBnClickedHelp();
};
