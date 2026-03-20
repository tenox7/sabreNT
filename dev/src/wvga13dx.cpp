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
 * File   : wvga13dx.cpp                         *
 * Date   : December, 1998                       *
 * Author : Dan Hammer                           *
 *************************************************/
#define WIN32_LEAN_AND_MEAN  
// #define INITGUID
#include <windows.h> 
#include <windowsx.h> 
#include <mmsystem.h>
#include <commdlg.h>
#include <iostream.h>
#include <stdio.h>
#include <dos.h>
#include <direct.h>
#include <math.h>
#include <conio.h>
#include <time.h>
#include <stdarg.h>
#include <signal.h>
#define D3D_OVERLOADS
#include <ddraw.h>
#include <d3d.h>
#include "d3dtex.h"
#include "D3DTextr.h"
#include "D3DUtil.h"
#include "D3DMath.h"



#include "wvga_13.h"
#include "simerr.h"
#include "simfile.h"
#include "clip.h"
#include "ddaline.h"
#include "vmath.h"
#include "port_3d.h"
#include "txtrmap.h"


Rect     cliprect;
SimFont *g_font = NULL;
int SCREEN_WIDTH = 320;
int SCREEN_PITCH = 320;
int SCREEN_HEIGHT = 200;
int MXSCREEN_WIDTH = 320;
int MXSCREEN_HEIGHT = 200;
float aspect_ratio = 1.0;
int   VGAMODE =        0;
int   MAX_X =        319;
int   MAX_Y =        199;
int   N_COLORS =     256;
int   PALETTE_SIZE = 768;
int   wvga_palette_flag = 1;

static PALETTEENTRY palette[256];
static RGBQUAD rgb_colors [ 256 ];

TCHAR* g_strAppTitle       = TEXT( "Sabre Fighter Plane Simulator" );
BOOL   g_bAppUseZBuffer    = TRUE;    // Create/use a z-buffering
BOOL   g_bAppUseBackBuffer = TRUE;    // Create/use a back buffer

static HWND hwnd;
static LPDIRECT3D3				lpd3d;
static LPDIRECT3DDEVICE3		lpd3dDevice;
static LPDIRECT3DVIEWPORT3    lpd3dViewPort;
static LPDIRECTDRAWPALETTE		lpddpal=0;
static HRESULT						ddresult;
static LPDIRECT3DLIGHT        lpd3dLight = 0;
static D3DTexture					Texture;
static D3DTEXTUREHANDLE			TextureHandle = 0;
static D3DMATERIAL				mat;
static D3DMATERIALHANDLE		hMat = 0;
static LPDIRECT3DMATERIAL2		lpMat	= NULL;

static int ddraw_err_check(HRESULT ddresult, char *s = NULL);
static char *display_ddx_error(HRESULT err, char *s = NULL);
static char *dderr_2_string(HRESULT dderr);
static void attach_z_buffer();

static void wvga_init_d3d(void);

RGBQUAD *wvga_get_rgb_colors(void)
{
	return rgb_colors;
}

void wvga_release()
{
}

void attach_z_buffer(D3DDEVICEDESC *hal, D3DDEVICEDESC *hel, LPDIRECTDRAWSURFACE dsurface)
{
int zBuffDepth = 0;

	if (hal->dwDeviceZBufferBitDepth != 0)
	{
		if (hal->dwDeviceZBufferBitDepth & (DDBD_16 | DDBD_24 | DDBD_32))
		{
			zBuffDepth = 16;
			sim_printf("Hardware supports z-buffering, using 16 bits\n");
		}
	}
	if (zBuffDepth == 0)
	{
		if (hel->dwDeviceZBufferBitDepth != 0)
		{
			if (hel->dwDeviceZBufferBitDepth & (DDBD_16 | DDBD_24 | DDBD_32))
			{
				zBuffDepth = 16;
				sim_printf("Using 16-bit software z-buffer\n");
			}
			else if (hal->dwDeviceZBufferBitDepth & DDBD_8)
			{
				zBuffDepth = 8;
				sim_printf("Using hardware 8-bit zbuffer");
			}
			else
			{
				zBuffDepth = 8;
				sim_printf("Using software 8-bit zbuffer");
			}
		}
		else if (hal->dwDeviceZBufferBitDepth & DDBD_8)
		{
			zBuffDepth = 8;
			sim_printf("Using hardware 8-bit zbuffer");
		}
	}
	if (zBuffDepth == 0)
	{
		sim_printf("Device does not support z-buffering\n");
		return;
	}
	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize            = sizeof(ddsd);
	ddsd.dwFlags           = DDSD_CAPS   |
									DDSD_WIDTH  |
									DDSD_HEIGHT |
									DDSD_ZBUFFERBITDEPTH;
	ddsd.ddsCaps.dwCaps    = DDSCAPS_ZBUFFER | DDSCAPS_SYSTEMMEMORY;
	ddsd.dwWidth           = SCREEN_WIDTH;
	ddsd.dwHeight          = SCREEN_HEIGHT;
	ddsd.dwZBufferBitDepth = zBuffDepth;
	ddresult = ddraw_err_check(lpdd->CreateSurface(&ddsd, &lpddszbuffer, NULL),"Create Z Buffer Surface");
	/*
	* Attach it to the rendering target.
	*/
	ddresult = ddraw_err_check(dsurface->AddAttachedSurface(lpddszbuffer),"Add Z buffer Surface");
}

