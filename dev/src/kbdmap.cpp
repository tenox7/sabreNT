/*
    Sabre Fighter Plane Simulator 
    Copyright (c) 1997 Dan Hammer
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
 *           Sabre Fighter Plane Simulator       *
 * File   : kbdmap.cpp                           *
 * Date   : March, 1997                          *
 * Author : Dan Hammer                           *
 *************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream.h>
#include <fstream.h>
#include <math.h>
#include <limits.h>
#include <values.h>
#include "defs.h"
#include "pc_keys.h"
#include "key_map.h"
#include "vmath.h"
#include "port_3d.h"
#include "sim.h"
#include "cpoly.h"
#include "obj_3d.h"
#include "copoly.h"
#include "input.h"
#include "rtkey.h"
#include "simfile.h"
#include "flight.h"
#include "flight.h"
#include "fltlite.h"
#include "weapons.h"
#include "pilot.h"
#include "zview.h"
#include "fltzview.h"
#include "unguided.h"
#include "fltmngr.h"
#include "terrain.h"
#include "kbdhit.h"
#include "siminput.h"
#include "globals.h"

KeyMapEntry FlightInput::kbdMap[256];

