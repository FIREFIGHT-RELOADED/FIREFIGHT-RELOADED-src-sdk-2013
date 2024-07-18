//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#include "cbase.h"
#include "singleplay_gamerules.h"
#include "fmtstr.h"
#include "hl2_shareddefs.h"

#ifdef CLIENT_DLL

#else

	#include "player.h"
	#include "basecombatweapon.h"
	#include "gamerules.h"
	#include "game.h"
	#include "items.h"
	#include "SMMOD/mapadd.h"
	#include "iscorer.h"
	#include "ai_basenpc.h"
	#include "tier3/tier3.h"
	#include "vgui/ILocalize.h"
	#include "hl2/hl2_player.h"

#endif

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

ConVar sk_money_multiplier1("sk_money_multiplier1", "4");
ConVar sk_money_multiplier2("sk_money_multiplier2", "4");
ConVar sk_money_multiplier3("sk_money_multiplier3", "3");
ConVar sk_money_multiplier4("sk_money_multiplier4", "2");
ConVar sk_money_multiplier5("sk_money_multiplier5", "1");

ConVar sk_exp_multiplier1("sk_exp_multiplier1", "4");
ConVar sk_exp_multiplier2("sk_exp_multiplier2", "4");
ConVar sk_exp_multiplier3("sk_exp_multiplier3", "3");
ConVar sk_exp_multiplier4("sk_exp_multiplier4", "2");
ConVar sk_exp_multiplier5("sk_exp_multiplier5", "1");

ConVar sv_killingspree("sv_killingspree", "1", FCVAR_ARCHIVE);

ConVar sv_healthcharger_recharge("sv_healthcharger_recharge", "1", FCVAR_ARCHIVE);
ConVar sv_healthcharger_recharge_time("sv_healthcharger_recharge_time", "180", FCVAR_ARCHIVE);
ConVar sv_suitcharger_recharge("sv_suitcharger_recharge", "1", FCVAR_ARCHIVE);
ConVar sv_suitcharger_recharge_time("sv_suitcharger_recharge_time", "180", FCVAR_ARCHIVE);
ConVar sv_item_respawn("sv_item_respawn", "1", FCVAR_ARCHIVE);
ConVar sv_item_respawn_time("sv_item_respawn_time", "180", FCVAR_ARCHIVE);
ConVar sv_weapon_respawn("sv_weapon_respawn", "1", FCVAR_ARCHIVE);
ConVar sv_weapon_respawn_time("sv_weapon_respawn_time", "180", FCVAR_ARCHIVE);

ConVar sv_player_dropweaponsondeath("sv_player_dropweaponsondeath", "1", FCVAR_ARCHIVE);
ConVar sv_player_autoaimcrosshair("sv_player_autoaimcrosshair", "0", FCVAR_ARCHIVE);

extern ConVar sv_player_voice;
extern ConVar sv_player_voice_kill_freq;
extern ConVar sv_player_voice_kill;

//=========================================================
//=========================================================
bool CSingleplayRules::IsMultiplayer( void )
{
	return gpGlobals->maxClients > 1;
}

