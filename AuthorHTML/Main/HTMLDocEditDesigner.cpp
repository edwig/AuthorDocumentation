//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    HTMLDocEditDesigner.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Implementation of an edit designer
// for the document. Catches the events of the m_spBrowser
// and redirects it to our behavior sites
// (CHTMLControlSite & IMGControlSite)
//
#include "StdAfx.h"
#include <Mshtmdid.h>
#include "HTMLDocEditDesigner.h"
#include "HtmlTable.h"
#include "HTMLTableRow.h"
#include "HTMLTableCell.h"
#include "IMGControlSite.h"
#include "HTMLEdView.h"
#include <algorithm>
#include "AreaDlg.h"

#define BUTTON_LEFT   0x001
#define BUTTON_RIGHT  0x002
#define BUTTON_MIDDLE 0x004
#define MARGIN 6

#define CELL_TOP      0x001
#define CELL_LEFT     0x002
#define CELL_RIGHT    0x003
#define CELL_BOTTOM   0x004

HTMLDocEditDesigner::HTMLDocEditDesigner()
{
  m_mouseDown   = false;
  m_newContinue = false;
	m_uRefCount   = 0;
  m_newShape    = AREA_INVALID;
	m_pServices   = (IHTMLEditServices   *) NULL;
  m_controlSite = (CHTMLEditControlSite*) NULL;
}

HTMLDocEditDesigner::~HTMLDocEditDesigner()
{
  DetachImages();
}


HRESULT STDMETHODCALLTYPE 
HTMLDocEditDesigner::QueryInterface(REFIID riid, void __RPC_FAR *__RPC_FAR *ppvObject)
{
	HRESULT hrRet = S_OK;

	// Standard QI() initialization - set *ppv to NULL.
	*ppvObject = NULL;

	// If the client is requesting an interface we support, set *ppvObject.
	if (IsEqualIID(riid, IID_IUnknown))
  {
    *ppvObject = (IUnknown *) this;
  }
	else if (IsEqualIID(riid, IID_IHTMLEditDesigner))
  {
    *ppvObject = (IHTMLEditDesigner *) this;
  }
	else
  {
    // We don't support the interface the client is asking for.
		hrRet = E_NOINTERFACE;
  }
	// If we're returning an interface pointer, AddRef() it.
	if (S_OK == hrRet)
  {
    ((IUnknown *) *ppvObject)->AddRef();
  }
	return hrRet;
}

ULONG STDMETHODCALLTYPE 
HTMLDocEditDesigner::AddRef(void)
{
	return ++m_uRefCount;
}

ULONG STDMETHODCALLTYPE 
HTMLDocEditDesigner::Release(void)
{
	return --m_uRefCount;
}

