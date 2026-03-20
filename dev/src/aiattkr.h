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
 * File   : aimath.h                              *
 * Date   : April, 1998                          *
 * Author : Dan Hammer                           *
 * Attacker list                                 *
 *************************************************/
#ifndef __aiattkr_h
#define __aiattkr_h

#ifdef __cplusplus

#include "aimath.h"
#include "dhlist.h"

/*
 * Attacker record
 */
class aiAttacker
{
protected:
	unsigned long idx;
	aiREAL	threatValue;
public:

	aiAttacker()
	{
		idx = 0L;
		threatValue = 0.0;
	}

	aiAttacker(unsigned long idx)
	{
		this->idx = idx;
		threatValue = 0.0;
	}
       
	void Copy(const aiAttacker &attkr)
	{
		idx = attkr.idx;
		threatValue = attkr.threatValue;
	}

	aiAttacker(const aiAttacker &attkr)
	{
		Copy(attkr);
	}

	aiAttacker & operator =(const aiAttacker &attkr)
	{
		Copy(attkr);
		return (*this);
	}

	int IsEqual(const aiAttacker &attkr)
	{
		return (idx == attkr.idx);
	}

    unsigned long GetIdx()
    {
      return (idx);
    }

    void SetThreatValue(aiREAL threatValue)
    {
        this->threatValue = threatValue;
    }

    aiREAL GetThreatValue()
    {
      return (threatValue);
    }

    friend class aiAttackerList;
};

/*
 * Linked list of attackers
 */
class aiAttackerList
{
protected:
	Dhlist attacker_list;		/* "c" linked list container */
	int	   count;
public:
	aiAttackerList()
	{
		new_dhlist(&attacker_list);
		count = 0;
	}

	~aiAttackerList()
	{
		dhlist_free(&attacker_list,1);
	}

	void AddAttacker(unsigned long idx);
	void DeleteAttacker(unsigned long idx);
	void DeleteAllAttackers();
	int GetCount()
	{
	   return (count);
	}

	aiAttacker *GetFirstAttacker();
	aiAttacker *GetNextAttacker();
	aiAttacker *GetAttackerByIdx(unsigned long idx);

	static void DeleteAiAttacker(void *);
	static int CompareAiAttacker(void *, void *);
	static int CompareAiAttackerThreat(void *, void *);


};

#endif /* __cplusplus */

#endif
