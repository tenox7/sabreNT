/*
    Sabre Flight Simulator
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
 * File   : dibsect.h                            *
 * Date   : December, 1997                       *
 * Author : Dan Hammer                           *
 * Based on FastWin.hpp by John De Goes          *
 *************************************************/
#ifndef __dibsect_h
#define __dibsect_h

#include <math.h>
#include <windows.h>
#include <windowsx.h>

class DibSect
{
protected:

 struct bm_info_str
 {
   BITMAPINFOHEADER bmiHeader;
   RGBQUAD          bmiColors [ 256 ];
 } *bm_info;

 struct pal_info_str
 {
   WORD         palVersion;
   WORD         palNumEntries;
   PALETTEENTRY palPalEntry [ 256 ];
 } *pal_info;

 HBITMAP    bm_handle;
 BYTE       *bits;
 int        syspal_static;
 static int syspal_index[];
 static COLORREF mono_colors[];
 static COLORREF old_colors[];

public:

  DibSect();

  ~DibSect();

  void init(DWORD width, DWORD height,
            RGBQUAD *pal = NULL,
            HWND hWnd = NULL);

  unsigned char *getBits()
  {
      return (bits);
  }
  void blitBits(HWND = NULL);
  void makeColors(HWND = NULL);
  HBITMAP createDIB(RGBQUAD *, HWND);
  int wantsActivateMessage()
  {
	return (syspal_static != 0);
  }
  void activate(BOOL active, HWND hWnd = NULL);
 
};

#endif
