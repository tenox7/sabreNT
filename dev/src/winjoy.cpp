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
/*************************************************
 *           Sabre Flight Simulator              *
 * File   : winjoy.cpp                           *
 * Date   : November 21, 1998                    *
 * Author : Dan Hammer                           *
 * Windows joystick (mmx)                        *
 *************************************************/
#include <stdio.h>
#include <windows.h>
#include <mmsystem.h>
#include "simfile.h"
#include "winjoy.h"
#include "sbrvkeys.h"

WinJoy::WinJoy(int which)
{
	joyNo = which;
	if (which == 0)
		joyId = JOYSTICKID1;
	else
		joyId = JOYSTICKID2;
	open();
}

WinJoy::~WinJoy()
{
	close();
}

void WinJoy::calibrate()
{

}

int WinJoy::open()
{
	UINT numDevs;
	JOYINFO joyInfo;
	JOYCAPS joyCaps;
	numDevs = joyGetNumDevs();
	has_z = has_r = has_pov = 0;
	if (numDevs - 1 < joyNo)
		active = 0;
	else if (joyGetPos(joyId,&joyInfo) == JOYERR_UNPLUGGED)
		active = 0;
	else
	{
		memset((char *)&joyCaps,0,sizeof(JOYCAPS));
		if (joyGetDevCaps(joyId,&joyCaps,sizeof(JOYCAPS)) == JOYERR_NOERROR)
		{
			naxis = joyCaps.wNumAxes;
			has_z = ((joyCaps.wCaps & JOYCAPS_HASZ) != 0) && joyCaps.wNumAxes >= 3;
			has_r = ((joyCaps.wCaps & JOYCAPS_HASR) != 0) && joyCaps.wNumAxes >= 4;
			min_x = joyCaps.wXmin;
			max_x = joyCaps.wXmax;
			min_y = joyCaps.wYmin;
			max_y = joyCaps.wYmax;
			min_z = joyCaps.wZmin;
			max_z = joyCaps.wZmax;
			min_r = joyCaps.wRmin;
			max_r = joyCaps.wRmax;
			if ((joyCaps.wCaps & JOYCAPS_HASPOV) && (joyCaps.wCaps & JOYCAPS_POV4DIR))
				has_pov = 1;
			active = 1;
		}
		else
			active = 0;
	}
	sim_printf("Joystick info: naxis = %d has_z = %d, has_r = %d, has_pov = %d\n",
					naxis,has_z,has_r,has_pov);
	return active;
}

void WinJoy::close()
{

}

int WinJoy::update()
{
	int r;
	JOYINFO joyInfo;
	r = joyGetPos(joyId,&joyInfo);
	if (r != JOYERR_NOERROR)
	{
		active = 0;
		return 0;
	}
	active = 1;
   buttons = joyInfo.wButtons;
   raw_x = joyInfo.wXpos;
   raw_y =	joyInfo.wYpos;
	raw_z = joyInfo.wZpos;

	if (has_r || has_pov)
	{
		JOYINFOEX joyInfoEx;
		memset((char *)&joyInfoEx,0,sizeof(JOYINFOEX));
		joyInfoEx.dwSize = sizeof(JOYINFOEX);
		if (has_r)
			joyInfoEx.dwFlags = JOY_RETURNR;
		if (has_pov)
			joyInfoEx.dwFlags |= JOY_RETURNPOV;
		r = joyGetPosEx(joyId,&joyInfoEx);
		if (r == JOYERR_NOERROR)
		{
			if (has_r && joyInfoEx.dwFlags & JOY_RETURNR)
				raw_r = joyInfoEx.dwRpos;
			if (has_pov && joyInfoEx.dwFlags & JOY_RETURNPOV)
			{
				switch (joyInfoEx.dwPOV)
				{
					case JOY_POVBACKWARD:
						buttons = FI_CH_BACK;
						break;

					case JOY_POVFORWARD:
						buttons = FI_CH_FRWRD;
						break;

					case JOY_POVRIGHT:
						buttons = FI_CH_RIGHT;
						break;

					case JOY_POVLEFT:
						buttons = FI_CH_LEFT;
						break;
				}
			}
		}
	}

   cooked_x = (float)( -1.0 + (((float)raw_x) / ((float)(max_x - min_x + 1)) * 2.0 ));
   cooked_y = (float)( -1.0 + (((float)raw_y) / ((float)(max_y - min_y + 1)) * 2.0 ));
   cooked_z = (float)( -1.0 + (((float)raw_z) / ((float)(max_z - min_z + 1)) * 2.0 ));
   cooked_r = (float)( -1.0 + (((float)raw_r) / ((float)(max_r - min_r + 1)) * 2.0 ));

	if (cooked_x < -1.0)
		cooked_x = -1.0;
	if (cooked_x > 1.0)
		cooked_x = 1.0;
	if (cooked_y < -1.0)
		cooked_y = -1.0;
	if (cooked_y > 1.0)
		cooked_y = 1.0;
	if (cooked_z < -1.0)
		cooked_z = -1.0;
	if (cooked_z > 1.0)
		cooked_z = 1.0;
	if (cooked_r < -1.0)
		cooked_r = -1.0;
	if (cooked_r > 1.0)
		cooked_r = 1.0;
	return active;
}

int WinJoy::read_def_file(char *)
{
      return (1);
}

float WinJoy::getx()
{
  return(cooked_x);
}

float WinJoy::gety()
{
  return(cooked_y);
}

int WinJoy::getrx()
    {
      return(raw_x);
    }

int WinJoy::getry()
    {
      return(raw_y);
    }

int WinJoy::getbuttons()
    {
      return(buttons);
    }