//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:	'weapon' what lets the player controll the rollerbuddy.
//
// $Revision: $
// $NoKeywords: $
//=============================================================================//

#include "cbase.h"
#include "basehlcombatweapon.h"
#include "NPCevent.h"
#include "basecombatcharacter.h"
#include "ai_basenpc.h"
#include "player.h"
#include "entitylist.h"
#include "ndebugoverlay.h"
#include "soundent.h"
#include "engine/IEngineSound.h"
#include "rotorwash.h"
#include "npc_turret_floor.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

#define TURRET_MAX_PLACEMENT_RANGE 128.0f
#define TURRET_MIN_PLACEMENT_RANGE 32.0f

ConVar	sv_infinite_turrets("sv_infinite_turrets", "0", FCVAR_ARCHIVE);

class CTurretHologram : public CAI_BaseNPC
{
public:
	DECLARE_CLASS(CTurretHologram, CAI_BaseNPC);
	DECLARE_DATADESC();

	static enum HologramStatus
	{
		TURRET_JUSTRIGHT,
		TURRET_TOOFAR,
		TURRET_INVALIDPLACEMENT
	};

	void Spawn(void)
	{
		Precache();
		BaseClass::Spawn();

		m_bImportantOutline = true;
		SetModel(FLOOR_TURRET_WEAPON_MODEL);

		SetRenderMode(kRenderTransColor);
		SetRenderColor(80, 80, 80, 180);

		SetThink(&CTurretHologram::OnThink);
		SetNextThink(gpGlobals->curtime + 0.01f);
	}

	void OnThink(void)
	{
		SetNextThink(gpGlobals->curtime + 0.01f);

		switch (status)
		{
			case TURRET_JUSTRIGHT:
				clrHighlightColor = Color(80, 255, 80);
				break;
			case TURRET_TOOFAR:
			case TURRET_INVALIDPLACEMENT:
				clrHighlightColor = Color(255, 80, 80);
				break;
		}

		SetRenderMode(kRenderTransColor);
		SetRenderColor(clrHighlightColor.r(), clrHighlightColor.g(), clrHighlightColor.b(), 150);
		
		Vector outlineColor = Vector(clrHighlightColor.r(), clrHighlightColor.g(), clrHighlightColor.b());
		GiveOutline(outlineColor);
	}

	void SetStatus(HologramStatus statusToReport) { status = statusToReport; }
	void Precache(void) { PrecacheModel(FLOOR_TURRET_WEAPON_MODEL); }
	HologramStatus GetStatus(void) { return status; }

private:
	Color clrHighlightColor;
	HologramStatus status;
};

LINK_ENTITY_TO_CLASS(turret_hologram, CTurretHologram);
PRECACHE_REGISTER(turret_hologram);

BEGIN_DATADESC(CTurretHologram)
DEFINE_FIELD(clrHighlightColor, FIELD_COLOR32),
DEFINE_THINKFUNC(OnThink),
END_DATADESC()

class CWeaponTurret: public CBaseHLCombatWeapon
{
	DECLARE_CLASS( CWeaponTurret, CBaseHLCombatWeapon );
public:
	DECLARE_SERVERCLASS();
	DECLARE_ACTTABLE();
	DECLARE_DATADESC();
	CWeaponTurret();

	bool				Deploy(void);
	void				Spawn( void );
	void				Precache( void );
	void				ItemPreFrame(void);
	void				ItemPostFrame(void);

	bool				Holster(CBaseCombatWeapon* pSwitchingTo = NULL);
	void				Drop(const Vector& velocity);

	void				StartHologram(void);
	void				MoveHologram(void);
	void				StopHologram(void);

	float GetFireRate(void)
	{
		return 0.5f;
	}
    
	void				PrimaryAttack( void );
	bool				Reload( void );
	bool				DecrementAmmo( CBaseCombatCharacter *pOwner );

private:
	CTurretHologram		*pHologram;
	bool				m_bSetToRemoveAmmo;
	bool				m_bStopMovingHologram;
};

IMPLEMENT_SERVERCLASS_ST(CWeaponTurret, DT_WeaponTurret)
END_SEND_TABLE()

LINK_ENTITY_TO_CLASS( weapon_turret, CWeaponTurret );
PRECACHE_WEAPON_REGISTER(weapon_turret);

