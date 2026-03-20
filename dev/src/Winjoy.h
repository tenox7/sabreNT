#ifndef __winjoy_h
#define __winjoy_h

#include <windows.h>
#include <mmsystem.h>
class WinJoy
{
public:
  int max_x,min_x;
  int max_y,min_y;
  int max_z,min_z;
  int max_r,min_r;
  int naxis;
  int has_z, has_r;
  int has_pov;

  float cooked_x,cooked_y;
  float cooked_z,cooked_r;
  int   raw_x,raw_y;
  int   raw_z,raw_r;

  int buttons;

  int active;
  UINT joyNo;
  UINT joyId;

  WinJoy(int which);
  ~WinJoy();
  void calibrate();
  int open();
  void close();
  int update();
  int read_def_file(char *);
  float getx();
  float gety();
  int getrx();
  int getry();
  int getbuttons();

  int HasThrottle()
  {
		return has_z;
  }
  int HasRudder()
  {
		return has_r;
  }
};

#endif