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
 * File   : aipilot.h                            *
 * Date   : April, 1998                          *
 * Author : Dan Hammer                           *
 * aiPilot class definition                      *
 *************************************************/
#ifndef __aipilot_h
#define __aipilot_h

/* enum for skills */
enum	{ 
			aiPILOT_SKILL_MANEUVER, aiPILOT_SKILL_SHOOT, aiPILOT_SKILL_ENERGY, 
			aiPILOT_SKILL_SA, aiPILOT_SKILL_BOMB
		}; 	

#ifdef __cplusplus
#include "aiobject.h"
#include "aiarray.h"
#include "aifltmdl.h"
#include "scntrl.h"
#include "aitarget.h"
#include "aiweapon.h"
#include "aimnvrst.h"
#include "aitact.h"
#include "aiplncaps.h"
#include "aipilcaps.h"
#include "aiwaypnt.h"
#include "aiattkr.h"
#include "aipid.h"
#include "aigunner.h"
#include "aibase.h"
#include "aicomm.h"

#define aiPILOT_MAX_PILOTS       256
#define aiPILOT_SKILL_SLOTS        5
#define aiPILOT_AVERAGING_N       16
#define aiPILOT_MAX_GUNNERS       20
#define aiPILOT_BROADCAST_ALL    255
#define aiPILOT_DEFAULT_OWNERSHIP  0

class aiPilot : public aiBase
{
public:
	/* enums for task */
	enum { 
           NOTHING, CAP, ENGAGE_TARGET, FORMATION_FLY, NAVIGATE, INTERCEPT_TARGET, 
           STRAFE, ROCKET, LEVEL_BOMB, DIVE_BOMB, TORPEDO_BOMB, AIRSHOW, EVADE_TARGET,
			  TAKEOFF, LAND 
         };

	aiPilot(aiFlightModel *flightModel);

	virtual ~aiPilot();

	virtual int IsA() const
	{
		return (aiAI_PILOT_T);
	}

	virtual void Init();
	virtual void Update(double timeFrame);
	virtual void Destroy();

	void UpdateCallback(int sw, int iter);
    /*
     *  Tasks
     */
   void Nothing();
   void InterceptTarget();
   void EngageTarget();
   void FlyFormation();
   void Navigate();
   void Strafe();
   void Rocket();
   void LevelBomb();
   void DiveBomb();
   void TorpedoBomb();
   void AirShow();
	void EvadeTarget();
	void TakeOff();
	void Land();

	/*
	 * Set a task
	 */
	void SetInterceptTarget(unsigned long targetIndex);
	void SetEngageTarget(unsigned long targetIndex);
	void SetFormationTarget(unsigned long targetIndex, int wingPosition = 1, int formationType = 0);
	void SetNavigatePoint(aiWaypoint *wp);
	void SetSurfaceTarget(unsigned long surfaceTargetIndex, int attackMethod, aiPoint *worldPoint = NULL);
	void SetAirShowPoint(aiWaypoint *wp);
	void SetEvadeTarget(unsigned long targetIndex);
	void SetTakeOff(aiWaypoint *wp);
	void SetLand(aiWaypoint *wp);

   int GetInterceptStatus();
	int	GetEngageStatus();
   unsigned long GetTargetIdx();
	void Disengage();

   void AddAttacker(aiPilot *pil);
   void DeleteAttacker(aiPilot *pil);
     

	virtual void GetRemoteControlInputs() = 0;

	virtual void SetFlightModel(aiFlightModel *flt_model);

	aiFlightModel *GetFlightModel()
	{
		return (flightModel);
	}

	unsigned long GetFliteId()
	{
		return (fliteId);
	}
	void SetFliteId(unsigned long fliteId)
	{
		this->fliteId = fliteId;
	}
	int GetRemoteControl()
	{
		return (remoteControl);
	}

	int IsDead()
	{
		return (dead);
	}

	virtual void SetDeath(int dead);

	int IsEjected()
	{
		return (ejected);
	}
	void SetEjected(int ejected);

