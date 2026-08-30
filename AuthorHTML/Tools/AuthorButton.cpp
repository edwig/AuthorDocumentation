//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    AuthorButton.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: A specialized window button
//
#include "StdAfx.h"
#include "Images.h"
#include "AuthorButton.h"
#include "AuMessageBox.h"

CButtonC::CButtonC(CString& p_type)
{
  m_buttonImage.CalculateStandard(p_type.GetString(),"KNOP"); //AFB_SET_KNOPPEN);
}

CButtonC::~CButtonC()
{
  DestroyWindow();
}

BEGIN_MESSAGE_MAP(CButtonC, AD_Button)
  ON_WM_PAINT()
  ON_WM_KEYDOWN()
  ON_WM_ERASEBKGND()
  ON_WM_GETDLGCODE()
  ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

void
CButtonC::SetImage(CString p_type,CString p_library)
{
  m_buttonImage.CalculateStandard(p_type.GetString(),p_library.IsEmpty() ? "KNOP" : p_library);
}

void
CButtonC::OnKeyDown(UINT nChar, 
                    UINT nRepCnt, 
                    UINT nFlags)
{
  AuMessageBox* parent = dynamic_cast<AuMessageBox*> (GetParent());
  if(parent)
  {
    parent->OnKeyDown(nChar,nRepCnt,nFlags);
  }
  else
  {
    CWnd* wparent = dynamic_cast<CWnd*> (GetParent());
    if(wparent)
    {
      ::SendMessage(wparent->m_hWnd,WM_KEYDOWN,(WPARAM)nChar,(LPARAM)nFlags);
    }
  }
  AD_Button::OnKeyDown(nChar,nRepCnt,nFlags);
}

void
CButtonC::OnPaint()
{
  AD_Button::OnPaint();
}

