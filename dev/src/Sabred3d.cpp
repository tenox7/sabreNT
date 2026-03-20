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
 * File   : sabred3d.cpp                         *
 * Date   : December, 1997                       *
 * Author : Dan Hammer                           *
 * Port to D3D                                   *
 *************************************************/
#define THISVER "Version 0.2.4a.d3d " ## __DATE__
const char *wVERSION = THISVER;

//#define STRICT
#include <windows.h>
#include <mmsystem.h>
#include <stdio.h>
#include "D3DFrame.h"
#include "D3DEnum.h"
#include "D3DUtil.h"

#include <stdlib.h>
#include <dos.h>
#include <math.h>
#include <time.h>
#include <stdarg.h>
#include <signal.h>
#include <iostream.h>
#include "wvga_13.h"
#include "resource.h"
#include "siminc.h"
#include "simsnd.h"
#include "scnedit.h"
#include "game.h"
#include "about.h"
#include "textures.h"
#include "mission.h"
#include "scenario.h"
#include "winkeys.h"
#include "wincntrl.h"

//-----------------------------------------------------------------------------
// Global WIN32 variables
//-----------------------------------------------------------------------------
HINSTANCE	   g_hInstance;	/* application instance */
HWND				g_hWnd;			/* handle to main window */
HMENU				g_hMenu;			/* handle to main windows' menu */

//-----------------------------------------------------------------------------
// Global variables for using the D3D sample framework class
//-----------------------------------------------------------------------------
CD3DFramework* g_pFramework        = NULL;
BOOL           g_bActive           = FALSE;
BOOL           g_bReady            = FALSE;
BOOL           g_bFrameMoving      = TRUE;
BOOL           g_bSingleStep       = FALSE;
BOOL           g_bWindowed         = TRUE;
BOOL           g_bShowStats        = TRUE;
RECT           g_rcWindow;
DWORD          g_dwBaseTime;
DWORD          g_dwPauseTime;
extern BOOL    g_bAppUseZBuffer;
extern BOOL    g_bAppUseBackBuffer;
extern TCHAR*  g_strAppTitle;

enum APPMSGTYPE { MSG_NONE, MSGERR_APPMUSTEXIT, MSGWARN_SWITCHTOSOFTWARE };

//-----------------------------------------------------------------------------
// Local function-prototypes
//-----------------------------------------------------------------------------
LRESULT CALLBACK WndProc ( HWND hWnd, unsigned int iMessage, unsigned int wParam, LONG lParam );
HRESULT Initialize3DEnvironment( HWND );
HRESULT Change3DEnvironment( HWND );
HRESULT Render3DEnvironment();
VOID    Cleanup3DEnvironment();
VOID    DisplayFrameworkError( HRESULT, APPMSGTYPE );
VOID    AppShowStats();
VOID    AppOutputText( LPDIRECT3DDEVICE3, DWORD, DWORD, CHAR* );
VOID    AppPause( BOOL );

VOID    ReadInitFile(VOID);
void InitSabreGraphics();
int MessageHook();
void doEdit();
void handleMenuCommand(WORD command);
void __cdecl float_error_handler(int,int,int*);
void updateGUI();
void ShowMouse();
void HideMouse();
void setMouseYoke(int);
void pauseGame(int);
void handleDemoAuto();
void resizeApp(int w, int h);
void setDevices(void);


//-----------------------------------------------------------------------------
// External function-prototypes
//-----------------------------------------------------------------------------
HRESULT wvga13dx_confirm_device( DDCAPS*, D3DDEVICEDESC* );
HRESULT wvga13dx_one_time_scene_init( HWND );
VOID    App_DeleteDeviceObjects( LPDIRECT3DDEVICE3, LPDIRECT3DVIEWPORT3 );
HRESULT wvga13dx_init_device_objects( LPDIRECT3DDEVICE3, LPDIRECT3DVIEWPORT3 );
HRESULT App_FrameMove( LPDIRECT3DDEVICE3, FLOAT );
HRESULT App_Render( LPDIRECT3DDEVICE3, LPDIRECT3DVIEWPORT3, D3DRECT* );
HRESULT App_RestoreSurfaces();
HRESULT wvga13dx_final_cleanup( LPDIRECT3DDEVICE3, LPDIRECT3DVIEWPORT3 );



MSG Message;
WNDCLASS WndClass;
POINT curPos;

int ini_screen_width = -1;
int ini_screen_height = -1;


SceneEdit		*the_edit = NULL;

char *palette_file = "a.plt";
char *world_file   = "a.wld";
char *texture_file = "a.txr";
char *hud_file    = "a.hud";
char *cpk_file    = "a.cpk";
char *snd_file		= "a.wvs";

