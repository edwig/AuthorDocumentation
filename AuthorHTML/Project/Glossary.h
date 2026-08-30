//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    Glossary.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Registration of payload files in the CHM
//              Mostly images and stylesheets
//
#pragma once
#include <map>

typedef std::map<CString,CString> GlossaryMap;

class Glossary
{
public:
   Glossary();
  ~Glossary();
   bool    HasEntries();
   CString GetFilename();
   bool    ReadFromFile(CString filename);
   bool    WriteToFile();
   bool    AddToGlossary     (CString p_name,CString p_description);
   bool    UpdateGlossary    (CString p_name,CString p_description);
   bool    RemoveFromGlossary(CString p_name,CString p_description);

private:
   CString      m_filename; // Mostly <projectname>.glo by convention
   GlossaryMap  m_glos;
};

inline bool
Glossary::HasEntries()
{
  return (m_glos.size() > 0);
}

inline CString
Glossary::GetFilename()
{
  return m_filename;
}