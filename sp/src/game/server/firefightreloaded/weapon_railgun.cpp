//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#include "cbase.h"
#include "basecombatcharacter.h"
#include "ai_basenpc.h"
#include "ai_memory.h"
#include "npcevent.h"
#include "in_buttons.h"
#include "takedamageinfo.h"
#include "weapon_railgun.h"
#include "hl2_player.h"
#include "ammodef.h"
#include "explode.h"
#include "globalstate.h"

ConVar sk_weapon_railgun_overcharge_limit("sk_weapon_railgun_overcharge_limit", "500", FCVAR_ARCHIVE);
ConVar sk_weapon_railgun_warning_beep_time("sk_weapon_railgun_warning_beep_time", "3.5", FCVAR_ARCHIVE);

IMPLEMENT_SERVERCLASS_ST(CWeaponRailgun, DT_WeaponRailgun)
END_SEND_TABLE()

BEGIN_DATADESC(CWeaponRailgun)
DEFINE_FIELD(m_flNextCharge, FIELD_TIME),
DEFINE_FIELD(m_flNextSuitCharge, FIELD_TIME),
DEFINE_FIELD(m_flNextDecharge, FIELD_TIME),
DEFINE_FIELD(m_bInZoom, FIELD_BOOLEAN),
DEFINE_FIELD(m_bJustOvercharged, FIELD_BOOLEAN),
DEFINE_FIELD(m_bIsLowBattery, FIELD_BOOLEAN),
DEFINE_FIELD(m_bOverchargeDamageBenefits, FIELD_BOOLEAN),
DEFINE_FIELD(m_flNextWarningBeep, FIELD_TIME),
DEFINE_FIELD(m_bPlayedDechargingSound, FIELD_BOOLEAN),
DEFINE_FIELD(m_bPlayedChargingSound, FIELD_BOOLEAN),
DEFINE_FIELD(m_bFirstEquip, FIELD_BOOLEAN),
DEFINE_FIELD(m_bJustRecovered, FIELD_BOOLEAN),
END_DATADESC()

LINK_ENTITY_TO_CLASS( weapon_railgun, CWeaponRailgun );
PRECACHE_WEAPON_REGISTER( weapon_railgun );

