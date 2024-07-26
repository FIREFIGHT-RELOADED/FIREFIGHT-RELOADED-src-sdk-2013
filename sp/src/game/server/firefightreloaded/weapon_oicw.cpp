//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: "when in doubt, shoot someone" - Pepper Axebane
//
//=============================================================================//

#include "cbase.h"
#include "basehlcombatweapon.h"
#include "npcevent.h"
#include "basecombatcharacter.h"
#include "ai_basenpc.h"
#include "player.h"
#include "game.h"
#include "in_buttons.h"
#include "ai_memory.h"
#include "soundent.h"
#include "rumble_shared.h"
#include "gamestats.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"	

class CWeaponOICW : public CHLMachineGun
{
	DECLARE_CLASS(CWeaponOICW, CHLMachineGun);
public:

	CWeaponOICW(void);
	
	DECLARE_SERVERCLASS();
	
	void	PrimaryAttack(void);
	void	ItemPostFrame(void);
	int		GetMinBurst() { return 6; }
	int		GetMaxBurst() { return 9; }
	void	Precache(void);
	int		CapabilitiesGet(void) { return bits_CAP_WEAPON_RANGE_ATTACK1; }
	void	FireNPCPrimaryAttack(CBaseCombatCharacter* pOperator, Vector& vecShootOrigin, Vector& vecShootDir);
	void	Operator_HandleAnimEvent(animevent_t *pEvent, CBaseCombatCharacter *pOperator);
	void	Operator_ForceNPCFire(CBaseCombatCharacter* pOperator, bool bSecondary);
	float	GetFireRate(void);
	void AddViewKick(void);
	void DoMachineGunKick(CBasePlayer *pPlayer, float dampEasy, float maxVerticleKickAngle, float fireDurationTime, float slideLimitTime);
	Activity	GetPrimaryAttackActivity(void);
    
    bool	Holster(CBaseCombatWeapon* pSwitchingTo = NULL);
	bool	Reload( void );
    bool	Deploy(void);
	void	Drop(const Vector& velocity);

	float	WeaponAutoAimScale()	{ return 0.6f; }

	virtual const Vector& GetBulletSpread(void)
	{
		static Vector cone;

		if (GetOwner() && GetOwner()->IsPlayer())
		{
			if (m_bZoomed)
			{
				cone = VECTOR_CONE_1DEGREES;
			}
		}
		else
		{
			cone = VECTOR_CONE_4DEGREES;
		}

		return cone;
	}

	const WeaponProficiencyInfo_t* GetProficiencyValues();
	
protected:
	void				Zoom(void);

	bool			m_bZoomed;
	int				m_nShotsFired;
    
    float			m_flSoonestPrimaryAttack;
	
	DECLARE_DATADESC();
	DECLARE_ACTTABLE();
};

IMPLEMENT_SERVERCLASS_ST(CWeaponOICW, DT_WeaponOICW)
END_SEND_TABLE()

LINK_ENTITY_TO_CLASS( weapon_oicw, CWeaponOICW );
PRECACHE_WEAPON_REGISTER(weapon_oicw);

BEGIN_DATADESC( CWeaponOICW )
DEFINE_FIELD( m_bZoomed,	FIELD_BOOLEAN ),
DEFINE_FIELD( m_nShotsFired, FIELD_INTEGER),
DEFINE_FIELD( m_flSoonestPrimaryAttack, FIELD_TIME ),
END_DATADESC()

acttable_t	CWeaponOICW::m_acttable[] =
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
	{ ACT_HL2MP_IDLE,				ACT_HL2MP_IDLE_AR2,				false },
	{ ACT_HL2MP_RUN,				ACT_HL2MP_RUN_AR2,				false },
	{ ACT_HL2MP_IDLE_CROUCH,		ACT_HL2MP_IDLE_CROUCH_AR2,		false },
	{ ACT_HL2MP_WALK_CROUCH,		ACT_HL2MP_WALK_CROUCH_AR2,		false },
	{ ACT_HL2MP_GESTURE_RANGE_ATTACK,			ACT_HL2MP_GESTURE_RANGE_ATTACK_AR2,			false },
	{ ACT_HL2MP_GESTURE_RELOAD,		ACT_GESTURE_RELOAD_SMG1,		false },
	{ ACT_HL2MP_JUMP,				ACT_HL2MP_JUMP_AR2,				false },
	{ ACT_RANGE_ATTACK1,			ACT_RANGE_ATTACK_AR2,			false },
};

