// ControlsInfo.h
//
#pragma once 
#include "StdAfx.h"
#include "ImageDefinitions.h"
//#include "Bouwstenen\SOType.h"
//#include <Lijsten/MFClozeAfxTempl.h>

class VarObjectLijst;
class CAfbeeldingen;
class CToolBar;

template<typename T>
class Ref;

#ifdef AFXTEMPLATE_INCLUDE
#define _AFXAPI 
#else
#define _AFXAPI AFXAPI
#endif

template <>
inline UINT _AFXAPI
HashKey(CString key)
{
  int hier = key.GetLength();
  UINT sleutel = 1;
  while (hier)
  {
    sleutel *= hier;
    sleutel += key[--hier];
  }

  return sleutel;
}

class AppFont : public CFont
{
public:
  short m_gemBreed;
  short m_hoog;

  LOGFONT m_logfont;
};

typedef struct tag_commandInfo
{
  UINT ID;
  char* naam;
  char* code;
  char* defKnop; 
  
  UINT  ibsTeller;
  bool  aktief;

  bool  vorigeAktief;
  char* prompt;
} commandInfo;


static const int MAXCONTROLS = 30;

// Deze klasse houdt bij wat de normale groottes en lettertypes zijn
// voor verschillende soorten controls op het scherm. Deze informatie 
// wordt ingelezen uit een bestaand dialoogvenster met (onzichtbare)
// voorbeelden van deze controls.
class ControlsInfo
{
public:
  ControlsInfo();
  virtual ~ControlsInfo();

  void BepaalInfo();



  long GemiddeldeCharBreedte(CFont* lettertype);
  long CharHoogte(CFont* lettertype);


  WORD ResourceIDFromNaam(LPCSTR naam);

  // Static helpers
  static LOGFONT MaakLOGFONTVanString(CString fontstring,CString deffont = "",int defsize = 0,int defgewicht = 0,int maxsize = INT_MAX);

  static int  PaintRectKader(CDC& dc,CRect& rect,int soort,COLORREF kleur = 0);
  static int  PaintRectEffect(CDC& dc,CRect& rect,COLORREF van,COLORREF naar,int type);
  static void TekenVorm(CDC& dc,CRect& rect,COLORREF kleur,int vorm,int rectAdjust = -1,int penDikte = 1);
  static void MaakSchaduwRgn(CDC& dc,CRect& rect,int type = 0);
  static void PlaatsRectInRect(const CRect& inRect,CRect& rect,int pos = 0,int marges = 0);

  int CheckBoxPromptPositie();
  bool FocusLabel();
  int  TabVerwKnop();
  int  KnopLayout();
  int  SetKnopLayout(int layout);

  void KnopAfbeeldingRect(CRect& rect);

  static commandInfo* ZoekCommandInfo(UINT ID);
  static commandInfo* ZoekCommandInfo(LPCSTR code);

  static LPCSTR GeefCodeVanCommandID(UINT ID);
  static UINT GeefCommandIDVanCode(LPCSTR code);

  static LPCSTR GeefAfbeedingVanCommandID(UINT ID);
  static int    MaakDefaultAfbeelding(Ref<CAfbeeldingen> afbeeldingen);
  static bool   CommandSetAktief(UINT ID,bool waarde,bool or = true);
  static bool   CommandSetAktief(LPCSTR code,bool waarde,bool or = true);
  static bool   CommandSet(LPCSTR code,int waarde);
  static bool   CommandSet(UINT ID,int waarde);

  static UINT   CommandUpdateAKtief();

  LPCSTR CommandPrompt(UINT ID);
protected:

private:

  void InstellingenGewijzigd();

  void MaakLeeg();


  // Lijst van (pointers naar) lettertypes
  AppFont* m_lettertypes[MAXCONTROLS];

  // Lijst van normale hoogtes voor alle controls
  int m_hoogtes[MAXCONTROLS];
  // Lijst van normale breedtes voor alle controls
  int m_breedtes[MAXCONTROLS];
  // Lijst van normale randhoogtes voor alle controls
  int m_randhoogtes[MAXCONTROLS];
  // Lijst van normale randbreedtes voor alle controls
  int m_randbreedtes[MAXCONTROLS];

  int  m_checkBoxPromptPositie;
  bool m_focusLabel;
  int  m_tabVerwKnop;
  int  m_knopLayout;

  CRect m_miniRect;
  CRect m_fullRect;

  int m_enkeleRegelHoogte;

  LOGFONT     m_lf_default;
  TEXTMETRIC  m_textmetric;
};

inline
int
ControlsInfo::CheckBoxPromptPositie()
{
  return m_checkBoxPromptPositie;
}

inline
bool
ControlsInfo::FocusLabel()
{
  return m_focusLabel;
}

inline
int
ControlsInfo::TabVerwKnop()
{
  return m_tabVerwKnop;
}


inline
int
ControlsInfo::KnopLayout()
{
  return (m_knopLayout & BUTT_LAYOUT_ALL)?m_knopLayout:BUTT_LAYOUT_NONE;
}

inline
int
ControlsInfo::SetKnopLayout(int layout)
{
  int cc = m_knopLayout;
  m_knopLayout = layout;
  return cc;
}

inline  void
ControlsInfo::KnopAfbeeldingRect(CRect& rect)
{
  rect = m_miniRect;
}
