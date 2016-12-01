#pragma once
//========= Copyright © 2015, Dark Interval.  =================================//
//
// Purpose: Keep it clean. OR... HE... WILL!..
//
// $NoKeywords: $
//=============================================================================//

#ifndef NPC_CREMATOR_H
#define NPC_CREMATOR_H
#ifdef _WIN32
#pragma once
#endif

#include "ai_baseNPC.h"
#include "ai_hint.h"
#include "ai_link.h"
#include "ai_motor.h"
#include "ai_node.h"
#include "ai_senses.h"
#include "ai_utils.h"
#include "ai_waypoint.h"

#include "npcevent.h"
#include "datamap.h"
#include "basehlcombatweapon.h"

#include "IEffects.h"

#include "activitylist.h"
#include "engine/IEngineSound.h"

#include "props.h"
#include "gib.h"

// for determing whether or not to be passive to the player
#include "globalstate.h"

//#include "grenade_homer.h" // for test attack (disabled) // obsolete
#include "particle_parse.h" // for particle attack and for oil sprays
#include "te_particlesystem.h" // for particle attack and for oil sprays
#include "te_effect_dispatch.h" // for particle attack and for oil sprays

// for the beam attack
#include "fire.h"
#include "beam_shared.h"
#include "beam_flags.h"

// for the grenade attack
#include "basegrenade_shared.h"
#include "fire.h"
#include "Sprite.h"
#include "SpriteTrail.h"
#include "world.h"

// for finding and handling corpses to burn
#include "physics_prop_ragdoll.h"
#include "EntityFlame.h"

// tracing ammo
#include "ammodef.h" // included for ammo-related damage table

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

#define DARKINTERVAL_CREMATOR_ROUTINE 1

/*------------Feb 25th '15 code revision------------*/
class CNPC_Cremator : public CAI_BaseNPC
{
	DECLARE_CLASS(CNPC_Cremator, CAI_BaseNPC);
	DECLARE_DATADESC();
public:
	Class_T Classify(void) { return CLASS_COMBINE_CREMATOR; }
	virtual void Precache(void);
	virtual void Spawn(void);

#if 0
	virtual int GetSoundInterests(void) { return SOUND_CARCASS | SOUND_MEAT | SOUND_GARBAGE; }
	virtual void OnListened(void);
#endif
	void SelectSkin(void);
	void SearchForCorpses(void);
	virtual float MaxYawSpeed(void);

	virtual void AlertSound(void);
	virtual void IdleSound(void);
	//virtual void FoundEnemySound(void);
	virtual void DeathSound(const CTakeDamageInfo &info);

	virtual void HandleAnimEvent(animevent_t *pEvent);
	Vector LeftFootHit(float eventtime);
	Vector RightFootHit(float eventtime);
	void FootstepEffect(const Vector &origin);

	int OnTakeDamage_Alive( const CTakeDamageInfo &info );
	void TraceAttack(const CTakeDamageInfo &info, const Vector &vecDir, trace_t *ptr, CDmgAccumulator *pAccumulator);
	void Event_Killed(const CTakeDamageInfo &info);
	virtual const char *GetHeadpropModel(void);
	void DropHead(int iVelocity, Vector &vecVelocity);

	void DispatchSpray(CBaseEntity *pEntity);

	virtual void RunAI(void);
	virtual void StartTask(const Task_t *pTask);
	virtual void RunTask(const Task_t *pTask);
	virtual int RangeAttack1Conditions(float flDot, float flDist);
	NPC_STATE SelectIdealState(void);
	Activity TranslateActivity(Activity activity);
	virtual int TranslateSchedule(int scheduleType);
	virtual int SelectSchedule(void);
	virtual void OnScheduleChange(void);
	virtual void PrescheduleThink(void);

	DEFINE_CUSTOM_AI;
protected:
	bool m_bHeadshot;
	bool m_bIsPlayerEnemy;
	bool m_bIsNPCEnemy;
	bool m_bPlayAngrySound;
	int m_iAmmo;
private:
	Disposition_t IRelationType(CBaseEntity *pTarget);
	void IncinerateCorpse(CBaseEntity *pTarget);
};

#if 0 // Pre-revision code
#define	CREMATOR_BEAM_ATTACH		1
#define CREMATOR_BURN_TIME			20 // burning time for an ignited npc

