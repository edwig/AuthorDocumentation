//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    AuthorHTML.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// The definition of the main application
//
#include "stdafx.h"
#include "AuthorHTML.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "HTMLEdDoc.h"
#include "HTMLEdView.h"
#include "ScintillaScriptView.h"
#include "ScintillaScriptDoc.h"
#include "ScintillaChildFrm.h"
#include "FileDialog.h"
#include "ProjectDlg.h"
#include "WindowDefDlg.h"
#include "AuMessageBox.h"
#include "Spelling.h"
#include "ControlsInfo.h"
#include "GetURL.h"
#include "ProjectFile.h"
#include "TOC.h"
#include "IndexFile.h"
#include "StartupDlg.h"
#include "ImportDlg.h"
#include "AuMessageBox.h"
#include <GetExePath.h>
#include <WideMessageBox.h>
#include <StringUtilities.h>
#include <afxadv.h>
#include <direct.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CAUTION
// The settingsmanager and theApp must appear in this order, to make sure
// they are destructed in the oposite order. This is neccesary as the settingsManager
// is still needed during the destruction time of theApp.

/////////////////////////////////////////////////////////////////////////////
// The one and only SettingsManager object
SettingsManager settings;

/////////////////////////////////////////////////////////////////////////////
// The one and only AuthorHTMLApp object
AuthorHTMLApp theApp;

CRuntimeClass* pRTC = RUNTIME_CLASS(CMFCToolBarComboBoxButton);

/////////////////////////////////////////////////////////////////////////////
// AuthorHTMLApp construction

AuthorHTMLApp::AuthorHTMLApp()
              :m_speller(NULL)
              ,m_controlsInfo(NULL)
              ,m_projectFile(NULL)
              ,m_contentFile(NULL)
              ,m_sweep(false)
              ,m_pRecentProjectList(NULL)
              ,m_startup(NULL)
              ,m_uniqueDocID(0)
{
}

AuthorHTMLApp::~AuthorHTMLApp()
{
  m_messages.clear();
  ResetSpeller();
  if(m_controlsInfo)
  {
    delete m_controlsInfo;
    m_controlsInfo = NULL;
  }
}

void
AuthorHTMLApp::ResetProject()
{
  if(m_indexFile)
  {
    m_indexFile->WriteIndexFile();
    delete m_indexFile;
    m_indexFile = NULL;
  }
  if(m_contentFile)
  {
    m_contentFile->WriteTOCFile();
    delete m_contentFile;
    m_contentFile = NULL;
  }
  // Project file must be destroyed last
  // It ownn's the DocumentFiles which are needed in the re-writing
  // of the HHC and HHK files
  if(m_projectFile)
  {
    m_projectFile->WriteProjectFile();
    delete m_projectFile;
    m_projectFile = NULL;
  }
  MainFrame::SetStatusText(_T(""));
}

/////////////////////////////////////////////////////////////////////////////
// AuthorHTMLApp message map
//
BEGIN_MESSAGE_MAP(AuthorHTMLApp, CWinAppEx)
  ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
  // Standard file based document commands
  ON_COMMAND(ID_FILE_NEW,          OnNewFile)
  ON_COMMAND(ID_FILE_NEWPROJECT,   OnNewProject)
  ON_COMMAND(ID_FILE_OPEN,         OnFileOpen)
  ON_COMMAND(ID_FILE_OPENURL,      OnFileOpenurl)
  ON_COMMAND(ID_FILE_OPENPROJECT,  OnProjectOpen)
  ON_COMMAND(ID_CLOSEPROJECT,      OnCloseProject)
  ON_COMMAND(ID_SAVEALL,           OnFileSaveAll)
  ON_COMMAND(ID_APP_EXIT,          OnAppExit)
  // Standard print setup command
  ON_COMMAND(ID_FILE_PRINT_SETUP,  CWinAppEx::OnFilePrintSetup)
  ON_COMMAND(ID_COMPILE_HELP,      OnCompile)
  ON_COMMAND(ID_READHELP,          OnReadHelp)

  ON_COMMAND(ID_TOOLS_PROJECT,     OnProjectSettings)
  ON_COMMAND(ID_WINDOWDEFS,        OnWindowDefinitions)
  ON_COMMAND(ID_STARTUP,           OnStartup)
  ON_COMMAND(ID_IMPORTCHM,         OnImport)
  // HELP MENU
  ON_COMMAND(ID_REFERENCEMANUALS_MICROSOFTHTML, OnManualMicrosoftHTML)
  ON_COMMAND(ID_REFERENCEMANUALS_W3CHTML4,      OnManualW3C_HTML)
  ON_COMMAND(ID_REFERENCEMANUALS_W3CCSS2,       OnManualW3C_CSS)
  // Command UI Helpers
  ON_UPDATE_COMMAND_UI(ID_COMPILE_HELP,         OnHasProject)
  ON_UPDATE_COMMAND_UI(ID_READHELP,             OnHasProject)
  ON_UPDATE_COMMAND_UI(ID_CLOSEPROJECT,         OnHasProject)
  ON_UPDATE_COMMAND_UI(ID_FILE_OPENPROJECT,     OnHasNoProject)
  ON_UPDATE_COMMAND_UI(ID_FILE_NEWPROJECT,      OnHasNoProject)
  ON_UPDATE_COMMAND_UI(ID_TOOLS_PROJECT,        OnHasProject)
  ON_UPDATE_COMMAND_UI(ID_WINDOWDEFS,           OnHasProject)
  ON_UPDATE_COMMAND_UI(ID_STARTUP,              OnHasContent)
  ON_UPDATE_COMMAND_UI(ID_IMPORTCHM,            OnHasNoProject)
  ON_UPDATE_COMMAND_UI(ID_TOOLS_BROKENLINKS,    OnHasProject)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHTMLEdApp initialization