acttable_t CWeaponRailgun::m_acttable[] =
{
	{ ACT_RANGE_ATTACK1,			ACT_RANGE_ATTACK_AR2,			true },
	{ ACT_RELOAD,					ACT_RELOAD_SMG1,				true },		// FIXME: hook to AR2 unique
	{ ACT_IDLE,						ACT_IDLE_SMG1,					true },		// FIXME: hook to AR2 unique
	{ ACT_IDLE_ANGRY,				ACT_IDLE_ANGRY_SMG1,			true },		// FIXME: hook to AR2 unique

	{ ACT_WALK,						ACT_WALK_RIFLE,					true },

// Readiness activities (not aiming)
	{ ACT_IDLE_RELAXED,				ACT_IDLE_SMG1_RELAXED,			false },//never aims
	{ ACT_IDLE_STIMULATED,			ACT_IDLE_SMG1_STIMULATED,		false },
	{ ACT_IDLE_AGITATED,			ACT_IDLE_ANGRY_SMG1,			false },//always aims

	{ ACT_WALK_RELAXED,				ACT_WALK_RIFLE_RELAXED,			false },//never aims
	{ ACT_WALK_STIMULATED,			ACT_WALK_RIFLE_STIMULATED,		false },
	{ ACT_WALK_AGITATED,			ACT_WALK_AIM_RIFLE,				false },//always aims

	{ ACT_RUN_RELAXED,				ACT_RUN_RIFLE_RELAXED,			false },//never aims
	{ ACT_RUN_STIMULATED,			ACT_RUN_RIFLE_STIMULATED,		false },
	{ ACT_RUN_AGITATED,				ACT_RUN_AIM_RIFLE,				false },//always aims

	// Readiness activities (aiming)
	{ ACT_IDLE_AIM_RELAXED,			ACT_IDLE_SMG1_RELAXED,			false },//never aims	
	{ ACT_IDLE_AIM_STIMULATED,		ACT_IDLE_AIM_RIFLE_STIMULATED,	false },
	{ ACT_IDLE_AIM_AGITATED,		ACT_IDLE_ANGRY_SMG1,			false },//always aims

	{ ACT_WALK_AIM_RELAXED,			ACT_WALK_RIFLE_RELAXED,			false },//never aims
	{ ACT_WALK_AIM_STIMULATED,		ACT_WALK_AIM_RIFLE_STIMULATED,	false },
	{ ACT_WALK_AIM_AGITATED,		ACT_WALK_AIM_RIFLE,				false },//always aims

	{ ACT_RUN_AIM_RELAXED,			ACT_RUN_RIFLE_RELAXED,			false },//never aims
	{ ACT_RUN_AIM_STIMULATED,		ACT_RUN_AIM_RIFLE_STIMULATED,	false },
	{ ACT_RUN_AIM_AGITATED,			ACT_RUN_AIM_RIFLE,				false },//always aims
	//End readiness activities

	{ ACT_WALK_AIM,					ACT_WALK_AIM_RIFLE,				true },
	{ ACT_WALK_CROUCH,				ACT_WALK_CROUCH_RIFLE,			true },
	{ ACT_WALK_CROUCH_AIM,			ACT_WALK_CROUCH_AIM_RIFLE,		true },
	{ ACT_RUN,						ACT_RUN_RIFLE,					true },
	{ ACT_RUN_AIM,					ACT_RUN_AIM_RIFLE,				true },
	{ ACT_RUN_CROUCH,				ACT_RUN_CROUCH_RIFLE,			true },
	{ ACT_RUN_CROUCH_AIM,			ACT_RUN_CROUCH_AIM_RIFLE,		true },
	{ ACT_GESTURE_RANGE_ATTACK1,	ACT_GESTURE_RANGE_ATTACK_AR2,	false },
	{ ACT_COVER_LOW,				ACT_COVER_SMG1_LOW,				false },		// FIXME: hook to AR2 unique
	{ ACT_RANGE_AIM_LOW,			ACT_RANGE_AIM_AR2_LOW,			false },
	{ ACT_RANGE_ATTACK1_LOW,		ACT_RANGE_ATTACK_SMG1_LOW,		true },		// FIXME: hook to AR2 unique
	{ ACT_RELOAD_LOW,				ACT_RELOAD_SMG1_LOW,			false },
	{ ACT_GESTURE_RELOAD,			ACT_GESTURE_RELOAD_SMG1,		true },
	//	{ ACT_RANGE_ATTACK2, ACT_RANGE_ATTACK_AR2_GRENADE, true },

	{ ACT_HL2MP_IDLE,					ACT_HL2MP_IDLE_CROSSBOW,					false },
	{ ACT_HL2MP_RUN,					ACT_HL2MP_RUN_CROSSBOW,						false },
	{ ACT_HL2MP_IDLE_CROUCH,			ACT_HL2MP_IDLE_CROUCH_CROSSBOW,				false },
	{ ACT_HL2MP_WALK_CROUCH,			ACT_HL2MP_WALK_CROUCH_CROSSBOW,				false },
	{ ACT_HL2MP_GESTURE_RANGE_ATTACK,	ACT_HL2MP_GESTURE_RANGE_ATTACK_CROSSBOW,	false },
	{ ACT_HL2MP_GESTURE_RELOAD,			ACT_HL2MP_GESTURE_RELOAD_CROSSBOW,			false },
	{ ACT_HL2MP_JUMP,					ACT_HL2MP_JUMP_CROSSBOW,					false },
	{ ACT_RANGE_ATTACK1,			ACT_RANGE_ATTACK_AR2,			false },
};

