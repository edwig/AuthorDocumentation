//////////////////////////////////////////////////////////////////////
// 
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    IMGControlSite.cpp
//
// Description: Implementation of a control-site for an image
// taking control of drawing and shaping the cursor over an image
// that has a mapping of areas (MAP/AREA) for hyperlinking
//
#include "stdafx.h"
#include "AfxOcc.h"
#include "IMGControlSite.h"
#include <math.h>

//////////////////////////////////////////////////////////////////////
//	Implementation of the IMGControlSite class
BEGIN_INTERFACE_MAP(IMGControlSite, COleControlSite)
	INTERFACE_PART(IMGControlSite, IID_IHTMLPainter,            HTMLPainter)
	INTERFACE_PART(IMGControlSite, IID_IElementBehavior,        HTMLElementBehavior)
	INTERFACE_PART(IMGControlSite, IID_IElementBehaviorFactory, HTMLElementBehaviorFactory)
END_INTERFACE_MAP()

IMGControlSite::IMGControlSite(COleControlContainer* pContainer) 
               :COleControlSite(pContainer)
{
 	m_imgCookie = NULL;
  m_Image     = NULL;
}

IMGControlSite::~IMGControlSite() 
{
  DetachBehavior();
}

void  
IMGControlSite::InstallIMGBehavior(HTMLDocEditDesigner* designer
                                  ,IHTMLDocument2*  doc
                                  ,IHTMLImgElement* pImage)
{
	HRESULT hr;
  CComVariant vFactory;

	if (pImage == (IHTMLImgElement *) NULL)
  {
    return;
  }
  CComQIPtr<IHTMLElement2,&IID_IHTMLElement2> pImage2 = pImage;
	if(pImage2.p == NULL)
	{
		return;
	}
  // Record our image element
  CComQIPtr<IHTMLElement,&IID_IHTMLElement> pElement = pImage;
  m_Image = pElement;
  m_Image->AddRef();

  // Record our designer
  m_xHTMLPainter.SetDesigner(designer);

	if (m_imgCookie)
	{
	 	VARIANT_BOOL dummy;
		hr = pImage2->removeBehavior(m_imgCookie, &dummy);

		//	MSHTML does an AddRef when you add a behaviour
		//	but it doesn't do a Release when you remove it.
		m_xHTMLElementBehaviorFactory.Release();
		m_imgCookie = NULL;
	}

	// Convert the grid factory pointer to the proper CComVariant data type 
	// for IHTMLElement2::AddBehavior
	V_VT(&vFactory) = VT_UNKNOWN;
  V_UNKNOWN(&vFactory) = &m_xHTMLElementBehaviorFactory;

	// Add Grid behavior
	hr = pImage2->addBehavior(NULL, &vFactory, &m_imgCookie);

  // Set Paint site
  //m_xHTMLPainter.SetPaintSite(m_xHTMLElementBehavior.m_spPaintSite);

  // Cache the IHTMLWindow2 interface pointer
  hr = doc->get_parentWindow(&m_spWin);

  // Init the HMTLPainter
  m_xHTMLPainter.InitPainter(doc,pElement);

	return;
}

void
IMGControlSite::AttachImgArea(CString id,ImgArea& area,bool setNew /*=false*/)
{
  m_xHTMLPainter.m_areas.insert(std::make_pair(id,area));
  if(setNew)
  {
    m_xHTMLPainter.m_lPartArea = &area;
  }
}

bool
IMGControlSite::DetachBehavior()
{
  VARIANT_BOOL result = VARIANT_FALSE;

  if(m_Image)
  {
    CComQIPtr<IHTMLElement2,&IID_IHTMLElement2> elem2 = m_Image;
    elem2->removeBehavior(m_imgCookie,&result);
    m_Image->Release();
    m_Image = NULL;
  }
  return (result == VARIANT_TRUE);
}

AreaShape
IMGControlSite::GetCurrentShape()
{
  if(m_xHTMLPainter.m_lPartArea)
  {
    return m_xHTMLPainter.m_lPartArea->GetShape();
  }
  return AREA_INVALID;
}

