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
#define WIN32_LEAN_AND_MEAN  
#include <windows.h> 
#include <windowsx.h> 
#include <mmsystem.h>
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
#include <ddraw.h>
#include <dsound.h>

#include "simerr.h"
#include "simfile.h"
#include "simsnd.h"
#include "winsndx.h"

#define DSVOLUME_TO_DB(volume) ((DWORD)(-30*(100 - volume)))

/******************************************************************
 * Direct Sound globs                                             *
 ******************************************************************/
LPDIRECTSOUND			lpds;								// directsound interface pointer
DSBUFFERDESC			dsbd;								// directsound description
DSCAPS					dscaps;							// directsound caps
HRESULT					dsresult;						// general directsound result
DSBCAPS					dsbcaps;							// directsound buffer caps
LPDIRECTSOUNDBUFFER	lpdsbprimary;					// the primary mixing buffer
DSound					sounds[DSND_MAX_SOUNDS];	// the array of secondary sound buffers

WAVEFORMATEX		pcmwf;          // generic waveformat structure
static int dsnd_inited = 0;
static HRESULT	ddresult;
static HWND hWnd;
static int sndCnt = 0;

static char *display_ddx_error(HRESULT err, char *s);
static char *dderr_2_string(HRESULT dderr);
static DSound *find_sound(const char *);
static DSound *find_avail_sound(void );

const char *dsnd_search_id = NULL;

/*****************************************************************
 * DSound class methods                                          *
 *****************************************************************/
void DSound::Init()
{
	if (IsLoaded())
	{
		Stop();
		Release();
	}
	idx = -1;
	memset(id,' ',sizeof(id));
	id[sizeof(id)-1] = 0;
}

void DSound::SetId(const char *newId)
{
	if (strlen(newId) < sizeof(id) - 1)
		strcpy(id,newId);
	else
	{
		memcpy(id,newId,sizeof(id)-1);
		id[sizeof(id)-1] = 0;
	}
}

/******************************************************************
 * Direct sound interface                                         *
 ******************************************************************/
int dsnd_init(HWND hwnd)
{
	hWnd = hwnd;
	// Need to init?
	if (!dsnd_inited)
	{		
		// reset first time
		dsnd_inited = 1;

		// create a directsound object
		if ((ddresult = DirectSoundCreate(NULL, &lpds, NULL)) != DS_OK )
		{
			display_ddx_error(ddresult,"DirectSoundCreate");
			return SOUND_ERROR_INIT;
		}
		// set cooperation level
		if ((ddresult = lpds->SetCooperativeLevel(hWnd,DSSCL_NORMAL)) != DS_OK)
		{
			display_ddx_error(ddresult,"DS::SetCooperativeLevel");
			return SOUND_ERROR_INIT;
		}
	}
	// Init the sounds
	for (int i=0;i<DSND_MAX_SOUNDS;i++)
		sounds[i].Init();
	sndCnt = 0;
	return SOUND_OK;
}

void dsnd_destroy(void)
{
	for (int i=0;i<sndCnt;i++)
		sounds[i].Release();
	sndCnt = 0;
	lpds->Release();
}

// #define SAVE_SND


/**************************************
 * Load a sound from a *.wav file     *
 **************************************/
