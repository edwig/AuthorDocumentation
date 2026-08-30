//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    AuthorImage.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Handles an BMP / PNG / JPEG image
//
#include "StdAfx.h"
#include "AuthorHTML.h"
#include "AuthorImage.h"
#include "Images.h"

AuthorImage::AuthorImage()
              :m_image(NULL),
               m_posOptions(BUTT_LAYOUT_ALL)
{
}

AuthorImage::~AuthorImage()
{
}

bool
AuthorImage::CalculateStandard(LPCSTR type,LPCSTR def)
{
  if (theApp.GetImages())
  {
    m_image = theApp.GetImages()->GeefStandaard(type);
  }
  if (!m_image && def != NULL)
  {
    m_image = CalculateImageInfo(def);
  }
  return m_image;
}

bool
AuthorImage::ZetAfbeelding(LPCSTR afbeelding)
{
  m_image = CalculateImageInfo(afbeelding);
  return m_image;
}


CRect
AuthorImage::PaintBitmap(CDC& pDC,CRect& rect,int volgNr,int drawParam)
{
  if (theApp.GetImages())
  {
    return theApp.GetImages()->PaintBitmap(pDC,rect,m_image,volgNr,drawParam);
  }
  rect = CRect();
  return rect;
}

Ref<CAfbeeldingInfo> 
AuthorImage::CalculateImageInfo(LPCSTR str,int logica)
{
  if (theApp.GetImages())
  {
    return theApp.GetImages()->GetImageInfo(str,logica);
  }
  else
  {
    return Ref<CAfbeeldingInfo>(); 
  }
}

int
AuthorImage::CalculateButtonLayout(int& buttonLayout)
{
  if (!(buttonLayout & m_posOptions))
  {
         if (m_posOptions & BUTT_LAYOUT_LEFT)   buttonLayout = BUTT_LAYOUT_LEFT;
    else if (m_posOptions & BUTT_LAYOUT_RIGHT)  buttonLayout = BUTT_LAYOUT_RIGHT;
    else if (m_posOptions & BUTT_LAYOUT_TOP)    buttonLayout = BUTT_LAYOUT_TOP;
    else if (m_posOptions & BUTT_LAYOUT_BOTTOM) buttonLayout = BUTT_LAYOUT_BOTTOM;
    else if (m_posOptions & BUTT_LAYOUT_NONE)   buttonLayout = BUTT_LAYOUT_NONE;
    else if (m_posOptions & BUTT_LAYOUT_IMAGE)  buttonLayout = BUTT_LAYOUT_IMAGE;
  }
  return buttonLayout;
}

bool 
AuthorImage::HasImage()
{
  return (m_image != NULL && m_image->HasImage());
}

