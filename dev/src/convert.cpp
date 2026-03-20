/*************************************************************
 * convert.cpp                                               *
 *************************************************************/
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

#include "simerr.h"
#include "simfile.h"
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
#include "convert.h"
#ifdef SABREWIN
#include "rpoint.h"
#endif
extern int s_axis;
extern float scale_f;
extern int mode;
extern Z_Node_Manager *zm;
extern int frame_switch;
extern int current_poly;
extern int current_shape;
extern Port_3D ref_port,vport;

/* For wrapping a texture around
   a selected set of co-axial polys */
int        *twrp = NULL;
int        twrp_idx = 0;
int        twrap_n = 0;

float gRatios[4][2] =
{
   { 1.0 / 3.0, 2.0 / 3.0 },
   { 1.0 / 3.0, 5.0 / 6.0 },
   { 1.0 / 6.0, 5.0 / 6.0 },
   { 1.0 / 6.0, 2.0 / 3.0 }
};

inline int RANDOM(int x)
{
  return (rand() % (x));
}

C_ShapeInfo &centerShape(C_ShapeInfo &cs, R_3DPoint &cp)
{
  cp = cs.bcube.centerPoint();
  cp *= -1.0;
  cs += cp;
  return (cs);
}

C_PolyInfo &centerPoly(C_PolyInfo &cp, R_3DPoint &p)
{
  p = cp.bcube.centerPoint();
  p *= -1.0;
  cp += p;
  return (cp);
}

void poly2xzPlane(C_PolyInfo &cp)
{
  R_3DPoint p;
  float     theta;
  float     cos_t,sin_t;
  float     dx,dy,dz;
  int       i;
  REAL_TYPE xx,yy,zz;
  bounding_cube bc;

  centerPoly(cp,p);
  measure_poly(cp,bc,&dx,&dy,&dz);
  cout << bc;
  /*
  dx = bc.max_x - bc.min_x;
  dy = bc.max_y - bc.min_y;
  dz = bc.max_x - bc.min_z;
  */

  dx = cp.lpoints[1].x - cp.lpoints[0].x;
  dy = cp.lpoints[1].y - cp.lpoints[0].y;

  printf("dx: %3.3f, dy: %3.3f\n",dx,dy);
  if (dx != 0)
    theta = atan2(dy,dx);
  else
    theta = 0.0;
  cos_t = cos(-theta);
  sin_t = sin(-theta);
  printf("-theta: %1.3f, cos(-theta): %1.3f, sin(-theta): %1.3f\n",
	 -theta,cos_t,sin_t);
  for (i=0;i<cp.npoints;i++)
    {
      xx = cp.lpoints[i].x * cos_t - cp.lpoints[i].y * sin_t;
      yy = cp.lpoints[i].x * sin_t + cp.lpoints[i].y * cos_t;
      printf("%3.2f==>%3.2f, %3.2f==>%3.2f\n",cp.lpoints[i].x,xx,
	     cp.lpoints[i].y,yy);
      cp.lpoints[i].x = xx;
      cp.lpoints[i].y = yy;
    }

  dz = cp.lpoints[1].z - cp.lpoints[0].z;
  dy = cp.lpoints[1].y - cp.lpoints[0].y;
  if (dy != 0.0)
    {
      printf("dz: %3.3f, dy: %3.3f\n",dz,dy);
      if (dz != 0.0)
	theta = atan2(dy,dz);
      else
	theta = 0.0;
      cos_t = cos(-theta);
      sin_t = sin(-theta);
      printf("-theta: %1.3f, cos(-theta): %1.3f, sin(-theta): %1.3f\n",
	     -theta,cos_t,sin_t);
      for (i=0;i<cp.npoints;i++)
	{
	  zz = cp.lpoints[i].z * cos_t - cp.lpoints[i].y * sin_t;
	  yy = cp.lpoints[i].z * sin_t + cp.lpoints[i].y * cos_t;
	  printf("%3.2f==>%3.2f, %3.2f==>%3.2f\n",cp.lpoints[i].z,zz,
		 cp.lpoints[i].y,yy);
	  cp.lpoints[i].z = zz;
	  cp.lpoints[i].y = yy;
	}
    }
  /*
  p *= -1.0;
  cp += p;
  */
}


void reverse(C_ShapeInfo &cs)
{
  int t,t1;
  if (mode == 1)
    {
      t = 0;
      t1 = cs.npolys;
    }
  else
    {
      t = current_poly;
      t1 = current_poly + 1;
    }
  for (int i=t;i<t1;i++)
    reverse_polyinfo(cs.polyinfos[i]);
}

void reflect_axis()
{
  C_ShapeInfo *cs = reflect(zm->shape_info[current_shape],s_axis);
  if (cs)
    {
      shape_params ps;
      ps.p_params = 0;
      ps = zm->s_params[current_shape];
      zm->add(*cs,ps);
      delete cs;
      adjust_params(current_shape);
    }
}


void dupe_reverse()
{
  C_ShapeInfo cs(zm->shape_info[current_shape]);
  for (int i=0;i<cs.npolys;i++)
    reverse_polyinfo(cs.polyinfos[i]);
  for (i=0;i<cs.npolys;i++)
    zm->shape_info[current_shape].add(cs.polyinfos[i]);
  adjust_params(current_shape);
}


C_ShapeInfo *reflect(C_ShapeInfo &cs, int which_axis)
{
  C_ShapeInfo *result = new C_ShapeInfo(cs);
  for (int i=0;i<result->npolys;i++)
    {
      C_PolyInfo &cp = result->polyinfos[i];
      for (int j=0;j<cp.npoints;j++)
	{
	  switch (which_axis)
	    {
	    case 0:
	    default:
	      cp.lpoints[j].x *= -1;
	      break;

	    case 1:
	      cp.lpoints[j].y *= -1;
	      break;

	    case 2:
	      cp.lpoints[j].z *= -1;
	      break;
	    }
	}
    }
  return (result);
}

