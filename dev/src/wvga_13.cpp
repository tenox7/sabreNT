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
 * File   : wvga_13.cpp                          *
 * Date   : December, 1997                       *
 * Author : Dan Hammer                           *
 * Port to Windows NT/95                         *
 *************************************************/
#include <dos.h>
#include <direct.h>
#include <math.h>
#include <conio.h>
#include <time.h>
#include <stdarg.h>
#include <signal.h>
#include <stdio.h>

#include <windows.h>
#include <commdlg.h>
#include <iostream.h>

#include "wvga_13.h"
#include "dibsect.h"
#include "simfile.h"
#include "clip.h"
#include "ddaline.h"

Rect     cliprect;
SimFont *g_font = NULL;
int SCREEN_WIDTH = 320;
int SCREEN_PITCH = 320;
int SCREEN_HEIGHT = 200;
int MXSCREEN_WIDTH = 320;
int MXSCREEN_HEIGHT = 200;
float aspect_ratio = 1.0;
int   VGAMODE =        0;
int   MAX_X =        319;
int   MAX_Y =        199;
int   N_COLORS =     256;
int   PALETTE_SIZE = 768;

int   wvga_palette_flag = 1;

static unsigned char *xbuffer;
static unsigned char *buffer_ptr;
static unsigned char *screen_ptr;

static DibSect dib_sect;
static RGBQUAD rgb_colors [ 256 ];
static HWND hwnd;

void wvga_params(int ini_screen_width, int ini_screen_height)
{
   VGAMODE       =   0;
   if (ini_screen_width > 0)
      SCREEN_WIDTH = ini_screen_width;
   else
      SCREEN_WIDTH  =  320;
   if (ini_screen_height > 0)
      SCREEN_HEIGHT = ini_screen_height;
   else
      SCREEN_HEIGHT = 200;
   int dv = SCREEN_WIDTH / 8;
   SCREEN_WIDTH = 8 * dv;
	SCREEN_PITCH = SCREEN_WIDTH;
   MAX_X         = SCREEN_WIDTH - 1;
   MAX_Y         = SCREEN_HEIGHT - 1;
   MXSCREEN_HEIGHT = SCREEN_HEIGHT;
   MXSCREEN_WIDTH  = SCREEN_WIDTH;
   N_COLORS      = 256;
   PALETTE_SIZE  = 768;
   aspect_ratio  = 1.0;
   cliprect.topLeft.x = 0;
   cliprect.topLeft.y = 0;
   cliprect.botRight.x = MAX_X;
   cliprect.botRight.y = MAX_Y;
   sim_printf("screen size: %d %d\n",SCREEN_WIDTH,SCREEN_HEIGHT);
}

void wvga_init(HWND hw, char *palette_file)
{
   hwnd = hw;
   sim_printf("wvga_init hw=%p palette=%s\n", hw, palette_file ? palette_file : "NULL");
   if (palette_file)
      wvga_palette(palette_file);
   sim_printf("wvga_init calling dib_sect.init\n");
   dib_sect.init(SCREEN_WIDTH,SCREEN_HEIGHT,rgb_colors,hwnd);
   buffer_ptr = dib_sect.getBits();
   xbuffer = buffer_ptr;
   screen_ptr = buffer_ptr;
   sim_printf("wvga_init done buffer=%p\n", buffer_ptr);
}

void wvga_release(void)
{
}

void wvga_palette(char *palette_file)
{
   PALETTEENTRY system_palette[256];
   sim_printf("wvga_palette hwnd=%p\n", hwnd);
   HDC dc = GetDC(hwnd);
   sim_printf("wvga_palette dc=%p\n", dc);
   UINT r = GetSystemPaletteEntries(dc,0,256,system_palette);
   if (r)
   {
      for (int i=0;i<256;i++)
      {
         rgb_colors[i].rgbRed = system_palette[i].peRed;
         rgb_colors[i].rgbGreen = system_palette[i].peGreen;
         rgb_colors[i].rgbBlue = system_palette[i].peBlue;
         rgb_colors[i].rgbReserved = 0;
      }
   }
   ReleaseDC (hwnd,dc);
   wvga_palette_flag = 1;
   read_palette_file(palette_file);
   wvga_palette_flag = 0;
}

void wvga_realize_palette()
{
  dib_sect.makeColors(hwnd);
}

RGBQUAD *wvga_get_rgb_colors(void)
{
	return rgb_colors;
}

