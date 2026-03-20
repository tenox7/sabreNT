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
 * File   : aiplncaps.h                           *
 * Date   : April, 1998                          *
 * Author : Dan Hammer                           *
 * Plane capabilities                            *
 *************************************************/
#ifndef __aiplncaps_h
#define __aiplncaps_h

#include "aiobject.h"
#include "aiarray.h"
#include "aimath.h"
#include "aipid.h"

#define aiMAXPLANECAPS 32

typedef struct aiplane_caps_struct
{
	char    model[32];
	int     canDoAnglesFight;
	int     canDoEnergyFight;
	int     canStrafe;
	int     canRocket;
	int     canDiveBomb;
	int     canTorpedoBomb;
	int     canLightBomb;
	int     canHeavyBomb;

	aiREAL   maxSpeed;
	aiREAL   stallSpeed;
	aiREAL   cornerSpeed;
	aiREAL   climbSpeed;

	aiREAL   weaponsRange;
	aiREAL   weaponsEffect;

	aiREAL   maxRollRate;
	aiREAL   minRollRate;
	aiREAL   rollAccelFactor;
	aiREAL   formationLen;
	aiREAL   formationOffset;

	_aiPID pitchPid;
	_aiPID rollPid;
	_aiPID yawPid;
	_aiPID gPid;

	aiREAL			collisionAvoidSecs;		/* secs look-ahead to detect collision with target */
	aiREAL			immelmanGs;					/* g's to pull for immelman maneuver */
	aiREAL			immelmanAltMin;			/* min alt for immelman maneuver */
	aiREAL			noseOnRollLimit;			/* limits roll when getting nose on target */
	aiREAL			speedLossLimit;			/* limit airspeed loss during gun attack */
	aiREAL			leadAdjustMult;			/* mult to increase gun lead point */
	aiREAL			splitSGs;					/* g's to pull for split_s maneuver */
	aiREAL			splitSAltMin;				/* min alt for split_s maneuver */
	aiREAL			posGLimit;
	aiREAL			negGLimit;
	aiREAL			grndColSecs;				/* secs look-ahead to det. ground coll */
	aiREAL			grndColAGL;					/* min AGL for ground coll */
	aiREAL			grndColRcvrAGL;			/* min AGL to recover */
	aiREAL			grndColPullUpAngle;		/* angle to pull up */
	aiREAL			zoomAngle;

	aiREAL			standardTurnSpeedMPH;
	aiREAL			finalApproachDistance;
	aiREAL			finalApproachAGL;
} aiplane_caps;

#ifdef __cplusplus

class aiPlaneCaps : public aiObject
{
public:
	char    model[32];
	int     canDoAnglesFight;
	int     canDoEnergyFight;
	int     canStrafe;
	int     canRocket;
	int     canDiveBomb;
	int     canTorpedoBomb;
	int     canLightBomb;
	int     canHeavyBomb;

	aiREAL   maxSpeed;
	aiREAL   stallSpeed;
	aiREAL   cornerSpeed;
	aiREAL   climbSpeed;

	aiREAL   weaponsRange;
	aiREAL   weaponsEffect;

	aiREAL   maxRollRate;
	aiREAL   minRollRate;
	aiREAL   rollAccelFactor;
	aiREAL   formationLen;
	aiREAL   formationOffset;

	aiPID pitchPid;
	aiPID rollPid;
	aiPID yawPid;
	aiPID gPid;

	aiREAL			collisionAvoidSecs;		/* secs look-ahead to detect collision with target */
	aiREAL			immelmanGs;					/* g's to pull for immelman maneuver */
	aiREAL			immelmanAltMin;			/* min alt for immelman maneuver */
	aiREAL			noseOnRollLimit;			/* limits roll when getting nose on target */
	aiREAL			speedLossLimit;			/* limit airspeed loss during gun attack */
	aiREAL			leadAdjustMult;			/* mult to increase gun lead point */
	aiREAL			splitSGs;					/* g's to pull for split_s maneuver */
	aiREAL			splitSAltMin;				/* min alt for split_s maneuver */
	aiREAL			posGLimit;
	aiREAL			negGLimit;
	aiREAL			grndColSecs;				/* secs look-ahead to det. ground coll */
	aiREAL			grndColAGL;					/* min AGL for ground coll */
	aiREAL			grndColRcvrAGL;			/* min AGL to recover */
	aiREAL			grndColPullUpAngle;		/* angle to pull up */
	aiREAL			zoomAngle;

	aiREAL			standardTurnSpeedMPH;
	aiREAL			finalApproachDistance;
	aiREAL			finalApproachAGL;

	aiPlaneCaps()
	{}

	aiPlaneCaps(const aiplane_caps &);

	static aiObjectArray aiPlaneCapsArray;
	static void AddaiPlaneCaps(aiPlaneCaps &);
	static void AddaiPlaneCaps(aiplane_caps *);

	static aiPlaneCaps *GetaiPlaneCaps(const char *model);
	static void FlushaiPlaneCaps();
};
#endif

#endif