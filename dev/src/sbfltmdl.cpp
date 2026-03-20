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
 * File   : sbfltmdl.h                           *
 * Date   : April, 1998                          *
 * Author : Dan Hammer                           *
 * Wrap our Sabre flight model inside an         *
 * aiFlightModel so AI can access it.             *
 *************************************************/
#include <stdio.h>
#include "simfile.h"
#include "sbfltmdl.h"
#include "ai2sbr.h"

extern float time_frame;

#define STATE       flight->state
#define CONTROLS    flight->controls
#define FORCES		  flight->forces
#define SPECS		  flight->specs
#define FLIGHT_PORT STATE.flight_port
#define LOOK_FROM   FLIGHT_PORT.look_from
#define LOOK_AT     FLIGHT_PORT.look_at
#define FLIGHT_ROLL FLIGHT_PORT.roll

sbrFlightModel::sbrFlightModel(Flight *flight)
{
aiPoint  position;
aiAttitude attitude;

	this->flight = flight;
	R_3DPoint2aiPoint(LOOK_FROM,position);
	Port2aiAttitude(FLIGHT_PORT,attitude);
	aiSlewer::SetPositionAndAttitude(position,attitude);
	turnRate = 0.0;
}

void sbrFlightModel::Update(double /* ignored */)
{
aiPoint  position;
aiAttitude attitude;

	flight->l_time = time_frame;
	if (flight->l_time <= 0.0)
		flight->l_time = 0.1;
	flight->applyForces(flight->l_time);
	flight->applyRotations(flight->l_time);
	flight->calcState(flight->l_time);
	DoControlCallback();
	flight->calcForces(flight->l_time);
	flight->calcRotations(flight->l_time);

	/* call Flight::Update() */
	/* convert position and attitude */
	R_3DPoint2aiPoint(LOOK_FROM,position);
	Port2aiAttitude(FLIGHT_PORT,attitude);
	
	turnRate = (attitude.yaw - att.yaw) / time_frame;
	aiSlewer::SetPositionAndAttitude(position,attitude);
}

void sbrFlightModel::Destroy()
{
}

void sbrFlightModel::SetPositionAndAttitude(const aiPoint &position, const aiAttitude &attitude)
{
aiPoint	 lookAt;
R_3DPoint look_at;
R_3DPoint look_from;
Vector	 v;

	aiSlewer::SetPositionAndAttitude(position,attitude);
	aiPoint2R_3DPoint(position,look_from);
	lookAt = position + dirNormal;
	aiPoint2R_3DPoint(lookAt,look_at);
	FLIGHT_PORT.set_view(look_from,look_at);
	FLIGHT_PORT.set_roll(CvToSabreAngle(attitude.roll));
	aiVector2Vector(dirNormal,v);
	STATE.velocity.direction = v;
}

void sbrFlightModel::Rotate(const aiAttitude &rotateMatrix)
{
aiPoint  position;
aiAttitude attitude;

	FLIGHT_PORT.rotate(CvToSabreAngle(rotateMatrix.pitch),
								CvToSabreAngle(rotateMatrix.roll),
								CvToSabreAngle(rotateMatrix.yaw));
	R_3DPoint2aiPoint(LOOK_FROM,position);
	Port2aiAttitude(FLIGHT_PORT,attitude);
	aiSlewer::SetPositionAndAttitude(position,attitude);
}

void sbrFlightModel::SetPosition(const aiPoint &position)
{
R_3DPoint look_from;

	aiPoint2R_3DPoint(position,look_from);
	FLIGHT_PORT.set_look_from(look_from);
	aiSlewer::SetPosition(pos);
}

void sbrFlightModel::SetAttitude(const aiAttitude &attitude)
{
	SetPositionAndAttitude(pos,attitude);		
}

aiREAL sbrFlightModel::GetPitchRate()
{
	return (aiREAL) -STATE.pitch_rate;
}

void sbrFlightModel::SetPitchRate(aiREAL pitchRate)
{
	STATE.pitch_rate = (REAL_TYPE) pitchRate;
}

aiREAL sbrFlightModel::GetPitch()
{
	return att.pitch;
}

void sbrFlightModel::SetPitch(aiREAL pitch)
{
aiAttitude attitude = att;

	attitude.pitch = pitch;
	SetAttitude(attitude);		
}

