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
 * File   : aimath.h                             *
 * Date   : April, 1998                          *
 * Author : Dan Hammer                           *
 * Convert from and to "ai" structures           *
 *************************************************/
#ifndef __ai2sbr_h
#define __ai2sbr_h


#ifdef __cplusplus
#include "vmath.h"
#include "port_3d.h"
#include "aimath.h"

extern float world_scale;

inline aiVector & Vector2aiVector(const Vector &v, aiVector &sv)
{
	sv.x = (aiREAL) v.X;
	sv.y = (aiREAL) v.Y;
	sv.z = (aiREAL) v.Z;

	return sv;
}

inline Vector & aiVector2Vector(const aiVector &sv, Vector &v)
{
	v.X = (REAL_TYPE) sv.x;
	v.Y = (REAL_TYPE) sv.y;
	v.Z = (REAL_TYPE) sv.z;

	return v;
}

inline aiPoint & R_3DPoint2aiPoint(const R_3DPoint &r, aiPoint &s)
{
	s.x = (aiREAL) (r.x / world_scale);
	s.y = (aiREAL) (r.y / world_scale);
	s.z = (aiREAL) (r.z / world_scale);
	return s;
}

inline R_3DPoint & aiPoint2R_3DPoint(const aiPoint &s, R_3DPoint &r)
{
	r.x = ((REAL_TYPE) s.x * world_scale);
	r.y = ((REAL_TYPE) s.y * world_scale);
	r.z = ((REAL_TYPE) s.z * world_scale);
	return r;
}

inline aiREAL CvFrSabreAngle(REAL_TYPE sbrAng)
{
	if ((aiREAL) sbrAng > Pi)
		return (aiREAL)sbrAng - _2Pi;
	else
		return (aiREAL) sbrAng;
}

inline REAL_TYPE CvToSabreAngle(aiREAL ang)
{
	return ((REAL_TYPE) ConvertAngle(ang));
}

inline void Port2aiAttitude(const Port_3D &port, aiAttitude &att)
{
aiPoint  lookFrom;
aiPoint  lookAt;
aiVector dirNormal;

	R_3DPoint2aiPoint(port.look_from, lookFrom);
	R_3DPoint2aiPoint(port.look_at,lookAt);
	dirNormal = lookAt - lookFrom;
	dirNormal.Normalize();
	aiVector2aiAttitude(dirNormal,att);
	att.roll = CvFrSabreAngle(port.roll);
}

inline void GetRandomVector(Vector *v, REAL_TYPE xbase,
													REAL_TYPE xmag,
													REAL_TYPE ybase,
													REAL_TYPE ymag,
													REAL_TYPE zbase,
													REAL_TYPE zmag)
{
	v->X = aiRand((aiREAL)xmag);
	if (v->X < 0)
		v->X -= xbase;
	else
		v->X += xbase;
	v->Y = aiRand((aiREAL)ymag);
	if (v->Y < 0)
		v->Y -= ybase;
	else
		v->Y += ybase;
	v->Z = aiRand((aiREAL)zmag);
	if (v->Z < 0)
		v->Z -= zbase;
	else
		v->Z += zbase;
}
#endif

#endif
