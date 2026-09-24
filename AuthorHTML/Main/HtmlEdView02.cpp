//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    HTMLEdView.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Iimplementation of the CHTMLEdView class
// VIEW with the MSHMTL control for a HTML page
//
// This is the main view for a HTML editor
// Contains and calls all the other HTML stuff.
//
#include "stdafx.h"
#include "AuthorHTML.h"

#include "HTMLEdDoc.h"
#include "htmledview.h"
#include "mainfrm.h"
#include "IMGControlSite.h"
#include "HTMLArea.h"
#include "AreaDlg.h"
#include <afxpriv.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

struct _glyph_table
{
  TCHAR* tag;
  TCHAR* resource;
  int   tag_type;           // 0=opening, 1=closing, 2=both                        1-2
  int   alignment;          // 0=left, 1=center, 2=right, 3=undefined              3
  int   positioning;        // 0=static, 1=absolute, 2=relative, 3=all             3
  int   direction;          // 0=ltr, 1=rtl, 2=toptobottom, 3=bottomtotop, 4=all   4
  int   width;              // image width in pixels
  int   height;             // image height in pixels
};

typedef struct _glyph_table GLYPHTABLE;

GLYPHTABLE green_glyphs[] = 
{
   { _T("p"),     _T("pbgn.gif"),   0, 3, 3, 4, 0,  0  }
  ,{ _T("p"),     _T("pend.gif"),   1, 3, 3, 4, 20, 15 }
  ,{ _T("table"), _T("table.gif"),  0, 3, 3, 4, 20, 15 }
  ,{ _T("br"),    _T("br.gif"),     0, 3, 3, 4, 20, 15 } 
  ,{ _T("script"),_T("script.gif"), 0, 3, 3, 4, 20, 15 }
  ,{ _T("!"),     _T("comment.gif"),0, 3, 3, 4, 20, 15 }
  ,{ _T("style"), _T("style.gif"),  0, 3, 3, 4, 20, 15 }
  ,{ _T("a"),     _T("anchor.gif"), 0, 3, 3, 4, 20, 15 }
  ,{ _T("a"),     _T("anchor.gif"), 1, 3, 3, 4, 0,  0  }
  ,{ _T("h1"),    _T("h1begin.gif"),0, 3, 3, 4, 30, 14 }
  ,{ _T("h1"),    _T("h1end.gif"),  1, 3, 3, 4, 30, 14 }
  ,{ _T("h2"),    _T("h2begin.gif"),0, 3, 3, 4, 30, 14 }
  ,{ _T("h2"),    _T("h2end.gif"),  1, 3, 3, 4, 30, 14 }
  ,{ _T("h3"),    _T("h3begin.gif"),0, 3, 3, 4, 30, 14 }
  ,{ _T("h3"),    _T("h3end.gif"),  1, 3, 3, 4, 30, 14 }
  ,{ _T("span"),  _T("spanbgn.gif"),0, 3, 3, 4, 43, 14 } 
  ,{ _T("span"),  _T("spanend.gif"),1, 3, 3, 4, 43, 14 }
  ,{ NULL,    NULL,         0, 0, 0, 0, 0,  0  }
};

