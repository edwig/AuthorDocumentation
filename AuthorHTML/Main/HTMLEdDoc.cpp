//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    HTMLEdDoc.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Document backend for an HTML documentation file
// 
#include "stdafx.h"
#include "AuthorHTML.h"
#include "ChildFrm.h"
#include "HTMLEdDoc.h"
#include "htmledview.h"
#include "geturl.h"
#include "mainfrm.h"
#include "ScintillaHTMLView.h"
#include "tidy.h"
#include <io.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Needed for HTMLTidy of saved HTML file
static FILE* errout = NULL;

/////////////////////////////////////////////////////////////////////////////
// CHTMLEdDoc

IMPLEMENT_DYNCREATE(CHTMLEdDoc, CHtmlEditDoc)

BEGIN_MESSAGE_MAP(CHTMLEdDoc, CHtmlEditDoc)
  ON_COMMAND(ID_FILE_SAVE_AS,OnSaveAs)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHTMLEdDoc construction/destruction

CHTMLEdDoc::CHTMLEdDoc()
{
	m_bDoSaveOnDocCom = FALSE;
	m_bCallNewDocument = FALSE;
	m_bCallCloseDocument = FALSE;
}

CHTMLEdDoc::~CHTMLEdDoc()
{
}


/////////////////////////////////////////////////////////////////////////////
// CHTMLEdDoc diagnostics

#ifdef _DEBUG
void CHTMLEdDoc::AssertValid() const
{
	CHtmlEditDoc::AssertValid();
}

void CHTMLEdDoc::Dump(CDumpContext& dc) const
{
	CHtmlEditDoc::Dump(dc);
}
#endif //_DEBUG

// Create a new empty HTML document
// Optionally with a document title
void
CHTMLEdDoc::CreateNewDocument(CString& p_filename,bool p_setTitle /*=true*/)
{
  CString title = Misc::BasenamePart (p_filename);
  CString path  = Misc::DirectoryPart(p_filename);
  CString ext   = Misc::ExtensionPart(p_filename);
  CString file(title);
  file.Replace(' ','_');

  CString fullPath = path + file + ext;
  FILE* fout = fopen(fullPath,"wb");
  if(fout)
  {
    fprintf(fout,"<html>\n"
                 "<head>\n"
                 "    <title>%s</title>\n"
                 "</head>\n"
                 "<body></body>\n"
                 "</html>\n", p_setTitle ? (LPCTSTR)title : "");
    fclose(fout);
  }
  m_sSaveFileName = fullPath;
  m_strPathName   = fullPath;
  m_strTitle      = file;
}


/////////////////////////////////////////////////////////////////////////////
// CHTMLEdDoc commands

//We want the default functionality of CHtmlEditDoc::OnNewDocument(), but just need to update
//the view with the new document contents
BOOL CHTMLEdDoc::OnNewDocument()
{
	//If the old document needs to be saved, and is waiting for a OnDocumentComplete,
	//OnNewDocument waits for it to gets done
	if(m_bDoSaveOnDocCom)
	{
		m_bCallNewDocument = TRUE;
		return FALSE;
	}

	BOOL bRet = CHtmlEditDoc::OnNewDocument();
	if (bRet)
	{
		// have to clear the source view too!
		MainFrame *pFrm = (MainFrame*)AfxGetMainWnd();
		if (pFrm)
		{
			//ScintillaHTMLView *pView = (ScintillaHTMLView*)pFrm->GetSourceView();
      CMDIChildFrame* frame = (CMDIChildFrame*) pFrm->MDIGetActive(); //GetActiveFrame();
      if(frame)
      {
        ScintillaHTMLView *pView = (ScintillaHTMLView*) frame->GetSourceView();
        if (pView)
        {
          pView->NewDocument();
          frame->SwapView(ID_VIEW_WEB);

          // Show empty document
          CHTMLEdView* pWeb = frame->GetWebView();
          pWeb->Navigate("about:blank");

          CString newName;
          CDocTemplate* pTemplate = GetDocTemplate();
          ASSERT(pTemplate != NULL);

          if (!AfxGetApp()->DoPromptFileName(newName
                                            ,AFX_IDS_SAVEFILECOPY
                                            ,OFN_HIDEREADONLY | OFN_PATHMUSTEXIST
                                            ,FALSE
                                            ,pTemplate))
          {
            // don't even attempt to save
            return FALSE;
          }
          CreateNewDocument(newName);
          // Navigate web view to new filename
          pWeb->Navigate(m_strPathName);
        }
      }
		}
	}
	return bRet;
}

void CHTMLEdDoc::OnCloseDocument()
{
	//If the old document needs to be saved, and is waiting for a OnDocumentComplete,
	//OnCloseDocument waits for it to gets done
	if(m_bDoSaveOnDocCom)
	{
		m_bCallCloseDocument = TRUE;
		return;
	}

	CDocument::OnCloseDocument();
}

