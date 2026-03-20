/*
    Sabre Fighter Plane Simulator 
    Copyright (c) 1997/1998 Dan Hammer
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
 *         Sabre Fighter Plane Simulator         *
 * File   : stime.c                              *
 * Date   : April, 1998                          *
 * Author : Dan Hammer                           *
 *************************************************/

static double aiTimeFrame = 0.0;
static double aiElapsedTime = 0.0;

double aiGetTimeFrame(void)
{
	return (aiTimeFrame);
}

double aiGetElapsedTime(void)
{
	return (aiElapsedTime);
}

void aiSetTimeFrame(double timeFrame)
{
	aiTimeFrame = timeFrame;
	aiElapsedTime += timeFrame;
}

void aiResetElapsedTime(void)
{
	aiTimeFrame = 0.0;
	aiElapsedTime = 0.0;
}
