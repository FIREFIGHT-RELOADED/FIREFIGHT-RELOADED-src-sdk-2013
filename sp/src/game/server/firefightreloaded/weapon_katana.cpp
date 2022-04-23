//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:		Katana - HATATATATATATATATATATATATATATATATATATATATATATATATATATATA
// https://twitter.com/SadlyItsBradley/status/1516391717867507712
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
extern ConVar sk_plr_dmg_katana;
extern ConVar sk_npc_dmg_katana;
static ConVar sv_katana_healthbonus_postdelay("sv_katana_healthbonus_postdelay", "5.0", FCVAR_CHEAT);
static ConVar sv_katana_healthbonus_maxmultiplier("sv_katana_healthbonus_maxmultiplier", "5", FCVAR_CHEAT);
static ConVar sv_katana_healthbonus_maxtimestogivebonus("sv_katana_healthbonus_maxtimestogivebonus", "10", FCVAR_CHEAT);
static ConVar sv_katana_antlionguard_damageresistance("sv_katana_antlionguard_damageresistance", "0.2", FCVAR_CHEAT);

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
	m_iKillMultiplier = 0;
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
			Hit(traceHit, GetActivity(), false);
			WeaponSound(MELEE_HIT_WORLD);
		}
		else
		{
			if (traceHit.m_pEnt)
			{
				if (traceHit.m_pEnt->IsNPC() || traceHit.m_pEnt->IsPlayer() || traceHit.m_pEnt->IsBaseCombatWeapon())
				{
					CBaseEntity* ent = (traceHit.m_pEnt->IsBaseCombatWeapon() && 
						traceHit.m_pEnt->GetOwnerEntity() && 
						(traceHit.m_pEnt->GetOwnerEntity()->IsNPC() || traceHit.m_pEnt->GetOwnerEntity()->IsPlayer()))
						? traceHit.m_pEnt->GetOwnerEntity() 
						: traceHit.m_pEnt;
					Vector vecSrc = pPlayer->Weapon_ShootPosition();
					Vector vecAiming = pPlayer->GetAutoaimVector(AUTOAIM_SCALE_DEFAULT);

					Ammo_t *ammodef = GetAmmoDef()->GetAmmoOfIndex(m_iPrimaryAmmoType);
					bool bIsAntlionGuard = (traceHit.m_pEnt->IsNPC() && (FClassnameIs(traceHit.m_pEnt, "npc_antlionguard") || FClassnameIs(traceHit.m_pEnt, "npc_antlionguardian")));
					pPlayer->FireBullets(3, vecSrc, vecAiming, VECTOR_CONE_4DEGREES, GetRange() * 2, m_iPrimaryAmmoType, 0, -1, -1, 
						(bIsAntlionGuard ? (ammodef->pPlrDmgCVar->GetInt() * sv_katana_antlionguard_damageresistance.GetFloat()) : ammodef->pPlrDmgCVar->GetInt()));

					if (ent && !ent->IsAlive() && g_pGameRules->isInBullettime && m_bKillMultiplier)
					{
						if (m_iKills < sv_katana_healthbonus_maxtimestogivebonus.GetInt())
						{
							m_iKills += 1;

							if (m_iKillMultiplier < sv_katana_healthbonus_maxmultiplier.GetInt())
							{
								m_iKillMultiplier += 1;
								pPlayer->TakeHealth((ent->GetMaxHealth() * 0.5) * m_iKillMultiplier, DMG_GENERIC);
								m_flLastKill = gpGlobals->curtime + sv_katana_healthbonus_postdelay.GetFloat();
								const char* hintMultiplier = CFmtStr("%ix!", m_iKillMultiplier);
								pPlayer->ShowLevelMessage(hintMultiplier);
							}
						}
					}

					WeaponSound(MELEE_HIT);
				}
				else
				{
					Hit(traceHit, GetActivity(), false);
					WeaponSound(MELEE_HIT_WORLD);
				}
			}
			else
			{
				WeaponSound(SINGLE);
			}
		}

		AddViewKick();
	}

	//Setup our next attack times
	m_flNextPrimaryAttack = gpGlobals->curtime + GetFireRate();
	m_flNextSecondaryAttack = gpGlobals->curtime + GetFireRate();

	CSoundEnt::InsertSound(SOUND_COMBAT, GetAbsOrigin(), 300, 0.2, GetOwner());
}

bool CWeaponKatana::Holster(CBaseCombatWeapon* pSwitchingTo)
{
	if (pSwitchingTo)
	{
		if (FClassnameIs(pSwitchingTo, "weapon_grapple"))
		{
			//force switch to the grapple if we want to use it.
			return BaseClass::Holster(pSwitchingTo);
		}
	}

	//this is dumb. why does it bug out when we get the 357??
	if (g_pGameRules->isInBullettime && m_iKillMultiplier > 0)
	{
		return false;
	}

	if (!g_pGameRules->isInBullettime)
	{
		if (m_flLastKill > gpGlobals->curtime && m_bKillMultiplier)
		{
			m_bKillMultiplier = false;
		}
	}

	if (m_iKillMultiplier > 0)
	{
		m_iKillMultiplier = 0;
		m_iKills = 0;
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

		if (m_flLastKill > gpGlobals->curtime && m_bKillMultiplier)
		{
			m_bKillMultiplier = false;
		}
	}
	else
	{
		if (m_iKills >= sv_katana_healthbonus_maxtimestogivebonus.GetInt())
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

	if ((m_iKills < sv_katana_healthbonus_maxtimestogivebonus.GetInt() && m_iKillMultiplier > 0 && g_pGameRules->isInBullettime) || 
		!g_pGameRules->isInBullettime)
	{
		if (m_flLastKill < gpGlobals->curtime && !m_bKillMultiplier)
		{
			m_iKillMultiplier = 0;
			m_iKills = 0;
			m_bKillMultiplier = true;
		}
	}

	BaseClass::ItemPostFrame();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CWeaponKatana::ImpactEffect(trace_t& traceHit)
{
	// See if we hit water (we don't do the other impact effects in this case)
	if (ImpactWater(traceHit.startpos, traceHit.endpos))
		return;

	//FIXME: need new decals
	UTIL_ImpactTrace(&traceHit, DMG_SLASH);
}

//-----------------------------------------------------------------------------
// Purpose: Get the damage amount for the animation we're doing
// Input  : hitActivity - currently played activity
// Output : Damage amount
//-----------------------------------------------------------------------------
float CWeaponKatana::GetDamageForActivity(Activity hitActivity)
{
	if ((GetOwner() != NULL) && (GetOwner()->IsPlayer()))
		return sk_plr_dmg_katana.GetFloat();

	return sk_npc_dmg_katana.GetFloat();
}