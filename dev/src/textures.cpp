/*
    Sabre Fighter Plane Simulator
    Copyright (C) 1997	Dan Hammer

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 1, or (at your option)
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

/*************************************************
 * Sabre Fighter Plane Simulator                 *
 * File   : about.cpp                            *
 * Date   : December, 1997                       *
 * Author : Dan Hammer                           *
 * Port to Windows NT/95                         *
 *************************************************/
#include <windows.h>
#include <stdio.h>
#include <math.h>
#include "iostream.h"
#include "fstream.h"
#include "simerr.h"
#include "simfile.h"
#include "txtrmap.h"
#include "resource.h"
#include "about.h"

static int curTexture = 0;
extern char *build_libpath(char *);
extern HBITMAP GetTextureBitmap(int);
extern int GetMaxTextures();
extern HANDLE hinstance;
static void showCurrentTexture(HDC hdc, HWND hdlg);
static void writeCurrentTexture(HWND hdlg);
static PBITMAPINFO CreateBitmapInfoStruct(HWND hwnd, HBITMAP hBmp);
static void CreateBMPFile(HWND hwnd, LPTSTR pszFile, PBITMAPINFO pbi, 
                  HBITMAP hBMP, HDC hDC);


BOOL CALLBACK TexturesDialogProc(HWND hdlg, UINT msg, WPARAM wparam,
                              LPARAM)
{
	switch (msg)
	{

	case WM_INITDIALOG:
		break;

	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC dc = BeginPaint(hdlg,&ps);
			showCurrentTexture(dc,hdlg);
			EndPaint(hdlg,&ps);
		}
		break;

	case WM_COMMAND:
		switch (wparam)
		{
		case IDOK:
			EndDialog(hdlg,0);
			return TRUE;

		case IDC_TXTR_NEXT:
			if (curTexture < GetMaxTextures() - 1)
				curTexture++;
			InvalidateRect(hdlg,NULL,TRUE);
			return TRUE;

		case IDC_TXTR_PREV:
			if (curTexture > 0)
				curTexture--;
			InvalidateRect(hdlg,NULL,TRUE);
			return TRUE;

		case IDC_TXTR_FILE:
			writeCurrentTexture(hdlg);
			return TRUE;

		}
	}
	return FALSE;
}

void showCurrentTexture(HDC hdc,HWND hdlg)
{
HBITMAP hm;
BITMAP  bm;
HDC     dc;
TextrMap *tmap;
char info[256];

	if (map_man == NULL)
		return;
	tmap = map_man->get_map_ptr(curTexture);
	sprintf(info,"%d",curTexture);			
	if (tmap && tmap->size > 16)
	{
		sprintf(info,"%s %dx%d %d bytes",
					tmap->id,tmap->map_w,tmap->map_h,
					tmap->size);
		hm = (HBITMAP) tmap->createDIB((long)hdc);
		if (hm != NULL)
		{
			memset(&bm,sizeof(BITMAP),0);
			if (GetObject(hm,sizeof(bm),&bm))
			{
				dc = CreateCompatibleDC(hdc);
				if (dc != NULL)
				{
					SelectObject(dc,hm);
					BitBlt(hdc,0,0,bm.bmWidth,bm.bmHeight,dc,0,0,SRCCOPY);
					DeleteDC(dc);
				}
			}
			tmap->freeDIB();
		}
	}
	SendDlgItemMessage(hdlg,IDC_TXTR_INFO,WM_SETTEXT,
									(WPARAM) 0,(LPARAM)info);
}

