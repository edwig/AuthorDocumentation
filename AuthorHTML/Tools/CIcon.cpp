//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    CIcon.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Handles an icon directly from an image
//
#include "StdAfx.h"
#include "CIcon.h"
#define WIN32_LEAN_AND_MEAN 
#include <afxwin.h>

Cicon::Cicon()
      :m_icon(NULL)
{
}

Cicon::~Cicon()
{
  if (m_icon)
    DestroyIcon(m_icon);
}


bool
Cicon::LoadFromFile(const char *filename,int cx,int cy)
{
  HICON icon = LoadIconFile(filename,cx,cy);
  if (icon)
  {
    if (m_icon)
      DestroyIcon(m_icon);
    m_icon = icon;
    return true;
  }

  return  false;
}

HICON
Cicon::LoadIconFile(const char *filename,int iconx, int icony)
{
  if (!iconx)
	  iconx = GetSystemMetrics(SM_CXICON);
  if (!icony)
  	icony = GetSystemMetrics(SM_CYICON);
  
  return (HICON)LoadImage(AfxGetInstanceHandle(),
                          filename,
                          IMAGE_ICON,
                          iconx,icony,
                          //16,16,
                          LR_LOADFROMFILE );
}



HICON
Cicon::ReadIconFile(const char *filename,int iconx, int icony)
{
	int				file;
	int				i, j;
	int				offset;
	int				rc;
	HICON				hicon = (HICON)NULL;
	HDC				hdc;
	int				ncolors;
	OFSTRUCT			reopen;
	ICONFILEHEADER	header;
	ICONDATA			icon, best;
	char				*buffer;
	BITMAPINFO		*bmi;
	char				*cp, *cp2;
	char				mask[512];
	char				*iconDIB;

	/*
	**	Get some system values
	*/
  CDC *pDC = CWnd::GetDesktopWindow()->GetDC();
  hdc = pDC->m_hDC;
	if (hdc == (HDC)NULL)
	{
		CWnd::GetDesktopWindow()->ReleaseDC(pDC);
    return (HICON)NULL;
	}
  if (!iconx)
	  iconx = GetSystemMetrics(SM_CXICON);
  if (!icony)
  	icony = GetSystemMetrics(SM_CYICON);
	ncolors = GetDeviceCaps(hdc, NUMCOLORS);
  ncolors = 32;
  CWnd::GetDesktopWindow()->ReleaseDC(pDC);

	/*
	**	Allocate some needed buffers
	*/
	buffer = (char*)malloc(4712);
	if (buffer == NULL)
	{
		//ErrorBox("ReadIconFile(): Unable to allocate memory for icon buffer");
		return (HICON)NULL;
	}
	bmi = (BITMAPINFO *)buffer;
	iconDIB = (char*)malloc(4096);
	if (iconDIB == NULL)
	{
		//ErrorBox("ReadIconFile(): Unable to allocate memory for icon DIB");
		free(buffer);
		return (HICON)NULL;
	}

	file = OpenFile((LPSTR)filename, (LPOFSTRUCT)&reopen, OF_READ | OF_SHARE_DENY_NONE);

	if (file >= 0)
	{
		/*
		** Read in Icon File header
		*/
		rc = _lread(file, (char far *)&header, sizeof(header));
		if (rc == sizeof(header) && header.icoReserved == 0
			&& header.icoResourceType == 1)
		{
			if (header.icoResourceCount != 0)
			{
				/*
				** Look for the correct Icon by scanning through the directory for
				** an extact match.
				*/
				memset(&best, 0, sizeof(best));

				for (i = rc = 0 ; rc >= 0 && i < header.icoResourceCount ; ++i)
				{
					rc = _lread(file, (char far *)&icon, sizeof(ICONDATA));
					if (rc == sizeof(ICONDATA))
					{
						if (icon.width == iconx && icon.height == icony)
						{
							if (icon.colorCount == ncolors)
							{
								best = icon;
								break;
							}
							else if (icon.colorCount > best.colorCount)
							{
								if (icon.colorCount < ncolors)
									best = icon;
							}
							else
							{
								if (icon.colorCount > ncolors)
									best = icon;
							}
						}
					}
					else
					{
						//ErrorBox("ReadIconFile(): Error reading icon directory");
						rc = -1;
					}
				}

				if (rc >= 0)
				{
					if (best.width != 0)
					{
						_llseek(file, best.icoDIBOffset, 0);
						rc = _lread(file, (LPSTR)buffer, (int)best.icoDIBSize);
						if (rc == (int)best.icoDIBSize)
						{
							offset = sizeof(BITMAPINFO) + sizeof(RGBQUAD) * (best.colorCount - 1);
							bmi->bmiHeader.biSizeImage -= best.width * best.height / 8;

							if (bmi->bmiHeader.biBitCount == 1)
							{
								/*
								** Invert Icon image - only if monochrome
								**		rc  == line width in bytes
								**		cp  == pointer to DIB from file
								**		cp2 == pointer to proper line in iconDIB
								*/
								rc = best.width / 8 * bmi->bmiHeader.biPlanes
									* bmi->bmiHeader.biBitCount;
								cp = buffer + offset;
								cp2 = iconDIB + (best.height - 1) * rc;

								for (j = 0 ; j < best.height ; ++j)
								{
									memcpy(cp2, cp, rc);
									cp += rc;
									cp2 -= rc;
								}
							}

							/*
							** Invert Icon mask
							**		rc  == line width in bytes
							**		cp  == pointer to mask DIB from file
							**		cp2 == pointer to proper line in mask
							*/
							rc = best.width / 8;
							cp = buffer + offset + (int)bmi->bmiHeader.biSizeImage;
							cp2 = mask + (best.height - 1) * rc;

							for (j = 0 ; j < best.height ; ++j)
							{
								memcpy(cp2, cp, rc);
								cp += rc;
								cp2 -= rc;
							}

							if (bmi->bmiHeader.biBitCount > 1)
							{
								HBITMAP	hbm;
								BITMAP	bm;

								/*
								**	Create the DIB portion of the bitmap
								*/
								bmi->bmiHeader.biHeight = best.height;
								bmi->bmiHeader.biWidth = best.width;

								hbm = CreateDIBitmap(hdc, (BITMAPINFOHEADER FAR *)bmi,
									CBM_INIT, (LPSTR)buffer + offset,
									(BITMAPINFO FAR *)bmi, DIB_RGB_COLORS);

								if (hbm != (HBITMAP)NULL)
								{
									if (GetObject(hbm, sizeof(bm), (LPSTR)&bm)
										== sizeof(bm))
									{
										rc = bm.bmWidthBytes * bm.bmPlanes * bm.bmHeight;
										if (GetBitmapBits(hbm, rc, (LPSTR)iconDIB) == rc)
										{
											bmi->bmiHeader.biPlanes = bm.bmPlanes;
											bmi->bmiHeader.biBitCount = bm.bmBitsPixel;
										}
										else
										{
											//ErrorBox("ReadIconFile(): Error getting bitmap bits");
										}
									}
									else
									{
										//ErrorBox("ReadIconFile(): Error getting bitmap data");
									}
									DeleteObject(hbm);
								}
								else
								{
									//ErrorBox("ReadIconFile(): Error creating bitmap");
								}
							}

							/*
							**	Now to create the icon
							*/
							hicon = CreateIcon(AfxGetInstanceHandle(), best.width, best.height,
								(BYTE)bmi->bmiHeader.biPlanes, (BYTE)bmi->bmiHeader.biBitCount,
								(const unsigned char*)(LPSTR)mask, (const unsigned char*)(LPSTR)iconDIB);

							if (hicon != (HICON)NULL)
							{
								//CurIcon = best;
								//CurIconFile = header;
							}
						}
						else
						{
							//ErrorBox("ReadIconFile(): Error reading icon data");
						}
					}
				}
			}
			else
			{
				//ErrorBox("ReadIconFile(): Error Icon Count is 0");
			}
		}
		else
		{
			//ErrorBox("ReadIconFile(): Error Reading Icon Header");
		}
		_lclose(file);
	}
	else
	{
		//ErrorBox("ReadIconFile(): Error Opening Icon File");
	}

	/*
	**	Free up allocated memory
	*/
	free(buffer);
	free(iconDIB);

	return hicon;
}


HICON
Cicon::GeefHICON()
{
  return m_icon;
}