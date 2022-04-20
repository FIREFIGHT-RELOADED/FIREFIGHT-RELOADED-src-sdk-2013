//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:		Katana - HATATATATATATATATATATATATATATATATATATATATATATATATATATATA
//
// $NoKeywords: $
//=============================================================================//

#include "cbase.h"
#include "basehlcombatweapon.h"
#include "player.h"
#include "gamerules.h"
#include "ammodef.h"
#include "mathlib/mathlib.h"
#include "in_buttons.h"
#include "soundent.h"
#include "basebludgeonweapon.h"
#include "vstdlib/random.h"
#include "npcevent.h"
#include "ai_basenpc.h"
#include "weapon_katana.h"
#include "rumble_shared.h"
#include "gamestats.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

#define BLUDGEON_HULL_DIM		16

static const Vector g_bludgeonMins(-BLUDGEON_HULL_DIM, -BLUDGEON_HULL_DIM, -BLUDGEON_HULL_DIM);
static const Vector g_bludgeonMaxs(BLUDGEON_HULL_DIM, BLUDGEON_HULL_DIM, BLUDGEON_HULL_DIM);

//-----------------------------------------------------------------------------
// CWeaponKatana
//-----------------------------------------------------------------------------

IMPLEMENT_SERVERCLASS_ST(CWeaponKatana, DT_WeaponKatana)
END_SEND_TABLE()

#ifndef HL2MP
LINK_ENTITY_TO_CLASS( weapon_katana, CWeaponKatana );
PRECACHE_WEAPON_REGISTER( weapon_katana );
#endif

BEGIN_DATADESC(CWeaponKatana)
DEFINE_FIELD(m_iKillMultiplier, FIELD_INTEGER),
DEFINE_FIELD(m_iKills, FIELD_INTEGER),
DEFINE_FIELD(m_flLastKill, FIELD_TIME),
DEFINE_FIELD(m_bKillMultiplier, FIELD_BOOLEAN),
END_DATADESC()

acttable_t CWeaponKatana::m_acttable[] = 
{
	{ ACT_MELEE_ATTACK1,	ACT_MELEE_ATTACK_SWING, true },
	{ ACT_IDLE,				ACT_IDLE_ANGRY_MELEE,	false },
	{ ACT_IDLE_ANGRY,		ACT_IDLE_ANGRY_MELEE,	false },
	{ ACT_RANGE_ATTACK1,	ACT_RANGE_ATTACK_SLAM,	true },
	{ ACT_HL2MP_IDLE,		ACT_HL2MP_IDLE_MELEE,	false },
	{ ACT_HL2MP_RUN,		ACT_HL2MP_RUN_MELEE,	false },
	{ ACT_HL2MP_IDLE_CROUCH,	ACT_HL2MP_IDLE_CROUCH_MELEE,	false },
	{ ACT_HL2MP_WALK_CROUCH,	ACT_HL2MP_WALK_CROUCH_MELEE,	false },
	{ ACT_HL2MP_GESTURE_RANGE_ATTACK,	ACT_HL2MP_GESTURE_RANGE_ATTACK_MELEE,	false },
	{ ACT_HL2MP_GESTURE_RELOAD,			ACT_HL2MP_GESTURE_RELOAD_MELEE,			false },
	{ ACT_HL2MP_JUMP,		ACT_HL2MP_JUMP_MELEE,	false },
};

IMPLEMENT_ACTTABLE(CWeaponKatana);

//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------
CWeaponKatana::CWeaponKatana( void )
{
	m_iKillMultiplier = 1;
	m_iKills = 0;
	m_flLastKill = 0;
	m_bKillMultiplier = true;
}

//-----------------------------------------------------------------------------
// Purpose: Add in a view kick for this weapon
//-----------------------------------------------------------------------------
void CWeaponKatana::AddViewKick( void )
{
	CBasePlayer *pPlayer  = ToBasePlayer( GetOwner() );
	
	if ( pPlayer == NULL )
		return;

	QAngle punchAng;

	punchAng.x = random->RandomFloat( 1.0f, 2.0f );
	punchAng.y = random->RandomFloat( -2.0f, -1.0f );
	punchAng.z = 0.0f;
	
	pPlayer->ViewPunch( punchAng ); 
}


