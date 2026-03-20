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
 * 12/97 gdev library                            *
 *************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "defs.h"
#include "grafix.h"
#include "traveler.h"
#include "vga_13.h"
#include "gdev-svgalib.h"
#include "ddaline.h"

/*
  Define our own "vgamodes"
 */

#define G320x200x256  0
#define G640x480x256  1
#define G800x600x256  2
#define G1024x768x256 3

unsigned char *xbuffer;
unsigned char *buffer_ptr;
unsigned char *screen_ptr;
float         aspect_ratio    = 1.0;
int           SCREEN_WIDTH    = 320; 
int           SCREEN_HEIGHT   = 200;
int           MXSCREEN_WIDTH  = 320;
int           MXSCREEN_HEIGHT = 200;
int           MAX_X           = 319;
int           MAX_Y           = 199;
int           VGAMODE         = G320x200x256;
int           PALETTE_SIZE    = 768;
int           N_COLORS        = 256;

int           window_width      = 0;
int           window_height      = 0;

int           wx0,wy0,wx1,wy1;
SimFont       *g_font = NULL;
Rect          cliprect(0,0,319,199);

#define GDEV_SCREEN_DIM 1
#define GDEV_WINDOW_DIM 0

void init_vga_13(void)
{
  int dimx,dimy;

  /* switch on vgamode to get x,y dims */
  switch (VGAMODE)
    {
    case G320x200x256:
      dimx = 320;
      dimy = 200;
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
      dimx = 1024;
      dimy = 768;
      break;

    default:
      printf("svgalib mode %d not allowable, defaulting to G320x200x256\n",
	     VGAMODE);
      VGAMODE = G320x200x256;
      dimx = 320;
      dimy = 200;
      break;
    }

  /* Create gdev for svgalib */
  assign_gdev_svgalib();
  /* Create rgbdev for svgalib */
  assign_rgbdev8_svgalib();

  if (window_width == 0)
    window_width = dimx;
  if (window_height == 0)
    window_height = dimy;

  /* 
     Try twice, once with desired dims, 
     next with standard VGA 320x200 if desired dims
     are not available
  */

  int goodOpen = 0;
  for (int i=0;i<2;i++)
    {
      if (G->open(dimx,dimy,8,window_width,window_height) == 0)
	{
	  G->clear(0,3);
	  /* always point to buffered screen */
	  xbuffer    = (unsigned char *) G->getvbuf();
	  screen_ptr = xbuffer;
	  buffer_ptr = xbuffer;
	  SCREEN_WIDTH = G->getdimx(GDEV_WINDOW_DIM);
	  SCREEN_HEIGHT = G->getdimy(GDEV_WINDOW_DIM);
	  MXSCREEN_WIDTH = G->getdimx(GDEV_SCREEN_DIM);
	  MXSCREEN_HEIGHT = G->getdimy(GDEV_SCREEN_DIM);
	  MAX_X = SCREEN_WIDTH - 1;
	  MAX_Y = SCREEN_HEIGHT - 1;
	  cliprect.topLeft.x = 0;
	  cliprect.topLeft.y = 0;
	  cliprect.botRight.x = MAX_X;
	  cliprect.botRight.y = MAX_Y;
	  aspect_ratio = calc_aspect_ratio(MXSCREEN_WIDTH,MXSCREEN_HEIGHT);
	  printf("GDEV Params: %d %d %d %d %f\n",
		 MXSCREEN_WIDTH,MXSCREEN_HEIGHT,SCREEN_WIDTH,SCREEN_HEIGHT,aspect_ratio);
	  goodOpen = 1;
	}
      if (!goodOpen && i==0)
	{
	  printf("%dx%d mode not available, using 320x200\n",dimx,dimy);
	  dimx = 320;
	  dimy = 200;
	  if (window_width > dimx)
	    window_width = dimx;
	  if (window_height > dimy)
	    window_height = dimy;
	}
      else
	break;
    }

  if (!goodOpen)
    {
      /* couldn't even open a 320x200 VGA mode ... */
      printf("Unable to set video mode ... exiting\n");
      exit(1);
    }
}

void restorecrtmode()
{
  G->close();
}

float calc_aspect_ratio(float width, float height)
{
  float ideal_vert;
  ideal_vert = width * 2.8 / 4.0;
  return (height / ideal_vert);
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

void fill_rect(Rect &r, int color)
{
  if (valid_rect(r))
    {
      Rect r0 = r;
      cliprect2rect(cliprect,r0);
      G->rect(r0.left(),r0.top(),RWIDTH(r0),RHEIGHT(r0),color);
    }
}

void putpixel(int x, int y, int color)
{
  if (x < SCREEN_WIDTH && x >= 0 && y < SCREEN_HEIGHT && y >= 0)
    *(buffer_ptr + (y * SCREEN_WIDTH) + x) = (char ) color;
}

void h_line(int x, int y, int len, int color)
{
  register unsigned char *bf = buffer_ptr + (y * SCREEN_WIDTH) + x;
  while (len--)
    *bf++ = color;
}

void v_line(int x, int y, register int len, int color)
{
  register unsigned char *bf = buffer_ptr + (y * SCREEN_WIDTH) + x;
  while(len--)
    {
      *bf = color;
      bf += SCREEN_WIDTH;
    }
}

void set_rgb_value(int color, char red, char green, char blue)
{
  RGB8->set(color,red,green,blue);
}

void get_rgb_value(int color, char *red, char *green, char *blue)
{
  RGB8->get(color,red,green,blue);
}

void set_palette(int startcolor, int endcolor, char *palette)
{
  RGB8->set(palette,startcolor,endcolor);
}

void get_palette(int startcolor, int endcolor, char *palette)
{
  RGB8->get(palette,startcolor,endcolor);
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

void clr_palette(int start, int number)
{
  int i;
  if ((start>256) | (start<0) | ((start+number)>256))
    return;
  for (i=start;i<(start+number);i++)
    set_rgb_value(i,0,0,0);
}

extern int ch_clip(int *, Rect *);
void mline(int, int, int, int, char);

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
  mline(x1,y1,x2,y2,(char) color);
}

void mline(int x0, int y0, int x1, int y1, char color)
{
  DDALine dline(x0,y0,x1,y1);
  do
    {
      putpixel(dline.X(),dline.Y(),color);
    }
  while (dline.step());
}



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


