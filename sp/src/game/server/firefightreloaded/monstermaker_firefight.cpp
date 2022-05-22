//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: An entity that creates NPCs in the game.
//
//=============================================================================//

#include "cbase.h"
#include "datacache/imdlcache.h"
#include "entityapi.h"
#include "entityoutput.h"
#include "ai_basenpc.h"
#include "monstermaker_firefight.h"
#include "TemplateEntities.h"
#include "ndebugoverlay.h"
#include "mapentities.h"
#include "IEffects.h"
#include "props.h"
#include "npc_metropolice.h"
#include "npc_strider.h"
#include "npc_scanner.h"
#include "globalstate.h"
#include "filesystem.h"
#include "KeyValues.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

ConVar sk_initialspawnertime("sk_initialspawnertime", "5", FCVAR_CHEAT);
ConVar sk_spawnrareenemies("sk_spawnrareenemies", "1", FCVAR_ARCHIVE);
ConVar sk_spawnerhidefromplayer("sk_spawnerhidefromplayer", "1", FCVAR_ARCHIVE);
ConVar sk_spawner_npc_ragdoll_fade("sk_spawner_npc_ragdoll_fade", "1", FCVAR_ARCHIVE);
ConVar sk_spawner_largenpc_spawndelay("sk_spawner_largenpc_spawntime", "300", FCVAR_CHEAT);
ConVar debug_spawner_info("debug_spawner_info", "0", FCVAR_CHEAT);
ConVar debug_spawner_disable("debug_spawner_disable", "0", FCVAR_CHEAT);
ConVar sk_spawner_defaultspawnlist("sk_spawner_defaultspawnlist", "scripts/spawnlists/default.txt", FCVAR_ARCHIVE);

//spawn lists (TODO: use KeyValues files)
const char *g_CombineSoldierWeapons[] =
{
	"weapon_smg1",
	"weapon_ar2"
};

const char *g_MetropoliceWeapons[] =
{
	"weapon_smg1",
	"weapon_pistol",
	"weapon_stunstick"
};

//precache list
const char* g_Weapons[] =
{
	"weapon_smg1",
	"weapon_ar2",
	"weapon_shotgun",
	"weapon_pistol",
	"weapon_stunstick",
	"weapon_crowbar"
};

static void DispatchActivate( CBaseEntity *pEntity )
{
	bool bAsyncAnims = mdlcache->SetAsyncLoad( MDLCACHE_ANIMBLOCK, false );
	pEntity->Activate();
	mdlcache->SetAsyncLoad( MDLCACHE_ANIMBLOCK, bAsyncAnims );
}

LINK_ENTITY_TO_CLASS(npc_maker_firefight, CNPCMakerFirefight);

//-------------------------------------
BEGIN_DATADESC(CNPCMakerFirefight)
	
	DEFINE_KEYFIELD( m_nMaxLiveChildren,		FIELD_INTEGER,	"MaxLiveChildren" ),
	DEFINE_KEYFIELD( m_flSpawnFrequency,		FIELD_FLOAT,	"SpawnFrequency" ),
	DEFINE_KEYFIELD( m_bDisabled,			FIELD_BOOLEAN,	"StartDisabled" ),
	DEFINE_KEYFIELD(m_bLargeNPCsEnabled, FIELD_BOOLEAN, "AllowLargeNPCs" ),
	DEFINE_KEYFIELD( m_iszNPCClassname, FIELD_STRING, "NPCType" ),
	DEFINE_KEYFIELD( m_ChildTargetName, FIELD_STRING, "NPCTargetname" ),
	DEFINE_KEYFIELD( m_SquadName, FIELD_STRING, "NPCSquadName" ),
	DEFINE_KEYFIELD( m_spawnEquipment, FIELD_STRING, "additionalequipment" ),
	DEFINE_KEYFIELD( m_strHintGroup, FIELD_STRING, "NPCHintGroup" ),
	DEFINE_KEYFIELD( m_RelationshipString, FIELD_STRING, "Relationship" ),
	DEFINE_KEYFIELD( m_nMaxLiveRareNPCs, FIELD_INTEGER, "MaxLiveRareNPCs" ),
	DEFINE_KEYFIELD( m_nRareNPCRarity, FIELD_INTEGER, "RareNPCRarity" ),

	DEFINE_FIELD(	m_nLiveChildren,		FIELD_INTEGER ),
	DEFINE_FIELD(	m_nLiveRareNPCs,		FIELD_INTEGER ),
	DEFINE_FIELD(	m_flLastLargeNPCSpawn,	FIELD_TIME),

	// Inputs
	DEFINE_INPUTFUNC( FIELD_VOID,	"Spawn",	InputSpawnNPC ),
	DEFINE_INPUTFUNC( FIELD_VOID,	"SpawnRare", InputSpawnRareNPC ),
	DEFINE_INPUTFUNC( FIELD_VOID,	"Enable",	InputEnable ),
	DEFINE_INPUTFUNC( FIELD_VOID,	"Disable",	InputDisable ),
	DEFINE_INPUTFUNC( FIELD_VOID,	"Toggle",	InputToggle ),
	DEFINE_INPUTFUNC( FIELD_INTEGER, "SetMaxLiveChildren", InputSetMaxLiveChildren ),
	DEFINE_INPUTFUNC( FIELD_FLOAT,	 "SetSpawnFrequency", InputSetSpawnFrequency ),
	DEFINE_INPUTFUNC( FIELD_INTEGER, "SetRareNPCRarity", InputSetRareNPCRarity ),

	// Outputs
	DEFINE_OUTPUT( m_OnAllLiveChildrenDead,	"OnAllLiveChildrenDead" ),
	DEFINE_OUTPUT( m_OnSpawnNPC,		"OnSpawnNPC" ),
	DEFINE_OUTPUT( m_OnSpawnRareNPC,		"OnSpawnRareNPC" ),

	// Function Pointers
	DEFINE_THINKFUNC( MakerThink ),

	DEFINE_FIELD( m_hIgnoreEntity, FIELD_EHANDLE ),
	DEFINE_KEYFIELD( m_iszIngoreEnt, FIELD_STRING, "IgnoreEntity" ), 
