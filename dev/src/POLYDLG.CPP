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
 * File   : polydlg.cpp                          *
 * Date   : December, 1997                       *
 * Author : Dan Hammer                           *
 * Port to Windows NT/95                         *
 *************************************************/
#include <windows.h>
#include <stdio.h>
#include <math.h>
#include "vmath.h"
#include "zvedtwin.rh"
#include "polydlg.h"

int rpoint::instance_count = 0;
HWND dlgwin;
HDC  dlgdc;
PAINTSTRUCT dlgpnt;
rpoint *rp_head;
rpoint *rp_tail;
rpoint *ins0;
rpoint *ins1;
rpoint  *sel = NULL;
int   inited = 0;
RECT  dlgRect;
RECT  actvRect;
float dlg_cx;
float dlg_cy;
float dlg_w;
float dlg_h;
int   dlg_mx;
int   dlg_my;
float rnding = 100.0;
float step   = 0.01;

static BOOL handleCommand(WPARAM wparam, LPARAM lparam);
static BOOL handleLMouseClick(WPARAM wparam, LPARAM lparam);
static BOOL handleRMouseClick(WPARAM wparam, LPARAM lparam);
static void doInitDialog();
static void doInit();
static void drawPoints(HDC dc = NULL);
static void drawDialog(HDC dc = NULL);
static void bumpInsertion();
static void bumpSelection();
static void showControls();
static void deleteSelectedPoint();

inline float roundoff(float val)
{
   float dv = floor(val * rnding);
   return (dv / rnding);
}

inline rpoint *rlink(const R_3DPoint &pnt, rpoint *r0)
{
   rpoint *nxt = r0->next;
   r0->next = new rpoint(pnt);
   r0->next->next = nxt;
   return (r0->next);
}

rpoint *xlink(rpoint *r0)
{
   if (r0 == NULL)
      return(NULL);

   if (r0 == rp_head)
   {
      rpoint *ptr = rp_head->next;
      delete rp_head;
      rp_head = ptr;
      return (rp_head);
   }

   rpoint *ptr = rp_head->next;
   rpoint *ptr0 = rp_head;
   while (ptr != r0 && ptr != NULL)
   {
      ptr0 = ptr;
      ptr = ptr->next;
   }

   if (ptr != NULL)
   {
      ptr0->next = ptr->next;
   }

   delete r0;
   return (ptr0);
}

inline void toScreenPoints(const R_3DPoint &p, int *xx, int *yy)
{
   *xx = (p.x / 2.0 * dlg_w) + dlg_cx;
   *yy = (p.y / 2.0 * dlg_h) + dlg_cy;
}


inline void toRealPoint(int xx, int yy, R_3DPoint *p)
{
   float xxf = xx - dlg_cx;
   float yyf = yy - dlg_cy;

   p->x = xxf * 2.0 / dlg_w;
   p->x = roundoff(p->x);
   p->y = yyf * 2.0 / dlg_h;
   p->y = roundoff(p->y);
}

inline void toRealPoint(LPARAM prm, R_3DPoint *p)
{
   toRealPoint((int) LOWORD(prm),(int) HIWORD(prm),p);
}

BOOL CALLBACK PolyDialogProc(HWND hdlg, UINT msg, WPARAM wparam,
                              LPARAM lparam)
{
   dlgwin = hdlg;

   switch (msg)
   {
      case WM_INITDIALOG:
      doInitDialog();
      return(TRUE);

      case WM_PAINT:
      dlgdc = BeginPaint(dlgwin,&dlgpnt);
      drawDialog(dlgdc);
      EndPaint(dlgwin,&dlgpnt);
      dlgdc = NULL;
      return(TRUE);

      case WM_COMMAND:
      return (handleCommand(wparam,lparam));

      case WM_LBUTTONDOWN:
      return (handleLMouseClick(wparam,lparam));

      case WM_RBUTTONDOWN:
      return (handleRMouseClick(wparam,lparam));

   }

   return (FALSE);
}


BOOL handleLMouseClick(WPARAM , LPARAM lparam)
{
   POINT pnt;
   R_3DPoint pt;

   pnt.x = LOWORD(lparam);
   pnt.y = HIWORD(lparam);
   if (PtInRect(&actvRect,pnt))
   {
      toRealPoint(lparam, &pt);
      rpoint *rpnt = rlink(pt,ins0);
      ins1 = sel = rpnt;
      InvalidateRect(dlgwin,NULL,TRUE);
      return (TRUE);
   }
   else
      return (FALSE);
}

BOOL handleRMouseClick(WPARAM , LPARAM )
{
   return (FALSE);
}

void bumpInsertion()
{
   ins0 = ins0->next;
   if (ins0 == NULL)
      ins0 = rp_head;
   ins1 = ins1->next;
   if (ins1 == NULL)
      ins1 = rp_head;
   InvalidateRect(dlgwin,NULL,TRUE);
}

void bumpSelection()
{
   sel = sel->next;
   if (sel == NULL)
      sel = rp_head;
   InvalidateRect(dlgwin,NULL,TRUE);
}