BOOL AuthorHTMLApp::InitInstance()
{
  // InitCommonControlsEx() is required on Windows XP if an application
  // manifest specifies use of ComCtl32.dll version 6 or later to enable
  // visual styles.  Otherwise, any window creation will fail.
  INITCOMMONCONTROLSEX InitCtrls;
  InitCtrls.dwSize = sizeof(InitCtrls);
  // Set this to include all the common control classes you want to use
  // in your application.
  InitCtrls.dwICC = ICC_WIN95_CLASSES;
  InitCommonControlsEx(&InitCtrls);

  CWinAppEx::InitInstance();

  // Initialize OLE libraries
  if (!AfxOleInit())
  {
    return FALSE;
  }	
	AfxEnableControlContainer();
  AfxInitRichEdit2();

  // Init image/controlinfo library
  InitImages();

  // Register scintilla window class
  if (!Scintilla_RegisterClasses(m_hInstance))
  {
    MessageBox(_T("Cannot register the scintilla editor classes. Please reinstall"),_T("Starterror"),MB_OK|MB_ICONHAND);
    return FALSE;
  }
	// Standard initialization

  // Register our main key for this application
	SetRegistryKey(_T(REGISTER_APP));

	LoadStdProfileSettings(15);  // Load standard INI file options (including MRU)

  InitContextMenuManager();
  InitKeyboardManager();
  InitTooltipManager();

  CMFCToolTipInfo ttParams;
  ttParams.m_bVislManagerTheme = TRUE;
  theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,RUNTIME_CLASS(CMFCToolTipCtrl),&ttParams);

  // Load the recent project list
  m_pRecentProjectList = new CRecentFileList(0,_T("RecentProjects"),_T("Project%d"),15,50);
  m_pRecentProjectList->ReadList();

  // Needed for toolbar reloads
  CMFCToolBar::AddToolBarForImageCollection(IDR_MAINFRAME);
  CMFCToolBar::EnableQuickCustomization();
  CMFCToolBarComboBoxButton comboDummy;
  GetCmdMgr()->EnableMenuItemImage(FALSE);

	// Register the application's document templates.  Document templates
	// serve as the connection between documents, frame windows and views.

  // HTML, HTM Topic Document template
  CMultiDocTemplate* pHTMLDocTemplate;
  pHTMLDocTemplate = new CMultiDocTemplate(IDR_MAINFRAME
                                          ,RUNTIME_CLASS(CHTMLEdDoc)
                                          ,RUNTIME_CLASS(CMDIChildFrame)
                                          ,RUNTIME_CLASS(CHTMLEdView));
  if (!pHTMLDocTemplate)
  {
    Panic(_T("HTML document template not loaded"));
  }  
  AddDocTemplate(pHTMLDocTemplate);

  // JS Javascript template
	CMultiDocTemplate* pJScriptDocTemplate;
	pJScriptDocTemplate = new CMultiDocTemplate(IDR_JAVASCRIPT,
 		                                         RUNTIME_CLASS(CScintillaScriptDoc),
 		                                         RUNTIME_CLASS(ScintillaChildFrame), // custom MDI child frame
 		                                         RUNTIME_CLASS(CScintillaScriptView));
  if(!pJScriptDocTemplate)
  {
    Panic(_T("J-Script document template not loaded"));
  }
	AddDocTemplate(pJScriptDocTemplate);

  // VB VisualBasic template
  CMultiDocTemplate* pVBScriptDocTemplate;
  pVBScriptDocTemplate = new CMultiDocTemplate(IDR_VBSCRIPT,
                                               RUNTIME_CLASS(CScintillaScriptDoc),
                                               RUNTIME_CLASS(ScintillaChildFrame), // custom MDI child frame
                                               RUNTIME_CLASS(CScintillaScriptView));
  if(!pVBScriptDocTemplate)
  {
    Panic(_T("VB-Script document template not loaded"));
  }
  AddDocTemplate(pVBScriptDocTemplate);

  // create main MDI Frame window
  MainFrame* pMainFrame = new MainFrame;
  m_pMainWnd = pMainFrame;

  EnableLoadWindowPlacement(FALSE);
  if(!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
  {
    return FALSE;
  }

  // Parse our own parameters before opening files in ProcessShellCommand
  CString commandLine(GetCommandLine());
  // Find directory of this binary executable program
  SetBinDir();
  ParseOptions(commandLine);

  // Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

  // No default opening of a new document
  if(cmdInfo.m_nShellCommand == CCommandLineInfo::FileNew)
  {
    cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;
  }
  else
  {
    // Dispatch commands specified on the command line
    if (!ProcessShellCommand(cmdInfo))
    {
      return FALSE;
    }
  }
	// The one and only window has been initialized, so show and update it.
  m_nCmdShow = SW_SHOWMAXIMIZED;

  // The main window has been initialized, so show and update it.
  if(!ReloadWindowPlacement(pMainFrame))
  {
    pMainFrame->ShowWindow(m_nCmdShow);
    pMainFrame->UpdateWindow();
  }

  // Show first MDI window maximized if not already so.
  BOOL alreadyMax;
  CMDIChildFrame* first = (CMDIChildFrame*)pMainFrame->MDIGetActive(&alreadyMax);
  if(first)
  {
    if(!alreadyMax)
    {
      first->MDIMaximize();
    }
  }
  else
  {
    ((MainFrame*)pMainFrame)->SetMainMenu(false);
    if(m_projectFile == NULL)
    {
      // Do the startup dialog
      PostMessage(pMainFrame->GetSafeHwnd(),WM_COMMAND,ID_STARTUP,0);
      // OnStartup();
    }
  }
	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();

  return TRUE;
}