void
IMGControlSite::SetPartID(long partid)
{
  m_xHTMLPainter.m_lPartID = partid;
  if(partid < 0)
  {
    // Alsoo reset. No touchdouwn area if no endpoint
    m_xHTMLPainter.m_lPartArea = NULL;
  }
}

bool
IMGControlSite::TouchDown(POINT mouse)
{
  ImgAreaMap::iterator it = m_xHTMLPainter.m_areas.begin();
  while(it != m_xHTMLPainter.m_areas.end())
  {
    int num = it->second.TouchDown(mouse);
    if(num >= 0)
    {
      m_xHTMLPainter.m_lPartID   = num;
      m_xHTMLPainter.m_lPartArea = &(it->second);
      return true;
    }
    ++it;
  }
  return false;
}

void 
IMGControlSite::SetPolyCorner(long X,long Y,bool newContinue)
{
  if(m_xHTMLPainter.m_lPartArea)
  {
    if(newContinue)
    {
      m_xHTMLPainter.m_lPartArea->SetNewCorner(X,Y);
    }
    else if(m_xHTMLPainter.m_lPartID >=0)
    {
      m_xHTMLPainter.m_lPartArea->SetPolyCorner(m_xHTMLPainter.m_lPartID,X,Y);
    }
  }
}

void
IMGControlSite::RewriteArea()
{
  if(m_xHTMLPainter.m_lPartArea && 
     m_xHTMLPainter.m_lPartID >=0)
  {
    CComPtr<IDispatch> disp;
    m_xHTMLPainter.m_spElem->get_document(&disp);
    CComQIPtr<IHTMLDocument2,&IID_IHTMLDocument2> doc = disp;
    m_xHTMLPainter.m_lPartArea->RewriteArea(doc);
  }
}

bool
IMGControlSite::AddToNewArea(POINT mouse)
{
  ImgAreaMap::iterator it = m_xHTMLPainter.m_areas.begin();
  while(it != m_xHTMLPainter.m_areas.end())
  {
    if(it->second.IsPartial())
    {
      bool ready = it->second.AddDefintionPoint(mouse);
      if(ready)
      {
        CComPtr<IDispatch> disp;
        m_xHTMLPainter.m_spElem->get_document(&disp);
        CComQIPtr<IHTMLDocument2,&IID_IHTMLDocument2> doc = disp;
        it->second.RewriteArea(doc);
        it->second.SetNewCorner(0,0);
      }
      return ready;
    }
    ++it;
  }
  return false;
}

CString 
IMGControlSite::IDNewArea()
{
  ImgAreaMap::iterator it = m_xHTMLPainter.m_areas.begin();
  while(it != m_xHTMLPainter.m_areas.end())
  {
    if(it->second.IsPartial())
    {
      return it->second.ID();
    }
    ++it;
  }
  return "";
}

ImgArea*        
IMGControlSite::FindAreaByID(CString id)
{
  ImgAreaMap::iterator it = m_xHTMLPainter.m_areas.begin();
  while(it != m_xHTMLPainter.m_areas.end())
  {
    if(id.CompareNoCase(it->second.ID()) == 0)
    {
      return &(it->second);
    }
    ++it;
  }
  return NULL;
}

bool
IMGControlSite::RemoveArea(CString id)
{
  ImgAreaMap::iterator it = m_xHTMLPainter.m_areas.begin();
  while(it != m_xHTMLPainter.m_areas.end())
  {
    if(id.CompareNoCase(it->second.ID()) == 0)
    {
      m_xHTMLPainter.m_areas.erase(it);
      return true;
    }
    ++it;
  }
  return false;
}

/////////////////////////////////////////////
// 
// IHTMLElementBehavior
// 
/////////////////////////////////////////////

//	The behavior implementation. This is queried by MSHTML to get an instance 
//	of an IHTMLPainter. Our painter draws the grid.
//
IMGControlSite::XHTMLElementBehavior::XHTMLElementBehavior()
{
  m_spPaintSite = NULL;
}

STDMETHODIMP_(ULONG) 
IMGControlSite::XHTMLElementBehavior::AddRef()
{
	return 1;
}

STDMETHODIMP_(ULONG) 
IMGControlSite::XHTMLElementBehavior::Release()
{
	return 1;
}

