// CPlaatjes.cpp
//
#include "Stdafx.h"
#include "Images.h"
#include "DIB.h"
#include "CIcon.h"
#include <algorithm>
#include <vector>
#include <map>
#include <crtdbg.h>

using namespace std;

#pragma warning (disable: 4239)

CAfbeeldingInfo::CAfbeeldingInfo(CAfbeeldingen& afbeeldingen)
                :m_afbeeldingen(afbeeldingen)
{
}

CAfbeeldingInfo::~CAfbeeldingInfo()
{ 
}

Ref<CAfbeeldingEntry>
CAfbeeldingInfo::SetEntry(Ref<CAfbeeldingEntry> entry)
{
  Ref<CAfbeeldingEntry> oudeEntry = m_entry;
  m_entry = entry;
  return oudeEntry;
}

bool 
CAfbeeldingInfo::HasImage() const
{
  return (m_entry && m_entry->m_afbeelding && !(m_entry->m_info & AFB_INFO_INVALID));
}


//****************************************************************************
//                                                                           *
//                                CBitmapDC                                  *
//                   Klasse om een dc met een bitmap te beheren              *
//                                                                           *
//****************************************************************************
CBitmapDC::CBitmapDC(CDC* dc)
         :m_bitmap(NULL),
          m_oldBitmap(NULL)
{
  if (!CreateCompatibleDC(dc))
  {
    TRACE(_T("Error: CreateCompatibleDC\n"));
  }
  if (dc)
    SetMapMode(dc->GetMapMode());
  m_oldBitmap = GetCurrentBitmap();
}

CBitmapDC::CBitmapDC(CDC* dc,CBitmap* bitmap)
         :m_bitmap(NULL),
          m_oldBitmap(NULL)
{
  if (!CreateCompatibleDC(dc))
  {
    TRACE(_T("Error: CreateCompatibleDC\n"));
  }
  if (dc)
    SetMapMode(dc->GetMapMode());
  m_oldBitmap = GetCurrentBitmap();
  SelectBitmap(bitmap);
}

CBitmapDC::CBitmapDC(CDC* dc,int nWidth,int nHeight)
         :m_bitmap(NULL),
          m_oldBitmap(NULL)
{
  if (!CreateCompatibleDC(dc))
  {
    TRACE(_T("Error: CreateCompatibleDC\n"));
  }
  if (dc)
    SetMapMode(dc->GetMapMode());
  m_oldBitmap = GetCurrentBitmap();
  CreateCompatibleBitmap(dc,nWidth,nHeight);
}

CBitmapDC::CBitmapDC(CDC* dc,int nWidth,int nHeight,UINT nPlanes, UINT nBitcount)
         :m_bitmap(NULL),
          m_oldBitmap(NULL)
{
  if (!CreateCompatibleDC(dc))
  {
    TRACE(_T("Error: CreateCompatibleDC\n"));
  }
  if (dc)
    SetMapMode(dc->GetMapMode());
  m_oldBitmap = GetCurrentBitmap();
  CreateBitmap(nWidth,nHeight,nPlanes,nBitcount,NULL);
}

CBitmapDC::~CBitmapDC()
{
  DeSelectBitmap();
  delete m_bitmap;
}

bool
CBitmapDC::CreateCompatibleBitmap(CDC* dc,int nWidth,int nHeight)
{
  bool cc = false;
  delete m_bitmap;
  m_bitmap = new CBitmap;
  cc = m_bitmap->CreateCompatibleBitmap(dc,nWidth,nHeight) == TRUE;
  if (cc)
    SelectBitmap(m_bitmap);
  return cc;
}

bool
CBitmapDC::CreateBitmap( int nWidth, int nHeight, UINT nPlanes, UINT nBitcount, const void* lpBits )
{
  bool cc = false;
  delete m_bitmap;
  m_bitmap = new CBitmap;
  cc = m_bitmap->CreateBitmap(nWidth,nHeight,nPlanes,nBitcount,lpBits) == TRUE;
  if (cc)
    SelectBitmap(m_bitmap);
  return cc;
}
bool
CBitmapDC::LoadBitmap( UINT ID )
{
  bool cc = false;
  delete m_bitmap;
  m_bitmap = new CBitmap;
  cc = m_bitmap->LoadBitmap(ID) == TRUE;
  if (cc)
    SelectBitmap(m_bitmap);
  return cc;
}


void 
CBitmapDC::SelectBitmap(CBitmap* bitmap)
{
  if (!SelectObject(bitmap))
  {
    TRACE(_T("Cannot select bitmap"));
  }
}

void
CBitmapDC::DeSelectBitmap()
{
  SelectObject(m_oldBitmap);
}


CBitmap*
CBitmapDC::GeefBitmap()
{
  return m_bitmap;
}

//****************************************************************************
//                                                                           *
//                                CAfbeelding                                *
//                                                                           *
//                                                                           *
//****************************************************************************
CAfbeelding::CAfbeelding(CAfbeeldingen& afbeeldingen)
         :m_afbeeldingen(afbeeldingen),
          m_cx(0),
          m_cy(0),
          m_aantal(0),
          m_aantalPS(1),
          m_heeftDiasabled(false),
          m_typeSet(AFB_SET_KNOPPEN)
{
  SetType(_T("S"));
}

CAfbeelding::~CAfbeelding()
{
}


void
CAfbeelding::Reset()
{
  SetType(_T("S"));
  OnReset();
}

bool 
CAfbeelding::SetType(LPCTSTR nieuwType)
{
  if (_tcslen(nieuwType) > AFB_POS_AANTAL)
    return false;

  m_heeftDiasabled = false;
  memset(m_pos,255,sizeof(m_pos));
  memset(m_type,_T('\0'),sizeof(m_type));


  if (nieuwType && CAfbeeldingen::IsSetNummer(nieuwType))
  {
    short aantal = (short)_ttoi(nieuwType);
    m_aantalPS = aantal;
    m_type[0] = *nieuwType;

    for (short i = 0 ; i < AFB_POS_AANTAL ; i++)
    {
      if (i < aantal)
      {
        m_pos[i] = i;
      }
      else
        m_pos[i] = AFB_POS_UIT;
    }
    m_typeSet = AFB_SET_SERIE;
    return true;
  }


  bool enkelX = true;
  m_aantalPS = 0;
  m_typeSet = AFB_SET_KNOPPEN;
  for (LPCTSTR p = nieuwType ; *p != _T('\0') ; p++)
  {
    int pos    = -1;
    m_aantalPS++;
    switch(_totupper(*p))
    {
    case _T('S'):
      pos = AFB_POS_STAN;
      enkelX = false;
      break;
    case _T('F'):
      pos    = AFB_POS_FOCUS;
      enkelX = false;
      break;
    case _T('P'):
      pos = AFB_POS_PRESS;
      enkelX = false;
      break;
    case _T('D'):
      m_heeftDiasabled = true;
      pos = AFB_POS_DIS;
      enkelX = false;
      break;
    case _T('E'):
      pos = AFB_POS_EXTRA;
      enkelX = false;
      break;
    case _T('X'):
      pos = m_aantalPS -1;
      break;
    }
    m_type[p - nieuwType] = (TCHAR)_totupper(*p);
    if (pos >= 0 && m_pos[pos] == -1)
      m_pos[pos] = (short)(p - nieuwType);
  }
  if (m_aantalPS > 0 && enkelX)
    m_typeSet = AFB_SET_SET;
  if (m_typeSet == AFB_SET_KNOPPEN)
  {
    MaakStandaardKnopVolgnummers(m_pos,AFB_POS_AANTAL);
    SetInterneKnopLogica(m_pos,0,AFB_POS_AANTAL);
  }
  return true;
}

void 
CAfbeelding::MaakStandaardKnopVolgnummers(short volgnr[],int aantal)
{
  if (volgnr[AFB_POS_STAN] == AFB_POS_GEEN)
  {
    volgnr[AFB_POS_STAN] = 0;
    for(int i = 0; i < aantal; i++)
    {
      if (volgnr[i] != AFB_POS_GEEN)
      {
        volgnr[AFB_POS_STAN] = volgnr[i];
        break;
      }
    }
  }

  if (volgnr[AFB_POS_PRESS] == AFB_POS_GEEN && volgnr[AFB_POS_FOCUS] != AFB_POS_GEEN)
    volgnr[AFB_POS_PRESS] = volgnr[AFB_POS_FOCUS];
}

short
CAfbeelding::GeefNummer(short setNummer, int srtPlaatje)
{
  if (setNummer == AFB_POS_UIT)
    return AFB_POS_UIT;
  if (setNummer == AFB_POS_GEEN)
    setNummer = 0;
  if (srtPlaatje < 0 || srtPlaatje >= AFB_POS_AANTAL)
    srtPlaatje = 0;
  if (m_pos[srtPlaatje] < 0)
    return AFB_POS_UIT;
  return (short)((setNummer * m_aantalPS) + m_pos[srtPlaatje]);
}

