#ifndef __rpoint_h
#define __rpoint_h

struct rpoint 
{
   static  int instance_count;
   R_3DPoint pnt;
   rpoint *next;
   rpoint(const R_3DPoint &p)
   {
      pnt = p;
      next = NULL;
      instance_count++;
   }
   ~rpoint()
   {
      instance_count--;
   }
};

extern rpoint *rp_head;


#endif