#define SF_CREMATOR_USES_GRENADES		1<<2
#define SF_CREMATOR_FUEL_SPILLING		1<<3 
#define SF_CREMATOR_PATROL_BEHAVIOR		1<<4

#define CREMATOR_SKIN_ALERT				0 // yellow eyes // обычные желтые глаза
#define CREMATOR_SKIN_CALM				1 // blue eyes // голубые глаза
#define CREMATOR_SKIN_ANGRY				2 // red eyes // красные, Ђзлыеї глаза
#define CREMATOR_SKIN_DEAD				3 // black eyes // отключенные глаза Ч†используетс€ дл€ мертвого крематора.

extern ConVar							di_dynamic_lighting_npconfire; // declared in ai_basenpc.cpp

ConVar									sk_cremator_health("sk_cremator_health", "180");
ConVar									sk_cremator_firedamage("sk_cremator_firedamage", "0");
ConVar									sk_cremator_radiusdamage("sk_cremator_radiusdamage", "0");

//class CSprite;

enum
{
	TASK_CREMATOR_ALERT = LAST_SHARED_TASK,
	//	TASK_CREMATOR_IDLE,
	TASK_CREMATOR_RANGE_ATTACK1,
	TASK_CREMATOR_RELOAD,
	TASK_CREMATOR_SET_BALANCE,
	TASK_CREMATOR_INVESTIGATE_INTEREST,
	TASK_CREMATOR_KEEP_IT_CLEAN,
};
enum
{
	SCHED_CREMATOR_CHASE = LAST_SHARED_SCHEDULE,
	SCHED_CREMATOR_RANGE_ATTACK1,
	//	SCHED_CREMATOR_IDLE,
	SCHED_CREMATOR_PATROL,
	SCHED_CREMATOR_INVESTIGATE_INTEREST,
};
enum
{
	COND_CREMATOR_HEAR_INTEREST = LAST_SHARED_CONDITION + 1,
	COND_CREMATOR_HAS_THROWN_GRENADE,
	COND_CREMATOR_OUT_OF_AMMO,
	COND_NEW_ENEMY_HIGHER_PRIORITY,
};
enum CrematorDistance_e
{
	CREMATOR_DIST_SHORT,
	CREMATOR_DIST_MEDIUM,
	CREMATOR_DIST_LONG,
};

/*===================================================================================*/
//============================|FGD entry for the cremator|============================
/*

@NPCClass base(BaseNPC) studio("models/cremator_tess.mdl") = npc_cremator : "Cremator"
[
FireDistance(choices) : "Fire Distance" : "Short" =
[
0 : "Short"
1 : "Medium"
2 : "Long"
]
FlameColour(choices) : "Flame Colour" : "Orange" =
[
0 : "Orange"
1 : "Green"
]
spawnflags(Flags) =
[
4 : "Throws grenades" : 0
8 : "Fuel Spilling" : 1
16 : "Uses patrol behavior" : 0
]
]

*/
//====================================================================================
/*===================================================================================*/
#define CREMATOR_IMMOLATOR_RANGE				450

#define	CREMATOR_AE_IMMO_START					( 6 )

#define	CREMATOR_AE_IMMO_PARTICLE				( 7 )
#define	CREMATOR_AE_IMMO_PARTICLEOFF			( 8 )

#define CREMATOR_AE_THROWGRENADE				( 9 )

#define CREMATOR_AE_RELOAD						( 15 )

#define CREMATOR_AE_FLLEFT						( 98 )
#define CREMATOR_AE_FLRIGHT						( 99 )

// specific hitgroups, numbers are defined in the .qc
#define CREMATOR_CANISTER						2
#define CREMATOR_GUN							10

/*=====================================================================*/
// Cremator class
/*=====================================================================*/

class CNPC_Cremator : public CAI_BaseNPC
{

	DECLARE_CLASS(CNPC_Cremator, CAI_BaseNPC);

public:

	Class_T			Classify(void);
	Disposition_t	IRelationType(CBaseEntity *pTarget);
	virtual const char *GetHeadpropModel(void);
	int				GetSoundInterests(void);
	virtual void	OnListened(void);
	bool			QueryHearSound(CSound *pSound);
	void			RemoveIgnoredConditions(void);

	void			Precache(void);
	void			Spawn(void);
	NPC_STATE		SelectIdealState(void);
	void			SelectSkin(void);