char *flight_file = NULL;
char *ground_file = NULL;
char init_flight_file[256];
char init_ground_file[256];
char init_scenario_file[256];

char *log_path    = "sabrewin.log";
char *scenario_path = "sabrewin.sns";

int  cmd   = 0;


int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow )
{

	g_hInstance = hInstance;
   // Try to open up a log
	if ((simlog = fopen(log_path,"w")) == NULL)
      simlog = stdout;
	srand(time(NULL));

	ReadInitFile();
	ini_screen_width = 300;
	ini_screen_height = 300;

	if (!hPrevInstance)
	{
		WndClass.style = CS_HREDRAW | CS_VREDRAW;
		WndClass.lpfnWndProc = WndProc;
		WndClass.cbClsExtra = 0;
		WndClass.cbWndExtra = 0;
		WndClass.hInstance = hInstance;
		WndClass.hIcon = LoadIcon ( hInstance, NULL );
		WndClass.hbrBackground = GetStockObject(WHITE_BRUSH);
		WndClass.lpszClassName = "SABREW";
		WndClass.lpszMenuName = (const char *)MENU_1;
		WndClass.hCursor =  LoadCursor ( NULL, IDC_ARROW );
		if ( !RegisterClass ( &WndClass ) )
			return 0;
	}


	g_hWnd = CreateWindow("SABREW",             // class name
								"Sabre Fighter Plane Simulator",  // Caption
								WS_OVERLAPPEDWINDOW,
								CW_USEDDEFAULT, CW_USEDEFAULT,
								ini_screen_width,
								ini_screen_height,
								NULL,
								NULL,
								hInstance,
								NULL );
	ShowWindow( g_hWnd, SW_SHOWNORMAL );
	UpdateWindow( g_hWnd );
	g_hMenu = GetMenu(g_hWnd);
	// Save the window size/pos for switching modes
	GetWindowRect(g_hWnd, &g_rcWindow);
	// Initialize remnants of home-grown graphic engine
   InitSabreGraphics();	

	// Enumerate available D3D devices, passing a callback that allows devices
	// to be accepted/rejected based on what capabilities the app requires.
	HRESULT hr;
	if( FAILED( hr = D3DEnum_EnumerateDevices(wvga13dx_confirm_device) ) )
	{
	  DisplayFrameworkError( hr, MSGERR_APPMUSTEXIT );
	  return 0;
	}

	// Check if we could not get a device that renders into a window, which
	// means the display must be 16- or 256-color mode. If so, let's bail.
	D3DEnum_DriverInfo* pDriverInfo;
	D3DEnum_DeviceInfo* pDeviceInfo;
	D3DEnum_GetSelectedDriver( &pDriverInfo, &pDeviceInfo );
	if( FALSE == pDeviceInfo->bWindowed )
	{
	  Cleanup3DEnvironment();
	  DisplayFrameworkError( D3DFWERR_INVALIDMODE, MSGERR_APPMUSTEXIT );
	  return 0;
	}

	// Initialize the 3D environment for the app
	if( FAILED( hr = Initialize3DEnvironment( g_hWnd ) ) )
	{
		Cleanup3DEnvironment();
		DisplayFrameworkError( hr, MSGERR_APPMUSTEXIT );
		return 0;
	}

   while (running)
   {
		doEdit();
         cmd = 0;
		//     MessageHook();
   }

  sim_printf("exiting app\n");
  return Message.wParam;

 }

void InitSabreGraphics()
{
	curpen->color = 2;
	curpen->position = Point(0,0);
	curpen->width = 1;
	wvga_params(ini_screen_width, ini_screen_height);
	sim_printf("Initing edge bounds\n");
	init_edge_bounds();
	sim_printf("Initing z buffer\n");
	tr_init();
	sim_printf("Initing Port_3d\n");
	Port_3D::initPort3D();
	sim_printf("Initing font\n");
	g_font = new Font6x6("simfnt6");
	g_font->put_width = 6;
	MYCHECK(g_font != NULL);
	sim_printf("Reading textures\n");
	read_texture_file(texture_file);
}


void doEdit()
{
	the_edit = new SceneEdit(
					world_file,
					&mouse,
					MessageHook);
  setFlightMenuItems(FALSE);
  the_edit->doEdit();
  delete the_edit;
  the_game = NULL;
}

