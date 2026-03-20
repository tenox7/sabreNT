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
 * File   : aifield.h                            *
 * Date   : April, 1998                          *
 * Author : Dan Hammer                           *
 *************************************************/
#ifndef __aifield_h
#define __aifield_h

#include "airunway.h"
#include "aiarray.h"

#define aiFIELD_MAX_RUNWAYS		4
#define aiFIELD_MAX_AIRFIELDS		256

typedef struct aifield_info_struct
{
	char		id[32];
	int		nrunways;
	double	x;
	double	y;
	double	z;
	airunway_info runways[aiFIELD_MAX_RUNWAYS];
} aifield_info;

#ifdef __cplusplus
extern "C" {
#endif	
	void add_srunway(aifield_info *airfield, airunway_info *runwayinf);
	void clear_airfield_info(aifield_info *airfield_info);
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus


class aiField : public aiObject
{
public:
	aiField()
	{}

	aiField(const aifield_info &);

	aiRunway	*GetRunway(int which);		
	
	const aiPoint &GetPosition()
	{
		return position;
	}

	static void AddaiField(aiField *);
	static void AddaiField(const aifield_info &);
	static aiField *GetaiField(const char *airfieldId);
	static void FlushaiFields();

protected:
	char						id[32];
	aiObjectArray			runways;
	aiPoint					position;

	static aiObjectArray airfields;
};

#endif

#endif
