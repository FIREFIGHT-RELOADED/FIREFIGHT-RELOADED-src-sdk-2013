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
#include "monstermaker_xeninvasion.h"
#include "TemplateEntities.h"
#include "ndebugoverlay.h"
#include "mapentities.h"
#include "IEffects.h"
#include "props.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

const char *g_charNPCSXenInvasionSupport[] =
{
	"npc_houndeye",
	"npc_bullsquid",
	"npc_agrunt",
};

static void DispatchActivate( CBaseEntity *pEntity )
{
	bool bAsyncAnims = mdlcache->SetAsyncLoad( MDLCACHE_ANIMBLOCK, false );
	pEntity->Activate();
	mdlcache->SetAsyncLoad( MDLCACHE_ANIMBLOCK, bAsyncAnims );
}

LINK_ENTITY_TO_CLASS(npc_maker_xeninvasion, CNPCMakerXenInvasion);

//-------------------------------------
BEGIN_DATADESC(CNPCMakerXenInvasion)
	
	DEFINE_KEYFIELD( m_nMaxLiveChildren,		FIELD_INTEGER,	"MaxLiveChildren" ),
	DEFINE_KEYFIELD( m_flSpawnFrequency,		FIELD_FLOAT,	"SpawnFrequency" ),
	DEFINE_KEYFIELD( m_bDisabled,			FIELD_BOOLEAN,	"StartDisabled" ),
	DEFINE_KEYFIELD( m_iszNPCClassname, FIELD_STRING, "NPCType" ),
	DEFINE_KEYFIELD( m_ChildTargetName, FIELD_STRING, "NPCTargetname" ),
	DEFINE_KEYFIELD( m_SquadName, FIELD_STRING, "NPCSquadName" ),
	DEFINE_KEYFIELD( m_spawnEquipment, FIELD_STRING, "additionalequipment" ),
	DEFINE_KEYFIELD( m_strHintGroup, FIELD_STRING, "NPCHintGroup" ),
	DEFINE_KEYFIELD( m_RelationshipString, FIELD_STRING, "Relationship" ),

	DEFINE_FIELD(	m_nLiveChildren,		FIELD_INTEGER ),

	// Inputs
	DEFINE_INPUTFUNC( FIELD_VOID,	"Spawn",	InputSpawnNPC ),
	DEFINE_INPUTFUNC( FIELD_VOID,	"Enable",	InputEnable ),
	DEFINE_INPUTFUNC( FIELD_VOID,	"Disable",	InputDisable ),
	DEFINE_INPUTFUNC( FIELD_VOID,	"Toggle",	InputToggle ),
	DEFINE_INPUTFUNC( FIELD_INTEGER, "SetMaxLiveChildren", InputSetMaxLiveChildren ),
	DEFINE_INPUTFUNC( FIELD_FLOAT,	 "SetSpawnFrequency", InputSetSpawnFrequency ),

	// Outputs
	DEFINE_OUTPUT( m_OnAllLiveChildrenDead,	"OnAllLiveChildrenDead" ),
	DEFINE_OUTPUT( m_OnSpawnNPC,		"OnSpawnNPC" ),

	// Function Pointers
	DEFINE_THINKFUNC( MakerThink ),

	DEFINE_FIELD( m_hIgnoreEntity, FIELD_EHANDLE ),
	DEFINE_KEYFIELD( m_iszIngoreEnt, FIELD_STRING, "IgnoreEntity" ), 
END_DATADESC()

//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------
CNPCMakerXenInvasion::CNPCMakerXenInvasion(void)
{
	m_spawnEquipment = NULL_STRING;
}

//-----------------------------------------------------------------------------
// Purpose: Spawn
//-----------------------------------------------------------------------------
void CNPCMakerXenInvasion::Spawn(void)
{
	SetSolid( SOLID_NONE );
	m_nLiveChildren		= 0;
	Precache();

	m_spawnflags |= SF_NPCMAKER_FADE;

	ConVar *sk_initialspawnertime = cvar->FindVar("sk_initialspawnertime");

	//Start on?
	if ( m_bDisabled == false )
	{
		SetThink(&CNPCMakerXenInvasion::MakerThink);
		SetNextThink(gpGlobals->curtime + sk_initialspawnertime->GetFloat());
	}
	else
	{
		//wait to be activated.
		SetThink(&CNPCMakerXenInvasion::SUB_DoNothing);
	}
}

