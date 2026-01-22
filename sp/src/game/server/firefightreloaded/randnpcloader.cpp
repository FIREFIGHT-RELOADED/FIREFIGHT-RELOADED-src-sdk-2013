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
#include "firefightreloaded/fr_shareddefs.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

ConVar sk_spawner_defaultspawnlist("sk_spawner_defaultspawnlist", "scripts/spawnlists/default.txt", FCVAR_ARCHIVE);

void dumpspawnlist_cb()
{
	extern CRandNPCLoader* g_npcLoader;

	if (!g_npcLoader)
	{
		Warning("No spawnlist found.\n");
		return;
	}

	Color settings = Color(255, 64, 64, 255);
	Color spawnEntries = Color(255, 166, 64, 255);
	Color spawnEntryEquip = Color(255, 249, 64, 255);
	Color spawnEntryMaps = Color(153, 255, 64, 255);
	Color spawnEntryAttribute = Color(64, 255, 93, 255);

	if (g_npcLoader->m_Settings.spawnTime == -1)
	{
		ConColorMsg(settings, "[settings] spawnTime: %f (managed with entity definition from Hammer or Mapadd)\n", g_npcLoader->m_Settings.spawnTime);
	}
	else
	{
		ConColorMsg(settings, "[settings] spawnTime: %f\n", g_npcLoader->m_Settings.spawnTime);
	}

	for ( auto& iter : g_npcLoader->m_Entries )
	{
		ConColorMsg(spawnEntries, "[%s (%p)] name=\"%s\", %s minPlayerLevel=%d npcAttributePreset=%d npcAttributeWildcard=%d grenades=[%d, %d] weight=%f, totalEquipWeight=%f, extraExp=%d, extraMoney=%d, subsituteValues=%s taskIgnore=%s ally=%s\n",
			iter.classname,
			&iter,
			iter.classname,
			iter.isRare ? "rare" : "notRare",
			iter.minPlayerLevel,
			iter.npcAttributePreset,
			iter.npcAttributeWildcard,
			iter.grenadesMin,
			iter.grenadesMax,
			iter.weight,
			iter.totalEquipWeight,
			iter.extraExp,
			iter.extraMoney,
			iter.subsituteValues ? "true" : "false",
			iter.taskIgnore ? "true" : "false",
			iter.ally ? "true" : "false"
		);
		for ( auto& iter2 : iter.spawnEquipment )
		{
			ConColorMsg(spawnEntryEquip, "[%s (%p)] Equipment: name=\"%s\", weight=%f\n",
				iter.classname,
				&iter,
				iter2.name,
				iter2.weight
			);
		}

		for (auto& iter3 : iter.maps)
		{
			ConColorMsg(spawnEntryMaps, "[%s (%p)] MapEntry: name=\"%s\"\n",
				iter.classname,
				&iter,
				iter3.name
			);
		}

		if (iter.npcCustomAttributes != NULL && iter.npcCustomAttributes->GetFirstSubKey() != NULL)
		{
			ConColorMsg(spawnEntryAttribute, "[%s (%p)] Attributes: ", iter.classname, &iter);
			KeyValuesDumpAsColorMsg(iter.npcCustomAttributes, 1, spawnEntryAttribute);
		}
	}
}

static ConCommand dumpspawnlist( "dumpspawnlist", dumpspawnlist_cb, "Dumps the spawn list." );

CRandNPCLoader::CRandNPCLoader()
{
}

CRandNPCLoader::~CRandNPCLoader()
{
}

bool CRandNPCLoader::Load()
{
	bool gamemodeMode = true;
	const char* gamemodeName = g_pGameRules->GetGamemodeName();
	const char* gamemodeNameSB = g_pGameRules->GetGamemodeName_ServerBrowser();

	if (gamemodeName == NULL || strlen(gamemodeName) == 0 ||
		(!g_pGameRules->bSkipFuncCheck && !g_fr_spawneroldfunctionality.GetBool()))
	{
		gamemodeMode = false;
	}

	KeyValues* pKV = NULL;
	bool failed = false;

	if (gamemodeMode)
	{
		char szScriptPath[_MAX_PATH] = { 0 };
		Q_snprintf(szScriptPath, sizeof(szScriptPath), "scripts/spawnlists/%s.txt", gamemodeName);

		pKV = new KeyValues(gamemodeName);
		if (pKV->LoadFromFile(filesystem, szScriptPath))
		{
			Msg("CRandNPCLoader: Spawnlist for '%s' loaded.\n", gamemodeNameSB);
		}
		else
		{
			failed = true;
		}
	}
	else
	{
		const char* mapName = STRING(gpGlobals->mapname);
		char szMapScriptPath[_MAX_PATH] = { 0 };
		Q_snprintf(szMapScriptPath, sizeof(szMapScriptPath), "scripts/spawnlists/maps/%s.txt", mapName);

		pKV = new KeyValues(mapName);
		if (pKV->LoadFromFile(filesystem, szMapScriptPath))
		{
			Msg("CRandNPCLoader: Spawnlist for '%s' loaded.\n", mapName);
		}
		else
		{
			failed = true;
		}
	}

	if (failed)
	{
		pKV = new KeyValues("Spawnlist");
		const char* userSpecified = sk_spawner_defaultspawnlist.GetString();
		Warning("CRandNPCLoader: Failed to load '%s' spawnlist! File may not exist. Using user-specified default spawn list...\n", gamemodeNameSB);
		if (pKV->LoadFromFile(filesystem, userSpecified))
		{
			Msg("CRandNPCLoader: '%s' loaded.\n", userSpecified);
		}
		else
		{
			Warning("CRandNPCLoader: Failed to load '%s'! File may not exist. Using default spawn list...\n", userSpecified);
			if (pKV->LoadFromFile(filesystem, "scripts/spawnlists/default.txt"))
			{
				Msg("CRandNPCLoader: Default spawnlist loaded.\n");
			}
			else
			{
				Warning("CRandNPCLoader: Failed to load default spawnlist! File may not exist. Spawners will not function properly.\n");
				return false;
			}
		}
	}

	//load settings in the spawnlist if any.
	KeyValues* settings = pKV->FindKey("settings");
	if (settings)
	{
		if (UTIL_UsingSteamInput())
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

		m_Settings.canUseBugbait = settings->GetBool("canusebugbait", true);
	}
	else
	{
		m_Settings.spawnTime = TIME_SETBYHAMMER;
		m_Settings.canUseBugbait = true;
	}

	AddEntries( pKV );

	pKV->deleteThis();

	return true;
}

