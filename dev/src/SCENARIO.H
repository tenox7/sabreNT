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
 * Version: 0.1                                  *
 * File   : scenario.h                           *
 * Date   : March, 1997                          *
 * Author : Dan Hammer                           *
 *************************************************/
#ifndef __scenario_h
#define __scenario_h

#define SCENARIO_PATH_SIZE 13

class SimScenario
{
public:
   char title[48];
   char flight_file[SCENARIO_PATH_SIZE];
   char ground_file[SCENARIO_PATH_SIZE];
   int  has_ground_file;
   char world_file[SCENARIO_PATH_SIZE];
   int  has_world_file;
   int  command_no;
   int  randomize_positions;

   SimScenario();

   void read(istream &);
   void write(ostream &);

   static SimScenario *scenarios;
   static int          nscenarios;
   static int readFile(char *path);
   static int writeFile(char *path);
   static SimScenario *cmd2scn(int cmd);
   static SimScenario *getScenario(int i);
   static int start_cmd_no;
};

#endif
