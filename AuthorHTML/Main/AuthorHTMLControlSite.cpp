//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    AuthorHTMLControlSite.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Handles the MSHTML OCX COM-Interfaces sink
//
#include "stdafx.h"
#include "AfxOcc.h"
#include "AuthorHTMLControlSite.h"

DEFINE_GUID(SID_SHTMLEditHost, 0x3050f6a0, 0x98b5, 0x11cf, 0xbb, 0x82, 0x00, 0xaa, 0x00, 0xbd, 0xce, 0x0b);

//////////////////////////////////////////////////////////////////////
//	Implementation of the CHTMLEditControlSite class, including the
//	IDocHostUIHandler implementation
BEGIN_INTERFACE_MAP(CHTMLEditControlSite, COleControlSite)
	INTERFACE_PART(CHTMLEditControlSite, IID_IDocHostUIHandler,       DocHostUIHandler)
	INTERFACE_PART(CHTMLEditControlSite, IID_IServiceProvider,        ServiceProvider)
	INTERFACE_PART(CHTMLEditControlSite, IID_IHTMLEditHost,           HTMLEditHost)
	INTERFACE_PART(CHTMLEditControlSite, IID_IHTMLPainter,            HTMLPainter)
	INTERFACE_PART(CHTMLEditControlSite, IID_IElementBehavior,        HTMLElementBehavior)
	INTERFACE_PART(CHTMLEditControlSite, IID_IElementBehaviorFactory, HTMLElementBehaviorFactory)
END_INTERFACE_MAP()

CHTMLEditControlSite::CHTMLEditControlSite(COleControlContainer* pContainer) 
                     :COleControlSite(pContainer)
{
 	m_gridCookie  = NULL;
}

inline CHtmlView* 
CHTMLEditControlSite::GetView() const
{
	return STATIC_DOWNCAST(CHtmlView, m_pCtrlCont->m_pWnd);
}

void 
CHTMLEditControlSite::InstallGrid(IHTMLDocument2 *pDoc)
{
	HRESULT hr;

	IHTMLElement  *pBody = NULL;
	IHTMLElement2 *pBody2;
    CComVariant		  vFactory;

	if (pDoc == (IHTMLDocument2 *) NULL)
  {
    return;
  }
	// Get IHTMLElement and IHTMLElement2 interfaces for the body
	hr = pDoc->get_body(&pBody);

	if (pBody == (IHTMLElement *) NULL)
  {
    return;
  }
	hr = pBody->QueryInterface(IID_IHTMLElement2, (void **) &pBody2);

	if (pBody2 == (IHTMLElement2 *) NULL)
	{
		pBody->Release();
		return;
	}

	if (m_gridCookie)
	{
		VARIANT_BOOL dummy;
		hr = pBody2->removeBehavior(m_gridCookie, &dummy);

		//	MSHTML does an AddRef when you add a behaviour
		//	but it doesn't do a Release when you remove it.
		m_xHTMLElementBehaviorFactory.Release();
		m_gridCookie = NULL;
	}

	// Convert the grid factory pointer to the proper CComVariant data type 
	// for IHTMLElement2::AddBehavior
	V_VT(&vFactory) = VT_UNKNOWN;
  V_UNKNOWN(&vFactory) = &m_xHTMLElementBehaviorFactory;

	// Add Grid behavior
	hr = pBody2->addBehavior(NULL, &vFactory, &m_gridCookie);

  // Set Paint site
  m_xHTMLPainter.SetPaintSite(m_xHTMLElementBehavior.m_spPaintSite);

	// Release resources
	hr = pBody->Release();
	hr = pBody2->Release();

	return;
}

void 
CHTMLEditControlSite::Grid(bool bState)
{ 
	m_xHTMLPainter.m_bGrid = bState;

	if (m_xHTMLElementBehavior.m_spPaintSite != (IHTMLPaintSite *) NULL)
  {
    m_xHTMLElementBehavior.m_spPaintSite->InvalidateRect(NULL);
  }
}

void
CHTMLEditControlSite::Snap(bool bState)
{
  m_xHTMLEditHost.m_bSnap = bState;
}

void       
CHTMLEditControlSite::GridSize(int width,int height)
{
  m_xHTMLPainter.m_iGridX = width;
  m_xHTMLPainter.m_iGridY = height;
}

void
CHTMLEditControlSite::SnapSize(int width,int height)
{
  m_xHTMLEditHost.m_iSnapX = width;
  m_xHTMLEditHost.m_iSnapY = height;
}

