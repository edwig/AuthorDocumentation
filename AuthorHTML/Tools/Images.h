// Images
//
#pragma once

#include <algorithm>
#include <afxcmn.h>
#include "CNamenMap.h"
#include "StringArray.h"
#include "DIB.h"
#include "RefCounted.h"
#include "ImageDefinitions.h"
#include <vector>
#include <map>

class CAfbeeldingen;
class CAfbeelding;
class CAfbeeldingEntry;

class CAfbeeldingInfo : public RefCounted
{
public:
  CAfbeeldingInfo(CAfbeeldingen& afbeeldingen);
  ~CAfbeeldingInfo();

  bool HeeftReferenties() const;
  bool HasImage() const;
  
  bool HeeftAfbeeldingsNummer(int idx) const 
  {
    return m_nr[idx] >= 0;
  }
  const CRect& GeefRect() const
  { 
    return m_rect;
  }
private:
  // De lijst met afbeeldingen waar wij in zitten. (Geen referentie, anders cykel.)
  CAfbeeldingen&    m_afbeeldingen;

  Ref<CAfbeeldingEntry> m_entry;
protected:
  short  m_nr[AFB_POS_AANTAL_INFO];
  CRect  m_rect;

  Ref<CAfbeeldingEntry> SetEntry(Ref<CAfbeeldingEntry> entry);
  friend class CAfbeeldingen;
};



class CBitmapDC : public CDC
{
public:
  CBitmapDC(CDC* dc,CBitmap* bitmap);
  CBitmapDC(CDC* dc);
  CBitmapDC(CDC* dc,int nWidth,int nHeight);
  CBitmapDC(CDC* dc,int nWidth,int nHeight,UINT nPlanes, UINT nBitcount);
  ~CBitmapDC();
  bool CreateCompatibleBitmap(CDC* dc,int nWidth,int nHeight);
  bool CreateBitmap( int nWidth, int nHeight, UINT nPlanes, UINT nBitcount, const void* lpBits );
  bool LoadBitmap( UINT ID );
  CBitmap* GeefBitmap();
  void SelectBitmap(CBitmap* bitmap);
  void DeSelectBitmap();

private:
  CBitmap* m_bitmap;
  CBitmap* m_oldBitmap;
};

/////////////////////////////////////////////////////////////////////////////
// CAfbeelding
class CAfbeelding : public RefCounted
{
public:
  CAfbeelding(CAfbeeldingen& afbeeldingen);
  virtual ~CAfbeelding();

  void  Reset();
  bool  SetType(LPCSTR nieuwType);
  void  MaakStandaardKnopVolgnummers(short volgnr[],int aantal);
  short GeefNummer(short setNummer, int srtPlaatje = 0);
  bool  SetKnopNummers(short m_volgnr[],short setNummer,int aantal);
  bool  SetSerieNummers(short m_volgnr[],short setNummer,int aantal);
  bool  SetSetNummers(short volgnr[],short setNummer,int aantal);

  short GeefSetNummerVanNaam(LPCSTR naam,LPCSTR defExt = ".ico");
  int   GeefPosNummerVanNaam(LPCSTR naam);
  
  static bool  SetInterneKnopLogica(short volgnr[],short setNummer,int aantal);

  int   Aantal()       { return m_aantal; };

  short AantalPerSet() { return max((short)m_aantalPS,(short)1); };

  CRect GeefRect() {return CRect(0,0,m_cx,m_cy);};
  CSize GeefSize() {return CSize(m_cx,m_cy);};
  int   GeefBreedte() {return m_cx;};

  bool  HeeftDisabled() {return m_heeftDiasabled;};

  LPCSTR GeefTypeSetNaam();


  virtual HICON GeefIcon(int nr = 0);

  virtual void OnReset();

// implementation pure virtual
  virtual int  Create(UINT nBitmapID, UINT nExtraBitmapID = 0) = 0;
  virtual int  LoadDir(LPCSTR) {return -1;};
  virtual bool Paint(CDC& pDC,CRect& rect,int nr = 0, int drawParam = 0 ) = 0;
  
protected:
  // De lijst met afbeeldingen waar wij in zitten. (Geen referentie, anders cykel!)
  CAfbeeldingen& m_afbeeldingen;

  int    m_cx;
  int    m_cy;
  int    m_aantal;
  // Type definitie
  bool   m_heeftDiasabled;
  short  m_aantalPS;
  char   m_type[AFB_POS_AANTAL+1];
  short  m_pos[AFB_POS_AANTAL];
  char   m_typeSet;                     // 0 set 1 knoppen

