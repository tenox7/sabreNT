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
 * File   : aipilcaps.h                           *
 * Date   : April, 1998                          *
 * Author : Dan Hammer                           *
 * Pilot caps class                              *
 * Overrides default values for various AI pilot *                                              
 * members.                                      *                                              
 *************************************************/
#ifndef __aipilcaps_h
#define __aipilcaps_h

#include "aimath.h"

#define aiMAXPILOTCAPS 32

/*
 *  equivalent structure for "C"-only situations
 */
typedef struct aipilot_caps_struct
{
  /*
   *   id
   */
  char capsId[32];

  /*
   *  tactics overrides
   */
  aiREAL targetRQCone;
  aiREAL	targetFQCone;
  aiREAL	aspectHeadOnCone;
  aiREAL	aspectRearCone;
  aiREAL	angleOffCone;
  aiREAL	rearThreatDistance;
  aiREAL rearThreatCone;     
  aiREAL cancelRearThreat;
  aiREAL	pursuitDistance;
  aiREAL	pursuitCone;         
  aiREAL	cancelPursuit;
  aiREAL	turnThreshold;
  aiREAL	altThreshold;
  aiREAL	rangeThreshold;
  aiREAL playerThreatDistance;
  aiREAL playerThreatAspect;
  aiREAL attackerCheckInterval;

  /*
   *  ground collision overrides
   */
  aiREAL grndColSecs;
  aiREAL	grndColAGL;
  aiREAL	grndColRcvrAGL;
  aiREAL	grndColPullUpAngle;

  /*
   * g-limit overrides
   */
  aiREAL posGLimit;
  aiREAL negGLimit;

  /*
   * marksmanship overrides
   */
  aiREAL	bulletRadiusMin;
  aiREAL	bulletRadiusMax;

  /*
   *  gunners skill overrides
   */
  aiREAL gunnersPitchRate;
  aiREAL gunnersYawRate;
  ai_attitude gunnersJiggle;
  aiREAL gunnersBulletRadiusMin;
  aiREAL gunnersBulletRadiusMax;

} aipilot_caps;

#ifdef __cplusplus

#include "aimath.h"
#include "aiobject.h"
#include "aiarray.h"

class aiPilotCaps : public aiObject
{
public:
  /*
   *   id
   */
  char capsId[32];

  /*
   *  tactics overrides
   */
  aiREAL targetRQCone;
  aiREAL	targetFQCone;
  aiREAL	aspectHeadOnCone;
  aiREAL	aspectRearCone;
  aiREAL	angleOffCone;
  aiREAL	rearThreatDistance;
  aiREAL rearThreatCone;     
  aiREAL cancelRearThreat;
  aiREAL	pursuitDistance;
  aiREAL	pursuitCone;         
  aiREAL	cancelPursuit;
  aiREAL	turnThreshold;
  aiREAL	altThreshold;
  aiREAL	rangeThreshold;
  aiREAL playerThreatDistance;
  aiREAL playerThreatAspect;
  aiREAL attackerCheckInterval;

  /*
   *  ground collision overrides
   */
  aiREAL grndColSecs;
  aiREAL	grndColAGL;
  aiREAL	grndColRcvrAGL;
  aiREAL	grndColPullUpAngle;

  /*
   * g-limit overrides
   */
  aiREAL posGLimit;
  aiREAL negGLimit;

  /*
   * marksmanship overrides
   */
  aiREAL	bulletRadiusMin;
  aiREAL	bulletRadiusMax;

  /*
   *  gunners skill overrides
   */
  aiREAL gunnersPitchRate;
  aiREAL gunnersYawRate;
  aiAttitude gunnersJiggle;
  aiREAL gunnersBulletRadiusMin;
  aiREAL gunnersBulletRadiusMax;

  aiPilotCaps()
    {}

  aiPilotCaps(const aipilot_caps &);

  static aiObjectArray aiPilotCapsArray;
  static void AddaiPilotCaps(aiPilotCaps &);
  static void AddaiPilotCaps(aipilot_caps *);
  static aiPilotCaps *GetaiPilotCaps(const char *capsId);
  static void FlushaiPilotCaps();
};
#endif

#endif

