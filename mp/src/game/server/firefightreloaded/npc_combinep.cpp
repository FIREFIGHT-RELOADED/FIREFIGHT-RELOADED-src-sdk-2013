//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: This is the soldier version of the combine, analogous to the HL1 grunt.
//
//=============================================================================//

#include "cbase.h"
#include "ai_hull.h"
#include "ai_motor.h"
#include "npc_combinep.h"
#include "bitstring.h"
#include "engine/IEngineSound.h"
#include "soundent.h"
#include "ndebugoverlay.h"
#include "npcevent.h"
#include "hl2/hl2_player.h"
#include "game.h"
#include "ammodef.h"
#include "explode.h"
#include "ai_memory.h"
#include "Sprite.h"
#include "soundenvelope.h"
#include "weapon_physcannon.h"
#include "hl2_gamerules.h"
#include "gameweaponmanager.h"
#include "vehicle_base.h"
#include "gib.h"
#include "particle_parse.h"
#include "multiplay_gamerules.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

ConVar	sk_combine_p_health( "sk_combine_p_health","50");
ConVar	sk_combine_p_kick( "sk_combine_p_kick","10");

extern ConVar sk_plr_dmg_buckshot;	
extern ConVar sk_plr_num_shotgun_pellets;

//Whether or not the combine should spawn health on death
ConVar	combine_p_spawn_health( "combine_p_spawn_health", "1" );

ConVar	combine_p_spawnwithgrenades("combine_p_spawnwithgrenades", "1", FCVAR_ARCHIVE);

LINK_ENTITY_TO_CLASS( npc_combine_p, CNPC_CombineP );


#define AE_SOLDIER_BLOCK_PHYSICS		20 // trying to block an incoming physics object

extern Activity ACT_WALK_EASY;
extern Activity ACT_WALK_MARCH;

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CNPC_CombineP::Spawn( void )
{
	Precache();
	SetModel( "models/combine_soldier_prisonguard.mdl" );

	m_iNumGrenades = random->RandomInt(2, 3);

	m_fIsElite = false;
	m_fIsAce = false;
	m_iUseMarch = true;

	SetHealth( sk_combine_p_health.GetFloat() );
	SetMaxHealth( sk_combine_p_health.GetFloat() );
	SetKickDamage( sk_combine_p_kick.GetFloat() );

	CapabilitiesAdd( bits_CAP_ANIMATEDFACE );
	CapabilitiesAdd( bits_CAP_MOVE_SHOOT );
	CapabilitiesAdd( bits_CAP_DOORS_GROUP );

	BaseClass::Spawn();
	/*
#if HL2_EPISODIC
	if (m_iUseMarch && !HasSpawnFlags(SF_NPC_START_EFFICIENT))
	{
		Msg( "Soldier %s is set to use march anim, but is not an efficient AI. The blended march anim can only be used for dead-ahead walks!\n", GetDebugName() );
	}
#endif
	*/
}

//-----------------------------------------------------------------------------
// Purpose:
// Input  :
// Output :
//-----------------------------------------------------------------------------
void CNPC_CombineP::Precache()
{
	PrecacheModel( "models/combine_soldier_prisonguard.mdl" );
	PrecacheModel("models/gibs/combine_prisonguard_beheaded.mdl");
	PrecacheModel("models/gibs/pgib_p3.mdl");
	PrecacheModel("models/gibs/pgib_p4.mdl");

	PrecacheModel("models/gibs/soldier_prisonguard_head.mdl");
	PrecacheModel("models/gibs/soldier_prisonguard_left_arm.mdl");
	PrecacheModel("models/gibs/soldier_prisonguard_right_arm.mdl");
	PrecacheModel("models/gibs/soldier_prisonguard_torso.mdl");
	PrecacheModel("models/gibs/soldier_prisonguard_pelvis.mdl");
	PrecacheModel("models/gibs/soldier_prisonguard_left_leg.mdl");
	PrecacheModel("models/gibs/soldier_prisonguard_right_leg.mdl");

	PrecacheParticleSystem("headshotspray");

	UTIL_PrecacheOther( "item_healthvial" );
	UTIL_PrecacheOther( "weapon_frag" );

	PrecacheScriptSound("Gore.Headshot");

	BaseClass::Precache();
}


void CNPC_CombineP::DeathSound( const CTakeDamageInfo &info )
{
	// NOTE: The response system deals with this at the moment
	if ( GetFlags() & FL_DISSOLVING )
		return;

	if (IsOnFire())
		return;

	GetSentences()->Speak( "COMBINE_DIE", SENTENCE_PRIORITY_INVALID, SENTENCE_CRITERIA_ALWAYS ); 
}


