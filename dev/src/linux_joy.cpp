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
 * File   : linux_joy.C                          *
 * Date   : March, 1997                          *
 * Author : Dan Hammer                           *
 *************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Define this in the Makefile if joystick drivers
// installed
#ifdef JSTICK_INSTALLED
#include <linux/joystick.h>
#else
#include "./joydummy.h"
#endif
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <iostream.h>
#include <fstream.h>
#include "linux_joy.h"


char *LinuxJoystick::joydef0 = "joy0.def";
char *LinuxJoystick::joydef1 = "joy1.def";
char *LinuxJoystick::joydev0 = "/dev/js0";
char *LinuxJoystick::joydev1 = "/dev/js1";

#if 0
inline char *build_libpath(char *s)
{
  return (s);
}

main(int argc, char *argv[])
{
  int w;
  if (argc < 2)
    w = 0;
  else
    w = atoi(argv[1]);
  LinuxJoystick joystk(w);
  if (joystk.calibrated)
    {
      printf("Testing joystick %d\n",joystk.joy);
      if (joystk.open())
	{
	  while (joystk.update())
	    {
	      printf("raw_x: %d  raw_y: %d  cooked_x: %1.4f cooked_y: %1.4f buttons: %d            \r",
		     joystk.raw_x,
		     joystk.raw_y,
		     joystk.cooked_x,
		     joystk.cooked_y,
		     joystk.buttons);
	    }
	}
      joystk.close();
    }
}
#else
extern char *build_libpath(char *);
#endif

LinuxJoystick::LinuxJoystick(int which)
{
  joy = which;
  fd = -1;
  calibrated = read_def_file(joy ? joydef1 : joydef0 );
  if (!calibrated)
    calibrate();
  if (calibrated)
    active = open();
  else
    active = 0;
}

int LinuxJoystick::read_def_file(char *path)
{
  int result = 0;
  ifstream is(build_libpath(path));
  if (is)
    {
      result = 1;
      is >> cor_x >> cor_y >> max_x >> max_y >> min_x >> min_y;
    }
  return(result);
}

int LinuxJoystick::open()
{
  long tmpl;
  struct JS_DATA_TYPE js_data;
  int status;
  char *devpath = joy ? joydev1 : joydev0;
  fd = ::open(devpath, O_RDONLY);
  if (fd < 0)
    {
      perror("LinuxJoystick");
      return(0);
    }
  js_data.x = cor_x;
  js_data.y = cor_y;
  
  status = ioctl(fd,JS_SET_CAL,&js_data);
  if (status == -1)
    {
      perror("LinuxJoystick");
      return 0;
    }
  tmpl = 1L;
  status = ioctl (fd, JS_SET_TIMELIMIT, &tmpl);
  if (status == -1) 
    {
      perror ("jscal");
      return 0;
    }
  return(1);
}

int LinuxJoystick::update()
{
  struct JS_DATA_TYPE js_data;
  
  if (read(fd, &js_data, JS_RETURN) > 0)
    {
      buttons = js_data.buttons;
      raw_x = js_data.x;
      raw_y = js_data.y;
      cooked_x = -1.0 + (((float)raw_x) / ((float)(max_x - min_x + 1)) * 2.0 );
      cooked_y = -1.0 + (((float)raw_y) / ((float)(max_y - min_y + 1)) * 2.0 );
      return (1);
    }
  else
    return (0);
}

void LinuxJoystick::close()
{
  if (fd > 0)
    ::close(fd);
}

void LinuxJoystick::calibrate()
{
  struct JS_DATA_TYPE js_data;
  long tmpl;
  int tmp;
  int status;

  char *devpath = joy ? joydev1 : joydev0;
  fd = ::open(devpath, O_RDONLY);
  if (fd < 0)
    {
      perror("LinuxJoystick");
      return;
    }

  status = ioctl (fd, JS_GET_TIMEOUT, &tmp);
  if (status == -1) 
    {
      perror ("jscal");
      exit (1);
    }

  printf ("Timeout value = %d\n", tmp);

  status = ioctl (fd, JS_GET_TIMELIMIT, &tmpl);
  if (status == -1) 
    {
      perror ("jscal");
      return;
    }

  printf ("Timelimit value = %ld ms\nSetting Timelimit = 100 ms\n", tmpl);
 
  tmpl = 10;
  
  status = ioctl (fd, JS_SET_TIMELIMIT, &tmpl);
  if (status == -1) 
    {
      perror ("jscal");
      return;
    }
   status = ioctl (fd, JS_GET_CAL, &js_data);
  if (status == -1) 
    {
      perror ("jscal");
      return;
    }
  printf ("Need to calibrate joystick ... \n");
  printf ("Current correction: %d , %d\n", js_data.x, js_data.y);
  printf ("Move joystick to lower right corner and press either button\n");
  while ((read (fd, &js_data, JS_RETURN) > 0) && js_data.buttons == 0x00)
    printf ("Got x = %d, y = %d\r", js_data.x, js_data.y);
  
  for (tmp = 0; js_data.x > 0xff; tmp++, js_data.x = js_data.x >> 1);
  cor_x = tmp;
  for (tmp = 0; js_data.y > 0xff; tmp++, js_data.y = js_data.y >> 1);
  cor_y = tmp;

  printf ("Setting correction\n");

  js_data.x = cor_x;
  js_data.y = cor_y;
  status = ioctl(fd,JS_SET_CAL,&js_data);
  if (status == -1)
    {
      perror("jscal");
      return;
    }

  
  printf("Getting minimum values\n");
  printf("Move joystick to upper left corner and press either button\n");
  while(read(fd,&js_data,JS_RETURN) && js_data.buttons);

  while (1)
    {
      if (read(fd,&js_data,JS_RETURN) != JS_RETURN)
	perror("jscal");
      printf ("Got x = %d, y = %d  \r", js_data.x, js_data.y);
      if (js_data.buttons != 0x00)
	break;
    }
  min_x = js_data.x;
  min_y = js_data.y;

  printf("Getting maximum values\n");
  printf("Move joystick to lower right corner and press either button\n");
  while(read(fd,&js_data,JS_RETURN) && js_data.buttons);
  while (1)
    {
      if (read(fd,&js_data,JS_RETURN) != JS_RETURN)
	perror("jscal");
      printf ("Got x = %d, y = %d   \r", js_data.x, js_data.y);
      if (js_data.buttons != 0x00)
	break;
    }
  max_x = js_data.x;
  max_y = js_data.y;

  calibrated = 1;

  printf("Saving correction values %d, %d\n",cor_x,cor_y);
  ofstream os(joy ? build_libpath(joydef1) : build_libpath(joydef0));
  if (os)
    {
      os << cor_x << " " << cor_y << '\n';
      os << max_x << " " << max_y << '\n';
      os << min_x << " " << min_y << '\n';
    }
  else
    perror("jscal");
  close();
}


