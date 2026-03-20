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
#include "resource.h"
#include "about.h"

extern char *build_libpath(char *);
extern HANDLE hinstance;
extern const char *wVERSION;
void doAboutBackground(HWND hdl, HDC hdc);

BOOL CALLBACK AboutDialogProc(HWND hdlg, UINT msg, WPARAM wparam,
                              LPARAM)
{
	switch (msg)
	{

	case WM_INITDIALOG:
		 SendDlgItemMessage(hdlg,IDSTATIC_VERSION,WM_SETTEXT,
									(WPARAM) 0,(LPARAM) wVERSION);
		break;

	case WM_ERASEBKGND:
		doAboutBackground(hdlg,(HDC)wparam);
		return TRUE;

	case WM_COMMAND:
		switch (wparam)
		{
			case IDOK:
			EndDialog(hdlg,0);
			return TRUE;
		}
	}
	return FALSE;
}

void doAboutBackground(HWND , HDC hdc)
{
HBITMAP hm;
BITMAP  bm;
HDC     dc;

   hm = LoadBitmap((HINSTANCE)hinstance,MAKEINTRESOURCE(ABOUT_BKG));
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
            DeleteObject(hm);
         }
      }
   }
}
