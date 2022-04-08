//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: This is the soldier version of the combine, analogous to the HL1 grunt.
//
//=============================================================================//

#include "cbase.h"
#include "ai_hull.h"
#include "ai_motor.h"
#include "npc_player.h"
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
#include "filesystem.h"
// TODO: add npc_citizen17 features, like player squads!!!
#include "ai_squad.h"

/*
#include <time.h>
#include <vector>
#include "mathlib/vector.h"
*/

#include <animation.h>
#include <datacache/imdlcache.h>

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

//TODO: GIVE PLAYERS A HINT WHEN ALLY SPAWN

extern ConVar sk_combine_guard_kick;
extern ConVar sk_plr_dmg_buckshot;	
extern ConVar sk_plr_num_shotgun_pellets;
extern ConVar sv_player_hardcoremode;
extern ConVar fr_new_normspeed;
extern ConVar sv_regeneration;
extern ConVar sv_regeneration_wait_time;
extern ConVar sv_regeneration_rate_default;
extern ConVar sv_regeneration_rate;
extern ConVar sv_regen_interval;

static ConVar npc_playerbot_friendlyfire("npc_playerbot_friendlyfire", "0", FCVAR_ARCHIVE);
static ConVar npc_playerbot_useplayersmodel("npc_playerbot_useplayersmodel", "1", FCVAR_ARCHIVE);

//------------------------------------------------------------------------------
// Purpose: 
//------------------------------------------------------------------------------
void CC_Compatibility(void)
{
	MDLCACHE_CRITICAL_SECTION();
	FileFindHandle_t findHandle = NULL;
	int incompat = 0;

	const char* pszFilename = g_pFullFileSystem->FindFirst("models/player/playermodels/*.mdl", &findHandle);
	while (pszFilename)
	{
		char szModelName[2048];
		Q_snprintf(szModelName, sizeof(szModelName), "models/player/playermodels/%s", pszFilename);
		
		CStudioHdr studioHdr;
		const model_t* model = modelinfo->GetModel(modelinfo->GetModelIndex(szModelName));
		if (model)
		{
			studioHdr.Init(modelinfo->GetStudiomodel(model));
		}

		int NPCAnim = LookupSequence(&studioHdr, "Man_Gun");
		if (NPCAnim <= 0)
		{
			Warning("Model %s doesn't include animations from \"combine_soldier_anims.mdl\" and/or \"elitepolice_animations.mdl\".\n", szModelName);
			incompat++;
		}

		pszFilename = g_pFullFileSystem->FindNext(findHandle);
	}

	if (incompat > 0)
	{
		Warning("Please add these before '$includemodel \"player/male_shared.mdl\"' in your models' QCs for npc_playerbots to use the models!\n");
	}
	else
	{
		Msg("No NPC animation incompatibilities found!\n");
	}
	g_pFullFileSystem->FindClose(findHandle);
}
static ConCommand debug_check_incompatible_models("debug_check_incompatible_models", CC_Compatibility, "", FCVAR_NONE);

LINK_ENTITY_TO_CLASS( npc_playerbot, CNPC_Player );

BEGIN_DATADESC(CNPC_Player)
DEFINE_FIELD(m_flSoonestWeaponSwitch, FIELD_TIME),
DEFINE_FIELD(m_fTimeLastHurt, FIELD_TIME),
DEFINE_FIELD(m_fTimeLastHealed, FIELD_TIME),
END_DATADESC()

#define	PLAYERNPC_FASTEST_SWITCH_TIME 5.0f

const char* g_charNPCMidRangeWeapons[] =
{
	"weapon_smg1",
	"weapon_ar2"
};

const char* g_charNPCShortRangeWeapons[] =
{
	"weapon_shotgun",
	"weapon_pistol"
};