int MessageHook()
{
   if ( PeekMessage ( &Message, hWnd, 0, 0, PM_NOREMOVE ) == TRUE )
   {
      if (GetMessage ( &Message, hWnd, 0, 0 ))
      {
          TranslateMessage ( &Message );
          DispatchMessage ( &Message );
      }
       else
         running = 0;
   }
	
   updateGUI();

	int newKey = WinKeyboard::GetNextSabreVKey();
	if (newKey != FI_NO_KEY)
	{
		if (newKey == FI_EXIT_FLIGHT)
			running = 0;
		FlightInput::SetKeybdin(newKey);
	}
   return (running && !cmd);
}

LRESULT CALLBACK WndProc ( HWND hWnd, unsigned int iMessage,unsigned int wParam, LONG lParam )
{
   float XPos, YPos;
   static int wmsize_flag = 0;
   switch ( iMessage )
	{

	case WM_SIZE:
		if (!wmsize_flag)
			wmsize_flag = 1;
		else
			resizeApp(LOWORD(lParam),HIWORD(lParam));
		break;

	case WM_COMMAND:
		handleMenuCommand(LOWORD(wParam));
		break;

	case WM_LBUTTONUP:
		mouse.buttons = 0;
		break;

	case WM_LBUTTONDOWN:
		mouse.buttons = 1;
		break;

	case WM_RBUTTONUP:
		break;

	case WM_RBUTTONDOWN:
		pauseGame(gpause == 0);
		break;

	case WM_MOUSEMOVE:
		if (mouse_Yoke)
		{
			XPos = float ( ( short int ) ( LOWORD ( lParam ) ) );
			YPos = float ( ( short int ) ( HIWORD ( lParam ) ) );
			if (XPos > MAX_X)
				XPos = MAX_X;
			else if (XPos < 0)
				XPos = 0;
			if (YPos > MAX_Y)
				YPos = MAX_Y;
			else if (YPos < 0)
				YPos = 0;

			mouse.x = (XPos / MAX_X - 0.5) * 2.0;
			mouse.y = (YPos / MAX_Y - 0.5) * 2.0;
		}
		break;

	case WM_DESTROY:
		running = FALSE;
		break;

	case WM_KEYDOWN:
		WinKeyboard::PressWinVKey(wParam);
		break;

	case WM_KEYUP:
		WinKeyboard::ReleaseWinVKey(wParam);
		break;

#ifndef USES_DDRAW
	case WM_PAINT:
		blit_buff();
		break;
#endif

	case WM_ACTIVATEAPP:
		app_active = (BOOL)wParam;
		if (app_active == FALSE)
		{
			pause_state = gpause;
			pauseGame(1);
		}
		else
		{
			if (pause_state == 0)
				pauseGame(0);
		}
		if (wvga_wants_activate_message())
			wvga_activate(app_active);
		break;

#ifdef USES_DDRAW
    case WM_ENTERMENULOOP:
      pauseGame(1);
      break;

    case WM_EXITMENULOOP:
      pauseGame(0);
      break;
#endif

	default:
		return DefWindowProc ( hWnd, iMessage, wParam,
										  lParam );
	}
	return 0;
}



