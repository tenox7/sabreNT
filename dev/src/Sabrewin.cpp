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
 * File   : sabrewin.cpp                         *
 * Date   : December, 1997                       *
 * Author : Dan Hammer                           *
 * Port to Windows NT/95                         *
 * 04/17/98 MSD                                  *
 *************************************************/
#ifdef USES_DSOUND
#define THISVER "Version 0.2.5.ws " ## __DATE__
#else
#define THISVER "Version 0.2.5.w " ## __DATE__
#endif
const char *wVERSION = THISVER;

#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <math.h>
#include <time.h>
#include <stdarg.h>
#include <signal.h>

#include <direct.h>
#ifdef USES_DDRAW
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>
#else
#include <windows.h>
#endif
#include <commdlg.h>
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

LRESULT CALLBACK WndProc ( HWND hWnd, unsigned int iMessage,
								  unsigned int wParam, LONG lParam );

void initGraphics();
void initSound();
void initJoystick();
void buildFlightsMenu();
int MessageHook();
void doGame();
void doEdit();
void doOpenFlight();
void handleMenuCommand(WORD command);
void __cdecl float_error_handler(int,int,int*);
void readInitFile();
void setFlightMenuItems(BOOL state);
void setFlightMenuControls();
void checkViewMenuItems(int view);
void updateViewMenuItems(void);
void updateGUI();
void ShowMouse();
void HideMouse();
void setMouseYoke(int);
void pauseGame(int);
void handleDemoAuto();
void resizeApp(int w, int h);
void setDevices(void);

HBITMAP TextrMap2HBITMAP(TextrMap &tmap);
HBITMAP GetTextureBitmap(int);
int GetMaxTextures();

HWND   hWnd;
HANDLE hinstance;
MSG Message;
WNDCLASS WndClass;
HMENU hmenu;
POINT curPos;

int ini_screen_width = -1;
int ini_screen_height = -1;


long RightButton = FALSE, LeftButton = FALSE,
	  Keys, Dir, Mag;
BOOL running = TRUE;
BOOL app_active = FALSE;
int mouse_Yoke = 0;
GameSession    *the_game = NULL;
SceneEdit		*the_edit = NULL;
int           error_flag = 0;
int				editTextures = 0;
int				editMode = 0;

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

int js0 = 0;
int js1 = 0;
int demo = 0;
int edit_scene = 0;
int mouse_avail = 1;
int playing;
int pause_state;
int sound_active = 1;

int  cmd   = 0;

inline void SETGAME(char *flight_path,
               int random_mode = 0,
               int demo_mode = 0,
               char *ground_path = NULL,
               char *world_path = NULL)
{
   playing = 1;
   flight_file = flight_path;
   ground_file = ground_path;
   if (world_path)
      world_file = world_path;
   else
      world_file = "a.wld";
   demo = demo_mode;
   do_random = random_mode;
}

