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

void CServerGameClients::GetPlayerLimits( int& minplayers, int& maxplayers, int &defaultMaxPlayers ) const
{
	//minplayers = defaultMaxPlayers = 1; 
	//maxplayers = MAX_PLAYERS;
	minplayers = 1;
	defaultMaxPlayers = 1;
	maxplayers = 1;
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
		case FIREFIGHT_PRIMARY_COMBINEFIREFIGHT:
				if (pMapadd)
				{
					pMapadd->RunLabel("CombineFirefight");
				}
				break;

		case FIREFIGHT_PRIMARY_XENINVASION:
				if (pMapadd)
				{
					pMapadd->RunLabel("XenInvasion");
				}
				break;

		case FIREFIGHT_PRIMARY_ANTLIONASSAULT:
				if (pMapadd)
				{
					pMapadd->RunLabel("AntlionAssault");
				}
				break;

		case FIREFIGHT_PRIMARY_ZOMBIESURVIVAL:
				if (pMapadd)
				{
					pMapadd->RunLabel("ZombieSurvival");
				}
				break;

		case FIREFIGHT_PRIMARY_FIREFIGHTRUMBLE:
				if (pMapadd)
				{
					pMapadd->RunLabel("FirefightRumble");
				}
				break;
		default:
		case FIREFIGHT_PRIMARY_DEFAULT:
				if (pMapadd)
				{
					pMapadd->RunLabel("Default");
				}
				break;
	}
}
