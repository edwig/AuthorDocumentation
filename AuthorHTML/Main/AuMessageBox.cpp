// AUTHOR HTML MessageBox
//
#include "stdafx.h"
#include "AuthorHTML.h"
#include "AuMessageBox.h"
#include "ControlsInfo.h"

const int OFFSET       =   8; // Ruimte tussen controls en teksten
const int MAX_LABELS   =   5; // Maximaal aantal labels/knoppen
const int ID_OFFSET    =  10; // Blijf weg van IDOK / IDCANCEL
const double ButtonWidthFactor = 8;   // Breedte van een knop, in "W"-letters

BEGIN_MESSAGE_MAP(AuMessageBox, CDialog)
  ON_WM_PAINT()
  ON_WM_KEYDOWN()
  ON_BN_CLICKED(ID_ONDERDRUKKEN,OnNietMeer)
END_MESSAGE_MAP()

AuMessageBox::AuMessageBox(CWnd*  parent
                    ,LPCSTR titel
                    ,LPCSTR boodschap
                    ,CString labels)
          :CDialog(AuMessageBox::IDD,parent)
          ,m_titel(titel)
          ,m_boodschap(boodschap)
          ,m_stijlen(0)
          ,m_default(0)
          ,m_def_gedaan(false)
          ,m_onderdruk(NULL)
          ,m_alleenOK(false)
          ,m_nietMeer(false)
          ,m_ownsFont(false)
          ,m_font(NULL)
{
  InitButtons();

  // Zoek naar een signaal plaatje op het eerste label
  int piep = 0;
  if(labels.GetAt(0) == '!') 
  {
    piep = MB_ICONEXCLAMATION;
    m_image.ZetAfbeelding(m_afbeelding = "EXCLAMATION");
  }
  if(labels.GetAt(0) == '?') 
  {
    piep = MB_ICONQUESTION;
    m_image.ZetAfbeelding(m_afbeelding = "QUESTION");
  }
  if(labels.GetAt(0) == '.') 
  {
    piep = MB_ICONHAND;
    m_image.ZetAfbeelding(m_afbeelding = "STOP");
  }
  if(labels.GetAt(0) == '#') 
  {
    piep = MB_ICONASTERISK;
    m_image.ZetAfbeelding(m_afbeelding = "INFORMATION");
  }

  if(!m_afbeelding.IsEmpty())
  {
    // Verwijder teken !?.# van het label
    labels = labels.Mid(1);
    // Geef de juiste piep, volgens de stijl
    MessageBeep(piep);
  }

  if(labels == "ok cancel")
  {
    labels = "ok cancel$ca";
  }
  if(labels == "cancel retry ignore")
  {
    labels = "cancel$ca retry$ok ignore$ig";
  }
  if(labels == "yes no")
  {
    labels = "yes$ye no$no";
  }
  if(labels == "yes no cancel")
  {
    labels = "yes$ok no$ca cancel$ig";
  }
  if(labels == "retry ignore")
  {
    labels = "retry$ok ignore$ca";
  }
  MaakLabelTekstenEnStijlen(labels);
}

