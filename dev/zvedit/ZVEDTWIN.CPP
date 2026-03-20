/*
    SABRE Fighter Plane Simulator
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
 * SABRE Fighter Plane Simulator                 *
 * Version: 0.1                                  *
 * File   : sabrewin.cpp                         *
 * Date   : December, 1997                       *
 * Author : Dan Hammer                           *
 * Port to Windows NT/95                         *
 *************************************************/
// ------------------------------------------------------------
// | Global include files:                                    |
// ------------------------------------------------------------
#include <dos.h>
#include <dir.h>
#include <math.h>
#include <conio.h>
#include <time.h>
#include <stdarg.h>
#include <signal.h>
#include <stdio.h>

#include <windows.h>
#include <commdlg.h>
#include <iostream.h>

// ------------------------------------------------------------
// | Local include files:                                     |
// ------------------------------------------------------------
#include "wvga_13.h"
#include "zvedit.h"
#include "zvedtwin.rh"
#include "polydlg.h"

// ------------------------------------------------------------
// | Global variables/constants:                              |
// ------------------------------------------------------------

LRESULT CALLBACK WndProc ( HWND hWnd, unsigned int iMessage,
								  unsigned int wParam, LONG lParam );

void initGraphics();
int MessageHook();
void doEdit();
void handleMenuCommand(WORD command);
void float_error_handler(int,int,int*);
void readInitFile();
void updateGUI();
void ShowMouse();
void HideMouse();

HWND           hWnd;
HANDLE         hinstance;
MSG            Message;
WNDCLASS       WndClass;
HMENU          hmenu;
POINT          curPos;
int            ini_screen_width = -1;
int            ini_screen_height = -1;
long RightButton = FALSE, LeftButton = FALSE,
	  Keys, Dir, Mag;
BOOL           running = TRUE;
int            mouse_Yoke = 0;
int            kbdin;
int            editing;
char           init_shape_file[256];
char           init_palette_file[256];
char           init_txtr_file[256];
int            mouse_avail = 1;
Mouse          mouse;


// ------------------------------------------------------------
// | Program entry:                                           |
// ------------------------------------------------------------
#pragma argsused
int WINAPI WinMain ( HANDLE hInstance, HANDLE hPrevInstance,
			 LPSTR lpszCmdParam, int nCmdShow )

  {


  hinstance = hInstance;


  // Randomize the timer for sound events:
  randomize ();
  signal(SIGFPE,(void (*)(int)) float_error_handler);
  readInitFile();

  wvga_params(ini_screen_width, ini_screen_height);

  WndClass.cbClsExtra = 0;
  WndClass.cbWndExtra = 0;
  WndClass.hbrBackground = GetStockObject ( GRAY_BRUSH );
  WndClass.hCursor =  LoadCursor ( NULL, IDC_ARROW );
  WndClass.hIcon = LoadIcon ( hInstance, NULL );
  WndClass.hInstance = hInstance;
  WndClass.lpfnWndProc = WndProc;
  WndClass.lpszClassName = "ZVEDITW";
  WndClass.lpszMenuName = ( const char * ) MENU_1;
  WndClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;

  if ( !RegisterClass ( &WndClass ) )
     return 0;

  hWnd = CreateWindow ( "ZVEDITW",             // class name
								"ZVEDIT",  // Caption
								WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,// Style
								CW_USEDEFAULT,           // x position
								CW_USEDEFAULT,           // y position
								SCREEN_WIDTH+4,            // cx - size
								SCREEN_HEIGHT+40,        // cy - size
								NULL,                    // Parent window
								NULL,                    // Menu
								hInstance,               // Program Instance
								NULL );                  // Parameters

  ShowWindow ( hWnd, nCmdShow );
  hmenu = GetMenu(hWnd);

  initGraphics();

  kbdin = 0;
   if (shape_file != NULL)
      editing = 1;
   else
   {
      editing = 0;
   }


   while (running)
   {
      if (editing)
      {
         kbdin = 0;
         doEdit();
      }
      editing = 0;

      MessageHook();
   }

  return Message.wParam;
}


