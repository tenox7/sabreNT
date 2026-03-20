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
 * File   : aiwaypnt.h                            *
 * Date   : April, 1998                          *
 * Author : Dan Hammer                           *
 *************************************************/
#ifndef __aiwaypnt_h
#define __aiwaypnt_h

typedef struct tag_aiwaypoint_info
{
	float		to_x;
	float		to_y;
	float		to_z;
	float		speed;
	float		altitude;
	int		task;
	int		i_task_modifier[3];
	float		d_task_modifier[3];
	char		c_task_modifier[32];
	int		formation_type;
	double	time;
} aiwaypoint_info;

enum	{	
			swpNAVIGATE, swpCAP, swpINTERCEPT, swpENGAGE_PLAYER, 
			swpENGAGE_FLITE, swpSTRAFE_ATTACK, swpROCKET_ATTACK, 
			swpLEVEL_BOMB, swpDIVE_BOMB, swpTORPEDO_BOMB,
			swpAIRSHOW, swpTARGETS, swpTAKEOFF, swpLAND, swpMANEUVER  
		};

enum	{
			swpTARGET_LEVEL0, swpTARGET_LEVEL1, swpTARGET_LEVEL2 
		};

#ifdef __cplusplus
#include <string.h>

#include "aiobject.h"
#include "aimath.h"

inline void aiwaypoint_info_copy(aiwaypoint_info *wp0,
							   aiwaypoint_info *wp1)
{
	wp0->to_x = wp1->to_x;
	wp0->to_y = wp1->to_y;
	wp0->to_z = wp1->to_z;
	wp0->speed = wp1->speed;
	wp0->altitude = wp1->altitude;
	wp0->task = wp1->task;
	wp0->time = wp1->time;
	wp0->formation_type = wp1->formation_type;
	wp0->i_task_modifier[0] = wp1->i_task_modifier[0];
	wp0->i_task_modifier[1] = wp1->i_task_modifier[1];
	wp0->i_task_modifier[2] = wp1->i_task_modifier[2];
	wp0->d_task_modifier[0] = wp1->d_task_modifier[0];
	wp0->d_task_modifier[1] = wp1->d_task_modifier[1];
	wp0->d_task_modifier[2] = wp1->d_task_modifier[2];
	memcpy(wp0->c_task_modifier,wp1->c_task_modifier,
		sizeof(wp0->c_task_modifier));
}

/* Waypoint definition -- used by flite leader */
class aiWaypoint : public aiObject
{
public:
	aiPoint		to;
	aiREAL			speed;
	aiREAL			altitude;
	int			task;
	int         iTaskModifier[3];
	aiREAL			dTaskModifier[3];
   char			cTaskModifier[32];
	int			idx;
	int			formationType;
	double		time;
	aiWaypoint   *next;

	aiWaypoint()
	{
		next = NULL;
		idx = 0;
		formationType = 0;
	}

	void Copy(const aiWaypoint &wp)
	{
		to = wp.to;
		speed = wp.speed;
		altitude = wp.altitude;
		task = wp.task;
		time = wp.time;
		iTaskModifier[0] = wp.iTaskModifier[0];
		iTaskModifier[1] = wp.iTaskModifier[1];
		iTaskModifier[2] = wp.iTaskModifier[2];
		dTaskModifier[0] = wp.dTaskModifier[0];
		dTaskModifier[1] = wp.dTaskModifier[1];
		dTaskModifier[2] = wp.dTaskModifier[2];
		memcpy(cTaskModifier,wp.cTaskModifier,sizeof(cTaskModifier));
		next = wp.next;
		idx = wp.idx;
		formationType = wp.formationType;
	}

	aiWaypoint(const aiWaypoint &wp)
	{
		Copy(wp);
	}

	aiWaypoint &operator =(const aiWaypoint &wp)
	{
		Copy(wp);
		return (*this);
	}

	void Copy(const aiwaypoint_info &inf)
	{
		to = aiPoint(inf.to_x,inf.to_y,inf.to_z);
		speed = inf.speed;
		altitude = inf.altitude;
		task = inf.task;
		time = inf.time;
		iTaskModifier[0] = inf.i_task_modifier[0];
		iTaskModifier[1] = inf.i_task_modifier[1];
		iTaskModifier[2] = inf.i_task_modifier[2];
		dTaskModifier[0] = inf.d_task_modifier[0];
		dTaskModifier[1] = inf.d_task_modifier[1];
		dTaskModifier[2] = inf.d_task_modifier[2];
		memcpy(cTaskModifier,inf.c_task_modifier,sizeof(cTaskModifier));
		formationType = inf.formation_type;

	}

	aiWaypoint(const aiwaypoint_info &inf)
	{
		Copy(inf);
	}

	aiWaypoint &operator =(const aiwaypoint_info &inf)
	{
		Copy(inf);
		return (*this);
	}

	const char *GetFieldId()
	{
		return cTaskModifier;
	}

	int GetRunwayNo()
	{
		return iTaskModifier[0];
	}
	int GetManeuverType()
	{
		return iTaskModifier[0];
	}
	int GetManeuverFlags()
	{
		return iTaskModifier[1];
	}
	aiREAL GetManeuverData0()
	{
		return dTaskModifier[0];
	}
	aiREAL GetManeuverData1()
	{
		return dTaskModifier[1];
	}
	aiREAL GetManeuverData2()
	{
		return dTaskModifier[2];
	}
};

/*
 *  Navigation info
 */
class aiNavInfo
{
public:
	aiPoint		navPoint;			/* location in world */
	aiVector		worldDirection;	/* direction from us in world coords */
	aiAttitude	relAttitude;		/* pitch, yaw relative to us (body coords) */
	aiAttitude	attitude;			/* pitch, yaw in world coords */
	aiREAL			range;				/* current distance */
	aiREAL			dRange;				/* approach rate */
	aiWaypoint	waypoint;			/* waypoint info */
	aiNavInfo()
	{
		range = dRange = 0.0;
	}
};

#endif /* __cplusplus */
#endif
