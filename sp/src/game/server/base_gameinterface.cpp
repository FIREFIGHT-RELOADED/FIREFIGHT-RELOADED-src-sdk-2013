//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#include "cbase.h"
#include "gameinterface.h"
#include "mapentities.h"
#include "SMMOD/mapadd.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

ConVar sv_enable_coop("sv_enable_coop", "0", FCVAR_NONE, "Enable 6-player co-op. WARNING: This mode is unsupported and you will have issues regarding gameplay stability and consistency.");

void CServerGameClients::GetPlayerLimits( int& minplayers, int& maxplayers, int &defaultMaxPlayers ) const
{
	minplayers = defaultMaxPlayers = 1; 
	maxplayers = (sv_enable_coop.GetBool() ? 6 : 1); //MAX_PLAYERS;
}

// -------------------------------------------------------------------------------------------- //
// Mod-specific CServerGameDLL implementation.
// -------------------------------------------------------------------------------------------- //

void CServerGameDLL::LevelInit_ParseAllEntities( const char *pMapEntities )
{
	//after that, spawn all custom entities - Bitl
	CMapAdd *pMapadd = GetMapAddEntity();

	if (!pMapadd)
	{
		pMapadd = CreateMapAddEntity();
		pMapadd->RunLabel("Init");
	}
	else
	{
		pMapadd->RunLabel("Init");
	}

	//rewrote this because i hated how it worked.
	switch (g_pGameRules->GetGamemode())
	{
		case FR_GAMEMODE_COMBINEFIREFIGHT:
		{
			if (pMapadd)
			{
				pMapadd->RunLabel("CombineFirefight");
			}
			break;
		}
		case FR_GAMEMODE_XENINVASION:
		{
			if (pMapadd)
			{
				pMapadd->RunLabel("XenInvasion");
			}
			break;
		}
		case FR_GAMEMODE_ANTLIONASSAULT:
		{
			if (pMapadd)
			{
				pMapadd->RunLabel("AntlionAssault");
			}
			break;
		}
		case FR_GAMEMODE_ZOMBIESURVIVAL:
		{
			if (pMapadd)
			{
				pMapadd->RunLabel("ZombieSurvival");
			}
			break;
		}
		case FR_GAMEMODE_FIREFIGHTRUMBLE:
		{
			if (pMapadd)
			{
				pMapadd->RunLabel("FirefightRumble");
			}
			break;
		}
		case FR_GAMEMODE_CAMPAIGN:
		{
			if (pMapadd)
			{
				pMapadd->RunLabel("Campaign");
			}
			break;
		}
		default:
		case FR_GAMEMODE_DEFAULT:
		{
			if (pMapadd)
			{
				pMapadd->RunLabel("Default");
			}
			break;
		}
	}

	KeyValues* pInfo = CMapInfo::GetMapInfoData();

	if (pInfo != NULL)
	{
		const char* customMapAddLabel = pInfo->GetString("additional_mapadd_label", "");

		if (customMapAddLabel[0])
		{
			if (pMapadd)
			{
				pMapadd->RunLabel(customMapAddLabel);
			}
		}
	}
}
