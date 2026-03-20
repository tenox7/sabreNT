/***************************************************************
 * zvedit.C                                                    *
 * A quick and dirty shape editor                              *
 * for SABRE Fighter Plane Simulator                           *
 ***************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <values.h>
#include <float.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <signal.h>
#include <ctype.h>
#include <iostream.h>
#include <fstream.h>
#include <setjmp.h>
#include <stdarg.h>
#include <signal.h>

/* macros & file handling includes */
#include "simerr.h"
#include "rtkey.h"
#include "simfile.h"

/* set color, font includes */
#include "colorspc.h"
#include "font8x8.h"

/* basic graphics includes */
#include "defs.h"
#include "grafix.h"
#include "vga_13.h"
#include "traveler.h"
#include "pen.h"
#include "transblt.h"
#include "vmath.h"
#include "port_3d.h"
#include "clip.h"
#include "target.h"
#include "cpoly.h"
#include "copoly.h"
#include "txtrmap.h"
#include "bits.h"
#include "convpoly.h"
#include "rendpoly.h"
#include "rndrpoly.h"
#include "rndzpoly.h"
#include "zview.h"
#include "rotate.h"

/* input includes */
#include "pc_keys.h"
#include "kbdhit.h"
#include "key_map.h"

#include "convert.h"

#ifdef SABREWIN
#include "rpoint.h"
#endif
extern void buffer2ppm();

/******************************************************************
 * globals                                                        *
 ******************************************************************/
char *modes[] =
{
  "view",
  "edit1",
  "edit2",
  "txtr",
  "twrap"
};

char *sel_axis[] =
{
  "xz x",
  "xy y",
  "yz z"
};

int s_axis = 0;
int axis_on = 1;
float axis_len = 1.0;

float scale_f = 99;
Port_3D vport;
Port_3D ref_port;
Vector world_light_source;
float world_scale;
int xx = 0;
int yy = 0;
jmp_buf jump_buff;
REAL_TYPE view_distance;
int mode = 0;
int current_shape = 0;
Z_Node_Manager *zm;
extern int frame_switch;
char *out_file = "zvedit.out";
char *outfile = out_file;
color_spec cs;  
int current_poly;
#ifdef SABREWIN
char *lib_path = ".\\lib";
#else
char *lib_path = "./lib";
#endif
int routine_key;
REAL_TYPE shadow_level = 0.0;
REAL_TYPE vphi = 0.0;
REAL_TYPE vtheta = 0.0;
int current_txtr = 0;
int current_wrap = -1;

char *shape_file = "a.shp";
char *palette_file  = "a.plt";
char *txtr_file = "a.txr";

int gpause;
int dump_screen;
float raw_time = 0.0;

#ifndef SABREWIN
extern float delta;
#endif

/******************************************************************
 * prototypes                                                     *
 ******************************************************************/
int graphicsStart(void);
void doEditor(char *);
void setView();
int editKey(Port_3D &);
void save();
void showDir();
void showStats();
void showAxis(Port_3D &viewport, Port_3D &ref_port);

#define PRINTLN(a) { g_font->put_string((a),xx,yy,12); \
		     yy += 7; }

void printLn(char *str, ...)
{
  char buf[BUFSIZ];
  va_list ap;
  va_start(ap,str);
  vsprintf(buf,str,ap);
  va_end(ap);
  PRINTLN(buf);
}

/*************************************************************************
 * MAIN                                                                  *
 *************************************************************************/
#ifndef SABREWIN

void (*oldsigsegv)(int);

void mysigsegv(int )
{
  printf("got sigsegv!\n");
  //  (*oldsigsegv)(seg);
  restorecrtmode();
}

extern int window_width,window_height;

main(int argc, char *argv[])
{
  int i;
  int m;
  for (i = 1; i < argc; i++)
    {
      if (!strcmp(argv[i],"-f"))
	shape_file = argv[++i];
      else if (!strcmp(argv[i],"-p"))
	palette_file = argv[++i];
      else if (!strcmp(argv[i],"-mode"))
	{
	  m = atoi(argv[++i]);
	  if (m >= 0 && m <= 5 )
	    VGAMODE = m;
	}
      else if (!strcmp(argv[i],"-window"))
	{
	  window_width = atoi(argv[++i]);
	  window_height = atoi(argv[++i]);
	}
    }

  if (setjmp(jump_buff))
    {
      printf("exiting\n");
      return(1);
    }

  if (graphicsStart())
    {
      oldsigsegv = signal(SIGSEGV,mysigsegv);
      g_font = new ConsoleFont("simfnt-8x8");
      g_font->put_width = 6;
      doEditor(shape_file);
      restorecrtmode();
      return 0;
    }
  return 1;
}

