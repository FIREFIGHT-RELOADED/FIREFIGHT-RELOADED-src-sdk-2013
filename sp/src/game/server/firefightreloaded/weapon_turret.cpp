//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:	'weapon' what lets the player controll the rollerbuddy.
//
// $Revision: $
// $NoKeywords: $
//=============================================================================//

#include "cbase.h"
#include "basehlcombatweapon.h"
#include "npcevent.h"
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

#define TURRET_MAX_PLACEMENT_RANGE 192.0f
#define TURRET_MIN_PLACEMENT_RANGE 32.0f

ConVar	sv_infinite_turrets("sv_infinite_turrets", "0", FCVAR_ARCHIVE);
ConVar	sv_turret_hologram_opacity("sv_turret_hologram_opacity", "180", FCVAR_ARCHIVE);
ConVar  sv_turret_hologram_rotation_speed("sv_turret_hologram_rotation_speed", "250", FCVAR_CHEAT, "Degrees per second to rotate building when player alt-fires during placement.");

class CTurretHologram : public CAI_BaseNPC
{
public:
	DECLARE_CLASS(CTurretHologram, CAI_BaseNPC);
	DECLARE_DATADESC();

	enum HologramStatus
	{
		TURRET_JUSTRIGHT,
		TURRET_TOOFAR,
		TURRET_INVALIDPLACEMENT
	};

	void Spawn(void)
	{
		Precache();
		BaseClass::Spawn();

		//using a wide human as our measurement hull to allow for a better spawing area
		SetHullType(HULL_WIDE_HUMAN);
		SetHullSizeNormal();

		AddEFlags(EFL_DIRTY_ABSTRANSFORM);

#ifdef GLOWS_ENABLE
		m_bImportantOutline = true;
#endif
		SetModel(FLOOR_TURRET_WEAPON_HOLOGRAM_MODEL);

		//use the colorable skin.
		m_nSkin = 1;

		SetThink(&CTurretHologram::OnThink);
		SetNextThink(gpGlobals->curtime + 0.01f);
	}

	void OnThink(void)
	{
		SetNextThink(gpGlobals->curtime + 0.01f);

		switch (status)
		{
			case TURRET_JUSTRIGHT:
				clrHighlightColor = Color("#50ff50");
				break;
			case TURRET_TOOFAR:
				clrHighlightColor = Color("#ff9350");
				break;
			case TURRET_INVALIDPLACEMENT:
				clrHighlightColor = Color("#fc3a3a");
				break;
		}

		SetRenderMode(kRenderTransColor);
		SetRenderColor(255, 255, 255, sv_turret_hologram_opacity.GetInt());
		SetMaterialColorMode(COLORMODE_NORMAL);
		Vector matColor = Vector(clrHighlightColor.r(), clrHighlightColor.g(), clrHighlightColor.b());

		UpdateMaterialColor(matColor.x, matColor.y, matColor.z);
		
#ifdef GLOWS_ENABLE
		GiveOutline(matColor);
#endif
	}

	void SetStatus(HologramStatus statusToReport) { status = statusToReport; }
	void Precache(void) { PrecacheModel(FLOOR_TURRET_WEAPON_HOLOGRAM_MODEL); }
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

	void				SpawnTurret(void);

	float GetFireRate(void)
	{
		return 0.5f;
	}
    
	void				PrimaryAttack( void );
	void				SecondaryAttack(void);
	bool				Reload( void );
	bool				DecrementAmmo( CBaseCombatCharacter *pOwner );

private:
	CHandle<CTurretHologram>	pHologram;
	bool				m_bSetToRemoveAmmo;
	bool				m_bStopMovingHologram;

	float				m_flCurrentBuildRotation;
	int					m_iDesiredBuildRotations;
};

IMPLEMENT_SERVERCLASS_ST(CWeaponTurret, DT_WeaponTurret)
END_SEND_TABLE()

LINK_ENTITY_TO_CLASS( weapon_turret, CWeaponTurret );
PRECACHE_WEAPON_REGISTER(weapon_turret);

BEGIN_DATADESC(CWeaponTurret)
	DEFINE_FIELD(pHologram, FIELD_EHANDLE),
	DEFINE_FIELD(m_bSetToRemoveAmmo, FIELD_BOOLEAN),
	DEFINE_FIELD(m_bStopMovingHologram, FIELD_BOOLEAN),
	DEFINE_FIELD(m_flCurrentBuildRotation, FIELD_FLOAT),
	DEFINE_FIELD(m_iDesiredBuildRotations, FIELD_INTEGER),
END_DATADESC()

acttable_t CWeaponTurret::m_acttable[] =
{
	{ ACT_RANGE_ATTACK1, ACT_RANGE_ATTACK_SLAM, true },
	{ ACT_HL2MP_IDLE,					ACT_HL2AC_IDLE_TURRET,					false },
	{ ACT_HL2MP_RUN,					ACT_HL2AC_RUN_TURRET,					false },
	{ ACT_HL2MP_IDLE_CROUCH,			ACT_HL2AC_IDLE_CROUCH_TURRET,			false },
	{ ACT_HL2MP_WALK_CROUCH,			ACT_HL2AC_WALK_CROUCH_TURRET,			false },
	{ ACT_HL2MP_GESTURE_RANGE_ATTACK,	ACT_HL2AC_GESTURE_RANGE_ATTACK_TURRET,	false },
	{ ACT_HL2MP_GESTURE_RELOAD,			ACT_HL2MP_GESTURE_RELOAD_SLAM,		false },
	{ ACT_HL2MP_JUMP,					ACT_HL2AC_JUMP_TURRET,					false },
};

