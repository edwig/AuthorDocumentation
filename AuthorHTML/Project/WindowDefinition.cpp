//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    WindowDefinition.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: The definition of the main CHM windo
//
#include "StdAfx.h"
#include "WindowDefinition.h"
#include "ProjectFile.h"
#include "Misc.h"

WindowDefinition::WindowDefinition(ProjectFile* p_project)
                 :m_project(p_project)
{
  Reset();
}

WindowDefinition::~WindowDefinition()
{
}

// Private part

void
WindowDefinition::Reset()
{
  m_name            = "";
  m_title           = "";
  m_HHCFile         = "";
  m_HHKFile         = "";
  m_defaultTopic    = "";
  m_homeTopic       = "";
  m_jump1Topic      = "";
  m_jump2Topic      = "";
  m_jump1Text       = "";
  m_jump2Text       = "";
  m_navigationPane  = 0L;
  m_paneWidth       = 0L;
  m_buttons         = 0L;
  m_windowStyles    = 0L;
  m_extendedStyles  = 0L;
  m_win_left        = 0;
  m_win_top         = 0;
  m_win_right       = 0;
  m_win_bottom      = 0;
  m_unknown1        = 0;
  m_hidePane        = 0;
  m_defaultPane     = 0;
  m_tabPosition     = 0;
}

// Values for m_navigationPane

bool
WindowDefinition::IsNavPaneOption(int p_option)
{
  switch(p_option)
  {
    case NAVPANE_TOC_INDEX:
    case NAVPANE_AUTOSYNCH:
    case NAVPANE_SEARCH:
    case NAVPANE_FAVOURITES:
    case NAVPANE_ADV_SEARCH:
    case NAVPANE_GLOSSARY:
    case NAVPANE_BROWSE_SEQ:
    case NAVPANE_SIZE_POS:
    case NAVPANE_SHOW_HIDE:  return true;
  }
  return false;
}

bool    
WindowDefinition::GetNavPaneOption(int p_option)
{
  if(IsNavPaneOption(p_option))
  {
    return ((m_navigationPane & p_option) != 0);
  }
  return false;
}

void    
WindowDefinition::SetNavPaneOption(int p_option,bool p_set)
{
  if(IsNavPaneOption(p_option))
  {
    if(p_set)
    {
      m_navigationPane |= p_option;
    }
    else
    {
      m_navigationPane &= ~p_option;
    }
  }
}

bool
WindowDefinition::IsButton(int p_button)
{
  switch(p_button)
  {
    case BUTTONS_HIDESHOW:
    case BUTTONS_BACK:
    case BUTTONS_FORWARD:
    case BUTTONS_STOP:
    case BUTTONS_REFRESH:
    case BUTTONS_HOME:
    case BUTTONS_OPTIONS:
    case BUTTONS_PRINT:
    case BUTTONS_LOCATE:
    case BUTTONS_JUMP1:
    case BUTTONS_JUMP2:  return true;
  }
  return false;
}

bool    
WindowDefinition::GetButton(int p_button)
{
  if(IsButton(p_button))
  {
    return ((m_buttons & p_button) != 0);
  }
  return false;
}

void
WindowDefinition::SetButton(int p_button,bool p_set)
{
  if(IsButton(p_button))
  {
    if(p_set)
    {
      m_buttons |= p_button;
    }
    else
    {
      m_buttons &= ~p_button;
    }
  }
}

bool
WindowDefinition::IsWinStyle(int p_style)
{
  switch(p_style)
  {
    case STYLES_SYSTEMMENU:
    case STYLES_MINIMIZEBOX:
    case STYLES_MAXIMIZEBOX:
    case STYLES_CLIPSIBLINGS:
    case STYLES_CLIPCHILDREN:
    case STYLES_VERTSCROLL:
    case STYLES_HORZSCROLL:
    case STYLES_POPUP:
    case STYLES_MINIMIZE:
    case STYLES_MAXIMIZE:
    case STYLES_BORDER:
    case STYLES_DLGFRAME:
    case STYLES_VISIBLE:
    case STYLES_THICK:
    case STYLES_DISABLED: return true;
  }
  return false;
}

