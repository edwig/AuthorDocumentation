//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    ProjectDlg.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Manages the project (*.hpp) file settings
//
#include "stdafx.h"
#include "AuthorHTML.h"
#include "ProjectDlg.h"
#include "FindTopicDlg.h"
#include "Misc.h"
#include "WindowDefDlg.h"
#include "TOC.h"
#include <StringUtilities.h>

// ProjectDlg dialog
IMPLEMENT_DYNAMIC(ProjectDlg, CDialog)

ProjectDlg::ProjectDlg(CWnd* p_parent,ProjectFile* p_project)
	         :CDialog(ProjectDlg::IDD, p_parent)
           ,m_project(p_project)
{
}

ProjectDlg::~ProjectDlg()
{
}

void ProjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
  DDX_Text   (pDX,IDC_PRONAME,      m_projectName);
  DDX_Text   (pDX,IDC_COMPILEDNAME, m_compiledName);
  DDX_Text   (pDX,IDC_TITLE,        m_title);
  DDX_Text   (pDX,IDC_DEFAULTTOPIC, m_defaultTopic);
  DDX_Control(pDX,IDC_BUTTOTOPIC,   m_buttonDefTopic);
  DDX_Text   (pDX,IDC_DEFAULTFONT,  m_defaultFont);
  DDX_Control(pDX,IDC_BUTTONFONT,   m_buttonDefFont);
  DDX_Control(pDX,IDC_DEFWINDOW,    m_comboWindow);
  DDX_Control(pDX,IDC_EDITWINDOWS,  m_buttonEditWindows);
  DDX_Control(pDX,IDC_TEXTSEARCH,   m_buttonFullSearch);
  DDX_Control(pDX,IDC_BINARYINDEX,  m_buttonBinIndex);
  DDX_Control(pDX,IDC_AUTOINDEX,    m_buttonAutoIndex);
  DDX_Control(pDX,IDC_AUTOTOC,      m_buttonAutoTOC);
  DDX_Control(pDX,IDC_FLATTOC,      m_buttonFlatTOC);
  DDX_Control(pDX,IDC_ENHANCED,     m_buttonEnhanced);

  DDX_Control(pDX,IDOK,             m_buttonOK);
  DDX_Control(pDX,IDCANCEL,         m_buttonCancel);
  DDX_Control(pDX,IDC_HELPPROJECT,  m_buttonHelp);

  if(pDX->m_bSaveAndValidate == FALSE)
  {
    int ind = m_comboWindow.FindString(0,m_defaultWindow);
              m_comboWindow.SetCurSel(ind);
  }
}


BEGIN_MESSAGE_MAP(ProjectDlg, CDialog)
  ON_EN_KILLFOCUS (IDC_COMPILEDNAME, &ProjectDlg::OnEnChangeCompiledname)
  ON_EN_KILLFOCUS (IDC_TITLE,        &ProjectDlg::OnEnChangeTitle)
  ON_EN_KILLFOCUS (IDC_DEFAULTTOPIC, &ProjectDlg::OnEnChangeDefaulttopic)
  ON_BN_CLICKED   (IDC_BUTTOTOPIC,   &ProjectDlg::OnBnClickedButtotopic)
  ON_EN_KILLFOCUS (IDC_DEFAULTFONT,  &ProjectDlg::OnEnChangeDefaultfont)
  ON_BN_CLICKED   (IDC_BUTTONFONT,   &ProjectDlg::OnBnClickedButtonfont)
  ON_CBN_SELCHANGE(IDC_DEFWINDOW,    &ProjectDlg::OnCbnSelchangeDefwindow)
  ON_BN_CLICKED   (IDC_EDITWINDOWS,  &ProjectDlg::OnBnClickedEditwindows)
  ON_BN_CLICKED   (IDC_TEXTSEARCH,   &ProjectDlg::OnBnClickedTextsearch)
  ON_BN_CLICKED   (IDC_BINARYINDEX,  &ProjectDlg::OnBnClickedBinaryindex)
  ON_BN_CLICKED   (IDC_AUTOINDEX,    &ProjectDlg::OnBnClickedAutoindex)
  ON_BN_CLICKED   (IDC_AUTOTOC,      &ProjectDlg::OnBnClickedAutoTOC)
  ON_BN_CLICKED   (IDC_FLATTOC,      &ProjectDlg::OnBnClickedFlatTOC)
  ON_BN_CLICKED   (IDC_ENHANCED,     &ProjectDlg::OnBnClickedEnhanced)
  ON_BN_CLICKED   (IDOK,             &ProjectDlg::OnBnClickedOk)
  ON_BN_CLICKED   (IDCANCEL,         &ProjectDlg::OnBnClickedCancel)
  ON_BN_CLICKED   (IDC_HELPPROJECT,  &ProjectDlg::OnBnClickedHelpproject)