void set_rgb_value(int n, int red, int green, int blue)
{
   if (wvga_palette_flag && n >= 0 && n < 256)
   {
      float r,g,b;

      r = ((float) red) / 63.0 * 255.0;
      g = ((float) green) / 63.0 * 255.0;
      b = ((float) blue) / 63.0 * 255.0;
      rgb_colors[n].rgbRed = (BYTE) r ;
      rgb_colors[n].rgbGreen = (BYTE) g;
      rgb_colors[n].rgbBlue = (BYTE) b;
      rgb_colors[n].rgbReserved = 0;
   }
}

void get_rgb_value(int n, char *red, char *green, char *blue)
{
   if (n >= 0 && n < 256)
   {
      *red = (char) (((float)rgb_colors[n].rgbRed) / 255.0 * 63.0);
      *green = (char)(((float) rgb_colors[n].rgbGreen) / 255.0 * 63.0);
      *blue = (char) (((float)rgb_colors[n].rgbBlue) / 255.0 * 63.0);
   }
}

void clear_scr(int color, int row)
{
   int n = (row + 1) * SCREEN_WIDTH;
   memset(buffer_ptr,color,n);
}

void clear_scr(int color)
{
   clear_scr(color,MAX_Y);
}

void fill_rect(Rect &r, int color)
{
  if (valid_rect(r))
    {
      Rect r0 = r;
      cliprect2rect(cliprect,r0);
      int h = RHEIGHT(r0);
      int w = RWIDTH(r0);
      unsigned char *ptr = 
         buffer_ptr + r0.left() + (r0.top() * SCREEN_WIDTH);
      for (int i=0;i<h;i++)
      {
         memset(ptr,(char)color,w);
         ptr += SCREEN_WIDTH;
      }
    }
      
}

void blit_buff()
{
   dib_sect.blitBits(hwnd);
}

void wvga_line(int x1, int y1, int x2, int y2, int color)
{
   DDALine daline(x1,y1,x2,y2);
   do
      {
         putpixel(daline.X(),daline.Y(),color);
      } while (daline.step());
}

void b_linedraw(int x1, int y1, int x2, int y2, int color, Rect *bounds)
{
  if (bounds)
    {
      int points[4];
      points[0] = x1;
      points[1] = y1;
      points[2] = x2;
      points[3] = y2;
      if (!ch_clip(points,bounds))
	return;
      x1 = points[0];
      y1 = points[1];
      x2 = points[2];
      y2 = points[3];
    }
   wvga_line(x1,y1,x2,y2,color);
}

void putpixel(int x, int y, int color)
{
   if (x >= 0 && x<= MAX_X && y >= 0 && y <= MAX_Y)
   {
      *(buffer_ptr + (y * SCREEN_WIDTH) + x) = (char) color;
   }
}

void h_line(int x, int y, int len, int color)
{
   register unsigned char *bf = buffer_ptr + (y * SCREEN_WIDTH) + x;
   while (len--)
   {
      *bf++ = (char ) color;
   }
}

void v_line(int x, int y, register int len, int color)
{
  register unsigned char *bf = buffer_ptr + (y * SCREEN_WIDTH) + x;
  while(len--)
    {
      *bf = (char ) color;
      bf += SCREEN_WIDTH;
    }
}

void set_palette(int , int , char *)
{
   /*
   char *pptr = palette + (startcolor * 3);
   for (int i=0;i<count;i++)
   {
      set_rgb_value(i,(int) *pptr, (int) *pptr+1, (int) *pptr+2);
      pptr += 3;
   }
   */
   /*
   dib_sect.newPalette(rgb_colors,hwnd);
   buffer_ptr = dib_sect.getBits();
   xbuffer = buffer_ptr;
   screen_ptr = buffer_ptr;
   */
}

void get_palette(int startcolor, int count, char *palette)
{
  char *pptr = palette + (startcolor * 3);
  for (int i=0;i<count;i++)
  {
      get_rgb_value(i,pptr,pptr+1,pptr+2);
      pptr += 3;
  }
}

unsigned char *lock_xbuff()
{
  return (xbuffer);
}

void free_xbuff()
{
}

void vga13_suspend()
{
}

void vga13_resume()
{
}

void vga13_begin_scene()
{
}

void vga13_end_scene()
{
}

int wvga_wants_activate_message(void)
{
	return dib_sect.wantsActivateMessage();
}

void wvga_activate(int activate)
{
	dib_sect.activate(activate);
}