//-----------------------------------------------------------------------------
// Purpose: Soldiers use CAN_RANGE_ATTACK2 to indicate whether they can throw
//			a grenade. Because they check only every half-second or so, this
//			condition must persist until it is updated again by the code
//			that determines whether a grenade can be thrown, so prevent the 
//			base class from clearing it out. (sjb)
//-----------------------------------------------------------------------------
void CNPC_CombineP::ClearAttackConditions( )
{
	bool fCanRangeAttack2 = HasCondition( COND_CAN_RANGE_ATTACK2 );

	// Call the base class.
	BaseClass::ClearAttackConditions();

	if( fCanRangeAttack2 )
	{
		// We don't allow the base class to clear this condition because we
		// don't sense for it every frame.
		SetCondition( COND_CAN_RANGE_ATTACK2 );
	}
}

void CNPC_CombineP::PrescheduleThink( void )
{
	/*//FIXME: This doesn't need to be in here, it's all debug info
	if( HasCondition( COND_HEAR_PHYSICS_DANGER ) )
	{
		// Don't react unless we see the item!!
		CSound *pSound = NULL;

		pSound = GetLoudestSoundOfType( SOUND_PHYSICS_DANGER );

		if( pSound )
		{
			if( FInViewCone( pSound->GetSoundReactOrigin() ) )
			{
				DevMsg( "OH CRAP!\n" );
				NDebugOverlay::Line( EyePosition(), pSound->GetSoundReactOrigin(), 0, 0, 255, false, 2.0f );
			}
		}
	}
	*/

	BaseClass::PrescheduleThink();
}

//-----------------------------------------------------------------------------
// Purpose: Allows for modification of the interrupt mask for the current schedule.
//			In the most cases the base implementation should be called first.
//-----------------------------------------------------------------------------
void CNPC_CombineP::BuildScheduleTestBits( void )
{
	//Interrupt any schedule with physics danger (as long as I'm not moving or already trying to block)
	if ( m_flGroundSpeed == 0.0 && !IsCurSchedule( SCHED_FLINCH_PHYSICS ) )
	{
		SetCustomInterruptCondition( COND_HEAR_PHYSICS_DANGER );
	}

	BaseClass::BuildScheduleTestBits();
}

