//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: An entity that creates NPCs in the game.
//
//=============================================================================//

#include "cbase.h"
#include "globalstate.h"
#include "filesystem.h"
#include "KeyValues.h"
#include "randnpcloader.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

ConVar sk_spawner_defaultspawnlist("sk_spawner_defaultspawnlist", "scripts/spawnlists/default.txt", FCVAR_ARCHIVE);

CRandNPCLoader::CRandNPCLoader()
{
	m_KVs = new KeyValues("");
}

CRandNPCLoader::~CRandNPCLoader()
{
	m_KVs->deleteThis();
}

bool CRandNPCLoader::Load()
{
	bool gamemodeMode = true;
	const char* gamemodeName = g_pGameRules->GetGamemodeName();
	const char* mapName = STRING(gpGlobals->mapname);

	if ((!gamemodeName || gamemodeName == NULL || strlen(gamemodeName) == 0) || 
		(!g_pGameRules->bSkipFuncCheck && !g_fr_spawneroldfunctionality.GetBool()))
	{
		gamemodeMode = false;
	}

	char szScriptPath[512] = { 0 };
	if (gamemodeMode)
		Q_snprintf(szScriptPath, sizeof(szScriptPath), "scripts/spawnlists/gamemodes/%s.txt", gamemodeName);
	else
		Q_snprintf(szScriptPath, sizeof(szScriptPath), "scripts/spawnlists/maps/%s.txt", mapName);

	const char* name = gamemodeMode ? gamemodeName : mapName;
	KeyValues* pKV = new KeyValues(name);
	if (pKV->LoadFromFile(filesystem, szScriptPath))
		DevMsg("CRandNPCLoader: Spawnlist for %s loaded.\n", name);
	else
	{
		DevWarning("CRandNPCLoader: Failed to load %s spawnlist! File may not exist. Using default spawn list...\n", name);
		if (pKV->LoadFromFile(filesystem, sk_spawner_defaultspawnlist.GetString()))
			DevMsg("CRandNPCLoader: Default Spawnlist loaded.\n");
		else
		{
			DevWarning("CRandNPCLoader: Failed to load default spawnlist! File may not exist. Spawners will not function properly.\n");
			return false;
		}
	}

	AddEntries( pKV );

	pKV->deleteThis();

	return true;
}

const CRandNPCLoader::SpawnEntry_t* CRandNPCLoader::GetRandomEntry(bool isRare) const
{
	int count = 0;
	int largestPlayerLevel = GetLargestLevel();

	//check all the keys to generate a level based spawnlist.
	const auto end = m_Entries.end();
	for ( auto iter = m_Entries.begin(); iter != end; ++iter )
	{
		if ( largestPlayerLevel >= iter->minPlayerLevel && iter->isRare == isRare)
			++count;
	}

	int choice = random->RandomInt( 1, count );
	for ( auto iter = m_Entries.begin(); iter != end; ++iter )
	{
		if ( largestPlayerLevel >= iter->minPlayerLevel && iter->isRare == isRare)
		{
			if ( choice == 1 )
				return iter;
			else
				--choice;
		}
	}
	return NULL;
}

bool CRandNPCLoader::AddEntries( KeyValues* pKV )
{
	bool ret = true;
	for ( KeyValues* iter = pKV->GetFirstSubKey(); iter != NULL; iter = iter->GetNextKey() )
	{
		auto newKV = iter->MakeCopy();
		m_KVs->AddSubKey( iter->MakeCopy() );
		SpawnEntry_t entry;
		if ( !ParseEntry( entry, newKV ) )
			return false;
		m_Entries.AddToTail( entry );
	}
	return ret;
}

bool CRandNPCLoader::ParseRange( int &min, int &max, const char* s )
{
	if ( strchr( s, '-' ) != NULL )
	{
		if ( sscanf( s, "%d-%d", &min, &max ) != 2 )
			return false;
	}
	else
		min = max = atoi( s );

	return 0 <= min && min <= max;
}