	int HasGunners()
	{
		return (hasGunners);
	}
	int HasNGunners()
	{
		return (gunners.Count());
	}
	int HaaiAttackers()
	{
		return (attackList.GetCount() > 0);
	}
	/* 
	 * to allow us to generically include the player as part of
	 * flights
	 */
	int isPlayerFlag;
	virtual int IsPlayer()
	{
		return (isPlayerFlag);
	}
	void SetAsPlayer(int);

	virtual const char *GetModel();
	virtual aiREAL GetAltitudeFPS();
	virtual void GetPositionAndAttitude(aiPoint &position, 
		aiAttitude &attitude);
	virtual void SetPositionAndAttitude(const aiPoint &position,
													const aiAttitude &attitude);
	virtual void GetDirectionNormal(aiVector &);
	aiREAL GetFormationWingLen() 
	{ 
		return formationWingLen;
	}
	aiREAL GetFormationOffset()
	{
		return formationOffset;
	}
	virtual int IsActive()	
	{ 
		return !(IsDead() || IsEjected());
	}
  
	int GetTargetActive();

	void SetRemoteControl(int remoteControl);

	virtual void DoHUD()
	{}


	const char *GetCapsId()	{ return (capsId); }
	void SetCapsId(const char *);

	aiREAL GetNavigationDistance();
	aiREAL GetSurfaceTargetDistance();
	int   IsBombing()	
	{ 
		return isBombing; 
	}

	aiREAL	timerExtra; /* for convenience of iFlite class */
	int markedAsFree;
	int GetWingPosition()
	{
		return (wingPos);
	}
	void EnableGroundCollision(int enable)
	{
		groundCollisionOn = enable;
	}
	int GetGroundCollisionOn()
	{
		return (groundCollisionOn);
	}
	aiREAL GetHeightAGL()
	{
		return (flightModel->GetHeightAGL());
	}
	aiREAL GetAirSpeedFPS()
	{
		return (flightModel->GetAirSpeedFPS());
	}
	int GetClearForTakeoff()
	{
		return (clearForTakeoff);
	}
	void SetClearForTakeoff(int clear);
	int GetClearForLanding()
	{
		return (clearForLanding);
	}
	void SetClearForLanding(int clear);

	void SetGunnersTarget(unsigned long targetIdx, aiREAL threatValue = ITARGET_MIN_THREAT_VALUE);
	void SetGunnersTarget(aiAttacker *attkr);
	void SetGunnerTarget(int whichGunner, unsigned long targetIdx, 
								aiREAL threatValue = ITARGET_MIN_THREAT_VALUE);
	void ClearGunneraiTarget();

	void BodyVector2WorldVector(const aiVector &body, aiVector &world)
	{
		if (flightModel)
			flightModel->BodyVector2WorldVector(body,world);
	}
	void WorldVector2BodyVector(const aiVector &world, aiVector &body)
	{
		if (flightModel)
			flightModel->WorldVector2BodyVector(world,body);
	}
	void BodyPoint2WorldPoint(const aiPoint &body, aiPoint &world)
	{
		if (flightModel)
			flightModel->BodyPoint2WorldPoint(body,world);
	}
	void WorldPoint2BodyPoint(const aiPoint &world, aiPoint &body)
	{
		if (flightModel)
			flightModel->WorldPoint2BodyPoint(world,body);
	}

	const aiTarget &GetEngageTarget()
	{
		return engageTarget;
	}
	const aiTarget &GetFormationTarget()
	{
		return formationTarget;
	}
	const aiTarget &GetInterceptTarget()
	{
		return interceptTarget;
	}
	const aiTarget &getEvadeTarget()
	{
		return evadeTarget;
	}

	void SetFlightModelUpdateFlag(int flag)
	{
		flightModelUpdateFlag = flag;
	}
	int GetFlightModelUpdateFlag()
	{
		return flightModelUpdateFlag;
	}
	const aiNavInfo &GetNavInfo()
	{
		return navInfo;
	}

