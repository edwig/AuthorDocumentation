//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    CompilePane.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: A compile pane shows the status of the current
//              compilation of a CHM help file
//
#include "StdAfx.h"
#include "CompilePane.h"
#include "Compilation.h"
#include "AuthorHTML.h"
#include "MainFrm.h"
#include "Misc.h"
#include "CheckWorkshop.h"

#define ID_COMPILE_EDIT 1234
#define ID_COMPILE_LINE 1235
#define ID_COMPILE_END  1236

CompilePane::CompilePane()
{
  m_errors      = 0;
  m_comp        = NULL;
  m_currentLine = 0;
  m_compilationRunning = false;
  InitializeCriticalSection((LPCRITICAL_SECTION)&m_criticalSection);
}

CompilePane::~CompilePane()
{
}

BEGIN_MESSAGE_MAP(CompilePane, CDockablePane)
  ON_WM_CREATE()
  ON_WM_SIZE()
  ON_COMMAND(ID_COMPILE_LINE, OnCompileLine)
  ON_COMMAND(ID_COMPILE_END,  OnCompileEnd)
  ON_COMMAND_RANGE(ID_CLEAR_COMPILE, ID_CLOSE_COMPILE, OnCompileCommand)
END_MESSAGE_MAP()

int
CompilePane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  CDockablePane::OnCreate(lpCreateStruct);

  if(!m_edit.Create(WS_VISIBLE|WS_CHILD|WS_HSCROLL|WS_VSCROLL|
                    ES_AUTOHSCROLL|ES_AUTOVSCROLL|ES_MULTILINE|ES_WANTRETURN|ES_READONLY
                   ,CRect(0,0,0,0),this,ID_COMPILE_EDIT))
  {
    theApp.Panic("Unable to create compile edit control");
  }
  AdjustLayout();
  return 0;
}

void
CompilePane::OnCompileCommand(UINT nCommand)
{
  switch(nCommand)
  {
    case ID_CLEAR_COMPILE:  ClearPane(); break;
    case ID_CLOSE_COMPILE:  ClosePane(); break;
  }
}

void
CompilePane::CompileHelp(CString& p_basedir
                        ,CString& p_projectName
                        ,int      p_size)
{
  m_errors = 0;
  m_maxLine = p_size;

  // Reset the compilation pane
  if (ClearPane() == false)
  {
    // Not reset, don't compile again
    return;
  }

  CheckWorkshop workshop;
  if(!workshop.CheckHTMLHelpWorkshop())
  { 
    theApp.Panic("HTML Help Workshop is not installed. Cannot continue compiling the help file.");
    return;
  }

  // Highest setting for our designers!
  workshop.SetHHZoneRestriction(HHZone_Untrusted);

  // Save old directory
  char buffer[MAX_PATH + 1];
  if (GetCurrentDirectory(MAX_PATH, buffer) > 0)
  {
    m_oldDir = buffer;
    if(SetCurrentDirectory(p_basedir) == FALSE)
    {
      theApp.Panic("Unable to set current directory to: " + p_basedir);
      return;
    }
  }
  else
  {
    theApp.Panic("Unable to get current directory");
    return;
  }
  m_comp = new Compilation(this,true);
  m_comp->DoTheInit();
  m_comp->SetMax(m_maxLine);
  m_comp->SetCurrent(m_currentLine = 0);

  // Set the compiler command
  XString cmdLine = "\"" + workshop.HTMLHelpWorkshopPath() + "\\hhc.exe\" \"" + p_projectName + "\"";
  StartChildProcess(cmdLine);
}

void 
CompilePane::OnChildStarted(LPCTSTR lpszCmdLine)
{
  CString command(lpszCmdLine);
  CString line;
  line.Format("Starting compilation: %s\r\n",lpszCmdLine);
  m_compilationRunning = true;

  Acquire();
  m_lines.push_back(line);
  Release();

  ::PostMessage(m_hWnd,WM_COMMAND,ID_COMPILE_LINE,0);
}

void 
CompilePane::OnChildStdOutWrite(LPCTSTR lpszOutput)
{
  // Normal output line
  ++m_currentLine;

  CString standard(lpszOutput);
  Acquire();
  m_lines.push_back(standard);
  Release();

  ::PostMessage(m_hWnd,WM_COMMAND,ID_COMPILE_LINE,0);
}

