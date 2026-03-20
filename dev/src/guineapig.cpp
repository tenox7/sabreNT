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
/*******************************************************
 * flight.cpp                                          *
 *******************************************************/
#include <stdio.h>
#include <iostream.h>
#include <fstream.h>
#include <math.h>
#include <limits.h>
#include <values.h>
#include "defs.h"
#include "simerr.h"
#include "sim.h"
#include "pc_keys.h"
#include "vmath.h"
#include "port_3d.h"
#include "rtkey.h"
#include "simfile.h"
#include "flight.h"

const float nz = 0.000001;
const float g = 32.16;

istream & operator >>(istream &is, Flight_Controls &fc)
{
  int vw;
  is >> fc.throttle >> fc.landing_gear >> fc.wheel_brakes;
  is >> fc.cockpit >> fc.hud_on >> fc.vextern;
  is >> fc.autopilot >> fc.gunsight >> fc.radar ;
  is >> fc.vdist >> fc.vtheta >> fc.vphi;
  is >> vw;
  fc.view = (flight_view) vw;
  if (fc.throttle)
    fc.engine_on = 1;
  // Default radar to on for now
  fc.radar = 1;
  return is;
}

ostream & operator <<(ostream &os, Flight_Controls &fc)
{
  os << fc.elevators << fc.ailerons << fc.throttle ;
  os << fc.rudder << fc.air_brakes;
  os << fc.flaps << fc.wheel_brakes << fc.landing_gear ;
  return os;
}

istream &operator >>(istream &is, Vector_Q &vq)
{
  char c;
  is >> c;
  while (is && c != '(')
    is >> c;
  if (is)
    {
      is >> vq.magnitude >> vq.direction >> c;
      while (is && c!= ')')
	is >> c;
    }
  return is;
}

Vector to_vector(Vector_Q &vq)
{
  return Vector(vq.direction.X * vq.magnitude,
		 vq.direction.Y * vq.magnitude,
		 vq.direction.Z * vq.magnitude);
}

DVector operator +(Vector_Q &vq, DVector &v)
{
  DVector v1 = vq.direction * vq.magnitude;
  return DVector(v + v1);
}

/* convert a double vector into port coords */
DVector Flight::to_port(DVector &v)
{
  R_3DPoint r_w,r_p;
  r_w = v.to_R_3DPoint();
  r_w += state.flight_port.look_from;
  state.flight_port.world2port(r_w,&r_p);
  return (DVector(r_p));
}

DVector Flight::to_world(DVector &v)
{
  R_3DPoint r_w,r_p;
  r_p = v.to_R_3DPoint();
  //	r_p += state.flight_port.look_from;
  state.flight_port.port2world(r_p,&r_w);
  r_w -= state.flight_port.look_from;
  return (DVector(r_w));
}

Vector_Q operator +(Vector_Q &vq1, Vector_Q &vq2)
{
  R_KEY_BEGIN(3)
    // Convert magnitudes
    float r_mag;
  DVector v1 = vq1.direction * vq1.magnitude;
  DVector v2 = vq2.direction * vq2.magnitude;
  DVector v_result = v1 + v2;
  // Get magnitude of result
  r_mag = v_result.Magnitude();
  // Normalize result
  if (r_mag > 0)
    {
      v_result.X = v_result.X / r_mag;
      v_result.Y = v_result.Y / r_mag;
      v_result.Z = v_result.Z / r_mag;
    }
  else
    v_result.X = v_result.Y = v_result.Z = 0;
  R_KEY_END
    return Vector_Q(r_mag,v_result);
}

int Flight_Specs::read_file(char *path)
{
  int result = 0;
  ifstream infile;
  if (open_is(infile,path))
    {
      result = 1;
      infile >> *this;
      infile.close();
    }
  return result;
}

