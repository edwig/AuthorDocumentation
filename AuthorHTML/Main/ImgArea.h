//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    ImgArea.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Integrates an IMAGE and an AREA on a HTML document
//              So that we can have different hyperlinks on different
//              parts of the image
//
#pragma once
#include <vector>

// Number of pixels mouse-touchdown
#define HANDLE_RADIUS 10

// Types of shapes of an area
typedef enum _areaShape
{
   AREA_INVALID   = 0
  ,AREA_RECTANGLE = 1
  ,AREA_CIRCLE
  ,AREA_POLYGON
}
AreaShape;

typedef std::vector<POINT> AreaPoints;

class ImgArea
{
public:
  ImgArea(AreaShape p_shape     // Existing area
         ,CString   p_id
         ,CString   p_href
         ,CString   p_coords);
  ImgArea(AreaShape p_shape     // Newly defined area
         ,CString   p_id
         ,POINT     p_center);
  ~ImgArea();
  int       TouchDown(POINT mouse);
  void      SetPolyCorner(int corner,int X,int Y);
  void      Draw   (HDC hdc,RECT rcBounds,int point = -1);
  void      DrawNew(HDC hdc,RECT rcBounds);
  void      RewriteArea(CComPtr<IHTMLDocument2> doc);
  bool      AddDefintionPoint(POINT extra);
  CString   MakeCoords();
  bool      IsPartial();
  AreaShape GetShape();
  void      SetNewCorner(int x,int y);
  bool      HasNewCorner();
  CString   ID();
private:
  void   SetPoints(CString& coords);
  double Distance(POINT pt1, POINT pt2);

  AreaShape  m_shape;
  CString    m_id;
  CString    m_href;
  int        m_lPartID; // Touchdown part
  bool       m_partial; // Newly defined area?
  // Map with all the points in an area
  // Depending on m_shape (4,2,n)
  AreaPoints m_points;
  // Extra 1 point for painting new shapes
  POINT      m_newCorner;
};

inline AreaShape
ImgArea::GetShape()
{
  return m_shape;
}

inline bool
ImgArea::IsPartial()
{
  return m_partial;
}

inline CString
ImgArea::ID()
{
  return m_id;
}

inline void
ImgArea::SetNewCorner(int x,int y)
{
  m_newCorner.x = x;
  m_newCorner.y = y;
}

inline bool
ImgArea::HasNewCorner()
{
  if(m_newCorner.x * m_newCorner.y > 0)
  {
    return true;
  }
  return false;
}