void 
CompilePane::OnChildStdErrWrite(LPCTSTR lpszOutput)
{
  CString errorLine(lpszOutput);
  Acquire();
  m_lines.push_back(errorLine);
  Release();

  ::PostMessage(m_hWnd,WM_COMMAND,ID_COMPILE_LINE,0);
  ++m_errors;
}

void 
CompilePane::OnChildTerminate()
{
  Acquire();

  // Write an END-OF-TRANSMISSION after the output, so the
  // Redirect scanner can stop reading
  if (m_hStdOut != NULL)
  {
    TCHAR buf[1] = { EOT };
    ::WriteFile(m_hStdOut, &buf, sizeof(TCHAR), NULL, NULL);
  }
  if (m_hStdErr != NULL)
  {
    TCHAR buf[1] = { EOT };
    ::WriteFile(m_hStdErr, &buf, sizeof(TCHAR), NULL, NULL);
  }
  Release();
}

void
CompilePane::OnChildComplete()
{
  m_lines.push_back(CString(""));
  m_lines.push_back(CString("\r\nCompilation ready.\r\n"));
  ::PostMessage(m_hWnd, WM_COMMAND, ID_COMPILE_LINE, 0);
  ::PostMessage(m_hWnd, WM_COMMAND, ID_COMPILE_LINE, 0);
  Release();

  // Remove compilation dialog
  ::PostMessage(m_hWnd, WM_COMMAND, ID_COMPILE_END, 0);
  // Reset the directory
  SetCurrentDirectory(m_oldDir);

  m_compilationRunning = false;
}

void
CompilePane::ChildAbort()
{
  TerminateChildProcess();
  Acquire();
  m_lines.push_back(CString(""));
  m_lines.push_back(CString("\r\nCompilation aborted.\r\n"));
  Release();

  ::PostMessage(m_hWnd,WM_COMMAND,ID_COMPILE_LINE,0);
  ::PostMessage(m_hWnd,WM_COMMAND,ID_COMPILE_LINE,0);

  // Remove compilation dialog
  ::PostMessage(m_hWnd,WM_COMMAND,ID_COMPILE_END,0);
  // Reset the directory
  SetCurrentDirectory(m_oldDir);
}

void
CompilePane::OnCompileLine()
{
  CString line;

  Acquire();
  if(m_lines.size())
  {
    line = m_lines[0];
    m_lines.pop_front();
    AddLine(line);
  }
  Release();

  if(m_comp)
  {
    m_comp->SetText(line);
    m_comp->SetCurrent(m_currentLine);
  }
}

void
CompilePane::OnCompileEnd()
{
  if(m_comp)
  {
    delete m_comp;
    m_comp = NULL;
  }
}

void
CompilePane::AddLine(CString& p_line)
{
  int lines = m_edit.GetLineCount();
  int begin = m_edit.LineIndex(lines);
  int end   = begin + m_edit.LineLength(lines);
  m_edit.SetSel(begin,end);
  m_edit.ReplaceSel(p_line.GetString());
}

bool
CompilePane::ClearPane()
{
  m_edit.SetReadOnly(FALSE);
  m_edit.SetSel(0,-1);
  m_edit.Clear();
  m_edit.SetReadOnly(TRUE);
  return true;
}

void
CompilePane::ClosePane()
{
  MainFrame* frame = (MainFrame*) AfxGetMainWnd();
  frame->ClosePane(ID_PANE_COMPILE);
}

// Acquire a multithread lock for QueueSection
void
CompilePane::Acquire()   
{
  EnterCriticalSection(&m_criticalSection);
}

// Release the multithread lock for QueueSection
void 
CompilePane::Release()   
{
  LeaveCriticalSection(&m_criticalSection);
}

void
CompilePane::OnSize(UINT nType,int cx,int cy)
{
  CDockablePane::OnSize(nType,cx,cy);
  AdjustLayout();
}

void
CompilePane::AdjustLayout()
{
  if(GetSafeHwnd() == nullptr)
  {
    return;
  }

  CRect rectClient;
  GetClientRect(rectClient);

  m_edit.SetWindowPos(nullptr,rectClient.left,rectClient.top,rectClient.Width(),rectClient.Height(),SWP_NOACTIVATE | SWP_NOZORDER);
}