void
CHTMLEditControlSite::GetGridSize(int& width,int& height)
{
  width  = m_xHTMLPainter.m_iGridX;
  height = m_xHTMLPainter.m_iGridY;
}

void
CHTMLEditControlSite::GetSnapSize(int& width,int& height)
{
  width  = m_xHTMLEditHost.m_iSnapX;
  height = m_xHTMLEditHost.m_iSnapY;
}

int
CHTMLEditControlSite::Color()
{
  return m_xHTMLPainter.m_color;
}

void
CHTMLEditControlSite::Color(int color)
{
  m_xHTMLPainter.m_color = color;
}

void
CHTMLEditControlSite::SetDragLine(bool bState
                                 ,bool Horizontal
                                 ,int  dragX
                                 ,int  dragY)
{
  m_xHTMLPainter.m_dragLine   = bState;
  m_xHTMLPainter.m_horizontal = Horizontal;
  m_xHTMLPainter.m_dragX      = dragX;
  m_xHTMLPainter.m_dragY      = dragY;
  m_xHTMLElementBehavior.m_spPaintSite->InvalidatePainterInfo();
}

//	DocHostUIHandler methods.  These all delegate through the owner view
//	except for the standard IUnknown methods which delegate to the
//	CCmdTarget base class.
STDMETHODIMP_(ULONG) 
CHTMLEditControlSite::XDocHostUIHandler::AddRef()
{
	METHOD_PROLOGUE_EX_(CHTMLEditControlSite, DocHostUIHandler);
	
	return pThis->ExternalAddRef();
}

STDMETHODIMP_(ULONG) 
CHTMLEditControlSite::XDocHostUIHandler::Release()
{
	METHOD_PROLOGUE_EX_(CHTMLEditControlSite, DocHostUIHandler)

	return pThis->ExternalRelease();
}

STDMETHODIMP 
CHTMLEditControlSite::XDocHostUIHandler::QueryInterface(REFIID iid, LPVOID far* ppvObj)     
{
	METHOD_PROLOGUE_EX_(CHTMLEditControlSite, DocHostUIHandler)

	return pThis->ExternalQueryInterface(&iid, ppvObj);
}

STDMETHODIMP 
CHTMLEditControlSite::XDocHostUIHandler::GetExternal(LPDISPATCH *lppDispatch)
{
	METHOD_PROLOGUE_EX_(CHTMLEditControlSite, DocHostUIHandler)

	CHtmlView* pView = pThis->GetView();
	return pView->OnGetExternal(lppDispatch);
}

STDMETHODIMP 
CHTMLEditControlSite::XDocHostUIHandler::ShowContextMenu(DWORD dwID, LPPOINT ppt, LPUNKNOWN pcmdtReserved, LPDISPATCH pdispReserved)
{
	METHOD_PROLOGUE_EX_(CHTMLEditControlSite, DocHostUIHandler)

	CHtmlView* pView = pThis->GetView();
	return pView->OnShowContextMenu(dwID, ppt, pcmdtReserved, pdispReserved);
}

STDMETHODIMP 
CHTMLEditControlSite::XDocHostUIHandler::GetHostInfo(DOCHOSTUIINFO *pInfo)
{
	METHOD_PROLOGUE_EX_(CHTMLEditControlSite, DocHostUIHandler)
	CHtmlView* pView = pThis->GetView();
	return pView->OnGetHostInfo(pInfo);
}

STDMETHODIMP 
CHTMLEditControlSite::XDocHostUIHandler::ShowUI(DWORD dwID, LPOLEINPLACEACTIVEOBJECT pActiveObject, LPOLECOMMANDTARGET pCommandTarget, LPOLEINPLACEFRAME pFrame, LPOLEINPLACEUIWINDOW pDoc)
{
	METHOD_PROLOGUE_EX_(CHTMLEditControlSite, DocHostUIHandler)

	CHtmlView* pView = pThis->GetView();
	return pView->OnShowUI(dwID, pActiveObject, pCommandTarget, pFrame, pDoc);
}

STDMETHODIMP 
CHTMLEditControlSite::XDocHostUIHandler::HideUI(void)
{
	METHOD_PROLOGUE_EX_(CHTMLEditControlSite, DocHostUIHandler)

	CHtmlView* pView = pThis->GetView();
	
	return pView->OnHideUI();
}

