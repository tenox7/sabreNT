#ifndef __zvedit_h
#define __zvedit_h

/* set color, font includes */
#include "colorspc.h"
#include "font8x8.h"

/* basic graphics includes */
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

/* input includes */
#include "pc_keys.h"
#include "kbdhit.h"
#include "key_map.h"
#include "input.h"

#include "convert.h"

extern char *modes[];
extern char *sel_axis[];

extern int s_axis;
extern int axis_on;
extern float axis_len;
extern float scale_f;
extern Port_3D vport;
extern Port_3D ref_port;
extern Vector world_light_source;
extern float world_scale;
extern int xx;
extern int yy;
extern REAL_TYPE view_distance;
extern int mode;
extern int current_shape;
extern Z_Node_Manager *zm;
extern char *out_file;
extern char *outfile;
extern color_spec cs;
extern int current_poly;
extern char *lib_path;
extern int routine_key;
extern REAL_TYPE shadow_level;
extern REAL_TYPE vphi;
extern REAL_TYPE vtheta ;
extern int current_txtr;
extern int current_wrap ;

extern char *shape_file;
extern char *palette_file;
extern char *txtr_file;

extern int gpause;
extern int dump_screen;
extern float raw_time;

extern int (* messagehook)(void);

void doEditor(char *fname);
void save();
#endif