int
AuthorHTMLApp::ExitInstance()
{
  // Free Scintilla 
  Scintilla_ReleaseResources();

  // Write/reset the project
  ResetProject();

  // Free recent project list
  m_pRecentProjectList->WriteList();

  delete m_pRecentProjectList;
  m_pRecentProjectList = NULL;
  // Normal exit
  return CWinAppEx::ExitInstance();
}

void
AuthorHTMLApp::ParseOptions(CString& commandLine)
{
  m_baseDir = Misc::GetParamFromCommandline(commandLine,_T("/BASE"));
  if(!m_baseDir.IsEmpty())
  {
    if(m_baseDir.Right(1) != _T('/') && m_baseDir.Right(1) != _T('\\'))
    {
      m_baseDir += _T("\\");
    }
  }
  m_project = Misc::GetParamFromCommandline(commandLine,_T("/HHP"));
  if(!m_project.IsEmpty())
  {
    if(!m_baseDir.IsEmpty())
    {
      CString message;
      message.Format(_T("The basedir option (/BASE) conflicts with the project option (/HHP)!\n")
                     _T("Basedir option: /BASE=%s\n")
                     _T("Project option: /HHP=%s\n")
                     _T("Use either but not both!!")
                    ,m_baseDir.GetString()
                    ,m_project.GetString());
      ErrorMessage(message);
      ExitInstance();
    }
    OpenProjectFile();
  }
}

// New documents ID
int
AuthorHTMLApp::GetUniqueDocID()
{
  return ++m_uniqueDocID;
}

void
AuthorHTMLApp::Panic(CString message)
{
  CString mess;
  mess.Format(_T("Panic: AuthorHTML cannot continue. A serious condition has arisen:\n%s"),message.GetString());
  MessageBox(mess,_T("PANIC!!"),MB_OK|MB_ICONHAND);
}

BOOL 
AuthorHTMLApp::OnIdle(LONG lCount)
{
	if (lCount <= 0)
	{
    // Does WM_IDLEUPDATECMDUI processing and so
    // activates all OnCommandUI-like functions in HTMLEdView
		CWinThread::OnIdle(lCount);
	}
  else if (lCount == 1)
  {
    if(m_sweep && m_projectFile)
    {
      // Reset m_sweep prior to the sweeping proces
      // Sweeping can so re-sweep the project after adding files
      m_sweep = false;

      // Remove page indexes
      if(m_reindex && m_indexFile)
      {
        m_reindex = false;
        m_indexFile->RemovePageIndexes();
        m_projectFile->ResetSweeped();
        m_projectFile->ResetMetadataRead();
      }
      m_projectFile->SwipeProject();
    }
  }
	else if (lCount == 2)
	{
    // Does Lock/Unlock TempMaps and Check Memory
		VERIFY(!CWinThread::OnIdle(lCount));

    CMDIChildFrame* first = dynamic_cast<CMDIChildFrame*> (((MainFrame *)m_pMainWnd)->MDIGetActive());
    if(first)
    {
      CHTMLEdView* web = first->GetWebView();
      web->SpellCheckOnIdle();
    }
	}
	return lCount < 2;  // more to do if lCount < 1
}

/////////////////////////////////////////////////////////////////////////////
// AuthorHTMLApp message handlers

void
AuthorHTMLApp::OnFileOpen()
{
  DocFileDialog diag(true
                    ,_T("Open a document")
                    ,_T("html")
                    ,_T("")
                    ,0
                    ,_T("Documentation files (*.htm, *.html)|*.htm;*.html|")
                     _T("Author Documentation Project(*.adp)|*.adp|")
                     _T("Documentation project (*.hhp)|*.hhp|")
                     _T("VB-Script files (*.vbs)|*.vbs|")
                     _T("J-Script files (*.js)|*.js|")
                     _T("All files|*.*"));
  if(diag.DoModal() == IDOK)
  {
    CString file = diag.GetChosenFile();
    OpenTypedDocumentFile(file);
  }
}

void
AuthorHTMLApp::OnProjectOpen()
{
  DocFileDialog diag(true
                    ,_T("Open a documentation project")
                    ,_T("hpp")
                    ,_T("")
                    ,0
                    ,_T("Documentation project (*.hhp)|*.hhp|")
                     _T("Author Documentation Project(*.adp)|*.adp|")
                     _T("All files|*.*"));
  if(diag.DoModal() == IDOK)
  {
    CString file = diag.GetChosenFile();
    OpenTypedDocumentFile(file);
  }
}

