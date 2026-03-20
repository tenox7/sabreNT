/*
    Sabre Fighter Plane Simulator 
    Copyright (c) 1997/1998 Dan Hammer
    Portions Donated By Antti Barck
	
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
 *         Sabre Fighter Plane Simulator         *
 * File   : sftlmdl.C                            *
 * Date   : Apirl, 1998                          *
 * Author : Dan Hammer                           *
 *************************************************/
#include "aifltmdl.h"
 
/*************************************************
 * Call back prior to final calc of vector forces*
 *************************************************/
void aiFlightModel::DoVForcesCallback()
{
  if (
      (updateCallback != NULL ) &&
      ( callbackFlags & AIFMCB_VFORCES_UPDATE) 
      )
    (*updateCallback)(VFORCES_UPDATE,iter,this,callbackData);
}

/************************************************
 * Call back prior to final calc of rotational  *
 * forces                                       *
 ************************************************/
void aiFlightModel::DoRForcesCallback()
{
  if (
      (updateCallback != NULL ) &&
      ( callbackFlags & AIFMCB_RFORCES_UPDATE) 
      )
    (*updateCallback)(RFORCES_UPDATE,iter,this,callbackData);
}

/**********************************************
 * Call back prior to calc of new position    *
 * based on current vector forces             *
 **********************************************/
void aiFlightModel::DoPositionCallback()
{
  if (
      (updateCallback != NULL ) &&
      ( callbackFlags & AIFMCB_POSITION_UPDATE) 
      )
    (*updateCallback)(POSITION_UPDATE,iter,this,callbackData);
}

/**********************************************
 * Call back priot to calc of new orientation *
 * based on current rotational forces         *
 **********************************************/
void aiFlightModel::DoOrientationCallback()
{
  if (
      (updateCallback != NULL ) &&
      ( callbackFlags & AIFMCB_ORIENTATION_UPDATE) 
      )
    (*updateCallback)(ORIENTATION_UPDATE,iter,this,callbackData);
}

/*********************************************
 * Call back for setting of flight controls  *
 *********************************************/
void aiFlightModel::DoControlCallback()
{
  if (
      (updateCallback != NULL ) &&
      ( callbackFlags & AIFMCB_CONTROL_UPDATE) 
      )
    (*updateCallback)(CONTROL_UPDATE,iter,this,callbackData);
}




