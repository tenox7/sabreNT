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
 * File   : aimath.C                             *
 * Date   : April, 1998                          *
 * Author : Dan Hammer                           *
 *************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "aimath.h"

aiREAL aiDistanceSquared(const aiPoint &p0, const aiPoint &p1)
{
  aiVector v0(p1.x - p0.x,p1.y - p0.y,p1.z - p0.z);
  return (v0.MagnitudeSquared());
}

aiREAL aiDistance(const aiPoint &p0, const aiPoint &p1)
{
  return (sSQRT(aiDistanceSquared(p0,p1)));
}

void aiVector2aiAttitude(const aiVector &vect, aiAttitude &att)
{
  att.roll = 0.0;
  aiREAL d;
  att.yaw = (aiREAL) atan2(-vect.x,vect.y+eps);
  d = sSQRT((vect.x*vect.x)+(vect.y*vect.y));
  att.pitch = atan2(vect.z,d+eps);
}

aiREAL aiRandPer()
{
  int d;
  d = rand();
  return ( ((aiREAL)d) / ((aiREAL)RAND_MAX) );
}

aiREAL aiRand(aiREAL x)
{
	aiREAL result = aiRandPer() * x;
	if (aiFlipCoin())
		result = -result;
	return result;
}

int aiRand(int min, int max)
{
  int swap;
  int result;

  if (min > max)
    {
      swap = min;
      min = max;
      max = swap;
    }
  aiREAL rmin = (aiREAL) min;
  aiREAL rmax = (aiREAL) max + 1;
  aiREAL v = aiRandPer();
  aiREAL i = rmin + ((rmax - rmin) * v);
  result = (int) i;
  if (result < min)
    result = min;
  if (result > max)
    result = max;
  return result;
}

int aiFlipCoin()
{
  aiREAL rp = aiRandPer();
  int result = rp <= 0.5;
  return (result);
}

int sFlip3SideCoin()
{
  return (aiRandPer() <= 0.333);
}

int sFlip4SideCoin()
{
  return (aiRandPer() <= 0.25);
}

/***************************************************
 * matrix stuff                                    *
 ***************************************************/
// Create a rotation matrix from an attitude
void aiMatrix3RPY(const aiAttitude &att, aiVector *mat3)
{
  aiVector tmp0[3];
  aiVector tmp1[3];
  aiVector tmp2[3];

  aiMatrix3X(att.pitch,tmp0);
  aiMatrix3Y(att.roll,tmp1);
  aiMatrix3Mult(tmp0,tmp1,tmp2);
  aiMatrix3Z(att.yaw,tmp0);
  aiMatrix3Mult(tmp0,tmp2,mat3);
}

// Create an attitude from roll, pitch normals
void aiRPY(aiVector v[2], aiAttitude &att)
{
	aiVector roll = v[0];
	aiVector pitch = v[1];
	aiVector mat3[3];

   if ( pitch.y != 0.0 ||  pitch.x != 0.0)
   {
      att.yaw = atan2(pitch.y,pitch.x) - Pi_2;
      aiMatrix3Z((_2Pi - att.yaw), mat3);
		aiMatrix3Rotate(mat3, pitch, pitch);
		aiMatrix3Rotate(mat3, roll, roll);
   }
   else
      att.yaw = 0.0;

   att.pitch = atan2(pitch.z,pitch.y);
   aiMatrix3X((_2Pi - att.pitch), mat3);
   aiMatrix3Rotate(mat3,roll,roll);
   att.roll = atan2(roll.z,roll.x);
}


void aiMatrix3Mult(aiVector *mat0, aiVector *mat1, aiVector *mat2)
{
  mat2[0].x = mat0[0].x * mat1[0].x + mat0[0].y * mat1[1].x + mat0[0].z * mat1[2].x;
  mat2[0].y = mat0[0].x * mat1[0].y + mat0[0].y * mat1[1].y + mat0[0].z * mat1[2].y;
  mat2[0].z = mat0[0].x * mat1[0].z + mat0[0].y * mat1[1].z + mat0[0].z * mat1[2].z;

  mat2[1].x = mat0[1].x * mat1[0].x + mat0[1].y * mat1[1].x + mat0[1].z * mat1[2].x;
  mat2[1].y = mat0[1].x * mat1[0].y + mat0[1].y * mat1[1].y + mat0[1].z * mat1[2].y;
  mat2[1].z = mat0[1].x * mat1[0].z + mat0[1].y * mat1[1].z + mat0[1].z * mat1[2].z;

  mat2[2].x = mat0[2].x * mat1[0].x + mat0[2].y * mat1[1].x + mat0[2].z * mat1[2].x;
  mat2[2].y = mat0[2].x * mat1[0].y + mat0[2].y * mat1[1].y + mat0[2].z * mat1[2].y;
  mat2[2].z = mat0[2].x * mat1[0].z + mat0[2].y * mat1[1].z + mat0[2].z * mat1[2].z;
}

