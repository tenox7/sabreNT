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
 * File   : winkeys.cpp                          *
 * Date   : December, 1997                       *
 * Author : Dan Hammer                           *
 *************************************************/
#include <windows.h>
#include "sbrvkeys.h"
#include "winkeys.h"

/*
 * So Init() gets called, instantiate a keyboard
 */
static WinKeyboard theKeyboard;
int WinKeyboard::keyBuff[256];
int WinKeyboard::keyInIndex = 0;
int WinKeyboard::keyOutIndex = 0;
unsigned char WinKeyboard::keyboard[256];

/*
 *  Set up default key map
 */
typedef struct wvInitKey_struct
{
	int winVirtKey;
	int sabreVirtKey;
	int sabreVirtKeyShift;
	int sabreVirtKeyCtrl;
} wvInitKey;

wvInitKey wvInitKeyTab[] =
{ 
	{ VK_BACK,		FI_NEXT_WEAPON,			FI_NEXT_ANY,				FI_NO_KEY },
	{ VK_TAB,		FI_NEXT_VIEW_NODE,		FI_NO_KEY,					FI_NO_KEY },
	{ VK_CLEAR,		FI_NO_KEY,					FI_NO_KEY,					FI_NO_KEY },
	{ VK_RETURN,	FI_NEXT_ENEMY,			   FI_NEXT_WINGMAN,			FI_NEXT_FRIENDLY },
	{ VK_ESCAPE,	FI_EXIT_FLIGHT,			FI_EXIT_FLIGHT,			FI_EXIT_FLIGHT },
	{ VK_SPACE,		FI_BANG_BANG,				FI_AI_INFO_TOGGLE,		FI_NO_KEY },
	{ VK_HOME,		FI_PLAYER_VIEW_NODE,		FI_NO_KEY,					FI_NO_KEY },
	{ VK_LEFT,		FI_YOKE_LEFT,           FI_NO_KEY,					FI_NO_KEY },
	{ VK_RIGHT,		FI_YOKE_RIGHT,				FI_NO_KEY,					FI_NO_KEY },
	{ VK_UP,			FI_YOKE_UP,					FI_NO_KEY,					FI_NO_KEY },
	{ VK_DOWN,		FI_YOKE_DOWN,				FI_NO_KEY,					FI_NO_KEY },
	{ '0',			FI_VIEW_VIRTUAL,			FI_RZ_WHICHLINE,			FI_NO_KEY },
	{ '1',			FI_VIEW_FRONT,				FI_VIEW_FRONT_UP,			FI_VIEW_FRONT_DOWN },
	{ '2',			FI_VIEW_LEFT,				FI_VIEW_LEFT_UP,			FI_VIEW_LEFT_DOWN },
	{ '3',			FI_VIEW_RIGHT,				FI_VIEW_RIGHT_UP,			FI_VIEW_RIGHT_DOWN },
	{ '4',			FI_VIEW_REAR,				FI_VIEW_REAR_UP,			FI_VIEW_REAR_DOWN },
	{ '5',			FI_VIEW_SATELLITE,		FI_NO_KEY,					FI_NO_KEY },
	{ '6',			FI_VIEW_EXTERNAL,			FI_NO_KEY,					FI_NO_KEY },
	{ '7',			FI_VIEW_TRACK,				FI_VIEW_PADLOCK,			FI_NO_KEY },
	{ '8',			FI_VIEW_TARGET_TRACK,	FI_NO_KEY,					FI_NO_KEY },
	{ '9',			FI_VIEW_FLYBY,				FI_NO_KEY,					FI_NO_KEY },
	{ 'A',			FI_AUTOPILOT_TOGGLE,		FI_AUTOPILOT_TOGGLE,		FI_NO_KEY },
	{ 'B',			FI_SPEED_BRAKES_TOGGLE,	FI_SPEED_BRAKES_TOGGLE,	FI_NO_KEY },
	{ 'C',			FI_COCKPIT_TOGGLE,		FI_COCKPIT_TOGGLE,		FI_NO_KEY },
	{ 'F',			FI_FLAPS_TOGGLE,			FI_FLAPS_TOGGLE,				FI_WIREFRAME_TOGGLE },
	{ 'G',			FI_GEAR_TOGGLE,			FI_GEAR_TOGGLE,			FI_NO_KEY },
	{ 'H',			FI_HUD_TOGGLE,				FI_HUD_TOGGLE,				FI_NO_KEY },
	{ 'J',			FI_RUDDER_LEFT,			FI_RUDDER_LEFT,			FI_NO_KEY },
	{ 'K',			FI_RUDDER_CENTER,			FI_RUDDER_CENTER,			FI_NO_KEY },
	{ 'L',			FI_RUDDER_RIGHT,			FI_RUDDER_RIGHT,			FI_NO_KEY },
	{ 'O',			FI_YOKE_CENTER,			FI_YOKE_CENTER,			FI_NO_KEY },
	{ 'P',			FI_PAUSE_TOGGLE,			FI_PAUSE_TOGGLE,			FI_NO_KEY },
	{ 'Q',			FI_EXIT_FLIGHT,			FI_EXIT_FLIGHT,			FI_NO_KEY },
	{ 'R',			FI_RADAR_TOGGLE,			FI_RADAR_TOGGLE,			FI_NO_KEY },
	{ 'S',			FI_PLANEFRAME_TOGGLE,	FI_PLANEFRAME_TOGGLE,	FI_NO_KEY },
	{ 'T',			FI_YOKE_TRIM_UP,			FI_YOKE_TRIM_DOWN,		FI_YOKE_TRIM_CENTER },
	{ 'V',			FI_FLIGHTPATH_INDICATOR_TOGGLE,	FI_NO_KEY,		FI_NO_KEY },
	{ 'W',			FI_WHEEL_BRAKES_TOGGLE,	FI_WHEEL_BRAKES_TOGGLE,	FI_NO_KEY },
	{ 'X',			FI_WEAPON_ARM_TOGGLE,	FI_WEAPON_ARM_TOGGLE,	FI_NO_KEY },
	{ 'Z',			FI_THROTTLE_FULL,			FI_THROTTLE_ZERO,			FI_FRAMERATE_TOGGLE },
	{ VK_MULTIPLY,	FI_THROTTLE_FULL,			FI_THROTTLE_FULL,			FI_NO_KEY },
	{ VK_DIVIDE,	FI_THROTTLE_ZERO,			FI_THROTTLE_ZERO,			FI_NO_KEY },
	{ VK_ADD,		FI_THROTTLE_PLUS,			FI_NO_KEY,					FI_NO_KEY },
	{ VK_SUBTRACT, FI_THROTTLE_MINUS,		FI_NO_KEY,					FI_NO_KEY },
	{ VK_F2,			FI_VIEW_THETA_MINUS,		FI_NO_KEY,					FI_NO_KEY },
	{ VK_F3,			FI_VIEW_THETA_PLUS,		FI_NO_KEY,					FI_NO_KEY },
	{ VK_F4,			FI_VIEW_PHI_PLUS,			FI_NO_KEY,					FI_NO_KEY },
	{ VK_F5,			FI_VIEW_PHI_MINUS,		FI_NO_KEY,					FI_NO_KEY },
	{ VK_F6,			FI_VIEW_VIRTUAL_SNAP_FRONT,FI_NO_KEY,				FI_NO_KEY },
	{ VK_F7,			FI_EXTERN_VIEW_DISTANCE_PLUS,FI_NO_KEY,			FI_NO_KEY },
	{ VK_F8,			FI_EXTERN_VIEW_DISTANCE_MINUS,FI_NO_KEY,			FI_NO_KEY },
	{ VK_F11,		FI_DISPLAY_TEXTURED_GROUND_TOGGLE,FI_NO_KEY,		FI_NO_KEY },
	{ VK_F12,		FI_DISPLAY_CLOUDS_TOGGLE,FI_NO_KEY,					FI_NO_KEY },
	{ VK_NUMPAD8,	FI_VIEW_FRONT,				FI_VIEW_FRONT_UP,			FI_VIEW_FRONT_DOWN },
	{ VK_NUMPAD2,	FI_VIEW_REAR,				FI_VIEW_REAR_UP,			FI_VIEW_REAR_DOWN },
	{ VK_NUMPAD4,	FI_VIEW_LEFT,				FI_VIEW_LEFT_UP,			FI_VIEW_LEFT_DOWN },
	{ VK_NUMPAD6,	FI_VIEW_RIGHT,				FI_VIEW_RIGHT_UP,			FI_VIEW_RIGHT_DOWN },
	{ VK_NUMPAD7,	FI_VIEW_FRONT_LEFT,		FI_VIEW_FRONT_LEFT_UP,	FI_VIEW_FRONT_LEFT_DOWN },
	{ VK_NUMPAD9,	FI_VIEW_FRONT_RIGHT,		FI_VIEW_FRONT_RIGHT_UP,	FI_VIEW_FRONT_RIGHT_DOWN },
	{ VK_NUMPAD1,	FI_VIEW_REAR_LEFT,		FI_VIEW_REAR_LEFT_UP,	FI_VIEW_REAR_LEFT_DOWN },
	{ VK_NUMPAD3,	FI_VIEW_REAR_RIGHT,		FI_VIEW_REAR_RIGHT_UP,	FI_VIEW_REAR_RIGHT_DOWN },
	{	0,				0,							0,								0 }
};

