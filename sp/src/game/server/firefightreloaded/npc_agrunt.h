/***
*
*	Copyright (c) 1996-2001, Valve LLC. All rights reserved.
*	
*	This product contains software technology licensed from Id 
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc. 
*	All Rights Reserved.
*
*   Use, distribution, and modification of this source code and/or resulting
*   object code is restricted to non-commercial enhancements to products from
*   Valve LLC.  All other use, distribution, or modification is prohibited
*   without written permission from Valve LLC.
*
****/

#ifndef NPC_AGRUNT_H
#define NPC_AGRUNT_H
//=========================================================
// AGrunt Defines
//=========================================================
// -----------------------------------------------
//	> Squad slots
// -----------------------------------------------
enum AGruntSquadSlot_T
{
	AGRUNT_SQUAD_SLOT_HORNET1 = LAST_SHARED_SQUADSLOT,
	AGRUNT_SQUAD_SLOT_HORNET2,
	AGRUNT_SQUAD_SLOT_CHASE,
};

//=========================================================
// monster-specific schedule types
//=========================================================
enum
{
	SCHED_AGRUNT_FAIL = LAST_SHARED_SCHEDULE,
	SCHED_AGRUNT_COMBAT_FAIL,
	SCHED_AGRUNT_STANDOFF,
	SCHED_AGRUNT_SUPPRESS_HORNET,
	SCHED_AGRUNT_RANGE_ATTACK,
	SCHED_AGRUNT_HIDDEN_RANGE_ATTACK,
	SCHED_AGRUNT_TAKE_COVER_FROM_ENEMY,
	SCHED_AGRUNT_VICTORY_DANCE,
	SCHED_AGRUNT_THREAT_DISPLAY,
};

//=========================================================
// monster-specific tasks
//=========================================================
enum
{
	TASK_AGRUNT_SETUP_HIDE_ATTACK = LAST_SHARED_TASK,
	TASK_AGRUNT_GET_PATH_TO_ENEMY_CORPSE,
};

//=========================================================
// Monster's Anim Events Go Here
//=========================================================
#define		AGRUNT_AE_HORNET1	( 1 )
#define		AGRUNT_AE_HORNET2	( 2 )
#define		AGRUNT_AE_HORNET3	( 3 )
#define		AGRUNT_AE_HORNET4	( 4 )
#define		AGRUNT_AE_HORNET5	( 5 )
// some events are set up in the QC file that aren't recognized by the code yet.
#define		AGRUNT_AE_PUNCH		( 6 )
#define		AGRUNT_AE_BITE		( 7 )

#define		AGRUNT_AE_LEFT_FOOT	 ( 10 )
#define		AGRUNT_AE_RIGHT_FOOT ( 11 )

#define		AGRUNT_AE_LEFT_PUNCH ( 12 )
#define		AGRUNT_AE_RIGHT_PUNCH ( 13 )

// dist/dot vals
#define		AGRUNT_MELEE_DIST	100
#define		AGRUNT_MELEE_DOT	0.6f

#define		AGRUNT_RANGED_DIST	1024
#define		AGRUNT_RANGED_DOT	0.5f

//=========================================================
// Hornet Defines
//=========================================================
#define HORNET_TYPE_RED			0
#define HORNET_TYPE_ORANGE		1
#define HORNET_RED_SPEED		600
#define HORNET_ORANGE_SPEED		800

extern int iHornetPuff;

//=========================================================
// Hornet - this is the projectile that the Alien Grunt fires.
//=========================================================
class CHornet : public CAI_BaseNPC
{
	DECLARE_CLASS(CHornet, CAI_BaseNPC);
public:
	DECLARE_DATADESC();

	void Spawn( void );
	void Precache( void );
	Class_T	Classify ( void );
	Disposition_t IRelationType(CBaseEntity *pTarget);

	void IgniteTrail(void);
	void StartDart(void);
	void DieTouch(CBaseEntity *pOther);
	void TrackTouch(CBaseEntity *pOther);
	void TrackTarget(void);
	void StartTrack(void);
	void Event_Killed(const CTakeDamageInfo &info);
	void DartTouch(CBaseEntity* pOther);
	bool CanBecomeRagdoll(void) { return false; }
	
	virtual unsigned int PhysicsSolidMaskForEntity() const;
	virtual bool ShouldGib( const CTakeDamageInfo &info ) { return false; }

public:
	float			m_flStopAttack;
	int				m_iHornetType;
	float			m_flFlySpeed;
	int				m_flDamage;
	Vector			m_vecEnemyLKP;
};

#endif //NPC_AGRUNT_H