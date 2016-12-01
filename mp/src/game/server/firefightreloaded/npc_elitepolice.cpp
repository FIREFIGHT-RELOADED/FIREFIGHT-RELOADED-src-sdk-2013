//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: This is the soldier version of the combine, analogous to the HL1 grunt.
//
//=============================================================================//

#include "cbase.h"
#include "ai_hull.h"
#include "ai_motor.h"
#include "npc_elitepolice.h"
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

ConVar	sk_elitepolice_health("sk_elitepolice_health", "45");

extern ConVar sk_plr_dmg_buckshot;
extern ConVar sk_plr_num_shotgun_pellets;

LINK_ENTITY_TO_CLASS(npc_elitepolice, CNPC_ElitePolice);

#define AE_SOLDIER_BLOCK_PHYSICS		20 // trying to block an incoming physics object

extern Activity ACT_WALK_EASY;
extern Activity ACT_WALK_MARCH;

#define bits_MEMORY_PAIN_LIGHT_SOUND		bits_MEMORY_CUSTOM1
#define bits_MEMORY_PAIN_HEAVY_SOUND		bits_MEMORY_CUSTOM2
#define bits_MEMORY_PLAYER_HURT				bits_MEMORY_CUSTOM3
//TODO add painsound from CNPC_ Combine
//Also finish adding this:
//if (!m_fIsPoliceRank)
//{
//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CNPC_ElitePolice::Spawn(void)
{
	Precache();
	SetModel("models/elitepolice.mdl");

	m_fIsElite = false;
	m_fIsAce = false;
	m_fIsPoliceRank = true;
	if (m_spawnEquipment != MAKE_STRING("weapon_pistol"))
	{
		m_iUseMarch = true;
	}

	SetHealth(sk_elitepolice_health.GetFloat());
	SetMaxHealth(sk_elitepolice_health.GetFloat());
	SetKickDamage(0);

	CapabilitiesAdd(bits_CAP_ANIMATEDFACE);
	CapabilitiesAdd(bits_CAP_MOVE_SHOOT);
	CapabilitiesAdd(bits_CAP_DOORS_GROUP);

	BaseClass::Spawn();

	/*
#if HL2_EPISODIC
	if (m_iUseMarch && !HasSpawnFlags(SF_NPC_START_EFFICIENT))
	{
		Msg("Soldier %s is set to use march anim, but is not an efficient AI. The blended march anim can only be used for dead-ahead walks!\n", GetDebugName());
	}
#endif
	*/
}

//-----------------------------------------------------------------------------
// Purpose:
// Input  :
// Output :
//-----------------------------------------------------------------------------
void CNPC_ElitePolice::Precache()
{
	PrecacheModel("models/elitepolice.mdl");
	PrecacheModel("models/gibs/elitepolice_beheaded.mdl");
	PrecacheModel("models/gibs/pgib_p3.mdl");
	PrecacheModel("models/gibs/pgib_p4.mdl");

	//GIBS!
	PrecacheModel("models/gibs/elitepolice_head.mdl");
	PrecacheModel("models/gibs/elitepolice_left_arm.mdl");
	PrecacheModel("models/gibs/elitepolice_right_arm.mdl");
	PrecacheModel("models/gibs/elitepolice_torso.mdl");
	PrecacheModel("models/gibs/elitepolice_pelvis.mdl");
	PrecacheModel("models/gibs/elitepolice_left_leg.mdl");
	PrecacheModel("models/gibs/elitepolice_right_leg.mdl");

	PrecacheParticleSystem("headshotspray");

	UTIL_PrecacheOther("item_healthvial");
	UTIL_PrecacheOther("weapon_frag");

	PrecacheScriptSound("Gore.Headshot");
	enginesound->PrecacheSentenceGroup("METROPOLICE");

	BaseClass::Precache();
}

//-----------------------------------------------------------------------------
// Create components
//-----------------------------------------------------------------------------
bool CNPC_ElitePolice::CreateComponents()
{
	if (!BaseClass::CreateComponents())
		return false;

	m_Sentences.Init(this, "NPC_Metropolice.SentenceParameters");
	return true;
}


