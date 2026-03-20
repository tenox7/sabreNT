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
 * File   : wincntrl.h                           *
 * Date   : December, 1998                       *
 * Author : Dan Hammer                           *
 * Port to Windows NT/95                         *
 *************************************************/
#ifndef __wincntrl_h
#define __wincntrl_h

#define wcJOY_DEVICE      0
#define wcMOUSE_DEVICE    1
#define wcKEY_DEVICE      2

BOOL CALLBACK FlightControlsDialogProc(HWND, UINT, WPARAM, LPARAM);

extern int wc_joystick_avail;
extern int wc_throttle_avail;
extern int wc_rudder_pedals_avail;

extern int wc_yoke_device;
extern int wc_throttle_device;
extern int wc_rudder_device;

#endif