void inline r_swap(REAL_TYPE &x, REAL_TYPE &y)
{
  REAL_TYPE swp = x;
  x = y;
  y = swp;
}
// swap coordinants
//
void swap(C_ShapeInfo &cs, int w)
{
  //	C_ShapeInfo &cs = zm->shape_info[current_shape];
  //	delta *= cs.pscale;
  int t,t1;
  if (mode == 1)
    {
      t = 0;
      t1 = cs.npolys;
    }
  else
    {
      t = current_poly;
      t1 = current_poly + 1;
    }
  for (int i=t;i<t1;i++)
    {
      C_PolyInfo &cp = cs.polyinfos[i];
      REAL_TYPE swp;
      for (int j=0;j<cp.npoints;j++)
	{
	  switch (w)
	    {
	      //swap z and y coords
	    case 0:
	      r_swap(cp.lpoints[j].z,cp.lpoints[j].y);
	      break;

	      // swap z and x coords
	    case 1:
	      r_swap(cp.lpoints[j].z,cp.lpoints[j].x);
	      break;

	      // swap x and y coords
	    case 2:
	      r_swap(cp.lpoints[j].x,cp.lpoints[j].y);
	      break;
	    }
	}
    }
}

void rotate(C_ShapeInfo &cs, float angle, Port_3D &ref_port)
{
  int t,t1;
  R_3DPoint p;

  centerShape(cs,p);

  if (mode == 1)
    {
      t = 0;
      t1 = cs.npolys;
    }
  else
    {
      t = current_poly;
      t1 = current_poly + 1;
    }
  Port_3D rport = ref_port;
  switch (s_axis)
    {
    case 0:
      rport > angle;
      break;

    case 1:
      rport < angle;
      break;

    case 2:
      rport / angle;
      break;
    }
  for (int i=t;i<t1;i++)
    {
      C_PolyInfo &cp = cs.polyinfos[i];
      for (int j=0;j<cp.npoints;j++)
	{
	  R_3DPoint wp;
	  rport.port2world(cp.lpoints[j],&wp);
	  ref_port.world2port(wp,&cp.lpoints[j]);
	}
    }
  p *= -1.0;
  cs += p;
}

void scale(C_ShapeInfo &cs, float delta, int all)
{
  int t,t1;
  R_3DPoint p;
  
  centerShape(cs,p);

  if (mode == 1)
    {
      t = 0;
      t1 = cs.npolys;
    }
  else
    {
      t = current_poly;
      t1 = current_poly + 1;
    }
  for (int i=t;i<t1;i++)
    {
      C_PolyInfo &cp = cs.polyinfos[i];
      for (int j=0;j<cp.npoints;j++)
	{
	  if (!all)
	    {
	      switch (s_axis)
		{
		case 0:
		  cp.lpoints[j].x *= delta;
		  break;

		case 1:
		  cp.lpoints[j].y *= delta;
		  break;

		case 2:
		  cp.lpoints[j].z *= delta;
		  break;
		}
	    }
	  else
	    {
	      cp.lpoints[j].x *= delta;
	      cp.lpoints[j].y *= delta;
	      cp.lpoints[j].z *= delta;
	    }
	}
    }

  p *= -1.0;
  cs += p;
}

void move(C_ShapeInfo &cs, float delta_x, float delta_y, float delta_z)
{
  int t,t1;
  if (mode == 1)
    {
      t = 0;
      t1 = cs.npolys;
    }
  else
    {
      t = current_poly;
      t1 = current_poly + 1;
    }
  for (int i=t;i<t1;i++)
    {
      C_PolyInfo &cp = cs.polyinfos[i];
      for (int j=0;j<cp.npoints;j++)
	{
	  cp.lpoints[j].x += delta_x;
	  cp.lpoints[j].y += delta_y;
	  cp.lpoints[j].z += delta_z;
	}
    }
}


int convert3(int m)
{
  if (m == 3)
    {
      ifstream is;
      if (open_is(is, "zv.conv3.dat"))
	{
	  for (int i=0;i<4;i++)
	    for (int j=0;j<2;j++)
	      is >> gRatios[i][j];
	}
    }
  poly2shape(zm->shape_info[current_shape],m);
  adjust_params(current_shape);
  return 1;
}

inline REAL_TYPE calc_run(R_3DPoint &p1, R_3DPoint &p2)
{
  REAL_TYPE result;
  REAL_TYPE dx,dy;
  dx = p2.x - p1.x;
  dy = p2.y - p1.y;
  if (dx == 0 && dy == 0)
    return 0.0;
  else
    result = sqrt((dx * dx) + (dy * dy));
  return result;
}

int convert5()
{
  REAL_TYPE y_step = 0.01;
  int steps = 3;
  int perc, perc2;
  REAL_TYPE top_perc;
  int n;
  int i;
  ifstream f("testshp.5");
  if (f)
    f >> y_step >> steps >> perc >> perc2 >> top_perc;
  f.close();

  C_ShapeInfo &cs = zm->shape_info[current_shape];
  C_PolyInfo *new_polys = new C_PolyInfo[steps];
  new_polys[0] = cs.polyinfos[0];
  for (i=1;i<steps;i++)
    {
      REAL_TYPE d;
      if (i == steps - 1)
	d = top_perc;
      else
	d = ((REAL_TYPE)(RANDOM(perc) + perc2)) / 100.0;
      C_PolyInfo *cp = dupe_poly_info(new_polys[i-1],d);
      new_polys[i] = *cp;
      delete cp;
      for (int j=0;j<new_polys[i].npoints;j++)
	new_polys[i].lpoints[j].y = new_polys[i-1].lpoints[j].y + y_step;
    }
  C_PolyInfo *tpolys = contour2polys(new_polys[0], new_polys[1], n);
  for (i=1;i<steps-1;i++)
    {
      int n2;
      C_PolyInfo *t2polys = contour2polys(new_polys[i],new_polys[i+1],n2);
      tpolys = cat_poly_infos(tpolys,n,t2polys,n2,n,0);
      delete [] t2polys;
    }
  /*
    cs.npolys = steps;
    cs.polyinfos = new_polys;
    */
  tpolys = cat_poly_infos(tpolys,n,&new_polys[steps-1],1,n,0);
  delete [] new_polys;
  delete [] cs.polyinfos;
  cs.npolys = n;
  cs.polyinfos = tpolys;
  adjust_params(current_shape);
  return 1;
}