void aiMatrix3X(aiREAL angle, aiVector *mat3)
{
  aiREAL c,s;
  c = sCOS(angle);
  s = sSIN(angle);

  mat3[0].x = 1; mat3[0].y = 0; mat3[0].z = 0;
  mat3[1].x = 0; mat3[1].y = c; mat3[1].z = -s;
  mat3[2].x = 0; mat3[2].y = s; mat3[2].z = c;
}

void aiMatrix3Y(aiREAL angle, aiVector *mat3)
{
  aiREAL  c,s;
  c = sCOS(angle);
  s = sSIN(angle);

  mat3[0].x = c; mat3[0].y = 0; mat3[0].z = -s;
  mat3[1].x = 0; mat3[1].y = 1; mat3[1].z = 0;
  mat3[2].x = s; mat3[2].y = 0; mat3[2].z = c;
}

void aiMatrix3Z(aiREAL angle, aiVector *mat3)
{
  aiREAL c,s;
  c = sCOS(angle);
  s = sSIN(angle);

  mat3[0].x = c; mat3[0].y = -s; mat3[0].z = 0;
  mat3[1].x = s; mat3[1].y = c;  mat3[1].z = 0;
  mat3[2].x = 0; mat3[2].y = 0;  mat3[2].z = 1;
}

void aiMatrix3Rotate(aiVector *mat3, const aiVector &v0, aiVector &v1)
{
  v1.x = v0.x * mat3[0].x + v0.y * mat3[0].y + v0.z * mat3[0].z;
  v1.y = v0.x * mat3[1].x + v0.y * mat3[1].y + v0.z * mat3[1].z;
  v1.z = v0.x * mat3[2].x + v0.y * mat3[2].y + v0.z * mat3[2].z;
}

void aiMatrix3Transpose(aiVector *mat0, aiVector *mat1)
{
  mat1[0].x = mat0[0].x;
  mat1[1].x = mat0[0].y;
  mat1[2].x = mat0[0].z;

  mat1[0].y = mat0[1].x;
  mat1[1].y = mat0[1].y;
  mat1[2].y = mat0[1].z;

  mat1[0].z = mat0[2].x;
  mat1[1].z = mat0[2].y;
  mat1[2].z = mat0[2].z;
}

/*
 *  Increment an angle & wrap around
 *  Pi
 */
aiREAL aiIncrementAngle(aiREAL angle, aiREAL inc)
{
  int cw = 1;   // clockwise
  if (inc <= 0.0)
    cw = 0;
  angle += inc;
  if (cw && angle > Pi)
    angle = -Pi + (angle - Pi);
  else if (angle <= -Pi)
    angle = Pi + (angle + Pi);
  return (angle);
}

/*
 *  Get angle between (shortest distance)
 */
aiREAL GetMedianAngle(aiREAL angle0, aiREAL angle1)
{
  aiREAL result;
  aiREAL d = aiGetCircularDistance(angle0, angle1);
  d /= 2.0;
  result = aiIncrementAngle(angle0,d);
  return (result);
}

/*
 *	Get shortest arc length and direction between two angles 
 */
aiREAL aiGetCircularDistance(aiREAL from, aiREAL to)
{
  aiREAL result;
  /* clockwise distance */
  aiREAL cw_d = 0.0;
  /* counter-clockwise distance */
  aiREAL ccw_d= 0.0;

  if ((to >= 0.0 && from >= 0.0)
      || 
      (to <= 0.0 && from <= 0.0))
    result = to - from;
  else
    {
      if (from >= 0.0)
	{
	  /* 0.0 > to > -Pi */
	  cw_d = Pi - from + Pi + to;
	  ccw_d = to - from;
	}
      else
	{
	  /* 0.0 > from > -Pi */
	  cw_d = to - from;
	  ccw_d = -Pi - from + to - Pi;
	}
      if (fabs(cw_d) < fabs(ccw_d))
	result = cw_d;
      else
	result = ccw_d;
    }
  return (result);
}

/*
 * is angle2 between angle0 & angle1 ?
 */
int aiIsBetween(aiREAL angle0, aiREAL angle1, aiREAL angle2)
{
  aiREAL c_angle0,c_angle1,c_angle2;
  aiREAL min,max;
  /*
   *  convert to 0 ... 2Pi
   */
  c_angle0 = ConvertAngle(angle0);
  c_angle1 = ConvertAngle(angle1);
  c_angle2 = ConvertAngle(angle2);
  aiMinMax(c_angle0,c_angle1,min,max);
  return (c_angle2 >= min && c_angle2 <= max);
}