STDMETHODIMP 
CHTMLEditControlSite::XDocHostUIHandler::UpdateUI(void)
{
	METHOD_PROLOGUE_EX_(CHTMLEditControlSite, DocHostUIHandler)

	CHtmlView* pView = pThis->GetView();
	
	return pView->OnUpdateUI();
}

STDMETHODIMP 
CHTMLEditControlSite::XDocHostUIHandler::EnableModeless(BOOL fEnable)
{
	METHOD_PROLOGUE_EX_(CHTMLEditControlSite, DocHostUIHandler)
	
	CHtmlView* pView = pThis->GetView();
	
	return pView->OnEnableModeless(fEnable);
}

STDMETHODIMP 
CHTMLEditControlSite::XDocHostUIHandler::OnDocWindowActivate(BOOL fActivate)
{
	METHOD_PROLOGUE_EX_(CHTMLEditControlSite, DocHostUIHandler)
	
	CHtmlView* pView = pThis->GetView();
	
	return pView->OnDocWindowActivate(fActivate);
}

STDMETHODIMP 
CHTMLEditControlSite::XDocHostUIHandler::OnFrameWindowActivate(BOOL fActivate)
{
	METHOD_PROLOGUE_EX_(CHTMLEditControlSite, DocHostUIHandler)
	
	CHtmlView* pView = pThis->GetView();
	
	return pView->OnFrameWindowActivate(fActivate);
}

STDMETHODIMP 
CHTMLEditControlSite::XDocHostUIHandler::ResizeBorder(LPCRECT prcBorder, LPOLEINPLACEUIWINDOW pUIWindow, BOOL fFrameWindow)
{
	METHOD_PROLOGUE_EX_(CHTMLEditControlSite, DocHostUIHandler)

	CHtmlView* pView = pThis->GetView();

	return pView->OnResizeBorder(prcBorder, pUIWindow, fFrameWindow);
}

STDMETHODIMP 
CHTMLEditControlSite::XDocHostUIHandler::TranslateAccelerator(LPMSG lpMsg, const GUID* pguidCmdGroup, DWORD nCmdID)
{
	METHOD_PROLOGUE_EX_(CHTMLEditControlSite, DocHostUIHandler)

	CHtmlView* pView = pThis->GetView();

	return pView->OnTranslateAccelerator(lpMsg, pguidCmdGroup, nCmdID);
}

STDMETHODIMP 
CHTMLEditControlSite::XDocHostUIHandler::GetOptionKeyPath(LPOLESTR* pchKey, DWORD dwReserved)
{
	METHOD_PROLOGUE_EX_(CHTMLEditControlSite, DocHostUIHandler)

	CHtmlView* pView = pThis->GetView();

	return pView->OnGetOptionKeyPath(pchKey, dwReserved);
}

STDMETHODIMP 
CHTMLEditControlSite::XDocHostUIHandler::GetDropTarget(LPDROPTARGET pDropTarget, LPDROPTARGET* ppDropTarget)
{
	METHOD_PROLOGUE_EX_(CHTMLEditControlSite, DocHostUIHandler)
	
	CHtmlView* pView = pThis->GetView();
	
	return pView->OnGetDropTarget(pDropTarget, ppDropTarget);
}

STDMETHODIMP 
CHTMLEditControlSite::XDocHostUIHandler::TranslateUrl(DWORD dwTranslate, OLECHAR* pchURLIn, OLECHAR** ppchURLOut)
{
	METHOD_PROLOGUE_EX_(CHTMLEditControlSite, DocHostUIHandler)
	
	CHtmlView* pView = pThis->GetView();
	
	return pView->OnTranslateUrl(dwTranslate, pchURLIn, ppchURLOut);
}

STDMETHODIMP 
CHTMLEditControlSite::XDocHostUIHandler::FilterDataObject(LPDATAOBJECT pDataObject, LPDATAOBJECT* ppDataObject)
{
	METHOD_PROLOGUE_EX_(CHTMLEditControlSite, DocHostUIHandler)
	
	CHtmlView* pView = pThis->GetView();
	
	return pView->OnFilterDataObject(pDataObject, ppDataObject);
}

//	The service provider implementation. This is called by MSHTML to get
//	a pointer to an instance of IHTMLEditHost
STDMETHODIMP_(ULONG) 
CHTMLEditControlSite::XServiceProvider::AddRef()
{
	METHOD_PROLOGUE_EX_(CHTMLEditControlSite, ServiceProvider);

	return pThis->ExternalAddRef();
}