void handleMenuCommand(WORD command)
{
   switch (command)
   {

	case CM_EXIT:
     	running = FALSE;
  		break;

	case CM_SABRENEXT_AIRCRAFT:
		FlightInput::SetKeybdin(FI_NEXT_VIEW_NODE);
      break;

	case CM_CONTROLS0THROTTLE:
		FlightInput::SetKeybdin(FI_THROTTLE_ZERO);
      break;

	case CM_CONTROLS_FULL_THROTTLE:
		FlightInput::SetKeybdin(FI_THROTTLE_FULL);
      break;

	case CM_CONTROLWBRAKES:
		FlightInput::SetKeybdin(FI_WHEEL_BRAKES_TOGGLE);
      break;

	case CM_CONTROLSBRAKES:
		FlightInput::SetKeybdin(FI_SPEED_BRAKES_TOGGLE);
		break;

	case CM_CONTROLS_GEAR:
		FlightInput::SetKeybdin(FI_GEAR_TOGGLE);
      break;

	case CM_SABREGUNS:
		FlightInput::SetKeybdin(FI_WEAPON_ARM_TOGGLE);
      break;

	case CM_NEXT_WEAPON:
		FlightInput::SetKeybdin(FI_NEXT_WEAPON);
      break;

	case CM_SABREAUTO:
      if (demo)
         handleDemoAuto();
      else
			FlightInput::SetKeybdin(FI_AUTOPILOT_TOGGLE);
      break;

	case CM_VIEW_FLYBY:
		FlightInput::SetKeybdin(FI_VIEW_FLYBY);
		break;

	case CM_VIEW_COMBAT_TRACK_II:
		FlightInput::SetKeybdin(FI_VIEW_TARGET_TRACK);
      break;

	case CM_VIEW_COMBAT_TRACK:
		FlightInput::SetKeybdin(FI_VIEW_TRACK);
      break;

	case CM_VIEW_OUTSIDE:
		FlightInput::SetKeybdin(FI_VIEW_EXTERNAL);
      break;

	case CM_VIEW_TOP:
		FlightInput::SetKeybdin(FI_VIEW_SATELLITE);
      break;

	case CM_VIEW_BACK:
		FlightInput::SetKeybdin(FI_VIEW_REAR);
      break;

	case CM_VIEW_RIGHT:
		FlightInput::SetKeybdin(FI_VIEW_RIGHT);
      break;

	case CM_VIEW_LEFT:
		FlightInput::SetKeybdin(FI_VIEW_LEFT);
      break;

	case CM_VIEW_FRONT:
		FlightInput::SetKeybdin(FI_VIEW_FRONT);
      break;

	case CM_VIEW_VIRTUAL:
		FlightInput::SetKeybdin(FI_VIEW_VIRTUAL);
		break;

	case CM_VIEW_PADLOCK:
		FlightInput::SetKeybdin(FI_VIEW_PADLOCK);
		break;

	case CM_SCNTRRN:
      display_flags ^= TERRAIN_ON;
       CheckMenuItem(hmenu,CM_SCNTRRN,MF_BYCOMMAND |
                        (display_flags & TERRAIN_ON ?
                         MF_CHECKED : MF_UNCHECKED));

      break;

	case CM_SCNCLOUDS:
      display_flags ^= CLOUDS_ON;
      CheckMenuItem(hmenu,CM_SCNCLOUDS,MF_BYCOMMAND |
                    (display_flags & CLOUDS_ON ?
                         MF_CHECKED : MF_UNCHECKED));
      break;

	case CM_SABREC_COCKPIT:
		FlightInput::SetKeybdin(FI_COCKPIT_TOGGLE);
      break;

	case CM_HUD:
		FlightInput::SetKeybdin(FI_HUD_TOGGLE);
		break;

	case CM_FLIGHTPATH:
		FlightInput::SetKeybdin(FI_FLIGHTPATH_INDICATOR_TOGGLE);
		break;

	case CM_AIRFRAME:
		FlightInput::SetKeybdin(FI_PLANEFRAME_TOGGLE);
		break;

	case CM_SOUND:
		sound_toggle_active();
		sound_active = sound_get_active();
		break;

	case CM_SABREP_PAUSE:
      if (gpause != 0)
         pauseGame(0);
      else
         pauseGame(1);
      return;


	case CM_ABOUT_SABRE:
			pauseGame(1);
			DialogBox(hinstance,MAKEINTRESOURCE(ABOUT_DLG),hWnd,
                  (DLGPROC) AboutDialogProc);
			pauseGame(0);
      break;

#ifndef USES_DDRAW
	case CM_TEXTURES:
		DialogBox(hinstance,MAKEINTRESOURCE(TEXTURES_DLG),hWnd,
					(DLGPROC) TexturesDialogProc);
		break;
#endif

	case CM_FLIGHTCUSTM:
			pauseGame(1);
			if (DialogBox(hinstance,MAKEINTRESOURCE(MISSION_DLG),hWnd,
							(DLGPROC) MissionDialogProc))
			{
				if (strlen(dlg_flight_file) > 0)
				{
					flight_file = dlg_flight_file;
					if (strlen(dlg_ground_file) > 0 )
						ground_file = dlg_ground_file;
					else
						ground_file = NULL;
					demo = dlg_demo;
					do_random = dlg_random;
					cmd = CM_NEWGAME;
					pauseGame(0);
					return;
				}
			}
			pauseGame(0);
      break;

	case CM_DEVICES:
		pauseGame(1);
		DialogBox(hinstance,MAKEINTRESOURCE(FLTCNTRL_DLG),hWnd,
						(DLGPROC) FlightControlsDialogProc);
		setDevices();
		pauseGame(0);
		break;

	case CM_SABRE_HELP:
      ::WinHelp(hWnd,"SABRE.HLP",HELP_CONTENTS,0L);
      break;

	default:
      cmd = command;
      break;

   }
   if (gpause)
   {
      gpause = 0;
		if (wc_yoke_device == wcMOUSE_DEVICE)
		{
			setMouseYoke(1);
			SetCursorPos(curPos.x,curPos.y);
		}
   }
}

