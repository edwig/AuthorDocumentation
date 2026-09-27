//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    Keyword.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Definition of a keyword
//
#pragma once
#include "HtmlElement.h"
#include "resource.h"
#include "DocumentFile.h"

//          | K-Link                 | A-Link
//          ------------------------ | ----------------------
// tag      | MS-HKWD                | MS-HAID
// content  | Composite K-Link       | Composite A-Link
// API      | HH_KEYWORD_LOOKUP      | HH_ALINK_LOOKUP
// Use      | Visible for users      | Invisible for applications
//          | searchable             | Fuzzy app search

enum class KeywordType
{
  MapID,
  KLink,
  ALink
};

typedef struct _KeywordDef
{
  KeywordType m_type;       // MapID, K-Link or A-Link
  unsigned    m_mapID = 0;  // Static MAP ID
  CString     m_composite;  // Composite K-Link
  CString     m_level1;     // Map alias or first keyword
  CString     m_level2;
  CString     m_level3;
  CString     m_level4;
  CString     m_level5;
}
KeywordDef;

typedef std::vector<KeywordDef> KeywordVector;
