//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    ImgArea.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Integrates an IMAGE and an AREA on a HTML document
//              So that we can have different hyperlinks on different
//              parts of the image
//
#include "StdAfx.h"
#include "ImgArea.h"
#include <math.h>

ImgArea::ImgArea(AreaShape p_shape 
                ,CString   p_id 
                ,CString   p_href 
                ,CString   p_coords)
        :m_shape(p_shape)
        ,m_id(p_id)
        ,m_href(p_href)
        ,m_partial(false)
{
  // No touchdown yet
  m_lPartID = -1;
  // New corner is 0.0
  SetNewCorner(0,0);
  // discover our waypoints
  SetPoints(p_coords);
}

// Partial instantiation of a new 
// still to define area
ImgArea::ImgArea(AreaShape p_shape
                ,CString   p_id
                ,POINT     p_center)
        :m_shape(p_shape)
        ,m_id(p_id)
{
  m_lPartID = -1;
  m_points.push_back(p_center);
  m_partial = true;
  SetNewCorner(0,0);
}


ImgArea::~ImgArea()
{
  // Clear all polygon waypoints
  m_points.clear();
}

// Checks tocuhdown of mouse
// returns number of point in the shape
int 
ImgArea::TouchDown(POINT mouse)
{
  if(m_shape == AREA_CIRCLE)
  {
    // Check center
    int fromCenter = Distance(mouse,m_points[0]);
    if(fromCenter <= HANDLE_RADIUS)
    {
      return 0; // center
    }
    // Check the perimiter
    int radius = Distance(m_points[0],m_points[1]);
    if(std::abs(radius - fromCenter) <= HANDLE_RADIUS)
    {
      return 1; // on perimiter
    }
    return -1;
  }
  // For RECTANGLE and POLYGON just walk through all the points
  for(int point = 0;point < (int)m_points.size(); ++point)
  {
    if(Distance(mouse,m_points[point]) <= HANDLE_RADIUS)
    {
      return point;
    }
  }
  // No hit
  return -1;
}

#pragma warning(disable: 4701) // ptClosure may be used without having been initialized
void
ImgArea::Draw(HDC hdc,RECT rcBounds,int point)
{
  if(m_shape == AREA_INVALID)
  {
    // Cannot render an invalid area!
    return;
  }
  POINT	ptGlobalCorners;
  POINT ptClosure;

  if(m_shape == AREA_POLYGON || m_shape == AREA_RECTANGLE)
  {
    for(int corner = 0;corner < (int)m_points.size(); ++corner)
    {
      ptGlobalCorners.x = m_points[corner].x + rcBounds.left + HANDLE_RADIUS;
      ptGlobalCorners.y = m_points[corner].y + rcBounds.top  + HANDLE_RADIUS;
      if(corner == 0)
      {
        ptClosure = ptGlobalCorners;
        MoveToEx(hdc,ptGlobalCorners.x,ptGlobalCorners.y,NULL);
      }
      else
      {
        LineTo(hdc,ptGlobalCorners.x,ptGlobalCorners.y);
      }
    }
    // Paint closure of polygon
    LineTo(hdc,ptClosure.x,ptClosure.y);
  }
  else if(m_shape == AREA_CIRCLE) 
  {
    if(m_points.size() != 2)
    {
      // Partial new circle
      return;
    }
    POINT center;
    center.x = m_points[0].x + rcBounds.left + HANDLE_RADIUS;
    center.y = m_points[0].y + rcBounds.top  + HANDLE_RADIUS;
    int radius = Distance(m_points[0],m_points[1]);
    SelectObject(hdc,(HBRUSH)GetStockObject(HOLLOW_BRUSH));
    Ellipse(hdc
           ,center.x - radius
           ,center.y - radius
           ,center.x + radius
           ,center.y + radius);
  }
  // For all forms (POLYGON, RECTANGLE, CIRCLE)
  if(point >= 0)
  {
    // Paint the mouse touchdown
    ptGlobalCorners.x = m_points[point].x + rcBounds.left + HANDLE_RADIUS;
    ptGlobalCorners.y = m_points[point].y + rcBounds.top  + HANDLE_RADIUS;
    SelectObject(hdc,(HBRUSH)GetStockObject(HOLLOW_BRUSH));
    Ellipse(hdc
            ,ptGlobalCorners.x - HANDLE_RADIUS
            ,ptGlobalCorners.y - HANDLE_RADIUS
            ,ptGlobalCorners.x + HANDLE_RADIUS
            ,ptGlobalCorners.y + HANDLE_RADIUS);
  }
}

