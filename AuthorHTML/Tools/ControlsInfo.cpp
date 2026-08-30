// ControlsInfo.cpp
//
#include "StdAfx.h"
#include "resource.h"
#include "ControlsInfo.h"
#include "Images.h"
#include "AuthorHTML.h"

/*
#include <Basisklassen\instellingen.h>
#include <Basisklassen\VarLijst.h>
#include <AlgemeenBasis.h>
#include <basisklassen/PVector.h>
*/

commandInfo commandos[] =
{
  // ID                name              code      defKnop      ibsTeller,aktief,vorigeAktief,prompt
  // Menu en IBS
  IDOK,                "OK",             "ok",     "KNOP[5]",   0,0,0,NULL,
  IDCANCEL,            "Cancel",         "ca",     "KNOP[23]",  0,0,0,NULL,
  IDRETRY,             "Retry",          "re",     "KNOP[26]",  0,0,0,NULL,
  IDYES,               "Yes",            "ye",     "KNOP[5]",   0,0,0,NULL,
  IDNO,                "No",             "no",     "KNOP[8]",   0,0,0,NULL,
  IDAPPLY,             "Apply",          "ap",     "KNOP[31]",  0,0,0,NULL,
  IDIGNORE,            "Ignore",         "ig",     "KNOP[23]",  0,0,0,NULL,
  0,                   NULL,             NULL,     NULL,        0,0,0,NULL
};

//
// Omschrijving:   Constructor van klasse ControlsInfo
//
ControlsInfo::ControlsInfo() 
             :m_checkBoxPromptPositie(0)
             ,m_focusLabel(false)
             ,m_tabVerwKnop(1)
             ,m_knopLayout(BUTT_LAYOUT_LEFT)
{
  for (int i = 0;i < MAXCONTROLS;i++) 
  {
    m_lettertypes[i] = 0;
  }
  BepaalInfo();
}

//
// Omschrijving:   Destructor van klasse ControlsInfo
//
ControlsInfo::~ControlsInfo()
{
  MaakLeeg();
}

void
ControlsInfo::MaakLeeg()
{
  // Geef geheugen weer vrij voor lettertypes van alle controls
  for (int i = 0;i < MAXCONTROLS;i++) 
  {
    delete m_lettertypes[i];
    m_lettertypes[i] = 0;
  }
}

void
ControlsInfo::BepaalInfo()
{
  MaakLeeg();
  m_miniRect.SetRect(0,0,16,16);
  m_fullRect.SetRect(0,0,32,32);

  // Zet alle waarden op nul
  for (int i = 0;i < MAXCONTROLS;i++) 
  {
    m_lettertypes[i]= 0;
    m_hoogtes[i]= 0;
    m_breedtes[i]= 0;
    m_randhoogtes[i]= 0;
    m_randbreedtes[i]= 0;
    m_lettertypes[i]= 0;
  }
  // Maak dialoogvenster
  m_lf_default = MaakLOGFONTVanString("Tahoma");

  //  Haal de textmetrics van het font op
  CWindowDC dc(0);
  
  CFont* font = new CFont;
  font->CreateFontIndirect(&m_lf_default);

  HGDIOBJ oldFont = dc.SelectObject(font);
  dc.GetTextMetrics(&m_textmetric);
  m_textmetric.tmMaxCharWidth = dc.GetTextExtent("W").cx;
  dc.SelectObject(oldFont);

  // Want in GetInfo wordt er nog twee keer SM_CYFIXEDFRAME bij opgeteld.
  //m_enkeleRegelHoogte = m_dialog.GetDlgItem(IDC_COMBOBOX)->SendMessage(CB_GETITEMHEIGHT, (WPARAM)-1);
  m_enkeleRegelHoogte  = m_textmetric.tmHeight;

  delete font;

  m_checkBoxPromptPositie = 0; 
  m_focusLabel          = 0;
  m_tabVerwKnop         = 1;
}

WORD
ControlsInfo::ResourceIDFromNaam(LPCSTR naam)
{
  commandInfo* inf = commandos;
  while(inf->ID != 0)
  {
    if (_stricmp(inf->naam,naam) == 0)
    {
      return (WORD)inf->ID;
    }
    inf += 1;
  }
  return NULL;
}

long
ControlsInfo::GemiddeldeCharBreedte(CFont* lettertype)
{
  for (int i = 0;i < MAXCONTROLS;i++) 
  {
    if (m_lettertypes[i] == lettertype)
      return m_lettertypes[i]->m_gemBreed;
  }
  return 5;
}

long
ControlsInfo::CharHoogte(CFont* lettertype)
{
  for (int i = 0;i < MAXCONTROLS;i++) 
  {
    if (m_lettertypes[i] == lettertype)
      return m_lettertypes[i]->m_hoog;
  }
  return 5;
}



