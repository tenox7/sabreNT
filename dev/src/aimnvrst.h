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
 * File   : aimnvrst.h                            *
 * Date   : April, 1998                          *
 * Author : Dan Hammer                           *
 * Object for maneuver stack                     *
 *************************************************/
#ifndef __aimnvrst_h
#define __aimnvrst_h

typedef struct maneuver_st_type
{
  int type;
  unsigned long flags;
  float data0;
  float data1;
  float data2;
} maneuver_str;

#ifdef __cplusplus /* don't let "C" compiler see this */

#include "aiobject.h"
#include "aimath.h"

class aiManeuver
{
public:
enum {
		STRAIGHT_AND_LEVEL = 1, // 1
		STANDARD_TURN,          // 2
		CLIMB,                  // 3
		DESCEND,                // 4
		BREAK_TURN,					// 5
		HARD_TURN,					// 6
		IMMELMAN,               // 7
		SPLIT_S,                // 8
		INVERT,                 // 9
		EXTEND,                 // 10
		JINK,                   // 11
		ENGAGE,                 // 12
		ALIGN,						// 13
		RQ_EVADE,					// 14
		RQ_ENGAGE,					// 15
		FQ_ENGAGE,					// 16
		PURSUIT,						// 17
		GUN_ATTACK,					// 18
		NAVIGATE,					// 19
		FORMATION,					// 20
		PULLUP,						// 21
		LEVEL_ROLL,					// 22
		PITCHED_ROLL,				// 23
		WINGOVER,					// 24
		LEVEL_BOMB,					// 25
		INTERCEPT,					// 26
		CLIMBING_TURN,				// 27
		DESCENDING_TURN,			// 28
		SNAP_ROLL,					// 29
		CHANDELLE,					// 30          
		AIRSHOW,						// 31
		AILERON_ROLL,				// 32
		EVADE,						// 33
		ZOOM,							// 34
		BARREL_ROLL,				// 35
		SPIRAL_DIVE,				//	36
		TAKEOFF,						// 37
		LAND,							// 38
		TAXI,							// 39
		TURN_FIGHT,					// 40
		LAST
	};

	enum { LEFT, RIGHT };

	static char *maneuver2string[];
	static char *Maneuver2String(int);
};

#define AAIMNVR_DIRBIT   0x01
#define AIMNVR_LOOPBIT  0x02
#define AIMNVR_FLAG0BIT 0x04
#define AIMNVR_FLAG1BIT 0x08
#define AIMNVR_FLAG2BIT 0x10
#define AIMNVR_LEFT     0x00
#define AIMNVR_RIGHT    0x01

#define AIMNVR_DIR(a) ((int)((a) & AAIMNVR_DIRBIT))

class aiManeuverState
{
public:
	int maneuver;
	int state;
	int done;
	int  stackLevel;
	unsigned long flags;
	aiREAL data0;
	aiREAL data1;
	aiREAL data2;
	aiREAL data3;
	aiREAL data4;
	aiREAL data5;

	aiManeuverState()
	{
		maneuver = 0;
		state = 0;
		flags = 0;
		data0 = data1 = data2 = 0.0;
		data3 = data4 = data5 = 0.0;
		done = 0;
	}

	friend void SETMANEUVER(aiManeuverState &ms,
								   int maneuver,
								   int stackLevel,
								   unsigned long flgs = 0,
								   aiREAL d0 = 0.0,
								   aiREAL d1 = 0.0,
								   aiREAL d2 = 0.0)
	{
		ms.maneuver = maneuver;
		ms.stackLevel = stackLevel;
		ms.flags = flgs;
		ms.data0 = d0;
		ms.data1 = d1;
		ms.data2 = d2;
		ms.state = 0;
		ms.done = 0;
	}

	friend void SETMANEUVERX(aiManeuverState &ms,
								   int maneuver,
								   int stackLevel,
								   unsigned long flgs = 0,
								   aiREAL d0 = 0.0,
								   aiREAL d1 = 0.0,
								   aiREAL d2 = 0.0,
                           aiREAL d3 = 0.0,
                           aiREAL d4 = 0.0,
                           aiREAL d5 = 0.0)
	{
		ms.maneuver = maneuver;
		ms.stackLevel = stackLevel;
		ms.flags = flgs;
		ms.data0 = d0;
		ms.data1 = d1;
		ms.data2 = d2;
		ms.data3 = d3;
		ms.data4 = d4;
		ms.data5 = d5;
		ms.done = 0;
	}

	int GetManeuverDirection();
	int IsLoop()
	{
		return (((int)(flags & AIMNVR_LOOPBIT)) != 0);
	}
	char *GetManeuverString()
	{
		return (aiManeuver::Maneuver2String(maneuver));
	}
};

#endif /* __cplusplus */
#endif
