//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#ifndef NPC_HGRUNT_H
#define NPC_HGRUNT_H
#ifdef _WIN32
#pragma once
#endif

#include    "ai_basenpc.h"
#include    "ai_squad.h"

#define SF_GRUNT_LEADER	( 1 << 5  )
#define SF_GRUNT_FRIENDLY ( 1 << 6  )

#define HGRUNT_GUN_SPREAD 0.08716f

//=========================================================
// monster-specific DEFINE's
//=========================================================
#define	GRUNT_CLIP_SIZE					36 // how many bullets in a clip? - NOTE: 3 round burst sound, so keep as 3 * x!
#define HGRUNT_LIMP_HEALTH				20
#define HGRUNT_DMG_HEADSHOT				( DMG_BULLET | DMG_CLUB | DMG_SNIPER )	// damage types that can kill a grunt with a single headshot.
#define HGRUNT_NUM_HEADS				2 // how many grunt heads are there? 
#define HGRUNT_MINIMUM_HEADSHOT_DAMAGE	15 // must do at least this much damage in one shot to head to score a headshot kill

#define HGRUNT_9MMAR				( 1 << 0)
#define HGRUNT_HANDGRENADE			( 1 << 1)
#define HGRUNT_GRENADELAUNCHER		( 1 << 2)
#define HGRUNT_SHOTGUN				( 1 << 3)

#define HEAD_GROUP					1
#define HEAD_GRUNT					0
#define HEAD_COMMANDER				1
#define HEAD_SHOTGUN				2
#define HEAD_M203					3
#define GUN_GROUP					2
#define GUN_MP5						0
#define GUN_SHOTGUN					1
#define GUN_NONE					2

//=========================================================
// Monster's Anim Events Go Here
//=========================================================
#define		HGRUNT_AE_RELOAD		( 2 )
#define		HGRUNT_AE_KICK			( 3 )
#define		HGRUNT_AE_BURST1		( 4 )
#define		HGRUNT_AE_BURST2		( 5 ) 
#define		HGRUNT_AE_BURST3		( 6 ) 
#define		HGRUNT_AE_GREN_TOSS		( 7 )
#define		HGRUNT_AE_GREN_LAUNCH	( 8 )
#define		HGRUNT_AE_GREN_DROP		( 9 )
#define		HGRUNT_AE_CAUGHT_ENEMY	( 10) // grunt established sight with an enemy (player only) that had previously eluded the squad.
#define		HGRUNT_AE_DROP_GUN		( 11) // grunt (probably dead) is dropping his mp5.

//=========================================================
// monster-specific schedule types
//=========================================================
enum
{
	SCHED_GRUNT_FAIL = LAST_SHARED_SCHEDULE,
	SCHED_GRUNT_COMBAT_FAIL,
	SCHED_GRUNT_VICTORY_DANCE,
	SCHED_GRUNT_ESTABLISH_LINE_OF_FIRE,
	SCHED_GRUNT_ESTABLISH_LINE_OF_FIRE_RETRY,
	SCHED_GRUNT_FOUND_ENEMY,
	SCHED_GRUNT_COMBAT_FACE,
	SCHED_GRUNT_SIGNAL_SUPPRESS,
	SCHED_GRUNT_SUPPRESS,
	SCHED_GRUNT_WAIT_IN_COVER,
	SCHED_GRUNT_TAKE_COVER,
	SCHED_GRUNT_GRENADE_COVER,
	SCHED_GRUNT_TOSS_GRENADE_COVER,
	SCHED_GRUNT_HIDE_RELOAD,
	SCHED_GRUNT_SWEEP,
	SCHED_GRUNT_RANGE_ATTACK1A,
	SCHED_GRUNT_RANGE_ATTACK1B,
	SCHED_GRUNT_RANGE_ATTACK2,
	SCHED_GRUNT_REPEL,
	SCHED_GRUNT_REPEL_ATTACK,
	SCHED_GRUNT_REPEL_LAND,
	SCHED_GRUNT_TAKE_COVER_FAILED,
	SCHED_GRUNT_RELOAD,
	SCHED_GRUNT_TAKE_COVER_FROM_ENEMY,
};

//=========================================================
// monster-specific tasks
//=========================================================
enum 
{
	TASK_GRUNT_FACE_TOSS_DIR = LAST_SHARED_TASK + 1,
	TASK_GRUNT_SPEAK_SENTENCE,
	TASK_GRUNT_CHECK_FIRE,
};


