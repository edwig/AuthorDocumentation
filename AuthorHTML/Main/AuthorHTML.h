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

  int                 GetButtonLayout();
  Ref<CAfbeeldingen>  GetImages();
  ControlsInfo*       GetControlsInfo();
  ProjectFile*        GetProjectFile();
  TOC*                GetTOC();
  IndexFile*          GetIndex();
  CRecentFileList*    GetRecentFileList();
  CRecentFileList*    GetRecentProjectList();

  CString MessageBox(const CString& text
                    ,const CString& title
                    ,const CString& knoppen);
  int     MessageBox(const CString& text,
                     const CString& title,
                           UINT     uType);
  int   ErrorMessage(const CString& message);
  void  Panic(CString message);

  UINT  m_nAppLook;
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

  CString             m_baseDir;
  CString             m_binDir;
  Spelling*           m_speller;
  std::set<CString>   m_messages;
  Ref<CAfbeeldingen>  m_images;
  ControlsInfo*       m_controlsInfo;

  // Started on a HPP project file
  CString            m_project;
  bool               m_sweep; 
  bool               m_reindex;
  ProjectFile*       m_projectFile;
  TOC*               m_contentFile;
  IndexFile*         m_indexFile;
  StartupDlg*        m_startup;

  HINSTANCE m_hSciDLL;

  CCriticalSection   m_criticalSection;
  CStringList        m_StdioCommand;

  CRecentFileList*   m_pRecentProjectList;
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