int convert4()
{
  int x1,x2,x3,x4,x5,x6;
  REAL_TYPE x7,x8;
  ifstream params("testshp.mnt");
  if (params)
    params >> x1 >> x2 >> x3 >> x4 >> x5 >> x6 >> x7 >> x8;
  params.close();

  int i,n,tn;
  C_PolyInfo *temp_polys,*temp_polys2, *temp_polys3;
  C_PolyInfo *trap_poly,*tri_poly;
  C_ShapeInfo &cs = zm->shape_info[current_shape];
  //	for (i=0;i<cs.polyinfos[0].npoints;i++)
  //		cs.polyinfos[0].lpoints[i].y = x8;
  REAL_TYPE f1 = (REAL_TYPE) (RANDOM(x5) + 2);
  REAL_TYPE f2 = (REAL_TYPE) (RANDOM(x6) + 2);
  temp_polys = poly2pyramid(cs.polyinfos[0], &n, f1, f2);
  temp_polys2 = create_poly_info(n,3);
  temp_polys3 = NULL;
  tn = 0;

  int a = RANDOM(x1) + x2;
  for (int k=0;k<a;k++)
    {
      REAL_TYPE perc = (REAL_TYPE) (RANDOM(x3) + x4);
      REAL_TYPE perc2 = (REAL_TYPE) (RANDOM(x3) + x4);
      for (i=0;i<n;i++)
	{
	  tri2trap(temp_polys[i], &trap_poly, &tri_poly, perc);
	  temp_polys[i] = *trap_poly;
	  temp_polys2[i] = *tri_poly;
	  REAL_TYPE yd = temp_polys2[i].lpoints[1].y;
	  temp_polys2[i].lpoints[1].y -= yd * ( perc2 / 100.0 );
	  delete trap_poly;
	  delete tri_poly;
	}
      temp_polys3 = cat_poly_infos(temp_polys3, tn, temp_polys, n, tn, 0);
      if (k == a - 1)
	break;
      for (i=0;i<n;i++)
	temp_polys[i] = temp_polys2[i];
    }

  temp_polys2 = create_poly_info(1,n);
  for (i=0;i<n;i++)
    temp_polys2->lpoints[i] = temp_polys[i].lpoints[1];
  reverse_polyinfo(*temp_polys2);

  temp_polys3 = cat_poly_infos(temp_polys3, tn, temp_polys2, 1, tn, 0);
  // Alter the shape info
  delete [] cs.polyinfos;
  cs.polyinfos = temp_polys3;
  cs.npolys = tn;
  adjust_params(current_shape);
  return cs.npolys;
}

#ifdef SABREWIN
void append_rpoints(rpoint *rptr, int n)
{
   int i = 0;
   R_3DPoint *pnts = new R_3DPoint[n];
   while (i < n && rptr)
   {
      pnts[i] = rptr->pnt;
      rptr = rptr->next;
      i++;
   }
   C_PolyInfo newinfo(n,pnts);
   newinfo.delete_flag = 1;
   C_ShapeInfo &cs = zm->shape_info[current_shape];
   cs.add(newinfo);
   adjust_params(current_shape);
}
#endif

void adjust_params(int i)
{
  int n = zm->shape_info[i].npolys;
  poly_params pp = zm->s_params[i].p_params[0];
  delete zm->s_params[i].p_params;
  zm->s_params[i].p_params = new poly_params[n];
  for (int j=0;j<n;j++)
    zm->s_params[i].p_params[j] = pp;
  zm->create();
}


// This conversion takes a poly defined along the z-axis and produces
// a set of polys forming a 3-d convex shape based on the poly's dimensions.
// The poly must start with the 'most positive' vertex, that is the vertex
// with the highest x,y & z values. The other vertexes must be defined in
// counter-clockwise order from that vertex. The vertexes must be in pairs
// so that vertex i is matched with vertex (n - 1 - i), where n is the
// number of vertexes.
int poly2shape(C_ShapeInfo &cs, int cv_type)
{
  int i,idx1,idx2;
  C_PolyInfo *axis_polys;
  C_PolyInfo *end_polys;
  C_PolyInfo *p1,*p2;
  int n1,n2;
  int sz;
  int nn;

  C_PolyInfo &ply = cs.polyinfos[0];
  int n_axis = ply.npoints / 2;
  // Create set of axis polys
  axis_polys = new C_PolyInfo[n_axis];
  end_polys = new C_PolyInfo[2];
  MYCHECK(axis_polys != NULL);
  MYCHECK(end_polys != NULL);
  for (i=0;i<n_axis;i++)
    {
      axis_polys[i].lpoints = new R_3DPoint[4];
      MYCHECK(axis_polys[i].lpoints != NULL);
      axis_polys[i].npoints = 4;
      axis_polys[i].delete_flag = 1;
      if (i < 2)
	{
	  end_polys[i].lpoints = new R_3DPoint[4];
	  MYCHECK(end_polys[i].lpoints != NULL);
	  end_polys[i].npoints = 4;
	  end_polys[i].delete_flag = 1;
	}
    }
  R_3DPoint *ptr1 = &ply.lpoints[0];
  R_3DPoint *ptr2 = &ply.lpoints[ply.npoints - 1];
  for (i=0;i<n_axis;i++)
    {
      C_PolyInfo &cp = axis_polys[i];
      cp.lpoints[0] = *ptr1;
      cp.lpoints[1] = *ptr1;
      cp.lpoints[1].x *= -1;
      cp.lpoints[2] = *ptr2;
      cp.lpoints[2].x *= -1;
      cp.lpoints[3] = *ptr2;
      *ptr1++;
      *ptr2--;
    }
  // Copy the first & last axis polys
  // to the end polys
  for (i=0;i<4;i++)
    {
      end_polys[0].lpoints[i] = axis_polys[0].lpoints[i];
      end_polys[1].lpoints[i] = axis_polys[n_axis-1].lpoints[i];
    }
  // Reverse the first end poly
  //	reverse_polyinfo(end_polys[0]);

  // Now create 8-sided axis
  if (cv_type)
    {
      for (i=0;i<n_axis;i++)
	{
	  C_PolyInfo *cp;
	  switch (cv_type)
	    {
	    case 1:
	      cp = axis4to8(&axis_polys[i]);
	      break;

	    case 2:
	      cp = axis4to6(&axis_polys[i]);
	      break;

	    case 3:
	      cp = axis4toN(&axis_polys[i],gRatios);
	      break;
	    }
	  axis_polys[i] = *cp;
	  delete cp;
	  if (i < 2)
	    {
	      switch (cv_type)
		{
		case 1:
		  cp = axis4to8(&end_polys[i]);
		  break;
		  
		case 2:
		  cp = axis4to6(&end_polys[i]);
		  break;
		  
		case 3:
		  cp = axis4toN(&end_polys[i],gRatios);
		  break;
		}
	      end_polys[i] = *cp;
	      delete cp;
	    }
	}
    }

  reverse_polyinfo(end_polys[0]);

  // Create indexes into polys
  // so we can selectively wrap a texture around
  // them
  twrp = new int[n_axis * 2];
  MYCHECK(twrp != NULL);
  twrp_idx = 0;
  twrap_n = 0;
  // First axis pair
  idx1 = 0;
  idx2 = 1;
  p1 = axis2shape(axis_polys[idx2],axis_polys[idx1],n1);
  twrp[0] = 0;
  twrp[1] = n1 - 1;
  twrp_idx = 2;
  twrap_n++;
  idx1++;
  idx2++;
  nn = n1;

  while (idx2 < n_axis)
    {
      p2 = axis2shape(axis_polys[idx2],axis_polys[idx1],n2);
      p1 = cat_poly_infos(p1,n1,p2,n2,sz);
      twrp[twrp_idx] = nn;
      nn += n2;
      twrp[twrp_idx+1] = nn - 1;
      twrp_idx += 2;
      twrap_n++;
      n1 = sz;
      idx1++;
      idx2++;
    }
  // Add on the end axis
  p1 = cat_poly_infos(p1,n1,end_polys,2,sz);
  n1 = sz;
  delete [] cs.polyinfos;
  cs.polyinfos = p1;
  cs.npolys = n1;
  /*
  printf("%d\n",twrap_n);
  for (i=0;i<twrap_n * 2;i+=2)
    printf("%d %d\n",twrp[i],twrp[i+1]);
    */
  return n1;
}