void
AuthorHTMLApp::OnNewFile()
{
  CWinAppEx::OnFileNew();
}

void
AuthorHTMLApp::OnNewProject()
{
  DocFileDialog diag(false
                    ,_T("Create a new documentation project")
                    ,_T("hpp")
                    ,_T("")
                    ,0
                    ,_T("Documentation project (*.hhp)|*.hhp|"));
  if(diag.DoModal() == IDOK)
  {
    CString newProjectFile = diag.GetChosenFile();
    if(newProjectFile.GetLength() > 3)
    {
      m_project = newProjectFile;
      OpenProjectFile(true);
    }
  }
}

void 
AuthorHTMLApp::OnFileOpenurl() 
{
  CGetURL dlg;
  if(IDOK == dlg.DoModal())	
  {
    CString url = dlg.GetURL();
    if(!url.IsEmpty())
    {
      OpenTypedDocumentFile(url);
    }
  }
}

void
AuthorHTMLApp::OpenProjectFile(bool p_create /*=false*/)
{
  if(m_projectFile)
  {
    m_projectFile->WriteProjectFile();
    delete m_projectFile;
  }
  m_baseDir = Misc::DirectoryPart(m_project);
  CString baseName = Misc::BasenamePart(m_project);
  if(m_baseDir.GetLength() < 3)
  {
    // Smaller than "X:\"
    CString message;
    message.Format(_T("The project option must provide a full path to the project file:\n")
                   _T("e.g.: X:\\Directory1\\Directory2\\projectname.hhp\n")
                   _T("but you supplied: /HHP=%s"),m_project.GetString());
    ErrorMessage(message);
    ExitInstance();
  }
  m_projectFile = new ProjectFile(m_project);
  if(!m_projectFile->ReadProjectFile() && !p_create)
  {
    CString message;
    message.Format(_T("Error reading HHP project file: %s"),m_project.GetString());
    ErrorMessage(message);
    return;
  }
  if(p_create)
  {
    // Create all three files: HHP, HHC and HHK
    OpenContentsFile(m_baseDir + baseName + _T(".hhc"),true);
    OpenIndexFile   (m_baseDir + baseName + _T(".hhk"),true);
    m_projectFile->CreateNewDefaultProject(baseName);
  }

  // Check minimal contents file
  CString contents = m_projectFile->GetContentsFile();
  if(contents.IsEmpty())
  {
    contents = baseName + _T(".hhc");
    m_projectFile->SetContentsFile(contents);
  }
  CString contentsFile = m_baseDir + contents;
  OpenContentsFile(contentsFile,p_create);

  // Check minimal index file
  CString index = m_projectFile->GetIndexFile();
  if(index.IsEmpty())
  {
    index = baseName + _T(".hhk");
    m_projectFile->SetIndexFile(index);
  }
  CString indexFile = m_baseDir + index;
  OpenIndexFile(indexFile,p_create);

  // Check minimal compiled name
  CString compiled = m_projectFile->GetCompiledName();
  if(compiled.IsEmpty())
  {
    m_projectFile->SetCompiledName(baseName);
  }

  // Sync fontname/size between project/toc
  SyncFontNameAndSize();

  // Now set a new subtree for the HTML documentation site
  MainFrame* _tmain = (MainFrame*)m_pMainWnd;
  _tmain->ShowPane(ID_PANE_EXPLORER);
  _tmain->SetSubtree(m_baseDir);

  // And redisplay the TOC
  _tmain->ShowPane(ID_PANE_TOC);

  m_pRecentProjectList->Add((LPCTSTR)m_project);

  // Sweep the project as soon as we have idle time to spare
  // Collecting topic meta-data and keyword data for the index
  m_sweep = true;
}

void 
AuthorHTMLApp::SyncFontNameAndSize()
{
  if(!m_contentFile || !m_projectFile)
  {
    return;
  }
  MainFrame* _tmain = (MainFrame*)m_pMainWnd;

  CString prj_font = m_projectFile->GetDefaultFont();
  CString toc_font = m_contentFile->GetFontName();
  int     toc_size = m_contentFile->GetFontSize();

  // Standard is the fontname/size of the TOC file.
  if(!toc_font.IsEmpty() && toc_size > 0)
  {
    _tmain->m_wndTOCView.SetFontSize(toc_size,toc_font);
    return;
  }

  // Use the font definition of the project file
  if(!prj_font.IsEmpty())
  {
    std::vector<XString> options;
    XString font(prj_font);
    SplitString(font,options,_T(','),true);

    if(options.size() >= 2)
    {
      _tmain->m_wndTOCView.SetFontSize(_ttoi(options[1].GetString()),options[0].GetString());
    }
  }
}

void
AuthorHTMLApp::OpenContentsFile(CString contents,bool p_create /*=false*/)
{
  if(m_contentFile)
  {
    m_contentFile->WriteTOCFile();
    delete m_contentFile;
  }
  m_contentFile = new TOC(contents);
  if(!m_contentFile->ReadTOCFile() && !p_create)
  {
    CString message;
    message.Format(_T("Error reading HHC contents file: %s"),contents.GetString());
    ErrorMessage(message);
    return;
  }
  // Make visible in the TOC tree
  RedisplayTOC(true);
}

