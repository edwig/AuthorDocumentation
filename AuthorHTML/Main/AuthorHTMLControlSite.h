//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    AuthorHTMLControlSite.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Handles the MSHTML OCX COM-Interfaces sink
//
#pragma once
#include <afxocc.h>

#undef  DEFINE_GUID
#define DEFINE_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) EXTERN_C const GUID DECLSPEC_SELECTANY name = { l, w1, w2, { b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } }

class CHTMLEditControlSite : public COleControlSite
{
public:
	CHTMLEditControlSite(COleControlContainer* pParentWnd);

	CHtmlView*  GetView() const;
	void			  InstallGrid    (IHTMLDocument2 *pDoc);
  void        InstallDesigner(IHTMLDocument2* pDoc);
	bool        Grid();
	void		    Grid(bool bState);
  bool        Snap();
  void        Snap(bool bState);
  void        GridSize(int width,int height);
  void        SnapSize(int width,int height);
  void        GetGridSize(int& width,int& height);
  void        GetSnapSize(int& width,int& height);
  void        SetDragLine(bool bState
                         ,bool Horizontal = true
                         ,int dragX = 0
                         ,int dragY = 0);
  int         Color();
  void        Color(int color);

protected:
	LONG			  m_gridCookie;

  // Implementation
	DECLARE_INTERFACE_MAP()

	//	This is the implementation of the IDocHostUIHandler interface
	//	MSHMTL gets this interface from us so we have to reference count it.
	BEGIN_INTERFACE_PART(DocHostUIHandler, IDocHostUIHandler)
		STDMETHOD(ShowContextMenu)(DWORD, LPPOINT, LPUNKNOWN, LPDISPATCH);
		STDMETHOD(GetHostInfo)(DOCHOSTUIINFO*);
		STDMETHOD(ShowUI)(DWORD, LPOLEINPLACEACTIVEOBJECT, LPOLECOMMANDTARGET, LPOLEINPLACEFRAME, LPOLEINPLACEUIWINDOW);
		STDMETHOD(HideUI)(void);
		STDMETHOD(UpdateUI)(void);
		STDMETHOD(EnableModeless)(BOOL);
		STDMETHOD(OnDocWindowActivate)(BOOL);
		STDMETHOD(OnFrameWindowActivate)(BOOL);
		STDMETHOD(ResizeBorder)(LPCRECT, LPOLEINPLACEUIWINDOW, BOOL);
		STDMETHOD(TranslateAccelerator)(LPMSG, const GUID*, DWORD);
		STDMETHOD(GetOptionKeyPath)(OLECHAR **, DWORD);
		STDMETHOD(GetDropTarget)(LPDROPTARGET, LPDROPTARGET*);
		STDMETHOD(GetExternal)(LPDISPATCH*);
		STDMETHOD(TranslateUrl)(DWORD, OLECHAR*, OLECHAR **);
		STDMETHOD(FilterDataObject)(LPDATAOBJECT , LPDATAOBJECT*);
	END_INTERFACE_PART(DocHostUIHandler)

	//	This is the implementation of the IServiceProvider interface
	//	MSHMTL gets this interface from us so we have to reference count it.
	BEGIN_INTERFACE_PART(ServiceProvider, IServiceProvider)
		STDMETHOD(QueryService)(REFGUID, REFIID, void **);
	END_INTERFACE_PART(ServiceProvider)

	//	This is the implementation of the IHTMLEditHost interface
	//	MSHMTL gets this interface from us so we have to reference count it.
	BEGIN_INTERFACE_PART(HTMLEditHost, IHTMLEditHost)
		STDMETHOD(SnapRect)(IHTMLElement *pIElement, RECT *prcNew, ELEMENT_CORNER eHandle);
		XHTMLEditHost();
    bool m_bSnap;
		int	 m_iSnapX;
		int	 m_iSnapY;
	END_INTERFACE_PART(HTMLEditHost)

	//	This is the implementation of the IHTMLPainter interface
	//	We pass this interface to MSHMTL so we don't reference count it.
	//	(It ain't going away unless the entire class goes away)
	BEGIN_INTERFACE_PART(HTMLPainter, IHTMLPainter)
		STDMETHOD(Draw)(RECT rcBounds, RECT rcUpdate, LONG lDrawFlags, HDC hdc, LPVOID pvDrawObject);
		STDMETHOD(OnResize)(SIZE size);
		STDMETHOD(GetPainterInfo)(HTML_PAINTER_INFO * pInfo);
		STDMETHOD(HitTestPoint)(POINT pt, BOOL* pbHit, LONG * plPartID);
    STDMETHOD(SetPaintSite)(IHTMLPaintSite* site);
  	XHTMLPainter();
		bool m_bGrid;
		int	 m_iGridX;
		int	 m_iGridY;
    int  m_color;

    bool m_dragLine;
    bool m_doDrag;
    bool m_horizontal;
    int  m_dragX;
    int  m_dragY;
    IHTMLPaintSite* m_spPaintSite;
	END_INTERFACE_PART(HTMLPainter)

	//	This is the implementation of the HTMLElementBehavior interface
	//	We pass this interface to MSHMTL so we don't reference count it.
	//	(It ain't going away unless the entire class goes away)
	BEGIN_INTERFACE_PART(HTMLElementBehavior, IElementBehavior)
		STDMETHOD(Init)(IElementBehaviorSite *pBehaviorSite);
		STDMETHOD(Notify)(LONG lEvent, VARIANT* pVar);
		STDMETHOD(Detach)();
  	XHTMLElementBehavior();
		IHTMLPaintSite* m_spPaintSite;
	END_INTERFACE_PART(HTMLElementBehavior)

	//	This is the implementation of the HTMLElementBehaviorFactory interface
	//	We pass this interface to MSHMTL so we don't reference count it.
	//	(It ain't going away unless the entire class goes away)
	BEGIN_INTERFACE_PART(HTMLElementBehaviorFactory, IElementBehaviorFactory)
		STDMETHOD(FindBehavior)(BSTR bstrBehavior, BSTR bstrBehaviorUrl, IElementBehaviorSite* pSite, IElementBehavior** ppBehavior);
	END_INTERFACE_PART(HTMLElementBehaviorFactory)
};

inline bool
CHTMLEditControlSite::Grid()				
{ 
  return m_xHTMLPainter.m_bGrid; 
}

inline bool
CHTMLEditControlSite::Snap()
{
  return m_xHTMLEditHost.m_bSnap;
}