BEGIN_DATADESC(CWeaponTurret)
	DEFINE_FIELD(pHologram, FIELD_EHANDLE),
END_DATADESC()

acttable_t CWeaponTurret::m_acttable[] =
{
	{ ACT_RANGE_ATTACK1, ACT_RANGE_ATTACK_SLAM, true },
	{ ACT_HL2MP_IDLE,					ACT_HL2MP_IDLE_SLAM,					false },
	{ ACT_HL2MP_RUN,					ACT_HL2MP_RUN_SLAM,					false },
	{ ACT_HL2MP_IDLE_CROUCH,			ACT_HL2MP_IDLE_CROUCH_SLAM,			false },
	{ ACT_HL2MP_WALK_CROUCH,			ACT_HL2MP_WALK_CROUCH_SLAM,			false },
	{ ACT_HL2MP_GESTURE_RANGE_ATTACK,	ACT_HL2MP_GESTURE_RANGE_ATTACK_SLAM,	false },
	{ ACT_HL2MP_GESTURE_RELOAD,			ACT_HL2MP_GESTURE_RELOAD_SLAM,		false },
	{ ACT_HL2MP_JUMP,					ACT_HL2MP_JUMP_SLAM,					false },
};

IMPLEMENT_ACTTABLE(CWeaponTurret);

CWeaponTurret::CWeaponTurret()
{
	m_fMinRange1 = TURRET_MIN_PLACEMENT_RANGE;
	m_fMaxRange1 = TURRET_MAX_PLACEMENT_RANGE;
	m_bSetToRemoveAmmo = false;
	m_bStopMovingHologram = false;
}

void CWeaponTurret::Spawn( )
{
	BaseClass::Spawn();
	Precache( );
}

bool CWeaponTurret::Deploy(void)
{
	if (!pHologram)
	{
		StartHologram();
	}

	return BaseClass::Deploy();
}

void CWeaponTurret::Precache( void )
{
	BaseClass::Precache();
	UTIL_PrecacheOther( "npc_turret_floor" );
}

void CWeaponTurret::ItemPreFrame(void)
{
	if (!m_bStopMovingHologram)
	{
		MoveHologram();
	}

	BaseClass::ItemPreFrame();
}

void CWeaponTurret::ItemPostFrame(void)
{
	CBasePlayer* pOwner = ToBasePlayer(GetOwner());
	if (!pOwner)
		return;

	MoveHologram();

	if (m_bSetToRemoveAmmo && (m_flNextPrimaryAttack <= gpGlobals->curtime))
	{
		CNPC_FloorTurret* pTurret = dynamic_cast<CNPC_FloorTurret*>(CreateEntityByName("npc_turret_floor_weapon"));
		if (pTurret)
		{
			pTurret->SetName(AllocPooledString("spawnedTurret"));
			pTurret->m_bDisableInitAttributes = true;
			pTurret->AddSpawnFlags(SF_FLOOR_TURRET_WEAPON);
			DispatchSpawn(pTurret);
			pTurret->SetOwnerEntity(pOwner);
			if (pHologram)
			{
				pTurret->Teleport(&pHologram->GetAbsOrigin(), &pHologram->GetAbsAngles(), NULL);
			}
			UTIL_DropToFloor(pTurret, MASK_NPCSOLID);


			pTurret->Activate();

			WeaponSound(SPECIAL1);
		}

		if (!DecrementAmmo(pOwner))
		{
			BaseClass::ItemPostFrame();
			return;
		}
		else
		{
			m_bSetToRemoveAmmo = false;
		}
	}

	BaseClass::ItemPostFrame();
}

bool CWeaponTurret::Reload( void )
{
	WeaponIdle();
	return true;
}

