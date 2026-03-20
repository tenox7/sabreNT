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
 * File   : aipilot3.cpp                         *
 * Date   : April, 1998                          *
 * Author : Dan Hammer                           *
 *************************************************/
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>

#include "aipilot.h"
#include "aipildef.h"

/*************************************************************
 * Maneuver update routines                                  *
 *************************************************************/

/*************************************************************
 * Air combat engage routines                                *
 *************************************************************/
// Top level engage maneuver
void aiPilot::DoEngage(aiManeuverState &mv)
{
	enum { INIT, INITX, BASE,REAR, PURSUE, HEADON, CLIMB, GRAB_ALTITUDE,
			 EXTEND, NOSEON, JINK, AVOID_GROUND, TARGET_DEAD, AVOID_COLLISION,
			 WEENIE, EXIT_HEADON, TURN_FIGHT };

	EnableGroundCollision(1);
	curTarget = &engageTarget;
	GetTargetFlags(engageTarget);
	if (TARGET_ACTIVE)
		GetTargetGeometry(TARGET_IDX,TARGET_GEOMETRY);

	engageMaxSpeed = flightModel->GetMaxSpeedFPS() * engageMaxSpeedMul;

	/**********************************************************
	 * check for things which override current state, such as *
	 * ground collisions, immediate rear threats              *
	 **********************************************************/
	/*
	 * If target is "dead", go straight and level and wait for further
	 * instructions
	 */
	if (!TARGET_ACTIVE && mv.state != TARGET_DEAD)
	{
		LOG("killed target");
		aiPilot *targetPilot = GetaiPilot(TARGET_IDX);
		if (targetPilot != NULL)
			Broadcast(commWINGMAN_KILL,commCHANNEL_AFFILIATION,
						 commPRIORITY_STANDARD,(void *)targetPilot);
		ClearTargetPilot();
		ClearManeuverStackTo(mv.stackLevel);
		PushManeuver(aiManeuver::STRAIGHT_AND_LEVEL,AIMNVR_LOOPBIT);
		mv.state = TARGET_DEAD;
 	}

	/*
	 * Always take a shot opportunity
	 */
	if (TARGET_ACTIVE)
		Shoot();

	/* check ground collision */
	if (mv.state != AVOID_GROUND)
	{
		aiREAL secs,agl;
		aiREAL rcvrAGL;
		secs = grndColSecs;
		agl  = grndColAGL;
		rcvrAGL = grndColRcvrAGL;

		if (curManeuver->maneuver == aiManeuver::GUN_ATTACK)
		{
			secs *= 0.8;
			agl *= 0.8;
			rcvrAGL *= 0.8;
		}
		else if (mv.state == JINK)
		{
			secs *= 0.3;
			agl *= 0.3;
			rcvrAGL *= 0.3;
		}

		if (flightModel->GetHeightAGL() + deltaAltitude * secs <= agl)
		{
			mv.data3 = (aiREAL) mv.state;
			mv.data4 = flightModel->GetHeightAGL();
			mv.state = AVOID_GROUND;
			ClearManeuverStackTo(mv.stackLevel);
			PushManeuver(aiManeuver::PULLUP,0,
						rcvrAGL,
						grndColPullUpAngle);
			return;
		}
	}

	/* check six */
	if (TARGET_ACTIVE && mv.state > INITX  && mv.state < JINK )
	{
		aiREAL rThreatDist;
		rThreatDist = REAR_THREAT_DISTANCE;
		if (TARGET_FLAGS.headOnAspect	&& ((TARGET_RANGE <= (rThreatDist)) &&
			(TARGET_RANGE >= 100.0) && TARGET_FLAGS.rear))
		{
			ClearManeuverStackTo(mv.stackLevel);
			PushManeuver(aiManeuver::RQ_EVADE);
			mv.state = JINK;
		}
	}

	/* try to avoid collision with target 
	if (mv.state < AVOID_GROUND)
	{
		if (TARGET_FLAGS.forward && TARGET_FLAGS.rangeClose &&
			 (collisionAvoidSecs * fabs(TARGET_DRANGE)) > TARGET_RANGE)
		{
			if (aiFlipCoin())
				mv.data0 = aiManeuver::LEFT;
			else
				mv.data0 = aiManeuver::RIGHT;
			ClearManeuverStackTo(mv.stackLevel);
			mv.state = AVOID_COLLISION;
		}
	}
	*/

	switch (mv.state)
	{
	case INIT:
		ControlsOff();
		flightModel->SetWEP(1);
		flightModel->SetEngineControlPer(1.0);
		GetWeaponLimits(weaponLimits);
		if (flightModel->GetAirSpeedFPS() <= flightModel->GetStallSpeedFPS() * 1.2)
		{
			PushManeuver(aiManeuver::EXTEND,0,flightModel->GetStallSpeedFPS() * 1.2,
							 10,extendAngle);
			mv.state = INITX;
		}
		else
			mv.state = BASE;
		break;

	case INITX:
		if (mnvrStackPtr == mv.stackLevel)
			mv.state = BASE;
		break;

	case BASE:
	{
		flightModel->SetWEP(1);
		flightModel->SetEngineControlPer(1.0);

		if (IS_WEENIE_FIGHT)
			mv.state = WEENIE;
		else if (TARGET_RANGE <= gunAttackDistance * 1.5)
		{
			PushManeuver(aiManeuver::TURN_FIGHT);
			mv.state = TURN_FIGHT;
		}
		else if (TARGET_FLAGS.rear)
		{
			PushManeuver(aiManeuver::RQ_ENGAGE);
			mv.state = REAR;
		}
		/* Head on situation ? */
		else if ( TARGET_FLAGS.headOnAspect &&
				  TARGET_FLAGS.forward )
		{
			PushManeuver(aiManeuver::FQ_ENGAGE);
			mv.state = HEADON;
		}
		/* Pursue for gun attack */
		else if (TARGET_RANGE < PURSUIT_DISTANCE * 1.1)
		{
			PushManeuver(aiManeuver::PURSUIT);
			mv.state = PURSUE;
		}
		else if (TARGET_FLAGS.veryHigh && TARGET_FLAGS.forward )
		{
			PushManeuver(aiManeuver::CLIMB,0,TARGET_ALTITUDE + 2000,300);
             mv.state = CLIMB;
		}
      /* or extend */
      else if (flightModel->GetAirSpeedFPS() <= flightModel->GetStallSpeedFPS())
      {
          PushManeuver(aiManeuver::EXTEND,0,flightModel->GetCornerSpeedFPS(),
								20.0,
								extAngle);
          mv.state = EXTEND;
      }
      if (mv.state == BASE)
      {
        mv.state= NOSEON;
        SetNoseOn(TARGET_FLAGS.onNose,LEAD_POSITION,LEAD_PITCH,LEAD_YAW);
      }
	}
		break;

	case NOSEON:
		SetNoseOn(TARGET_FLAGS.onNose,LEAD_POSITION,LEAD_PITCH,LEAD_YAW);
		if (TARGET_FLAGS.onNose)
			mv.state = BASE;
		break;

	/* Rear threat */
	case JINK:
		if (
			 TARGET_RANGE > REAR_THREAT_DISTANCE * 1.2
			 || TARGET_FLAGS.forward
		  )
		ClearManeuverStackTo(mv.stackLevel);
		if (mnvrStackPtr == mv.stackLevel)
			mv.state = BASE;
		break;

	/* Head on */
	case HEADON:
		if (TARGET_FLAGS.rear)
			ClearManeuverStackTo(mv.stackLevel);
		if (mnvrStackPtr == mv.stackLevel)
			mv.state = BASE;
		break;

	case EXIT_HEADON:
		if (TARGET_RANGE > PURSUIT_DISTANCE * 1.3)
			ClearManeuverStackTo(mv.stackLevel);
		if (mnvrStackPtr == mv.stackLevel)
			mv.state = BASE;
		break;

	/* Pursuit */
	case PURSUE:
		if (TARGET_FLAGS.rear || TARGET_RANGE > PURSUIT_DISTANCE * 1.3)
			ClearManeuverStackTo(mv.stackLevel);
		if (mnvrStackPtr == mv.stackLevel)
			mv.state = BASE;
		break;
	
	/* Target in rear quarter */
	case REAR:
		if (TARGET_FLAGS.veryLow && !TARGET_FLAGS.rear)
			ClearManeuverStackTo(mv.stackLevel);
		if (mnvrStackPtr == mv.stackLevel)
			mv.state = BASE;
		break;

	case CLIMB:
		if (!TARGET_FLAGS.high || TARGET_FLAGS.rear)
			ClearManeuverStackTo(mv.stackLevel);
		else if (TARGET_RANGE < PURSUIT_DISTANCE * 0.8)
			ClearManeuverStackTo(mv.stackLevel);
		if (mnvrStackPtr == mv.stackLevel)
		{
			if (!TARGET_FLAGS.forward)
				mv.state = NOSEON;
			else
				mv.state = BASE;
		}
		break;

	case TARGET_DEAD:
		if (mnvrStackPtr == mv.stackLevel)
			mv.state = BASE;
		break;

	case AVOID_GROUND:
		if (flightModel->GetHeightAGL() >= mv.data4)
			ClearManeuverStackTo(mv.stackLevel);
		if (mnvrStackPtr == mv.stackLevel)
		{
			/*
			int saved_state = (int) mv.data3;
			if (saved_state != PURSUE && saved_state != JINK)
			{
				aiREAL rcvrAlt;
				if (IS_ENERGY_FIGHT)
					rcvrAlt = flightModel->GetHeightAGL() + 3000.0;
				else
					rcvrAlt = flightModel->GetHeightAGL() + 1000.0;
				PushManeuver(aiManeuver::CLIMB,0,rcvrAlt,300);
				mv.state = GRAB_ALTITUDE;
			}
			else
			*/
				mv.state = BASE;
		}
		break;

	case GRAB_ALTITUDE:
		if (TARGET_RANGE <= PURSUIT_DISTANCE)
			ClearManeuverStackTo(mv.stackLevel);
		if (mnvrStackPtr == mv.stackLevel)
			mv.state = BASE;
		break;

	case WEENIE:
		/* 
		 *  we're a bomber or transport so all we can do
		 *  is fly and pray
		 */
		if (mnvrStackPtr == mv.stackLevel)
			PushManeuver(aiManeuver::STRAIGHT_AND_LEVEL);
		break;

	case EXTEND:
		if (TARGET_RANGE < (PURSUIT_DISTANCE * 0.8))
			ClearManeuverStackTo(mv.stackLevel);
		if (!TARGET_FLAGS.forward)
			ClearManeuverStackTo(mv.stackLevel);
		if (mnvrStackPtr == mv.stackLevel)
			mv.state = BASE;
		break;

	case AVOID_COLLISION:
		if (!TARGET_FLAGS.forward ||
			 !TARGET_FLAGS.rangeClose)
			mv.state = BASE;
		else
		{
			ControlsOff();
			if (mv.data0 == aiManeuver::LEFT)
			{
				SETROLLPID(-_degree*60.0);
				SETCONTROL(yawRateCtl,-Pi);
			}
			else
			{
				SETROLLPID(_degree*60.0);
				SETCONTROL(yawRateCtl,Pi);
			}
			SETCONTROL(gCtl,posGLimit * 0.7);
		}
		break;

	case TURN_FIGHT:
		if (TARGET_RANGE > gunAttackDistance * 3.0)
			ClearManeuverStackTo(mv.stackLevel);
		/*
		if (IS_ENERGY_FIGHT && !CAN_ANGLES_FIGHT)
		{
			if (TARGET_FLAGS.high)
				ClearManeuverStackTo(mv.stackLevel);
			if (flightModel->GetAirSpeedFPS() <= flightModel->GetStallSpeedFPS() * 1.2)
				ClearManeuverStackTo(mv.stackLevel);
		}
		*/
		if (mnvrStackPtr == mv.stackLevel)
			mv.state = BASE;
		break;

	}
}