istream & operator >>(istream & is, Flight_Specs &fs)
{
  char c = ' ';
  while (is && c != '{')
    is >> c;
  if (!is)
    return is;

  is >> fs.weight >> fs.max_thrust >> fs.max_speed >> fs.corner_speed;
  is >> fs.lift_factor >> fs.max_aoa >> fs.wing_aoa;
  is >> fs.drag_factor >> fs.idrag_factor ;
  is >> fs.drag_aoa >> fs.drag_yaw >> fs.drag_wb;
  is >> fs.drag_gr >> fs.drag_ab >> fs.drag_whls;
  is >> fs.control_pitch >> fs.control_yaw >> fs.control_roll;
  is >> fs.return_pitch >> fs.return_yaw;
  is >> fs.pitch_drag >> fs.yaw_drag >> fs.roll_drag;
  is >> fs.pitch_damp >> fs.yaw_damp >> fs.roll_damp;
  is >> fs.lspeed >> fs.l_z >> fs.l_aoa_max >> fs.max_wb;
  is >> fs.adv_yaw >> fs.adv_roll >> fs.g_height ;
  is >> fs.flap_lift >> fs.flap_drag;
  is >> fs.damp_time >> fs.wb_damp;
  is >> fs.g_limit >> fs.stall_spin;
  is >> fs.max_damage;
  is >> fs.view_point >> fs.max_aoa_factor;
  is >> fs.engine_type >> fs.flags;
  is >> fs.model;
  fs.model[8] = '\0';

  // m = W/g
  fs.mass = fs.weight / g;
  fs.max_speed = kts2fps(fs.max_speed);
  fs.corner_speed = kts2fps(fs.corner_speed);
  fs.lspeed = kts2fps(fs.lspeed);
  fs.max_wb = kts2fps(fs.max_wb);
  fs.g_height *= world_scale;
  while (is && (c != '}'))
    is >> c;
  return is;
}

ostream & operator <<(ostream & os, Flight_Specs &fs)
{
  os << "{\n";
  os << fs.weight << fs.max_thrust << fps2kts(fs.max_speed)
     << fps2kts(fs.corner_speed);
  os << fs.lift_factor << fs.max_aoa << fs.wing_aoa;
  os << fs.drag_factor << fs.idrag_factor ;
  os << fs.drag_aoa << fs.drag_yaw << fs.drag_wb;
  os << fs.drag_gr << fs.drag_ab << fs.drag_whls;
  os << fs.control_pitch << fs.control_yaw << fs.control_roll;
  os << fs.return_pitch << fs.return_yaw;
  os << fs.pitch_drag << fs.yaw_drag << fs.roll_drag;
  os << fs.pitch_damp << fs.yaw_damp << fs.roll_damp;
  os << fps2kts(fs.lspeed) << fs.l_z << fs.l_aoa_max << fps2kts(fs.max_wb);
  os << fs.adv_yaw << fs.adv_roll << fs.g_height / world_scale;
  os << fs.flap_lift << fs.flap_drag;
  os << fs.damp_time << fs.wb_damp;
  os << fs.g_limit << fs.stall_spin;
  os << fs.max_damage;
  os << fs.view_point << fs.max_aoa_factor;
  os << fs.engine_type << fs.flags;
  os << "}\n";
  return os;
}

istream &operator >>(istream &is, Flight_State &fp)
{
  float kph;
  is >> kph >> fp.flight_port;
  if (kph > 0)
    {
      fp.velocity.magnitude = kts2fps(kph);
      fp.velocity.direction = fp.flight_port.view_normal;
    }
  return is;
}

istream & operator >>(istream & is, Flight &f)
{
  char c;
  is >> c;
  while (is && c!='{')
    is >> c;
  if (is)
    {
      //		is >> f.time_factor >> f.world_scale;
      is >> f.controls >> f.state >> c;
      while (is && c!='}')
	is >> c;
    }
  return is;
}

void Flight::start()
{
  // avoid div by zero, at all costs!
  l_time = 0.1;
  calc_state(1.0);
  calc_forces(1.0);
}

