//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    CIcon.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Handles an icon directly from an image
//
#pragma once

struct IconData 
{
	BYTE		width;			  // width of icon (16, 32, 64)
	BYTE		height;			  // height of icon (16, 32, 64)
	BYTE		colorCount;		// number of colors in icon (2, 8, 16)
	BYTE		reserved0;		// reserved for future use
	WORD		reserved1;		// reserved for future use
	WORD		reserved2;		// reserved for future use
	DWORD		icoDIBSize;		// size, in bytes, of icon image
	DWORD		icoDIBOffset;	// offset, in bytes, from start of file to image
};
typedef struct IconData ICONDATA;

/*
**	Icon File Header Format
*/
struct IconFileHeader 
{
	WORD	icoReserved;		// must be 0
	WORD	icoResourceType;	// must be 1
	WORD	icoResourceCount;	// number of images in this file
};
typedef struct IconFileHeader ICONFILEHEADER;


class Cicon 
{
public:
  Cicon();
  ~Cicon();

  HICON GeefHICON();

  bool LoadFromFile(const char *filename,int iconx = 0,int icony = 0);
private:
  HICON LoadIconFile(const char *filename,int iconx, int icony);
  HICON ReadIconFile(const char *filename,int iconx = 0,int icony = 0);

private:
  HICON m_icon;
};