//-----------------------------------------------------------------------------
// Purpose: Precache the target NPC
//-----------------------------------------------------------------------------
void CNPCMakerXenInvasion::Precache(void)
{
	BaseClass::Precache();

	int i;

	int nNPCsPrecache = ARRAYSIZE(g_charNPCSXenInvasionSupport);

	for (i = 0; i < nNPCsPrecache; ++i)
	{
		UTIL_PrecacheOther(g_charNPCSXenInvasionSupport[nNPCsPrecache]);
	}

	/*
	const char *pszNPCName = STRING(m_iszNPCClassname);
	if (!pszNPCName || !pszNPCName[0])
	{
		Warning("npc_maker_firefight %s has no specified NPC-to-spawn classname.\n", STRING(GetEntityName()));
	}
	else
	{
		UTIL_PrecacheOther(pszNPCName);
	}
	*/
}

//-----------------------------------------------------------------------------
// Purpose: Creates a new NPC every so often.
//-----------------------------------------------------------------------------
void CNPCMakerXenInvasion::MakerThink(void)
{
	SetNextThink(gpGlobals->curtime + m_flSpawnFrequency);
	if (HasSpawnFlags(SF_NPCMAKER_DOUBLETROUBLE))
	{
		MakeNPC();
		SetNextThink(gpGlobals->curtime + 0.5);
		MakeNPC();
	}
	else
	{
		MakeNPC();
	}
}