//We want the default functionality of CHtmlEditDoc::OnOpenDocument(lpszFileName), but just
//need to update the view with the opened document contents
BOOL CHTMLEdDoc::OnOpenDocument(LPCTSTR lpszFileName)
{
  bool doNew = false;
  CString newName;
  CString oldFile;

  // Check if the file exists
  if(_access(lpszFileName,0) == -1)
  {
    CString message;
    message.Format("The file [%s] does not exist.",lpszFileName);
    theApp.MessageBox(message,"File error",MB_OK|MB_ICONERROR);
    return FALSE;
  }
  // Check if it is a template-open
  if(Misc::ExtensionPart(lpszFileName).CompareNoCase(".htt") == 0)
  {
    CString ask;
    ask.Format("Do you want to open file [%s] as a template to edit the template\n"
               "or do you want to make a copy of the template and save it as a regular document?",lpszFileName);
    if(theApp.MessageBox(ask,"Template","?open_template regular_document") == "regular document")
    {
      CDocTemplate* pTemplate = GetDocTemplate();
      ASSERT(pTemplate != NULL);
      if (AfxGetApp()->DoPromptFileName(newName
                                       ,AFX_IDS_SAVEFILECOPY
                                       ,OFN_HIDEREADONLY | OFN_PATHMUSTEXIST
                                       ,FALSE
                                       ,pTemplate))
      {
        // AFX open filename is static and must be copied _MAX_PATH chars max
        // necessary for document framework, as titles and pathnames are used
        // to be set to document, frames and menubar
        oldFile = lpszFileName;
        strncpy((char*)lpszFileName,newName.GetString(),_MAX_PATH);
        unlink(lpszFileName);
        if(!CopyFile(oldFile,lpszFileName,TRUE))
        {
          CString mess;
          mess.Format("Sorry: Cannot make a copy of the file [%s]",lpszFileName);
          theApp.ErrorMessage(mess);
          return FALSE;
        }
        doNew = true;
      }
    }
  }
  CWaitCursor take_a_deep_breath;
 	BOOL bRet = CHtmlEditDoc::OnOpenDocument(lpszFileName);
	if (bRet)
	{
		// have to clear the source view too!
		MainFrame *pFrm = (MainFrame*)AfxGetMainWnd();		
		if (pFrm)
		{
      CMDIChildFrame* frame = (CMDIChildFrame*) pFrm->MDIGetActive(); //GetActiveFrame();
      if(frame)
      {
        ScintillaHTMLView *pView = (ScintillaHTMLView*) frame->GetSourceView();
        if (pView)
        {
          pView->NewDocument();
        }
        frame->SwapView(ID_VIEW_WEB);
        CHTMLEdView* pWeb = frame->GetWebView();
        pWeb->Navigate(lpszFileName);

        if(doNew)
        {
          // Make new from *.htt template
          CString relative;
          if(Misc::MakeRelativePathname(newName,oldFile,relative))
          {
            oldFile = relative;
          }
          m_fromTemplate = oldFile;
        }
      }
		}
	}

	return bRet;
}

void
CHTMLEdDoc::OnSaveAs()
{
  CString newName;
  CDocTemplate* pTemplate = GetDocTemplate();
  ASSERT(pTemplate != NULL);

  if (!AfxGetApp()->DoPromptFileName(newName
                                    ,AFX_IDS_SAVEFILECOPY
                                    ,OFN_HIDEREADONLY | OFN_PATHMUSTEXIST
                                    ,FALSE
                                    ,pTemplate))
  {
    // don't even attempt to save
    return;       
  }
  if(Misc::ExtensionPart(newName).CompareNoCase(".htt") == 0)
  {
    CString message;
    message.Format("Do you want to save the TEMPLATE [%s]?",newName.GetString());
    if(theApp.MessageBox(message,"Sure?",MB_YESNO|MB_ICONQUESTION) == IDNO)
    {
      return;
    }
  }
  OnSaveDocument(newName);
}

BOOL CHTMLEdDoc::IsModified()
{
	//Whether in Source or Web view, whenever we edit, CDocument::SetModified get's called.
	return CDocument::IsModified();
}

//We want the default functionality of CHtmlEditDoc::OnSaveDocument(), but need to update
//the web view with the contents of the source view before saving
BOOL 
CHTMLEdDoc::OnSaveDocument(LPCTSTR lpszFileName)
{
  m_sSaveFileName = lpszFileName;
  return SaveMyDocument();
}

