#ifndef NPC_ADVISOR_H
#define NPC_ADVISOR_H

#if defined( _WIN32 )
#pragma once
#endif

#include "ai_basenpc.h"
#include "ai_schedule.h"
#include "ai_hull.h"
#include "ai_hint.h"
#include "ai_motor.h"
#include "ai_navigator.h"
#include "beam_shared.h"
#include "npcevent.h"
// #include "mathlib/noise.h"
#include	"ai_baseactor.h"
#include	"ai_moveprobe.h"
// this file contains the definitions for the message ID constants (eg ADVISOR_MSG_START_BEAM etc)
#include "npc_advisor_shared.h"

#include "ai_default.h"
#include "ai_task.h"
#include "activitylist.h"

#if NPC_ADVISOR_HAS_BEHAVIOR
// how long it will take an object to get hauled to the staging point
#define STAGING_OBJECT_FALLOFF_TIME 0.15f
#endif

//
// Spawnflags.
//

//
// Animation events.
//
#define ADVISOR_MELEE_LEFT					( 3 )
#define ADVISOR_MELEE_RIGHT					( 4 )

#if NPC_ADVISOR_HAS_BEHAVIOR
//
// Custom schedules.
//
enum
{
	SCHED_ADVISOR_COMBAT = LAST_SHARED_SCHEDULE,
	SCHED_ADVISOR_IDLE_STAND,
	SCHED_ADVISOR_TOSS_PLAYER,
	SCHED_ADVISOR_DRONIFY,
};


//
// Custom tasks.
//
enum 
{
	TASK_ADVISOR_FIND_OBJECTS = LAST_SHARED_TASK,
	TASK_ADVISOR_LEVITATE_OBJECTS,
	TASK_ADVISOR_STAGE_OBJECTS,
	TASK_ADVISOR_BARRAGE_OBJECTS,
	TASK_ADVISOR_PIN_PLAYER,
	TASK_ADVISOR_DRONIFY,
};

//
// Custom conditions.
//
enum
{
	COND_ADVISOR_PHASE_INTERRUPT = LAST_SHARED_CONDITION,
};
#endif

class CNPC_Advisor;


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
class CAdvisorLevitate : public IMotionEvent
{
	DECLARE_SIMPLE_DATADESC();

public:

	// in the absence of goal entities, we float up before throwing and down after
	inline bool OldStyle( void )
	{
		return !(m_vecGoalPos1.IsValid() && m_vecGoalPos2.IsValid());
	}

	virtual simresult_e	Simulate( IPhysicsMotionController *pController, IPhysicsObject *pObject, float deltaTime, Vector &linear, AngularImpulse &angular );

	EHANDLE m_Advisor; ///< handle to the advisor.

	Vector m_vecGoalPos1;
	Vector m_vecGoalPos2;

	float m_flFloat;
};

//-----------------------------------------------------------------------------
// The advisor class.
//-----------------------------------------------------------------------------
class CNPC_Advisor : public CAI_BaseActor
{
	DECLARE_CLASS( CNPC_Advisor, CAI_BaseActor);

#if NPC_ADVISOR_HAS_BEHAVIOR
	DECLARE_SERVERCLASS();
#endif

public:

	//
	// CBaseEntity:
	//
	virtual void Activate();
	virtual void Spawn();
	virtual void Precache();
	virtual void OnRestore();
	virtual void UpdateOnRemove();

	virtual int DrawDebugTextOverlays();

	//
	// CAI_BaseNPC:
	//
	virtual float MaxYawSpeed();

	virtual void LoadInitAttributes(void);
	
	virtual Class_T Classify();

#if NPC_ADVISOR_HAS_BEHAVIOR
	virtual int GetSoundInterests();
	virtual int SelectSchedule();
	virtual void StartTask( const Task_t *pTask );
	virtual void RunTask( const Task_t *pTask );
	virtual void OnScheduleChange( void );
	void FlingPlayer(CBaseEntity *pEnt, float force, bool upVector = false);

	void HandleAnimEvent(animevent_t *pEvent);
	int MeleeAttack1Conditions(float flDot, float flDist);

	void Event_Killed(const CTakeDamageInfo &info);

	//bullet resistance
	void				EnableBulletResistanceOutline(void);
	CTakeDamageInfo		BulletResistanceLogic(const CTakeDamageInfo& info, trace_t* ptr);
	virtual bool		PassesDamageFilter(const CTakeDamageInfo& info);
	bool				m_bBulletResistanceBroken;
	bool				m_bBulletResistanceOutlineDisabled;
	int					OnTakeDamage_Alive(const CTakeDamageInfo& info);
	void				TraceAttack(const CTakeDamageInfo& inputInfo, const Vector& vecDir, trace_t* ptr, CDmgAccumulator* pAccumulator);

	void	MovetoTarget(Vector vecTarget);
	void	Touch(CBaseEntity* pOther);
	void	Dronify(CBaseEntity* pOther);

private:
	int					m_iSpriteTexture;

public:
#endif

	virtual void PainSound( const CTakeDamageInfo &info );
	virtual void DeathSound( const CTakeDamageInfo &info );
	virtual void IdleSound();
	virtual void AlertSound();

#if NPC_ADVISOR_HAS_BEHAVIOR
	virtual bool QueryHearSound( CSound *pSound );
	virtual void GatherConditions( void );

