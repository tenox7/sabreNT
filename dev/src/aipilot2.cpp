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
 * File   : aipilot2.cpp                         *
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
 * Basic maneuvers                                           *
 *************************************************************/
// straight and level maneuver
void aiPilot::DoStraightAndLevel(aiManeuverState &mv)
{
	switch (mv.state)
	{
	case 0:
		PushManeuver(aiManeuver::LEVEL_ROLL,0,0.0,1);
		mv.state = 1;
		break;

	case 1:
		if (mnvrStackPtr == mv.stackLevel)
			mv.state = 2;
		break;

	case 2:
		ControlsOff();
		SETROLLPID(0.0);
		SETPITCHPID(levelAttitude.pitch);
		SETYAWPID(0.0);
		if (fabs(flightModel->GetPitch()) <= _degree * 2.0)
			mv.done = 1;
		break;
   }
}


// roll, keeping nose level with horizion
// data0 = roll desired (degrees)
// data1 = minimum (degrees)
void aiPilot::DoLevelRoll(aiManeuverState &mv)
{
	switch (mv.state)
	{
	case 0:
		ControlsOff();
		mv.data1 *= _degree;
		mv.data0 *= _degree;
		mv.state = 1;

	case 1:
		SETROLLPID(mv.data0,mv.data1);
		SETPITCHPID(levelAttitude.pitch);
		SETYAWPID(levelAttitude.yaw);
		if (fabs(aiGetCircularDistance(mv.data0,flightModel->GetRoll())) <=	mv.data1)
			mv.done = 1;
		break;
	}
}

// roll, keeping nose +/- x degrees with horizion
// data0 = roll desired (in degrees)
// data1 = minimum
// data2 = x (degrees above or below horizon)
void aiPilot::DoPitchedRoll(aiManeuverState &mv)
{
	switch (mv.state)
	{
	case 0:
	{
		ControlsOff();
		mv.data0 *= _degree;
		mv.data1 *= _degree;
		mv.data2 *= _degree;
		/* calc z for given degree above or below horizon */
		mv.data3 = tan(mv.data2) * 1000.0;
		mv.state = 1;
	}

	case 1:
	{
		aiAttitude att;
		if (rollCtl.flag)
			mv.done = 1;
		GetLevelAttitude(att,mv.data3);
		SETROLLPID(mv.data0,mv.data1);
		SETPITCHPID(att.pitch);
		SETYAWPID(att.yaw);
		break;
	}


	}
}

// Dive to gain air speed
// data0 = airspeed in FPS to achieve
// data1 = minimum
// data2 = dive angle in degrees
void aiPilot::DoExtend(aiManeuverState &mv)
{
	switch (mv.state)
	{
	case 0:
	{
		/* 
			mv.data2 contains dive angle in degrees,
			convert to rads
		*/
		ControlsOff();
		mv.data2 *= _degree;
		mv.state = 1;
		flightModel->SetWEP(1);
		flightModel->SetEngineControlPer(1.0);
		PushManeuver(aiManeuver::STRAIGHT_AND_LEVEL);
		break;
	}

	case 1:
	{
		if (
			fabs(flightModel->GetPitch()) < Pi_4 &&
			fabs(flightModel->GetRoll()) < Pi_4
			)
		{
			ClearManeuverStackTo(mv.stackLevel);
			mv.state = 2;
		}
		else if (mnvrStackPtr == mv.stackLevel)
			mv.state = 2;
	}
		break;

	case 2:
	{
		aiREAL airSpeedFPS = flightModel->GetAirSpeedFPS();
		if (airSpeedFPS >= mv.data0 || mv.data0 - airSpeedFPS <= mv.data1)
    	  mv.done = 1;
		else
		{
			ControlsOff();
         flightModel->SetYawControlPer(0.0);
			aiREAL distance = aiGetCircularDistance(-mv.data2,flightModel->GetPitch());
			SETPITCHPID(-distance);
			SETYAWPID(0.0);
			SETROLLPID(0.0,_half_degree);
		}
		break;
	}

	}
}

// roll to an inverted position
void aiPilot::DoInvert(aiManeuverState &mv)
{
	switch (mv.state)
	{
	case 0:
		PushManeuver(aiManeuver::LEVEL_ROLL,0,180,1.0);
		mv.state = 1;
		break;

	case 1:
		if (mnvrStackPtr == mv.stackLevel)
			mv.done = 1;
		break;
	}
}