HRESULT STDMETHODCALLTYPE 
HTMLDocEditDesigner::PreHandleEvent(DISPID inEvtDispId
                                   ,IHTMLEventObj *pIEventObj)
{
  USES_CONVERSION;
  static bool   mouseout   = false;
  static bool   horizontal = true;
  static int    side       = 0;
  static int    orig_x     = 0;
  static int    orig_y     = 0;
  static int    elem_w     = 0;
  static int    elem_h     = 0;
  static IHTMLElement* origin = NULL;
  static IMGControlSite* imageBehavior = NULL;

  // SPEED OPTIMIZATIONS !!
  if(inEvtDispId == DISPID_HTMLELEMENTEVENTS2_ONMOUSEMOVE)
  {
    if(imageBehavior)
    {
      IHTMLElement* pSel;
      CComBSTR		  b = CComBSTR(NULL);
      pIEventObj->get_srcElement(&pSel);
      //	We've got our source element, get it's tag
      if (pSel != (IHTMLElement *) NULL)
      {
        pSel->get_tagName(&b);
        if (_tcsicmp("IMG", W2A(b)) == 0 ||
            _tcsicmp("AREA",W2A(b)) == 0 )
        {
          //ForbidSelection(imageBehavior->GetImage());
			    // If mouse button is not down, don't do anything
			    if (m_mouseDown || m_newContinue)
          {
			      // Obtain X & Y
            long X,Y;
			      pIEventObj->get_offsetY(&Y);
			      pIEventObj->get_offsetX(&X);

            imageBehavior->SetPolyCorner(X,Y,m_newContinue);
			      imageBehavior->GetPaintSite()->InvalidateRect(NULL);
            return S_FALSE;
          }
        }
      }
    }
		if (pIEventObj != (IHTMLEventObj *) NULL)
		{
      IHTMLElement* pSel;
      CComBSTR		  b = CComBSTR(NULL);

			pIEventObj->get_srcElement(&pSel);
			//	We've got our source element, get it's tag
			if (pSel != (IHTMLElement *) NULL)
			{
				pSel->get_tagName(&b);
				if (_tcsicmp("TD", W2A(b)) == 0)
				{
          long cx,cy,x,y,w,h;
          pIEventObj->get_clientX(&cx);
          pIEventObj->get_clientY(&cy);
          pIEventObj->get_offsetX(&x);
          pIEventObj->get_offsetY(&y);
          CComQIPtr<IHTMLElement2,&IID_IHTMLElement2> elem2 = pSel;
          elem2->get_clientWidth(&w);
          elem2->get_clientHeight(&h);
          w -= MARGIN;
          h -= MARGIN;

          if(mouseout)
          {
            long button = 0;
            pIEventObj->get_button(&button);
            if(button & BUTTON_LEFT)
            {
              ForbidSelection(pSel);
              long xcoord = 0;
              long ycoord = 0;
              pIEventObj->get_clientX(&xcoord);
              pIEventObj->get_clientY(&ycoord);
              TRACE("DRAG: %d,%d\n",xcoord,ycoord);
              if(m_controlSite)
              {
                m_controlSite->SetDragLine(true
                                          ,horizontal
                                          ,xcoord
                                          ,ycoord);
              }
            }
            else
            {
              if(!(x <= MARGIN || x >= w || y <= MARGIN || y >= h))
              {
                mouseout = false;
                if(m_controlSite)
                {
                  m_controlSite->SetDragLine(false);
                }
                // Restore cursor
                // SetCursor(m_cursor);
                // ForbidSelection(pSel);
                if(origin)
                {
                  origin->Release();
                  origin = NULL;
                }
              }
            }
            return S_FALSE;
          }
          else // Not yet mouseout
          {
            if(x <= MARGIN || x >= w || y <= MARGIN || y >= h)
            {
              mouseout = true;
              orig_x   = cx;
              orig_y   = cy;
              elem_h   = h;
              elem_w   = w;
              if(origin)
              {
                origin->Release();
              }
              origin   = pSel;
              origin->AddRef();

              //ForbidSelection(pSel);
              TRACE("Out of cell: ");
              if(x <= MARGIN)
              {
                TRACE("Left\n");
                side = CELL_LEFT;
              }
              if(x >= w)
              {
                TRACE("Right\n");
                side = CELL_RIGHT;
              }
              if(y <= MARGIN)
              {
                TRACE("Top\n");
                side = CELL_TOP;
              }
              if(y >= h)
              {
                TRACE("Bottom\n");
                side = CELL_BOTTOM;
              }
              if(y <= MARGIN || y >= h) 
              {
                horizontal = true;
              }
              else // if(x <= 0 || x >= w) 
              {
                horizontal = false;
              }
              return S_FALSE;
            }
          }
        }
      }
    }
  }
  else if(inEvtDispId == DISPID_HTMLELEMENTEVENTS2_ONMOUSEDOWN)
  {
    // Is it a context menu?
    long button;
    pIEventObj->get_button(&button);
    if(button == 2)
    {
      long screenX;
      long screenY;
      pIEventObj->get_screenX(&screenX);
      pIEventObj->get_screenY(&screenY);
      POINT ppt;
      ppt.x = screenX;
      ppt.y = screenY;
      ::ClipCursor(NULL);
      m_view->OnShowContextMenu(inEvtDispId,&ppt,NULL,NULL);
      return TRUE;
    }
    // Declare source element and get the tag
    CComPtr<IHTMLElement> pSel;
    CComBSTR b;
    pIEventObj->get_srcElement(&pSel);
    pSel->get_tagName(&b);
    CString tag = W2A(b);

    // See if we are in business for a new area to be defined
    if(m_newShape)
    {
      if(tag.CompareNoCase("img") == 0)
      {
        if(m_newContinue)
        {
          if(AddToImgArea(pIEventObj,pSel))
          {
            // Ready with new shape
            imageBehavior = NULL;
          }
        }
        else
        {
          imageBehavior = NewImgArea(pIEventObj,pSel);
          imageBehavior->GetPaintSite()->InvalidateRect(NULL);
        }
      }
    }
    else
    {
      // Not a mousedown after a new shape
      // see if we are trying to edit area on an image
      if(tag.CompareNoCase("area") == 0)
      {
        CComPtr<IDispatch> disp;
        pSel->get_document(&disp);
        CComQIPtr<IHTMLDocument2,&IID_IHTMLDocument2> doc = disp;

        pSel = Misc::AreaToImg(doc,pSel);
		    pSel->get_tagName(&b);
        tag = W2A(b);
      }
		  if(tag.CompareNoCase("img") == 0)
		  {
        IMGControlSite* behavior = FindIMGSite(pSel);    
        if(behavior)
        {
          // Mouse down on an image with behavior
          imageBehavior = behavior;
          MouseDownOnImage(pIEventObj,behavior);
          if(m_mouseDown)
          {
            return S_OK;
          }
        }
      }
    }
    return S_FALSE;
  }
  else if(inEvtDispId == DISPID_HTMLELEMENTEVENTS2_ONMOUSEUP)
  {
    if(imageBehavior)
    {
      if(!m_newContinue)
      {
        ForbidSelection(imageBehavior->GetImage());
			  m_mouseDown = FALSE;
			  // Free mouse cursor to move anywhere on screen
			  ClipCursor(NULL);

        // Rewrite area to document
        imageBehavior->RewriteArea();

			  imageBehavior->SetPartID(-1);
			  imageBehavior->GetPaintSite()->InvalidateRect(NULL);
        imageBehavior = NULL;
        return S_OK;
      }
      else
      {
        imageBehavior->GetPaintSite()->InvalidateRect(NULL);
        return S_OK;
      }
    }
    if(mouseout)
    {
		  if (pIEventObj != (IHTMLEventObj *) NULL)
		  {
        IHTMLElement* pSel;
			  pIEventObj->get_srcElement(&pSel);
			  if (pSel != (IHTMLElement *) NULL)
			  {
          mouseout   = false;

          // Get coordinates at mouse-up time
          long xcoord = 0;
          long ycoord = 0;
          pIEventObj->get_clientX(&xcoord);
          pIEventObj->get_clientY(&ycoord);

          if(m_controlSite)
          {
            m_controlSite->SetDragLine(false);
          }
          // Restore cursor / caret
          //SetCursor(m_cursor);
          ForbidSelection(pSel);

          // Make table cell / table from this element
          HtmlTableCell cell(origin);
          HtmlTable table = cell.GetParentTable();
          UINT curCol = cell.GetColIndex();
          if(!horizontal)
          {
            int delta = (int) (orig_x - xcoord);
            if(side == CELL_LEFT)
            {
              table.AlterColumnWidth(curCol - 1,-delta,elem_w);
              table.AlterColumnWidth(curCol,     delta,elem_w);
            }
            else if(side == CELL_RIGHT)
            {
              table.AlterColumnWidth(curCol,    -delta,elem_w);
              table.AlterColumnWidth(curCol + 1, delta,elem_w);
            }
          }
          // Reset side counter
          side = 0;
          if(origin)
          {
            origin->Release();
            origin = NULL;
          }
        }
      }
      return S_OK;
    }
  }
	return S_FALSE;
}