  StringArray m_namen;
private:
  friend class CAfbeeldingen;
  friend class CAfbeeldingInfo;
};


inline
LPCSTR
CAfbeelding::GeefTypeSetNaam()
{
  switch(m_typeSet)
  {
    case AFB_SET_SET:     return "SET";
    case AFB_SET_SERIE:   return "SERIE";
    case AFB_SET_KNOPPEN: return "KNOPPEN";
  }
  return "STANDAARD";
}


class CAfbeeldingIM : public CAfbeelding
{
public:
  CAfbeeldingIM(CAfbeeldingen& afbeeldingen);
  CAfbeeldingIM(CAfbeeldingen& afbeeldingen,UINT nBitmapID,int cx,int cy,UINT extraBitmapID);
  virtual ~CAfbeeldingIM();
  
  int Create(UINT nBitmapID, UINT extraBitmapID);
  int Create(UINT nBitmapID,int cx,int cy, UINT extraBitmapID);
  int Create(char* file,int cx);
  int Create(char* file,CDIB& dib,int cx);
  int LoadDir(LPCSTR naam);
  int VoegIconToe(HICON icon,LPCSTR naam);
  HICON GeefIcon(int nr = 0);
  int VoegBitmapToe(CBitmap* bitmap,LPCSTR naam,COLORREF bk);

  virtual bool Paint(CDC& pDC,CRect& rect,int nr = 0, int drawParam = 0 );

  virtual bool DrawState(CBitmapDC& dc,int index, COLORREF bkColor = 0);
  virtual bool DrawDisabled(CDC& dc,int index,CPoint& pos,bool mask = false);

protected:
  CImageList  m_imageList;

  friend class CAfbeeldingen;
};

class CAfbeeldingBM : public CAfbeelding
{
public:
  CAfbeeldingBM(CAfbeeldingen& afbeeldingen);
  int Create(UINT nBitmapID, UINT extraBitmapID);
  bool FinishCreate(CDC& dcTemp);
  bool MaakBWBitmap(CDC& dcTemp);

  virtual bool Paint(CDC& pDC,CRect& rect,int nr = 0, int drawParam = 0);
protected:
  CBitmap m_bitmap;
  COLORREF m_tColor;
  CBitmap m_bitmapBW;
};

class CAfbeeldingDIB : public CAfbeeldingBM
{
public:
  CAfbeeldingDIB(CAfbeeldingen& afbeeldingen);

  virtual int Create(char* file);
  virtual int Create(CDIB& dib);
};


class CAfbeeldingEntry : public RefCounted
{
public:
  CAfbeeldingEntry(CAfbeeldingEntry& entry);
  CAfbeeldingEntry(UINT resourceID,LPCSTR naam,LPCSTR type, int cx = 0, int sets = 0, UINT extraResourceID = 0);
  CAfbeeldingEntry(LPCSTR naam,LPCSTR type = NULL,bool lijst = false,int cx = 0,int sets = 0);
  ~CAfbeeldingEntry();

  Ref<CAfbeelding> SetAfbeelding(Ref<CAfbeelding> afbeelding);

private:
  static UINT m_volgendeEntry;

  UINT         m_ID;                    // Entry ID verwijst naar de Cafbeelding 
  Ref<CAfbeelding> m_afbeelding;
  UINT         m_resourceID;            // Laden uit resource
  UINT         m_extraResourceID;       // Bitmaps mogen maar 2048 pixels breed zijn -- extra resource om nog meer in te kunnen lezen.
  int          m_cx;                    // Breedte van de plaatjes
  int          m_sets;                  // minimaal aantal sets in een imagelist
  char         m_type[AFB_POS_AANTAL+1];

  CString       m_fileLoaded;
  int          m_info;
  USHORT       m_status;
public:
  friend class CAfbeeldingen;
  friend class CAfbeeldingInfo;
};



class CAfbeeldingExtensie : public RefCounted
{
public:
  CAfbeeldingExtensie(LPCSTR ext,LPCSTR type,USHORT cx = 0);
  ~CAfbeeldingExtensie();

private:
  CString m_ext;
  CString m_type;
  USHORT  m_cx;

  friend class CAfbeeldingen;
};

class CAfbeeldingZoekpad
{
public:
  CAfbeeldingZoekpad(LPCSTR pad = "",USHORT type = 0);
  ~CAfbeeldingZoekpad();

private:
  CString m_pad;
  USHORT  m_type;