int dsnd_load_wav(char *path, const char *id, int control_flags)
{
	DSound	*dsound;
	HMMIO hwav;    // handle to wave file
	MMCKINFO parent,  // parent chunk
            child;   // child chunk
	WAVEFORMATEX wfmtx;   // wave format structure
	UCHAR *snd_buffer,       // temporary sound buffer to hold voc data
         *audio_ptr_1=NULL, // data ptr to first write buffer 
	      *audio_ptr_2=NULL; // data ptr to second write buffer

	DWORD audio_length_1=0,  // length of first write buffer
	      audio_length_2=0;  // length of second write buffer

#ifdef SAVE_SND
	char *cptr;
	char *snd_path;
	FILE *snd_file;
	
	snd_path = strdup(path);
	cptr = strstr(snd_path,".wav");
	if (cptr)
	{
		memcpy(cptr+1,"snd",3);
		snd_file = fopen(snd_path,"wb");
	}
#endif
	// set up chunk info structure
	parent.ckid 	    = (FOURCC)0;
	parent.cksize 	    = 0;
	parent.fccType	    = (FOURCC)0;
	parent.dwDataOffset = 0;
	parent.dwFlags		= 0;

	// copy data
	child = parent;

	// open the WAV file
	if ((hwav = mmioOpen(path, NULL, MMIO_READ | MMIO_ALLOCBUF))==NULL)
		 return WAV_PATH_NOT_FOUND;

	// descend into the RIFF 
	parent.fccType = mmioFOURCC('W', 'A', 'V', 'E');

	if (mmioDescend(hwav, &parent, NULL, MMIO_FINDRIFF))
    {
		 // close the file
		mmioClose(hwav, 0);

		// return error, no wave section
		return WAV_NO_SECTION; 	
    }

	// descend to the WAVEfmt 
	child.ckid = mmioFOURCC('f', 'm', 't', ' ');

	if (mmioDescend(hwav, &child, &parent, 0))
    {
		 // close the file
		mmioClose(hwav, 0);
		// return error, no format section
		return WAV_NO_FORMAT; 	
    } 

	// now read the wave format information from file
	if (mmioRead(hwav, (char *)&wfmtx, sizeof(wfmtx)) != sizeof(wfmtx))
	 {
		 // close file
		 mmioClose(hwav, 0);
		 // return error, no wave format data
		 return WAV_NO_DATA;
	 } 

	// make sure that the data format is PCM
	if (wfmtx.wFormatTag != WAVE_FORMAT_PCM)
    {
		 // close the file
		mmioClose(hwav, 0);

		// return error, not the right data format
		return WAV_BAD_FORMAT; 
    }

	// now ascend up one level, so we can access data chunk
	if (mmioAscend(hwav, &child, 0))
   {
		// close file
		mmioClose(hwav, 0);
		// return error, couldn't ascend
		return WAV_CANT_ASCEND; 	
   }

	// descend to the data chunk 
	child.ckid = mmioFOURCC('d', 'a', 't', 'a');

	if (mmioDescend(hwav, &child, &parent, MMIO_FINDCHUNK))
    {
		 // close file
		mmioClose(hwav, 0);
		// return error, no data
		return WAV_NO_DATA_CHUNK; 	
    } 

	// finally!!!! now all we have to do is read the data in and
	// set up the directsound buffer

	// allocate the memory to load sound data
	snd_buffer = (UCHAR *)malloc(child.cksize);

	// read the wave data 
	mmioRead(hwav, (char *)snd_buffer, child.cksize);

#ifdef SAVE_SND
	if (snd_file)
	{
		fwrite(snd_buffer,1,child.cksize,snd_file);
		fclose(snd_file);
		free(snd_path);
	}
#endif
	// close the file
	mmioClose(hwav, 0);


	// set up the format data structure
	memset(&pcmwf, 0, sizeof(WAVEFORMATEX));

	pcmwf.wFormatTag = WAVE_FORMAT_PCM;  // pulse code modulation
	pcmwf.nChannels = 1;                // mono 
	pcmwf.nSamplesPerSec = 11025;            // always this rate
	pcmwf.nBlockAlign = 1;                
	pcmwf.nAvgBytesPerSec = pcmwf.nSamplesPerSec * pcmwf.nBlockAlign;
	pcmwf.wBitsPerSample = 8;
	pcmwf.cbSize = 0;

	// prepare to create sounds buffer
	dsbd.dwSize	= sizeof(DSBUFFERDESC);
	dsbd.dwFlags = control_flags | DSBCAPS_STATIC | DSBCAPS_LOCSOFTWARE;
	dsbd.dwBufferBytes = child.cksize;
	dsbd.lpwfxFormat = &pcmwf;

	// First check if room for new sound
	if (sndCnt < DSND_MAX_SOUNDS)
		dsound = &sounds[sndCnt++];
	else
	{
		dsound = find_avail_sound();
		if (dsound == NULL)
			return SOUND_FULL;
	}
	// create the sound buffer
	if (lpds->CreateSoundBuffer(&dsbd,&dsound->dsbuffer,NULL)!=DS_OK)
	{
		free(snd_buffer);
		// return error
		return SOUND_ERROR_CREATE;
	}


	// set id, rate and size in data structure
	dsound->SetId(id);
	dsound->rate  = wfmtx.nSamplesPerSec;
	dsound->size  = child.cksize;
	dsound->state = DSND_LOADED;

	// copy data into sound buffer
	if (dsound->dsbuffer->Lock(0,					 
								      child.cksize,			
								      (void **) &audio_ptr_1, 
								      &audio_length_1,
								      (void **)&audio_ptr_2, 
								      &audio_length_2,
								      DSBLOCK_FROMWRITECURSOR)!=DS_OK)
		 return SOUND_ERROR_LOCK;

	// copy first section of circular buffer
	memcpy(audio_ptr_1, snd_buffer, audio_length_1);

	// copy last section of circular buffer
	memcpy(audio_ptr_2, (snd_buffer+audio_length_1),audio_length_2);

	// unlock the buffer
	if (dsound->dsbuffer->Unlock(audio_ptr_1, 
											 audio_length_1, 
											 audio_ptr_2, 
											 audio_length_2)!=DS_OK)
 			return SOUND_ERROR_UNLOCK;

	// release the temp buffer
	free(snd_buffer);

	return SOUND_OK;
}

