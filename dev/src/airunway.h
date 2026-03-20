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
 * File   : airunway.h                            *
 * Date   : April, 1998                          *
 * Author : Dan Hammer                           *
 *************************************************/
#ifndef __airunway_h
#define __airunway_h

typedef struct airunway_info_struct
{
	double	x;
	double	y;
	double	z;
	double	heading;
	double	length;
	double	width;
} airunway_info;

#ifdef __cplusplus

#include "aislewer.h"

class aiRunway : public aiSlewer
{
public:

	aiRunway()
	{}

	aiRunway(const airunway_info &);

	int GetRunwayNumber()
	{
		return runwayNumber;
	}
	/*
	 * position is touchdown point for landing
	 */
	aiPoint &GetPosition()
	{
		return pos;
	}
	aiAttitude &GetAttitude()
	{
		return att;
	}
	aiREAL GetLength()
	{
		return length;
	}
	aiREAL GetWidth()
	{
		return width;
	}
	aiREAL GetFinalApproachHeading()
	{
		return finalApproachHeading;
	}
	aiREAL GetTakeoffHeading()
	{
		return takeoffHeading;
	}
	/* 
	 *  approach heading & takeoff heading
	 *  are relative to aircraft
	 */
	void SelectFinalApproachHeading()
	{
		att.yaw = finalApproachHeading;
		BuildMatrixes();
	}
	void SelectTakeoffHeading()
	{
		att.yaw = takeoffHeading;
		BuildMatrixes();
	}


	void SetPositionAndAttitude(const aiPoint &position, 
										const aiAttitude &attitude)
	{
		aiAttitude tempAtt = attitude;
		tempAtt.pitch = tempAtt.roll = 0.0;
		finalApproachHeading = attitude.yaw;
		takeoffHeading = aiIncrementAngle(attitude.yaw,Pi);
		aiSlewer::SetPositionAndAttitude(position,tempAtt);
	}

	void GetLandingTurnPoint(aiPoint &turnPoint, aiREAL approachDistance, aiREAL turnRadius,
									int clockWise);
	void GetTakeoffPositionAndAttitude(aiPoint &, aiAttitude &);

protected:
	int			runwayNumber;
	aiREAL			length;
	aiREAL			width;
	aiREAL			finalApproachHeading;
	aiREAL			takeoffHeading;
};

#endif

#endif
