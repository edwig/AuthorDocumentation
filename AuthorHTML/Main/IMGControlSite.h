//////////////////////////////////////////////////////////////////////
// 
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    IMGControlSite.h
//
// Description: Interface of a control-site for an image
// taking control of drawing and shaping the cursor over an image
// that has a mapping of areas (MAP/AREA) for hyperlinking
//
#pragma once
#include <afxocc.h>
#include "HTMLDocEditDesigner.h"
#include "ImgArea.h"
#include <vector>
#include <map>

#define IMAGE_POSITIONS 99999

typedef std::map<CString,ImgArea> ImgAreaMap;

class IMGControlSite : public COleControlSite
{
public:
	 IMGControlSite(COleControlContainer* pParentWnd);
  ~IMGControlSite();

	void InstallIMGBehavior(HTMLDocEditDesigner* designer
                         ,IHTMLDocument2*      doc
                         ,IHTMLImgElement*     pImage);
  bool            DetachBehavior();
  void            AttachImgArea(CString id,ImgArea& area,bool setNew = false);
  IHTMLElement*   GetImage();
  IHTMLPaintSite* GetPaintSite();
  RECT            GetPainterElementCorners();
  void            SetPartID(long partid);
  void            SetPolyCorner(long X,long Y,bool newContinue);
  bool            TouchDown(POINT mouse);
  void            RewriteArea();
  bool            AddToNewArea(POINT mouse);
  AreaShape       GetCurrentShape();
  CString         IDNewArea();
  ImgArea*        FindAreaByID(CString id);
  bool            RemoveArea(CString id);
  CComPtr<IHTMLWindow2>  m_spWin;

protected:
  // Helper functions;
	LONG          m_imgCookie;
  IHTMLElement* m_Image;

  // Implementation
	DECLARE_INTERFACE_MAP()

	//	This is the implementation of the IHTMLPainter interface
	//	We pass this interface to MSHMTL so we don't reference count it.
	//	(It ain't going away unless the entire class goes away)
	BEGIN_INTERFACE_PART(HTMLPainter, IHTMLPainter)
		STDMETHOD(Draw)(RECT rcBounds, RECT rcUpdate, LONG lDrawFlags, HDC hdc, LPVOID pvDrawObject);
		STDMETHOD(OnResize)(SIZE size);
		STDMETHOD(GetPainterInfo)(HTML_PAINTER_INFO * pInfo);
		STDMETHOD(HitTestPoint)(POINT pt, BOOL* pbHit, LONG * plPartID);
    STDMETHOD(SetPaintSite)(IHTMLPaintSite* site);
    STDMETHOD(SetDesigner)(HTMLDocEditDesigner* designer);
    STDMETHOD(InitPainter)(CComPtr<IHTMLDocument2> doc,CComPtr<IHTMLElement> pElement);
  	XHTMLPainter();
    IHTMLPaintSite* m_spPaintSite;
    RECT						m_rtElemCorners;
    POINT						m_ptPolyCorners[4]; // GOES AWAY
    long						m_lPartID;          // Corner in area with touchdown
    ImgArea*        m_lPartArea;        // Area with touchdown
    ImgAreaMap      m_areas;            // All the areas
    HTMLDocEditDesigner* m_designer;
    CComPtr<IHTMLElement> m_spElem;
    CComPtr<IHTMLElement> m_spParent;
  END_INTERFACE_PART(HTMLPainter)

  BEGIN_INTERFACE_PART(HTMLPainterEventInfo,IHTMLPainterEventInfo)
    STDMETHOD(GetEventInfoFlags)(long *plEventInfoFlags);
    STDMETHOD(GetEventTarget)(IHTMLElement **ppElement);
    STDMETHOD(SetCursor)(LONG lPartID);
    STDMETHOD(StringFromPartID)(LONG lPartID,BSTR* pbstrPart);
    XHTMLPainterEventInfo();
 END_INTERFACE_PART(HTMLPainterEventInfo)


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

inline IHTMLElement* 
IMGControlSite::GetImage()
{
  return m_Image;
}

inline IHTMLPaintSite* 
IMGControlSite::GetPaintSite()
{
  return m_xHTMLElementBehavior.m_spPaintSite;
}

inline RECT     
IMGControlSite::GetPainterElementCorners()
{
  return m_xHTMLPainter.m_rtElemCorners;
}