bool 
CAfbeelding::SetKnopNummers(short volgnr[],short setNummer,int aantal)
{
  if (setNummer < 0)
    setNummer = 0;

  if (m_typeSet == AFB_SET_KNOPPEN)
  {
    setNummer = (short)(setNummer * m_aantalPS);
    for(int i = 0; i < aantal; i++)
    {
      if (volgnr[i] == AFB_POS_GEEN)
        volgnr[i] = (short)(m_pos[i] + setNummer);
    }
  }
  else if (m_typeSet == AFB_SET_SET )
    SetInterneKnopLogica(volgnr,setNummer,aantal);
  else
    SetInterneKnopLogica(volgnr,setNummer,aantal);

  return true;
}

bool 
CAfbeelding::SetSerieNummers(short volgnr[],short setNummer,int aantal)
{
  if (setNummer < 0)
    setNummer = 0;

  setNummer = (short)(setNummer * m_aantalPS);
  for(int i = 0; i < aantal; i++)
  {
    if (volgnr[i] == AFB_POS_GEEN && i < m_aantalPS)
      volgnr[i] = (short)(setNummer + i);
  }
  return true;
}

bool 
CAfbeelding::SetSetNummers(short volgnr[],short setNummer,int aantal)
{
  if (setNummer < 0)
    setNummer = 0;
  setNummer = (short)(setNummer * m_aantalPS);
  for(int i = 0; i < aantal; i++)
  {
    if (volgnr[i] != AFB_POS_GEEN)
      return true;
  }
  volgnr[0] = setNummer;
  return true;
}


bool 
CAfbeelding::SetInterneKnopLogica(short volgnr[],short /* setNummer */,int aantal)
{
  bool doorgaan = true;
  bool veranderd = false;
  while (doorgaan)
  {
    doorgaan = false;
    veranderd = false;
    for(int t = 0; t < aantal; t++)
    {
      if (volgnr[t] == AFB_POS_GEEN)
      {
        switch(t)
        {
        case AFB_POS_STAN:
          break;
        case AFB_POS_FOCUS:
          volgnr[AFB_POS_FOCUS] = volgnr[AFB_POS_STAN];
          break;
        case AFB_POS_PRESS:
          if(volgnr[AFB_POS_FOCUS] != AFB_POS_GEEN)
            volgnr[AFB_POS_PRESS] = volgnr[AFB_POS_FOCUS];
          else
            volgnr[AFB_POS_PRESS] = volgnr[AFB_POS_STAN];
          break;
        case AFB_POS_DIS:
          volgnr[AFB_POS_DIS] = volgnr[AFB_POS_STAN];
          break;
        case AFB_POS_EXTRA:
          volgnr[AFB_POS_EXTRA] = volgnr[AFB_POS_STAN];
          break;
        default:
          volgnr[t] = volgnr[AFB_POS_STAN];
        }
        veranderd =  veranderd || (volgnr[t] != AFB_POS_GEEN);
        doorgaan = doorgaan || (volgnr[t] == AFB_POS_GEEN);
      }
    }
    if (doorgaan && !veranderd)
      volgnr[AFB_POS_STAN] = 0;
  }
  return true;
}


short
CAfbeelding::GeefSetNummerVanNaam(LPCTSTR naam,LPCTSTR defExt)
{
  int upper = m_namen.GetUpperBound();
  for(int index = 0; index <= upper; index++)
  {
    if (m_namen[index])
    {
      LPCTSTR p1 = naam;
      LPCTSTR p2 = (LPCTSTR)(m_namen[index]);
      bool gelijk = true;

      while (gelijk && *p1 != _T('\0') && *p2 != _T('\0'))
      {
        gelijk = _totupper(*p1) == _totupper(*p2);
        p1++;
        p2++;
      }
      if (gelijk)
        gelijk = (*p1 == _T('\0') && *p2 == _T('\0'));

      if (!gelijk && ((*p1 == _T('\0') && _tcsicmp(p2,defExt) == 0)  || (*p2 == _T('\0') && _tcsicmp(p1,defExt) == 0)) )
        gelijk = true;
      //if (_stricmp((LPCSTR)(m_namen[index]),naam) == 0 )
      //  return index / m_aantalPS;
      if (gelijk)
        return (short)(index / m_aantalPS);
    }
  }
  return -2;
}

int
CAfbeelding::GeefPosNummerVanNaam(LPCTSTR naam)
{
  if (naam == NULL || _tcslen(naam) == 0)
    return AFB_POS_STAN;

  if (naam[0] == AFB_NULL_PREFIX || _tcsicmp(naam,_T("GEEN")) == 0)
    return AFB_POS_UIT;
  else if (_tcsicmp(naam,_T("STD")) == 0 || _tcsicmp(naam,_T("STANDAARD")) == 0)
    return AFB_POS_STAN;
  else if (_tcsicmp(naam,_T("FOC")) == 0 || _tcsicmp(naam,_T("FOCUS")) == 0)
    return AFB_POS_FOCUS;
  else if (_tcsicmp(naam,_T("PRE")) == 0 || _tcsicmp(naam,_T("INGEDRUKT")) == 0)
    return AFB_POS_PRESS;
  else if (_tcsicmp(naam,_T("DIS")) == 0 || _tcsicmp(naam,_T("INAKTIEF")) == 0)
    return AFB_POS_DIS;
  else if (_tcsicmp(naam,_T("EX"))  == 0 || _tcsicmp(naam,_T("EXTRA")) == 0)
    return AFB_POS_EXTRA;

  return AFB_POS_GEEN;
}


HICON 
CAfbeelding::GeefIcon(int /* nr */)
{
  return NULL;
}



void
CAfbeelding::OnReset()
{
}

//****************************************************************************
//                                                                           *
//                                CAfbeeldingBM                              *
//                                                                           *
//                                                                           *
//****************************************************************************
CAfbeeldingBM::CAfbeeldingBM(CAfbeeldingen& afbeeldingen)
              :CAfbeelding(afbeeldingen)
{
}

BOOL
CAfbeeldingBM::Create(UINT nBitmapID, UINT nExtraBitmapID)
{
  if (nExtraBitmapID != 0)
  {
    TRACE(_T("Extra bitmap-ID may not be filled"));
  }
  if (m_bitmap.LoadBitmap(nBitmapID))
  {
    CBitmapDC dcTemp(NULL,&m_bitmap);
    FinishCreate(dcTemp);
  }
  else
  {
    Reset();
  }
  return m_aantal;
}

bool
CAfbeeldingBM::FinishCreate(CDC& /*dcTemp*/)
{
  BITMAP     bm;
  m_bitmap.GetObject(sizeof(BITMAP), (LPTSTR)&bm);
  CRect     ptRect(0,0,bm.bmWidth,bm.bmHeight);

  m_cx = bm.bmWidth;
  m_cy = bm.bmHeight;
  m_aantal = 1;
  m_tColor = CLR_NONE;
  return true;
}

bool
CAfbeeldingBM::MaakBWBitmap(CDC& dcTemp)
{
  m_tColor = dcTemp.GetPixel(0,0);

  CDC dcBW;
  dcBW.CreateCompatibleDC(NULL);  // screen display compatible
  m_bitmapBW.CreateBitmap(m_cx, m_cy, 1, 1, NULL);    // Zwart/wit BITMAP
  CBitmap* pbmBWOld = dcBW.SelectObject(&m_bitmapBW);

  dcTemp.SetBkColor(m_tColor);    // De tramsparant kleur
  dcBW.BitBlt(0,0,m_cx,m_cy,&dcTemp, 0, 0, SRCCOPY);

  dcBW.SelectObject(pbmBWOld);
  return true;
}