STDMETHODIMP_(ULONG) 
CHTMLEditControlSite::XServiceProvider::Release()
{
	METHOD_PROLOGUE_EX_(CHTMLEditControlSite, ServiceProvider)

	return pThis->ExternalRelease();
}

STDMETHODIMP 
CHTMLEditControlSite::XServiceProvider::QueryInterface(REFIID iid, LPVOID far* ppvObj)     
{
	METHOD_PROLOGUE_EX_(CHTMLEditControlSite, ServiceProvider)

	return pThis->ExternalQueryInterface(&iid, ppvObj);
}

STDMETHODIMP 
CHTMLEditControlSite::XServiceProvider::QueryService(REFGUID guidService, REFIID riid, void **ppObj)
{
	METHOD_PROLOGUE_EX_(CHTMLEditControlSite, ServiceProvider)

	HRESULT hr = E_NOINTERFACE;

	*ppObj = NULL;

  if (guidService == SID_SHTMLEditHost && riid == IID_IHTMLEditHost)
  {
		*ppObj = (void **) &pThis->m_xHTMLEditHost;
		hr = S_OK;
	}

	return hr;
}

//	The edit host implementation, which is where the snap-to-grid is done
CHTMLEditControlSite::XHTMLEditHost::XHTMLEditHost()
{
  m_bSnap  = FALSE;
	m_iSnapX = 8;
	m_iSnapY = 8;
}

STDMETHODIMP_(ULONG) 
CHTMLEditControlSite::XHTMLEditHost::AddRef()
{
	return 1;
}

STDMETHODIMP_(ULONG) 
CHTMLEditControlSite::XHTMLEditHost::Release()
{
	return 1;
}

STDMETHODIMP 
CHTMLEditControlSite::XHTMLEditHost::QueryInterface(REFIID iid, LPVOID far* ppvObj)     
{
	METHOD_PROLOGUE(CHTMLEditControlSite, HTMLEditHost);

	return pThis->ExternalQueryInterface(&iid, ppvObj);
}

STDMETHODIMP 
CHTMLEditControlSite::XHTMLEditHost::SnapRect(IHTMLElement * /*pIElement*/, RECT * prcNew, ELEMENT_CORNER eHandle)
{
	if (GetAsyncKeyState(VK_CONTROL) & 0x10000000)
  {
    //	If the control key is down return (no snap).
		return S_OK;
  }
  if(!m_bSnap)
  {
    // Turned of snap in the interface
    return S_OK;
  }
	LONG lWidth  = prcNew->right  - prcNew->left;
	LONG lHeight = prcNew->bottom - prcNew->top;

	switch (eHandle)
	{
	case ELEMENT_CORNER_NONE:
		prcNew->top    = ((prcNew->top  + (m_iSnapY / 2)) / m_iSnapY) * m_iSnapY - 2;
		prcNew->left   = ((prcNew->left + (m_iSnapX / 2)) / m_iSnapX) * m_iSnapX - 2;
		prcNew->bottom = prcNew->top  + lHeight;
		prcNew->right  = prcNew->left + lWidth;
		break;

	case ELEMENT_CORNER_TOP:
		prcNew->top = ((prcNew->top + (m_iSnapY / 2)) / m_iSnapY) * m_iSnapY;
		break;

	case ELEMENT_CORNER_LEFT:
		prcNew->left = ((prcNew->left + (m_iSnapX / 2)) / m_iSnapX) * m_iSnapX;
		break;

	case ELEMENT_CORNER_BOTTOM:
		prcNew->bottom = ((prcNew->bottom + (m_iSnapY / 2)) / m_iSnapY) * m_iSnapY;
		break;

	case ELEMENT_CORNER_RIGHT:
		prcNew->right = ((prcNew->right + (m_iSnapX / 2)) / m_iSnapX) * m_iSnapX;
		break;

	case ELEMENT_CORNER_TOPLEFT:
		prcNew->top  = ((prcNew->top  + (m_iSnapX / 2)) / m_iSnapX) * m_iSnapX;
		prcNew->left = ((prcNew->left + (m_iSnapY / 2)) / m_iSnapY) * m_iSnapY;
		break;

	case ELEMENT_CORNER_TOPRIGHT:
		prcNew->top   = ((prcNew->top   + (m_iSnapX / 2)) / m_iSnapX) * m_iSnapX;
		prcNew->right = ((prcNew->right + (m_iSnapY / 2)) / m_iSnapY) * m_iSnapY;
		break;

	case ELEMENT_CORNER_BOTTOMLEFT:
		prcNew->bottom = ((prcNew->bottom + (m_iSnapY / 2)) / m_iSnapY) * m_iSnapY;
		prcNew->left   = ((prcNew->left   + (m_iSnapX / 2)) / m_iSnapX) * m_iSnapX;
		break;

	case ELEMENT_CORNER_BOTTOMRIGHT:
		prcNew->bottom = ((prcNew->bottom + (m_iSnapY / 2)) / m_iSnapY) * m_iSnapY;
		prcNew->right  = ((prcNew->right  + (m_iSnapX / 2)) / m_iSnapX) * m_iSnapX;
		break;
	}

	return S_OK;
}