// This conversion takes the two axis polys
// and creates the polys between them.
// Both polys must have the same number of sides and represent the
// axis polys of a convex shape. Also, both polys must start with the
// same vertex, and go in the same direction.
// The function returns an array of polys, and sets n equal
// to the size of the array.
C_PolyInfo *axis2shape(C_PolyInfo &axis_1, C_PolyInfo &axis_2, int &n)
{
  C_PolyInfo *result;
  int i;
  // Total perimeter
  float tper1 = 0.0;
  float  per1 = 0.0;
  float tper2 = 0.0;
  float  per2 = 0.0;
  float  *d1;
  float  *d2;
  float  *dd;
  float ttper;
 
  int n_faces = axis_1.npoints;
  // Create temporary set of polys
  result = new C_PolyInfo[n_faces];
  MYCHECK(result != NULL);
  d1 = new float[n_faces];
  MYCHECK(d1 != NULL);
  d2 = new float[n_faces];
  MYCHECK(d2 != NULL);
  // Initialize
  for (i=0;i<n_faces;i++)
    {
      result[i].lpoints = new R_3DPoint[4];
      MYCHECK(result[i].lpoints != NULL);
      result[i].tpoints = new TxtPoint[4];
      MYCHECK(result[i].tpoints != NULL);
      result[i].npoints = 4;
      result[i].delete_flag = 1;
    }
  // Create the faces
  int idx1 = 0;
  R_3DPoint *ptr1 = axis_1.lpoints;
  R_3DPoint *ptr2 = axis_2.lpoints;
  for (i=0;i<n_faces;i++)
    {
      if (i == n_faces - 1)
	idx1 = 0;
      else
	idx1 = i + 1;
      R_3DPoint *ptr = result[i].lpoints;
      ptr[0] = ptr1[i];
      ptr[1] = ptr2[i];
      ptr[2] = ptr2[idx1];
      ptr[3] = ptr1[idx1];
      TxtPoint *tptr = result[i].tpoints;
      tptr[0] = TxtPoint(0,0,0);
      tptr[1] = TxtPoint(127,0,0);
      tptr[2] = TxtPoint(127,127,0);
      tptr[3] = TxtPoint(0,127,0);
      d1[i] = distance(ptr1[i],ptr1[idx1]);
      tper1 += d1[i];
      d2[i] = distance(ptr2[i],ptr2[idx1]);
      tper2 += d2[i];
    }

  //  printf("tper1: %3.3f, tper2: %3.3f\n",tper1,tper2);
  // Adjust the texture coordinants so that
  // the texture wraps around
  if (tper1 >= tper2)
    {
      ttper = tper1;
      dd = d1;

    }
  else
    {
      ttper = tper2;
      dd = d2;
    }

  per1 = per2 = 0.0;
  for (i=0;i<n_faces;i++)
    {
      TxtPoint *tptr = result[i].tpoints;
      tptr[0].v = 127 * per1 / ttper;
      tptr[1].v = 127 * per2 / ttper;
      tptr[2].v = 127 * (per2 + dd[i]) / ttper;
      tptr[3].v = 127 * (per1 + dd[i]) / ttper;
      per1 += dd[i];
      per2 += dd[i];
     }
  n = n_faces;
  if (d1)
    delete [] d1;
  if (d2)
    delete [] d2;

  return result;
}

int convert2(C_ShapeInfo &cs)
{
  int i,n;
  C_PolyInfo *temp_polys;
  C_PolyInfo &axis_neg = cs.polyinfos[0];
  C_PolyInfo &axis_pos = cs.polyinfos[1];
  temp_polys = axis2shape(axis_pos,axis_neg,n);
  // Alter the shape info
  delete [] cs.polyinfos;
  cs.polyinfos = temp_polys;
  cs.npolys = n;
  adjust_params(current_shape);
  return n;
}

// This conversion takes a 4 sided shape and converts it into
// an 8-sided one. Assumptions: poly[0] & poly[1] are the two
// axis faces (ie, perp to the z-axis). These also need be the
// only two faces specified
int convert1(C_ShapeInfo &cs)
{
  // Perform the axis conversion
  C_PolyInfo *cp = axis4to8(& cs.polyinfos[0]);
  cs.polyinfos[0] = *cp;
  delete cp;
  cp = axis4to8(& cs.polyinfos[1]);
  cs.polyinfos[1] = *cp;
  delete cp;
  return (cp != NULL);
}