void Flight::update()
{
  // Get elapsed time in microseconds
  l_time = time_frame;
  if (l_time <= 0.0)
    l_time = 0.1;
  dmp_time += l_time;
  apply_forces(l_time);
  if (do_rotations != 0)
    apply_rotations(l_time);
  calc_state(l_time);
  calc_forces(l_time);
  if (dmp_time > specs->damp_time)
    dmp_time = 0.0;
}

void Flight::pause()
{

}

extern REAL_TYPE getGroundLevel(R_3DPoint &);

void Flight::calc_state(float)
{
  float d;
  if (l_time <= 0.0)
    l_time = 0.1;

  // Set inverted flag
  state.negative_phi = 0;
  state.negative_roll = 0;
  if ((state.flight_port.slook_from.phi < _2PI) &&
      (state.flight_port.slook_from.phi > _PI))
    state.negative_phi = 1;
  if ((state.flight_port.roll > _PI2) &&
      (state.flight_port.roll < _PI34))
    state.negative_roll = 1;
  if (state.negative_roll)
    {
      if (state.negative_phi)
	state.inverted = 0;
      else
	state.inverted = 1;
    }
  else if (state.negative_phi)
    {
      if (state.negative_roll)
	state.inverted = 0;
      else
	state.inverted = 1;
    }
  else
    state.inverted = 0;

  /* calc ground level, agl, and deltas */
  d = getGroundLevel(state.flight_port.look_from);
  state.d_ground_height = (d - state.ground_height) / l_time;
  state.ground_height = d;
  d = state.flight_port.look_from.z - state.ground_height;
  state.d_agl = d - state.agl;
  state.d_agl /= l_time;
  state.agl = d;


  if (state.flight_port.look_from.z <= state.ground_height + specs->g_height)
    state.on_ground = 1;
  else
    {
      state.on_ground = 0;
      state.was_airborne = 1;
    }
  state.g_force = forces.lift.magnitude / specs->weight;
  if (forces.lift.direction.Y < 0.0)
    state.g_force = -state.g_force;
  float d1 = state.flight_port.look_from.z / world_scale;

  if (d1 <= 1000.0)
    state.air_density = 1.0;
  else if (d1 >= 100000.0)
    state.air_density = 0.05;
  else
    state.air_density = 1.0 - (d1 / 200000.0) + 0.10;
}

// Note that in this simplified version, thrust = max_thrust
//                                       lift = lift_factor,
void Flight::calc_forces(float )
{
  R_KEY_BEGIN(4)
    float vel_sq = state.velocity.magnitude * state.velocity.magnitude;
  if (vel_sq < 1.0)
    vel_sq = 1.0 + (1.0 - vel_sq);
  forces.thrust.magnitude = specs->max_thrust;
  forces.thrust.direction = DVector(0,0,1);
  forces.drag.magnitude = vel_sq * specs->drag_factor;
  forces.drag.direction = DVector(0,0,-1);
  forces.lift.magnitude = specs->lift_factor;
  forces.lift.direction = DVector(0,1,0);
  forces.g.magnitude = specs->weight;
  forces.g.direction = DVector(0,0,-1);
  R_KEY_END
    }