BOOL 
CHTMLEdDoc::SaveMyDocument()
{
  CWaitCursor take_a_deep_sigh;

	MainFrame *pFrame = (MainFrame*)AfxGetMainWnd();
	ASSERT_VALID(pFrame);
  CMDIChildFrame* frame = (CMDIChildFrame*) pFrame->MDIGetActive(); //GetActiveFrame();
  if(frame)
  {
    CString text;
    DWORD viewType = frame->GetCurrentView();

    if(viewType == ID_VIEW_SOURCE)
    {
      ScintillaHTMLView* pSrcView = (ScintillaHTMLView*)frame->GetSourceView();
      pSrcView->GetWindowText(text);
      return InternalSave(text);
    }
    else if(viewType == ID_VIEW_WEB)
    {
      CHTMLEdView* pBrowseView = frame->GetWebView();
      // Save and register as saved with the browser control
      bool result = pBrowseView->DoSave();
      // Register doc/view as saved
      SetModifiedFlag(FALSE);
      // Register for our menu and the registry
      if(m_sSaveFileName.IsEmpty())
      {
        m_sSaveFileName = pBrowseView->RegisteredURL();
      }
      AfxGetApp()->AddToRecentFileList(m_sSaveFileName);
      TidyFile();
      return result;
    }
  }
  ASSERT(FALSE);
  return FALSE;
}

void
CHTMLEdDoc::SetTitle(LPCTSTR p_title)
{
  CString anchor;
  CString filename;
  CString title(p_title);
  Misc::SplitMidpageAnchor(title,filename,anchor);

  m_strTitle = filename;
  // will cause name change in views
  UpdateFrameCounts();
}

void 
CHTMLEdDoc::SetPathName(LPCTSTR lpszPathName, BOOL bAddToMRU)
{
  CString anchor;
  CString filename;
  CString pathname(lpszPathName);
  Misc::SplitMidpageAnchor(pathname,filename,anchor);

  if(_access(filename,0) == 0)
  {
    CDocument::SetPathName(filename,bAddToMRU);
  }
}

// Now REALLY save it.
BOOL
CHTMLEdDoc::InternalSave(CString& text)
{
  BOOL bRet = FALSE;
  try
  {
		CFile file;
		if (file.Open(m_sSaveFileName, CFile::modeCreate|CFile::modeWrite))
    {
      file.Write(text.GetString(),text.GetLength());
			SetModifiedFlag(FALSE);
      file.Close();
      // Register for our menu and the registry
      AfxGetApp()->AddToRecentFileList(m_sSaveFileName);
			bRet = TRUE;
    }
    else
    {
      CString message = "Cannot open file for saving: " + m_sSaveFileName;
      theApp.ErrorMessage(message);
    }
  }
  catch(...)
  {
    // Cannot save for any reason
    CString message = "Cannot save file : " + m_sSaveFileName;
    theApp.ErrorMessage(message);
  }
	return bRet;
}

BOOL
CHTMLEdDoc::GetFile(CString& text)
{
  BOOL bRet = FALSE;
  try
  {
    CFile file;
    if(file.Open(m_sSaveFileName, CFile::modeRead))
    {
      int len = file.GetLength();
      int buf = len + (len / 20); // 5% bigger
      text.GetBuffer(buf);
      memset((void*)text.GetString(),0,buf);
      int readin = file.Read((void*)text.GetString(),buf-1);
      text.ReleaseBuffer();
    }
  }
  catch(...)
  {
    // Cannot read for any reason
    CString message = "Cannot read file : " + m_sSaveFileName;
    theApp.ErrorMessage(message);
  }
  return bRet;
}

int
CHTMLEdDoc::TidyFile()
{
  ctmbstr cfgfil = NULL, errfil = NULL, htmlfil = NULL;
  TidyDoc tdoc = tidyCreate();
  int status = 0;

  uint contentErrors = 0;
  uint contentWarnings = 0;
  uint accessWarnings = 0;

  errout = stderr;  /* initialize to stderr */
  status = 0;

  // Do identations
  tidyOptSetInt( tdoc, TidyIndentContent, TidyAutoState );
  tidyOptSetInt( tdoc, TidyIndentSpaces,  4);
  // Write back in same file
  tidyOptSetBool( tdoc, TidyWriteBack, yes );
  // No errors in AuthorHTML, no error file
  tidyOptSetBool( tdoc, TidyQuiet, yes );
  // Force output
  // tidyOptSetBool( tdoc, TidyForceOutput, yes);
  // Wrap length
  tidyOptSetInt( tdoc, TidyWrapLen, 128 );
  // No extra generator
  tidyOptSetBool( tdoc, TidyMark, no);

  status = tidyParseFile( tdoc, m_sSaveFileName );
  if ( status >= 0 )
  {
    status = tidyCleanAndRepair( tdoc );
  }
  if ( status >= 0 )
  {
    status = tidyRunDiagnostics( tdoc );
  }
  status = tidySaveFile( tdoc, m_sSaveFileName );

  contentErrors   += tidyErrorCount( tdoc );
  contentWarnings += tidyWarningCount( tdoc );
  accessWarnings  += tidyAccessWarningCount( tdoc );

  /* called to free hash tables etc. */
  tidyRelease( tdoc );
  /* return status can be used by scripts */
  if ( contentErrors > 0 )
  {
    return 2;
  }
  if ( contentWarnings > 0 )
  {
    return 1;
  }
  /* 0 signifies all is ok */
  return 0;
}
