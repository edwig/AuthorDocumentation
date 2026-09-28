//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    AboutDlg.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Showing the about information of the program
//
#include "StdAfx.h"
#include "AboutDlg.h"
#include "Version.h"

AboutDlg::AboutDlg() 
         :CDialog(IDD_ABOUTBOX)
{
}

void 
AboutDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Control(pDX,IDC_TEXT,m_edit);
}

BEGIN_MESSAGE_MAP(AboutDlg, CDialog)
END_MESSAGE_MAP()

BOOL
AboutDlg::OnInitDialog()
{
  CDialog::OnInitDialog();
  m_text = _T("{\\rtf1\\ansi\\ansicpg1252\\deff0\\deflang1043{\\fonttbl{\\f0\\fnil\\fprq1\\fcharset238 r_eeurope;}{\\f1\\fnil\\fcharset0 Calibri;}}\n")
           _T("{\\colortbl ;\\red163\\green21\\blue21;}\n")
           _T("\\viewkind4\\uc1\\pard\\sl240\\slmult1\\cf1\\b\\f0\\fs20 AUTHOR DOCUMENTATION\\par\\par\n")
           _T("\\b0 A authoring tool for writing of\\par on-line documentation. Contains\\par\n")
           _T("complete single-source genera-\\par tion of on-line help systems.\\par\n")
           _T("\\par")
           _T("\\b Active subsystems:\\par\n")
           _T("\\b0 - CSS Parser\\par\n")
           _T("- XML Parser\\par\n")
           _T("- HTML 4.1 cleaner\\par\n")
           _T("- Microsoft CHM compiler\\par\n")
           _T("- Scintilla editor\\par\n")
           _T("- Skinning engine\\par\n")
           _T("\\par")
           _T("\\b Version: \\b0 ") VERSION_NUMBER _T("\\cf0\\par\n")
           _T("\\cf1\\b Build: \\f1      \\b0\\f0 ") STRINGIZE(BUILD_NUMBER) _T("\\par\n")
           _T("\\cf0\\par\n")
           _T("\\cf1\\b Dates: \\b0 ") VERSION_DATES _T("\\par\n")
           _T("Written by: ir. W.E. Huisman\\cf0\\lang19\\f1\\fs22\\par");
        
  m_edit.SetFontname(1,_T("Verdana"));
  m_edit.SetFontsize(1,8);
  m_edit.SetColor(1,RGB(163,21,21));
  m_edit.SetRTFText(m_text);
  UpdateData(FALSE);
  return FALSE;
}