END_MESSAGE_MAP()

BOOL
ProjectDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  m_projectName    = Misc::FilenamePart(m_project->GetProjectFilename());
  m_compiledName   = m_project->GetCompiledName();
  m_title          = m_project->GetTitle();
  m_defaultTopic   = m_project->GetDefaultTopic();
  m_defaultFont    = m_project->GetDefaultFont();
  m_defaultWindow  = m_project->GetDefaultWindow();
  m_fullTextSearch = m_project->GetFullTextSearch();
  m_binaryIndex    = m_project->GetBinaryIndex();
  m_autoIndex      = m_project->GetAutoIndex();
  m_autoTOC        = m_project->GetAutoTOC();
  m_flatTOC        = m_project->GetFlatTOC();
  m_enhanced       = m_project->GetEnhancedDecompilation();

  m_buttonFullSearch.SetCheck(m_fullTextSearch);
  m_buttonBinIndex  .SetCheck(m_binaryIndex);
  m_buttonAutoIndex .SetCheck(m_autoIndex);
  m_buttonAutoTOC   .SetCheck(m_autoTOC);
  m_buttonFlatTOC   .SetCheck(m_flatTOC);
  m_buttonEnhanced  .SetCheck(m_enhanced);

  SetWindowNames();
  UpdateData(FALSE);
  return TRUE;
}

void
ProjectDlg::SetWindowNames()
{
  m_comboWindow.ResetContent();
  WindowNames names;
  m_project->GetWindowNames(names);
  for(unsigned int ind = 0;ind < names.size(); ++ind)
  {
    m_comboWindow.AddString(names[ind]);
  }
}

void
ProjectDlg::UpdateProject()
{
  UpdateData();

  if(m_compiledName.CompareNoCase(m_project->GetCompiledName()))
  {
    if(theApp.MessageBox("You are about to change the name of the project files. Continue?","Project name",MB_YESNO|MB_DEFBUTTON2) == IDYES)
    {
      // Project name is changed!
      m_project->SetCompiledName(m_compiledName);
      // Set project file + index file + TOC File!!
      m_project->SetProjectFilename(m_compiledName);
      // Re-read the filename
      m_projectName  = Misc::FilenamePart(m_project->GetProjectFilename());
    }
    else
    {
      // Do not change the compiled name
      m_compiledName = m_project->GetCompiledName();
      UpdateData(FALSE);
    }
  }
  if(m_title.CompareNoCase(m_project->GetTitle()))
  {
    // Project title is changed
    m_project->SetTitle(m_title);
  }
  if(m_defaultTopic.CompareNoCase(m_project->GetDefaultTopic()))
  {
    m_project->SetDefaultTopic(m_defaultTopic);
  }
  if(m_defaultFont.CompareNoCase(m_project->GetDefaultFont()))
  {
    m_project->SetDefaultFont(m_defaultFont);
  }
  if(m_defaultWindow.CompareNoCase(m_project->GetDefaultWindow()))
  {
    m_project->SetDefaultWindow(m_defaultWindow);
  }
  m_project->SetFullTextSearch(m_fullTextSearch);
  m_project->SetBinaryIndex(m_binaryIndex);
  m_project->SetAutoIndex(m_autoIndex);
  m_project->SetAutoTOC(m_autoTOC);
  m_project->SetFlatTOC(m_flatTOC);
  m_project->SetEnhancedDecompilation(m_enhanced);
}


// ProjectDlg message handlers