void CButtonC::DrawItem(LPDRAWITEMSTRUCT s) 
{
  CDC* dc = CDC::FromHandle(s->hDC);;
  int saveDC = dc->SaveDC();
  //KleurenInfo&ki = ibs->GeefKleurenInfo();
  
  CWnd* win = CWnd::FromHandle(s->hwndItem);
  bool transparant = (win->GetExStyle() & WS_EX_TRANSPARENT) > 0;   // buiten de knop transparant
  bool transparantFace = transparant;                               // knop oppervlakte transparant
  bool BGclear = false;                                             // is de knopface overschreven ?
  bool BGerase = false;                                             // alternatief om de face te cleren
  bool ddKader = (win->GetStyle() & BS_FLAT) == 0;                  // Drie d kader
  bool bAktief  = true;
  bool bFocus   = ::GetFocus() == s->hwndItem;
  bool bDefault = false;
  int  hoogte   = 16;
  int  breedte  = 16;
  int  focusOffset = 0;

  if(m_buttonImage.m_image)
  {
    hoogte   = m_buttonImage.m_image->GeefRect().Height();
    breedte  = m_buttonImage.m_image->GeefRect().Width();
  }

  // TODO FOR DEFAULT BUTTON
  bDefault = false;
  AuMessageBox* parent = dynamic_cast<AuMessageBox*> (GetParent());
  if(parent)
  {
    bDefault = parent->GeefDefault(GetDlgCtrlID());
  }
  int  kaderBreedte = ddKader?3:0;                                  // pixels in het kader
  int  tplaats = -1;                                                // TEXT plaats
  int  bplaats = -1;                                                // BITMAP plaats

	CRect crect,crectOrig;
  CBrush br;
  CSize tsize(0,0);
  CRect trect(0,0,0,0);
  HBRUSH hBr = NULL;
  win->GetClientRect(crect);
  win->GetClientRect(crectOrig);
  CRgn knopRgn;

  // Zet de kleur van de achtergrond (default=4/focus=2/selected=1/gewoon=0)
  int buttonCtlState = s->itemState & (ODS_SELECTED)?1:0;
  if(bFocus)   buttonCtlState |= 2;
  if(bDefault) buttonCtlState |= 4;

  // ***********************************************************
  // Eerst wordt afhankelik van het knoptype de clipping bepaald
  CRect rgnRect(crect);
  rgnRect.DeflateRect(kaderBreedte,kaderBreedte);
  knopRgn.CreateRectRgnIndirect(rgnRect);

  if(!transparant) //((kaderBreedte > 0) && !transparant)
  {
    dc->FillSolidRect( crect,dc->GetBkColor());
  }
  dc->SelectClipRgn(&knopRgn);

  // ***********************************************************
  // Plaatsen van de text en het plaatje

  // TODO was controlsinfo
  int buttonLayout = BUTT_LAYOUT_LEFT;
  m_buttonImage.CalculateButtonLayout(buttonLayout);

  CString txt;
  if (!m_buttonImage.HasImage() || buttonLayout != BUTT_LAYOUT_IMAGE)
  {
    win->GetWindowText(txt);
  }
  if (txt != "")
  {
    CRect tinrect(crect);

    tsize = dc->GetTextExtent(txt);
    trect.SetRect(0,0,tsize.cx,tsize.cy);
    dc->DrawText(txt,&trect,DT_CALCRECT|DT_CENTER|DT_VCENTER|DT_SINGLELINE);

    if(m_buttonImage.HasImage() && buttonLayout != BUTT_LAYOUT_NONE)
    {
      if (buttonLayout & BUTT_LAYOUT_TOP)
      {
        tplaats = 7;
        bplaats = 1;
      }
      else if (buttonLayout & BUTT_LAYOUT_BOTTOM)
      {
        tplaats = 1;
        bplaats = 7;
      }
      else if (buttonLayout & BUTT_LAYOUT_LEFT)
      {
        tplaats = 4;
        bplaats = 3;
        tinrect.left += breedte;
      }
      else if (buttonLayout & BUTT_LAYOUT_RIGHT)
      {
        tplaats = 4;
        bplaats = 5;
        tinrect.right -= breedte;
      }
    }
    else
    {
      tplaats = 4;
    }
    PlaatsRectInRect(tinrect,trect,tplaats,3);
  }
  else
  {
    bplaats = 4;
  }

  if (s->itemAction & (ODA_SELECT |ODA_DRAWENTIRE | ODA_FOCUS))
  {
    int Drawstate = AFB_PAINT_FILL | AFB_PAINT_TRANS;

    if(m_buttonImage.HasImage() && !(buttonLayout & BUTT_LAYOUT_NONE))
    {
      // teken een plaatje
      int volgnr = -1;
      if (IsWindowEnabled())
      {
        // Bepaal welk plaatje wordt getekend uit de ControlsInfo set
        if(s->itemState & ODS_SELECTED)
        {
          focusOffset = 2;             // Plaatje/Text 2 pixels naar rechtsonder
          volgnr = AFB_POS_PRESS; // Rechtermuisknop ingedrukt
        }
        else
        {
          if(s->itemState & ODS_FOCUS)
          {
            volgnr = AFB_POS_FOCUS; // Knop heeft focus, focus plaatje
          }
          else
          {
            volgnr = AFB_POS_STAN; // Standaard plaatje
          }
        }
      }
      else
      {
        Drawstate |= AFB_PAINT_DISABLED;
      }
      CRect bmpRect(0,0,breedte,hoogte);

      PlaatsRectInRect(crect,bmpRect,bplaats,3);
      bmpRect.OffsetRect(focusOffset,focusOffset);
      m_buttonImage.PaintBitmap(*dc,bmpRect,volgnr,Drawstate);

      if (!win->IsWindowEnabled() && !BGclear)
      {
        dc->FillSolidRect(trect,dc->GetBkColor());
      }
      ::DrawState(dc->m_hDC,NULL,NULL
                 ,(LPARAM)txt.GetString()
                 ,0
                 ,trect.left + focusOffset
                 ,trect.top  + focusOffset
                 ,trect.Width()
                 ,trect.Height(),
                 DST_PREFIXTEXT | (win->IsWindowEnabled()?0:DSS_DISABLED));

      dc->ExcludeClipRect(&bmpRect);
    }
    else
    {
      if (transparantFace)
      {
        dc->SetBkMode(TRANSPARENT);
      }
      if(win->IsWindowEnabled())
      {
        if(s->itemState & ODS_SELECTED)
        {
          focusOffset = 2;   // Text 2 pixels naar rechtsonder
        }
      }
      if (!win->IsWindowEnabled() && !BGclear)
      {
        dc->FillSolidRect(trect,dc->GetBkColor());
      }
      ::DrawState(dc->m_hDC,NULL,NULL
                 ,(LPARAM)txt.GetString()
                 ,0
                 ,trect.left + focusOffset
                 ,trect.top  + focusOffset
                 ,trect.Width()
                 ,trect.Height()
                 ,DST_PREFIXTEXT | (win->IsWindowEnabled()?0:DSS_DISABLED));
    }
    dc->ExcludeClipRect(&trect);
  
    if (BGerase || (!BGclear && !transparantFace && s->itemAction & (ODA_DRAWENTIRE|ODA_SELECT)))
    {
      dc->FillSolidRect( crect,dc->GetBkColor());
      BGclear = true;
    }
  }
  dc->SelectClipRgn(NULL);

  // Brush voor randje
  COLORREF randKleur = 0; // RGB(0,0,0) = Zwart
  hBr = NULL;

  bool maakRand  = false;
  if ((bFocus || bDefault) && bAktief)
  {
    maakRand = true;
    randKleur = 0;
  }
  br.CreateSolidBrush(randKleur);
  hBr = (HBRUSH)br;

  // Standaard vierkante knop intekenen
  // Het randje;
  dc->SelectStockObject(HOLLOW_BRUSH);
  if (maakRand)
  {
    ::FrameRect(s->hDC,crect,hBr);
    crect.DeflateRect(1,1);
  }
  // Knop kader tekenen
  int kleurLB, kleurRO;
  if(s->itemState & ODS_SELECTED)
  {
    kleurLB =
    kleurRO = ::GetSysColor(COLOR_BTNSHADOW);
  }
  else
  {
    kleurLB = ::GetSysColor(COLOR_BTNHILIGHT);
    kleurRO = ::GetSysColor(COLOR_3DDKSHADOW);
  }
  if (ddKader)
  {
    if(bDefault)
    {
      crect.DeflateRect(2,2,1,1);
    }
    dc->Draw3dRect( crect,kleurLB,kleurRO);
    if(bDefault)
    {
      crect.InflateRect(2,2,1,1);
    }
    crect.DeflateRect(1,1);
  }
  else
  {
    dc->Rectangle(crect);
  }
  crect.DeflateRect(3,3);
  trect.InflateRect(1,1);

  // Focus: Knop extra benadrukken
  trect.IntersectRect(&trect,&crect);
  trect.OffsetRect(focusOffset,focusOffset);
  dc->SetBkColor(GetSysColor(COLOR_3DFACE));
  if (BGclear)
  {
    if (::GetFocus() == s->hwndItem)
    {
      dc->DrawFocusRect(trect);
    }
  }
  else if (s->itemAction & ODA_FOCUS)
  {
    dc->DrawFocusRect(trect);
  }
  // Default knop: Extra zwarte rand eromheen om het te benadrukken
  // Zonder kleurinstellingen dan toch nog te zien
  if(bDefault)
  {
    CBrush brush(RGB(0,0,0));
    crect.InflateRect(4,4);
    dc->FrameRect(crect,&brush);
  }
  dc->RestoreDC(saveDC);
}

