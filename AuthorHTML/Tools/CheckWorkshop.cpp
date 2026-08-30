//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    CheckWorkshop.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Device-Independent Bitmap
//
#include "StdAfx.h"
#include "CheckWorkshop.h"
#include "AuthorHTML.h"
#include <WideMessageBox.h>
#include <MapDialog.h>

CheckWorkshop::CheckWorkshop()
{
  m_workshopPath = DEFAULT_HTMLWORKSHOP_PATH;
}

CheckWorkshop::~CheckWorkshop()
{
}

CString 
CheckWorkshop::HTMLHelpWorkshopPath() 
{
  return m_workshopPath;
}

bool
CheckWorkshop::CheckHTMLHelpWorkshop()
{
  // STEP 1: Check the default path for HTML Workshop
  if (CheckExists(m_workshopPath))
  {
    return true;
  }

  // STEP 2: Check the %windir% environment variable for HTML Workshop
  if(m_workshopPath.GetEnvironmentVariable("windir"))
  {
    m_workshopPath += DEFAULT_WORKSHOP_FOLDER;
    if(CheckExists(m_workshopPath))
    {
      return true;
    }
  }

  // STEP 3: Check our settings for a previously stored path to HTML Workshop
  m_workshopPath = settings.GetSetting("HTMLHelpWorkshopPath","");
  if(!m_workshopPath.IsEmpty())
  {
    if(CheckExists(m_workshopPath))
    {
      return true;
    }
  }

  // STEP 4: Ask for a directory where HTML Workshop is installed
  CString askPath = "Please select the folder where 'HTML Help Workshop' is installed (Say: YES)\n"
                    "OR stop here, and install Microsoft HTML Help Workshop by hand. (Say: NO)";
  if(WideMessageBox(nullptr,askPath,_T("NOT INSTALLED"),MB_YESNO | MB_DEFBUTTON2 | MB_ICONWARNING) == IDYES)
  {
    MapDialog dlg;
    if(dlg.Browse(nullptr,"Select the HTML Help Workshop folder"))
    {
      m_workshopPath = dlg.GetPath();
      if(CheckExists(m_workshopPath))
      {
        settings.SetSetting("HTMLHelpWorkshopPath",m_workshopPath);
        return true;
      }
    }
  }
  // STEP 5: If not found: Tell the user that HTML Workshop is not installed 
  WideMessageBox(nullptr, "The selected directory does not contain HTML Help Workshop.", _T("NOT FOUND"), MB_OK | MB_ICONERROR);
  return false;
}

//////////////////////////////////////////////////////////////////////////
//
// PRIVATE 
//
//////////////////////////////////////////////////////////////////////////

bool 
CheckWorkshop::SetHHZoneRestriction(HHZone p_zone)
{
  bool result = true;
  // HKLM\SOFTWARE\Microsoft\HTML Help Workshop\1.x\HHRestrictions\MaxAllowedZone   
  // Set the MaxAllowedZone to 3 (Internet) or 4 (Untrusted)
  if (!settings.ChangeCHMRestrictions(p_zone,false))
  {
    result = false;
  }
  if (!settings.ChangeCHMRestrictions(p_zone,true))
  {
    result = false;
  }
  return result;
}

bool
CheckWorkshop::CheckExists(CString p_path)
{
  // STEP 1: Check if the directory exists
  if(!CheckPathExists(p_path))
  {
    return false;
  }

  // STEP 2: Check if the HTML Workshop executable exists in the directory
  if(!CheckPathExists(p_path + "\\hhc.exe"))
  {
    Improper();
    return false;
  }

  // STEP 3: Check if the HHA.DLL file exists in the directory
  if(!CheckPathExists(p_path + "\\hha.dll"))
  {
    Improper();
    return false;
  }

  // Should be good
  return true;
}

bool 
CheckWorkshop::CheckPathExists(CString p_path)
{
  // Check if the directory exists
  return ::PathFileExists(p_path);
}

void
CheckWorkshop::Improper()
{
  WideMessageBox(nullptr,_T("The selected directory does not contain a proper installation of HTML Help Workshop.")
                ,_T("IMPROPER INSTALLATION"),MB_OK | MB_ICONERROR);
}