	int				RangeAttack1Conditions(float flDot, float flDist);
	int				RangeAttack2Conditions(float flDot, float flDist);
	//	int				TranslateSchedule( int type );	
	virtual int		SelectSchedule(void);
	float			MaxYawSpeed(void);

	bool			FValidateHintType(CAI_Hint *pHint);

	void			RunAI(void);
	void			StartTask(const Task_t *pTask);
	void			RunTask(const Task_t *pTask);
	void			HandleAnimEvent(animevent_t *pEvent);
	void			OnChangeActivity(Activity eNewActivity);
	void			OnScheduleChange(void);

	void			DispatchSpray(CBaseEntity *pEntity);
	void			FireDroplet(float min_fwd, float max_fwd, float up_value, float min_rt, float max_rt, float timer, bool bVisible);
	void			ThrowIncendiaryGrenade(void);

	void			IdleSound(void);
	void			FoundEnemySound(void);
	void			DeathSound(const CTakeDamageInfo &info);
	void			FootstepEffect(const Vector &origin);
	
	// so we won't be seen by other Cremators.
	virtual bool			CanBecomeServerRagdoll(void) { return false; }

	//	int				OnTakeDamage( const CTakeDamageInfo &info );
#if ( DARKINTERVAL_CREMATOR_BALANCING )
	void			TraceAttack(const CTakeDamageInfo &info, const Vector &vecDir, trace_t *ptr, CDmgAccumulator *pAccumulator);
#endif
	void			Event_Killed(const CTakeDamageInfo &info);
	void			DropHead(int iVelocity);

	/*	void			ShouldDropHead( const CTakeDamageInfo &HeadInfo, float flDamageThreshold );
	void			ShouldExplodeCanister( const CTakeDamageInfo &expInfo, float flDamageThreshold );*/

	DECLARE_DATADESC();
	DEFINE_CUSTOM_AI;

private:

	int				GetMaxRange();

	int				m_MuzzleAttachment;
	int				m_HeadAttachment;
	int				m_iAmmo;
	int				m_iMaxAmmo;
	int				m_iMaxHealth;

	bool			IsPlayerCriminal(void);
	bool			m_bHeadshot;
	bool			m_bCanisterShot;

	float			m_flNextRangeAttack2Time;
	float			m_flFireDropletTimer;
	float			m_flLastIdleSoundTime;
	float			m_flLastIdleFlapTime;
	float			m_flPatrolTimer;
	float			m_flLastHeardDanger;


	Vector			LeftFootHit(float eventtime);
	Vector			RightFootHit(float eventtime);

	CSound *pInterestSound; // sound of a carcass or trash that interests us
};
#endif
/*=====================================================================*/
// Cremator's incendiary grenades
/*=====================================================================*/

ConVar sk_cremator_grenades_timer("sk_cremator_grenades_timer", "0");
ConVar sk_cremator_grenades_radius("sk_cremator_grenades_radius", "0");
ConVar test_cremator_grenades_firesystems("test_cremator_grenades_firesystems", "0");

#define GRENADE_INCENDIARY_MODEL "models/weapons/w_grenade.mdl"

class CGrenadeIncendiary : public CBaseGrenade
{
	DECLARE_CLASS(CGrenadeIncendiary, CBaseGrenade);
public:
	float					m_fSpawnTime;
	float					m_fDangerRadius;

	void					Spawn(void);
	bool					CreateVPhysics(void);
	void					CreateTrail(void);
	void					Precache(void);
	void 					IncendiaryTouch(CBaseEntity *pOther);
	void					IncendiaryThink(void);
	void					Event_Killed(const CTakeDamageInfo &info);

public:
	void EXPORT				Detonate(void);

private:

	CHandle<CSprite>		m_pTrailGlow;
	CHandle<CSpriteTrail>	m_pTrailBeam;

	DECLARE_DATADESC();
};
extern short	g_sModelIndexFireball;	// (in combatweapon.cpp) holds the index for the smoke cloud

BEGIN_DATADESC(CGrenadeIncendiary)

DEFINE_FIELD(m_pTrailGlow, FIELD_EHANDLE),
DEFINE_FIELD(m_pTrailBeam, FIELD_EHANDLE),
DEFINE_FIELD(m_fSpawnTime, FIELD_TIME),
DEFINE_FIELD(m_fDangerRadius, FIELD_FLOAT),

DEFINE_ENTITYFUNC(IncendiaryTouch),
DEFINE_THINKFUNC(IncendiaryThink),