// Needed during the conversion, but once DMG_* types have been fixed, this isn't used anymore.
//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
int CSingleplayRules::Damage_GetTimeBased( void )
{
	int iDamage = ( DMG_PARALYZE | DMG_NERVEGAS | DMG_POISON | DMG_RADIATION | DMG_DROWNRECOVER | DMG_ACID | DMG_SLOWBURN );
	return iDamage;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
int	CSingleplayRules::Damage_GetShouldGibCorpse( void )
{
	int iDamage = ( DMG_CRUSH | DMG_FALL | DMG_BLAST | DMG_SONIC | DMG_CLUB );
	return iDamage;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
int CSingleplayRules::Damage_GetShowOnHud( void )
{
	int iDamage = ( DMG_POISON | DMG_ACID | DMG_DROWN | DMG_BURN | DMG_SLOWBURN | DMG_NERVEGAS | DMG_RADIATION | DMG_SHOCK );
	return iDamage;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
int	CSingleplayRules::Damage_GetNoPhysicsForce( void )
{
	int iTimeBasedDamage = Damage_GetTimeBased();
	int iDamage = ( DMG_FALL | DMG_BURN | DMG_PLASMA | DMG_DROWN | iTimeBasedDamage | DMG_CRUSH | DMG_PHYSGUN | DMG_PREVENT_PHYSICS_FORCE );
	return iDamage;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
int	CSingleplayRules::Damage_GetShouldNotBleed( void )
{
	int iDamage = ( DMG_POISON | DMG_ACID );
	return iDamage;
}


//-----------------------------------------------------------------------------
// Purpose: 
// Input  : iDmgType - 
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
bool CSingleplayRules::Damage_IsTimeBased( int iDmgType )
{
	// Damage types that are time-based.
	return ( ( iDmgType & ( DMG_PARALYZE | DMG_NERVEGAS | DMG_POISON | DMG_RADIATION | DMG_DROWNRECOVER | DMG_ACID | DMG_SLOWBURN ) ) != 0 );
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : iDmgType - 
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
bool CSingleplayRules::Damage_ShouldGibCorpse( int iDmgType )
{
	// Damage types that gib the corpse.
	return ( ( iDmgType & ( DMG_CRUSH | DMG_FALL | DMG_BLAST | DMG_SONIC | DMG_CLUB ) ) != 0 );
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : iDmgType - 
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
bool CSingleplayRules::Damage_ShowOnHUD( int iDmgType )
{
	// Damage types that have client HUD art.
	return ( ( iDmgType & ( DMG_POISON | DMG_ACID | DMG_DROWN | DMG_BURN | DMG_SLOWBURN | DMG_NERVEGAS | DMG_RADIATION | DMG_SHOCK ) ) != 0 );
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : iDmgType - 
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
bool CSingleplayRules::Damage_NoPhysicsForce( int iDmgType )
{
	// Damage types that don't have to supply a physics force & position.
	int iTimeBasedDamage = Damage_GetTimeBased();
	return ( ( iDmgType & ( DMG_FALL | DMG_BURN | DMG_PLASMA | DMG_DROWN | iTimeBasedDamage | DMG_CRUSH | DMG_PHYSGUN | DMG_PREVENT_PHYSICS_FORCE ) ) != 0 );
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : iDmgType - 
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
bool CSingleplayRules::Damage_ShouldNotBleed( int iDmgType )
{
	// Damage types that don't make the player bleed.
	return ( ( iDmgType & ( DMG_POISON | DMG_ACID ) ) != 0 );
}

#ifdef CLIENT_DLL

#else

	extern CGameRules	*g_pGameRules;
	extern bool		g_fGameOver;

	//=========================================================
	//=========================================================
	CSingleplayRules::CSingleplayRules( void )
	{
		RefreshSkillData( true );

		char mapcfg[256];
		Q_snprintf(mapcfg, sizeof(mapcfg), "%s.cfg", STRING(gpGlobals->mapname));

		Q_FixSlashes(mapcfg);
		Q_strlower(mapcfg);

		if (mapcfg)
		{
			char szCommandMap[256];

			Log("Executing map config file %s\n", mapcfg);
			Q_snprintf(szCommandMap, sizeof(szCommandMap), "exec %s\n", mapcfg);
			engine->ServerCommand(szCommandMap);
		}

		char mapname[256];
#if !defined( CLIENT_DLL )
		Q_snprintf(mapname, sizeof(mapname), "maps/%s", STRING(gpGlobals->mapname));
#else
		Q_strncpy(mapname, engine->GetLevelName(), sizeof(mapname));
#endif

		Q_FixSlashes(mapname);
		Q_strlower(mapname);

		if (g_fr_spawneroldfunctionality.GetBool())
		{
			SetGamemode(g_gamemode.GetInt());
			Log("Setting the gamemode to %i.\n", GetGamemode());
		}

		if (V_stristr(mapname, "cf_"))
		{
			if (!g_fr_spawneroldfunctionality.GetBool())
			{
				bSkipFuncCheck = true;
			}
			SetGamemode(FIREFIGHT_PRIMARY_COMBINEFIREFIGHT);
			Log("Automatically setting the gamemode to COMBINE FIREFIGHT due to mapname.\n");
		}
		else if (V_stristr(mapname, "xi_"))
		{
			if (!g_fr_spawneroldfunctionality.GetBool())
			{
				bSkipFuncCheck = true;
			}
			SetGamemode(FIREFIGHT_PRIMARY_XENINVASION);
			Log("Automatically setting the gamemode to XEN INVASION due to mapname.\n");
		}
		else if (V_stristr(mapname, "aa_"))
		{
			if (!g_fr_spawneroldfunctionality.GetBool())
			{
				bSkipFuncCheck = true;
			}
			SetGamemode(FIREFIGHT_PRIMARY_ANTLIONASSAULT);
			Log("Automatically setting the gamemode to ANTLION ASSAULT due to mapname.\n");
		}
		else if (V_stristr(mapname, "zs_"))
		{
			if (!g_fr_spawneroldfunctionality.GetBool())
			{
				bSkipFuncCheck = true;
			}
			SetGamemode(FIREFIGHT_PRIMARY_ZOMBIESURVIVAL);
			Log("Automatically setting the gamemode to ZOMBIE SURVIVAL due to mapname.\n");
		}
		else if (V_stristr(mapname, "fr_"))
		{
			if (!g_fr_spawneroldfunctionality.GetBool())
			{
				bSkipFuncCheck = true;
			}
			SetGamemode(FIREFIGHT_PRIMARY_FIREFIGHTRUMBLE);
			Log("Automatically setting the gamemode to FIREFIGHT RUMBLE due to mapname.\n");
		}
		else
		{
			bSkipFuncCheck = false;
		}

		if (bSkipFuncCheck)
		{
			Log("Overriding g_fr_spawneroldfunctionality...\n");
		}

		if (bSkipFuncCheck || g_fr_spawneroldfunctionality.GetBool())
		{
			if (GetGamemode() == FIREFIGHT_PRIMARY_DEFAULT)
			{
				Log("No gamemode defined! Randomizing gamemodes.\n");
				SetGamemodeRandom(FIREFIGHT_PRIMARY_COMBINEFIREFIGHT, FIREFIGHT_PRIMARY_FIREFIGHTRUMBLE);
			}

			const char* cfgfilecf = combinefirefightcfgfile.GetString();
			const char* cfgfilexi = xeninvasioncfgfile.GetString();
			const char* cfgfileaa = antlionassaultcfgfile.GetString();
			const char* cfgfilezs = zombiesurvivalcfgfile.GetString();
			const char* cfgfilefr = firefightrumblecfgfile.GetString();

			switch (g_pGameRules->GetGamemode())
			{
			case FIREFIGHT_PRIMARY_COMBINEFIREFIGHT:
				if (cfgfilecf && cfgfilecf[0])
				{
					char szCommand[256];

					Log("Executing COMBINE FIREFIGHT gamemode config file %s\n", cfgfilecf);
					Q_snprintf(szCommand, sizeof(szCommand), "exec %s\n", cfgfilecf);
					engine->ServerCommand(szCommand);
				}
				break;
			case FIREFIGHT_PRIMARY_XENINVASION:
				if (cfgfilexi && cfgfilexi[0])
				{
					char szCommand[256];

					Log("Executing XEN INVASION gamemode config file %s\n", cfgfilexi);
					Q_snprintf(szCommand, sizeof(szCommand), "exec %s\n", cfgfilexi);
					engine->ServerCommand(szCommand);
				}
				break;
			case FIREFIGHT_PRIMARY_ANTLIONASSAULT:
				if (cfgfileaa && cfgfileaa[0])
				{
					char szCommand[256];

					Log("Executing ANTLION ASSAULT gamemode config file %s\n", cfgfileaa);
					Q_snprintf(szCommand, sizeof(szCommand), "exec %s\n", cfgfileaa);
					engine->ServerCommand(szCommand);
				}
				break;
			case FIREFIGHT_PRIMARY_ZOMBIESURVIVAL:
				if (cfgfilezs && cfgfilezs[0])
				{
					char szCommand[256];

					Log("Executing ZOMBIE SURVIVAL gamemode config file %s\n", cfgfilezs);
					Q_snprintf(szCommand, sizeof(szCommand), "exec %s\n", cfgfilezs);
					engine->ServerCommand(szCommand);
				}
				break;
			case FIREFIGHT_PRIMARY_FIREFIGHTRUMBLE:
				if (cfgfilefr && cfgfilefr[0])
				{
					char szCommand[256];

					Log("Executing FIREFIGHT RUMBLE gamemode config file %s\n", cfgfilefr);
					Q_snprintf(szCommand, sizeof(szCommand), "exec %s\n", cfgfilefr);
					engine->ServerCommand(szCommand);
				}
				break;
			default:
				Log("No gamemode? What did you do?\n");
				break;
			}
		}
	}

	//=========================================================
	//=========================================================
	void CSingleplayRules::Think ( void )
	{
		BaseClass::Think();
	}

	//=========================================================
	//=========================================================
	bool CSingleplayRules::IsDeathmatch ( void )
	{
		return false;
	}

	//=========================================================
	//=========================================================
	bool CSingleplayRules::IsCoOp( void )
	{
		return gpGlobals->maxClients > 1;
	}

	//-----------------------------------------------------------------------------
	// Purpose: Determine whether the player should switch to the weapon passed in
	// Output : Returns true on success, false on failure.
	//-----------------------------------------------------------------------------
	static ConVarRef sv_player_autoswitch( "sv_player_autoswitch" );
	bool CSingleplayRules::FShouldSwitchWeapon( CBasePlayer *pPlayer, CBaseCombatWeapon *pWeapon )
	{
		if ( !sv_player_autoswitch.GetBool() )
			return false;

		//Must have ammo
		if ( ( pWeapon->HasAnyAmmo() == false ) && ( pPlayer->GetAmmoCount( pWeapon->m_iPrimaryAmmoType ) <= 0 ) )
			return false;

		//Always take a loaded gun if we have nothing else
		if ( pPlayer->GetActiveWeapon() == NULL )
			return true;

		// The given weapon must allow autoswitching to it from another weapon.
		if ( !pWeapon->AllowsAutoSwitchTo() )
			return false;

		// The active weapon must allow autoswitching from it.
		if ( !pPlayer->GetActiveWeapon()->AllowsAutoSwitchFrom() )
			return false;

		//Don't switch if our current gun doesn't want to be holstered
		if ( pPlayer->GetActiveWeapon()->CanHolster() == false )
			return false;

		//Only switch if the weapon is better than what we're using
		if ( ( pWeapon != pPlayer->GetActiveWeapon() ) && ( pWeapon->GetWeight() <= pPlayer->GetActiveWeapon()->GetWeight() ) )
			return false;

		return true;
	}

	//-----------------------------------------------------------------------------
	// Purpose: Find the next best weapon to use and return it.
	//-----------------------------------------------------------------------------
	CBaseCombatWeapon *CSingleplayRules::GetNextBestWeapon( CBaseCombatCharacter *pPlayer, CBaseCombatWeapon *pCurrentWeapon )
	{
		if ( pCurrentWeapon && !pCurrentWeapon->AllowsAutoSwitchFrom() )
			return NULL;

		CBaseCombatWeapon	*pBestWeapon = NULL;
		CBaseCombatWeapon	*pWeapon;
		
		int	nBestWeight	= -1;

		//Search for the best weapon to use next based on its weight
		for ( int i = 0; i < pPlayer->WeaponCount(); i++ )
		{
			pWeapon = pPlayer->GetWeapon(i);

			if ( pWeapon == NULL )
				continue;

			// If we have an active weapon and this weapon doesn't allow autoswitching away
			// from another weapon, skip it.
			if ( pCurrentWeapon && !pWeapon->AllowsAutoSwitchTo() )
				continue;

			// Must be eligible for switching to.
			if (!pPlayer->Weapon_CanSwitchTo(pWeapon))
				continue;
			
			// Must be of higher quality.
			if ( pWeapon->GetWeight() <= nBestWeight )
				continue;

			// We must have primary ammo
			if ( pWeapon->UsesClipsForAmmo1() && pWeapon->Clip1() <= 0 && !pPlayer->GetAmmoCount( pWeapon->GetPrimaryAmmoType() ) )
				continue;

			// This is a better candidate than what we had.
			nBestWeight = pWeapon->GetWeight();
			pBestWeapon = pWeapon;
		}

		return pBestWeapon;
	}

	//-----------------------------------------------------------------------------
	// Purpose: 
	// Output : Returns true on success, false on failure.
	//-----------------------------------------------------------------------------
	bool CSingleplayRules::SwitchToNextBestWeapon( CBaseCombatCharacter *pPlayer, CBaseCombatWeapon *pCurrentWeapon )
	{
		CBaseCombatWeapon *pWeapon = GetNextBestWeapon( pPlayer, pCurrentWeapon );

		if ( pWeapon != NULL )
			return pPlayer->Weapon_Switch( pWeapon );

		return false;
	}

	//=========================================================
	//=========================================================
	bool CSingleplayRules::ClientConnected( edict_t *pEntity, const char *pszName, const char *pszAddress, char *reject, int maxrejectlen )
	{
		return true;
	}

	void CSingleplayRules::InitHUD( CBasePlayer *pl )
	{
	}

	//=========================================================
	//=========================================================
	void CSingleplayRules::ClientDisconnected( edict_t *pClient )
	{
	}

	//=========================================================
	//=========================================================
	float CSingleplayRules::FlPlayerFallDamage( CBasePlayer *pPlayer )
	{
		// subtract off the speed at which a player is allowed to fall without being hurt,
		// so damage will be based on speed beyond that, not the entire fall
		//pPlayer->m_Local.m_flFallVelocity -= PLAYER_MAX_SAFE_FALL_SPEED;
		//return pPlayer->m_Local.m_flFallVelocity * DAMAGE_FOR_FALL_SPEED;
		return 0;
	}

	//=========================================================
	//=========================================================
	bool CSingleplayRules::AllowDamage( CBaseEntity *pVictim, const CTakeDamageInfo &info )
	{
		return true;
	}

	//=========================================================
	//=========================================================
	void CSingleplayRules::PlayerSpawn( CBasePlayer *pPlayer )
	{
		bool		addDefault;
		CBaseEntity* pWeaponEntity = NULL;

		pPlayer->EquipSuit();

		addDefault = true;

		while ((pWeaponEntity = gEntList.FindEntityByClassname(pWeaponEntity, "game_player_equip")) != NULL)
		{
			pWeaponEntity->Touch(pPlayer);
			addDefault = false;
		}
	}

	//=========================================================
	//=========================================================
	bool CSingleplayRules::AllowAutoTargetCrosshair( void )
	{
		return ( IsSkillLevel(SKILL_EASY) || sv_player_autoaimcrosshair.GetBool());
	}

	//=========================================================
	//=========================================================
	int	CSingleplayRules::GetAutoAimMode()
	{
		return sk_autoaim_mode.GetInt();
	}

	//=========================================================
	//=========================================================
	bool CSingleplayRules::FPlayerCanRespawn( CBasePlayer *pPlayer )
	{
		return true;
	}

	//=========================================================
	//=========================================================
	float CSingleplayRules::FlPlayerSpawnTime( CBasePlayer *pPlayer )
	{
		return gpGlobals->curtime;//now!
	}

	//=========================================================
	// IPointsForKill - how many points awarded to anyone
	// that kills this player?
	//=========================================================
	int CSingleplayRules::IPointsForKill(CBasePlayer *pAttacker, CBasePlayer *pKilled)
	{
		return 1;
	}

	int CSingleplayRules::IPointsForKillEntity(CBasePlayer *pAttacker, CBaseEntity *pKilled)
	{
		return 1;
	}
	
	//-----------------------------------------------------------------------------
	// Purpose: 
	//-----------------------------------------------------------------------------
	CBasePlayer *CSingleplayRules::GetDeathScorer( CBaseEntity *pKiller, CBaseEntity *pInflictor )
	{
		if ( pKiller)
		{
			if ( pKiller->Classify() == CLASS_PLAYER )
				return (CBasePlayer*)pKiller;

			if (!pKiller->IsNPC())
			{
				// Killing entity might be specifying a scorer player
				IScorer *pScorerInterface = dynamic_cast<IScorer*>(pKiller);
				if (pScorerInterface)
				{
					CBasePlayer *pPlayer = pScorerInterface->GetScorer();
					if (pPlayer)
						return pPlayer;
				}

				// Inflicting entity might be specifying a scoring player
				pScorerInterface = dynamic_cast<IScorer*>(pInflictor);
				if (pScorerInterface)
				{
					CBasePlayer *pPlayer = pScorerInterface->GetScorer();
					if (pPlayer)
						return pPlayer;
				}
			}
		}

		return NULL;
	}

	//-----------------------------------------------------------------------------
	// Purpose: Returns player who should receive credit for kill
	//-----------------------------------------------------------------------------
	CBasePlayer *CSingleplayRules::GetDeathScorer( CBaseEntity *pKiller, CBaseEntity *pInflictor, CBaseEntity *pVictim )
	{
		// if this method not overridden by subclass, just call our default implementation
		return GetDeathScorer( pKiller, pInflictor );
	}

#define KILLING_SPREE_AMOUNT	5
#define KILLING_FRENZY_AMOUNT	10
#define OVERKILL_AMOUNT	15
#define RAMPAGE_AMOUNT	20
#define UNSTOPPABLE_AMOUNT	25
#define INCONCEIVABLE_AMOUNT	30
#define INVINCIBLE_AMOUNT	35
#define GODLIKE_AMOUNT	40

#define CLASSICLEVELUP_AMOUNT	15

	//=========================================================
	// PlayerKilled - someone/something killed this player
	//=========================================================
	void CSingleplayRules::PlayerKilled( CBasePlayer *pVictim, const CTakeDamageInfo &info )
	{
		DeathNotice( pVictim, info );

		// Find the killer & the scorer
		CBaseEntity* pInflictor = info.GetInflictor();
		CBaseEntity* pKiller = info.GetAttacker();
		CBasePlayer* pScorer = GetDeathScorer(pKiller, pInflictor, pVictim);

		pVictim->IncrementDeathCount(1);

		// dvsents2: uncomment when removing all FireTargets
		// variant_t value;
		// g_EventQueue.AddEvent( "game_playerdie", "Use", value, 0, pVictim, pVictim );
		FireTargets("game_playerdie", pVictim, pVictim, USE_TOGGLE, 0);

		if (pScorer && pVictim != pScorer)
		{
			if (!pScorer->IsNPC())
			{
				// if a player dies in a deathmatch game and the killer is a client, award the killer some points
				pScorer->IncrementFragCount(IPointsForKill(pScorer, pVictim));

				if (sv_player_voice.GetBool())
				{
					if (sv_player_voice_kill.GetBool())
					{
						int killvoicerandom = random->RandomInt(0, sv_player_voice_kill_freq.GetInt());
						if (killvoicerandom == 0)
						{
							pScorer->EmitSound("Player.VoiceKill");
						}
					}
				}
			}

			// dvsents2: uncomment when removing all FireTargets
			//variant_t value;
			//g_EventQueue.AddEvent( "game_playerkill", "Use", value, 0, pScorer, pScorer );
			FireTargets("game_playerkill", pScorer, pScorer, USE_TOGGLE, 0);
		}
	}

	void CSingleplayRules::NPCKilled(CBaseEntity *pVictim, const CTakeDamageInfo &info)
	{
		// Find the killer & the scorer
		CBaseEntity *pInflictor = info.GetInflictor();
		CBaseEntity *pKiller = info.GetAttacker();
		CBasePlayer *pEntity = GetDeathScorer(pKiller, pInflictor, pVictim);

		int moneyReward = 0;
		int xpReward = 0;
		
		if (pEntity)
		{
			if (pEntity->GetDefaultRelationshipDisposition(pVictim->Classify()) == D_HT)
			{
				if (pVictim->m_isRareEntity)
				{
					switch (GetSkillLevel())
					{
					case SKILL_EASY:
						moneyReward += 6 * sk_money_multiplier1.GetInt();
						xpReward += 10 * sk_exp_multiplier1.GetInt();
						break;

					case SKILL_MEDIUM:
						moneyReward += 6 * sk_money_multiplier2.GetInt();
						xpReward += 10 * sk_exp_multiplier2.GetInt();
						break;

					case SKILL_HARD:
						moneyReward += 6 * sk_money_multiplier3.GetInt();
						xpReward += 10 * sk_exp_multiplier3.GetInt();
						break;

					case SKILL_VERYHARD:
						moneyReward += 6 * sk_money_multiplier4.GetInt();
						xpReward += 10 * sk_exp_multiplier4.GetInt();
						break;

					case SKILL_NIGHTMARE:
						moneyReward += 6 * sk_money_multiplier5.GetInt();
						xpReward += 10 * sk_exp_multiplier5.GetInt();
						break;
					}
				}
				else
				{
					switch (GetSkillLevel())
					{
					case SKILL_EASY:
						moneyReward += 4 * sk_money_multiplier1.GetInt();
						xpReward += 6 * sk_exp_multiplier1.GetInt();
						break;

					case SKILL_MEDIUM:
						moneyReward += 4 * sk_money_multiplier2.GetInt();
						xpReward += 6 * sk_exp_multiplier2.GetInt();
						break;

					case SKILL_HARD:
						moneyReward += 4 * sk_money_multiplier3.GetInt();
						xpReward += 6 * sk_exp_multiplier3.GetInt();
						break;

					case SKILL_VERYHARD:
						moneyReward += 4 * sk_money_multiplier4.GetInt();
						xpReward += 6 * sk_exp_multiplier4.GetInt();
						break;

					case SKILL_NIGHTMARE:
						moneyReward += 4 * sk_money_multiplier5.GetInt();
						xpReward += 6 * sk_exp_multiplier5.GetInt();
						break;
					}
				}

				pEntity->IncrementFragCount(IPointsForKillEntity(pEntity, pVictim));

				if (info.GetInflictor() == pEntity)
				{
					if (sv_player_voice.GetBool())
					{
						if (sv_player_voice_kill.GetBool())
						{
							int killvoicerandom = random->RandomInt(0, sv_player_voice_kill_freq.GetInt());
							if (killvoicerandom == 0)
							{
								pEntity->EmitSound("Player.VoiceKill");
							}
						}
					}
				}

				//make it so the katana hit multiplier shows up.
				if (sv_killingspree.GetBool() && !isInBullettime)
				{
					int m_iKillsInSpree = pEntity->FragCount();

					CFmtStr hint;

					switch (m_iKillsInSpree)
					{
						case KILLING_SPREE_AMOUNT:
							hint.sprintf("#Valve_Hud_KILLINGSPREE");
							pEntity->ShowLevelMessage(hint.Access());
							moneyReward += 2;
							xpReward += 3;
							break;
						case KILLING_FRENZY_AMOUNT:
							hint.sprintf("#Valve_Hud_KILLINGFRENZY");
							pEntity->ShowLevelMessage(hint.Access());
							moneyReward += 4;
							xpReward += 6;
							break;
						case OVERKILL_AMOUNT:
							hint.sprintf("#Valve_Hud_OVERKILL");
							pEntity->ShowLevelMessage(hint.Access());
							moneyReward += 6;
							xpReward += 9;
							break;
						case RAMPAGE_AMOUNT:
							hint.sprintf("#Valve_Hud_RAMPAGE");
							pEntity->ShowLevelMessage(hint.Access());
							moneyReward += 8;
							xpReward += 12;
							break;
						case UNSTOPPABLE_AMOUNT:
							hint.sprintf("#Valve_Hud_UNSTOPPABLE");
							pEntity->ShowLevelMessage(hint.Access());
							moneyReward += 10;
							xpReward += 15;
							break;
						case INCONCEIVABLE_AMOUNT:
							hint.sprintf("#Valve_Hud_INCONCEIVABLE");
							pEntity->ShowLevelMessage(hint.Access());
							moneyReward += 12;
							xpReward += 18;
							break;
						case INVINCIBLE_AMOUNT:
							hint.sprintf("#Valve_Hud_INVINCIBLE");
							pEntity->ShowLevelMessage(hint.Access());
							moneyReward += 14;
							xpReward += 21;
							break;
						case GODLIKE_AMOUNT:
							hint.sprintf("#Valve_Hud_GODLIKE");
							pEntity->ShowLevelMessage(hint.Access());
							moneyReward += 16;
							xpReward += 24;
							break;
						default:
							break;
					}
				}

				if (pVictim->m_iExtraMoney > 0)
				{
					moneyReward += pVictim->m_iExtraMoney;
				}

				if (pVictim->m_iExtraExp > 0)
				{
					xpReward += pVictim->m_iExtraExp;
				}

				if ((info.GetDamageType() & DMG_BLAST) ||
					(info.GetDamageType() & DMG_SHOCK) ||
					(info.GetDamageType() & DMG_SNIPER) ||
					(info.GetDamageType() & DMG_BUCKSHOT) ||
					(info.GetDamageType() & DMG_ALWAYSGIB))
				{
					moneyReward += 50;
					xpReward += 50;
				}

				CAI_BaseNPC* pNPC = (CAI_BaseNPC*)pEntity;

				if (pNPC && pNPC->m_bDecapitated)
				{
					moneyReward += 50;
					xpReward += 50;
				}

				CHL2_Player* pEntityHL2 = ToHL2Player(pEntity);

				if (pEntityHL2 && pEntityHL2->m_bIsKicking)
				{
					moneyReward += 30;
					xpReward += 30;
				}

				if (pInflictor)
				{
					if (FClassnameIs(pInflictor, "knife_bolt") ||
						FClassnameIs(pInflictor, "crossbow_bolt") ||
						FClassnameIs(pInflictor, "env_flare"))
					{
						moneyReward += 30;
						xpReward += 30;
					}
				}

				CBaseCombatWeapon* pWeapon = pEntity->GetActiveWeapon();

				if (pWeapon)
				{
					if (FClassnameIs(pWeapon, "weapon_katana"))
					{
						moneyReward += 50;
						xpReward += 50;
					}
				}

				moneyReward += pEntity->GetLevel() * 10;
				xpReward += pEntity->GetLevel() * 10;

				if (g_fr_economy.GetBool())
				{
					pEntity->AddMoney(moneyReward);
				}

				if (!g_fr_classic.GetBool())
				{
					pEntity->AddXP(xpReward);
				}

				if (pVictim->m_isRareEntity)
				{
					if (g_fr_classic.GetBool())
					{
						pEntity->LevelUpClassic();
					}
				}
			}
		}

		DeathNoticeNPC(pVictim, info, xpReward, moneyReward);
	}

	//=========================================================
	// Deathnotice. 
	//=========================================================
	typedef char NpcName[256];
	void CSingleplayRules::DeathNotice(CBasePlayer* pVictim, const CTakeDamageInfo& info)
	{
		// Work out what killed the player, and send a message to all clients about it
		const char* killer_weapon_name = "world";		// by default, the player is killed by the world
		int killer_ID = 0;

		// Find the killer & the scorer
		CBaseEntity* pInflictor = info.GetInflictor();
		CBaseEntity* pKiller = info.GetAttacker();
		if (pKiller->IsPlayer())
		{
			CBasePlayer* pScorer = GetDeathScorer(pKiller, pInflictor, pVictim);

			// Custom damage type?
			if (info.GetDamageCustom())
			{
				killer_weapon_name = GetDamageCustomString(info);
				if (pScorer)
				{
					killer_ID = pScorer->GetUserID();
				}
			}
			else
			{
				// Is the killer a client?
				if (pScorer)
				{
					killer_ID = pScorer->GetUserID();

					if (pInflictor)
					{
						if (pInflictor == pScorer)
						{
							CHL2_Player* pScorerHL2 = ToHL2Player(pScorer);

							// If the inflictor is the killer,  then it must be their current weapon doing the damage
							if (pScorerHL2 && pScorerHL2->m_bIsKicking)
							{
								// fake it
								killer_weapon_name = "kick";
							}
							else if (pScorer->GetActiveWeapon())
							{
								killer_weapon_name = pScorer->GetActiveWeapon()->GetClassname();
							}
						}
						else
						{
							killer_weapon_name = STRING(pInflictor->m_iClassname);  // it's just that easy
						}
					}
				}
				else
				{
					killer_weapon_name = STRING(pInflictor->m_iClassname);
				}
			}

			IGameEvent* event = gameeventmanager->CreateEvent("player_death");
			if (event)
			{
				event->SetInt("userid", pVictim->GetUserID());
				event->SetInt("attacker", killer_ID);
				event->SetInt("customkill", info.GetDamageCustom());
				event->SetInt("priority", 7);	// HLTV event priority, not transmitted
				event->SetString("weapon", killer_weapon_name);
				gameeventmanager->FireEvent(event);
			}
		}
		else if (pKiller->IsNPC())
		{
			NpcName killer_name;
			GetNPCName(killer_name, pKiller);

			CAI_BaseNPC *pNPC = pKiller->MyNPCPointer();

			if (pNPC)
			{
				if (pNPC->GetActiveWeapon())
				{
					killer_weapon_name = pNPC->GetActiveWeapon()->GetClassname();
				}
				else if (pInflictor)
				{
					killer_weapon_name = STRING(pInflictor->m_iClassname);  // it's just that easy
				}
				else
				{
					killer_weapon_name = STRING(pKiller->m_iClassname);
				}
			}
			else
			{
				killer_weapon_name = STRING(pKiller->m_iClassname);
			}

			IGameEvent* event = gameeventmanager->CreateEvent("player_death_npc");
			if (event)
			{
				event->SetInt("userid", pVictim->GetUserID());
				event->SetString("attacker", killer_name);
				event->SetInt("customkill", info.GetDamageCustom());
				event->SetInt("priority", 7);	// HLTV event priority, not transmitted
				event->SetString("weapon", killer_weapon_name);
				gameeventmanager->FireEvent(event);
			}
		}
	}

	void CSingleplayRules::DeathNoticeNPC(CBaseEntity* pVictim, const CTakeDamageInfo& info, int xpReward, int moneyReward)
	{
		// Work out what killed the player, and send a message to all clients about it
		const char* killer_weapon_name = "world";		// by default, the player is killed by the world
		int killer_ID = 0;

		// Find the killer & the scorer
		CBaseEntity* pInflictor = info.GetInflictor();
		CBaseEntity* pKiller = info.GetAttacker();
		if (pKiller->IsPlayer())
		{
			CBasePlayer* pScorer = GetDeathScorer(pKiller, pInflictor, pVictim);

			// Custom damage type?
			if (info.GetDamageCustom())
			{
				killer_weapon_name = GetDamageCustomString(info);
				if (pScorer)
				{
					killer_ID = pScorer->GetUserID();
				}
			}
			else
			{
				// Is the killer a client?
				if (pScorer)
				{
					killer_ID = pScorer->GetUserID();

					if (pInflictor)
					{
						if (pInflictor == pScorer)
						{
							CHL2_Player* pScorerHL2 = ToHL2Player(pScorer);
							CAI_BaseNPC* pNPC = pVictim->MyNPCPointer();

							// If the inflictor is the killer,  then it must be their current weapon doing the damage
							if (pNPC->m_bDecapitated)
							{
								// fake it
								killer_weapon_name = "headshot";
							}
							else if (pScorerHL2 && pScorerHL2->m_bIsKicking)
							{
								// fake it
								killer_weapon_name = "kick";
							}
							else if (pScorer->GetActiveWeapon())
							{
								killer_weapon_name = pScorer->GetActiveWeapon()->GetClassname();
							}
						}
						else
						{
							killer_weapon_name = STRING(pInflictor->m_iClassname);  // it's just that easy
						}
					}
				}
				else
				{
					killer_weapon_name = STRING(pInflictor->m_iClassname);
				}
			}

			NpcName vic_name;
			GetNPCName(vic_name, pVictim);

			IGameEvent* event = gameeventmanager->CreateEvent("npc_death");
			if (event)
			{
				event->SetInt("attacker", killer_ID);
				event->SetString("victimname", vic_name);
				event->SetInt("customkill", info.GetDamageCustom());
				event->SetInt("xpreward", xpReward);
				event->SetInt("moneyreward", moneyReward);
				event->SetInt("priority", 7);	// HLTV event priority, not transmitted
				event->SetString("weapon", killer_weapon_name);
				gameeventmanager->FireEvent(event);
			}
		}
	}

	void CSingleplayRules::GetNPCName(NpcName npcName, CBaseEntity* pVictim)
	{
		//todo: use properly localized names.
		const char* entityClassname = pVictim->GetClassname();
		const char* fullEntityClassname = entityClassname;
		bool useLocalization = true;

		CAI_BaseNPC *pNPC = pVictim->MyNPCPointer();
		if (pNPC && pNPC->m_pAttributes != NULL)
		{
			const char* presetName = pNPC->m_pAttributes->GetString("name", "NULL");

			if (Q_strnicmp(presetName, "NULL", 11) != 0)
			{
				useLocalization = false;
				fullEntityClassname = presetName;
			}
			else 
			{
				if (pNPC->m_pAttributes->presetNum > 0)
				{
					if (pNPC->m_pAttributes->wildcard)
					{
						entityClassname = "npc_any";
					}

					fullEntityClassname = CFmtStr("%s_%i", entityClassname, pNPC->m_pAttributes->presetNum);
				}
			}
		}

		const char* FinalString = fullEntityClassname;

		if (useLocalization)
		{
			CFmtStr localizedName;
			localizedName.sprintf("#fr_%s", fullEntityClassname);

			Q_strncpy(npcName, localizedName.Access(), sizeof(NpcName));
		}
		else
		{
			Q_strncpy(npcName, FinalString, sizeof(NpcName));
		}
	}

	//=========================================================
	// FlWeaponRespawnTime - what is the time in the future
	// at which this weapon may spawn?
	//=========================================================
	float CSingleplayRules::FlWeaponRespawnTime( CBaseCombatWeapon *pWeapon )
	{
		if (sv_weapon_respawn.GetBool())
		{
			return sv_weapon_respawn_time.GetFloat();
		}
		else
		{
			return -1;
		}
	}
	
	#define ENTITY_INTOLERANCE	100
	//=========================================================
	// FlWeaponRespawnTime - Returns 0 if the weapon can respawn 
	// now,  otherwise it returns the time at which it can try
	// to spawn again.
	//=========================================================
	float CSingleplayRules::FlWeaponTryRespawn( CBaseCombatWeapon *pWeapon )
	{
		if ( gEntList.NumberOfEntities() < (gpGlobals->maxEntities - ENTITY_INTOLERANCE) )
			return 0;

		// we're past the entity tolerance level,  so delay the respawn
		return FlWeaponRespawnTime( pWeapon );
	}

	//=========================================================
	// VecWeaponRespawnSpot - where should this weapon spawn?
	// Some game variations may choose to randomize spawn locations
	//=========================================================
	Vector CSingleplayRules::VecWeaponRespawnSpot( CBaseCombatWeapon *pWeapon )
	{
		return pWeapon->GetAbsOrigin();
	}

	//=========================================================
	// WeaponShouldRespawn - any conditions inhibiting the
	// respawning of this weapon?
	//=========================================================
	int CSingleplayRules::WeaponShouldRespawn( CBaseCombatWeapon *pWeapon )
	{
		if (sv_weapon_respawn.GetBool())
		{
			if (pWeapon->HasSpawnFlags(SF_NORESPAWN))
			{
				return GR_WEAPON_RESPAWN_NO;
			}
			else
			{
				return GR_WEAPON_RESPAWN_YES;
			}
		}
		else
		{
			return GR_WEAPON_RESPAWN_NO;
		}
	}

	//=========================================================
	//=========================================================
	bool CSingleplayRules::CanHaveItem( CBasePlayer *pPlayer, CItem *pItem )
	{
		return true;
	}

	//=========================================================
	//=========================================================
	void CSingleplayRules::PlayerGotItem( CBasePlayer *pPlayer, CItem *pItem )
	{
	}

	//=========================================================
	//=========================================================
	int CSingleplayRules::ItemShouldRespawn( CItem *pItem )
	{
		if (sv_item_respawn.GetBool())
		{
			if (pItem->HasSpawnFlags(SF_NORESPAWN))
			{
				return GR_ITEM_RESPAWN_NO;
			}
			else
			{
				return GR_ITEM_RESPAWN_YES;
			}
		}
		else
		{
			return GR_ITEM_RESPAWN_NO;
		}
	}


	//=========================================================
	// At what time in the future may this Item respawn?
	//=========================================================
	float CSingleplayRules::FlItemRespawnTime( CItem *pItem )
	{
		if (sv_item_respawn.GetBool())
		{
			return sv_item_respawn_time.GetFloat();
		}
		else
		{
			return -1;
		}
	}

	//=========================================================
	// Where should this item respawn?
	// Some game variations may choose to randomize spawn locations
	//=========================================================
	Vector CSingleplayRules::VecItemRespawnSpot( CItem *pItem )
	{
		return pItem->GetAbsOrigin();
	}

	//=========================================================
	// What angles should this item use to respawn?
	//=========================================================
	QAngle CSingleplayRules::VecItemRespawnAngles( CItem *pItem )
	{
		return pItem->GetAbsAngles();
	}

	//=========================================================
	//=========================================================
	bool CSingleplayRules::IsAllowedToSpawn( CBaseEntity *pEntity )
	{
		return true;
	}

	//=========================================================
	//=========================================================
	void CSingleplayRules::PlayerGotAmmo( CBaseCombatCharacter *pPlayer, char *szName, int iCount )
	{
	}

	//=========================================================
	//=========================================================
	float CSingleplayRules::FlHealthChargerRechargeTime( void )
	{
		if (sv_healthcharger_recharge.GetBool())
		{
			return sv_healthcharger_recharge_time.GetFloat();
		}
		else
		{
			return 0;// don't recharge
		}
	}

	float CSingleplayRules::FlHEVChargerRechargeTime(void)
	{
		if (sv_suitcharger_recharge.GetBool())
		{
			return sv_suitcharger_recharge_time.GetFloat();
		}
		else
		{
			return 0;// don't recharge
		}
	}

	//=========================================================
	//=========================================================
	int CSingleplayRules::DeadPlayerWeapons( CBasePlayer *pPlayer )
	{
		if (sv_player_dropweaponsondeath.GetBool())
		{
			return GR_PLR_DROP_GUN_ACTIVE;
		}
		else
		{
			return GR_PLR_DROP_GUN_NO;
		}
	}

	//=========================================================
	//=========================================================
	int CSingleplayRules::DeadPlayerAmmo( CBasePlayer *pPlayer )
	{
		return GR_PLR_DROP_AMMO_NO;
	}

	//=========================================================
	//=========================================================
	int CSingleplayRules::PlayerRelationship( CBaseEntity *pPlayer, CBaseEntity *pTarget )
	{
		// why would a single player in half life need this? 
		return GR_TEAMMATE;
	}

	//=========================================================
	//=========================================================
	bool CSingleplayRules::PlayerCanHearChat( CBasePlayer *pListener, CBasePlayer *pSpeaker )
	{
		return true;
	}

	//=========================================================
	//=========================================================
	bool CSingleplayRules::FAllowNPCs( void )
	{
		return true;
	}

#endif