//-----------------------------------------------------------------------------
// A not-very-robust check to see if a human hull could fit at this location.
// used to validate spawn destinations.
//-----------------------------------------------------------------------------
bool CNPCMakerXenInvasion::HumanHullFits(const Vector &vecLocation)
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
bool CNPCMakerXenInvasion::CanMakeNPC(bool bIgnoreSolidEntities)
{
	int iMinPlayersToSpawn = 0;

	for (int i = 1; i <= gpGlobals->maxClients; i++)
	{
		CBasePlayer	*pPlayer = UTIL_PlayerByIndex(i);
		if (pPlayer != NULL)
		{
			iMinPlayersToSpawn++;
		}
	}

	ConVar *sk_spawnerminclientstospawn = cvar->FindVar("sk_spawnerminclientstospawn");
	if (iMinPlayersToSpawn < sk_spawnerminclientstospawn->GetInt())
		return false;

	if ( m_nMaxLiveChildren > 0 && m_nLiveChildren >= m_nMaxLiveChildren )
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

	ConVar *sk_spawnerhidefromplayer = cvar->FindVar("sk_spawnerhidefromplayer");

	// Do we need to check to see if the player's looking?
	if (sk_spawnerhidefromplayer->GetBool())
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
// Purpose: 
// Input  : *pVictim - 
//-----------------------------------------------------------------------------
void CNPCMakerXenInvasion::DeathNotice(CBaseEntity *pVictim)
{
	// ok, we've gotten the deathnotice from our child, now clear out its owner if we don't want it to fade.
	m_nLiveChildren--;

	// If we're here, we're getting erroneous death messages from children we haven't created
	AssertMsg(m_nLiveChildren >= 0, "npc_maker_firefight receiving child death notice but thinks has no children\n");
}

//-----------------------------------------------------------------------------
// Purpose: Creates the NPC.
//-----------------------------------------------------------------------------
void CNPCMakerXenInvasion::MakeNPC(void)
{
	if (!CanMakeNPC())
		return;
	
	if (g_pGameRules->bHasRandomized)
	{
		if (g_pGameRules->iRandomGamemode == FIREFIGHT_PRIMARY_XENINVASION)
		{
			int nNPCs = ARRAYSIZE(g_charNPCSXenInvasionSupport);
			int randomChoice = rand() % nNPCs;
			const char *pRandomName = g_charNPCSXenInvasionSupport[randomChoice];

			CAI_BaseNPC	*pent = (CAI_BaseNPC*)CreateEntityByName(pRandomName);

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

			if (m_spawnflags & SF_NPCMAKER_FADE)
			{
				pent->AddSpawnFlags(SF_NPC_FADE_CORPSE);
			}

			pent->m_isRareEntity = false;
			pent->SetSquadName(m_SquadName);
			pent->SetHintGroup(m_strHintGroup);

			ChildPreSpawn(pent);

			DispatchSpawn(pent);
			pent->SetOwnerEntity(this);
			DispatchActivate(pent);

			if (m_ChildTargetName != NULL_STRING)
			{
				// if I have a netname (overloaded), give the child NPC that name as a targetname
				pent->SetName(m_ChildTargetName);
			}

			ChildPostSpawn(pent);
		}
		else
		{
			CAI_BaseNPC	*pent = (CAI_BaseNPC*)CreateEntityByName(STRING(m_iszNPCClassname));

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

			if (m_spawnflags & SF_NPCMAKER_FADE)
			{
				pent->AddSpawnFlags(SF_NPC_FADE_CORPSE);
			}

			pent->m_spawnEquipment = m_spawnEquipment;
			pent->m_isRareEntity = false;
			pent->SetSquadName(m_SquadName);
			pent->SetHintGroup(m_strHintGroup);

			ChildPreSpawn(pent);

			DispatchSpawn(pent);
			pent->SetOwnerEntity(this);
			DispatchActivate(pent);

			if (m_ChildTargetName != NULL_STRING)
			{
				// if I have a netname (overloaded), give the child NPC that name as a targetname
				pent->SetName(m_ChildTargetName);
			}

			ChildPostSpawn(pent);
		}
	}
	else
	{
		if (g_pGameRules->GetGamemode() == FIREFIGHT_PRIMARY_XENINVASION)
		{
			int nNPCs = ARRAYSIZE(g_charNPCSXenInvasionSupport);
			int randomChoice = rand() % nNPCs;
			const char *pRandomName = g_charNPCSXenInvasionSupport[randomChoice];

			CAI_BaseNPC	*pent = (CAI_BaseNPC*)CreateEntityByName(pRandomName);

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

			if (m_spawnflags & SF_NPCMAKER_FADE)
			{
				pent->AddSpawnFlags(SF_NPC_FADE_CORPSE);
			}

			pent->m_isRareEntity = false;
			pent->SetSquadName(m_SquadName);
			pent->SetHintGroup(m_strHintGroup);

			ChildPreSpawn(pent);

			DispatchSpawn(pent);
			pent->SetOwnerEntity(this);
			DispatchActivate(pent);

			if (m_ChildTargetName != NULL_STRING)
			{
				// if I have a netname (overloaded), give the child NPC that name as a targetname
				pent->SetName(m_ChildTargetName);
			}

			ChildPostSpawn(pent);
		}
		else
		{
			CAI_BaseNPC	*pent = (CAI_BaseNPC*)CreateEntityByName(STRING(m_iszNPCClassname));

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

			if (m_spawnflags & SF_NPCMAKER_FADE)
			{
				pent->AddSpawnFlags(SF_NPC_FADE_CORPSE);
			}

			pent->m_spawnEquipment = m_spawnEquipment;
			pent->m_isRareEntity = false;
			pent->SetSquadName(m_SquadName);
			pent->SetHintGroup(m_strHintGroup);

			ChildPreSpawn(pent);

			DispatchSpawn(pent);
			pent->SetOwnerEntity(this);
			DispatchActivate(pent);

			if (m_ChildTargetName != NULL_STRING)
			{
				// if I have a netname (overloaded), give the child NPC that name as a targetname
				pent->SetName(m_ChildTargetName);
			}

			ChildPostSpawn(pent);
		}
	}

	m_nLiveChildren++;// count this NPC
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : *pChild - 
//-----------------------------------------------------------------------------
void CNPCMakerXenInvasion::ChildPostSpawn(CAI_BaseNPC *pChild)
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
bool CNPCMakerXenInvasion::IsDepleted()
{
	return false;
}


//-----------------------------------------------------------------------------
// Purpose: Toggle the spawner's state
//-----------------------------------------------------------------------------
void CNPCMakerXenInvasion::Toggle(void)
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
void CNPCMakerXenInvasion::Enable(void)
{
	m_bDisabled = false;
	SetThink(&CNPCMakerXenInvasion::MakerThink);
	SetNextThink( gpGlobals->curtime );
}


//-----------------------------------------------------------------------------
// Purpose: Stop the spawner
//-----------------------------------------------------------------------------
void CNPCMakerXenInvasion::Disable(void)
{
	m_bDisabled = true;
	SetThink ( NULL );
}


//-----------------------------------------------------------------------------
// Purpose: Input handler that spawns an NPC.
//-----------------------------------------------------------------------------
void CNPCMakerXenInvasion::InputSpawnNPC(inputdata_t &inputdata)
{
	MakeNPC();
}

//-----------------------------------------------------------------------------
// Purpose: Input hander that starts the spawner
//-----------------------------------------------------------------------------
void CNPCMakerXenInvasion::InputEnable(inputdata_t &inputdata)
{
	Enable();
}


//-----------------------------------------------------------------------------
// Purpose: Input hander that stops the spawner
//-----------------------------------------------------------------------------
void CNPCMakerXenInvasion::InputDisable(inputdata_t &inputdata)
{
	Disable();
}


//-----------------------------------------------------------------------------
// Purpose: Input hander that toggles the spawner
//-----------------------------------------------------------------------------
void CNPCMakerXenInvasion::InputToggle(inputdata_t &inputdata)
{
	Toggle();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CNPCMakerXenInvasion::InputSetMaxLiveChildren(inputdata_t &inputdata)
{
	m_nMaxLiveChildren = inputdata.value.Int();
}

void CNPCMakerXenInvasion::InputSetSpawnFrequency(inputdata_t &inputdata)
{
	m_flSpawnFrequency = inputdata.value.Float();
}
