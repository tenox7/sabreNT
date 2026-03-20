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
 * File   : wincntrl.cpp                         *
 * Date   : December, 1998                       *
 * Author : Dan Hammer                           *
 *************************************************/
#include <windows.h>
#include "resource.h"
#include "wincntrl.h"

int wc_joystick_avail = 0;
int wc_throttle_avail = 0;
int wc_rudder_pedals_avail = 0;

int wc_yoke_device = wcMOUSE_DEVICE;
int wc_throttle_device = wcKEY_DEVICE;
int wc_rudder_device = wcKEY_DEVICE;

static int wc_yoke_device_temp;
static int wc_throttle_device_temp;
static int wc_rudder_device_temp;

static void DoInitDialog(HWND);
static void DoEndDialogOK(HWND);
void CheckYokeDevices(HWND);
void CheckThrottleDevices(HWND);
void CheckRudderDevices(HWND);

BOOL CALLBACK FlightControlsDialogProc(HWND hdlg, UINT msg, WPARAM wparam,
                              LPARAM)
{
	switch (msg)
	{
	case WM_INITDIALOG:
		DoInitDialog(hdlg);
		return TRUE;

	case WM_COMMAND:
		switch (wparam)
		{
		case IDOK:
			DoEndDialogOK(hdlg);
			EndDialog(hdlg,1);
			return TRUE;

		case IDCANCEL:
			EndDialog(hdlg,0);
			return TRUE;

		case IDC_YDJOY:
			wc_yoke_device_temp = wcJOY_DEVICE;
			CheckYokeDevices(hdlg);
			if (wc_throttle_avail)
			{
				wc_throttle_device_temp = wcJOY_DEVICE;
				CheckThrottleDevices(hdlg);
			}
			if (wc_rudder_pedals_avail)
			{
				wc_rudder_device_temp = wcJOY_DEVICE;
				CheckRudderDevices(hdlg);
			}
			return TRUE;

		case IDC_YDMOU:
			wc_yoke_device_temp = wcMOUSE_DEVICE;
			wc_throttle_device_temp = wcKEY_DEVICE;
			wc_rudder_device_temp = wcKEY_DEVICE;
			CheckYokeDevices(hdlg);
			CheckThrottleDevices(hdlg);
			CheckRudderDevices(hdlg);
			return TRUE;

		case IDC_YDKEY:
			wc_yoke_device_temp = wcKEY_DEVICE;
			wc_throttle_device_temp = wcKEY_DEVICE;
			wc_rudder_device_temp = wcKEY_DEVICE;
			CheckYokeDevices(hdlg);
			CheckThrottleDevices(hdlg);
			CheckRudderDevices(hdlg);
			return TRUE;

		case IDC_TDJOY:
			if (wc_yoke_device_temp == wcJOY_DEVICE)
			{
				wc_throttle_device_temp = wcJOY_DEVICE;
				CheckThrottleDevices(hdlg);
			}
			return TRUE;

		case IDC_TDKEY:
			wc_throttle_device_temp = wcKEY_DEVICE;
			CheckThrottleDevices(hdlg);
			return TRUE;

		case IDC_RDJOY:
			if (wc_yoke_device_temp == wcJOY_DEVICE)
			{
				wc_rudder_device_temp = wcJOY_DEVICE;
				CheckRudderDevices(hdlg);
			}
			return TRUE;

		case IDC_RDKEY:
			wc_rudder_device_temp = wcKEY_DEVICE;
			CheckRudderDevices(hdlg);
			return TRUE;
		}
	}

   return (FALSE);
}

