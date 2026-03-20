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
 * File   : sfrmtnt.h                            *
 * Date   : April, 1998                          *
 * Author : Dan Hammer                           *
 * formation flying data                         *
 *************************************************/
#ifndef __aifrmtn_h
#define __aifrmtn_h

enum	{	
			aifWEDGE, aifVIC, aifECHELON_R, aifECHELON_L,
			aifTRAIL, aifLADDER, aifDIAMOND, aifFIGHTING_WING 
		};
#define aifMAXWINGMEN			3
#define aifFORMATION_MAX		(aifMAXWINGMEN+1)
#define aifMAXGROUPS			5
#define aifNFORMATION_TYPES	8

#ifdef __cplusplus

#include "aiobject.h"
#include "aimath.h"


struct FormationDat
{
	char * name;
	aiREAL woffsets[aifFORMATION_MAX][3];
	aiREAL goffsets[aifMAXGROUPS][3];
};

class aiFormationData : public aiObject
{
public:
	char		name[32];
	aiVector     wingmanOffsets[aifFORMATION_MAX];
	aiVector		groupOffsets[aifMAXGROUPS];

	aiFormationData();
	~aiFormationData();
	void Set(const struct FormationDat &);
	static aiFormationData formationData[aifNFORMATION_TYPES];
	static struct FormationDat formationDat[aifNFORMATION_TYPES];
	static void Init(void);
	static const aiFormationData &GetFormationData(int formation);
	static const aiFormationData &GetFormationData(char *formation_name);
	static void GetWingmanVector(int formation,
								int wingpos,
								aiVector &result);
	static void GetWingmanVector(char *formation_name,
								int wingpos,
								aiVector &result);
};




#endif /* __cplusplus */
#endif
