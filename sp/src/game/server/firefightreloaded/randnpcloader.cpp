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

void dumpspawnlist_cb()
{
	extern CRandNPCLoader* g_npcLoader;

	ConMsg("m_Settings.spawnTime: %f\n", g_npcLoader->m_Settings.spawnTime);
	for ( auto& iter : g_npcLoader->m_Entries )
	{
		ConMsg( "[%p] name=\"%s\", %s minPlayerLevel=%d npcAttributePreset=%d npcAttributeWildcard=%d grenades=[%d, %d] weight=%f, totalEquipWeight=%f\n",
			&iter,
			iter.classname,
			iter.isRare ? "rare" : "notRare",
			iter.minPlayerLevel,
			iter.npcAttributePreset,
			iter.npcAttributeWildcard,
			iter.grenadesMin,
			iter.grenadesMax,
			iter.weight,
			iter.totalEquipWeight
		);
		for ( auto& iter2 : iter.spawnEquipment )
		{
			ConMsg( "[%p]  name=\"%s\", weight=%f\n",
				&iter,
				iter2.name,
				iter2.weight
			);
		}
	}
}

static ConCommand dumpspawnlist( "dumpspawnlist", dumpspawnlist_cb, "Dumps the spawn list." );

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

	if (gamemodeName == NULL || strlen(gamemodeName) == 0 ||
		(!g_pGameRules->bSkipFuncCheck && !g_fr_spawneroldfunctionality.GetBool()))
	{
		gamemodeMode = false;
	}

	char szScriptPath[512] = { 0 };
	if (gamemodeMode)
		Q_snprintf(szScriptPath, sizeof(szScriptPath), "scripts/spawnlists/%s.txt", gamemodeName);
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
			DevMsg("CRandNPCLoader: User-specified default spawnlist loaded.\n");
		else
		{
			DevWarning("CRandNPCLoader: Failed to load default spawnlist! File may not exist. Spawners will not function properly.\n");
			return false;
		}
	}

	//load settings in the spawnlist if any.
	KeyValues* settings = pKV->FindKey("settings");
	if (settings)
	{
		if (UTIL_IsSteamDeck())
		{
			float spawntimeDeck = settings->GetFloat("spawntime_steamdeck", TIME_SETBYHAMMER);

			if (spawntimeDeck != TIME_SETBYHAMMER)
			{
				m_Settings.spawnTime = spawntimeDeck;
			}
			else
			{
				m_Settings.spawnTime = settings->GetFloat("spawntime", TIME_SETBYHAMMER);
			}
		}
		else
		{
			m_Settings.spawnTime = settings->GetFloat("spawntime", TIME_SETBYHAMMER);
		}
	}
	else
	{
		m_Settings.spawnTime = TIME_SETBYHAMMER;
	}

	AddEntries( pKV );

	pKV->deleteThis();

	return true;
}

const CRandNPCLoader::SpawnEntry_t* CRandNPCLoader::GetRandomEntry(bool isRare) const
{
	int largestPlayerLevel = GetLargestLevel();

	// Create list of candidates by checking all the keys to generate a level based spawnlist.
	CUtlBlockLinkedList<const SpawnEntry_t*> candidates;
	float totalWeight = 0;
	for ( auto& iter : m_Entries )
	{
		if ( largestPlayerLevel >= iter.minPlayerLevel && iter.isRare == isRare )
		{
			candidates.AddToTail( &iter );
			totalWeight += iter.weight;
		}
	}

	// This naive algorithm (implemented elsewhere too) could ignore very small weights.
	// If this comes up, then we'll look into this.
	float choice = random->RandomFloat( 0, totalWeight );
	for ( auto iter : candidates )
	{
		choice -= iter->weight;
		if ( choice <= 0 )
			return iter;
	}

	return NULL;
}

bool CRandNPCLoader::AddEntries( KeyValues* pKV )
{
	bool ret = true;
	int num = 1;
	for ( auto iter = pKV->GetFirstSubKey(); iter != NULL; iter = iter->GetNextKey() )
	{
		if (!strcmp(iter->GetName(), "settings"))
			continue;

		auto newKV = iter->MakeCopy();
		m_KVs->AddSubKey( iter->MakeCopy() );
		SpawnEntry_t entry;
		if (!ParseEntry(entry, newKV))
		{
			ConWarning("CRandNPCLoader::AddEntries: Unable to parse entry number %d. Ending.\n", num);
			return false;
		}

		if (!entry.maps.IsEmpty() && !entry.maps.HasElement(gpGlobals->mapname))
		{
			DevMsg("CRandNPCLoader::AddEntries: Not in right map for entry number %d. Ending.\n", num);
			return false;
		}

		m_Entries.AddToTail(entry);
		num++;
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
	entry.weight = kv->GetFloat( "weight", 1 );
	entry.minPlayerLevel = kv->GetInt( "min_level", 1 );
	entry.npcAttributePreset = kv->GetInt( "preset", -1 );
	entry.npcAttributeWildcard = kv->GetInt("wildcard", -1);
	entry.spawnEquipment.RemoveAll();
	entry.totalEquipWeight = 0;
	entry.grenadesMin = entry.grenadesMax = -1;

	auto grenades = MAKE_STRING( kv->GetString( "grenades" ) );
	if ( grenades != NULL_STRING )
	{
		if (!SetRandomGrenades(entry, grenades))
      		return false;
	}

	KeyValues* mapKv = kv->FindKey("mapspawn");
	if (mapKv != NULL && mapKv->GetFirstSubKey() != NULL)
	{
		for (KeyValues* iter = mapKv->GetFirstSubKey(); iter != NULL; iter = iter->GetNextKey())
		{
			MapFilterEntry_t MapEntry{ iter->GetName() };
			entry.maps.AddToTail(MapEntry);
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
			EquipEntry_t equipEntry{ iter->GetName(), iter->GetFloat() };
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
	npcAttributeWildcard = -1;
	minPlayerLevel = 1;
	isRare = false;
	weight = 1;
	grenadesMin = grenadesMax = -1;
}

const char* CRandNPCLoader::SpawnEntry_t::GetRandomEquip() const
{
	float choice = random->RandomFloat( 0, totalEquipWeight );
	for ( auto& iter : spawnEquipment )
	{
		choice -= iter.weight;
		if ( choice <= 0 )
			return iter.name;
	}

	return NULL;
}
