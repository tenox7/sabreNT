/*
    Sabre Fighter Plane Simulator
    Copyright (C) 1997	Dan Hammer

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
 * Sabre Fighter Plane Simulator                 *
 * File   : simsnd.cpp                           *
 * Direct sound                                  *
 * Date   : December, 1998                       *
 * Author : Dan Hammer                           *
 *************************************************/

#ifdef USES_DSOUND
#define WIN32_LEAN_AND_MEAN  
#include <windows.h> 
#include <windowsx.h> 
#include <mmsystem.h>
#include <objbase.h>
#include <iostream.h>
#include <conio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <math.h>
#include <io.h>
#include <fcntl.h>
#include <dsound.h>
#include "winsndx.h"
#endif
#include <stdio.h>
#include "sim.h"
#include "simerr.h"
#include "simfile.h"
#include "simsnd.h"

/*
 *  Map enumerated sounds to id
 */
char *enumSounds[4] = 
{
	"JET_ENGINE",
	"GUN",
	"CRASH",
	"CANNON"
};

static char *soundErr2String(int err);

static R_3DPoint soundViewPoint;
static int soundLock = 0;
static int soundActive = 1;
static int soundAffiliation = 0;

int sound_avail = 0;

/***************************************************************************
 * generic sound interface 
 ***************************************************************************/

int sound_init(long param)
{
	int result = SOUND_NOT_LOADED;
#ifdef USES_DSOUND
	result = sound_error_check(dsnd_init((HWND)param));
#endif
	if (result == SOUND_OK)
		sound_avail = 1;
	else
		sound_avail = 0;
	return result;
}

void sound_destroy(void)
{
#ifdef USES_DSOUND
	dsnd_destroy();
#endif
}

int sound_load_wav(char *path, const char *id)
{
#ifdef USES_DSOUND
	int result = dsnd_load_wav(path,id);
	if (result == SOUND_OK)
		sim_printf("Loaded sound id \"%s\" from %s\n",
						id,path);
	else
		sound_error_check(result);
	return (result);
#else
	return 0;
#endif
}

int sound_on(const char *id, int mode, int vol)
{
	if (soundLock)
		return SOUND_OK;
	if (!soundActive)
		return SOUND_OK;
#ifdef USES_DSOUND
	return sound_error_check(dsnd_play_sound(id,mode,vol));
#else
	return 0;
#endif
}

int sound_on(int idx, int mode, int vol)
{
	if (soundLock)
		return SOUND_OK;
	if (idx >= 0 && idx <= 4)
		return sound_on(enumSounds[idx],mode,vol);
	else
		return SOUND_NOT_FOUND;
}

int sound_off(const char *id)
{
#ifdef USES_DSOUND
	return sound_error_check(dsnd_stop_sound(id));
#else
	return 0;
#endif
}

int sound_off(int idx)
{
	if (idx >= 0 && idx <= 4)
		return sound_off(enumSounds[idx]);
	else
		return SOUND_NOT_FOUND;
}	

int sound_off_all(void)
{
#ifdef USES_DSOUND
	return sound_error_check(dsnd_stop_sounds());
#else
	return 0;
#endif
}

int sound_vol(const char *id, int vol)
{
#ifdef USES_DSOUND
	return sound_error_check(dsnd_set_vol(id,vol));
#else
	return 0;
#endif
}

int sound_freq(const char *id, int freq)
{
#ifdef USES_DSOUND
	return sound_error_check(dsnd_set_freq(id,freq));
#else
	return 0;
#endif
}

int sound_pan(const char *id, int pan)
{
#ifdef USES_DSOUND
	return sound_error_check(dsnd_set_pan(id,pan));
#else
	return 0;
#endif
}

int sound_free(const char *id)
{
#ifdef USES_DSOUND
	return sound_error_check(dsnd_delete_sound(id));
#else
	return 0;
#endif
}

int sound_free_all(void)
{
#ifdef USES_DSOUND
	return sound_error_check(dsnd_delete_all_sounds());
#else
	return 0;
#endif
}

unsigned int sound_status(const char *id)
{
#ifdef USES_DSOUND
	unsigned int result = 0;
	dsnd_status_sound(id,&result);
	return result;	
#else
	return 0;
#endif
}

void sound_set_viewpoint(const R_3DPoint &p)
{
	soundViewPoint = p;
}

int sound_calc_distant_vol(const R_3DPoint &origin, REAL_TYPE maxDistSq)
{
	int result = 0;
	REAL_TYPE distSq;
	REAL_TYPE f;

	distSq = distance_squared(origin,soundViewPoint);
	distSq *= world_scale;
	if (distSq < maxDistSq)
	{
		f = distSq / maxDistSq;
		result = (int) (100 - (100.0 * f));
	}
	return result;
}

void sound_set_lock(int lock)
{
	soundLock = lock;
}

void sound_set_active(int active)
{
	if (!active && soundActive)
		sound_off_all();
	soundActive = active;
}

int sound_get_active(void)
{
	return (sound_avail && soundActive);
}

void sound_toggle_active(void)
{
	soundActive = !soundActive;
	if (!soundActive)
		sound_off_all();
}

void sound_set_affiliation(int affiliation)
{
	soundAffiliation = affiliation;
}

int sound_get_affiliation(void)
{
	return soundAffiliation;
}

int sound_error_check(int err)
{
	if (err != SOUND_OK)
	{
		if (err == SOUND_NOT_FOUND)
			return err; //sim_printf("\"%s\" sound id not found\n", dsnd_search_id);
		else
			sim_printf("sound error: %s\n",
						soundErr2String(err));
	}
	return err;
}

char *soundErr2String(int err)
{
	switch (err)
	{
	case SOUND_OK:
		return "SOUND_OK";
		break;

	case SOUND_FULL:
		return "SOUND_FULL";
		break;

	case SOUND_ERROR_INIT:
		return "SOUND_ERROR_INIT";
		break;

	case SOUND_ERROR_CREATE:
		return "SOUND_ERROR_CREATE";
		break;

	case SOUND_ERROR_LOCK:
		return "SOUND_ERROR_LOCK";
		break;

	case SOUND_ERROR_UNLOCK:
		return "SOUND_ERROR_UNLOCK";
		break;

	case SOUND_NOT_FOUND:
		return "SOUND_NOT_FOUND";
		break;

	case SOUND_NOT_LOADED:
		return "SOUND_NOT_LOADED";
		break;

	case SOUND_ERROR_SETPOS:
		return "SOUND_ERROR_SETPOS";
		break;

	case SOUND_ERROR_PLAY:
		return "SOUND_ERROR_PLAY";
		break;

	case SOUND_ERROR_VOL:
		return "SOUND_ERROR_VOL";
		break;

	case SOUND_ERROR_FREQ:
		return "SOUND_ERROR_FREQ";
		break;

	case SOUND_ERROR_PAN:
		return "SOUND_ERROR_PAN";
		break;

	case WAV_PATH_NOT_FOUND:
		return "WAV_PATH_NOT_FOUND";
		break;

	case WAV_NO_SECTION:
		return "WAV_NO_SECTION";
		break;

	case WAV_NO_FORMAT:
		return "WAV_NO_FORMAT";
		break;

	case WAV_NO_DATA:
		return "WAV_NO_DATA";
		break;

	case WAV_BAD_FORMAT:
		return "WAV_BAD_FORMAT";
		break;

	case WAV_CANT_ASCEND:
		return "WAV_CANT_ASCEND";
		break;

	case WAV_NO_DATA_CHUNK:
		return "WAV_NO_DATA_CHUNK";
		break;
	}
	return ("unknown");
}