const char* g_charAvailableModels[] =
{
	"models/player/playermodels/gordon.mdl",
	"models/player/playermodels/gordon_old.mdl",
	"models/player/playermodels/male_01.mdl",
	"models/player/playermodels/male_02.mdl",
	"models/player/playermodels/male_03.mdl",
	"models/player/playermodels/male_04.mdl",
	"models/player/playermodels/male_05.mdl",
	"models/player/playermodels/male_06.mdl",
	"models/player/playermodels/male_07.mdl",
	"models/player/playermodels/male_08.mdl",
	"models/player/playermodels/male_09.mdl",
	"models/player/playermodels/female_01.mdl",
	"models/player/playermodels/female_02.mdl",
	"models/player/playermodels/female_03.mdl",
	"models/player/playermodels/female_04.mdl",
	"models/player/playermodels/female_06.mdl",
	"models/player/playermodels/female_07.mdl",
	"models/player/playermodels/gascit_gmadador.mdl",
	"models/player/playermodels/american_assault.mdl",
	"models/player/playermodels/american_mg.mdl",
	"models/player/playermodels/american_rifleman.mdl",
	"models/player/playermodels/american_rocket.mdl",
	"models/player/playermodels/american_sniper.mdl",
	"models/player/playermodels/american_support.mdl",
	"models/player/playermodels/ct_gign.mdl",
	"models/player/playermodels/ct_gsg9.mdl",
	"models/player/playermodels/ct_sas.mdl",
	"models/player/playermodels/ct_urban.mdl",
	"models/player/playermodels/t_arctic.mdl",
	"models/player/playermodels/t_guerilla.mdl",
	"models/player/playermodels/t_leet.mdl",
	"models/player/playermodels/t_phoenix.mdl"
};

CNPC_Player::CNPC_Player()
{
	m_flSoonestWeaponSwitch = gpGlobals->curtime;
	m_fRegenRemander = 0;
}

