//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:		357 - hand gun
//
// $NoKeywords: $
//=============================================================================//

#include "cbase.h"
#include "npcevent.h"
#include "basehlcombatweapon.h"
#include "basecombatcharacter.h"
#include "ai_basenpc.h"
#include "player.h"
#include "gamerules.h"
#include "in_buttons.h"
#include "soundent.h"
#include "game.h"
#include "vstdlib/random.h"
#include "engine/IEngineSound.h"
#include "te_effect_dispatch.h"
#include "gamestats.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

#define	DEAGLE_FASTEST_REFIRE_TIME		0.1f

//-----------------------------------------------------------------------------
// CWeaponDeagle
//-----------------------------------------------------------------------------

class CWeaponDeagle : public CBaseHLCombatWeapon
{
	DECLARE_CLASS( CWeaponDeagle, CBaseHLCombatWeapon );
public:

	CWeaponDeagle( void );

	void	PrimaryAttack( void );
	void	ItemPostFrame(void);
	void	Operator_HandleAnimEvent( animevent_t *pEvent, CBaseCombatCharacter *pOperator );

	float	WeaponAutoAimScale()	{ return 0.6f; }
	
	virtual const Vector& GetBulletSpread( void )
	{
		static const Vector cone = VECTOR_CONE_3DEGREES;
		static const Vector ironsightCone = VECTOR_CONE_1DEGREES;
		if (IsIronsighted())
		{
			return ironsightCone;
		}
		
		return cone;
	}

	DECLARE_SERVERCLASS();
	DECLARE_DATADESC();
	DECLARE_ACTTABLE();

private:
	float	m_flSoonestPrimaryAttack;
};

acttable_t CWeaponDeagle::m_acttable[] =
{
	{ ACT_IDLE,							ACT_IDLE_PISTOL,						true },
	{ ACT_IDLE_ANGRY,					ACT_IDLE_ANGRY_PISTOL,					true },
	{ ACT_RANGE_ATTACK1,				ACT_RANGE_ATTACK_PISTOL,				true },
	{ ACT_RELOAD,						ACT_RELOAD_PISTOL,						true },
	{ ACT_WALK_AIM,						ACT_WALK_AIM_PISTOL,					true },
	{ ACT_RUN_AIM,						ACT_RUN_AIM_PISTOL,						true },
	{ ACT_GESTURE_RANGE_ATTACK1,		ACT_GESTURE_RANGE_ATTACK_PISTOL,		true },
	{ ACT_RELOAD_LOW,					ACT_RELOAD_PISTOL_LOW,					false },
	{ ACT_RANGE_ATTACK1_LOW,			ACT_RANGE_ATTACK_PISTOL_LOW,			false },
	{ ACT_COVER_LOW,					ACT_COVER_PISTOL_LOW,					false },
	{ ACT_RANGE_AIM_LOW,				ACT_RANGE_AIM_PISTOL_LOW,				false },
	{ ACT_GESTURE_RELOAD,				ACT_GESTURE_RELOAD_PISTOL,				false },
	{ ACT_WALK,							ACT_WALK_PISTOL,						false },
	{ ACT_RUN,							ACT_RUN_PISTOL,							false },
	{ ACT_HL2MP_IDLE,					ACT_HL2MP_IDLE_PISTOL,					false },
	{ ACT_HL2MP_RUN,					ACT_HL2MP_RUN_PISTOL,					false },
	{ ACT_HL2MP_IDLE_CROUCH,			ACT_HL2MP_IDLE_CROUCH_PISTOL,			false },
	{ ACT_HL2MP_WALK_CROUCH,			ACT_HL2MP_WALK_CROUCH_PISTOL,			false },
	{ ACT_HL2MP_GESTURE_RANGE_ATTACK,	ACT_HL2MP_GESTURE_RANGE_ATTACK_PISTOL,	false },
	{ ACT_HL2MP_GESTURE_RELOAD,			ACT_HL2MP_GESTURE_RELOAD_PISTOL,		false },
	{ ACT_HL2MP_JUMP,					ACT_HL2MP_JUMP_PISTOL,					false },
	{ ACT_RANGE_ATTACK1,				ACT_RANGE_ATTACK_PISTOL,				false },
};

IMPLEMENT_ACTTABLE(CWeaponDeagle);

LINK_ENTITY_TO_CLASS( weapon_deagle, CWeaponDeagle );

PRECACHE_WEAPON_REGISTER( weapon_deagle );

IMPLEMENT_SERVERCLASS_ST( CWeaponDeagle, DT_WeaponDeagle )
END_SEND_TABLE()

BEGIN_DATADESC( CWeaponDeagle )
	DEFINE_FIELD(m_flSoonestPrimaryAttack, FIELD_TIME),
