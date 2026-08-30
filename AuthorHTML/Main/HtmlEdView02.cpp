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
  char* tag;
  char* resource;
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
   { "p",     "pbgn.gif",   0, 3, 3, 4, 0,  0  }
  ,{ "p",     "pend.gif",   1, 3, 3, 4, 20, 15 }
  ,{ "table", "table.gif",  0, 3, 3, 4, 20, 15 }
  ,{ "br",    "br.gif",     0, 3, 3, 4, 20, 15 } 
  ,{ "script","script.gif", 0, 3, 3, 4, 20, 15 }
  ,{ "!",     "comment.gif",0, 3, 3, 4, 20, 15 }
  ,{ "style", "style.gif",  0, 3, 3, 4, 20, 15 }
  ,{ "a",     "anchor.gif", 0, 3, 3, 4, 20, 15 }
  ,{ "a",     "anchor.gif", 1, 3, 3, 4, 0,  0  }
  ,{ "h1",    "h1begin.gif",0, 3, 3, 4, 30, 14 }
  ,{ "h1",    "h1end.gif",  1, 3, 3, 4, 30, 14 }
  ,{ "h2",    "h2begin.gif",0, 3, 3, 4, 30, 14 }
  ,{ "h2",    "h2end.gif",  1, 3, 3, 4, 30, 14 }
  ,{ "h3",    "h3begin.gif",0, 3, 3, 4, 30, 14 }
  ,{ "h3",    "h3end.gif",  1, 3, 3, 4, 30, 14 }
  ,{ "span",  "spanbgn.gif",0, 3, 3, 4, 43, 14 } 
  ,{ "span",  "spanend.gif",1, 3, 3, 4, 43, 14 }
  ,{ NULL,    NULL,         0, 0, 0, 0, 0,  0  }
};