void CNPC_ElitePolice::DeathSound(const CTakeDamageInfo &info)
{
	// NOTE: The response system deals with this at the moment
	if (GetFlags() & FL_DISSOLVING)
		return;

	if (IsOnFire())
		return;

	m_Sentences.Speak("METROPOLICE_DIE", SENTENCE_PRIORITY_INVALID, SENTENCE_CRITERIA_ALWAYS);
}


//-----------------------------------------------------------------------------
// Purpose: Soldiers use CAN_RANGE_ATTACK2 to indicate whether they can throw
//			a grenade. Because they check only every half-second or so, this
//			condition must persist until it is updated again by the code
//			that determines whether a grenade can be thrown, so prevent the 
//			base class from clearing it out. (sjb)
//-----------------------------------------------------------------------------
void CNPC_ElitePolice::ClearAttackConditions()
{
	bool fCanRangeAttack2 = HasCondition(COND_CAN_RANGE_ATTACK2);

	// Call the base class.
	BaseClass::ClearAttackConditions();

	if (fCanRangeAttack2)
	{
		// We don't allow the base class to clear this condition because we
		// don't sense for it every frame.
		SetCondition(COND_CAN_RANGE_ATTACK2);
	}
}

void CNPC_ElitePolice::PrescheduleThink(void)
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
void CNPC_ElitePolice::BuildScheduleTestBits(void)
{
	//Interrupt any schedule with physics danger (as long as I'm not moving or already trying to block)
	if (m_flGroundSpeed == 0.0 && !IsCurSchedule(SCHED_FLINCH_PHYSICS))
	{
		SetCustomInterruptCondition(COND_HEAR_PHYSICS_DANGER);
	}

	BaseClass::BuildScheduleTestBits();
}

//-----------------------------------------------------------------------------
// Purpose:
// Input  :
// Output :
//-----------------------------------------------------------------------------
int CNPC_ElitePolice::SelectSchedule(void)
{
	return BaseClass::SelectSchedule();
}

//-----------------------------------------------------------------------------
// Purpose: Announce an assault if the enemy can see me and we are pretty 
//			close to him/her
// Input  :
// Output :
//-----------------------------------------------------------------------------
void CNPC_ElitePolice::AnnounceAssault(void)
{
	if (random->RandomInt(0, 5) > 1)
		return;

	// If enemy can see me make assualt sound
	CBaseCombatCharacter* pBCC = GetEnemyCombatCharacterPointer();

	if (!pBCC)
		return;

	if (!FOkToMakeSound())
		return;

	// Make sure we are pretty close
	if (WorldSpaceCenter().DistToSqr(pBCC->WorldSpaceCenter()) > (2000 * 2000))
		return;

	// Make sure we are in view cone of player
	if (!pBCC->FInViewCone(this))
		return;

	// Make sure player can see me
	if (FVisible(pBCC))
	{
		m_Sentences.Speak("METROPOLICE_GO_ALERT");
	}
}


void CNPC_ElitePolice::AnnounceEnemyType(CBaseEntity *pEnemy)
{
	const char *pSentenceName = "METROPOLICE_MONST";
	switch (pEnemy->Classify())
	{
	case CLASS_PLAYER:
		pSentenceName = "METROPOLICE_MONST_PLAYER";
		break;

	case CLASS_PLAYER_ALLY:
	case CLASS_CITIZEN_REBEL:
	case CLASS_CITIZEN_PASSIVE:
	case CLASS_VORTIGAUNT:
		pSentenceName = "METROPOLICE_MONST_CITIZENS";
		break;

	case CLASS_PLAYER_ALLY_VITAL:
		pSentenceName = "METROPOLICE_MONST_CHARACTER";
		break;

	case CLASS_ANTLION:
		pSentenceName = "METROPOLICE_MONST_BUGS";
		break;

	case CLASS_ZOMBIE:
		pSentenceName = "METROPOLICE_MONST_ZOMBIES";
		break;

	case CLASS_HEADCRAB:
	case CLASS_BARNACLE:
		pSentenceName = "METROPOLICE_MONST_PARASITES";
		break;
	}

	m_Sentences.Speak(pSentenceName, SENTENCE_PRIORITY_HIGH);
}