bool    
WindowDefinition::GetWinStyle(int p_style)
{
  if(IsWinStyle(p_style))
  {
    return ((m_windowStyles & p_style) != 0);
  }
  return false;
}

void    
WindowDefinition::SetWinStyle(int p_style,bool p_set)
{
  if(IsWinStyle(p_style))
  {
    if(p_set)
    {
      m_windowStyles |= p_style;
    }
    else
    {
      m_windowStyles &= ~p_style;
    }
  }
}

bool
WindowDefinition::IsWinExStyle(int p_style)
{
  switch(p_style)
  {
    case STYLES_TOOLWIN:
    case STYLES_CLIENTEDGE:
    case STYLES_STATICEDGE:
    case STYLES_TRANSPARENT:
    case STYLES_ACCEPTFILES:
    case STYLES_CONTROLPAR:
    case STYLES_CONTEXTHELP:
    case STYLES_RTL:
    case STYLES_NOPARNOTIFY:
    case STYLES_RIGHTALIGNED:
    case STYLES_LEFTSCROLL:
    case STYLES_TOPMOST:      return true;
  }
  return false;
}

bool    
WindowDefinition::GetWinExStyle(int p_style)
{
  if(IsWinExStyle(p_style))
  {
    return ((m_extendedStyles & p_style) != 0);
  }
  return false;
}

void    
WindowDefinition::SetWinExStyle(int p_style,bool p_set)
{
  if(IsWinExStyle(p_style))
  {
    if(p_set)
    {
      m_extendedStyles |= p_style;
    }
    else
    {
      m_extendedStyles &= ~p_style;
    }
  }
}

void    
WindowDefinition::GetWindowSizePos(int& left,int& top,int& right,int& bottom)
{
  left   = m_win_left;
  top    = m_win_top;
  right  = m_win_right;
  bottom = m_win_bottom;
}

void
WindowDefinition::SetWindowSizePos(int left,int top,int right,int bottom)
{
  m_win_left   = left;
  m_win_top    = top;
  m_win_right  = right;
  m_win_bottom = bottom;
}

CString
WindowDefinition::DefinitionString()
{
  //  main="Call center file","CallCenter.hhc","CallCenter.hhk","Introduction_to_the_Call_Center_sample.htm"
  //       ,"","Crossword_puzzle.htm","JUMP1","Invalid_credit_card_number.htm","JUMP2"
  //       ,0x22520,250,0xc304e,[95,101,873,743],0x10b0000,0x0,,0,0,0,0
  //        NavPane wid buttons  size            styles    ext pane options

  CString definition;
  definition.Format("%s=\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\","
                    "0x%x,%d,0x%x,[%d,%d,%d,%d],0x%x,0x%x,,%d,%d,%d"
                   ,m_name.GetString()
                   ,m_title.GetString()
                   ,m_HHCFile.GetString()
                   ,m_HHKFile.GetString()
                   ,m_defaultTopic.GetString()
                   ,m_homeTopic.GetString()
                   ,m_jump1Topic.GetString()
                   ,m_jump1Text.GetString()
                   ,m_jump2Topic.GetString()
                   ,m_jump2Text.GetString()
                   ,m_navigationPane
                   ,m_paneWidth
                   ,m_buttons
                   ,m_win_left
                   ,m_win_top
                   ,m_win_right
                   ,m_win_bottom
                   ,m_windowStyles
                   ,m_extendedStyles
                   // ,m_unknown1
                   ,m_hidePane
                   ,m_defaultPane
                   ,m_tabPosition);
  if(m_project->GetGlossary()->HasEntries() && GetNavPaneOption(NAVPANE_GLOSSARY))
  {
    // If we have a filled glossary AND the glossary pane is ON in this window, use it
    definition += ",[Glossary,HHActiveX.GlossaryPane]";
  }
  return definition;
}

