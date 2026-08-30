//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    CompilePane.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: A compile pane shows the status of the current
//              compilation of a CHM help file
//
#pragma once
#include "resource.h"
#include <Redirect.h>
#include <deque>

typedef std::deque<CString> CompLines;
class Compilation;

class CompilePane : public CDockablePane, public Redirect
{
public:
   CompilePane();
  ~CompilePane();
  // Compile our help. This is the main public entry
  void CompileHelp(CString& p_baseDir,CString& p_projectName,int p_size);
  bool CompilationRunning();
  void AdjustLayout();

  // Implementation
  void ChildAbort();

  // Virtual interface. Derived class must implement this!!
  void OnChildStarted    (LPCTSTR lpszCmdLine) override;
  void OnChildStdOutWrite(LPCTSTR lpszOutput)  override; 
  void OnChildStdErrWrite(LPCTSTR lpszOutput)  override;
  void OnChildTerminate  () override;
  void OnChildComplete   () override;

protected:
  DECLARE_MESSAGE_MAP()

  afx_msg int   OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void  OnCompileCommand(UINT nCommand);
  afx_msg void  OnCompileLine();
  afx_msg void  OnCompileEnd();
  afx_msg void  OnSize(UINT nType,int cx,int cy);
 
private:
  void    AddLine(CString& p_line);
  bool    ClearPane();
  void    ClosePane();
  CString GetExePath();
  void    Acquire();
  void    Release();

  CEdit             m_edit;
  int               m_errors;
  CompLines         m_lines;
  Compilation*      m_comp;
  int               m_currentLine;
  int               m_maxLine;
  CRITICAL_SECTION  m_criticalSection;
  bool              m_compilationRunning;
  // Saving old values
  BOOL              m_oldApply;
  CString           m_oldDir;
};

inline bool 
CompilePane::CompilationRunning()
{
  return m_compilationRunning;
}