void CNPC_ElitePolice::AnnounceEnemyKill(CBaseEntity *pEnemy)
{
	if (!pEnemy)
		return;

	const char *pSentenceName = "METROPOLICE_KILL_MONST";
	switch (pEnemy->Classify())
	{
	case CLASS_PLAYER:
		pSentenceName = "METROPOLICE_KILL_PLAYER";
		break;

		// no sentences for these guys yet
	case CLASS_PLAYER_ALLY:
	case CLASS_CITIZEN_REBEL:
	case CLASS_CITIZEN_PASSIVE:
	case CLASS_VORTIGAUNT:
		pSentenceName = "METROPOLICE_KILL_CITIZENS";
		break;

	case CLASS_PLAYER_ALLY_VITAL:
		pSentenceName = "METROPOLICE_KILL_CHARACTER";
		break;

	case CLASS_ANTLION:
		pSentenceName = "METROPOLICE_KILL_BUGS";
		break;

	case CLASS_ZOMBIE:
		pSentenceName = "METROPOLICE_KILL_ZOMBIES";
		break;

	case CLASS_HEADCRAB:
	case CLASS_BARNACLE:
		pSentenceName = "METROPOLICE_KILL_PARASITES";
		break;
	}

	m_Sentences.Speak(pSentenceName, SENTENCE_PRIORITY_HIGH);
}

//-----------------------------------------------------------------------------
// My buddies got killed!
//-----------------------------------------------------------------------------
void CNPC_ElitePolice::NotifyDeadFriend(CBaseEntity* pFriend)
{
	BaseClass::NotifyDeadFriend(pFriend);

	m_Sentences.Speak("METROPOLICE_MAN_DOWN", SENTENCE_PRIORITY_MEDIUM);
}

//=========================================================
// PainSound
//=========================================================
void CNPC_ElitePolice::PainSound(void)
{
	if (gpGlobals->curtime < m_flNextPainSoundTime)
		return;

	// Don't make pain sounds if I'm on fire. The looping sound will take care of that for us.
	if (IsOnFire())
		return;

	float healthRatio = (float)GetHealth() / (float)GetMaxHealth();
	if (healthRatio > 0.0f)
	{
		const char *pSentenceName = "METROPOLICE_PAIN";
		if (!HasMemory(bits_MEMORY_PAIN_HEAVY_SOUND) && (healthRatio < 0.25f))
		{
			Remember(bits_MEMORY_PAIN_HEAVY_SOUND | bits_MEMORY_PAIN_LIGHT_SOUND);
			pSentenceName = "METROPOLICE_PAIN_HEAVY";
		}
		else if (!HasMemory(bits_MEMORY_PAIN_LIGHT_SOUND) && healthRatio > 0.8f)
		{
			Remember(bits_MEMORY_PAIN_LIGHT_SOUND);
			pSentenceName = "METROPOLICE_PAIN_LIGHT";
		}

		// This causes it to speak it no matter what; doesn't bother with setting sounds.
		m_Sentences.Speak(pSentenceName, SENTENCE_PRIORITY_INVALID, SENTENCE_CRITERIA_ALWAYS);
		m_flNextPainSoundTime = gpGlobals->curtime + 1;
	}
}

//-----------------------------------------------------------------------------
// Purpose: implemented by subclasses to give them an opportunity to make
//			a sound when they lose their enemy
// Input  :
// Output :
//-----------------------------------------------------------------------------
void CNPC_ElitePolice::LostEnemySound(void)
{
	if (gpGlobals->curtime <= m_flNextLostSoundTime)
		return;

	const char *pSentence;
	if (!(CBaseEntity*)GetEnemy() || gpGlobals->curtime - GetEnemyLastTimeSeen() > 10)
	{
		pSentence = "METROPOLICE_LOST_LONG";
	}
	else
	{
		pSentence = "METROPOLICE_LOST_SHORT";
	}

	if (m_Sentences.Speak(pSentence) >= 0)
	{
		m_flNextLostSoundTime = gpGlobals->curtime + random->RandomFloat(5.0, 15.0);
	}
}

//-----------------------------------------------------------------------------
// Purpose: implemented by subclasses to give them an opportunity to make
//			a sound when they lose their enemy
// Input  :
// Output :
//-----------------------------------------------------------------------------
void CNPC_ElitePolice::FoundEnemySound(void)
{
	m_Sentences.Speak("METROPOLICE_REFIND_ENEMY", SENTENCE_PRIORITY_HIGH);
}