END_DATADESC()

//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------
CNPCMakerFirefight::CNPCMakerFirefight(void)
{
	m_spawnEquipment = NULL_STRING;
}

//-----------------------------------------------------------------------------
// Purpose: Spawn
//-----------------------------------------------------------------------------
void CNPCMakerFirefight::Spawn(void)
{
	SetSolid( SOLID_NONE );
	m_nLiveChildren		= 0;
	m_nLiveRareNPCs		= 0;
	m_flLastLargeNPCSpawn = 0;
	Precache();

	//m_spawnflags |= SF_NPCMAKER_FADE;

	//Start on?
	if ( m_bDisabled == false)
	{
		SetThink(&CNPCMakerFirefight::MakerThink);
		SetNextThink(gpGlobals->curtime + sk_initialspawnertime.GetFloat());
	}
	else
	{
		//wait to be activated.
		SetThink(&CNPCMakerFirefight::SUB_DoNothing);
	}
}

//-----------------------------------------------------------------------------
// Purpose: Precache the target NPC
//-----------------------------------------------------------------------------
void CNPCMakerFirefight::Precache(void)
{
	BaseClass::Precache();

	if (!m_hSpawnListController)
	{
		m_hSpawnListController = new CRandNPCLoader(this);
	}

	int nWeapons = ARRAYSIZE(g_Weapons);
	for (int i = 0; i < nWeapons; ++i)
	{
		UTIL_PrecacheOther(g_Weapons[i]);
	}

	if (m_spawnEquipment != NULL_STRING)
	{
		UTIL_PrecacheOther(STRING(m_spawnEquipment));
	}

	m_hSpawnListController->PrecacheSpawnlist();
}

//-----------------------------------------------------------------------------
// Purpose: Creates a new NPC every so often.
//-----------------------------------------------------------------------------
void CNPCMakerFirefight::MakerThink(void)
{
	SetNextThink(gpGlobals->curtime + m_flSpawnFrequency);

	//rare npcs will be handled by the spawnlist
	MakeNPC();

	if (m_nLiveChildren < 0)
	{
		m_nLiveChildren = 0;
	}

	if (m_nLiveRareNPCs < 0)
	{
		m_nLiveRareNPCs = 0;
	}

	if (m_nRareNPCRarity > SKILL_MEDIUM)
	{
		m_nRareNPCRarity = m_nRareNPCRarity - g_pGameRules->GetSkillLevel();
	}

	if (m_nRareNPCRarity <= 0)
	{
		m_nRareNPCRarity = 1;
	}

	if (debug_spawner_info.GetBool())
	{
		m_debugOverlays |= OVERLAY_TEXT_BIT;
	}
	else
	{
		if (m_debugOverlays & OVERLAY_TEXT_BIT)
		{
			m_debugOverlays &= ~OVERLAY_TEXT_BIT;
		}
	}
}