// Attempt to close with a target to achieve
// weapons parameters
void aiPilot::DoPursuit(aiManeuverState &mv)
{
	enum { BASE, GUN_ATTACK, CLIMB };
	switch (mv.state)
	{
	case BASE:
		if (TARGET_RANGE <= gunAttackDistance)
		{
			/* maneuver for guns */
			PushManeuver(aiManeuver::GUN_ATTACK);
			mv.state = GUN_ATTACK;
		}
		else
			SetNoseOn(TARGET_FLAGS.onNose,GUN_LEAD_POSITION,GUN_LEAD_PITCH,GUN_LEAD_YAW);
		break;

	case GUN_ATTACK:
		if (TARGET_FLAGS.rear || TARGET_FLAGS.veryHigh ||
					TARGET_RANGE > gunAttackDistance * 1.3)
			ClearManeuverStackTo(mv.stackLevel);
		if (mnvrStackPtr == mv.stackLevel)
			mv.state = BASE;
		break;

	case CLIMB:
		if (!TARGET_FLAGS.high || TARGET_RANGE < gunAttackDistance)
			 ClearManeuverStackTo(mv.stackLevel);
		if (mnvrStackPtr == mv.stackLevel)
			 mv.state = BASE;
		break;
	}
}

// Deal with an immediate threat in our rear quarter
void aiPilot::DoRQEvade(aiManeuverState &mv)
{
	switch (mv.state)
	{

	case 0:
		{
			mv.data3 = 0.0;
			mv.data4 = (3.00) + aiRandPer() * 3.0;
	
			int choice = aiRand(0,2);
			switch (choice)
			{
			case 0:
				PushManeuver(aiManeuver::BREAK_TURN,AIMNVR_RIGHT,posGLimit);
				break;

			case 1:
				PushManeuver(aiManeuver::BREAK_TURN,AIMNVR_LEFT,posGLimit);
				break;

			case 2:
				PushManeuver(aiManeuver::JINK);
				break;
			}
			
			mv.state = 1;
		}
		break;

	case 1:
		mv.data3 += timeFrame;
		if (mv.data3 >= mv.data4)
			ClearManeuverStackTo(mv.stackLevel);
		if (mv.stackLevel == mnvrStackPtr)
			mv.state = 0;
		break;
	}
}	