void wvga_init_d3d()
{
	ddresult = lpdd->QueryInterface(IID_IDirect3D2, (void**)&lpd3d);
	ddraw_err_check(ddresult,"QueryInterface");
	
	//
	// create a Direct3D device.
	// first try HAL, then RGB, RAMP
	//
	d3dName = "HAL";
	ddresult = lpd3d->CreateDevice(IID_IDirect3DHALDevice, lpddssecondary , &lpd3dDevice);
	lpDeviceDesc = &hal;
	if (ddresult != DD_OK)
	{
		d3dName = "MMX";
		ddresult = lpd3d->CreateDevice(IID_IDirect3DMMXDevice, lpddssecondary, &lpd3dDevice);
		lpDeviceDesc = &hel;
	}
	if (ddresult != DD_OK)
	{
		d3dName = "RGB";
		ddresult = lpd3d->CreateDevice(IID_IDirect3DRGBDevice, lpddssecondary, &lpd3dDevice);
	}
	if (ddresult != DD_OK)
	{
		d3dName = "RAMP";
		ddresult = lpd3d->CreateDevice(IID_IDirect3DRampDevice, lpddssecondary, &lpd3dDevice);
	}

	ddraw_err_check(ddresult,"CreateDevice");
	sim_printf("Device = %s\n",
					d3dName);
	memset(&hal, 0, sizeof(hal));
	hal.dwSize = sizeof(hal);
	memset(&hel, 0, sizeof(hel));
	hel.dwSize = sizeof(hel);
	ddresult = ddraw_err_check(lpd3dDevice->GetCaps(&hal, &hel),"GetCaps");
	attach_z_buffer(&hal,&hel,lpddssecondary);
	ddresult = ddraw_err_check(lpd3dDevice->SetRenderTarget(lpddssecondary,0),"SetRenderTarget");
	if (hel.dpcTriCaps.dwTextureCaps & D3DPTEXTURECAPS_PERSPECTIVE)
		sim_printf("HEL Supports Texture Perspective\n");
	//
	// now make a Viewport
	ddresult = ddraw_err_check(lpd3d->CreateViewport(&lpd3dViewport, NULL),"CreateViewport");	
   ddresult = ddraw_err_check(lpd3dDevice->AddViewport(lpd3dViewport),"AddViewport");
   /*
	 *
    * Setup the viewport for a reasonable viewing area
	 */
	D3DVIEWPORT2 viewData;
	memset(&viewData, 0, sizeof(D3DVIEWPORT2));
	viewData.dwSize = sizeof(D3DVIEWPORT2);
	viewData.dwX = 0;
	viewData.dwY = 0;
	viewData.dwWidth  = SCREEN_WIDTH;
	viewData.dwHeight = SCREEN_HEIGHT;
	viewData.dvClipX = 0.0;
	viewData.dvClipY = 0.0;
	viewData.dvClipWidth = SCREEN_WIDTH;
	viewData.dvClipHeight = SCREEN_HEIGHT;
	viewData.dvMinZ = 1.0f;
	viewData.dvMaxZ = 10000.0f;
	ddresult = ddraw_err_check(lpd3dViewport->SetViewport2(&viewData),"SetViewport2");
   ddresult = ddraw_err_check(lpd3dDevice->SetCurrentViewport(lpd3dViewport),"SetCurrentViewport");


    Texture.Load(lpd3dDevice, TEST_BMP);
    TextureHandle = Texture.GetHandle();

   ddresult = ddraw_err_check(lpd3d->CreateMaterial(&lpMat, NULL),"CreateMaterial");
	memset(&mat, 0, sizeof(D3DMATERIAL));
	mat.dwSize = sizeof(D3DMATERIAL);
	mat.diffuse.r = (D3DVALUE)1.0;
	mat.diffuse.g = (D3DVALUE)1.0;
	mat.diffuse.b = (D3DVALUE)1.0;
	mat.ambient.r = (D3DVALUE)1.0;
	mat.ambient.g = (D3DVALUE)1.0;
	mat.ambient.b = (D3DVALUE)1.0;
	mat.specular.r = (D3DVALUE)1.0;
	mat.specular.g = (D3DVALUE)1.0;
	mat.specular.b = (D3DVALUE)1.0;
	mat.power = (float)40.0;
	mat.hTexture = TextureHandle;
	mat.dwRampSize = 256;
	lpMat->SetMaterial(&mat);
	lpMat->GetHandle(lpd3dDevice, &hMat);
}