const CRandNPCLoader::SpawnEntry_t* CRandNPCLoader::GetRandomEntry(bool isRare) const
{
	int largestPlayerLevel = GetLargestLevel();

	random->SetSeed((int)gpGlobals->curtime);

	// If the candidate is applicable, add to our weight.
	// originally, we had a list of candidates here, but that was causing a stack overflow crash in some instances.
	// so, we'll just go through the list we have.
	float totalWeight = 0;
	for ( auto& iter : m_Entries )
	{
		//prioritize enemies over allies.
		bool coinFlip = false;
		if (ContainsAllies())
		{
			coinFlip = ((random->RandomInt(0, 1) == 1) ? true : false);

			if (!coinFlip && iter.ally)
				continue;
		}

		if ( largestPlayerLevel >= iter.minPlayerLevel && iter.isRare == isRare )
		{
			totalWeight += iter.weight;
		}
	}

	// then, use the weight to determine what we should spawn, making sure the enemy can spawn in this fashion.
	// This naive algorithm (implemented elsewhere too) could ignore very small weights.
	// If this comes up, then we'll look into this.
	float choice = random->RandomFloat( 0, totalWeight );
	for ( auto& iter2 : m_Entries )
	{
		if (largestPlayerLevel >= iter2.minPlayerLevel && iter2.isRare == isRare)
		{
			choice -= iter2.weight;
			if (choice <= 0)
				return &iter2;
		}
	}

	return NULL;
}

const CRandNPCLoader::SpawnEntry_t* CRandNPCLoader::GetEntry(const char* query, int preset, bool wildcard) const
{
	for (auto &iter : m_Entries)
	{
		if (FStrEq(iter.classname, query))
		{
			bool cont = true;

			// HACK.
			int iWildcard = (wildcard ? 1 : 0);

			if (wildcard && (iWildcard != iter.npcAttributeWildcard))
			{
				cont = false;
			}

			if ((preset > -1) && (preset != iter.npcAttributePreset))
			{
				cont = false;
			}

			if (cont)
			{
				return &iter;
			}
		}
	}

	return NULL;
}

const bool CRandNPCLoader::ContainsRareEnemies() const
{
	int iRareEnemies = 0;

	for (auto& iter : m_Entries)
	{
		if (iter.isRare)
		{
			iRareEnemies++;
		}
	}

	return (iRareEnemies > 0);
}

const bool CRandNPCLoader::ContainsAllies() const
{
	int iAllies = 0;

	for (auto& iter : m_Entries)
	{
		if (iter.ally)
		{
			iAllies++;
		}
	}

	return (iAllies > 0);
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
	entry.extraExp = kv->GetInt("exp", -1);
	entry.extraMoney = kv->GetInt("kash", -1);
	entry.subsituteValues = kv->GetBool("subsitute", true);

	if (entry.subsituteValues)
	{
		if (entry.extraExp <= 0 && entry.extraMoney > 0)
		{
			entry.extraExp = entry.extraMoney;
		}
		else if (entry.extraMoney <= 0 && entry.extraExp > 0)
		{
			entry.extraMoney = entry.extraExp;
		}
	}

	entry.taskIgnore = kv->GetBool("task_selection_ignore", false);
	entry.ally = kv->GetBool("ally", false);
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

	KeyValues* attributesKv = kv->FindKey("attributes");
	if (attributesKv != NULL && attributesKv->GetFirstSubKey() != NULL)
	{
		entry.npcCustomAttributes = attributesKv->MakeCopy();
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
	extraExp = -1;
	extraMoney = -1;
	subsituteValues = true;
	taskIgnore = false;
	ally = false;
	isRare = false;
	weight = 1;
	grenadesMin = grenadesMax = -1;
	totalEquipWeight = 0;
	npcCustomAttributes = NULL;
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
