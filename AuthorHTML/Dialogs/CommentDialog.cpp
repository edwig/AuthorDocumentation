//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    CommentDialog.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog to edit the <COMMENT> tag in the HTML file. 
//              The comment is not visible in the browser, 
//              but it is visible in the HTML source code
//
#include "stdafx.h"
#include "AuthorHTML.h"
#include "CommentDialog.h"
#include "TableProperties.h"

// CommentDialog dialog

IMPLEMENT_DYNAMIC(CommentDialog, CDialog)
CommentDialog::CommentDialog(CString p_text
                            ,CWnd* pParent /*=NULL*/)
	            :CDialog(CommentDialog::IDD, pParent)
              ,m_text(p_text)
              ,m_parent(pParent)
{
}

CommentDialog::~CommentDialog()
{
}

void CommentDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
  DDX_Text   (pDX,IDC_EDIT_COMMENT, m_text);
  DDX_Control(pDX,IDOK,             m_buttonOK);
  DDX_Control(pDX,IDCANCEL,         m_buttonCancel);
  DDX_Control(pDX,IDC_COMMENT_HELP, m_buttonHelp);
}

BEGIN_MESSAGE_MAP(CommentDialog, CDialog)
  ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()

// CommentDialog message handlers

void CommentDialog::OnBnClickedOk()
{
  UpdateData(Controls2Data);
  OnOK();
}