HRESULT wvga13dx_confirm_device( DDCAPS*, D3DDEVICEDESC* )
{
	return S_OK;
}

HRESULT wvga13dx_final_cleanup( LPDIRECT3DDEVICE3, LPDIRECT3DVIEWPORT3 )
{
	if (lpMat)
		lpMat->Release();
	if (lpd3dLight)
		lpd3dLight->Release();
	if (lpddpal)
		lpddpal->Release();
	return S_OK;	
}

HRESULT wvga13dx_one_time_scene_init( HWND )
{
	return S_OK;
}

HRESULT wvga13dx_init_device_objects( LPDIRECT3DDEVICE3 dv, LPDIRECT3DVIEWPORT3 vw)
{
	lpd3dDevice = dv;
	lpd3dViewPort = vw;
	// Get a ptr to the ID3D object to create materials and/or lights. Note:
	// the Release() call just serves to decrease the ref count.
	if( FAILED( pd3dDevice->GetDirect3D( &lpd3d ) ) )
        return E_FAIL;
	lpd3d->Release();
}

void wvga_params(int ini_screen_width, int ini_screen_height)
{
   VGAMODE       =   0;
   if (ini_screen_width > 0)
      SCREEN_WIDTH = ini_screen_width;
   else
      SCREEN_WIDTH  =  320;
   if (ini_screen_height > 0)
      SCREEN_HEIGHT = ini_screen_height;
   else
      SCREEN_HEIGHT = 200;
   int dv = SCREEN_WIDTH / 8;
   SCREEN_WIDTH = 8 * dv;
	SCREEN_PITCH = SCREEN_WIDTH;
   MAX_X = SCREEN_WIDTH - 1;
   MAX_Y = SCREEN_HEIGHT - 1;
   MXSCREEN_HEIGHT = SCREEN_HEIGHT;
   MXSCREEN_WIDTH  = SCREEN_WIDTH;
   N_COLORS  = 256;
   PALETTE_SIZE = 768;
   aspect_ratio  = 1.0;
   cliprect.topLeft.x = 0;
   cliprect.topLeft.y = 0;
   cliprect.botRight.x = MAX_X;
   cliprect.botRight.y = MAX_Y;
   sim_printf("screen size: %d %d\n",SCREEN_WIDTH,SCREEN_HEIGHT);
}

