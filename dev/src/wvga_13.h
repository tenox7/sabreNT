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
 * File   : wvga_13.h                            *
 * Date   : December, 1997                       *
 * Author : Dan Hammer                           *
 * Port to Windows NT/95                         *
 *************************************************/
 #ifndef __wvga_13_h
 #define __wvga_13_h

 #include "vga_13.h"

 void   wvga_params(int ini_screen_width, int ini_screen_height);
 void   wvga_init(HWND hw, char *palette_file);
 void   wvga_palette(char *palette_file);
 void   wvga_realize_palette();
 int	  wvga_wants_activate_message(void);
 void	  wvga_activate(int activate);
 RGBQUAD *wvga_get_rgb_colors(void);
 void	  wvga_release(void);
 #endif