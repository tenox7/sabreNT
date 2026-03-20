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
 * File   : main.C                               *
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

#include "siminc.h"
#include "simsnd.h"
#include "game.h"
#include "scnedit.h"
#ifdef SNDSERV
#include "SoundClient.h"
#endif
/****************************************************************
 * globals                                                      *
 ****************************************************************/

jmp_buf jump_buff;
char mssg[64];
int xx = 0, yy = 0;
int mode = 0;
extern int frame_switch;
int mouse_x,mouse_y;
char *world_file  = "a.wld";
char *ed_world_file = "ed.wld";
char *flight_file = "a.flt";
char *ground_file = NULL;
/*
char *ground_file = "a.gru";
*/
char *hud_file    = "a.hud";
char *cpk_file    = "a.cpk";
int js0 = 0;
int js1 = 0;
int demo = 0;
int edit_scene = 0;
int want_sound = 1;

#ifdef SNDSERV
SoundClient *sndcli_ptr = NULL;
#endif

#define PRINTLN(a) { g_font->font_sprintf(xx,yy,5,NORMAL,(a)); \
  yy += 7; }

inline void println(char *format, ...)
{
  char buff[60];
  va_list ap;
  va_start(ap,format);
  vsprintf(buff,format,ap);
  va_end(ap);
  PRINTLN(buff)
    }

int graphics_start();
void doGame();
void doSceneEdit();
void init_sound();

/*************************************************************
 * SIGSEGV handler                                           *
 ************************************************************/
void (*oldsigsegv)(int);

void mysigsegv(int seg)
{
  printf("sabre: got sigsegv!\n");
  printf("routine_key: %d\n",
	 routine_key);
  printf("calling chained sigsegv handler\n");
  (*oldsigsegv)(seg); 
  restorecrtmode();
  abort();
}

/*************************************************************
 * MAIN                                                      *
 *************************************************************/
main(int argc, char *argv[])
{
  int i;
  int m;
  printf("Sabre Fighter Plane Simulator Version %s\n",
	 VERSION);
  for (i=1;i<argc;i++)
    {
      if (!strcmp(argv[i],"-o"))
	world_file = argv[++i];
      else if (!strcmp(argv[i],"-j0"))
      	js0 = 1;
      else if (!strcmp(argv[i],"-j1"))
	js1 = 1;
      else if (!strcmp(argv[i],"-tm"))
	mouse_throttle = 1;
      else if (!strcmp(argv[i],"-rm"))
	mouse_rudder = 1;
      else if (!strcmp(argv[i],"-flt"))
	flight_file = argv[++i];
      else if (!strcmp(argv[i],"-grnd"))
	ground_file = argv[++i];
      else if (!strcmp(argv[i],"-wld"))
	world_file = argv[++i];
      else if (!strcmp(argv[i],"-rnd"))
	do_random = 1;
      else if (!strcmp(argv[i],"-no_crash"))
	no_crash = 1;
      else if (!strcmp(argv[i],"-demo"))
	demo = 1;
      else if (!strcmp(argv[i],"-ed"))
	edit_scene = 1;
      else if (!strcmp(argv[i],"-mode"))
	{
	  m = atoi(argv[++i]);
	  VGAMODE = m;
	}
      /* window_x, window_y */
      else if (!strcmp(argv[i],"-window"))
	{
	  extern int window_width, window_height;
	  window_width = atoi(argv[++i]);
	  window_height = atoi(argv[++i]);
	}
      /*
       * A. Barck -- print banner and vamoos!
       */
      else if (!strcmp(argv[i],"--print-banner"))
	return(0);
    }


  mouse_avail = mouse.IsAvailable();
  if (!mouse_avail)
    printf("Mouse not available\n");
  srand(time(NULL));
  
  if (setjmp(jump_buff))
    {
#ifdef SNDSERV
      if (sndcli_ptr)
	delete sndcli_ptr;
#endif
      printf("Exiting\n");
      return(1);
    }

  if (js0)
    {
      printf("Initing joystick 0\n");
      joystick0 = new Joystick(0);
      if (!joystick0->IsAvailable())
	{
	  printf("Error activating joystick 0\n");
	  delete joystick0;
	  joystick0 = NULL;
	}
    }

  if (js1)
    {
      printf("Initing joystick 1\n");
      joystick1 = new Joystick(1);
      if (!joystick1->IsAvailable())
	{
	  printf("Error activating joystick 1\n");
	  delete joystick1;
	  joystick1 = NULL;
	}
    }

  printf("Initing graphics\n");
  if (graphics_start())
    {
      /*
      printf("chaining SIGSEGV handler\n");
      oldsigsegv = signal(SIGSEGV,mysigsegv);
      */
      if (edit_scene)
	doSceneEdit();
      else
	doGame();
      restorecrtmode();
#ifdef SNDSERV
      if (sndcli_ptr)
	delete sndcli_ptr;
#endif
      return 0;
    }
#ifdef SNDSERV
  if (sndcli_ptr)
    delete sndcli_ptr;
#endif
  return 1;
}

int graphics_start(void)
{
  curpen->color = 2;
  curpen->position = Point(0,0);
  curpen->width = 1;
  printf("Initing graphic interface\n");
  init_vga_13();
 /* Careful -- this fellow forks! */
  if (want_sound)
    init_sound();
  printf("Initing edge bounds\n");
  init_edge_bounds();
  printf("Initing zbuffer\n");
  tr_init();
  printf("Setting port_3D params\n");
  Port_3D::initPort3D();
  printf("Initing font object\n");
  g_font = new ConsoleFont("simfnt-8x8");
  g_font->put_width = 6;
  map_man = new TextrMap_Manager();
  Pilot::initPilot();
  if (mouse_avail)
    mouse.set_position((MXSCREEN_WIDTH - 1) / 2,
		       (MXSCREEN_HEIGHT - 1) / 2);
  return 1;
}

void doGame()
{

  GameSession theGame(world_file,
		      flight_file,
		      ground_file,
		      &mouse,
		      joystick0,
		      joystick1,
		      NULL,
		      1,
		      1,
		      cpk_file,
		      hud_file,
		      demo);
  theGame.doGame();
  theGame.printResults(cout);
}

void doSceneEdit()
{
  SceneEdit theEdit(world_file,
		      &mouse);
  theEdit.doEdit();
}

void error_jump(char *format, ... )
{
  restorecrtmode();
  if (format != NULL)
    {
      va_list ap;
      va_start(ap,format);
      vfprintf(stderr,format,ap);
      va_end(ap);
    }
  printf("\nPress Any Key ...");
  getchar();
  longjmp(jump_buff,1);
}

/************************************************
 * stubs for sound driver                       *
 ************************************************/

void init_sound()
{
#ifdef SNDSERV
  printf("Creating SoundClient\n");
  /* careful -- this fellow forks */
  sndcli_ptr = new SoundClient; 
  if (sndcli_ptr->PingServer())
    {
      printf("Sound Server Up");
    }
  else
    {
      printf("Error initializing sound server\n");
    }
#endif
}

void sound_on(int , int, float)
{
#ifdef SNDSERV
  if (sndcli_ptr && want_sound)
    {
    }
#endif
}

void sound_off(int)
{
}

