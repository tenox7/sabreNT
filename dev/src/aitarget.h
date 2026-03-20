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
 * File   : aitarget.h                            *
 * Date   : April, 1998                          *
 * Author : Dan Hammer                           *
 * Targeting thingees                            *
 *************************************************/
#ifndef __aitarget_h
#define __aitarget_h

#include "aiobject.h"
#include "aimath.h"
#include "aitact.h"

#ifdef __cplusplus

#define T_AVERAGING_N				16
#define FLAG_AVERAGING_N			30
#define iNOTARGET_INDEX				-2
#define ITARGET_MIN_THREAT_VALUE -99999.0

class aiTargetGeometry 
{
public:
  enum { LEFT_ASPECT, RIGHT_ASPECT };
  aiPoint    worldPosition;         /* target location in world */
  aiVector   worldHeading;          /* target heading in world */	
  aiVector   worldDirection;        /* target direction in world from us */
  aiAttitude relAttitude;           /* pitch,roll,yaw relative to our heading */
  aiREAL     angleOff;              /* angle between our heading and target heading */
  aiREAL     aspect;                /* angle between target heading and direction from us */
  int       aspectDir;             /* left or right aspect */
  aiREAL     range;                 /* range */
  aiREAL     ranges[T_AVERAGING_N]; /* average range array */
  int       rng_idx;
  aiREAL     dRange;                /* change in range (per sec) */
  aiVector   worldVelocity;         /* velocity in world */
  aiREAL	    airSpeed;              /* air speed (FPS) */
  aiPoint    leadPoint;             /* lead pursuit point */
  aiVector   leadVect;              /* vector to lead pursuit point */
  aiAttitude leadAttitude;          /* attitude to lead pursuit point */
  aiAttitude leadRate;				  /* normalized lead angular rates */
  aiPoint    lagPoint;              /* lag pursuit point */
  aiAttitude lagAttitude;           /* attitude to lag pursuit point */
  aiPoint    gunLeadPoint;          /* gun lead point */
  aiAttitude gunLeadAttitude;       /* attitude to gun lead point */
  aiREAL		gunTOF;					  /* time-of-flight of gun projectile */
  aiVector   gunLeadVector;         /* vector to gun lead pursuit point */
  aiREAL     gunLeadRangeSq;        /* range to gun lead point */
  /* for formation flying */
  aiPoint    formationPoint;
  aiAttitude formationAttitude;
  aiREAL     formationRange;
  aiREAL		formationDRange;
  aiREAL     turnRate;              /* current turning rate */
  aiREAL     load;                  /* current load */

  aiTargetGeometry();
};

/* flags indicating various target conditions   */
class aiTargetFlags : public aiObject
{
public:
  int rangeClose;   /* target is getting nearer   */
  int rangeOpen;    /* target is getting further  */
  int turning;      /* target is turning          */
  int turning_not;  /* not turning                */
  int below;        /* target below               */
  int above;        /* target above               */
  int low;          /* target is high             */
  int high;         /* target is low              */
  int veryLow;      /* target very low            */
  int veryHigh;     /* target very high           */
  int headOnAspect; /* target pointing towards us */
  int tailAspect;   /* target pointing away       */
  int forward;      /* target in forward quarter  */
  int rear;         /* target in rear quarter     */
  int onNose;       /* target on nose             */
  int onTail;       /* target on tail             */
  int highAOT;      /* high angle off             */
  int lowAOT;       /* low angle off              */

  void Reset()
    {
      rangeClose = 0;
      rangeOpen = 0;
      turning = 0;
      turning_not = 0;
      above = 0;
      below = 0;
      high = 0;
      low = 0;
      veryHigh = 0;
      veryLow = 0;
      headOnAspect= 0;
      tailAspect = 0;
      onNose = 0;
      onTail = 0;
      forward = 0;
      rear = 0;
      highAOT = 0;
      lowAOT = 0;
    }

  void Add(const aiTargetFlags &fl);
  void Set(int max = 14);

  aiTargetFlags()
    {
      Reset();
    }
};

class aiTargetInfo : public aiObject
{
public:
  char	model[32];
  int	tacticalCaps[AIPT_NTACTICS];
  aiREAL	weaponsRange;
  aiREAL	weaponsEffect;
  aiREAL	maxSpeed;
  aiTargetInfo();
};

class aiTarget : public aiObject
{
public:
  aiTargetInfo      info;
  aiTargetGeometry  geometry;
  aiTargetFlags     flags;
  aiTargetFlags     tFlags[FLAG_AVERAGING_N];
  int              tFlagIndex;
  unsigned long    idx;
  int              active;
  aiREAL            threatValue;
  aiTarget()
    {
      tFlagIndex = 0;
      active = 1;
      idx = 0;
      threatValue = ITARGET_MIN_THREAT_VALUE;
    }
  static void GetTargetInfo(unsigned long targetIdx, aiPoint &position, 
			    aiVector &velocity, aiVector &heading);
  static int GetTargetActive(unsigned long targetIdx);
};

/*
 * surface target info
 */
class aiSurfaceTarget : public aiObject
{
public:
  int        index;
  int        isStatic;
  aiPoint     worldPoint;
  aiPoint     alignPoint;
  aiVector    worldDirection;
  aiVector    worldVelocity;
  aiAttitude  relAttitude;
  aiREAL      range;
  aiREAL      dRange;
  aiREAL      vertDistance;
  aiREAL      dVertDistance;
    
  aiSurfaceTarget()
    {
      isStatic = 0;
      range = dRange = vertDistance = dVertDistance = 0.0;
    }  
};

#endif /* __cplusplus */
#endif
