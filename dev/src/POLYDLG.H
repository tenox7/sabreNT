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
 * File   : polydlg.h                            *
 * Date   : December, 1997                       *
 * Author : Dan Hammer                           *
 * Port to Windows NT/95                         *
 *************************************************/
#ifndef __polydlg_h
#define __polydlg_h

#include "vmath.h"

struct rpoint 
{
   static  int instance_count;
   R_3DPoint pnt;
   rpoint *next;
   rpoint(const R_3DPoint &p)
   {
      pnt = p;
      next = NULL;
      instance_count++;
   }
   ~rpoint()
   {
      instance_count--;
   }
};

extern rpoint *rp_head;

BOOL CALLBACK PolyDialogProc(HWND, UINT, WPARAM, LPARAM);

#endif