void CWeaponTurret::PrimaryAttack( void )
{
	CBasePlayer *pPlayer = ToBasePlayer( GetOwner() );
	if ( !pPlayer )
		return;

	if (pHologram)
	{
		if (pHologram->GetStatus() == CTurretHologram::TURRET_INVALIDPLACEMENT ||
			pHologram->GetStatus() == CTurretHologram::TURRET_TOOFAR)
		{
			WeaponSound(EMPTY);
			m_flNextPrimaryAttack = gpGlobals->curtime + GetFireRate();
			return;
		}

		m_bStopMovingHologram = true;
	}

	WeaponSound(SINGLE);
	SendWeaponAnim(ACT_SLAM_TRIPMINE_ATTACH);
	pPlayer->SetAnimation(PLAYER_ATTACK1);

	m_flNextPrimaryAttack = gpGlobals->curtime + (SequenceDuration() * 0.3f);
	m_bSetToRemoveAmmo = true;

	//sequence duration dictates turret spawn.
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : NULL - 
//-----------------------------------------------------------------------------
bool CWeaponTurret::Holster(CBaseCombatWeapon* pSwitchingTo)
{
	StopHologram();
	return BaseClass::Holster(pSwitchingTo);
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CWeaponTurret::Drop(const Vector& velocity)
{
	StopHologram();
	BaseClass::Drop(velocity);
}

void CWeaponTurret::StartHologram(void)
{
	CBasePlayer* pOwner = ToBasePlayer(GetOwner());
	if (!pOwner)
		return;

	pHologram = (CTurretHologram*)CBaseEntity::Create("turret_hologram", pOwner->GetAbsOrigin(), pOwner->GetAbsAngles(), pOwner);
	if (pHologram)
	{
		pHologram->SetOwnerEntity(pOwner);
		pHologram->Spawn();
	}

	MoveHologram();
}

void CWeaponTurret::MoveHologram(void)
{
	CBasePlayer* pOwner = ToBasePlayer(GetOwner());
	if (!pOwner)
		return;

	if (pHologram)
	{
		// Now attempt to drop into the world
		QAngle angles;
		trace_t tr;
		Vector forward;
		pOwner->EyeVectors(&forward);
		VectorAngles(forward, angles);
		angles.x = 0;
		angles.z = 0;
		AI_TraceLine(pOwner->EyePosition(),
			pOwner->EyePosition() + forward * MAX_TRACE_LENGTH, MASK_NPCSOLID,
			pOwner, COLLISION_GROUP_NONE, &tr);

		if (tr.fraction != 1.0)
		{
			pHologram->SetAbsOrigin(tr.endpos);
			pHologram->SetAbsAngles(angles);
			//pHologram->Teleport(&tr.endpos, &angles, NULL);
			UTIL_DropToFloor(pHologram, MASK_NPCSOLID);
			// Now check that this is a valid location for the new npc to be
			Vector	vUpBit = pHologram->GetAbsOrigin();
			vUpBit.z += 1;

			//mins and maxs for a human hull (turret's hull)
			AI_TraceHull(pHologram->GetAbsOrigin(), vUpBit, Vector(-13, -13, 0), Vector(13, 13, 72),
				MASK_NPCSOLID, pHologram, COLLISION_GROUP_NONE, &tr);
			if (tr.startsolid || (tr.fraction < 1.0))
			{
				pHologram->SetStatus(CTurretHologram::TURRET_INVALIDPLACEMENT);
				return;
			}

			float enemyDelta = (pHologram->WorldSpaceCenter() - pOwner->WorldSpaceCenter()).Length();

			if (enemyDelta > TURRET_MAX_PLACEMENT_RANGE)
			{
				pHologram->SetStatus(CTurretHologram::TURRET_TOOFAR);
				return;
			}
			else if (enemyDelta < TURRET_MIN_PLACEMENT_RANGE)
			{
				pHologram->SetStatus(CTurretHologram::TURRET_INVALIDPLACEMENT);
				return;
			}

			pHologram->SetStatus(CTurretHologram::TURRET_JUSTRIGHT);
		}
	}
	else
	{
		StartHologram();
	}
}

void CWeaponTurret::StopHologram(void)
{
	if (pHologram)
	{
		pHologram->SUB_Remove();
		pHologram = NULL;
	}
}

bool CWeaponTurret::DecrementAmmo( CBaseCombatCharacter *pOwner )
{
	if (!sv_infinite_turrets.GetBool())
	{
		pOwner->RemoveAmmo(1, m_iPrimaryAmmoType);

		if (pOwner->GetAmmoCount(m_iPrimaryAmmoType) <= 0)
		{
			StopHologram();
			pOwner->Weapon_Detach(this);
			engine->ClientCommand(pOwner->edict(), "lastinv");
			engine->ClientCommand(pOwner->edict(), "-attack");
			UTIL_Remove(this);
			return false;
		}
	}
    
    return true;
}