GLYPHTABLE yellow_glyphs[] = 
{
  { _T("abbr"),      _T("abbr.gif"),   2, 3, 3, 4, 34, 16 }
 ,{ _T("acronym"),   _T("acr.gif"),    2, 3, 3, 4, 56, 16 }
 ,{ _T("address"),   _T("adr.gif"),    2, 3, 3, 4, 16, 44 }
 ,{ _T("a"),         _T("anchor.gif"), 2, 3, 3, 4, 19, 16 }
 ,{ _T("applet"),    _T("applet.gif"), 2, 3, 3, 4, 46, 16 }
 ,{ _T("area"),      _T("area.gif"),   0, 3, 3, 4, 38, 16 } 
 ,{ _T("b"),         _T("b.gif"),      2, 3, 3, 4, 19, 16 }
 ,{ _T("strong"),    _T("b.gif"),      2, 3, 3, 4, 19, 16 }
 ,{ _T("base"),      _T("base.gif"),   2, 3, 3, 4, 16, 36 }
 ,{ _T("bdo"),       _T("bdo.gif"),    2, 3, 3, 4, 30, 16 }
 ,{ _T("big"),       _T("big.gif"),    2, 3, 3, 4, 30, 16 }
 ,{ _T("blockquote"),_T("blq.gif"),    2, 3, 3, 4, 16, 44 }
 ,{ _T("body"),      _T("body.gif"),   2, 3, 3, 4, 16, 36 }
 ,{ _T("br"),        _T("br.gif"),     0, 3, 3, 4, 22, 16 }
 ,{ _T("basefont"),  _T("bsf.gif"),    2, 3, 3, 4, 56, 16 }
 ,{ _T("button"),    _T("btn.gif"),    2, 3, 3, 4, 50, 16 }
 ,{ _T("cite"),      _T("cit.gif"),    2, 3, 3, 4, 38, 16 }
 ,{ _T("colgroup"),  _T("clg.gif"),    2, 3, 3, 4, 50, 16 }
 ,{ _T("code"),      _T("cod.gif"),    2, 3, 3, 4, 38, 16 }   // Code
 ,{ _T("col"),       _T("col.gif"),    2, 3, 3, 4, 30, 16 }   // Columnar data
 ,{ _T("comment"),   _T("com.gif"),    0, 3, 3, 4, 19, 16 }   // Comment
 ,{ _T("caption"),   _T("cpt.gif"),    0, 3, 3, 4, 54, 16 }
 ,{ _T("caption"),   _T("cpt.gif"),    1, 3, 3, 4, 54, 16 }
 ,{ _T("center"),    _T("ctr.gif"),    2, 3, 3, 4, 16, 44 }   // Centered text
 ,{ _T("dd"),        _T("dd.gif"),     0, 3, 3, 4, 22, 16 }
 ,{ _T("dd"),        _T("dd.gif"),     2, 3, 3, 4, 0,  0  }   // Definition data
 ,{ _T("del"),       _T("del.gif"),    2, 3, 3, 4, 30, 16 }   // Deleted text
 ,{ _T("dir"),       _T("dir.gif"),    2, 3, 3, 4, 30, 16 }   // Directory of files
 ,{ _T("div"),       _T("div.gif"),    2, 3, 3, 4, 16, 24 }   // Division of running text
 ,{ _T("div"),       _T("div.gif"),    0, 3, 1, 4, 16, 24 }   // Division (abs/relative)
 ,{ _T("dl"),        _T("dl.gif"),     2, 3, 3, 4, 16, 20 }   // Definition list
 ,{ _T("dt"),        _T("dt.gif"),     2, 3, 3, 4, 22, 16 }   // Definition term
 ,{ _T("em"),        _T("em.gif"),     2, 3, 3, 4, 22, 16 }   // Emphasis = Italic
 ,{ _T("fieldset"),  _T("fld.gif"),    2, 3, 3, 4, 16, 44 }
 ,{ _T("font"),      _T("font.gif"),   2, 3, 3, 4, 38, 16 }
 ,{ _T("form"),      _T("form.gif"),   2, 3, 3, 4, 16, 36 }
 ,{ _T("frame"),     _T("frm.gif"),    2, 3, 3, 4, 16, 40 }
 ,{ _T("frameset"),  _T("fst.gif"),    2, 3, 3, 4, 16, 44 }
 ,{ _T("h1"),        _T("h1.gif"),     2, 3, 3, 4, 16, 20 }
 ,{ _T("h2"),        _T("h2.gif"),     2, 3, 3, 4, 16, 20 }
 ,{ _T("h3"),        _T("h3.gif"),     2, 3, 3, 4, 16, 20 }
 ,{ _T("h4"),        _T("h4.gif"),     2, 3, 3, 4, 16, 20 }
 ,{ _T("h5"),        _T("h5.gif"),     2, 3, 3, 4, 16, 20 }
 ,{ _T("h6"),        _T("h6.gif"),     2, 3, 3, 4, 16, 20 }
 ,{ _T("head"),      _T("head.gif"),   2, 3, 3, 4, 16, 36 } 
 ,{ _T("hr"),        _T("hr.gif"),     2, 3, 3, 4, 16, 20 }
 ,{ _T("html"),      _T("html.gif"),   2, 3, 3, 4, 16, 38 }
 ,{ _T("i"),         _T("i.gif"),      2, 3, 3, 4, 19, 16 }   // Alsoo italic = see EM
 ,{ _T("iframe"),    _T("ifr.gif"),    2, 3, 3, 4, 46, 16 }
 ,{ _T("img"),       _T("img.gif"),    0, 3, 3, 4, 30, 16 }
 ,{ _T("img"),       _T("img.gif"),    1, 3, 3, 4, 0,  0  }
 ,{ _T("input"),     _T("inp.gif"),    2, 3, 3, 4, 38, 16 }
 ,{ _T("ins"),       _T("ins.gif"),    2, 3, 3, 4, 30, 16 }
 ,{ _T("isindex"),   _T("isx.gif"),    2, 3, 3, 4, 16, 40 }
 ,{ _T("kbd"),       _T("kbd.gif"),    2, 3, 3, 4, 30, 16 }
 ,{ _T("label"),     _T("lbl.gif"),    2, 3, 3, 4, 38, 16 }
 ,{ _T("legend"),    _T("lgn.gif"),    2, 3, 3, 4, 48, 16 }
 ,{ _T("li"),        _T("li.gif"),     2, 3, 3, 4, 22, 16 }   // Lines in ordered/unorderd lists
 ,{ _T("link"),      _T("lnk.gif"),    2, 3, 3, 4, 16, 30 }   // External link (stylesheet)
 ,{ _T("listing"),   _T("lst.gif"),    2, 3, 3, 4, 50, 16 }
 ,{ _T("map"),       _T("map.gif"),    2, 3, 3, 4, 30, 16 } 
 ,{ _T("marquee"),   _T("marq.gif"),   0, 3, 3, 4, 56, 16 }
 ,{ _T("menu"),      _T("men.gif"),    2, 3, 3, 4, 38, 16 }
 ,{ _T("meta"),      _T("met.gif"),    2, 3, 3, 4, 16, 36 }
 ,{ _T("nobr"),      _T("nbr.gif"),    2, 3, 3, 4, 38, 16 }
 ,{ _T("noframes"),  _T("nfr.gif"),    2, 3, 3, 4, 16, 44 }
 ,{ _T("noscript"),  _T("nsr.gif"),    2, 3, 3, 4, 16, 44 }
 ,{ _T("object"),    _T("obj.gif"),    2, 3, 3, 4, 48, 16 }
 ,{ _T("ol"),        _T("ol.gif"),     2, 3, 3, 4, 16, 36 }
 ,{ _T("optgroup"),  _T("opg.gif"),    2, 3, 3, 4, 50, 16 }
 ,{ _T("option"),    _T("opt.gif"),    2, 3, 3, 4, 46, 16 }
 ,{ _T("p"),         _T("p.gif"),      0, 3, 3, 4, 16, 20 }
 ,{ _T("p") ,        _T("pi.gif"),     1, 3, 3, 4, 16, 13 }
 ,{ _T("pre"),       _T("pre.gif"),    2, 3, 3, 4, 16, 24 }
 ,{ _T("param"),     _T("prm.gif"),    2, 3, 3, 4, 42, 16 }
 ,{ _T("q"),         _T("q.gif"),      2, 3, 3, 4, 19, 16 }
 ,{ _T("s"),         _T("s.gif"),      2, 3, 3, 4, 19, 16 }   // Strikethrough
 ,{ _T("script"),    _T("scr.gif"),    0, 3, 3, 4, 44, 16 }
 ,{ _T("select"),    _T("slc.gif"),    2, 3, 3, 4 ,46, 16 }
 ,{ _T("small"),     _T("sml.gif"),    2, 3, 3, 4, 40, 16 }
 ,{ _T("samp"),      _T("smp.gif"),    2, 3, 3, 4, 38, 16 }
 ,{ _T("span"),      _T("spn.gif"),    2, 3, 3, 4, 38, 16 }
 ,{ _T("strike"),    _T("stk.gif"),    2, 3, 3, 4, 44, 16 }
 ,{ _T("style"),     _T("stl.gif"),    2, 3, 3, 4, 16, 40 } 
 ,{ _T("sub"),       _T("sub.gif"),    2, 3, 3, 4, 30, 16 }
 ,{ _T("sup"),       _T("sup.gif"),    2, 3, 3, 4, 30, 16 }
 ,{ _T("tbody"),     _T("tbd.gif"),    2, 3, 3, 4, 16, 42 }
 ,{ _T("table"),     _T("tbl.gif"),    2, 3, 3, 4, 16, 40 }
 ,{ _T("td"),        _T("td.gif"),     2, 3, 3, 4, 16, 22 } 
 ,{ _T("tfoot"),     _T("tft.gif"),    2, 3, 3, 4, 16, 42 } 
 ,{ _T("th"),        _T("th.gif"),     2, 3, 3, 4, 16, 22 }   // Table header cell
 ,{ _T("thead"),     _T("thd.gif"),    2, 3, 3, 4, 16, 42 }
 ,{ _T("tr"),        _T("tr.gif"),     2, 3, 3, 4, 16, 22 }
 ,{ _T("tt"),        _T("tt.gif"),     2, 3, 3, 4, 22, 16 }   // Typewriter text
 ,{ _T("title"),     _T("ttl.gif"),    2, 3, 3, 4, 16, 40 }
 ,{ _T("textarea"),  _T("txt.gif"),    2, 3, 3, 4, 58, 16 }
 ,{ _T("u"),         _T("u.gif"),      2, 3, 3, 4, 19, 16 }   // Underlined
 ,{ _T("ul"),        _T("ul.gif"),     2, 3, 3, 4, 16, 36 }
 ,{ _T("var"),       _T("var.gif"),    2, 3, 3, 4, 30, 16 }
 ,{ _T("wbr"),       _T("wbr.gif"),    0, 3, 3, 4, 16, 13 }
 ,{ _T("xmp"),       _T("xmp.gif"),    2, 3, 3, 4, 30, 16 }
 ,{ NULL,        NULL,         0, 0, 0, 0, 0,  0  }
};

