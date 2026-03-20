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
 * Version: 0.1                                  *
 * File   : dibsect.cpp                          *
 * Date   : December, 1997                       *
 * Author : Dan Hammer                           *
 * Based on FastWin.cpp by John De Goes          *
 *************************************************/

#include <stdio.h>
#include "dibsect.h"
#include "simfile.h"

#define NumSysColors 19
#define rgbBlack RGB(0,0,0)
#define rgbWhite RGB(255,255,255)

int DibSect::syspal_index[NumSysColors] = 
{
	COLOR_ACTIVEBORDER,
	COLOR_ACTIVECAPTION,
	COLOR_APPWORKSPACE,
	COLOR_BACKGROUND,
	COLOR_BTNFACE,
	COLOR_BTNSHADOW,
	COLOR_BTNTEXT,
	COLOR_CAPTIONTEXT,
	COLOR_GRAYTEXT,
	COLOR_HIGHLIGHT,
	COLOR_HIGHLIGHTTEXT,
	COLOR_INACTIVEBORDER,
	COLOR_INACTIVECAPTION,
	COLOR_MENU,
	COLOR_MENUTEXT,
	COLOR_SCROLLBAR,
	COLOR_WINDOW,
	COLOR_WINDOWFRAME,
	COLOR_WINDOWTEXT
};

COLORREF DibSect::mono_colors[NumSysColors] = 
{
	rgbBlack,
	rgbWhite,
	rgbWhite,
	rgbWhite,
	rgbWhite,
	rgbBlack,
	rgbBlack,
	rgbBlack,
	rgbBlack,
	rgbBlack,
	rgbWhite,
	rgbWhite,
	rgbWhite,
	rgbWhite,
	rgbBlack,
	rgbWhite,
	rgbWhite,
	rgbBlack,
	rgbBlack
};

COLORREF DibSect::old_colors[NumSysColors];

DibSect::DibSect()
  : bm_info(NULL),
    bits(NULL),
    pal_info(NULL),
    bm_handle(NULL)
{
	syspal_static = 0;
}


DibSect::~DibSect()
{
    if (bm_info != NULL)
      delete bm_info;
    if (pal_info != NULL)
      delete pal_info;
     DeleteObject (bm_handle);
	if (syspal_static)
	{
		HDC hdc = GetDC(NULL);
		SetSystemPaletteUse(hdc, SYSPAL_STATIC);
		SetSysColors(NumSysColors, syspal_index, old_colors);
		ReleaseDC(NULL,hdc);
	}
}

void DibSect::init ( DWORD width, DWORD height, RGBQUAD *pal,
                     HWND hw )
{
   HDC dc;
   HWND hwnd;
   sim_printf("dibsect init %dx%d\n", width, height);
   if (hw == NULL)
      hwnd = GetActiveWindow();
   else
      hwnd = hw;
   if (bm_info != NULL)
      delete bm_info;
   if (pal_info != NULL)
      delete pal_info;
   if (bm_handle != NULL)
      DeleteObject(bm_handle);

   dc = GetDC(NULL);
   sim_printf("dibsect GetDC=%p GetSystemPaletteUse=%d\n", dc, GetSystemPaletteUse(dc));
   if (GetSystemPaletteUse(dc) == SYSPAL_NOSTATIC)
      syspal_static = 0;
   else
   {
      int i;
      syspal_static = 1;
      for (i=0; i<NumSysColors; i++)
         old_colors[i] = GetSysColor(syspal_index[i]);
   }
   ReleaseDC(NULL,dc);
   sim_printf("dibsect syspal_static=%d\n", syspal_static);

   bm_info  = new bm_info_str;
   pal_info = NULL;

   bm_info->bmiHeader.biSize          = sizeof (BITMAPINFOHEADER);
   bm_info->bmiHeader.biWidth         = width;
   bm_info->bmiHeader.biHeight        = -abs (height);
   bm_info->bmiHeader.biPlanes        = 1;
   bm_info->bmiHeader.biBitCount      = 8;
   bm_info->bmiHeader.biCompression   = BI_RGB;
   bm_info->bmiHeader.biSizeImage     = NULL;
   bm_info->bmiHeader.biXPelsPerMeter = NULL;
   bm_info->bmiHeader.biYPelsPerMeter = NULL;
   bm_info->bmiHeader.biClrUsed       = 256;
   bm_info->bmiHeader.biClrImportant  = 256;

   createDIB(pal,hw);
   sim_printf("dibsect init done bm_handle=%p bits=%p\n", bm_handle, bits);
}