// Deal with a target in the rear quarter,
void aiPilot::DoRQEngage(aiManeuverState &mv)
{
	switch (mv.state)
	{
	case 0:
		if (flightModel->GetAirSpeedFPS() < flightModel->GetCornerSpeedFPS() * 0.5)
		{
		  PushManeuver(aiManeuver::EXTEND,0,flightModel->GetCornerSpeedFPS() * 0.8,10,extAngle);
		  mv.state = 4;
		}

	case 1:
		if (flightModel->GetAirSpeedFPS() <= flightModel->GetStallSpeedFPS() * 1.1)
		{
			PushManeuver(aiManeuver::EXTEND,0,flightModel->GetStallSpeedFPS() * 1.2,
							 10,extAngle);
			mv.state = 3;
		}
		else 
		{
			int choice = aiRand(0,3);
			switch (choice)
			{
			case 0:
				PushManeuver(aiManeuver::IMMELMAN,0,immelmanGs);
				mv.state = 3;
				break;

			case 1:
				if (flightModel->GetHeightAGL() >= splitSAltMin &&
					TARGET_FLAGS.low)
				{
					PushManeuver(aiManeuver::SPLIT_S,0,splitSGs);
					mv.state = 3;
					break;
				}
			case 2:
				{
					int turn_bits;
					if (TARGET_FLAGS.rear)
					{
						if (aiFlipCoin())
							turn_bits = AIMNVR_RIGHT;
						else
							turn_bits = AIMNVR_LEFT;
					}
					else
					{
						if (TARGET_YAW < 0.0)
							turn_bits = AIMNVR_LEFT;
						else
							turn_bits = AIMNVR_RIGHT;
					}
					mv.data3 = (aiREAL) turn_bits;
					/*
					if (aiFlipCoin())
						PushManeuver(aiManeuver::CLIMBING_TURN,turn_bits);
					else
					*/
						PushManeuver(aiManeuver::BREAK_TURN,turn_bits);
					mv.state = 2;
				}
				break;
			}
		}
		break;

	case 2:
		if (TARGET_FLAGS.forward ||
			 (TARGET_FLAGS.veryLow && !TARGET_FLAGS.rear))
		{
			ClearManeuverStackTo(mv.stackLevel);
			mv.done = 1;
		}
		if (mnvrStackPtr == mv.stackLevel)
			mv.state = 1;
		break;

	case 3:
		if (TARGET_FLAGS.forward)
			ClearManeuverStackTo(mv.stackLevel);
		if (mnvrStackPtr == mv.stackLevel)
		{
			if (TARGET_FLAGS.forward)
				mv.done = 1;
			mv.state = 1;
		}
		break;

	case 4:
		if (TARGET_FLAGS.forward)
			mv.done = 1;
		if (mnvrStackPtr == mv.stackLevel)
			mv.state = 1;
		break;

	case 5:
		if (TARGET_FLAGS.rangeClose && TARGET_RANGE < PURSUIT_DISTANCE * 0.5
			&& TARGET_FLAGS.rear && TARGET_FLAGS.headOnAspect)
		{
			ClearManeuverStackTo(mv.stackLevel);
			PushManeuver(aiManeuver::BREAK_TURN,(int)mv.data3);
			mv.state = 2;
		}
		if (TARGET_FLAGS.forward)
			mv.done = 1;
		break;

	}
}