LOGFONT
ControlsInfo::MaakLOGFONTVanString(CString fontstring, CString deffont, int defsize, int defgewicht, int maxsize)
{
  LOGFONT lf;
  memset(&lf, 0, sizeof(lf));  

  int fontpuntgrootte = 0;
  CString font1,font2,font3;

  font1 = fontstring;
  int pos1 = fontstring.Find(';');
  if(pos1 >= 0)
  {
    font1 = fontstring.Left(pos1);
    font2 = fontstring.Mid(pos1 + 1);
    int pos2 = font2.Find(';');
    if(pos2 >= 0)
    {
      font3 = font2.Mid(pos2 + 1);
      font2 = font2.Left(pos2);
    }
  }
  strncpy_s(lf.lfFaceName,LF_FACESIZE,font1,LF_FACESIZE);
  fontpuntgrootte = atoi(font2);
  lf.lfWeight     = atoi(font3);

  if (strlen(lf.lfFaceName) == 0)
  {
    strncpy_s(lf.lfFaceName, LF_FACESIZE, deffont.IsEmpty() ? "Tahoma" : deffont, LF_FACESIZE);
  }
  if (!lf.lfWeight)
  {
    lf.lfWeight = defgewicht ? defgewicht : 200;
  }

  CWindowDC dc(0);  
  if(fontpuntgrootte == 0)
  {
    if (defsize)
    {
      fontpuntgrootte = defsize;
    }
    else
    {
      // Default nemen we de grootte van het systeemfont.
      NONCLIENTMETRICS ncm;
      ncm.cbSize = sizeof(ncm);
      if (SystemParametersInfo(SPI_GETNONCLIENTMETRICS,sizeof(ncm),&ncm,0))
      {
        lf.lfHeight = ncm.lfMessageFont.lfHeight;
      }
    }
  }
  else
  {
    //  Bereken standaard fontgrootte
    lf.lfHeight = -MulDiv(fontpuntgrootte, dc.GetDeviceCaps(LOGPIXELSY), 72);
  }

  int uiteindelijke_puntgrootte = -MulDiv(lf.lfHeight, 72, dc.GetDeviceCaps(LOGPIXELSY));
  if (uiteindelijke_puntgrootte > maxsize)
  {
    lf.lfHeight = -MulDiv(maxsize, dc.GetDeviceCaps(LOGPIXELSY), 72);
  }
  return lf;
}


// Teken functies
int
ControlsInfo::PaintRectKader(CDC& dc,CRect& rect,int soort,COLORREF /*kleur*/)
{
  if(!soort)
    return 0;

  if(soort == 1)
  {
    dc.DrawEdge(rect,BDR_RAISEDOUTER,BF_RECT);
    rect.DeflateRect(4,4);
    dc.DrawEdge(rect,BDR_SUNKENINNER,BF_RECT);
  }
  else if(soort == 2) // kader
    dc.Rectangle(rect);
  else if(soort == 3) // rond kader
    dc.RoundRect(rect,CPoint(10,10));
  else if(soort == 4) // edge
    dc.DrawEdge(rect,EDGE_ETCHED,BF_RECT);
  else if(soort == 5) // omhoog
    dc.DrawEdge(rect,EDGE_RAISED,BF_RECT);
  else if(soort == 6) // omlaag
    dc.DrawEdge(rect,EDGE_SUNKEN,BF_RECT);
  else if(soort == 7) // bump
    dc.DrawEdge(rect,EDGE_BUMP,BF_RECT);

  return soort;
}

