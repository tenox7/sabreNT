/*
    Sabre Fighter Plane Simulator 
    Copyright (c) 1997/1998 Dan Hammer
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
 *         Sabre Fighter Plane Simulator         *
 * File   : aiflite.h                            *
 * Date   : April, 1998                          *
 * Author : Dan Hammer                           *
 *************************************************/
#ifndef __aiflite_h
#define __aiflite_h

enum  { aiF_TARGETMODE_NOACTION, aiF_TARGETMODE_EVADE, aiF_TARGETMODE_ENGAGE };

#ifdef __cplusplus

#include "aibase.h"
#include "aipilot.h"
#include "aiwaypnt.h"

#define aiMAX_FLITES              256
#define aiFLITE_DEFAULT_OWNERSHIP   1

class aiFlite : public aiBase
{
public:

   enum {   aiF_NAVIGATE, aiF_INTERCEPT, aiF_ENGAGE, aiF_ENGAGE_PLAYER,
            aiF_STRIKE, aiF_MANEUVER, aiF_TARGETS, aiF_TAKEOFF, aiF_LAND
   };
   
   aiFlite();
   aiFlite(int max, int owns = 1, const char *id = NULL);
   
   virtual ~aiFlite()
   {}

   int IsA() const
   {
      return (aiAI_FLITE_T);
   }

   virtual void Init();
   virtual void Update(double time_frame);
   virtual void Destroy();

   int Add(aiPilot *);
   void AddPlayerPilot(aiPilot *);
   aiPilot *GetPilot(int idx = 0);
   void SetWaypoints(const aiWaypoint *wps, int nn);
   void SetWaypoints(const aiwaypoint_info *winfo, int nn);
   aiWaypoint *GetWaypoint(int idx);
   aiWaypoint *GetNextWaypoint();
   aiWaypoint *GetPreviouaiWaypoint();
   void InitFormation(aiWaypoint *wp);
   void DoCapWaypoint();
   void DoInterceptWaypoint();
   void DoEngagePlayerWaypoint();
   void DoStrikeWaypoint();
   void DoNavigateWaypoint();
   void DoTargetaiWaypoint();

   void DoEngageUpdate();
   void DoInterceptUpdate();
   void DoEngagePlayerUpdate();
   void DoStrikeUpdate();
   void DoTargetsUpdate();
   void DoTakeOffUpdate();
   void DoLandUpdate();

   int GetCount()
   {
      return (n);
   }
   int IsPlayerFlite()
   {
      return (isPlayerFlite);
   }
   int GetPlayerWingPos()
   {
      return (playerPilotWingPos);
   }
   int IncViewPilot();
   int GetViewIndex()
   {
      return (viewPilot);
   }

   void SetFormationWaypoint(aiWaypoint *wp, int leadPilot = 0);
   void SetManeuver(int maneuver, long flags, aiREAL d0, aiREAL d1,
                aiREAL d2);
   
   void EngageFlite(aiFlite *);
   int AssignEngageTarget(aiPilot *pil, aiFlite *targetFlite);
   void DoWaypoints();
   aiPilot *GetLeader();
   aiPilot *GetNextActivePilot(int & start);
   
   const aiPoint &GetPosition()
   {
      return (position);
   }
   void SetId(const char *id)
   {
      SetHandle(id);
   }
   const char *GetId()
   {
      return GetHandle();
   }
   int NWaypoints()
   {
      return waypoints.Count();
   }
   int GetActiveCount()
   {
      return activeCount;
   }
   int IsActive()
   {
      return (activeCount > 0);
   }
   int GetMode()
   {
      return (mode);
   }
   void SetVisualEngagementRadius(aiREAL radius)
   {
      visualEngagementRadius = radius * radius;
   }
   void KillAttackers();
   void KillEngageFlite(aiFlite *engageFlite);

   void SetTargetMode(int mode)
   {
      if (mode >= aiF_TARGETMODE_NOACTION && mode <= aiF_TARGETMODE_ENGAGE)
         targetMode = mode;
      else
         targetMode = aiF_TARGETMODE_EVADE;
   }

   aiREAL GetNewTargetDelayTime()
   {
      return (newTargetDelayTime);
   }
   void SetNewTargetDelayTime(aiREAL newTargetDelayTime)
   {
      this->newTargetDelayTime = newTargetDelayTime;
   }

   void SetDamage(int damage, int shooterIndex)
   {
      damageFlag = 1;
      damagerIdx = shooterIndex;
   }

   aiPilot *GetPlayerWingman();

   /***********************************************
    * player-issued commands                      *
    ***********************************************/
   void SetPlayerCommandMode(int on)
   {
      playerCommandMode = on;
   }
   int GetPlayerCommandMode()
   {
      return (playerCommandMode);
   }
   void EngageNearestFlite();
   void FormUp();
   void NextWaypoint();
   void PreviouaiWaypoint();
   void ProtectPlayer();
   
   void SetAsPlayerFlite(int isPlayer);
protected:
   int            curTask;                /* current task                       */
   int            mode;                   /* current mode                       */
   aiObjectArray   pilots;                 /* array of pilots                    */
   int            n;                      /* how many pilots/flight modls       */
   int            max;                    /* max pilots/flight models           */
   aiObjectArray   waypoints;              /* array of waypoints                 */
   int            wpidx;                  /* index into                         */
   aiWaypoint      *curWaypoint;           /* current waypoint                   */
   int            viewPilot;              /* pilot providing debug viewpoint    */
   int            isPlayerFlite;          /* true if player is in this flite    */
   int            playerPilotWingPos;     /* player's wing position             */
   unsigned long  engageFliteIdx;         /* idx of flite engaged with          */
   unsigned long  interceptFliteIdx;      /* idx of flite intercepting          */
   char           id[9];                  /* char id                            */
   aiPilot        *leader;                /* flite leader pilot                 */
   int            leaderIndex;            /* index of                           */
   int            activeCount;            /* how many pilots active             */
   aiPoint         position;               /* current position                   */
   aiREAL          visualEngagementRadius; /* max distance for visual engagement */
   aiREAL          newTargetDelayTime;     /* delay for assigning pilots targets */
   int            attackerCount;          /* how many attackers                 */
   aiObjectArray   freePilots;             /* working free pilot list            */
   int            targetMode;             /* mode for acting as target-practice */
   int            damageFlag;             /* true if any pilots hit by enemy fire*/
   unsigned long  damagerIdx;             /* idx of pilot causing above damage  */
   int            nextPilot;              /* next pilot to takeoff,land         */
   int            playerCommandMode;      /* if 1 & player is leader, player    */
                                          /*    commands flite                  */

   void           DistributeAttackers();
   void           HandleDelayedTargets(aiFlite *engageFlite);
   static void    IssuePlayerAttackWarning(aiFlite *playerFlite, aiPilot *attackerPilot,
                                           aiPilot *playerPilot);
public:
   /******************
    * static members
    ******************/
   static aiObjectArray aiFlites;
   static unsigned long nextIdx;
   static aiFlite *playerFlite;

   static void AddaiFlite(aiFlite *flite);
   static aiFlite *GetaiFlite(unsigned long idx);
   static aiFlite *GetaiFlite(char *id);
   static int GetFliteCount();
   static void FlushaiFlites();
   static void RemoveaiFlite(aiFlite *flite);
   static aiFlite *GetPlayerFlite();
   static aiFlite *GetFliteByIndex(int i);
   static void SetOwnership(int owns)
   {
      aiFlites.SetOwnership(owns);
   }
   static void aiFlitesUpdate(double timeFrame);
};

#endif /* __cplusplus */
#endif /* __ifilite_h */
