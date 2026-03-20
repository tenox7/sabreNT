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
 * File   : aigunner.h                           *
 * Date   : April, 1998                          *
 * Author : Dan Hammer                           *
 * aiGunner class definition                     *
 *************************************************/
#ifndef __aigunner_h
#define __aigunner_h

#define aiGUN_PITCH_RATE      (_degree * 20.0)
#define aiGUN_YAW_RATE        (_degree * 20.0)

#ifdef __cplusplus
#include "fstream.h"
#include "aiobject.h"
#include "aiarray.h"
#include "aifltmdl.h"
#include "scntrl.h"
#include "aitarget.h"
#include "aiweapon.h"
#include "aimnvrst.h"
#include "aitact.h"
#include "aiwaypnt.h"
#include "aiattkr.h"

class aiGunner: public aiObject
{
protected:
	/********************************************************************
	* protected members                                                 *
	*********************************************************************/
	int				index;					/* index                        */
	unsigned long	ownerIdx;				/* owner's idx						  */
	int				affiliation;			/* whose side                   */
	aiVector			offset;					/* offset relative to 0,0,0 in  */
													/*  plane's body coords         */
	aiFlightModel	*flightModel;			/* flight model pointer         */
	aiPoint			worldPosition;			/* where am I in world          */
	int				isDead;					/* 1 == dead                    */
	int				isEjected;				/* 1 == ejected                 */
	int				hasTarget;				/* 1 == has a target            */
	aiTarget			target;					/* current target               */
	aiWeaponLimits	weaponLimits;			/* current weapon information   */
	aiREAL				bulletRadius;			/* marksmanship factor          */
	aiREAL				timeFrame;				/* time between frames          */
	aiAttitude		gunAttitude;			/* position of gun              */
	aiREAL				gunPitchRate;			/* rate at which gun can change pitch */
	aiREAL				gunYawRate;				/*   same for yaw               */
	aiAttitude		jiggleAtt;				/* gun jiggling data            */

	virtual void	GetTargetGeometry();
	virtual int		Shoot();
	virtual void	GetAttitude(const aiPoint &position, 
								aiVector &direction, 
								aiAttitude &attitude) = 0;
	virtual void	GetWeaponLimits(aiWeaponLimits &weaponLimits) = 0;
	void				AdjustGun();
public:
	aiGunner();
	aiGunner(aiFlightModel *fltModel, int index,
			unsigned long ownerIdx, int affiliation, aiREAL bulletRadius,
			aiREAL gunPitchRate = aiGUN_PITCH_RATE,
			aiREAL gunYawRate = aiGUN_YAW_RATE,
			aiAttitude *jiggleAtt = NULL );
	virtual ~aiGunner()
	{}
	virtual void Init();
	virtual void Update(double frame_time);
	virtual void Destroy();

	int HasTarget()
	{
		return (hasTarget);
	}
	void  SetTarget(unsigned long targetIdx);
	void  UnsetTarget()
	{
		hasTarget = 0;
	}
	unsigned long GetTargetIdx();
	int GetAffiliation()
	{
		return (affiliation);
	}
	void SetAffiliation(int affiliation)
	{
		this->affiliation = affiliation;
	}
	int GetIndex()
	{
		return (index);
	}
	void SetIndex(int index)
	{
		this->index = index;
	}
	unsigned long GetOwnerIdx()
	{
		return (ownerIdx);
	}
	void SetOwnerIdx(unsigned long ownerIdx)
	{
		this->ownerIdx = ownerIdx;
	}
	int IsDead()
	{
		return (isDead);
	}
	void SetDeath(int death)
	{
		isDead = death;
	}
	int IsEjected()
	{
		return (isEjected);
	}
	void SetEjected(int ejected)
	{
		this->isEjected = ejected;
	}
	int IsActive()
	{
		return (!(IsDead() || IsEjected()));
	}
	void SetBulletRadius(aiREAL bulletRadius)
	{
		this->bulletRadius = bulletRadius;
	}
	aiREAL GetBulletRadius()
	{
		return (bulletRadius);
	}
	void SetFlightModel(aiFlightModel *flightModel)
	{
		this->flightModel = flightModel;
	}
	void SetOffset(const aiVector &offset)
	{
		this->offset = offset;
	}
	void SetWorldPosition(const aiPoint &worldPosition)
	{
		this->worldPosition = worldPosition;
	}
	virtual void DoHUD() = 0;
	aiREAL GetTargetThreatValue()
	{
		return target.threatValue;	
	}
	void SetTargetThreatValue(aiREAL threatValue)
	{
		target.threatValue = threatValue;
	}

	friend class aiPilot;
	friend class aiFlite;
};
#endif
#endif