IMPLEMENT_ACTTABLE(CWeaponRailgun);

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CWeaponRailgun::CWeaponRailgun( void )
{
	m_bReloadsSingly	= false;
	m_bFiresUnderwater	= false;
	m_flNextCharge = 0;
	m_flNextSuitCharge = 0;
	m_flNextDecharge = 0;
	m_flNextWarningBeep = 0;
	m_bInZoom = false;
	m_bJustOvercharged = false;
	m_bOverchargeDamageBenefits = false;
	m_bIsLowBattery = false;
	m_bPlayedDechargingSound = false;
	m_bPlayedChargingSound = true;
	m_bFirstEquip = true;
	m_bJustRecovered = false;

	m_fMinRange1 = 0;
	m_fMaxRange1 = 99999;
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : *pOperator - 
//-----------------------------------------------------------------------------
void CWeaponRailgun::FireNPCPrimaryAttack(CBaseCombatCharacter* pOperator, bool bUseWeaponAngles)
{
	Vector vecShootOrigin, vecShootDir;

	CAI_BaseNPC* npc = pOperator->MyNPCPointer();
	ASSERT(npc != NULL);

	if (bUseWeaponAngles)
	{
		QAngle	angShootDir;
		GetAttachment(LookupAttachment("muzzle"), vecShootOrigin, angShootDir);
		AngleVectors(angShootDir, &vecShootDir);
	}
	else
	{
		vecShootOrigin = pOperator->Weapon_ShootPosition();
		vecShootDir = npc->GetActualShootTrajectory(vecShootOrigin);
	}

	Vector	endPos = vecShootOrigin + (vecShootDir * MAX_TRACE_LENGTH);

	//Shoot a shot straight out
	trace_t	tr;
	UTIL_TraceLine(vecShootOrigin, endPos, MASK_SHOT, npc, COLLISION_GROUP_NONE, &tr);

	CAmmoDef* def = GetAmmoDef();
	int definedDamage = def->NPCDamage(m_iPrimaryAmmoType);
	bool bUsesOvercharge = (npc && npc->m_pAttributes != NULL && npc->m_pAttributes->GetBool("use_railgun_overcharge"));
	int iDamage = (bUsesOvercharge ? (int)(definedDamage * 2) : definedDamage);

	FireBulletsInfo_t info(1, vecShootOrigin, vecShootDir, vec3_origin, MAX_TRACE_LENGTH, m_iPrimaryAmmoType);
	info.m_pAttacker = npc;
	info.m_flDamage = iDamage;
	info.m_flDamageForceScale = 0.2f;

	WeaponSound(SINGLE);
	CSoundEnt::InsertSound(SOUND_COMBAT | SOUND_CONTEXT_GUNFIRE, pOperator->GetAbsOrigin(), SOUNDENT_VOLUME_MACHINEGUN, 0.2, pOperator, SOUNDENT_CHANNEL_WEAPON, pOperator->GetEnemy());

	// Fire the bullets, and force the first shot to be perfectly accurate
	npc->FireBullets(info);

	float hitAngle = -DotProduct(tr.plane.normal, vecShootDir);

	Vector vReflection;

	vReflection = 2.0 * tr.plane.normal * hitAngle + vecShootDir;

	vecShootOrigin = tr.endpos;
	endPos = vecShootOrigin + (vReflection * MAX_TRACE_LENGTH);

	//Kick up an effect
	if (!(tr.surface.flags & SURF_SKY))
	{
		UTIL_ImpactTrace(&tr, m_iPrimaryAmmoType, "ImpactJeep");

		//Do a gauss explosion
		CPVSFilter filter(tr.endpos);
		te->GaussExplosion(filter, 0.0f, tr.endpos, tr.plane.normal, 0);
	}

	//Draw beam to reflection point
	DrawBeam(tr.startpos, tr.endpos);
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CWeaponRailgun::Operator_ForceNPCFire(CBaseCombatCharacter* pOperator, bool bSecondary)
{
	FireNPCPrimaryAttack(pOperator, false);
}

//-----------------------------------------------------------------------------
void CWeaponRailgun::Operator_HandleAnimEvent(animevent_t* pEvent, CBaseCombatCharacter* pOperator)
{
	switch (pEvent->event)
	{
	case EVENT_WEAPON_AR2:
	{
		FireNPCPrimaryAttack(pOperator, false);
		break;
	}

	default:
		BaseClass::Operator_HandleAnimEvent(pEvent, pOperator);
		break;
	}
}

void CWeaponRailgun::Precache(void)
{
	BaseClass::Precache();

	PrecacheScriptSound("SuitRecharge.ChargingLoop");
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
bool CWeaponRailgun::Deploy(void)
{
	return BaseClass::Deploy();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
bool CWeaponRailgun::Holster(CBaseCombatWeapon* pSwitchingTo)
{
	CBasePlayer* pPlayer = ToBasePlayer(GetOwner());

	if (!pPlayer)
		return BaseClass::Holster(pSwitchingTo);

	if (pPlayer->GetAmmoCount(m_iPrimaryAmmoType) <= 0)
	{
		pPlayer->GiveAmmo(1, m_iPrimaryAmmoType, true);
	}

	if (m_bInZoom)
	{
		ToggleZoom();
	}

	return BaseClass::Holster(pSwitchingTo);
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CWeaponRailgun::CheckZoomToggle(void)
{
	CBasePlayer* pPlayer = ToBasePlayer(GetOwner());

	if (!pPlayer)
		return;

	if (pPlayer->m_afButtonPressed & IN_ATTACK2)
	{
		ToggleZoom();
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CWeaponRailgun::ItemBusyFrame(void)
{
	// Allow zoom toggling even when we're reloading
	CheckZoomToggle();
}

void CWeaponRailgun::HolsterThink(void)
{
	CBasePlayer* pOwner = ToBasePlayer(GetOwner());
	if (!pOwner || !pOwner->IsAlive())
		return;

	// no check for buttons here. we're holstered.
	if (pOwner->GetAmmoCount(m_iPrimaryAmmoType) < GetDefaultClip1())
	{
		RechargeAmmo(true);
	}
	else if (pOwner->GetAmmoCount(m_iPrimaryAmmoType) > GetDefaultClip1())
	{
		m_bJustOvercharged = true;
		m_bOverchargeDamageBenefits = true;
	}
}

void CWeaponRailgun::ItemPostFrame(void)
{
	// Allow zoom toggling
	CheckZoomToggle();

	CBasePlayer* pOwner = ToBasePlayer(GetOwner());
	if (!pOwner)
		return;

	if (m_bFirstEquip)
	{
		pOwner->SetAmmoCount(GetDefaultClip1(), m_iPrimaryAmmoType);
		m_bFirstEquip = false;
	}

	if (pOwner->GetAmmoCount(m_iPrimaryAmmoType) < GetDefaultClip1())
	{
		if ((pOwner->GetAmmoCount(m_iPrimaryAmmoType) < RAIL_AMMO_OVERCHARGE))
		{
			m_bIsLowBattery = true;
		}

		if (((pOwner->m_nButtons & IN_ATTACK) == false) && ((pOwner->m_nButtons & IN_ATTACK2) == false))
		{
			RechargeAmmo(false);
		}
	}
	else if (pOwner->GetAmmoCount(m_iPrimaryAmmoType) > GetDefaultClip1())
	{
		m_bJustOvercharged = true;
		m_bOverchargeDamageBenefits = true;
	}

	UpdateAutoFire();

	//Track the duration of the fire
	//FIXME: Check for IN_ATTACK2 as well?
	//FIXME: What if we're calling ItemBusyFrame?
	m_fFireDuration = (pOwner->m_nButtons & IN_ATTACK) ? (m_fFireDuration + gpGlobals->frametime) : 0.0f;

	bool bFired = false;

	if (!bFired && (pOwner->m_nButtons & IN_ATTACK) && (m_flNextPrimaryAttack <= gpGlobals->curtime))
	{
		//NOTENOTE: There is a bug with this code with regards to the way machine guns catch the leading edge trigger
			//			on the player hitting the attack key.  It relies on the gun catching that case in the same frame.
			//			However, because the player can also be doing a secondary attack, the edge trigger may be missed.
			//			We really need to hold onto the edge trigger and only clear the condition when the gun has fired its
			//			first shot.  Right now that's too much of an architecture change -- jdw

			// If the firing button was just pressed, or the alt-fire just released, reset the firing time
		if ((pOwner->m_afButtonPressed & IN_ATTACK) || (pOwner->m_afButtonReleased & IN_ATTACK2))
		{
			m_flNextPrimaryAttack = gpGlobals->curtime;
		}

		if (!(pOwner->GetAmmoCount(m_iPrimaryAmmoType) <= 0))
		{
			PrimaryAttack();
		}

		if (AutoFiresFullClip())
		{
			m_bFiringWholeClip = true;
		}
	}

	if (m_bOverchargeDamageBenefits && (m_flNextWarningBeep <= gpGlobals->curtime) && (sk_weapon_railgun_overcharge_limit.GetInt() > 0 && pOwner->GetAmmoCount(m_iPrimaryAmmoType) > sk_weapon_railgun_overcharge_limit.GetInt()))
	{
		if (sk_weapon_railgun_warning_beep_time.GetFloat() > 0)
		{
			WeaponSound(SPECIAL1);
			m_flNextWarningBeep = gpGlobals->curtime + sk_weapon_railgun_warning_beep_time.GetFloat();
		}
		else
		{
			m_flNextWarningBeep = gpGlobals->curtime + 3.0f;
		}

		if (GlobalEntity_GetState("weapon_hidehints") == GLOBAL_OFF)
		{
			CFmtStr hint;
			hint.sprintf("#valve_hint_warning_%s", GetClassname());
			UTIL_HudHintText(GetOwner(), hint.Access());
			m_iStandardHudHintCount++;
			m_bStandardHudHintDisplayed = true;
			m_flHudHintMinDisplayTime = gpGlobals->curtime + MIN_HUDHINT_DISPLAY_TIME;
		}
	}

	if (pOwner->m_nButtons & IN_RELOAD)
	{
		if (m_bOverchargeDamageBenefits && (sk_weapon_railgun_overcharge_limit.GetInt() > 0 && pOwner->GetAmmoCount(m_iPrimaryAmmoType) > sk_weapon_railgun_overcharge_limit.GetInt()))
		{
			DechargeAmmo();
		}
	}

	if (pOwner->m_nButtons & IN_ATTACK3)
	{
		ChargeAmmo();
	}

	if (m_bOverchargeDamageBenefits && pOwner->GetAmmoCount(m_iPrimaryAmmoType) < sk_weapon_railgun_overcharge_limit.GetInt())
	{
		if (m_bJustRecovered)
		{
			m_flNextSuitCharge = gpGlobals->curtime + RAIL_RECHARGE_RECOVERY_TIME;
			m_bJustRecovered = false;
		}

		if (m_bPlayedDechargingSound)
		{
			StopSound("SuitRecharge.ChargingLoop");
			m_bPlayedDechargingSound = false;
		}
	}

	// -----------------------
	//  No buttons down
	// -----------------------
	if (!((pOwner->m_nButtons & IN_ATTACK) || 
		(pOwner->m_nButtons & IN_ATTACK2) || 
		(pOwner->m_nButtons & IN_RELOAD) || 
		(pOwner->m_nButtons & IN_ATTACK3)))
	{
		// no fire buttons down or reloading
		if (!ReloadOrSwitchWeapons() && (m_bInReload == false))
		{
			WeaponIdle();
		}

		if (m_bPlayedDechargingSound)
		{
			StopSound("SuitRecharge.ChargingLoop");
			m_bPlayedDechargingSound = false;
		}

		if (m_bPlayedChargingSound)
		{
			StopSound("SuitRecharge.ChargingLoop");
			m_bPlayedChargingSound = false;
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CWeaponRailgun::ToggleZoom(void)
{
	CBasePlayer* pPlayer = ToBasePlayer(GetOwner());

	if (pPlayer == NULL)
		return;

	CBaseViewModel* vm = pPlayer->GetViewModel();

	if (vm == NULL)
		return;

	if (IsIronsighted())
		return;

	if (m_bInZoom)
	{
		if (pPlayer->SetFOV(this, 0, 0.2f))
		{
			vm->RemoveEffects(EF_NODRAW);
			m_bInZoom = false;
		}
	}
	else
	{
		if (pPlayer->SetFOV(this, 20, 0.1f))
		{
			vm->AddEffects(EF_NODRAW);
			m_bInZoom = true;
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CWeaponRailgun::Fire( void )
{
	CBasePlayer *pOwner = ToBasePlayer( GetOwner() );
	
	if ( pOwner == NULL )
		return;

	//oh my fucking god
	if (sk_weapon_railgun_overcharge_limit.GetInt() > 0 && pOwner->GetAmmoCount(m_iPrimaryAmmoType) > sk_weapon_railgun_overcharge_limit.GetInt())
	{
		ExplosionCreate(pOwner->EyePosition(), pOwner->GetAbsAngles(), pOwner, 150, 128, 0, true);
		pOwner->Weapon_Detach(this);
		engine->ClientCommand(pOwner->edict(), "lastinv");
		engine->ClientCommand(pOwner->edict(), "-attack2");
		pOwner->SetAmmoCount(sk_weapon_railgun_overcharge_limit.GetInt(), m_iPrimaryAmmoType);
		UTIL_Remove(this);
		return;
	}

	/*Vector	startPos = pOwner->Weapon_ShootPosition();
	Vector	aimDir;

	Vector vecUp, vecRight, vecForward;

	pOwner->EyeVectors(&aimDir);

	Vector	endPos	= startPos + ( aimDir * MAX_TRACE_LENGTH );*/

	Vector	startPos = pOwner->Weapon_ShootPosition();
	Vector	aimDir = pOwner->GetAutoaimVector(AUTOAIM_5DEGREES);

	Vector vecUp, vecRight;
	VectorVectors(aimDir, vecRight, vecUp);

	float x, y, z;

	//Gassian spread
	do {
		x = 0.25;
		y = 0.25;
		z = x * x + y * y;
	} while (z > 1);

	aimDir = aimDir + x * GetBulletSpread().x * vecRight + y * GetBulletSpread().y * vecUp;

	Vector	endPos = startPos + (aimDir * MAX_TRACE_LENGTH);
	
	//Shoot a shot straight out
	trace_t	tr;
	UTIL_TraceLine( startPos, endPos, MASK_SHOT, pOwner, COLLISION_GROUP_NONE, &tr );

	CAmmoDef *def = GetAmmoDef();
	int definedDamage = def->PlrDamage(m_iPrimaryAmmoType);
	// rgettman - https://stackoverflow.com/questions/18407634/rounding-up-to-the-nearest-hundred
	int rounded = ((pOwner->GetAmmoCount(m_iPrimaryAmmoType) + 99) / 100 ) * 100;
	int iDamage = (m_bOverchargeDamageBenefits ? (int)(definedDamage * (rounded / 100)) : definedDamage);
	DevMsg("%i\n", iDamage);

	FireBulletsInfo_t info(1, startPos, aimDir, vec3_origin, MAX_TRACE_LENGTH, m_iPrimaryAmmoType);
	info.m_pAttacker = pOwner;
	info.m_iPlayerDamage = info.m_flDamage = iDamage;
	info.m_flDamageForceScale = 0.2f;

	// Fire the bullets, and force the first shot to be perfectly accurate
	pOwner->FireBullets(info);
	
	float hitAngle = -DotProduct(tr.plane.normal, aimDir);

	Vector vReflection;

	vReflection = 2.0 * tr.plane.normal * hitAngle + aimDir;

	startPos = tr.endpos;
	endPos = startPos + (vReflection * MAX_TRACE_LENGTH);

	//Kick up an effect
	if (!(tr.surface.flags & SURF_SKY))
	{
		UTIL_ImpactTrace(&tr, m_iPrimaryAmmoType, "ImpactJeep");

		//Do a gauss explosion
		CPVSFilter filter(tr.endpos);
		te->GaussExplosion(filter, 0.0f, tr.endpos, tr.plane.normal, 0);
	}

	//Draw beam to reflection point
	DrawBeam(tr.startpos, tr.endpos);

	// Register a muzzleflash for the AI
	pOwner->SetMuzzleFlashTime( gpGlobals->curtime + 0.5 );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CWeaponRailgun::DrawBeam(const Vector& startPos, const Vector& endPos)
{
	float flWidth = (m_bOverchargeDamageBenefits ? 4.5f : 2.0f);

	//Draw the main beam shaft
	m_pBeam = CBeam::BeamCreate(GAUSS_BEAM_SPRITE, flWidth);

	m_pBeam->SetStartPos(startPos);
	m_pBeam->PointEntInit(endPos, this);
	m_pBeam->SetEndAttachment(LookupAttachment("muzzle"));

	m_pBeam->SetColor(196, 47 + random->RandomInt(-16, 16), 250);
	m_pBeam->SetScrollRate(25.6);
	m_pBeam->SetBrightness(m_bIsLowBattery ? 128 : 255);
	m_pBeam->RelinkBeam();
	m_pBeam->LiveForTime(0.1f);
}

void CWeaponRailgun::RechargeAmmo(bool bIsHolstered)
{
	//if (m_flNextPrimaryAttack >= gpGlobals->curtime)
		//return;

	// Time to recharge yet?
	if (m_flNextCharge >= gpGlobals->curtime)
		return;

	CBasePlayer* pPlayer = ToBasePlayer(GetOwner());

	if (pPlayer == NULL)
		return;

	pPlayer->GiveAmmo(1, m_iPrimaryAmmoType, true);

	if (!bIsHolstered)
	{
		m_flNextCharge = gpGlobals->curtime + (m_bJustOvercharged ? RAIL_RECHARGE_OVERCHARGE_TIME : RAIL_RECHARGE_TIME);

		if ((pPlayer->GetAmmoCount(m_iPrimaryAmmoType) % 25) == 0 || pPlayer->GetAmmoCount(m_iPrimaryAmmoType) == 99)
		{
			WeaponSound(SPECIAL1);
		}
	}
	else
	{
		m_flNextCharge = gpGlobals->curtime + (m_bJustOvercharged ? RAIL_RECHARGE_OVERCHARGE_TIME : RAIL_RECHARGE_BACKGROUND_TIME);

		if (pPlayer->GetAmmoCount(m_iPrimaryAmmoType) == 99)
		{
			WeaponSound(SPECIAL1);
		}
	}

	if (m_bIsLowBattery && (pPlayer->GetAmmoCount(m_iPrimaryAmmoType) > RAIL_AMMO_OVERCHARGE))
	{
		m_bIsLowBattery = false;
	}

	if (m_bOverchargeDamageBenefits && pPlayer->GetAmmoCount(m_iPrimaryAmmoType) <= GetDefaultClip1())
	{
		m_bOverchargeDamageBenefits = false;
	}

	if (m_bJustOvercharged && pPlayer->GetAmmoCount(m_iPrimaryAmmoType) == GetDefaultClip1())
	{
		m_bJustOvercharged = false;
	}
}

void CWeaponRailgun::DechargeAmmo()
{
	//if (m_flNextPrimaryAttack >= gpGlobals->curtime)
		//return;

	// Time to recharge yet?
	if (m_flNextDecharge >= gpGlobals->curtime)
		return;

	CBasePlayer* pPlayer = ToBasePlayer(GetOwner());

	if (pPlayer == NULL)
		return;

	if (sk_weapon_railgun_overcharge_limit.GetInt() <= 0)
		return;

	if (pPlayer->GetAmmoCount(m_iPrimaryAmmoType) > sk_weapon_railgun_overcharge_limit.GetInt())
	{
		if (!m_bPlayedDechargingSound)
		{
			CPASAttenuationFilter filter(this, "SuitRecharge.ChargingLoop");
			filter.MakeReliable();
			EmitSound(filter, entindex(), "SuitRecharge.ChargingLoop");
			m_bPlayedDechargingSound = true;
		}

		pPlayer->RemoveAmmo(25, m_iPrimaryAmmoType);
		pPlayer->IncrementArmorValue(25);
		m_flNextDecharge = gpGlobals->curtime + RAIL_DECHARGE_TIME;
		m_bJustRecovered = true;
	}
}

void CWeaponRailgun::ChargeAmmo()
{
	if (m_flNextSuitCharge >= gpGlobals->curtime)
		return;

	CBasePlayer* pPlayer = ToBasePlayer(GetOwner());

	if (pPlayer == NULL)
		return;

	if (pPlayer->ArmorValue() <= 0 || pPlayer->GetAmmoCount(m_iPrimaryAmmoType) >= GetAmmoDef()->MaxCarry(m_iPrimaryAmmoType))
	{
		if (m_bPlayedChargingSound)
		{
			StopSound("SuitRecharge.ChargingLoop");
			m_bPlayedChargingSound = false;
		}
		return;
	}

	if (!m_bPlayedChargingSound)
	{
		CPASAttenuationFilter filter(this, "SuitRecharge.ChargingLoop");
		filter.MakeReliable();
		EmitSound(filter, entindex(), "SuitRecharge.ChargingLoop");
		m_bPlayedChargingSound = true;
	}

	// allow us to overcharge with as much battery as we have. WHO NEEDS SAFETY?
	pPlayer->GiveAmmo(5, m_iPrimaryAmmoType, true);
	pPlayer->RemoveArmor(5);

	m_flNextSuitCharge = gpGlobals->curtime + RAIL_RECHARGE_SUIT_TIME;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CWeaponRailgun::PrimaryAttack(void)
{
	if (!(m_flNextPrimaryAttack <= gpGlobals->curtime))
		return;

	CBasePlayer* pOwner = ToBasePlayer(GetOwner());

	if (pOwner == NULL)
		return;

	if (pOwner->GetAmmoCount(m_iPrimaryAmmoType) < RAIL_AMMO)
	{
		SendWeaponAnim(ACT_VM_DRYFIRE);
		WeaponSound(EMPTY);
		m_flNextPrimaryAttack = gpGlobals->curtime + 0.5f;
		return;
	}

	WeaponSound(SINGLE);

	pOwner->DoMuzzleFlash();
	pOwner->ViewPunch(QAngle(-4, random->RandomFloat(-2, 2), 0));

	int iMinAmmoToUse = (m_bOverchargeDamageBenefits ? RAIL_AMMO_OVERCHARGE : RAIL_AMMO);
	pOwner->RemoveAmmo(iMinAmmoToUse, m_iPrimaryAmmoType);

	m_flNextPrimaryAttack = gpGlobals->curtime + GetFireRate();

	Fire();

	if (!IsIronsighted())
	{
		SendWeaponAnim(ACT_VM_PRIMARYATTACK);
	}
	else
	{
		SendWeaponAnim(ACT_SHOTGUN_PUMP);
	}

	if (!m_iClip1 && pOwner->GetAmmoCount(m_iPrimaryAmmoType) <= 0)
	{
		// HEV suit - indicate out of ammo condition
		pOwner->SetSuitUpdate("!HEV_AMO0", FALSE, 0);
	}
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CWeaponRailgun::SecondaryAttack(void)
{
	//NOTENOTE: The zooming is handled by the post/busy frames
}

//-----------------------------------------------------------------------------
const WeaponProficiencyInfo_t* CWeaponRailgun::GetProficiencyValues()
{
	static WeaponProficiencyInfo_t proficiencyTable[] =
	{
		{ 7.0,		0.75	},
		{ 5.00,		0.75	},
		{ 3.0,		0.85	},
		{ 5.0 / 3.0,	0.75	},
		{ 1.00,		1.0		},
	};

	COMPILE_TIME_ASSERT(ARRAYSIZE(proficiencyTable) == WEAPON_PROFICIENCY_PERFECT + 1);

	return proficiencyTable;
}