void
ImgArea::DrawNew(HDC hdc,RECT rcBounds)
{
  if(m_shape == AREA_INVALID)
  {
    // Cannot render an invalid area!
    return;
  }
  POINT	ptGlobalCorners;
  POINT ptClosure;

  if(m_shape == AREA_RECTANGLE)
  {
    int width = m_newCorner.x - m_points[0].x;
    int high  = m_newCorner.y - m_points[0].y;
    if(width <= 0 || high <= 0)
    {
      // Nothing to draw
      return;
    }
    POINT top_left;
    POINT top_right;
    POINT bot_left;
    POINT bot_right;
    top_left .x = m_points[0].x + rcBounds.left + HANDLE_RADIUS;
    top_left .y = m_points[0].y + rcBounds.top  + HANDLE_RADIUS;
    top_right.x = top_left.x + width;
    top_right.y = top_left.y;
    bot_left .x = top_left.x;
    bot_left .y = top_left.y + high;
    bot_right.x = top_left.x + width;
    bot_right.y = top_left.y + high;
    MoveToEx(hdc,top_left .x,top_left .y,NULL);
    LineTo  (hdc,top_right.x,top_right.y);
    LineTo  (hdc,bot_right.x,bot_right.y);
    LineTo  (hdc,bot_left .x,bot_left .y);
    LineTo  (hdc,top_left .x,top_left .y);
  }
  else if(m_shape == AREA_POLYGON)
  {
    POINT start;
    ptClosure.x = m_newCorner.x + rcBounds.left + HANDLE_RADIUS;
    ptClosure.y = m_newCorner.y + rcBounds.top  + HANDLE_RADIUS;
    for(int corner = 0;corner < (int)m_points.size(); ++corner)
    {
      ptGlobalCorners.x = m_points[corner].x + rcBounds.left + HANDLE_RADIUS;
      ptGlobalCorners.y = m_points[corner].y + rcBounds.top  + HANDLE_RADIUS;
      if(corner == 0)
      {
        start = ptGlobalCorners;
        MoveToEx(hdc,ptGlobalCorners.x,ptGlobalCorners.y,NULL);
      }
      else
      {
        LineTo(hdc,ptGlobalCorners.x,ptGlobalCorners.y);
      }
    }
    // Paint closure of polygon
    LineTo(hdc,ptClosure.x,ptClosure.y);
    LineTo(hdc,start.x,start.y);
  }
  else if(m_shape == AREA_CIRCLE) 
  {
    if(m_points.size() != 2)
    {
      // Partial new circle
      return;
    }
    POINT center;
    center.x = m_points[0].x + rcBounds.left + HANDLE_RADIUS;
    center.y = m_points[0].y + rcBounds.top  + HANDLE_RADIUS;
    int radius = std::abs(m_newCorner.x - m_points[0].x);
    SelectObject(hdc,(HBRUSH)GetStockObject(HOLLOW_BRUSH));
    Ellipse(hdc
           ,center.x - radius
           ,center.y - radius
           ,center.x + radius
           ,center.y + radius);
  }
}



void
ImgArea::SetPolyCorner(int corner,int X,int Y)
{
  if(corner >= 0 && corner < (int)m_points.size())
  {
    if(corner == 0)
    {
      // Move the shape
      int deltax = X - m_points[0].x;
      int deltay = Y - m_points[0].y;
      for(int ind = 0;ind < (int)m_points.size(); ++ind)
      {
        m_points[ind].x += deltax;
        m_points[ind].y += deltay;
      }
      return;
    }
    // Change the size of the shape
    if(m_shape == AREA_POLYGON || m_shape == AREA_CIRCLE)
    {
      m_points[corner].x = X;
      m_points[corner].y = Y;
    }
    if(m_shape == AREA_RECTANGLE)
    {
      int deltax = X - m_points[corner].x;
      int deltay = Y - m_points[corner].y;
      // Resize horizontally
      if(corner < 3)
      {
        m_points[1].x += deltax;
        m_points[2].x += deltax;
      }
      if(corner > 1)
      {
        // Resize vertically
        m_points[2].y += deltay;
        m_points[3].y += deltay;
      }
    }
  }
}

void
ImgArea::RewriteArea(CComPtr<IHTMLDocument2> doc)
{
  // Recalculate our COORDS
  CString coords = MakeCoords();

  // Put it back on the area
  CComPtr<IHTMLElementCollection> coll;
  HRESULT hr = doc->get_all(&coll);
  if(SUCCEEDED(hr))
  {
    CComVariant ident(m_id);
    CComVariant number((long)0);
    CComPtr<IDispatch> disp;
    hr = coll->item(ident,number,&disp);
    if(SUCCEEDED(hr) && disp.p)
    {
      CComQIPtr<IHTMLAreaElement,&IID_IHTMLAreaElement> area = disp;
      CComBSTR bCoords = CT2CW(coords);
      area->put_coords(bCoords);
    }
  }
}

