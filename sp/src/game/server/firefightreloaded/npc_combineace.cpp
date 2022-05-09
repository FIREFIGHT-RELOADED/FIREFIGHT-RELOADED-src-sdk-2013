//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: This is the soldier version of the combine, analogous to the HL1 grunt.
//
//=============================================================================//

#include "cbase.h"
#include "ai_hull.h"
#include "ai_motor.h"
#include "npc_combineace.h"
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
#include "IEffects.h"
#include "hl2/prop_combine_ball.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

void CArmorPiece::Spawn(void)
{
	BaseClass::Spawn();
	Precache();

	SetModel(STRING(GetModelName()));

	CreateVPhysics();
}

void CArmorPiece::Precache(void)
{
	PrecacheModel(STRING(GetModelName()));
}

bool CArmorPiece::CreateVPhysics(void)
{
	SetSolid(SOLID_VPHYSICS);
	IPhysicsObject *pPhysicsObject = VPhysicsInitShadow(false, false);

	if (!pPhysicsObject)
	{
		SetSolid(SOLID_NONE);
		SetMoveType(MOVETYPE_NONE);
		Warning("ERROR!: Can't create physics object for %s\n", STRING(GetModelName()));
	}

	return true;
}

LINK_ENTITY_TO_CLASS(combine_armor_piece, CArmorPiece);

//ACE BELOW.

ConVar sk_combine_ace_health( "sk_combine_ace_health", "0");
ConVar sk_combine_ace_kick( "sk_combine_ace_kick", "0");
 
// Whether or not the combine guard should spawn health on death
ConVar combine_ace_spawn_health("combine_ace_spawn_health", "1");
ConVar combine_ace_spawnwithgrenades("combine_ace_spawnwithgrenades", "1", FCVAR_ARCHIVE);
ConVar combine_ace_shieldspawnmode("combine_ace_shieldspawnmode", "2", FCVAR_ARCHIVE);
ConVar combine_ace_disablebulletresistance("combine_ace_disablebulletresistance", "0", FCVAR_ARCHIVE);

extern ConVar sk_plr_dmg_buckshot;	
extern ConVar sk_plr_num_shotgun_pellets;

LINK_ENTITY_TO_CLASS( npc_combine_ace, CNPC_CombineAce );

#define AE_SOLDIER_BLOCK_PHYSICS		20 // trying to block an incoming physics object