typedef struct wvKey_struct
{
	int sabreVirtKey;
	int sabreVirtKeyShift;
	int sabreVirtKeyCtrl;
} wvKey;

wvKey wvKeyTab[256];

void WinKeyboard::Init(void)
{
	wvInitKey *initKey;
	int		  wVirtKey;

	for (int i=0;i<256;i++)
	{
		keyboard[i] = 0;
		wvKeyTab[i].sabreVirtKey = wvKeyTab[i].sabreVirtKeyShift = wvKeyTab[i].sabreVirtKeyCtrl =
			FI_NO_KEY;
	}
	Flush();
	initKey = wvInitKeyTab;
	wVirtKey = initKey->winVirtKey;
	while (wVirtKey != 0)
	{
		if (wVirtKey >= 0 && wVirtKey < 256)
		{
			wvKeyTab[wVirtKey].sabreVirtKey = initKey->sabreVirtKey;
			wvKeyTab[wVirtKey].sabreVirtKeyShift = initKey->sabreVirtKeyShift;
			wvKeyTab[wVirtKey].sabreVirtKeyCtrl = initKey->sabreVirtKeyCtrl;
		}
		initKey++;
		wVirtKey = initKey->winVirtKey;
	}
}

void WinKeyboard::Flush(void)
{
	keyInIndex = keyOutIndex = 0;
}

