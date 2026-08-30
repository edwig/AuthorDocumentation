//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    CheckWorkshop.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Device-Independent Bitmap
//
#pragma once

#define DEFAULT_HTMLWORKSHOP_PATH "C:\\Program Files (x86)\\HTML Help Workshop"
#define DEFAULT_WORKSHOP_FOLDER   "\\HTML Help Workshop"

typedef enum _hhZone
{
  HHZone_Local     = 1   // The default
 ,HHZone_Intranet  = 2
 ,HHZone_Internet  = 3
 ,HHZone_Untrusted = 4
} 
HHZone;

class CheckWorkshop
{
public:
  CheckWorkshop();
  ~CheckWorkshop();

  bool    CheckHTMLHelpWorkshop();
  CString HTMLHelpWorkshopPath();
  bool    SetHHZoneRestriction(HHZone p_zone);

private:
  bool CheckExists(CString p_path);
  bool CheckPathExists(CString p_path);
  void Improper();

  // Our goal to find
  CString m_workshopPath;
};
