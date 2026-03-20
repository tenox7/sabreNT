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
 * File   : aifltmdl.h                            *
 * Date   : April, 1998                          *
 * Author : Dan Hammer                           *
 * An "abstract" flight model, for use with AI   *
 * or a human player. Defines an interface which *
 * allows the flight model to be controlled both *
 * directly (by setting roll rates, velocity,    *
 * etc), or indirectly (by setting control       *
 * inputs).                                      *
 *************************************************/
#ifndef __aifltmdl_h
#define __aifltmdl_h
#ifdef __cplusplus
#include <stdio.h>

#include "aislewer.h"

/*
 *  We will use a callback method where the flight model
 *  will call a provided function when it is at various
 *  points in it's update routine. These bit-flags are set
 *  in the call to SetUpdateFunction
 */
#define AIFMCB_VFORCES_UPDATE      0x01     // call back on calc of vector forces
#define AIFMCB_RFORCES_UPDATE      0x02     // call back on calc of rotational forces
#define AIFMCB_POSITION_UPDATE     0x04     // call back on calc of position
#define AIFMCB_ORIENTATION_UPDATE  0x08     // call back on calc of orientation
#define AIFMCB_CONTROL_UPDATE      0x10     // call back to set flight controls

#define AIFMCB_FORCES    AIFMCB_VFORCES_UPDATE | AIFMCB_RFORCES_UPDATE
#define AIFMCB_POSITIONS  AIFMCB_POSITION_UPDATE | AIFMCB_ORIENTATION_UPDATE
#define AIFMCB_ALL AIFMCB_FORCES | AIFMCB_POSITIONS | AIFMCB_CONTROL_UPDATE

class aiFlightModel : public aiSlewer
{
protected:
  /* 
     This callback function allows various aspects of the flight
     model to be overridden to adjust, tweak or completely invalidate
     the calculations.
     */
  void (* updateCallback)(int sw, int iter, aiFlightModel *fm, void *);
  /* flags indicating which updates the above function applies to */
  unsigned int callbackFlags;
  /* additional data the callbackee wishes to have passed */
  void *callbackData;
  /* current calculation iteration value */
  int iter;
public:
  /*
    values of sw in above callback indicating what's about to be updated
    VFORCES_UPDATE  = model is about to calculate vector forces from state info
    RFORCES_UPDATE  = model is about to calculate rotational forces from state info
    POSITION_UPDATE = model is about to position from vector velocities
    ORIENTATION_UPDATE = model is about to calculate orientation from rotational velocities
    CONTROL_UPDATE  = set flight control values for new iteration
    */
  enum { VFORCES_UPDATE, RFORCES_UPDATE, POSITION_UPDATE, ORIENTATION_UPDATE, CONTROL_UPDATE };
protected:
  void DoVForcesCallback();
  void DoRForcesCallback();
  void DoPositionCallback();
  void DoOrientationCallback();
  void DoControlCallback();
public:

  aiFlightModel()
    {
      updateCallback = NULL;
      callbackFlags = 0;
      callbackData = NULL;
    }

  virtual ~aiFlightModel()
    {}


  void SetUpdateCallback(unsigned int callbackFlags, void (* updateCallback)(int, int, aiFlightModel *, void *),
									void *callbackData)
    {
      this->callbackFlags = callbackFlags;
      this->updateCallback = updateCallback;
      this->callbackData = callbackData;
    }

	/* Pure virtual functions */
	virtual void Update(double frame_time) = 0;
	virtual void Destroy() = 0;
	virtual aiREAL GetPitchRate() = 0;
	virtual void SetPitchRate(aiREAL pitchRate) = 0;
	virtual aiREAL GetPitch() = 0;
	virtual void SetPitch(aiREAL pitch) = 0;
	virtual aiREAL GetRollRate() = 0;
	virtual void SetRollRate(aiREAL rollRate) = 0;
	virtual aiREAL GetRoll() = 0;
	virtual void SetRoll(aiREAL roll) = 0;
	virtual aiREAL GetYawRate() = 0;
	virtual void SetYawRate(aiREAL yawRate) = 0;
	virtual aiREAL GetYaw() = 0;
	virtual void SetYaw(aiREAL yaw) = 0;
	virtual aiREAL GetAOA() = 0;
	virtual aiREAL GetAOARate() = 0;
	virtual void SetAOA(aiREAL AOA) = 0;
	virtual aiREAL GetLift() = 0;
	virtual void SetLift(aiREAL lift) = 0;
	virtual aiREAL GetAirSpeedFPS() = 0;
	virtual aiREAL GetAirSpeedMPH() = 0;
	virtual void GetVelocity(aiVector &vel) = 0;
	virtual void SetVelocity(const aiVector &vel) = 0;
	virtual aiREAL GetPitchControlPer() = 0;
	virtual void SetPitchControlPer(aiREAL per) = 0;
	virtual void IncPitchControlPer(aiREAL inc)  = 0;
	virtual aiREAL GetRollControlPer()  = 0;
	virtual void SetRollControlPer(aiREAL per)  = 0;
	virtual void IncRollControlPer(aiREAL inc) = 0;
	virtual aiREAL GetYawControlPer() = 0;
	virtual void SetYawControlPer(aiREAL per) = 0;
	virtual void IncYawControlPer(aiREAL inc) = 0;
	virtual aiREAL GetEngineControlPer() = 0;
	virtual void SetEngineControlPer(aiREAL per) = 0;
	virtual void IncEngineControlPer(aiREAL per) = 0;
	virtual aiREAL GetLoad() = 0;
	virtual aiREAL GetLoadRate() = 0;
	virtual aiREAL GetAltitudeFPS() = 0;
	virtual aiREAL GetBestClimbSpeedFPS() = 0;
	virtual aiREAL GetStallSpeedFPS() = 0;
	virtual aiREAL GetMaxSpeedFPS() = 0;
	virtual aiREAL GetThrust() = 0;
	virtual aiREAL GetDrag() = 0;
	virtual aiREAL GetWeight() = 0;
	virtual void GetWorldVelocity(aiVector &worldVel) = 0;
	virtual void SetWEP(int wep) = 0;
	virtual int GetWEP() = 0;
	virtual void GetDirectionNormal(aiVector &directionNormal) = 0;
	virtual aiREAL GetTurnRate() = 0;
	virtual aiREAL GetHeightAGL() = 0;
	virtual int GetEngineCount() = 0;
	virtual aiREAL GetGroundHeight(const aiPoint &) = 0;
	virtual aiREAL GetPitchAccel() = 0;
	virtual aiREAL GetYawAccel() = 0;
	virtual aiREAL GetRollAccel() = 0;
	virtual void SetFlaps(aiREAL per) = 0;
	virtual void SetSpeedBreaks(aiREAL per) = 0;
	virtual void ExtendLandingGear() = 0;
	virtual void RetractLandingGear() = 0;
	virtual int GetGearPoints(aiPoint *) = 0;
	virtual aiREAL GetRestPitch() = 0;
	virtual const char *GetModel() = 0;
	virtual int ControlsProportional()
	{
		return 1;
	}
	virtual aiREAL GetCornerSpeedFPS() = 0;
	virtual void SetWheelBrakes(int) = 0;
};
#endif
#endif
