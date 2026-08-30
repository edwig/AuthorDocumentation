//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    DirTreeCtrl.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: A tree control for the directory of the documentation project
//
#pragma once

class CDirTreeCtrl : public CTreeCtrl
{
public:
	CDirTreeCtrl();
	virtual ~CDirTreeCtrl();

	BOOL    SetSelPath( LPCTSTR strPath );
	CString GetFullPath( HTREEITEM hItem );
	LPCTSTR GetSubPath( LPCTSTR strPath );
	BOOL    DisplayTree( LPCTSTR strRoot, BOOL bFiles = FALSE );
	CImageList& GetSysImageList ();
	void    RefreshFolder (HTREEITEM hItem);
  void    SetFilter(CString& filter);
  BOOL    DisplayDrives();

protected:
	BOOL      IsValidPath( LPCTSTR strPath );
	void      ExpandItem( HTREEITEM hItem, UINT nCode );
	HTREEITEM SearchSiblingItem( HTREEITEM hItem, LPCTSTR strText );
	BOOL      FindSubDir( LPCTSTR strPath );
	HTREEITEM AddItem( HTREEITEM hParent, LPCTSTR strPath );
	void      DisplayPath( HTREEITEM hParent, LPCTSTR strPath );
	BOOL      GetSysImgList();
	
  BOOL       m_bFiles;
	CString    m_strError;
  CImageList m_imgList;
	CString    m_strRoot;
  CString    m_filter;

	afx_msg void OnItemexpanded (NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
};

inline
CImageList& 
CDirTreeCtrl::GetSysImageList()
{
  if (!m_imgList.m_hImageList) 
  {
    GetSysImgList();
  }
  return m_imgList;
}

inline void
CDirTreeCtrl::SetFilter(CString& filter)
{
  m_filter = filter;
}
