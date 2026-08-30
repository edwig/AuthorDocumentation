//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    WindowDefinition.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: The definition of the main CHM windo
//
#pragma once 

// Values for m_navigationPane
#define NAVPANE_TOC_INDEX   0x00000020   // Use TOC and INDEX tab
#define NAVPANE_AUTOSYNCH   0x00000100   // Auto synchronize TOC with topic
#define NAVPANE_SEARCH      0x00000400   // Use the text search tab
#define NAVPANE_FAVOURITES  0x00001000   // Use favourites tab
#define NAVPANE_ADV_SEARCH  0x00020000   // Use advanced search on searchtab
#define NAVPANE_GLOSSARY    0x00080000   // Use glossary tab
#define NAVPANE_BROWSE_SEQ  0x40000000   // Use browse sequences as of BRS file
#define NAVPANE_SIZE_POS    0x00040000   // Remember window size and position
#define NAVPANE_SHOW_HIDE   0x00000001   // Auto show/hide the navigation pane

// Values for m_buttons
#define BUTTONS_HIDESHOW    0x00000002
#define BUTTONS_BACK        0x00000004
#define BUTTONS_FORWARD     0x00000008
#define BUTTONS_STOP        0x00000010
#define BUTTONS_REFRESH     0x00000020
#define BUTTONS_HOME        0x00000040
#define BUTTONS_OPTIONS     0x00001000
#define BUTTONS_PRINT       0x00002000
#define BUTTONS_LOCATE      0x00000800
#define BUTTONS_JUMP1       0x00040000
#define BUTTONS_JUMP2       0x00080000

// Values for m_windowStyles
#define STYLES_SYSTEMMENU   WS_SYSMENU          // 0x00080000
#define STYLES_MINIMIZEBOX  WS_MINIMIZEBOX
#define STYLES_MAXIMIZEBOX  WS_MAXIMIZEBOX
#define STYLES_CLIPSIBLINGS WS_CLIPSIBLINGS
#define STYLES_CLIPCHILDREN WS_CLIPCHILDREN
#define STYLES_VERTSCROLL   WS_VSCROLL
#define STYLES_HORZSCROLL   WS_HSCROLL
#define STYLES_CHILD        WS_CHILD
#define STYLES_POPUP        WS_POPUP
#define STYLES_MINIMIZE     WS_MINIMIZE
#define STYLES_MAXIMIZE     WS_MAXIMIZE
#define STYLES_BORDER       WS_BORDER
#define STYLES_DLGFRAME     WS_DLGFRAME
#define STYLES_VISIBLE      WS_VISIBLE
#define STYLES_THICK        WS_THICKFRAME
#define STYLES_DISABLED     WS_DISABLED
#define STYLES_OVERLAPPED   WS_OVERLAPPED

// Values for m_extendedStyles
#define STYLES_TOOLWIN      WS_EX_TOOLWINDOW
#define STYLES_CLIENTEDGE   WS_EX_CLIENTEDGE
#define STYLES_STATICEDGE   WS_EX_STATICEDGE
#define STYLES_TRANSPARENT  WS_EX_TRANSPARENT
#define STYLES_ACCEPTFILES  WS_EX_ACCEPTFILES
#define STYLES_CONTROLPAR   WS_EX_CONTROLPARENT
#define STYLES_CONTEXTHELP  WS_EX_CONTEXTHELP
#define STYLES_RTL          WS_EX_RTLREADING
#define STYLES_NOPARNOTIFY  WS_EX_NOPARENTNOTIFY
#define STYLES_RIGHTALIGNED WS_EX_RIGHT
#define STYLES_LEFTSCROLL   WS_EX_LEFTSCROLLBAR
#define STYLES_TOPMOST      WS_EX_TOPMOST

// Values for DefaultPane
#define DEFPANE_TOC         0
#define DEFPANE_INDEX       1
#define DEFPANE_SEARCH      2
#define DEFPANE_FAVOURITES  3
#define DEFPANE_GLOSSARY    4

class ProjectFile;

class WindowDefinition
{
public:
   WindowDefinition(ProjectFile* p_project);
  ~WindowDefinition();

  CString DefinitionString();
  bool    ParseDefinitionString(CString definition);