void wvga_init(HWND hw, char *palette_file)
{
   hwnd = hw;
	ddresult = DirectDrawCreate(NULL,&lpdd,NULL);
	ddraw_err_check(ddresult,"DirectDrawCreate");
	//ddresult = lpdd->SetCooperativeLevel(hwnd,DDSCL_NORMAL);
	
//	ddresult = lpdd->SetCooperativeLevel(hwnd,DDSCL_FULLSCREEN | DDSCL_EXCLUSIVE);
	ddresult = lpdd->SetCooperativeLevel(hwnd,DDSCL_NORMAL);
	ddraw_err_check(ddresult,"SetCooperativeLevel");
//	ddresult = lpdd->SetDisplayMode(SCREEN_WIDTH,SCREEN_HEIGHT,8);
	ddresult = lpdd->SetDisplayMode(SCREEN_WIDTH,SCREEN_HEIGHT,16);

	ddraw_err_check(ddresult,"SetDisplayMode");
	memset(&ddsd,0,sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
	ddsd.ddsCaps.dwCaps = 
		DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX | DDSCAPS_3DDEVICE;
	ddsd.dwBackBufferCount = 1;
	ddresult = lpdd->CreateSurface(&ddsd,&lpddsprimary,NULL);
	ddraw_err_check(ddresult,"CreateSurface");
	ddscaps.dwCaps = DDSCAPS_BACKBUFFER;
	ddresult = lpddsprimary->GetAttachedSurface(&ddscaps,&lpddssecondary);
	ddraw_err_check(ddresult,"GetAttachedSurface");
	if (palette_file)
		wvga_palette(palette_file);
	wvga_init_d3d();
}

void wvga_palette(char *palette_file)
{
   HDC dc = GetDC(hwnd);
   GetSystemPaletteEntries(dc,0,256,palette);
   ReleaseDC (hwnd,dc);
   wvga_palette_flag = 1;
   read_palette_file(palette_file);
   wvga_palette_flag = 0;
	ddresult = lpdd->CreatePalette(DDPCAPS_8BIT /* | DDPCAPS_INITIALIZE | DDPCAPS_ALLOW256 */,
											palette,&lpddpal,NULL);
	ddraw_err_check(ddresult,"CreatePalette");
	lpddsprimary->SetPalette(lpddpal);
	lpddssecondary->SetPalette(lpddpal);
}

void wvga_realize_palette()
{
	 
}

int wvga_wants_activate_message(void)
{
	return 0;
}

void wvga_activate(int activate)
{

}


void set_rgb_value(int i, int red, int green, int blue)
{
   if (wvga_palette_flag && i >= 0 && i < 256)
   {
      float r,g,b;

      r = ((float) red) / 63.0 * 255.0;
      g = ((float) green) / 63.0 * 255.0;
      b = ((float) blue) / 63.0 * 255.0;
		palette[i].peRed = (BYTE)r;
		palette[i].peGreen = (BYTE)g;
		palette[i].peBlue = (BYTE)b;
		rgb_colors[i].rgbRed = (BYTE)r;
		rgb_colors[i].rgbGreen = (BYTE)g;
		rgb_colors[i].rgbBlue = (BYTE)b;
   }
}

void get_rgb_value(int i, char *red, char *green, char *blue)
{
   if (i >= 0 && i < 256)
   {
      *red = (char) (((float)palette[i].peRed) / 255.0 * 63.0);
      *green = (char)(((float)palette[i].peGreen) / 255.0 * 63.0);
      *blue = (char) (((float)palette[i].peBlue) / 255.0 * 63.0);
   }
}

void clear_scr(int color, int row)
{
	unsigned char *buffer_ptr = lock_xbuff();
   int n = (row + 1) * SCREEN_PITCH;
   memset(buffer_ptr,color,n);
	free_xbuff();
}

void clear_scr(int color)
{
   clear_scr(color,MAX_Y);
}

void fill_rect(Rect &r, int color)
{
	if (valid_rect(r))
	{
		unsigned char *buffer_ptr = lock_xbuff();
		Rect r0 = r;
		cliprect2rect(cliprect,r0);
		int h = RHEIGHT(r0);
		int w = RWIDTH(r0);
		unsigned char *ptr = 
		buffer_ptr + r0.left() + (r0.top() * SCREEN_PITCH);
		for (int i=0;i<h;i++)
		{
			memset(ptr,(char)color,w);
			ptr += SCREEN_PITCH;
		}
		free_xbuff();
	}
}

void blit_buff()
{
	if (lpddsprimary->IsLost() == DDERR_SURFACELOST)
		lpddsprimary->Restore();
	while (lpddsprimary->Flip(NULL,DDFLIP_WAIT) != DD_OK);
}

static unsigned char *bffr_ptr = NULL;
void putpixel(int x, int y, int color)
{
	int flag = 0;
	if (bffr_ptr == NULL)
	{
		bffr_ptr = lock_xbuff();
		flag = 1;
	}
   if (x >= 0 && x<= MAX_X && y >= 0 && y <= MAX_Y)
      *(bffr_ptr + (y * SCREEN_PITCH) + x) = (char) color;
	if (flag)
	{
		free_xbuff();
		bffr_ptr = NULL;
	}
}

void wvga_line(int x1, int y1, int x2, int y2, int color)
{
	bffr_ptr = lock_xbuff();
   DDALine daline(x1,y1,x2,y2);
   do
      {
         putpixel(daline.X(),daline.Y(),color);
      } while (daline.step());
	free_xbuff();
	bffr_ptr = NULL;
}

void b_linedraw(int x1, int y1, int x2, int y2, int color, Rect *bounds)
{
	if (bounds)
	{
		int points[4];
		points[0] = x1;
		points[1] = y1;
		points[2] = x2;
		points[3] = y2;
		if (!ch_clip(points,bounds))
			return;
		x1 = points[0];
		y1 = points[1];
		x2 = points[2];
		y2 = points[3];
	}
	wvga_line(x1,y1,x2,y2,color);
}


void h_line(int x, int y, int len, int color)
{
	unsigned char *buffer_ptr = lock_xbuff();
	register unsigned char *bf = buffer_ptr + (y * SCREEN_PITCH) + x;
	while (len--)
	{
		*bf++ = (char ) color;
	}
	free_xbuff();
}

void v_line(int x, int y, register int len, int color)
{
	unsigned char *buffer_ptr = lock_xbuff();
	register unsigned char *bf = buffer_ptr + (y * SCREEN_PITCH) + x;
	while(len--)
	{
		*bf = (char ) color;
		bf += SCREEN_PITCH;
	}
	free_xbuff();
}

void set_palette(int , int , char *)
{
   /*
   char *pptr = palette + (startcolor * 3);
   for (int i=0;i<count;i++)
   {
      set_rgb_value(i,(int) *pptr, (int) *pptr+1, (int) *pptr+2);
      pptr += 3;
   }
   */
   /*
   dib_sect.newPalette(rgb_colors,hwnd);
   buffer_ptr = dib_sect.getBits();
   xbuffer = buffer_ptr;
   screen_ptr = buffer_ptr;
   */
}

void get_palette(int startcolor, int count, char *palette)
{
  char *pptr = palette + (startcolor * 3);
	for (int i=0;i<count;i++)
  {
      get_rgb_value(i,pptr,pptr+1,pptr+2);
      pptr += 3;
  }
}

unsigned char *lock_xbuff()
{
	lpddssecondary->Lock(NULL,&ddsd,DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT,NULL);
	SCREEN_PITCH = ddsd.lPitch;
	return (unsigned char *) ddsd.lpSurface;
}

void free_xbuff()
{
	lpddssecondary->Unlock(ddsd.lpSurface);
}

void vga13_suspend()
{
	lpdd->FlipToGDISurface();
}

void vga13_resume()
{
}

void vga13_drawprep()
{
}

extern int frame_switch;

void vga13_begin_scene()
{
	float fogStart = 0.999;
	float fogEnd = 1.0;
	float fogDensity = 0.0001;
	lpd3dDevice->BeginScene();
	 DDBLTFX ddbltfx;
	ddbltfx.dwSize = sizeof(ddbltfx);
	ddbltfx.dwFillColor = 0;
	lpddssecondary->Blt(NULL,NULL,NULL,DDBLT_COLORFILL | DDBLT_WAIT,&ddbltfx);
	ddbltfx.dwFillDepth = 0xffff;
	// ddbltfx.dwFillDepth = 0;
	lpddszbuffer->Blt(NULL,NULL,NULL,DDBLT_DEPTHFILL | DDBLT_WAIT,&ddbltfx);
	
	lpd3dDevice->SetRenderState(D3DRENDERSTATE_SHADEMODE,D3DSHADE_FLAT);

	lpd3dDevice->SetRenderState(D3DRENDERSTATE_CULLMODE,D3DCULL_NONE);
	lpd3dDevice->SetLightState(D3DLIGHTSTATE_COLORMODEL,D3DCOLOR_RGB);

	lpd3dDevice->SetLightState(D3DLIGHTSTATE_MATERIAL,hMat);

	lpd3dDevice->SetRenderState(D3DRENDERSTATE_TEXTUREPERSPECTIVE,TRUE);
 

	lpd3dDevice->SetRenderState(D3DRENDERSTATE_FOGTABLEMODE,D3DFOG_LINEAR);
	lpd3dDevice->SetRenderState(D3DRENDERSTATE_FOGTABLESTART,*(DWORD *) &fogStart);
	lpd3dDevice->SetRenderState(D3DRENDERSTATE_FOGTABLEEND,*(DWORD *) &fogEnd);
	lpd3dDevice->SetRenderState(D3DRENDERSTATE_FOGTABLEDENSITY,*(DWORD *)&fogDensity);

	if (frame_switch)
	{
		lpd3dDevice->SetRenderState(D3DRENDERSTATE_FILLMODE,D3DFILL_WIREFRAME);
		lpd3dDevice->SetRenderState(D3DRENDERSTATE_ZENABLE,FALSE);
		lpd3dDevice->SetRenderState(D3DRENDERSTATE_TEXTUREHANDLE,NULL);
		lpd3dDevice->SetRenderState(D3DRENDERSTATE_FOGENABLE,FALSE);
	}
	else
	{
		lpd3dDevice->SetRenderState(D3DRENDERSTATE_FILLMODE,D3DFILL_SOLID);
		lpd3dDevice->SetRenderState(D3DRENDERSTATE_ZENABLE,TRUE);
		lpd3dDevice->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE,TRUE);
		lpd3dDevice->SetRenderState(D3DRENDERSTATE_FOGENABLE,FALSE);
	}

}