void ProjectDlg::OnEnChangeCompiledname()
{
  CString name;
  CWnd* w = GetDlgItem(IDC_COMPILEDNAME);
  w->GetWindowText(name);
  if(name.CompareNoCase(m_compiledName))
  {
    // Ok, Name changed
    if(theApp.MessageBox("Changing the compiled name of the project also changes the names of the index and TOC.\n"
                         "Do you really want to change the name now and recreate the index and TOC files?"
                        ,"Question"
                        ,MB_YESNO|MB_DEFBUTTON2|MB_ICONQUESTION) == IDYES)
    {
      // Project name is changed!
      m_project->SetCompiledName(name);
      // Set project file + index file + TOC File!!
      m_project->SetProjectFilename(name);
      // Re-read the filename and compiled name for the case of a files system error.
      m_projectName  = Misc::FilenamePart(m_project->GetProjectFilename());
      m_compiledName = m_project->GetCompiledName();
      UpdateData(FALSE);
    }
    else
    {
      m_compiledName = m_project->GetCompiledName();
      UpdateData(FALSE);
    }
  }
}

void 
ProjectDlg::OnEnChangeTitle()
{
  CWnd* w = GetDlgItem(IDC_TITLE);
  w->GetWindowText(m_title);
}

void 
ProjectDlg::OnEnChangeDefaulttopic()
{
  CWnd* w = GetDlgItem(IDC_DEFAULTTOPIC);
  w->GetWindowText(m_defaultTopic);
}

void 
ProjectDlg::OnBnClickedButtotopic()
{
  CString base = theApp.GetBaseDirectory();
  FindTopicDlg dlg(this,base,false);
  if(dlg.DoModal() == IDOK)
  {
    CString topic = dlg.GetFoundTopic();
    if(!topic.IsEmpty())
    {
      m_defaultTopic = topic;
      UpdateData(FALSE);
    }
  }
}

void 
ProjectDlg::OnEnChangeDefaultfont()
{
  CWnd* w = GetDlgItem(IDC_DEFAULTFONT);
  w->GetWindowText(m_defaultFont);
}

void 
ProjectDlg::OnBnClickedButtonfont()
{
  // TODO: Add your control notification handler code here
  std::vector<XString> options;
  XString font(m_defaultFont);
  SplitString(font,options, ',',true);
  if (options.size() < 1)
  {
    options.push_back("Verdana");
    if(options.size() < 2)
    {
      options.push_back("10");
      if (options.size() < 3)
      {
        options.push_back("0");
      }
    }
  }
  CString oldFontName(options[0]);
  int     oldFontSize(_ttoi(options[1].GetString()));


  LOGFONT    lFont;
  CHOOSEFONT cFont;
  ZeroMemory(&lFont,sizeof(LOGFONT));
  ZeroMemory(&cFont,sizeof(CHOOSEFONT));
 
  // Fill logic font based on current font
  lFont.lfHeight         = -MulDiv(oldFontSize,GetDeviceCaps(GetDC()->m_hDC, LOGPIXELSY),72);
  lFont.lfWidth          = 0;
  lFont.lfEscapement     = 0;
  lFont.lfOrientation    = 0;
  lFont.lfWeight         = FW_NORMAL;
  lFont.lfItalic         = FALSE;
  lFont.lfUnderline      = FALSE;
  lFont.lfStrikeOut      = FALSE;
  lFont.lfOutPrecision   = OUT_DEFAULT_PRECIS;
  lFont.lfClipPrecision  = CLIP_DEFAULT_PRECIS;
  lFont.lfQuality        = DEFAULT_QUALITY;
  lFont.lfPitchAndFamily = FF_DONTCARE;
  _tcscpy_s(lFont.lfFaceName,LF_FACESIZE,oldFontName);

  // Fill choose-a-font structure
  cFont.iPointSize  = oldFontSize;
  cFont.nSizeMin    = 8;
  cFont.nSizeMax    = 28;
  cFont.lStructSize = sizeof(CHOOSEFONT);
  cFont.hwndOwner   = GetSafeHwnd();
  cFont.lpLogFont   = &lFont;
  cFont.nFontType   = SCREEN_FONTTYPE|PRINTER_FONTTYPE|REGULAR_FONTTYPE;
  cFont.Flags       = CF_BOTH|CF_INITTOLOGFONTSTRUCT|CF_LIMITSIZE;

  // Now go choose a font
  if(!ChooseFont(&cFont))
  {
    // No other font chosen or CANCEL pressed
    return;
  }
  // Check not-used options and report
  if(lFont.lfWeight    != FW_NORMAL ||
     lFont.lfItalic    != FALSE ||
     lFont.lfUnderline != FALSE ||
     lFont.lfStrikeOut != FALSE)
  {
    theApp.MessageBox(_T("You have chosen a different font weight/style than 'normal'.\n")
                      _T("Unfortunately, AuthorHTML will ignore this choice and do nothing with it.")
                     ,_T("WARNING !!")
                     ,MB_OK | MB_ICONINFORMATION);
  }
  // Register the choice
  bool changed = false;
  if(_tcslen(lFont.lfFaceName) > 2)
  {
    changed = true;
    options[0] = lFont.lfFaceName;
  }
  if(cFont.iPointSize >= 80 && cFont.iPointSize <= 140)
  {
    changed = true;
    XString size;
    size.Format("%d",(cFont.iPointSize / 10));
    options[1] = size;
  }
  if(changed)
  {
    m_defaultFont = options[0] + "," + options[1] + ",0";
  }
  UpdateData(FALSE);
}