HBITMAP DibSect::createDIB(RGBQUAD *pal, HWND hw)
{
   int i;
   HPALETTE pal_handle;
   sim_printf("createDIB hw=%p\n", hw);
   HDC hdc = GetDC(hw);
   sim_printf("createDIB hdc=%p\n", hdc);

   if (pal_info == NULL)
   {
      pal_info = new pal_info_str;
      pal_info->palVersion = 0x300;
      pal_info->palNumEntries = 256;
   }

	//*** For SYSPAL_NOSTATIC, just copy the color table into
	//*** a PALETTEENTRY array and replace the first and last entries
	//*** with black and white
	   for (i=0;i<256;i++)
		{
			pal_info->palPalEntry [i].peRed   = pal[i].rgbRed;
			pal_info->palPalEntry [i].peGreen = pal[i].rgbGreen;
			pal_info->palPalEntry [i].peBlue  = pal[i].rgbBlue;
			pal_info->palPalEntry [i].peFlags = PC_NOCOLLAPSE;
		}
		//*** Make sure the last entry is white
		//*** This may replace an entry in the array!
		pal_info->palPalEntry[255].peRed = 255;
		pal_info->palPalEntry[255].peGreen = 255;
		pal_info->palPalEntry[255].peBlue = 255;
		pal_info->palPalEntry[255].peFlags = 0;
		//*** And the first is black
		//*** This may replace an entry in the array!
		pal_info->palPalEntry[0].peRed = 0;
		pal_info->palPalEntry[0].peGreen = 0;
		pal_info->palPalEntry[0].peBlue = 0;
		pal_info->palPalEntry[0].peFlags = 0;

   sim_printf("createDIB CreatePalette\n");
   pal_handle = CreatePalette ((tagLOGPALETTE *)pal_info);
   sim_printf("createDIB pal_handle=%p SelectPalette\n", pal_handle);
   SelectPalette(hdc,pal_handle,FALSE);
   sim_printf("createDIB RealizePalette\n");
   RealizePalette(hdc);
   DeleteObject(pal_handle);

   for (i=0;i<256;i++)
   {
		bm_info->bmiColors[i].rgbRed = pal_info->palPalEntry[i].peRed;
		bm_info->bmiColors[i].rgbGreen = pal_info->palPalEntry[i].peGreen;
		bm_info->bmiColors[i].rgbBlue = pal_info->palPalEntry[i].peBlue;
   }

   if (bm_handle != NULL)
      DeleteObject(bm_handle);
   sim_printf("createDIB CreateDIBSection %dx%d\n", bm_info->bmiHeader.biWidth, bm_info->bmiHeader.biHeight);
   bm_handle = CreateDIBSection (hdc,(BITMAPINFO *) bm_info, DIB_RGB_COLORS,
                              (VOID * *) &bits, NULL, NULL);
   sim_printf("createDIB bm_handle=%p bits=%p\n", bm_handle, bits);
   ReleaseDC(hw,hdc);
   return (bm_handle);
}

void DibSect::blitBits(HWND hw)
{
   HWND hwnd;
   HDC  hdc;
   RECT r;

   if (bm_handle == NULL)
      return;

   if (hw == NULL)
      hwnd = GetActiveWindow();
   else
      hwnd = hw;

   if (hwnd == NULL)
      return;

   hdc = GetDC(hwnd);
   GetClientRect(hwnd,&r);

   HDC context = CreateCompatibleDC (0);
   HBITMAP defbm = SelectBitmap (context,bm_handle);

   BitBlt ( hdc,
                   0, // x-coordinate of upper-left corner of destination rectangle
                   0, // y-coordinate of upper-left corner of destination rectangle
          r.right - r.left, // width of destination rectangle
          r.bottom - r.top, // height of destination rectangle
               context, // handle of source device context
                   0, // x-coordinate of upper-left corner of source rectangle
                   0, // y-coordinate of upper-left corner of source rectangle
              SRCCOPY // raster operation code
          );

   SelectBitmap (context,defbm);
   DeleteDC (context);
   DeleteObject (defbm);
   ReleaseDC (hwnd,hdc);
}

void DibSect::makeColors(HWND hw)
{
   HWND hwnd;
   if (pal_info == NULL)
      return;
   if (hw)
      hwnd = hw;
   else
      hwnd = GetActiveWindow ();

   HDC hdc = GetDC (hwnd);
   HPALETTE pal_handle;

   // Create, select, and realize a palette:
   pal_handle = CreatePalette((tagLOGPALETTE *)pal_info);
   SelectPalette(hdc,pal_handle,FALSE);
   RealizePalette(hdc);

   // Release the screen device context:
   ReleaseDC (hwnd,hdc);

   // Delete the palette handle:
   DeleteObject (pal_handle);
}

void DibSect::activate(BOOL fActive, HWND hWnd)
{
	HDC hdc;

	if (!wantsActivateMessage())
		return;

	hdc = GetDC(NULL);
	if (fActive && GetSystemPaletteUse(hdc) == SYSPAL_STATIC)
	{
		SetSystemPaletteUse(hdc, SYSPAL_NOSTATIC);
		SetSysColors(NumSysColors, syspal_index, mono_colors);
		makeColors(hWnd);
	}
	else if (!fActive)
	{
		SetSystemPaletteUse(hdc, SYSPAL_STATIC);
		SetSysColors(NumSysColors, syspal_index, old_colors);
	}
	ReleaseDC(NULL,hdc);
}