void initGraphics()
{
  curpen->color = 2;
  curpen->position = Point(0,0);
  curpen->width = 1;
  wvga_init(hWnd,palette_file);
  init_edge_bounds();
  tr_init();
  Port_3D::initPort3D();
  g_font = new Font6x6("simfnt6");
  g_font->put_width = 6;
  MYCHECK(g_font != NULL);
  read_texture_file(txtr_file);
  wvga_realize_palette();
}

void doEdit()
{
   RECT rect;
   int x,y;
   kbdin = 0;
   GetWindowRect(hWnd,&rect);
   x = rect.left + ((rect.right - rect.left + 1) / 2);
   y = rect.top + ((rect.bottom - rect.top + 1) / 2);
   SetCursorPos(x,y);
   messagehook = MessageHook;
   doEditor(shape_file);
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
   if (kbdin == 'q' || kbdin == 'Q')
   {
      if (kbdin == 'q')
         save();
      running = 0;
   }
   return (running );
}

LRESULT CALLBACK WndProc ( HWND hWnd, unsigned int iMessage,
								  unsigned int wParam, LONG lParam )
{
   float XPos, YPos;
   switch ( iMessage )
	{
      case WM_COMMAND:
      handleMenuCommand(LOWORD(wParam));
      break;

      case WM_LBUTTONUP:
      {
         mouse.buttons = 0;
         break;
      }

      case WM_LBUTTONDOWN:
		{
         mouse.buttons = 1;
         break;
      }

      case WM_RBUTTONUP:
      {
         break;
		}

      case WM_RBUTTONDOWN:
		{
			break;
		}

      case WM_MOUSEMOVE:
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
         break;
      }

      case WM_DESTROY:
      {
         running = FALSE;
         break;
      }

      case WM_CHAR:
      {
         kbdin = (char) wParam;
         break;
      }

      case WM_PAINT:
      blit_buff();
      break;

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

      case CM_EDIT_POLY:
      if (DialogBox(hinstance,MAKEINTRESOURCE(POLY_DLG),hWnd,
                  (DLGPROC) PolyDialogProc))
      {
         mode = 1;
         kbdin = '"';
      }


      break;

      default:
//      cmd = command;
      break;

   }

}


void updateGUI()
{

}

void readInitFile()
{
   char dirbuff[512];
   char pathbuff[512 + 13 + 1];

   getcwd(dirbuff,sizeof(dirbuff));
   wsprintf(pathbuff,"%s\\sabrewin.ini",dirbuff);
   ini_screen_width = GetPrivateProfileInt("SABRE","screen_width",SCREEN_WIDTH,
                                           pathbuff);
   ini_screen_height = GetPrivateProfileInt("SABRE","screen_height",SCREEN_HEIGHT,
                                           pathbuff);

   GetPrivateProfileString("ZVEDIT","shape_file","",
                           init_shape_file,
                           sizeof(init_shape_file),
                           pathbuff);
   if (strlen(init_shape_file) > 0)
      shape_file = init_shape_file;

   GetPrivateProfileString("ZVEDIT","txtr_file","",
                           init_txtr_file,
                           sizeof(init_txtr_file),
                           pathbuff);
   if (strlen(init_txtr_file) > 0)
      txtr_file = init_txtr_file;
   GetPrivateProfileString("ZVEDIT","palette_file","",
                           init_palette_file,
                           sizeof(init_palette_file),
                           pathbuff);
   if (strlen(init_palette_file) > 0)
      palette_file = init_palette_file;
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
void error_jump(char *format, ...)
{
  char buff[BUFSIZ];
  va_list ap;
  va_start(ap,format);
  vsprintf(buff,format,ap);
  va_end(ap);
  ::MessageBox(NULL,buff,"SABRE Error",MB_OK);
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

void float_error_handler(int ,int type, int *)
{
   char buff[512];
   char bbuff[512];

   wsprintf(buff,"FPE Error type: %d in Routine Key %d",
            type,routine_key);
   if (type >= 126 && type <= 140)
      wsprintf(bbuff,"%s",fpe_errors[type - 126]);
   else
      wsprintf(bbuff,"Unknown type");
   ::MessageBox(NULL,bbuff,buff,MB_OK);
	exit(1);
}

int _cdecl matherr (struct exception *a)
{
	error_jump("Math Error: %s,%f\nRoutine Key: %d\n",
					a->name,a->arg1,routine_key);
	return(1);
}
