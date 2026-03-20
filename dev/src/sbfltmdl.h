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
#ifndef __sbfltmdl_h
#define __sbfltmdl_h

#ifdef __cplusplus
#include "simerr.h"
#include "simfile.h"
#include "vmath.h"
#include "port_3d.h"
#include "flight.h"
#include "aifltmdl.h"

class sbrFlightModel : public aiFlightModel
{
protected:
	Flight *flight;
	aiREAL   turnRate;
public:
	sbrFlightModel(Flight *flight);
	/*
	 * virtuals from aiSlewer
	 */
	void SetPositionAndAttitude(const aiPoint &position, const aiAttitude &attitude);
	void Rotate(const aiAttitude &rotateMatrix);
	void SetPosition(const aiPoint &position);
	void SetAttitude(const aiAttitude &attitude);
	/*
	 *  virtuals from aiFlightModel
	 */
	void Update(double frame_time);
	void Destroy();
	aiREAL GetPitchRate();
	void SetPitchRate(aiREAL pitchRate);
	aiREAL GetPitch();
	void SetPitch(aiREAL pitch);
	aiREAL GetRollRate();
	void SetRollRate(aiREAL rollRate);
	aiREAL GetRoll();
	void SetRoll(aiREAL roll);
	aiREAL GetYawRate();
	void SetYawRate(aiREAL yawRate);
	aiREAL GetYaw();
	void SetYaw(aiREAL yaw);
	aiREAL GetAOA();
	aiREAL GetAOARate();
	void SetAOA(aiREAL AOA);
	aiREAL GetLift();
	void SetLift(aiREAL lift);
	aiREAL GetAirSpeedFPS();
	aiREAL GetAirSpeedMPH();
	void GetVelocity(aiVector &vel);
	void SetVelocity(const aiVector &vel);
	aiREAL GetPitchControlPer();
	void SetPitchControlPer(aiREAL per);
	void IncPitchControlPer(aiREAL inc) ;
	aiREAL GetRollControlPer() ;
	void SetRollControlPer(aiREAL per) ;
	void IncRollControlPer(aiREAL inc);
	aiREAL GetYawControlPer();
	void SetYawControlPer(aiREAL per);
	void IncYawControlPer(aiREAL inc);
	aiREAL GetEngineControlPer();
	void SetEngineControlPer(aiREAL per);
	void IncEngineControlPer(aiREAL per);
	aiREAL GetLoad();
	aiREAL GetLoadRate();
	aiREAL GetAltitudeFPS();
	aiREAL GetBestClimbSpeedFPS();
	aiREAL GetStallSpeedFPS();
	aiREAL GetMaxSpeedFPS();
	aiREAL GetThrust();
	aiREAL GetDrag();
	aiREAL GetWeight();
	void GetWorldVelocity(aiVector &worldVel);
	void SetWEP(int wep);
	int GetWEP();
	void GetDirectionNormal(aiVector &directionNormal);
	aiREAL GetTurnRate();
	aiREAL GetHeightAGL();
	int GetEngineCount();
	aiREAL GetGroundHeight(const aiPoint &);
	aiREAL GetPitchAccel();
	aiREAL GetYawAccel();
	aiREAL GetRollAccel();
	void SetFlaps(aiREAL per);
	void SetSpeedBreaks(aiREAL per);
	void ExtendLandingGear();
	void RetractLandingGear();
	int GetGearPoints(aiPoint *);
	aiREAL GetRestPitch();
	const char *GetModel();
	aiREAL GetCornerSpeedFPS();
	void SetWheelBrakes(int engaged);

	Flight *GetFlight(void)
	{
		return flight;
	}
	Port_3D *GetPort(void)
	{
		return & GetFlight()->state.flight_port;
	}
	int ControlsProportional()
	{
		return 0;
	}
};

#endif /* __cplusplus */


#endif