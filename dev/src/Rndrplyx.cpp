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
 *     Sabre Fighter Plane Simulator             *
 * File   : rndrplyx.cpp                         *
 * Date   : March, 1997                          *
 * Author : Dan Hammer                           *
 *************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream.h>
#include <fstream.h>
#include <math.h>
#include <limits.h>
#include <values.h>
#include "defs.h"
#include "grafix.h"
#include "traveler.h"
#include "pen.h"
#include "vga_13.h"
#include "vmath.h"
#include "port_3d.h"
#include "cpoly.h"
#include "colorspc.h"
#include "clip.h"
#include "convpoly.h"
#include "simerr.h"
#include "simfile.h"
#include "txtrmap.h"
#include "rendpoly.h"
#include "rndzpoly.h"
#include "ddaline.h"
#include "brsnline.h"

#define LOW_VALUE R2D_TYPE(-INT_MAX)
#define HIGH_VALUE R2D_TYPE(INT_MAX)


extern TR_2DPoint *tr_l_edge;
extern TR_2DPoint *tr_r_edge;
extern int xmax,ymax;
extern R2D_TYPE min_y,max_y;
extern R2D_TYPE min_x,max_x;
extern R2D_TYPE min_z,max_z;
extern R2D_TYPE bnds_l, bnds_r;
extern R2D_TYPE bnds_t, bnds_b;
extern REAL_TYPE dz;

extern int rz_which_line;

#define TPREC 20

inline void tr_set_edge(R2D_TYPE x, R2D_TYPE y, R2D_TYPE z, R2D_TYPE u, R2D_TYPE v)
{

  if (y < 0 || y > ymax - 1)
    return;

  if (x < 0)
    x = 0;
  if (x > xmax)
    x = xmax;

  if (y < min_y)
    min_y = y;
  if (y > max_y)
    max_y = y;
  if (x < min_x)
    min_x = x;
  if (x > max_x)
    max_x = x;

  if (tr_l_edge[y].x > x)
    {
      tr_l_edge[y].x = x;
      tr_l_edge[y].z = z;
      tr_l_edge[y].u = u;
      tr_l_edge[y].v = v;
    }
  if (tr_r_edge[y].x < x)
    {
      tr_r_edge[y].x = x;
      tr_r_edge[y].z = z;
      tr_r_edge[y].u = u;
      tr_r_edge[y].v = v;
    }
}

void rendzline(const R_2DPoint &, const R_2DPoint &, int);

static void trx_build_edge_array(TR_2DPoint *points, int n)
{
	int i;
	TR_2DPoint *p0,*p1;
	R2D_TYPE zstp,zz,ustp,uu,vstp,vv;

	min_x = R2D_TYPE(INT_MAX);
	max_x = R2D_TYPE(INT_MIN);
	min_y = R2D_TYPE(INT_MAX);
	max_y = R2D_TYPE(INT_MIN);

	p0 = points;
	for (i = 0; i < (n - 1); i++)
	{
		p1 = p0 + 1;

		zz = p0->z;
		uu = p0->u;
		vv = p0->v;

		if ((p0->x==p1->x) && (p0->y==p1->y))
		{
			tr_set_edge(p0->x,p0->y,zz,uu,vv);
			p0++;
			continue;
		}


		/* 
		Can't decide upon which line algorythm is
		better (Bresenham or DDA),
		so let's waste a cpu cycle and make it
		a per-frame option (')' key toggles). DDA
		clearly gives a more accurate line, but 
		Bresenham gives a smoother edge. Defaults
		Bresenham 
		*/

		if (rz_which_line)
		{
			DDALine dline(p0->x,p0->y,p1->x,p1->y);
			zstp = (p1->z - p0->z) / dline.STEPS();
			ustp = (p1->u - p0->u) / dline.STEPS();
			vstp = (p1->v - p0->v) / dline.STEPS();
			do
			{
				tr_set_edge(dline.X(),dline.Y(),zz,uu,vv);
				zz = zz + zstp;
				uu = uu + ustp;
				vv = vv + vstp;
			} while (dline.step());
		}
		else
		{
			BresenhamLine dline(p0->x,p0->y,p1->x,p1->y);
			zstp = (p1->z - p0->z) / dline.STEPS();
			ustp = (p1->u - p0->u) / dline.STEPS();
			vstp = (p1->v - p0->v) / dline.STEPS();
			do
			{
				tr_set_edge(dline.X(),dline.Y(),zz,uu,vv);
				zz = zz + zstp;
				uu = uu + ustp;
				vv = vv + vstp;
			} while (dline.step());
		}
		p0++;
	}
}

