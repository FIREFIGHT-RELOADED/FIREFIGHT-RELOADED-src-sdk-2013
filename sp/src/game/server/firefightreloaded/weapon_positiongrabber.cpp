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

//-----------------------------------------------------------------------------
// CWeaponPositionGrabber
//-----------------------------------------------------------------------------

class CWeaponPositionGrabber : public CBaseHLCombatWeapon
{
	DECLARE_CLASS( CWeaponPositionGrabber, CBaseHLCombatWeapon );
public:

	CWeaponPositionGrabber( void );

	void	PrimaryAttack( void );

	float	WeaponAutoAimScale()	{ return 0.0f; }

	DECLARE_SERVERCLASS();
	DECLARE_DATADESC();
	DECLARE_ACTTABLE();
};

acttable_t CWeaponPositionGrabber::m_acttable[] =
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

IMPLEMENT_ACTTABLE(CWeaponPositionGrabber);

LINK_ENTITY_TO_CLASS(weapon_positiongrabber, CWeaponPositionGrabber);

PRECACHE_WEAPON_REGISTER(weapon_positiongrabber);

IMPLEMENT_SERVERCLASS_ST( CWeaponPositionGrabber, DT_WeaponPositionGrabber )
END_SEND_TABLE()

BEGIN_DATADESC( CWeaponPositionGrabber )
END_DATADESC()

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CWeaponPositionGrabber::CWeaponPositionGrabber( void )
{
	m_bReloadsSingly	= false;
	m_bFiresUnderwater	= false;
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CWeaponPositionGrabber::PrimaryAttack( void )
{
	// Only the player fires this way so we can cast
	CBasePlayer *pPlayer = ToBasePlayer( GetOwner() );

	if ( !pPlayer )
	{
		return;
	}

	m_flNextPrimaryAttack = gpGlobals->curtime + 0.75;
	m_flNextSecondaryAttack = gpGlobals->curtime + 0.75;

	m_iPrimaryAttacks++;
	gamestats->Event_WeaponFired( pPlayer, true, GetClassname() );

	WeaponSound( SINGLE );

	SendWeaponAnim( ACT_VM_PRIMARYATTACK );
	pPlayer->SetAnimation( PLAYER_ATTACK1 );

	m_flNextPrimaryAttack = gpGlobals->curtime + 0.75;
	m_flNextSecondaryAttack = gpGlobals->curtime + 0.75;

	Vector vecAiming = pPlayer->GetAutoaimVector(0);
	Vector vecOrigin = pPlayer->GetAbsOrigin() + Vector(0, 0, 10);

	QAngle angAiming;
	VectorAngles(vecAiming, angAiming);

	Msg("Player eye angle XYZ Coords: X: %f Y: %f Z: %f\n", angAiming.x, angAiming.y, angAiming.z);
	Msg("Player position XYZ Coords: X: %f Y: %f Z: %f\n", vecOrigin.x, vecOrigin.y, vecOrigin.z);
}