	/* static functions */
	static aiObjectArray aiPilots;
	static unsigned long	nextIdx;
	static aiPilot *playerPilot; /* represents player */
	static aiPilot *cashedPilot;	/* to avoid too many array lookups */
	static void AddaiPilot(aiPilot *pilot);
	static aiPilot *GetaiPilot(unsigned long idx);
	static aiPilot *GetaiPilot(char *handle);
	static int GetPilotCount();
	static void aiPilotUpdateCallback(int, int, aiFlightModel *, void *);
	static void FlushaiPilots();
	static void RemoveaiPilot(aiPilot *pil);
	static aiPilot *GetPilotByIndex(int i);
	static void __cdecl TextMessageToPlayer(char *, ...);
	static void BodyVector2WorldVector(int idx, const aiVector &body, aiVector &world);
	static void WorldVector2BodyVector(int idx, const aiVector &world, aiVector &body);
	static void BodyPoint2WorldPoint(int idx, const aiPoint &body, aiPoint &world);
	static void WorldPoint2BodyPoint(int idx, const aiPoint &world, aiPoint &body);
	
	static void SetOwnership(int owns)
	{
		aiPilots.SetOwnership(owns);
	}

	/* friendly stuff */
	friend class aiFlite;

protected:
    /**********************************************************************
     * protected members                                                  *
     **********************************************************************/
	aiFlightModel   *flightModel;        /* pointer to flight model */
	aiTarget        engageTarget;        /* air combat target info */
	aiTarget        formationTarget;		/* formation flying target info */
	aiTarget        interceptTarget;		/* intercept target */
	aiTarget        evadeTarget;			/* evasion only target */

	int            wingPos;             /* wing position (0 = leader) */
	int            formationType;       /* formation type */
	aiREAL          formationWingLen;		/* length for formation flying spacing */
	aiREAL          engagementMinSpeed;	/* minimum speed to maintain during dogfight */
	aiREAL          formationOffset;		/* align offset for formation flying */

   aiTarget        *curTarget;          /* pointer to current target */
   aiNavInfo       navInfo;             /* navigation information */
   aiSurfaceTarget surfaceTarget;       /* surface target info       */
   aiWeaponLimits  weaponLimits;        /* current weapon information */
   aiPilotTactics  tactics;             /* tactics info */
   aiAttitude      levelAttitude;       /* pitch, yaw rel to horizon */

	/*
	 * The flight model can be controlled indirectly, via roll,pitch, yaw &
	 * throttle inputs, or by directly setting the velocities & angular rates
	 */
   sControlState     rollCtl;             /* controls absolute roll indirectly */              
   sControlState     rollCtlDirect;       /* direct control of absolute roll */
   sControlState     rollRateCtl;         /* indirect control of roll rate */
   sControlState     rollRateCtlDirect;   /* direct control of roll rate */
   aiREAL             rollMult;            /* multiplier for above */

   sControlState     pitchRateCtl;        /* controls pitch rate indirectly */
   sControlState     pitchRateCtlDirect;  /* controls pitch rate directly */
   sControlState     pitchCtl;            /* indirect pitch position control */
   sControlState     pitchCtlDirect;      /* direct pitch position control */

   sControlState     yawRateCtl;          /* controls yaw rate indirectly */
   sControlState     yawRateCtlDirect;    /* direct yaw rate control */
   sControlState     yawCtl;              /* indirect yaw control */
   sControlState     speedCtl;            /* airspeed, indirect */

   sControlState     gCtl;                /* gs, indirect */
   sControlState     velCtl;              /* direct control of airspeed */
   int               rollLock;            /* if set, keep roll rate = 0.0 */
   int               pitchLock;           /*    "         pitch "    */
   int               yawLock;             /*    "         yaw   "    */
   aiREAL             maxRollRate;          
   aiREAL             minRollRate;
    
   int               dead;                      /* living or dead */
   int               ejected;                   /* in the silk */
   unsigned long     fliteId;                   /* id of flite we're part of */              

   aiREAL           timers[8];                  /* multi-purpose timers */
   int             remoteControl;              /* if true, allow player to fly us */