// do a standard turn
// flags = direction
// data0 = bank angle in degrees
// data1 = speed to maintain in mph
void aiPilot::DoStandardTurn(aiManeuverState &mv)
{
	switch (mv.state)
	{
	case 0:
		mv.data0 = fabs(mv.data0) * _degree;
		if (mv.GetManeuverDirection() == aiManeuver::LEFT)
			mv.data0 *= -1;
		if (flightModel->GetAirSpeedFPS() <= flightModel->GetStallSpeedFPS())
		{
			PushManeuver(aiManeuver::EXTEND,0,flightModel->GetStallSpeedFPS(),
					 10,extAngle);
			mv.state = 4;
		}
		else
			mv.state = 1;
		break;

	case 1:
		PushManeuver(aiManeuver::LEVEL_ROLL,0,mv.data0,1.0);
		mv.state = 2;
		break;

	case 2:
		if (mnvrStackPtr == mv.stackLevel)
			mv.state = 3;
		break;

	case 3:
	{
		aiREAL load_needed;
		ControlsOff();
		load_needed = 1.0 / cos(fabs(flightModel->GetRoll()));
		SETCONTROL(gCtl,load_needed);
		SETROLLPID(mv.data0);
		SETYAWPID(levelAttitude.yaw);
		SETCONTROL(speedCtl,sMPH2FPS(mv.data1));
		break;
	}

	case 4:
		if (mnvrStackPtr == mv.stackLevel)
		{
			ControlsOff();
			mv.state = 1;
		}
		break;
	}
}

// do a break turn -- pull as many g's as we
// can, despite loosing altitude and speed 
// data0 = degrees nose high/low
// data1 = max gs 
// flags = direction
void aiPilot::DoBreakTurn(aiManeuverState &mv)
{
	switch (mv.state)
	{
	case 0:
	{
		aiREAL bankAngleDeg = BREAK_TURN_ANGLE;
		if (bankAngleDeg > 88)
			bankAngleDeg = 88;
		if (mv.GetManeuverDirection() == aiManeuver::LEFT)
			bankAngleDeg *= -1.0;
		mv.data3 = bankAngleDeg * _degree;;
		flightModel->SetEngineControlPer(1.0);
		flightModel->SetWEP(1);
		PushManeuver(aiManeuver::LEVEL_ROLL,0,bankAngleDeg,5.0);
		mv.state = 1;
		break;
	}

	case 1:
		if (fabs(flightModel->GetRoll()) > Pi_4 )
		{
			pitchRateCtl.Off();
			pitchRateCtlDirect.Off();
			pitchCtl.Off();
			pitchCtlDirect.Off();
			if (mv.data1 <= 0.0)
				SETCONTROL(gCtl,posGLimit);
			else
				SETCONTROL(gCtl,mv.data1);
		}
		if (mnvrStackPtr == mv.stackLevel)
			mv.state = 2;
		break;

	case 2:
		ControlsOff();
		if (mv.data1 <= 0.0)
			SETCONTROL(gCtl,posGLimit);
		else
			SETCONTROL(gCtl,mv.data1);
		SETROLLPID(mv.data3);
		SETYAWPID(levelAttitude.yaw);
		break;
	}
}