bool
WindowDefinition::ParseDefinitionString(CString definition)
{
  //  main="Call center file","CallCenter.hhc","CallCenter.hhk","Introduction_to_the_Call_Center_sample.htm"
  //       ,"","Crossword_puzzle.htm","JUMP1","Invalid_credit_card_number.htm","JUMP2"
  //       ,0x22520,250,0xc304e,[95,101,873,743],0x10b0000,0x0,,0,0,0,0
  //        NavPane wid buttons  size            styles    ext pane options
  if(definition.IsEmpty())
  {
    return false;
  }
  // Find the name
  int pos = definition.Find('=');
  if(pos > 0)
  {
    m_name = definition.Left(pos);
    definition = definition.Mid(pos+1);
  }
  else
  {
    return false;
  }
  // Find the title;
  ParseQuotedString(definition,m_title);
  // Find the HHC
  ParseQuotedString(definition,m_HHCFile);
  // Find the HHK
  ParseQuotedString(definition,m_HHKFile);
  // Find default topic
  ParseQuotedString(definition,m_defaultTopic);
  // Find home topic
  ParseQuotedString(definition,m_homeTopic);
  // Find Jump1 topic
  ParseQuotedString(definition,m_jump1Topic);
  // Find Jump1 text on button
  ParseQuotedString(definition,m_jump1Text);
  // Find Jump2 topic
  ParseQuotedString(definition,m_jump2Topic);
  // Find Jump2 text on button
  ParseQuotedString(definition,m_jump2Text);
  // Parse navigationPane
  Misc::ParseNumber(definition,m_navigationPane);
  // Parse PaneWidth
  Misc::ParseNumber(definition,m_paneWidth);
  // Parse buttons
  Misc::ParseNumber(definition,m_buttons);
  // Parse position rectangle
  ParseWindowSize(definition,m_win_left,m_win_top,m_win_right,m_win_bottom);
  // Parse windowStyles
  Misc::ParseNumber(definition,m_windowStyles);
  // Parse extended styles
  Misc::ParseNumber(definition,m_extendedStyles);
  // Rest of the string is reserved?
  Misc::ParseNumber(definition,m_unknown1);
  // Parse Hide nav pane on startup
  Misc::ParseNumber(definition,m_hidePane);
  // Reserved
  Misc::ParseNumber(definition,m_defaultPane);
  // Reserved
  Misc::ParseNumber(definition,m_tabPosition);

  return true;
}

void
WindowDefinition::ParseQuotedString(CString& def,CString& name)
{
  // Reset name
  name = "";
  if(def.IsEmpty())
  {
    return;
  }
  def.TrimLeft();
  if(def.GetAt(0) == '\"')
  {
    def = def.Mid(1);
  }
  int pos = def.Find('\"');
  if(pos >= 2)
  {
    // Non Empty string;
    name = def.Left(pos);
  }
  // Parse off \"
  def = def.Mid(pos+1);
  def.TrimLeft();
  // Parse off ','
  if(def.GetAt(0) == ',')
  {
    def = def.Mid(1);
  }
}

void
WindowDefinition::ParseWindowSize(CString& def
                                 ,unsigned long& top
                                 ,unsigned long& left
                                 ,unsigned long& right
                                 ,unsigned long& bottom)
{
  top = left = right = bottom = 0;
  def.TrimLeft();
  if(def.GetLength() < 10)
  {
    return;
  }
  if(def.GetAt(0) == '[')
  {
    def = def.Mid(1);
  }
  Misc::ParseNumber(def,top);
  Misc::ParseNumber(def,left);
  Misc::ParseNumber(def,right);
  Misc::ParseNumber(def,bottom);
}