int
ControlsInfo::PaintRectEffect(CDC& dc,CRect& rect,COLORREF van,COLORREF naar,int type)
{
  COLORREF redBkColor = dc.GetBkColor();
  CRect inRect(rect);

  int stap = 1;

  int v_rood = GetRValue(van);
  int v_groen = GetGValue(van);
  int v_blauw = GetBValue(van);

  int n_rood = GetRValue(naar);
  int n_groen = GetGValue(naar);
  int n_blauw = GetBValue(naar);

  int d_rood = v_rood - n_rood;
  int d_groen = v_groen - n_groen;
  int d_blauw = v_blauw - n_blauw;

  int decrX = 1,decrY = 1,ctrl;

  bool reverse = true;
  bool doorgaan = true;
  double richting = 0;
  switch(type)
  {
  case 1:
    decrX = (inRect.Width() /255);
    if (decrX <= 0) decrX = 1;
    ctrl = inRect.Width()  / (decrX);
    inRect.right = inRect.left + decrX;
    break;
  case 2:
    decrY = (inRect.Height() /255);
    if (decrY <= 0) decrY = 1;
    ctrl = inRect.Height()  / (decrY);
    inRect.bottom = inRect.top + decrY;
    break;
  case 3:
    decrY = (inRect.Height() /255);
    if (decrY <= 0) decrY = 1;
    ctrl = inRect.Height()  / (2*decrY);
    inRect.bottom = inRect.top + decrY;
    break;
  case 4:
    decrX = (inRect.Width() /255);
    if (decrX <= 0) decrX = 1;
    ctrl = inRect.Width()  / (2*decrX);
    inRect.right = inRect.left + decrX;
    break;
  default:
    decrX = (inRect.Width() /255) +1;
    decrY = (inRect.Height() /255) + 1;
    ctrl = max(inRect.Width()  / (2*decrX) , inRect.Height()  / (2*decrY));
    //decrX = inRect.Width();
    //decrY = inRect.Height();
    break;
  }
  while(doorgaan)
  {

#undef min

    double deel;
    deel = richting - ((double)std::min(stap,ctrl)/ctrl);
    if (deel < 0 )
      deel *= -1;


    //CBrush br(RGB(v_rood  - d_rood*deel,
    //              v_groen - d_groen*deel,
    //              v_blauw  - d_blauw*deel));

    //CRgn rg;
    //rg.CreateRectRgnIndirect(inRect);
    //dc.FrameRgn( &rg, &br, decrX , decrY );
    dc.FillSolidRect(inRect,RGB(v_rood  - d_rood*deel,v_groen - d_groen*deel,v_blauw  - d_blauw*deel));
    doorgaan = stap <= (ctrl);
    switch(type)
    {
    case 1:
      inRect.right += decrX;
      inRect.left += decrX;
      break;
    case 2:
      inRect.top += decrY;
      inRect.bottom += decrY;
      break;
    case 3:
      inRect.top += decrY;
      inRect.bottom += decrY;
      if (stap >= ctrl && reverse)
      {
        stap = 0;
        reverse = false;
        richting = 1;
      }
      break;
    case 4:
      inRect.right += decrX;
      inRect.left += decrX;
      if (stap >= ctrl && reverse)
      {
        stap = 0;
        reverse = false;
        richting = 1;
      }
      break;
    default:
      inRect.top = rect.top + (decrY * stap);
      inRect.bottom = rect.bottom - (decrY * stap);
      inRect.left = rect.left + (decrX * stap);
      inRect.right = rect.right - (decrX * stap);
      doorgaan = inRect.Height() >= 0 && inRect.Width() >= 0;
      if(!doorgaan)
        doorgaan = false;
      break;
    }

    stap++;
  }
  dc.SetBkColor(redBkColor);
  return 1;
}

void
ControlsInfo::TekenVorm(CDC& dc,CRect& rect,COLORREF kleur,int vorm,int rectAdjust,int penDikte)
{
  rect.InflateRect(rectAdjust,rectAdjust);

  CPen pen(PS_SOLID,penDikte,kleur);
  CPen* oldPen = dc.SelectObject(&pen);
  switch(vorm)
  {
  case 1:
    dc.Ellipse( rect );
    break;
  case 2:
    {
    int r = 10;
    dc.RoundRect( rect ,CPoint(r,r));
    break;
    }
  case 3:
    {
      int r = max(rect.Width(),rect.Height())/2;
      dc.RoundRect( rect ,CPoint(r,r));
      break;
    }
  default:
    dc.Rectangle( rect );
    break;
  }
  dc.SelectObject(oldPen);
} 


void
ControlsInfo::MaakSchaduwRgn(CDC& dc,CRect& pRect,int type)
{
  CRect rect(pRect);
  int corry,corrx = 12;
  switch(type)
  {
  default:
  case 0:
    corry = corrx = 2;
    break;
  case 1:
    corrx = 2;
    corry = (rect.Height()/10)*3;
    break;
  case 2:
    corry = corrx = 5;
    break;
  case 3:
    corry = corrx = 12;
    break;
  }
  dc.MoveTo(rect.left,rect.bottom);
  dc.BeginPath();
  dc.LineTo(rect.right,rect.bottom);
  dc.LineTo(rect.right,rect.top);
  dc.LineTo(rect.right-corrx,rect.top);
  dc.LineTo(rect.left,rect.bottom-corry);
  dc.CloseFigure();
  dc.EndPath();
  dc.SelectClipPath(RGN_COPY);
}



