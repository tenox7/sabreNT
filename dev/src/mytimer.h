/*
    Sabre Flight Simulator 
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
/*
 *  08/31/87 Antti Barck
 *           Portable timing using gettimeofday
 */

/* #define CLOCK_TIMING dangerous, but whatta heck ... */
/* #define PORTABLE_TIMING */

#ifdef PORTABLE_TIMING
#include <sys/time.h>
#include <unistd.h>
#else
#include <time.h>
#endif

class MTimer
{
public:
#ifdef PORTABLE_TIMING
  struct timeval t1,t2;
  struct timezone dontcare;
#else
  clock_t t1,t2;
#endif   
  MTimer();
  float get_time();
  int check_time();
  void reset();
};