    /***************************************************************
     * dogfighting/maneuvering modifiers                           *
     ***************************************************************/
   aiREAL       posGLimit;              /* max pos gs we can pull */
   aiREAL       posStructGLimit;        /* mas gs the plane can take */
   aiREAL       negGLimit;              /* mas neg gs we can pull */
   aiREAL       negStructGLimit;        /* max neg gs the plane can take */
   aiREAL       rollLimit;              /* angular roll rate limit */
   aiREAL       extendAngle;            /* default extend pitch angle (degrees */
   aiREAL       extAngle;               /* working copy of above */
   aiREAL       gunAttackDistance;      /* working distance to enter gun attack mode */
   aiREAL       gunAttackDistanceMul;   /*  times weapon max range gives us above */
   aiREAL       engageMaxSpeed;         /* working variable for air speed max */
   aiREAL       engageMaxSpeedMul;      /*  times flight model max speed gives above */
   /* PIDS */
   aiPID        pitchPid;               /* pitch pid */
   aiPID        yawPid;                 /* yaw pid */
   aiPID        rollPid;                /* roll pid */
   aiPID        gPid;                   /* g-force pid */
   aiPID        airSpeedPid;            /* air-speed pid */
   aiPID        targetClosurePid;       /* closure to target pid */

   aiREAL       collisionAvoidSecs;           /* secs look-ahead to detect collision with target */
   aiREAL       immelmanGs;                   /* g's to pull for immelman maneuver */
   aiREAL       immelmanAltMin;               /* min alt for immelman maneuver */
   aiREAL       noseOnRollLimit;              /* limits roll when getting nose on target */
   aiREAL       speedLossLimit;               /* limit airspeed loss during gun attack */
   aiREAL       splitSGs;                     /* g's to pull for split_s maneuver */
   aiREAL       splitSAltMin;                 /* min alt for split_s maneuver */
   aiREAL       zoomAngle;                    /* angle for zoom climb (degrees) */
   aiREAL       standardTurnSpeedMPH;         /* speed for standard turn */
   aiREAL       finalApproachDistance;        /* distance for final approach */
   aiREAL       finalApproachAGL;             /* AGL for final approach */

    /**********************************************************
     * additional flight data                                 *
     **********************************************************/
   aiREAL       Es;                  /* specific energy */
   aiREAL       Ps;                  /* specific excess power */
   aiREAL       deltaAirSpeed;       /* air speed loss/gained */
   aiREAL       dAirSpeed[aiPILOT_AVERAGING_N]; 
   int         dAirSpeedIdx;
   aiREAL       lastAirSpeed;
   aiREAL       deltaAltitude;       /* altitude loss/gained */
   aiREAL       dAltitude[aiPILOT_AVERAGING_N];
   int         dAltitudeIdx;
   aiREAL       lastAltitude;
   int         turnBits;            /* for jinking */
   /* ground collision avoidance */
   aiREAL       grndColSecs;         /* secs look-ahead to det. ground coll */
   aiREAL       grndColAGL;          /* min AGL for ground coll */
   aiREAL       grndColRcvrAGL;      /* min AGL to recover */
   aiREAL       grndColPullUpAngle;  /* angle to pull up */

   /* attacker list */
   int            newAttacker;               /* set if a new attacker has been added    */
   int            playerInAttackList;        /* player has been added to attacker list  */
   aiREAL          playerThreatDistance;      /* distance at which player becomes threat */
   aiREAL          playerThreatAspect;        /* aspect at which player becomes threat   */
   aiREAL          playerThreatValue;         /* additional threat value of player       */
   unsigned long  currentHiThreatAttkrIdx;   /* idx of current highest-threat attacker  */
   int            currentAttkrPrime;         /* priming flag for above                  */

   char            capsId[32];              /* id for pilot caps lookup                 */

   /*****************************************************
    * gunners                                           *
    *****************************************************/
   int            hasGunners;             /* if true, gunners aboard */
   aiObjectArray   gunners;                /* gunners                 */
   aiREAL          gunnersPitchRate;       /* how fast gunners can swing gun up, down */
   aiREAL          gunnersYawRate;         /*          "                    left, right */
   aiAttitude      gunnersJiggle;          /* jiggling factor for gunners */        
   aiREAL          gunnersBulletRadius;    /* marksmanship factor for gunners */
protected:
    /* bullet radius -- major effect on marksmanship skill                    */
   aiREAL          bulletRadiusMin;        /* minimum bullet radius            */
   aiREAL          bulletRadiusMax;        /* maximum bullet radius            */
   aiREAL          bulletRadius;           /* calc'd from above w/ skill level */          

