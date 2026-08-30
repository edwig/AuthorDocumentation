//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    AuthorImage.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Handles an BMP / PNG / JPEG image
//
#pragma once 

class AfbeeldingInfo;
class OnderdeelInfo;
class CAfbeeldingInfo;

#include "Images.h"

class AuthorImage
{
public:
  AuthorImage();
  ~AuthorImage();

  //bool BepaalAfbeelding(OnderdeelInfo* ond,int logica = AFB_SET_KNOPPEN, LPCSTR ptype = NULL,SchermAttribuut att = sat_afbeelding);
  bool CalculateStandard(LPCSTR type,LPCSTR def = NULL);
  bool ZetAfbeelding(LPCSTR afbeelding = NULL);

  bool HasImage();
  CRect PaintBitmap(CDC& pDC,CRect& rect,int volgNr,int drawParam);

  static Ref<CAfbeeldingInfo> CalculateImageInfo(LPCSTR str,int logica = AFB_SET_KNOPPEN);
  int CalculateButtonLayout(int& buttonLayout);

public:
  short m_posOptions;
  Ref<CAfbeeldingInfo> m_image;
};
