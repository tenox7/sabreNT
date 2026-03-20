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
 * File   : scenario.cpp                         *
 * Date   : April, 1998                          *
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
#include "vmath.h"
#include "grafix.h"
#include "simerr.h"
#include "simfile.h"
#include "scenario.h"

SimScenario *SimScenario::scenarios = NULL;
int SimScenario::nscenarios = 0;
int SimScenario::start_cmd_no = 200;

SimScenario::SimScenario()
{
   strcpy(title,"Fly");
   strcpy(flight_file,"a.flt");
   strcpy(ground_file,"");
   strcpy(world_file,"");
   has_ground_file = has_world_file = 0;
   command_no = start_cmd_no;
   randomize_positions = 0;
}

void SimScenario::read(istream &is)
{
   char c;
   char title_buff[50];
   READ_TOKI('{',is,c);
   get_line(is,title_buff,sizeof(title_buff));
   strncpy(title,title_buff,sizeof(title));
   is >> flight_file;
   is >> ground_file;
   is >> world_file;
   is >> randomize_positions;
   READ_TOK('}',is,c);
   has_ground_file = !(*ground_file == '*' || *ground_file == ' ');
   has_world_file = !(*world_file == '*' || *world_file == ' ');
}

void SimScenario::write(ostream &os)
{
   os << "{\n";
   os << title << "\n";
   os << flight_file << "\n";
   if (has_ground_file)
      os << ground_file << "\n";
   else
      os << "*\n";
   if (has_world_file)
      os << world_file << "\n";
   else
      os << "*\n";
   os << randomize_positions << '\n';
   os << "{\n";
}

int SimScenario::readFile(char *path)
{
   if (scenarios != NULL)
      delete [] scenarios;
   scenarios = NULL;
   nscenarios = 0;
 
   ifstream is;
   if (open_is(is,path))
   {
      is >> nscenarios;
      scenarios = new SimScenario[nscenarios];   
      MYCHECK(scenarios != NULL);
      int ncmd = start_cmd_no;
      for (int i=0;i<nscenarios;i++)
      {
         scenarios[i].read(is);
         scenarios[i].command_no = ncmd++;
      }
   }
   return (nscenarios);
}

int SimScenario::writeFile(char *path)
{
   ofstream os;
   if (open_libos(os,path))
   {
      os << nscenarios << "\n";
      for (int i=0;i<nscenarios;i++)
         scenarios[i].write(os);
   }
   return (nscenarios);
}

SimScenario *SimScenario::cmd2scn(int cmd)
{
   SimScenario *result = NULL;
   for (int i=0;i<nscenarios;i++)
   {
      if (scenarios[i].command_no == cmd)
      {
         result = &scenarios[i];
         break;
      }
   }
   if (!result)
      sim_printf("Menu Command %d Not Found\n",cmd);
   return (result);
}

SimScenario *SimScenario::getScenario(int i)
{
   SimScenario *result = NULL;
   if (i >= 0 && i < nscenarios)
      result = &scenarios[i];
   return (result);
}

