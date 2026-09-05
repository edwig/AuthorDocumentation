//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    Glossary.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Registration of the glossary entries in a Microsoft HTML Help project
//
#include <StdAfx.h>
#include "Glossary.h"
#include "Misc.h"
#include "Version.h"

Glossary::Glossary()
{
}

Glossary::~Glossary()
{
  m_glos.clear();
}

bool
Glossary::ReadFromFile(CString p_filename)
{
  p_filename.MakeLower();
  p_filename.TrimRight(_T(".hhp"));
  p_filename += _T(".glo");
  m_filename = p_filename;

  WinFile file(p_filename.GetString());
  if(file.Open(winfile_read | open_trans_text))
  {
    CString name;
    CString desc;
    XString buffer;

    while(file.Read(buffer))
    {
      int len = buffer.GetLength();
      if(len && buffer.GetAt(len - 1) == _T('\n'))
      {
        buffer = buffer.Left(len - 1);
      }
      if(buffer.Left(5).CompareNoCase(_T("NAME=")) == 0)
      {
        if(!name.IsEmpty() && !desc.IsEmpty())
        {
          AddToGlossary(name,desc);
        }
        // New definition
        name = buffer.Mid(5).Trim();
        desc.Empty();
      }
      else
      {
        // Part of the description
        if(!desc.IsEmpty())
        {
          desc += _T("\n");
        }
        desc += buffer;
      }
    }
    AddToGlossary(name,desc);
    file.Close();
    return true;
  }
  return false;
}

bool
Glossary::WriteToFile()
{
  if(m_glos.size() == 0)
  {
    // Nothing to be done
    return true;
  }
  WinFile file(m_filename.GetString());
  if(file.Open(winfile_write | open_trans_text,attrib_none,AUTHOR_HTML_ENCODING))
  {
    GlossaryMap::iterator it = m_glos.begin();
    while(it != m_glos.end())
    {
      CString name = it->first;
      XString desc = it->second;

      // Some programs like RoboHelp will crash if glossary 'NAME' is not in uppercase!!
      file.Format(_T("NAME=%s\n"),name.GetString());
      file.Write(desc);
      if(desc.GetLength() > 0 && desc.GetAt(desc.GetLength() - 1) != _T('\n'))
      {
        file.Write(_T("\n"));
      }
      ++it;
    }
    return true;
  }
  return false;
}

bool 
Glossary::AddToGlossary(CString p_name,CString p_description)
{
  if(p_name.IsEmpty())
  {
    // Name without description is possible
    // Description without name is not possible
    return false;
  }
  if(m_glos.find(p_name) == m_glos.end())
  {
    m_glos.insert(std::make_pair(p_name,p_description));
    return true;
  }
  return false;
}

bool
Glossary::UpdateGlossary(CString p_name,CString p_description)
{
  GlossaryMap::iterator  it = m_glos.find(p_name);
  if(it != m_glos.end())
  {
    it->second = p_description;
    return true;
  }
  return AddToGlossary(p_name,p_description);
}

bool
Glossary::RemoveFromGlossary(CString p_name,CString /*p_description*/)
{
  GlossaryMap::iterator  it = m_glos.find(p_name);
  if(it != m_glos.end())
  {
    m_glos.erase(it);
    return true;
  }
  return false;
}