IMPLEMENT_ACTTABLE(CWeaponOICW);

//=========================================================
CWeaponOICW::CWeaponOICW( )
{
	m_fMinRange1		= 0;// No minimum range. 
	m_fMaxRange1		= 1400;
	
	m_nShotsFired = 0;

	m_bFiresUnderwater = false;
}

bool CWeaponOICW::Deploy(void)
{
	m_nShotsFired = 0;

	return BaseClass::Deploy();
}

void CWeaponOICW::Precache(void)
{
	BaseClass::Precache();
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CWeaponOICW::Operator_HandleAnimEvent(animevent_t *pEvent, CBaseCombatCharacter *pOperator)
{
	switch (pEvent->event)
	{
	case EVENT_WEAPON_AR2:
	{
		Vector vecShootOrigin, vecShootDir;
		QAngle angDiscard;

		// Support old style attachment point firing
		if ((pEvent->options == NULL) || (pEvent->options[0] == '\0') || (!pOperator->GetAttachment(pEvent->options, vecShootOrigin, angDiscard)))
		{
			vecShootOrigin = pOperator->Weapon_ShootPosition();
		}

		CAI_BaseNPC* npc = pOperator->MyNPCPointer();
		ASSERT(npc != NULL);
		vecShootDir = npc->GetActualShootTrajectory(vecShootOrigin);

		FireNPCPrimaryAttack(pOperator, vecShootOrigin, vecShootDir);
	}
	break;

	default:
		BaseClass::Operator_HandleAnimEvent(pEvent, pOperator);
		break;
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CWeaponOICW::FireNPCPrimaryAttack(CBaseCombatCharacter* pOperator, Vector& vecShootOrigin, Vector& vecShootDir)
{
	// FIXME: use the returned number of bullets to account for >10hz firerate
	WeaponSoundRealtime(SINGLE);

	CSoundEnt::InsertSound(SOUND_COMBAT | SOUND_CONTEXT_GUNFIRE, pOperator->GetAbsOrigin(), SOUNDENT_VOLUME_MACHINEGUN, 0.2, pOperator, SOUNDENT_CHANNEL_WEAPON, pOperator->GetEnemy());
	pOperator->FireBullets(1, vecShootOrigin, vecShootDir, VECTOR_CONE_PRECALCULATED,
		MAX_TRACE_LENGTH, m_iPrimaryAmmoType, 2, entindex(), 0);

	pOperator->DoMuzzleFlash();
	m_iClip1 = m_iClip1 - 1;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CWeaponOICW::Operator_ForceNPCFire(CBaseCombatCharacter* pOperator, bool bSecondary)
{
	// Ensure we have enough rounds in the clip
	m_iClip1++;

	Vector vecShootOrigin, vecShootDir;
	QAngle	angShootDir;
	GetAttachment(LookupAttachment("muzzle"), vecShootOrigin, angShootDir);
	AngleVectors(angShootDir, &vecShootDir);
	FireNPCPrimaryAttack(pOperator, vecShootOrigin, vecShootDir);
}

//-----------------------------------------------------------------------------
// Purpose: 
// Output : Activity
//-----------------------------------------------------------------------------
Activity CWeaponOICW::GetPrimaryAttackActivity(void)
{
	int iRandomActivity = RandomInt(0,2);

	switch (iRandomActivity)
	{
		case 0:
			return ACT_VM_PRIMARYATTACK;
		case 1:
			return ACT_VM_RECOIL1;
		case 2:
			return ACT_VM_RECOIL2;
	}

	return ACT_VM_PRIMARYATTACK;
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------

void CWeaponOICW::PrimaryAttack(void)
{
	CBasePlayer* pOwner = ToBasePlayer(GetOwner());
	if (!pOwner)
		return;

	if (m_bFireOnEmpty)
	{
		return;
	}

	if (m_flNextPrimaryAttack > gpGlobals->curtime)
		return;

	// Only the player fires this way so we can cast
	CBasePlayer *pPlayer = ToBasePlayer(GetOwner());
	if (!pPlayer)
		return;

	// Abort here to handle burst and auto fire modes
	if ((UsesClipsForAmmo1() && m_iClip1 == 0) || (!UsesClipsForAmmo1() && !pPlayer->GetAmmoCount(m_iPrimaryAmmoType)))
		return;

	m_nShotsFired++;

	pPlayer->DoMuzzleFlash();

	// To make the firing framerate independent, we may have to fire more than one bullet here on low-framerate systems, 
	// especially if the weapon we're firing has a really fast rate of fire.
	int iBulletsToFire = 0;
	float fireRate = GetFireRate();

	while (m_flNextPrimaryAttack <= gpGlobals->curtime)
	{
		// MUST call sound before removing a round from the clip of a CHLMachineGun
		WeaponSound(SINGLE, m_flNextPrimaryAttack);
		m_flNextPrimaryAttack = m_flNextPrimaryAttack + fireRate;
		iBulletsToFire++;
	}

	// Make sure we don't fire more than the amount in the clip, if this weapon uses clips
	if (UsesClipsForAmmo1())
	{
		if (iBulletsToFire > m_iClip1)
			iBulletsToFire = m_iClip1;

		m_iClip1 -= iBulletsToFire;
	}

	m_iPrimaryAttacks++;
	gamestats->Event_WeaponFired(pPlayer, true, GetClassname());

	// Fire the bullets
	FireBulletsInfo_t info;
	info.m_iShots = iBulletsToFire;
	info.m_vecSrc = pPlayer->Weapon_ShootPosition();
	info.m_vecDirShooting = pPlayer->GetAutoaimVector(AUTOAIM_SCALE_DEFAULT);
	info.m_vecSpread = pPlayer->GetAttackSpread(this);
	info.m_flDistance = MAX_TRACE_LENGTH;
	info.m_iAmmoType = m_iPrimaryAmmoType;
	info.m_iTracerFreq = 2;
	FireBullets(info);

	//Factor in the view kick
	AddViewKick();

	CSoundEnt::InsertSound(SOUND_COMBAT, GetAbsOrigin(), SOUNDENT_VOLUME_MACHINEGUN, 0.2, pPlayer);

	if (!m_iClip1 && pPlayer->GetAmmoCount(m_iPrimaryAmmoType) <= 0)
	{
		// HEV suit - indicate out of ammo condition
		pPlayer->SetSuitUpdate("!HEV_AMO0", FALSE, 0);
	}

	SendWeaponAnim(GetPrimaryAttackActivity());
	pPlayer->SetAnimation(PLAYER_ATTACK1);

	if (GetWpnData().m_bUseMuzzleSmoke)
	{
		DispatchParticleEffect("weapon_muzzle_smoke", PATTACH_POINT_FOLLOW, pPlayer->GetViewModel(), "muzzle", true);
	}
    
    // Register a muzzleflash for the AI
	pPlayer->SetMuzzleFlashTime(gpGlobals->curtime + 0.5);
}

//-----------------------------------------------------------------------------
// Purpose: Handle grenade detonate in-air (even when no ammo is left)
//-----------------------------------------------------------------------------
void CWeaponOICW::ItemPostFrame(void)
{
	CBasePlayer *pOwner = ToBasePlayer(GetOwner());
	if (!pOwner)
		return;

	if ((pOwner->m_nButtons & IN_ATTACK) == false)
	{
		m_nShotsFired = 0;
	}
    
    if (pOwner->m_afButtonPressed & IN_ATTACK2)
    {
        Zoom();
    }
    
    //Allow a refire as fast as the player can click
    if (m_bZoomed && ((pOwner->m_nButtons & IN_ATTACK) == false) && (m_flSoonestPrimaryAttack < gpGlobals->curtime))
    {
        m_flNextPrimaryAttack = gpGlobals->curtime - 0.1f;
    }

    //Don't kick the same when we're zoomed in
    if (m_bZoomed)
    {
        m_fFireDuration = 0.05f;
    }
	
	BaseClass::ItemPostFrame();
}

void CWeaponOICW::Zoom(void)
{
	CBasePlayer* pPlayer = ToBasePlayer(GetOwner());

	if (pPlayer == NULL)
		return;

	if (IsIronsighted())
		return;

	color32 lightGreen = { 50, 255, 170, 32 };

	if (m_bZoomed)
	{
		if (pPlayer->SetFOV(this, 0, 0.1f))
		{
			pPlayer->ShowViewModel(true);
            
            WeaponSound(SPECIAL2);
            
			m_bZoomed = false;

			UTIL_ScreenFade(pPlayer, lightGreen, 0.2f, 0, (FFADE_IN | FFADE_PURGE));
		}
	}
	else
	{
		if (pPlayer->SetFOV(this, 35, 0.1f))
		{
			pPlayer->ShowViewModel(false);
            
            WeaponSound(SPECIAL1);
            
			m_bZoomed = true;

			UTIL_ScreenFade(pPlayer, lightGreen, 0.2f, 0, (FFADE_OUT | FFADE_PURGE | FFADE_STAYOUT));
		}
	}
}

float CWeaponOICW::GetFireRate(void)
{
	if (m_bZoomed)
		return 0.4f;

	return 0.085f;
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : NULL - 
//-----------------------------------------------------------------------------
bool CWeaponOICW::Holster(CBaseCombatWeapon* pSwitchingTo)
{
	if (m_bZoomed)
	{
		Zoom();
	}

	return BaseClass::Holster(pSwitchingTo);
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CWeaponOICW::Drop(const Vector& velocity)
{
	if (m_bZoomed)
	{
		Zoom();
	}

	BaseClass::Drop(velocity);
}

//-----------------------------------------------------------------------------
// Purpose: Override if we're waiting to release a shot
//-----------------------------------------------------------------------------
bool CWeaponOICW::Reload( void )
{
	if (m_bZoomed)
	{
		Zoom();
	}
    
	return BaseClass::Reload();
}

void CWeaponOICW::AddViewKick(void)
{
	#define	EASY_DAMPEN			0.5f
	#define	MAX_VERTICAL_KICK	1.2f	//Degrees
	#define	SLIDE_LIMIT			2.0f	//Seconds

	//Get the view kick
	CBasePlayer *pPlayer = ToBasePlayer(GetOwner());

	if (pPlayer == NULL)
		return;

	DoMachineGunKick(pPlayer, EASY_DAMPEN, MAX_VERTICAL_KICK, m_fFireDuration, SLIDE_LIMIT);
}

void CWeaponOICW::DoMachineGunKick(CBasePlayer *pPlayer, float dampEasy, float maxVerticleKickAngle, float fireDurationTime, float slideLimitTime)
{
	#define	KICK_MIN_X			0.2f	//Degrees
	#define	KICK_MIN_Y			0.2f	//Degrees
	#define	KICK_MIN_Z			0.1f	//Degrees

	QAngle vecScratch;

	//Find how far into our accuracy degradation we are
	float duration = (fireDurationTime > slideLimitTime) ? slideLimitTime : fireDurationTime;
	float kickPerc = duration / slideLimitTime;

	// do this to get a hard discontinuity, clear out anything under 10 degrees punch
	pPlayer->ViewPunchReset(10);

	//Apply this to the view angles as well
	vecScratch.x = -(KICK_MIN_X + (maxVerticleKickAngle * kickPerc));
	vecScratch.y = -(KICK_MIN_Y + (maxVerticleKickAngle * kickPerc)) / 3;
	vecScratch.z = KICK_MIN_Z + (maxVerticleKickAngle * kickPerc) / 8;

	//Wibble left and right
	if (random->RandomInt(-1, 1) >= 0)
		vecScratch.y *= -1;

	//Wobble up and down
	if (random->RandomInt(-1, 1) >= 0)
		vecScratch.z *= -1;

	//If we're in easy, dampen the effect a bit
	if (g_pGameRules->IsSkillLevel(SKILL_EASY))
	{
		for (int i = 0; i < 3; i++)
		{
			vecScratch[i] *= dampEasy;
		}
	}

	//Clip this to our desired min/max
	UTIL_ClipPunchAngleOffset(vecScratch, pPlayer->m_Local.m_vecPunchAngle, QAngle(24.0f, 3.0f, 1.0f));

	//Add it to the view punch
	// NOTE: 0.5 is just tuned to match the old effect before the punch became simulated
	pPlayer->ViewPunch(vecScratch * 0.5);
}

const WeaponProficiencyInfo_t* CWeaponOICW::GetProficiencyValues()
{
	static WeaponProficiencyInfo_t proficiencyTable[] =
	{
		{ 7.0,		0.75	},
		{ 5.00,		0.75	},
		{ 10.0 / 3.0, 0.75	},
		{ 5.0 / 3.0,	0.75	},
		{ 1.00,		1.0		},
	};

	COMPILE_TIME_ASSERT(ARRAYSIZE(proficiencyTable) == WEAPON_PROFICIENCY_PERFECT + 1);

	return proficiencyTable;
}