aiREAL sbrFlightModel::GetRollRate()
{
	return (aiREAL) STATE.roll_rate;
}

void sbrFlightModel::SetRollRate(aiREAL rollRate)
{
	STATE.roll_rate = (REAL_TYPE) rollRate;		
}

aiREAL sbrFlightModel::GetRoll()
{
	return att.roll;
}

void sbrFlightModel::SetRoll(aiREAL roll)
{
aiAttitude attitude = att;

	attitude.roll = roll;
	FLIGHT_PORT.set_roll(roll);
	aiSlewer::SetPositionAndAttitude(pos,attitude);
//	SetAttitude(attitude);
}

aiREAL sbrFlightModel::GetYawRate()
{
	return (aiREAL) -STATE.yaw_rate;
}

void sbrFlightModel::SetYawRate(aiREAL yawRate)
{
	STATE.yaw_rate = (REAL_TYPE) yawRate;
}

aiREAL sbrFlightModel::GetYaw()
{
	return att.yaw;
}

void sbrFlightModel::SetYaw(aiREAL yaw)
{
aiAttitude attitude = att;

	attitude.yaw = yaw;
	SetAttitude(attitude);
}

aiREAL sbrFlightModel::GetAOA()
{
	return (aiREAL) STATE.angle_of_attack;
}

aiREAL sbrFlightModel::GetAOARate()
{
	/*
	 * FIXME!!
	 */
	return 0.0;
}

void sbrFlightModel::SetAOA(aiREAL AOA)
{
	STATE.angle_of_attack = (REAL_TYPE) AOA;
}

aiREAL sbrFlightModel::GetLift()
{
	return (aiREAL) FORCES.lift.magnitude;
}

void sbrFlightModel::SetLift(aiREAL lift)
{
	FORCES.lift.magnitude = (REAL_TYPE) lift;
}

aiREAL sbrFlightModel::GetAirSpeedFPS()
{
	return (aiREAL) STATE.z_vel;
}

aiREAL sbrFlightModel::GetAirSpeedMPH()
{
	return sFPS2MPH((aiREAL) STATE.z_vel);
}

void sbrFlightModel::GetVelocity(aiVector &vel)
{
	Vector port_vel = STATE.vel_pv;
	Vector2aiVector(port_vel,vel);
}

void sbrFlightModel::GetWorldVelocity(aiVector &worldVel)
{
	Vector worldVelD = to_vector(STATE.velocity);
	Vector worldV = worldVelD;
	Vector2aiVector(worldV,worldVel);
}

void sbrFlightModel::SetVelocity(const aiVector &vel)
{
	/*
	 * FIXME!!
	 */
}

aiREAL sbrFlightModel::GetPitchControlPer()
{
	return (aiREAL) (CONTROLS.elevators / CONTROLS.elevator_max);
}

void sbrFlightModel::SetPitchControlPer(aiREAL per)
{
	if (per > 1.0)
		per = 1.0;
	if (per < -1.0)
		per = -1.0;
	CONTROLS.elevators = ((float)per) * CONTROLS.elevator_max;
}

void sbrFlightModel::IncPitchControlPer(aiREAL inc)
{
	SetPitchControlPer(GetPitchControlPer() + inc);
}

aiREAL sbrFlightModel::GetRollControlPer() 
{
	return (aiREAL) (CONTROLS.ailerons / CONTROLS.aileron_max);
}

void sbrFlightModel::SetRollControlPer(aiREAL per) 
{
	if (per > 1.0)
		per = 1.0;
	if (per < -1.0)
		per = -1.0;
	CONTROLS.ailerons = ((float)per) * CONTROLS.aileron_max;
}

void sbrFlightModel::IncRollControlPer(aiREAL inc)
{
	SetRollControlPer(GetRollControlPer() - inc);
}

aiREAL sbrFlightModel::GetYawControlPer()
{
	return (aiREAL) (CONTROLS.rudder / CONTROLS.rudder_max);
}

void sbrFlightModel::SetYawControlPer(aiREAL per)
{
	if (per > 1.0)
		per = 1.0;
	if (per < -1.0)
		per = -1.0;
	CONTROLS.rudder = ((float)per) * CONTROLS.rudder_max;
}

void sbrFlightModel::IncYawControlPer(aiREAL inc)
{
	SetYawControlPer(GetYawControlPer() - inc);
}

