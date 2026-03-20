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
 * File   : aiweapon.h                            *
 * Date   : April, 1998                          *
 * Author : Dan Hammer                           *
 *************************************************/
#ifndef __aiweapon_h
#define __aiweapon_h

#define aiwDEFAULT_CONE_PITCH		   (_degree * 1.0)
#define aiwDEFAULT_CONE_YAW          (_degree * 1.0)
#define aiwDEFAULT_MAX_RANGE         1360.0
#define aiwDEFAULT_INITIAL_VELOCITY  2502.0
#define aiwDEFAULT_BURST_TIME        0.5
#define aiwDEFAULT_BURST_PAUSE       0.4
#define aiwDEFAULT_CONVERGANCE		 900.0
#define aiwMAX_RANGE_FACTOR			 0.6

#ifdef __cplusplus

#include "aiobject.h"
#include "aimath.h"
#include "aitarget.h"

class aiConeOfFire
{
public:
	aiAttitude	min;
	aiAttitude   max;
	aiConeOfFire()
	{}
};

class aiWeaponLimits
{
public:

	aiREAL			maxRange;
	aiREAL			minRange;
	aiAttitude	cone;
	aiVector		jiggle;
	aiREAL			initialVelocity;
	aiREAL			fireInterval;
	aiREAL			fireTimeAccumulator;
	aiConeOfFire	coneOfFire;
	enum { BURST_INIT, BURST_BURST, BURST_PAUSE };
	int			burstState;
	aiREAL			burstTime;
	aiREAL			burstPauseTime;
	aiREAL			burstTimeAccumulator;
	aiREAL			convergance;

	aiWeaponLimits()
	{
		maxRange = aiwDEFAULT_MAX_RANGE;
		minRange = 20.0;
		coneOfFire.min.yaw = -aiwDEFAULT_CONE_YAW / 2.0;
		coneOfFire.min.pitch = -aiwDEFAULT_CONE_PITCH / 2.0;
		coneOfFire.max.yaw = aiwDEFAULT_CONE_YAW / 2.0;
		coneOfFire.max.pitch = aiwDEFAULT_CONE_PITCH / 2.0;
		cone.pitch = aiwDEFAULT_CONE_PITCH;
		cone.yaw = aiwDEFAULT_CONE_YAW;
		jiggle.x = jiggle.y = jiggle.z = 0.0;
		initialVelocity = aiwDEFAULT_INITIAL_VELOCITY;
		fireInterval = 0.06;
		fireTimeAccumulator = 0.0;
		burstTimeAccumulator = 0.0;
		burstState = BURST_INIT;
		burstTime = aiwDEFAULT_BURST_TIME;
		burstPauseTime = aiwDEFAULT_BURST_PAUSE;
		convergance = aiwDEFAULT_CONVERGANCE;
	}

    void Init()
    {
      fireTimeAccumulator = 0.0;
    }

    void Update(aiREAL timeFrame)
    {
      burstTimeAccumulator += timeFrame;
      fireTimeAccumulator += timeFrame;
    }

    int UpdateBurst(int inFireParams);
    
};
#endif /* __cplusplus */

#endif

