//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: Implements the grapple hook weapon.
//			
//			Primary attack: fires a beam that hooks on a surface.
//			Secondary attack: switches between pull and rapple modes
//
//
//=============================================================================//

#include "cbase.h"
#include "weapon_grapple.h"   
#include "npcevent.h"
#include "in_buttons.h"                           
#include "game.h"                           
#include "player.h"                 
#include "te_effect_dispatch.h"
#include "IEffects.h"
#include "SpriteTrail.h"
#include "beam_shared.h"
#include "explode.h"
#include "ai_basenpc.h"

#include "ammodef.h"		/* This is needed for the tracing done later */
#include "gamestats.h" //
#include "soundent.h" //

#include "vphysics/constraints.h"
#include "physics_saverestore.h"
#include "hl2_player.h"
 
// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

#define BEAM_SPRITE "sprites/orangelight1.vmt"
#define GLOW_SPRITE "sprites/orangeflare1.vmt"

ConVar sk_grapple_delay("sk_grapple_delay", "0.5");

static const char* ppszIgnoredClasses[] =
{
	"prop_dynamic_override",
	"prop_static"
};
 
LINK_ENTITY_TO_CLASS( grapple_hook, CGrappleHook );
 
BEGIN_DATADESC( CGrappleHook )
	DEFINE_FIELD( m_hPlayer, FIELD_EHANDLE ),
	DEFINE_FIELD( m_hOwner, FIELD_EHANDLE ),
	DEFINE_FIELD( m_hBolt, FIELD_EHANDLE ),
	DEFINE_FIELD( m_bPlayerWasStanding, FIELD_BOOLEAN ),

	DEFINE_ENTITYFUNC( HookTouch ),

	DEFINE_THINKFUNC( FlyThink ),
	DEFINE_THINKFUNC( HookedThink ),
END_DATADESC()
 
CGrappleHook *CGrappleHook::HookCreate( const Vector &vecOrigin, const QAngle &angAngles, CBaseEntity *pentOwner )
{
	// Create a new entity with CGrappleHook private data
	CGrappleHook *pHook = (CGrappleHook *)CreateEntityByName( "grapple_hook" );
	UTIL_SetOrigin( pHook, vecOrigin );
	pHook->SetAbsAngles( angAngles );
	pHook->Spawn();
 
	CWeaponGrapple *pOwner = (CWeaponGrapple *)pentOwner;
	pHook->m_hOwner = pOwner;
	pHook->SetOwnerEntity( pOwner->GetOwner() );
	pHook->m_hPlayer = (CBasePlayer *)pOwner->GetOwner();
 
	return pHook;
}
 
//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CGrappleHook::~CGrappleHook( void )
{ 
	if ( m_hBolt )
	{
		UTIL_Remove( m_hBolt );
		m_hBolt = NULL;
	}
 
	// Revert Jay's gai flag
	if ( m_hPlayer )
		m_hPlayer->SetPhysicsFlag( PFLAG_VPHYSICS_MOTIONCONTROLLER, false );
}
 
//-----------------------------------------------------------------------------
// Purpose: 
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
bool CGrappleHook::CreateVPhysics( void )
{
	// Create the object in the physics system
	VPhysicsInitNormal( SOLID_BBOX, FSOLID_NOT_STANDABLE, false );
 
	return true;
}
 
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
unsigned int CGrappleHook::PhysicsSolidMaskForEntity() const
{
	return ( BaseClass::PhysicsSolidMaskForEntity() | CONTENTS_HITBOX ) & ~CONTENTS_GRATE;
}
 
//-----------------------------------------------------------------------------
// Purpose: Spawn
//-----------------------------------------------------------------------------
void CGrappleHook::Spawn( void )
{
	Precache( );
 
	SetModel( HOOK_MODEL );
	SetMoveType( MOVETYPE_FLYGRAVITY, MOVECOLLIDE_FLY_CUSTOM );
	UTIL_SetSize( this, -Vector(1,1,1), Vector(1,1,1) );
	SetSolid( SOLID_BBOX );
	SetGravity( 0.05f );
 
	// The rock is invisible, the crossbow bolt is the visual representation
	AddEffects( EF_NODRAW );
 
	// Make sure we're updated if we're underwater
	UpdateWaterState();
 
	SetTouch( &CGrappleHook::HookTouch );
 
	SetThink( &CGrappleHook::FlyThink );
	SetNextThink( gpGlobals->curtime + 0.1f );

	m_bPlayerWasStanding = false;
}
 
