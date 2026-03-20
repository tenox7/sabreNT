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
 * File   : linux_joy.h                          *
 * Date   : March, 1997                          *
 * Author : Dan Hammer                           *
 *************************************************/
#ifndef __linux_joy_h
#define __linux_joy_h

class LinuxJoystick
{
public:
  static char *joydef0;
  static char *joydef1;
  static char *joydev0;
  static char *joydev1;

  int max_x,min_x;
  int max_y,min_y;
  int cor_x, cor_y;

  float cooked_x,cooked_y;
  int   raw_x,raw_y;
  int buttons;

  int fd;
  int joy;
  int calibrated;
  int active;

  LinuxJoystick(int which);
  ~LinuxJoystick()
    {
      close();
    }
  void calibrate();
  int open();
  void close();
  int update();
  int read_def_file(char *);

  float getx()
    {
      return(cooked_x);
    }

  float gety()
    {
      return(cooked_y);
    }

  int getrx()
    {
      return(raw_x);
    }

  int getry()
    {
      return(raw_y);
    }

  int getbuttons()
    {
      return(buttons);
    }
};

#endif
