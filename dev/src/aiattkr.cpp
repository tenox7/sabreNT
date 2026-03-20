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
 * File   : sattkr.cpp                           *
 * Date   : April, 1998                          *
 * Author : Dan Hammer                           *
 *************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include "dhlist.h"
#include "aiattkr.h"

/*
 * static aiAttackerList functions
 */
// callbacks from dhlist
void aiAttackerList::DeleteAiAttacker(void *v)
{
  aiAttacker *attkr = (aiAttacker *) v;
  delete attkr;
}

// callback for find
int aiAttackerList::CompareAiAttacker(void *v0, void *v1)
{
  aiAttacker *attkr0 = (aiAttacker *) v0;
  aiAttacker *attkr1 = (aiAttacker *) v1;
  if (attkr0->IsEqual(*attkr1))
    return (1);
  else
    return (0);
}

// callback for sort
int aiAttackerList::CompareAiAttackerThreat(void *v0, void *v1)
{
  aiAttacker *attkr0 = (aiAttacker *) v0;
  aiAttacker *attkr1 = (aiAttacker *) v1;
  return (attkr0->GetThreatValue() > attkr1->GetThreatValue());  
}

/*
 *  add an attacker 
 */
void aiAttackerList::AddAttacker(unsigned long idx)
{
	void *v;
	aiAttacker tmp(idx);
	v = dhlist_find(&attacker_list,CompareAiAttacker,&tmp);
	if (v == NULL)
	{
		aiAttacker *attkr = new aiAttacker(idx);
		dhlist_append(&attacker_list,attkr,DeleteAiAttacker);
		count++;
	}
}

/*
 * delete an attacker
 */
void aiAttackerList::DeleteAttacker(unsigned long idx)
{
	void *v;
	aiAttacker tmp(idx);
	v = dhlist_find(&attacker_list,CompareAiAttacker,&tmp);
	if (v)
	{
		dhlist_delete(&attacker_list,v,1);
		count--;
	}
}

void aiAttackerList::DeleteAllAttackers()
{
  dhlist_flush(&attacker_list,1);
  count = 0;
}

/*
 * return first attacker in list
 */
aiAttacker *aiAttackerList::GetFirstAttacker()
{
  void *v;
  dhlist_start_iterator(&attacker_list);
  v = dhlist_next_object(&attacker_list);
  return ((aiAttacker *)v);
}

/*
 * return next attacker in list
 */
aiAttacker *aiAttackerList::GetNextAttacker()
{
  void *v;
  v = dhlist_next_object(&attacker_list);
  return ((aiAttacker *)v);
}


aiAttacker *aiAttackerList::GetAttackerByIdx(unsigned long idx)
{
	aiAttacker *result = NULL;
	aiAttacker *attkr = GetFirstAttacker();
	while (attkr)
	{
		if (attkr->GetIdx() == idx)
		{
			result = attkr;
			break;
		}
	}
	return result;
}