void vga13_end_scene()
{
	lpd3dDevice->EndScene();
}

void wvgad3_rndrpolyf(TRF_2DPoint *points, int np, TextrMap *tmap,  int fillcolor);

void wvgad3_rndrpoly(R_3DPoint *points, TxtPoint *tpoints, int np, 
							int fillColor, TextrMap *tmap, Port_3D &port)
{
int zcount;
int i;
REAL_TYPE zmin = 1.0;
int pp;
int sp;
TR_3DPoint *ppoints;
TR_3DPoint portPoints[32];
TR_3DPoint portPoints_cl[32];
TRF_2DPoint screenPoints[32];
TRF_2DPoint screenPoints_cl[32];

	if (np > 12)
		return;
	zcount = 0;
	for (i=0;i<np;i++)
	{
		port.world2port(points[i],tpoints[i],&portPoints[i]);
		if (portPoints[i].r.z < zmin)
			zcount++;
	}
	if (zcount == np)
		return;
	else if (zcount == 0)
	{
		pp = np;
		ppoints = portPoints;
	}
	else
	{
		zpoly_clip(portPoints,np,portPoints_cl,&pp,zmin);
		ppoints = portPoints_cl;
	}

	if (pp)
	{
		for (i=0;i<pp;i++)
			port.port2screen(ppoints[i],&screenPoints[i]);
		lock_xbuff();
		wvgad3_rndrpolyf(screenPoints,pp,tmap,fillColor);
		free_xbuff();
	}
}

