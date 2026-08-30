//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    GetURL.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for managing the URL
//
#include "stdafx.h"
#include "AuthorHTML.h"
#include "GetURL.h"
#include "FileDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGetURL dialog


CGetURL::CGetURL(CWnd* pParent /*=NULL*/)
	: CDialog(CGetURL::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGetURL)
	m_URL = _T("");
	//}}AFX_DATA_INIT
}


void CGetURL::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGetURL)

	//}}AFX_DATA_MAP
	if(::IsWindow(m_AddrCombo.m_hWnd))
		DDX_Text(pDX, IDC_ADDRCOMBO, m_URL);
}


BEGIN_MESSAGE_MAP(CGetURL, CDialog)
	//{{AFX_MSG_MAP(CGetURL)
	//}}AFX_MSG_MAP
	ON_CONTROL(CBN_CLOSEUP,IDC_ADDRCOMBO,OnCloseup)
	ON_CONTROL(BN_CLICKED,IDC_BTNGO,OnGo)
  ON_BN_CLICKED(IDC_BUTTON_OPEN, OnBnClickedButtonOpen)
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CGetURL, CDialog)
	ON_EVENT(CGetURL, IDC_EXPLORER, 0x00000103, OnDocumentComplete, VTS_DISPATCH VTS_VARIANT)
END_EVENTSINK_MAP()
/////////////////////////////////////////////////////////////////////////////
// CGetURL message handlers
void CGetURL::OnDocumentComplete(LPDISPATCH /*pDisp*/, LPVARIANT pURL)
{
	CString szURL(COLE2T(pURL->pvarVal->bstrVal));
	if(::IsWindow(m_AddrCombo.m_hWnd))
	{
		m_AddrCombo.SetWindowText(szURL);
		m_szCurAddr = szURL;
	}
	
}
void CGetURL::OnOK() 
{
	UpdateData();
	CDialog::OnOK();
}

BOOL CGetURL::OnInitDialog() 
{
	CDialog::OnInitDialog();
	IUnknown *pUnk;
	
	CWnd *pWnd = GetDlgItem(IDC_EXPLORER);
	pUnk = NULL;
	if(pWnd)
		m_spBrowser = pWnd->GetControlUnknown();

	pWnd = NULL;
	CRect rcItem;
	pWnd = GetDlgItem(IDC_PHSTATIC);
	if(pWnd)
	{
		pWnd->GetClientRect(rcItem);
		pWnd->ClientToScreen(rcItem);
		pWnd->DestroyWindow();
		ScreenToClient(rcItem);
		rcItem.bottom += 150;
		if(m_AddrCombo.Create(WS_VSCROLL|WS_CHILD|WS_VISIBLE|CBS_DROPDOWN|CBS_AUTOHSCROLL,rcItem,this,IDC_ADDRCOMBO))
		{
			m_AddrCombo.SetCurSel(0);
			m_AddrCombo.GetWindowText(m_szCurAddr);
			if(m_spBrowser && m_szCurAddr.GetLength())
				m_spBrowser->Navigate(m_szCurAddr.AllocSysString(),NULL,NULL,NULL,NULL);
		}
	}
	return TRUE;
}

void CGetURL::OnCloseup()
{	
	int nSel = m_AddrCombo.GetCurSel();
	if(CB_ERR != nSel)
	{
		m_AddrCombo.GetLBText(nSel,m_szCurAddr);
		if(m_spBrowser)
			m_spBrowser->Navigate(m_szCurAddr.AllocSysString(),NULL,NULL,NULL,NULL);

	}
}

void CGetURL::OnGo()
{
	m_AddrCombo.GetWindowText(m_szCurAddr);
	if(m_spBrowser)
		m_spBrowser->Navigate(m_szCurAddr.AllocSysString(),NULL,NULL,NULL,NULL);

}

void CGetURL::OnBnClickedButtonOpen()
{
  DocFileDialog diag(true
                     ,"Open a documentation page"
                     ,"htm"
                     ,""
                     ,0
                     ,"Documentation files (*.htm, *.html)|*.htm;*.html|All files|*.*");
  if(diag.DoModal() == IDOK)
  {
    CString file = diag.GetChosenFile();
    int num = m_AddrCombo.AddString(file);
    m_AddrCombo.SetCurSel(num);
    OnGo();
  }
}