void
HTMLDocEditDesigner::ForbidSelection(IHTMLElement* elem)
{
  CComPtr<IDispatch> disp;
  CComPtr<IHTMLSelectionObject> sel;
  if(elem)
  {
    elem->get_document(&disp);
    CComQIPtr<IHTMLDocument2,&IID_IHTMLDocument2> doc = disp;
    HRESULT hr = doc->get_selection(&sel);
    if(SUCCEEDED(hr) && sel.p)
    {
      sel->empty();
    }
  }
}

void
HTMLDocEditDesigner::MouseDownOnImage(IHTMLEventObj*  pIEvent
                                     ,IMGControlSite* behavior)
{
	POINT ptElemOriginLocal;
  POINT ptElemOriginGlobal;
	IHTMLWindow3* pWin3;
	long winTop;
  long winLeft;
	RECT clipRect;

	ptElemOriginLocal.x = HANDLE_RADIUS;
	ptElemOriginLocal.y = HANDLE_RADIUS;

	behavior->GetPaintSite()->TransformLocalToGlobal(ptElemOriginLocal, &ptElemOriginGlobal);

	behavior->m_spWin->QueryInterface(IID_IHTMLWindow3, (void**)&pWin3);
	pWin3->get_screenTop(&winTop);
	pWin3->get_screenLeft(&winLeft);
	pWin3->Release();

  RECT rt = behavior->GetPainterElementCorners();
	clipRect.top    = rt.top    + ptElemOriginGlobal.y + winTop;
	clipRect.bottom = rt.bottom + ptElemOriginGlobal.y + winTop;
	clipRect.left   = rt.left   + ptElemOriginGlobal.x + winLeft;
	clipRect.right  = rt.right  + ptElemOriginGlobal.x + winLeft;
	
	// Invoke ClipCursor
  ::ClipCursor(&clipRect);

	// Prevent selection on this element
  ForbidSelection(behavior->GetImage());

	// Obtain X & Y
	long X = NULL;
	long Y = NULL;
	pIEvent->get_offsetY(&Y);
	pIEvent->get_offsetX(&X);
  		
	POINT mousePoint;
	mousePoint.x = X;
	mousePoint.y = Y;

	behavior->SetPartID(-1);
  m_mouseDown = behavior->TouchDown(mousePoint);
}

