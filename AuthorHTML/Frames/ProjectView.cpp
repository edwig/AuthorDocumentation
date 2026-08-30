//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    ProjectView.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: A ProjectView is a view over the project's documents
// This is in essence the contents of a *.HHP file
//
#include "stdafx.h"
#include "AuthorHTML.h"
#include "MainFrm.h"
#include "ProjectView.h"
#include "ProjectFile.h"
#include "NewFileDlg.h"
#include "float.h"

//#define XML_STATE

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ProjectView

IMPLEMENT_DYNCREATE(ProjectView, CView)

BEGIN_MESSAGE_MAP(ProjectView, CView)
	ON_WM_CREATE()
  ON_WM_SETFOCUS()
  ON_WM_SIZE()
	ON_COMMAND(ID_FILE_PRINT,         CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT,  CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)

	ON_NOTIFY(NM_DBLCLK,IDC_GRID, OnReportItemDblClick)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ProjectView construction/destruction

ProjectView::ProjectView()
{
}

ProjectView::~ProjectView()
{
}

// Needed to derive the class from CView!
void
ProjectView::OnDraw(CDC* /*pDC*/)
{
}

BOOL ProjectView::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CView::PreCreateWindow(cs))
		return FALSE;

	//cs.dwExStyle |=  WS_EX_STATICEDGE;
	//cs.dwExStyle &= ~WS_EX_CLIENTEDGE;

	return TRUE;

}

/////////////////////////////////////////////////////////////////////////////
// ProjectView diagnostics

#ifdef _DEBUG
void ProjectView::AssertValid() const
{
	CView::AssertValid();
}

void ProjectView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

ProjectDoc* ProjectView::GetDocument() // non-debug version is inline
{
	//ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(ProjectDoc)));
	return (ProjectDoc*)m_pDocument;
}
#endif //_DEBUG


CWnd* 
ProjectView::CreateView(CWnd* pParentWnd)
{
  if (GetSafeHwnd() == 0)
  {
    VERIFY(Create(_T("STATIC"), NULL, WS_CHILD|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, CRect(0,0,0,0),pParentWnd,0));
  }
  return this;
}

/////////////////////////////////////////////////////////////////////////////
// ProjectView message handlers

int ProjectView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  if(CView::OnCreate(lpCreateStruct) == -1)
  {
    return -1;
  }

  // Create the grid
  m_grid.Create(CRect(0,0,0,0),this,IDC_GRID);

  // Load the icons image list
	VERIFY(m_ilIcons.Create(16,16, ILC_COLOR24|ILC_MASK, 0, 1));
	CBitmap bmp;
	VERIFY(bmp.LoadBitmap(IDB_PROJECT));
	m_ilIcons.Add(&bmp, RGB(255, 0, 255));
	m_grid.SetImageList(&m_ilIcons);

	// Configure the grid
  InitGrid();

  // Reset state of last run
  LoadProjectFile();
 	return 0;
}

void
ProjectView::InitGrid()
{
  // Set the columns
  m_grid.SetColumnCount(COLUMN_TODO + 1);
  m_grid.SetRowCount(1);
  m_grid.SetFixedRowCount(1);
  m_grid.SetSingleColSelection(TRUE);
  m_grid.SetSingleRowSelection(TRUE);
  m_grid.SetEditable(FALSE);
  m_grid.SetHeaderSort();

  m_grid.GetCell(0, COLUMN_TYPE        )->SetText(_T("Type"));
  m_grid.GetCell(0, COLUMN_IN_TOC      )->SetText(_T("TOC"));
  m_grid.GetCell(0, COLUMN_IN_INDEX    )->SetText(_T("Index"));
  m_grid.GetCell(0, COLUMN_RELDIRECTORY)->SetText(_T("Directory"));
  m_grid.GetCell(0, COLUMN_FILENAME    )->SetText(_T("Filename"));
  m_grid.GetCell(0, COLUMN_TITLE       )->SetText(_T("Topic title"));
  m_grid.GetCell(0, COLUMN_AUTHOR      )->SetText(_T("Author"));
  m_grid.GetCell(0, COLUMN_STATUS      )->SetText(_T("Status"));
  m_grid.GetCell(0, COLUMN_PRIORITY    )->SetText(_T("Priority"));
  m_grid.GetCell(0, COLUMN_TODO        )->SetText(_T("To do"));

  m_grid.SetColumnWidth(0,40);
  m_grid.SetColumnWidth(1,40);
  m_grid.SetColumnWidth(2,40);
  m_grid.SetColumnWidth(3,180);
  m_grid.SetColumnWidth(4,180);
  m_grid.SetColumnWidth(5,180);
  m_grid.SetColumnWidth(6,180);
  m_grid.SetColumnWidth(7,180);
  m_grid.SetColumnWidth(8,180);
  m_grid.SetColumnWidth(9,180);
}