extern int frame_switch;
extern void frame_convpoly(int *, int, int);

static void trx_frame_convpoly(TR_2DPoint *points, int n)
{
  if (n >= RENDMAX)
    return;
  int ipoints[RENDMAX * 2];
  int *iptr = ipoints;
  int i;
  for (i=0;i<n;i++)
    {
      *iptr = (int) points[i].x * 2;
      *(iptr + 1) = (int) points[i].y * 2;
      iptr += 2;
    }
  frame_convpoly(ipoints,n,10);
}

/**************************************************************
 * Texture mapping routines                                   *
 **************************************************************/
/* No perspective correction */
static void trx_fill_convpoly_nc(TR_2DPoint *points, int n, TextrMap *tmap, 
		      int fillcolor);
/* all pixels perspective corrected */
static void trx_fill_convpoly_c(TR_2DPoint *points, int n, TextrMap *tmap, 
		      int fillcolor);

static unsigned char *buffer_ptr;

void trx_fill_convpoly(TR_2DPoint *points, int n, TextrMap *tmap, 
		      int fillcolor)
{
	if (frame_switch)
	{
		trx_frame_convpoly(points,n);
		return;
	}
	buffer_ptr = lock_xbuff();
	/*
	if (TSTPS)
		trx_fill_convpoly_nc(points,n,tmap,fillcolor);
	else
	*/
	trx_fill_convpoly_c(points,n,tmap,fillcolor);
	
	free_xbuff();
}

static inline void setxxpixel(unsigned char *b_ptr,
										int color,
										R2D_TYPE *z_ptr,
										R2D_TYPE zvalue)
{
	if (b_ptr != NULL)
	{
		if (*z_ptr < zvalue)
		{
			*z_ptr = zvalue;
			*b_ptr = color;
		}
	}
}

static inline void setxpixel(int xx, int yy,
										unsigned char *b_ptr,
										R2D_TYPE *z_ptr,
										int color,
										R2D_TYPE zvalue)
{

	if (xx >= SCREEN_WIDTH - 1)
		 return;

	*b_ptr = color;
	*z_ptr = zvalue;

	if (*(z_ptr + 1) < zvalue)
	{
		*(b_ptr + 1) = color;
		*(z_ptr + 1) = zvalue;
	}

	if (*(z_ptr+SCREEN_WIDTH) < zvalue)
	{
	*(b_ptr+SCREEN_PITCH) = color;
	*(z_ptr+SCREEN_WIDTH) = zvalue;
	}
	if (*(z_ptr + SCREEN_WIDTH + 1) < zvalue)
	{
	*(b_ptr+ SCREEN_PITCH + 1) = color;
	*(z_ptr + SCREEN_WIDTH + 1) = zvalue;
	}
}

/****************************************************************
 * All pixels perspective-corrected by dividing by z-value      *
 ****************************************************************/