AuMessageBox::AuMessageBox(CWnd* parent
                    ,LPCSTR titel
                    ,LPCSTR boodschap
                    ,int    stijlen)
          :CDialog(AuMessageBox::IDD,parent)
          ,m_titel(titel)
          ,m_boodschap(boodschap)
          ,m_default(0)
          ,m_def_gedaan(false)
          ,m_stijlen(stijlen)
          ,m_onderdruk(NULL)
          ,m_alleenOK(false)
          ,m_nietMeer(false)
          ,m_font(NULL)
{
  InitButtons();

  CString labels;
  if((stijlen & 0x0f) == MB_OK)       
  {
    labels = "ok";
  }
  if((stijlen & 0x0f) == MB_OKCANCEL) 
  {
    labels = "ok cancel$ca";
  }
  if((stijlen & 0x0f) == MB_ABORTRETRYIGNORE) 
  {
    labels = "abort$ca retry$ok ignore$ig";
  }
  if((stijlen & 0x0f) == MB_YESNO)            
  {
    labels = "yes$ok no$no";
  }
  if((stijlen & 0x0f) == MB_YESNOCANCEL)
  {
    labels = "yes$ok no$no cancel$ca";
  }
  if((stijlen & 0x0f) == MB_RETRYCANCEL)
  {
    labels = "retry$ok cancel$ca";
  }
  MaakLabelTekstenEnStijlen(labels);

  if(stijlen & 0x0f0)
  {
    // (MB_ICONHAND | MB_ICONQUESTION | MB_ICONEXCLAMATION | MB_ICONASTERISK)
    int stijl = stijlen & 0x0f0;
    if(stijl == MB_ICONASTERISK)     m_afbeelding = "INFORMATION";
    if(stijl == MB_ICONEXCLAMATION)  m_afbeelding = "EXCLAMATION";
    if(stijl == MB_ICONHAND)         m_afbeelding = "STOP";
    if(stijl == MB_ICONQUESTION)     m_afbeelding = "QUESTION";
    m_image.ZetAfbeelding(m_afbeelding);
    m_alleenOK = false;
    // Geef de juiste beep, volgens de stijl
    MessageBeep(stijl);
  }
  // Eventueel andere default button zetten.
  if(stijlen & 0xf00)
  {
    if((stijlen & 0xf00) == MB_DEFBUTTON1) m_default = ID_OFFSET + 0;
    if((stijlen & 0xf00) == MB_DEFBUTTON2) m_default = ID_OFFSET + 1;
    if((stijlen & 0xf00) == MB_DEFBUTTON3) m_default = ID_OFFSET + 2;
  }
}

// Destructor
AuMessageBox::~AuMessageBox()
{
  OnderdrukVoortaan();
  ResetButtons();
  if (m_ownsFont)
  {
    delete m_font;
  }
}

// Init buttons
void
AuMessageBox::InitButtons()
{
  for(int i = 0;i < MAX_LABELS; ++i)
  {
    m_button[i] = NULL;
  }
}

// Reset de button controls
void
AuMessageBox::ResetButtons()
{
  for(int i = 0;i < MAX_LABELS; ++i)
  {
    delete m_button[i];
    m_button[i] = NULL;
  }
  if(m_onderdruk)
  {
    delete m_onderdruk;
    m_onderdruk = NULL;
  }
}

// Splits de labeltekst op in afzonderlijke labels
// ok -> ok
// "ja nee" -> "ja", "nee" 
// Haal ook de knoptype uit het label op
// "knoptekst$ab" -> label = "knoptekst" style = "ab"
//
void
AuMessageBox::MaakLabelTekstenEnStijlen(CString& p_labels)
{
  // Alles resetten
  for(int i = 0; i < MAX_LABELS; ++i)
  {
    m_label[i] = CString("");
    m_style[i] = CString("");
    m_breed[i] = 0;
  }
  // label string doorlopen
  CString rest = p_labels;
  int spatiePos = 0;
  for(int i = 0; i < MAX_LABELS; ++i)
  {
    spatiePos = rest.Find(' ');
    if(spatiePos > 0)
    {
      m_label[i] = rest.Left(spatiePos);
      rest = rest.Mid(spatiePos + 1);
      rest.TrimLeft();
    }
    else
    {
      m_label[i] = rest;
    }
    rest.TrimLeft();
    if(m_label[i].Find('$') > 0)
    {
      int pos = m_label[i].Find('$');
      m_style[i] = m_label[i].Mid(pos + 1);
      m_label[i] = m_label[i].Left(pos);
    }
    m_label[i].Replace('_',' ');
    if(spatiePos < 0)
    {
      break;
    }
  }

  // Zoek naar de default knop (alleen de eerste wordt gevonden)
  for(int i = 0; i < MAX_LABELS; ++i)
  {
    if(!m_label[i].IsEmpty())
    {
      if(m_label[i].GetAt(0) == '@')
      {
        m_default  = i + ID_OFFSET;
        m_label[i] = m_label[i].Mid(1);
        break;
      }
    }
  }
  // Zet het eerste teken op uppercase (uitzondering = OK)
  for(int i = 0; i < MAX_LABELS; ++i)
  {
    if(!m_label[i].IsEmpty())
    {
      if(m_label[i] == "ok")
      {
        m_label[i] = "OK";
      }
      else
      {
        m_label[i].SetAt(0,(char)toupper(m_label[i].GetAt(0)));
      }
    }
  }
  // Als er geen stijlen zijn gevonden
  // maar er zijn wel labels
  // Zoek dan naar de knopstijl in controlsinfo
  ControlsInfo* info = theApp.GetControlsInfo();
  for (int i = 0; i < MAX_LABELS; ++i)
  {
    if(m_style[i].IsEmpty())
    {
      UINT ID = info->ResourceIDFromNaam(m_label[i]);
      if(ID)
      {
        commandInfo* com = info->ZoekCommandInfo(ID);
        if(com)
        {
          m_style[i] = com->code;
        }
      }
    }
  }
  // Controle op alleen een OK knop
  if((m_label[0].CompareNoCase("ok") == 0) && m_label[1].IsEmpty())
  {
    if(m_afbeelding.IsEmpty())
    {
      m_alleenOK = true;
    }
  }
}

