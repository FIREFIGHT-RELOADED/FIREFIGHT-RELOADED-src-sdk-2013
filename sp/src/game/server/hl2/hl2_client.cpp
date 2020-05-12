//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//
//=============================================================================//
/*

===== tf_client.cpp ========================================================

  HL2 client/server game specific stuff

*/

#include "cbase.h"
#include "hl2_player.h"
#include "hl2_gamerules.h"
#include "gamerules.h"
#include "teamplay_gamerules.h"
#include "entitylist.h"
#include "physics.h"
#include "game.h"
#include "player_resource.h"
#include "engine/IEngineSound.h"
#include "filesystem.h"

#include "tier0/vprof.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

void Host_Say( edict_t *pEdict, bool teamonly );

extern CBaseEntity*	FindPickerEntityClass( CBasePlayer *pPlayer, char *classname );
extern bool			g_fGameOver;
extern ConVar sv_player_hardcoremode;
//const char			*szModelName = NULL; //already declared?

void CC_EntityCount(void)
{
	Msg("%d/%d entities, %d/%d NPCs\n", gEntList.NumberOfEntities(), gpGlobals->maxEntities, g_iNPCLimit, g_fr_npclimit.GetInt());
}

static ConCommand entitycount("entitycount", CC_EntityCount, "Shows map entity and NPC count");

/*
===========
ClientPutInServer

called each time a player is spawned into the game
============
*/
void ClientPutInServer( edict_t *pEdict, const char *playername )
{
	// Allocate a CBasePlayer for pev, and call spawn
	CHL2_Player *pPlayer = CHL2_Player::CreatePlayer( "player", pEdict );
	pPlayer->SetPlayerName( playername );
}


void ClientActive( edict_t *pEdict, bool bLoadGame )
{
	CHL2_Player *pPlayer = dynamic_cast< CHL2_Player* >( CBaseEntity::Instance( pEdict ) );
	Assert( pPlayer );

	if ( !pPlayer )
	{
		return;
	}

	pPlayer->InitialSpawn();

	if ( !bLoadGame )
	{
		pPlayer->Spawn();
	}
}


/*
===============
const char *GetGameDescription()

Returns the descriptive name of this .dll.  E.g., Half-Life, or Team Fortress 2
===============
*/
const char *GetGameDescription()
{
	if ( g_pGameRules ) // this function may be called before the world has spawned, and the game rules initialized
		return g_pGameRules->GetGameDescription();
	else
		return "FIREFIGHT RELOADED";
}

//-----------------------------------------------------------------------------
// Purpose: Given a player and optional name returns the entity of that 
//			classname that the player is nearest facing
//			
// Input  :
// Output :
//-----------------------------------------------------------------------------
CBaseEntity* FindEntity( edict_t *pEdict, char *classname)
{
	// If no name was given set bits based on the picked
	if (FStrEq(classname,"")) 
	{
		return (FindPickerEntityClass( static_cast<CBasePlayer*>(GetContainingEntity(pEdict)), classname ));
	}
	return NULL;
}

//-----------------------------------------------------------------------------
// Purpose: Precache game-specific models & sounds
//-----------------------------------------------------------------------------
void ClientGamePrecache( void )
{
	CBaseEntity::PrecacheModel( "models/player.mdl");
	
	CBaseEntity::PrecacheModel( "models/gibs/agibs.mdl" );
	CBaseEntity::PrecacheModel ("models/weapons/v_hands.mdl");

	CBaseEntity::PrecacheModel("models/weapons/v_kick.mdl");
	CBaseEntity::PrecacheModel("models/gibs/pgib_p3.mdl");
	CBaseEntity::PrecacheModel("models/gibs/pgib_p4.mdl");
	CBaseEntity::PrecacheModel("models/gibs/agib_p3.mdl");
	CBaseEntity::PrecacheModel("models/gibs/agib_p4.mdl");

	PrecacheParticleSystem("smod_headshot_r");
	PrecacheParticleSystem("smod_headshot_g");
	PrecacheParticleSystem("smod_headshot_y");
	PrecacheParticleSystem("smod_blood_decap_r");
	PrecacheParticleSystem("smod_blood_decap_g");
	PrecacheParticleSystem("smod_blood_decap_y");
	PrecacheParticleSystem("smod_blood_gib_r");
	PrecacheParticleSystem("smod_blood_gib_g");
	PrecacheParticleSystem("smod_blood_gib_y");
	PrecacheParticleSystem("weapon_muzzle_smoke");

	CBaseEntity::PrecacheScriptSound( "HUDQuickInfo.LowAmmo" );
	CBaseEntity::PrecacheScriptSound( "HUDQuickInfo.LowHealth" );

	CBaseEntity::PrecacheScriptSound( "FX_AntlionImpact.ShellImpact" );
	CBaseEntity::PrecacheScriptSound( "Missile.ShotDown" );
	CBaseEntity::PrecacheScriptSound( "Bullets.DefaultNearmiss" );
	CBaseEntity::PrecacheScriptSound( "Bullets.GunshipNearmiss" );
	CBaseEntity::PrecacheScriptSound( "Bullets.StriderNearmiss" );

	CBaseEntity::PrecacheScriptSound( "Player.IronSightIn" );
	CBaseEntity::PrecacheScriptSound( "Player.IronSightOut" );

	CBaseEntity::PrecacheScriptSound("Gore.Headshot");
	
	CBaseEntity::PrecacheScriptSound( "Geiger.BeepHigh" );
	CBaseEntity::PrecacheScriptSound( "Geiger.BeepLow" );
}


// called by ClientKill and DeadThink
void respawn( CBaseEntity *pEdict, bool fCopyCorpse )
{
	CHL2_Player *pPlayer = (CHL2_Player *)pEdict;
	if (pPlayer)
	{
		if (sv_player_hardcoremode.GetBool() && !g_pGameRules->IsMultiplayer())
		{
			char szMapCommand[1024];
			// create the command to execute
			Q_snprintf(szMapCommand, sizeof(szMapCommand), "map credits\nprogress_enable\n");
			engine->ServerCommand(szMapCommand);
		}
		else if (pPlayer->GetLevel() == MAX_LEVEL && !g_pGameRules->IsMultiplayer())
		{
			char szMapCommand[1024];
			// create the command to execute
			Q_snprintf(szMapCommand, sizeof(szMapCommand), "map credits\nprogress_enable\n");
			engine->ServerCommand(szMapCommand);
		}
		else
		{
			pPlayer->Spawn();
		}
	}
}

void GameStartFrame( void )
{
	VPROF("GameStartFrame()");
	if ( g_fGameOver )
		return;

	gpGlobals->teamplay = (teamplay.GetInt() != 0);
}

#ifdef HL2_EPISODIC
extern ConVar gamerules_survival;
#endif

//=========================================================
// instantiate the proper game rules object
//=========================================================
void InstallGameRules()
{
#ifdef HL2_EPISODIC
	if ( gamerules_survival.GetBool() )
	{
		// Survival mode
		CreateGameRulesObject( "CHalfLife2Survival" );
	}
	else
#endif
	{
		// generic half-life
		CreateGameRulesObject( "CHalfLife2" );
	}
}