// do a hard turn -- pull as many g's as we can
// while maintaining most of our airspeed and
// altitude. Idea is to have a sustained turn
// flags = direction
void aiPilot::DoHardTurn(aiManeuverState &mv)
{
	switch (mv.state)
	{
	case 0:
		if (fabs(flightModel->GetPitch()) > 0.4)
		{
			PushManeuver(aiManeuver::STRAIGHT_AND_LEVEL);
			mv.state = 5;
			break;
		}
		else if (flightModel->GetAirSpeedFPS() < flightModel->GetStallSpeedFPS())
		{
			PushManeuver(aiManeuver::EXTEND,0,flightModel->GetStallSpeedFPS(),10,extAngle);
			mv.state = 5;
			break;
		}
		else
			mv.state = 1;

	case 1:
   {
		ControlsOff();
		flightModel->SetEngineControlPer(1.0);
		flightModel->SetWEP(1);
		aiREAL bankAngleDeg = BREAK_TURN_ANGLE * 0.8;
		if (mv.GetManeuverDirection() == aiManeuver::LEFT)
			bankAngleDeg *= -1.0;
		mv.data3 = bankAngleDeg * _degree;
		PushManeuver(aiManeuver::LEVEL_ROLL,0,bankAngleDeg,3.0);
		mv.state = 2;
		break;
   }

	case 2:
		if (mnvrStackPtr == mv.stackLevel)
		{
			mv.state = 3;
			mv.data3 = 0.0;
			mv.data4 = posGLimit;
		}
		break;

	case 3:
	{
		ControlsOff();
		aiREAL d0,d1;
		/* set bank angle to maintain roughly constant altitude */
		d0 = flightModel->GetWeight() / (flightModel->GetLift() + eps);
		if (d0 < 0.8 && d0 > 0.0)
		{
			d1 = acos(d0);
			if (flightModel->GetPitch() >= 0.0)
				d1 *= 1.1;
			if (d1 > Pi_2 - 0.05)
				d1 = Pi_2 - 0.05 ;
			if (mv.GetManeuverDirection() == aiManeuver::LEFT)
				d1 = -d1;
		}
		else
		{
			d1 = Pi_2 - 0.05 ;
			if (mv.GetManeuverDirection() == aiManeuver::LEFT)
				d1 = -d1;
		}
		SETROLLPID(d1);
		/* 
		See if we are losing airspeed, and if so,
		lay off the g's.
		*/
		mv.data3 += timeFrame;
		if (mv.data3 > 0.1)
		{
			mv.data3 = 0.0;
			if (deltaAirSpeed < 0)
				mv.data4 -= 0.1;
			else
				mv.data4 += 0.1;
			if (mv.data4 < 1.0)
				mv.data4 = 1.0;
			if (mv.data4 > posGLimit)
				mv.data4 = posGLimit;
		}
		SETCONTROL(gCtl,mv.data4);
		/*
		if (deltaAirSpeed < 0)
			SETCONTROL(gCtl,1.0);
		else
			SETCONTROL(gCtl,posGLimit);
		*/
		SETYAWPID(levelAttitude.yaw);
		break;
	}

	case 5:
		if (mnvrStackPtr == mv.stackLevel)
			mv.state = 1;
		break;

	}

}

// climb for altitude
// data0 = altitude (feet, above sea level) to reach
// data1 = minimum
void aiPilot::DoClimb(aiManeuverState &mv)
{
	if (flightModel->GetAirSpeedFPS() <= flightModel->GetStallSpeedFPS()
		&& mv.state != 3)
	{
		ClearManeuverStackTo(mv.stackLevel);
		PushManeuver(aiManeuver::EXTEND,0,flightModel->GetStallSpeedFPS() * 1.5,10.0,
					extAngle);
		mv.state = 3;
	}

	switch (mv.state)
	{

	case 0:
		mv.state = 1;
		flightModel->SetPitchControlPer(0.0);
		mv.data3 = _degree * 10.0;
		mv.data4 = 0.0;
		break;

	case 1:
	{
		flightModel->SetEngineControlPer(1.0);
		flightModel->SetWEP(1);
		flightModel->SetYawControlPer(0.0);
		aiREAL curAlt = flightModel->GetAltitudeFPS();
		if (fabs(mv.data0 - curAlt) <= mv.data1)
		{
			mv.state = 0;
			mv.done = 1;
		}
		else
		{
			ControlsOff();
			SETROLLPID(0.0);
			aiREAL distance = aiGetCircularDistance(mv.data3,flightModel->GetPitch());
			SETPITCHPID(-distance);
			mv.data4 += timeFrame;
			if (mv.data4 > 0.1)
			{
				mv.data4 = 0.0;
				if (deltaAirSpeed < 0.0 && 
					flightModel->GetAirSpeedFPS() < flightModel->GetBestClimbSpeedFPS())
					mv.data3 -= _half_degree * 0.1;
				else if (deltaAirSpeed > 0.0)
					mv.data3 += _half_degree * 0.1;

				if (mv.data3 < _half_degree)
					mv.data3 = _half_degree;
			}
		}
	}
		break;

	case 3:
		if (mnvrStackPtr == mv.stackLevel)
			mv.state = 0;
		break;

	}	
}