//=========================================================
// monster-specific conditions
//=========================================================
enum
{
	COND_GRUNT_NOFIRE = LAST_SHARED_CONDITION + 1,
};

// -----------------------------------------------
//	> Squad slots
// -----------------------------------------------
enum SquadSlot_T
{	
	SQUAD_SLOT_GRENADE1 = LAST_SHARED_SQUADSLOT,
	SQUAD_SLOT_GRENADE2,
	SQUAD_SLOT_ENGAGE1,
	SQUAD_SLOT_ENGAGE2,
};

//mainly for the spawner.
enum eHGruntWeapons
{
	WEAPON_HGRUNT_SMG = HGRUNT_9MMAR,
	WEAPON_HGRUNT_SMG_FRAG = (HGRUNT_9MMAR | HGRUNT_HANDGRENADE),
	WEAPON_HGRUNT_SMG_GL = (HGRUNT_9MMAR | HGRUNT_GRENADELAUNCHER),
	WEAPON_HGRUNT_SHOTGUN = HGRUNT_SHOTGUN,
	WEAPON_HGRUNT_SHOTGUN_FRAG = (HGRUNT_SHOTGUN | HGRUNT_HANDGRENADE),
};

class CHGrunt : public CAI_BaseNPC
{
	DECLARE_CLASS( CHGrunt, CAI_BaseNPC );
public:
	void	Spawn( void );
	void	BecomeFriendly(void);
    void	Precache( void );
    float	MaxYawSpeed( void );
	Class_T	Classify(void);
    int     GetSoundInterests ( void ) { return (SOUND_WORLD | SOUND_COMBAT | SOUND_PLAYER | SOUND_BULLET_IMPACT | SOUND_DANGER); }
    void	HandleAnimEvent( animevent_t *pEvent );
    bool	FCanCheckAttacks( void );
   	int     RangeAttack1Conditions ( float flDot, float flDist );
	int		MeleeAttack1Conditions ( float flDot, float flDist );
	int     RangeAttack2Conditions ( float flDot, float flDist );
    void	ClearAttackConditions( void );
	int     GetGrenadeConditions ( float flDot, float flDist );
    void	CheckAmmo ( void );
    Activity NPC_TranslateActivity( Activity NewActivity );
    void	StartTask ( const Task_t *pTask );
	void	RunTask ( const Task_t *pTask );
    void	StartNPC ( void );
    void	PainSound( const CTakeDamageInfo &info );
	void	DeathSound( const CTakeDamageInfo &info );
    void    IdleSound( void );
    Vector	Weapon_ShootPosition( void );
    int		SquadRecruit( int searchRadius, int maxMembers );
    void	Shoot (int bulletnum, Vector cone);
    void	PrescheduleThink ( void );
    void	Event_Killed( const CTakeDamageInfo &info );
    void	SpeakSentence( void );
    
    CBaseEntity *Kick( void );
	int		SelectSchedule( void );
	int		TranslateSchedule( int scheduleType );
    void    TraceAttack( const CTakeDamageInfo &info, const Vector &vecDir, trace_t *ptr, CDmgAccumulator* pAccumulator);
	int		OnTakeDamage_Alive( const CTakeDamageInfo &inputInfo );
    
	int		IRelationPriority( CBaseEntity *pTarget );

	bool	FOkToSpeak( void );
    void	JustSpoke( void );
	
    DECLARE_DATADESC();
	DEFINE_CUSTOM_AI;

	// checking the feasibility of a grenade toss is kind of costly, so we do it every couple of seconds,
	// not every server frame.
	float m_flNextGrenadeCheck;
	float m_flNextPainTime;
	float m_flLastEnemySightTime;
	float m_flTalkWaitTime;

	Vector	m_vecTossVelocity;

	int		m_iLastGrenadeCondition;
	bool	m_fStanding;
	bool	m_fIsFriendly;
	bool	m_fFirstEncounter;// only put on the handsign show in the squad's first encounter.
	int		m_iClipSize;

	int		m_voicePitch;

	int		m_iSentence;

	float	m_flCheckAttackTime;

	int		m_iAmmoType;
	
	int		m_iWeapons;
    
    static const char *pGruntSentences[];
};

#endif // MONSTERMAKER_H