void
ProjectView::LoadProjectFile()
{
  // EMpty the control
  int rows = m_grid.GetRowCount();
  for(int row = 1; row < rows;++row)
  {
    m_grid.DeleteRow(row);
  }
  // Add the records
  AddRecords();
}

void
ProjectView::OnInitialUpdate()
{
  CView::OnInitialUpdate();
}

void
ProjectView::OnSize(UINT nType,int cx,int cy)
{
  CView::OnSize(nType,cx,cy);

  CRect rectClient;
  GetClientRect(rectClient);

  m_grid.SetWindowPos(nullptr,rectClient.left,rectClient.top,rectClient.Width(),rectClient.Height(),SWP_NOACTIVATE | SWP_NOZORDER);
}

// Handles double clicking ReportControl item.
// If "Read" attribute is False, changes it to True
void
ProjectView::OnReportItemDblClick(NMHDR* pNMHDR, LRESULT * /*result*/)
{
  LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
  CCellID id = m_grid.GetFocusCell();
	if(id.row > 0)
	{
		TRACE(_T("Double Click on row %d\n"),id.row);

    DocumentFile* doc = reinterpret_cast<DocumentFile*>(m_grid.GetItemData(id.row,0));
    if(doc)
		{
      // Probably a topic document
      CString path = theApp.GetBaseDirectory()   +
                     doc->GetRelativeDirectory() + 
                     doc->GetRelativeFilename();
      theApp.OpenTypedDocumentFile(path);
		}
	}
}

// Adds the records of the Project file
void 
ProjectView::AddRecords()
{
  ProjectFile* project = theApp.GetProjectFile();
  if(project)
  {
    DocumentMap& documents = project->GetDocumentMap();

    for(auto& dd : documents)
    {
      AddRecord(dd.second);
    }
  }
}

void
ProjectView::AddRecord(DocumentFile* p_doc)
{
  int row = m_grid.InsertRow("");
  m_grid.SetItemData(row,0,(LPARAM)p_doc);
  UpdateRecord(row,p_doc);
}

void
ProjectView::UpdateRecord(int row,DocumentFile* p_doc)
{
  SetTextImage(row,COLUMN_TYPE,    _T(""),p_doc->GetDocumentType());
  SetTextImage(row,COLUMN_IN_TOC,  _T(""),p_doc->IsInTOC()   ? COLUMN_ICON_INTOC    : COLUMN_ICON_NOTINTOC);
  SetTextImage(row,COLUMN_IN_INDEX,_T(""),p_doc->IsInIndex() ? COLUMN_ICON_INDEXHHK : COLUMN_ICON_NOTININDEX);

  m_grid.GetCell(row,COLUMN_RELDIRECTORY)->SetText(p_doc->GetRelativeDirectory());
  m_grid.GetCell(row,COLUMN_FILENAME)    ->SetText(p_doc->GetRelativeFilename());
  m_grid.GetCell(row,COLUMN_TITLE)       ->SetText(p_doc->GetTitle());
  m_grid.GetCell(row,COLUMN_AUTHOR)      ->SetText(p_doc->GetAuthor());

  CString stext = p_doc->GetStatusDescription();
  if(stext.IsEmpty())
  {
    stext = _T("Unknown");
  }
  m_grid.GetCell(row,COLUMN_STATUS)->SetText(stext);

  stext = p_doc->GetPriorityDescription();
  if(stext.IsEmpty())
  {
    stext = _T("Unknown");
  }
  m_grid.GetCell(row,COLUMN_PRIORITY)->SetText(stext);

  stext = p_doc->GetToDoFirstDescription();
  if(stext.IsEmpty())
  {
    stext = _T("Unknown");
  }
  m_grid.GetCell(row,COLUMN_TODO)->SetText(stext);
}