static float mapw=1.0,maph=1.0;

inline void R2D3D(const TRF_2DPoint &rp, D3DTLVERTEX &vtx, D3DCOLOR col)
{
	vtx.sx = rp.x;
	vtx.sy = rp.y;
	vtx.sz = 1.0 - (1.0 / (rp.z));
	vtx.rhw = 1.0 / (rp.z);
	vtx.color = col;
	vtx.specular = RGB_MAKE(0,0,0);
	vtx.tu = rp.u / mapw;
	vtx.tv = rp.v / maph;
}

extern int frame_switch;

void wvgad3_rndrpolyf(TRF_2DPoint *points, int np, TextrMap *tmap,  int fillcolor)
{
D3DTLVERTEX tlvtx[12];
D3DCOLOR col;
char r,g,b;
int i;
		if (tmap)
			lpd3dDevice->SetRenderState(D3DRENDERSTATE_TEXTUREHANDLE, tmap->Handle);
		else
			lpd3dDevice->SetRenderState(D3DRENDERSTATE_TEXTUREHANDLE,NULL);
	if (!frame_switch && tmap)
	{
		tmap->Load(lpd3dDevice,NULL);
		mapw = tmap->map_w;
		maph = tmap->map_h;
		col = RGB_MAKE(255,255,255);
	}
	else
	{
		get_rgb_value(fillcolor,&r,&g,&b);
		col = RGB_MAKE(r,g,b);
	}
	if (frame_switch)
	{
		col = RGB_MAKE(255,255,255);
		lpd3dDevice->SetRenderState(D3DRENDERSTATE_TEXTUREHANDLE,NULL);
	}
	if (np == 3)
	{
		R2D3D(points[0], tlvtx[0], col);
		R2D3D(points[1], tlvtx[1], col);
		R2D3D(points[2], tlvtx[2], col);
	}
	else if (np == 4)
	{
		R2D3D(points[0], tlvtx[0], col);
		R2D3D(points[1], tlvtx[1], col);
		R2D3D(points[2], tlvtx[2], col);
		R2D3D(points[3], tlvtx[3], col );

	} else if (np == 5)
	{
		R2D3D(points[0], tlvtx[0], col);
		R2D3D(points[1], tlvtx[1], col);
		R2D3D(points[2], tlvtx[2], col);
		R2D3D(points[3], tlvtx[3], col );
		R2D3D(points[4], tlvtx[4], col );
	}
	else 
	{
		for (i=np-1;i>=0;i--)
		{
			D3DTLVERTEX &vtx = tlvtx[i];
			R2D3D(points[i], vtx, col);
		}
	}
	ddraw_err_check(lpd3dDevice->DrawPrimitive(D3DPT_TRIANGLEFAN,D3DVT_TLVERTEX,tlvtx,np,
		D3DDP_WAIT/*|D3DDP_DONOTCLIP*/),"DrawPrimitive");
}

void wvgad3_rndrpoly(TR_2DPoint *points, int np, TextrMap *tmap,  int fillcolor)
{
D3DTLVERTEX tlvtx[12];
D3DCOLOR col;
char r,g,b;
float mapw,maph;
float tu,tv;

	tmap->Load(lpd3dDevice,NULL);
	lpd3dDevice->SetRenderState(D3DRENDERSTATE_TEXTUREHANDLE, tmap->Handle);
	lpd3dDevice->SetRenderState(D3DRENDERSTATE_TEXTUREPERSPECTIVE,TRUE);
	lpd3dDevice->SetRenderState(D3DRENDERSTATE_TEXTUREMAPBLEND,D3DTBLEND_COPY);
	get_rgb_value(fillcolor, &r,&g,&b);
		col = RGB_MAKE(palette[fillcolor].peRed, palette[fillcolor].peGreen,palette[fillcolor].peBlue);
	mapw = tmap->map_w;
	maph = tmap->map_h;

	for (int i=0;i<np;i++)
	{
		float zv = ((float)points[i].z) / ((float)zfact);

		D3DTLVERTEX &vtx = tlvtx[i];
		vtx.sx = ((float)points[i].x);
		vtx.sy = ((float)points[i].y);
		vtx.sz = zv;
		vtx.rhw = 1 / zv;
		vtx.color = col;
		vtx.specular = RGB_MAKE(0,0,0);

		tu = ((float)points[i].u / zfact) / zv;
		tv = ((float)points[i].v / zfact) / zv;

		vtx.tu = tu / mapw;
		vtx.tv = tv / maph;
	}
	ddraw_err_check(lpd3dDevice->DrawPrimitive(D3DPT_TRIANGLEFAN,D3DVT_TLVERTEX,tlvtx,np,
		D3DDP_WAIT | D3DDP_DONOTCLIP),"DrawPrimitive");
}