	/// true iff I recently threw the given object (not so fast)
	bool DidThrow(const CBaseEntity *pEnt);

	void Write_BeamOn(CBaseEntity* pEnt); 	///< write a message turning a beam on
	void Write_BeamOff(CBaseEntity* pEnt);   	///< write a message turning a beam off
	void Write_AllBeamsOff(void);				///< tell client to kill all beams
#else
	inline bool DidThrow(const CBaseEntity *pEnt) { return false; }
#endif

	virtual bool IsHeavyDamage( const CTakeDamageInfo &info );
	virtual int	 OnTakeDamage( const CTakeDamageInfo &info );

	virtual const impactdamagetable_t &GetPhysicsImpactDamageTable( void );
	COutputInt   m_OnHealthIsNow;

#if NPC_ADVISOR_HAS_BEHAVIOR

	DEFINE_CUSTOM_AI;

	void InputSetThrowRate( inputdata_t &inputdata );
	void InputWrenchImmediate( inputdata_t &inputdata ); ///< immediately wrench an object into the air
	void InputSetStagingNum( inputdata_t &inputdata );
	void InputPinPlayer( inputdata_t &inputdata );
	void InputTurnBeamOn( inputdata_t &inputdata );
	void InputTurnBeamOff( inputdata_t &inputdata );
	void InputElightOn( inputdata_t &inputdata );
	void InputElightOff( inputdata_t &inputdata );
	void InputSetTurnOnBulletResistanceOutline(inputdata_t& inputdata);

	void StopPinPlayer(inputdata_t &inputdata);

	COutputEvent m_OnPickingThrowable, m_OnThrowWarn, m_OnThrow;

	enum { kMaxThrownObjectsTracked = 4 };
#endif

	DECLARE_DATADESC();

protected:

#if NPC_ADVISOR_HAS_BEHAVIOR
	Vector GetThrowFromPos( CBaseEntity *pEnt ); ///< Get the position in which we shall hold an object prior to throwing it
#endif

	bool CanLevitateEntity( CBaseEntity *pEntity, int minMass, int maxMass );
	void StartLevitatingObjects( void );


#if NPC_ADVISOR_HAS_BEHAVIOR
	// void PurgeThrownObjects(); ///< clean out the recently thrown objects array
	void AddToThrownObjects(CBaseEntity *pEnt); ///< add to the recently thrown objects array

	void HurlObjectAtPlayer( CBaseEntity *pEnt, const Vector &leadVel );
	void PullObjectToStaging( CBaseEntity *pEnt, const Vector &stagingPos );
	CBaseEntity *ThrowObjectPrepare( void );

	CBaseEntity *PickThrowable( bool bRequireInView ); ///< choose an object to throw at the player (so it can get stuffed in the handle array)

	/// push everything out of the way between an object I'm about to throw and the player.
	void PreHurlClearTheWay( CBaseEntity *pThrowable, const Vector &toPos );
#endif

	CUtlVector<EHANDLE>	m_physicsObjects;
	CUtlVector<EHANDLE>	m_droneObjects;
	IPhysicsMotionController *m_pLevitateController;
	CAdvisorLevitate m_levitateCallback;
	
	EHANDLE m_hLevitateGoal1;
	EHANDLE m_hLevitateGoal2;
	EHANDLE m_hLevitationArea;

#if NPC_ADVISOR_HAS_BEHAVIOR
	// EHANDLE m_hThrowEnt;
	CUtlVector<EHANDLE>	m_hvStagedEnts;
	CUtlVector<EHANDLE>	m_hvStagingPositions; 
	// todo: write accessor functions for m_hvStagedEnts so that it doesn't have members added and removed willy nilly throughout
	// code (will make the networking below more reliable)

	int beamonce = 1; //Makes sure it only plays once the beam

	// for the pin-the-player-to-something behavior
	EHANDLE m_hPlayerPinPos;
	float  m_playerPinFailsafeTime;
	int  m_playerPinDamage;
	bool  m_playerPinnedBecauseWallRunning;

	// keep track of up to four objects after we have thrown them, to prevent oscillation or levitation of recently thrown ammo.
	EHANDLE m_haRecentlyThrownObjects[kMaxThrownObjectsTracked]; 
	float   m_flaRecentlyThrownObjectTimes[kMaxThrownObjectsTracked];

	float  m_fllastDronifiedTime;
	float  m_fllastPinnedTime;
#endif

	string_t m_iszLevitateGoal1;
	string_t m_iszLevitateGoal2;
	string_t m_iszLevitationArea;


#if NPC_ADVISOR_HAS_BEHAVIOR
	string_t m_iszStagingEntities;
	string_t m_iszPriorityEntityGroupName;

	float m_flStagingEnd; 
	float m_flThrowPhysicsTime;
	float m_flLastThrowTime;
	float m_flLastPlayerAttackTime; ///< last time the player attacked something. 

	int   m_iStagingNum; ///< number of objects advisor stages at once
	bool  m_bWasScripting;
	bool  m_bStopMoving;

	Vector m_velocity;

	// unsigned char m_pickFailures; // the number of times we have tried to pick a throwable and failed 

	Vector m_vSavedLeadVel; ///< save off player velocity for leading a bit before actually pelting them. 
#endif
};

#endif