STDMETHODIMP 
IMGControlSite::XHTMLElementBehavior::QueryInterface(REFIID iid, LPVOID far* ppvObj)     
{
	METHOD_PROLOGUE(IMGControlSite, HTMLElementBehavior);

	if (IsEqualIID(iid, IID_IUnknown))
	{
		*ppvObj = (IUnknown *) pThis;
		return S_OK;
	}
	else if (IsEqualIID(iid, IID_IHTMLPainter))
	{
		*ppvObj = (IUnknown *) &pThis->m_xHTMLPainter;
		return S_OK;
	}
  else if (IsEqualIID(iid, IID_IHTMLPainterEventInfo))
  {
    *ppvObj = (IUnknown *) &pThis->m_xHTMLPainter;
    return S_OK;
  }
	*ppvObj = NULL;
	return E_NOTIMPL;
}

STDMETHODIMP 
IMGControlSite::XHTMLElementBehavior::Init(IElementBehaviorSite *pBehaviorSite)
{
	HRESULT	hr = pBehaviorSite->QueryInterface(IID_IHTMLPaintSite, (void **) &m_spPaintSite);

	if (m_spPaintSite != (IHTMLPaintSite *) NULL)
  {
    m_spPaintSite->InvalidateRect(NULL);
  }
	return hr;
}

STDMETHODIMP 
IMGControlSite::XHTMLElementBehavior::Notify(LONG /*lEvent*/, VARIANT *pVar)
{
	if (pVar == NULL)
  {
    return E_POINTER;
  }
	return E_NOTIMPL;
}

STDMETHODIMP 
IMGControlSite::XHTMLElementBehavior::Detach()
{
	m_spPaintSite->InvalidateRect(NULL);
	return S_OK;
}

/////////////////////////////////////////////
// 
// IHTMLElementBehaviorFactory
// 
/////////////////////////////////////////////

STDMETHODIMP_(ULONG) 
IMGControlSite::XHTMLElementBehaviorFactory::AddRef()
{
	return 1;
}

STDMETHODIMP_(ULONG) 
IMGControlSite::XHTMLElementBehaviorFactory::Release()
{
	return 1;
}

STDMETHODIMP 
IMGControlSite::XHTMLElementBehaviorFactory::QueryInterface(REFIID iid, LPVOID far* ppvObj)     
{
	METHOD_PROLOGUE(IMGControlSite, HTMLElementBehaviorFactory);

	*ppvObj = NULL;

	if (IsEqualIID(iid, IID_IElementBehaviorFactory))
	{
		*ppvObj = (IUnknown *) this;
		return S_OK;
	}
	else if (IsEqualIID(iid, IID_IElementBehavior))
	{
		*ppvObj = (IUnknown *) &pThis->m_xHTMLElementBehavior;
		return S_OK;
	}
	return E_NOTIMPL;
}

STDMETHODIMP 
IMGControlSite::XHTMLElementBehaviorFactory::FindBehavior(BSTR /*bstrBehavior*/, BSTR /*bstrBehaviorUrl*/, IElementBehaviorSite * /*pSite*/, IElementBehavior** ppBehavior)
{
	METHOD_PROLOGUE(IMGControlSite, HTMLElementBehaviorFactory)

	*ppBehavior = &pThis->m_xHTMLElementBehavior;
	return S_OK;
}

/////////////////////////////////////////////
// 
// IHTMLPainter
// 
/////////////////////////////////////////////

IMGControlSite::XHTMLPainter::XHTMLPainter()
{
  m_spPaintSite = NULL;
  m_lPartArea   = NULL;
}

STDMETHODIMP
IMGControlSite::XHTMLPainter::InitPainter(CComPtr<IHTMLDocument2> /*doc*/
                                         ,CComPtr<IHTMLElement> pElem)
{
  // Cache the IHTMLElement interface pointer for this element
  // and its parent
  m_spElem = pElem;
  m_spElem->get_offsetParent(&m_spParent);

  long lHeight, lWidth;

  m_spElem->get_offsetHeight(&lHeight);
  m_spElem->get_offsetWidth(&lWidth);

  // Cache the initial coordinates of the element's 
  // corner points
  m_rtElemCorners.top = 0;
  m_rtElemCorners.bottom = lHeight; 
  m_rtElemCorners.left = 0;
  m_rtElemCorners.right = lWidth;

  return S_OK;
}