// Load glyphs in the glyphtable
// type     : 0   -> Green glyphs  (Microsoft style)
//            1   -> Yellow glyphs (Mozilla style)
// tags     : false -> No tags
//          : true  -> show all tags
// para     : false -> No tags
//          : true  -> show end-of-paragraph
void
CHTMLEdView::LoadGlyphs(int  type
                       ,bool tags
                       ,bool paragraph)
{
  CString tab;
  CString part;
  CString giftype;
  GLYPHTABLE* table;

  switch(type)
  {
    default:
    case 0: table = green_glyphs;  
            giftype = _T("g_");
            break;
    case 1: table = yellow_glyphs; 
            giftype = _T("y_");
            break;
  }
  for(int ind=0; table[ind].tag; ++ind)
  {
    bool addToTable = false;
    if(_tcscmp(table[ind].tag,_T("p")) == 0 && table[ind].tag_type == 1)
    {
      // Closing of a paragraph is the "pi" sign
      if(tags || paragraph)
      {
        addToTable = true;
      }
    }
    else
    {
      addToTable = tags;
    }
    if(tags || paragraph)
    {
      int width  = table[ind].width;
      int height = table[ind].height;
      if(paragraph && !addToTable)
      {
        width  = 0;
        height = 0;
      }
      part.Format(_T("%%%%%s^^%%%%") GLYPH_SEES_EXE _T("%s%s^^%%%%%d^^%%%%%d^^%%%%%d^^%%%%%d^^%%%%%d^^%%%%%d^^%%%%%d^^%%%%%d^^**")
                  ,table[ind].tag
                  ,giftype.GetString()
                  ,table[ind].resource
                  ,table[ind].tag_type
                  ,table[ind].alignment
                  ,table[ind].positioning
                  ,table[ind].direction
                  ,width
                  ,height
                  ,width
                  ,height);
      tab += part;
    }
  }
  // Add Glyph Table
  HRESULT hr;
  CComVariant stringTable;
  CComBSTR bstrGlyphTable = CT2CW(tab);
  V_VT(&stringTable)   = VT_BSTR;  
  V_BSTR(&stringTable) = bstrGlyphTable;

  // Empty the table
  hr = ExecCommand(&CGID_MSHTML,IDM_EMPTYGLYPHTABLE,MSOCMDEXECOPT_DODEFAULT,NULL,NULL);
  // Set the new table
  if(!tab.IsEmpty())
  {
    hr = ExecCommand(IDM_ADDTOGLYPHTABLE,MSOCMDEXECOPT_DODEFAULT,&stringTable,NULL);
  }
}