void error_jump(char *format, ... )
{
  restorecrtmode();
  if (format != NULL)
    {
      va_list ap;
      va_start(ap,format);
      vfprintf(stderr,format,ap);
      va_end(ap);
    }
  longjmp(jump_buff,1);
}


int graphicsStart(void)
{
  curpen->color = 2;
  curpen->position = Point(0,0);
  curpen->width = 1;

  printf("Initing graphic interface\n");
  init_vga_13();
  printf("Initing edge bounds\n");
  init_edge_bounds();
  printf("Initing zbuffer\n");
  tr_init();
  printf("Setting port_3D params\n");
  Port_3D::initPort3D();
  ref_port.screen = Port_3D::gbounds;
  vport.screen = Port_3D::gbounds;
  read_palette_file(palette_file);
  map_man = new TextrMap_Manager();
  read_texture_file(txtr_file);
  return 1;
}

#else
   int (* messagehook)(void) = NULL;
   float delta = 0.1;
#endif

void setView()
{
  Vector v,v1;
  R_3DPoint r1,r2;
  vport = ref_port;
  vport.set_roll(0);

  //  vport.set_phi(1.57);
  //  vport.set_phi(0.0);
  //  vport.set_phi(0.0);
  vport.set_phi(_PI);
  vport.set_theta(0);
  vport > vphi;
  vport < vtheta;
  v = to_nvector(vport.look_from,vport.look_at);
  v *= view_distance;
  r1 = R_3DPoint(v) + vport.look_from;
  v1 = to_nvector(r1,vport.look_from);
  r2 = R_3DPoint(v1) + r1;
  //  vport.set_view(r1,r2);
  vport.set_view(r1,vport.look_from);
}

int zz = 0;
void doEditor(char *fname)
{
  zm = new Z_Node_Manager();
  if (setjmp(jump_buff))
    {
      delete zm;
      return;
    }
  ifstream test;
  if (open_is(test,fname))
    {
      test >> world_scale ;
      test >> view_distance;
      test >> ref_port;
      test >> world_light_source;
      test >> cs;

      test >> *zm;

      view_distance *= world_scale;
      do
	{
#ifdef SABREWIN
      if (messagehook && !messagehook())
         break;
#endif
	  setView();
	  zz++;
	  if (zz == 30)
	    {
	      clear_zbuff();
	      zz = 0;
	    }
	  else
	    ztrans += (3L << Z_PREC);

	  clear_scr(cs.color);
	  Z_Viewer zview(zm);
	  zview.draw_shadow = 0;
	  zview.reference_port = &ref_port;
	  xx = 0;
	  yy = 0;
	  zview.draw_prep(vport);
	  if (zview.visible_flag)
	    {
	      switch (mode)
		{
		case 0:
		  zview.draw(vport);
		  break;

		case 1:
		  {
		    int fs = frame_switch;
		    frame_switch = 1;
		    zview.shapes[current_shape].visible = 0;
		    zview.draw(vport);
		    frame_switch = fs;
		    zview.shapes[current_shape].visible = 1;
		    Vector vc = world_light_source;
		    R_3DPoint dummy;
		    vc.Normalize();
		    zview.shapes[current_shape].set_world_points(ref_port);
		    zview.shapes[current_shape].set_visible(vport);
		    zview.shapes[current_shape].set_poly_icolor(vc);
		    zview.shapes[current_shape].draw(vport,dummy);
		    break;
		  }

		case 2:
		case 3:
		  {
		    C_Oriented_Poly &ply = zview.shapes[current_shape].o_polys[current_poly];
		    int fs = frame_switch;
		    frame_switch = 1;
		    ply.visible = 0;
		    zview.draw(vport);
		    frame_switch = fs;
		    ply.visible = 1;
		    Vector vc = world_light_source;
		    vc.Normalize();
		    ply.set_world_points(ref_port);
		    ply.set_poly_icolor(vc);
		    ply.create_points(ref_port);
		    ply.draw(vport,ref_port);
		    if (ply.wpoints)
		      {
			delete [] ply.wpoints;
			ply.wpoints = NULL;
		      }

		    break;
		  }

		case 4:
		  {
		    if (twrp == NULL)
		      break;
		    int fs = frame_switch;
		    frame_switch = 1;
		    zview.draw(vport);
		    frame_switch = 0;
		    Vector vc = world_light_source;
		    vc.Normalize();
		    
		    for (int i=twrp[current_wrap*2];i<=twrp[current_wrap*2+1];i++)
		      {
			C_Oriented_Poly &ply = zview.shapes[current_shape].o_polys[i];
			ply.visible = 1;
			ply.set_world_points(ref_port);
			ply.set_poly_icolor(vc);
			ply.create_points(ref_port);
			ply.draw(vport,ref_port);
			if (ply.wpoints)
			  {
			    delete [] ply.wpoints;
			    ply.wpoints = NULL;
			  }
		      }
		    frame_switch =fs;
		    break;
		  }
		}
	    }
	  if (axis_on)
	    showAxis(vport,ref_port);
	  if (twrp != NULL && current_wrap == -1)
	    current_wrap = 0;
	  showDir();
	  showStats();
	  blit_buff();
	} while (editKey(vport));
      //     delete zm;
      // zm = NULL;
    }
}