void CGrappleHook::Precache( void )
{
	PrecacheModel( HOOK_MODEL );
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : *pOther - 
//-----------------------------------------------------------------------------
void CGrappleHook::HookTouch( CBaseEntity *pOther )
{
	if (!pOther || pOther->IsSolidFlagSet(FSOLID_NOT_SOLID | FSOLID_VOLUME_CONTENTS) || pOther->IsEffectActive(EF_NODRAW))
		return;
 
	EmitSound("Weapon_AR2.Reload_Push");

	// if what we hit is static architecture, can stay around for a while.
	Vector vecDir = GetAbsVelocity();

	//FIXME: We actually want to stick (with hierarchy) to what we've hit
	SetMoveType(MOVETYPE_NONE);

	Vector vForward;

	AngleVectors(GetAbsAngles(), &vForward);
	VectorNormalize(vForward);

	CEffectData	data;

	trace_t	tr;
	tr = BaseClass::GetTouchTrace();
	data.m_vOrigin = tr.endpos;
	data.m_vNormal = vForward;
	data.m_nEntIndex = 0;

	//	DispatchEffect( "Impact", data );

	//	AddEffects( EF_NODRAW );
	SetTouch(NULL);

	VPhysicsDestroyObject();
	VPhysicsInitNormal(SOLID_VPHYSICS, FSOLID_NOT_STANDABLE, false);
	AddSolidFlags(FSOLID_NOT_SOLID);
	//	SetMoveType( MOVETYPE_NONE );

	if (!m_hPlayer)
	{
		Assert(0);
		return;
	}

	// Set Jay's gai flag
	m_hPlayer->SetPhysicsFlag(PFLAG_VPHYSICS_MOTIONCONTROLLER, true);
	m_hPlayer->SetAnimation(PLAYER_JUMP);

	//IPhysicsObject *pPhysObject = m_hPlayer->VPhysicsGetObject();
	IPhysicsObject *pRootPhysObject = VPhysicsGetObject();
	Assert(pRootPhysObject);
	//Assert(pPhysObject);

	if (!pRootPhysObject)
	{
		Assert(0);
		return;
	}

	pRootPhysObject->EnableMotion(false);

	// Root has huge mass, tip has little
	pRootPhysObject->SetMass(VPHYSICS_MAX_MASS);
	//	pPhysObject->SetMass( 100 );
	//	float damping = 3;
	//	pPhysObject->SetDamping( &damping, &damping );

	m_bPlayerWasStanding = ((m_hPlayer->GetFlags() & FL_DUCKING) == 0);

	SetThink(&CGrappleHook::HookedThink);
	SetNextThink(gpGlobals->curtime);
}
 
//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CGrappleHook::HookedThink( void )
{
	//set next globalthink
	SetNextThink( gpGlobals->curtime); //0.1f

	//All of this push the player far from the hook
	Vector tempVec1 = m_hPlayer->GetAbsOrigin() - GetAbsOrigin();
	VectorNormalize(tempVec1);

	int temp_multiplier = -1;

	m_hPlayer->SetGravity(0.0f);
	m_hPlayer->SetGroundEntity(NULL);
	m_hPlayer->m_nWallRunState = WALLRUN_NOT;
	m_hPlayer->StopWallRunSound();
	m_hPlayer->DeriveMaxSpeed();
	m_hPlayer->m_flCoyoteTime = 0;
	m_hPlayer->m_Local.m_vecTargetPunchAngle.Set(ROLL, 0);
	m_hPlayer->m_vecLastWallRunPos = m_hPlayer->GetAbsOrigin();
	m_hPlayer->m_bIsPowerSliding = false;
	m_hPlayer->m_Local.m_flSlideTime = 0.0f;
	m_hPlayer->StopPowerSlideSound();

	float flDistance = (m_hPlayer->GetAbsOrigin() - GetAbsOrigin()).Length();

	if (flDistance < 128.0f)
	{
		SetTouch(NULL);
		SetThink(NULL);

		m_hOwner->NotifyHookDied();
		m_hPlayer->SelectLastItem();

		UTIL_Remove(this);
	}
	else
	{
		float velocity = 1350.0f;
		m_hPlayer->SetAbsVelocity(tempVec1 * temp_multiplier * velocity);//400
	}
}
 
//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CGrappleHook::FlyThink( void )
{
	QAngle angNewAngles;
 
	VectorAngles( GetAbsVelocity(), angNewAngles );
	SetAbsAngles( angNewAngles );
 
	SetNextThink( gpGlobals->curtime + 0.1f );
}

IMPLEMENT_SERVERCLASS_ST(CWeaponGrapple, DT_WeaponGrapple)
END_SEND_TABLE()
 
LINK_ENTITY_TO_CLASS( weapon_grapple, CWeaponGrapple );

BEGIN_DATADESC(CWeaponGrapple)
DEFINE_FIELD(m_hHook, FIELD_EHANDLE),
DEFINE_FIELD(m_pLightGlow, FIELD_EHANDLE),
DEFINE_FIELD(pBeam, FIELD_EHANDLE),
DEFINE_FIELD(m_bHook, FIELD_BOOLEAN),
END_DATADESC()
 
PRECACHE_WEAPON_REGISTER( weapon_grapple );
 
acttable_t	CWeaponGrapple::m_acttable[] = 
{
	{ ACT_HL2MP_IDLE, ACT_HL2MP_IDLE_PISTOL, false },
	{ ACT_HL2MP_RUN, ACT_HL2MP_RUN_PISTOL, false },
	{ ACT_HL2MP_IDLE_CROUCH, ACT_HL2MP_IDLE_CROUCH_PISTOL, false },
	{ ACT_HL2MP_WALK_CROUCH, ACT_HL2MP_WALK_CROUCH_PISTOL, false },
	{ ACT_HL2MP_GESTURE_RANGE_ATTACK, ACT_HL2MP_GESTURE_RANGE_ATTACK_PISTOL, false },
	{ ACT_HL2MP_GESTURE_RELOAD, ACT_HL2MP_GESTURE_RELOAD_PISTOL, false },
	{ ACT_HL2MP_JUMP, ACT_HL2MP_JUMP_PISTOL, false },
	{ ACT_RANGE_ATTACK1, ACT_RANGE_ATTACK_PISTOL, false },
};
 
IMPLEMENT_ACTTABLE(CWeaponGrapple);
 
//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CWeaponGrapple::CWeaponGrapple( void )
{
	m_bReloadsSingly	= true;
	m_bFiresUnderwater	= true;
	m_nBulletType = -1;
	m_pLightGlow= NULL;
	pBeam	= NULL;
}
 
//-----------------------------------------------------------------------------
// Purpose: Precache
//-----------------------------------------------------------------------------
void CWeaponGrapple::Precache( void )
{
	UTIL_PrecacheOther( "grapple_hook" );
 
//	PrecacheScriptSound( "Weapon_Crossbow.BoltHitBody" );
//	PrecacheScriptSound( "Weapon_Crossbow.BoltHitWorld" );
//	PrecacheScriptSound( "Weapon_Crossbow.BoltSkewer" );
 
	PrecacheModel(BEAM_SPRITE);
	PrecacheModel(GLOW_SPRITE);
 
	BaseClass::Precache();
}

bool CWeaponGrapple::Deploy(void)
{
	return BaseClass::Deploy();
}

bool CWeaponGrapple::CanDeploy(void)
{
	if (m_flNextPrimaryAttack < gpGlobals->curtime)
		return true;

	EmitSound("Weapon_SMG1.Empty");
	return false;
}
 
//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CWeaponGrapple::PrimaryAttack( void )
{
	// Can't have an active hook out
	if ( m_hHook != NULL )
		return;

	CBasePlayer *pPlayer = ToBasePlayer( GetOwner() );

	if ( !pPlayer )
	{
		return;
	}

	m_iPrimaryAttacks++;
	gamestats->Event_WeaponFired( pPlayer, true, GetClassname() );

	WeaponSound( SINGLE );

	SendWeaponAnim( ACT_VM_PRIMARYATTACK );
	pPlayer->SetAnimation( PLAYER_ATTACK1 );

	Vector vecSrc		= pPlayer->Weapon_ShootPosition();
	Vector vecAiming	= pPlayer->GetAutoaimVector( AUTOAIM_SCALE_DEFAULT );	

	CSoundEnt::InsertSound( SOUND_COMBAT, GetAbsOrigin(), 600, 0.2, GetOwner() );
	
	trace_t tr;
	Vector vecShootOrigin, vecShootDir, vecDir, vecEnd;

	//Gets the direction where the player is aiming
	AngleVectors (pPlayer->EyeAngles(), &vecDir);

	//Gets the position of the player
	vecShootOrigin = pPlayer->Weapon_ShootPosition();

	//Gets the position where the hook will hit
	vecEnd	= vecShootOrigin + (vecDir * MAX_TRACE_LENGTH);	
	
	//Traces a line between the two vectors
	UTIL_TraceLine( vecShootOrigin, vecEnd, MASK_SHOT, pPlayer, COLLISION_GROUP_NONE, &tr);

	CBaseEntity* pOther = tr.m_pEnt;

	if (!pOther || pOther->IsSolidFlagSet(FSOLID_NOT_SOLID | FSOLID_VOLUME_CONTENTS) || pOther->IsEffectActive(EF_NODRAW))
	{
		WeaponSound(EMPTY);
		return;
	}

	bool foundIgnoredProp = false;

	for (int i = 0; i < ARRAYSIZE(ppszIgnoredClasses); i++)
	{
		if (!pOther->ClassMatches(ppszIgnoredClasses[i]))
		{
			continue;
		}
		else
		{
			foundIgnoredProp = true;
			break;
		}
	}

	if (foundIgnoredProp)
	{
		WeaponSound(EMPTY);
		return;
	}

	if (tr.surface.flags & SURF_SKY)
	{
		WeaponSound(EMPTY);
		return;
	}

	//Draws the beam
	DrawBeam( vecShootOrigin, tr.endpos, 5 );

	CPVSFilter filter( tr.endpos );
	te->GaussExplosion( filter, 0.0f, tr.endpos, tr.plane.normal, 0 );
	m_nBulletType = GetAmmoDef()->Index("GaussEnergy");
	UTIL_ImpactTrace( &tr, m_nBulletType );

	//Makes a sprite at the end of the beam
	m_pLightGlow = CSprite::SpriteCreate(GLOW_SPRITE, GetAbsOrigin(), TRUE);

	//Sets FX render and color
	m_pLightGlow->SetTransparency( 9, 255, 255, 255, 200, kRenderFxNoDissipation );
		
	//Sets the position
	m_pLightGlow->SetAbsOrigin(tr.endpos);
		
	//Bright
	m_pLightGlow->SetBrightness( 255 );
		
	//Scale
	m_pLightGlow->SetScale( 0.65 );

	FireHook(tr.endpos);
}
 
//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CWeaponGrapple::ItemPostFrame( void )
{
	//Enforces being able to use PrimaryAttack and Secondary Attack
	CBasePlayer *pOwner = ToBasePlayer( GetOwner() );
 
	if ((pOwner->m_nButtons & IN_ATTACK) || (pOwner->m_afButtonPressed & IN_GRAPPLE))
	{
		if (m_flNextPrimaryAttack < gpGlobals->curtime)
		{
			PrimaryAttack();
			if (pOwner->m_afButtonPressed & IN_ATTACK)
			{
				SetWeaponIdleTime(gpGlobals->curtime + SequenceDuration(ACT_VM_PRIMARYATTACK));
			}
		}
	}

	if ( m_hHook )
	{
		if (!(pOwner->m_nButtons & IN_ATTACK) && !(pOwner->m_nButtons & IN_GRAPPLE))
		{
			m_hHook->SetTouch( NULL );
			m_hHook->SetThink( NULL );
 
			UTIL_Remove( m_hHook );
			m_hHook = NULL;
 
			NotifyHookDied();
 
			//Redraw the weapon
			SendWeaponAnim( ACT_VM_IDLE ); //ACT_VM_RELOAD
 
			//Update our times
			m_flNextPrimaryAttack = gpGlobals->curtime + sk_grapple_delay.GetFloat();
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: Fires the hook
//-----------------------------------------------------------------------------
void CWeaponGrapple::FireHook(const Vector& endPos)
{
	CBasePlayer *pOwner = ToBasePlayer( GetOwner() );
 
	if ( pOwner == NULL )
		return;

	Vector vecAiming	= pOwner->GetAutoaimVector( 0 );	
	Vector vecSrc		= pOwner->Weapon_ShootPosition();
 
	QAngle angAiming;
	VectorAngles( vecAiming, angAiming );
 
	CGrappleHook *pHook = CGrappleHook::HookCreate(endPos, angAiming, this );
 
	if ( pOwner->GetWaterLevel() == 3 )
	{
		pHook->SetAbsVelocity( vecAiming * BOLT_WATER_VELOCITY );
	}
	else
	{
		pHook->SetAbsVelocity( vecAiming * BOLT_AIR_VELOCITY );
	}
 
	m_hHook = pHook;
 
	pOwner->ViewPunch( QAngle( -2, 0, 0 ) );
 
	SendWeaponAnim( ACT_VM_PRIMARYATTACK );
}
 
//-----------------------------------------------------------------------------
// Purpose: 
// Input  : *pSwitchingTo - 
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
bool CWeaponGrapple::Holster( CBaseCombatWeapon *pSwitchingTo )
{
	if ( m_hHook )
	{
		m_hHook->SetTouch( NULL );
		m_hHook->SetThink( NULL );
 
		UTIL_Remove( m_hHook );
		m_hHook = NULL;
 
		NotifyHookDied();
	}
 
	return BaseClass::Holster( pSwitchingTo );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CWeaponGrapple::Drop( const Vector &vecVelocity )
{
	if ( m_hHook )
	{
		m_hHook->SetTouch( NULL );
		m_hHook->SetThink( NULL );
 
		UTIL_Remove( m_hHook );
		m_hHook = NULL;
 
		NotifyHookDied();
	}

	BaseClass::Drop( vecVelocity );
}
 
//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
bool CWeaponGrapple::HasAnyAmmo( void )
{
	if ( m_hHook != NULL )
		return true;
 
	return BaseClass::HasAnyAmmo();
}
 
//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
bool CWeaponGrapple::CanHolster( void )
{
	//Can't have an active hook out
	if ( m_hHook != NULL )
		return false;
 
	return BaseClass::CanHolster();
}
 
//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CWeaponGrapple::NotifyHookDied( void )
{
	m_hHook = NULL;

	if ( pBeam )
	{
		UTIL_Remove( pBeam ); //Kill beam
		pBeam = NULL;

		UTIL_Remove( m_pLightGlow ); //Kill sprite
		m_pLightGlow = NULL;

		SendWeaponAnim( ACT_VM_IDLE );
	}
}

//-----------------------------------------------------------------------------
// Purpose: Draws a beam
// Input  : &startPos - where the beam should begin
//          &endPos - where the beam should end
//          width - what the diameter of the beam should be (units?)
//-----------------------------------------------------------------------------
void CWeaponGrapple::DrawBeam( const Vector &startPos, const Vector &endPos, float width )
{
	//Tracer down the middle (NOT NEEDED, IT WILL FIRE A TRACER)
	//UTIL_Tracer( startPos, endPos, 0, TRACER_DONT_USE_ATTACHMENT, 6500, false, "GaussTracer" );
 
	trace_t tr;
	//Draw the main beam shaft
	pBeam = CBeam::BeamCreate(BEAM_SPRITE, width);

	// It starts at startPos
	pBeam->SetStartPos( startPos );
 
	// This sets up some things that the beam uses to figure out where
	// it should start and end
	pBeam->PointEntInit( endPos, this );
 
	// This makes it so that the beam appears to come from the muzzle of the pistol
	pBeam->SetEndAttachment( LookupAttachment("Muzzle") );
	pBeam->SetWidth( width );
//	pBeam->SetEndWidth( 0.05f );
 
	// Higher brightness means less transparent
	pBeam->SetBrightness( 255 );
	//pBeam->SetColor( 255, 185+random->RandomInt( -16, 16 ), 40 );
	pBeam->RelinkBeam();

	//Sets scrollrate of the beam sprite 
	float scrollOffset = gpGlobals->curtime + 5.5;
	pBeam->SetScrollRate(scrollOffset);
 
	// The beam should only exist for a very short time
	//pBeam->LiveForTime( 0.1f );

	UpdateWaterState();
}
//-----------------------------------------------------------------------------
// Purpose: 
// Input  : &tr - used to figure out where to do the effect
//          nDamageType - ???
//-----------------------------------------------------------------------------
void CWeaponGrapple::DoImpactEffect( trace_t &tr, int nDamageType )
{
	if ( (tr.surface.flags & SURF_SKY) == false )
	{
		CPVSFilter filter( tr.endpos );
		te->GaussExplosion( filter, 0.0f, tr.endpos, tr.plane.normal, 0 );
		m_nBulletType = GetAmmoDef()->Index("GaussEnergy");
		UTIL_ImpactTrace( &tr, m_nBulletType );
	}
}