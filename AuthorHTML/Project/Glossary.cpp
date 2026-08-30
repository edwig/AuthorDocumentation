//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    Glossary.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Registration of payload files in the CHM
//              Mostly images and stylesheets
//
#include <StdAfx.h>
#include "Glossary.h"
#include "Misc.h"

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
  p_filename.TrimRight(".hhp");
  p_filename += ".glo";
  m_filename = p_filename;

  FILE* file = fopen(p_filename.GetString(),"r");
  if(file)
  {
    CString name;
    CString desc;
    char buffer[1024];

    Misc::SkipBOM(file);

    while(fgets(buffer,1024,file))
    {
      int len = (int) strlen(buffer);
      if(len && buffer[len-1] == '\n')
      {
        buffer[--len] = 0;
      }
      if(_strnicmp(buffer,"NAME=",5) == 0)
      {
        if(!name.IsEmpty() && !desc.IsEmpty())
        {
          AddToGlossary(name,desc);
        }
        // New defintion
        name = &buffer[5];
        desc = "";
      }
      else
      {
        // Part of the description
        if(!desc.IsEmpty()) desc += "\n";
        desc += buffer;
      }
    }
    AddToGlossary(name,desc);
    fclose(file);
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
  FILE* file = fopen(m_filename.GetString(),"w");
  if(file)
  {
    GlossaryMap::iterator it = m_glos.begin();
    while(it != m_glos.end())
    {
      CString name = it->first;
      CString desc = it->second;
      // Some programs like RoboHelp will crash if glossary 'NAME' is not in uppercase!!
      fprintf(file,"NAME=%s\n",(LPCTSTR)name);
      fprintf(file,desc);
      if(desc[((int)strlen(desc))-1] != '\n')
      {
        fputc('\n',file);
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