void
AuthorHTMLApp::RedisplayTOC(bool showPane /*=false*/)
{
  if(m_contentFile)
  {
    // Make visible in the TOC tree
    MainFrame* _tmain = (MainFrame*) m_pMainWnd;
    if(showPane)
    {
      _tmain->ShowPane(ID_PANE_TOC);
    }
    m_contentFile->DisplayTOCInTree(&(_tmain->m_wndTOCView.m_wndExplorer));
  }
}

void
AuthorHTMLApp::OpenIndexFile(CString index,bool p_create /*=false*/)
{
  if(m_indexFile)
  {
    m_indexFile->WriteIndexFile();
    delete m_indexFile;
  }
  m_indexFile = new IndexFile(index);
  if(!m_indexFile->ReadIndexFile() && !p_create)
  {
    CString message;
    message.Format(_T("Error reading HHK index file: %s"),index.GetString());
    ErrorMessage(message);
    return;
  }
  // Make visible in INDEX Tree
  RedisplayIndex(true);
}

void
AuthorHTMLApp::RedisplayIndex(bool showPane /*=false*/)
{
  if(m_indexFile)
  {
    MainFrame* _tmain = (MainFrame*) m_pMainWnd;
    if(showPane)
    {
      _tmain->ShowPane(ID_PANE_KEYWORDS);
    }
    m_indexFile->SortIndex();
    m_indexFile->DisplayIndexInTree(&(_tmain->m_wndIndexView.m_wndExplorer));
  }
}

void
AuthorHTMLApp::OpenTypedDocumentFile(CString& file)
{
  CString anchor;
  CString bareFilename;

  // Split off the anchor and the extension
  Misc::SplitMidpageAnchor(file,bareFilename,anchor);
  CString extension = Misc::ExtensionPart(bareFilename);
  extension.MakeLower();

  // Process by extension, beginning at the highest level
  if(extension.CompareNoCase(_T(".hhp")) == 0)
  {
    // Microsofts HTMLHelp project format
    m_project = file;
    OpenProjectFile();
    return;
  }
  if(extension.CompareNoCase(_T(".adp")) == 0)
  {
    // Author Documentation Project file format
    return;
  }
  // *.HTML | *.JS | *.VBS
  // We have document templates for these!!
  // Call default MFC Document template framework
  if(extension.CompareNoCase(_T(".html")) == 0 ||
     extension.CompareNoCase(_T(".htm") ) == 0 ||
     extension.CompareNoCase(_T(".js")  ) == 0 ||
     extension.CompareNoCase(_T(".vbs") ) == 0 ||
     extension.CompareNoCase(_T(".htt") ) == 0  )
  {
    CString filename = Misc::StripFileProtocol(bareFilename);
    CDocument* doc = OpenDocumentFile(filename);
    if(!anchor.IsEmpty())
    {
      CHTMLEdDoc* document = (CHTMLEdDoc*) doc;
      CHTMLEdView* view    = (CHTMLEdView*)document->GetView();
      // Navigate to FULL URL
      view->Navigate2(file);
    }
    return;
  }
  if(extension.CompareNoCase(_T(".css")) == 0)  
  {
    CSSVector list;
    SheetDef  def;
    def.filename = file;
    def.csstext  = _T("css/text");
    def.link     = NULL;
    def.style    = NULL;
    list.push_back(def);

    CString type = _T("Style sheet in CSS file");
    CString base = Misc::DirectoryPart(file);
    StyleSheetsDlg dlg(m_pMainWnd,type,list,NULL,base);
    dlg.DoModal();
    return;
  }
  CString msg;
  msg.Format(_T("AuthorDocumentation doesn't know how to open: %s"),file.GetString());
  MessageBox(msg,_T("File error"),MB_OK|MB_ICONERROR);
}

void
AuthorHTMLApp::SetBinDir()
{
  m_binDir = GetExePath();
}

Spelling*
AuthorHTMLApp::GetSpeller()
{
  if(!m_speller)
  {
    m_speller = new Spelling();
  }
  return m_speller;
}

void
AuthorHTMLApp::ResetSpeller()
{
  if(m_speller)
  {
    delete m_speller;
    m_speller = NULL;
  }
}

void
AuthorHTMLApp::OnFileSaveAll()
{
  MainFrame*  _tmain  = (MainFrame*) m_pMainWnd;
  CMDIChildFrame* first = NULL;
  CMDIChildFrame* child = (CMDIChildFrame*) _tmain->MDIGetActive();

  try
  {
    // Block the recalculation of the link pages
    _tmain->LockLinkPages(true);

    // Cycle through all the documents to save them
    while(child && child != first)
    {
      CHTMLEdDoc* doc = (CHTMLEdDoc*) child->GetActiveDocument();
      doc->SaveMyDocument();
      if(!first)
      {
        first = child;
      }
      _tmain->MDINext();
      child = (CMDIChildFrame*) _tmain->MDIGetActive();
    }

    // Save the global project files
    if(m_projectFile)
    {
      m_projectFile->WriteProjectFile();
    }
    if(m_contentFile)
    {
      m_contentFile->WriteTOCFile();
    }
    if(m_indexFile)
    {
      m_indexFile->WriteIndexFile();
    }
  }
  catch(...)
  {
    CString message(_T("Error in saving the document state"));
    ErrorMessage(message);
  }
  // Free the recalculation of the link pages
  _tmain->LockLinkPages(false);

  // Make sure we are where we started off
  if(first)
  {
    _tmain->MDIActivate(first);
  }
}