void
AuMessageBox::OnOK()
{
  EndDialog(GetFocus()->GetDlgCtrlID());
}

// Vertaal het knop ID van het resultaat
// IDCANCEL = ESC   toets
// ID       = ID van het control
CString
AuMessageBox::GeefResultaat(int p_id)
{
  CString resultaat;
  if((p_id >= ID_OFFSET) && (p_id < (ID_OFFSET + MAX_LABELS)))
  {
    int label = p_id - ID_OFFSET;
    if(label >=0 && label < MAX_LABELS)
    {
      resultaat = m_label[label];
    }
  }
  else
  {
    if(p_id == IDCANCEL)
    {
      // Op de ESCape toets gedrukt
      resultaat = "";
    }
  }
  resultaat.MakeLower();
  return resultaat;
}

// Geef het resultaat van een standaard boodschap
int
AuMessageBox::GetResultID(int p_id)
{
  int altOK = 0;
  switch(m_stijlen & 0x0f)
  {
    case MB_OK:          if(p_id == (ID_OFFSET + 0)) return IDOK;
                         break;
    case MB_YESNO:       altOK = IDYES;
                         if(p_id == (ID_OFFSET + 0)) return IDYES;
                         if(p_id == (ID_OFFSET + 1)) return IDNO;
                         break;
    case MB_RETRYCANCEL: altOK = IDRETRY;
                         if(p_id == (ID_OFFSET + 0)) return IDRETRY;
                         if(p_id == (ID_OFFSET + 1)) return IDCANCEL;
                         break;
    case MB_OKCANCEL:    if(p_id == (ID_OFFSET + 0)) return IDOK;
                         if(p_id == (ID_OFFSET + 1)) return IDCANCEL;
                         break;
    case MB_YESNOCANCEL: altOK = IDYES;
                         if(p_id == (ID_OFFSET + 0)) return IDYES;
                         if(p_id == (ID_OFFSET + 1)) return IDNO;
                         if(p_id == (ID_OFFSET + 2)) return IDCANCEL;
                         break;
    case MB_ABORTRETRYIGNORE: 
                         altOK = IDABORT;
                         if(p_id == (ID_OFFSET + 0)) return IDABORT;
                         if(p_id == (ID_OFFSET + 1)) return IDRETRY;
                         if(p_id == (ID_OFFSET + 2)) return IDIGNORE;
                         break;
  }
  if(p_id == IDCANCEL)
  {
    return p_id;
  }
  // Onbekend resultaat
  return 0;
}

CString
AuMessageBox::GeefStandaardPositief()
{
  for(int i = 0; i < MAX_LABELS; ++i)
  {
    if(!m_style[i].IsEmpty())
    {
      if(m_style[i].CompareNoCase("ok") == 0)
      {
        return m_label[i];
      }
    }
  }
  return "";
}

CString
AuMessageBox::GeefStandaardNegatief()
{
  for(int i = 0; i < MAX_LABELS; ++i)
  {
    if(!m_style[i].IsEmpty())
    {
      if(m_style[i].CompareNoCase("ca") == 0)
      {
        return m_label[i];
      }
    }
  }
  return "";
}

int
AuMessageBox::GeefStandaardPositiefID()
{
  for(int i = 0; i < MAX_LABELS; ++i)
  {
    if(!m_style[i].IsEmpty())
    {
      if(m_style[i].CompareNoCase("ok") == 0)
      {
        return i + ID_OFFSET;
      }
    }
  }
  return 0;
}