int WINAPI WinMain ( HANDLE hInstance, HANDLE hPrevInstance,
			 LPSTR lpszCmdParam, int nCmdShow )

  {


  hinstance = hInstance;
   // Try to open up a log
  if ((simlog = fopen(log_path,"w")) == NULL)
      simlog = stdout;

//  ClearSystemPalette();

 // randomize ();
  srand(time(NULL));
//  signal(SIGFPE, float_error_handler);

  readInitFile();
#ifdef USES_DDRAW
	ini_screen_width = 640;
	ini_screen_height = 480;
#endif
  wvga_params(ini_screen_width, ini_screen_height);
  initJoystick();

#ifdef USES_DDRAW
  if (!hPrevInstance)
  {
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = 0; // GetStockObject ( GRAY_BRUSH );
	WndClass.hCursor =  LoadCursor ( NULL, IDC_ARROW );
	WndClass.hIcon = LoadIcon ( hInstance, NULL );
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = "SABREW";
	WndClass.lpszMenuName = ( const char * ) MENU_1;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	if ( !RegisterClass ( &WndClass ) )
		 return 0;
  }

  hWnd = CreateWindowEx (WS_EX_TOPMOST, 
								 "SABREW",             // class name
								"Sabre Fighter Plane Simulator",  // Caption
								WS_POPUP,
								0, 0,
								GetSystemMetrics( SM_CXSCREEN ),
								GetSystemMetrics( SM_CYSCREEN ),
								NULL,
								NULL,
								hInstance,
								NULL );
#else
  if (!hPrevInstance)
  {
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = GetStockObject ( GRAY_BRUSH );
	WndClass.hCursor =  LoadCursor ( NULL, IDC_ARROW );
	WndClass.hIcon = LoadIcon ( hInstance, NULL );
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = "SABREW";
	WndClass.lpszMenuName = ( const char * ) MENU_1;
	WndClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	if ( !RegisterClass ( &WndClass ) )
		 return 0;
  }

  hWnd = CreateWindow ( "SABREW",             // class name
								"Sabre Fighter Plane Simulator",  // Caption
								WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU
                        | WS_THICKFRAME,// Style
								CW_USEDEFAULT,           // x position
								CW_USEDEFAULT,           // y position
								SCREEN_WIDTH+8,            // cx - size
								SCREEN_HEIGHT+48,        // cy - size
								NULL,                    // Parent window
								NULL,                    // Menu
								hInstance,               // Program Instance
								NULL );                  // Parameters
#endif
  sim_printf("CreateWindow done\n");
  ShowWindow ( hWnd, nCmdShow );
  sim_printf("ShowWindow done\n");
   initGraphics();	
  hmenu = GetMenu(hWnd);
	if (!editTextures)
		DeleteMenu(hmenu,CM_TEXTURES,MF_BYCOMMAND);

  buildFlightsMenu();
  initSound();



   if (flight_file != NULL)
      playing = 1;
   else
   {
      playing = 0;
      setFlightMenuItems(FALSE);
   }


   while (running)
   {
		if (editMode)
		{
			doEdit();
			continue;
		}

      if (playing)
      {
         cmd = 0;
         doGame();
      }

      playing = 0;

      if (cmd)
      {
         SimScenario *scn = SimScenario::cmd2scn(cmd);
         if (scn)
            SETGAME(scn->flight_file,
                    scn->randomize_positions,
                    0,
                    scn->has_ground_file ? scn->ground_file : NULL,
                    scn->has_world_file ? scn->world_file : NULL);
         else switch (cmd)
         {
				case CM_NEWGAME:
					playing = 1;
					break;

           case CM_DEMFURBALL:
            SETGAME("furball.flt",1,1);
            break;

            case CM_DEMOPVY:
            SETGAME("pistons.flt",1,1);
            break;

            case CM_DEMOSMELEE:
            SETGAME("melee.flt",1,1);
            break;

            case CM_DEMOSFSCRAP:
            SETGAME("thunder.flt",1,1);
            break;

            default:
            SETGAME("fly.flt");
            break;

         }

         cmd = 0;
      }

      MessageHook();
   }

  ::WinHelp(hWnd,"SABRE.HLP",HELP_QUIT,0L);
  wvga_activate(FALSE);
  wvga_release();
  sound_destroy();
  sim_printf("exiting app\n");
  return Message.wParam;

 }

void buildFlightsMenu()
{
  sim_printf("Reading Scenarios File %s\n",scenario_path);
  SimScenario::readFile(scenario_path);
  HMENU hm = GetSubMenu(hmenu,0);
  for (int i=0;i<SimScenario::nscenarios;i++)
  {
      SimScenario *sm = SimScenario::getScenario(i);
      InsertMenu(hm,i,MF_STRING | MF_BYPOSITION,sm->command_no,sm->title);
  }
}