void writeCurrentTexture(HWND hdlg)
{
HBITMAP hm;
TextrMap *tmap;
char filePath[100];

	if (map_man == NULL)
		return;
	HDC hdc = GetDC(hdlg);
	tmap = map_man->get_map_ptr(curTexture);
	if (tmap && tmap->size > 16)
	{
		hm = (HBITMAP) tmap->createDIB((long)hdc);
		if (hm != NULL)
		{
			PBITMAPINFO mapInfo = CreateBitmapInfoStruct(hdlg,hm);
			if (mapInfo)
			{
				sprintf(filePath,"%s.bmp",tmap->id);
				CreateBMPFile(hdlg, build_libpath(filePath), mapInfo, 
                  hm,hdc);
			}
			tmap->freeDIB();
		}
	}
	DeleteDC(hdc);
}

PBITMAPINFO CreateBitmapInfoStruct(HWND hwnd, HBITMAP hBmp)
{ 
    BITMAP bmp; 
    PBITMAPINFO pbmi; 
    WORD    cClrBits; 
	 WORD	   sizePal;

    /* Retrieve the bitmap's color format, width, and height. */ 
 
    if (!GetObject(hBmp, sizeof(BITMAP), (LPSTR)&bmp)) 
        error_jump("GetObject", hwnd); 
 
 
    /* Convert the color format to a count of bits. */ 
 
    cClrBits = (WORD)(bmp.bmPlanes * bmp.bmBitsPixel); 
 
    if (cClrBits == 1) 
        cClrBits = 1; 
    else if (cClrBits <= 4) 
        cClrBits = 4; 
    else if (cClrBits <= 8) 
        cClrBits = 8; 
    else if (cClrBits <= 16) 
        cClrBits = 16; 
    else if (cClrBits <= 24) 
        cClrBits = 24; 
    else 
        cClrBits = 32; 
 
    /* 
     * Allocate memory for the BITMAPINFO structure. (This structure 
     * contains a BITMAPINFOHEADER structure and an array of RGBQUAD data 
     * structures.) 
     */ 
	 sizePal = (WORD) (pow(2.0,(double)cClrBits));
    if (cClrBits != 24) 
         pbmi = (PBITMAPINFO) LocalAlloc(LPTR, 
                    sizeof(BITMAPINFOHEADER) + 
                    sizeof(RGBQUAD) * (sizePal)); 
 
    /* 
     * There is no RGBQUAD array for the 24-bit-per-pixel format. 
     */ 
 
    else 
         pbmi = (PBITMAPINFO) LocalAlloc(LPTR, 
                    sizeof(BITMAPINFOHEADER)); 
 
 
 
    /* Initialize the fields in the BITMAPINFO structure. */ 
 
    pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER); 
    pbmi->bmiHeader.biWidth = bmp.bmWidth; 
    pbmi->bmiHeader.biHeight = bmp.bmHeight; 
    pbmi->bmiHeader.biPlanes = bmp.bmPlanes; 
    pbmi->bmiHeader.biBitCount = bmp.bmBitsPixel; 
    if (cClrBits < 24) 
        pbmi->bmiHeader.biClrUsed = sizePal; 
 
 
    /* If the bitmap is not compressed, set the BI_RGB flag. */ 
 
    pbmi->bmiHeader.biCompression = BI_RGB; 
 
    /* 
     * Compute the number of bytes in the array of color 
     * indices and store the result in biSizeImage. 
     */ 
 
    pbmi->bmiHeader.biSizeImage = (pbmi->bmiHeader.biWidth + 7) /8 
                                  * pbmi->bmiHeader.biHeight 
                                  * cClrBits; 
 
    /* 
     * Set biClrImportant to 0, indicating that all of the 
     * device colors are important. 
     */ 
 
    pbmi->bmiHeader.biClrImportant = 0; 
 
    return pbmi; 
 
} 
 
#define MAXWRITE 1024L