// Convert a four-vertex axis poly to an eight-vertex one.
C_PolyInfo *axis4to8(C_PolyInfo *axis_poly)
{
  REAL_TYPE d,d1,d2,d3;
  C_PolyInfo *result;
  R_3DPoint *lp = &axis_poly->lpoints[0];
  R_3DPoint *new_points = new R_3DPoint[8];
  R_3DPoint *a,*b;
  int new_idx = 0;
  for (int i = 0;i<4;i++)
    {
      a = &axis_poly->lpoints[i];
      if (i < 3)
	b = &axis_poly->lpoints[i+1];
      else
	b = lp;
      if (r_equal(a->x,b->x))
	{
	  new_points[new_idx].x = a->x;
	  new_points[new_idx+1].x = a->x;
	  d = b->y - a->y;
	  if (r_equal(b->z,a->z))
	    d1 = 0.0;
	  else
	    d1 = (b->z - a->z) / d;
	  d2 = (d / 3.0);
	  d3 = (2 * (d / 3.0));
	  new_points[new_idx].y = a->y + d2;
	  new_points[new_idx+1].y = a->y + d3;
	  new_points[new_idx].z = a->z + (d1 * d2);
	  new_points[new_idx+1].z = a->z + (d1 * d3);
	}
      else
	{
	  new_points[new_idx].z = a->z;
	  new_points[new_idx+1].z = b->z;
	  new_points[new_idx].y = a->y;
	  new_points[new_idx+1].y = a->y;
	  d = b->x - a->x;
	  new_points[new_idx].x = a->x + (d / 3.0);
	  new_points[new_idx+1].x = a->x + (2 * (d / 3.0));
	}
      new_idx += 2;
    }
  result = new C_PolyInfo(8,new_points);
  result->bcube = axis_poly->bcube;
  result->delete_flag  = 1;
  return (result);
}

// Convert a four-vertex axis poly to an eight-vertex one,
// using a set of ratios 
C_PolyInfo *axis4toN(C_PolyInfo *axis_poly, float ratios[][2])
{
  REAL_TYPE d,d1,d2,d3;
  C_PolyInfo *result;
  R_3DPoint *lp = &axis_poly->lpoints[0];
  R_3DPoint *new_points = new R_3DPoint[8];
  R_3DPoint *a,*b;
  int new_idx = 0;
  for (int i = 0;i<4;i++)
    {
      a = &axis_poly->lpoints[i];
      if (i < 3)
	b = &axis_poly->lpoints[i+1];
      else
	b = lp;
      if (r_equal(a->x,b->x))
	{
	  new_points[new_idx].x = a->x;
	  new_points[new_idx+1].x = a->x;
	  d = b->y - a->y;
	  d2 = d * ratios[i][0];
	  d3 = d * ratios[i][1];
	  if (r_equal(b->z,a->z))
	    d1 = 0.0;
	  else
	    d1 = (b->z - a->z) / d;
	  new_points[new_idx].y = a->y + d2;
	  new_points[new_idx+1].y = a->y + d3;
	  new_points[new_idx].z = a->z + (d1 * d2);
	  new_points[new_idx+1].z = a->z + (d1 * d3);
	}
      else
	{
	  new_points[new_idx].z = a->z;
	  new_points[new_idx+1].z = b->z;
	  new_points[new_idx].y = a->y;
	  new_points[new_idx+1].y = a->y;
	  d = b->x - a->x;
	  new_points[new_idx].x = a->x + d * ratios[i][0];
	  new_points[new_idx+1].x = a->x + d * ratios[i][1];
	}
      new_idx += 2;
    }
  result = new C_PolyInfo(8,new_points);
  result->bcube = axis_poly->bcube;
  result->delete_flag  = 1;
  return (result);
}

// Convert a four-vertex axis poly to a six-vertex one.
// Assumes the first two vertexes are the top right & top left
// i.e.,
C_PolyInfo *axis4to6(C_PolyInfo *axis_poly)
{
  REAL_TYPE d,d1,d2,d3;
  C_PolyInfo *result;
  R_3DPoint *lp = &axis_poly->lpoints[0];
  R_3DPoint *new_points = new R_3DPoint[6];
  R_3DPoint *a,*b;
  int new_idx = 0;
  for (int i = 0;i<4;i++)
    {
      a = &axis_poly->lpoints[i];
      if (i < 3)
	b = &axis_poly->lpoints[i+1];
      else
	b = lp;
      if (i==2)
	{
	  new_points[new_idx++] = *a;
	  new_points[new_idx++] = *b;
	  continue;
	}
      if (r_equal(a->x,b->x))
	{
	  d = b->y - a->y;
	  if (r_equal(b->z,a->z))
	    d1 = 0.0;
	  else
	    d1 = (b->z - a->z) / d;
	  d2 = (d / 3.0);
	  d3 = (2 * (d / 3.0));

	  if (i == 0 || i == 1)
	    {
	      new_points[new_idx].x = a->x;
	      new_points[new_idx].y = a->y + d2;
	      new_points[new_idx].z = a->z + (d1 * d2);
	      new_idx++;
	    }
	  if (i == 0 || i == 3)
	    {
	      new_points[new_idx].x = a->x;
	      new_points[new_idx].y = a->y + d3;
	      new_points[new_idx].z = a->z + (d1 * d3);
	      new_idx++;
	    }
	}
      else
	{
	  d = b->x - a->x;

	  if (i==0 || i==1)
	    {
	      new_points[new_idx].z = a->z;
	      new_points[new_idx].y = a->y;
	      new_points[new_idx].x = a->x + (d / 3.0);
	      new_idx++;
	    }
	  if (i==0 || i==3)
	    {
	      new_points[new_idx].y = a->y;
	      new_points[new_idx].z = b->z;
	      new_points[new_idx].x = a->x + (2 * (d / 3.0));
	      new_idx++;
	    }
	}
    }
  result = new C_PolyInfo(6,new_points);
  result->bcube = axis_poly->bcube;
  result->delete_flag  = 1;
  return (result);
}

// Combine two arrays of poly infos into one. This function will delete
// the two argument arrays
C_PolyInfo *cat_poly_infos(C_PolyInfo *cp1, int size1, C_PolyInfo *cp2,
			   int size2, int &size, int del)
{
  C_PolyInfo *result;
  int i;
  int idx = 0;
  size = size1 + size2;
  result = new C_PolyInfo[size];
  MYCHECK(result != NULL);
  for (i=0;i<size1;i++)
    result[idx++] = cp1[i];
  for (i=0;i<size2;i++)
    result[idx++] = cp2[i];
  if (del && cp1 != NULL)
    delete [] cp1;
  if (del && cp2 != NULL)
    delete [] cp2;
  return result;
}