//-----------------------------------------------------------------------------
// IdleSound 
//-----------------------------------------------------------------------------
void CNPC_ElitePolice::IdleSound(void)
{
	m_Sentences.Speak("METROPOLICE_IDLE", SENTENCE_PRIORITY_HIGH);
}

//-----------------------------------------------------------------------------
// Purpose: Implemented by subclasses to give them an opportunity to make
//			a sound before they attack
// Input  :
// Output :
//-----------------------------------------------------------------------------

// BUGBUG: It looks like this is never played because combine don't do SCHED_WAKE_ANGRY or anything else that does a TASK_SOUND_WAKE
void CNPC_ElitePolice::AlertSound(void)
{
	if (gpGlobals->curtime > m_flNextAlertSoundTime)
	{
		m_Sentences.Speak("METROPOLICE_GO_ALERT", SENTENCE_PRIORITY_HIGH);
		m_flNextAlertSoundTime = gpGlobals->curtime + 10.0f;
	}
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
float CNPC_ElitePolice::GetHitgroupDamageMultiplier(int iHitGroup, const CTakeDamageInfo &info)
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
				SetModel("models/gibs/elitepolice_beheaded.mdl");
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

	return BaseClass::GetHitgroupDamageMultiplier(iHitGroup, info);
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CNPC_ElitePolice::HandleAnimEvent(animevent_t *pEvent)
{
	switch (pEvent->event)
	{
	case AE_SOLDIER_BLOCK_PHYSICS:
		DevMsg("BLOCKING!\n");
		m_fIsBlocking = true;
		break;

	default:
		BaseClass::HandleAnimEvent(pEvent);
		break;
	}
}

void CNPC_ElitePolice::OnChangeActivity(Activity eNewActivity)
{
	// Any new sequence stops us blocking.
	m_fIsBlocking = false;

	BaseClass::OnChangeActivity(eNewActivity);

	if (m_iUseMarch)
	{
		SetPoseParameter("casual", 1.0);
	}
}

void CNPC_ElitePolice::OnListened()
{
	BaseClass::OnListened();

	if (HasCondition(COND_HEAR_DANGER) && HasCondition(COND_HEAR_PHYSICS_DANGER))
	{
		if (HasInterruptCondition(COND_HEAR_DANGER))
		{
			ClearCondition(COND_HEAR_PHYSICS_DANGER);
		}
	}

	// debugging to find missed schedules
#if 0
	if (HasCondition(COND_HEAR_DANGER) && !HasInterruptCondition(COND_HEAR_DANGER))
	{
		DevMsg("Ignore danger in %s\n", GetCurSchedule()->GetName());
	}
#endif
}

int CNPC_ElitePolice::MeleeAttack1Conditions(float flDot, float flDist)
{
	//breen didn't teach me to melee, so ill just shoot my way out instead.
	return COND_NONE;
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : &info - 
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
void CNPC_ElitePolice::Event_Killed(const CTakeDamageInfo &info)
{
	if (!(g_Language.GetInt() == LANGUAGE_GERMAN || UTIL_IsLowViolence()) && info.GetDamageType() & (DMG_BLAST | DMG_CRUSH) && !(info.GetDamageType() & (DMG_DISSOLVE)) && !PlayerHasMegaPhysCannon())
	{
		Vector vecDamageDir = info.GetDamageForce();
		SpawnBlood(GetAbsOrigin(), g_vecAttackDir, BloodColor(), info.GetDamage());
		DispatchParticleEffect("headshotspray", GetAbsOrigin(), GetAbsAngles(), this);
		EmitSound("Gore.Headshot");
		float flFadeTime = 25.0;

		CGib::SpawnSpecificGibs(this, 1, 750, 1500, "models/gibs/elitepolice_head.mdl", flFadeTime);

		Vector vecRagForce;
		vecRagForce.x = random->RandomFloat(-400, 400);
		vecRagForce.y = random->RandomFloat(-400, 400);
		vecRagForce.z = random->RandomFloat(0, 250);

		Vector vecRagDmgForce = (vecRagForce + vecDamageDir);

		CBaseEntity *pLeftArmGib = CreateRagGib("models/gibs/elitepolice_left_arm.mdl", GetAbsOrigin(), GetAbsAngles(), vecRagDmgForce, flFadeTime, IsOnFire());
		if (pLeftArmGib)
		{
			color32 color = pLeftArmGib->GetRenderColor();
			pLeftArmGib->SetRenderColor(color.r, color.g, color.b, color.a);
		}

		CBaseEntity *pRightArmGib = CreateRagGib("models/gibs/elitepolice_right_arm.mdl", GetAbsOrigin(), GetAbsAngles(), vecRagDmgForce, flFadeTime, IsOnFire());
		if (pRightArmGib)
		{
			color32 color = pRightArmGib->GetRenderColor();
			pRightArmGib->SetRenderColor(color.r, color.g, color.b, color.a);
		}

		CBaseEntity *pTorsoGib = CreateRagGib("models/gibs/elitepolice_torso.mdl", GetAbsOrigin(), GetAbsAngles(), vecRagDmgForce, flFadeTime, IsOnFire());
		if (pTorsoGib)
		{
			color32 color = pTorsoGib->GetRenderColor();
			pTorsoGib->SetRenderColor(color.r, color.g, color.b, color.a);
		}

		CBaseEntity *pPelvisGib = CreateRagGib("models/gibs/elitepolice_pelvis.mdl", GetAbsOrigin(), GetAbsAngles(), vecRagDmgForce, flFadeTime, IsOnFire());
		if (pPelvisGib)
		{
			color32 color = pPelvisGib->GetRenderColor();
			pPelvisGib->SetRenderColor(color.r, color.g, color.b, color.a);
		}

		CBaseEntity *pLeftLegGib = CreateRagGib("models/gibs/elitepolice_left_leg.mdl", GetAbsOrigin(), GetAbsAngles(), vecRagDmgForce, flFadeTime, IsOnFire());
		if (pLeftLegGib)
		{
			color32 color = pLeftLegGib->GetRenderColor();
			pLeftLegGib->SetRenderColor(color.r, color.g, color.b, color.a);
		}

		CBaseEntity *pRightLegGib = CreateRagGib("models/gibs/elitepolice_right_leg.mdl", GetAbsOrigin(), GetAbsAngles(), vecRagDmgForce, flFadeTime, IsOnFire());
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

	BaseClass::Event_Killed(info);
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : &info - 
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
bool CNPC_ElitePolice::IsLightDamage(const CTakeDamageInfo &info)
{
	return BaseClass::IsLightDamage(info);
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : &info - 
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
bool CNPC_ElitePolice::IsHeavyDamage(const CTakeDamageInfo &info)
{
	// Combine considers AR2 fire to be heavy damage
	if (info.GetAmmoType() == GetAmmoDef()->Index("AR2"))
		return true;

	// 357 rounds are heavy damage
	if (info.GetAmmoType() == GetAmmoDef()->Index("357"))
		return true;

	// Shotgun blasts where at least half the pellets hit me are heavy damage
	if (info.GetDamageType() & DMG_BUCKSHOT)
	{
		int iHalfMax = sk_plr_dmg_buckshot.GetFloat() * sk_plr_num_shotgun_pellets.GetInt() * 0.5;
		if (info.GetDamage() >= iHalfMax)
			return true;
	}

	// Rollermine shocks
	if ((info.GetDamageType() & DMG_SHOCK) && hl2_episodic.GetBool())
	{
		return true;
	}

	return BaseClass::IsHeavyDamage(info);
}

#if HL2_EPISODIC
//-----------------------------------------------------------------------------
// Purpose: Translate base class activities into combot activites
//-----------------------------------------------------------------------------
Activity CNPC_ElitePolice::NPC_TranslateActivity(Activity eNewActivity)
{
	// If the special ep2_outland_05 "use march" flag is set, use the more casual marching anim.
	if (m_iUseMarch && eNewActivity == ACT_WALK)
	{
		eNewActivity = ACT_WALK_MARCH;
	}

	return BaseClass::NPC_TranslateActivity(eNewActivity);
}


//---------------------------------------------------------
// Save/Restore
//---------------------------------------------------------
BEGIN_DATADESC(CNPC_ElitePolice)

DEFINE_KEYFIELD(m_iUseMarch, FIELD_INTEGER, "usemarch"),

END_DATADESC()
#endif