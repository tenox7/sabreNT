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
 *        Sabre Fighter Plane Simulator          *
 * File   : sfacei.h                             *
 * Date   : December, 1999                       *
 * Author : Dan Hammer                           *
 *************************************************/
#ifndef __sfacei_h
#define __sfacei_h

class sFaceInfo
{
  int			npoints;
  int			*pidxs;
  R_3DPoint *wPoints;
  TxtPoint  *tPoints;
  int       flags;
  int       tag;
public:
  int npoints;
  R_3DPoint *lpoints;
  TxtPoint  *tpoints;
  int delete_flag;
  bounding_cube bcube;

  C_PolyInfo()
    : npoints(0),
    lpoints(NULL),
    tpoints(NULL),
    delete_flag(0)
    {}

  C_PolyInfo(int np, R_3DPoint *pnts, TxtPoint *tpnts = NULL)
    :npoints(np),
    lpoints(pnts),
    tpoints(tpnts),
    delete_flag(0)
    {}

  C_PolyInfo(const C_PolyInfo &cp)
    {
      lpoints = NULL;
      tpoints = NULL;
      copy (cp);
    }

  C_PolyInfo &operator =(const C_PolyInfo &cp)
    {
      copy(cp);
      return(*this);
    }

  ~C_PolyInfo()
    { if (lpoints && delete_flag) delete [] lpoints; }

  C_PolyInfo &operator *=(REAL_TYPE scaler);
  C_PolyInfo &operator +=(const R_3DPoint &p);

  void copy(const C_PolyInfo &);

  void read(istream &);
  void write(ostream &);

  friend inline istream &operator >>(istream &is, C_PolyInfo &cp);
  friend inline ostream &operator <<(ostream &os, C_PolyInfo &cp);

};

inline istream &operator >>(istream &is, C_PolyInfo &cp)
{
  cp.read(is);
  return(is);
}

inline ostream &operator <<(ostream &os, C_PolyInfo &cp)
{
  cp.write(os);
  return(os);
}

class C_ShapeInfo
{
public:
  int npolys;
  C_PolyInfo *polyinfos;
  int delete_flag;
  bounding_cube bcube;


  C_ShapeInfo()
    : npolys(0),
    polyinfos(NULL),
    delete_flag(0)
    {}

  C_ShapeInfo(int np, C_PolyInfo *ip)
    :npolys(np),
    polyinfos(ip),
    delete_flag(0)
    {}

  C_ShapeInfo(const C_ShapeInfo &cs)
    {
      polyinfos = NULL;
      copy(cs);
    }

  C_ShapeInfo &operator =(const C_ShapeInfo &cs)
    {
      copy(cs);
      return(*this);
    }

  ~C_ShapeInfo()
    { if (delete_flag && polyinfos) delete [] polyinfos; }

  C_ShapeInfo &operator *=(REAL_TYPE scaler);
  C_ShapeInfo &operator +=(const R_3DPoint &p);
  void copy(const C_ShapeInfo &);
  void add(const C_PolyInfo &ip);

  void read(istream &);
  void write(ostream &);

  friend inline istream &operator >>(istream &, C_ShapeInfo &);
  friend inline ostream &operator <<(ostream &, C_ShapeInfo &);
};

inline istream &operator >>(istream &is, C_ShapeInfo &cp)
{
  cp.read(is);
  return(is);
}

inline ostream &operator <<(ostream &os, C_ShapeInfo &cp)
{
  cp.write(os);
  return(os);
}

extern C_ShapeInfo *shape_infos;

class C_3DObjectInfo
{
public:
  C_ShapeInfo *shapes;
  int nshapes;
  shape_params *dflt_params;
  bounding_cube bcube;
  char *path;
  char id[16];

  C_3DObjectInfo()
    :shapes(NULL),
    nshapes(0),
    dflt_params(NULL),
    path(NULL)
    {}

  ~C_3DObjectInfo()
    {
      if (shapes)
	delete [] shapes;
      if (dflt_params)
	delete [] dflt_params;
      if (path)
	delete path;
    }

  void readFile(char *path);
  void writeFile(char *path);
  void read(istream &);
  void write(ostream &);
  friend inline istream &operator >>(istream &is, C_3DObjectInfo &co);
  friend inline ostream &operator <<(ostream &os, C_3DObjectInfo &co);
};

inline istream &operator >>(istream &is, C_3DObjectInfo &co)
{
  co.read(is);
  return(is);
}

inline ostream &operator <<(ostream &os, C_3DObjectInfo &co)
{
  co.write(os);
  return(os);
}

class C_Poly
{
public:
  int color,base_color;
  int tmap;
  unsigned long flags;
  int color_range;
  int visible;
  REAL_TYPE scale;
  C_PolyInfo *p_info;
  R_3DPoint *wpoints;
  R_3DPoint *zpoints;
  Vector	surface_normal;
  REAL_TYPE plane_constant;
  int shadow_flag;
  int tag;
  static int txtrflag;

  C_Poly();
  virtual ~C_Poly();
  virtual int create(C_PolyInfo *);
  void compute_plane_constant(R_3DPoint &loc,
			      R_3DPoint *xpoints = NULL);
  void set_world_points(R_3DPoint &loc);
  void draw(Port_3D &theport, int = 1);
  void render_shadow(Port_3D &theport, int z_clip,
		     REAL_TYPE z_value,
		     Vector &light_source,
		     int shadow_color);
  void set_shadow(REAL_TYPE z_value, Vector &light_source);
  void draw_shadow(Port_3D &theport, int z_clip, int shadow_color);
  virtual void set_poly_icolor(Vector &);
  friend istream &operator >>(istream &, C_Poly &);
  void set_params(poly_params *prms);
  void set_params(const C_Poly &);
  virtual void set_scale(REAL_TYPE x)
    { scale = x; }
  virtual void getBounds(bounding_cube &);
  virtual void getWorldPoint(R_3DPoint &, int, R_3DPoint &);
  int getScreenPoints(Port_3D &, R_2DPoint *spoints);
  int getScreenPoints(Port_3D &, TR_2DPoint *spoints);
  void set_flag(unsigned long flag, int set);
};

class C_Shape
{
public:
  int npolys;
  int visible;
  unsigned long flags;
  REAL_TYPE maxlen;
  C_Poly *polys;
  C_ShapeInfo *info_ptr;
  C_Shape();
  virtual ~C_Shape();
  R_3DPoint location;
  int memopt;

  virtual int create(C_ShapeInfo *info_ptr);
  virtual void set_poly_color(int n, int color);
  virtual void set_world_location(R_3DPoint &);
  virtual void set_visible(Port_3D &);
  virtual void draw_shadow(Port_3D &);
  virtual void set_shadow(REAL_TYPE , Vector &);
  virtual void draw(Port_3D &, R_3DPoint &);
  void set_poly_icolor(Vector &);
  friend istream & operator >>(istream &, C_Shape &);
  virtual void set_params(shape_params *);
  virtual void set_scale(REAL_TYPE);
  virtual void getBounds(bounding_cube &);
  REAL_TYPE shadow_z;
  Vector light_source;
  C_Poly * find_poly_by_tag(int tag);
  virtual void set_poly_flag_by_tag(unsigned long flag, int set, int tag);
};

#endif