int ddraw_err_check(HRESULT ddresult, char *s)
{
	char *errmsg;

	if (ddresult != DD_OK)
	{
		errmsg = display_ddx_error(ddresult,s);
		error_jump("%s",errmsg);
	}
	return 0;
}



char *display_ddx_error(HRESULT err, char *s)
{
	if (s == NULL)
		s = "DDX Error";
	char *errString = dderr_2_string(err);
	static char errmssg[256];
	sprintf(errmssg,"%s %s", s, errString);
	vga13_suspend();
	MessageBox(NULL,errmssg,s,MB_OK);
	sim_printf("%s\n",errmssg);
	return errmssg;
}

static char *dderr_2_string(HRESULT dderr)
{
	char *result = "Unknown";
	switch (dderr)
	{
	case DDERR_GENERIC :
		return "DDERR_GENERIC";

	case DDERR_INVALIDMODE :
		return "DDERR_INVALIDMODE";

	case DDERR_INVALIDOBJECT :
		return "DDERR_INVALIDOBJECT";

	case DDERR_INVALIDPARAMS :
		return "DDERR_INVALIDPARAMS";

	case DDERR_LOCKEDSURFACES :
		return "DDERR_LOCKEDSURFACES";

	case DDERR_NOEXCLUSIVEMODE :
		return "DDERR_NOEXCLUSIVEMODE";

	case DDERR_SURFACEBUSY :
		return "DDERR_SURFACEBUSY";

	case DDERR_UNSUPPORTED :
		return "DDERR_UNSUPPORTED";

	case DDERR_UNSUPPORTEDMODE :
		return "DDERR_UNSUPPORTEDMODE";

	case DDERR_WASSTILLDRAWING :
		return "DDERR_WASSTILLDRAWING";

	case DDERR_OUTOFMEMORY:
		return "DDERR_OUTOFMEMORY";
		break;

	case D3DERR_BADMAJORVERSION:
		return "D3DERR_BADMAJORVERSION";

	case D3DERR_DEVICEAGGREGATED:
		return "D3DERR_DEVICEAGGREGATED";
		
	case D3DERR_EXECUTE_CLIPPED_FAILED:
		return "D3DERR_EXECUTE_CLIPPED_FAILED";

	case D3DERR_EXECUTE_CREATE_FAILED:
		return "D3DERR_EXECUTE_CREATE_FAILED";

	case D3DERR_EXECUTE_DESTROY_FAILED:	
		return "D3DERR_EXECUTE_DESTROY_FAILED";

	case D3DERR_EXECUTE_FAILED:
		return "D3DERR_EXECUTE_FAILED";

	case D3DERR_EXECUTE_LOCK_FAILED:
		return "D3DERR_EXECUTE_LOCK_FAILED";

	case D3DERR_EXECUTE_LOCKED:
		return "D3DERR_EXECUTE_LOCKED";
	
	case D3DERR_EXECUTE_NOT_LOCKED:
		return "D3DERR_EXECUTE_NOT_LOCKED";

	case D3DERR_EXECUTE_UNLOCK_FAILED:
		return "D3DERR_EXECUTE_UNLOCK_FAILED";

	case D3DERR_INITFAILED:
		return "D3DERR_INITFAILED";

	case D3DERR_INBEGIN:
		return "D3DERR_INBEGIN";

	case D3DERR_INVALID_DEVICE:
		return "D3DERR_INVALID_DEVICE";

	case D3DERR_INVALIDCURRENTVIEWPORT:
		return "D3DERR_INVALIDCURRENTVIEWPORT";

	case D3DERR_INVALIDPALETTE:
		return "D3DERR_INVALIDPALETTE";

	case D3DERR_INVALIDPRIMITIVETYPE:
		return "D3DERR_INVALIDPRIMITIVETYPE";

	case D3DERR_INVALIDRAMPTEXTURE:
		return "D3DERR_INVALIDRAMPTEXTURE";

	case D3DERR_INVALIDVERTEXTYPE:
		return "D3DERR_INVALIDVERTEXTYPE";

	case D3DERR_LIGHT_SET_FAILED:
		return "D3DERR_LIGHT_SET_FAILED";

	case D3DERR_LIGHTHASVIEWPORT:
		return "D3DERR_LIGHTHASVIEWPORT";

	case D3DERR_LIGHTNOTINTHISVIEWPORT:
		return "D3DERR_LIGHTNOTINTHISVIEWPORT";

	case D3DERR_MATERIAL_CREATE_FAILED:
		return "D3DERR_MATERIAL_CREATE_FAILED";

	case D3DERR_MATERIAL_DESTROY_FAILED:
		return "D3DERR_MATERIAL_DESTROY_FAILED";

	case D3DERR_MATERIAL_GETDATA_FAILED:
		return "D3DERR_MATERIAL_GETDATA_FAILED";

	case D3DERR_MATERIAL_SETDATA_FAILED:
		return "D3DERR_MATERIAL_SETDATA_FAILED";

	case D3DERR_MATRIX_CREATE_FAILED:
		return "D3DERR_MATRIX_CREATE_FAILED";

	case D3DERR_MATRIX_DESTROY_FAILED:
		return "D3DERR_MATRIX_DESTROY_FAILED";

	case D3DERR_MATRIX_GETDATA_FAILED:
		return "D3DERR_MATRIX_GETDATA_FAILED";

	case D3DERR_MATRIX_SETDATA_FAILED:
		return "D3DERR_MATRIX_SETDATA_FAILED";

	case D3DERR_NOCURRENTVIEWPORT:
		return "D3DERR_NOCURRENTVIEWPORT";

	case D3DERR_NOTINBEGIN:
		return "D3DERR_NOTINBEGIN";

	case D3DERR_NOVIEWPORTS:
		return "D3DERR_NOVIEWPORTS";

	case D3DERR_SCENE_BEGIN_FAILED:
		return "D3DERR_SCENE_BEGIN_FAILED";

	case D3DERR_SCENE_END_FAILED:
		return "D3DERR_SCENE_END_FAILED";

	case D3DERR_SCENE_IN_SCENE:
		return "D3DERR_SCENE_IN_SCENE";

	case D3DERR_SCENE_NOT_IN_SCENE:
		return "D3DERR_SCENE_NOT_IN_SCENE";

	case D3DERR_SETVIEWPORTDATA_FAILED:
		return "D3DERR_SETVIEWPORTDATA_FAILED";

	case D3DERR_SURFACENOTINVIDMEM:
		return "D3DERR_SURFACENOTINVIDMEM";

	case D3DERR_TEXTURE_BADSIZE:
		return "D3DERR_TEXTURE_BADSIZE";

	case D3DERR_TEXTURE_CREATE_FAILED:
		return "D3DERR_TEXTURE_CREATE_FAILED";

	case D3DERR_TEXTURE_DESTROY_FAILED:
		return "D3DERR_TEXTURE_CREATE_FAILED";

	case D3DERR_TEXTURE_GETSURF_FAILED:
		return "D3DERR_TEXTURE_GETSURF_FAILED";

	case D3DERR_TEXTURE_LOAD_FAILED:
		return "D3DERR_TEXTURE_LOAD_FAILED";

	case D3DERR_TEXTURE_LOCK_FAILED:
		return "D3DERR_TEXTURE_LOCK_FAILED";

	case D3DERR_TEXTURE_LOCKED:
		return "D3DERR_TEXTURE_LOCKED";

	case D3DERR_TEXTURE_NO_SUPPORT:
		return "D3DERR_TEXTURE_NO_SUPPORT";

	case D3DERR_TEXTURE_NOT_LOCKED:
		return "D3DERR_TEXTURE_NOT_LOCKED";

	case D3DERR_TEXTURE_SWAP_FAILED:
		return "D3DERR_TEXTURE_SWAP_FAILED";

	case D3DERR_TEXTURE_UNLOCK_FAILED:
		return "D3DERR_TEXTURE_UNLOCK_FAILED";

	case D3DERR_VIEWPORTDATANOTSET:
		return "D3DERR_VIEWPORTDATANOTSET";

	case D3DERR_VIEWPORTHASNODEVICE:
		return "D3DERR_VIEWPORTHASNODEVICE";

	case D3DERR_ZBUFF_NEEDS_SYSTEMMEMORY:
		return "D3DERR_ZBUFF_NEEDS_SYSTEMMEMORY";

	case D3DERR_ZBUFF_NEEDS_VIDEOMEMORY:
		return "D3DERR_ZBUFF_NEEDS_VIDEOMEMORY";
 	}
	return result;
}