bool
CAfbeeldingBM::Paint(CDC& pDC,CRect& rect,int /*nr*/,int drawParam)
{
  // Maskout bitmap 
  if (drawParam & AFB_PAINT_TRANS)
  {
    CBitmapDC  dcTemp(&pDC,m_cx,m_cy);
    CBitmapDC  dcMem(&pDC,m_cx,m_cy);
    int saveTemp = dcTemp.SaveDC();
    int saveMem = dcMem.SaveDC();

    CBitmapDC* dcPaintBDC = NULL;
    CDC* dcPaint = &pDC;
    if (!(drawParam & AFB_PAINT_BW) && (rect.Width() * rect.Height() > 2000))
    {
      dcPaintBDC = new CBitmapDC(&pDC,rect.Width(), rect.Height());
      dcPaintBDC->SetViewportOrg(-rect.left,-rect.top);
      dcPaintBDC->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(),&pDC,rect.left,rect.top,SRCCOPY);
      dcPaint = dcPaintBDC;
    }

    if (m_bitmapBW.m_hObject == NULL)
    {
      dcTemp.SelectBitmap(&m_bitmap);
      MaakBWBitmap(dcTemp);
    }

    COLORREF nColor = dcPaint->SetBkColor(RGB(255,255,255));
    if (drawParam & AFB_PAINT_FILL)
      dcPaint->FillSolidRect(rect,nColor);

    if (rect.Width() == m_cx && rect.Height() == m_cy) // geen stretch
    {
      dcTemp.SelectBitmap(&m_bitmapBW);
      dcMem.BitBlt(0, 0, m_cx, m_cy, &dcTemp, 0, 0, NOTSRCCOPY);
      dcPaint->BitBlt(rect.left, rect.top, m_cx, m_cy, &dcTemp, 0, 0, SRCAND);

      if (!(drawParam & AFB_PAINT_BW))
      {
        dcTemp.SelectObject(&m_bitmap);
        dcMem.BitBlt(0, 0, m_cx, m_cy, &dcTemp, 0, 0, SRCAND);
        dcPaint->BitBlt(rect.left, rect.top, m_cx, m_cy, &dcMem, 0, 0, SRCPAINT);
      }
    }
    else
    {
      dcTemp.SelectBitmap(&m_bitmapBW);
      dcMem.BitBlt(0, 0, m_cx, m_cy, &dcTemp, 0, 0, SRCCOPY);
      dcPaint->StretchBlt(rect.left, rect.top, rect.Width(), rect.Height(), &dcMem, 0, 0, m_cx, m_cy, SRCAND);
      if (!(drawParam & AFB_PAINT_BW))
      {
        dcMem.BitBlt(0, 0, m_cx, m_cy, &dcTemp, 0, 0, NOTSRCCOPY);
        dcTemp.SelectObject(&m_bitmap);
        dcMem.BitBlt(0, 0, m_cx, m_cy, &dcTemp, 0, 0, SRCAND);
        dcPaint->StretchBlt(rect.left, rect.top, rect.Width(), rect.Height(), &dcMem, 0, 0, m_cx, m_cy,SRCPAINT);
      }
    }
    dcPaint->SetBkColor(nColor);
    if (dcPaintBDC)
    {
      //dcPaintBDC->SetViewportOrg(0,0);
      pDC.BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), dcPaintBDC, rect.left,rect.top, SRCCOPY);
      delete dcPaintBDC;
    }
    dcTemp.RestoreDC(saveTemp);
    dcMem.RestoreDC(saveMem);
  }
  else // niet transparant
  {
    CBitmapDC dcTemp(&pDC,&m_bitmap);
    int saveTemp = dcTemp.SaveDC();
    if ((drawParam & AFB_PAINT_BW))
    {
      if (m_bitmapBW.m_hObject == NULL)
        MaakBWBitmap(dcTemp);
      dcTemp.SelectBitmap(&m_bitmapBW);
    }

    if (rect.Width() == m_cx && rect.Height() == m_cy) // geen stretch
    {
      pDC.BitBlt(rect.left, rect.top, m_cx, m_cy, &dcTemp, 0, 0, SRCCOPY);
    }
    else
    {
      //StretchDIBits(pDC->m_hDC,rect.left,rect.top,rect.Width(), rect.Height(),0,0,m_cx,m_cy,bits,BITMAPINFO,DIB_RGB_COLORS,SRCCOPY)
      pDC.StretchBlt(rect.left, rect.top, rect.Width(), rect.Height(), &dcTemp, 0, 0, m_cx, m_cy, SRCCOPY);
    }
    dcTemp.RestoreDC(saveTemp);
  }

  return true;
}

//****************************************************************************
//                                                                           *
//                                CAfbeeldingDIB                             *
//                                                                           *
//                                                                           *
//****************************************************************************
CAfbeeldingDIB::CAfbeeldingDIB(CAfbeeldingen& afbeeldingen)
               :CAfbeeldingBM(afbeeldingen)
{
}

int
CAfbeeldingDIB::Create(TCHAR* file)
{
  CDIB dib;
  if (dib.Load((TCHAR *)file))
    return Create(dib);
  else
    Reset();

  return m_aantal;
}

int
CAfbeeldingDIB::Create(CDIB& dib)
{
  CDC *pDC = CWnd::GetDesktopWindow()->GetDC();
  m_bitmap.CreateCompatibleBitmap(pDC, dib.GetWidth(), dib.GetHeight()); // Compatible DC
  CWnd::GetDesktopWindow()->ReleaseDC(pDC);
  CBitmapDC dcTemp(NULL,&m_bitmap);
  dib.Draw(&dcTemp,0,0);
  FinishCreate(dcTemp);
  return m_aantal;
}


//****************************************************************************
//                                                                           *
//                                CAfbeeldingIM                              *
//                                                                           *
//                                                                           *
//****************************************************************************
CAfbeeldingIM::CAfbeeldingIM(CAfbeeldingen& afbeeldingen)
              :CAfbeelding(afbeeldingen)
{
}

CAfbeeldingIM::CAfbeeldingIM(CAfbeeldingen& afbeeldingen,UINT nBitmapID,int cx,int cy,UINT extraBitmapID)
              :CAfbeelding(afbeeldingen)
{
  Create(nBitmapID,cx,cy,extraBitmapID);
}

CAfbeeldingIM::~CAfbeeldingIM()
{
}

int
CAfbeeldingIM::Create(UINT nBitmapID, UINT extraBitmapId)
{
  return Create(nBitmapID,16,16, extraBitmapId);
}

int
CAfbeeldingIM::Create(UINT nBitmapID,int cx,int cy, UINT extraBitmapId)
{
  if (cx <= 0) cx = 16;

  CWindowDC winDC (CWnd::GetDesktopWindow());
  
  CBitmapDC dc(&winDC);
  dc.LoadBitmap(nBitmapID);
  BITMAP     bmi;
  dc.GeefBitmap()->GetObject(sizeof(BITMAP), (LPTSTR)&bmi);
  COLORREF bkColor = dc.GetPixel(0,0);
  dc.DeSelectBitmap();

  if (m_imageList.Create( nBitmapID, cx, 50, bkColor))
  {
    if (extraBitmapId)
    {
      CImageList extraList;
      if (extraList.Create(extraBitmapId, cx, 50, bkColor))
      {
        for (int n = 0; n < extraList.GetImageCount(); n++)
        {
          HICON h = extraList.ExtractIcon(n);
          if (m_imageList.Add(h) < 0)
          {
            TRACE(_T("Cannot copy image"));
          }
          DeleteObject(h);
        }
      }
    }

    m_cx = cx;
    m_cy = cy;
    m_aantal = m_imageList.GetImageCount();
    if (m_cy == 0)
    {
      IMAGEINFO pImageInfo;
      if (m_imageList.GetImageInfo(0,&pImageInfo))
        m_cy = pImageInfo.rcImage.bottom - pImageInfo.rcImage.top;
      else
        m_cy = m_cx;

    }
  }
  else
    Reset();

  return m_aantal;
}

int
CAfbeeldingIM::Create(TCHAR* file,int cx)
{
  CDIB dib;
  if (dib.Load((TCHAR *)file))
    return Create(file,dib,cx);
  else
    Reset();
  return m_aantal;
}

int
CAfbeeldingIM::Create(TCHAR* file,CDIB& dib,int cx)
{
  if (cx <= 0)
    cx = dib.GetHeight();

  if (cx > 0)
  {
    HIMAGELIST im = ImageList_LoadImage(AfxGetInstanceHandle(), 
                                       (LPCTSTR)file, 
                                       cx, 
                                       10,
                                       ::GetSysColor(COLOR_3DFACE),
                                       IMAGE_BITMAP,
                                       LR_LOADFROMFILE|LR_LOADTRANSPARENT|LR_LOADMAP3DCOLORS);
    if (im)
    {
      m_imageList.Attach(im);
      m_cx = cx;
      m_cy = dib.GetHeight();
      m_aantal = m_imageList.GetImageCount();
    }
    else
    {
      Reset();
    }
  }
  else
  {
    Reset();
  }

  return m_aantal;
}

int  
CAfbeeldingIM::LoadDir(LPCTSTR p_naam)
{
  CString zoekBestand(p_naam);
  TCHAR drive[_MAX_DRIVE];   
  TCHAR dir[_MAX_DIR];   
  TCHAR fname[_MAX_FNAME];
  TCHAR ffname[_MAX_FNAME];
  TCHAR ext[_MAX_EXT];
  CFileFind ff;

  int cx = 32;
  int cy = 32;
  if (!m_imageList.m_hImageList)
  {
    HIMAGELIST im = ImageList_Create( cx, cy,  ILC_COLOR32|ILC_MASK, 50,10);
    m_imageList.Attach(im);

    m_cx = cx;
    m_cy = cy;
    m_aantalPS = 1;
  }

  BOOL bWorking = ff.FindFile(zoekBestand);
  CDC *pDC = CWnd::GetDesktopWindow()->GetDC();
  CBitmapDC dcDis(pDC,cx, cy);
  CWnd::GetDesktopWindow()->ReleaseDC(pDC);

  while(bWorking)
  {
    CString naam;

    bWorking = ff.FindNextFile();
    
    CString gfile = ff.GetFilePath();
    _tsplitpath_s( gfile, drive,dir,fname,ext);
    _tcscpy_s(ffname,_MAX_FNAME,fname);
    _tcscat_s(ffname,_MAX_FNAME,ext);

    for(TCHAR* p = ffname; *p != _T('\0'); *p = (TCHAR)_totlower(*p) , p++);
  

    if (_tcsicmp(ext,_T(".ico")) == 0)
    {
      Cicon ico;
      if (ico.LoadFromFile((LPCTSTR)gfile,cx,cy))
        VoegIconToe(ico.GeefHICON(),(LPCTSTR)fname);
    }
    else if (_tcsicmp(ext,_T(".BMP")) == 0)
    {
      CDIB dib;
      if (dib.Load((TCHAR*)gfile.GetString()))
      {
        int aantal = max(1,(dib.GetWidth() / cx));
        int num = 0;
     
        while (num < aantal)
        {
          dib.Draw(&dcDis, 0,0,cx,cy,   cx*num,0,min(cx,(dib.GetWidth()-cx*num)),min(cy,dib.GetHeight()));
          COLORREF bk = dcDis.GetPixel(0,0);
          dcDis.DeSelectBitmap();
          VoegBitmapToe(dcDis.GeefBitmap(),ffname,bk);
          dcDis.SelectBitmap(dcDis.GeefBitmap());
          num++;
        }
      }
    }
  }
  return -1;
}