void
ControlsInfo::PlaatsRectInRect(const CRect& mRect,CRect& rect,int pos,int marges)
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
    if (pos > 30)
      pos -= 30;
    if (pos > 20)
      pos -= 20;
  }
  rect.OffsetRect(inRect.left - rect.left ,inRect.top - rect.top);

  switch(pos) // textpositie
  {
  default:
  case 0:
    break;
  case 1:
    rect.OffsetRect(restx/2,0);
    break;
  case 2:
    rect.OffsetRect(restx,0);
    break;
  case 3:
    rect.OffsetRect(0,resty/2);
    break;
  case 4:
    rect.OffsetRect(restx/2,resty/2);
    break;
  case 5:
    rect.OffsetRect(restx,resty/2);
    break;
  case 6:
    rect.OffsetRect(0,resty);
    break;
  case 7:
    rect.OffsetRect(restx/2,resty);
    break;
  case 8:
    rect.OffsetRect(restx,resty);
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
  //rect.IntersectRect(&rect,&inRect);
}


//************************************************************************************
//************************************************************************************
// Command interface 
//************************************************************************************
//************************************************************************************

commandInfo*
ControlsInfo::ZoekCommandInfo(UINT ID)
{
  commandInfo* inf = commandos;
  while(inf->ID != 0)
  {
    if (inf->ID == ID)
      return inf;

    inf += 1;
  }
  return NULL;
}

commandInfo*
ControlsInfo::ZoekCommandInfo(LPCSTR code)
{
  commandInfo* inf = commandos;
  while(inf->ID != 0)
  {
    if (strcmp(inf->code,code) == 0)
      return inf;

    inf += 1;
  }
  return NULL;
}

UINT
ControlsInfo::GeefCommandIDVanCode(LPCSTR code)
{
  commandInfo* inf = ZoekCommandInfo(code);
  if (inf)
      return inf->ID;

  return 0;
}

LPCSTR
ControlsInfo::GeefCodeVanCommandID(UINT ID)
{
  commandInfo* inf = ZoekCommandInfo(ID);
  if (inf)
    return inf->code;

  return "";
}

LPCSTR
ControlsInfo::GeefAfbeedingVanCommandID(UINT ID)
{
  commandInfo* inf = ZoekCommandInfo(ID);
  if (inf)
    return inf->defKnop;

  return "";
}

int
ControlsInfo::MaakDefaultAfbeelding(Ref<CAfbeeldingen> afbeeldingen)
{
  afbeeldingen->RegistreerExtensie(".BMP", "");
  afbeeldingen->RegistreerZoekPad(theApp.GetBinDirectory());
  afbeeldingen->RegistreerSystemZoekPad(theApp.GetBinDirectory()+"ImageLibraries\\");
  afbeeldingen->Init();

  commandInfo* inf = commandos;
  int aantal = 0;
  while(inf->ID != 0)
  {
    if (strlen(inf->code) > 0 && strcmp(inf->code,"??") != 0)
    {
      afbeeldingen->RegistreerStandaard(inf->code,inf->defKnop,AFB_SET_KNOPPEN);
      ++aantal;
    }
    inf += 1;
  }
  return aantal;
}

bool
ControlsInfo::CommandSetAktief(UINT ID,bool waarde,bool or)
{
  commandInfo* inf = ZoekCommandInfo(ID);
  if (inf)
  {
    if (or)
      inf->aktief |= waarde;
    else
      inf->aktief = waarde;

    return true;
  }
  return false;
}

bool
ControlsInfo::CommandSetAktief(LPCSTR code,bool waarde,bool or)
{
  commandInfo* inf = ZoekCommandInfo(code);
  if (inf)
  {
    if (or)
      inf->aktief |= waarde;
    else
      inf->aktief = waarde;

    return true;
  }
  return false;
}

bool
ControlsInfo::CommandSet(LPCSTR code,int waarde)
{
  commandInfo* inf = ZoekCommandInfo(code);
  if (inf)
  {
    inf->ibsTeller = waarde;
    return true;
  }
  return false;
}

bool
ControlsInfo::CommandSet(UINT ID,int waarde)
{
  commandInfo* inf = ZoekCommandInfo(ID);
  if (inf)
  {
    inf->ibsTeller = waarde;
    return true;
  }
  return false;
}

UINT
ControlsInfo::CommandUpdateAKtief()
{
  bool gewijzigd = false;

  commandInfo* inf = commandos;
  while(inf->ID != 0)
  {
    gewijzigd = gewijzigd || ((inf->ibsTeller == 1 || inf->aktief) != inf->vorigeAktief);
    inf->vorigeAktief = (inf->ibsTeller == 1 || inf->aktief);

    inf += 1;
  }
  return gewijzigd;
}

LPCSTR 
ControlsInfo::CommandPrompt(UINT ID)
{
  commandInfo* inf = ZoekCommandInfo(ID);
  if (inf)
  {
    if (inf->prompt) return inf->prompt;
    else             return inf->naam;
  }
  return "!!UNKNOWN!!";
}