  friend class CAfbeeldingen;
};

class CAfbeeldingen : public RefCounted
{
public:
  CAfbeeldingen();
  ~CAfbeeldingen();

  void Init();
  void Reset();
  
  // Entry interface
  Ref<CAfbeeldingEntry> MaakResourceEntry(UINT resourceID,LPCSTR naam,LPCSTR type, int cx = 0, int sets = 0, UINT extraResourceID = 0);
  Ref<CAfbeeldingEntry> MaakNaamEntry(LPCSTR naam,LPCSTR type = NULL,bool lijst = false,int cx = 0,int sets = 0);


  // Operations
  CRect PaintBitmap(CDC& pDC,CRect& rect,Ref<CAfbeeldingInfo> info,int volgnr = 0, int drawParam = 0);
  HICON GeefIcon(LPCSTR info,LPCSTR definfo = NULL);
  HICON GeefIcon(Ref<CAfbeeldingInfo> info,int volgNr);

  Ref<CAfbeeldingInfo> GetImageInfo(LPCSTR str = NULL,int logica = AFB_SET_KNOPPEN);
  static bool IsSetNummer(LPCSTR str);

  Ref<CAfbeeldingInfo>  GeefStandaard(LPCSTR naam);
  CString GeefInfoSTR(Ref<CAfbeeldingInfo> info);

  bool RegistreerStandaard(LPCSTR naam,LPCSTR soort,int logica = AFB_SET_KNOPPEN);
  int  RegistreerExtensie(LPCSTR ext,LPCSTR type,USHORT cx = 0);
  int  RegistreerZoekPad(const CString& pad);         // Zoek paden voor losse plaatjes
  int  RegistreerSystemZoekPad(const CString& pad);   // Zoek paden voor library's en losse plaatjes
  bool LaatsteFout();

protected:
  // Info interface
  bool MaakAfbeeldingInfo(LPCSTR str,Ref<CAfbeeldingInfo> info,int logica = AFB_SET_KNOPPEN);

  bool SetAt(UINT key,Ref<CAfbeelding> pAfbeelding,LPCSTR naam);


  // Zoeken en vinden van afbeeldingen eventueel extern
  Ref<CAfbeeldingEntry> ZoekEntry(LPCSTR naam, bool lijst = false, int* defSet = NULL);
  UINT               LoadEntry(LPCSTR naam,Ref<CAfbeeldingEntry> entry);
  UINT               LoadEntryFromFile(LPCSTR naam,Ref<CAfbeeldingEntry> entry);
  CDIB*              FindDIBFile(LPCSTR naam, DWORD lengte, LPTSTR PathName, LPTSTR* FileName);
  bool               LoadDIBFile(CDIB& dib,LPCSTR naam, DWORD lengte, LPTSTR PathName, LPTSTR* FileName);
 
  // Configuratie van de zoekpaden
  bool IsVolledigPad(LPCSTR pad);
  bool HeeftExtensie(LPCSTR pad,LPCSTR extensie = ".ico");
  Ref<CAfbeeldingExtensie> SplitEnFindExtensie(LPCSTR ext,CString deel1,CString deel2);


  // Standaards

private:
  bool m_initGedaan;
  
  typedef std::map<CString,Ref<CAfbeeldingInfo>     > StandaardsMap;
  typedef std::map<CString,Ref<CAfbeeldingEntry>    > EntriesMap;
  typedef std::map<CString,Ref<CAfbeeldingInfo>     > InfosMap;
  typedef std::map<CString,Ref<CAfbeeldingExtensie> > ExtensiesMap;
  typedef std::vector<CAfbeeldingZoekpad>             PadenMap;
  // Entries geven aan hoe een afbeelding geladen moet worden
  EntriesMap    m_AfbeeldingenEntries;

  // CString naar afbeeldingsinfo
  InfosMap      m_Infos;

  // Standaard afbeeldingsinfos met een naam
  StandaardsMap m_Standaards;

  ExtensiesMap m_AfbeeldingenExtensies;

  // ZoekPaden
  PadenMap m_AfbeeldingenPaden;


  friend class CAfbeelding;
  friend class CAfbeeldingInfo;
  friend class CAfbeeldingEntry;


  // Painting etc
public:
  Ref<CAfbeeldingEntry> LoadDir(LPCSTR naam,bool altijdAanmaken = false);
  int PaintAfbeelding(LPCSTR naam, CDC* pDC);
  int PaintAfbeeldingText(int& xPos, int& yPos, CDC* pDC, LPCSTR text);
};

