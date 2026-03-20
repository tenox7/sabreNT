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
 * Version: 0.1                                  *
 * File   : cmprtxtr.C                           *
 * Date   : March, 1997                          *
 * Author : Dan Hammer                           *
 * Main file                                     *
 *************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <values.h>
#include <float.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <signal.h>
#include <ctype.h>
#include <iostream.h>
#include <fstream.h>
#include <setjmp.h>
#include <stdarg.h>

#include "defs.h"
#include "simerr.h"
#include "rtkey.h"
#include "simfile.h"
#include "colorspc.h"

#include "grafix.h"
#include "vga_13.h"
#include "traveler.h"
#include "pen.h"
#include "transblt.h"
#include "vmath.h"
#include "port_3d.h"
#include "clip.h"
#include "target.h"
#include "cpoly.h"
#include "obj_3d.h"
#include "group_3d.h"
#include "copoly.h"
#include "txtrmap.h"
#include "bits.h"
#include "convpoly.h"
#include "rendpoly.h"
#include "rndrpoly.h"
#include "rndzpoly.h"
#include "zview.h"
#include "rotate.h"


float scale_f = 99;
Port_3D vport;
Port_3D ref_port;
Vector world_light_source;
float world_scale;
int xx = 0;
int yy = 0;
jmp_buf jump_buff;
REAL_TYPE view_distance;
int mode = 0;
int current_shape = 0;
Z_Node_Manager *zm;
extern int frame_switch;
char *out_file = "zvedit.out";
char *outfile = out_file;
REAL_TYPE delta = 0.01;
REAL_TYPE delta_angle = 0.1;
int current_poly;
char *lib_path = "/home/src/sabre/lib";
int routine_key;
REAL_TYPE shadow_level = 0.0;
REAL_TYPE vphi = 0.1;
REAL_TYPE vtheta = 0.0;
int current_txtr = 0;

/*************************************************************
 * MAIN                                                      *
 *************************************************************/
void error_jump(char *format, ... )
{
  if (format != NULL)
    {
      va_list ap;
      va_start(ap,format);
      vfprintf(stderr,format,ap);
      va_end(ap);
    }
  longjmp(jump_buff,1);
}

main(int argc, char *argv[])
{
  ifstream is;
  ofstream os;
  istream *isptr;
  ostream *osptr;

  TextrMap tmap;
  int ntextrs;

  if (setjmp(jump_buff))
    {
      printf("exiting\n");
      return(1);
    }

  if (*argv[1] == '-')
    isptr = &cin;
  else
    {
      is.open(argv[1]);
      if (!is)
	{
	  printf("%s: unable to open %s for reading\n",argv[0],argv[1]);
	  exit(1);
	}
      isptr = &is;
    }

  if (*argv[2] == '-')
    osptr = &cout;
  else
    {
    os.open(argv[2]);
    if (!os)
      {
	printf("%s: unable to open %s for reading\n",argv[0],argv[2]);
	exit(1);
      }
    osptr = &os;
    }
  *isptr >> ntextrs;
  *osptr << ntextrs << '\n';
  while (ntextrs--)
    {
      *isptr >> tmap;
      printf("compressing %s\n",tmap.id);
      //      printf("reading  %s\n",tmap.id);
      //      tmap.read_compressed();
      tmap.write(*osptr);
    }
}
