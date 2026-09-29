//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    AuthorHTML.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// The definition of the main application
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "SettingsManager.h"
#include "Images.h"
#include "RefCounted.h"
#include "Version.h"
#include <set>
#include <afxmt.h>
#include <afxwinappex.h>

// For all Dialogs and DDX Exchange functions
// Call "UpdateData(Controls2Data)"
//   or "UpdateData(Data2Controls)"
const bool Controls2Data = true;
const bool Data2Controls = false;

class Spelling;
class ControlsInfo;
class ProjectFile;
class TOC;
class IndexFile;
class StartupDlg;

typedef HWND(CALLBACK* LPFNHTMLHELP)(HWND,LPCTSTR,UINT,DWORD_PTR);

/////////////////////////////////////////////////////////////////////////////
// AuthorHTMLApp:
// See HTMLEdit.cpp for the implementation of this class
//
class AuthorHTMLApp : public CWinAppEx
{
public:
  AuthorHTMLApp();
 ~AuthorHTMLApp();
  BOOL      CanExitInstance();
  CString   GetBaseDirectory();
  CString   GetBinDirectory();
  Spelling* GetSpeller();
  void      ResetSpeller();
  void      ReSweepProject();
  void      ReSweepIndex();
  void      SuppressMessage(CString message);
  bool      IsSuppressedMessage(CString message);
  void      OpenTypedDocumentFile(CString& file);
  void      RedisplayTOC  (bool showPane = false);
  void      RedisplayIndex(bool showPane = false);
  int       GetUniqueDocID();
  void      SyncFontNameAndSize();

  int                 GetButtonLayout();
  Ref<CAfbeeldingen>  GetImages();
  ControlsInfo*       GetControlsInfo();
  ProjectFile*        GetProjectFile();
  TOC*                GetTOC();
  IndexFile*          GetIndex();
  CRecentFileList*    GetRecentFileList();
  CRecentFileList*    GetRecentProjectList();
  bool                GetDBCSMode();

  CString MessageBox(const CString& text
                    ,const CString& title
                    ,const CString& knoppen);
  int     MessageBox(const CString& text,
                     const CString& title,
                           UINT     uType);
  int   ErrorMessage(const CString& message);
  void  Panic(CString message);

  void  ShowHtmlHelp(CString const& pad,UINT uMode,HH_AKLINK& link);


  UINT  m_nAppLook { 0 };
public:
	virtual BOOL InitInstance();
  virtual int  ExitInstance();
  virtual BOOL SaveAllModified();
  virtual BOOL OnIdle(LONG lCount); // return TRUE if more idle processing

  // Implementation
  afx_msg void OnStartup();
	afx_msg void OnAppAbout();
  afx_msg void OnFileOpen();
  afx_msg void OnNewFile();
  afx_msg void OnNewProject();
  afx_msg void OnProjectOpen();
  afx_msg void OnFileOpenurl();
  afx_msg void OnFileSaveAll();
  afx_msg void OnCloseProject();
  afx_msg void OnProjectSettings();
  afx_msg void OnWindowDefinitions();
  afx_msg void OnCompile();
  afx_msg void OnImport();
  afx_msg void OnReadHelp();
  afx_msg void OnHasProject  (CCmdUI* pCmdUI);
  afx_msg void OnHasNoProject(CCmdUI* pCmdUI);
  afx_msg void OnHasContent  (CCmdUI* pCmdUI);

  DECLARE_MESSAGE_MAP()

protected:
  afx_msg void OnAppExit();
private:
  void ResetProject();
  void OpenProjectFile(bool p_create = false);
  void OpenContentsFile(CString contents,bool p_create = false);
  void OpenIndexFile   (CString index,   bool p_create = false);
  void ParseOptions(CString& commandLine);
  void InitImages();
  void SetBinDir();
  void OnManualMicrosoftHTML();
  void OnManualW3C_HTML();
  void OnManualW3C_CSS();
  bool LoadHtmlHelpDLL();

  CString            m_baseDir;                                     // Directory folder of the documentation project
  CString            m_binDir;                                      // Directory folder of the AuthorHTML binaries
  Spelling*          m_speller            { nullptr };              // Current spell checker
  std::set<CString>  m_messages;                                    // Set of suppressedmessages
  Ref<CAfbeeldingen> m_images;                                      // Set of images for the application
  ControlsInfo*      m_controlsInfo       { nullptr };              // Controls on dialogs
  CString            m_project;                                     // Current project name
  bool               m_sweep              { false   };              // Project folder has been swept for tags
  bool               m_reindex            { false   };              // Index file needs to be re-indexed
  ProjectFile*       m_projectFile        { nullptr };              // Current project file
  TOC*               m_contentFile        { nullptr };              // Table of contents file
  IndexFile*         m_indexFile          { nullptr };              // Index file
  StartupDlg*        m_startup            { nullptr };              // Startup dialog
  int                m_uniqueDocID        { 0       };              // Unique document ID for new documents
  HINSTANCE          m_hSciDLL            { NULL    };              // Scintilla DLL handle
  CRecentFileList*   m_pRecentProjectList { nullptr };              // List of recent project files (For the startup dialog)
  // Our own help system
  HMODULE            m_htmlLib            { nullptr };              // Our own HTML help DLL handle (hhctrl.ocx)
  bool               m_noHelpAvailabale   { false   };              // Status if HTML Help is not properly installed on the system
  LPFNHTMLHELP       m_htmlHelp           { nullptr };              // Pointer to the HTML Help function
};

inline CString
AuthorHTMLApp::GetBaseDirectory()
{
  return m_baseDir;
}

inline CString
AuthorHTMLApp::GetBinDirectory()
{
  return m_binDir;
}

inline Ref<CAfbeeldingen>
AuthorHTMLApp::GetImages()
{
  return m_images;
}

inline ProjectFile*
AuthorHTMLApp::GetProjectFile()
{
  return m_projectFile;
}

inline TOC*
AuthorHTMLApp::GetTOC()
{
  return m_contentFile;
}

inline IndexFile*
AuthorHTMLApp::GetIndex()
{
  return m_indexFile;
}

inline void
AuthorHTMLApp::ReSweepProject()
{
  m_sweep = true;
}

inline void
AuthorHTMLApp::ReSweepIndex()
{
  m_sweep   = true;
  m_reindex = true;
}

inline CRecentFileList*   
AuthorHTMLApp::GetRecentFileList()
{
  return m_pRecentFileList;
}

inline CRecentFileList*
AuthorHTMLApp::GetRecentProjectList()
{
  return m_pRecentProjectList;
}

extern SettingsManager settings;
extern AuthorHTMLApp   theApp;