// Deal with a head-on situation
void aiPilot::DoFQEngage(aiManeuverState &mv)
{
	enum { INIT, HEADON, LEADTURN, GUNATTACK };

	switch (mv.state)
	{
	
	case INIT:
		{
			aiPilot *pil = GetaiPilot(TARGET_IDX);
			if (pil->IsPlayer())
			{
				mv.state = HEADON;
				return;
			}
			if (aiFlipCoin())
				mv.state = HEADON;
			else if (TARGET_FLAGS.headOnAspect)
			{
				if (aiFlipCoin())
					mv.data3 = -1.0;
				else
					mv.data3 = 1.0;
				mv.state = LEADTURN;
			}
			else
				mv.state = HEADON;
		}
		break;

	case HEADON:
		if (TARGET_RANGE + TARGET_DRANGE <= gunAttackDistance)
		{
			PushManeuver(aiManeuver::GUN_ATTACK);
			mv.state = GUNATTACK;
		}
		else
			SetNoseOn(TARGET_FLAGS.onNose,GUN_LEAD_POSITION,GUN_LEAD_PITCH,GUN_LEAD_YAW);

		break;

	case LEADTURN:
		if (TARGET_RANGE + TARGET_DRANGE <= gunAttackDistance)
		{
			PushManeuver(aiManeuver::GUN_ATTACK);
			mv.state = GUNATTACK;
		}
		else
		{
			aiPilot *pil;
			aiPoint w;
			aiPoint b0;
			aiAttitude att;
			aiVector v;
			aiREAL r;

			
			pil = GetaiPilot(TARGET_IDX);
			if (pil)
			{
				r = TARGET_RANGE;
				if (r > 500.0)
					r = 500.0;
				b0.x = r * mv.data3;
				pil->BodyPoint2WorldPoint(b0,w);
				GetAttitude(w,v,att);
				SetNoseOn(0,w,att.pitch,att.yaw);
			}
			else
				SetNoseOn(TARGET_PITCH,TARGET_YAW);
		}
		break;

	case GUNATTACK:
		if (TARGET_FLAGS.tailAspect || !TARGET_FLAGS.forward ||
			 (TARGET_RANGE + TARGET_DRANGE > gunAttackDistance))
		{
			ClearManeuverStackTo(mv.stackLevel);
			mv.state = 0;
			mv.done = 1;
		}
		break;
	}
}