aiREAL sbrFlightModel::GetEngineControlPer()
{
	return CONTROLS.throttle * 0.01;
}

void sbrFlightModel::SetEngineControlPer(aiREAL per)
{
	if (per > 1.0)
		per = 1.0;
	if (per < 0.0)
		per = 0.0;
	CONTROLS.throttle = ((float)per) * 100.0;
}

void sbrFlightModel::IncEngineControlPer(aiREAL inc)
{
	SetEngineControlPer(GetEngineControlPer() + inc);
}

aiREAL sbrFlightModel::GetLoad()
{
	return (aiREAL) STATE.load;
}

aiREAL sbrFlightModel::GetLoadRate()
{
	return (aiREAL) STATE.d_load;
}

aiREAL sbrFlightModel::GetAltitudeFPS()
{
	return (aiREAL) (FLIGHT_PORT.look_from.z / world_scale);	
}

aiREAL sbrFlightModel::GetBestClimbSpeedFPS()
{
	return (aiREAL) SPECS->corner_speed;
}

aiREAL sbrFlightModel::GetStallSpeedFPS()
{
	return (aiREAL) SPECS->lspeed;
}

aiREAL sbrFlightModel::GetMaxSpeedFPS()
{
	return (aiREAL) SPECS->max_speed;
}

aiREAL sbrFlightModel::GetCornerSpeedFPS()
{
	return (aiREAL) SPECS->corner_speed;
}

aiREAL sbrFlightModel::GetThrust()
{
	return (aiREAL) FORCES.thrust.magnitude;
}

aiREAL sbrFlightModel::GetDrag()
{
	return (aiREAL) FORCES.drag.magnitude;
}

aiREAL sbrFlightModel::GetWeight()
{
	return (aiREAL) STATE.weight;
}

void sbrFlightModel::SetWEP(int wep)
{
	/*
	 * FIXME!!
	 */
}

int sbrFlightModel::GetWEP()
{
	return 0;
}

void sbrFlightModel::GetDirectionNormal(aiVector &directionNormal)
{
	directionNormal = dirNormal;
}

aiREAL sbrFlightModel::GetTurnRate()
{
	return turnRate;
}

aiREAL sbrFlightModel::GetHeightAGL()
{
	return (aiREAL) (STATE.agl / world_scale);
}

int sbrFlightModel::GetEngineCount()
{
	/*
	 * FIXME!!
	 */
	return 1;
}

extern REAL_TYPE getGroundLevel(R_3DPoint &p);

aiREAL sbrFlightModel::GetGroundHeight(const aiPoint &s0)
{
R_3DPoint p0;

	aiPoint2R_3DPoint(s0,p0);
	return (aiREAL) (getGroundLevel(p0) / world_scale);
}

aiREAL sbrFlightModel::GetPitchAccel()
{
	return (aiREAL) STATE.d_pitch_rate;
}

aiREAL sbrFlightModel::GetYawAccel()
{
	return (aiREAL) STATE.d_yaw_rate;
}

aiREAL sbrFlightModel::GetRollAccel()
{
	return (aiREAL) STATE.d_roll_rate;
}

void sbrFlightModel::SetFlaps(aiREAL per)
{
	if (per > 1.0)
		per = 1.0;
	if (per < 0.0)
		per = 0.0;

	CONTROLS.flaps = (REAL_TYPE) (per * CONTROLS.flaps_max);
}

void sbrFlightModel::SetSpeedBreaks(aiREAL per)
{
	if (per > 0.5)
		CONTROLS.speed_brakes = 1;
	else
		CONTROLS.speed_brakes = 0;
}

void sbrFlightModel::ExtendLandingGear()
{
	CONTROLS.landing_gear = 1;
}

void sbrFlightModel::RetractLandingGear()
{
	CONTROLS.landing_gear = 0;
}

int sbrFlightModel::GetGearPoints(aiPoint *)
{
	/* 
	 * FIXME!!
	 */
	return 0;
}

aiREAL sbrFlightModel::GetRestPitch()
{
	/*
	 * FIXME!!
	 */
	return 0;
}

const char *sbrFlightModel::GetModel()
{
	return SPECS->model;
}

void sbrFlightModel::SetWheelBrakes(int engaged)
{
	CONTROLS.wheel_brakes = engaged;
}