//	The behavior implementation. This is queried by MSHTML to get an instance 
//	of an IHTMLPainter. Our painter draws the grid.
CHTMLEditControlSite::XHTMLElementBehavior::XHTMLElementBehavior()
{
  m_spPaintSite = NULL;
}

STDMETHODIMP_(ULONG) 
CHTMLEditControlSite::XHTMLElementBehavior::AddRef()
{
	return 1;
}

STDMETHODIMP_(ULONG) 
CHTMLEditControlSite::XHTMLElementBehavior::Release()
{
	return 1;
}

STDMETHODIMP 
CHTMLEditControlSite::XHTMLElementBehavior::QueryInterface(REFIID iid, LPVOID far* ppvObj)     
{
	METHOD_PROLOGUE(CHTMLEditControlSite, HTMLElementBehavior);

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
	*ppvObj = NULL;
	return E_NOTIMPL;
}

STDMETHODIMP 
CHTMLEditControlSite::XHTMLElementBehavior::Init(IElementBehaviorSite *pBehaviorSite)
{
	HRESULT	hr = pBehaviorSite->QueryInterface(IID_IHTMLPaintSite, (void **) &m_spPaintSite);

	if (m_spPaintSite != (IHTMLPaintSite *) NULL)
  {
    m_spPaintSite->InvalidateRect(NULL);
  }
	return hr;
}

STDMETHODIMP 
CHTMLEditControlSite::XHTMLElementBehavior::Notify(LONG /*lEvent*/, VARIANT*pVar)
{
	if (pVar == NULL)
  {
    return E_POINTER;
  }
	return E_NOTIMPL;
}

STDMETHODIMP 
CHTMLEditControlSite::XHTMLElementBehavior::Detach()
{
	m_spPaintSite->InvalidateRect(NULL);
	return S_OK;
}

//	The behavior factory

STDMETHODIMP_(ULONG) 
CHTMLEditControlSite::XHTMLElementBehaviorFactory::AddRef()
{
	return 1;
}

STDMETHODIMP_(ULONG) 
CHTMLEditControlSite::XHTMLElementBehaviorFactory::Release()
{
	return 1;
}