IMGControlSite*
HTMLDocEditDesigner::NewImgArea(IHTMLEventObj*  pIEvent
                               ,CComPtr<IHTMLElement> pElement)
{
  CComPtr<IDispatch> disp;
  pElement->get_document(&disp);
  CComQIPtr<IHTMLDocument2,&IID_IHTMLDocument2> doc = disp;
  CComQIPtr<IHTMLImgElement,&IID_IHTMLImgElement> image = pElement;

  CString mapid;
  IMGControlSite* behavior = FindIMGSite(pElement);    
  if(behavior)
  {
    // Find mapid
    CComBSTR bMapID;
    image->get_useMap(&bMapID);
    mapid = CW2CT(bMapID);
    if(mapid.GetLength() > 1)
    {
      if(mapid.GetAt(0) == '#')
      {
        mapid = mapid.Mid(1);
      }
    }
  }
  if(mapid.IsEmpty())
  {
    // New MAP/AREA needed
    //1) Make map
    mapid = Misc::CreateNewMap(doc);
    //2) set UseMap
    CString imgMapid = CString("#") + mapid;
    CComBSTR bMapID = CT2CW(imgMapid);
    image->put_useMap(bMapID);
    //3) Make behavior
     behavior= new IMGControlSite(NULL);
    //4) InstallImageBehavior
    // Connect the designer, Document and image to this behaviour
    behavior->InstallIMGBehavior(this,doc,image);
     // Record this behaviour in this designer, so it can find the image
     AttachImg(behavior);
  }
  //5 ) Create first area in map / behavior
  CString cID;
  cID.Format("area%d",Misc::GetUniqueID(doc,"area"));
  CString shape;
  switch(m_newShape)
  {
    case AREA_POLYGON:   shape = "poly"; break;
    case AREA_CIRCLE:    shape = "circ"; break;
    case AREA_RECTANGLE: 
    case AREA_INVALID:
    default:             shape = "rect"; break;
  }
  Misc::CreateNewArea(doc,mapid,cID,shape);
	// Obtain X & Y
	long X = NULL;
	long Y = NULL;
	pIEvent->get_offsetY(&Y);
	pIEvent->get_offsetX(&X);
  		
  POINT center;
  center.x = X;
  center.y = Y;
  // Add area to IMG behavior
  ImgArea area(m_newShape,cID,center);

  // setNew (3e param) for drawing new areas
  behavior->AttachImgArea(cID,area,true);

  // Next mousedown is next point on this area!
  m_newContinue = true;

  return behavior;
}

