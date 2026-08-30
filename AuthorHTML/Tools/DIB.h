//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    DIB.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Device-Independent Bitmap
//
#pragma once

#include <AfxWin.h>

class CDIB 
{
    //DECLARE_SERIAL(CDIB)
public:
    CDIB();
    ~CDIB();

public:
    int GetNumClrEntries();                     // Number of color table entries
    BOOL Create(int width, int height);         // Create a new DIB
    BOOL Create(BITMAPINFO* pBMI, BYTE* pBits); // Create from existing mem
    void* GetPixelAddress(int x, int y);
    virtual BOOL Load(CFile* fp);               // Load from file
    virtual BOOL Load(char* pszFileName = NULL);// Load DIB from disk file
    virtual BOOL LoadBitmap(WORD wResid);
    virtual BOOL Load(WORD wResid);             // Load DIB from resource
    virtual BOOL Save(char* pszFileName = NULL);// Save DIB to disk file
    virtual BOOL Save(CFile* fp);               // Save to file
    virtual void Draw(CDC* pDC, int x, int y);
    virtual void Draw(CDC* pDC, int x, int y, int w, int h);
    virtual void Draw(CDC* pDC, int x, int y, int w, int h, int sx, int sy, int sw, int sh);


    virtual int GetWidth();                       // Image width
    virtual int GetHeight();                      // Image height
    virtual BOOL MapColorsToPalette(CPalette* pPal);
    virtual void GetRect(CRect* pRect);
    virtual void CopyBits(CDIB* pDIB, 
                          int xd, int yd,
                          int w,  int h,
                          int xs, int ys,
                          COLORREF clrTrans = 0xFFFFFFFF);
protected:
    BITMAPINFO* m_pBMI;         // Pointer to BITMAPINFO struct
    BYTE* m_pBits;              // Pointer to the bits
    BOOL  m_bMyBits;            // TRUE if DIB owns Bits memory

private:
    int DibWidth();             // Breedte in pixels
    int DibHeight();            // Hoogte  in pixels
    int StorageWidth();         // Opslag breedte in bytes
    int DibPlanes();            // Aantal kleurenlagen (planes)
    int DibBitCount();          // Aantal bits
    BITMAPINFOHEADER* GetBitmapHeaderAdress()  {return &(m_pBMI->bmiHeader); }
    BITMAPINFO*       GetBitmapInfoAddress()   {return m_pBMI;}     // Pointer to bitmap info
    void*             GetBitsAddress()         {return m_pBits;}    // Pointer to the bits
    RGBQUAD*          GetClrTabAddress()
    {
      return reinterpret_cast<LPRGBQUAD>( reinterpret_cast<BYTE*>(m_pBMI)
            + sizeof(BITMAPINFOHEADER));
    }       // Pointer to color table
};