int WinKeyboard::WinVKeyIsDown(int winVKeyCode)
{
	int result = 0;
	if (winVKeyCode >= 0 && winVKeyCode < 256)
		result = keyboard[winVKeyCode];
	return result;
}

int WinKeyboard::GetNextSabreVKey(void)
{
	int result = FI_NO_KEY;

   if (keyInIndex != keyOutIndex)
   {
      result = keyBuff[keyOutIndex++];
      if ( keyOutIndex >= 256)
         keyOutIndex = 0;
   }
   return result;
}


void WinKeyboard::PushSabreVKey(int sabreVKey)
{
   keyBuff[keyInIndex++] = sabreVKey;
   if (keyInIndex >= 256)
      keyInIndex = 0;
}

extern int __cdecl sim_printf(const char *format, ...);

void WinKeyboard::PressWinVKey(int winVKeyCode)
{
	int sabreVKey;
	wvKey	*wvkey;

	if (winVKeyCode >= 0 && winVKeyCode < 256)
		keyboard[winVKeyCode] = 1;
	else
		return;

	wvkey = &wvKeyTab[winVKeyCode];
	
	// Let VK_NUMPAD5 act as shift key && VK_NUMPAD0 as ctrl key
	if (WinVKeyIsDown(VK_SHIFT) || WinVKeyIsDown(VK_NUMPAD5))
		sabreVKey = wvkey->sabreVirtKeyShift;
	else if (WinVKeyIsDown(VK_CONTROL) || WinVKeyIsDown(VK_NUMPAD0))
		sabreVKey = wvkey->sabreVirtKeyCtrl;
	else
		sabreVKey = wvkey->sabreVirtKey;

	if (sabreVKey != FI_NO_KEY)
		PushSabreVKey(sabreVKey);
}

void WinKeyboard::ReleaseWinVKeys(void)
{
	for (int i=0; i < 256; i++)
		ReleaseWinVKey(i);
}

void WinKeyboard::ReleaseWinVKey(int winVKeyCode)
{
	if (winVKeyCode >= 0 && winVKeyCode < 256)
		keyboard[winVKeyCode] = 0;
}