int view2cmd[11][2] =
{
   { fv_front, CM_VIEW_FRONT },
   { fv_left,  CM_VIEW_LEFT },
   { fv_right, CM_VIEW_RIGHT },
   { fv_rear,  CM_VIEW_BACK },
   { fv_satellite, CM_VIEW_TOP },
   { fv_external, CM_VIEW_OUTSIDE },
   { fv_track, CM_VIEW_COMBAT_TRACK },
   { fv_target_track, CM_VIEW_COMBAT_TRACK_II },
   { fv_flyby, CM_VIEW_FLYBY },
	{ fv_virtual, CM_VIEW_VIRTUAL },
	{ fv_padlock, CM_VIEW_PADLOCK }
};

void checkViewMenuItems(int view)
{
   int i;
   UINT fuFlags;

   for (i=0;i<11;i++)
   {
      if (view2cmd[i][0] == view)
         fuFlags = MF_BYCOMMAND | MF_CHECKED;
      else
         fuFlags = MF_BYCOMMAND | MF_UNCHECKED;
      CheckMenuItem(hmenu,view2cmd[i][1],fuFlags);
   }
}

void updateViewMenuItems(void)
{
	if (the_game)
	{
		UINT flags;
		Flight_Node *viewNode = the_game->getFliteNode();
		if (viewNode)
		{
			Pilot *pilot = viewNode->pilot;
			if (pilot->haaiTargetPilot())
				flags = MF_BYCOMMAND | MF_ENABLED;
			else
				flags = MF_BYCOMMAND | MF_GRAYED;
			EnableMenuItem(hmenu,CM_VIEW_COMBAT_TRACK,flags);
			EnableMenuItem(hmenu,CM_VIEW_COMBAT_TRACK_II,flags);
			EnableMenuItem(hmenu,CM_VIEW_PADLOCK,flags);
		}
	}
}

void setFlightMenuControls()
{
   Flight_Controls *fc;
   if (!the_game)
      return;
   fc = the_game->getFlightControls();
   checkViewMenuItems(fc->view);
   CheckMenuItem(hmenu,CM_CONTROLWBRAKES, MF_BYCOMMAND |
                 fc->wheel_brakes ? MF_CHECKED : MF_UNCHECKED);
   CheckMenuItem(hmenu,CM_CONTROLSBRAKES, MF_BYCOMMAND |
                 fc->speed_brakes ? MF_CHECKED : MF_UNCHECKED);
   CheckMenuItem(hmenu,CM_CONTROLS_GEAR, MF_BYCOMMAND |
                 fc->landing_gear ? MF_CHECKED : MF_UNCHECKED);
   CheckMenuItem(hmenu,CM_SABREAUTO, MF_BYCOMMAND |
                 fc->autopilot ? MF_CHECKED : MF_UNCHECKED);
   CheckMenuItem(hmenu,CM_SABREGUNS, MF_BYCOMMAND |
                 fc->armed_w ? MF_CHECKED : MF_UNCHECKED);
   if (SCREEN_WIDTH == 320 && SCREEN_HEIGHT == 200)
      CheckMenuItem(hmenu,CM_SABREC_COCKPIT, MF_BYCOMMAND |
                     fc->cockpit ? MF_CHECKED : MF_UNCHECKED );
   CheckMenuItem(hmenu,CM_SCNTRRN,MF_BYCOMMAND |
                     (display_flags & TERRAIN_ON ?
                      MF_CHECKED : MF_UNCHECKED));
   CheckMenuItem(hmenu,CM_SCNCLOUDS,MF_BYCOMMAND |
                 (display_flags & CLOUDS_ON ?
                  MF_CHECKED : MF_UNCHECKED));
	CheckMenuItem(hmenu,CM_HUD,fc->hud_on ?
						MF_CHECKED : MF_UNCHECKED);
	CheckMenuItem(hmenu,CM_FLIGHTPATH, fc->vect_on ?
						MF_CHECKED : MF_UNCHECKED);
	CheckMenuItem(hmenu,CM_AIRFRAME, fc->vextern ?
						MF_CHECKED : MF_UNCHECKED );
	CheckMenuItem(hmenu,CM_SOUND, sound_get_active() ?
						MF_CHECKED : MF_UNCHECKED);

}

void handleDemoAuto()
{
Flight_Controls *fc;

   if (!the_game)
      return;
	FlightInput::SetKeybdin(FI_AUTOPILOT_TOGGLE);
   fc = the_game->getFlightControls();
   setMouseYoke(fc->autopilot && wc_yoke_device == wcMOUSE_DEVICE);
}

void updateGUI()
{
   setFlightMenuControls();
	updateViewMenuItems();
}