extern Activity ACT_WALK_EASY;
extern Activity ACT_WALK_MARCH;
extern Activity ACT_COMBINE_THROW_GRENADE;
extern Activity ACT_COMBINE_LAUNCH_GRENADE;

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CNPC_CombineAce::Spawn( void )
{
	Precache();

	SetModel( "models/combine_ace_soldier.mdl" );

	bool grenadeoverride = false;
	//change manhack number
	if (m_pAttributes != NULL)
	{
		grenadeoverride = m_pAttributes->GetBool("grenade_override");
		if (grenadeoverride && combine_ace_spawnwithgrenades.GetBool())
		{
			int grenades = m_pAttributes->GetInt("grenade_count");
			m_iNumGrenades = grenades;
		}
	}

	//Give him a random amount of grenades on spawn
	if (!grenadeoverride && combine_ace_spawnwithgrenades.GetBool())
	{
		if (g_pGameRules->IsSkillLevel(SKILL_HARD))
		{
			m_iNumGrenades = random->RandomInt(2, 3);
		}
		else if (g_pGameRules->IsSkillLevel(SKILL_VERYHARD))
		{
			m_iNumGrenades = random->RandomInt(4, 6);
		}
		else if (g_pGameRules->IsSkillLevel(SKILL_NIGHTMARE))
		{
			m_iNumGrenades = random->RandomInt(8, 12);
		}
		else
		{
			m_iNumGrenades = random->RandomInt(0, 2);
		}
	}

	m_fIsAce = true;
	m_iUseMarch = true;
	m_bisEyeForcedDead = false;

	// Stronger, tougher.
	SetHealth(sk_combine_ace_health.GetFloat());
	SetMaxHealth(sk_combine_ace_health.GetFloat());
	SetKickDamage(sk_combine_ace_kick.GetFloat());

	CapabilitiesAdd( bits_CAP_ANIMATEDFACE );
	CapabilitiesAdd( bits_CAP_MOVE_SHOOT );
	CapabilitiesAdd( bits_CAP_MOVE_JUMP );
	CapabilitiesAdd( bits_CAP_DOORS_GROUP );

	int attachment = LookupAttachment("eyes");

	// Start up the eye glow
	m_pEyeSprite = CSprite::SpriteCreate("sprites/redglow1.vmt", GetLocalOrigin(), false);

	if (m_pEyeSprite != NULL)
	{
		m_pEyeSprite->SetAttachment(this, attachment);
		m_pEyeSprite->SetTransparency(kRenderTransAdd, 255, 255, 255, 200, kRenderFxNone);
		m_pEyeSprite->SetScale(0.25f);
	}

	// Start up the eye trail
	m_pEyeTrail = CSpriteTrail::SpriteTrailCreate("sprites/bluelaser1.vmt", GetLocalOrigin(), false);

	if (m_pEyeTrail != NULL)
	{
		m_pEyeTrail->SetAttachment(this, attachment);
		m_pEyeTrail->SetTransparency(kRenderTransAdd, 255, 0, 0, 200, kRenderFxNone);
		m_pEyeTrail->SetStartWidth(8.0f);
		m_pEyeTrail->SetLifeTime(0.75f);
	}

	SetEyeState(ACE_EYE_DORMANT);

	if (combine_ace_shieldspawnmode.GetInt() == 1)
	{
		SpawnArmorPieces();
	}
	else if (combine_ace_shieldspawnmode.GetInt() > 1)
	{
		int iShieldRandom = random->RandomInt(0, 3);
		if (iShieldRandom == 0)
		{
			SpawnArmorPieces();
		}
		else
		{
			pArmor = NULL;
			m_bNoArmor = true;
		}
	}
	else
	{
		pArmor = NULL;
		m_bNoArmor = true;
	}

	if (g_pGameRules->GetSkillLevel() > SKILL_EASY)
	{
		m_bBulletResistanceBroken = combine_ace_disablebulletresistance.GetBool();
	}
	else
	{
		m_bBulletResistanceBroken = true;
	}

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

void CNPC_CombineAce::LoadInitAttributes()
{
	if (m_pAttributes != NULL)
	{
		bool disableeye = m_pAttributes->GetBool("disable_eye");

		if (disableeye)
		{
			SetEyeState(ACE_EYE_DEAD);
			m_bisEyeForcedDead = disableeye;
		}

		bool armor = m_pAttributes->GetBool("shield", m_bNoArmor);

		if (armor)
		{
			if (m_bNoArmor)
			{
				SpawnArmorPieces();
			}
		}
		else
		{
			if (!m_bNoArmor)
			{
				pArmor->Remove();
				m_bNoArmor = true;
			}
		}

		if (!m_bNoArmor)
		{
			m_pAttributes->SwitchEntityModel(pArmor, "new_shield_model", STRING(pArmor->GetModelName()));
			m_pAttributes->SwitchEntityColor(pArmor, "new_shield_color");
		}

		bool disableBulletResistance = m_pAttributes->GetBool("disable_bullet_resistance", m_bBulletResistanceBroken);

		if (disableBulletResistance)
		{
			if (m_bBulletResistanceBroken)
			{
				m_bBulletResistanceBroken = false;
			}
		}
		else
		{
			if (!m_bBulletResistanceBroken)
			{
				m_bBulletResistanceBroken = true;
			}
		}
	}

	BaseClass::LoadInitAttributes();
}

//-----------------------------------------------------------------------------
// Purpose:
// Input  :
// Output :
//-----------------------------------------------------------------------------
void CNPC_CombineAce::Precache()
{
	PrecacheModel( "models/combine_ace_soldier.mdl" );
	PrecacheModel(GetGibModel(APPENDAGE_DECAP_BODY));

	//GIBS!
	PrecacheModel(GetGibModel(APPENDAGE_HEAD));
	PrecacheModel(GetGibModel(APPENDAGE_TORSO));
	PrecacheModel(GetGibModel(APPENDAGE_PELVIS));
	PrecacheModel(GetGibModel(APPENDAGE_ARML));
	PrecacheModel(GetGibModel(APPENDAGE_ARMR));
	PrecacheModel(GetGibModel(APPENDAGE_LEGL));
	PrecacheModel(GetGibModel(APPENDAGE_LEGR));

	UTIL_PrecacheOther( "item_healthvial" );
	UTIL_PrecacheOther( "weapon_frag" );
	UTIL_PrecacheOther( "item_ammo_ar2_altfire" );
	UTIL_PrecacheOther( "item_ammo_smg1_grenade" );
	UTIL_PrecacheOther( "item_shield" );

	PrecacheModel("sprites/redglow1.vmt");

	BaseClass::Precache();
}

const char* CNPC_CombineAce::GetGibModel(appendage_t appendage)
{
	switch (appendage)
	{
	case APPENDAGE_HEAD:
		return "models/gibs/soldier_ace_head.mdl";
		break;
	case APPENDAGE_TORSO:
		return "models/gibs/soldier_ace_torso.mdl";
		break;
	case APPENDAGE_PELVIS:
		return "models/gibs/soldier_ace_pelvis.mdl";
		break;
	case APPENDAGE_ARML:
		return "models/gibs/soldier_ace_left_arm.mdl";
		break;
	case APPENDAGE_ARMR:
		return "models/gibs/soldier_ace_right_arm.mdl";
		break;
	case APPENDAGE_LEGR:
		return "models/gibs/soldier_ace_right_leg.mdl";
		break;
	case APPENDAGE_LEGL:
		return "models/gibs/soldier_ace_left_leg.mdl";
		break;
	default:
	case APPENDAGE_DECAP_BODY:
		return "models/gibs/combine_ace_soldier_beheaded.mdl";
		break;
	}

	return BaseClass::GetGibModel(appendage);
}

void CNPC_CombineAce::DeathSound( const CTakeDamageInfo &info )
{
	// NOTE: The response system deals with this at the moment
	if ( GetFlags() & FL_DISSOLVING )
		return;

	if (m_bNoDeathSound)
		return;

	if (IsOnFire())
		return;

	GetSentences()->Speak( "COMBINE_DIE", SENTENCE_PRIORITY_INVALID, SENTENCE_CRITERIA_ALWAYS ); 
}

void CNPC_CombineAce::PainSound(const CTakeDamageInfo& info)
{
	if (!m_bBulletResistanceBroken)
	{
		m_Sentences.Speak("COMBINE_TAUNT", SENTENCE_PRIORITY_INVALID, SENTENCE_CRITERIA_ALWAYS);
		return;
	}

	BaseClass::PainSound(info);
}

//-----------------------------------------------------------------------------
// Purpose: Soldiers use CAN_RANGE_ATTACK2 to indicate whether they can throw
//			a grenade. Because they check only every half-second or so, this
//			condition must persist until it is updated again by the code
//			that determines whether a grenade can be thrown, so prevent the 
//			base class from clearing it out. (sjb)
//-----------------------------------------------------------------------------
void CNPC_CombineAce::ClearAttackConditions( )
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

void CNPC_CombineAce::PrescheduleThink( void )
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
void CNPC_CombineAce::BuildScheduleTestBits( void )
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
int CNPC_CombineAce::SelectSchedule ( void )
{
	return BaseClass::SelectSchedule();
}

void CNPC_CombineAce::SetEyeState(aceEyeState_t state)
{
	//Must have a valid eye to affect
	if ((m_pEyeSprite == NULL) || (m_pEyeTrail == NULL))
		return;

	//Set the state
	switch (state)
	{
	default:
	case ACE_EYE_ACTIVATE: //Fade in and scale up
		if (!m_bisEyeForcedDead)
		{
			m_pEyeSprite->SetColor(255, 0, 0);
			m_pEyeSprite->SetBrightness(164, 0.1f);
			m_pEyeSprite->SetScale(0.8f, 0.1f);

			m_pEyeTrail->SetColor(255, 0, 0);
			m_pEyeTrail->SetScale(2.0f);
			m_pEyeTrail->SetBrightness(164);
		}
		break;
	
	case ACE_EYE_DORMANT: //Fade out and scale down
		if (!m_bisEyeForcedDead)
		{
			m_pEyeSprite->SetScale(0.4f, 0.5f);
			m_pEyeSprite->SetBrightness(64, 0.5f);

			m_pEyeTrail->SetScale(0.8f);
			m_pEyeTrail->SetBrightness(64);
		}
		break;

	case ACE_EYE_DEAD: //Fade out slowly
		if (!m_bisEyeForcedDead)
		{
			m_pEyeSprite->SetColor(255, 0, 0);
			m_pEyeSprite->SetScale(0.1f, 5.0f);
			m_pEyeSprite->SetBrightness(0, 5.0f);

			m_pEyeTrail->SetColor(255, 0, 0);
			m_pEyeTrail->SetScale(0.1f);
			m_pEyeTrail->SetBrightness(0);
		}
		break;
	}
}

CTakeDamageInfo CNPC_CombineAce::BulletResistanceLogic(const CTakeDamageInfo& info, trace_t* ptr)
{
	CTakeDamageInfo outputInfo = info;

	if ((GetHealth() > GetMaxHealth() * 0.5) && !FStrEq(outputInfo.GetAmmoName(), "Katana") && !m_bBulletResistanceBroken)
	{
		if (!(outputInfo.GetDamageType() & (DMG_GENERIC)))
		{
			if (ptr != NULL)
			{
				CPVSFilter filter(ptr->endpos);
				te->ArmorRicochet(filter, 0.0, &ptr->endpos, &ptr->plane.normal);
			}
			
			float fDifficultyBasedDamage = 0.0f;

			if (g_pGameRules->GetSkillLevel() < SKILL_VERYHARD)
			{
				fDifficultyBasedDamage = 0.2f;
			}
			else if (g_pGameRules->GetSkillLevel() > SKILL_VERYHARD)
			{
				fDifficultyBasedDamage = 0.1f;
			}

			if (ptr->hitgroup != HITGROUP_HEAD)
			{
				outputInfo.SetDamage(info.GetDamage() * fDifficultyBasedDamage);
			}
			else
			{
				outputInfo.SetDamage(info.GetDamage() * 0.5f);
			}

			outputInfo.AddDamageType(DMG_NEVERGIB);
		}
	}

	if ((GetHealth() <= GetMaxHealth() * 0.5) && !m_bBulletResistanceBroken)
	{
		m_bBulletResistanceBroken = true;
	}

	return outputInfo;
}

//---------------------------------------------------------
//---------------------------------------------------------
void CNPC_CombineAce::TraceAttack(const CTakeDamageInfo& inputInfo, const Vector& vecDir, trace_t* ptr, CDmgAccumulator* pAccumulator)
{
	// special interaction with combine balls
	if (!m_bBulletResistanceBroken)
	{
		CTakeDamageInfo bulletResistanceInfo = BulletResistanceLogic(inputInfo, ptr);
		BaseClass::TraceAttack(bulletResistanceInfo, vecDir, ptr, pAccumulator);
	}
	else
	{
		BaseClass::TraceAttack(inputInfo, vecDir, ptr, pAccumulator);
	}
}

int CNPC_CombineAce::OnTakeDamage_Alive(const CTakeDamageInfo& info)
{
	// special interaction with combine balls
	if (UTIL_IsAR2CombineBall(info.GetInflictor()) && !m_bBulletResistanceBroken)
	{
		CTakeDamageInfo bulletResistanceInfo = BulletResistanceLogic(info, NULL);
		return BaseClass::OnTakeDamage_Alive(bulletResistanceInfo);
	}
	else
	{
		return BaseClass::OnTakeDamage_Alive(info);
	}
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
float CNPC_CombineAce::GetHitgroupDamageMultiplier( int iHitGroup, const CTakeDamageInfo &info )
{
	switch (iHitGroup)
	{
	case HITGROUP_HEAD:
		if (m_bBulletResistanceBroken)
		{
			// Soldiers take double headshot damage
			if (CorpseDecapitate(info))
			{
				//we're dead by this point, lol
				return BaseClass::GetHitgroupDamageMultiplier(iHitGroup, info);
			}
			else
			{
				return 1.5f;
			}
		}
		else
		{
			return 1.5f;
		}
	}

	return BaseClass::GetHitgroupDamageMultiplier(iHitGroup, info);
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CNPC_CombineAce::HandleAnimEvent( animevent_t *pEvent )
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

void CNPC_CombineAce::OnChangeActivity( Activity eNewActivity )
{
	// Any new sequence stops us blocking.
	m_fIsBlocking = false;

	BaseClass::OnChangeActivity( eNewActivity );

	if (m_iUseMarch)
	{
		SetPoseParameter("casual", 1.0);
	}
}

void CNPC_CombineAce::OnListened()
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
void CNPC_CombineAce::Event_Killed( const CTakeDamageInfo &info )
{
	if (CorpseGib(info))
	{
		return;
	}

	// Don't bother if we've been told not to, or the player has a megaphyscannon
	if ( combine_ace_spawn_health.GetBool() == false || PlayerHasMegaPhysCannon() )
	{
		BaseClass::Event_Killed( info );
		return;
	}

	SetEyeState(ACE_EYE_DEAD);

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
		if (m_hActiveWeapon)
		{
			// Elites drop alt-fire ammo, so long as they weren't killed by dissolving.
#ifdef HL2_EPISODIC
			if (HasSpawnFlags(SF_COMBINE_NO_AR2DROP) == false)
#endif
			{
				CBaseEntity* pItem = NULL;

				if (FClassnameIs(GetActiveWeapon(), "weapon_ar2"))
				{
					pItem = DropItem("item_ammo_ar2_altfire", WorldSpaceCenter() + RandomVector(-4, 4), RandomAngle(0, 360));
				}
				else if (FClassnameIs(GetActiveWeapon(), "weapon_smg1"))
				{
					pItem = DropItem("item_ammo_smg1_grenade", WorldSpaceCenter() + RandomVector(-4, 4), RandomAngle(0, 360));
				}

				if (pItem)
				{
					IPhysicsObject* pObj = pItem->VPhysicsGetObject();

					if (pObj)
					{
						Vector			vel = RandomVector(-64.0f, 64.0f);
						AngularImpulse	angImp = RandomAngularImpulse(-300.0f, 300.0f);

						vel[2] = 0.0f;
						pObj->AddVelocity(&vel, &angImp);
					}

					if (info.GetDamageType() & DMG_DISSOLVE)
					{
						CBaseAnimating* pAnimating = dynamic_cast<CBaseAnimating*>(pItem);

						if (pAnimating)
						{
							pAnimating->Dissolve(NULL, gpGlobals->curtime, false, ENTITY_DISSOLVE_NORMAL);
						}
					}
					else
					{
						WeaponManager_AddManaged(pItem);
					}
				}
			}
		}

		CHalfLife2 *pHL2GameRules = static_cast<CHalfLife2 *>(g_pGameRules);

		// Attempt to drop health
		if ( pHL2GameRules->NPC_ShouldDropHealth( pPlayer ) )
		{
			DropItem( "item_healthvial", WorldSpaceCenter()+RandomVector(-4,4), RandomAngle(0,360) );
			pHL2GameRules->NPC_DroppedHealth();
		}

		if (!m_bNoArmor && combine_ace_shieldspawnmode.GetInt() > 0)
		{
			pArmor->Remove();
			CBaseEntity* shield = DropItem("item_shield", WorldSpaceCenter() + RandomVector(-4, 4), RandomAngle(0, 360));

			if (m_pAttributes != NULL)
			{
				m_pAttributes->SwitchEntityModel(shield, "new_armor_model", STRING(shield->GetModelName()));
				m_pAttributes->SwitchEntityColor(shield, "new_armor_color");
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
bool CNPC_CombineAce::IsLightDamage( const CTakeDamageInfo &info )
{
	return BaseClass::IsLightDamage( info );
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : &info - 
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
bool CNPC_CombineAce::IsHeavyDamage( const CTakeDamageInfo &info )
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
Activity CNPC_CombineAce::NPC_TranslateActivity( Activity eNewActivity )
{
	// If the special ep2_outland_05 "use march" flag is set, use the more casual marching anim.
	if ( m_iUseMarch && eNewActivity == ACT_WALK )
	{
		eNewActivity = ACT_WALK_MARCH;
	}
	else if (eNewActivity == ACT_IDLE)
	{
		SetEyeState(ACE_EYE_DORMANT);

		if (!IsCrouching() && (m_NPCState == NPC_STATE_COMBAT || m_NPCState == NPC_STATE_ALERT))
		{
			SetEyeState(ACE_EYE_ACTIVATE);
			eNewActivity = ACT_IDLE_ANGRY;
		}
	}
	else if (eNewActivity == ACT_RANGE_ATTACK1)
	{
		SetEyeState(ACE_EYE_ACTIVATE);
	}
	else if (eNewActivity == ACT_RANGE_ATTACK2)
	{
		SetEyeState(ACE_EYE_ACTIVATE);

		// grunt is going to a secondary long range attack. This may be a thrown 
		// grenade or fired grenade, we must determine which and pick proper sequence
		if (Weapon_OwnsThisType("weapon_grenadelauncher"))
		{
			return (Activity)ACT_COMBINE_LAUNCH_GRENADE;
		}
		else
		{
			return (Activity)ACT_COMBINE_THROW_GRENADE;
		}
	}

	return BaseClass::NPC_TranslateActivity( eNewActivity );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CNPC_CombineAce::SpawnArmorPieces(void)
{
	pArmor = (CArmorPiece *)CBaseEntity::CreateNoSpawn("combine_armor_piece", GetAbsOrigin(), GetAbsAngles(), this);
	pArmor->SetModelName(MAKE_STRING("models/armor/shield.mdl"));
	pArmor->SetParent(this, LookupAttachment("shield_attach"));
	pArmor->SetLocalOrigin(vec3_origin);
	pArmor->SetLocalAngles(vec3_angle);
	DispatchSpawn(pArmor);
	pArmor->Activate();

	m_bNoArmor = false;
}


//---------------------------------------------------------
// Save/Restore
//---------------------------------------------------------
BEGIN_DATADESC( CNPC_CombineAce )

	DEFINE_KEYFIELD( m_iUseMarch, FIELD_INTEGER, "usemarch" ),
	DEFINE_FIELD(m_pEyeSprite, FIELD_CLASSPTR),
	DEFINE_FIELD(m_pEyeTrail, FIELD_CLASSPTR),

END_DATADESC()
#endif