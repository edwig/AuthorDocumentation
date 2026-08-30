//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    SettingsManager.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Used to read/write to the MS-Windows registry
//
#include "StdAfx.h"
#include "AuthorHTML.h"
#include "SettingsManager.h"

#define BUFF_LEN 512

SettingsManager::SettingsManager()
{
  ReadSettings();
}

SettingsManager::~SettingsManager()
{
  WriteSettings();
  m_allSettings.clear();
}

// Read settings from the registry
void
SettingsManager::ReadSettings()
{
  HKEY  hkUserURL;
  DWORD dwIndex = 0;
  DWORD dwType  = 0;
  TCHAR buffName[BUFF_LEN];
  BYTE  buffData[BUFF_LEN];
  DWORD dwNameSize;
  DWORD dwDataSize;

  DWORD dwErr = RegOpenKeyEx(HKEY_CURRENT_USER
                            ,_T("Software\\" REGISTER_APP "\\AuthorHTML\\Settings")
                            ,0
                            ,KEY_QUERY_VALUE
                            ,&hkUserURL);
  if(dwErr == ERROR_SUCCESS)
  {
    //enumerate this key's values
    while(ERROR_SUCCESS == dwErr)
    {
      dwNameSize = BUFF_LEN;
      dwDataSize = BUFF_LEN;
      dwErr = RegEnumValue(hkUserURL
                          ,dwIndex++
                          ,buffName
                          ,&dwNameSize
                          ,NULL
                          ,&dwType
                          ,buffData
                          ,&dwDataSize);
      if(dwErr == ERROR_SUCCESS && dwType == REG_SZ)
      {
        // Add to settings map
        CString setting = buffName;
        CString value   = buffData;
        m_allSettings.insert(std::make_pair(setting,value));
      }
    }
    RegCloseKey(hkUserURL);
  }
  else
  {
    TRACE(_T("Couldn't open registry settings key for read\n"));
  }
}

// Dump settings to the registry
void
SettingsManager::WriteSettings()
{
  HKEY  hkUserURL;

  DWORD dwErr = RegOpenKeyEx(HKEY_CURRENT_USER
                            ,_T("Software\\" REGISTER_APP "\\AuthorHTML\\Settings")
                            ,0
                            ,KEY_SET_VALUE
                            ,&hkUserURL);
  if(dwErr == ERROR_SUCCESS)
  {
    CString setting;
    CString value;
    SettingsMap::iterator it;
    
    for(it = m_allSettings.begin(); it!= m_allSettings.end(); ++it)
    {
      setting = it->first;
      value   = it->second;
      BYTE  buffData[BUFF_LEN];
      strncpy_s((char*)buffData,BUFF_LEN,value,BUFF_LEN);
      dwErr = RegSetValueEx(hkUserURL
                           ,setting
                           ,0
                           ,REG_SZ
                           ,buffData
                           ,(DWORD)strlen(value));
    }
    RegCloseKey(hkUserURL);
  }
  else
  {
    TRACE(_T("Couldn't open registry settings key for write\n"));
  }
}

// Search for a setting and return it
CString
SettingsManager::GetSetting(CString setting,CString defValue)
{
  SettingsMap::const_iterator it = m_allSettings.find(setting);
  if(it != m_allSettings.end())
  {
    return it->second;
  }
  return defValue;
}

// Store a setting
void
SettingsManager::SetSetting(CString setting,CString value)
{
  SettingsMap::iterator it;
  it = m_allSettings.find(setting);
  if(it != m_allSettings.end())
  {
    m_allSettings.erase(it);
  }
  m_allSettings.insert(std::make_pair(setting,value));
}

bool
SettingsManager::ChangeCHMRestrictions(int p_allow, bool p_64bit)
{
  HKEY  hkUserURL;
  DWORD dwErr = RegOpenKeyEx(HKEY_LOCAL_MACHINE
                            ,_T("Software\\Microsoft\\HTMLHelp\\1.x\\HHRestrictions")
                            ,0
                            ,KEY_READ | KEY_WRITE | (p_64bit ? KEY_WOW64_64KEY : KEY_WOW64_32KEY)
                            ,&hkUserURL);
  if(dwErr == ERROR_SUCCESS)
  {
    dwErr = RegSetValueEx(hkUserURL
                         ,"MaxAllowedZone"
                         ,0
                         ,REG_DWORD
                         ,(const BYTE*) &p_allow
                         ,sizeof(DWORD));
    RegCloseKey(hkUserURL);
    return true;
  }
  TRACE(_T("Couldn't open registry CHM HTML key for write\n"));
  return false;
}