void setFlightMenuItems(BOOL state)
{
   UINT fuFlags;
   if (hmenu)
   {
      if (state == FALSE)
         fuFlags = MF_BYCOMMAND | MF_GRAYED;
      else
         fuFlags = MF_BYCOMMAND | MF_ENABLED;

      EnableMenuItem(hmenu,CM_SABRENEXT_AIRCRAFT,fuFlags);
      EnableMenuItem(hmenu,CM_CONTROLS0THROTTLE,fuFlags);
      EnableMenuItem(hmenu,CM_CONTROLS_FULL_THROTTLE,fuFlags);
      EnableMenuItem(hmenu,CM_CONTROLWBRAKES,fuFlags);
      EnableMenuItem(hmenu,CM_CONTROLSBRAKES,fuFlags);
      EnableMenuItem(hmenu,CM_CONTROLS_GEAR,fuFlags);
      EnableMenuItem(hmenu,CM_SABREGUNS,fuFlags);
      EnableMenuItem(hmenu,CM_SABREAUTO,fuFlags);
      EnableMenuItem(hmenu,CM_VIEW_FLYBY,fuFlags);
      EnableMenuItem(hmenu,CM_VIEW_COMBAT_TRACK_II,fuFlags);
      EnableMenuItem(hmenu,CM_VIEW_COMBAT_TRACK,fuFlags);
      EnableMenuItem(hmenu,CM_VIEW_OUTSIDE,fuFlags);
      EnableMenuItem(hmenu,CM_VIEW_TOP,fuFlags);
      EnableMenuItem(hmenu,CM_VIEW_BACK,fuFlags);
      EnableMenuItem(hmenu,CM_VIEW_RIGHT,fuFlags);
      EnableMenuItem(hmenu,CM_VIEW_LEFT,fuFlags);
      EnableMenuItem(hmenu,CM_VIEW_FRONT,fuFlags);
		EnableMenuItem(hmenu,CM_VIEW_PADLOCK,fuFlags);
		EnableMenuItem(hmenu,CM_VIEW_VIRTUAL,fuFlags);
      EnableMenuItem(hmenu,CM_SCNTRRN,fuFlags);

      if (SCREEN_WIDTH == 320 && SCREEN_HEIGHT == 200)
         EnableMenuItem(hmenu,CM_SABREC_COCKPIT,fuFlags);
      else
         EnableMenuItem(hmenu,CM_SABREC_COCKPIT,MF_BYCOMMAND | MF_GRAYED);
   }
	if (sound_avail)
		EnableMenuItem(hmenu,CM_SOUND,MF_BYCOMMAND | MF_ENABLED);
	else
		EnableMenuItem(hmenu,CM_SOUND,MF_BYCOMMAND | MF_GRAYED);
}


VOID readInitFile(VOID )
{
   char dirbuff[512];
   char pathbuff[512 + 13 + 1];

   _getcwd(dirbuff,sizeof(dirbuff));
   wsprintf(pathbuff,"%s\\sabrewin.ini",dirbuff);
   ini_screen_width = GetPrivateProfileInt("SABRE","screen_width",SCREEN_WIDTH,
                                           pathbuff);
   ini_screen_height = GetPrivateProfileInt("SABRE","screen_height",SCREEN_HEIGHT,
                                           pathbuff);
	/*
   GetPrivateProfileString("SABRE","flight","",
                                         init_flight_file,
                                         sizeof(init_flight_file),
                                         pathbuff);
   if (strlen(init_flight_file) > 0)
      flight_file = init_flight_file;
   else
      flight_file = NULL;

   GetPrivateProfileString("SABRE","ground","",
                                         init_ground_file,
                                         sizeof(init_ground_file),
                                         pathbuff);
   if (strlen(init_ground_file) > 0)
      ground_file = init_ground_file;
   else
      ground_file = NULL;


   GetPrivateProfileString("SABRE","scenarios","",
                                         init_scenario_file,
                                         sizeof(init_scenario_file),
                                         pathbuff);
   if (strlen(init_scenario_file) > 0)
      scenario_path = init_scenario_file;

   demo = GetPrivateProfileInt("SABRE","demo",0,pathbuff);
   do_random = GetPrivateProfileInt("SABRE","random",0,pathbuff);
   extern int rz_steps;
   rz_steps = GetPrivateProfileInt("SABRE","rz_steps",4,pathbuff);
	editTextures = GetPrivateProfileInt("SABRE","edit_textures",0,pathbuff);
	editMode = GetPrivateProfileInt("SABRE","edit_mode",0,pathbuff);
	*/
}

