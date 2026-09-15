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
#include "Misc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(AddressCombo,CComboBox)

/////////////////////////////////////////////////////////////////////////////
// CddressCombo

AddressCombo::AddressCombo()
{
}

AddressCombo::~AddressCombo()
{
}

BEGIN_MESSAGE_MAP(AddressCombo, CComboBox)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// AddressCombo message handlers

#define BUFF_LEN 512

void AddressCombo::FillWithHistory()
{
  HKEY hkUserURL;
  DWORD dwIndex = 0, dwType;
  TCHAR buffName[BUFF_LEN];
  BYTE buffData[BUFF_LEN];
  DWORD dwNameSize,dwDataSize;
  DWORD dwErr = ERROR_SUCCESS;
  CString base = theApp.GetBaseDirectory();

  if(ERROR_SUCCESS == RegOpenKeyEx(HKEY_CURRENT_USER,_T("Software\\" REGISTER_APP "\\AuthorHTML\\Recent File List"),0,KEY_QUERY_VALUE,&hkUserURL))
  {
    //enumerate this key's values
    while(ERROR_SUCCESS == dwErr)
    {
      dwNameSize = BUFF_LEN;
      dwDataSize = BUFF_LEN;
      dwErr = RegEnumValue(hkUserURL,dwIndex++,buffName,&dwNameSize,NULL,&dwType,buffData,&dwDataSize);
      if(dwErr == ERROR_SUCCESS)
      {
        CString file = buffData;
        file = Misc::RemoveBasePart(base,file);
        AddString(file);
      }
    }
    RegCloseKey(hkUserURL);
  }
  else
  {
    // TRACE(_T("Couldn't open registry key\n"));
  }
}