HRESULT 
CHTMLEdView::GetDHtmlDocument(IHTMLDocument2** doc)
{
  if(m_Doc2.p)
  {
    *doc = m_Doc2.p;
    return S_OK;
  }
  return S_FALSE;
}

//////////////////////////////////////////////////////////////////////////
//
// MAP / AREA / IMG combinations
//
//////////////////////////////////////////////////////////////////////////

void
CHTMLEdView::ConnectIMGBehaviors()
{
  CComPtr<IHTMLElementCollection> imgColl;
  HRESULT hr = m_Doc2->get_images(&imgColl);
  if(SUCCEEDED(hr))
  {
    long count = 0;
    imgColl->get_length(&count);
    for(int ind = 0; ind < count; ++ ind)
    {
      CComVariant number;
      V_VT(&number) = VT_I4;
      V_I4(&number) = ind;
      CComPtr<IDispatch> disp;
      hr = imgColl->item(number,number,&disp);
      CComQIPtr<IHTMLImgElement,&IID_IHTMLImgElement> image = disp;
      if (SUCCEEDED(hr) && image.p)
      {
        // See if the image has a map
        CComBSTR bMap;
        image->get_useMap(&bMap);
        CString cMap = CW2CT(bMap);
        if(!cMap.IsEmpty())
        {
          IMGControlSite* imb = new IMGControlSite(NULL);
          // Connect the designer, Document and image to this behaviour
          imb->InstallIMGBehavior(&m_designer,m_Doc2,image);
          // Record this behaviour at the designer, so it can find the image
          m_designer.AttachImg(imb);
          // Set map on image
          IMGBehaviorSetMap(imb,cMap);
        }
      }
    }
  }
}