void trx_fill_convpoly_c(TR_2DPoint *points, int n, TextrMap *tmap, 
		      int fillcolor)
{
	int j,stps;
	int u,v;
	unsigned char *b_ptr;
	R2D_TYPE *z_ptr;
	R2D_TYPE *z_ptr2;
	R2D_TYPE *z_ptr3;
	R2D_TYPE *z_ptr4;
	R2D_TYPE *z_ptr5;

	int xx,yy;
	R2D_TYPE zstp,ustp,vstp;
	R2D_TYPE zz,zr,uu,ur,vv,vr;
	R2D_TYPE zb,zt;
	int tw,th,tw_1,th_1;
	int trans_color;
	int tflag;
	int xl;
	unsigned char *tbytes;
	unsigned char clr;
	unsigned char *b_ptr2;
	unsigned char *b_ptr3;
	unsigned char *b_ptr4;
	unsigned char *b_ptr5;

	tw = tmap->map_w;
	th = tmap->map_h;
	tw_1 = tw - 1;
	th_1 = th - 1;
	trans_color = tmap->trans_colr;

	tbytes = tmap->getBytes();
	if (tbytes == NULL)
		return;

	tflag = trans_color > -1;

	trx_build_edge_array(points,n);

	b_ptr = buffer_ptr + (((int)min_y * 2) * SCREEN_PITCH);
	z_ptr = zbuff + (((int)min_y * 2) * SCREEN_WIDTH);
	yy = min_y * 2;

	if (yy >= SCREEN_HEIGHT - 1)
	{
		j = min_y;
		goto done;
	}

	zb = zbias + ztrans;

	// Do for each row 
	for (j = (int) min_y; j <= (int)max_y; j++)
	{
		xl = (int) tr_l_edge[j].x;
		xx = (xl * 2);

		if (xx >= SCREEN_WIDTH)
			goto nextrow;

		b_ptr2 = (unsigned char *) b_ptr + xx;
		z_ptr2 = z_ptr + xx;

		if (xx < SCREEN_WIDTH - 1)
		{
			b_ptr3 = b_ptr2 + 1;
			z_ptr3 = z_ptr2 + 1;
		}
		else
		{
			b_ptr3 = NULL;
			z_ptr3 = NULL;
		}
		if (yy < SCREEN_HEIGHT - 1)
		{
			b_ptr4 = b_ptr2 + SCREEN_PITCH;
			z_ptr4 = z_ptr2 + SCREEN_WIDTH;
			if (xx < SCREEN_WIDTH - 1)
			{
				b_ptr5 = b_ptr4 + 1;
				z_ptr5 = z_ptr4 + 1;
			}
			else
			{
				b_ptr5 = NULL;
				z_ptr5 = NULL;
			}
		}
		else
		{
			b_ptr4 = b_ptr5 = NULL;
			z_ptr4 = z_ptr5 = NULL;
		}

		

		stps = (int) ((tr_r_edge[j].x - xl) + 1) ;
		zz = tr_l_edge[j].z;
		zr = tr_r_edge[j].z;
		uu = tr_l_edge[j].u;
		ur = tr_r_edge[j].u;
		vv = tr_l_edge[j].v;
		vr = tr_r_edge[j].v;

		if (TSTPS)
		{
			uu = (uu / zz) << TPREC;
			vv = (vv / zz) << TPREC;
			ur = (ur / zr) << TPREC;
			vr = (vr / zr) << TPREC;
		}

		zstp = (zr - zz) / stps;
		ustp = (ur - uu) / stps;
		vstp = (vr - vv) / stps;


		// Do for each column
		while (stps--)
		{

			zt = zz + zb;
			if (zt > *z_ptr2 /* || (z_ptr3 && (zt > *z_ptr3)) ||
									  (z_ptr4 && (zt > *z_ptr4)) ||
									  (z_ptr5 && (zt > *z_ptr5))
								   */
				)
			{
				if (TSTPS)
				{
					u = uu >> TPREC;
					v = vv >> TPREC;
				}
				else
				{
					u = (int) (uu / zz );
					v = (int) (vv / zz );
				}
				if (u > tw_1)
					u = tw_1;
				else if (u < 0)
					u = 0;
				if (v > th_1)
					v = th_1;
				else if (v < 0)
					v = 0;
				clr = (char) tmap->getColorMap(*(tbytes + v * tw + u));
				if (tflag)
				{
					if (clr != trans_color)
					{
						// setxxpixel(b_ptr2,clr,z_ptr2,zt);
						*z_ptr2 = zt;
						*b_ptr2 = clr;
						setxxpixel(b_ptr3,clr,z_ptr3,zt);
						setxxpixel(b_ptr4,clr,z_ptr4,zt);
						setxxpixel(b_ptr5,clr,z_ptr5,zt);
					}
					else if (fillcolor != -1)
					{
						// setxxpixel(b_ptr2,fillcolor,z_ptr2,zt);
						*z_ptr2 = zt;
						*b_ptr2 = fillcolor;
						setxxpixel(b_ptr3,fillcolor,z_ptr3,zt);
						setxxpixel(b_ptr4,fillcolor,z_ptr4,zt);
						setxxpixel(b_ptr5,fillcolor,z_ptr5,zt);
					}
				}
				else
				{
					// setxxpixel(b_ptr2,clr,z_ptr2,zt);
					*z_ptr2 = zt;
					*b_ptr2 = clr;
					setxxpixel(b_ptr3,clr,z_ptr3,zt);
					setxxpixel(b_ptr4,clr,z_ptr4,zt);
					setxxpixel(b_ptr5,clr,z_ptr5,zt);
				}
			}
			zz += zstp;
			uu += ustp;
			vv += vstp;
			xx += 2;
			b_ptr2 += 2;
			z_ptr2 += 2;
			if (xx < SCREEN_WIDTH - 1)
			{
				b_ptr3 = b_ptr2 + 1;
				z_ptr3 = z_ptr2 + 1;
			}
			else
			{
				b_ptr3 = NULL;
				z_ptr3 = NULL;
			}
			if (b_ptr4 != NULL)
			{
				if (xx < SCREEN_WIDTH - 1)
				{
					b_ptr4 += 2;
					z_ptr4 += 2;
					b_ptr5 += 2;
					z_ptr5 += 2;
				}
				else
				{
					b_ptr4 = NULL;
					z_ptr4 = NULL;
					b_ptr5 = NULL;
					z_ptr5 = NULL;
				}
			}
		}
nextrow:
		// reset row hi/lo 
		tr_r_edge[j].x = R2D_TYPE(LOW_VALUE);
		tr_l_edge[j].x = R2D_TYPE(HIGH_VALUE);
		// next row
		b_ptr += SCREEN_PITCH * 2;
		z_ptr += SCREEN_WIDTH * 2;
		yy += 2;
		if (yy >= SCREEN_HEIGHT - 1)
			break;
	}
done:
	while (j <= (int)max_y)
	{
		tr_r_edge[j].x = R2D_TYPE(LOW_VALUE);
		tr_l_edge[j++].x = R2D_TYPE(HIGH_VALUE);
	}
}

