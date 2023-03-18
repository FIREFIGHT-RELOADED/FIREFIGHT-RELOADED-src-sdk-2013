//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:		Knife - an old favorite
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
#include "weapon_knife.h"
#include "rumble_shared.h"
#include "gamestats.h"
#include "cleanup_manager.h"
#include "physics_prop_ragdoll.h"
#include "effect_dispatch_data.h"
#include "te_effect_dispatch.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"


#define BLUDGEON_HULL_DIM		16

static const Vector g_bludgeonMins(-BLUDGEON_HULL_DIM, -BLUDGEON_HULL_DIM, -BLUDGEON_HULL_DIM);
static const Vector g_bludgeonMaxs(BLUDGEON_HULL_DIM, BLUDGEON_HULL_DIM, BLUDGEON_HULL_DIM);

ConVar    sk_plr_dmg_knife		( "sk_plr_dmg_knife","0");
ConVar    sk_npc_dmg_knife		( "sk_npc_dmg_knife","0");
ConVar    sk_plr_dmg_knife_thrown("sk_plr_dmg_knife_thrown", "0");
ConVar    sk_npc_dmg_knife_thrown("sk_npc_dmg_knife_thrown", "0");

//-----------------------------------------------------------------------------
// CWeaponKnife
//-----------------------------------------------------------------------------

IMPLEMENT_SERVERCLASS_ST(CWeaponKnife, DT_WeaponKnife)
SendPropEHandle(SENDINFO(m_hStuckRagdoll))
END_SEND_TABLE()

#ifndef HL2MP
LINK_ENTITY_TO_CLASS( weapon_knife, CWeaponKnife );
PRECACHE_WEAPON_REGISTER( weapon_knife );
#endif

acttable_t CWeaponKnife::m_acttable[] = 
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

IMPLEMENT_ACTTABLE(CWeaponKnife);

//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------
CWeaponKnife::CWeaponKnife( void )
{
}

bool CWeaponKnife::Deploy(void)
{
	bool deployVal = BaseClass::Deploy();

	CBasePlayer* pOwner = ToBasePlayer(GetOwner());

	if (pOwner != NULL)
	{
		// Can't shoot again until we've finished deploying
		pOwner->SetNextAttack(gpGlobals->curtime + SequenceDuration());
		m_flNextPrimaryAttack = gpGlobals->curtime + SequenceDuration();
		m_flNextSecondaryAttack = gpGlobals->curtime + SequenceDuration();
	}

	return deployVal;
}

#define THROWNKNIFE_AIR_VELOCITY	2500
#define THROWNKNIFE_WATER_VELOCITY	1500
void CWeaponKnife::ThrowKnife(void)
{
	CBasePlayer* pOwner = ToBasePlayer(GetOwner());

	if (pOwner == NULL)
		return;

	Vector vecAiming = pOwner->GetAutoaimVector(0);
	Vector vecSrc = pOwner->Weapon_ShootPosition();

	QAngle angAiming;
	VectorAngles(vecAiming, angAiming);

	// Create a new entity with CCrossbowBolt private data
	CBaseEntity* pBolt = CreateEntityByName("knife_bolt");
	if (pBolt)
	{
		UTIL_SetOrigin(pBolt, vecSrc);
		pBolt->SetAbsAngles(angAiming);
		pBolt->Spawn();
		pBolt->SetOwnerEntity(pOwner);

		if (pOwner->GetWaterLevel() == 3)
		{
			pBolt->SetAbsVelocity(vecAiming * THROWNKNIFE_WATER_VELOCITY);
		}
		else
		{
			pBolt->SetAbsVelocity(vecAiming * THROWNKNIFE_AIR_VELOCITY);
		}
	}

	WeaponSound(WPN_DOUBLE);
	CSoundEnt::InsertSound(SOUND_COMBAT, GetAbsOrigin(), 200, 0.2);

	SendWeaponAnim(ACT_VM_SECONDARYATTACK);

	// Send the player 'attack' animation.
	pOwner->SetAnimation(PLAYER_ATTACK1);

	AddEffects(EF_NODRAW);
	m_flNextSecondaryAttack = gpGlobals->curtime + KNIFE_REFIRE_THROW;
}

ConVar sv_infinite_knives( "sv_infinite_knives", "0", FCVAR_CHEAT );
void CWeaponKnife::SecondaryAttack(void)
{
	CBasePlayer* pPlayer = ToBasePlayer(GetOwner());
	if (pPlayer)
	{
		m_iSecondaryAttacks++;
		gamestats->Event_WeaponFired(pPlayer, true, GetClassname());
		ThrowKnife();
		if ( !sv_infinite_knives.GetBool() )
		{
			pPlayer->Weapon_Detach( this );
			engine->ClientCommand( pPlayer->edict(), "lastinv" );
			engine->ClientCommand( pPlayer->edict(), "-attack2" );
			UTIL_Remove( this );
		}
	}
}

