//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:		Sniper Rifle
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

enum ZoomType
{
	ZOOM_NONE,
	ZOOM_1X,
	ZOOM_2X,
	ZOOM_3X,

	ZOOM_LAST
};

//-----------------------------------------------------------------------------
// CWeaponSniperRifle
//-----------------------------------------------------------------------------

class CWeaponSniperRifle : public CBaseHLCombatWeapon
{
	DECLARE_CLASS( CWeaponSniperRifle, CBaseHLCombatWeapon );
public:

	CWeaponSniperRifle( void );

	void	PrimaryAttack( void );
	void	SecondaryAttack( void );
	void	ItemPostFrame( void );
	void	ItemBusyFrame( void );
	void	Operator_HandleAnimEvent( animevent_t *pEvent, CBaseCombatCharacter *pOperator );
	int		CapabilitiesGet(void) { return bits_CAP_WEAPON_RANGE_ATTACK1; }
	bool	Reload(void);
	void	Drop(const Vector &vecVelocity);
	bool	Holster(CBaseCombatWeapon *pSwitchingTo = NULL);
	
	virtual const Vector& GetBulletSpread( void )
	{
		static const Vector cone = VECTOR_CONE_4DEGREES;
		static const Vector npccone = VECTOR_CONE_1DEGREES;
		static const Vector zoomcone = VECTOR_CONE_1DEGREES;
		if (m_iZoomMode > 0)
		{
			return zoomcone;
		}

		if (GetOwner() && GetOwner()->IsNPC())
		{
			return npccone;
		}

		return cone;
	}

	float	WeaponAutoAimScale()	{ return 0.6f; }

	DECLARE_SERVERCLASS();
	DECLARE_DATADESC();
	DECLARE_ACTTABLE();
	
private:
	void	CheckZoomToggle( void );
	void	ToggleZoom( void );
	
private:
	int		m_iZoomMode;
};

acttable_t CWeaponSniperRifle::m_acttable[] =
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

IMPLEMENT_ACTTABLE(CWeaponSniperRifle);

LINK_ENTITY_TO_CLASS( weapon_sniper_rifle, CWeaponSniperRifle );

PRECACHE_WEAPON_REGISTER( weapon_sniper_rifle );

IMPLEMENT_SERVERCLASS_ST( CWeaponSniperRifle, DT_WeaponSniperRifle )
END_SEND_TABLE()

BEGIN_DATADESC( CWeaponSniperRifle )
	DEFINE_FIELD(m_iZoomMode,		FIELD_INTEGER ),