void trx_fill_convpoly_nc(TR_2DPoint *points, int n, TextrMap *tmap,
				int fillcolor)
{
	int j,stps;
	register int u,v;
	register unsigned char *b_ptr;

	R2D_TYPE *z_ptr;
	R2D_TYPE *z_ptr2;

	R2D_TYPE zstp,ustp,vstp;
	R2D_TYPE zz,zr,uu,ur,vv,vr;
	R2D_TYPE zb,zt;

	int tw,th,tw_1,th_1;
	int trans_color;
	int tflag;
	int xl;
	unsigned char *tbytes;
	unsigned char clr;
	unsigned char *b_ptr2;
	int xx,yy;

	tw = tmap->map_w;
	th = tmap->map_h;
	tw_1 = tw - 1;
	th_1 = th - 1;
	trans_color = tmap->trans_colr;

	tbytes = tmap->getBytes();
	if (tbytes == NULL)
		return;

	tflag = trans_color > -1;

	trx_build_edge_array(points,n);

	b_ptr = buffer_ptr + (((int)min_y*2) * SCREEN_PITCH);
	z_ptr = zbuff + (((int)min_y*2) * SCREEN_WIDTH);
	yy = min_y * 2;
	if (yy >= SCREEN_HEIGHT - 1)
		return;

	zb = zbias + ztrans;

	for (j = (int) min_y; j <= (int)max_y; j++)
	{
		xl = (int) tr_l_edge[j].x;
		stps = (int) ((tr_r_edge[j].x - xl) + 1) ;
		zz = tr_l_edge[j].z;
		zr = tr_r_edge[j].z;
		uu = tr_l_edge[j].u;
		ur = tr_r_edge[j].u;
		vv = tr_l_edge[j].v;
		vr = tr_r_edge[j].v;

		uu = (uu / zz) << TPREC;
		vv = (vv / zz) << TPREC;
		ur = (ur / zr) << TPREC;
		vr = (vr / zr) << TPREC;

		zstp = (zr - zz) / stps;
		ustp = (ur - uu) / stps;
		vstp = (vr - vv) / stps;

		xx = xl * 2;
		b_ptr2 = (unsigned char *) b_ptr + xl * 2;
		z_ptr2 = z_ptr + xl * 2;

		while (stps--)
		{
			zt = zz + zb;
			if (zt > *z_ptr2)
			{
				u = uu >> TPREC;
				v = vv >> TPREC;
				if (u > tw_1)
					u = tw_1;
				else if (u < 0)
					u = 0;
				if (v > th_1)
					v = th_1;
				else if (v < 0)
					v = 0;

				clr = (char) tmap->getColorMap(*(tbytes + (v * tw) + u));
				if (tflag)
				{
					if (clr != trans_color)
					{
						setxpixel(xx,yy,b_ptr2,z_ptr2,clr,zt);
					}
					else if (fillcolor != -1)
					{
						setxpixel(xx,yy,b_ptr2,z_ptr2,fillcolor,zt);
					}
				}
				else
				{
					setxpixel(xx,yy,b_ptr2,z_ptr2,clr,zt);
				}
			}
			zz += zstp;
			uu += ustp;
			vv += vstp;
			z_ptr2 += 2;
			b_ptr2 += 2;
			xx += 2;
		}
		tr_r_edge[j].x = R2D_TYPE(LOW_VALUE);
		tr_l_edge[j].x = R2D_TYPE(HIGH_VALUE);
		b_ptr += SCREEN_PITCH * 2;
		z_ptr += SCREEN_WIDTH * 2;
		yy += 2;
		if (yy >= SCREEN_HEIGHT - 1)
			break;
	}
	while (j <= (int)max_y)
	{
		tr_r_edge[j].x = R2D_TYPE(LOW_VALUE);
		tr_l_edge[j++].x = R2D_TYPE(HIGH_VALUE);
	}
}