int
AuMessageBox::GeefStandaardNegatiefID()
{
  for(int i = 0; i < MAX_LABELS; ++i)
  {
    if(!m_style[i].IsEmpty())
    {
      if(m_style[i].CompareNoCase("ca") == 0)
      {
        return i + ID_OFFSET;
      }
    }
  }
  return 0;
}

/*************************************************************\
*                                                             *
*   Aanmaken en tekenen van de boodschap                      *
*                                                             *
\*************************************************************/

// Hele boodschap dialoog herberekenen bij het starten ervan
BOOL
AuMessageBox::OnInitDialog()
{
  CDialog::OnInitDialog();


  m_font = new CFont;
  LOGFONT lf = ControlsInfo::MaakLOGFONTVanString("");
  m_font->CreateFontIndirect( &lf );
  m_ownsFont = true;

  SetWindowText(m_titel); 

  // Pak control, en zet font erop
  CEdit* edit = (CEdit*) GetDlgItem(IDC_AUMESSAGEBOX);
  edit->SetFont(m_font);

  // GetDC levert NIET een dc waarin het goede font zit.
  // Daarom ook hier het font erin voor de berekening.
  // Alle berekeningen moeten dan ook verder dc gebruiken.
  CDC* dc = edit->GetDC();
  dc->SelectObject(m_font);

  // Bepaal meldingtekst
  CString text = m_boodschap;

  // Vervang alle voorkomens van \n door \r\n
  text.Replace("\r\n", "\n");
  text.Replace("\n", "\r\n");


  // Bepaal breedte/hoogte
  CRect tekstRect = CRect(0, 0, 0, 0);
  tekstRect.right = GetSystemMetrics(SM_CXSCREEN) * 90 / 100;
  dc->DrawText(text, &tekstRect, 
               DT_CALCRECT|DT_LEFT|DT_NOPREFIX|DT_WORDBREAK|DT_EXPANDTABS|DT_EDITCONTROL);

  // Tel de interne marges van het edit control bij de afmetingen het de tekstdeel,
  // zodat de edit control groot genoeg wordt om ook z'n eigen marges te bevatten
  CRect margins;
  edit->GetRect(margins);
  tekstRect.right += margins.left * 2;
  tekstRect.bottom += margins.top * 2;

  // Niet te hoog laten worden, anders scrollbar plaatsen
  int maxHeight = GetSystemMetrics(SM_CYSCREEN) * 80 / 100;
  if(tekstRect.bottom > maxHeight)
  {
    edit->ModifyStyle(0, WS_VSCROLL);
    tekstRect.bottom = maxHeight;
    tekstRect.right += GetSystemMetrics(SM_CXHTHUMB);
  }

  // Voeg fixed offsets (kaders) toe
  tekstRect.OffsetRect(OFFSET,OFFSET);

  // Informatie pictogram
  if(!m_afbeelding.IsEmpty())
  {
    // Extra ruimte links (32 pixels) maken voor het plaatje
    tekstRect.OffsetRect(32 + OFFSET,0);
    if(tekstRect.bottom < 32)
    {
      // TODO De tekst verticaal centreren
      tekstRect.bottom = 32 + OFFSET;
    }
  }

  // Plaatsen en tekst erin zetten
  edit->MoveWindow(tekstRect);
  edit->SetWindowText(text);

  // Knoppen maken
  // Voor de breedte gebruiken we de breedte van 
  // een "W", voor de hoogte de echte fonthoogte.
  CSize tsize = dc->GetTextExtent("W");
  int buttonTop    = tekstRect.bottom + OFFSET;
  int buttonWidth  = (int)(ButtonWidthFactor * tsize.cx + (3 * OFFSET));
  int buttonHeight = tsize.cy + 4 * GetSystemMetrics(SM_CYFIXEDFRAME);
  int totalWidth   = OFFSET;

  // Bekijk de vorm van de knoppen
  int layout = theApp.GetButtonLayout();
  if(layout == BUTT_LAYOUT_IMAGE)
  {
    buttonWidth = buttonHeight;
  }
  if(layout & (BUTT_LAYOUT_TOP | BUTT_LAYOUT_BOTTOM))
  {
    buttonHeight += 16;
    buttonWidth  -= (3 * OFFSET);
  }
  if(layout & BUTT_LAYOUT_NONE)
  {
    buttonWidth -= (3 * OFFSET);
  }
  m_buttonRect.left   = 0;
  m_buttonRect.top    = 0;
  m_buttonRect.right  = buttonWidth;
  m_buttonRect.bottom = buttonHeight;
  // Bereken de totale breedte van alle knoppen tesamen
  for(int i = 0; i < MAX_LABELS; ++i)
  {
    if(!m_label[i].IsEmpty())
    {
      if(totalWidth > OFFSET) 
      {
        totalWidth += OFFSET;
      }
      m_breed[i] = buttonWidth;
      if(layout != BUTT_LAYOUT_IMAGE)
      {
        int breedte = (((m_label[i].GetLength() * tsize.cx) * 2) / 3) + (3 * OFFSET);
        if(breedte > buttonWidth)
        {
          m_breed[i] = breedte;
        }
      }
      totalWidth += m_breed[i];
    }
  }
  // Bereken het begin van de buttons.
  // Als de tekst breder is dan de buttons, gebruik dan de tekst als total breedte
  int buttonBegin = OFFSET;
  if(tekstRect.right > totalWidth)
  {
    buttonBegin += tekstRect.right - totalWidth;
    totalWidth   = tekstRect.right;
  }
  // Creeer de buttons
  long buttonStyle = BS_OWNERDRAW | WS_TABSTOP | BS_NOTIFY | WS_CHILD | WS_VISIBLE;
  for(int i = 0; i < MAX_LABELS; ++i)
  {
    CString sButtonTekst = m_label[i];
    if(!sButtonTekst.IsEmpty())
    {
      if(sButtonTekst.Find('&') < 0)
      {
        sButtonTekst = "&" + sButtonTekst;
      }
      buttonWidth = m_breed[i];
      CRect rect(buttonBegin,buttonTop,buttonBegin + buttonWidth,buttonTop + buttonHeight);
      m_button[i]  = new CButtonC(m_style[i]);
      m_button[i]->Create(sButtonTekst  // Labeltekst
                         ,buttonStyle                 // MS-Windows window style
                         ,rect                        // Rechthoek
                         ,this                        // Mijn kind
                         ,i + ID_OFFSET);             // CtrlID van deze knop
      m_button[i]->SetFont(m_font);
      // Voor volgende knop
      buttonBegin += buttonWidth + OFFSET;
    }
  }
  // Grootte van het window herberekenen
  totalWidth += OFFSET;
  int totalHeight = buttonTop + OFFSET + buttonHeight;
  if(m_alleenOK)
  {
    int hcb = tsize.cy + 2*GetSystemMetrics(SM_CYFIXEDFRAME);
    int wcb = tsize.cx + 2*GetSystemMetrics(SM_CXFIXEDFRAME);
    totalHeight += hcb + OFFSET;

    CString sNietMeerHerhalen = "Do not show again      ";

    // Bepaal tekst breedte
    CRect textRect = CRect(0, 0, 0, 0);
    dc->DrawText(sNietMeerHerhalen, &textRect,DT_CALCRECT|DT_LEFT|DT_NOPREFIX|DT_SINGLELINE|DT_EXPANDTABS);


    // Maak checkbox
    int top = buttonTop + buttonHeight + 2 * OFFSET;
    m_lijn.left   = OFFSET;
    m_lijn.top    = top - OFFSET;
    m_lijn.right  = wcb + textRect.Width() - OFFSET;
    m_lijn.bottom = m_lijn.top;
    CRect brect(OFFSET,top,m_lijn.right,top + hcb);
    totalWidth = max(totalWidth, wcb + textRect.Width() + OFFSET);


    m_onderdruk = new AD_Checkbox();

    m_onderdruk->Create(sNietMeerHerhalen
                       ,WS_TABSTOP | BS_AUTOCHECKBOX | BS_NOTIFY | WS_CHILD | WS_VISIBLE
                       ,brect
                       ,this    
                       ,ID_ONDERDRUKKEN);
    m_onderdruk->SetFont(m_font);
  }

  CRect rect(0, 0, totalWidth, totalHeight);
  AdjustWindowRect(rect,GetStyle(),false);
  MoveWindow(rect);
  CenterWindow(AfxGetMainWnd());

  return TRUE;
}