// descend
// data0 = altitude in feet above sea level
// data1 = minimum
// data2 = angle
void aiPilot::DoDescend(aiManeuverState &mv)
{
	switch (mv.state)
	{
		case 0:
		{
            mv.data2 *= _degree;
			ControlsOff();
            if (fabs(flightModel->GetPitch()) > Pi_4)
            {
			   PushManeuver(aiManeuver::STRAIGHT_AND_LEVEL);
               mv.state = 1;
            }
            else
               mv.state = 2;
		}
		break;

		case 1:
			if (mnvrStackPtr == mv.stackLevel)
				mv.state = 2;
		break;

		case 2:
		{
			flightModel->SetYawControlPer(0.0);
			aiREAL curAlt = flightModel->GetAltitudeFPS();
			if (curAlt <= mv.data0 ||
				fabs(curAlt - mv.data0) < mv.data1)
			{
				mv.state = 0;
				mv.done = 1;
			}
			else
			{
				ControlsOff();
				SETROLLPID(0.0);
				aiREAL distance = aiGetCircularDistance(-mv.data2,flightModel->GetPitch());
				SETPITCHPID(-distance);
			}
		}
	}
}

// Do the immelman maneuver
// mv.data0 = max g's
void aiPilot::DoImmelman(aiManeuverState &mv)
{
	switch (mv.state)
	{
	case 0:
	{
		ControlsOff();
		aiREAL climbFPS = flightModel->GetMaxSpeedFPS() * 0.7;
		if (flightModel->GetAirSpeedFPS() < climbFPS )
			PushManeuver(aiManeuver::EXTEND,0,climbFPS ,10,extAngle);
		else
			PushManeuver(aiManeuver::STRAIGHT_AND_LEVEL);
		mv.state = 1;
		break;
	}

	case 1:
		if (mnvrStackPtr == mv.stackLevel)
			mv.state = 2;
		break;

	case 2:
	{
		ControlsOff();
		flightModel->SetYawControlPer(0.0);
		rollLock = 1;
		SETCONTROL(gCtl,mv.data0);
		mv.state = 3;
		break;
	}

	case 3:
	{
		aiREAL curRoll;
		aiREAL curPitch;
		curRoll = flightModel->GetRoll();
		curPitch = flightModel->GetPitch();
		if ((fabs(curRoll) >= Pi_2) &&
			(fabs(curPitch) <= _degree * 8.0))
			mv.state = 4;
		else
		{
			if (fabs(flightModel->GetPitchControlPer()) >= 0.95 &&
				fabs(flightModel->GetPitchRate()) <= _half_degree)
			{
				rollLock = 0;
				PushManeuver(aiManeuver::LEVEL_ROLL,0,0.0,3.0);
				mv.state = 6;
			}
		}
		break;
	}

	case 4:
		rollLock = 0;
		PushManeuver(aiManeuver::LEVEL_ROLL,0,0.0,2.0);
		mv.state = 5;
		break;

	case 5:
		if (mnvrStackPtr == mv.stackLevel)
		{
		   mv.done = 1;
			mv.state = 0;
		}
		break;

	case 6:
		if (mnvrStackPtr == mv.stackLevel)
		   mv.state = 0;
		break;

	}
}

// Do a split-s maneuver
// mv.data0 = gs
void aiPilot::DoSplitS(aiManeuverState &mv)
{
	switch (mv.state)
	{
	case 0:
		if (fabs(flightModel->GetPitch()) > Pi_4)
		{
			PushManeuver(aiManeuver::STRAIGHT_AND_LEVEL);
			mv.state = 5;
		}
		else
		{
			PushManeuver(aiManeuver::LEVEL_ROLL,0,180,1.0);
			mv.state = 1;
		}
		break;

	case 1:
		if (fabs(Pi - flightModel->GetRoll()) < _degree * 3.0)
			ClearManeuverStackTo(mv.stackLevel);
		if (mnvrStackPtr == mv.stackLevel)
			mv.state = 2;
		break;

	case 2:
		ControlsOff();
		rollLock = 1;
		flightModel->SetRollControlPer(0);
		flightModel->SetYawControlPer(0.0);
		SETROLLPID(Pi);
		SETCONTROL(gCtl,mv.data0);
		mv.state = 3;

	case 3:
	{
		aiREAL curRoll;
		aiREAL curPitch;
		curRoll = flightModel->GetRoll();
		curPitch = flightModel->GetPitch();
		if ((curRoll < 1.57 && curRoll > -1.57)
			&& fabs(curPitch) < 0.6)
		{
			rollLock = 0;
			/* check for quick abort */
			if (mv.flags & AIMNVR_FLAG0BIT)
			{
				mv.done = 1;
				mv.state = 0;
			}
			else
			{
				mv.state = 4;
				PushManeuver(aiManeuver::LEVEL_ROLL,0,0.0,1.0);
			}
			break;
		}
		ControlsOff();
		rollLock = 1;
		flightModel->SetRollControlPer(0);
		flightModel->SetYawControlPer(0.0);
		SETCONTROL(gCtl,mv.data0);
	}
	break;
		
	case 4:
		if (mnvrStackPtr == mv.stackLevel)
		{
			mv.done = 1;
			mv.state = 0;
		}
		break;

	case 5:
		if (fabs(flightModel->GetPitch()) < Pi_4 * 0.5)
			ClearManeuverStackTo(mv.stackLevel);
		if (mnvrStackPtr == mv.stackLevel)
			mv.state = 0;
		break; 
	}
}	