afx_msg BOOL 
CButtonC::OnEraseBkgnd( CDC* )
{
  return TRUE;
}

void 
CButtonC::OnLButtonUp( UINT nFlags, CPoint point )
{
  AuMessageBox* box = (AuMessageBox*)(GetParent());
  if(box)
  {
    box->DrukOpKnop(GetDlgCtrlID(),point);
  }
  else
  {
    CWnd* parent = dynamic_cast<CWnd*> (GetParent());
    if(parent)
    {
      ::SendMessage(parent->m_hWnd,WM_LBUTTONUP,0,MAKELPARAM(point.x,point.y));
    }
  }
  AD_Button::OnLButtonUp(nFlags,point);
}

BOOL CButtonC::PreTranslateMessage(MSG* pMsg) 
{
  if 	(pMsg->message == WM_LBUTTONDBLCLK)
  {
    pMsg->message = WM_LBUTTONDOWN;
  }
	return AD_Button::PreTranslateMessage(pMsg);
}

void
CButtonC::PlaatsRectInRect(const CRect& mRect,CRect& rect,int pos,int marges)
{
  CRect inRect(mRect);
  inRect.DeflateRect(2*marges,2*marges);
  // Plaats rect linksboven in inRect
  // Bepaal marges
  int restx = (inRect.Width()  -  rect.Width());
  int resty = (inRect.Height() -  rect.Height());

  double prop = 0;
  // 20 - 30   // Proportionele schaling
  // 30 - 40   // Vergroten maar niet verkleinen
  if (pos >= 20 && pos < 40 )  // Proportioneel
  {
    if (restx != 0 && resty != 0)
    {
      prop = __min((double)restx / (double)rect.Width(),(double)resty / (double)rect.Height());
      prop = prop/2.0;
      if ( prop != 0.0 && !(pos > 30 && prop > 0.0) )
      {
        rect.InflateRect((int)(rect.Width() * prop) , (int)(rect.Height() * prop));
        restx = (inRect.Width()  -  rect.Width());
        resty = (inRect.Height() -  rect.Height());
      }
    }
    if (pos > 30) pos -= 30;
    if (pos > 20) pos -= 20;
  }
  rect.OffsetRect(inRect.left - rect.left ,inRect.top - rect.top);

  switch(pos) // textpositie
  {
    default:
    case 0:    break;
    case 1:    rect.OffsetRect(restx/2,0);
              break;
    case 2:    rect.OffsetRect(restx,0);
              break;
    case 3:    rect.OffsetRect(0,resty/2);
              break;
    case 4:    rect.OffsetRect(restx/2,resty/2);
              break;
    case 5:    rect.OffsetRect(restx,resty/2);
              break;
    case 6:    rect.OffsetRect(0,resty);
              break;
    case 7:    rect.OffsetRect(restx/2,resty);
              break;
    case 8:    rect.OffsetRect(restx,resty);
              break;
    case 10: // alles
              rect = inRect;
              break;
    case 11: // boven lang
              rect.right= inRect.right;
              break;
    case 12: // rechts hoog
              rect.OffsetRect(restx,0);
              rect.bottom = inRect.bottom;
              break;
    case 13: // onder lang
              rect.OffsetRect(0,resty);
              rect.right= inRect.right;
              break;
    case 14: // links hoog
              rect.bottom = inRect.bottom;
              break;
  }
}