void Flight::apply_forces(float t)
{
  R_KEY_BEGIN(5)
    Vector_Q sum_forces,acc;
  Vector_Q tmp1,tmp2;
  Vector delta;
  DVector  vc;
  DVector world_v;
  // Flag which is set when the 'normal' force is active
  int no_z = 0;
  // Add up all 'port coordinant' forces
  sum_forces.direction = to_vector(forces.thrust)
    + to_vector(forces.drag)
    + to_vector(forces.lift);
  sum_forces.magnitude = sum_forces.direction.Magnitude();
  if (sum_forces.magnitude > 0)
    {
      sum_forces.direction.X /= sum_forces.magnitude;
      sum_forces.direction.Y /= sum_forces.magnitude;
      sum_forces.direction.Z /= sum_forces.magnitude;
    }
  else
    sum_forces.direction.X = sum_forces.direction.Y =
      sum_forces.direction.Z = 0;

  // Convert to world coordinants & add in gravity
  world_v = to_world(sum_forces.direction);
  sum_forces.direction = world_v;
  sum_forces = sum_forces + forces.g;
  // If on ground, apply normal force
  if (state.on_ground)
    {
      no_z = 1;
      // record landing parameters
      if (state.was_airborne)
	{
	  state.landing_z = state.climb_vel;
	  state.landing_velocity = state.z_vel;
	  state.landing_aoa = state.angle_of_attack;
	  state.landing_pitch = state.flight_port.slook_from.phi;
	  state.landing_roll = state.flight_port.roll;
	  state.was_airborne = 0;
	  state.landing_recorded = 1;
	}
      if (sum_forces.direction.Z < 0)
	{
	  vc = to_vector(sum_forces);
	  vc.Z = 0;
	  sum_forces.magnitude = vc.Magnitude();
	  vc.Normalize();
	  sum_forces.direction = vc;
	}
    }


  // Determine acceletation
  // a = F / m
  acc.magnitude = sum_forces.magnitude / specs->mass;
  acc.direction = sum_forces.direction;
  DVector delta_v;

  // Apply to position
  // s = v0t + 1/2at^2
  tmp1 = state.velocity;
  tmp1.magnitude *= t;
  tmp2 = acc;
  tmp2.magnitude *= (t * t);
  tmp2.magnitude /= 2;
  tmp1 = tmp1 + tmp2;
  delta_v = to_vector(tmp1);
  delta_v *= world_scale;
  delta = delta_v.to_vector();
  state.flight_port.delta_look_from(delta);
  if (no_z && delta.Z < 0)
    delta.Z = 0;
  // Calculate new velocity
  // v = v0 + at
  tmp1 = acc;
  tmp1.magnitude *= t;
  state.velocity = state.velocity + tmp1;
  if (state.velocity.magnitude >= specs->max_speed)
    state.velocity.magnitude = specs->max_speed;
  // Kill any negative
  if (state.velocity.magnitude < 0.0)
    state.velocity.magnitude = 0.0;
  if (no_z && state.velocity.direction.Z < 0)
    {
      state.velocity.magnitude -= (state.velocity.magnitude *
				   state.velocity.direction.Z);
      state.velocity.direction.Z = 0;
      if (state.flight_port.look_from.z < state.ground_height + specs->g_height)
	state.flight_port.delta_look_from(Vector(0,0,
						 (state.ground_height + specs->g_height) - state.flight_port.look_from.z));
    }
  if (no_z)
    {
      float aX;
      DVector v = to_port(state.velocity.direction);
      aX = fabs(v.X);
      if (aX > 0.0)
	{
	  state.velocity.magnitude -= state.velocity.magnitude * aX;
	  v.X = 0.0;
	  state.velocity.direction = to_world(v);
	}
    }
  if (state.on_ground
      && controls.wheel_brakes
      && state.velocity.magnitude <= specs->max_wb)
    state.velocity.magnitude = damp(state.velocity.magnitude,specs->wb_damp * t);
  R_KEY_END
    }


void Flight::calc_velocity(float  )
{
  state.velocity.direction = DVector(state.flight_port.look_at -
				     state.flight_port.look_from);
  state.velocity.direction.Normalize();
  state.velocity.magnitude = specs->max_speed;
}

void Flight::apply_velocity(float  time)
{
  Vector_Q tmp1;
  Vector delta;

  tmp1 = state.velocity;
  tmp1.magnitude *= time;
  // Get vector representing change in position
  DVector vdelta = to_vector(tmp1);
  vdelta *= world_scale;
  delta = vdelta.to_vector();
  state.flight_port.delta_look_from(delta);
}

void Flight::apply_rotations(float t)
{
  state.flight_port > (state.pitch_rate * t);
  state.flight_port < (state.yaw_rate * t);
  state.flight_port / (state.roll_rate * t);
}