CNPC_Player::~CNPC_Player(void)
{
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CNPC_Player::Spawn( void )
{
	Precache();

	// Stronger, tougher.
	SetHealth(200);
	SetMaxHealth(200);
	SetKickDamage(sk_combine_guard_kick.GetFloat());

	int iShouldUsePlayersModel = random->RandomInt(0, 3);
	const char* modelName = "";

	int nModels = ARRAYSIZE(g_charAvailableModels);
	int randomChoiceModels = rand() % nModels;

	if (iShouldUsePlayersModel == 1 && npc_playerbot_useplayersmodel.GetBool())
	{
		CBasePlayer* pLocalPlayer = UTIL_GetNearestVisiblePlayer(this);

		if (pLocalPlayer)
		{
			int NPCAnim = pLocalPlayer->LookupSequence("Man_Gun");
			//oh my god...
			const char* model = STRING(pLocalPlayer->GetModelName());
			const char* fixedModelName = STRING(AllocPooledString(model));
			if (NPCAnim <= 0)
			{
				Warning("npc_player: Using pre-selected model. Check debug_check_incompatible_models for more info.\n");
				modelName = g_charAvailableModels[randomChoiceModels];
			}
			else
			{
				modelName = fixedModelName;
			}
		}
		else
		{
			modelName = g_charAvailableModels[randomChoiceModels];
		}
	}
	else
	{
		modelName = g_charAvailableModels[randomChoiceModels];
	}

	SetModel(modelName);

	//Give him a random amount of grenades on spawn
	if (g_pGameRules->IsSkillLevel(SKILL_HARD))
	{
		m_iNumGrenades = random->RandomInt(4, 6);
	}
	else if (g_pGameRules->IsSkillLevel(SKILL_VERYHARD))
	{
		m_iNumGrenades = random->RandomInt(2, 3);
	}
	else if (g_pGameRules->IsSkillLevel(SKILL_NIGHTMARE))
	{
		m_iNumGrenades = random->RandomInt(0, 2);
	}
	else
	{
		m_iNumGrenades = random->RandomInt(8, 12);
	}

	GiveOutline(Vector(26, 77, 153));

	m_fIsPlayer = true;
	m_fIsAce = true;

	CapabilitiesAdd( bits_CAP_ANIMATEDFACE );
	CapabilitiesAdd( bits_CAP_MOVE_SHOOT );
	CapabilitiesAdd(bits_CAP_MOVE_JUMP);
	CapabilitiesAdd( bits_CAP_DOORS_GROUP );

	GiveWeapons();

	BaseClass::Spawn();

	SetCollisionGroup(COLLISION_GROUP_NPC_ACTOR);
}

void CNPC_Player::GiveWeapons(void)
{
	int nWeaponsMid = ARRAYSIZE(g_charNPCMidRangeWeapons);
	int randomChoiceMid = rand() % nWeaponsMid;
	const char* pRandomNameMid = g_charNPCMidRangeWeapons[randomChoiceMid];
	GiveWeapon(pRandomNameMid);
	DevMsg("PLAYER: GIVING MID RANGE WEAPON %s.\n", pRandomNameMid);

	int nWeaponsShort = ARRAYSIZE(g_charNPCShortRangeWeapons);
	int randomChoiceShort = rand() % nWeaponsShort;
	const char* pRandomNameShort = g_charNPCShortRangeWeapons[randomChoiceShort];
	GiveWeapon(pRandomNameShort);
	DevMsg("PLAYER: GIVING SHORT RANGE WEAPON %s.\n", pRandomNameShort);
}

void CNPC_Player::GiveWeapon(const char* iszWeaponName)
{
	CBaseCombatWeapon* pWeapon = Weapon_Create(iszWeaponName);
	if (!pWeapon)
	{
		Warning("Couldn't create weapon %s to give NPC %s.\n", iszWeaponName, GetEntityName());
		return;
	}

	// If I have a weapon already, drop it
	if (GetActiveWeapon())
	{
		GetActiveWeapon()->DestroyItem();
	}

	// If I have a name, make my weapon match it with "_weapon" appended
	if (GetEntityName() != NULL_STRING)
	{
		pWeapon->SetName(AllocPooledString(UTIL_VarArgs("%s_weapon", GetEntityName())));
	}

	Weapon_Equip(pWeapon);

	// Handle this case
	OnGivenWeapon(pWeapon);
}

//-----------------------------------------------------------------------------
// Purpose:
// Input  :
// Output :
//-----------------------------------------------------------------------------
void CNPC_Player::Precache()
{
	for (const char* i : g_charAvailableModels)
	{
		CBaseEntity::PrecacheModel(i);
	}

	for (const char* i : g_charNPCShortRangeWeapons)
	{
		UTIL_PrecacheOther(i);
	}

	for (const char* i : g_charNPCMidRangeWeapons)
	{
		UTIL_PrecacheOther(i);
	}

	UTIL_PrecacheOther( "item_healthvial" );
	UTIL_PrecacheOther( "weapon_frag" );
	UTIL_PrecacheOther( "item_ammo_ar2_altfire" );
	UTIL_PrecacheOther( "item_ammo_smg1_grenade" );
	UTIL_PrecacheOther( "item_oicw_grenade" );

	BaseClass::Precache();
}

Class_T	CNPC_Player::Classify(void)
{
	return CLASS_PLAYER_NPC;
}

float CNPC_Player::GetSequenceGroundSpeed(CStudioHdr* pStudioHdr, int iSequence)
{
	return fr_new_normspeed.GetFloat();
}

//-----------------------------------------------------------------------------
// Purpose: Soldiers use CAN_RANGE_ATTACK2 to indicate whether they can throw
//			a grenade. Because they check only every half-second or so, this
//			condition must persist until it is updated again by the code
//			that determines whether a grenade can be thrown, so prevent the 
//			base class from clearing it out. (sjb)
//-----------------------------------------------------------------------------
void CNPC_Player::ClearAttackConditions( )
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
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
int CNPC_Player::OnTakeDamage_Alive(const CTakeDamageInfo& info)
{
	CTakeDamageInfo subInfo = info;

	if (subInfo.GetDamageType() != DMG_GENERIC)
	{
		Relationship_t* relations = FindEntityRelationship(info.GetAttacker());
		if (relations->disposition == D_LI && !npc_playerbot_friendlyfire.GetBool())
		{
			// no friendly fire.
			subInfo.SetDamage(0);
		}
		else
		{
			//only take half of the damage so we can be around for a bit longer.
			float flDamage = subInfo.GetDamage();
			float flNewDmg = (flDamage * 0.5);
			if (flDamage > flNewDmg)
			{
				flDamage = flNewDmg;
				subInfo.SetDamage(flDamage);
			}
		}
	}

	return BaseClass::OnTakeDamage_Alive(subInfo);
}

void CNPC_Player::NPCThink( void )
{
	// EXCEPTIONS
	if (GetEnemy() && (m_flSoonestWeaponSwitch < gpGlobals->curtime))
	{
		CBaseCombatWeapon* pActiveWeapon = GetActiveWeapon();
		if (pActiveWeapon)
		{
			DevMsg("PLAYER: SWITCHING.\n");
			if (SwitchToNextBestWeaponBot(pActiveWeapon))
			{
				m_flSoonestWeaponSwitch = gpGlobals->curtime + PLAYERNPC_FASTEST_SWITCH_TIME;
			}
		}
	}

	// regeneration
	if (IsAlive() && GetHealth() < GetMaxHealth() && (sv_regeneration.GetInt() == 1))
	{
		// Color to overlay on the screen while the player is taking damage

		if (gpGlobals->curtime > m_fTimeLastHurt + sv_regeneration_wait_time.GetFloat())
		{
			//Regenerate based on rate, and scale it by the frametime
			m_fRegenRemander += sv_regeneration_rate.GetFloat() * gpGlobals->frametime;

			if (m_fRegenRemander >= 1)
			{
				//If the regen interval is set, and the health is evenly divisible by that interval, don't regen.
				if (sv_regen_interval.GetFloat() > 0 && floor(m_iHealth / sv_regen_interval.GetFloat()) == m_iHealth / sv_regen_interval.GetFloat())
				{
					m_fRegenRemander = 0;
					DevMsg("PLAYER: Player %s health is at %i\n", GetModelName(), GetHealth());
				}
				else
				{
					TakeHealth(m_fRegenRemander, DMG_GENERIC);
					m_fRegenRemander = 0;
					DevMsg("PLAYER: Player %s health is at %i\n", GetModelName(), GetHealth());
				}
			}
		}
	}

	BaseClass::NPCThink();
}

float CNPC_Player::BotWeaponRangeDetermine(float flDist)
{
	if (flDist <= SKILL_MID_RANGE)
		return SKILL_SHORT_RANGE;

	if (flDist >= SKILL_SHORT_RANGE)
		return SKILL_MID_RANGE;

	return SKILL_MAX_RANGE;
}

CBaseCombatWeapon* CNPC_Player::GetNextBestWeaponBot(CBaseCombatWeapon* pCurrentWeapon)
{
	CBaseCombatWeapon* pCheck;
	CBaseCombatWeapon* pBest;// this will be used in the event that we don't find a weapon in the same category.

	pBest = NULL;

	for (int i = 0; i < WeaponCount(); ++i)
	{
		pCheck = GetWeapon(i);
		if (!pCheck)
			continue;

		if (pCheck->HasAnyAmmo())
		{
			if (GetEnemy())
			{
				float flDist;
				flDist = (GetLocalOrigin() - GetEnemy()->GetLocalOrigin()).Length();
				float weaponRange = BotWeaponRangeDetermine(flDist);

				if (weaponRange == SKILL_SHORT_RANGE)
				{
					DevMsg("PLAYER: SHORT RANGE DETECTED\n");
					for (const char* i : g_charNPCShortRangeWeapons)
					{
						if (FClassnameIs(pCheck, i))
						{
							DevMsg("PLAYER: SETTING %s AS BEST SHORT RANGE.\n", pCheck->GetClassname());
							// if this weapon is useable, flag it as the best
							pBest = pCheck;
						}
					}
				}
				else if (weaponRange == SKILL_MID_RANGE)
				{
					DevMsg("PLAYER: MID RANGE DETECTED\n");
					for (const char* i : g_charNPCMidRangeWeapons)
					{
						if (FClassnameIs(pCheck, i))
						{
							DevMsg("PLAYER: SETTING %s AS BEST MID RANGE.\n", pCheck->GetClassname());
							// if this weapon is useable, flag it as the best
							pBest = pCheck;
						}
					}
				}
				else
				{
					CBaseCombatWeapon* pActiveWeapon = GetActiveWeapon();
					DevMsg("PLAYER: SETTING CURRENT WEAPON AS BEST.\n");
					return pActiveWeapon;
				}
			}
		}
		else
		{
			CBaseCombatWeapon* pActiveWeapon = GetActiveWeapon();
			DevMsg("PLAYER: SETTING CURRENT WEAPON AS BEST.\n");
			return pActiveWeapon;
		}
	}

	// if we make it here, we've checked all the weapons and found no useable 
	// weapon in the same catagory as the current weapon. 

	// if pBest is null, we didn't find ANYTHING. Shouldn't be possible- should always 
	// at least get the crowbar, but ya never know.
	return pBest;
}

bool CNPC_Player::SwitchToNextBestWeaponBot(CBaseCombatWeapon* pCurrent)
{
	CBaseCombatWeapon* pNewWeapon = GetNextBestWeaponBot(pCurrent);

	if ((pNewWeapon != NULL))
	{
		return Weapon_Switch(pNewWeapon);
	}

	return false;
}

bool CNPC_Player::Weapon_Switch(CBaseCombatWeapon* pWeapon)
{
	if (!Weapon_OwnsThisType(pWeapon->GetClassname()))
	{
		GiveWeapon(pWeapon->GetClassname());
	}

	// Already have it out?
	if (m_hActiveWeapon.Get() == pWeapon)
	{
		if (!m_hActiveWeapon->IsWeaponVisible() || m_hActiveWeapon->IsHolstered())
			return m_hActiveWeapon->Deploy();
		return false;
	}

	if (!Weapon_CanSwitchTo(pWeapon))
	{
		return false;
	}

	if (m_hActiveWeapon)
	{
		if (!m_hActiveWeapon->Holster(pWeapon))
		{
			return false;
		}
		else
		{
			m_hActiveWeapon->AddEffects(EF_NODRAW);
		}
	}

	pWeapon->RemoveEffects(EF_NODRAW);
	m_hActiveWeapon = pWeapon;

	DevMsg("PLAYER: SWITCHED WEAPON TO: %s\n", pWeapon->GetClassname());

	return pWeapon->Deploy();
}

//-----------------------------------------------------------------------------
// Purpose: Allows for modification of the interrupt mask for the current schedule.
//			In the most cases the base implementation should be called first.
//-----------------------------------------------------------------------------
void CNPC_Player::BuildScheduleTestBits( void )
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
int CNPC_Player::SelectSchedule ( void )
{
	m_FollowBehavior.SetFollowTarget(UTIL_GetNearestPlayer(GetAbsOrigin()));
	m_FollowBehavior.SetParameters(AIF_SIMPLE);

	return BaseClass::SelectSchedule();
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
float CNPC_Player::GetHitgroupDamageMultiplier( int iHitGroup, const CTakeDamageInfo &info )
{
	return BaseClass::GetHitgroupDamageMultiplier( iHitGroup, info );
}

void CNPC_Player::OnListened()
{
	BaseClass::OnListened();

	if ( HasCondition( COND_HEAR_DANGER ) && HasCondition( COND_HEAR_PHYSICS_DANGER ) )
	{
		if ( HasInterruptCondition( COND_HEAR_DANGER ) )
		{
			ClearCondition( COND_HEAR_PHYSICS_DANGER );
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : &info - 
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
void CNPC_Player::Event_Killed( const CTakeDamageInfo &info )
{
	RemoveGlowEffect();

	if (m_hActiveWeapon)
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
		else if (FClassnameIs(GetActiveWeapon(), "weapon_oicw"))
		{
			pItem = DropItem("item_oicw_grenade", WorldSpaceCenter() + RandomVector(-4, 4), RandomAngle(0, 360));
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

	BaseClass::Event_Killed( info );
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : &info - 
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
bool CNPC_Player::IsLightDamage( const CTakeDamageInfo &info )
{
	return BaseClass::IsLightDamage( info );
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : &info - 
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
bool CNPC_Player::IsHeavyDamage( const CTakeDamageInfo &info )
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

Activity CNPC_Player::NPC_TranslateActivity(Activity eNewActivity)
{
	return BaseClass::NPC_TranslateActivity(eNewActivity);
}