// Find the MAP/AREA definition and set it on the image
void
CHTMLEdView::IMGBehaviorSetMap(IMGControlSite* site,CString cmap)
{
  USES_CONVERSION;

  if(cmap.GetLength() > 1 && cmap.GetAt(0) == _T('#'))
  {
    cmap = cmap.Mid(1);
  }
  CComPtr<IDispatch> disp;
  CComPtr<IHTMLElementCollection> coll;
  HRESULT hr = m_Doc2->get_all(&coll);
  if(SUCCEEDED(hr))
  {
    VARIANT tagName;
    V_VT  (&tagName) = VT_BSTR;
    V_BSTR(&tagName) = L"MAP";
    hr = coll->tags(tagName,&disp);
    if(SUCCEEDED(hr))
    {
      CComQIPtr<IHTMLElementCollection,&IID_IHTMLElementCollection> coll2 = disp;
      long count = 0;
      coll2->get_length(&count);
      for(long ind = 0;ind < count; ++ind)
      {
        CComVariant num;
        V_VT(&num) = VT_I4;
        V_I4(&num) = ind;
        CComPtr<IDispatch> disp2;
        coll2->item(num,num,&disp2);
        CComBSTR bID;
        CComQIPtr<IHTMLElement,&IID_IHTMLElement> pElem = disp2;
        pElem->get_id(&bID);
        CString cID = CW2CT(bID);
        CComQIPtr<IHTMLMapElement,&IID_IHTMLMapElement> pMap = disp2;
        pMap->get_name(&bID);
        CString cName = CW2CT(bID);

        if(cID  .CompareNoCase(cmap) == 0 ||
           cName.CompareNoCase(cmap) == 0 )
        {
          // OK, This is our map
          CComPtr<IDispatch> disp3;
          pElem->get_children(&disp3);
          if(SUCCEEDED(hr))
          {
            VARIANT tagArea;
            V_VT  (&tagArea) = VT_BSTR;
            V_BSTR(&tagArea) = L"AREA";
            CComPtr<IDispatch> disp4;
            CComQIPtr<IHTMLElementCollection,&IID_IHTMLElementCollection> coll3 = disp3;
            // Map can contain other things than AREA only
            hr = coll3->tags(tagArea,&disp4);
            if(SUCCEEDED(hr))
            {
              CComQIPtr<IHTMLElementCollection,&IID_IHTMLElementCollection> coll4 = disp4;
              long count4 = 0;
              coll4->get_length(&count4);
              for(int areas = 0;areas < count4; ++areas)
              {
                V_I4(&num) = areas;
                CComPtr<IDispatch> disp5;
                hr = coll4->item(num,num,&disp5);
                CComQIPtr<IHTMLElement,    &IID_IHTMLElement>     pElement = disp5;
                CComQIPtr<IHTMLAreaElement,&IID_IHTMLAreaElement> pArea    = disp5;
                if(SUCCEEDED(hr) && pArea.p)
                {
                  // This is an area that belongs in our image
                  CComBSTR aShape;
                  CComBSTR aCoords;
                  CComBSTR aID;
                  CComVariant  varHREF;
                  V_VT(&varHREF) = VT_BSTR;

                  pElement->get_id(&aID);
                  pElement->getAttribute(L"href",2,&varHREF);
                  pArea->get_coords(&aCoords);
                  pArea->get_shape(&aShape);

                  CString asShape  = CW2CT(aShape);
                  CString asCoords = CW2CT(aCoords);
                  CString asID     = CW2CT(aID);
                  CString asHref   = CW2CT(V_BSTR(&varHREF));
                  AreaShape areaShape = AREA_INVALID;
  
                  if(asShape.CompareNoCase(_T("circ"))   == 0 ||
                     asShape.CompareNoCase(_T("circle")) == 0)
                  {
                    areaShape = AREA_CIRCLE;
                  }
                  if(asShape.CompareNoCase(_T("poly"))    == 0 ||
                     asShape.CompareNoCase(_T("polygon")) == 0)
                  {
                    areaShape = AREA_POLYGON;
                  }
                  if(asShape.CompareNoCase(_T("rect"))      == 0 ||
                     asShape.CompareNoCase(_T("rectangle")) == 0)
                  {
                    areaShape = AREA_RECTANGLE;
                  }
                  // Add area to IMG behavior
                  ImgArea area(areaShape,asID,asHref,asCoords);
                  site->AttachImgArea(asID,area);
                }
              }
            }
          }
          // Ready with the children of the MAP
          return;
        }
      }
    }
  }
}