int  
CAfbeeldingIM::VoegIconToe(HICON icon,LPCTSTR naam)
{
  if (!m_imageList.m_hImageList)
  {
    HIMAGELIST im = ImageList_Create( 32, 32,  ILC_COLOR32|ILC_MASK, 50,10);
    m_imageList.Attach(im);

    m_cx = 32;
    m_cy = 32;
    m_aantalPS = 1;
  }
  int imID = m_imageList.Add(icon);
  if (imID >= 0)
  {
    m_namen.SetAtGrow(imID, naam);
    m_aantal = m_imageList.GetImageCount( );
  }
  return imID;
}

HICON 
CAfbeeldingIM::GeefIcon(int nr)
{
  if (m_imageList.m_hImageList)
    return m_imageList.ExtractIcon(nr);
  return NULL;
}



int  
CAfbeeldingIM::VoegBitmapToe(CBitmap* bitmap,LPCTSTR naam,COLORREF bk)
{
  if (!m_imageList.m_hImageList)
  {
    HIMAGELIST im = ImageList_Create( 32, 32,  ILC_COLOR32|ILC_MASK, 50,10);
    m_imageList.Attach(im);

    m_cx = 32;
    m_cy = 32;
    m_aantalPS = 1;
  }
  int imID = -1;
  imID = m_imageList.Add(bitmap,bk);
  if (imID >= 0)
  {
    m_namen.SetAtGrow(imID, naam);
    m_aantal = m_imageList.GetImageCount( );
  }
  return imID;
}



bool
CAfbeeldingIM::Paint(CDC& pDC,CRect& rect,int nr,int drawParam)
{
  UINT nStyle = ILD_TRANSPARENT;
  pDC.SetTextColor(RGB(0,0,0));

  if (rect.Width() == m_cx && rect.Height() == m_cy) // geen stretch
  {
    if (drawParam & AFB_PAINT_FILL)
    {
      pDC.FillSolidRect(rect,pDC.GetBkColor());
    }

    if (drawParam & AFB_PAINT_DISABLED)
    {
      DrawDisabled(pDC,nr,CPoint(rect.left,rect.top));
    }
    else
    {
      m_imageList.DrawEx(&pDC,nr,CPoint(rect.left,rect.top),rect.Size(),pDC.GetBkColor(),pDC.GetTextColor(),nStyle);
    }

  }
  else
  {
    CBitmapDC dcMem(&pDC,m_cx,m_cy);
    // Stretch Geen SAVESTRETCH is sneller maar incorrect indien er niet een kleur op
    // de achtergrond staat. Bij fill is dit altijd het geval
    if (!(drawParam & AFB_PAINT_SAVESTRETCH) || drawParam & AFB_PAINT_FILL) 
    {
      if (drawParam & AFB_PAINT_FILL)
      {
        if (drawParam & AFB_PAINT_DISABLED)
        {
          DrawState(dcMem,nr,pDC.GetBkColor());
        }
        else
        {
          dcMem.FillSolidRect(0, 0, m_cx, m_cy,pDC.GetBkColor());
          m_imageList.DrawEx(&dcMem,nr,CPoint(0,0),CSize(m_cx,m_cy),dcMem.GetBkColor(),dcMem.GetTextColor(),nStyle);
        }
      }
      else
      {
        dcMem.BitBlt(0, 0, m_cx, m_cy, &pDC, rect.left, rect.top, SRCCOPY);
        if (drawParam & AFB_PAINT_DISABLED)
          DrawDisabled(dcMem,nr,CPoint(0,0));
        else
          m_imageList.DrawEx(&dcMem,nr,CPoint(0,0),CSize(m_cx,m_cy),dcMem.GetBkColor(),dcMem.GetTextColor(),nStyle);
      }

      pDC.StretchBlt(rect.left, rect.top, rect.Width(), rect.Height(), &dcMem, 0, 0, m_cx, m_cy, SRCCOPY);
    }
    else // Stretch vooraf
    {
      dcMem.FillSolidRect(0,0,rect.Width(), rect.Height(),RGB(255,255,255));

      if (drawParam & AFB_PAINT_DISABLED)
      {
        DrawDisabled(dcMem,nr,CPoint(0,0),true);
        pDC.StretchBlt(rect.left, rect.top, rect.Width(), rect.Height(), &dcMem, 0, 0, m_cx, m_cy, SRCAND);

        dcMem.FillSolidRect(0,0,rect.Width(), rect.Height(),RGB(0,0,0));
        DrawDisabled(dcMem,nr,CPoint(0,0));
        pDC.StretchBlt(rect.left, rect.top, rect.Width(), rect.Height(), &dcMem, 0, 0, m_cx, m_cy, SRCPAINT);
      }
      else
      {
        m_imageList.DrawEx(&dcMem,nr,CPoint(0,0),CSize(m_cx,m_cy),dcMem.GetBkColor(),dcMem.GetTextColor(),nStyle & ILD_MASK);
        pDC.StretchBlt(rect.left, rect.top, rect.Width(), rect.Height(), &dcMem, 0, 0, m_cx, m_cy, SRCAND);

        dcMem.FillSolidRect(0,0,rect.Width(), rect.Height(),0);
        m_imageList.DrawEx(&dcMem,nr,CPoint(0,0),CSize(m_cx,m_cy),dcMem.GetBkColor(),dcMem.GetTextColor(),nStyle);
        pDC.StretchBlt(rect.left, rect.top, rect.Width(), rect.Height(), &dcMem, 0, 0, m_cx, m_cy, SRCPAINT);
      }
    }
  }
  return TRUE;
}

bool 
CAfbeeldingIM::DrawState(CBitmapDC& dc,int index, COLORREF bkColor)
{
  CBitmapDC dcTemp(&dc,m_cx, m_cy);

  dc.FillSolidRect(0,0,m_cx,m_cy,bkColor);
  dcTemp.FillSolidRect(0,0,m_cx,m_cy,RGB(255,255,255));
  m_imageList.DrawEx(&dcTemp,index,CPoint(0,0),CSize(m_cx,m_cy),dcTemp.GetBkColor(),dcTemp.GetTextColor(),ILD_NORMAL);

  dcTemp.DeSelectBitmap();
  dc.DrawState(CPoint(0, 0),CSize(m_cx, m_cy),dcTemp.GeefBitmap(),DSS_DISABLED);

  return true;
}

bool 
CAfbeeldingIM::DrawDisabled(CDC& dc,int index,CPoint& pos, bool mask)
{
  CBitmapDC dcTemp(&dc,m_cx, m_cy);
  CBitmapDC dcBW(&dc,m_cx, m_cy,1,1);

  DrawState(dcTemp,index);
  dcBW.SetBkColor(0);
  dcBW.SetTextColor(RGB(255,255,255));
  dcBW.BitBlt(0,0,m_cx,m_cy,&dcTemp,0,0,SRCCOPY);

  COLORREF color = dc.SetBkColor(RGB(255,255,255));
  dc.BitBlt(pos.x,pos.y,m_cx,m_cy,&dcBW,0,0,SRCAND);
  dc.SetBkColor(color);

  if (!mask)
    dc.BitBlt(pos.x,pos.y,m_cx,m_cy,&dcTemp,0,0,SRCPAINT);
  return false;
}



//****************************************************************************
//                                                                           *
//                                CAfbeeldingEntry                           *
//                 Informatie klasse mbt te/geladen afbeeldingen             *
//                                                                           *
//****************************************************************************
UINT CAfbeeldingEntry::m_volgendeEntry = 1;

CAfbeeldingEntry::CAfbeeldingEntry(UINT resourceID,LPCTSTR /* naam */,LPCTSTR type,int cx,int sets, UINT extraResourceID)
:m_ID(m_volgendeEntry++),
 m_afbeelding(0),
 m_resourceID(resourceID),
 m_extraResourceID(extraResourceID),
 m_cx(cx),
 m_sets(sets),
 m_fileLoaded(_T("")),
 m_info(AFB_INFO_RESOURCE | AFB_INFO_FILE),
 m_status(0)
{
  _tcsncpy_s(m_type,AFB_POS_AANTAL,type,AFB_POS_AANTAL);
}

CAfbeeldingEntry::CAfbeeldingEntry(LPCTSTR /* naam */,LPCTSTR type,bool lijst,int cx,int sets)
:m_ID(m_volgendeEntry++),
 m_afbeelding(0),
 m_resourceID(0),
 m_extraResourceID(0),
 m_cx(cx),
 m_sets(sets),
 m_fileLoaded(_T("")),
 m_info(AFB_INFO_FILE | (lijst?AFB_INFO_LIJST:0)),
 m_status(0)
{
  _tcsncpy_s(m_type,AFB_POS_AANTAL,type?type:_T(""),AFB_POS_AANTAL);
}