END_DATADESC()

LINK_ENTITY_TO_CLASS(grenade_incendiary, CGrenadeIncendiary);

void CGrenadeIncendiary::Precache(void)
{
	PrecacheModel(GRENADE_INCENDIARY_MODEL);
	PrecacheModel("models/cremator/grenade_glow.vmt");
	PrecacheModel("models/cremator/grenade_trail.vmt");
}
void CGrenadeIncendiary::Spawn(void)
{
	Precache();
	SetSolid(SOLID_VPHYSICS);
	SetMoveType(MOVETYPE_FLYGRAVITY, MOVECOLLIDE_FLY_CUSTOM);
	SetCollisionGroup(COLLISION_GROUP_INTERACTIVE);

	SetModel(GRENADE_INCENDIARY_MODEL);
	UTIL_SetSize(this, Vector(-3, -3, -3), Vector(3, 3, 3));

	SetUse(&CGrenadeIncendiary::DetonateUse);
	SetTouch(&CGrenadeIncendiary::IncendiaryTouch);
	SetThink(&CGrenadeIncendiary::IncendiaryThink);
	SetNextThink(gpGlobals->curtime + 0.1f);

	m_flDamage = 25.0;

	m_DmgRadius = 128.0;
	m_takedamage = DAMAGE_YES;
	m_bIsLive = false;
	m_iHealth = 1;

	SetGravity(UTIL_ScaleForGravity(400));	// use a lower gravity for grenades to make them easier to see
	SetFriction(0.8);
	SetSequence(0);

	m_fSpawnTime = gpGlobals->curtime;

	CreateVPhysics();
	CreateTrail();
}
bool CGrenadeIncendiary::CreateVPhysics()
{
	VPhysicsInitNormal(SOLID_BBOX, 0, false);
	return true;
}
void CGrenadeIncendiary::CreateTrail(void)
{
	m_pTrailGlow = CSprite::SpriteCreate("models/cremator/grenade_glow.vmt", GetLocalOrigin(), false);

	int	nAttachment = LookupAttachment("fuse");

	if (m_pTrailGlow != NULL)
	{
		m_pTrailGlow->FollowEntity(this);
		m_pTrailGlow->SetAttachment(this, nAttachment);
		m_pTrailGlow->SetTransparency(kRenderGlow, 255, 180, 0, 200, kRenderFxNone);
		m_pTrailGlow->SetScale(1.5f);
		m_pTrailGlow->SetGlowProxySize(4.0f);
	}

	// Start up the eye trail
	m_pTrailBeam = CSpriteTrail::SpriteTrailCreate("models/cremator/grenade_trail.vmt", GetLocalOrigin(), false);
	if (m_pTrailBeam != NULL)
	{
		m_pTrailBeam->FollowEntity(this);
		m_pTrailBeam->SetAttachment(this, nAttachment);
		m_pTrailBeam->SetTransparency(kRenderGlow, 255, 180, 0, 255, kRenderFxNone);
		m_pTrailBeam->SetStartWidth(16.0f);
		m_pTrailBeam->SetEndWidth(1.0f);
		m_pTrailBeam->SetLifeTime(0.5f);
	}
}
void CGrenadeIncendiary::IncendiaryThink(void)
{
	SetNextThink(gpGlobals->curtime + sk_cremator_grenades_timer.GetFloat());

	if (!m_bIsLive)
	{
		if (m_fSpawnTime + sk_cremator_grenades_timer.GetFloat() < gpGlobals->curtime)
		{
			m_bIsLive = true;
		}
	}
	if (m_bIsLive)
	{
		if (GetAbsVelocity().Length() == 0.0 ||
			GetGroundEntity() != NULL)
		{
			Detonate();
		}
	}
}
void CGrenadeIncendiary::Event_Killed(const CTakeDamageInfo &info)
{
	Detonate();
}
void CGrenadeIncendiary::IncendiaryTouch(CBaseEntity *pOther)
{
	Assert(pOther);
	if (!pOther->IsSolid())
		return;

	if (m_bIsLive)
	{
		Detonate();
	}
	else
	{
		CBaseCombatCharacter *pBCC = ToBaseCombatCharacter(pOther);
		if (pBCC && GetThrower() != pBCC)
		{
			m_bIsLive = true;
			Detonate();
		}
	}
}
void CGrenadeIncendiary::Detonate(void)
{
	if (!m_bIsLive)
	{
		return;
	}

	m_bIsLive = false;
	m_takedamage = DAMAGE_NO;
	m_pTrailGlow = NULL;
	m_pTrailBeam = NULL;

	CPASFilter filter(GetAbsOrigin());

	te->Explosion(filter, 0.0,
		&GetAbsOrigin(),
		g_sModelIndexFireball,
		2.0,
		15,
		TE_EXPLFLAG_NONE,
		m_DmgRadius,
		m_flDamage);

	Vector vecForward = GetAbsVelocity();
	VectorNormalize(vecForward);
	trace_t		tr;

	UTIL_TraceLine(GetAbsOrigin(), GetAbsOrigin() + 60 * vecForward, MASK_SHOT,
		this, COLLISION_GROUP_NONE, &tr);

	if ((tr.m_pEnt != GetWorldEntity()) || (tr.hitbox != 0))
	{
		if (tr.m_pEnt && !tr.m_pEnt->IsNPC())
		{
			UTIL_DecalTrace(&tr, "SmallScorch");
		}
	}
	else
	{
		UTIL_DecalTrace(&tr, "Scorch");
	}

	//=================================
	// This would spawn fires on grenade's detonation. 
	// Fire systems spawning has been disabled for now, because it rather often produces a CTD when a grenade detonates
	// upon contact with an NPC/player, or on a displacement surface. Has something to do with firesystem spawning coordinates...
	if (test_cremator_grenades_firesystems.GetBool() == 1)
	{
		trace_t trace;
		UTIL_TraceLine(GetAbsOrigin(), GetAbsOrigin() + Vector(0, 0, -128), MASK_SOLID_BRUSHONLY,
			this, COLLISION_GROUP_NONE, &trace);

		// Pull out of the wall a bit
		if (trace.fraction != 1.0)
		{
			SetLocalOrigin(trace.endpos + (trace.plane.normal * (m_flDamage - 24) * 0.6));
		}

		int contents = UTIL_PointContents(GetAbsOrigin());

		if ((contents & MASK_WATER))
		{
			UTIL_Remove(this);
			return;
		}

		int i;
		QAngle vecTraceAngles;
		Vector vecTraceDir;
		trace_t firetrace;

		for (i = 0; i < 6; i++)
		{
			// build a little ray
			vecTraceAngles[PITCH] = random->RandomFloat(45, 135);
			vecTraceAngles[YAW] = random->RandomFloat(0, 360);
			vecTraceAngles[ROLL] = 0.0f;

			AngleVectors(vecTraceAngles, &vecTraceDir);

			Vector vecStart, vecEnd;

			vecStart = GetAbsOrigin() + (trace.plane.normal * 128);
			vecEnd = vecStart + vecTraceDir * 512;

			UTIL_TraceLine(vecStart, vecEnd, MASK_SOLID_BRUSHONLY, this, COLLISION_GROUP_NONE, &firetrace);

			Vector	ofsDir = (firetrace.endpos - GetAbsOrigin());
			float	offset = VectorNormalize(ofsDir);

			if (offset > sk_cremator_grenades_radius.GetFloat())
				offset = sk_cremator_grenades_radius.GetFloat();

			//Get our scale based on distance
			float scale = 0.1f + (0.75f * (1.0f - (offset / 128.0f)));
			float growth = 0.1f + (0.75f * (offset / 128.0f));
			float time;

			if (g_pGameRules->IsSkillLevel(SKILL_EASY))
			{
				time = 5.0f;
			}
			else if (g_pGameRules->IsSkillLevel(SKILL_HARD))
			{
				time = 25.0f;
			}
			else
			{
				time = 15.0f;
			}

			if (firetrace.fraction != 1.0)
			{
				FireSystem_StartFire(firetrace.endpos, scale, growth, time, (SF_FIRE_START_ON | SF_FIRE_START_FULL), (CBaseEntity*) this, FIRE_NATURAL);
			}

		}
	}

	RadiusDamage(CTakeDamageInfo(this, GetThrower(), m_flDamage, DMG_BLAST), GetAbsOrigin(), m_DmgRadius, CLASS_NONE, NULL);

	UTIL_Remove(this);
}