void save()
{
  printf("saving\n");
  ofstream os;
  if ((zm != NULL) && open_os(os,out_file))
    {
      os << world_scale << " " << (view_distance / world_scale)<< '\n';
      os << ref_port.slook_from << ' ' << ref_port.look_from << ' ';
      os << Port_3D::fovx  << ' ' << ref_port.roll << ' ';
      os  << ref_port.z_min << ' ' << ref_port.z_max << ' ' << ref_port.horizon << '\n';
      os << world_light_source << '\n';
      os << cs << '\n';
      os << *zm;
      os.close();
    }
  else
    printf("zm is null!\n");
}

void showDir()
{
  printLn("shape: %d  poly: %d  txtr: %d  wrap: %d",
	  current_shape,
	  current_poly,
	  current_txtr,
	  current_wrap
	  );
}

void showStats()
{
  printLn("mode: %s axis: %s scale: %3.2f%% delta: %3.3f",
	  modes[mode],
	  sel_axis[s_axis],
	  scale_f,
	  delta );
}

int editKey(Port_3D &port)
{
  char c;
  if ((c = (char ) kbdin) != 0)
   {
      kbdin = 0;
      Vector v = Vector(port.look_at - port.look_from);
      v.Normalize();
      Vector v2 = v;
      v2 *= delta;

      switch (c)
	{
	case ESC:
	case 'Q':
	  return 0;

	case 'q':
	  save();
	  return 0;

	case HT:
	case '`':
	  if (current_shape < zm->n_shapes - 1)
	    current_shape++;
	  else
	    current_shape = 0;
	  break;

	case ' ':
	  if (current_poly <
	      zm->shape_info[current_shape].npolys - 1)
	    current_poly++;
	  else
	    current_poly = 0;
	  break;

	case BS:
	case '~':
	  if (current_shape > 0)
	    current_shape--;
	  else
	    current_shape = zm->n_shapes - 1;
	  break;

	case 't':
	  if (current_txtr <= map_man->nxt - 2)
	    current_txtr++;
	  else
	    current_txtr = 0;
	  break;

	case 'T':
	  if (current_txtr > 0)
	    current_txtr--;
	  else
	    current_txtr = map_man->nxt - 1;
	  break;


	case '1':
	  if (mode == 1)
	    convert1(zm->shape_info[current_shape]);
	  break;

	case '2':
	  if (mode == 1)
	    convert2(zm->shape_info[current_shape]);
	  break;


	  /* poly2shape conversions */
	case '4':
	  /* 4-sided axis */
	  if (mode == 1)
	    convert3(0);
	  break;

	case '6':
	  /* 6-sided axis */
	  if (mode == 1)
	    convert3(2);
	  break;

	case '8':
	  /* 8-sided axis */
	  if (mode == 1)
	    convert3(1);
	  break;

	case '9':
	  /* 8-sided proportioned */
	  if (mode == 1)
	    convert3(3);
	  break;

	case '5':
	  /* shape2mountain */
	  if (mode == 1)
	    convert4();
	  break;


	case '7':
	  /* vasisdis? */
	  if (mode == 1)
	    convert5();
	  break;


	case '+':
	  if (mode == 4)
	    adjust_texture_coords(twrp[current_wrap*2],twrp[current_wrap*2+1],1.0,1.0,1);
	  else if (mode == 3)
	    adjust_texture_coords(current_poly,current_poly,0.98,0.98,1);
	  else
	    delta += 0.001;
	  break;

	case '-':
	  if (mode == 4)
	    adjust_texture_coords(twrp[current_wrap*2],twrp[current_wrap*2+1],-1.0,-1.0,1);
	  else if (mode == 3)
	    adjust_texture_coords(current_poly,current_poly,1.2,1.2,1);
	  else
	    if (delta > 0.0)
	      delta -= 0.001;
	  break;

	case 'D':
	  if (mode == 1)
	    zm->deleteShape(current_shape);
	  else
	    buffer2ppm();
	  break;


	case 'M':
	case 'm':
	  mode++;
	  if (mode > 4)
	    mode = 0;
	  /*
	  if (current_wrap == -1)
	    {
	      if (mode >= 4)
		mode = 0;
	    }
	  else
	    {
	      if (mode >= 5)
		mode = 0;
	    }
	    */
	  break;

	case 'A':
	case 'a':
	  s_axis++;
	  if (s_axis > 2)
	    s_axis = 0;
	  break;

	case 'F':
	case 'f':
	  frame_switch = !frame_switch;
	  break;

	case 'W':
	case 'w':
	  if (mode <= 2)
	    port2world("testout.wld");
	  else
	    {
	      current_wrap++;
	      if (current_wrap > twrap_n - 1)
		current_wrap = 0;
	    }
	  break;

	case 'C':
	  if (mode == 1)
	    {
	      consolidatePolys();
	      current_shape = current_poly = 0;
	    }
	  break;

	case 'S':
	  if (mode >= 1)
	    scale(zm->shape_info[current_shape],scale_f / 100.0,1);
	  break;

	case 's':
	  if (mode >= 1)
	    scale(zm->shape_info[current_shape],scale_f / 100.0);
	  break;

	case 'R':
	  if (mode >= 1)
	    rotate(zm->shape_info[current_shape],_2PI / 360.0,ref_port);
	  break;

	case 'r':
	  if (mode >= 1)
	    rotate(zm->shape_info[current_shape],-(_2PI / 360.0),ref_port);
	  break;

	case 'Z':
	  if (mode >= 1)
	    move(zm->shape_info[current_shape],0,0,delta);
	  break;

	case 'z':
	  if (mode >= 1)
	    move(zm->shape_info[current_shape],0,0,-delta);
	  break;

	case 'X':
	  if (mode >= 1 && mode < 3)
	    move(zm->shape_info[current_shape],delta,0,0);
	  else if (mode == 4)
	    adjust_texture_coords(twrp[current_wrap*2],twrp[current_wrap*2+1],1.0,0,1);
	  else if (mode == 3)
	    adjust_texture_coords(current_poly,current_poly,0.98,1.0,1);
	  break;

	case 'x':
	  if (mode >= 1 && mode < 3)
	    move(zm->shape_info[current_shape],-delta,0,0);
	  else if (mode == 4)
	    adjust_texture_coords(twrp[current_wrap*2],twrp[current_wrap*2+1],-1.0,0,1);
	  else if (mode == 3)
	    adjust_texture_coords(current_poly,current_poly,1.2,1.0,1);
	  break;

	case 'Y':
	  if (mode >= 1 && mode < 3)
	    move(zm->shape_info[current_shape],0,delta,0);
	  else if (mode == 4)
	    adjust_texture_coords(twrp[current_wrap*2],twrp[current_wrap*2+1],0,1.0,1);
	  else if (mode == 3)
	    adjust_texture_coords(current_poly,current_poly,1.0,0.98,1);
	  break;

	case 'y':
	  if (mode >= 1 && mode < 3)
	    move(zm->shape_info[current_shape],0,-delta,0);
	  else if (mode == 4)
	    adjust_texture_coords(twrp[current_wrap*2],twrp[current_wrap*2+1],0,-1.0,1);
	  else if (mode == 3)
	    adjust_texture_coords(current_poly,current_poly,1.0,1.2,1);
	  break;

	case 'o':
	case 'O':
	  axis_on = !axis_on;
	  break;

	case '*':
	  scale_f += 1.0;
	  break;

	case '/':
	  scale_f -= 1.0;
	  break;


	case '<':
	  vtheta += _2PI / 60.0;
	  break;

	case ',':
	  vtheta -= _2PI / 60.0;
	  break;

	case '>':
	  vphi += _2PI / 60.0;
	  break;

	case '.':
	  vphi -= _2PI / 60.0;
	  break;
	  
	case ':':
	  vphi = 0.0;
	  break;

	case ';':
	  vtheta = 0.0;
	  break;

	case 'U':
	case 'u':
	  {
	    REAL_TYPE ud = c == 'U' ? 1.0 : -1.0;
	    if (mode == 4)
	      adjust_texture_coords(twrp[current_wrap*2],twrp[current_wrap*2+1],ud,0);
	    else if (mode == 3)
	      adjust_texture_coords(current_poly,current_poly,ud,0);
	  }
	  break;


	case 'V':
	case 'v':
	  {
	    REAL_TYPE vd = c == 'V' ? 1.0 : -1.0;
	    if (mode == 4)
	      adjust_texture_coords(twrp[current_wrap*2],twrp[current_wrap*2+1],0,vd);
	    else if (mode == 3)
	      adjust_texture_coords(current_poly,current_poly,0,vd);
	  }
	  break;

	case 'I':
	case 'i':
	    view_distance += 2 * world_scale;
	  break;

	case 'E':
	case 'e':
	  //	  swap(zm->shape_info[current_shape],1);
	  if (view_distance > 0)
	    view_distance -= 2 * world_scale;
	  break;

	case 'L':
	case 'l':
	  swap(zm->shape_info[current_shape],s_axis);
	  break;

	case '!':
	  reverse(zm->shape_info[current_shape]);
	  break;

	case '@':
	  reflect_axis();
	  break;

	case '#':
	  dupe_reverse();
	  break;
	  
	case '$':
	  interpolate_x(zm->shape_info[current_shape].polyinfos[current_poly]);
	  break;

	case '%':
	  interpolate_y(zm->shape_info[current_shape].polyinfos[current_poly]);
	  break;

	case '^':
	  interpolate_z(zm->shape_info[current_shape].polyinfos[current_poly]);
	  break;

	case '&':
	  interpolate_z(zm->shape_info[current_shape].polyinfos[current_poly],0);
	  break;

	case '(':
	  if (mode == 4)
	    apply_texture(current_txtr,twrp[current_wrap*2],twrp[current_wrap*2+1]);
	  else if (mode == 3)
	    apply_texture(current_txtr,current_poly,current_poly);
	  break;

	case ')':
	  dumpTxtrMaps();
	  break;

	case '_':
	  if (mode == 1)
	  {
	    R_3DPoint p;
	    centerShape(zm->shape_info[current_shape],p);
	  }
	  else if (mode == 2)
	    {
	      poly2xzPlane(zm->shape_info[current_shape].polyinfos[current_poly]);
	    }
	  break;

   case '"':
   if (mode == 1)
      append_rpoints(rp_head, rpoint::instance_count);
   break;

	}

    }
  return 1;
}

void showAxis(Port_3D &viewport, Port_3D &ref_port)
{
  R_3DPoint w1,w2;
  
  w1 = ref_port.look_from;
  
  ref_port.port2world(R_3DPoint(axis_len,0,0),&w2);
  zline(w1,w2,0,viewport);
  ref_port.port2world(R_3DPoint(-axis_len,0,0),&w2);
  zline(w1,w2,0,viewport);

  ref_port.port2world(R_3DPoint(0,axis_len,0),&w2);
  zline(w1,w2,1,viewport);
  ref_port.port2world(R_3DPoint(0,-axis_len,0),&w2);
  zline(w1,w2,1,viewport);

  ref_port.port2world(R_3DPoint(0,0,axis_len),&w2);
  zline(w1,w2,2,viewport);
  ref_port.port2world(R_3DPoint(0,0,-axis_len),&w2);
  zline(w1,w2,2,viewport);
}