// Pullup to avoid ground collision
// data0 = minimum height AGL to achieve
// data1 = pullup angle in degrees
void aiPilot::DoPullUp(aiManeuverState &mv)
{
	switch (mv.state)
	{
	case 0:
		{
			flightModel->SetEngineControlPer(1.0);
			flightModel->SetWEP(1);
			mv.data1 *= _degree;
			aiREAL curAlt = flightModel->GetHeightAGL();
			if (curAlt >= mv.data0)
			{
				mv.state = 0;
				mv.done = 1;
			}
			ControlsOff();
			flightModel->SetYawControlPer(0.0);
			SETROLLPID(0.0);
			aiREAL distance = aiGetCircularDistance(16.0 * _degree,flightModel->GetPitch());
			SETPITCHPID(-distance);
			/* cheat on speed so we don't look foolish! */
			aiREAL cheatSpeed = flightModel->GetStallSpeedFPS() * 2.0;
			if (cheatSpeed > flightModel->GetMaxSpeedFPS())
				cheatSpeed = flightModel->GetMaxSpeedFPS();
			if (flightModel->GetAirSpeedFPS() < cheatSpeed)
				SETCONTROL(velCtl,cheatSpeed);
		}
		break;


	case 1:
		if (mnvrStackPtr == mv.stackLevel)
			mv.state = 0;
		break;
	}	

}

void aiPilot::DoAlign(aiManeuverState &mv)
{
	mv.done = 1;
}

void aiPilot::DoWingOver(aiManeuverState &mv)
{
	switch (mv.state)
	{
	case 0:
	{
		ControlsOff();
		aiREAL climbFPS = flightModel->GetMaxSpeedFPS();
		PushManeuver(aiManeuver::EXTEND,0,climbFPS,10,extAngle);
		mv.data3 = aiIncrementAngle(flightModel->GetYaw(),Pi);
		mv.state = 1;
		break;
	}

	case 1:
		if (mnvrStackPtr == mv.stackLevel)
			mv.state = 2;
		break;

	case 2:
		ControlsOff();
		flightModel->SetYawControlPer(0.0);
		rollLock = 1;
		flightModel->SetPitchControlPer(0.0);
		SETCONTROL(gCtl,posGLimit);
		mv.state = 3;
		break;

	case 3:
	{
		aiREAL curPitch;
		ControlsOff();
		rollLock = 1;
		SETCONTROL(gCtl,posGLimit);
		curPitch = flightModel->GetPitch();
		if (fabs(Pi_2 - curPitch) < _degree * 2.0)
			mv.state = 4;
		break;
	}

	case 4:
	{
		aiREAL curPitch;
		curPitch = flightModel->GetPitch();
		if (fabs(curPitch) < _degree * 2.0)
		{
			rollLock = 0;
			pitchLock = 0;
			mv.data4 = flightModel->GetRoll();
			mv.state = 5;
		}
		else
		{   
			ControlsOff();
			rollLock = 1;
			pitchLock = 1;            
			SETCONTROL(yawRateCtl,-Pi_2);
		}
		break;
	}


	case 5:
		if (fabs(flightModel->GetYaw() - mv.data3) <= _degree * 2.0)
		{
			PushManeuver(aiManeuver::STRAIGHT_AND_LEVEL);
			mv.state = 6;
		}
		else
		{
			ControlsOff();
			SETCONTROL(yawRateCtl,0.0);
			SETCONTROL(rollCtl,mv.data4);
			SETCONTROL(gCtl,posGLimit);
		}
		break;

	case 6:
		if (mnvrStackPtr == mv.stackLevel)
		{
			 mv.done = 1;
			 mv.state = 0;
		}
		break;

	}
}