void
AuthorHTMLApp::OnCloseProject()
{
  if(m_projectFile)
  {
    if(MessageBox(_T("Do you want to close the current documentation project?")
                 ,_T("Close project")
                 ,MB_YESNO|MB_DEFBUTTON2|MB_ICONQUESTION|MB_TASKMODAL) == IDYES)
    {
      MainFrame*  _tmain  = (MainFrame*) m_pMainWnd;
      CMDIChildFrame* child = (CMDIChildFrame*) _tmain->MDIGetActive();
      while(child)
      {
        CHTMLEdDoc* doc = (CHTMLEdDoc*) child->GetActiveDocument();
        doc->SaveMyDocument();
        doc->OnCloseDocument();
        child = (CMDIChildFrame*) _tmain->MDIGetActive();
      }
      // Now reset project
      ResetProject();
      // Close the project panes
      _tmain->CloseAllPanes();
      // Do startup again
      ::PostMessage(theApp.m_pMainWnd->GetSafeHwnd(),WM_COMMAND,ID_STARTUP,0);
    }
  }
}

void
AuthorHTMLApp::OnStartup()
{
  if(m_startup == NULL)
  {
    m_startup = new StartupDlg(m_pMainWnd);
    m_startup->DoModal();
    delete m_startup;
    m_startup = NULL;
  }
}

void
AuthorHTMLApp::OnImport()
{
  ImportDlg dlg(m_pMainWnd);
  if(dlg.DoModal() == IDOK)
  {
    m_project = dlg.GetHHPFilename();
    OpenProjectFile(false);
    if(dlg.GetDoSweep() == false)
    {
      if(m_projectFile)
      {
        m_projectFile->BlockSweep();
      }
    }

  }
}

void
AuthorHTMLApp::OnCompile()
{
  if(m_projectFile)
  {
    // Save everything including project/index/toc on compile
    OnFileSaveAll();

    CString basedir     = m_projectFile->GetBaseDirectory();
    CString projectName = m_projectFile->GetProjectFilename();
    int     compileSize = m_projectFile->FindCompileLength();
    // Make sure we can see it
    MainFrame *pFrame = (MainFrame*) m_pMainWnd;
    pFrame->ShowPane(ID_PANE_COMPILE);
    pFrame->m_wndCompileView.CompileHelp(basedir,projectName,compileSize);
  }
}

void
AuthorHTMLApp::OnReadHelp()
{
  if(m_projectFile)
  {
    MainFrame *pFrame = (MainFrame*) m_pMainWnd;
    CString basedir  = m_projectFile->GetBaseDirectory();
    CString helpname = m_projectFile->GetCompiledName();
    CString path = basedir + helpname;
    ShellExecute(pFrame->m_hWnd,_T("open"),path.GetString(),NULL,NULL,SW_SHOW);
  }
}

void
AuthorHTMLApp::OnHasProject(CCmdUI* pCmdUI)
{
  pCmdUI->Enable(m_projectFile ? TRUE : FALSE);
}

void
AuthorHTMLApp::OnHasNoProject(CCmdUI* pCmdUI)
{
  pCmdUI->Enable(m_projectFile ? FALSE : TRUE);
}

void
AuthorHTMLApp::OnHasContent(CCmdUI* pCmdUI)
{
  bool hasNoContent = m_projectFile ? FALSE : TRUE;
  if(hasNoContent)
  {
    MainFrame* _tmain = (MainFrame*) m_pMainWnd;
    BOOL maxim;
    if(_tmain->MDIGetActive(&maxim))
    {
      hasNoContent = FALSE;
    }
  }
  pCmdUI->Enable(hasNoContent);
}

void
AuthorHTMLApp::OnProjectSettings()
{
  if(m_projectFile)
  {
    ProjectDlg dlg(m_pMainWnd,m_projectFile);
    dlg.DoModal();
  }
}

void
AuthorHTMLApp::OnWindowDefinitions()
{
  if(m_projectFile)
  {
    WindowDefDlg dlg(m_pMainWnd,m_projectFile,_T(""));
    dlg.DoModal();
  }
}

///////////////////////////////////////////////////////////////////
//
// HELP MENU REFERENCE MANUALS
//
///////////////////////////////////////////////////////////////////

void
AuthorHTMLApp::OnManualMicrosoftHTML()
{
  CString file = theApp.GetBinDirectory();
#ifdef DEBUG
  file += _T("..\\");
#endif
  file += _T("..\\doc\\htmlref.chm");
  ShellExecute(m_pMainWnd->GetSafeHwnd(),_T("open"),file,_T(""),_T(""),SW_SHOWMAXIMIZED);
}