GLYPHTABLE yellow_glyphs[] = 
{
  { "abbr",      "abbr.gif",   2, 3, 3, 4, 34, 16 }
 ,{ "acronym",   "acr.gif",    2, 3, 3, 4, 56, 16 }
 ,{ "address",   "adr.gif",    2, 3, 3, 4, 16, 44 }
 ,{ "a",         "anchor.gif", 2, 3, 3, 4, 19, 16 }
 ,{ "applet",    "applet.gif", 2, 3, 3, 4, 46, 16 }
 ,{ "area",      "area.gif",   0, 3, 3, 4, 38, 16 } 
 ,{ "b",         "b.gif",      2, 3, 3, 4, 19, 16 }
 ,{ "strong",    "b.gif",      2, 3, 3, 4, 19, 16 }
 ,{ "base",      "base.gif",   2, 3, 3, 4, 16, 36 }
 ,{ "bdo",       "bdo.gif",    2, 3, 3, 4, 30, 16 }
 ,{ "big",       "big.gif",    2, 3, 3, 4, 30, 16 }
 ,{ "blockquote","blq.gif",    2, 3, 3, 4, 16, 44 }
 ,{ "body",      "body.gif",   2, 3, 3, 4, 16, 36 }
 ,{ "br",        "br.gif",     0, 3, 3, 4, 22, 16 }
 ,{ "basefont",  "bsf.gif",    2, 3, 3, 4, 56, 16 }
 ,{ "button",    "btn.gif",    2, 3, 3, 4, 50, 16 }
 ,{ "cite",      "cit.gif",    2, 3, 3, 4, 38, 16 }
 ,{ "colgroup",  "clg.gif",    2, 3, 3, 4, 50, 16 }
 ,{ "code",      "cod.gif",    2, 3, 3, 4, 38, 16 }   // Code
 ,{ "col",       "col.gif",    2, 3, 3, 4, 30, 16 }   // Columnar data
 ,{ "comment",   "com.gif",    0, 3, 3, 4, 19, 16 }   // Comment
 ,{ "comment",   "com.gif",    1, 3, 3, 4, 0,  0  }
 ,{ "caption",   "cpt.gif",    0, 3, 3, 4, 54, 16 }
 ,{ "caption",   "cpt.gif",    1, 3, 3, 4, 54, 16 }
 ,{ "center",    "ctr.gif",    2, 3, 3, 4, 16, 44 }   // Centered text
 ,{ "dd",        "dd.gif",     0, 3, 3, 4, 22, 16 }
 ,{ "dd",        "dd.gif",     2, 3, 3, 4, 0,  0  }   // Definition data
 ,{ "del",       "del.gif",    2, 3, 3, 4, 30, 16 }   // Deleted text
 ,{ "dir",       "dir.gif",    2, 3, 3, 4, 30, 16 }   // Directory of files
 ,{ "div",       "div.gif",    2, 3, 3, 4, 16, 24 }   // Division of running text
 ,{ "div",       "div.gif",    0, 3, 1, 4, 16, 24 }   // Division (abs/relative)
 ,{ "dl",        "dl.gif",     2, 3, 3, 4, 16, 20 }   // Definition list
 ,{ "dt",        "dt.gif",     2, 3, 3, 4, 22, 16 }   // Definition term
 ,{ "em",        "em.gif",     2, 3, 3, 4, 22, 16 }   // Emphasis = Italic
 ,{ "fieldset",  "fld.gif",    2, 3, 3, 4, 16, 44 }
 ,{ "font",      "font.gif",   2, 3, 3, 4, 38, 16 }
 ,{ "form",      "form.gif",   2, 3, 3, 4, 16, 36 }
 ,{ "frame",     "frm.gif",    2, 3, 3, 4, 16, 40 }
 ,{ "frameset",  "fst.gif",    2, 3, 3, 4, 16, 44 }
 ,{ "h1",        "h1.gif",     2, 3, 3, 4, 16, 20 }
 ,{ "h2",        "h2.gif",     2, 3, 3, 4, 16, 20 }
 ,{ "h3",        "h3.gif",     2, 3, 3, 4, 16, 20 }
 ,{ "h4",        "h4.gif",     2, 3, 3, 4, 16, 20 }
 ,{ "h5",        "h5.gif",     2, 3, 3, 4, 16, 20 }
 ,{ "h6",        "h6.gif",     2, 3, 3, 4, 16, 20 }
 ,{ "head",      "head.gif",   2, 3, 3, 4, 16, 36 } 
 ,{ "hr",        "hr.gif",     2, 3, 3, 4, 16, 20 }
 ,{ "html",      "html.gif",   2, 3, 3, 4, 16, 38 }
 ,{ "i",         "i.gif",      2, 3, 3, 4, 19, 16 }   // Alsoo italic = see EM
 ,{ "iframe",    "ifr.gif",    2, 3, 3, 4, 46, 16 }
 ,{ "img",       "img.gif",    0, 3, 3, 4, 30, 16 }
 ,{ "img",       "img.gif",    1, 3, 3, 4, 0,  0  }
 ,{ "input",     "inp.gif",    2, 3, 3, 4, 38, 16 }
 ,{ "ins",       "ins.gif",    2, 3, 3, 4, 30, 16 }
 ,{ "isindex",   "isx.gif",    2, 3, 3, 4, 16, 40 }
 ,{ "kbd",       "kbd.gif",    2, 3, 3, 4, 30, 16 }
 ,{ "label",     "lbl.gif",    2, 3, 3, 4, 38, 16 }
 ,{ "legend",    "lgn.gif",    2, 3, 3, 4, 48, 16 }
 ,{ "li",        "li.gif",     2, 3, 3, 4, 22, 16 }   // Lines in ordered/unorderd lists
 ,{ "link",      "lnk.gif",    2, 3, 3, 4, 16, 30 }   // External link (stylesheet)
 ,{ "listing",   "lst.gif",    2, 3, 3, 4, 50, 16 }
 ,{ "map",       "map.gif",    2, 3, 3, 4, 30, 16 } 
 ,{ "marquee",   "marq.gif",   0, 3, 3, 4, 56, 16 }
 ,{ "menu",      "men.gif",    2, 3, 3, 4, 38, 16 }
 ,{ "meta",      "met.gif",    2, 3, 3, 4, 16, 36 }
 ,{ "nobr",      "nbr.gif",    2, 3, 3, 4, 38, 16 }
 ,{ "noframes",  "nfr.gif",    2, 3, 3, 4, 16, 44 }
 ,{ "noscript",  "nsr.gif",    2, 3, 3, 4, 16, 44 }
 ,{ "object",    "obj.gif",    2, 3, 3, 4, 48, 16 }
 ,{ "ol",        "ol.gif",     2, 3, 3, 4, 16, 36 }
 ,{ "optgroup",  "opg.gif",    2, 3, 3, 4, 50, 16 }
 ,{ "option",    "opt.gif",    2, 3, 3, 4, 46, 16 }
 ,{ "p",         "p.gif",      0, 3, 3, 4, 16, 20 }
 ,{ "p" ,        "pi.gif",     1, 3, 3, 4, 16, 13 }
 ,{ "pre",       "pre.gif",    2, 3, 3, 4, 16, 24 }
 ,{ "param",     "prm.gif",    2, 3, 3, 4, 42, 16 }
 ,{ "q",         "q.gif",      2, 3, 3, 4, 19, 16 }
 ,{ "s",         "s.gif",      2, 3, 3, 4, 19, 16 }   // Strikethrough
 ,{ "script",    "scr.gif",    0, 3, 3, 4, 44, 16 }
 ,{ "select",    "slc.gif",    2, 3, 3, 4 ,46, 16 }
 ,{ "small",     "sml.gif",    2, 3, 3, 4, 40, 16 }
 ,{ "samp",      "smp.gif",    2, 3, 3, 4, 38, 16 }
 ,{ "span",      "spn.gif",    2, 3, 3, 4, 38, 16 }
 ,{ "strike",    "stk.gif",    2, 3, 3, 4, 44, 16 }
 ,{ "style",     "stl.gif",    2, 3, 3, 4, 16, 40 } 
 ,{ "sub",       "sub.gif",    2, 3, 3, 4, 30, 16 }
 ,{ "sup",       "sup.gif",    2, 3, 3, 4, 30, 16 }
 ,{ "tbody",     "tbd.gif",    2, 3, 3, 4, 16, 42 }
 ,{ "table",     "tbl.gif",    2, 3, 3, 4, 16, 40 }
 ,{ "td",        "td.gif",     2, 3, 3, 4, 16, 22 } 
 ,{ "tfoot",     "tft.gif",    2, 3, 3, 4, 16, 42 } 
 ,{ "th",        "th.gif",     2, 3, 3, 4, 16, 22 }   // Table header cell
 ,{ "thead",     "thd.gif",    2, 3, 3, 4, 16, 42 }
 ,{ "tr",        "tr.gif",     2, 3, 3, 4, 16, 22 }
 ,{ "tt",        "tt.gif",     2, 3, 3, 4, 22, 16 }   // Typewriter text
 ,{ "title",     "ttl.gif",    2, 3, 3, 4, 16, 40 }
 ,{ "textarea",  "txt.gif",    2, 3, 3, 4, 58, 16 }
 ,{ "u",         "u.gif",      2, 3, 3, 4, 19, 16 }   // Underlined
 ,{ "ul",        "ul.gif",     2, 3, 3, 4, 16, 36 }
 ,{ "var",       "var.gif",    2, 3, 3, 4, 30, 16 }
 ,{ "wbr",       "wbr.gif",    0, 3, 3, 4, 16, 13 }
 ,{ "xmp",       "xmp.gif",    2, 3, 3, 4, 30, 16 }
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
            giftype = "g_";
            break;
    case 1: table = yellow_glyphs; 
            giftype = "y_";
            break;
  }
  for(int ind=0; table[ind].tag; ++ind)
  {
    bool addToTable = false;
    if(strcmp(table[ind].tag,"p") == 0 && table[ind].tag_type == 1)
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
      part.Format("%%%%%s^^%%%%" GLYPH_SEES_EXE "%s%s^^%%%%%d^^%%%%%d^^%%%%%d^^%%%%%d^^%%%%%d^^%%%%%d^^%%%%%d^^%%%%%d^^**"
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

  if(cmap.GetLength() > 1 && cmap.GetAt(0) == '#')
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
                  pElement->getAttribute(CT2CW("href"),2,&varHREF);
                  pArea->get_coords(&aCoords);
                  pArea->get_shape(&aShape);

                  CString asShape  = CW2CT(aShape);
                  CString asCoords = CW2CT(aCoords);
                  CString asID     = CW2CT(aID);
                  CString asHref   = CW2CT(V_BSTR(&varHREF));
                  AreaShape areaShape = AREA_INVALID;
  
                  if(asShape.CompareNoCase("circ")   == 0 ||
                     asShape.CompareNoCase("circle") == 0)
                  {
                    areaShape = AREA_CIRCLE;
                  }
                  if(asShape.CompareNoCase("poly")    == 0 ||
                     asShape.CompareNoCase("polygon") == 0)
                  {
                    areaShape = AREA_POLYGON;
                  }
                  if(asShape.CompareNoCase("rect")      == 0 ||
                     asShape.CompareNoCase("rectangle") == 0)
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
  AreaDlg diag(this,&area,GetBase());
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
  AreaMessage("rectangle","upper-left","click on the lower left of the rectangle");
}

void
CHTMLEdView::OnInsertAreaCircle()
{
  m_designer.SetNewShape(AREA_CIRCLE);
  AreaMessage("circle","center","click on the outer edge of the circle");
}

void
CHTMLEdView::OnInsertAreaPolygon()
{
  m_designer.SetNewShape(AREA_POLYGON);
  AreaMessage("polygon","first-point","click on all other polygon points");
}

void
CHTMLEdView::AreaMessage(CString type,CString start,CString next)
{

  CString mess;
  mess.Format("Create a new %s area on an image by painting it with the mouse\n"
              "Move the mouse to the %s of the area and left-click on the mouse\n"
              "After that, you must %s."
             ,type .GetString()
             ,start.GetString()
             ,next .GetString());
  theApp.MessageBox(mess,"Drawing the area","ok");
}

void
CHTMLEdView::OnRemoveArea(CComPtr<IHTMLElement> pElement)
{
  HtmlArea area(pElement);
  CString title  = area.GetProperty(HtmlArea::E_Title);
  CString coords = area.GetProperty(HtmlArea::E_Coords);
  CString href   = area.GetProperty(HtmlArea::E_Href);
  CString shape  = area.GetProperty(HtmlArea::E_Shape);
  CString id     = area.GetAttribute("id");

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
      if(shape == "rect") shape = "rectangle";
      if(shape == "poly") shape = "polygon";
      if(title.IsEmpty()) title = "(No title)";

      CString mess;
      mess.Format("Do you want to remove the %s area titled \"%s\"\n"
                  "with a reference to: \"%s\"\n"
                  "and coordinates: %s ??\n\n"
                  "(NOTICE: Removal of the last area will try to remove the MAP)\n"
                 ,shape .GetString()
                 ,title .GetString()
                 ,href  .GetString()
                 ,coords.GetString());
      if(theApp.MessageBox(mess,"Remove area?","?yes no") == "yes")
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
                  theApp.MessageBox("Removed the last area from this image.\n"
                                    "Also removed the total area MAP!!"
                                   ,"Last map",MB_OK);
                }
              }
            }
          }
        }
      }
    }
  }
}