/*=====================================================================*/
// Cremator's fire droplet class
/*=====================================================================*/
class CFireDroplet : public CBaseAnimating
{
	DECLARE_CLASS(CFireDroplet, CBaseAnimating);
public:
	void Precache(void);
	void Spawn(void);
	bool CreateVPhysics(void);
	void DropletThink(void);
	void Impact(void);

	float m_flFireTimer;
private:
	CHandle<CSpriteTrail>	m_pTrailBeam;

	DECLARE_DATADESC();
};
BEGIN_DATADESC(CFireDroplet)
DEFINE_FIELD(m_pTrailBeam, FIELD_EHANDLE),
DEFINE_FIELD(m_flFireTimer, FIELD_FLOAT),
DEFINE_THINKFUNC(DropletThink),
END_DATADESC()

LINK_ENTITY_TO_CLASS(fire_droplet, CFireDroplet);

void CFireDroplet::Precache(void)
{
	PrecacheParticleSystem("env_fire_tiny");
	PrecacheModel("models/spitball_small.mdl");
	PrecacheModel("models/cremator/grenade_trail.vmt");
	//TODO: Particles
	BaseClass::Precache();
}
bool CFireDroplet::CreateVPhysics(void)
{
	VPhysicsInitNormal(SOLID_BBOX, 0, false);
	return true;
}
void CFireDroplet::Spawn(void)
{
	//TODO: Particles
	Precache();
	SetSolid(SOLID_VPHYSICS);
	SetMoveType(MOVETYPE_FLYGRAVITY, MOVECOLLIDE_FLY_CUSTOM);
	SetCollisionGroup(COLLISION_GROUP_DEBRIS);

	SetModel("models/spitball_small.mdl");
	UTIL_SetSize(this, Vector(-.05, -.05, -.05), Vector(.05, .05, .05));

	SetThink(&CFireDroplet::DropletThink);
	SetNextThink(gpGlobals->curtime + 0.1f);

	m_takedamage = DAMAGE_NO;
	m_flFireTimer = 5.0;

	AddEffects(EF_NOSHADOW | EF_NORECEIVESHADOW | EF_NODRAW);

	SetGravity(UTIL_ScaleForGravity(400));

	CreateVPhysics();

	BaseClass::Spawn();
}
void CFireDroplet::DropletThink(void)
{
	SetNextThink(gpGlobals->curtime + 0.01f);

	if (GetAbsVelocity().Length() == 0.0)
	{
		if (GetGroundEntity() != NULL)
			Impact();
		else
			UTIL_Remove(this);
	}
}
void CFireDroplet::Impact(void)
{
	trace_t trace;
	UTIL_TraceLine(GetAbsOrigin(), GetAbsOrigin() + Vector(0, 0, -128), MASK_SOLID_BRUSHONLY,
		this, COLLISION_GROUP_NONE, &trace);

	// Pull out of the wall a bit
	if (trace.fraction != 1.0)
	{
		SetLocalOrigin(trace.endpos + (trace.plane.normal * 0.6));
	}

	int contents = UTIL_PointContents(GetAbsOrigin());

	if ((contents & MASK_WATER))
	{
		// TODO: Oily puddles on water
		UTIL_Remove(this);
		return;
	}

	int i;
	QAngle vecTraceAngles;
	Vector vecTraceDir;
	trace_t firetrace;

	for (i = 0; i < 1; i++)
	{
		// build a little ray
		vecTraceAngles[PITCH] = random->RandomFloat(45, 135);
		vecTraceAngles[YAW] = random->RandomFloat(0, 360);
		vecTraceAngles[ROLL] = 0.0f;

		AngleVectors(vecTraceAngles, &vecTraceDir);

		Vector vecStart, vecEnd;

		vecStart = GetAbsOrigin() + (trace.plane.normal * 16);
		vecEnd = vecStart + vecTraceDir * 32;

		UTIL_TraceLine(vecStart, vecEnd, MASK_SOLID_BRUSHONLY, this, COLLISION_GROUP_NONE, &firetrace);

		if (firetrace.fraction != 1.0)
		{
			FireSystem_StartFire(firetrace.endpos, 0.2, 0.20, 5.0, (SF_FIRE_SMOKELESS), (CBaseEntity*) this, FIRE_NATURAL);
			//	DispatchParticleEffect( "env_fire_tiny", GetAbsOrigin(), GetAbsAngles(), this );
			UTIL_DecalTrace(&firetrace, "SmallScorch");
		}
	}

	SetThink(NULL);
	UTIL_Remove(this);
}

#endif // NPC_CREMATOR_H