void
AuthorHTMLApp::OnManualW3C_HTML()
{
  CString file = theApp.GetBinDirectory();
#ifdef DEBUG
  file += _T("..\\");
#endif
  file += _T("..\\doc\\W3C_HTML401.pdf");
  ShellExecute(m_pMainWnd->GetSafeHwnd(),_T("open"),file,_T(""),_T(""),SW_SHOWMAXIMIZED);
}

void
AuthorHTMLApp::OnManualW3C_CSS()
{
  CString file = theApp.GetBinDirectory();
#ifdef DEBUG
  file += _T("..\\");
#endif
  file += _T("..\\doc\\W3C_CSS2.pdf");
  ShellExecute(m_pMainWnd->GetSafeHwnd(),_T("open"),file,_T(""),_T(""),SW_SHOWMAXIMIZED);
}

void
AuthorHTMLApp::SuppressMessage(CString message)
{
  std::set<CString>::iterator it;
  it = m_messages.find(message);
  if(it == m_messages.end())
  {
    m_messages.insert(message);
  }
}

bool
AuthorHTMLApp::IsSuppressedMessage(CString message)
{
  std::set<CString>::iterator it = m_messages.find(message);
  return it != m_messages.end();
}

int
AuthorHTMLApp::GetButtonLayout()
{
  // TODO : FOR NOW
  // BUTT_LAYOUT_LEFT
  return 2;
}

ControlsInfo*
AuthorHTMLApp::GetControlsInfo()
{
  if(!m_controlsInfo)
  {
    m_controlsInfo = new ControlsInfo();
  }
  return m_controlsInfo;
}

void
AuthorHTMLApp::InitImages()
{
  m_images = Ref<CAfbeeldingen>(new CAfbeeldingen());

  m_images->RegistreerExtensie(_T(".BMP"),_T("")    ,0);
  m_images->RegistreerExtensie(_T(".IML"),_T("S")   ,0);
  m_images->RegistreerExtensie(_T(".IM2"),_T("SP")  ,0);
  m_images->RegistreerExtensie(_T(".IM3"),_T("SFP") ,0);
  m_images->RegistreerExtensie(_T(".IBL"),_T("S")   ,200);
  m_images->RegistreerExtensie(_T(".IB2"),_T("SP")  ,200);
  m_images->RegistreerExtensie(_T(".IB3"),_T("SFP") ,200);

  // Image libraries
  m_images->MaakResourceEntry(IDB_ALG,            _T("ALG")         ,_T("S")   , 0,  0);
  m_images->MaakResourceEntry(IDB_ALG32,          _T("ALG32")       ,_T("S")   , 32, 0);
  m_images->MaakResourceEntry(IDB_ARROWS,         _T("ARROWS")      ,_T("4")   , 0,  0);
  m_images->MaakResourceEntry(IDB_ARROWS_WIDE,    _T("ARROWS_WIDE") ,_T("2")   , 32, 0);
  m_images->MaakResourceEntry(IDB_BUTTONS1,       _T("KNOP")        ,_T("SFP") , 0,  33);
  m_images->MaakResourceEntry(IDB_BUTTONS2,       _T("KNOP2")       ,_T("SFP") , 0,  14);
  m_images->MaakResourceEntry(IDB_COMPUTER,       _T("COMP")        ,_T("S")   , 0,  0);
  m_images->MaakResourceEntry(IDB_DOCUMENTS,      _T("DOC")         ,_T("S")   , 0,  0);
  m_images->MaakResourceEntry(IDB_INTERN1,        _T("P")           ,_T("10")  , 0,  0, IDB_INTERN2);
  m_images->MaakResourceEntry(IDB_OFFICE,         _T("OFFICE")      ,_T("S")   , 0,  0);
  m_images->MaakResourceEntry(IDB_SCIENCE,        _T("TECH")        ,_T("S")   , 0,  0);
  m_images->MaakResourceEntry(IDB_SIGNS,          _T("SIGNS")       ,_T("S")   , 0,  0);
  m_images->MaakResourceEntry(IDB_WINDOWS,        _T("WINDOWS")     ,_T("X")   , 0,  10);
  m_images->MaakResourceEntry(IDB_ICONEXCLAMATION,_T("EXCLAMATION") ,_T("")    , 0,  0);
  m_images->MaakResourceEntry(IDB_ICONHAND,       _T("STOP")        ,_T("")    , 0,  0);
  m_images->MaakResourceEntry(IDB_ICONASTERISK,   _T("INFORMATION") ,_T("")    , 0,  0);
  m_images->MaakResourceEntry(IDB_ICONQUESTION,   _T("QUESTION")    ,_T("")    , 0,  0);
  
  ControlsInfo::MaakDefaultAfbeelding(m_images);

}

CString
AuthorHTMLApp::MessageBox(const CString& tekst
                         ,const CString& titel
                         ,const CString& knoppen)
{
  MainFrame::SetStatusText(tekst);
  HWND focuswin = ::GetFocus();

  AuMessageBox msg(m_pMainWnd,titel.GetString(), tekst.GetString(),knoppen);
  int id = msg.DoModal();
  CString resultstr = msg.GeefResultaat(id);
  if(::IsWindow(focuswin)) 
  {
    ::SetFocus(focuswin);
  }
  MainFrame::SetStatusText(_T(""));
  return resultstr;
}