CAfbeeldingEntry::CAfbeeldingEntry(CAfbeeldingEntry& entry)
:m_ID(m_volgendeEntry++),
 m_afbeelding(0),
 m_resourceID(entry.m_resourceID),
 m_extraResourceID(entry.m_extraResourceID),
 m_cx(entry.m_cx),
 m_sets(entry.m_sets),
 m_fileLoaded(entry.m_fileLoaded),
 m_info(entry.m_info),
 m_status(entry.m_status)
{
  _tcsncpy_s(m_type,AFB_POS_AANTAL,entry.m_type,AFB_POS_AANTAL);
}

CAfbeeldingEntry::~CAfbeeldingEntry()
{
}

Ref<CAfbeelding>
CAfbeeldingEntry::SetAfbeelding(Ref<CAfbeelding> afbeelding)
{
  Ref<CAfbeelding> oudeAfbeelding = m_afbeelding;
  m_afbeelding = afbeelding;
  if (!m_afbeelding)
  {
    m_fileLoaded = _T("");
    m_status = 0;
    m_info = 0;
  }
  return oudeAfbeelding;
}



CAfbeeldingExtensie::CAfbeeldingExtensie(LPCTSTR ext,LPCTSTR type,USHORT cx)
                    :m_ext(ext),
                     m_type(type),
                     m_cx(cx)
{
}

CAfbeeldingExtensie::~CAfbeeldingExtensie()
{
}


CAfbeeldingZoekpad::CAfbeeldingZoekpad(LPCTSTR pad,USHORT type)
                   :m_pad(pad),
                    m_type(type)
{
}

CAfbeeldingZoekpad::~CAfbeeldingZoekpad()
{
}


//****************************************************************************
//                                                                           *
//                                CAfbeeldingen                              *
//                 Dit is de container class voor Afbeeldingen               *
//                                                                           *
//****************************************************************************
CAfbeeldingen::CAfbeeldingen()
              :m_initGedaan(false)
{
}

CAfbeeldingen::~CAfbeeldingen()
{
  Reset();
  
  m_Standaards.clear();
  m_AfbeeldingenEntries.clear();
  m_Infos.clear();
  m_AfbeeldingenExtensies.clear();
  m_AfbeeldingenPaden.clear();
}

void
CAfbeeldingen::Init()
{
  if (m_initGedaan)
    return;
  m_initGedaan = true;

  Ref<CAfbeeldingEntry> entry = LoadDir(AFB_ICON_LIB,true);
  if (entry)
  {
    Ref<CAfbeeldingIM> afbeeldingIM (dynamic_cast<CAfbeeldingIM*>(entry->m_afbeelding.GeefPointer()));
    if (afbeeldingIM)
    {
      afbeeldingIM->SetType(_T("X"));
    }
  }
}

void
CAfbeeldingen::Reset()
{
}


Ref<CAfbeeldingEntry>
CAfbeeldingen::MaakResourceEntry(UINT resourceID,LPCTSTR naam,LPCTSTR type, int cx, int sets, UINT extraResourceID)
{
  CString entryNaam(naam);
  entryNaam.MakeUpper();

  Ref<CAfbeeldingEntry> entry (new CAfbeeldingEntry(resourceID,entryNaam,type,cx,sets,extraResourceID));
  m_AfbeeldingenEntries.insert(make_pair(entryNaam,entry));
  return entry;
}

Ref<CAfbeeldingEntry>
CAfbeeldingen::MaakNaamEntry(LPCTSTR naam,LPCTSTR type,bool lijst,int cx,int sets)
{
  CString entryNaam(naam);
  entryNaam.MakeUpper();

  Ref<CAfbeeldingEntry> entry (new CAfbeeldingEntry(entryNaam,type,lijst,cx,sets));
  m_AfbeeldingenEntries.insert(make_pair(entryNaam,entry));
  if (lijst)
  {
    entry->m_info |= AFB_INFO_LIB;
  }
  return entry;
}


Ref<CAfbeeldingInfo> 
CAfbeeldingen::GetImageInfo(LPCTSTR str,int logica)
{
  if (!this)
  {
    TRACE(_T("ImageInfo is NULL"));
  }
  int len = _tcslen(str);
  if (!str || len == 0)
  {
    return Ref<CAfbeeldingInfo>();
  }

  static TCHAR buffer[512];
  TCHAR* p = buffer;
  TCHAR* eind;

  // Omzetten naar hoofdletters en eventueel een setlogica eruit halen
  while(*str != _T('\0'))
  {
    if (*str == _T('{') && *(str+1) != _T('\0') &&*(str+2) == _T('}'))
    {
      logica = *(str+1) - _T('0');
      break;
    }
    *p++ = (TCHAR)_totupper(*str++);
  }
  *p = _T('\0');
  eind = p;


  if(buffer[0] == AFB_STD_PREFIX)
  {
    return GeefStandaard(&buffer[1]);
  }
  if(buffer[0] == AFB_NAAM_PREFIX)
  {
    // Niet geimplementeerd.
    return Ref<CAfbeeldingInfo>();
  }


  *p++ = _T('{');
  *p++ = (TCHAR)(logica + _T('0'));
  *p++ = _T('}');
  *p   = _T('\0');
  Ref<CAfbeeldingInfo> info;
  InfosMap::iterator it = m_Infos.find(buffer);
  if(it != m_Infos.end())
  {
    info = it->second;
    return info;
  }

  info = Ref<CAfbeeldingInfo>(new CAfbeeldingInfo(*this));
  //m_Infos[buffer] = info;
  m_Infos.insert(std::make_pair(buffer,info));

  *eind = _T('\0');
  MaakAfbeeldingInfo(buffer,info,logica);
  return info;
}

bool
CAfbeeldingen::MaakAfbeeldingInfo(LPCTSTR str,Ref<CAfbeeldingInfo> info,int logica)
{
  memset(&info->m_nr,255,sizeof(info->m_nr));
  info->m_rect.SetRect(0,0,0,0);
  
  if (str == NULL || *str == _T('\0'))
  {
    info->SetEntry( Ref<CAfbeeldingEntry>() );
    return false;
  }

  // Bepaal de naam van het plaatje of imagelist
  TCHAR * pos = (TCHAR*)_tcschr(str,_T('['));
  TCHAR buffer[101];
  TCHAR libNaam[101];
  if (pos)
  {
    _tcsncpy_s(buffer,100,str,max(pos-str,100));
    buffer[pos-str] = _T('\0');
  }
  else
  {
    _tcsncpy_s(buffer,100,str,100);
  }
  _tcsncpy_s(libNaam,100,buffer,100);

  // Bepaal de afbeelding
  int defSet = -1;
  Ref<CAfbeeldingEntry> entry = ZoekEntry(buffer,pos != NULL,&defSet);  // Indien pos dan [ aanwezig dus het liefst een list aanmaken
  Ref<CAfbeelding> Afbeelding;

  if (entry && entry->m_afbeelding)
  {
    Afbeelding = entry->m_afbeelding;
    info->SetEntry(entry);
    info->m_rect = Afbeelding->GeefRect();
    if (defSet >= 0)
    {
      info->m_nr[AFB_POS_STAN] = (short)defSet;
    }
  }
  else
  {
    info->SetEntry(entry);
    return false;
  }


  // Bepaal welke nummers erbij horen
  if(pos && defSet == -1)
  {
    bool indeel = false;
    bool vorigepunt = false;
    TCHAR instring = _T('\0');
    TCHAR* lpunt  = NULL;
    int index = 0;
    short setNummer = 0;
    short srtPlaatje = 0;
    TCHAR* bpos = buffer;
    TCHAR* cpos = pos+1;

    for(; ; cpos++)
    {
      
      // Uitzoeken
      if (_istspace(*cpos) && (!indeel || vorigepunt) && instring != _T('\0'))
        continue;
      else if (*cpos == _T('\'') || *cpos == _T('"'))
      {
        if (instring == _T('\0'))
        {
          instring = *cpos;
          continue;
        }
        else if (instring == *cpos)
        {
          instring = _T('\0');
          continue;
        }
        else
        {
          *bpos++ = *cpos;
          continue;
        }
      }
      else if(instring != _T('\0'))
      {
        *bpos++ = *cpos;
        continue;
      }
      else if (*cpos == _T(';') || *cpos == _T(',') || *cpos == _T(':') || *cpos == _T(']') || *cpos == _T('\0'))
      {
        vorigepunt = false;
        while(bpos > buffer && _istspace(*(bpos-1)))
          bpos--;
        *bpos = _T('\0');

        if (_tcslen(buffer) > 0)
        {
          if (lpunt)
            *lpunt++ = _T('\0');

          if (IsSetNummer(buffer))
            setNummer = (short)_ttoi(buffer);
          else
          {
            if (buffer[0] == _T('*') && buffer[1] == _T('\0'))
              setNummer = -2;
            else
            {
              setNummer = Afbeelding->GeefSetNummerVanNaam(buffer);
              // Indien uit de library ICONS dan eventueel opzoeken
              if (setNummer < 0 && _tcsicmp(libNaam,AFB_ICON_LIB) == 0 )
              {
                int iSet = -1;
                if (!HeeftExtensie(buffer,_T("ico")))
				{
					_tcscat_s(buffer,100,_T(".ICO"));
				}
                ZoekEntry(buffer,false,&iSet);
                if(iSet >= 0)
                  setNummer = (short)iSet;
              }
            }
          }
          if (_tcslen(buffer) > 0 && defSet < 0 && setNummer >= 0)
            defSet = setNummer;

          if (lpunt)
          {
            if (IsSetNummer(lpunt))
              srtPlaatje = (short)_ttoi(lpunt);
            else
              srtPlaatje = (short)Afbeelding->GeefPosNummerVanNaam(lpunt);
          }

          info->m_nr[index] = Afbeelding->GeefNummer(setNummer,srtPlaatje);
        }

        setNummer = srtPlaatje = 0;
        bpos = buffer;
        index++;
        indeel = false;
      }
      else
      {
        if (!indeel)
          indeel = true;

        if (*cpos == _T('#'))
        {
          // trailing whitespace
          while(bpos > buffer && _istspace(*(bpos-1)))
            bpos--;
          lpunt = bpos;
          vorigepunt = true;
        }
        else
          vorigepunt = false;

        *bpos++ = *cpos;
      }
      
      if (*cpos == _T('\0') || *cpos == _T(']') || index >= AFB_POS_AANTAL)
        break;
    }


    if (*cpos == _T(']'))
    {
      cpos++;
    }

    if (logica == -1 || logica == AFB_SET_KNOPPEN) // Knop Logica
    {
      Afbeelding->MaakStandaardKnopVolgnummers(info->m_nr,AFB_POS_AANTAL_INFO);
    }
  }


  if (logica == -1 || logica == AFB_SET_KNOPPEN) // Knop Logica
  {
    Afbeelding->SetKnopNummers(info->m_nr,(short)defSet,AFB_POS_AANTAL_INFO);
  }
  else if (logica == AFB_SET_SERIE) // Serie Logica
  {
    Afbeelding->SetSerieNummers(info->m_nr,(short)defSet,AFB_POS_AANTAL_INFO);
  }
  else 
  {
    Afbeelding->SetSetNummers(info->m_nr,(short)defSet,AFB_POS_AANTAL_INFO);
  }
  return info->m_entry;
}