STDMETHODIMP 
CHTMLEditControlSite::XHTMLElementBehaviorFactory::QueryInterface(REFIID iid, LPVOID far* ppvObj)     
{
	METHOD_PROLOGUE(CHTMLEditControlSite, HTMLElementBehaviorFactory);

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

STDMETHODIMP CHTMLEditControlSite::XHTMLElementBehaviorFactory::FindBehavior(BSTR /*bstrBehavior*/, BSTR /*bstrBehaviorUrl*/, IElementBehaviorSite * /*pSite*/, IElementBehavior** ppBehavior)
{
	METHOD_PROLOGUE(CHTMLEditControlSite, HTMLElementBehaviorFactory)

	*ppBehavior = &pThis->m_xHTMLElementBehavior;
	return S_OK;
}

//	The painter part of the class
CHTMLEditControlSite::XHTMLPainter::XHTMLPainter()
{
	m_bGrid  = FALSE;
	m_iGridX = 32;
	m_iGridY = 32;
  m_color  = RGB(0xff, 0x99, 0x99); // Dark red
  m_dragLine = false;
  m_doDrag   = false;
  m_dragX    = 0;
  m_dragY    = 0;
  m_spPaintSite = NULL;
}

STDMETHODIMP
CHTMLEditControlSite::XHTMLPainter::SetPaintSite(IHTMLPaintSite* site)
{
  m_spPaintSite = site;
  return S_OK;
}

STDMETHODIMP_(ULONG) 
CHTMLEditControlSite::XHTMLPainter::AddRef()
{
	return 1;
}

STDMETHODIMP_(ULONG) 
CHTMLEditControlSite::XHTMLPainter::Release()
{
	return 1;
}

STDMETHODIMP 
CHTMLEditControlSite::XHTMLPainter::QueryInterface(REFIID iid, LPVOID far* ppvObj)     
{
	METHOD_PROLOGUE(CHTMLEditControlSite, HTMLPainter);

	return pThis->ExternalQueryInterface(&iid, ppvObj);
}

// DRAW THE GRID
STDMETHODIMP 
CHTMLEditControlSite::XHTMLPainter::Draw(RECT rcBounds, RECT /*rcUpdate*/, LONG /*lDrawFlags*/, HDC hdc, LPVOID /*pvDrawObject*/)
{
  static bool forDrag = false;

	if (m_bGrid != FALSE)
	{
    m_doDrag = false;
    if(forDrag)
    {
      m_spPaintSite->InvalidatePainterInfo();
      forDrag = false;
    }
    HPEN redPen	= (HPEN) CreatePen(PS_DOT, 0, m_color);
		HPEN oldPen = (HPEN) SelectObject(hdc, redPen);

		long lFirstLine = rcBounds.left + m_iGridX;

    int i = 0;
		for (i = lFirstLine; i <= rcBounds.right; i += m_iGridX)
		{
			MoveToEx(hdc, i, rcBounds.top, NULL);
			LineTo  (hdc, i, rcBounds.bottom);
		}

		lFirstLine = rcBounds.top + m_iGridY;

		for (i = lFirstLine ; i <= rcBounds.bottom; i += m_iGridY)
		{
			MoveToEx(hdc, rcBounds.left, i,  NULL);
			LineTo  (hdc, rcBounds.right, i);
		}
		SelectObject(hdc, oldPen);
		DeleteObject(redPen);
	}
  if(m_dragLine)
  {
    m_doDrag = true;
    if(!forDrag)
    {
      m_spPaintSite->InvalidatePainterInfo();
      forDrag = true;
    }
    HPEN redPen	= (HPEN) CreatePen(PS_DOT, 1, RGB(0,0,0xff));
		HPEN oldPen = (HPEN) SelectObject(hdc, redPen);

    if(m_horizontal)
    {
      TRACE("Bounds %d,%d,%d,%d\n",rcBounds.left,rcBounds.right,rcBounds.top,rcBounds.bottom);
      TRACE("DRAG Horizontal: %d\n",m_dragY);
      MoveToEx(hdc,rcBounds.left, m_dragY,NULL);
      LineTo  (hdc,rcBounds.right,m_dragY);
      m_dragLine = false;
    }
    else
    {
      TRACE("Bounds %d,%d,%d,%d\n",rcBounds.left,rcBounds.right,rcBounds.top,rcBounds.bottom);
      TRACE("DRAG Vertical: %d\n",m_dragX);
      MoveToEx(hdc,m_dragX, rcBounds.top,   NULL);
      LineTo  (hdc,m_dragX, rcBounds.bottom);
    }
		SelectObject(hdc, oldPen);
		DeleteObject(redPen);
  }
	return S_OK;
}

STDMETHODIMP 
CHTMLEditControlSite::XHTMLPainter::OnResize(SIZE /*size*/)
{
	return S_OK;
}

STDMETHODIMP 
CHTMLEditControlSite::XHTMLPainter::GetPainterInfo(HTML_PAINTER_INFO *pInfo)
{
	if (pInfo == NULL)
  {
		return E_POINTER;
  }
  pInfo->lFlags  = m_doDrag ? HTMLPAINTER_OPAQUE             : HTMLPAINTER_TRANSPARENT;
  pInfo->lZOrder = m_doDrag ? HTMLPAINT_ZORDER_ABOVE_CONTENT : HTMLPAINT_ZORDER_BELOW_CONTENT;

	memset(&pInfo->iidDrawObject, 0, sizeof(IID));

	pInfo->rcExpand.left   = 0;
	pInfo->rcExpand.right  = 0;
	pInfo->rcExpand.top    = 0;
	pInfo->rcExpand.bottom = 0;

	return S_OK;
}

STDMETHODIMP 
CHTMLEditControlSite::XHTMLPainter::HitTestPoint(POINT /*pt*/, BOOL *pbHit, LONG *plPartID)
{
	if (pbHit == NULL)
  {
    return E_POINTER;
  }
	if (plPartID == NULL)
  {
    return E_POINTER;
  }
	return E_NOTIMPL;
}

