/*
    Sabre Fighter Plane Simulator 
    Copyright (c) 1997 Dan Hammer
    Portions Donated By Antti Barck

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
 *           Sabre Fighter Plane Simulator              *
 * Version: 0.1 Linux Operating System           *
 * File   : vga_13.C                             *
 * Date   : March, 1997                          *
 * Author : Dan Hammer                           *
 * low-level graphics routines using svgalib     *
 *                                               *
 * Now using Antti Barck's GDEV                  *
 *************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "defs.h"
#include "grafix.h"
#include "vga.h"
#include "vgagl.h"
#include "vga_13.h"
#include "gdev.h"


unsigned char *xbuffer;
unsigned char *buffer_ptr;
unsigned char *screen_ptr;
float         aspect_ratio  = 1.0;
int           SCREEN_WIDTH  = 320; 
int           SCREEN_HEIGHT = 200;
int           MAX_X         = 319;
int           MAX_Y         = 199;
int           VGAMODE       = G320x200x256;
int           PALETTE_SIZE  = 768;
int           N_COLORS      = 256;

int           window_x      = 0;
int           window_y      = 0;

int           wx0,wy0,wx1,wy1;

GraphicsContext *physicalscreen;
GraphicsContext *virtualscreen;

// #define TEST_VGA13

#ifdef TEST_VGA13
int main()
{
  printf("About to init vga lib!\n");
  init_vga_13();
  clear_scr(0);
  getchar();
  set_buffer(xbuffer);
  clear_scr(1);
  for (int i=20;i<60;i++)
    {
      *(buffer_ptr + (i * (WIDTH)) + 10) = 0;
      *(buffer_ptr + (i * (WIDTH)) + 11) = 100;
      *(buffer_ptr + (i * (WIDTH)) + 12) = 0;
      *(buffer_ptr + (i * (WIDTH)) + 13) = 50;
      *(buffer_ptr + (i * (WIDTH)) + 14) = 0;
      *(buffer_ptr + (i * (WIDTH)) + 15) = 15;
    }
  blit_buff();
  getchar();
  restorecrtmode();
  printf("Well, what happened?\n");
}

#endif

#define GDEV_SCREEN_DIM 1
#define GDEV_WINDOW_DIM 0

void init_vga_13(void)
{
  int dimx,dimy;
  int gdev_screenx;
  int gdev_screeny;

  /* switch on vgamode to get x,y dims */
  switch (VGAMODE)
    {

    case G320x200x256:
      dimx = 320;
      dimy = 200;
      break;

    case G320x240x256:
      dimx = 320;
      dimy = 240;
      break;

    case G320x400x256:
      dimx = 320;
      dimy = 400;
      break;

    case G360x480x256:
      dimx = 360;
      dimy = 480;
      break;

    case G640x480x256:
      dimx = 640;
      dimy = 480;
      break;

    case G800x600x256:
      dimx = 800;
      dimy = 600;
      break;

    case G1024x768x256:
      dimx = 102;
      dimy = x76;
      break;

    default:
      printf("svgalib mode %d not allowable, defaulting to G320x200x256\n",
	     VGAMODE);
      VGAMODE = G320x200x256;
      dimx = 320;
      dimy = 200;
      break;
    }

  /* Open up gdev */
  if (G->open(dimx,dimy,8) == 0)
    {
      gdev_screenx = G->getdimx(GDEV_SCREEN_DIM);
      gdev_screeny = G->getdimy(GDEV_SCREEN_DIM);

      /* set the window */
      if (window_x <= 0)
	window_x = gdev_screenx;
      if (window_y <= 0)
	window_y = gdev_screeny;
      if (window_x > gdev_screenx)
	window_x = gdev_screenx;
      if (window_y > gdev_screeny)
	window_y = gdev_screeny;
      wx0 = (gdev_screenx / 2) -  (window_x / 2);
      wx1 = wx0 + window_x;
      wy0 = (gdev_screeny / 2) -  (window_y / 2);
      wy1 = wy0 + window_y;
      G->setview(wx0,wy0,wx1,wy1);
      //      screen_ptr = (unsigned char *) graph_mem;
      /* always point to buffered screen */
      xbuffer    = (unsigned char *) G->getvbuf();
      screen_ptr = xbuffer;
      buffer_ptr = xbuffer;
      SCREEN_WIDTH = G->getdimx(GDEV_WINDOW_DIM);
      SCREEN_HEIGHT = G->getdimy(GDEV_WINDOW_DIM);
      MAX_X = SCREEN_WIDTH - 1;
      MAX_Y = SCREEN_HEIGHT - 1;
      aspect_ratio = calc_aspect_ratio(SCREEN_WIDTH,SCREEN_HEIGHT);
      printf("GDEV Params: %d %d %d %d %f\n",SCREEN_WIDTH,SCREEN_HEIGHT,window_x,window_y,
	 aspect_ratio);
    }
  else
    {
      printf("Error opening dev ... exiting\n");
      exit(1);
    }
}

