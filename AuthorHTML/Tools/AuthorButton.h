//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    AuthorButton.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: A specialized window button
//
#pragma once
#include "StdAfx.h"
#include "AuthorImage.h"

class CButtonC : public AD_Button
{
public:
  CButtonC(CString& p_type);
  virtual ~CButtonC();

  void         SetImage(CString p_type,CString p_library = "");
  AuthorImage& GetImageInfo();

  void OnKeyDown(UINT nChar,UINT nRepCnt,UINT nFlags);
  void OnLButtonUp(UINT nFlags,CPoint point);
  void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
  void OnPaint();
  afx_msg BOOL OnEraseBkgnd( CDC* pDC );
  BOOL PreTranslateMessage(MSG* pMsg);

  DECLARE_MESSAGE_MAP()

protected:
   void PlaatsRectInRect(const CRect& mRect,CRect& rect,int pos,int marges);
private:
  AuthorImage m_buttonImage;
};

inline
AuthorImage& CButtonC::GetImageInfo()
{
  return m_buttonImage;
}
