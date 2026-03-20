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
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "aislewer.h"

void aiSlewer::Rotate(const aiAttitude &rotateMatrix)
{
	aiVector matN[3];
	aiVector rotN[2];
	aiAttitude newAtt;


	// Build matrix for rotation
	aiMatrix3RPY(rotateMatrix,matN);
	// Make pitch, roll normals
	rotN[0] = aiVector(1,0,0);		
	rotN[1] = aiVector(0,1,0);
	// Rotate them
	aiMatrix3Rotate(matN,rotN[0],rotN[0]);
	aiMatrix3Rotate(matN,rotN[1],rotN[1]);
	// Convert to world (rotate to our pos)
	BodyVector2WorldVector(rotN[0],rotN[0]);
	BodyVector2WorldVector(rotN[1],rotN[1]);
	// Get new attitude
	aiRPY(rotN,newAtt);
	SetAttitude(newAtt);
}

void aiSlewer::TranslateWorld(const aiVector &worldVector)
{
  aiPoint position;
  GetPosition(position);
  position += worldVector;
  SetPosition(position);
}

void aiSlewer::TranslateBody(const aiVector &bodyVector)
{
  aiVector worldVector;
  BodyVector2WorldVector(bodyVector,worldVector);
  TranslateWorld(worldVector);
}

void aiSlewer::BodyVector2WorldVector(const aiVector &body, aiVector &world)
{
  aiMatrix3Rotate(mat,body,world);
}

void aiSlewer::WorldVector2BodyVector(const aiVector &world, aiVector &body)
{
  aiMatrix3Rotate(invMat,world,body);
}

void aiSlewer::BodyPoint2WorldPoint(const aiPoint &body, aiPoint &world)
{
  aiPoint position;
  aiVector worldVector;
  GetPosition(position);
  BodyVector2WorldVector((const aiVector &)body,worldVector);
  world = position + worldVector;
}

void aiSlewer::WorldPoint2BodyPoint(const aiPoint &world, aiPoint &body)
{
	aiPoint position;
	aiVector worldVector;
	aiVector bodyVector;

  GetPosition(position);
  worldVector = world - position;
  WorldVector2BodyVector(worldVector,bodyVector);
  body = (aiPoint &) bodyVector;
}

void aiSlewer::BuildDirectionNormal()
{
  aiVector bodyNorm = aiVector(0,1.0,0);
  BodyVector2WorldVector(bodyNorm,dirNormal);
}

void aiSlewer::GetAttitudeToPoint(const aiPoint &wPoint, aiVector &direction, aiAttitude &attitude)
{
	aiPoint position;
	aiVector bodyV;

	GetPosition(position);
	direction = wPoint - position;
	WorldVector2BodyVector(direction,bodyV);
	aiVector2aiAttitude(bodyV,attitude);
}