inline aiREAL GET_ROLL_ANGLE(aiREAL x, aiREAL z)
{
aiREAL result;
	z += eps;

	if (x > 0 && z > 0)
		result = (atan(x/z));
	else if (x > 0 && z < 0)
		result = (Pi + atan(x/z));
	else if (x < 0 && z > 0)
		result = (atan(x/z));
	else
		result = (-Pi + atan(x/z));

	return result;
}

// Maneuver to bring guns on target
void aiPilot::DoGunAttack(aiManeuverState &)
{
	aiREAL rollNeeded;
	aiPoint rollPoint;
	aiVector vel;

	if ((fabs(GUN_LEAD_PITCH) < _degree * 3.0 && 
		 fabs(GUN_LEAD_YAW) < _degree *3.0 ) &&
		 (TARGET_FLAGS.lowAOT || TARGET_FLAGS.highAOT)
		 && TARGET_FLAGS.onNose)
		rollNeeded = atan(flightModel->GetAirSpeedFPS() *
						GUN_LEAD_YAW / 32.2);
	else
	{
		vel = TARGET_VELOCITY;
		vel *= GUN_TOF;
		rollPoint = GUN_LEAD_POSITION + vel;
		rollNeeded = Point2Roll(rollPoint);
	}

	ControlsOff();
	SETROLLPID(rollNeeded);
	SETPITCHPID(GUN_LEAD_PITCH,LEAD_PITCH_RATE);
	SETYAWPID(GUN_LEAD_YAW,LEAD_YAW_RATE);

	if (TARGET_FLAGS.forward && TARGET_FLAGS.lowAOT && TARGET_FLAGS.rangeClose &&
		 TARGET_RANGE < 800)
		SETCONTROL(speedCtl,CalcClosureSpeed(curTarget->geometry));
	else
	{
		flightModel->SetEngineControlPer(1.0);
		flightModel->SetWEP(1);
		flightModel->SetSpeedBreaks(0);
	}
}

