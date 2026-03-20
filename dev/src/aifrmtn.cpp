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
 * File   : sfrmtn.cpp                           *
 * Date   : April, 1998                          *
 * Author : Dan Hammer                           *
 *************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "aifrmtn.h"

struct FormationDat aiFormationData::formationDat[aifNFORMATION_TYPES] =
{
	{
		"wedge",
		{
         {  0,  0, 0   },
			{ -2, -2, 0.2 },
			{  2, -2, 0.2 },
			{  4, -4, 0.4 },
		},
		{
			{ -6, -6, 0   },
			{  6, -6, 0   },
			{-12,-12, 0   },
			{ 12,-12, 0   },
			{  0,-18, 0   }
		}
	},
	{
		"vic",
		{
         {  0,  0, 0   },
			{ -1, -1, 0.1 },
			{  1, -1, 0.1 },
			{  2, -2, 0.2 },
		},
		{
			{ -3, -3, 0   },
			{  3, -3, 0   },
			{ -6, -6, 0   },
			{  6, -6, 0   },
			{  0,-12, 0   }
		}
	},
	{
		"echelon right",
		{
            {  0,  0, 0   },
			{  2, -2, 0.2 },
			{  4, -4, 0.4 },
			{  6, -6, 0.6 },
		},
		{
			{-12, -6, 0   },
			{ 12, -6, 0   },
			{-24,-12, 0   },
			{ 24,-12, 0   },
			{  0,-18, 0   }
		}
	},
	{
		"echelon left",
		{
			{  0,  0, 0   },
			{ -2, -2, 0.2 },
			{ -4, -4, 0.4 },
			{ -6, -6, 0.6 },
		},
		{
			{-12, -6, 0   },
			{ 12, -6, 0   },
			{-24,-12, 0   },
			{ 24,-12, 0   },
			{  0,-18, 0   }
		}
	},
	{
		"trail",
		{
         {  0,  0, 0   },
			{  0, -2, 0.2 },
			{  0, -4, 0.4 },
			{  0, -6, 0.6 },
		},
		{
			{-12, -8, 0   },
			{ 12, -8, 0   },
			{-24,-16, 0   },
			{ 24,-16, 0   },
			{ 32,-24, 0   }
		}
	},
	{
		"ladder",
		{
         {  0,  0, 0   },
			{  0, -1, 0.4 },
			{  0, -2, 0.8 },
			{  0, -3, 1.2 },
		},
		{
			{ -4, -3, 0   },
			{  4, -3, 0   },
			{ -8, -6, 0   },
			{  8, -6, 0   },
			{  0, -9, 0   }
		}
	},
	{
		"diamond",
		{
         {  0,  0, 0 },
			{ -2, -2, 0 },
			{  2, -2, 0 },
			{  0, -4, 0 },
		},
		{
			{ -8, -4, 0   },
			{  4, -8, 0   },
			{  0, -8, 0   },
			{ -8,-12, 0   },
			{  8, 12, 0   }
		}
	},
	{
		"fighting wing",
		{
         {  0,  0, 0   },
			{ 20,-20, 0.2 },
			{ 40,-40, 0.4 },
			{ 60,-60, 0.6 },
		},
		{
			{ -2, -2, 0.2 },
			{  2, -2, 0.2 },
			{ -4, -4, 0.4 },
			{  4, -4, 0.4 },
			{ -6, -6, 0.6 }
		}
	}
};

aiFormationData aiFormationData::formationData[aifNFORMATION_TYPES];

void aiFormationData::Init(void)
{
	for (int i=0;i<aifNFORMATION_TYPES;i++)
	{
		aiFormationData &fmd = formationData[i];
		fmd.Set(formationDat[i]);
	}
}

void aiFormationData::Set(const struct FormationDat &dt)
{
int i;

	strncpy(name,dt.name,sizeof(name));
	name[sizeof(name)-1] = 0;
	for (i=0;i<aifFORMATION_MAX;i++)
		wingmanOffsets[i] = aiVector(-dt.woffsets[i][0],
									dt.woffsets[i][1],
									dt.woffsets[i][2]);
	for (i=0;i<aifMAXGROUPS;i++)
		groupOffsets[i] = aiVector(-dt.goffsets[i][0],
								  dt.goffsets[i][1],
								  dt.goffsets[i][2]);

}

const aiFormationData &aiFormationData::GetFormationData(int formation)
{
	if (formation < 0)
		formation = 0;
	if (formation >= aifNFORMATION_TYPES)
		formation = aifNFORMATION_TYPES - 1;
	return (formationData[formation]);
}

const aiFormationData &aiFormationData::GetFormationData(char *formation_name)
{
int formation = 0;

	for (int i=0;i<aifNFORMATION_TYPES;i++)
	{
		if (!strcmp(formation_name,formationData[i].name))
		{
			formation = i;
			break;
		}
	}
	return (GetFormationData(formation));
}

void aiFormationData::GetWingmanVector(int formation,
    					int wingpos,
	  				aiVector &result)
{
const aiFormationData &fmd = GetFormationData(formation);

	if (wingpos > aifMAXWINGMEN)
    {
        int wp;
        int group;
        group = wingpos / aifFORMATION_MAX;
        if (group >= aifMAXGROUPS)
          group = aifMAXGROUPS - 1;
        wp = (wingpos % aifFORMATION_MAX);
        result = fmd.wingmanOffsets[wp] + fmd.groupOffsets[group];
    }
    else
      result = fmd.wingmanOffsets[wingpos];
}

void aiFormationData::GetWingmanVector(char *formation_name,
													int wingpos,
													aiVector &result)
{
const aiFormationData &fmd = GetFormationData(formation_name);

	if (wingpos > aifMAXWINGMEN)
    {
        int wp;
        int group;
        group = wingpos / aifFORMATION_MAX;
        if (group >= aifMAXGROUPS)
          group = aifMAXGROUPS - 1;
        wp = (wingpos % aifFORMATION_MAX);
        result = fmd.wingmanOffsets[wp] + fmd.groupOffsets[group];
    }
    else
      result = fmd.wingmanOffsets[wingpos];
}

aiFormationData::aiFormationData()
{

}

aiFormationData::~aiFormationData()
{

}