//-----------------------------------------------------------------------------
// Purpose: Primary fire button attack
//-----------------------------------------------------------------------------
void CWeaponKatana::PrimaryAttack(void)
{
	// Only the player fires this way so we can cast
	CBasePlayer *pPlayer = ToBasePlayer(GetOwner());

	if (!pPlayer)
	{
		return;
	}

	trace_t traceHit;

	Vector swingStart = pPlayer->Weapon_ShootPosition();
	Vector forward;

	forward = pPlayer->GetAutoaimVector(AUTOAIM_SCALE_DEFAULT, GetRange());

	Vector swingEnd = swingStart + forward * GetRange();
	UTIL_TraceLine(swingStart, swingEnd, MASK_SHOT_HULL, pPlayer, COLLISION_GROUP_NONE, &traceHit);

	if (traceHit.fraction == 1.0)
	{
		float bludgeonHullRadius = 1.732f * BLUDGEON_HULL_DIM;  // hull is +/- 16, so use cuberoot of 2 to determine how big the hull is from center to the corner point

		// Back off by hull "radius"
		swingEnd -= forward * bludgeonHullRadius;

		UTIL_TraceHull(swingStart, swingEnd, g_bludgeonMins, g_bludgeonMaxs, MASK_SHOT_HULL, pPlayer, COLLISION_GROUP_NONE, &traceHit);
		if (traceHit.fraction < 1.0 && traceHit.m_pEnt)
		{
			Vector vecToTarget = traceHit.m_pEnt->GetAbsOrigin() - swingStart;
			VectorNormalize(vecToTarget);

			float dot = vecToTarget.Dot(forward);

			// YWB:  Make sure they are sort of facing the guy at least...
			if (dot < 0.70721f)
			{
				// Force amiss
				traceHit.fraction = 1.0f;
			}
		}
	}

	m_iPrimaryAttacks++;

	WeaponSound(SINGLE);
	SendWeaponAnim(ACT_VM_HITCENTER);

	pPlayer->SetAnimation(PLAYER_ATTACK1);

	pPlayer->RumbleEffect(RUMBLE_CROWBAR_SWING, 0, RUMBLE_FLAG_RESTART);

	// -------------------------
	//	Miss
	// -------------------------
	if (traceHit.fraction == 1.0f)
	{
		// We want to test the first swing again
		Vector testEnd = swingStart + forward * GetRange();

		// See if we happened to hit water
		ImpactWater(swingStart, testEnd);
	}
	else
	{
		if (traceHit.DidHitWorld())
		{
			WeaponSound(MELEE_HIT_WORLD);
		}
		else
		{
			WeaponSound(MELEE_HIT);
		}
		AddViewKick();
	}

	//Setup our next attack times
	m_flNextPrimaryAttack = gpGlobals->curtime + GetFireRate();
	m_flNextSecondaryAttack = gpGlobals->curtime + GetFireRate();

	Vector vecSrc = pPlayer->Weapon_ShootPosition();
	Vector vecAiming = pPlayer->GetAutoaimVector(AUTOAIM_SCALE_DEFAULT);

	pPlayer->FireBullets(1, vecSrc, vecAiming, vec3_origin, GetRange(), m_iPrimaryAmmoType, 0);

	CSoundEnt::InsertSound(SOUND_COMBAT, GetAbsOrigin(), 300, 0.2, GetOwner());

	if (traceHit.m_pEnt)
	{
		if (traceHit.m_pEnt && !traceHit.m_pEnt->IsAlive() && g_pGameRules->isInBullettime && m_bKillMultiplier)
		{
			m_iKills += 1;
			if (m_iKills < KATANA_MAXGIVEHEALTH)
			{
				pPlayer->TakeHealth((traceHit.m_pEnt->GetMaxHealth() * 0.5) * m_iKillMultiplier, DMG_GENERIC);
				if (m_iKillMultiplier < KATANA_MAXKILLHEALTHBONUS)
				{
					m_iKillMultiplier += 1;
					m_flLastKill = gpGlobals->curtime + KATANA_POSTKILLHEALTHBONUSDELAY;
					const char* hintMultiplier = CFmtStr("%ix!", m_iKillMultiplier);
					pPlayer->ShowLevelMessage(hintMultiplier);
				}
			}
		}
	}
}

bool CWeaponKatana::Holster(CBaseCombatWeapon* pSwitchingTo)
{
	bool isGrappling = false;

	if (pSwitchingTo)
	{
		if (FClassnameIs(pSwitchingTo, "weapon_grapple"))
		{
			isGrappling = true;
		}
		else
		{
			isGrappling = false;
		}
	}

	if (!isGrappling)
	{
		if (!g_pGameRules->isInBullettime)
		{
			if (m_flLastKill > gpGlobals->curtime)
			{
				m_bKillMultiplier = false;
			}
		}

		if (m_iKillMultiplier > 0)
		{
			m_iKillMultiplier = 0;
			m_iKills = 0;
		}
	}

	return BaseClass::Holster(pSwitchingTo);
}

void CWeaponKatana::ItemPostFrame(void)
{
	if (!g_pGameRules->isInBullettime)
	{
		if (m_iKillMultiplier > 0)
		{
			m_iKillMultiplier = 0;
			m_iKills = 0;
		}

		if (m_flLastKill > gpGlobals->curtime)
		{
			m_bKillMultiplier = false;
		}
	}
	else
	{
		if (m_iKills >= KATANA_MAXGIVEHEALTH)
		{
			m_bKillMultiplier = false;
		}
		else
		{
			if (m_flLastKill < gpGlobals->curtime && m_iKillMultiplier > 0)
			{
				m_iKillMultiplier = 0;
			}
		}
	}

	if (m_flLastKill < gpGlobals->curtime && !m_bKillMultiplier)
	{
		if (m_iKillMultiplier > 0)
		{
			m_iKillMultiplier = 0;
			m_iKills = 0;
		}

		m_bKillMultiplier = true;
	}

	BaseClass::ItemPostFrame();
}
