/*
    Sabre Fighter Plane Simulator 
    Copyright (c) 1997 Dan Hammer
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
#ifndef __pilot_h
#define __pilot_h

#include "aipilot.h"
#include "sbfltmdl.h"
#include "flight.h"
#include "weapons.h"
#include "pilobj.h"

#define sbrPIL_MAX_STORES 3
#define sbrPIL_MAX_GUNS   16

#define sbrPIL_SHOTS_PER_WEAPON_MIN  3
#define sbrPIL_SHOTS_PER_WEAPON_MAX  16


class Pilot : public aiPilot
{
public:
	Pilot( Flight *flt, Pilot_Params *,
	 Weapon_Instance *, int nw, char *, Target *);
	
	void init(void);
	Pilot *get_target_pilot(void)
	{
		return target_pilot;
	}
	Flight *get_target_flight(void)
	{
		return target_flight;
	}
	int get_functioning(void)
	{
		return functioning;
	}
	void set_functioning(int functioning)
	{
		this->functioning = functioning;
		if (!functioning)
			SetDeath(1);
	}
	const char *get_dbg(void)
	{
		return dbg;
	}
	void set_dbg(const char *s)
	{
		dbg = (char *) s;
	}
   Weapon_Instance *get_sel_weapon(void)
	{
		return (sel_weapon);
	}
	Flight *get_flight(void)
	{
		return sbrFlight.GetFlight();
	}
	Port_3D *get_port(void)
	{
		return sbrFlight.GetPort();
	}
	const R_3DPoint &get_position(void)
	{
		return get_port()->look_from;
	}
	const char *get_handle(void)
	{
		return GetHandle();
	}
	PilotMessage *get_message(void)
	{
		return (&message);
	}
	R_3DPoint get_track_point(void)
	{
		return track_point;
	}
	R_3DPoint get_lead_track_point(void)
	{
		return lead_track_point;
	}
	R_3DPoint get_gun_track_point(void)
	{
		return gun_track_point;
	}
	R_3DPoint get_gun_point(void)
	{
		return gun_point;
	}
	int get_in_range(void)
	{
		return in_range;
	}
	int haaiTargetPilot()
	{
		return (target_pilot != NULL);
	}
	int getAffiliation()
	{
		return GetAffiliation();
	}
   void selWeapon(int );
	void selectNextWeapon(int dir);
	void checkForNextWeapon(void);
	void set_target(Pilot *pl);
	void update(int);
	void pause();
	void start();
	Target *get_target_obj(void)
	{
		return target_obj;
	}
	void set_handle(const char *hndl)
	{
		SetHandle(hndl);		
	}
	void update_sbrflight()
	{
		sbrFlight.Update(0);
	}
	
	void set_node_index(int index)
	{
		node_index = index;
	}
	int get_node_index(void)
	{
		return node_index;
	}
	void set_target_node_index(int index)
	{
		target_node_index = index;
	}
	int get_target_node_index(void)
	{
		return target_node_index;
	}
	static void UpdateStatics(void);

public:
  static Pilot   *pilots[];
  static int     npilots;
  static int     maxpilots;
  static initPilot();
  static Pilot *getPilot(char *hndle);
  static float xpixel_adjust, ypixel_adjust;

protected:
	void GetWeaponLimits(aiWeaponLimits &weaponLimits);
	void Broadcast(int idx, int channel = commCHANNEL_AFFILIATION,
						 int priority = commPRIORITY_STANDARD,
						 void *extraInfo = NULL);
	void GetRemoteControlInputs();
	void DropBomb();
	void BuildGunners();
	void CalcGunLeadPoint(aiTargetGeometry &tg);
	void Shoot();
private:
	sbrFlightModel	 sbrFlight;
	Flight			 *flight;
	Target			 *target;
	Pilot           *target_pilot;
	Flight          *target_flight;
	int             functioning;
	Weapon_Instance *weapons;
	Weapon_Instance *sel_weapon;
	int				 sel_wpn;
	int				 n_weaps;
	PilotMessage    message;
	R_3DPoint       gun_point;
	R_3DPoint		 gun_track_point;
	R_3DPoint		 lead_track_point;
	R_3DPoint       track_point;
	int             nstores;
	int             stores[sbrPIL_MAX_STORES];
	int             nguns;
	int             guns[sbrPIL_MAX_GUNS];
	int             gunsRounds[sbrPIL_MAX_GUNS];
	int             gunsIdx;
	Pilot_Params    *params;
	int				 in_range;
	Target			 *target_obj;
	float				 t;
	float				 time_to_target;
	float				 new_damage;
	int             node_index;
	int             target_node_index;
	static float    gunsGunsSoundTime;

	int broadcast(char *mss, int priority, int freq = -1, char * = NULL);
	int __cdecl brdcst(int priority, int freq, char *mss, ...);
	void sound_on(const char *soundId, int affiliation = -1); 
	const char *buildSoundId(char *suffix, Flight *targetFlight);
	int damage_check(void);
};
#endif