bool CRandNPCLoader::ParseEntry( SpawnEntry_t& entry, KeyValues *kv)
{
	//CreateLevelBasedSpawnlist already does file checking for us.
	entry.classname = kv->GetString( "classname", NULL );
	if ( entry.classname == NULL )
		return false;
	UTIL_PrecacheOther( entry.classname );
	entry.isRare = kv->GetBool( "rare", false );
	entry.minPlayerLevel = kv->GetInt( "min_level", 1 );
	entry.npcAttributePreset = kv->GetInt( "preset", -1 );
	entry.spawnEquipment.RemoveAll();
	entry.totalEquipWeight = 0;
	entry.grenadesMin = entry.grenadesMax = -1;

	auto grenades = MAKE_STRING( kv->GetString( "grenades" ) );
	if ( grenades != NULL_STRING )
	{
		if (!SetRandomGrenades(entry, grenades))
		{
			return false;
		}
	}
	else 
	{
		auto manhacks = MAKE_STRING(kv->GetString("manhacks"));

		if (manhacks != NULL_STRING)
		{
			if (!SetRandomGrenades(entry, manhacks))
			{
				return false;
			}
		}
	}

	// The equipment key can just have a string value, or a list of subkeys with weapon/weight pairs.
	KeyValues* equipKv = kv->FindKey( "equipment" );
	if ( equipKv == NULL )
		return true;

	if ( equipKv->GetFirstSubKey() == NULL )
	{
		// Accept strings for backwards compatibility.
		entry.spawnEquipment.AddToTail( EquipEntry_t{ equipKv->GetString(), 1 } );
		entry.totalEquipWeight = 1;
		return true;
	}
	else
	{
		// The equipment subkey has a list of pairs with the weapon name as the key and weight as the value.
		for ( KeyValues* iter = equipKv->GetFirstSubKey(); iter != NULL; iter = iter->GetNextKey() )
		{
			EquipEntry_t equipEntry{ iter->GetName(), iter->GetInt() };
			entry.totalEquipWeight += equipEntry.weight;
			UTIL_PrecacheOther( equipEntry.name );
			entry.spawnEquipment.AddToTail( equipEntry );
		}
	}

	return true;
}

bool CRandNPCLoader::SetRandomGrenades(SpawnEntry_t& entry, string_t grenadeString)
{
	int max, min;
	if (!ParseRange(min, max, STRING(grenadeString)))
	{
		return false;
	}
	entry.grenadesMin = min;
	entry.grenadesMax = max;
	return true;
}

int CRandNPCLoader::GetLargestLevel()
{
	int largestPlayerLevel = 1;
	for ( int i = 1; i <= gpGlobals->maxClients; i++ )
	{
		CBasePlayer* pPlayer = UTIL_PlayerByIndex( i );
		if ( pPlayer )
		{
			if ( pPlayer->GetLevel() > largestPlayerLevel )
			{
				largestPlayerLevel = pPlayer->GetLevel();
			}
		}
	}

	return largestPlayerLevel;
}

CRandNPCLoader::SpawnEntry_t::SpawnEntry_t()
{
	classname = NULL;
	npcAttributePreset = -1; // 0 = no attributes, random. -1 and below: no attributes at all.
	minPlayerLevel = 1;
	isRare = false;
	grenadesMin = grenadesMax = -1;
}

const char* CRandNPCLoader::SpawnEntry_t::GetRandomEquip() const
{
	int choice = random->RandomInt( 1, totalEquipWeight );
	const auto end = spawnEquipment.end();
	for ( auto iter = spawnEquipment.begin(); iter != end; ++iter )
	{
		choice -= iter->weight;
		if ( choice < 1 )
			return iter->name;
	}

	return NULL;
}

int CRandNPCLoader::SpawnEntry_t::GetRandomGrenades() const
{
	return random->RandomInt( grenadesMin, grenadesMax );
}