// flags can be 0 or DSBPLAY_LOOPING
int dsnd_play_sound(const char *id, int flags, int vol)
{
	if (flags == NORM || flags == TIMED)
		flags = 0;
	else if (flags == LOOP)
		flags = DSBPLAY_LOOPING;
	else
		flags = 0;

	DSound *dsound = find_sound(id);
	if (dsound)
	{
		if (dsound->IsLoaded())
		{
			DWORD statusFlags;
			dsound->dsbuffer->GetStatus(&statusFlags);
			if (!(statusFlags & DSBSTATUS_PLAYING))
			{
				if (dsound->dsbuffer->SetCurrentPosition(0) != DS_OK)
					return SOUND_ERROR_SETPOS;
				if (dsound->dsbuffer->Play(0,0,flags) != DS_OK)
					return SOUND_ERROR_PLAY;
				if (vol >= 0 && vol <= 100)
					if (dsound->dsbuffer->SetVolume(DSVOLUME_TO_DB(vol)) != DS_OK)
						return SOUND_ERROR_VOL;
				dsound->state = DSND_PLAYING;
			}
			else if (dsound->state == DSND_PLAYING && vol >= 0 && vol<= 100)
			{
				if (dsound->dsbuffer->SetVolume(DSVOLUME_TO_DB(vol)) != DS_OK)
					return SOUND_ERROR_VOL;
			}
		}
		else
			return SOUND_NOT_LOADED;
	}
	else
		return SOUND_NOT_FOUND;

	return SOUND_OK;
}

int dsnd_set_vol(const char *id, int vol)
{
	DSound *dsound = find_sound(id);
	if (dsound)
	{
		if (dsound->dsbuffer->SetVolume(DSVOLUME_TO_DB(vol)) != DS_OK)
			return SOUND_ERROR_VOL;
	}
	else
		return SOUND_NOT_FOUND;

	return SOUND_OK;
}

int dsnd_set_freq(const char *id, int freq)
{
	DSound *dsound = find_sound(id);
	if (dsound)
	{
		if (dsound->dsbuffer->SetFrequency(freq) != DS_OK)
			return SOUND_ERROR_FREQ;
	}
	else
		return SOUND_NOT_FOUND;

	return SOUND_OK;
}