int CNPCMakerFirefight::DrawDebugTextOverlays(void)
{
	int text_offset = BaseClass::DrawDebugTextOverlays();

	if (m_debugOverlays & OVERLAY_TEXT_BIT)
	{
		char tempstr[512];
		Q_snprintf(tempstr, sizeof(tempstr), "NPCs Spawned: %i/%i", m_nLiveChildren, m_nMaxLiveChildren);
		EntityText(text_offset, tempstr, 0);
		text_offset++;
		Q_snprintf(tempstr, sizeof(tempstr), "Rare NPCs Spawned: %i/%i", m_nLiveRareNPCs, m_nMaxLiveRareNPCs);
		EntityText(text_offset, tempstr, 0);
		text_offset++;
		Q_snprintf(tempstr, sizeof(tempstr), "Rare NPC Rarity: 1 in %i", m_nRareNPCRarity);
		EntityText(text_offset, tempstr, 0);
		text_offset++;
		Q_snprintf(tempstr, sizeof(tempstr), "NPC Squad: %s", STRING(m_SquadName));
		EntityText(text_offset, tempstr, 0);
		text_offset++;
		Q_snprintf(tempstr, sizeof(tempstr), "Spawn Frequency: %f", m_flSpawnFrequency);
		EntityText(text_offset, tempstr, 0);
		text_offset++;
		Q_snprintf(tempstr, sizeof(tempstr), "Disabled: %i", m_bDisabled);
		EntityText(text_offset, tempstr, 0);
		text_offset++;
		Q_snprintf(tempstr, sizeof(tempstr), "Large NPCs Enabled: %i", m_bLargeNPCsEnabled);
		EntityText(text_offset, tempstr, 0);
		text_offset++;
	}
	return text_offset;

}

//-----------------------------------------------------------------------------
// A not-very-robust check to see if a human hull could fit at this location.
// used to validate spawn destinations.
//-----------------------------------------------------------------------------
bool CNPCMakerFirefight::HumanHullFits(const Vector &vecLocation)
{
	trace_t tr;
	UTIL_TraceHull( vecLocation,
					vecLocation + Vector( 0, 0, 1 ),
					NAI_Hull::Mins(HULL_HUMAN),
					NAI_Hull::Maxs(HULL_HUMAN),
					MASK_NPCSOLID,
					m_hIgnoreEntity,
					COLLISION_GROUP_NONE,
					&tr );

	if( tr.fraction == 1.0 )
		return true;

	return false;
}

//-----------------------------------------------------------------------------
// Purpose: Returns whether or not it is OK to make an NPC at this instant.
//-----------------------------------------------------------------------------
bool CNPCMakerFirefight::CanMakeNPC(bool bIgnoreSolidEntities)
{
	if (debug_spawner_disable.GetBool())
		return false;

	if ( gEntList.NumberOfEdicts() >= (MAX_EDICTS - g_fr_entitytolerance.GetInt()) )
		return false;

	if ((CAI_BaseNPC::m_nDebugBits & bits_debugDisableAI) == bits_debugDisableAI)
		return false;

	/*
	int iMinPlayersToSpawn = 0;

	for (int i = 1; i <= gpGlobals->maxClients; i++)
	{
		CBasePlayer	*pPlayer = UTIL_PlayerByIndex(i);
		if (pPlayer != NULL)
		{
			iMinPlayersToSpawn++;
		}
	}

	if (iMinPlayersToSpawn < sk_spawnerminclientstospawn.GetInt() && g_pGameRules->IsMultiplayer())
		return false;*/

	if ( m_nMaxLiveChildren > 0 && (m_nLiveChildren >= m_nMaxLiveChildren) )
	{// not allowed to make a new one yet. Too many live ones out right now.
		return false;
	}

	if ( m_iszIngoreEnt != NULL_STRING )
	{
		m_hIgnoreEntity = gEntList.FindEntityByName( NULL, m_iszIngoreEnt );
	}

	Vector mins = GetAbsOrigin() - Vector( 34, 34, 0 );
	Vector maxs = GetAbsOrigin() + Vector( 34, 34, 0 );
	maxs.z = GetAbsOrigin().z;
	
	// If we care about not hitting solid entities, look for 'em
	if ( !bIgnoreSolidEntities )
	{
		CBaseEntity *pList[128];

		int count = UTIL_EntitiesInBox( pList, 128, mins, maxs, FL_CLIENT|FL_NPC );
		if ( count )
		{
			//Iterate through the list and check the results
			for ( int i = 0; i < count; i++ )
			{
				//Don't build on top of another entity
				if ( pList[i] == NULL )
					continue;

				//If one of the entities is solid, then we may not be able to spawn now
				if ( ( pList[i]->GetSolidFlags() & FSOLID_NOT_SOLID ) == false )
				{
					// Since the outer method doesn't work well around striders on account of their huge bounding box.
					// Find the ground under me and see if a human hull would fit there.
					trace_t tr;
					UTIL_TraceHull( GetAbsOrigin() + Vector( 0, 0, 2 ),
									GetAbsOrigin() - Vector( 0, 0, 8192 ),
									NAI_Hull::Mins(HULL_HUMAN),
									NAI_Hull::Maxs(HULL_HUMAN),
									MASK_NPCSOLID,
									m_hIgnoreEntity,
									COLLISION_GROUP_NONE,
									&tr );

					if( !HumanHullFits( tr.endpos + Vector( 0, 0, 1 ) ) )
					{
						return false;
					}
				}
			}
		}
	}

	// Do we need to check to see if the player's looking?
	if (sk_spawnerhidefromplayer.GetBool())
	{
		for ( int i = 1; i <= gpGlobals->maxClients; i++ )
		{
			CBasePlayer *pPlayer = UTIL_PlayerByIndex(i);
			if ( pPlayer )
			{
				// Only spawn if the player's looking away from me
				if( pPlayer->FInViewCone( GetAbsOrigin() ) && pPlayer->FVisible( GetAbsOrigin() ) )
				{
					if ( !(pPlayer->GetFlags() & FL_NOTARGET) )
						return false;
					DevMsg( 2, "Spawner %s spawning even though seen due to notarget\n", STRING( GetEntityName() ) );
				}
			}
		}
	}

	return true;
}