void CreateBMPFile(HWND hwnd, LPTSTR pszFile, PBITMAPINFO pbi, 
                  HBITMAP hBMP, HDC hDC) 
{ 
 
    HANDLE hf;                  /* file handle */ 
    BITMAPFILEHEADER hdr;       /* bitmap file-header */ 
    PBITMAPINFOHEADER pbih;     /* bitmap info-header */ 
    LPBYTE lpBits;              /* memory pointer */ 
    DWORD dwTotal;              /* total count of bytes */ 
    DWORD cb;                   /* incremental count of bytes */ 
    BYTE *hp;                   /* byte pointer */ 
    DWORD dwTmp; 
 
 
    pbih = (PBITMAPINFOHEADER) pbi; 
    lpBits = (LPBYTE) GlobalAlloc(GMEM_FIXED, pbih->biSizeImage);
    if (!lpBits) 
         error_jump("GlobalAlloc", hwnd); 
 
    /* 
     * Retrieve the color table (RGBQUAD array) and the bits 
     * (array of palette indices) from the DIB. 
     */ 
 
    if (!GetDIBits(hDC, hBMP, 0, (WORD) pbih->biHeight, 
                   lpBits, pbi, DIB_RGB_COLORS)) 
        error_jump("GetDIBits", hwnd); 
 
    /* Create the .BMP file. */ 
 
    hf = CreateFile(pszFile, 
                   GENERIC_READ | GENERIC_WRITE, 
                   (DWORD) 0, 
                   (LPSECURITY_ATTRIBUTES) NULL, 
                   CREATE_ALWAYS, 
                   FILE_ATTRIBUTE_NORMAL, 
                   (HANDLE) NULL); 
 
    if (hf == INVALID_HANDLE_VALUE) 
        error_jump("CreateFile", hwnd); 
 
    hdr.bfType = 0x4d42;        /* 0x42 = "B" 0x4d = "M" */ 
 
    /* Compute the size of the entire file. */ 
 
    hdr.bfSize = (DWORD) (sizeof(BITMAPFILEHEADER) + 
                 pbih->biSize + pbih->biClrUsed 
                 * sizeof(RGBQUAD) + pbih->biSizeImage); 
 
    hdr.bfReserved1 = 0; 
    hdr.bfReserved2 = 0; 
 
    /* Compute the offset to the array of color indices. */ 
 
    hdr.bfOffBits = (DWORD) sizeof(BITMAPFILEHEADER) + 
                    pbih->biSize + pbih->biClrUsed 
                    * sizeof (RGBQUAD); 
 
    /* Copy the BITMAPFILEHEADER into the .BMP file. */ 
 
    if (!WriteFile(hf, (LPVOID) &hdr, sizeof(BITMAPFILEHEADER), 
       (LPDWORD) &dwTmp, (LPOVERLAPPED) NULL)) 
       error_jump("WriteFile", hwnd); 
 
    /* Copy the BITMAPINFOHEADER and RGBQUAD array into the file. */ 
 
    if (!WriteFile(hf, (LPVOID) pbih, sizeof(BITMAPINFOHEADER) 
                  + pbih->biClrUsed * sizeof (RGBQUAD), 
                  (LPDWORD) &dwTmp, (LPOVERLAPPED) NULL)) 
       error_jump("WriteFile", hwnd); 
 
    /* Copy the array of color indices into the .BMP file. */ 
 
    dwTotal = cb = pbih->biSizeImage; 
    hp = lpBits; 
    while (cb > MAXWRITE)  { 
            if (!WriteFile(hf, (LPSTR) hp, (int) MAXWRITE, 
                          (LPDWORD) &dwTmp, (LPOVERLAPPED) NULL)) 
                error_jump("WriteFile", hwnd); 
            cb-= MAXWRITE; 
            hp += MAXWRITE; 
    } 
    if (!WriteFile(hf, (LPSTR) hp, (int) cb, 
         (LPDWORD) &dwTmp, (LPOVERLAPPED) NULL)) 
           error_jump("WriteFile", hwnd); 
 
    /* Close the .BMP file. */ 
 
    if (!CloseHandle(hf)) 
           error_jump("CloseHandle", hwnd); 
 
    /* Free memory. */

    GlobalFree((HGLOBAL)lpBits);
} 