bool
CAfbeeldingen::IsSetNummer(LPCTSTR str)
{
  if (!str)
    return true;

  LPCTSTR p = str;
  while(_istspace(*p))
    p++;

  while(_istdigit(*p))
    p++;

  while(_istspace(*p))
    p++;

  return *p == _T('\0');
}

// Indien volgnummer < 0 dan wordt aan de hand van de disabled state een afbeelding
// gekozen
CRect 
CAfbeeldingen::PaintBitmap(CDC& pDC,CRect& rect,Ref<CAfbeeldingInfo> info,int volgNr,int drawParam)
{
  if (info && info->m_entry && info->m_entry->m_afbeelding)
  {
    if (volgNr < 0 || volgNr >= AFB_POS_AANTAL)
    {
      volgNr = drawParam & AFB_PAINT_DISABLED?AFB_POS_DIS:AFB_POS_STAN;
      if (info->m_entry->m_afbeelding->HeeftDisabled())
        drawParam &= ~AFB_PAINT_DISABLED;
    }

    if (info->m_nr[volgNr] >= 0)
    {
      COLORREF tcolor = pDC.SetTextColor(0);
      info->m_entry->m_afbeelding->Paint(pDC, rect, info->m_nr[volgNr],drawParam);
      tcolor = pDC.SetTextColor(tcolor);
    }
  }
  else
  {
    COLORREF tcolor = pDC.SetTextColor(RGB(0,255,0));
    COLORREF bcolor = pDC.GetBkColor();

    pDC.FillSolidRect(rect,255);
    pDC.MoveTo(rect.TopLeft());
    pDC.LineTo(rect.BottomRight());

    pDC.SetTextColor(tcolor);
    pDC.SetBkColor(bcolor);
  }
  return rect;
}

HICON
CAfbeeldingen::GeefIcon(LPCTSTR info,LPCTSTR definfo)
{
  HICON icon = NULL;
  Ref<CAfbeeldingInfo> ainfo;
  if (info && _tcslen(info) > 0)
  {
    ainfo = GetImageInfo(info);
    if (ainfo && !ainfo->HasImage())
    {
      ainfo = Ref<CAfbeeldingInfo>();
    }
  }
  if (!ainfo && definfo && _tcslen(definfo) > 0)
  {
    ainfo = GetImageInfo(definfo);
  }

  if (ainfo)
  {
    icon = GeefIcon(ainfo,0);
  }
  return icon;
}


HICON
CAfbeeldingen::GeefIcon(Ref<CAfbeeldingInfo> info,int volgNr)
{
  if (info && info->m_entry && info->m_entry->m_afbeelding)
  {
    return info->m_entry->m_afbeelding->GeefIcon(info->m_nr[volgNr]);
  }
  return NULL;

}

//*****************************************************
// Zoeken en vinden van afbeeldingen eventueel extern *
//*****************************************************
Ref<CAfbeeldingEntry>
CAfbeeldingen::ZoekEntry(LPCTSTR naam,bool lijst,int* defSet)
{
  LPCTSTR entryNaam = naam;

  if (!naam || naam[0] == _T('\0'))
  {
    return Ref<CAfbeeldingEntry>();
  }

  Ref<CAfbeeldingEntry> entry;

  // Afhandeling opvragen van een ICON file
  if (HeeftExtensie(naam,_T(".ico")))
  {
    EntriesMap::iterator it = m_AfbeeldingenEntries.find(AFB_ICON_LIB);
    if(it != m_AfbeeldingenEntries.end())
    {
      entry = it->second;
      Ref<CAfbeeldingIM> afbeeldingIM (dynamic_cast<CAfbeeldingIM*>(entry->m_afbeelding.GeefPointer()));
      if (afbeeldingIM)
      {
        int set = afbeeldingIM->GeefSetNummerVanNaam(naam);
        if (set < 0)
        {
          Cicon ico;
          CString icoFile;

          if (IsVolledigPad(naam))
          {
            if (ico.LoadFromFile(naam,32,32))
            {
              afbeeldingIM->VoegIconToe(ico.GeefHICON(),naam);
              set = afbeeldingIM->GeefSetNummerVanNaam(naam);
            }
          }
          else
          {
            int maxsize = m_AfbeeldingenPaden.size();
            for(int index = 0; index < maxsize; ++index)
            {
              CAfbeeldingZoekpad* zpad = &m_AfbeeldingenPaden[index];
              icoFile  =  zpad->m_pad;
              icoFile += naam;
              if (ico.LoadFromFile(icoFile,32,32))
              {
                afbeeldingIM->VoegIconToe(ico.GeefHICON(),naam);
                set = afbeeldingIM->GeefSetNummerVanNaam(naam);
              }
              ++it;
            }
          }
        }
        if (set >= 0)
        {
          if (defSet)
            *defSet = set;
          return entry;
        }
      }
    }
    return Ref<CAfbeeldingEntry>();
  }

  EntriesMap::iterator it = m_AfbeeldingenEntries.find(entryNaam);
  if(it == m_AfbeeldingenEntries.end())
  {
    // Not found
    entry = MaakNaamEntry(naam,NULL,lijst);
  }
  entry = it->second;

  if (entry)
  {
    if(entry->m_status == 0)  // nog niet geladen
    {
      LoadEntry(entryNaam,entry);
      return entry;
    }
    if(!(entry->m_status & AFB_INFO_INVALID))
    {
      return entry;
    }
  }
  return Ref<CAfbeeldingEntry>();
}

UINT
CAfbeeldingen::LoadEntry(LPCTSTR naam,Ref<CAfbeeldingEntry> entry)
{

  if (entry->m_info & AFB_INFO_LIB)
  {
    LoadDir(naam,false);
    if (entry->m_status & AFB_INFO_LIBLOAD)
    {
      return entry->m_ID;
    }
  }


  if (entry->m_info & AFB_INFO_FILE && LoadEntryFromFile(naam,entry) > 0)  // Load from file
  {
    return entry->m_ID;
  }

  if (entry->m_resourceID && entry->m_info & AFB_INFO_RESOURCE)  // Load from resource
  {
    entry->m_status = 0;
    if (_tcslen(entry->m_type) > 0)  // Laad imagelist
    {
      Ref<CAfbeeldingIM> afbeelding (new CAfbeeldingIM(*this));
      if (afbeelding && afbeelding->Create(entry->m_resourceID,entry->m_cx,0,entry->m_extraResourceID))
      {
        entry->SetAfbeelding(afbeelding.Als<CAfbeelding>());
        afbeelding->SetType(entry->m_type);
        entry->m_status |= AFB_INFO_RESOURCE;
        return entry->m_ID;
      }
    }
    else  // Laad bitmap
    {
      Ref<CAfbeeldingBM> afbeelding (new CAfbeeldingBM(*this));
      if (afbeelding && afbeelding->Create(entry->m_resourceID, entry->m_extraResourceID))
      {
        entry->SetAfbeelding(afbeelding.Als<CAfbeelding>());
        entry->m_status |= AFB_INFO_RESOURCE;
        return entry->m_ID;
      }
    }
  }

  entry->m_status |= AFB_INFO_INVALID;
  return 0;
}