int
AuthorHTMLApp::MessageBox(const CString& text,
                          const CString& title,
                                UINT     uType)
{
  MainFrame::SetStatusText(text);
  HWND focuswin = ::GetFocus();

  int res = WideMessageBox(focuswin,text,title,uType);
  if(::IsWindow(focuswin)) 
  {
    ::SetFocus(focuswin);
  }
  MainFrame::SetStatusText(_T(""));
  return res;
}

int
AuthorHTMLApp::ErrorMessage(const CString& message)
{
  MainFrame::SetStatusText(message);
  HWND focuswin = ::GetFocus();

  int res = WideMessageBox(focuswin,message,_T("ERROR"),MB_OK|MB_ICONERROR);
  if(::IsWindow(focuswin)) 
  {
    ::SetFocus(focuswin);
  }
  MainFrame::SetStatusText(_T(""));
  return res;
}

BOOL      
AuthorHTMLApp::CanExitInstance()
{
  bool canExit = true;

  if(m_projectFile && m_projectFile->NeedSaving())
  {
    if(!m_projectFile->WriteProjectFile()) canExit = false;
  }
  if(m_contentFile && m_contentFile->GetNeedSaving())
  {
    if(!m_contentFile->WriteTOCFile()) canExit = false; 
  }
  if(m_indexFile && m_indexFile->NeedSaving())
  {
    if(!m_indexFile->WriteIndexFile()) canExit = false;
  }
  // See if a compilation is running
  MainFrame* _tmain= (MainFrame*) m_pMainWnd;
  if(_tmain->m_wndCompileView.CompilationRunning())
  {
    if(MessageBox(_T("A help file compilation is still running. Do you still wish to exit this application?")
                 ,_T("Working...")
                 ,MB_YESNO|MB_DEFBUTTON2|MB_ICONEXCLAMATION) == IDNO)
    {
      canExit = false;
    }
  }
  return canExit;
}

void 
AuthorHTMLApp::OnAppExit()
{
  if(CanExitInstance())
  {
    // same as double-clicking on main window close box
    ASSERT(m_pMainWnd != NULL);
    m_pMainWnd->SendMessage(WM_CLOSE);
  }
}

BOOL
AuthorHTMLApp::SaveAllModified()
{
  if(!CWinAppEx::SaveAllModified())
  {
    return FALSE;
  }
  MainFrame* _tmain = reinterpret_cast<MainFrame*>(m_pMainWnd);
  if(_tmain)
  {
    SaveState(_tmain,_T("Layout"));
  }
  return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class AboutDlg : public CDialog
{
public:
  AboutDlg();
  virtual BOOL OnInitDialog() override;
  // Dialog Data
  enum { IDD = IDD_ABOUTBOX };
  // ClassWizard generated virtual function overrides
protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  CString       m_text;
  CRichEditCtrl m_edit;
  // Implementation
protected:
  DECLARE_MESSAGE_MAP()
};

AboutDlg::AboutDlg() 
         :CDialog(IDD_ABOUTBOX)
{
}

void 
AboutDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Control(pDX,IDC_TEXT,m_edit);
}

BEGIN_MESSAGE_MAP(AboutDlg, CDialog)
END_MESSAGE_MAP()

BOOL
AboutDlg::OnInitDialog()
{
  CDialog::OnInitDialog();
  m_edit.SetTextMode(TM_RICHTEXT);
  m_text = _T("{\\rtf1\\ansi\\ansicpg1252\\deff0\\deflang1043{\\fonttbl{\\f0\\fnil\\fprq1\\fcharset238 r_eeurope;}{\\f1\\fnil\\fcharset0 Calibri;}}\n")
           _T("{\\colortbl ;\\red163\\green21\\blue21;}\n")
           _T("\\viewkind4\\uc1\\pard\\sl240\\slmult1\\cf1\\b\\f0\\fs20 AUTHOR DOCUMENTATION\\par\\par\n")
           _T("\\b0 A authoring tool for writing of\\par on-line documentation. Contains\\par\n")
           _T("complete single-source genera-\\par tion of on-line help systems.\\par\n")
           _T("\\par")
           _T("\\b Active subsystems:\\par\n")
           _T("\\b0 - CSS Parser\\par\n")
           _T("- XML Parser\\par\n")
           _T("- HTML 4.1 cleaner\\par\n")
           _T("- Microsoft CHM compiler\\par\n")
           _T("- Scintilla editor\\par\n")
           _T("- Skinning engine\\par\n")
           _T("\\par")
           _T("\\b Version: \\b0 ") VERSION_NUMBER _T("\\cf0\\par\n")
           _T("\\cf1\\b Build: \\f1      \\b0\\f0 ") STRINGIZE(BUILD_NUMBER) _T("\\par\n")
           _T("\\cf0\\par\n")
           _T("\\cf1\\b Dates: \\b0 ") VERSION_DATES _T("\\par\n")
           _T("Written by: ir. W.E. Huisman\\cf0\\lang19\\f1\\fs22\\par");
        
  m_edit.ReplaceSel(m_text);
  UpdateData(FALSE);
  return FALSE;
}

// App command to run the dialog
void
AuthorHTMLApp::OnAppAbout()
{
  AboutDlg aboutDlg;
  aboutDlg.DoModal();
}