STDMETHODIMP
IMGControlSite::XHTMLPainter::SetPaintSite(IHTMLPaintSite* site)
{
  m_spPaintSite = site;
  return S_OK;
}

STDMETHODIMP_(ULONG) 
IMGControlSite::XHTMLPainter::AddRef()
{
	return 1;
}

STDMETHODIMP_(ULONG) 
IMGControlSite::XHTMLPainter::Release()
{
	return 1;
}

STDMETHODIMP 
IMGControlSite::XHTMLPainter::QueryInterface(REFIID iid, LPVOID far* ppvObj)     
{
	METHOD_PROLOGUE(IMGControlSite, HTMLPainter);

  if(IsEqualIID(iid, IID_IHTMLPainterEventInfo))
  {
    *ppvObj = (IUnknown *) &(pThis->m_xHTMLPainterEventInfo);
    return S_OK;
  }
	return pThis->ExternalQueryInterface(&iid, ppvObj);
}

STDMETHODIMP
IMGControlSite::XHTMLPainter::SetDesigner(HTMLDocEditDesigner* designer)
{
  m_designer = designer;
  return S_OK;
}

// DRAW ON THE IMAGE!!
STDMETHODIMP 
IMGControlSite::XHTMLPainter::Draw(RECT rcBounds
                                  ,RECT /* rcUpdate */
                                  ,LONG /* lDrawFlags */
                                  ,HDC hdc
                                  ,LPVOID /* pvDrawObject */)
{
  //HBRUSH hollowBrush    = (HBRUSH)GetStockObject(HOLLOW_BRUSH);
  HBRUSH coloredBrush   = CreateSolidBrush(RGB(0x99,0x66,0x99));
  HPEN   redPen         = (HPEN)CreatePen(PS_SOLID,0,RGB(0xff,0x00,0x00));

  HPEN   oldPen         = (HPEN)SelectObject(hdc,redPen);
  HBRUSH oldBrush       = (HBRUSH)SelectObject(hdc,coloredBrush);

  ImgAreaMap::iterator it = m_areas.begin();
  while(it != m_areas.end())
  {
    ImgArea* area = &(it->second);
    int point = -1;
    if(area == m_lPartArea && m_designer->m_mouseDown)
    {
      point = m_lPartID;
    }
    if(area->HasNewCorner())
    {
      area->DrawNew(hdc,rcBounds);
    }
    else
    {
      area->Draw(hdc,rcBounds,point);
    }
    // next area
    ++it;
  }
  SelectObject(hdc, oldPen);
  SelectObject(hdc, oldBrush);

  DeleteObject(coloredBrush);
  DeleteObject(redPen);

  return S_OK;
}

STDMETHODIMP 
IMGControlSite::XHTMLPainter::OnResize(SIZE /*size*/)
{
 return S_OK;
}

STDMETHODIMP 
IMGControlSite::XHTMLPainter::GetPainterInfo(HTML_PAINTER_INFO *pInfo)
{
	if (pInfo == NULL)
  {
		return E_POINTER;
  }
  pInfo->lFlags  = HTMLPAINTER_OPAQUE | HTMLPAINTER_HITTEST; // | HTMLPAINTER_NOSAVEDC;
  pInfo->lZOrder = HTMLPAINT_ZORDER_ABOVE_CONTENT;

	memset(&pInfo->iidDrawObject, 0, sizeof(IID));

  pInfo->rcExpand.left   = HANDLE_RADIUS;
  pInfo->rcExpand.right  = HANDLE_RADIUS;
  pInfo->rcExpand.top    = HANDLE_RADIUS;
  pInfo->rcExpand.bottom = HANDLE_RADIUS;

	return S_OK;
}

