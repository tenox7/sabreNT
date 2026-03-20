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
 * File   : winsndx.h                            *
 * Direct sound                                  *
 * Date   : December, 1998                       *
 * Author : Dan Hammer                           *
 *************************************************/
#ifndef __winsndx_h
#define __winsndx_h

#define DSND_MAX_SOUNDS     64  
#define DSND_MAX_ID			 32

enum {
			DSND_NULL,
			DSND_LOADED,
			DSND_PLAYING,
			DSND_STOPPED
};

// voc file defines
#define DSND_VBLOCK_SIZE       6  // size of new voice block in bytes
#define DSND_DSVOL_TO_DB(volume) ((DWORD)(-30*(100 - volume)))

class DSound
{
public:
	LPDIRECTSOUNDBUFFER dsbuffer;   // the ds buffer containing the sound
	int state;                      // state of the sound
	int rate;                       // playback rate
	int size;                       // size of sound
	int idx;                        // idx number of the sound
	char id[DSND_MAX_ID];			  // id of sound

	DSound()
	{
		dsbuffer = NULL;
		state = DSND_NULL;
		rate = 0;
		size = 0;
		idx = -1;
		memset(id,' ',sizeof(id));
		id[sizeof(id)-1] = 0;
	}

	void Init(void);
	void Stop(void)
	{
		if (dsbuffer)
		{
			dsbuffer->Stop();
			dsbuffer->SetCurrentPosition(0);
			state = DSND_STOPPED;
		}
	}
	void Release(void)
	{
		if (dsbuffer)
			dsbuffer->Release();
		dsbuffer = NULL;
		state = DSND_NULL;
	}
	int IsLoaded(void)
	{
		return dsbuffer != NULL;
	}
	void SetId(const char *id);
	int IsMyId(const char *id)
	{
		return (!strcmp(this->id,id));
	}
} ;

// sound
int dsnd_init(HWND hWnd);
void dsnd_destroy(void);
//int dsnd_load_wav(char *path, const char *id, int control_flags = DSBCAPS_CTRLDEFAULT);
int dsnd_load_wav(char *path, const char *id, int control_flags = 0);

/* int dsnd_replicate_sound(int source_id); */
int dsnd_play_sound(const char *id, int flags, int vol = -1);
int dsnd_set_vol(const char *id, int vol);
int dsnd_set_freq(const char *id, int freq);
int dsnd_set_pan(const char *id, int pan);
int dsnd_stop_sound(const char *id);
int dsnd_stop_sounds(void);
int dsnd_delete_sound(const char *id);
int dsnd_delete_all_sounds(void);
int dsnd_status_sound(const char *id, unsigned int *result);
extern const char *dsnd_search_id;
#endif