void pauseGame(int pause)
{
   if (pause)
   {
      gpause = 2;
		sound_active = sound_get_active();
		sound_set_active(0);
#ifdef USES_DDRAW
		vga13_suspend();
      DrawMenuBar(hWnd);
      RedrawWindow(hWnd, NULL, NULL, RDW_FRAME);
#endif
      setMouseYoke(0);
   }
   else
   {
      gpause = 0;
		sound_set_active(sound_active);
      if (!demo)
         setMouseYoke(wc_yoke_device == wcMOUSE_DEVICE);
   }

   EnableMenuItem(hmenu,CM_SABREP_PAUSE,MF_BYCOMMAND | MF_ENABLED);
   CheckMenuItem(hmenu,CM_SABREP_PAUSE, MF_BYCOMMAND |
                 (gpause != 0) ? MF_CHECKED : MF_UNCHECKED);
}

void setMouseYoke(int on)
{
   RECT r;

/*
 * FIXME!!
 */

   mouse_Yoke = on;
   if (on)
   {
#ifndef _DEBUG
      SetCapture(hWnd);
      GetWindowRect(hWnd,&r);
      ClipCursor(&r);
      HideMouse();
#endif
   }
   else
   {
#ifndef _DEBUG
      GetCursorPos(&curPos);
      ShowMouse();
      ClipCursor(NULL);
      ReleaseCapture();
#endif
   }
}

/* Make sure the cursor is showing */
void ShowMouse()
{
   while (ShowCursor(1) < 0);
}

void HideMouse()
{
  while (ShowCursor(0) >= 0);
}


/***********************************************************************
 * Nasty error-handling stuff                                          *
 ***********************************************************************/
void __cdecl error_jump(char *format, ...)
{
   error_flag = 1;
  char buff[BUFSIZ];
  va_list ap;
  sim_printf("Error Raised:\n");
  va_start(ap,format);
  vfprintf(simlog,format,ap);
  va_end(ap);
  va_start(ap,format);
  vsprintf(buff,format,ap);
  va_end(ap);
  ShowWindow(hWnd,SW_HIDE);
  ShowMouse();
  ::MessageBox(NULL,buff,"SABRE Fighter Simulator",MB_OK | MB_ICONEXCLAMATION);
  error_flag = 0;
  exit(1);
}

extern int routine_key;
#define fpe_error_idx 126
char *fpe_errors[] =
{
	"80x86 Interrupt on overflow",
	"80x86 Integer divide by zero",
	"Unknown error",
	"Invalid Operation",
	"Unknown error",
	"80x87 divide by zero",
	"80x87 overflow",
	"80x87 underflow",
	"80x87 precision loss",
	"80x87 stack overflow",
	"Unknown error",
	"Unknown error",
	"Unknown error",
	"Unknown error",
	"SIGFPE raise()'d"
};

void __cdecl float_error_handler(int ,int type, int *)
{
   char buff[512];
   char bbuff[512];

   wsprintf(buff,"FPE Error type: %d in Routine Key %d",
            type,routine_key);
   if (type >= 126 && type <= 140)
      wsprintf(bbuff,"%s",fpe_errors[type - 126]);
   else
      wsprintf(bbuff,"Unknown type");
  sim_printf("%s\n%s\n",buff,bbuff);
  ShowWindow(hWnd,SW_HIDE);
  ShowMouse();
   ::MessageBox(hWnd,bbuff,buff,MB_OK);
	exit(1);
}

int GetMaxTextures()
{
	if (map_man)
		return map_man->n_maps;
	else
		return 0;
}

HRESULT Initialize3DEnvironment( HWND hWnd )
{
    HRESULT hr;

    // Initialize the app
    if( FAILED( hr = wvgad13dx_one_time_scene_init( hWnd ) ) )
        return E_FAIL;

    // Create a new CD3DFramework class. This class does all of our D3D
    // initialization and manages the common D3D objects.
    if( NULL == ( g_pFramework = new CD3DFramework() ) )
        return E_OUTOFMEMORY;

    // Finally, initialize the framework and scene.
    return AppInitialize( hWnd );
}

//-----------------------------------------------------------------------------
// Name: Cleanup3DEnvironment()
// Desc: Cleanup scene objects
//-----------------------------------------------------------------------------
VOID Cleanup3DEnvironment()
{
    if( g_pFramework )
    {
        wvga13dx_final_cleanup( g_pFramework->GetD3DDevice(), 
                          g_pFramework->GetViewport() );

        SAFE_DELETE( g_pFramework );
    }

    D3DEnum_FreeResources();
    g_bActive = FALSE;
    g_bReady  = FALSE;
}