   int            isBombing;              /* if true, droppin' iron           */
   int            groundCollisionOn;      /* detect ground collision          */
   int            clearForTakeoff;        /* takeoff flag                     */
   int            clearForLanding;        /* landing flag                     */
    /**********************************************************************
     * protected member functions                                         *
     **********************************************************************/
    /*
     *   Maneuver routines
     */
   void        DoStraightAndLevel(aiManeuverState &ms);
   void        DoStandardTurn(aiManeuverState &ms);
   void        DoBreakTurn(aiManeuverState &ms);
   void        DoHardTurn(aiManeuverState &ms);
   void        DoClimb(aiManeuverState &ms);
   void        DoDescend(aiManeuverState &ms);
   void        DoImmelman(aiManeuverState &ms);
   void        DoSplitS(aiManeuverState &ms);
   void        DoInvert(aiManeuverState &ms);
   void        DoExtend(aiManeuverState &ms);
   void        DoEngage(aiManeuverState &ms);
   void        DoAlign(aiManeuverState &ms);
   void        DoJink(aiManeuverState &ms);
   void        DoRQEvade(aiManeuverState &ms);
   void        DoRQEngage(aiManeuverState &ms);
   void        DoFQEngage(aiManeuverState &ms);
   void        DoPursuit(aiManeuverState &ms);
   void        DoGunAttack(aiManeuverState &ms);
   void        DoNavigate(aiManeuverState &ms);
   void        DoFormation(aiManeuverState &ms);
   void        DoPullUp(aiManeuverState &ms);
   void        DoLevelRoll(aiManeuverState &ms);
   void        DoPitchedRoll(aiManeuverState &ms);
   void        DoWingOver(aiManeuverState &ms);
   void        DoLevelBomb(aiManeuverState &ms);
   void        DoIntercept(aiManeuverState &ms);
   void        DoClimbingTurn(aiManeuverState &ms);
   void        DoDescendingTurn(aiManeuverState &ms);
   void        DoSnapRoll(aiManeuverState &ms);
   void        DoChandelle(aiManeuverState &ms);
   void        DoAirShow(aiManeuverState &ms);
   void        DoAileronRoll(aiManeuverState &ms);
   void        DoEvade(aiManeuverState &ms);
   void        DoZoom(aiManeuverState &ms);
   void        DoBarrelRoll(aiManeuverState &ms);
   void        DoSpiralDive(aiManeuverState &ms);
   void        DoTakeOff(aiManeuverState &ms);
   void        DoLand(aiManeuverState &ms);
   void        DoTaxi(aiManeuverState &ms);
	void			DoTurnFight(aiManeuverState &ms);

   /*
    *  Flight model control routines
    */
   void        UpdateRollCtl(sControlState &);
   void        UpdateRollCtlDirect(sControlState &);
   void        UpdateRollRateCtl(sControlState &);
   void        UpdateRollRateCtlDirect(sControlState &);

   void        UpdatePitchRateCtl(sControlState &);
   void        UpdatePitchRateCtlDirect(sControlState &);
   void        UpdatePitchCtl(sControlState &);
   void        UpdatePitchCtlDirect(sControlState &);

   void        UpdateYawRateCtl(sControlState &);
   void        UpdateYawRateCtlDirect(sControlState &);
   void        UpdateYawCtl(sControlState &);

   void        UpdateSpeedCtl(sControlState &);
   void        UpdateGCtl(sControlState &);
   void        UpdateVelCtl(sControlState &);
   void        ControlsOff();

   void        UpdateManeuver(aiManeuverState &ms);

   int         flightModelUpdateFlag;     /* if true, update flight model */
    /*
     * Decide whether or not to shoot
     */
   virtual int InShootParams();
   virtual void Shoot() = 0;
    /*
     * Drop an egg
     */
    virtual void DropBomb() = 0;

    /*
     * Get hurting percentage
     */
    virtual aiREAL GetDamagePer();