void 
ProjectDlg::OnCbnSelchangeDefwindow()
{
  int ind = m_comboWindow.GetCurSel();
  if(ind >= 0)
  {
    m_comboWindow.GetLBText(ind,m_defaultWindow);
  }
  UpdateData(FALSE);
}

void 
ProjectDlg::OnBnClickedTextsearch()
{
  m_fullTextSearch = m_buttonFullSearch.GetCheck() == TRUE;
}

void 
ProjectDlg::OnBnClickedBinaryindex()
{
  m_binaryIndex = m_buttonBinIndex.GetCheck() == TRUE;
  if(!m_binaryIndex && m_project->GetSweepRebuildsIndex())
  {
    theApp.MessageBox("You cannot de-activate the binary index as long as you have topics with keywords in the header defined.\n"
                      "To deactivate: move all your keywords to the keyword pane and remove them from the individual topics."
                     ,"Error"
                     ,MB_OK|MB_ICONSTOP|MB_TASKMODAL);
    m_buttonBinIndex.SetCheck(TRUE);
    m_binaryIndex = true;
  }
}

void 
ProjectDlg::OnBnClickedAutoindex()
{
  m_autoIndex = m_buttonAutoIndex.GetCheck() == TRUE;
  if(!m_autoIndex && m_project->GetSweepRebuildsIndex())
  {
    theApp.MessageBox("You cannot de-activate the use of keywords in the topic headers as long as you have keywords defined there.\n"
                      "To deactivate: move all your keywords to the keyword pane and remove them from the individual topics."
                      ,"Error"
                      ,MB_OK|MB_ICONSTOP|MB_TASKMODAL);
    m_buttonAutoIndex.SetCheck(TRUE);
    m_autoIndex = true;
  }
}

void 
ProjectDlg::OnBnClickedEditwindows()
{
  WindowDefDlg dlg(this,m_project,m_defaultWindow);
  dlg.DoModal();
  SetWindowNames();
  UpdateData(FALSE);
}

void
ProjectDlg::OnBnClickedAutoTOC()
{
  m_autoTOC = m_buttonAutoTOC.GetCheck() == TRUE;
  if(m_autoTOC)
  {
    if(theApp.GetTOC()->CountEntries())
    {
      if(theApp.MessageBox("You can only use an automatic generated TOC, when the hand-written TOC is empty.\n"
                           "Would you like to continue to remove the current TOC?"
                          ,"Table-Of-Contents"
                          ,MB_YESNO | MB_DEFBUTTON2 | MB_ICONQUESTION) == IDNO)
      {
        return;
      }
      theApp.GetTOC()->Reset();
      theApp.ReSweepProject();
      theApp.ReSweepIndex();
    }
    m_flatTOC = true;
    m_buttonFlatTOC.SetCheck(TRUE);
  }
  else
  {
    m_autoTOC = false;
    m_buttonAutoTOC.SetCheck(FALSE);
  }
  UpdateData(FALSE);
}

void
ProjectDlg::OnBnClickedFlatTOC()
{
  m_flatTOC = m_buttonFlatTOC.GetCheck() == TRUE;
}

void 
ProjectDlg::OnBnClickedEnhanced()
{
  m_enhanced = m_buttonEnhanced.GetCheck() == TRUE;
}

void
ProjectDlg::OnBnClickedOk()
{
  UpdateProject();
  OnOK();
}

void 
ProjectDlg::OnBnClickedCancel()
{
  OnCancel();
}

void 
ProjectDlg::OnBnClickedHelpproject()
{
}