void initGraphics()
{
  curpen->color = 2;
  curpen->position = Point(0,0);
  curpen->width = 1;
  sim_printf("Initing wvga\n");
  wvga_init(hWnd,palette_file);
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

void initJoystick()
{
#ifdef _M_IX86
	joystick0 = new Joystick(0);
	if (!joystick0->IsAvailable())
	{
		delete joystick0;
		joystick0 = NULL;
	}
#else
	joystick0 = NULL;
#endif
	if (joystick0 != NULL)
	{
		wc_joystick_avail = 1;
		wc_yoke_device = wcJOY_DEVICE;
		if (joystick0->HasZ())
		{
			wc_throttle_avail = 1;
			wc_throttle_device = wcJOY_DEVICE;
		}
		if (joystick0->HasR())
		{
			wc_rudder_pedals_avail = 1;
			wc_rudder_device = wcJOY_DEVICE;
		}
	}
}

void resizeApp(int new_width, int new_height)
{
#ifndef USES_DDRAW
   sim_printf("Resizing\n");
	float pixl_ratio = SCREEN_WIDTH / 320.0;
   float fovx = Port_3D::fovx / pixl_ratio;
   float fovy = Port_3D::fovy / pixl_ratio;
   wvga_params(new_width,new_height);     
   wvga_init(hWnd,NULL);
   init_edge_bounds();
   tr_init();
   Port_3D::initPort3D();
   pixl_ratio = SCREEN_WIDTH / 320.0;
   Port_3D::fovy = fovy * pixl_ratio;
   Port_3D::fovx = fovx * pixl_ratio;
#endif
}

void initSound()
{
	if (sound_init((long)hWnd) == SOUND_OK)
		read_sound_file(snd_file);
	sound_active = sound_get_active();
}

void setDevices(void)
{
	if (the_game)
	{
		Joystick *yokeJoy = NULL;
		Joystick *throttleJoy = NULL;
		Joystick *rudderJoy = NULL;
		Mouse *yokeMouse = NULL;

		if (wc_yoke_device == wcJOY_DEVICE)
			yokeJoy = joystick0;
		else if (wc_yoke_device == wcMOUSE_DEVICE)
			yokeMouse = &mouse;
		if (wc_throttle_device == wcJOY_DEVICE)
			throttleJoy = joystick0;
		if (wc_rudder_device == wcJOY_DEVICE)
			rudderJoy = joystick0;

		the_game->setDevices(yokeMouse,yokeJoy,rudderJoy,throttleJoy);

		if (wc_yoke_device == wcMOUSE_DEVICE)
		{
			int x,y;
			RECT rect;
			setMouseYoke(1);
		   GetWindowRect(hWnd,&rect);
			x = rect.left + ((rect.right - rect.left + 1) / 2);
			y = rect.top + ((rect.bottom - rect.top + 1) / 2);
			SetCursorPos(x,y);
		}
	}
}

void doGame()
{
   RECT rect;
   int x,y;
	Joystick *yokeJoy = NULL;
	Joystick *throttleJoy = NULL;
	Joystick *rudderJoy = NULL;
	Mouse *yokeMouse = NULL;
   gpause = 0;

   if (!demo && wc_yoke_device == wcMOUSE_DEVICE)
   {
		setMouseYoke(1);
      GetWindowRect(hWnd,&rect);
      x = rect.left + ((rect.right - rect.left + 1) / 2);
      y = rect.top + ((rect.bottom - rect.top + 1) / 2);
      SetCursorPos(x,y);
   }
	WinKeyboard::Init();
	if (wc_yoke_device == wcJOY_DEVICE)
		yokeJoy = joystick0;
	else if (wc_yoke_device == wcMOUSE_DEVICE)
		yokeMouse = &mouse;
	if (wc_throttle_device == wcJOY_DEVICE)
		throttleJoy = joystick0;
	if (wc_rudder_device == wcJOY_DEVICE)
		rudderJoy = joystick0;
	
    the_game = new GameSession(
            world_file,
		      flight_file,
		      ground_file,
		      yokeMouse,
		      yokeJoy,
		      rudderJoy,
		      throttleJoy,
		      1,
		      1,
		      cpk_file,
		      hud_file,
		      demo,
            MessageHook);

  setFlightMenuItems(TRUE);
  the_game->doGame();
  delete the_game;
  the_game = NULL;
  setFlightMenuItems(FALSE);

  if (mouse_Yoke)
    setMouseYoke(0);
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
	if (joystick0 != NULL)
		joystick0->Update();
	if (joystick1 != NULL)
		joystick1->Update();
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

LRESULT CALLBACK WndProc ( HWND hWnd, unsigned int iMessage,
								  unsigned int wParam, LONG lParam )
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


void readInitFile()
{
   char dirbuff[512];
   char pathbuff[512 + 13 + 1];

   _getcwd(dirbuff,sizeof(dirbuff));
   wsprintf(pathbuff,"%s\\sabrewin.ini",dirbuff);
   ini_screen_width = GetPrivateProfileInt("SABRE","screen_width",SCREEN_WIDTH,
                                           pathbuff);
   ini_screen_height = GetPrivateProfileInt("SABRE","screen_height",SCREEN_HEIGHT,
                                           pathbuff);
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
   rz_steps = GetPrivateProfileInt("SABRE","rz_steps",0,pathbuff);
	editTextures = GetPrivateProfileInt("SABRE","edit_textures",0,pathbuff);
	editMode = GetPrivateProfileInt("SABRE","edit_mode",0,pathbuff);
	/*
	 *  Remember user's control device selections
	 */
	wc_yoke_device = GetPrivateProfileInt("SABRE","yoke_device",wcMOUSE_DEVICE,pathbuff);
	wc_throttle_device = GetPrivateProfileInt("SABRE","throttle_device",wcKEY_DEVICE,pathbuff);
	wc_rudder_device = GetPrivateProfileInt("SABRE","rudder_device",wcKEY_DEVICE,pathbuff);
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
#ifndef _DEBUG
   RECT r;
#endif

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

 
/*
int _cdecl matherr (struct exception *a)
{
	error_jump("Math Error: %s,%f\nRoutine Key: %d\n",
					a->name,a->arg1,routine_key);
	return(1);
}
*/