//-----------------------------------------------------------------------------
// Purpose:
// Input  :
// Output :
//-----------------------------------------------------------------------------
int CNPC_CombineP::SelectSchedule ( void )
{
	return BaseClass::SelectSchedule();
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
float CNPC_CombineP::GetHitgroupDamageMultiplier( int iHitGroup, const CTakeDamageInfo &info )
{
	bool isNohead = false;
	switch (iHitGroup)
	{
	case HITGROUP_HEAD:
		int HeadshotRandom = random->RandomInt(0, 4);
		if (!(g_Language.GetInt() == LANGUAGE_GERMAN || UTIL_IsLowViolence()))
		{
			if (isNohead == false && HeadshotRandom == 0 && !(info.GetDamageType() & DMG_NEVERGIB) || isNohead == false && (info.GetDamageType() & (DMG_SNIPER | DMG_BUCKSHOT)) && !(info.GetDamageType() & DMG_NEVERGIB))
			{
				SetModel("models/gibs/combine_prisonguard_beheaded.mdl");
				DispatchParticleEffect("headshotspray", PATTACH_POINT_FOLLOW, this, "bloodspurt", true);
				SpawnBlood(GetAbsOrigin(), g_vecAttackDir, BloodColor(), info.GetDamage());
				CGib::SpawnSpecificGibs(this, 6, 750, 1500, "models/gibs/pgib_p3.mdl", 6);
				CGib::SpawnSpecificGibs(this, 6, 750, 1500, "models/gibs/pgib_p4.mdl", 6);
				EmitSound("Gore.Headshot");
				m_iHealth = 0;
				isNohead = true;
			}
			else
			{
				// Soldiers take double headshot damage
				return 2.0f;
			}
		}
		else
		{
			// Soldiers take double headshot damage
			return 2.0f;
		}
	}

	return BaseClass::GetHitgroupDamageMultiplier( iHitGroup, info );
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CNPC_CombineP::HandleAnimEvent( animevent_t *pEvent )
{
	switch( pEvent->event )
	{
	case AE_SOLDIER_BLOCK_PHYSICS:
		DevMsg( "BLOCKING!\n" );
		m_fIsBlocking = true;
		break;

	default:
		BaseClass::HandleAnimEvent( pEvent );
		break;
	}
}

void CNPC_CombineP::OnChangeActivity( Activity eNewActivity )
{
	// Any new sequence stops us blocking.
	m_fIsBlocking = false;

	BaseClass::OnChangeActivity( eNewActivity );

	if (m_iUseMarch)
	{
		SetPoseParameter("casual", 1.0);
	}
}

void CNPC_CombineP::OnListened()
{
	BaseClass::OnListened();

	if ( HasCondition( COND_HEAR_DANGER ) && HasCondition( COND_HEAR_PHYSICS_DANGER ) )
	{
		if ( HasInterruptCondition( COND_HEAR_DANGER ) )
		{
			ClearCondition( COND_HEAR_PHYSICS_DANGER );
		}
	}

	// debugging to find missed schedules
#if 0
	if ( HasCondition( COND_HEAR_DANGER ) && !HasInterruptCondition( COND_HEAR_DANGER ) )
	{
		DevMsg("Ignore danger in %s\n", GetCurSchedule()->GetName() );
	}
#endif
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : &info - 
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
void CNPC_CombineP::Event_Killed( const CTakeDamageInfo &info )
{
	if (!(g_Language.GetInt() == LANGUAGE_GERMAN || UTIL_IsLowViolence()) && info.GetDamageType() & (DMG_BLAST | DMG_CRUSH) && !(info.GetDamageType() & (DMG_DISSOLVE)) && !PlayerHasMegaPhysCannon())
	{
		Vector vecDamageDir = info.GetDamageForce();
		SpawnBlood(GetAbsOrigin(), g_vecAttackDir, BloodColor(), info.GetDamage());
		DispatchParticleEffect("headshotspray", GetAbsOrigin(), GetAbsAngles(), this);
		EmitSound("Gore.Headshot");
		float flFadeTime = 25.0;

		CGib::SpawnSpecificGibs(this, 1, 750, 1500, "models/gibs/soldier_prisonguard_head.mdl", flFadeTime);

		Vector vecRagForce;
		vecRagForce.x = random->RandomFloat(-400, 400);
		vecRagForce.y = random->RandomFloat(-400, 400);
		vecRagForce.z = random->RandomFloat(0, 250);

		Vector vecRagDmgForce = (vecRagForce + vecDamageDir);

		CBaseEntity *pLeftArmGib = CreateRagGib("models/gibs/soldier_prisonguard_left_arm.mdl", GetAbsOrigin(), GetAbsAngles(), vecRagDmgForce, flFadeTime, IsOnFire());
		if (pLeftArmGib)
		{
			color32 color = pLeftArmGib->GetRenderColor();
			pLeftArmGib->SetRenderColor(color.r, color.g, color.b, color.a);
		}

		CBaseEntity *pRightArmGib = CreateRagGib("models/gibs/soldier_prisonguard_right_arm.mdl", GetAbsOrigin(), GetAbsAngles(), vecRagDmgForce, flFadeTime, IsOnFire());
		if (pRightArmGib)
		{
			color32 color = pRightArmGib->GetRenderColor();
			pRightArmGib->SetRenderColor(color.r, color.g, color.b, color.a);
		}

		CBaseEntity *pTorsoGib = CreateRagGib("models/gibs/soldier_prisonguard_torso.mdl", GetAbsOrigin(), GetAbsAngles(), vecRagDmgForce, flFadeTime, IsOnFire());
		if (pTorsoGib)
		{
			color32 color = pTorsoGib->GetRenderColor();
			pTorsoGib->SetRenderColor(color.r, color.g, color.b, color.a);
		}

		CBaseEntity *pPelvisGib = CreateRagGib("models/gibs/soldier_prisonguard_pelvis.mdl", GetAbsOrigin(), GetAbsAngles(), vecRagDmgForce, flFadeTime, IsOnFire());
		if (pPelvisGib)
		{
			color32 color = pPelvisGib->GetRenderColor();
			pPelvisGib->SetRenderColor(color.r, color.g, color.b, color.a);
		}

		CBaseEntity *pLeftLegGib = CreateRagGib("models/gibs/soldier_prisonguard_left_leg.mdl", GetAbsOrigin(), GetAbsAngles(), vecRagDmgForce, flFadeTime, IsOnFire());
		if (pLeftLegGib)
		{
			color32 color = pLeftLegGib->GetRenderColor();
			pLeftLegGib->SetRenderColor(color.r, color.g, color.b, color.a);
		}

		CBaseEntity *pRightLegGib = CreateRagGib("models/gibs/soldier_prisonguard_right_leg.mdl", GetAbsOrigin(), GetAbsAngles(), vecRagDmgForce, flFadeTime, IsOnFire());
		if (pRightLegGib)
		{
			color32 color = pRightLegGib->GetRenderColor();
			pRightLegGib->SetRenderColor(color.r, color.g, color.b, color.a);
		}

		//now add smaller gibs.
		CGib::SpawnSpecificGibs(this, 3, 750, 1500, "models/gibs/pgib_p3.mdl", flFadeTime);
		CGib::SpawnSpecificGibs(this, 3, 750, 1500, "models/gibs/pgib_p4.mdl", flFadeTime);

		Vector forceVector = CalcDamageForceVector(info);

		// Drop any weapon that I own
		if (VPhysicsGetObject())
		{
			Vector weaponForce = forceVector * VPhysicsGetObject()->GetInvMass();
			Weapon_Drop(m_hActiveWeapon, NULL, &weaponForce);
		}
		else
		{
			Weapon_Drop(m_hActiveWeapon);
		}

		if (info.GetAttacker()->IsPlayer())
		{
			((CMultiplayRules*)GameRules())->NPCKilled(this, info);
		}

		UTIL_Remove(this);
		SetThink(NULL);
		return;
	}

	// Don't bother if we've been told not to, or the player has a megaphyscannon
	if ( combine_p_spawn_health.GetBool() == false || PlayerHasMegaPhysCannon() )
	{
		BaseClass::Event_Killed( info );
		return;
	}

	CBasePlayer *pPlayer = ToBasePlayer( info.GetAttacker() );

	if ( !pPlayer )
	{
		CPropVehicleDriveable *pVehicle = dynamic_cast<CPropVehicleDriveable *>( info.GetAttacker() ) ;
		if ( pVehicle && pVehicle->GetDriver() && pVehicle->GetDriver()->IsPlayer() )
		{
			pPlayer = assert_cast<CBasePlayer *>( pVehicle->GetDriver() );
		}
	}

	if ( pPlayer != NULL )
	{
		CHalfLife2 *pHL2GameRules = static_cast<CHalfLife2 *>(g_pGameRules);

		// Attempt to drop health
		if ( pHL2GameRules->NPC_ShouldDropHealth( pPlayer ) )
		{
			DropItem( "item_healthvial", WorldSpaceCenter()+RandomVector(-4,4), RandomAngle(0,360) );
			pHL2GameRules->NPC_DroppedHealth();
		}
		
		if ( HasSpawnFlags( SF_COMBINE_NO_GRENADEDROP ) == false )
		{
			// Attempt to drop a grenade
			if ( pHL2GameRules->NPC_ShouldDropGrenade( pPlayer ) )
			{
				DropItem( "weapon_frag", WorldSpaceCenter()+RandomVector(-4,4), RandomAngle(0,360) );
				pHL2GameRules->NPC_DroppedGrenade();
			}
		}
	}

	BaseClass::Event_Killed( info );
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : &info - 
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
bool CNPC_CombineP::IsLightDamage( const CTakeDamageInfo &info )
{
	return BaseClass::IsLightDamage( info );
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : &info - 
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
bool CNPC_CombineP::IsHeavyDamage( const CTakeDamageInfo &info )
{
	// Combine considers AR2 fire to be heavy damage
	if ( info.GetAmmoType() == GetAmmoDef()->Index("AR2") )
		return true;

	// 357 rounds are heavy damage
	if ( info.GetAmmoType() == GetAmmoDef()->Index("357") )
		return true;

	// Shotgun blasts where at least half the pellets hit me are heavy damage
	if ( info.GetDamageType() & DMG_BUCKSHOT )
	{
		int iHalfMax = sk_plr_dmg_buckshot.GetFloat() * sk_plr_num_shotgun_pellets.GetInt() * 0.5;
		if ( info.GetDamage() >= iHalfMax )
			return true;
	}

	// Rollermine shocks
	if( (info.GetDamageType() & DMG_SHOCK) && hl2_episodic.GetBool() )
	{
		return true;
	}

	return BaseClass::IsHeavyDamage( info );
}

#if HL2_EPISODIC
//-----------------------------------------------------------------------------
// Purpose: Translate base class activities into combot activites
//-----------------------------------------------------------------------------
Activity CNPC_CombineP::NPC_TranslateActivity( Activity eNewActivity )
{
	// If the special ep2_outland_05 "use march" flag is set, use the more casual marching anim.
	if ( m_iUseMarch && eNewActivity == ACT_WALK )
	{
		eNewActivity = ACT_WALK_MARCH;
	}

	return BaseClass::NPC_TranslateActivity( eNewActivity );
}


//---------------------------------------------------------
// Save/Restore
//---------------------------------------------------------
BEGIN_DATADESC( CNPC_CombineP )

	DEFINE_KEYFIELD( m_iUseMarch, FIELD_INTEGER, "usemarch" ),

END_DATADESC()
#endif