// Reverse the order of vertexes in a poly info, retaining the
// first vertex in place
void reverse_polyinfo(C_PolyInfo &ply)
{
  R_3DPoint tmp;
  R_3DPoint *rp;
  TxtPoint  txttmp;
  TxtPoint  *tp;
  int idx = 1;
  int idx2 = ply.npoints - 1;

  rp = ply.lpoints;
  tp = ply.tpoints;
  while (idx < idx2)
    {
      tmp = rp[idx];
      rp[idx] = rp[idx2];
      rp[idx2] = tmp;

      if (tp != NULL)
	{
	  txttmp = tp[idx];
	  tp[idx] = tp[idx2];
	  tp[idx2] = txttmp;
	}

      idx++;
      idx2--;
    }
}

void port2world(char *outfilename)
{
  Z_Viewer zview(zm);
  zview.draw_shadow = 0;
  zview.reference_port = &ref_port;
  zview.draw_prep(vport);
  ofstream out;
  open_os(out,outfilename);
  out << zview;
}

// takes 2 points p1 & p2, and finds p' such that p' is y units
// on the y-axis up from p1
R_3DPoint step_y(R_3DPoint &p1, R_3DPoint &p2, REAL_TYPE y)
{
  REAL_TYPE t;
  Vector v;
  v = Vector(p2 - p1);
  t = y / v.Y;
  v *= t;
  v += Vector(p1);
  return R_3DPoint(v);
}

void measure_poly(C_PolyInfo & cp, bounding_cube & bc,
		  REAL_TYPE *x, REAL_TYPE *y, REAL_TYPE *z)
{
  bc.flg = 0;
  for (int i=0;i<cp.npoints;i++)
    bc.set(cp.lpoints[i]);
  *x = bc.max_x - bc.min_x;
  *y = bc.max_y - bc.min_y;
  *z = bc.max_z - bc.min_z;
}

// Take a convex poly defined on the x,z plane
C_PolyInfo *poly2pyramid(C_PolyInfo &cp, int *cnt, REAL_TYPE f1, REAL_TYPE f2)
{
  int i;
  C_PolyInfo *result;
  bounding_cube bcube;
  REAL_TYPE x,y,z;
  int idx;
  result = create_poly_info(cp.npoints,3);
  *cnt = cp.npoints;
  measure_poly(cp,bcube,&x,&y,&z);
  for (i=0;i<cp.npoints;i++)
    cp.lpoints[i].y = 0.0;
  R_3DPoint center = R_3DPoint(bcube.min_x+(x/f1),
			       bcube.max_y,bcube.min_z+(z/f2));
  for (i=0;i<cp.npoints;i++)
    {
      if (i == cp.npoints - 1)
	idx = 0;
      else
	idx = i + 1;
      result[i].lpoints[0] = cp.lpoints[i];
      result[i].lpoints[1] = center;
      result[i].lpoints[2] = cp.lpoints[idx];
    }
  return result;
}

C_PolyInfo *create_poly_info(int n, int np)
{
  C_PolyInfo *result;
  result = new C_PolyInfo[n];
  MYCHECK(result != NULL);
  for (int i=0;i<n;i++)
    {
      result[i].lpoints = new R_3DPoint[np];
      MYCHECK(result[i].lpoints != NULL);
      result[i].npoints = np;
      result[i].delete_flag = 1;
    }
  return result;
}

// Given a triangle, create a trapezoid, of height (perc / 100) * max_y. Returns
// the trapezoid in p1, the remaining triangle in p2.
// Assumptions: the triangle is in counter-clockwise order, with the
// middle point being the apex
void tri2trap(C_PolyInfo &p, C_PolyInfo **p1, C_PolyInfo **p2, REAL_TYPE perc)
{
  REAL_TYPE yd;
  yd = fabs(p.lpoints[1].y - p.lpoints[0].y) * perc / 100.0;
  //	yd = perc;
  C_PolyInfo *trapzd = create_poly_info(1,4);
  C_PolyInfo *trngl = create_poly_info(1,3);
  trapzd->lpoints[0] = p.lpoints[0];
  trapzd->lpoints[3] = p.lpoints[2];
  trngl->lpoints[1] = p.lpoints[1];
  trapzd->lpoints[1] = step_y(p.lpoints[0],p.lpoints[1],yd);
  trapzd->lpoints[2] = step_y(p.lpoints[2],p.lpoints[1],yd);
  trngl->lpoints[0] = trapzd->lpoints[1];
  trngl->lpoints[2] = trapzd->lpoints[2];
  *p1 = trapzd;
  *p2 = trngl;
}

C_PolyInfo *dupe_poly_info(C_PolyInfo &cp, REAL_TYPE scale)
{
  C_PolyInfo *result;

  result = create_poly_info(1, cp.npoints);
  for (int i=0;i<cp.npoints;i++)
    {
      result->lpoints[i] = cp.lpoints[i];
      result->lpoints[i] *= scale;
    }
  return result;
}

C_PolyInfo *contour2polys(C_PolyInfo &bot_c, C_PolyInfo &top_c, int &n)
{
  C_PolyInfo *result;
  int npoints = bot_c.npoints;
  //	n = npoints * 2;
  n = npoints;
  int j;
  //	result = create_poly_info(n,3);
  result = create_poly_info(n,4);
  int r = 0;
  for (int i=0;i<npoints;i++)
    {
      if (i == npoints - 1)
	j = 0;
      else
	j = i + 1;
      C_PolyInfo &cp = result[r++];
      cp.lpoints[0] = bot_c.lpoints[i];
      cp.lpoints[1] = bot_c.lpoints[j];
      cp.lpoints[2] = top_c.lpoints[j];
      cp.lpoints[3] = top_c.lpoints[i];
      /*
	C_PolyInfo &cp2 = result[r++];
	cp2.lpoints[0] = bot_c.lpoints[j];
	cp2.lpoints[1] = top_c.lpoints[j];
	cp2.lpoints[2] = top_c.lpoints[i];
	*/
    }
  return result;
}


