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
 * File   : stact.C                              *
 * Date   : April, 1998                          *
 * Author : Dan Hammer                           *
 *************************************************/
#include <stdio.h>
#include <string.h>
#include "aimath.h"
#include "aipilcaps.h"
#include "aitact.h"

/************************************************
 * aiPilotTactics methods                        *
 ************************************************/

aiPilotTactics::aiPilotTactics()
{
  curTactic = ANGLES_FIGHT;
  pursuitType = LEAD_PURSUIT;
  targetRQCone = AIPT_RQ_CONE;
  targetFQCone = AIPT_FQ_CONE;
  aspectHeadOnCone = AIPT_ASPECT_HEADON_CONE;
  aspectRearCone = AIPT_ASPECT_REAR_CONE;
  angleOffCone = AIPT_ANGLEOFF_CONE;
  rearThreatDistance = AIPT_RTHREAT_DISTANCE;
  rearThreatCone = AIPT_RTHREAT_CONE;
  cancelRearThreat = AIPT_CANCEL_RTHREAT;
  pursuitDistance = AIPT_PURSUIT_DISTANCE;
  pursuitCone = AIPT_PURSUIT_CONE;
  cancelPursuit = AIPT_CANCEL_PURSUIT;
  turnThreshold = AIPT_TURN_THRESHOLD;
  altThreshold = AIPT_ALT_THRESHOLD;
  rangeThreshold = AIPT_RANGE_THRESHOLD;
  engageDistance = AIPT_ENGAGE_DISTANCE;
  playerThreatDistance = AIPT_PLAYER_THREAT_DISTANCE;
  playerThreatAspect = AIPT_PLAYER_THREAT_ASPECT;
  attackerCheckInterval = AIPT_ATTACKER_CHECK_INTERVAL;
  
  planeTacticalCaps[WEENIE_FIGHT] = 1;
  planeTacticalCaps[ENERGY_FIGHT] = 1;
  planeTacticalCaps[ANGLES_FIGHT] = 1;
  planeTacticalCaps[STRAFE]  = 1;
  planeTacticalCaps[ROCKET] = 1;
  planeTacticalCaps[DIVE_BOMB] = 0;
  planeTacticalCaps[TORPEDO_BOMB] = 0;
  planeTacticalCaps[LEVEL_BOMB] = 0;
}

int aiPilotTactics::TacticAvail(int tactic)
{
int result = 0;
  if (tactic >= 0 && tactic < AIPT_NTACTICS)
    result = planeTacticalCaps[tactic] != 0;
  return (result);
}

void aiPilotTactics::SetPlaneCaps(const aiPlaneCaps &caps)
{
  planeTacticalCaps[ENERGY_FIGHT] = caps.canDoEnergyFight;
  planeTacticalCaps[ANGLES_FIGHT] = caps.canDoAnglesFight;
  planeTacticalCaps[STRAFE] = caps.canStrafe;
  planeTacticalCaps[ROCKET] = caps.canRocket;
  planeTacticalCaps[DIVE_BOMB] = caps.canDiveBomb;
  planeTacticalCaps[TORPEDO_BOMB] = caps.canTorpedoBomb;
  planeTacticalCaps[LEVEL_BOMB] = caps.canLightBomb || caps.canHeavyBomb;
}

void aiPilotTactics::SetPilotCaps(const aiPilotCaps &pcaps)
{
  targetRQCone = pcaps.targetRQCone;
  targetFQCone = pcaps.targetFQCone;
  aspectHeadOnCone = pcaps.aspectHeadOnCone;
  aspectRearCone = pcaps.aspectRearCone;
  angleOffCone = pcaps.angleOffCone;
  rearThreatDistance = pcaps.rearThreatDistance;
  rearThreatCone = pcaps.rearThreatCone;     
  cancelRearThreat = pcaps.cancelRearThreat;
  pursuitDistance = pcaps.pursuitDistance;
  pursuitCone = pcaps.pursuitCone;         
  cancelPursuit = pcaps.cancelPursuit;
  turnThreshold = pcaps.turnThreshold;
  altThreshold = pcaps.altThreshold;
  rangeThreshold = pcaps.rangeThreshold;
  playerThreatDistance = pcaps.playerThreatDistance;
  playerThreatAspect = pcaps.playerThreatAspect;
  attackerCheckInterval = pcaps.attackerCheckInterval;
}


