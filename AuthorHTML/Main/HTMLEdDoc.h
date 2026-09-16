//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    HTMLEdDoc.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Document backend for an HTML documentation file
// 
#pragma once

#define DELETE_EXCEPTION(e) do { e->Delete(); } while (0)

class CHTMLEdDoc : public CHtmlEditDoc
{
protected:
	// create from serialization only
  DECLARE_DYNCREATE(CHTMLEdDoc)
  CHTMLEdDoc();
public:
  virtual ~CHTMLEdDoc();

#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif
  
	BOOL SaveMyDocument();
	BOOL OnSaveDocument(LPCTSTR lpszFileName);
	BOOL OnNewDocument();
	void OnCloseDocument();
	BOOL OnOpenDocument(LPCTSTR lpszFileName);
	BOOL IsModified();
  BOOL InternalSave(CString& saveIt);
  BOOL GetFile(CString& text);
  void SetTitle(LPCTSTR lpszTitle);
  void SetPathName(LPCTSTR lpszPathName, BOOL bAddToMRU);
  // Create a new empty HTML document
  // Optionally with a document title
  void CreateNewDocument(CString& p_filename,bool p_setTitle = true);
  static bool CreateNewDocumentFile(CString& p_filename,CString& p_title);

  CString GetFromTemplate();

  BOOL m_bDoSaveOnDocCom;
  BOOL m_bCallNewDocument;
  BOOL m_bCallCloseDocument;

protected:
  CString m_fromTemplate;

  // Attributes
private:
  int  TidyFile();

  CString m_sSaveFileName;

protected:
	DECLARE_MESSAGE_MAP()

public:
  // Overrides
  afx_msg void OnSaveAs();
};

inline CString
CHTMLEdDoc::GetFromTemplate()
{
  return m_fromTemplate;
}