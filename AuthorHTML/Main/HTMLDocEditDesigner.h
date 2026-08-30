//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    HTMLDocEditDesigner.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Implementation of an edit designer
// for the document. Catches the events of the m_spBrowser
// and redirects it to our behavior sites
// (CHTMLControlSite & IMGControlSite)
//
#pragma once
#include "AuthorHTMLControlSite.h"
#include "ImgArea.h"
#include <vector>

class IMGControlSite;
class CHTMLEdView;

typedef std::vector<IMGControlSite*> ImgSites;

class HTMLDocEditDesignerDispatch : public IDispatch
{
public:
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void __RPC_FAR *__RPC_FAR *ppvObject);
	virtual ULONG	  STDMETHODCALLTYPE AddRef(void);
	virtual ULONG	  STDMETHODCALLTYPE Release(void);

	virtual HRESULT STDMETHODCALLTYPE GetTypeInfoCount(UINT *pctinfo);
	virtual HRESULT STDMETHODCALLTYPE GetTypeInfo(UINT iTInfo, LCID lcid, ITypeInfo **ppTInfo);
	virtual HRESULT STDMETHODCALLTYPE GetIDsOfNames(REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgDispId);
	virtual HRESULT STDMETHODCALLTYPE Invoke(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *pDispParams, VARIANT*pVarResult, EXCEPINFO *pExcepInfo, UINT *puArgErr);

private:
	UINT m_uRefCount;
};

class HTMLDocEditDesigner : public IHTMLEditDesigner
{
public:
	 HTMLDocEditDesigner();
  ~HTMLDocEditDesigner();
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void __RPC_FAR *__RPC_FAR *ppvObject);
	virtual ULONG	  STDMETHODCALLTYPE AddRef(void);
	virtual ULONG	  STDMETHODCALLTYPE Release(void);

	virtual HRESULT STDMETHODCALLTYPE PreHandleEvent       (DISPID inEvtDispId, IHTMLEventObj *pIEventObj);
	virtual HRESULT STDMETHODCALLTYPE PostHandleEvent      (DISPID inEvtDispId, IHTMLEventObj *pIEventObj);
	virtual HRESULT STDMETHODCALLTYPE TranslateAccelerator (DISPID inEvtDispId, IHTMLEventObj *pIEventObj);
	virtual HRESULT STDMETHODCALLTYPE PostEditorEventNotify(DISPID inEvtDispId, IHTMLEventObj *pIEventObj);

  void            ForbidSelection(IHTMLElement* elem);

	BOOL			      Attach(CHTMLEdView* view
                        ,IHTMLDocument2 *pDoc
                        ,CHTMLEditControlSite* site);
  bool            AttachImg(IMGControlSite* ics);
	void			      Detach();
  bool            DetachImg(IHTMLElement* pElement);
  void            DetachImages();
  void            SetNewShape(AreaShape shape);
  IMGControlSite* FindIMGSite(CComPtr<IHTMLElement> pElement);

  bool            m_mouseDown;

private:
  void MouseDownOnImage(IHTMLEventObj*  pIEventObj
                       ,IMGControlSite* behavior);
  IMGControlSite* NewImgArea(IHTMLEventObj*  pIEvent
                            ,CComPtr<IHTMLElement> pElement);
  bool AddToImgArea(IHTMLEventObj*  pIEvent
                   ,CComPtr<IHTMLElement> pElement);
	UINT			                  m_uRefCount;
  CHTMLEdView*                m_view;
	HTMLDocEditDesignerDispatch m_dp;
	IHTMLEditServices*          m_pServices;
  HCURSOR                     m_cursor;
  CHTMLEditControlSite*       m_controlSite;
  ImgSites                    m_imgSites;
  // Newly defined area of this shape
  AreaShape                   m_newShape;
  bool                        m_newContinue;
};

inline void            
HTMLDocEditDesigner::SetNewShape(AreaShape shape)
{
  m_newShape    = shape;
  m_newContinue = false;
}