END_DATADESC()

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CWeaponDeagle::CWeaponDeagle( void )
{
	m_bReloadsSingly	= false;
	m_bFiresUnderwater	= false;

	m_flSoonestPrimaryAttack = gpGlobals->curtime;
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CWeaponDeagle::Operator_HandleAnimEvent( animevent_t *pEvent, CBaseCombatCharacter *pOperator )
{
	switch( pEvent->event )
	{
		case EVENT_WEAPON_PISTOL_FIRE:
		{
			Vector vecShootOrigin, vecShootDir;
			vecShootOrigin = pOperator->Weapon_ShootPosition();

			CAI_BaseNPC *npc = pOperator->MyNPCPointer();
			ASSERT(npc != NULL);

			vecShootDir = npc->GetActualShootTrajectory(vecShootOrigin);

			CSoundEnt::InsertSound(SOUND_COMBAT | SOUND_CONTEXT_GUNFIRE, pOperator->GetAbsOrigin(), SOUNDENT_VOLUME_PISTOL, 0.2, pOperator, SOUNDENT_CHANNEL_WEAPON, pOperator->GetEnemy());

			WeaponSound(SINGLE_NPC);
			pOperator->FireBullets(1, vecShootOrigin, vecShootDir, VECTOR_CONE_PRECALCULATED, MAX_TRACE_LENGTH, m_iPrimaryAmmoType, 2);
			pOperator->DoMuzzleFlash();
			m_iClip1 = m_iClip1 - 1;
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

void CWeaponDeagle::PrimaryAttack( void )
{
	// Only the player fires this way so we can cast
	CBasePlayer *pPlayer = ToBasePlayer( GetOwner() );

	if ( !pPlayer )
	{
		return;
	}

	if ( m_iClip1 <= 0 )
	{
		if ( !m_bFireOnEmpty )
		{
			Reload();
		}
		else
		{
			WeaponSound( EMPTY );
			m_flNextPrimaryAttack = 0.15;
		}

		return;
	}

	m_flNextPrimaryAttack = gpGlobals->curtime + 0.75;
	m_flNextSecondaryAttack = gpGlobals->curtime + 0.75;

	m_iPrimaryAttacks++;
	gamestats->Event_WeaponFired( pPlayer, true, GetClassname() );

	WeaponSound( SINGLE );
	pPlayer->DoMuzzleFlash();

	SendWeaponAnim( ACT_VM_PRIMARYATTACK );
	pPlayer->SetAnimation( PLAYER_ATTACK1 );

	if (GetWpnData().m_bUseMuzzleSmoke)
	{
		DispatchParticleEffect("weapon_muzzle_smoke", PATTACH_POINT_FOLLOW, pPlayer->GetViewModel(), "muzzle", true);
	}

	m_flSoonestPrimaryAttack = gpGlobals->curtime + DEAGLE_FASTEST_REFIRE_TIME;

	if (!pPlayer->m_iPerkInfiniteAmmo == 1)
	{
		m_iClip1--;
	}

	Vector vecSrc		= pPlayer->Weapon_ShootPosition();
	Vector vecAiming	= pPlayer->GetAutoaimVector( AUTOAIM_SCALE_DEFAULT );	

	pPlayer->FireBullets( 1, vecSrc, vecAiming, vec3_origin, MAX_TRACE_LENGTH, m_iPrimaryAmmoType, 0 );

	pPlayer->SetMuzzleFlashTime( gpGlobals->curtime + 0.5 );

	//Disorient the player
	QAngle angles = pPlayer->GetLocalAngles();

	angles.x += random->RandomInt( -1, 1 );
	angles.y += random->RandomInt( -1, 1 );
	angles.z = 0;

	pPlayer->SnapEyeAngles( angles );

	pPlayer->ViewPunch( QAngle( -8, random->RandomFloat( -2, 2 ), 0 ) );

	CSoundEnt::InsertSound( SOUND_COMBAT, GetAbsOrigin(), 600, 0.2, GetOwner() );

	if ( !m_iClip1 && pPlayer->GetAmmoCount( m_iPrimaryAmmoType ) <= 0 )
	{
		// HEV suit - indicate out of ammo condition
		pPlayer->SetSuitUpdate( "!HEV_AMO0", FALSE, 0 ); 
	}
}

void CWeaponDeagle::ItemPostFrame(void)
{
	BaseClass::ItemPostFrame();

	if (m_bInReload)
		return;

	CBasePlayer *pOwner = ToBasePlayer(GetOwner());

	if (pOwner == NULL)
		return;

	//Allow a refire as fast as the player can click
	if (((pOwner->m_nButtons & IN_ATTACK) == false) && (m_flSoonestPrimaryAttack < gpGlobals->curtime))
	{
		m_flNextPrimaryAttack = gpGlobals->curtime - 0.1f;
	}
}
