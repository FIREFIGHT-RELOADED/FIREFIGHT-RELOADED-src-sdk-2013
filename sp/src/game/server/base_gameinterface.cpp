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
	char szMapadd[128];
	Q_snprintf(szMapadd, sizeof(szMapadd), "mapadd/%s.txt", STRING( gpGlobals->mapname ));
	if (!pMapadd)
	{
		pMapadd = CreateMapAddEntity();
		pMapadd->RunLabel(szMapadd, "Init");
	}
	else
	{
		pMapadd->RunLabel(szMapadd, "Init");
	}
}
