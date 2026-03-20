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
 * Version: 0.1                                  *
 * File   : mission.cpp                          *
 * Date   : December, 1997                       *
 * Author : Dan Hammer                           *
 * Port to Windows NT/95                         *
 *************************************************/
#include <windows.h>
#include "mission.h"

#define IDC_EDIT1	         101
#define IDC_EDIT2	         103
#define IDC_RADIOBUTTON2	104
#define IDC_RADIOBUTTON1	102

#define IDCTRL_FLIGHT_FILE IDC_EDIT1
#define IDCTRL_GROUND_FILE IDC_EDIT2
#define IDCTRL_RANDOM_CHECK IDC_RADIOBUTTON1
#define IDCTRL_DEMO_CHECK  IDC_RADIOBUTTON2

char dlg_flight_file[512];
char dlg_ground_file[512];
int  dlg_random = 0;
int  dlg_demo = 0;
int  dlg_init = 1;

extern char *flight_file;
extern char *ground_file;
extern int  demo;
extern int  do_random;

void doInitDialog(HWND);
void doEndDialogOK(HWND);

BOOL CALLBACK MissionDialogProc(HWND hdlg, UINT msg, WPARAM wparam,
                              LPARAM)
{

   switch (msg)
   {
      case WM_INITDIALOG:
      doInitDialog(hdlg);
      return (TRUE);

      case WM_COMMAND:

      switch (wparam)
      {
         case IDOK:
         doEndDialogOK(hdlg);
         EndDialog(hdlg,1);
         return (TRUE);

         case IDCANCEL:
         EndDialog(hdlg,0);
         return (TRUE);
      }
   }

   return (FALSE);
}

void doInitDialog(HWND hdlg)
{
 if (dlg_init)
 {
   if (flight_file != NULL)
      strcpy(dlg_flight_file,flight_file);
   else
     dlg_flight_file[0] = 0;
   if (ground_file != NULL)
     strcpy(dlg_ground_file,ground_file);
   else
     dlg_ground_file[0] = 0;
  dlg_demo = demo;
  dlg_random = do_random;
  dlg_init = 0;
 }

 SendDlgItemMessage(hdlg,IDCTRL_FLIGHT_FILE,WM_SETTEXT,
                    (WPARAM) 0,(LPARAM) dlg_flight_file);
 SendDlgItemMessage(hdlg,IDCTRL_GROUND_FILE,WM_SETTEXT,
                    (WPARAM) 0,(LPARAM) dlg_ground_file);
 SendDlgItemMessage(hdlg,IDCTRL_RANDOM_CHECK,BM_SETCHECK,
                    (WPARAM) dlg_random,(LPARAM)0);
 SendDlgItemMessage(hdlg,IDCTRL_DEMO_CHECK,BM_SETCHECK,
                    (WPARAM) dlg_demo,(LPARAM)0);
}

void doEndDialogOK(HWND hdlg)
{
 SendDlgItemMessage(hdlg,IDCTRL_FLIGHT_FILE,WM_GETTEXT,
                    (WPARAM) sizeof(dlg_flight_file),
                    (LPARAM) dlg_flight_file);
 SendDlgItemMessage(hdlg,IDCTRL_GROUND_FILE,WM_GETTEXT,
                    (WPARAM) sizeof(dlg_ground_file),
                    (LPARAM) dlg_ground_file);
 dlg_random = SendDlgItemMessage(hdlg,IDCTRL_RANDOM_CHECK,BM_GETCHECK,
                    (WPARAM) 0,(LPARAM)0);
 dlg_demo = SendDlgItemMessage(hdlg,IDCTRL_DEMO_CHECK,BM_GETCHECK,
                    (WPARAM) 0,(LPARAM)0);

}