void CWeaponKnife::ItemPostFrame(void)
{
	if (m_flNextSecondaryAttack <= gpGlobals->curtime)
	{
		RemoveEffects(EF_NODRAW);
	}

	BaseClass::ItemPostFrame();
}

//-----------------------------------------------------------------------------
// Purpose: Get the damage amount for the animation we're doing
// Input  : hitActivity - currently played activity
// Output : Damage amount
//-----------------------------------------------------------------------------
float CWeaponKnife::GetDamageForActivity( Activity hitActivity )
{
	if ((GetOwner() != NULL) && (GetOwner()->IsPlayer()))
		return sk_plr_dmg_knife.GetFloat();

	return sk_npc_dmg_knife.GetFloat();
}

//------------------------------------------------------------------------------
// Purpose: Implement impact function
//------------------------------------------------------------------------------
void CWeaponKnife::Hit(trace_t &traceHit, Activity nHitActivity, bool bIsSecondary)
{
	CBasePlayer *pPlayer = ToBasePlayer(GetOwner());

	//Do view kick
	AddViewKick();

	//Make sound for the AI
	CSoundEnt::InsertSound(SOUND_BULLET_IMPACT, traceHit.endpos, 400, 0.2f, pPlayer);

	// This isn't great, but it's something for when the crowbar hits.
	pPlayer->RumbleEffect(RUMBLE_AR2, 0, RUMBLE_FLAG_RESTART);

	CBaseEntity	*pHitEntity = traceHit.m_pEnt;

	//Apply damage to a hit target
	if (pHitEntity != NULL)
	{
		Vector hitDirection;
		pPlayer->EyeVectors(&hitDirection, NULL, NULL);
		VectorNormalize(hitDirection);

		CTakeDamageInfo info(GetOwner(), GetOwner(), GetDamageForActivity(nHitActivity), DMG_CLUB);

		if (pPlayer && pHitEntity->IsNPC())
		{
			// If bonking an NPC, adjust damage.
			if (FClassnameIs(pHitEntity, "npc_headcrab") || FClassnameIs(pHitEntity, "npc_headcrab_fast") || FClassnameIs(pHitEntity, "npc_headcrab_poison"))
			{
				info.CopyDamageToBaseDamage();
				info.SetDamage(pHitEntity->GetHealth());
			}
			else
			{
				info.AdjustPlayerDamageInflictedForSkillLevel();
			}
		}

		CalculateMeleeDamageForce(&info, hitDirection, traceHit.endpos);

		pHitEntity->DispatchTraceAttack(info, hitDirection, &traceHit);
		ApplyMultiDamage();

		// Now hit all triggers along the ray that... 
		TraceAttackToTriggers(info, traceHit.startpos, traceHit.endpos, hitDirection);

		if (ToBaseCombatCharacter(pHitEntity))
		{
			gamestats->Event_WeaponHit(pPlayer, !bIsSecondary, GetClassname(), info);
		}
	}

	// Apply an impact effect
	ImpactEffect(traceHit);
}

//-----------------------------------------------------------------------------
// Purpose: Add in a view kick for this weapon
//-----------------------------------------------------------------------------
void CWeaponKnife::AddViewKick( void )
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
// Attempt to lead the target (needed because citizens can't hit manhacks with the knife!)
//-----------------------------------------------------------------------------
ConVar sk_knife_lead_time( "sk_knife_lead_time", "0.9" );

int CWeaponKnife::WeaponMeleeAttack1Condition( float flDot, float flDist )
{
	// Attempt to lead the target (needed because citizens can't hit manhacks with the knife!)
	CAI_BaseNPC *pNPC	= GetOwner()->MyNPCPointer();
	CBaseEntity *pEnemy = pNPC->GetEnemy();
	if (!pEnemy)
		return COND_NONE;

	Vector vecVelocity;
	vecVelocity = pEnemy->GetSmoothedVelocity( );

	// Project where the enemy will be in a little while
	float dt = sk_knife_lead_time.GetFloat();
	dt += random->RandomFloat( -0.3f, 0.2f );
	if ( dt < 0.0f )
		dt = 0.0f;

	Vector vecExtrapolatedPos;
	VectorMA( pEnemy->WorldSpaceCenter(), dt, vecVelocity, vecExtrapolatedPos );

	Vector vecDelta;
	VectorSubtract( vecExtrapolatedPos, pNPC->WorldSpaceCenter(), vecDelta );

	if ( fabs( vecDelta.z ) > 70 )
	{
		return COND_TOO_FAR_TO_ATTACK;
	}

	Vector vecForward = pNPC->BodyDirection2D( );
	vecDelta.z = 0.0f;
	float flExtrapolatedDist = Vector2DNormalize( vecDelta.AsVector2D() );
	if ((flDist > 64) && (flExtrapolatedDist > 64))
	{
		return COND_TOO_FAR_TO_ATTACK;
	}

	float flExtrapolatedDot = DotProduct2D( vecDelta.AsVector2D(), vecForward.AsVector2D() );
	if ((flDot < 0.7) && (flExtrapolatedDot < 0.7))
	{
		return COND_NOT_FACING_ATTACK;
	}

	return COND_CAN_MELEE_ATTACK1;
}