void
CHTMLEdView::OnDoubleClickArea(CComPtr<IHTMLElement> pElement)
{
  HtmlArea area(pElement);
  AreaDlg diag(this,&area,theApp.GetBaseDirectory());
  if(diag.DoModal() == IDOK)
  {
    if(diag.GetDoPopup())
    {
      // Check for Popup SCRIPT section in head
      CheckPopupSCRIPT();
    }
  }
}

void 
CHTMLEdView::OnInsertAreaRectangle()
{
  m_designer.SetNewShape(AREA_RECTANGLE);
  AreaMessage(_T("rectangle"),_T("upper-left"),_T("click on the lower left of the rectangle"));
}

void
CHTMLEdView::OnInsertAreaCircle()
{
  m_designer.SetNewShape(AREA_CIRCLE);
  AreaMessage(_T("circle"),_T("center"),_T("click on the outer edge of the circle"));
}

void
CHTMLEdView::OnInsertAreaPolygon()
{
  m_designer.SetNewShape(AREA_POLYGON);
  AreaMessage(_T("polygon"),_T("first-point"),_T("click on all other polygon points,\nand end by clicking on the beginning point"));
}

void
CHTMLEdView::AreaMessage(CString type,CString start,CString next)
{

  CString mess;
  mess.Format(_T("Create a new %s area on an image by painting it with the mouse\n")
              _T("Move the mouse to the %s of the area and left-click on the mouse\n")
              _T("After that, you must %s.")
             ,type .GetString()
             ,start.GetString()
             ,next .GetString());
  theApp.MessageBox(mess,_T("Drawing the area"),_T("ok"));
}