//-----------------------------------------------------------------------------
// Name: AppOutputText()
// Desc: Draws text on the window.
//-----------------------------------------------------------------------------
VOID AppOutputText( LPDIRECT3DDEVICE3 pd3dDevice, DWORD x, DWORD y, CHAR* str )
{
    LPDIRECTDRAWSURFACE4 pddsRenderSurface;
    if( FAILED( pd3dDevice->GetRenderTarget( &pddsRenderSurface ) ) )
        return;

    // Get a DC for the surface. Then, write out the buffer
    HDC hDC;
    if( SUCCEEDED( pddsRenderSurface->GetDC(&hDC) ) )
    {
        SetTextColor( hDC, RGB(255,255,0) );
        SetBkMode( hDC, TRANSPARENT );
        ExtTextOut( hDC, x, y, 0, NULL, str, strlen(str), NULL );
    
        pddsRenderSurface->ReleaseDC(hDC);
    }
    pddsRenderSurface->Release();
}
 
//-----------------------------------------------------------------------------
// Name: DisplayFrameworkError()
// Desc: Displays error messages in a message box
//-----------------------------------------------------------------------------
VOID DisplayFrameworkError( HRESULT hr, APPMSGTYPE errType )
{
    CHAR strMsg[512];

    switch( hr )
    {
        case D3DENUMERR_NOCOMPATIBLEDEVICES:
            strcpy( strMsg, TEXT("Could not find any compatible Direct3D\n"
                    "devices.") );
            break;
        case D3DENUMERR_SUGGESTREFRAST:
            strcpy( strMsg, TEXT("Could not find any compatible devices.\n\n"
                    "Try enabling the reference rasterizer using\n"
                    "EnableRefRast.reg.") );
            break;
        case D3DENUMERR_ENUMERATIONFAILED:
            strcpy( strMsg, TEXT("Enumeration failed. Your system may be in an\n"
                    "unstable state and need to be rebooted") );
            break;
        case D3DFWERR_INITIALIZATIONFAILED:
            strcpy( strMsg, TEXT("Generic initialization error.\n\nEnable "
                    "debug output for detailed information.") );
            break;
        case D3DFWERR_NODIRECTDRAW:
            strcpy( strMsg, TEXT("No DirectDraw") );
            break;
        case D3DFWERR_NODIRECT3D:
            strcpy( strMsg, TEXT("No Direct3D") );
            break;
        case D3DFWERR_INVALIDMODE:
            strcpy( strMsg, TEXT("This sample requires a 16-bit (or higher) "
                    "display mode\nto run in a window.\n\nPlease switch "
                    "your desktop settings accordingly.") );
            break;
        case D3DFWERR_COULDNTSETCOOPLEVEL:
            strcpy( strMsg, TEXT("Could not set Cooperative Level") );
            break;
        case D3DFWERR_NO3DDEVICE:
            strcpy( strMsg, TEXT("No 3D Device") );
            break;
        case D3DFWERR_NOZBUFFER:
            strcpy( strMsg, TEXT("No ZBuffer") );
            break;
        case D3DFWERR_NOVIEWPORT:
            strcpy( strMsg, TEXT("No Viewport") );
            break;
        case D3DFWERR_NOPRIMARY:
            strcpy( strMsg, TEXT("No primary") );
            break;
        case D3DFWERR_NOCLIPPER:
            strcpy( strMsg, TEXT("No Clipper") );
            break;
        case D3DFWERR_BADDISPLAYMODE:
            strcpy( strMsg, TEXT("Bad display mode") );
            break;
        case D3DFWERR_NOBACKBUFFER:
            strcpy( strMsg, TEXT("No backbuffer") );
            break;
        case D3DFWERR_NONZEROREFCOUNT:
            strcpy( strMsg, TEXT("Nonzerorefcount") );
            break;
        case D3DFWERR_NORENDERTARGET:
            strcpy( strMsg, TEXT("No render target") );
            break;
        case E_OUTOFMEMORY:
            strcpy( strMsg, TEXT("Not enough memory!") );
            break;
        case DDERR_OUTOFVIDEOMEMORY:
            strcpy( strMsg, TEXT("There was insufficient video memory "
                    "to use the\nhardware device.") );
            break;
        default:
            strcpy( strMsg, TEXT("Generic application error.\n\nEnable "
                    "debug output for detailed information.") );
    }

    if( MSGERR_APPMUSTEXIT == errType )
    {
        strcat( strMsg, TEXT("\n\nThis sample will now exit.") );
        MessageBox( NULL, strMsg, g_strAppTitle, MB_ICONERROR|MB_OK );
    }
    else
    {
        if( MSGWARN_SWITCHTOSOFTWARE == errType )
            strcat( strMsg, TEXT("\n\nSwitching to software rasterizer.") );
        MessageBox( NULL, strMsg, g_strAppTitle, MB_ICONWARNING|MB_OK );
    }
}
