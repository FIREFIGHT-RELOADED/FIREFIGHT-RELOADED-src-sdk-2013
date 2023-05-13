//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: A hideous, putrescent, pus-filled undead carcass atop which a vile
//			nest of filthy poisonous headcrabs lurks.
//
//			Anyway, this guy has two range attacks: at short range, headcrabs
//			will leap from the nest to attack. At long range he will wrench a
//			headcrab from his back to throw it at his enemy.
//
//=============================================================================//

#ifndef NPC_POISONZOMBIE_H
#define NPC_POISONZOMBIE_H

#include "cbase.h"
#include "npc_BaseZombie.h"

#define BREATH_VOL_MAX  0.6

//
// Controls how soon he throws the first headcrab after seeing his enemy (also when the first headcrab leaps off)
//
#define ZOMBIE_THROW_FIRST_MIN_DELAY	1	// min seconds before first crab throw
#define ZOMBIE_THROW_FIRST_MAX_DELAY	2	// max seconds before first crab throw

//
// Controls how often he throws headcrabs (also how often headcrabs leap off)
//
#define ZOMBIE_THROW_MIN_DELAY	4			// min seconds between crab throws
#define ZOMBIE_THROW_MAX_DELAY	10			// max seconds between crab throws

//
// Ranges for throwing headcrabs.
//
#define ZOMBIE_THROW_RANGE_MIN	250
#define ZOMBIE_THROW_RANGE_MAX	800
#define ZOMBIE_THROW_CONE		0.6

//
// Ranges for headcrabs leaping off.
//
#define ZOMBIE_HC_LEAP_RANGE_MIN	12
#define ZOMBIE_HC_LEAP_RANGE_MAX	256
#define ZOMBIE_HC_LEAP_CONE		0.6


#define ZOMBIE_BODYGROUP_NEST_BASE		2	// First nest crab, +2 more
#define ZOMBIE_BODYGROUP_THROW			5	// The crab in our hand for throwing

#define ZOMBIE_ENEMY_BREATHE_DIST		300	// How close we must be to our enemy before we start breathing hard.


//
// Custom schedules.
//
enum
{
	SCHED_ZOMBIE_POISON_RANGE_ATTACK2 = LAST_BASE_ZOMBIE_SCHEDULE,
	SCHED_ZOMBIE_POISON_RANGE_ATTACK1,
};


//-----------------------------------------------------------------------------
// The maximum number of headcrabs we can have riding on our back.
// NOTE: If you change this value you must also change the lookup table in Spawn!
//-----------------------------------------------------------------------------
#define MAX_CRABS	3	

//-----------------------------------------------------------------------------
// Skill settings.
//-----------------------------------------------------------------------------

class CNPC_PoisonZombie : public CAI_BlendingHost<CNPC_BaseZombie>
{
	DECLARE_CLASS( CNPC_PoisonZombie, CAI_BlendingHost<CNPC_BaseZombie> );

public:

	//
	// CBaseZombie implemenation.
	//
	virtual Vector HeadTarget( const Vector &posSrc );
	bool ShouldBecomeTorso( const CTakeDamageInfo &info, float flDamageThreshold );
	virtual bool IsChopped( const CTakeDamageInfo &info )	{ return false; }

	//
	// CAI_BaseNPC implementation.
	//
	virtual float MaxYawSpeed( void );

	virtual int RangeAttack1Conditions( float flDot, float flDist );
	virtual int RangeAttack2Conditions( float flDot, float flDist );

	virtual float GetClawAttackRange() const { return 70; }

	virtual void PrescheduleThink( void );
	virtual void BuildScheduleTestBits( void );
	virtual int SelectSchedule( void );
	virtual int SelectFailSchedule( int nFailedSchedule, int nFailedTask, AI_TaskFailureCode_t eTaskFailCode );
	virtual int TranslateSchedule( int scheduleType );

	virtual bool ShouldPlayIdleSound( void );

	virtual void SetGrenadeCount( int count );

	//
	// CBaseAnimating implementation.
	//
	virtual void HandleAnimEvent( animevent_t *pEvent );

	//
	// CBaseEntity implementation.
	//
	virtual void Spawn( void );
	virtual void Precache( void );
	virtual void SetZombieModel( void );

	virtual Class_T Classify( void );
	virtual void Event_Killed( const CTakeDamageInfo &info );
	virtual int OnTakeDamage_Alive( const CTakeDamageInfo &inputInfo );
	virtual float GetHitgroupDamageMultiplier(int iHitGroup, const CTakeDamageInfo &info);

	DECLARE_DATADESC();
	DEFINE_CUSTOM_AI;

	void PainSound( const CTakeDamageInfo &info );
	void AlertSound( void );
	void IdleSound( void );
	void AttackSound( void );
	void AttackHitSound( void );
	void AttackMissSound( void );
	void FootstepSound( bool fRightFoot );
	void FootscuffSound( bool fRightFoot ) {};

	virtual void StopLoopingSounds( void );

	int m_nCrabCount;				// How many headcrabs we have on our back.
	bool m_bCrabCountOverride;

protected:

	virtual void MoanSound( envelopePoint_t *pEnvelope, int iEnvelopeSize );
	virtual bool MustCloseToAttack( void );

	virtual const char *GetMoanSound( int nSoundIndex );
	virtual const char *GetLegsModel( void );
	virtual const char *GetTorsoModel( void );
	virtual const char *GetHeadcrabClassname( void );
	virtual const char *GetHeadcrabModel( void );

private:

	void BreatheOffShort( void );

	void EnableCrab( int nCrab, bool bEnable );
	int RandomThrowCrab( void );
	void EvacuateNest( bool bExplosion, float flDamage, CBaseEntity *pAttacker );

	CSoundPatch *m_pFastBreathSound;
	CSoundPatch *m_pSlowBreathSound;

	bool m_bCrabs[MAX_CRABS];		// Which crabs in particular are on our back.
	float m_flNextCrabThrowTime;	// The next time we are allowed to throw a headcrab.

	float m_flNextPainSoundTime;

	bool m_bNearEnemy;

	// NOT serialized:
	int m_nThrowCrab;				// The crab we are about to throw.
};

#endif