  // Setters and getters
  CString GetName();
  void    SetName (CString name);
  CString GetTitle();
  void    SetTitle(CString title);
  CString GetHHCFile();
  void    SetHHCFile(CString p_file);
  CString GetHHKFile();
  void    SetHHKFile(CString p_file);
  CString GetDefaultTopic();
  void    SetDefaultTopic(CString p_topic);
  CString GetHomeTopic();
  void    SetHomeTopic(CString p_topic);
  CString GetJump1Text();
  void    SetJump1Text(CString p_text);
  CString GetJump2Text();
  void    SetJump2Text(CString p_text);
  CString GetJump1Topic();
  void    SetJump1Topic(CString p_topic);
  CString GetJump2Topic();
  void    SetJump2Topic(CString p_topic);
  bool    GetNavPaneOption(int p_option);
  void    SetNavPaneOption(int p_option,bool p_set);
  bool    GetButton(int p_button);
  void    SetButton(int p_button,bool p_set);
  bool    GetWinStyle(int p_style);
  void    SetWinStyle(int p_style,bool p_set);
  bool    GetWinExStyle(int p_style);
  void    SetWinExStyle(int p_style,bool p_set);
  int     GetDefaultPane();
  void    SetDefaultPane(int p_pane);
  int     GetTabPosition();
  void    SetTabPosition(int p_pos);
  bool    GetHidePane();
  void    SetHidePane(bool p_hide);
  int     GetPaneWidth();
  void    SetPaneWidth(int p_width);
  void    GetWindowSizePos(int& left,int& top,int& right,int& bottom);
  void    SetWindowSizePos(int  left,int  top,int  right,int  bottom);

private:
  void Reset();
  bool IsNavPaneOption(int p_option);
  bool IsButton(int p_button);
  bool IsWinStyle(int p_style);
  bool IsWinExStyle(int p_style);
  void ParseQuotedString(CString& def,CString& name);
  void ParseWindowSize(CString& def
                      ,unsigned long& top
                      ,unsigned long& left
                      ,unsigned long& right
                      ,unsigned long& bottom);

  ProjectFile* m_project;
  CString m_name;
  CString m_title;
  // Files
  CString m_HHCFile;
  CString m_HHKFile;
  CString m_defaultTopic;
  CString m_homeTopic;
  CString m_jump1Text;
  CString m_jump1Topic;
  CString m_jump2Text;
  CString m_jump2Topic;
  unsigned long m_navigationPane;
  unsigned long m_paneWidth;        // Width of the navigation pane
  unsigned long m_buttons;
  unsigned long m_windowStyles;
  unsigned long m_extendedStyles;
  unsigned long m_win_left;
  unsigned long m_win_top;
  unsigned long m_win_right;
  unsigned long m_win_bottom;
  unsigned long m_unknown1;
  unsigned long m_hidePane;         // Boolean 1 = hide pane on startup
  unsigned long m_defaultPane;      // Boolean 0 = TOC is default, 1 = Index is default, 2 = Search 3 = Favourites 4=Glossary
  unsigned long m_tabPosition;      // 0 = Top, 1 = left, 2 = bottom
};

inline CString 
WindowDefinition::GetName()
{ 
  return m_name;  
}

inline CString 
WindowDefinition::GetTitle()
{
  return m_title; 
}

inline void    
WindowDefinition::SetName(CString name)
{ 
  m_name  = name;  
}

inline void    
WindowDefinition::SetTitle(CString title) 
{ 
  m_title = title; 
}

inline int
WindowDefinition::GetDefaultPane()
{
  return m_defaultPane;
}

inline void    
WindowDefinition::SetDefaultPane(int p_pane)
{
  m_defaultPane = p_pane;
}

inline int
WindowDefinition::GetTabPosition()
{
  return m_tabPosition;
}

inline void
WindowDefinition::SetTabPosition(int p_pos)
{
  m_tabPosition = p_pos;
}

inline bool
WindowDefinition::GetHidePane()
{
  return (m_hidePane != 0);
}

inline void
WindowDefinition::SetHidePane(bool p_hide)
{
  m_hidePane = p_hide;
}

inline int
WindowDefinition::GetPaneWidth()
{
  return m_paneWidth;
}

inline void
WindowDefinition::SetPaneWidth(int p_width)
{
  m_paneWidth = p_width;
}

inline CString 
WindowDefinition::GetJump1Text()
{
  return m_jump1Text;
}

inline void    
WindowDefinition::SetJump1Text(CString p_text)
{
  m_jump1Text = p_text;
}

inline CString 
WindowDefinition::GetJump2Text()
{
  return m_jump2Text;
}

inline void    
WindowDefinition::SetJump2Text(CString p_text)
{
  m_jump2Text = p_text;
}

inline CString 
WindowDefinition::GetJump1Topic()
{
  return m_jump1Topic;
}

inline void    
WindowDefinition::SetJump1Topic(CString p_topic)
{
  m_jump1Topic = p_topic;
}

inline CString 
WindowDefinition::GetJump2Topic()
{
  return m_jump2Topic;
}

inline void
WindowDefinition::SetJump2Topic(CString p_topic)
{
  m_jump2Topic = p_topic;
}

inline CString 
WindowDefinition::GetHHCFile()
{
  return m_HHCFile;
}

inline void    
WindowDefinition::SetHHCFile(CString p_file)
{
  m_HHCFile = p_file;
}

inline CString 
WindowDefinition::GetHHKFile()
{
  return m_HHKFile;
}

inline void    
WindowDefinition::SetHHKFile(CString p_file)
{
  m_HHKFile = p_file;
}

inline CString 
WindowDefinition::GetDefaultTopic()
{
  return m_defaultTopic;
}

inline void    
WindowDefinition::SetDefaultTopic(CString p_topic)
{
  m_defaultTopic = p_topic;
}

inline CString 
WindowDefinition::GetHomeTopic()
{
  return m_homeTopic;
}

inline void    
WindowDefinition::SetHomeTopic(CString p_topic)
{
  m_homeTopic = p_topic;
}