void DoInitDialog(HWND hdlg)
{
	if (wc_joystick_avail)
		SendDlgItemMessage(hdlg,IDC_YDJOY,WM_ENABLE,(WPARAM)TRUE,(LPARAM)0);
	else
		SendDlgItemMessage(hdlg,IDC_YDJOY,WM_ENABLE,(WPARAM)FALSE,(LPARAM)0);
	if (wc_throttle_avail)
		SendDlgItemMessage(hdlg,IDC_TDJOY,WM_ENABLE,(WPARAM)TRUE,(LPARAM)0);
	else
		SendDlgItemMessage(hdlg,IDC_TDJOY,WM_ENABLE,(WPARAM)FALSE,(LPARAM)0);
	if (wc_rudder_pedals_avail)
		SendDlgItemMessage(hdlg,IDC_YDJOY,WM_ENABLE,(WPARAM)TRUE,(LPARAM)0);
	else
		SendDlgItemMessage(hdlg,IDC_YDJOY,WM_ENABLE,(WPARAM)FALSE,(LPARAM)0);

	wc_yoke_device_temp = wc_yoke_device;
	wc_throttle_device_temp = wc_throttle_device;
	wc_rudder_device_temp = wc_rudder_device;

	CheckYokeDevices(hdlg);
	CheckThrottleDevices(hdlg);
	CheckRudderDevices(hdlg);
}

void CheckYokeDevices(HWND hdlg)
{
	SendDlgItemMessage(hdlg,IDC_YDJOY,BM_SETCHECK,(WPARAM)FALSE,(LPARAM)0);
	SendDlgItemMessage(hdlg,IDC_YDMOU,BM_SETCHECK,(WPARAM)FALSE,(LPARAM)0);
	SendDlgItemMessage(hdlg,IDC_YDKEY,BM_SETCHECK,(WPARAM)FALSE,(LPARAM)0);

	switch (wc_yoke_device_temp)
	{
	case wcJOY_DEVICE:
		SendDlgItemMessage(hdlg,IDC_YDJOY,BM_SETCHECK,(WPARAM)TRUE,(LPARAM)0);
		break;

	case wcMOUSE_DEVICE:
		SendDlgItemMessage(hdlg,IDC_YDMOU,BM_SETCHECK,(WPARAM)TRUE,(LPARAM)0);
		break;

	case wcKEY_DEVICE:
		SendDlgItemMessage(hdlg,IDC_YDKEY,BM_SETCHECK,(WPARAM)TRUE,(LPARAM)0);
		break;
	}
}

void CheckThrottleDevices(HWND hdlg)
{
	SendDlgItemMessage(hdlg,IDC_TDJOY,BM_SETCHECK,(WPARAM)FALSE,(LPARAM)0);
	SendDlgItemMessage(hdlg,IDC_TDKEY,BM_SETCHECK,(WPARAM)FALSE,(LPARAM)0);

	switch (wc_throttle_device_temp)
	{
	case wcJOY_DEVICE:
		SendDlgItemMessage(hdlg,IDC_TDJOY,BM_SETCHECK,(WPARAM)TRUE,(LPARAM)0);
		break;

	case wcKEY_DEVICE:
		SendDlgItemMessage(hdlg,IDC_TDKEY,BM_SETCHECK,(WPARAM)TRUE,(LPARAM)0);
		break;
	}
}

void CheckRudderDevices(HWND hdlg)
{
	SendDlgItemMessage(hdlg,IDC_RDJOY,BM_SETCHECK,(WPARAM)FALSE,(LPARAM)0);
	SendDlgItemMessage(hdlg,IDC_RDKEY,BM_SETCHECK,(WPARAM)FALSE,(LPARAM)0);

	switch (wc_rudder_device_temp)
	{
	case wcJOY_DEVICE:
		SendDlgItemMessage(hdlg,IDC_RDJOY,BM_SETCHECK,(WPARAM)TRUE,(LPARAM)0);
		break;

	case wcKEY_DEVICE:
		SendDlgItemMessage(hdlg,IDC_RDKEY,BM_SETCHECK,(WPARAM)TRUE,(LPARAM)0);
		break;
	}
}


void DoEndDialogOK(HWND hdlg)
{
	wc_yoke_device = wc_yoke_device_temp;
	wc_throttle_device = wc_throttle_device_temp;
	wc_rudder_device = wc_rudder_device_temp;
}