void restorecrtmode()
{
  //  vga_setmode(TEXT);
  G->close();
}

float calc_aspect_ratio(float width, float height)
{
  float ideal_vert;
  ideal_vert = width * 2.8 / 4.0;
  return (height / ideal_vert);
}

void set_buffer(void *buff)
{
  if (buff == NULL)
    {
      buffer_ptr = screen_ptr;
    }
  else
    {
      buffer_ptr = xbuffer;
    }
}

void blit_buff()
{
  G->update();
}

void clear_scr(char color)
{
  clear_scr(color,MAX_Y);
}

void clear_scr(char color, int row)
{
  G->rect(0,0,SCREEN_WIDTH,row+1,(int) color);
}

unsigned char *xy_to_ptr(int x, int y)
{
  return (buffer_ptr + (y * WIDTH) + x);
}

void putpixel(int x, int y, int color)
{
  G->pixel(x,y,color);
}

void h_line(int x, int y, int len, int color)
{
  register unsigned char *bf = buffer_ptr + (y * WIDTH) + x;
  while (len--)
    *bf++ = color;
}

void v_line(int x, int y, register int len, int color)
{
  register unsigned char *bf = buffer_ptr + (y * WIDTH) + x;
  while(len--)
    {
      *bf = color;
      bf += WIDTH;
    }
}

void set_rgb_value(int color, char red, char green, char blue)
{
  gl_setpalettecolor(color,red,green,blue);
}

void get_rgb_value(int color, char *red, char *green, char *blue)
{
  int r,g,b;

  gl_getpalettecolor(color, &r, &g, &b);
  *red = r;
  *green = g;
  *blue = b;
}

void set_palette(int startcolor, int endcolor, char *palette)
{
  gl_setpalettecolors(startcolor,endcolor,palette);
}

void get_palette(int startcolor, int endcolor, char *palette)
{
  gl_getpalettecolors(startcolor,endcolor,palette);
}

void fade_palette_out(int startcolor, int endcolor, p_callback *pb)
{
  char rgb[PALETTE_SIZE];
  int i, j, max;
  get_palette(0,256,rgb);
  for (max = 63; max >= 0; max--)
    {
      if (!pb(max))
	break;
      for (i = startcolor; i <= endcolor; i++)
	{
	  /*
	   * Make each red, green, blue <= max
	   */
	  j = i * 3;
	  if (rgb[j] > max)
	    rgb[j] = max;
	  if (rgb[j+1] > max)
	    rgb[j+1] = max;
	  if (rgb[j+2] > max)
	    rgb[j+2] = max;
	}
      set_palette(startcolor,endcolor,rgb);
    }
}

void get_image( int x1, int y1, int x2, int y2, char *buffer )
{
  int i;
  int deltaY = ( y2-y1 ) + 1;
  int xdim = ( x2-x1 ) + 1;
  
  for ( i = 0; i < deltaY; i++ )
    {
      memcpy(( void* )&buffer[ i * xdim ], xy_to_ptr( x1,( y1 + i )), xdim );
    }
}

void scroll_one(int startRow)
{
  if (200 - startRow <= 0)
    return;
  //	put_image(0,startRow-1,319,199,xbuffer);
// scrolldown(200 - startRow);
}

void clr_palette(int start, int number)
{
  int i;
  if ((start>256) | (start<0) | ((start+number)>256))
    return;
  for (i=start;i<(start+number);i++)
    set_rgb_value(i,0,0,0);
}

int buf_size( int x1, int y1, int x2, int y2 )
{
  int size;
  size = ((x2 - x1) + 1) * ((y2 - y1) + 1);
  return( size );
}

extern int ch_clip(int *, Rect *);

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
  //  gl_line(x1,y1,x2,y2,color);
  G->line(x1,y1,x2,y2,color);
}