bool
HTMLDocEditDesigner::AddToImgArea(IHTMLEventObj*  pIEvent
                                 ,CComPtr<IHTMLElement> pElement)
{
  CComPtr<IDispatch> disp;
  pElement->get_document(&disp);
  CComQIPtr<IHTMLDocument2,&IID_IHTMLDocument2> doc = disp;

  IMGControlSite* behavior = FindIMGSite(pElement);    

  if(behavior)
  {
	  // Obtain X & Y
	  long X = NULL;
	  long Y = NULL;
	  pIEvent->get_offsetY(&Y);
	  pIEvent->get_offsetX(&X);
    		
    POINT mouse;
    mouse.x = X;
    mouse.y = Y;

    CString id = behavior->IDNewArea();
    if(behavior->AddToNewArea(mouse))
    {
      // OK, Shape is ready. now fill it
      m_newShape    = AREA_INVALID;
      m_newContinue = false;

      CComPtr<IHTMLElement> pElem = Misc::GetByID(doc,id);
      if(pElem.p)
      {
        HtmlArea area(pElem);
        AreaDlg diag(NULL,&area,m_view->GetBase());
        if(diag.DoModal() == IDOK)
        {
          if(diag.GetDoPopup())
          {
            // Check for Popup SCRIPT section in head
            m_view->CheckPopupSCRIPT();
          }
        }
      }
      // Ready with new shape
      return true;
    }
  }
  return false;
}

HRESULT STDMETHODCALLTYPE 
HTMLDocEditDesigner::PostHandleEvent(DISPID /*inEvtDispId*/, IHTMLEventObj * /*pIEventObj*/)
{
	return S_FALSE;
}

HRESULT STDMETHODCALLTYPE 
HTMLDocEditDesigner::TranslateAccelerator(DISPID /*inEvtDispId*/, IHTMLEventObj * /*pIEventObj*/)
{
	return S_FALSE;
}

HRESULT STDMETHODCALLTYPE 
HTMLDocEditDesigner::PostEditorEventNotify(DISPID /*inEvtDispId*/, IHTMLEventObj* /*pIEventObj*/)
{
	return S_FALSE;
}

IMGControlSite*
HTMLDocEditDesigner::FindIMGSite(CComPtr<IHTMLElement> pElement)
{
  // Hack to compare two elements for IMG's
  CComVariant source;
  V_VT(&source) = VT_BSTR;

  HRESULT hr = pElement->getAttribute(L"src",(long)2,&source);
  if(!SUCCEEDED(hr))
  {
    return NULL;
  }
  CString csource = CW2CT(V_BSTR(&source));

  size_t count = m_imgSites.size();
  for(size_t ind = 0;ind < count; ++ind)
  {
    IMGControlSite* ics = m_imgSites[ind];
    ics->GetImage()->getAttribute(L"src",(long)2,&source);
    CString theSource = CW2CT(V_BSTR(&source));
    if(theSource.CompareNoCase(csource) == 0)
    {
      return ics;
    }
  }
  return NULL;
}

BOOL 
HTMLDocEditDesigner::Attach(CHTMLEdView* view
                           ,IHTMLDocument2 *pDoc
                           ,CHTMLEditControlSite* site)
{
  m_view = view;
	if (m_pServices	!= (IHTMLEditServices *) NULL)
  {
    m_pServices->Release();
  }
	IServiceProvider *pTemp;

	if (pDoc ==	(IHTMLDocument2	*) NULL)
  {
    return FALSE;
  }
  m_controlSite = site;

	pDoc->QueryInterface(IID_IServiceProvider, (void **) &pTemp);

	if (pTemp != (IServiceProvider *) NULL)
	{
		pTemp->QueryService(SID_SHTMLEditServices, IID_IHTMLEditServices, (void	**)	&m_pServices);

		if (m_pServices	!= (IHTMLEditServices *) NULL)
		{
			m_pServices->AddDesigner(this);
			return TRUE;
		}
	}

	return FALSE;
}