void aiPilot::DoTurnFight(aiManeuverState &mv)
{
	enum { aiTF_BASE=0, aiTF_TURN, aiTF_BREAK };

	aiVector   mat[3];
	aiVector   transMat[3];
	aiVector   ourPosition;
	aiVector   rollPosition;
	aiVector   pitchPosition;
	aiAttitude att;
	aiAttitude rollAtt;
	aiVector   targetWorldVect;
	aiVector   targetBodyVect;
	aiVector   rollVect;
	aiREAL     rollNeeded;
	aiVector   tmp;
	aiREAL     c2;
	aiVector   vel;

	switch (mv.state)
	{
	case aiTF_BASE:
		mv.state = aiTF_TURN;

	case aiTF_TURN:
		if (TARGET_FLAGS.forward && TARGET_RANGE <= gunAttackDistance)
		{
			DoGunAttack(mv);
			break;
		}

		/*
		if (TARGET_FLAGS.rear &&
			!TARGET_FLAGS.high &&
			!TARGET_FLAGS.low
			)
		{
			if (TARGET_YAW >= 0.0)
				PushManeuver(aiManeuver::BREAK_TURN,AIMNVR_RIGHT,posGLimit);
			else
				PushManeuver(aiManeuver::BREAK_TURN,AIMNVR_LEFT,posGLimit);
			mv.state = aiTF_BREAK;
			break;
		}
		*/

		/* get our position and attitude */
		GetPositionAndAttitude((aiPoint &)ourPosition,att);
		/* set roll to 0.0 */
		att.roll = 0.0;
		/* build matrix & transpose */
		aiMatrix3RPY(att,mat);
		aiMatrix3Transpose(mat,transMat);
		vel = TARGET_VELOCITY;
		vel *= GUN_TOF;

		rollPosition = GUN_LEAD_POSITION + vel;
		/* get vector to roll position in world coords */
		targetWorldVect = rollPosition - ourPosition;
		GetAttitude((const aiPoint &)rollPosition,tmp,att);
		/* convert to body coords */
		aiMatrix3Rotate(transMat,targetWorldVect,targetBodyVect);    
		/* get roll needed to align with target */
		rollNeeded = GET_ROLL_ANGLE(-targetBodyVect.x,targetBodyVect.z);
		ControlsOff();
		SETROLLPID(rollNeeded,0.0,1.0);
		c2 = 1.15;
		SETPITCHPID(GUN_LEAD_PITCH+LEAD_PITCH,0.0,c2);
		SETYAWPID(0.0,0.0,1.0);
		break;


	case aiTF_BREAK:
		if (TARGET_FLAGS.forward)
			ClearManeuverStackTo(mv.stackLevel);
		if (mnvrStackPtr == mv.stackLevel)
			mv.state = aiTF_BASE;
		break;

	}
}


void aiPilot::DoJink(aiManeuverState &mv)
{
	switch (mv.state)
	{
	case 0:
		if (aiFlipCoin())
		{
			mv.data0 = Pi;
			mv.data1 = -1.0;
		}
		else
		{
			mv.data0 = -Pi;
			mv.data1 = 1.0;
		}
		mv.state = 1;
		break;

	case 1:
		{
			aiREAL sgn;
			if (mv.data0 < 0)
				sgn = 1.0;
			else
				sgn = -1.0;
			mv.data1 = 5.0;

			if (flightModel->GetHeightAGL() > grndColAGL)
			{
				mv.data0 = sgn*45.0 + sgn*aiRandPer()*50.0;
				PushManeuver(aiManeuver::SNAP_ROLL,0,mv.data0,mv.data1);
			}
			else
			{
				mv.data0 = sgn*30.0 + sgn*aiRandPer()*30.0;
				PushManeuver(aiManeuver::LEVEL_ROLL,0,mv.data0,mv.data1);
			}				
			mv.data0 *= _degree;
			mv.state = 2;
		}
		break;

	case 2:
		SETCONTROL(speedCtl,flightModel->GetMaxSpeedFPS());
		if (fabs(flightModel->GetRoll() - mv.data0) <= _degree * 10.0)
			ClearManeuverStackTo(mv.stackLevel);
		if (mnvrStackPtr == mv.stackLevel)
		{
			mv.data2 = 0.0;
			mv.data3 = 2.5 + aiRandPer() * 1.5;
			mv.state = 3;
		}
		break;

	case 3:
		mv.data2 += timeFrame;
		if (mv.data2 >= mv.data3)
			mv.state = 1;
		ControlsOff();
		SETROLLPID(mv.data0);
		if (aiFlipCoin())
			SETYAWPID(Pi);
		else
			SETYAWPID(-Pi);
		SETCONTROL(gCtl,posGLimit);
		SETCONTROL(speedCtl,flightModel->GetMaxSpeedFPS());
		break;
	}
}


void aiPilot::DoChandelle(aiManeuverState &)
{

}