void interpolate_x(C_PolyInfo &pi)
{
  REAL_TYPE t;
  R_3DPoint p0,p1;
  int i,j;
  int halfp = pi.npoints / 2;

  for (i=1;i<pi.npoints-1;i++)
    {
      R_3DPoint &p = pi.lpoints[i];
      if (p.x < 0.0)
	{

	  j = i - 1;
	  while (j > -1)
	    {
	      if (pi.lpoints[j].x >= 0.0)
		{
		  p0 = pi.lpoints[j];
		  break;
		}
	      j--;
	    }
	  if (j == -1)
	    {
	      printf("Interpolation error!\n");
	      return;
	    }
	  j = i + 1;
	  while (j < pi.npoints)
	    {
	      if (pi.lpoints[j].x >= 0.0)
		{
		  p1 = pi.lpoints[j];
		  break;
		}
	      j++;
	    }

	  if (j == pi.npoints)
	    {
	      printf("Interpolation error!\n");
	      return;
	    }
	 
	  if (i >= halfp)
	    {
	      R_3DPoint tmp = p0;
	      p0 = p1;
	      p1 = tmp;
	    }

	  t = (p.z - p0.z) / (p1.z - p0.z);
	  t = fabs(t);
	  p.x = p0.x + (t * (p1.x - p0.x));
	  
	}
    }
}

void interpolate_y(C_PolyInfo &pi)
{
  REAL_TYPE t;
  R_3DPoint p0,p1;
  int i,j;
  int halfp = pi.npoints / 2;
  printf("interpolate_y\n");
  for (i=1;i<pi.npoints-1;i++)
    {
      R_3DPoint &p = pi.lpoints[i];
      if (fabs(p.y) == 0.0)
	{

	  j = i - 1;
	  while (j > -1)
	    {
	      if (fabs(pi.lpoints[j].y) > 0.0)
		{
		  p0 = pi.lpoints[j];
		  break;
		}
	      j--;
	    }
	  if (j == -1)
	    {
	      printf("Interpolation error!\n");
	      return;
	    }
	  j = i + 1;
	  while (j < pi.npoints)
	    {
	      if (fabs(pi.lpoints[j].y) > 0.0)
		{
		  p1 = pi.lpoints[j];
		  break;
		}
	      j++;
	    }

	  if (j == pi.npoints)
	    {
	      printf("Interpolation error!\n");
	      return;
	    }
	 
	  if (i >= halfp)
	    {
	      R_3DPoint tmp = p0;
	      p0 = p1;
	      p1 = tmp;
	    }

	  t = (p.z - p0.z) / (p1.z - p0.z);
	  t = fabs(t);
	  p.y = p0.y + (t * (p1.y - p0.y));
	  
	  
	}
    }
}


void interpolate_z(C_PolyInfo &pi, int over_x)
{
  REAL_TYPE t;
  R_3DPoint p0,p1;
  int i,j;
  int halfp = pi.npoints / 2;
  printf("interpolate_y\n");
  for (i=1;i<pi.npoints-1;i++)
    {
      R_3DPoint &p = pi.lpoints[i];
      if (fabs(p.z) == 0.0)
	{

	  j = i - 1;
	  while (j > -1)
	    {
	      if (fabs(pi.lpoints[j].z) > 0.0)
		{
		  p0 = pi.lpoints[j];
		  break;
		}
	      j--;
	    }
	  if (j == -1)
	    {
	      printf("Interpolation error!\n");
	      return;
	    }
	  j = i + 1;
	  while (j < pi.npoints)
	    {
	      if (fabs(pi.lpoints[j].z) > 0.0)
		{
		  p1 = pi.lpoints[j];
		  break;
		}
	      j++;
	    }

	  if (j == pi.npoints)
	    {
	      printf("Interpolation error!\n");
	      return;
	    }
	 
	  if (i >= halfp)
	    {
	      R_3DPoint tmp = p0;
	      p0 = p1;
	      p1 = tmp;
	    }
	  
	  if (over_x)
	    t = (p.x - p0.x) / (p1.x - p0.x);
	  else
	    t = (p.y - p0.y) / (p1.y - p0.y);
	  t = fabs(t);
	  p.z = p0.z + (t * (p1.z - p0.z));
	  
	  
	}
    }
}

void adjust_texture_coords(int n, int m, REAL_TYPE ud, REAL_TYPE vd, int flg)
{
  C_ShapeInfo &cs = zm->shape_info[current_shape];
  for (int i=n;i<=m;i++)
    {
      C_PolyInfo &ply = cs.polyinfos[i];
      if (ply.tpoints != NULL)
	{
	  if (flg)
	    {
	      for (int j=0;j<ply.npoints;j++)
		{
		  ply.tpoints[j].u *= ud;
		  ply.tpoints[j].v *= vd;
		}
	    }
	  else
	    {
	      for (int j=0;j<ply.npoints;j++)
		{
		  ply.tpoints[j].u += ud;
		  ply.tpoints[j].v += vd;
		}
	    }
	}
    }
}

void apply_texture(int tmap_idx, int n, int m)
{
  C_ShapeInfo &cs = zm->shape_info[current_shape];
  TextrMap &tmap = map_man->get_map(tmap_idx);
  if (&tmap != &nullmap)
    {
      for (int i=n;i<=m;i++)
	{
	  C_PolyInfo &ply = cs.polyinfos[i];
	  if (ply.tpoints != NULL)
	    {
	      zm->s_params[current_shape].p_params[i].flags |= TXTR_MAP;
	      zm->s_params[current_shape].p_params[i].tmap = tmap_idx;
	    }
	  else if (apply_texture(ply,tmap))
	    {
	      zm->s_params[current_shape].p_params[i].flags |= TXTR_MAP;
	      zm->s_params[current_shape].p_params[i].tmap = tmap_idx;
	    }
	}
      zm->shapes[current_shape].set_params(&(zm->s_params[current_shape]));
    }
}

void apply_texture(int cn, int n)
{
  C_ShapeInfo &cs = zm->shape_info[cn];
  TextrMap &tmap = map_man->get_map(n);
  if (&tmap != &nullmap)
    {
      int t,t1;
      if (mode == 1)
	{
	  t = 0;
	  t1 = cs.npolys;
	}
      else
	{
	  t = current_poly;
	  t1 = current_poly + 1;
	}
      for (int i=t;i<t1;i++)
	{
	  C_PolyInfo &ply = cs.polyinfos[i];
	  //	  if (apply_texture(ply,tmap))
	  if (ply.tpoints != NULL)
	    {
	      zm->s_params[cn].p_params[i].flags |= TXTR_MAP;
	      zm->s_params[cn].p_params[i].tmap = n;
	      zm->shapes[cn].set_params(&(zm->s_params[cn]));
	    }
	}
    }
}


