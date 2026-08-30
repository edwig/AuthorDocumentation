//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    SettingsManager.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Used to read/write to the MS-Windows registry
//
#pragma once
#include "StdAfx.h"
#include <map>

typedef std::map<CString,CString> SettingsMap;

class SettingsManager
{
public:
   SettingsManager();
  ~SettingsManager();
   CString GetSetting(CString setting,CString defValue);
   void    SetSetting(CString setting,CString value);

   bool    ChangeCHMRestrictions(int p_allow,bool p_64bit);
private:
   void ReadSettings();
   void WriteSettings();

   SettingsMap m_allSettings;
};