int dsnd_set_pan(const char *id, int pan)
{
	DSound *dsound = find_sound(id);
	if (dsound)
	{
		if (dsound->dsbuffer->SetPan(pan) != DS_OK)
			return SOUND_ERROR_PAN;
	}
	else
		return SOUND_NOT_FOUND;
	return SOUND_OK;
}

int dsnd_stop_sound(const char *id)
{
	DSound *dsound = find_sound(id);
	if (dsound)
	{
		dsound->Stop();
	}
	else
		return SOUND_NOT_FOUND;

	return SOUND_OK;
}

int dsnd_stop_sounds(void)
{
	for (int i=0;i<sndCnt;i++)
		sounds[i].Stop();
	return SOUND_OK;
}

int dsnd_delete_sound(const char *id)
{
	DSound *dsound = find_sound(id);
	if (dsound)
	{
		dsound->Release();
		dsound->SetId(" ");
	}
	else
		return SOUND_NOT_FOUND;

	return SOUND_OK;

}

int dsnd_delete_all_sounds(void)
{
	for (int i=0;i<sndCnt;i++)
	{
		sounds[i].Release();
		sounds[i].SetId(" ");
	}
	sndCnt = 0;
	return SOUND_OK;
}

int dsnd_status_sound(const char *id, unsigned int *result)
{
	ULONG dresult = 0;
	DSound *dsound = find_sound(id);
	if (dsound && dsound->IsLoaded())
	{
		dsound->dsbuffer->GetStatus(&dresult);
		if (dresult & DSBSTATUS_PLAYING)
			*result |= SND_STATUS_PLAYING;
		if (dresult & DSBSTATUS_LOOPING)
			*result |= SND_STATUS_LOOPING;
	}
	else
		return SOUND_NOT_FOUND;
	return SOUND_OK;
}

/*************************************************************************
 * local static functions
 *************************************************************************/
DSound *find_sound(const char *id)
{
	dsnd_search_id = id;
	DSound *result = NULL;
	for (int i=0;i<sndCnt;i++)
	{
		if (sounds[i].IsMyId(id))
		{
			result = &sounds[i];
			break;
		}
	}
	return result;
}

DSound *find_avail_sound(void )
{
	DSound *result = NULL;
	for (int i=0;i<DSND_MAX_SOUNDS;i++)
	{
		if (sounds[i].state == DSND_NULL)
		{
			result = &sounds[i];
			break;
		}
	}
	return result;
}


char *display_ddx_error(HRESULT err, char *s)
{
	static char errmssg[256];
	if (s == NULL)
		s = "DDX Error";
	char *errString = dderr_2_string(err);
	sprintf(errmssg,"%s %s", s, errString);
	sim_printf("%s\n",errmssg);
	return errmssg;
}

char *dderr_2_string(HRESULT dderr)
{
	char *result = "Unknown";
	switch (dderr)
	{
	case DDERR_GENERIC :
		return "DDERR_GENERIC";
		break;

	case DDERR_INVALIDMODE :
		return "DDERR_INVALIDMODE";
		break;

	case DDERR_INVALIDOBJECT :
		return "DDERR_INVALIDOBJECT";
		break;

	case DDERR_INVALIDPARAMS :
		return "DDERR_INVALIDPARAMS";
		break;

	case DDERR_LOCKEDSURFACES :
		return "DDERR_LOCKEDSURFACES";
		break;

	case DDERR_NOEXCLUSIVEMODE :
		return "DDERR_NOEXCLUSIVEMODE";
		break;

	case DDERR_SURFACEBUSY :
		return "DDERR_SURFACEBUSY";
		break;

	case DDERR_UNSUPPORTED :
		return "DDERR_UNSUPPORTED";
		break;

	case DDERR_UNSUPPORTEDMODE :
		return "DDERR_UNSUPPORTEDMODE";
		break;

	case DDERR_WASSTILLDRAWING :
		return "DDERR_WASSTILLDRAWING";
		break;
	}
	return result;
}

