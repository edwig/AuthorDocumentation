//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    AddressCombo.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Combobox with the history of the addresses of documentation files
//              used in the AuthorHTML application
//
#include "stdafx.h"
#include "AuthorHTML.h"
#include "AddressCombo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAddressCombo

CAddressCombo::CAddressCombo()
{
}

CAddressCombo::~CAddressCombo()
{
}


BEGIN_MESSAGE_MAP(CAddressCombo, CComboBox)
	//{{AFX_MSG_MAP(CAddressCombo)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAddressCombo message handlers
#define BUFF_LEN 512
void CAddressCombo::FillWithHistory()
{
	HKEY hkUserURL;
	DWORD dwIndex = 0, dwType;
	TCHAR buffName[BUFF_LEN];
	BYTE buffData[BUFF_LEN];
	DWORD dwNameSize,dwDataSize;
	DWORD dwErr = ERROR_SUCCESS;
	if(ERROR_SUCCESS == RegOpenKeyEx(HKEY_CURRENT_USER,_T("Software\\" REGISTER_APP "\\AuthorHTML\\Recent File List"),0,KEY_QUERY_VALUE,&hkUserURL))
	{
		//enumerate this key's values
		while(ERROR_SUCCESS == dwErr)
		{
			dwNameSize = BUFF_LEN;
			dwDataSize = BUFF_LEN;
			dwErr = RegEnumValue(hkUserURL,dwIndex++,buffName,&dwNameSize,NULL,
								&dwType,buffData,&dwDataSize);
			if(dwErr == ERROR_SUCCESS)
			{
				AddString((TCHAR*)buffData);
			}
		}
		RegCloseKey(hkUserURL);
	}
	else
  {
    TRACE(_T("Couldn't open registry key\n"));
  }
}

int CAddressCombo::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CComboBox::OnCreate(lpCreateStruct) == -1)
  {
    return -1;
  }
	//try for courier, if that fails, go for the ANSI fixed font
	if(!m_font.CreateFont(-MulDiv(10, GetDeviceCaps(GetDC()->m_hDC, LOGPIXELSY), 72)
			,0,0,0,FW_DONTCARE,0,0,0,ANSI_CHARSET,OUT_TT_PRECIS,CLIP_DEFAULT_PRECIS
			,DEFAULT_QUALITY,DEFAULT_PITCH,_T("TAHOMA")))	
	{
		m_font.CreateStockObject(ANSI_FIXED_FONT);
	}
	SetFont(&m_font);
	FillWithHistory();
	
	return 0;
}
