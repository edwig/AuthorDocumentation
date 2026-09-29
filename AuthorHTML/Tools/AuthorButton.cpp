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

AuButton::AuButton(CString& p_type)
{
  m_buttonImage.CalculateStandard(p_type.GetString(),_T("KNOP"));
}

AuButton::~AuButton()
{
  DestroyWindow();
}

BEGIN_MESSAGE_MAP(AuButton, AD_Button)
  ON_WM_PAINT()
  ON_WM_KEYDOWN()
  ON_WM_ERASEBKGND()
  ON_WM_GETDLGCODE()
  ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

void
AuButton::SetImage(CString p_type,CString p_library)
{
  m_buttonImage.CalculateStandard(p_type.GetString(),p_library.IsEmpty() ? _T("KNOP") : p_library);
}

void
AuButton::OnKeyDown(UINT nChar, 
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
AuButton::OnPaint()
{
  AD_Button::OnPaint();
}

void AuButton::DrawItem(LPDRAWITEMSTRUCT s) 
{
  CDC* dc = CDC::FromHandle(s->hDC);;
  int saveDC = dc->SaveDC();
  
  CWnd* win = CWnd::FromHandle(s->hwndItem);
  bool transparant = (win->GetExStyle() & WS_EX_TRANSPARENT) > 0;   // Outside buttun it's transparent
  bool transparantFace = transparant;                               // Button surface transparant
  bool BGclear = false;                                             // Is the surface overwritten?
  bool BGerase = false;                                             // Alternative to clear the surface
  bool ddKader = (win->GetStyle() & BS_FLAT) == 0;                  // 3D frame or flat frame
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
    bDefault = parent->GetDefault(GetDlgCtrlID());
  }
  int  kaderBreedte = ddKader?3:0;                                  // pixels in the frame
  int  tplaats = -1;                                                // TEXT place
  int  bplaats = -1;                                                // BITMAP place

	CRect crect,crectOrig;
  CBrush br;
  CSize tsize(0,0);
  CRect trect(0,0,0,0);
  HBRUSH hBr = NULL;
  win->GetClientRect(crect);
  win->GetClientRect(crectOrig);
  CRgn knopRgn;

  // Set the background color (default=4/focus=2/selected=1/normal=0)
  int buttonCtlState = s->itemState & (ODS_SELECTED)?1:0;
  if(bFocus)   buttonCtlState |= 2;
  if(bDefault) buttonCtlState |= 4;

  // ***********************************************************
  // First, depending on the button type, the clipping is determined
  CRect rgnRect(crect);
  rgnRect.DeflateRect(kaderBreedte,kaderBreedte);
  knopRgn.CreateRectRgnIndirect(rgnRect);

  if(!transparant)
  {
    dc->FillSolidRect( crect,dc->GetBkColor());
  }
  dc->SelectClipRgn(&knopRgn);

  // ***********************************************************
  // Placing the text and the image

  int buttonLayout = BUTT_LAYOUT_LEFT;
  m_buttonImage.CalculateButtonLayout(buttonLayout);

  CString txt;
  if (!m_buttonImage.HasImage() || buttonLayout != BUTT_LAYOUT_IMAGE)
  {
    win->GetWindowText(txt);
  }
  if (txt != _T(""))
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
      // Draw an image
      int volgnr = -1;
      if (IsWindowEnabled())
      {
        // Determine which image is drawn from the ControlsInfo set
        if(s->itemState & ODS_SELECTED)
        {
          focusOffset = 2;             // Image/Text 2 pixels to the bottom right
          volgnr = AFB_POS_PRESS; // Right mouse button pressed
        }
        else
        {
          if(s->itemState & ODS_FOCUS)
          {
            volgnr = AFB_POS_FOCUS; // Button has focus, focus image
          }
          else
          {
            volgnr = AFB_POS_STAN; // Standard image
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
          focusOffset = 2;   // Text 2 pixels to the bottom right
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

  // Brush for the border
  COLORREF randKleur = 0; // RGB(0,0,0) = Black
  hBr = NULL;

  bool maakRand  = false;
  if ((bFocus || bDefault) && bAktief)
  {
    maakRand = true;
    randKleur = 0;
  }
  br.CreateSolidBrush(randKleur);
  hBr = (HBRUSH)br;

  // Draw standard square button
  // The border;
  dc->SelectStockObject(HOLLOW_BRUSH);
  if (maakRand)
  {
    ::FrameRect(s->hDC,crect,hBr);
    crect.DeflateRect(1,1);
  }
  // Draw button frame
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

  // Focus: Emphasize the button
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
  // Default button: Extra black border around it to emphasize
  // Without color settings, it is still visible
  if(bDefault)
  {
    CBrush brush(RGB(0,0,0));
    crect.InflateRect(4,4);
    dc->FrameRect(crect,&brush);
  }
  dc->RestoreDC(saveDC);
}

afx_msg BOOL 
AuButton::OnEraseBkgnd( CDC* )
{
  return TRUE;
}

void 
AuButton::OnLButtonUp( UINT nFlags, CPoint point )
{
  AuMessageBox* box = (AuMessageBox*)(GetParent());
  if(box)
  {
    box->PressOnButton(GetDlgCtrlID(),point);
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

BOOL AuButton::PreTranslateMessage(MSG* pMsg) 
{
  if 	(pMsg->message == WM_LBUTTONDBLCLK)
  {
    pMsg->message = WM_LBUTTONDOWN;
  }
	return AD_Button::PreTranslateMessage(pMsg);
}

void
AuButton::PlaatsRectInRect(const CRect& mRect,CRect& rect,int pos,int marges)
{
  CRect inRect(mRect);
  inRect.DeflateRect(2*marges,2*marges);
  // Place rect at the top left in inRect
  // Determine margins
  int restx = (inRect.Width()  -  rect.Width());
  int resty = (inRect.Height() -  rect.Height());

  double prop = 0;
  // 20 - 30   // Proportional scaling
  // 30 - 40   // Enlarge but do not reduce
  if (pos >= 20 && pos < 40 )  // Proportional
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

  switch(pos) // text position
  {
    default:
    case 0:   break;
    case 1:   rect.OffsetRect(restx/2,0);
              break;
    case 2:   rect.OffsetRect(restx,0);
              break;
    case 3:   rect.OffsetRect(0,resty/2);
              break;
    case 4:   rect.OffsetRect(restx/2,resty/2);
              break;
    case 5:   rect.OffsetRect(restx,resty/2);
              break;
    case 6:   rect.OffsetRect(0,resty);
              break;
    case 7:   rect.OffsetRect(restx/2,resty);
              break;
    case 8:   rect.OffsetRect(restx,resty);
              break;
    case 10:  // all
              rect = inRect;
              break;
    case 11:  // top long
              rect.right= inRect.right;
              break;
    case 12:  // right high
              rect.OffsetRect(restx,0);
              rect.bottom = inRect.bottom;
              break;
    case 13:  // bottom long
              rect.OffsetRect(0,resty);
              rect.right= inRect.right;
              break;
    case 14:  // left high
              rect.bottom = inRect.bottom;
              break;
  }
}