UINT
CAfbeeldingen::LoadEntryFromFile(LPCTSTR naam,Ref<CAfbeeldingEntry> entry)
{
  UINT ID = 0;
  TCHAR PathName[_MAX_PATH];
  TCHAR *FileName;

  CDIB* dib = FindDIBFile(naam, _MAX_PATH, PathName, &FileName);

  // Bepaal of de pathnaam al geladen is
  if (!dib)
    return 0;

  TCHAR drive[_MAX_DRIVE];   
  TCHAR dir[_MAX_DIR];   
  TCHAR fname[_MAX_FNAME];
  TCHAR ext[_MAX_EXT];
  _tsplitpath_s(FileName
	          ,drive, _MAX_DRIVE
			  ,dir,   _MAX_DIR
			  ,fname, _MAX_FNAME
			  ,ext,   _MAX_EXT);

  // We gaan uit van het type zoals opgegeven in de entry
  // De gebruikte extensie kan dit echter overschrijven
  double cxFactor = 0;   // Aantal keer breed als hoog
  LPCTSTR fileType = entry->m_type;
  if (_tcslen(ext) > 0)
  {
    CString deel1,deel2;
    Ref<CAfbeeldingExtensie> aext = SplitEnFindExtensie(ext,deel1,deel2);
    if (aext)
    {
      if (aext->m_type != _T(""))
        fileType = aext->m_type;
      if (aext->m_cx >= 0)
        cxFactor = aext->m_cx;
    }
  }
  // Indien minimum aantal gevraagd of geforceerd dan een imagelist aanmaken
  if ((entry->m_sets > 0 || entry->m_info & AFB_INFO_LIJST) && _tcslen(fileType) == 0)
    fileType = _T("S");

  if (_tcslen(fileType) > 0) // Probeer een Imagelist Te maken
  {
    Ref<CAfbeeldingIM> afbeelding (new CAfbeeldingIM(*this));
    if (cxFactor > 0)
    {
      entry->m_cx = (int)(dib->GetHeight() * (cxFactor/100));
    }
    if ( afbeelding && 
         afbeelding->Create(PathName,*dib,entry->m_cx) &&
         afbeelding->SetType(fileType) &&
        (afbeelding->Aantal()/afbeelding->AantalPerSet()) >= entry->m_sets) 
    {
      entry->SetAfbeelding(afbeelding.Als<CAfbeelding>());
      entry->m_status     |= AFB_INFO_FILE;
      entry->m_fileLoaded  = PathName;
      ID = entry->m_ID;
    }
  }
  else
  {
    Ref<CAfbeeldingDIB> afbeelding (new CAfbeeldingDIB(*this));
    if (afbeelding && afbeelding->Create(*dib))
    {        
      entry->SetAfbeelding(afbeelding.Als<CAfbeelding>());
      entry->m_status     |= AFB_INFO_FILE;
      entry->m_fileLoaded  = PathName;
      afbeelding->SetType(_T("X"));
      ID = entry->m_ID;
    }
  }
  delete dib;
  return ID;
}

CDIB*
CAfbeeldingen::FindDIBFile(LPCTSTR naam, DWORD lengte, LPTSTR PathName, LPTSTR* FileName)
{
  int naamLengte;
  if (naam == NULL || (naamLengte = _tcslen(naam)) == 0)
    return 0;

  bool cc = false;
  CDIB* dib = new CDIB;

  cc = LoadDIBFile(*dib,naam,lengte,PathName,FileName);

  if (!cc)
  {
    CString zoekBestand;

    TCHAR drive[_MAX_DRIVE];   
    TCHAR dir[_MAX_DIR];   
    TCHAR fname[_MAX_FNAME];
    TCHAR ext[_MAX_EXT];
    _tsplitpath_s(naam
		        ,drive, _MAX_DRIVE
				,dir,   _MAX_DIR
				,fname, _MAX_FNAME
				,ext,   _MAX_EXT );

    int maxsize = m_AfbeeldingenPaden.size();
    for(int index = 0; index < maxsize; ++index)
    {
      CAfbeeldingZoekpad* zpad = &m_AfbeeldingenPaden[index];
      zoekBestand =  zpad->m_pad;
      zoekBestand += naam;
      if (_tcslen(ext) == 0)
      {
        zoekBestand += _T(".*");
      }
      CFileFind ff;
      BOOL bWorking = ff.FindFile(zoekBestand);
      while(bWorking)
      {
        bWorking = ff.FindNextFile();
      
        CString gfile = ff.GetFilePath();
        _tsplitpath_s(gfile
			        ,drive, _MAX_DRIVE
					,dir,   _MAX_DIR
					,fname, _MAX_FNAME
					,ext,   _MAX_EXT);
        for(TCHAR* p = ext; *p != _T('\0'); *p = (TCHAR)_totupper(*p),++p);
        if (_tcslen(ext) > 0)
        {
          CString deel1,deel2;
          Ref<CAfbeeldingExtensie> aext = SplitEnFindExtensie(ext,deel1,deel2);
          if (aext)
          {
            cc = LoadDIBFile(*dib,gfile,lengte,PathName,FileName);
            if (cc)
            {
              bWorking = false;
            }
          }
        }
      }
    }
  }

  if (!cc) 
  {
    delete dib;
    dib = NULL;
  }
  return dib;
}


bool
CAfbeeldingen::LoadDIBFile(CDIB& dib,LPCTSTR naam, DWORD lengte, LPTSTR PathName, LPTSTR* FileName)
{
  CFile file;
  bool cc = false;
  if (file.Open(naam,CFile::modeRead | CFile::shareDenyWrite)) 
  {
    if (dib.Load(&file))
    {
      CString tmp = file.GetFilePath( );
      DWORD tmplengte = tmp.GetLength();
      if (tmplengte < lengte)
      {
        memcpy(PathName,(LPCTSTR)tmp,tmplengte+1);  // kopieer '\0' mee
        *FileName = PathName + (tmplengte - file.GetFileName().GetLength());
        cc = true;
      }
    }
  }
  return cc;
}


int
CAfbeeldingen::RegistreerExtensie(LPCTSTR ext,LPCTSTR type,USHORT cx)
{
  if (_tcslen(ext) == 0)
    return 0;

  CString nExt(ext);
  nExt.MakeUpper();
  nExt.TrimLeft();
  nExt.TrimRight();
  if (nExt[0] != _T('.'))
  {
    nExt = _T('.') + nExt;
  }
  Ref<CAfbeeldingExtensie> aext (new CAfbeeldingExtensie(nExt,type,cx));
  //m_AfbeeldingenExtensies[ext] = aext;
  m_AfbeeldingenExtensies.insert(std::make_pair(ext,aext));
  return 1;
}

int 
CAfbeeldingen::RegistreerZoekPad(const CString& pad)
{
  CAfbeeldingZoekpad zpad (pad,0);
  m_AfbeeldingenPaden.push_back(zpad);
  return m_AfbeeldingenPaden.size() - 1;
}

int 
CAfbeeldingen::RegistreerSystemZoekPad(const CString& pad)
{
  CAfbeeldingZoekpad zpad (pad,1);
  m_AfbeeldingenPaden.push_back(zpad);
  return m_AfbeeldingenPaden.size() - 1;
}


bool
CAfbeeldingen::IsVolledigPad(LPCTSTR pad)
{
  // c:\die\dir\file.pad
  if (!pad || _tcslen(pad) < 1)
    return false;

  if (pad[1] == _T(':'))
    return true;

  return false;
}

bool
CAfbeeldingen::HeeftExtensie(LPCTSTR pad,LPCTSTR extensie)
{
  if (!pad || !extensie)
    return false;

  int lenExt = _tcslen(extensie);
  int lenPad = _tcslen(pad);
  if (lenExt > lenPad)
    return false;

  return lenPad > lenExt && _tcsicmp(pad + lenPad -lenExt,extensie) == 0;
}

Ref<CAfbeeldingExtensie>
CAfbeeldingen::SplitEnFindExtensie(LPCTSTR ext,CString deel1,CString deel2)
{
  deel2 = _T("");
  deel1 = ext;
  deel1.MakeUpper();
  deel1.TrimLeft();
  deel1.TrimRight();
  if (deel1[0] != _T('.'))
  {
    deel1 = _T('.') + deel1;
  }
  Ref<CAfbeeldingExtensie> aext;
  ExtensiesMap::iterator it = m_AfbeeldingenExtensies.find(deel1);
  if(it != m_AfbeeldingenExtensies.end())
  {
    aext = it->second;
  }
  return aext;
}

bool
CAfbeeldingen::LaatsteFout()
{
    LPVOID lpMsgBuf;
    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                  NULL,GetLastError(),MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                  (LPTSTR) &lpMsgBuf,    0,    NULL );
    LocalFree( lpMsgBuf );
    return true;
}