void
AuMessageBox::OnPaint()
{
  CDialog::OnPaint();
  
  CDC* dc = GetDC();
  int saveDC = dc->SaveDC();

  // Het signaal tekenen
  if(!m_afbeelding.IsEmpty())
  {
    CRect rect(OFFSET,OFFSET,40,40);
    m_image.PaintBitmap(*dc,rect,0,AFB_PAINT_TRANS);
  }
  // Eventueel lijn tekenen
  if(m_alleenOK)
  {
    dc->DrawEdge(m_lijn,EDGE_ETCHED,BF_TOP);
  }
  dc->RestoreDC(saveDC);
}

// Override bij te onderdrukken boodschap
INT_PTR
AuMessageBox::DoModal()
{
  // Onderdruk de boodschap eventueel
  //AutoIBSMessageLock lock;
  if(m_alleenOK)
  {
    if(theApp.IsSuppressedMessage(m_boodschap))
    {
      return IDOK;
    }
  }
  // Vraag om aandacht, alleen als we nog niet de foreground window zijn.
  CWnd *w = GetForegroundWindow();
  bool bFlash = true;
  while(w)
  {
    if(w == AfxGetMainWnd())
    {
      bFlash = false;
      break;
    }
    w = w->GetParent();
  }

  // DJS Ik begrijp niet waarom het niet werkt met IsChild. 
  if(AfxGetMainWnd() && bFlash)
  //  if(AfxGetMainWnd() && !(GetForegroundWindow()->IsChild(AfxGetMainWnd())) )
  {
    AfxGetMainWnd()->FlashWindowEx(FLASHW_TRAY, 3, 0);
  }
  // Toon dialoog
  return CDialog::DoModal();
}

