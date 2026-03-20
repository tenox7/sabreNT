/* pursue version 0.2.2a */
void Pilot::pursue()
{
  R_KEY_BEGIN(4091) // Pilot::pursue

  float r_limit;
  R_3DPoint p0,p6,v6;
  DVector dv,dv2;
  float s_x,s_y,ss_x,ss_y;
  float abs_x,abs_y;
  int h_angle;
  float maxy,miny;

  flight->controls.flaps = 0;
  flight->controls.wheel_brakes = 0;
  flight->controls.landing_gear = 0;

  if (in_range)
    {
      s_x = g_trk.sc_x - gp_scx;
      s_y = g_trk.sc_y - gp_scy;
      c_trk = &g_trk;
    }
  else
    {
      s_x = l_trk.sc_x;
      s_y = l_trk.sc_y;
      c_trk = &l_trk;
    }

  abs_x = fabs(s_x);
  abs_y = fabs(s_y);

  if (in_range)
    {
      if (abs_x <= 20 &&
	  abs_y <= 20 )
	dbg = "PRST0B";
      else if (abs_x <= SCREEN_WIDTH / 2 &&
	       abs_y <= SCREEN_HEIGHT / 2)
	dbg = "PRST0A";
      else
	dbg = "PRST1";
    }
  else
    dbg = "PRST2";

  control_step =  ( params->c_step1 * fabs(trk.x_rate) / params->c_step2);
  if (control_step >  params->c_step1)
    control_step =  params->c_step1;
  else if (control_step <= 0)
    control_step = 1.0;


  // Attempt to align the lift vector with the
  // target

  /* calc bank angle */
  float roll = flight->state.flight_port.roll;
  flight->state.flight_port.set_roll(0);

  if (tg.angle_off < _PI2 - params->mparam4 ||
      tg.angle_off > _PI2 + params->mparam4 )
    {
      dv = DVector(0,100 * world_scale,0);
      dv2 = target_flight->to_world(dv);
      v6 = dv2.to_R_3DPoint();
      p6 = c_trk->w_position + v6;
      h_angle = 0;
    }
  else
    {
      p6 = c_trk->w_position;
      h_angle = 1;
    }

  tg.bp = p6;
  world_2_port(p6,&p0,&ss_x,&ss_y);
  
  tg.b_angle = -atan(ss_x / (ss_y + eps));
  if (p0.y < 0)
    tg.b_angle -= _PI;
  if (tg.b_angle < 0.0)
    tg.b_angle = _2PI + tg.b_angle;
  flight->state.flight_port.set_roll(roll);

  r_limit = abs_x;
  SETCONTROL(bank_control,tg.b_angle,6.0,0.06,1);
  bank_update();
  bank_control.snap_to = 0;

  /*********************************************************
   * Rudder                                                *
   *********************************************************/
  control_step = ( params->c_step1 * fabs(trk.x_rate) / params->c_step2);
  if (control_step > params->c_step1)
    control_step = params->c_step1;
  else if (control_step <= 0)
    control_step = 1.0;

  if (abs_x <= params->mparam1)
     {
       if (s_x < 0)
	 {
	   if (trk.x_rate < r_limit)
	     {
	       if (trk.x_rate < 0)
		 RIGHT_RUDDER
		   else
		     R_RUDDER
		 }
	   else if (trk.x_rate > fabs(trk.vel_scx))
	     {
	       L_RUDDER
		 }
	}
      else
	{
	  if (trk.x_rate < r_limit)
	    {
	      if (trk.x_rate < 0)
		LEFT_RUDDER
		  else
		    L_RUDDER
		  }
	  else if (trk.x_rate > fabs(trk.vel_scx))
	    {
	      R_RUDDER
		}
	}
     }
   else
     flight->controls.rudder = 0;

  /****************************************************************
   * CHEAT #1: Apply extra damping to the roll and yaw            *
   * Can be disabled by setting mdampx to 0                       *
   ****************************************************************/
  if (abs_x <= params->mdampx)
    {
      float damp_x = 1 / (abs_x + eps);
      flight->state.roll_rate = damp(flight->state.roll_rate,damp_x * t);
      flight->state.yaw_rate = damp(flight->state.yaw_rate,damp_x * t);
    }

  /*
  control_step =  ( params->c_step3 * fabs(trk.y_rate) / params->c_step4);
  if (control_step > (int) params->c_step3)
    control_step = (int) params->c_step3;
  else if (control_step <= 0)
    control_step = 1.0 / t;
  
  control_step = 32.0;
  Port_3D dport(flight->state.flight_port);
  dport.set_view(dport.look_from,c_trk->w_position);
  SETCONTROL(pitch_control,dport.slook_from.phi,abs_y * fabs(trk.y_rate),0.001,0);
  pitch_update();
  pitch_control.snap_to = 0;
  */
  
  
  if (fabs(trk.y_rate) > params->mparam2 ||
      abs_y > params->mparam3)
      
    {
      
      r_limit = abs_y;
      maxy = miny = 0.0;
      control_step =  ( params->c_step3 * fabs(trk.y_rate) / params->c_step4);
      // control_step =  ( params->c_step3 * fabs(trk.vel_scy) / params->c_step4);

      if (control_step > (int) params->c_step3)
	control_step = (int) params->c_step3;
      else if (control_step <= 0)
	control_step = 1.0 / t;
      //      dbg = "PRST0A";
    }
 
  else
    {
      control_step = 1.0;
      r_limit = 0.1;
      maxy = 4.0;
      miny = -4.0;
      //   dbg = "PRST0B";
    }

  if (s_y > maxy)
    {
      if (trk.y_rate < r_limit)
	{
	  if (abs_y > 4 && trk.y_rate < 0)
	  UP_ELEVATORS
	      else
		U_ELEVATORS
	      }
      else // if (trk.y_rate > fabs(trk.vel_scy) )
	{
	  D_ELEVATORS
	    }
    }
  else if (s_y  < miny)
    {
      if (trk.y_rate < r_limit)
	{
	  if (abs_y  > 4 && trk.y_rate < 0 )
	    DOWN_ELEVATORS
	      else
		D_ELEVATORS
	      }
      else // if (trk.y_rate > fabs(trk.vel_scy) )
	{
	  U_ELEVATORS
	    }
    }

  

  /*******************************************************************
   * CHEAT #2: Apply extra damping to the pitch                      *
   * can be disabled by setting mdampy to 0                          *
   *******************************************************************/
  if (( abs_y <= params->mdampy ) ) 
    {
      flight->state.pitch_rate = damp(flight->state.pitch_rate,abs_y * 0.01 * t);
    }

  /*****************************************************************
   * CHEAT #3: The Mother of All Cheats                            *
   * Actually line up the plane with the required vector. This is  *
   * truly shameful. It does make the pilots awfully good shots    *
   * when they want to be, though.                                 *
   * Can be disabled by setting mparam6 to 0                       *
   *****************************************************************/ 
  if (abs_x <= params->mparam6 &&
      abs_y <= params->mparam6 && in_range)
    {
      flight->state.flight_port.set_view(flight->state.flight_port.look_from,
					 g_trk.w_position);
      Vector v = Vector(g_trk.w_position - flight->state.flight_port.look_from);
      v.Normalize();
      flight->state.velocity.direction = DVector(v);
      /*
      flight->state.pitch_rate = damp(flight->state.pitch_rate,0.5 * t);
      flight->state.yaw_rate = damp(flight->state.yaw_rate,0.5 * t);
      */
      dbg = "LOCKON";
    }

  float wpr =  WPSPECS(sel_weapon)->min_range +
	  WPSPECS(sel_weapon)->max_range / 3.0;

  if ((tg.angle_off <= (_PI2 / 3.0)) && (tg.aspect < (_PI2 / 3.0)) 
      && (tg.range < wpr))
    //      && (tg.d_range < wpr))
    speed_control.goal = target_flight->state.z_vel;
  else
    speed_control.goal = flight->specs->max_speed;
  speed_update();

  shoot();
  control_step = 1.0 / t;
  R_KEY_END
    }