void
ProjectView::UpdateRecord(DocumentFile* p_doc)
{
  for(int index = 1;index < m_grid.GetRowCount();++index)
  {
    DocumentFile* here = reinterpret_cast<DocumentFile*>(m_grid.GetItemData(index,0));
    if(here == p_doc)
    {
      UpdateRecord(index,p_doc);
      return;
    }
  }
}

void
ProjectView::SetTextImage(int p_row,int p_col,CString p_text,int p_image)
{
  GV_ITEM item;
  item.mask    = GVIF_IMAGE;
  item.row    = p_row;
  item.col    = p_col;
  item.iImage = p_image;
  if(!p_text.IsEmpty())
  {
    item.mask |= GVIF_TEXT;
    item.strText = p_text;
  }
  m_grid.SetItem(&item);
}


void ProjectView::OnSetFocus(CWnd* pOldWnd)
{
	CView::OnSetFocus(pOldWnd);
	m_grid.SetFocus();
}

void
ProjectView::OnNewRecord()
{
  CString base = theApp.GetBaseDirectory();
  NewFileDlg dlg(this,base);
  if(dlg.DoModal() == IDOK)
  {
    CString filename = base + dlg.GetFilename();
    DocumentFile* doc = new DocumentFile(filename);

    // Find attributes from dialog
    CString title = dlg.GetTitle();
    CString templ = dlg.GetTemplate();
    doc->SetTitle(title);
    doc->SetTemplate(templ);

    // Add to the project file
    theApp.GetProjectFile()->AddDocumentFile(doc);

    // Add to the project control
    AddRecord(doc);
  }
}

void 
ProjectView::OnEditRecord()
{
  CCellID id = m_grid.GetFocusCell();
  if(id.row)
  {
    DocumentFile* doc = reinterpret_cast<DocumentFile*>(m_grid.GetItemData(id.row,0));
    if(doc)
    {
      // Probably a topic document
      CString path = theApp.GetBaseDirectory()   + 
                     doc->GetRelativeDirectory() +
                     doc->GetRelativeFilename();
      theApp.OpenTypedDocumentFile(path);
    }
  }
}

void
ProjectView::OnDeleteRecord()
{
  CCellID id = m_grid.GetFocusCell();
  if(id.row)
  {
    DocumentFile* doc = reinterpret_cast<DocumentFile*>(m_grid.GetItemData(id.row,0));
    if(doc)
    {
      ProjectFile* project = theApp.GetProjectFile();
      project->RemoveDocumentFile(doc);
    }
    m_grid.DeleteRow(id.row);
  }
}

void
ProjectView::OnRecordProperties()
{
  CCellID id = m_grid.GetFocusCell();
  if(id.row)
  {
    DocumentFile* doc = reinterpret_cast<DocumentFile*>(m_grid.GetItemData(id.row,0));
    if(doc)
    {
      // Probably a topic document
      CString path = theApp.GetBaseDirectory()   + 
                     doc->GetRelativeDirectory() +
                     doc->GetRelativeFilename();
      theApp.OpenTypedDocumentFile(path);

      // We must asynchroniously send the properties command
      // The document could still be opening itsself.
      // See alsoo CHTMLEdView::OnProperties
      MainFrame*  main  = (MainFrame*) theApp.m_pMainWnd;
      ::PostMessage(main->GetSafeHwnd(),WM_COMMAND,ID_PROPERTIES,0);
    }
  }
}

/////////////////////////////////////////////////////////////////////////////
// CGridViewDemoView printing

BOOL ProjectView::OnPreparePrinting(CPrintInfo* pInfo)
{
  // default preparation
  return DoPreparePrinting(pInfo);
}

void ProjectView::OnBeginPrinting(CDC* pDC,CPrintInfo* pInfo)
{
  m_grid.OnBeginPrinting(pDC,pInfo);
}

void ProjectView::OnPrint(CDC* pDC,CPrintInfo* pInfo)
{
  m_grid.OnPrint(pDC,pInfo);
}

void ProjectView::OnEndPrinting(CDC* pDC,CPrintInfo* pInfo)
{
  m_grid.OnEndPrinting(pDC,pInfo);
}