int apply_texture(C_PolyInfo &ply, TextrMap &tmap)
{
  switch (s_axis)
    {
    case xz_axis:
      return (apply_texture_xz(ply,tmap));
      break;

    case xy_axis:
      return (apply_texture_xy(ply,tmap));
      break;

    case yz_axis:
      return (apply_texture_yz(ply,tmap));
      break;

    }
}

int apply_texture_xz(C_PolyInfo &ply, TextrMap &tmap)
{
  float xspan,yspan;
  float min_x,min_y;
  float dx,dy;
  float th,tw;
  th = (float) tmap.map_h - 1;
  tw = (float) tmap.map_w - 1;
  if (ply.tpoints == NULL)
    {
      ply.tpoints = new TxtPoint[ply.npoints];
      MYCHECK(ply.tpoints != NULL);
    }
  xspan = (float) (ply.bcube.max_x - ply.bcube.min_x);
  yspan = (float) (ply.bcube.max_z - ply.bcube.min_z);
  min_x = (float) ply.bcube.min_x;
  min_y = (float) ply.bcube.min_z;
  for (int i=0;i<ply.npoints;i++)
    {
      R_3DPoint &p = ply.lpoints[i];
      dx = fabs((((float)p.x) - min_x) / xspan);
      dy = fabs((p.z - min_y) / yspan);
      ply.tpoints[i].u = tw * dx;
      ply.tpoints[i].v = th * dy;
    }
  return(1);
}

int apply_texture_xy(C_PolyInfo &ply, TextrMap &tmap)
{
  float xspan,yspan;
  float min_x,min_y;
  float dx,dy;
  float th,tw;
  th = (float) tmap.map_h - 1;
  tw = (float) tmap.map_w - 1;
  if (ply.tpoints == NULL)
    {
      ply.tpoints = new TxtPoint[ply.npoints];
      MYCHECK(ply.tpoints != NULL);
    }
  xspan = (float) (ply.bcube.max_x - ply.bcube.min_x);
  yspan = (float) (ply.bcube.max_y - ply.bcube.min_y);
  min_x = (float) ply.bcube.min_x;
  min_y = (float) ply.bcube.min_y;
  for (int i=0;i<ply.npoints;i++)
    {
      R_3DPoint &p = ply.lpoints[i];
      dx = fabs((((float)p.x) - min_x) / xspan);
      dy = fabs((p.y - min_y) / yspan);
      ply.tpoints[i].u = tw * dx;
      ply.tpoints[i].v = th * dy;
    }
  return(1);
}

int apply_texture_yz(C_PolyInfo &ply, TextrMap &tmap)
{
  float xspan,yspan;
  float min_x,min_y;
  float dx,dy;
  float th,tw;
  th = (float) tmap.map_h - 1;
  tw = (float) tmap.map_w - 1;
  if (ply.tpoints == NULL)
    {
      ply.tpoints = new TxtPoint[ply.npoints];
      MYCHECK(ply.tpoints != NULL);
    }
  xspan = (float) (ply.bcube.max_z - ply.bcube.min_z);
  yspan = (float) (ply.bcube.max_y - ply.bcube.min_y);
  min_x = (float) ply.bcube.min_z;
  min_y = (float) ply.bcube.min_y;
  for (int i=0;i<ply.npoints;i++)
    {
      R_3DPoint &p = ply.lpoints[i];
      dx = fabs((((float)p.z) - min_x) / xspan);
      dy = fabs((p.y - min_y) / yspan);
      ply.tpoints[i].u = tw * dx;
      ply.tpoints[i].v = th * dy;
    }
  return(1);
}

void dumpTxtrMaps()
{
  ofstream os("txtrmap.dmp");
  for (int i=0;i<map_man->n_maps;i++)
    {
      TextrMap &tm = map_man->get_map(i);
      if (&tm != &nullmap)
	enumColors(os,tm);
    }
}

void enumColors(ostream &os, TextrMap &tmap)
{
  char r,g,b;
  int cc[256];
  int i,ii;
  unsigned char *cptr;
  int n;

  for (i=0;i<256;i++)
    cc[i] = 0;

  n = tmap.map_w * tmap.map_h;
  cptr = tmap.getBytes();

  for (i=0;i<n;i++)
    {
      ii = (int) *cptr++;
      cc[ii] = 1;
    }

  os << "{\n" << tmap.id << " " << tmap.map_w << " " << tmap.map_h  <<  " "
     << tmap.trans_colr << '\n';
  for (i=0;i<256;i++)
    {
      if (cc[i])
	{
	  get_rgb_value(i,&r,&g,&b);
	  os << "\t(" << i << " " << (int) r << " " << (int) g << " " << (int) b << ")\n";
	}
    }
  os << "}\n";
}

/*****************************************************
 * Create 1 shape info containing all the polys.     *
 * Useful for creating static, world objects         *
 *****************************************************/
void consolidatePolys()
{
  int total_polys = 0;
  int idx;
  C_PolyInfo *new_polys;
  poly_params *new_params;
  
  C_ShapeInfo *new_shape_info;
  shape_params *new_sparams;

  for (int i=0;i<zm->n_shapes;i++)
    total_polys += zm->shape_info[i].npolys;
  
  new_polys = new C_PolyInfo[total_polys];
  new_params = new poly_params[total_polys];

  MYCHECK(new_polys != NULL);
  idx = 0;
  for (i=0;i<zm->n_shapes;i++)
    {
      int j;
      C_ShapeInfo &cs = zm->shape_info[i];
      shape_params &sp = zm->s_params[i];
      for (j=0;j<cs.npolys;j++)
	{
	  new_polys[idx] = cs.polyinfos[j];
	  new_params[idx++] = sp.p_params[j];
	}
    }

  new_shape_info = new C_ShapeInfo(total_polys,new_polys);
  new_shape_info->delete_flag = 1;
  MYCHECK(new_shape_info != NULL);
  new_sparams = new shape_params(total_polys,zm->s_params[0].flags,
				new_params);
  delete [] zm->shape_info;
  delete [] zm->s_params;

  zm->shape_info = new_shape_info;
  zm->s_params = new_sparams;
  zm->n_shapes = 1;
  zm->buildShapes();

}

