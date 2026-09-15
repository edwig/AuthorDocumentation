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
        :CDialog(CGetURL::IDD, pParent)
{
}

void 
CGetURL::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Control(pDX,IDC_ADDRCOMBO,m_addressCombo);
}

BEGIN_MESSAGE_MAP(CGetURL, CDialog)
  ON_CBN_CLOSEUP(IDC_ADDRCOMBO,   OnCloseup)
  ON_BN_CLICKED (IDC_BTNGO,       OnGo)
  ON_BN_CLICKED (IDC_BUTTON_OPEN, OnBnClickedButtonOpen)
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CGetURL, CDialog)
  ON_EVENT(CGetURL, IDC_EXPLORER, 0x00000103, OnDocumentComplete, VTS_DISPATCH VTS_VARIANT)
END_EVENTSINK_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGetURL message handlers

BOOL CGetURL::OnInitDialog() 
{
  CDialog::OnInitDialog();

  // Init the browser control
  CWnd *pWnd = GetDlgItem(IDC_EXPLORER);
  if(pWnd)
  {
    m_spBrowser = pWnd->GetControlUnknown();
  }
  m_addressCombo.FillWithHistory();

  FillAddressAndBrowser();

  UpdateData(Data2Controls);
  return TRUE;
}

void
CGetURL::FillAddressAndBrowser()
{
  if(m_szCurAddr.IsEmpty())
  {
    m_addressCombo.SetCurSel(-1);
  }
  else
  {
    int ind = m_addressCombo.FindString(-1,m_szCurAddr);
    if(ind < 0)
    {
      ind = m_addressCombo.AddString(m_szCurAddr);
    }
    m_addressCombo.SetCurSel(ind);
    if(m_spBrowser)
    {
      m_spBrowser->Navigate(m_szCurAddr.AllocSysString(),NULL,NULL,NULL,NULL);
    }
  }
}

void
CGetURL::OnDocumentComplete(LPDISPATCH /*pDisp*/,LPVARIANT pURL)
{
  CString szURL(COLE2T(pURL->pvarVal->bstrVal));
  if(::IsWindow(m_addressCombo.m_hWnd))
  {
    m_addressCombo.SetWindowText(szURL);
    m_szCurAddr = szURL;
    m_URL       = szURL;
  }

}
void CGetURL::OnOK()
{
  UpdateData();
  CDialog::OnOK();
}

void CGetURL::OnCloseup()
{	
  int nSel = m_addressCombo.GetCurSel();
  if(CB_ERR != nSel)
  {
    m_addressCombo.GetLBText(nSel,m_szCurAddr);
    if(m_spBrowser)
    {
      HRESULT res = m_spBrowser->Navigate(m_szCurAddr.AllocSysString(),NULL,NULL,NULL,NULL);
      if(SUCCEEDED(res))
      {
        m_URL = m_szCurAddr;
      }
    }
  }
}

void CGetURL::OnGo()
{
  m_addressCombo.GetWindowText(m_szCurAddr);
  if(m_spBrowser)
  {
    HRESULT res = m_spBrowser->Navigate(m_szCurAddr.AllocSysString(),NULL,NULL,NULL,NULL);
    if(SUCCEEDED(res))
    {
      m_URL = m_szCurAddr;
    }
  }
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
    int num = m_addressCombo.AddString(file);
    m_addressCombo.SetCurSel(num);
    OnGo();
  }
}