//-----------------------------------------------------------------------------
// Purpose: Returns whether or not it is OK to make an NPC at this instant.
//-----------------------------------------------------------------------------
bool CNPCMakerFirefight::CanMakeRareNPC()
{
	if (!CanMakeNPC())
		return false;

	if (m_nMaxLiveRareNPCs > 0 && m_nLiveRareNPCs >= m_nMaxLiveRareNPCs)
	{
		return false;
	}

	return true;
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : *pVictim - 
//-----------------------------------------------------------------------------
bool CNPCMakerFirefight::KilledNotice(CBaseEntity *pVictim)
{
	// ok, we've gotten the deathnotice from our child, now clear out its owner if we don't want it to fade.
	m_nLiveChildren--;

	// If we're here, we're getting erroneous death messages from children we haven't created
	AssertMsg(m_nLiveChildren >= 0, "npc_maker_firefight receiving child death notice but thinks has no children\n");

	if (pVictim->m_isRareEntity)
	{
		// ok, we've gotten the deathnotice from our child, now clear out its owner if we don't want it to fade.
		m_nLiveRareNPCs--;

		// If we're here, we're getting erroneous death messages from children we haven't created
		AssertMsg(m_nLiveRareNPCs >= 0, "npc_maker_firefight receiving child death notice but thinks has no children (RARE)\n");
	}

	return true;
}

void CNPCMakerFirefight::DeathNotice(CBaseEntity* pVictim)
{
	if (pVictim->GetOwnerEntity() == NULL)
		return;

	if (pVictim->m_isLargeEntity)
		return;

	KilledNotice(pVictim);
}

void AllyAlert()
{
	//alert all players.
	for (int i = 1; i <= gpGlobals->maxClients; i++)
	{
		CBasePlayer* pPlayer = UTIL_PlayerByIndex(i);
		if (pPlayer)
		{
			CFmtStr hint;
			hint.sprintf("#Valve_Hud_AllySpawned");
			pPlayer->ShowLevelMessage(hint.Access());
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: Creates the NPC.
//-----------------------------------------------------------------------------
void CNPCMakerFirefight::MakeNPC()
{
	if (!CanMakeNPC())
		return;

	bool newNPC = m_hSpawnListController->LoadNPC();

	if (!newNPC)
		return;

	if (m_hSpawnListController->m_bIsRare)
	{
		if (sk_spawnrareenemies.GetBool())
		{
			int rarenpcrandom = random->RandomInt(0, m_nRareNPCRarity);

			if (!CanMakeRareNPC() || rarenpcrandom > 0)
			{
				MakeNPC();
				return;
			}
		}
	}

	//if any player has the same level we should spawn at, we'll spawn.
	bool atMinLevel = false;

	for (int i = 1; i <= gpGlobals->maxClients; i++)
	{
		CBasePlayer* pPlayer = UTIL_PlayerByIndex(i);
		if (pPlayer)
		{
			int iPlayerLevel = m_hSpawnListController->m_iMinPlayerLevel;

			if (pPlayer->GetLevel() >= iPlayerLevel)
			{
				atMinLevel = true;
				break;
			}
		}
	}

	if (!atMinLevel)
		return;

	if (m_hSpawnListController->m_spawnEquipment != NULL_STRING)
	{
		m_spawnEquipment = m_hSpawnListController->m_spawnEquipment;
		UTIL_PrecacheOther(STRING(m_spawnEquipment));
	}

	const char* pRandomName = m_hSpawnListController->m_szClassname;
	CAI_BaseNPC* pent = (CAI_BaseNPC*)CreateEntityByName(pRandomName);

	if (!pent)
	{
		Warning("npc_maker_firefight: Entity classname does not exist in database.\n");
		return;
	}

	// ------------------------------------------------
	//  Intialize spawned NPC's relationships
	// ------------------------------------------------
	pent->SetRelationshipString(m_RelationshipString);

	m_OnSpawnNPC.Set(pent, pent, this);

	pent->SetAbsOrigin(GetAbsOrigin());

	// Strip pitch and roll from the spawner's angles. Pass only yaw to the spawned NPC.
	QAngle angles = GetAbsAngles();
	angles.x = 0.0;
	angles.z = 0.0;
	pent->SetAbsAngles(angles);

	pent->AddSpawnFlags(SF_NPC_FALL_TO_GROUND);
	pent->AddSpawnFlags(SF_NPC_LONG_RANGE);

	if (sk_spawner_npc_ragdoll_fade.GetBool() /* || m_spawnflags & SF_NPCMAKER_FADE*/)
	{
		pent->AddSpawnFlags(SF_NPC_FADE_CORPSE);
	}

	if (m_spawnEquipment == NULL_STRING)
	{
		if (Q_stristr(pRandomName, "npc_metropolice"))
		{
			int nWeaponsPolice = ARRAYSIZE(g_MetropoliceWeapons);
			int randomChoicePolice = rand() % nWeaponsPolice;
			const char* pRandomNamePolice = g_MetropoliceWeapons[randomChoicePolice];
			pent->m_spawnEquipment = MAKE_STRING(pRandomNamePolice);
			pent->AddSpawnFlags(SF_METROPOLICE_ALLOWED_TO_RESPOND);
		}
		else if (Q_stristr(pRandomName, "npc_combine_shot"))
		{
			pent->m_spawnEquipment = MAKE_STRING("weapon_shotgun");
		}
		else if (Q_stristr(pRandomName, "npc_combine_s"))
		{
			int nWeaponsSoldier = ARRAYSIZE(g_CombineSoldierWeapons);
			int randomChoiceSoldier = rand() % nWeaponsSoldier;
			const char* pRandomNameSoldier = g_CombineSoldierWeapons[randomChoiceSoldier];
			pent->m_spawnEquipment = MAKE_STRING(pRandomNameSoldier);
		}
		else if (Q_stristr(pRandomName, "npc_combine_e"))
		{
			int nWeaponsSoldier = ARRAYSIZE(g_CombineSoldierWeapons);
			int randomChoiceSoldier = rand() % nWeaponsSoldier;
			const char* pRandomNameSoldier = g_CombineSoldierWeapons[randomChoiceSoldier];
			pent->m_spawnEquipment = MAKE_STRING(pRandomNameSoldier);
		}
		else if (Q_stristr(pRandomName, "npc_combine_p"))
		{
			int nWeaponsSoldier = ARRAYSIZE(g_CombineSoldierWeapons);
			int randomChoiceSoldier = rand() % nWeaponsSoldier;
			const char* pRandomNameSoldier = g_CombineSoldierWeapons[randomChoiceSoldier];
			pent->m_spawnEquipment = MAKE_STRING(pRandomNameSoldier);
		}
		else if (Q_stristr(pRandomName, "npc_combine_ace"))
		{
			int nWeaponsSoldier = ARRAYSIZE(g_CombineSoldierWeapons);
			int randomChoiceSoldier = rand() % nWeaponsSoldier;
			const char* pRandomNameSoldier = g_CombineSoldierWeapons[randomChoiceSoldier];
			pent->m_spawnEquipment = MAKE_STRING(pRandomNameSoldier);
		}
	}
	else
	{
		pent->m_spawnEquipment = m_spawnEquipment;
	}

	if (Q_stristr(pRandomName, "npc_playerbot"))
	{
		if (!g_fr_lonewolf.GetBool())
		{
			//alert all players.
			AllyAlert();
		}
		else
		{
			UTIL_Remove(pent);
			return;
		}
	}
	else if (Q_stristr(pRandomName, "npc_strider"))
	{
		pent->AddSpawnFlags(SF_CAN_STOMP_PLAYER);
	}
	else if (Q_stristr(pRandomName, "npc_cscanner"))
	{
		if (g_pGameRules->GetSkillLevel() > SKILL_HARD)
		{
			pent->AddSpawnFlags(SF_CSCANNER_STRIDER_SCOUT);
		}
	}
	else if (Q_stristr(pRandomName, "npc_antlion"))
	{
		if (GlobalEntity_GetState("antlion_allied") == GLOBAL_ON && g_pGameRules->GetGamemode() != FIREFIGHT_PRIMARY_ANTLIONASSAULT && !g_fr_lonewolf.GetBool())
		{
			//alert all players.
			AllyAlert();
		}
	}

	if (m_hSpawnListController->m_iNPCAttributePreset > 0 || m_hSpawnListController->m_iNPCAttributePreset < 0)
	{
		pent->m_bDisableInitAttributes = true;
	}

	pent->m_isRareEntity = m_hSpawnListController->m_bIsRare;
	pent->SetSquadName(m_SquadName);
	pent->SetHintGroup(m_strHintGroup);

	ChildPreSpawn(pent);

	DispatchSpawn(pent);
	pent->SetOwnerEntity(this);

	// adding this check to make sure strders will work properly...
	if (FClassnameIs(pent, "npc_strider") || 
		FClassnameIs(pent, "npc_helicopter") || 
		FClassnameIs(pent, "npc_combinegunship") || 
		FClassnameIs(pent, "npc_combinedropship"))
	{
		pent->m_isLargeEntity = true;

		if (m_bLargeNPCsEnabled)
		{
			if (m_flLastLargeNPCSpawn < gpGlobals->curtime)
			{
				m_flLastLargeNPCSpawn = gpGlobals->curtime + sk_spawner_largenpc_spawndelay.GetFloat();
			}
			else
			{
				UTIL_Remove(pent);
				return;
			}
		}
		else
		{
			UTIL_Remove(pent);
			return;
		}
	}

	DispatchActivate(pent);

	if (m_ChildTargetName != NULL_STRING)
	{
		// if I have a netname (overloaded), give the child NPC that name as a targetname
		pent->SetName(m_ChildTargetName);
	}

	if (m_hSpawnListController->m_iNPCAttributePreset > 0)
	{
		pent->m_pAttributes = LoadPresetFile(pent->GetClassname(), m_hSpawnListController->m_iNPCAttributePreset);
		pent->LoadInitAttributes();
	}

	ChildPostSpawn(pent);

	if (m_hSpawnListController->m_bIsRare)
	{
		//rare entities have their own value we must consider.
		m_nLiveRareNPCs++;
	}

	m_nLiveChildren++;// count this NPC
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : *pChild - 
//-----------------------------------------------------------------------------
void CNPCMakerFirefight::ChildPostSpawn(CAI_BaseNPC *pChild)
{
	// If I'm stuck inside any props, remove them
	bool bFound = true;
	while (bFound)
	{
		trace_t tr;
		UTIL_TraceHull(pChild->GetAbsOrigin(), pChild->GetAbsOrigin(), pChild->WorldAlignMins(), pChild->WorldAlignMaxs(), MASK_NPCSOLID, pChild, COLLISION_GROUP_NONE, &tr);
		//NDebugOverlay::Box( pChild->GetAbsOrigin(), pChild->WorldAlignMins(), pChild->WorldAlignMaxs(), 0, 255, 0, 32, 5.0 );
		if (tr.fraction != 1.0 && tr.m_pEnt)
		{
			if (FClassnameIs(tr.m_pEnt, "prop_physics"))
			{
				// Set to non-solid so this loop doesn't keep finding it
				tr.m_pEnt->AddSolidFlags(FSOLID_NOT_SOLID);
				UTIL_RemoveImmediate(tr.m_pEnt);
				continue;
			}
		}

		bFound = false;
	}
	if (m_hIgnoreEntity != NULL)
	{
		pChild->SetOwnerEntity(m_hIgnoreEntity);
	}
}


//-----------------------------------------------------------------------------
// Purpose: If this had a finite number of children, return true if they've all
//			been created.
//-----------------------------------------------------------------------------
bool CNPCMakerFirefight::IsDepleted()
{
	return false;
}


//-----------------------------------------------------------------------------
// Purpose: Toggle the spawner's state
//-----------------------------------------------------------------------------
void CNPCMakerFirefight::Toggle(void)
{
	if ( m_bDisabled )
	{
		Enable();
	}
	else
	{
		Disable();
	}
}


//-----------------------------------------------------------------------------
// Purpose: Start the spawner
//-----------------------------------------------------------------------------
void CNPCMakerFirefight::Enable(void)
{
	m_bDisabled = false;
	SetThink(&CNPCMakerFirefight::MakerThink);
	SetNextThink( gpGlobals->curtime );
}


//-----------------------------------------------------------------------------
// Purpose: Stop the spawner
//-----------------------------------------------------------------------------
void CNPCMakerFirefight::Disable(void)
{
	m_bDisabled = true;
	SetThink ( NULL );
}


//-----------------------------------------------------------------------------
// Purpose: Input handler that spawns an NPC.
//-----------------------------------------------------------------------------
void CNPCMakerFirefight::InputSpawnNPC(inputdata_t &inputdata)
{
	MakeNPC();
}

//-----------------------------------------------------------------------------
// Purpose: Input handler that spawns an NPC.
//-----------------------------------------------------------------------------
void CNPCMakerFirefight::InputSpawnRareNPC(inputdata_t &inputdata)
{
	MakeNPC();
}

//-----------------------------------------------------------------------------
// Purpose: Input hander that starts the spawner
//-----------------------------------------------------------------------------
void CNPCMakerFirefight::InputEnable(inputdata_t &inputdata)
{
	Enable();
}


//-----------------------------------------------------------------------------
// Purpose: Input hander that stops the spawner
//-----------------------------------------------------------------------------
void CNPCMakerFirefight::InputDisable(inputdata_t &inputdata)
{
	Disable();
}


//-----------------------------------------------------------------------------
// Purpose: Input hander that toggles the spawner
//-----------------------------------------------------------------------------
void CNPCMakerFirefight::InputToggle(inputdata_t &inputdata)
{
	Toggle();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CNPCMakerFirefight::InputSetMaxLiveChildren(inputdata_t &inputdata)
{
	m_nMaxLiveChildren = inputdata.value.Int();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CNPCMakerFirefight::InputSetMaxLiveRareNPCS(inputdata_t &inputdata)
{
	m_nMaxLiveRareNPCs = inputdata.value.Int();
}

void CNPCMakerFirefight::InputSetSpawnFrequency(inputdata_t &inputdata)
{
	m_flSpawnFrequency = inputdata.value.Float();
}

void CNPCMakerFirefight::InputSetRareNPCRarity(inputdata_t &inputdata)
{
	m_nRareNPCRarity = inputdata.value.Int();
}

CRandNPCLoader::CRandNPCLoader(CNPCMakerFirefight* pSpawner)
{
	if (!pSpawner)
		return;

	m_szClassname = "";
	m_iNPCAttributePreset = -1; // 0 = no attributes, random. -1 and below: no attributes at all.
	m_iMinPlayerLevel = 1;
	m_bIsRare = false;
	m_spawnEquipment = NULL_STRING;

	bool gamemodeMode = true;
	const char* gamemodeName = g_pGameRules->GetGamemodeName();
	const char* mapName = STRING(gpGlobals->mapname);

	if ((!gamemodeName || gamemodeName == NULL || strlen(gamemodeName) == 0) || 
		(!g_pGameRules->bSkipFuncCheck && !g_fr_spawneroldfunctionality.GetBool()))
	{
		gamemodeMode = false;
	}

	char szScriptPath[512];

	if (gamemodeMode)
	{
		Q_snprintf(szScriptPath, sizeof(szScriptPath), "scripts/spawnlists/gamemodes/%s.txt", gamemodeName);
	}
	else
	{
		Q_snprintf(szScriptPath, sizeof(szScriptPath), "scripts/spawnlists/maps/%s.txt", mapName);
	}

	const char* name = (gamemodeMode ? gamemodeName : mapName);
	KeyValues* pKV = new KeyValues(name);
	if (pKV->LoadFromFile(filesystem, szScriptPath))
	{
		data = pKV->MakeCopy();
		//set the parent if we are SURE we have loaded it.
		pParent = pSpawner;
		DevMsg("CRandNPCLoader: Spawnlist for %s loaded for %s\n", name, pSpawner->GetEntityName());
		loadedNPCData = true;
	}
	else
	{
		DevWarning("CRandNPCLoader: Failed to load %s spawnlist! File may not exist. Using default spawn list...\n", name);
		KeyValues* pKV = new KeyValues("default");
		if (pKV->LoadFromFile(filesystem, sk_spawner_defaultspawnlist.GetString()))
		{
			data = pKV->MakeCopy();
			//set the parent if we are SURE we have loaded it.
			pParent = pSpawner;
			DevMsg("CRandNPCLoader: Default Spawnlist loaded for %s\n", pSpawner->GetEntityName());
			loadedNPCData = true;
		}
		else
		{
			DevWarning("CRandNPCLoader: Failed to load default spawnlist! File may not exist. Spawner %s will not function properly.\n", pSpawner->GetEntityName());
			loadedNPCData = false;
		}
	}

	pKV->deleteThis();
}

bool CRandNPCLoader::LoadNPC(void)
{
	if (!loadedNPCData)
		return false;

	int count = 0;

	KeyValues* currentSpawnlist = CreateLevelBasedSpawnlist();

	if (currentSpawnlist == NULL)
		return false;

	//we get the number of keys in the file.
	for (KeyValues* kv = currentSpawnlist->GetFirstSubKey(); kv != NULL; kv = kv->GetNextKey())
	{
		count++;
	}

	DevMsg("CRandNPCLoader: Spawnlist has %i keys.\n", count);

	if (count > 0)
	{
		KeyValues* pNode = LoadNPCData(currentSpawnlist, count);

		if (pNode != NULL)
		{
			//CreateLevelBasedSpawnlist already does file checking for us.
			m_szClassname = pNode->GetString("classname");
			m_bIsRare = pNode->GetBool("rare", false);
			m_iMinPlayerLevel = pNode->GetInt("min_level", 1);
			m_iNPCAttributePreset = pNode->GetInt("preset", -1);
			m_spawnEquipment = MAKE_STRING(pNode->GetString("equipment"));
			return true;
		}
	}

	return false;
}

KeyValues* CRandNPCLoader::LoadNPCData(KeyValues* pData, int count)
{
	int itemdrop = itemdrop = random->RandomInt(1, count);
	//then, we randomize them.
	DevMsg("CRandNPCLoader: Trying to load Node ID %i\n", itemdrop);
	CFmtStr sectionName;
	sectionName.sprintf("%i", itemdrop);
	KeyValues* pNode = pData->FindKey(sectionName.Access());
	while (pNode)
	{
		//if we found a key, then give the player an item.
		DevMsg("CRandNPCLoader: Node %i found\n", itemdrop);
		return pNode;
	}

	DevWarning("CRandNPCLoader: NPC node %i is not loading. Check the spawnlist script file/s.\n", itemdrop);
	return NULL;
}

KeyValues* CRandNPCLoader::CreateLevelBasedSpawnlist(void)
{
	KeyValues* output = new KeyValues("spawnlist_generated");

	//check all the keys to generate a level based spawnlist.
	for (KeyValues* kv = data->GetFirstSubKey(); kv != NULL; kv = kv->GetNextKey())
	{
		int pPlayerLevel = kv->GetInt("min_level", 1);

		int curMinLevel = 1;

		for (int i = 1; i <= gpGlobals->maxClients; i++)
		{
			CBasePlayer* pPlayer = UTIL_PlayerByIndex(i);
			if (pPlayer)
			{
				if (pPlayer->GetLevel() > curMinLevel)
				{
					curMinLevel = pPlayer->GetLevel();
				}

				if (curMinLevel >= pPlayerLevel)
				{
					KeyValues* newKey = output->CreateNewKey();
					const char* pClassname = kv->GetString("classname");

					if (strlen(pClassname) > 0)
					{
						newKey->SetString("classname", pClassname);
						bool pIsRare = kv->GetBool("rare");
						newKey->SetBool("rare", pIsRare);
						newKey->SetInt("min_level", pPlayerLevel);
						int pNPCPreset = kv->GetInt("preset", -1);
						newKey->SetInt("preset", pNPCPreset);

						const char* pEquipment = kv->GetString("equipment");
						if (strlen(pEquipment) > 0)
						{
							newKey->SetString("equipment", pEquipment);
						}
					}
					else
					{
						return NULL;
					}
				}
			}
		}
	}

	if (!output->IsEmpty())
	{
		return output;
	}
	else
	{
		output->deleteThis();
		return NULL;
	}
}

void CRandNPCLoader::PrecacheSpawnlist(void)
{
	for (KeyValues* kv = data->GetFirstSubKey(); kv != NULL; kv = kv->GetNextKey())
	{
		const char* pClassname = kv->GetString("classname");

		if (strlen(pClassname) > 0)
		{
			UTIL_PrecacheOther(pClassname);
		}
	}
}