CString
ImgArea::MakeCoords()
{
  // Recalculate our COORDS
  CString coords;

  if(m_points.size() < 2)
  {
    // Cannot write it back
    return "";
  }
  coords.Format("%d,%d",m_points[0].x,m_points[0].y);
  if(m_shape == AREA_CIRCLE)
  {
    CString radius;
    radius.Format(",%d",(int)(Distance(m_points[0],m_points[1])));
    coords += radius;
  }
  else if(m_shape == AREA_RECTANGLE)
  {
    ASSERT(m_points.size() == 4);
    CString lower_right;
    lower_right.Format(",%d,%d",m_points[2].x,m_points[2].y);
    coords += lower_right;
  }
  else
  {
    // For polygon add all the points
    for(unsigned int ind = 1;ind < m_points.size();++ind)
    {
      CString point;
      point.Format(",%d,%d",m_points[ind].x,m_points[ind].y);
      coords += point;
    }
  }
  return coords;
}

// Adds a next definition point to the area
// returns TRUE if ready with the shape
bool      
ImgArea::AddDefintionPoint(POINT extra)
{
  if(m_points.size() == 1 && m_shape == AREA_RECTANGLE)
  {
    POINT top_right;
    POINT bot_left;
    top_right.x = extra.x;
    top_right.y = m_points[0].y;
    bot_left.x  = m_points[0].x;
    bot_left.y  = extra.y;
    m_points.push_back(top_right);
    m_points.push_back(extra);
    m_points.push_back(bot_left);
    // READY
    m_partial = false;
    return true;
  }
  if(m_points.size() == 1 && m_shape == AREA_CIRCLE)
  {
    long radius = Distance(m_points[0],extra);
    POINT ext;
    ext.x = m_points[0].x + radius;
    ext.y = m_points[0].y;
    m_points.push_back(ext);
    // READY
    m_partial = false;
    return true;
  }
  if(m_shape == AREA_POLYGON)
  {
    if((m_points.size() >= 3) && (Distance(m_points[0],extra) < HANDLE_RADIUS))
    {
      m_partial = false;
      return true;
    }
    // Just add the point
    m_points.push_back(extra);
    // Not yet ready
    return false;
  }
  return false;
}

////////////////////////////////////////////////////////////////////////////
//
// PRIVATE METHODS
//
////////////////////////////////////////////////////////////////////////////

void 
ImgArea::SetPoints(CString& coords)
{
  int pos = coords.Find(',');
  if(pos < 0) 
  {
    m_shape = AREA_INVALID;
    return;
  }
  // All shapes have a first reference point (x1,y1)
  int x1 = atoi(coords); // Make use of the fact that atoi stops at ','
  coords = coords.Mid(pos+1);
  int y1 = atoi(coords);

  // The same for CIRCLE/RECTANGLE/POLYGON
  POINT center;
  center.x = x1;
  center.y = y1;
  m_points.push_back(center);

  // Find third point in coords
  pos = coords.Find(',');
  if(pos < 0)
  {
    m_shape = AREA_INVALID;
    return;
  }
  coords = coords.Mid(pos+1);

  if(m_shape == AREA_CIRCLE)
  {
    // A circle only has three points (x1,y1,r) in coords
    // Define two POINTS in m_points (radius lies directly right of the center
    int r = atoi(coords);
    POINT radius;
    radius.x = x1 + r;
    radius.y = y1;
    m_points.push_back(radius);
    return;
  }
  if(m_shape == AREA_RECTANGLE)
  {
    // A rectangle has four points (x1,y1,x2,y2) in coords
    // Define four polygon waypoints out of these two
    // First is left-upper, second is right-lower point of the rectangle!
    int x2 = atoi(coords);
    pos = coords.Find(',');
    if(pos < 0)
    {
      m_shape = AREA_INVALID;
      return;
    }
    coords = coords.Mid(pos + 1);
    int y2 = atoi(coords);
    POINT ri_lower;
    ri_lower.x = x2;
    ri_lower.y = y2;
    // construct the other two
    POINT ri_upper;
    POINT le_lower;
    ri_upper.x = x2;
    ri_upper.y = y1;
    le_lower.x = x1;
    le_lower.y = y2;
    m_points.push_back(ri_upper);
    m_points.push_back(ri_lower);
    m_points.push_back(le_lower);
    return;
  }
  // else AREA_POLYGON left
  while(coords.GetLength())
  {
    int x2 = atoi(coords);
    pos = coords.Find(',');
    if(pos < 0)
    {
      // Odd number of waypoints in shape=poly
      return;
    }
    coords = coords.Mid(pos + 1);
    int y2 = atoi(coords);

    // Save the waypoint
    POINT waypoint;
    waypoint.x = x2;
    waypoint.y = y2;
    m_points.push_back(waypoint);

    pos = coords.Find(',');
    if(pos < 0)
    {
      // Last point of the polygon.
      // Okay to stop.
      return;
    }
    coords = coords.Mid(pos + 1);
    coords = coords.Trim();
  }
}

// Calculates the distance between two points
double
ImgArea::Distance(POINT pt1, POINT pt2)
{
  return _hypot((pt1.x - pt2.x), (pt1.y - pt2.y));
}