END_DATADESC()

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CWeaponSniperRifle::CWeaponSniperRifle( void )
{
	m_bReloadsSingly	= false;
	m_bFiresUnderwater	= false;
	m_iZoomMode = 0;

	m_fMinRange1 = 0;
	m_fMaxRange1 = 99999;
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CWeaponSniperRifle::Operator_HandleAnimEvent( animevent_t *pEvent, CBaseCombatCharacter *pOperator )
{
	switch( pEvent->event )
	{
		case EVENT_WEAPON_RELOAD:
		{
			CBasePlayer *pOwner = ToBasePlayer(GetOwner());
			CEffectData data;

			for (int i = 0; i < 2; i++)
			{
				data.m_vOrigin = pOwner->WorldSpaceCenter() + RandomVector(-4, 4);
				data.m_vAngles = QAngle(90, random->RandomInt(0, 360), 0);
				data.m_nEntIndex = entindex();

				DispatchEffect("ShellEject", data);
			}
		}
		break;

		case EVENT_WEAPON_AR2:
		{
			Vector vecShootOrigin, vecShootDir;
			QAngle	angShootDir;

			CAI_BaseNPC* npc = pOperator->MyNPCPointer();
			ASSERT(npc != NULL);

			vecShootOrigin = pOperator->Weapon_ShootPosition();
			vecShootDir = npc->GetActualShootTrajectory(vecShootOrigin);

			WeaponSound(SINGLE);

			CSoundEnt::InsertSound(SOUND_COMBAT | SOUND_CONTEXT_GUNFIRE, pOperator->GetAbsOrigin(), SOUNDENT_VOLUME_MACHINEGUN, 0.2, pOperator, SOUNDENT_CHANNEL_WEAPON, pOperator->GetEnemy());

			//shoot 2 to make it seem we're shooting super fast.
			pOperator->FireBullets(1, vecShootOrigin, vecShootDir, GetBulletSpread(), MAX_TRACE_LENGTH, m_iPrimaryAmmoType, 2);

			// NOTENOTE: This is overriden on the client-side
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

void CWeaponSniperRifle::PrimaryAttack( void )
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

	m_flNextPrimaryAttack = gpGlobals->curtime + 0.85;

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

	m_iClip1--;

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

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CWeaponSniperRifle::SecondaryAttack( void )
{
	//NOTENOTE: The zooming is handled by the post/busy frames
}

//-----------------------------------------------------------------------------
// Purpose: 
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
bool CWeaponSniperRifle::Reload(void)
{
	if (m_iZoomMode > 0)
	{
		m_iZoomMode = ZOOM_LAST - 1;
		ToggleZoom();
	}

	return BaseClass::Reload();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CWeaponSniperRifle::CheckZoomToggle( void )
{
	CBasePlayer *pPlayer = ToBasePlayer( GetOwner() );
	
	if (!IsIronsighted())
	{
		if (pPlayer->m_afButtonPressed & IN_ATTACK2)
		{
			ToggleZoom();
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CWeaponSniperRifle::ItemBusyFrame( void )
{
	// Allow zoom toggling even when we're reloading
	CheckZoomToggle();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CWeaponSniperRifle::ItemPostFrame( void )
{
	// Allow zoom toggling
	CheckZoomToggle();

	BaseClass::ItemPostFrame();
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : *pSwitchingTo - 
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
bool CWeaponSniperRifle::Holster(CBaseCombatWeapon *pSwitchingTo)
{
	if (m_iZoomMode > 0)
	{
		m_iZoomMode = ZOOM_LAST - 1;
		ToggleZoom();
	}

	return BaseClass::Holster(pSwitchingTo);
}

void CWeaponSniperRifle::Drop(const Vector &vecVelocity)
{
	if (m_iZoomMode > 0)
	{
		m_iZoomMode = ZOOM_LAST - 1;
		ToggleZoom();
	}

	BaseClass::Drop(vecVelocity);
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CWeaponSniperRifle::ToggleZoom( void )
{
	CBasePlayer *pPlayer = ToBasePlayer( GetOwner() );
	
	if ( pPlayer == NULL )
		return;

	CBaseViewModel* vm = pPlayer->GetViewModel();

	if (vm == NULL)
		return;

	if (IsIronsighted())
		return;

	switch (m_iZoomMode)
	{
		case ZOOM_3X:
		default:
			if (pPlayer->SetFOV(this, 0, 0.2f))
			{
				vm->RemoveEffects(EF_NODRAW);
				WeaponSound(SPECIAL2);
				m_iZoomMode = ZOOM_NONE;
			}
			break;
		case ZOOM_2X:
			if (pPlayer->SetFOV(this, 5, 0.05f))
			{
				vm->AddEffects(EF_NODRAW);
				WeaponSound(SPECIAL1);
				m_iZoomMode = ZOOM_3X;
			}
			break;
		case ZOOM_1X:
			if (pPlayer->SetFOV(this, 10, 0.05f))
			{
				vm->AddEffects(EF_NODRAW);
				WeaponSound(SPECIAL1);
				m_iZoomMode = ZOOM_2X;
			}
			break;
		case ZOOM_NONE:
			if (pPlayer->SetFOV(this, 20, 0.05f))
			{
				vm->AddEffects(EF_NODRAW);
				WeaponSound(SPECIAL1);
				m_iZoomMode = ZOOM_1X;
			}
			break;
	}
}