    /*
     *  Target info 
     */
   void GetAttitude(const aiPoint &position, 
                        aiVector &direction, 
                        aiAttitude &attitude)
   {
      flightModel->GetAttitudeToPoint(position,direction,attitude);
   }
   void GetTargetFlags(aiTarget &);
   virtual void GetNavigationGeometry(aiNavInfo &ni);
   virtual void GetTargetGeometry(unsigned long targetIdx, aiTargetGeometry &targetGeometry);
   virtual void CalcGunLeadPoint(aiTargetGeometry &targetGeometry);
   void GetQuickTargetGeometry(unsigned long targetIdx, aiTargetGeometry &tg);
   virtual void GetTargetInfo(unsigned long idx,   aiTargetInfo &targetInfo);
   virtual void GetTargetFlags(aiTargetFlags &, aiTargetGeometry &);
   virtual void GetAttitudeToTarget(unsigned long targetIdx,
                                    aiPoint &targetPosition,
                                    aiAttitude &attitudeToTarget,
                                    aiVector &directionToTarget,
                                    aiVector &targetHeading,
                                    aiVector &targetVelocity);
   virtual aiREAL GetTargetTurnRate(unsigned long targetIdx);
   virtual aiREAL GetTargetLoad(unsigned long targetIdx);
   virtual aiREAL GetTargetAirSpeed(unsigned long targetIdx);
   virtual int GetTargetActive(unsigned long targetIdx);
   void GetSurfaceTargetGeometry(aiSurfaceTarget &st);
   void CalcAlignPoint(aiSurfaceTarget &st);
   virtual void GetWeaponLimits(aiWeaponLimits &weaponLimits) = 0;
   void GetFormationPoint(int wingPos, int formationType, 
                   int leaderIdx, aiPoint &offset);
   void ClearTargetPilot();
   void SetTargetPilot(unsigned long targetIdx);
   int EvalThreatFromPlayer();
   void SelectEngagementFightType(unsigned long targetIdx);
   void SetNoseOn(aiREAL, aiREAL, aiREAL = Pi_2);
   void SetNoseOn(int flag, const aiPoint &, aiREAL pitch, aiREAL yaw);

   void SetNoseOnX(aiREAL, aiREAL, aiREAL = 0.5, aiREAL = 0.0, aiREAL = Pi_2);
   aiREAL Point2Roll(const aiPoint &rollPosition);
   aiREAL CalcClosureSpeed(const aiTargetGeometry &, int flg = 0);
   /* communication */
   virtual void Broadcast(int idx, int channel = commCHANNEL_AFFILIATION,
                          int priority = commPRIORITY_STANDARD,
                          void *extraInfo = NULL) = 0;

   /* get plane capabilities / control parameters */
   void CheckPlaneCaps(const char *model);
   /* get pilot capabilities */
   void CheckPilotCaps(const char *capsId);
  
   /*
    * Get yaw, pitch relative to horizon
    */
   void GetLevelAttitude(aiAttitude &, aiREAL = 0.0);

   /*
    *  Gunners
    */
   virtual void BuildGunners() =0;
   void AddGunner(aiGunner *newGunner);
   aiGunner *GetGunner(int whichGunner);
   void UpdateGunners();

   /* attacker list */
   aiAttacker *CheckAttackerList();
   aiAttacker *EvalAttackerList();
   void DistributeAttackers();
   aiREAL EvalThreat(unsigned long idx, aiPilot **pil);

   void SETROLLPID(aiREAL goal, aiREAL dVel = 0.0, aiREAL controlStep = 1.0)
   {
      rollCtl.on = 1;
      rollCtl.setPoint = goal;
      rollCtl.dVel = dVel;
      rollCtl.controlStep = controlStep;
      rollCtl.flag = 0;
   }
   void SETPITCHPID(aiREAL goal, aiREAL dVel = 0.0, aiREAL controlStep = 1.0)
   {
      pitchCtl.on = 1;
      pitchCtl.setPoint = goal;
      pitchCtl.dVel = dVel;
      pitchCtl.controlStep = controlStep;
      pitchCtl.flag = 0;
   }
   void SETYAWPID(aiREAL goal, aiREAL dVel = 0.0, aiREAL controlStep = 1.0)
   {
      yawCtl.on = 1;
      yawCtl.setPoint = goal;
      yawCtl.dVel = dVel;
      yawCtl.controlStep = controlStep;
      yawCtl.flag = 0;
   }
};
#endif
#endif
