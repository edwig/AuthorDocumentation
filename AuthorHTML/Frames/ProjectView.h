//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    ProjectView.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: A ProjectView is a view over the project's documents
// This is in essence the contents of a *.HHP file
//
#pragma once
#include "GridCtrl.h"

// Constants used to identify columns, this will be the column ItemIndex
#define COLUMN_TYPE             0   // HTML or payload type
#define COLUMN_IN_TOC           1   // Topic is in TOC
#define COLUMN_IN_INDEX         2   // Topic is in Index
#define COLUMN_RELDIRECTORY     3   // Relative directory in project
#define COLUMN_FILENAME         4   // Stripped filename
#define COLUMN_TITLE            5   // Topic title
#define COLUMN_AUTHOR           6   // Author of the topic
#define COLUMN_STATUS           7   // Status of the topic
#define COLUMN_PRIORITY         8   // Priority of (re)writing
#define COLUMN_TODO             9   // Todo

// Constants used to identify icons used in the ReportControl
// in the IDB_PROJECT bitmap
#define COLUMN_ICON_HTMLTYPE    0
#define COLUMN_ICON_IMGTYPE     1
#define COLUMN_ICON_SCRIPTTYPE  2
#define COLUMN_ICON_INTOC       3
#define COLUMN_ICON_NOTINTOC    4
#define COLUMN_ICON_INDEXHHK    5
#define COLUMN_ICON_INDEXTOPIC  6
#define COLUMN_ICON_NOTININDEX  7

class ProjectDoc;
class DocumentFile;

class ProjectView : public CView
{
protected: // create from serialization only
	DECLARE_DYNCREATE(ProjectView)
  ProjectView();
 ~ProjectView();
public:
  CWnd* CreateView(CWnd* pParentWnd);
	ProjectDoc* GetDocument();
  void LoadProjectFile();

	CEdit     m_wndFilterEdit;

	CImageList m_ilIcons;

	CFrameWnd* m_pTaskFrame;

  void UpdateRecord(DocumentFile* doc);
  void UpdateRecord(int row,DocumentFile* doc);
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
  virtual void OnInitialUpdate();

  // Implementation
  void OnNewRecord();
  void OnEditRecord();
  void OnDeleteRecord();
  void OnRecordProperties();

protected:
// Implementation
  void AddRecords();
	void OnDraw(CDC* pDC) override;
	void InitGrid();
	void AddRecord(DocumentFile* p_doc);
	void SetTextImage(int p_row,int p_col,CString p_text,int p_image);

  int         m_nToolBarID;
  CGridCtrl		m_grid;
public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd* pOldWnd);

	afx_msg void OnReportColumnRClick(NMHDR * pNotifyStruct, LRESULT * result);
	afx_msg void OnReportItemDblClick(NMHDR * pNotifyStruct, LRESULT * result);

	afx_msg BOOL OnPreparePrinting(CPrintInfo* pInfo);
	afx_msg void OnBeginPrinting(CDC* pDC,CPrintInfo* pInfo);
	afx_msg void OnPrint(CDC* pDC,CPrintInfo* pInfo);
	afx_msg void OnEndPrinting(CDC* pDC,CPrintInfo* pInfo);
	afx_msg void OnSize(UINT nType,int cx,int cy);

  DECLARE_MESSAGE_MAP()
public:
};

#ifndef _DEBUG  // debug version in ReportSampleView.cpp
inline ProjectDoc* ProjectView::GetDocument()
	{ return (ProjectDoc*)m_pDocument; }
#endif

