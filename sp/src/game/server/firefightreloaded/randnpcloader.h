//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#ifndef RANDNPCLOADER_H
#define RANDNPCLOADER_H
#ifdef _WIN32
#pragma once
#endif

#include "cbase.h"

#define TIME_SETBYHAMMER -1

//-----------------------------------------------------------------------------
// Spawnflags
//-----------------------------------------------------------------------------
//#define SF_NPCMAKER_FADE			1	// Children's corpses fade
//TODO: add mutators, including this.
//#define SF_NPCMAKER_DOUBLETROUBLE	8	//Spawn double the enemies
//#define SF_NPCMAKER_ALWAYSUSERADIUS	32	// Use radius spawn whenever spawning

class CRandNPCLoader
{
public:
	struct Settings_t
	{
		float spawnTime;
	};

	struct EquipEntry_t
	{
		const char *name;
		float weight;
	};

	struct MapFilterEntry_t
	{
		const char* name;
	};

	struct SpawnEntry_t
	{
		SpawnEntry_t();

		const char* GetRandomEquip() const;
		int GetRandomGrenades() const;
		bool IsInRightMap() const;

		const char* classname;
		CCopyableUtlVector<MapFilterEntry_t> maps;
		bool isRare;
		float weight;
		int minPlayerLevel;
		int npcAttributePreset;
		int grenadesMin;
		int grenadesMax;
		CCopyableUtlVector<EquipEntry_t> spawnEquipment;
		float totalEquipWeight;
	};

	CRandNPCLoader();
	~CRandNPCLoader();

	bool Load();
	bool AddEntries( KeyValues *kv );
	const SpawnEntry_t* GetRandomEntry(bool isRare) const;

public:
	Settings_t m_Settings;

private:
	static int GetLargestLevel();
	static bool SetRandomGrenades(SpawnEntry_t& entry, string_t grenadeString);
	static bool ParseEntry( SpawnEntry_t& entry, KeyValues *kv );
	static bool ParseRange( int& min, int& man, const char* s );

	CUtlVector<SpawnEntry_t> m_Entries;
	KeyValues* m_KVs;

	friend void dumpspawnlist_cb();
};

#endif // MONSTERMAKER_H