/* Pursue version 0.2.2c */
void Pilot::pursue()
{
  R_KEY_BEGIN(4091) // Pilot::pursue
  
  float r_limit;
  R_3DPoint w0,w1;
  R_3DPoint p0,p1,v6;
  DVector dv,dv2;
  float s_x,s_y,ss_x0,ss_y0,ss_x1,ss_y1;
  float abs_x,abs_y;
  int h_angle;
  float maxy,miny;

  flight->controls.flaps = 0;
  flight->controls.wheel_brakes = 0;
  flight->controls.landing_gear = 0;

  if (in_range)
    {
      s_x = g_trk.sc_x - gp_scx;
      s_y = g_trk.sc_y - gp_scy;
      c_trk = &g_trk;
    }
  else
    {
      s_x = l_trk.sc_x;
      s_y = l_trk.sc_y;
      c_trk = &l_trk;
    }

  abs_x = fabs(s_x);
  abs_y = fabs(s_y);

  if (in_range)
    {
      if (abs_x <= 20 &&
	  abs_y <= 20 )
	dbg = "PRST0B";
      else if (abs_x <= SCREEN_WIDTH / 2 &&
	       abs_y <= SCREEN_HEIGHT / 2)
	dbg = "PRST0A";
      else
	dbg = "PRST1";
    }
  else
    dbg = "PRST2";

  control_step =  ( params->c_step1 * fabs(trk.x_rate) / params->c_step2);
  if (control_step >  params->c_step1)
    control_step =  params->c_step1;
  else if (control_step <= 0)
    control_step = 1.0;


  // Attempt to align the lift vector with the
  // target

  /* calc bank angle */
  float roll = flight->state.flight_port.roll;
  flight->state.flight_port.set_roll(0);
  /*
  if (tg.angle_off < _PI2 - params->mparam4 ||
      tg.angle_off > _PI2 + params->mparam4 )
      */
    {
      dv = DVector(0,1000,0);
      /*
      if (tg.range <= 6000.0)
	dv = DVector(0,150.0 * world_scale,0);
      else
	dv = DVector(0,tg.range * world_scale,0);
	*/
      dv2 = target_flight->to_world(dv);
      v6 = dv2.to_R_3DPoint();
      w0 = trk.w_position;
      w1 = trk.w_position + v6;
      world_2_port(w0,&p0,&ss_x0,&ss_y0);
      world_2_port(w1,&p1,&ss_x1,&ss_y1);
      
      h_angle = 0;
    }

  /*
  else
    {
      w0 = g_trk.w_position;
      w1 = g_trk.w_position + R_3DPoint(l_trk.w_position - trk.w_position);
      h_angle = 1;
    }
    */
  tg.bp = w0;
  world_2_port(w0,&p0,&ss_x0,&ss_y0);
  world_2_port(w1,&p1,&ss_x1,&ss_y1);
 
  if (ss_y0 < 0 && ss_y1 > ss_y0)
    {
      ss_x0 *= -1.0;
      ss_y0 *= -1.0;
    }

  tg.b_angle = -atan(ss_x0 / (ss_y0 + eps));
  if (p0.y < 0)
    tg.b_angle -= _PI;
  if (tg.b_angle < 0.0)
    tg.b_angle = _2PI + tg.b_angle;
  flight->state.flight_port.set_roll(roll);

  r_limit = abs_x;
  SETCONTROL(bank_control,tg.b_angle,100.0,0.06,1);
  bank_update();
  bank_control.snap_to = 0;

  /*********************************************************
   * Rudder                                                *
   *********************************************************/
  control_step = ( params->c_step1 * fabs(trk.x_rate) / params->c_step2);
  if (control_step > params->c_step1)
    control_step = params->c_step1;
  else if (control_step <= 0)
    control_step = 1.0;

  if (abs_x <= params->mparam1)
     {
       if (s_x < 0)
	 {
	   if (trk.x_rate < r_limit)
	     {
	       R_RUDDER
		 }
	   else if (trk.x_rate > fabs(trk.vel_scx))
	     {
	       L_RUDDER
		 }
	}
      else
	{
	  if (trk.x_rate < r_limit)
	    {
	      L_RUDDER
		  }
	  else if (trk.x_rate > fabs(trk.vel_scx))
	    {
	      R_RUDDER
		}
	}
     }
  /****************************************************************
   * CHEAT #1: Apply extra damping to the roll and yaw            *
   * Can be disabled by setting mdampx to 0                       *
   ****************************************************************/
  if (abs_x <= params->mdampx)
    {
      float damp_x = 1 / (abs_x + eps);
      flight->state.roll_rate = damp(flight->state.roll_rate,damp_x * t);
      flight->state.yaw_rate = damp(flight->state.yaw_rate,damp_x * t);
    }

  control_step =  ( params->c_step3 * fabs(trk.y_rate) / params->c_step4);
  if (control_step > (int) params->c_step3)
    control_step = (int) params->c_step3;
  else if (control_step <= 0)
    control_step = 1.0 / t;
  /*
  control_step = 32.0;
  Port_3D dport(flight->state.flight_port);
  dport.set_view(dport.look_from,c_trk->w_position);
  SETCONTROL(pitch_control,dport.slook_from.phi,abs_y * fabs(trk.y_rate),0.001,0);
  pitch_update();
  pitch_control.snap_to = 0;
  
  */
  r_limit = abs_y * 1.3;
  maxy = miny = 0.0;
  control_step =  ( params->c_step3 * fabs(trk.y_rate) / params->c_step4);
  if (control_step > (int) params->c_step3)
    control_step = (int) params->c_step3;
  else if (control_step <= 0)
    control_step = 1.0 / t;
 
  if (abs_y > params->mparam3)
    {
  if (s_y > maxy)
    {
      if (trk.y_rate < r_limit)
	{
	  U_ELEVATORS
	    }
      else 
	{
	  D_ELEVATORS
	    }
    }
  else if (s_y  < miny)
    {
      if (trk.y_rate < r_limit)
	{
	  D_ELEVATORS
	    }
      else 
	{
	  U_ELEVATORS
	    }
    }
    }
  

  /*******************************************************************
   * CHEAT #2: Apply extra damping to the pitch                      *
   * can be disabled by setting mdampy to 0                          *
   *******************************************************************/
  if (( abs_y <= params->mdampy ) ) 
    {
      flight->state.pitch_rate = damp(flight->state.pitch_rate,abs_y * 0.01 * t);
    }

  /*****************************************************************
   * CHEAT #3: The Mother of All Cheats                            *
   * Actually line up the plane with the required vector. This is  *
   * truly shameful. It does make the pilots awfully good shots    *
   * when they want to be, though.                                 *
   * Can be disabled by setting mparam6 to 0                       *
   *****************************************************************/ 
  if (abs_x <= params->mparam6 &&
      abs_y <= params->mparam6 && in_range)
    {
      flight->state.flight_port.set_view(flight->state.flight_port.look_from,
					 g_trk.w_position);
      Vector v = Vector(g_trk.w_position - flight->state.flight_port.look_from);
      v.Normalize();
      flight->state.velocity.direction = DVector(v);
      /*
      flight->state.pitch_rate = damp(flight->state.pitch_rate,0.5 * t);
      flight->state.yaw_rate = damp(flight->state.yaw_rate,0.5 * t);
      */
      dbg = "LOCKON";
    }

  float wpr =  WPSPECS(sel_weapon)->min_range +
	  WPSPECS(sel_weapon)->max_range / 3.0;

  if ((tg.angle_off <= (_PI2 / 3.0)) && (tg.aspect < (_PI2 / 3.0)) 
      && (tg.range < wpr))
    //      && (tg.d_range < wpr))
    speed_control.goal = target_flight->state.z_vel;
  else
    speed_control.goal = flight->specs->max_speed;
  speed_update();

  shoot();
  control_step = 1.0 / t;
  R_KEY_END
    }