STDMETHODIMP 
IMGControlSite::XHTMLPainter::HitTestPoint(POINT pt, BOOL *pbHit, LONG *plPartID)
{
  *pbHit = FALSE;
  *plPartID = NULL;
  m_lPartID = -1;

  // Convert point coordinates, which are given with respect to the element's
  // expanded region, to reflect the element without its expanded region
  pt.x = pt.x - HANDLE_RADIUS;
  pt.y = pt.y - HANDLE_RADIUS;

  ImgAreaMap::iterator it = m_areas.begin();
  while(it != m_areas.end())
  {
    int partid = it->second.TouchDown(pt);
    if(partid >= 0)
    {
      // Setting pbHit to TRUE will trigger an event that can be handled
      // by IEventSink::Invoke
      *pbHit = TRUE;
      *plPartID = partid;
      m_lPartID = partid;
      m_lPartArea = &(it->second);
      break;
    }
    // next area
    ++it;
  }

  // If no hit detected, we are on the 'rest' of the image
  if(*pbHit == FALSE)
  {
    *pbHit = TRUE;
    *plPartID = IMAGE_POSITIONS;
  }
  return S_OK;
}

/////////////////////////////////////////////
// 
// IHTMLPainterEventInfo part
// 
/////////////////////////////////////////////

IMGControlSite::XHTMLPainterEventInfo::XHTMLPainterEventInfo()
{
}

STDMETHODIMP 
IMGControlSite::XHTMLPainterEventInfo::QueryInterface(REFIID riid,void** ppvObject)
{
  METHOD_PROLOGUE(IMGControlSite, HTMLPainterEventInfo);
  return pThis->ExternalQueryInterface(&riid, ppvObject);
}

STDMETHODIMP_(ULONG) 
IMGControlSite::XHTMLPainterEventInfo::AddRef()
{
  return 1;
}

STDMETHODIMP_(ULONG) 
IMGControlSite::XHTMLPainterEventInfo::Release()
{
  return 1;
}

STDMETHODIMP
IMGControlSite::XHTMLPainterEventInfo::GetEventInfoFlags(long *plEventInfoFlags)
{
  // Request to set the cursor ourselves
  *plEventInfoFlags = HTMLPAINT_EVENT_SETCURSOR;
  return S_OK;
}

STDMETHODIMP 
IMGControlSite::XHTMLPainterEventInfo::GetEventTarget(IHTMLElement** /*ppElement*/)
{
  return E_NOTIMPL;
}

STDMETHODIMP
IMGControlSite::XHTMLPainterEventInfo::SetCursor(LONG lPartID)
{
	METHOD_PROLOGUE(IMGControlSite, HTMLPainterEventInfo);
  HCURSOR curs;
  AreaShape shape = pThis->GetCurrentShape();
  switch(lPartID)
  {
    case 0:     // Origin of the shape -> move it
                curs = ::LoadCursor(NULL,IDC_SIZEALL);
                break;
    case 1:     // Rectangles -> west-east
                if(shape == AREA_POLYGON || shape == AREA_CIRCLE)
                {
                  curs = ::LoadCursor(NULL,IDC_HAND);
                }
                else
                {
                  curs = ::LoadCursor(NULL,IDC_SIZEWE);
                }
                break;
    case 2:     // Rectangle -> nw-se
                if(shape == AREA_RECTANGLE)
                {
                  curs = ::LoadCursor(NULL,IDC_SIZENWSE);
                }
                else
                {
                  curs = ::LoadCursor(NULL,IDC_HAND);
                }
                break;
    case 3:     // Rectangle -> north-south
                if(shape == AREA_RECTANGLE)
                {
                  curs = ::LoadCursor(NULL,IDC_SIZENS);
                }
                else
                {
                  curs = ::LoadCursor(NULL,IDC_HAND);
                }
                break;
    case IMAGE_POSITIONS:
                curs = ::LoadCursor(NULL,IDC_CROSS);
                break;
    default:    // Rest of the polygon points
                curs = :: LoadCursor(NULL,IDC_HAND);
                break;
  }
  ::SetCursor(curs);
  return S_OK;
}

STDMETHODIMP
IMGControlSite::XHTMLPainterEventInfo::StringFromPartID(LONG /*lPartID*/,BSTR* /*pbstrPart*/)
{
  return E_NOTIMPL;
}