IMPLEMENT_ACTTABLE(CWeaponTurret);

CWeaponTurret::CWeaponTurret()
{
	m_fMinRange1 = TURRET_MIN_PLACEMENT_RANGE;
	m_fMaxRange1 = TURRET_MAX_PLACEMENT_RANGE;
	m_bSetToRemoveAmmo = false;
	m_bStopMovingHologram = false;
	m_iDesiredBuildRotations = 0;
	m_flCurrentBuildRotation = 0.0f;
	pHologram = NULL;
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
	UTIL_PrecacheOther( "npc_turret_floor_weapon" );
}

void CWeaponTurret::ItemPreFrame(void)
{
	MoveHologram();

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
		SpawnTurret();
	}

	BaseClass::ItemPostFrame();
}

void CWeaponTurret::SpawnTurret(void)
{
	CBasePlayer* pOwner = ToBasePlayer(GetOwner());
	if (!pOwner)
		return;

	CNPC_FloorTurret* pTurret = dynamic_cast<CNPC_FloorTurret*>(CreateEntityByName("npc_turret_floor_weapon"));
	if (pTurret)
	{
		pTurret->SetName(AllocPooledString("spawnedTurret"));
		pTurret->m_bDisableInitAttributes = true;
		pTurret->m_bNoRemove = true;
		pTurret->SetOwnerEntity(pOwner);
		DispatchSpawn(pTurret);
		if (pHologram)
		{
			pTurret->Teleport(&pHologram->GetAbsOrigin(), &pHologram->GetAbsAngles(), NULL);
		}

		pTurret->Activate();

		WeaponSound(SPECIAL1);
	}

	m_bStopMovingHologram = false;
	MoveHologram();

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

void CWeaponTurret::SecondaryAttack(void)
{
	if (pHologram)
	{
		// rotate the build angles by 90 degrees ( final angle calculated after we network this )
		m_iDesiredBuildRotations++;
		m_iDesiredBuildRotations = m_iDesiredBuildRotations % 4;
	}

	m_flNextSecondaryAttack = gpGlobals->curtime + 0.3f;
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : NULL - 
//-----------------------------------------------------------------------------
bool CWeaponTurret::Holster(CBaseCombatWeapon* pSwitchingTo)
{
	CBasePlayer* pOwner = ToBasePlayer(GetOwner());
	if (!pOwner)
		return BaseClass::Holster(pSwitchingTo);

	if (m_bSetToRemoveAmmo && (m_flNextPrimaryAttack > gpGlobals->curtime))
	{
		//spawn it NOW.
		SpawnTurret();
		StopHologram();
		pOwner->Weapon_Detach(this);
		UTIL_Remove(this);
	}
	else
	{
		StopHologram();
	}

	m_iDesiredBuildRotations = 0;
	m_flCurrentBuildRotation = 0.0f;

	return BaseClass::Holster(pSwitchingTo);
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CWeaponTurret::Drop(const Vector& velocity)
{
	CBasePlayer* pOwner = ToBasePlayer(GetOwner());
	if (!pOwner)
		return;

	if (m_bSetToRemoveAmmo && (m_flNextPrimaryAttack > gpGlobals->curtime))
	{
		//spawn it NOW.
		SpawnTurret();
		StopHologram();
		pOwner->Weapon_Detach(this);
		UTIL_Remove(this);
	}
	else
	{
		StopHologram();
	}

	m_iDesiredBuildRotations = 0;
	m_flCurrentBuildRotation = 0.0f;

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
	if (pHologram == nullptr || pHologram == NULL)
	{
		//StartHologram will move back to us.
		StartHologram();
		return;
	}

	if (m_bStopMovingHologram)
		return;

	CBasePlayer* pOwner = ToBasePlayer(GetOwner());
	if (!pOwner)
		return;

	if (pHologram)
	{
		if (!pOwner->IsAlive())
		{
			StopHologram();
		}

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
			//pHologram->SetAbsAngles(angles);

			// Calculate build angles
			QAngle vecAngles = vec3_angle;
			vecAngles.y = pOwner->EyeAngles().y;

			QAngle objAngles = vecAngles;

			SetAbsAngles(objAngles);

			float flBuildRotation = 90.0f * m_iDesiredBuildRotations;

			m_flCurrentBuildRotation = ApproachAngle(flBuildRotation, m_flCurrentBuildRotation, sv_turret_hologram_rotation_speed.GetFloat() * gpGlobals->frametime);

			objAngles.y = objAngles.y + m_flCurrentBuildRotation;

			pHologram->SetLocalAngles(objAngles);

			//pHologram->Teleport(&tr.endpos, &angles, NULL);
			UTIL_DropToFloor(pHologram, MASK_NPCSOLID);
			// Now check that this is a valid location for the new npc to be
			Vector	vUpBit = pHologram->GetAbsOrigin();
			vUpBit.z += 1;

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

			//mins and maxs for the hologram's hull
			AI_TraceHull(pHologram->GetAbsOrigin(), vUpBit, pHologram->GetHullMins(), pHologram->GetHullMaxs(),
				MASK_NPCSOLID, pHologram, COLLISION_GROUP_NONE, &tr);
			if (tr.fraction < 1.0)
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
	if (pHologram == nullptr || pHologram == NULL)
		return;

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