BOOL handleCommand(WPARAM wparam, LPARAM )
{
   switch (wparam)
   {
      case IDOK:
      EndDialog(dlgwin,1);
      return (TRUE);

      case IDCANCEL:
      EndDialog(dlgwin,0);
      return (TRUE);

      case IDC_SEL:
      bumpSelection();
      return (TRUE);

      case IDC_INS:
      bumpInsertion();
      return (TRUE);

      case IDC_UP:
      if (sel)
      {
         sel->pnt.y += step;
         InvalidateRect(dlgwin,NULL,TRUE);
      }
      return (TRUE);

      case IDC_DOWN:
      if (sel)
      {
         sel->pnt.y -= step;
         InvalidateRect(dlgwin,NULL,TRUE);
      }
      return (TRUE);

      case IDC_RIGHT:
      if (sel)
      {
         sel->pnt.x += step;
         InvalidateRect(dlgwin,NULL,TRUE);
      }
      return (TRUE);

      case IDC_LEFT:
      if (sel)
      {
         sel->pnt.x -= step;
         InvalidateRect(dlgwin,NULL,TRUE);
      }
      return (TRUE);

      case IDC_DEL:
      deleteSelectedPoint();
      InvalidateRect(dlgwin,NULL,TRUE);
      return (TRUE);

      default:
      return(FALSE);
   }
}

void deleteSelectedPoint()
{
   if (!(sel && rpoint::instance_count > 3))
      return;
   if (sel != ins0 && sel != ins1)
   {
      sel = xlink(sel);
      return;
   }

   if (sel == ins0)
   {
      sel = xlink(sel);
      ins0 = sel;
   }
   else
   {
      sel = xlink(sel);
      ins1 = sel;      
   }
   if (ins0 == ins1)
   {
      ins1 = ins0->next;
      if (ins1 == NULL)
         ins1 = rp_head;
   }
}

void doInitDialog()
{
   if (!inited)
      doInit();

   GetClientRect(dlgwin,&dlgRect);
   actvRect = dlgRect;

   actvRect.top = 10;
   actvRect.left = 20;
   actvRect.bottom -= 60;
   actvRect.right -= 20;

   dlg_w = actvRect.right - actvRect.left + 1;
   dlg_h = actvRect.bottom - actvRect.top + 1;
   dlg_cx = actvRect.left + dlg_w / 2.0;
   dlg_cy = actvRect.top + dlg_h / 2.0;
}

void doInit()
{
   rp_head = new rpoint(R_3DPoint(0,-0.5,0));
   rp_tail = new rpoint(R_3DPoint(-0.5,0.5,0));
   rp_head->next = rp_tail;
   rlink(R_3DPoint(0.5,0.5,0),rp_head);
   ins0 = rp_head;
   ins1 = rp_head->next;
   sel = ins0;
   inited = 1;      
}

void drawDialog(HDC dc)
{
   HPEN pn,op;
   int dcflg = 0;
   if (dc == NULL)
   {
      dc = GetDC(dlgwin);
      dcflg = 1;
   }
   pn = CreatePen(PS_SOLID,1,RGB(0,0,0));
   op = SelectObject(dc,pn);   
   Rectangle(dc,actvRect.left,actvRect.top,actvRect.right,actvRect.bottom);
   drawPoints(dc);
   SelectObject(dc,op);
   DeleteObject(pn);
   if (dcflg)
      ReleaseDC(dlgwin,dc);   
   showControls();
}

void drawPoints(HDC dc)
{
   int   ix,iy,ix0,iy0;
   int dcflg = 0;
   HPEN vpn,vpn0,vpn1,lpn,oldpn;
   rpoint *rpt;

   if (rp_head == NULL)
      return;

   if (dc == NULL)
   {
      dc = GetDC(dlgwin);
      dcflg = 1;
   }


   vpn = CreatePen(PS_SOLID,5,RGB(0,0,255));
   vpn0 = CreatePen(PS_SOLID,5,RGB(0,255,0));
   vpn1 = CreatePen(PS_SOLID,5,RGB(255,0,0));
   lpn = CreatePen(PS_SOLID,1,RGB(0,0,0));
   oldpn = SelectObject(dc,vpn);

   if (sel != NULL)
   {
      toScreenPoints(sel->pnt,&ix,&iy);
      SelectObject(dc,lpn);
      Rectangle(dc,ix-6,iy-6,ix+6,iy+6);
   }

   toScreenPoints(rp_head->pnt,&ix0,&iy0);

   rpt = rp_head->next;
   while (rpt != NULL)
   {
      toScreenPoints(rpt->pnt,&ix,&iy);
      SelectObject(dc,lpn);
      MoveToEx(dc,ix0,iy0,NULL);
      LineTo(dc,ix,iy);
      if (rpt == ins0)
         SelectObject(dc,vpn0);
      else if (rpt == ins1)
         SelectObject(dc,vpn1);
      else
         SelectObject(dc,vpn);   
      MoveToEx(dc,ix,iy,NULL);
      LineTo(dc,ix,iy);
      ix0 = ix;
      iy0 = iy;
      rpt = rpt->next;
   }

   rpt = rp_head;
   toScreenPoints(rpt->pnt,&ix,&iy);
   SelectObject(dc,lpn);
   MoveToEx(dc,ix0,iy0,NULL);
   LineTo(dc,ix,iy);
   if (rpt == ins0)
      SelectObject(dc,vpn0);
   else if (rpt == ins1)
      SelectObject(dc,vpn1);
   else
      SelectObject(dc,vpn);   
   MoveToEx(dc,ix,iy,NULL);
   LineTo(dc,ix,iy);


      
   SelectObject(dc,oldpn);
   if (dcflg)
      ReleaseDC(dlgwin,dc);
   DeleteObject(vpn);
   DeleteObject(vpn0);
   DeleteObject(vpn1);
   DeleteObject(lpn);
}

void showControls()
{
   char buff[64];
   sprintf(buff,"%03.2f",sel->pnt.x);
   SendDlgItemMessage(dlgwin,IDC_EDITX,WM_SETTEXT,(WPARAM)0,(LPARAM)buff);
   sprintf(buff,"%03.2f",sel->pnt.y);
   SendDlgItemMessage(dlgwin,IDC_EDITY,WM_SETTEXT,(WPARAM)0,(LPARAM)buff);
}
