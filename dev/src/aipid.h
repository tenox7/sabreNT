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
 * File   : aipid.h                               *
 * Date   : April, 1998                          *
 * Author : Dan Hammer                           *
 * PID class                                     *
 * PID stands for Proportional Integral          *
 * Derivative. These are 3 coefficiants which    *
 * are used in a standard control algorythm:     *
 * Given a set point y (where you want to be),   *
 * a current value x (where you are), and a rate *
 * of change r:                                  *
 * distance = y - x                              *
 * per = (distance * p - r * d) * i              *
 * The work is getting the p,i and d coefficiants*
 * balanced for a given flight model. We also use*
 * a "base" which represents the air-speed at    *
 * which the 3 coefficiants have been balanced ..*
 * we than adjust them to the current speed by   *
 * making them proportional to the square of the *
 * the base speed over the current speed         *
 *************************************************/
#ifndef __aipid_h
#define __aipid_h

#include "aimath.h"

#define DEFAULT_P      1.0
#define DEFAULT_I      1.0
#define DEFAULT_D      2.0
#define DEFAULT_BASE   (260.0*260.0)
/*
 * Make "C" happy
 */
typedef struct _aiPID_struct
{
  aiREAL p;
  aiREAL i;
  aiREAL d;
  aiREAL base;
} _aiPID;

#ifdef __cplusplus
extern "C" {
#endif
extern void default_aiPID(_aiPID *spid);
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

class aiPID
{
public:
  aiREAL p;
  aiREAL i;
  aiREAL d;
  aiREAL base;

  aiPID()
    :p(DEFAULT_P),
    i(DEFAULT_I),
    d(DEFAULT_D),
    base(DEFAULT_BASE*DEFAULT_BASE)
    {}

  void Copy(const aiPID &spid)
    {
      p = spid.p;
      i = spid.i;
      d = spid.d;
      base = spid.base;
    }
  
  aiPID(const aiPID &spid)
    {
      Copy(spid);
    }

  const aiPID &operator =(const aiPID &spid)
    {
      Copy(spid);
      return *this;
    }

  inline const aiPID &operator =(const _aiPID & _spid);
  inline aiREAL CalcPer(aiREAL setPoint, aiREAL curPoint, aiREAL rate, aiREAL curBase);  
};

inline aiREAL aiPID::CalcPer(aiREAL setPoint, aiREAL curPoint, aiREAL rate, aiREAL curBase)
{
  aiREAL fact;
  aiREAL pp,ii,dd;
  aiREAL result;
  aiREAL distance;

  fact = base / (curBase+eps);
  pp = p * fact;
  ii = i * fact;
  dd = d * fact;
  distance = setPoint - curPoint;
  result = (distance * pp - rate * dd) * ii;
  return result;
}

inline const aiPID &aiPID::operator =(const _aiPID & _spid)
{
  p = _spid.p;
  i = _spid.i;
  d = _spid.d;
  base = _spid.base;
  return *this;
}

#endif


#endif