// Standaards
bool
CAfbeeldingen::RegistreerStandaard(LPCTSTR pnaam,LPCTSTR soort,int logica)
{
  //m_Standaards[pnaam] = GetImageInfo(soort,logica);
  m_Standaards.insert(std::make_pair(pnaam,GetImageInfo(soort,logica)));
  return true;
}

Ref<CAfbeeldingInfo> 
CAfbeeldingen::GeefStandaard(LPCTSTR pnaam)
{
  if (!this)
  {
    TRACE(_T("GetStandard image is NULL"));
  }

  Ref<CAfbeeldingInfo> info;
  StandaardsMap::iterator it = m_Standaards.find(pnaam);
  if(it != m_Standaards.end())
  {
    info = it->second;
  }
  return info;
}

CString
CAfbeeldingen::GeefInfoSTR(Ref<CAfbeeldingInfo> info)
{
  InfosMap::iterator it = m_Infos.begin();
  while(it != m_Infos.end())
  {
    if(it->second == info)
    {
      return it->first;
    }
    ++it;
  }
  return _T("");
}


Ref<CAfbeeldingEntry>
CAfbeeldingen::LoadDir(LPCTSTR naam,bool altijdAanmaken)
{
  Ref<CAfbeeldingEntry> entry;
  EntriesMap::iterator it = m_AfbeeldingenEntries.find(naam);
  if(it != m_AfbeeldingenEntries.end())
  {
    entry = it->second;
  }
  CString dir;

  int maxsize = m_AfbeeldingenPaden.size();
  for(int index = 0; index < maxsize; ++index)
  {
    CAfbeeldingZoekpad* zpad = &m_AfbeeldingenPaden[index];

    CString pad = zpad->m_pad;
    int type =  zpad->m_type;
    if (type == 1)
    {
      UINT att;
      dir = pad;
      dir += _T("\\");
      dir += naam;
      att = GetFileAttributes(dir);

      if (att != 0xFFFFFFFF && (att & FILE_ATTRIBUTE_DIRECTORY))
      {
        dir += _T("\\*.*");
        break;
      }
      dir = _T("");
    }
  }  
  if (dir != _T("") || (!entry && altijdAanmaken))
  {
    if (!entry)
    {
      entry = Ref<CAfbeeldingEntry>(new CAfbeeldingEntry(naam,_T("S"),true,16,0));
      m_AfbeeldingenEntries.insert(make_pair(naam,entry));
      entry->m_fileLoaded = dir;
    }
    
    Ref<CAfbeelding> afbeelding = entry->m_afbeelding;
    if (!afbeelding)
    {
      afbeelding = Ref<CAfbeelding>(new CAfbeeldingIM(*this));
      entry->SetAfbeelding(afbeelding);

      entry->m_status |= AFB_INFO_LIBLOAD;
      entry->m_status |= AFB_INFO_FILE;
      entry->m_fileLoaded = dir;
    }

    if (dir != _T(""))
    {
      afbeelding->LoadDir(dir);
    }
    if (dir != _T("") || altijdAanmaken)
    {
      entry->m_status |= AFB_INFO_LIBLOAD;
      entry->m_status |= AFB_INFO_FILE;
    }
  }
  return entry;
}





// Painting etc
int  
CAfbeeldingen::PaintAfbeelding(LPCTSTR naam, CDC* pDC)
{
  if (!naam || _tcslen(naam) == 0)
    return 0;

  pDC->SetBkMode(TRANSPARENT);

  int tussen_afb_ruimte = 5;
  int tussen_set_ruimte = 20;
  int kantlijn = 10;

  CString meld;
  int xpos = 10;
  int ypos = 30;
  int perc = 100;

  CRect clientRect;
  pDC->GetWindow()->GetClientRect(clientRect);
  CSize tsize  = pDC->GetTextExtent(_T("00000"));

  Ref<CAfbeeldingEntry> entry;
  EntriesMap::iterator it = m_AfbeeldingenEntries.find(naam);
  if(it != m_AfbeeldingenEntries.end())
  {
    entry = it->second;
  }
  if(it == m_AfbeeldingenEntries.end() || !entry)
  {
    meld.Format(_T("Kan afbeeldingenbibliotheek '%s' niet vinden"), naam);
    pDC->TextOut(xpos,ypos,meld);
    return false;
  }

  if(entry->m_status == 0)  // nog niet geladen
  {
    LoadEntry(naam,entry);
  }

  meld.Format(_T("Afbeeldingenbibliotheek '%s'  %i%%"),naam,perc);
  pDC->TextOut(xpos, ypos, meld);
  xpos = kantlijn;
  ypos += tsize.cy;

  Ref<CAfbeelding> afb = entry->m_afbeelding;
  if (!afb)
  {
    if (entry->m_status & AFB_INFO_INVALID)
    {
      meld = _T("Kan de afbeelding niet laden");
    }
    else
    {
      meld = _T("(Nog) geen afbeelding geladen");
    }
    pDC->TextOut(xpos, ypos, meld);
    return false;
  }
  if (afb->m_namen.GetUpperBound() >= 0)
  {
    tsize  = pDC->GetTextExtent(_T("00000 Naam  van de file"));
  }

  meld = _T("Geladen uit: ");
  if (entry->m_fileLoaded == _T(""))
  {
    meld += _T("resource");
  }
  else
  {
    meld += _T(" bestand '");
    meld += entry->m_fileLoaded;
    meld += _T("'");
  }
  pDC->TextOut(xpos, ypos, meld);
  xpos = kantlijn;
  ypos += tsize.cy;


  meld.Format(_T("Aantal %i   Type: %s   Soort: %s   Aantal per set %i   Formaat: (%i,%i)"), afb->m_aantal, afb->m_type, afb->GeefTypeSetNaam(),afb->m_aantalPS,afb->m_cx,afb->m_cy);
  pDC->TextOut(xpos, ypos, meld);
  xpos = kantlijn;
  ypos += tsize.cy;


  if (afb->m_aantalPS > 1)
  {
    meld = _T("Setnummers: ");
    for (int i = 0 ; i < AFB_POS_AANTAL ; i++)
    {
      if (afb->m_pos[i] >= 0)
      {
        CString getal;
        getal.Format(_T("%i"),afb->m_pos[i]);
        meld += getal;
      }
      else
        meld += _T("*");
    }
    pDC->TextOut(xpos, ypos, meld);
    xpos = kantlijn;
    ypos += tsize.cy;
  }

  CSize asize( ((afb->m_cx  * perc)/100) + tussen_afb_ruimte , (afb->m_cy  * perc)/100 + tussen_afb_ruimte);

  COLORREF bkColor = pDC->GetBkColor();

  int drawParam = 0/*AFB_PAINT_TRANS | AFB_PAINT_SAVESTRETCH*/;
  int index = 0;
  int set = 0;
  int setTeller = 0;
  
  int breedte = max((int)((clientRect.Width() + tussen_set_ruimte - kantlijn) / ((asize.cx * afb->m_aantalPS) + tsize.cx + tussen_set_ruimte)),1);
  int colTeller = 0;

  ypos += tsize.cy + 10;
  xpos = kantlijn;

  if (afb->m_aantal > 0)
  {
    LPCTSTR afbNaam = _T("");
    if (afb->m_namen.GetUpperBound() >= index)
      afbNaam = (LPCTSTR)afb->m_namen[index];
    meld.Format(_T("%3.3i %s"),set,afbNaam);
    pDC->TextOut(xpos, ypos, meld);
    xpos += tsize.cx;
    set++;
  }

  CRect clipRect;
  pDC->GetClipBox( clipRect );
  int misteller = 0;
  while(index < afb->m_aantal)
  {
    pDC->SetBkColor(bkColor);

    if (setTeller >= afb->m_aantalPS)
    {
      colTeller++;
      if (colTeller >= breedte)
      {
        xpos = kantlijn;
        ypos += asize.cy;
        colTeller = 0;
      }
      else
        xpos += tussen_set_ruimte;

      LPCTSTR afbNaam = _T("");
      if (afb->m_namen.GetUpperBound() >= index)
        afbNaam = (LPCTSTR)afb->m_namen[index];
      meld.Format(_T("%3.3i %s"),set,afbNaam);
      pDC->TextOut(xpos, ypos, meld);
      xpos += tsize.cx;

      setTeller = 0;
      set++;
    }

    if (clipRect.PtInRect(CPoint(xpos,ypos)) || 
        clipRect.PtInRect(CPoint(xpos+((afb->m_cx * perc)/100),ypos+((afb->m_cy * perc)/100))))
    {
      CRect r(xpos,
              ypos,
              xpos + (afb->m_cx * perc)/100,
              ypos + (afb->m_cy * perc)/100);
      afb->Paint(*pDC, r, index,drawParam);
    }
    else
    {
      misteller++;
    }
    setTeller++;
    xpos += asize.cx;
    index++;
  }

  return ypos + (afb->m_cy * perc)/100;
}

int  
CAfbeeldingen::PaintAfbeeldingText(int& xPos, int& yPos, CDC* pDC, LPCTSTR text)
{
  CSize size = pDC->GetTextExtent(text);
  pDC->TextOut(xPos, yPos, text);
  return 0;
}