void
AuMessageBox::OnKeyDown(UINT nChar, 
                     UINT nRepCnt, 
                     UINT nFlags)
{
  ControleerActie(nChar);
  CDialog::OnKeyDown(nChar,nRepCnt,nFlags);
}

// Kijk of de ingedrukte toets overeenkomt met het eerste teken
// van een van onze labels van de knoppen
void
AuMessageBox::ControleerActie(UINT nChar)
{
  if(nChar == ' ')
  {
    // Spatie selecteert de huidige knop met focus
    CButtonC* wnd = (CButtonC *) GetFocus();
    for(int i = 0; i < MAX_LABELS; ++i)
    {
      if(m_button[i] == wnd)
      {
        EndDialog(i + ID_OFFSET);
      }
    }
  }
  for(int i = 0;i < MAX_LABELS; ++i)
  {
    // Zoek anders het label dat begint met deze letter
    if(!m_label[i].IsEmpty())
    {
      if(tolower(m_label[i].GetAt(0)) == tolower(nChar))
      {
        EndDialog(i + ID_OFFSET);
        return;
      }
    }
  }
}

// We drukken op deze knop
// Alleen een hit als muis nog boven de knop
void
AuMessageBox::DrukOpKnop(UINT p_id,CPoint point)
{
  if(p_id >= ID_OFFSET && p_id < ID_OFFSET + MAX_LABELS)
  {
    m_buttonRect.right = m_breed[p_id - ID_OFFSET];
    if(m_buttonRect.PtInRect(point))
    {
      EndDialog(p_id);
    }
  }
}

// Aanvraag vanuit de knop of het de standaard knop is
// Tevens controle op initiele focus regelen
bool
AuMessageBox::GeefDefault(UINT ID)
{
  if(!m_def_gedaan)
  {
    // Dit is het eerste moment na het starten van de dialoog en
    // voor het tekenen van de eerste button. Nog snel even de focus zetten
    m_def_gedaan = true;
    if(m_default)
    {
      GotoDlgCtrl(m_button[m_default - ID_OFFSET]);
    }
  }
  if(ID == (UINT)m_default)
  {
    return true;
  }
  return false;
}

// Check box handler voor onderdrukken
void
AuMessageBox::OnNietMeer()
{
  m_nietMeer = !m_nietMeer;
}

// Huidige boodschap nooit meer tonen, altijd OK.
void
AuMessageBox::OnderdrukVoortaan()
{
  if(m_nietMeer)
  {
    theApp.SuppressMessage(m_boodschap);
  }
}