void 
HTMLDocEditDesigner::Detach()
{
	if (m_pServices != (IHTMLEditServices *) NULL)
  {
    m_pServices->RemoveDesigner(this);
  }
}

bool 
HTMLDocEditDesigner::AttachImg(IMGControlSite* ics)
{
  ImgSites::iterator it = std::find(m_imgSites.begin(),m_imgSites.end(),ics);
  if(it == m_imgSites.end())
  {
    m_imgSites.push_back(ics);
    return true;
  }
  return false;
}

bool
HTMLDocEditDesigner::DetachImg(IHTMLElement* pElement)
{
  size_t count = m_imgSites.size();
  for(size_t ind = 0;ind < count; ++ind)
  {
    IMGControlSite* ics = m_imgSites[ind];
    if(ics->GetImage() == pElement)
    {
      // Found it;
      ImgSites::iterator it = std::find(m_imgSites.begin(),m_imgSites.end(),ics);
      if(it != m_imgSites.end())
      {
        m_imgSites.erase(it);
        delete ics;
        return true;
      }
    }
  }
  return false;
}

void
HTMLDocEditDesigner::DetachImages()
{
  size_t count = m_imgSites.size();
  for(size_t ind = 0;ind < count; ++ind)
  {
    IMGControlSite* ics = m_imgSites[ind];
    delete ics;
  } 
  m_imgSites.clear();
}
  
//	THe dispatcher for the designer.

HRESULT STDMETHODCALLTYPE 
HTMLDocEditDesignerDispatch::QueryInterface(REFIID riid, void __RPC_FAR *__RPC_FAR *ppvObject)
{
	HRESULT hrRet = S_OK;

	// Standard QI() initialization - set *ppv to NULL.
	*ppvObject = NULL;

	// If the client is requesting an interface we support, set *ppv.
	if (IsEqualIID(riid, IID_IUnknown))
  {
    *ppvObject = (IUnknown *) this;
  }
	else if (IsEqualIID(riid, IID_IDispatch))
  {
    *ppvObject = (IDispatch *) this;
  }
	else
  {
		// We don't support the interface the client is asking for.
		hrRet = E_NOINTERFACE;
  }
	// If we're returning an interface pointer, AddRef() it.
	if (S_OK == hrRet)
  {
    ((IUnknown *) *ppvObject)->AddRef();
  }
	return hrRet;
}

ULONG STDMETHODCALLTYPE 
HTMLDocEditDesignerDispatch::AddRef(void)
{
	return ++m_uRefCount;
}

ULONG STDMETHODCALLTYPE 
HTMLDocEditDesignerDispatch::Release(void)
{
	return --m_uRefCount;
}

HRESULT STDMETHODCALLTYPE 
HTMLDocEditDesignerDispatch::GetTypeInfoCount(UINT *pctinfo)
{
	*pctinfo = 0;
	return S_OK;
}

HRESULT STDMETHODCALLTYPE 
HTMLDocEditDesignerDispatch::GetTypeInfo(UINT /*iTInfo*/, LCID /*lcid*/, ITypeInfo ** /*ppTInfo*/)
{
	return DISP_E_BADINDEX;
}

HRESULT STDMETHODCALLTYPE 
HTMLDocEditDesignerDispatch::GetIDsOfNames(REFIID /*riid*/, LPOLESTR * /*rgszNames*/, UINT /*cNames*/, LCID /*lcid*/, DISPID * /*rgDispId*/)
{
	return DISP_E_UNKNOWNNAME;
}

HRESULT STDMETHODCALLTYPE 
HTMLDocEditDesignerDispatch::Invoke(DISPID /*dispIdMember*/, REFIID /*riid*/, LCID /*lcid*/, WORD /*wFlags*/, DISPPARAMS* /*pDispParams*/, VARIANT* /*pVarResult*/, EXCEPINFO* /*pExcepInfo*/, UINT* /*puArgErr*/)
{
	//	If we were installed it means we should disable
	//	dragging. So set the return value to false
	//pVarResult->vt = VT_BOOL;
	//pVarResult->boolVal = false;
	return S_FALSE;
}
