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
 * File   : aislewer.h                            *
 * Date   : November, 1998                       *
 * Author : Dan Hammer                           *
 * base class for anything that can move and     *
 * and rotate in a 3-D world                     *
 *************************************************/
#ifndef __aislewer_h
#define __aislewer_h

#ifdef __cplusplus
#include "aimath.h"
#include "aiobject.h"

class aiSlewer : public aiObject
{
public:

	aiSlewer()
	{}

	virtual int IsA() const
	{
		return (aiSLEWER_T);
	}

	virtual void SetPosition(const aiPoint &position)
	{
		pos = position;
	}
	void GetPosition(aiPoint &position)
	{
		position = pos;
	}
	virtual void SetAttitude(const aiAttitude &attitude)
	{
		att = attitude;
		BuildMatrixes();
	}
	void GetAttitude(aiAttitude &attitude)
	{
		attitude = att;
	}
	void GetPositionAndAttitude(aiPoint &position, aiAttitude &attitude)
	{
		position = pos;
		attitude = att;
	}
	virtual void SetPositionAndAttitude(const aiPoint &position, const aiAttitude &attitude)
	{
		pos = position;
		att = attitude;
		BuildMatrixes();
	}

	void GetDirectionNormal(aiVector &directionNormal)
	{
		directionNormal = dirNormal;
	}

	virtual void Rotate(const aiAttitude &rotateMatrix);
	void TranslateWorld(const aiVector &worldVector);
	void TranslateBody(const aiVector &bodyVector);

	void BodyVector2WorldVector(const aiVector &body, aiVector &world);
	void WorldVector2BodyVector(const aiVector &world, aiVector &body);
	void BodyPoint2WorldPoint(const aiPoint &body, aiPoint &world);
	void WorldPoint2BodyPoint(const aiPoint &world, aiPoint &body);
	void GetAttitudeToPoint(const aiPoint &wPoint, aiVector &direction, aiAttitude &attitude);
protected:
  aiPoint    pos;
  aiAttitude att;
  aiVector   mat[3];
  aiVector   invMat[3];
  aiVector   dirNormal;

	void BuildMatrixes()
	{
		aiMatrix3RPY(att,mat);
		aiMatrix3Transpose(mat,invMat);
		BuildDirectionNormal();
	}
  void BuildDirectionNormal();
};

#endif

#endif