void aiPilot::DoClimbingTurn(aiManeuverState &mv)
{
	switch (mv.state)
	{
	case 0:
		if (fabs(flightModel->GetPitch()) > Pi_4)
		{
			PushManeuver(aiManeuver::STRAIGHT_AND_LEVEL);
			mv.state = 1;
		}
		else
			mv.state = 2;
		break;

	case 1:
		if (mnvrStackPtr == mv.stackLevel)
			mv.state = 2;
		break;


	case 2:
		{
			/* set bank angle for 2g */
			aiREAL bankAngle = _degree * 45.0;
			if (mv.GetManeuverDirection() == aiManeuver::LEFT)
				bankAngle *= -1.0;
			ControlsOff();
			flightModel->SetEngineControlPer(1.0);
			flightModel->SetWEP(1);
			SETROLLPID(bankAngle);
			if (fabs(bankAngle - flightModel->GetRoll()) < 0.3)
				SETCONTROL(gCtl,2.0);
			else
				SETPITCHPID(levelAttitude.pitch);
			SETYAWPID(levelAttitude.yaw);
			if (fabs(flightModel->GetPitch() > Pi_4))
			{
				PushManeuver(aiManeuver::STRAIGHT_AND_LEVEL);
				mv.state = 1;
			}
			if (flightModel->GetAirSpeedFPS() <= flightModel->GetStallSpeedFPS())
			{
				PushManeuver(aiManeuver::EXTEND,0,flightModel->GetCornerSpeedFPS(),10.0,
														extendAngle);
				mv.state = 1;

			}
		}
		break;		
	}
}

void aiPilot::DoDescendingTurn(aiManeuverState &)
{

}

void aiPilot::DoSnapRoll(aiManeuverState &mv)
{
	switch (mv.state)
	{
	case 0:
		mv.data0 *= _degree;
		mv.data1 *= _degree;
		mv.data2 = aiGetCircularDistance(mv.data0,flightModel->GetRoll());
		mv.data3 = flightModel->GetRoll();
		mv.state = 1;

	case 1:
		ControlsOff();
		if (mv.data2 > 0)
		{
			SETCONTROL(rollRateCtl,_2Pi);
			SETCONTROL(yawRateCtl,-_2Pi);
			SETCONTROL(pitchRateCtl,levelAttitude.pitch);
		}
		else
		{
			SETCONTROL(rollRateCtl,_2Pi);
			SETCONTROL(yawRateCtl,-_2Pi);
			SETCONTROL(pitchRateCtl,levelAttitude.pitch);
		}
		if (fabs(flightModel->GetRoll() - mv.data0) < mv.data1)
			mv.state = 2;
		break;

	case 2:
		ControlsOff();
		SETPITCHPID(levelAttitude.pitch);
		SETYAWPID(levelAttitude.yaw);
		SETROLLPID(mv.data0);
		if (fabs(flightModel->GetRoll() - mv.data0) < mv.data1)
			mv.done = 1;
		break;
 
	}
}

// roll, keeping nose level with horizion
void aiPilot::DoAileronRoll(aiManeuverState &mv)
{
	ControlsOff();
	if (mv.GetManeuverDirection() == aiManeuver::LEFT)
		SETCONTROL(rollRateCtl,Pi);
	else
		SETCONTROL(rollRateCtl,-Pi);
	SETPITCHPID(levelAttitude.pitch);
	SETYAWPID(levelAttitude.yaw);
}

// zoom -- climb to given altitude, losing airspeed if necessary
// mv.data0 = altitude
// mv.data1 = angle to climb at in degrees
void aiPilot::DoZoom(aiManeuverState &mv)
{
	switch (mv.state)
	{

	case 0:
		mv.data1 *= _degree;
		mv.state = 1;

	case 1:
		{
			flightModel->SetEngineControlPer(1.0);
			flightModel->SetWEP(1);
			ControlsOff();
			flightModel->SetYawControlPer(0.0);
			SETROLLPID(0.0);
			aiREAL distance = aiGetCircularDistance(mv.data1,flightModel->GetPitch());
			SETPITCHPID(-distance);
			aiREAL curAlt = flightModel->GetAltitudeFPS();
			if (curAlt >= mv.data0)
			{
				mv.done = 1;
				break;
			}
		}
		break;
	}
}

void aiPilot::DoBarrelRoll(aiManeuverState &mv)
{
	aiREAL rollRate = Pi; 
	if (mv.GetManeuverDirection() == aiManeuver::LEFT)
		rollRate *= -1.0;
	ControlsOff();
	SETCONTROL(rollRateCtl,rollRate);
	SETCONTROL(gCtl,mv.data0);
	SETYAWPID(levelAttitude.yaw);
}

void aiPilot::DoSpiralDive(aiManeuverState &)
{

}