void
CHTMLEdView::OnRemoveArea(CComPtr<IHTMLElement> pElement)
{
  HtmlArea area(pElement);
  CString title  = area.GetProperty(HtmlArea::E_Title);
  CString coords = area.GetProperty(HtmlArea::E_Coords);
  CString href   = area.GetProperty(HtmlArea::E_Href);
  CString shape  = area.GetProperty(HtmlArea::E_Shape);
  CString id     = area.GetAttribute(_T("id"));

  CComPtr<IDispatch> disp;
  pElement->get_document(&disp);
  CComQIPtr<IHTMLDocument2,&IID_IHTMLDocument2> doc = disp;
  CComPtr<IHTMLElement> pImage = Misc::AreaToImg(doc,pElement);
  IMGControlSite* behavior = m_designer.FindIMGSite(pImage);
  if(behavior)
  {
    ImgArea* imgarea = behavior->FindAreaByID(id);
    if(imgarea)
    {
      shape.MakeLower();
      if(shape == _T("rect")) shape = _T("rectangle");
      if(shape == _T("poly")) shape = _T("polygon");
      if(title.IsEmpty()) title = _T("(No title)");

      CString mess;
      mess.Format(_T("Do you want to remove the %s area titled \"%s\"\n")
                  _T("with a reference to: \"%s\"\n")
                  _T("and coordinates: %s ??\n\n")
                  _T("(NOTICE: Removal of the last area will try to remove the MAP)\n")
                 ,shape .GetString()
                 ,title .GetString()
                 ,href  .GetString()
                 ,coords.GetString());
      if(theApp.MessageBox(mess,_T("Remove area?"),_T("?yes no")) == _T("yes"))
      {
        // Now really remove the area
        if(behavior->RemoveArea(id))
        {
          // Find parent (MAP) element
          CComPtr<IHTMLElement> pParent;
          HRESULT hr = pElement->get_parentElement(&pParent);
          if(SUCCEEDED(hr) && pParent.p)
          {
            if(area.Remove())
            {
              // Count the number of children
              long number = 0;
              CComPtr<IDispatch> disp2;
              pParent->get_children(&disp2);
              CComQIPtr<IHTMLElementCollection,&IID_IHTMLElementCollection> coll = disp2;
              coll->get_length(&number);
              if(number == 0)
              {
                // Remove MAP tag
                HtmlElement mapElem(pParent);
                BOOL mapRemoved = mapElem.Remove();
                // Remove "usemap" attribute from image
                VARIANT_BOOL success = VARIANT_FALSE;
                pImage->removeAttribute(L"usemap",(long)0,&success);
  
                if(mapRemoved && success == VARIANT_TRUE)
                {
                  theApp.MessageBox(_T("Removed the last area from this image.\n")
                                    _T("Also removed the total area MAP!!")
                                   ,_T("Last map"),MB_OK);
                }
              }
            }
          }
        }
      }
    }
  }
}
