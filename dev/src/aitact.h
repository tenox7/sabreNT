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
 * File   : aitact.h                              *
 * Date   : April, 1998                          *
 * Author : Dan Hammer                           *
 * Tactic definitions                            *
 *************************************************/
#ifndef __aitact_h
#define __aitact_h

#define AIPT_RQ_CONE             Pi_4
#define AIPT_FQ_CONE             Pi_4
#define AIPT_ASPECT_HEADON_CONE  Pi_4  
#define AIPT_ASPECT_REAR_CONE    Pi_4      
#define AIPT_ANGLEOFF_CONE       (_degree * 5.0)

#define AIPT_RTHREAT_DISTANCE    1800
#define AIPT_RTHREAT_CONE        Pi_4
#define AIPT_CANCEL_RTHREAT      0.8

#define AIPT_PURSUIT_DISTANCE        6200
#define AIPT_PURSUIT_CONE            (_degree * 60.0)
#define AIPT_CANCEL_PURSUIT          0.8
#define AIPT_WEAPON_THREAT_DISTANCE  2000 
#define AIPT_TURN_THRESHOLD          (_degree * 4.0)
#define AIPT_ALT_THRESHOLD           600
#define AIPT_RANGE_THRESHOLD         10
#define AIPT_HARDTURNALT_THRESHOLD   -180
#define AIPT_HARDTURNSPEED_THRESHOLD -100
#define AIPT_ENGAGE_DISTANCE         24000.0
#define AIPT_PLAYER_THREAT_DISTANCE  3000.0
#define AIPT_PLAYER_THREAT_ASPECT    (_degree * 12.0)
#define AIPT_ATTACKER_CHECK_INTERVAL 12.0

#ifdef __cplusplus

#include "aiobject.h"
#include "aiarray.h"
#include "aimath.h"
#include "aiplncaps.h"
#include "aipilcaps.h"

#define AIPT_NTACTICS   8
 
class sPlaneSpecs;
class aiPilotCaps;

class aiPilotTactics
{
public:
  enum { WEENIE_FIGHT, ANGLES_FIGHT, ENERGY_FIGHT, STRAFE, ROCKET, DIVE_BOMB, TORPEDO_BOMB,
			LEVEL_BOMB };

  enum { LAG_PURSUIT, LEAD_PURSUIT, PURE_PURSUIT };

  int   curTactic;                       /* one of WEENIE_FIGHT ... HEAVY_BOMB                          */
  int   planeTacticalCaps[AIPT_NTACTICS]; /* flags indicating which tactics we are capable of            */
  int   pursuitType;                     /* one of LAG_PURSUIT ... PURE_PURSUIT                         */
  aiREAL	targetRQCone;                    /* what constitutes a rear-quarter target                      */
  aiREAL targetFQCone;	                 /* what constitutes a target "on the nose"                     */
								
  aiREAL aspectHeadOnCone;                /* what constitutes a headon aspect to target                  */
  aiREAL	aspectRearCone;                  /* what constitutes a rear aspect to target                    */
  aiREAL	angleOffCone;                    /* what constitutes high/low angle off                         */

  aiREAL	rearThreatDistance;              /* what constitutes a rear threat                              */	
  aiREAL	rearThreatCone;     
  aiREAL	cancelRearThreat;

  aiREAL engageDistance;                  /* max distance for engage                                     */

  aiREAL pursuitDistance;                 /* pursuit stage parameters                                    */
  aiREAL pursuitCone;                              
  aiREAL cancelPursuit;

  aiREAL	turnThreshold;                   /* minimum turn rate to consitute a "turning" target            */
  aiREAL	altThreshold;                    /* minimum altitude difference to constitute a high/low target  */
  aiREAL altThresholdVery;                /* as above, for a "very" high/low target                       */
  aiREAL	rangeThreshold;			 /* minimum range difference to constitute closing/opening range */

  aiREAL playerThreatDistance;            /* max distance for considering player a threat                 */
  aiREAL playerThreatAspect;              /* aspect cone for considering player a threat                  */
  aiREAL attackerCheckInterval;           /* how often to check for attackers                             */

  aiPilotTactics();
  int TacticAvail(int tactic);
  void SetPlaneCaps(const aiPlaneCaps &); /* get tactical stuff from plane caps                         */
  void SetPilotCaps(const aiPilotCaps &); /* get tactical stuff from pilot caps                           */
};


#endif /* __cplusplus */
#endif