//-----------------------------------------------------------------------------
// Animation event handlers
//-----------------------------------------------------------------------------
void CWeaponKnife::HandleAnimEventMeleeHit( animevent_t *pEvent, CBaseCombatCharacter *pOperator )
{
	// Trace up or down based on where the enemy is...
	// But only if we're basically facing that direction
	Vector vecDirection;
	AngleVectors( GetAbsAngles(), &vecDirection );

	CBaseEntity *pEnemy = pOperator->MyNPCPointer() ? pOperator->MyNPCPointer()->GetEnemy() : NULL;
	if ( pEnemy )
	{
		Vector vecDelta;
		VectorSubtract( pEnemy->WorldSpaceCenter(), pOperator->Weapon_ShootPosition(), vecDelta );
		VectorNormalize( vecDelta );
		
		Vector2D vecDelta2D = vecDelta.AsVector2D();
		Vector2DNormalize( vecDelta2D );
		if ( DotProduct2D( vecDelta2D, vecDirection.AsVector2D() ) > 0.8f )
		{
			vecDirection = vecDelta;
		}
	}

	Vector vecEnd;
	VectorMA( pOperator->Weapon_ShootPosition(), 50, vecDirection, vecEnd );
	CBaseEntity *pHurt = pOperator->CheckTraceHullAttack( pOperator->Weapon_ShootPosition(), vecEnd, 
		Vector(-16,-16,-16), Vector(36,36,36), sk_npc_dmg_knife.GetFloat(), DMG_CLUB, 0.75 );
	
	// did I hit someone?
	if ( pHurt )
	{
		// play sound
		WeaponSound( MELEE_HIT );

		// Fake a trace impact, so the effects work out like a player's crowbaw
		trace_t traceHit;
		UTIL_TraceLine( pOperator->Weapon_ShootPosition(), pHurt->GetAbsOrigin(), MASK_SHOT_HULL, pOperator, COLLISION_GROUP_NONE, &traceHit );
		ImpactEffect( traceHit );
	}
	else
	{
		WeaponSound( MELEE_MISS );
	}
}


//-----------------------------------------------------------------------------
// Animation event
//-----------------------------------------------------------------------------
void CWeaponKnife::Operator_HandleAnimEvent( animevent_t *pEvent, CBaseCombatCharacter *pOperator )
{
	switch( pEvent->event )
	{
	case EVENT_WEAPON_MELEE_HIT:
		HandleAnimEventMeleeHit( pEvent, pOperator );
		break;

	default:
		BaseClass::Operator_HandleAnimEvent( pEvent, pOperator );
		break;
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CWeaponKnife::ImpactEffect(trace_t &traceHit)
{
	// See if we hit water (we don't do the other impact effects in this case)
	if (ImpactWater(traceHit.startpos, traceHit.endpos))
		return;

	//FIXME: need new decals
	UTIL_ImpactTrace(&traceHit, DMG_SLASH);
	//UTIL_DecalTrace(&traceHit, "ManhackCut");
}

void CWeaponKnife::DislodgeRagdoll()
{
	auto ragdoll = dynamic_cast<CBaseAnimating*>(m_hStuckRagdoll.Get());
	if ( ragdoll != nullptr && ragdoll->IsRagdoll() )
	{
		// This is an attempt to handle server-side ragdolls, but it doesn't seem
		// to work now.
		CRagdollProp* newRagdoll = (CRagdollProp*)CreateEntityByName( "prop_ragdoll" );
		if ( newRagdoll != nullptr )
		{
			newRagdoll->CopyAnimationDataFrom( ragdoll );
			newRagdoll->Spawn();
			ragdoll->Remove();
		}
	}

	CEffectData data;
	data.m_nEntIndex = entindex();
	DispatchEffect( "KnifeDislodge", data );
}

int CWeaponKnife::OnTakeDamage( const CTakeDamageInfo& info )
{
	auto phys = VPhysicsGetObject();
	if ( IsEffectActive( EF_ITEM_BLINK ) && phys != NULL && !phys->IsMotionEnabled() )
	{
		Vector forward;
		AngleVectors( GetAbsAngles(), &forward );
		phys->EnableMotion( true );
		phys->Wake();
		forward *= Clamp( info.GetDamage() + 100, 100.0f, 250.0f );
		phys->SetVelocity( &forward, NULL );
		DislodgeRagdoll();
		return 0;
	}
	else
		return BaseClass::OnTakeDamage( info );
}

void CWeaponKnife::Equip( CBaseCombatCharacter *pOwner )
{
	DislodgeRagdoll();
	CCleanupManager::RemoveThrownKnife( this );
	BaseClass::Equip( pOwner );
}

void CWeaponKnife::UpdateOnRemove()
{
	CCleanupManager::RemoveThrownKnife( this );
	BaseClass::UpdateOnRemove();
}