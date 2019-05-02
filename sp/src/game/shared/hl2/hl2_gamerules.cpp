//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: The Half-Life 2 game rules, such as the relationship tables and ammo
//			damage cvars.
//
//=============================================================================

#include "cbase.h"
#include "hl2_gamerules.h"
#include "ammodef.h"
#include "hl2_shareddefs.h"
#include "viewport_panel_names.h"

#ifdef CLIENT_DLL

#else
	#include "player.h"
	#include "game.h"
	#include "gamerules.h"
	#include "teamplay_gamerules.h"
	#include "hl2_player.h"
	#include "voice_gamemgr.h"
	#include "globalstate.h"
	#include "ai_basenpc.h"
	#include "weapon_physcannon.h"
	#include "Sprite.h"
	#include "utlbuffer.h"
	#include "filesystem.h"
	#include "cdll_int.h"
#endif

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"


REGISTER_GAMERULES_CLASS( CHalfLife2 );

BEGIN_NETWORK_TABLE_NOBASE( CHalfLife2, DT_HL2GameRules )
	#ifdef CLIENT_DLL
		RecvPropBool( RECVINFO( m_bMegaPhysgun ) ),
	#else
		SendPropBool( SENDINFO( m_bMegaPhysgun ) ),
	#endif
END_NETWORK_TABLE()


LINK_ENTITY_TO_CLASS( hl2_gamerules, CHalfLife2Proxy );
IMPLEMENT_NETWORKCLASS_ALIASED( HalfLife2Proxy, DT_HalfLife2Proxy )


#ifdef CLIENT_DLL
	void RecvProxy_HL2GameRules( const RecvProp *pProp, void **pOut, void *pData, int objectID )
	{
		CHalfLife2 *pRules = HL2GameRules();
		Assert( pRules );
		*pOut = pRules;
	}

	BEGIN_RECV_TABLE( CHalfLife2Proxy, DT_HalfLife2Proxy )
		RecvPropDataTable( "hl2_gamerules_data", 0, 0, &REFERENCE_RECV_TABLE( DT_HL2GameRules ), RecvProxy_HL2GameRules )
	END_RECV_TABLE()
#else
	void* SendProxy_HL2GameRules( const SendProp *pProp, const void *pStructBase, const void *pData, CSendProxyRecipients *pRecipients, int objectID )
	{
		CHalfLife2 *pRules = HL2GameRules();
		Assert( pRules );
		pRecipients->SetAllRecipients();
		return pRules;
	}

	BEGIN_SEND_TABLE( CHalfLife2Proxy, DT_HalfLife2Proxy )
		SendPropDataTable( "hl2_gamerules_data", 0, &REFERENCE_SEND_TABLE( DT_HL2GameRules ), SendProxy_HL2GameRules )
	END_SEND_TABLE()
#endif

ConVar  physcannon_mega_enabled( "physcannon_mega_enabled", "0", FCVAR_REPLICATED | FCVAR_CHEAT );

// Controls the application of the robus radius damage model.
ConVar	sv_robust_explosions( "sv_robust_explosions","1", FCVAR_REPLICATED );

// Damage scale for damage inflicted by the player on each skill level.
ConVar	sk_dmg_inflict_scale1( "sk_dmg_inflict_scale1", "1.50", FCVAR_REPLICATED );
ConVar	sk_dmg_inflict_scale2( "sk_dmg_inflict_scale2", "1.00", FCVAR_REPLICATED );
ConVar	sk_dmg_inflict_scale3( "sk_dmg_inflict_scale3", "0.75", FCVAR_REPLICATED );
ConVar	sk_dmg_inflict_scale4( "sk_dmg_inflict_scale4", "0.75", FCVAR_REPLICATED );
ConVar	sk_dmg_inflict_scale5( "sk_dmg_inflict_scale5", "0.75", FCVAR_REPLICATED );

// Damage scale for damage taken by the player on each skill level.
ConVar	sk_dmg_take_scale1( "sk_dmg_take_scale1", "0.50", FCVAR_REPLICATED );
ConVar	sk_dmg_take_scale2( "sk_dmg_take_scale2", "1.00", FCVAR_REPLICATED );
ConVar	sk_dmg_take_scale3("sk_dmg_take_scale3", "1.50", FCVAR_REPLICATED);
ConVar	sk_dmg_take_scale4("sk_dmg_take_scale4", "2.00", FCVAR_REPLICATED);
ConVar	sk_dmg_take_scale5("sk_dmg_take_scale5", "2.50", FCVAR_REPLICATED);

ConVar	sk_dmg_take_scale1_ace("sk_dmg_take_scale1_ace", "0.50", FCVAR_REPLICATED);
ConVar	sk_dmg_take_scale2_ace("sk_dmg_take_scale2_ace", "1.00", FCVAR_REPLICATED);
ConVar	sk_dmg_take_scale3_ace("sk_dmg_take_scale3_ace", "1.50", FCVAR_REPLICATED);
ConVar	sk_dmg_take_scale4_ace("sk_dmg_take_scale4_ace", "2.00", FCVAR_REPLICATED);
ConVar	sk_dmg_take_scale5_ace("sk_dmg_take_scale5_ace", "2.50", FCVAR_REPLICATED);

ConVar	sk_allow_autoaim( "sk_allow_autoaim", "1", FCVAR_REPLICATED | FCVAR_ARCHIVE_XBOX );

ConVar	firefightrumble_enemyattack("firefightrumble_enemyattack", "1", FCVAR_REPLICATED | FCVAR_ARCHIVE);

// Autoaim scale
ConVar	sk_autoaim_scale1( "sk_autoaim_scale1", "1.0", FCVAR_REPLICATED );
ConVar	sk_autoaim_scale2( "sk_autoaim_scale2", "1.0", FCVAR_REPLICATED );
//ConVar	sk_autoaim_scale3( "sk_autoaim_scale3", "0.0", FCVAR_REPLICATED ); NOT CURRENTLY OFFERED ON SKILL 3
//ConVar	sk_autoaim_scale4( "sk_autoaim_scale4", "0.0", FCVAR_REPLICATED ); NOT CURRENTLY OFFERED ON SKILL 4
//ConVar	sk_autoaim_scale5( "sk_autoaim_scale5", "0.0", FCVAR_REPLICATED ); NOT CURRENTLY OFFERED ON SKILL 5

// Quantity scale for ammo received by the player.
ConVar	sk_ammo_qty_scale1 ( "sk_ammo_qty_scale1", "1.20", FCVAR_REPLICATED );
ConVar	sk_ammo_qty_scale2 ( "sk_ammo_qty_scale2", "1.00", FCVAR_REPLICATED );
ConVar	sk_ammo_qty_scale3 ( "sk_ammo_qty_scale3", "0.60", FCVAR_REPLICATED );
ConVar	sk_ammo_qty_scale4 ( "sk_ammo_qty_scale4", "0.40", FCVAR_REPLICATED );
ConVar	sk_ammo_qty_scale5 ( "sk_ammo_qty_scale5", "0.20", FCVAR_REPLICATED );

ConVar	sk_plr_health_drop_time		( "sk_plr_health_drop_time", "30", FCVAR_REPLICATED );
ConVar	sk_plr_grenade_drop_time	( "sk_plr_grenade_drop_time", "30", FCVAR_REPLICATED );

ConVar	sk_plr_dmg_ar2			( "sk_plr_dmg_ar2","0", FCVAR_REPLICATED );
ConVar	sk_npc_dmg_ar2			( "sk_npc_dmg_ar2","0", FCVAR_REPLICATED);
ConVar	sk_max_ar2				( "sk_max_ar2","0", FCVAR_REPLICATED);
ConVar	sk_max_ar2_altfire		( "sk_max_ar2_altfire","0", FCVAR_REPLICATED);

ConVar	sk_plr_dmg_alyxgun		( "sk_plr_dmg_alyxgun","0", FCVAR_REPLICATED );
ConVar	sk_npc_dmg_alyxgun		( "sk_npc_dmg_alyxgun","0", FCVAR_REPLICATED);
ConVar	sk_max_alyxgun			( "sk_max_alyxgun","0", FCVAR_REPLICATED);

ConVar	sk_plr_dmg_pistol		( "sk_plr_dmg_pistol","0", FCVAR_REPLICATED );
ConVar	sk_npc_dmg_pistol		( "sk_npc_dmg_pistol","0", FCVAR_REPLICATED);
ConVar	sk_max_pistol			( "sk_max_pistol","0", FCVAR_REPLICATED);

ConVar	sk_plr_dmg_smg1			( "sk_plr_dmg_smg1","0", FCVAR_REPLICATED );
ConVar	sk_npc_dmg_smg1			( "sk_npc_dmg_smg1","0", FCVAR_REPLICATED);
ConVar	sk_max_smg1				( "sk_max_smg1","0", FCVAR_REPLICATED);


ConVar	sk_plr_dmg_flare_round	( "sk_plr_dmg_flare_round","0", FCVAR_REPLICATED);
ConVar	sk_npc_dmg_flare_round	( "sk_npc_dmg_flare_round","0", FCVAR_REPLICATED);
ConVar	sk_max_flare_round		( "sk_max_flare_round","0", FCVAR_REPLICATED);

ConVar	sk_plr_dmg_buckshot		( "sk_plr_dmg_buckshot","0", FCVAR_REPLICATED);	
ConVar	sk_npc_dmg_buckshot		( "sk_npc_dmg_buckshot","0", FCVAR_REPLICATED);
ConVar	sk_max_buckshot			( "sk_max_buckshot","0", FCVAR_REPLICATED);
ConVar	sk_plr_num_shotgun_pellets( "sk_plr_num_shotgun_pellets","7", FCVAR_REPLICATED);

ConVar	sk_plr_dmg_rpg_round	( "sk_plr_dmg_rpg_round","0", FCVAR_REPLICATED);
ConVar	sk_npc_dmg_rpg_round	( "sk_npc_dmg_rpg_round","0", FCVAR_REPLICATED);
ConVar	sk_max_rpg_round		( "sk_max_rpg_round","0", FCVAR_REPLICATED);

ConVar	sk_plr_dmg_sniper_round	( "sk_plr_dmg_sniper_round","0", FCVAR_REPLICATED);	
ConVar	sk_npc_dmg_sniper_round	( "sk_npc_dmg_sniper_round","0", FCVAR_REPLICATED);
ConVar	sk_max_sniper_round		( "sk_max_sniper_round","0", FCVAR_REPLICATED);

//ConVar	sk_max_slam				( "sk_max_slam","0", FCVAR_REPLICATED);
//ConVar	sk_max_tripwire			( "sk_max_tripwire","0", FCVAR_REPLICATED);

//ConVar	sk_plr_dmg_molotov		( "sk_plr_dmg_molotov","0", FCVAR_REPLICATED);
//ConVar	sk_npc_dmg_molotov		( "sk_npc_dmg_molotov","0", FCVAR_REPLICATED);
//ConVar	sk_max_molotov			( "sk_max_molotov","0", FCVAR_REPLICATED);

ConVar	sk_plr_dmg_grenade		( "sk_plr_dmg_grenade","0", FCVAR_REPLICATED);
ConVar	sk_npc_dmg_grenade		( "sk_npc_dmg_grenade","0", FCVAR_REPLICATED);
ConVar	sk_max_grenade			( "sk_max_grenade","0", FCVAR_REPLICATED);

#ifdef HL2_EPISODIC
ConVar	sk_max_hopwire			( "sk_max_hopwire", "3", FCVAR_REPLICATED);
ConVar	sk_max_striderbuster	( "sk_max_striderbuster", "3", FCVAR_REPLICATED);
#endif

//ConVar sk_plr_dmg_brickbat	( "sk_plr_dmg_brickbat","0", FCVAR_REPLICATED);
//ConVar sk_npc_dmg_brickbat	( "sk_npc_dmg_brickbat","0", FCVAR_REPLICATED);
//ConVar sk_max_brickbat		( "sk_max_brickbat","0", FCVAR_REPLICATED);

ConVar	sk_plr_dmg_smg1_grenade	( "sk_plr_dmg_smg1_grenade","0", FCVAR_REPLICATED);
ConVar	sk_npc_dmg_smg1_grenade	( "sk_npc_dmg_smg1_grenade","0", FCVAR_REPLICATED);
ConVar	sk_max_smg1_grenade		( "sk_max_smg1_grenade","0", FCVAR_REPLICATED );

ConVar	sk_plr_dmg_357			( "sk_plr_dmg_357", "0", FCVAR_REPLICATED );
ConVar	sk_npc_dmg_357			( "sk_npc_dmg_357", "0", FCVAR_REPLICATED );
ConVar	sk_max_357				( "sk_max_357", "0", FCVAR_REPLICATED );

ConVar	sk_plr_dmg_crossbow		( "sk_plr_dmg_crossbow", "0", FCVAR_REPLICATED );
ConVar	sk_npc_dmg_crossbow		( "sk_npc_dmg_crossbow", "0", FCVAR_REPLICATED );
ConVar	sk_max_crossbow			( "sk_max_crossbow", "0", FCVAR_REPLICATED );

ConVar	sk_dmg_sniper_penetrate_plr( "sk_dmg_sniper_penetrate_plr","0", FCVAR_REPLICATED);
ConVar	sk_dmg_sniper_penetrate_npc( "sk_dmg_sniper_penetrate_npc","0", FCVAR_REPLICATED);

ConVar	sk_plr_dmg_sniper("sk_plr_dmg_sniper", "0", FCVAR_REPLICATED);
ConVar	sk_npc_dmg_sniper("sk_npc_dmg_sniper", "0", FCVAR_REPLICATED);
ConVar	sk_max_sniper("sk_max_sniper", "0", FCVAR_REPLICATED);

ConVar	sk_plr_dmg_deagle("sk_plr_dmg_deagle", "0", FCVAR_REPLICATED);
ConVar	sk_npc_dmg_deagle("sk_npc_dmg_deagle", "0", FCVAR_REPLICATED);
ConVar	sk_max_deagle("sk_max_deagle", "0", FCVAR_REPLICATED);

ConVar	sk_plr_dmg_m249para("sk_plr_dmg_m249para", "0", FCVAR_REPLICATED);
ConVar	sk_npc_dmg_m249para("sk_npc_dmg_m249para", "0", FCVAR_REPLICATED);
ConVar	sk_max_m249para("sk_max_m249para", "0", FCVAR_REPLICATED);

ConVar	sk_plr_dmg_slam("sk_plr_dmg_slam", "0", FCVAR_REPLICATED);
ConVar	sk_npc_dmg_slam("sk_npc_dmg_slam", "0", FCVAR_REPLICATED);
ConVar	sk_max_slam("sk_max_slam", "0", FCVAR_REPLICATED);

ConVar	sk_plr_dmg_oicw("sk_plr_dmg_oicw", "0", FCVAR_REPLICATED);
ConVar	sk_npc_dmg_oicw("sk_npc_dmg_oicw", "0", FCVAR_REPLICATED);
ConVar	sk_max_oicw("sk_max_oicw", "0", FCVAR_REPLICATED);

ConVar	sk_plr_dmg_oicw_grenade("sk_plr_dmg_oicw_grenade", "0", FCVAR_REPLICATED);
ConVar	sk_npc_dmg_oicw_grenade("sk_npc_dmg_oicw_grenade", "0", FCVAR_REPLICATED);
ConVar	sk_max_oicw_grenade("sk_max_oicw_grenade", "0", FCVAR_REPLICATED);

//Custom Ammo Types
ConVar	sk_plr_dmg_custom_normal("sk_plr_dmg_custom_normal", "0", FCVAR_REPLICATED);
ConVar	sk_npc_dmg_custom_normal("sk_npc_dmg_custom_normal", "0", FCVAR_REPLICATED);
ConVar	sk_max_custom_normal("sk_max_custom_normal", "0", FCVAR_REPLICATED);

ConVar	sk_plr_dmg_custom_buckshot("sk_plr_dmg_custom_buckshot", "0", FCVAR_REPLICATED);
ConVar	sk_npc_dmg_custom_buckshot("sk_npc_dmg_custom_buckshot", "0", FCVAR_REPLICATED);
ConVar	sk_max_custom_buckshot("sk_max_custom_buckshot", "0", FCVAR_REPLICATED);

ConVar	sk_plr_dmg_custom_sniper("sk_plr_dmg_custom_sniper", "0", FCVAR_REPLICATED);
ConVar	sk_npc_dmg_custom_sniper("sk_npc_dmg_custom_sniper", "0", FCVAR_REPLICATED);
ConVar	sk_max_custom_sniper("sk_max_custom_sniper", "0", FCVAR_REPLICATED);

ConVar	sk_plr_dmg_custom_buckshotsniper("sk_plr_dmg_custom_buckshotsniper", "0", FCVAR_REPLICATED);
ConVar	sk_npc_dmg_custom_buckshotsniper("sk_npc_dmg_custom_buckshotsniper", "0", FCVAR_REPLICATED);
ConVar	sk_max_custom_buckshotsniper("sk_max_custom_buckshotsniper", "0", FCVAR_REPLICATED);

ConVar	sk_plr_dmg_airboat		( "sk_plr_dmg_airboat", "0", FCVAR_REPLICATED );
ConVar	sk_npc_dmg_airboat		( "sk_npc_dmg_airboat", "0", FCVAR_REPLICATED );

ConVar	sk_max_gauss_round		( "sk_max_gauss_round", "0", FCVAR_REPLICATED );

ConVar	sk_max_egon_round		("sk_max_egon_round", "0", FCVAR_REPLICATED);

ConVar	sk_plr_dmg_mp5("sk_plr_dmg_mp5", "0", FCVAR_REPLICATED);
ConVar	sk_npc_dmg_mp5("sk_npc_dmg_mp5", "0", FCVAR_REPLICATED);
ConVar	sk_max_mp5("sk_max_mp5", "0", FCVAR_REPLICATED);

ConVar    sk_plr_dmg_katana("sk_plr_dmg_katana", "0");
ConVar    sk_npc_dmg_katana("sk_npc_dmg_katana", "0");

// Gunship & Dropship cannons
ConVar	sk_npc_dmg_gunship			( "sk_npc_dmg_gunship", "0", FCVAR_REPLICATED );
ConVar	sk_npc_dmg_gunship_to_plr	( "sk_npc_dmg_gunship_to_plr", "0", FCVAR_REPLICATED );

extern ConVar mp_chattime;
extern ConVar mp_mapcycle_empty_timeout_seconds;
extern ConVar mp_timelimit;

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : iDmgType - m_flInitialSpawnerTime
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
int CHalfLife2::Damage_GetTimeBased( void )
{
#ifdef HL2_EPISODIC
	int iDamage = ( DMG_PARALYZE | DMG_NERVEGAS | DMG_POISON | DMG_RADIATION | DMG_DROWNRECOVER | DMG_ACID | DMG_SLOWBURN );
	return iDamage;
#else
	return BaseClass::Damage_GetTimeBased();
#endif
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : iDmgType - 
// Output :		bool
//-----------------------------------------------------------------------------
bool CHalfLife2::Damage_IsTimeBased( int iDmgType )
{
	// Damage types that are time-based.
#ifdef HL2_EPISODIC
	// This makes me think EP2 should have its own rules, but they are #ifdef all over in here.
	return ( ( iDmgType & ( DMG_PARALYZE | DMG_NERVEGAS | DMG_POISON | DMG_RADIATION | DMG_DROWNRECOVER | DMG_SLOWBURN ) ) != 0 );
#else
	return BaseClass::Damage_IsTimeBased( iDmgType );
#endif
}

float CHalfLife2::GetMapRemainingTime()
{
	// if timelimit is disabled, return 0
	if (mp_timelimit.GetInt() <= 0)
		return -1;

	// timelimit is in minutes

	float timeleft = (mp_timelimit.GetInt() * 60.0f) - gpGlobals->curtime;

	if (timeleft <= 0)
		return 0;

	return timeleft;
}

#ifdef CLIENT_DLL
#else

#ifdef HL2_EPISODIC
ConVar  alyx_darkness_force( "alyx_darkness_force", "0", FCVAR_CHEAT | FCVAR_REPLICATED );
#endif // HL2_EPISODIC

#endif // CLIENT_DLL


#ifdef CLIENT_DLL //{


#else //}{

	extern bool		g_fGameOver;

#if !(defined( HL2MP ) || defined( PORTAL_MP ))
	class CVoiceGameMgrHelper : public IVoiceGameMgrHelper
	{
	public:
		virtual bool		CanPlayerHearPlayer( CBasePlayer *pListener, CBasePlayer *pTalker, bool &bProximity )
		{
			return true;
		}
	};
	CVoiceGameMgrHelper g_VoiceGameMgrHelper;
	IVoiceGameMgrHelper *g_pVoiceGameMgrHelper = &g_VoiceGameMgrHelper;
#endif
	
	//-----------------------------------------------------------------------------
	// Purpose:
	// Input  :
	// Output :
	//-----------------------------------------------------------------------------
	CHalfLife2::CHalfLife2()
	{
		m_bMegaPhysgun = false;
		m_flIntermissionEndTime = 0.0f;
		m_tmNextPeriodicThink = 0;
		
		m_flLastHealthDropTime = 0.0f;
		m_flLastGrenadeDropTime = 0.0f;

		if (IsMultiplayer())
		{
			if (engine->IsDedicatedServer())
			{
				// dedicated server
				const char *cfgfile = servercfgfile.GetString();

				if (cfgfile && cfgfile[0])
				{
					char szCommand[256];

					Log("Executing dedicated server config file %s\n", cfgfile);
					Q_snprintf(szCommand, sizeof(szCommand), "exec %s\n", cfgfile);
					engine->ServerCommand(szCommand);
				}
			}
			else
			{
				// listen server
				const char *cfgfile = lservercfgfile.GetString();

				if (cfgfile && cfgfile[0])
				{
					char szCommand[256];

					Log("Executing listen server config file %s\n", cfgfile);
					Q_snprintf(szCommand, sizeof(szCommand), "exec %s\n", cfgfile);
					engine->ServerCommand(szCommand);
				}
			}
		}

		nextlevel.SetValue("");
		LoadMapCycleFile();
	}

	//-----------------------------------------------------------------------------
	// Purpose: called each time a player uses a "cmd" command
	// Input  : *pEdict - the player who issued the command
	//			Use engine.Cmd_Argv,  engine.Cmd_Argv, and engine.Cmd_Argc to get 
	//			pointers the character string command.
	//-----------------------------------------------------------------------------
	bool CHalfLife2::ClientCommand( CBaseEntity *pEdict, const CCommand &args )
	{
		if( BaseClass::ClientCommand( pEdict, args ) )
			return true;

		CHL2_Player *pPlayer = (CHL2_Player *) pEdict;

		if ( pPlayer->ClientCommand( args ) )
			return true;

		return false;
	}

	//-----------------------------------------------------------------------------
	// Purpose: Player has just spawned. Equip them.
	//-----------------------------------------------------------------------------
	void CHalfLife2::PlayerSpawn( CBasePlayer *pPlayer )
	{
	}

	//-----------------------------------------------------------------------------
	// Purpose: MULTIPLAYER BODY QUE HANDLING
	//-----------------------------------------------------------------------------
	class CCorpse : public CBaseAnimating
	{
	public:
		DECLARE_CLASS( CCorpse, CBaseAnimating );
		DECLARE_SERVERCLASS();

		virtual int ObjectCaps( void ) { return FCAP_DONT_SAVE; }	

	public:
		CNetworkVar( int, m_nReferencePlayer );
	};

	IMPLEMENT_SERVERCLASS_ST(CCorpse, DT_Corpse)
		SendPropInt( SENDINFO(m_nReferencePlayer), 10, SPROP_UNSIGNED )
	END_SEND_TABLE()

	LINK_ENTITY_TO_CLASS( bodyque, CCorpse );


	CCorpse		*g_pBodyQueueHead;

	void InitBodyQue(void)
	{
		CCorpse *pEntity = ( CCorpse * )CreateEntityByName( "bodyque" );
		pEntity->AddEFlags( EFL_KEEP_ON_RECREATE_ENTITIES );
		g_pBodyQueueHead = pEntity;
		CCorpse *p = g_pBodyQueueHead;
		
		// Reserve 3 more slots for dead bodies
		for ( int i = 0; i < 3; i++ )
		{
			CCorpse *next = ( CCorpse * )CreateEntityByName( "bodyque" );
			next->AddEFlags( EFL_KEEP_ON_RECREATE_ENTITIES );
			p->SetOwnerEntity( next );
			p = next;
		}
		
		p->SetOwnerEntity( g_pBodyQueueHead );
	}

	//-----------------------------------------------------------------------------
	// Purpose: make a body que entry for the given ent so the ent can be respawned elsewhere
	// GLOBALS ASSUMED SET:  g_eoBodyQueueHead
	//-----------------------------------------------------------------------------
	void CopyToBodyQue( CBaseAnimating *pCorpse ) 
	{
		if ( pCorpse->IsEffectActive( EF_NODRAW ) )
			return;

		CCorpse *pHead	= g_pBodyQueueHead;

		pHead->CopyAnimationDataFrom( pCorpse );

		pHead->SetMoveType( MOVETYPE_FLYGRAVITY );
		pHead->SetAbsVelocity( pCorpse->GetAbsVelocity() );
		pHead->ClearFlags();
		pHead->m_nReferencePlayer	= ENTINDEX( pCorpse );

		pHead->SetLocalAngles( pCorpse->GetAbsAngles() );
		UTIL_SetOrigin(pHead, pCorpse->GetAbsOrigin());

		UTIL_SetSize(pHead, pCorpse->WorldAlignMins(), pCorpse->WorldAlignMaxs());
		g_pBodyQueueHead = (CCorpse *)pHead->GetOwnerEntity();
	}

	//------------------------------------------------------------------------------
	// Purpose : Initialize all default class relationships
	// Input   :
	// Output  :
	//------------------------------------------------------------------------------
	void CHalfLife2::InitDefaultAIRelationships( void )
	{
		int i, j;

		//  Allocate memory for default relationships
		CBaseCombatCharacter::AllocateDefaultRelationships();

		// --------------------------------------------------------------
		// First initialize table so we can report missing relationships
		// --------------------------------------------------------------
		for (i=0;i<NUM_AI_CLASSES;i++)
		{
			for (j=0;j<NUM_AI_CLASSES;j++)
			{
				// By default all relationships are neutral of priority zero
				CBaseCombatCharacter::SetDefaultRelationship( (Class_T)i, (Class_T)j, D_NU, 0 );
			}
		}

		if (!firefightrumble_enemyattack.GetBool())
		{
			for (i = 0; i<NUM_AI_CLASSES; i++)
			{
				for (j = 0; j<NUM_AI_CLASSES; j++)
				{
					if (i == CLASS_NONE || i == CLASS_BULLSEYE || i == CLASS_FLARE)
					{
						CBaseCombatCharacter::SetDefaultRelationship((Class_T)i, (Class_T)j, D_NU, 0);
					}
					else if (j == CLASS_NONE || j == CLASS_BULLSEYE || j == CLASS_FLARE)
					{
						CBaseCombatCharacter::SetDefaultRelationship((Class_T)i, (Class_T)j, D_NU, 0);
					}
					else
					{
						CBaseCombatCharacter::SetDefaultRelationship((Class_T)i, (Class_T)j, D_LI, 0);
					}
				}
			}
			//add important relationships
			//enemies hate the player except CLASS_COMBINE_CREMATOR
			CBaseCombatCharacter::SetDefaultRelationship(CLASS_ANTLION, CLASS_PLAYER, D_HT, 0);
			CBaseCombatCharacter::SetDefaultRelationship(CLASS_BARNACLE, CLASS_PLAYER, D_HT, 0);
			CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE, CLASS_PLAYER, D_HT, 0);
			CBaseCombatCharacter::SetDefaultRelationship(CLASS_BULLSQUID, CLASS_PLAYER, D_HT, 0);
			CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE_GUNSHIP, CLASS_PLAYER, D_HT, 0);
			CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE_HUNTER, CLASS_PLAYER, D_HT, 0);
			CBaseCombatCharacter::SetDefaultRelationship(CLASS_HEADCRAB, CLASS_PLAYER, D_HT, 0);
			CBaseCombatCharacter::SetDefaultRelationship(CLASS_HOUNDEYE, CLASS_PLAYER, D_HT, 0);
			CBaseCombatCharacter::SetDefaultRelationship(CLASS_MANHACK, CLASS_PLAYER, D_HT, 0);
			CBaseCombatCharacter::SetDefaultRelationship(CLASS_METROPOLICE, CLASS_PLAYER, D_HT, 0);
			CBaseCombatCharacter::SetDefaultRelationship(CLASS_ALIEN_MILITARY, CLASS_PLAYER, D_HT, 0);
			CBaseCombatCharacter::SetDefaultRelationship(CLASS_SCANNER, CLASS_PLAYER, D_HT, 0);
			CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE_CREMATOR, CLASS_PLAYER, D_NU, 0);
			CBaseCombatCharacter::SetDefaultRelationship(CLASS_ZOMBIE, CLASS_PLAYER, D_HT, 0);
			CBaseCombatCharacter::SetDefaultRelationship(CLASS_CITIZEN_PASSIVE, CLASS_PLAYER, D_HT, 0);
			CBaseCombatCharacter::SetDefaultRelationship(CLASS_CITIZEN_REBEL, CLASS_PLAYER, D_HT, 0);
			//fixed the ALL HAIL CREMATOR bug.
			CBaseCombatCharacter::SetDefaultRelationship(CLASS_PLAYER, CLASS_ANTLION, D_HT, 0);
			CBaseCombatCharacter::SetDefaultRelationship(CLASS_PLAYER, CLASS_BARNACLE, D_HT, 0);
			CBaseCombatCharacter::SetDefaultRelationship(CLASS_PLAYER, CLASS_COMBINE, D_HT, 0);
			CBaseCombatCharacter::SetDefaultRelationship(CLASS_PLAYER, CLASS_BULLSQUID, D_HT, 0);
			CBaseCombatCharacter::SetDefaultRelationship(CLASS_PLAYER, CLASS_COMBINE_GUNSHIP, D_HT, 0);
			CBaseCombatCharacter::SetDefaultRelationship(CLASS_PLAYER, CLASS_COMBINE_HUNTER, D_HT, 0);
			CBaseCombatCharacter::SetDefaultRelationship(CLASS_PLAYER, CLASS_HEADCRAB, D_HT, 0);
			CBaseCombatCharacter::SetDefaultRelationship(CLASS_PLAYER, CLASS_HOUNDEYE, D_HT, 0);
			CBaseCombatCharacter::SetDefaultRelationship(CLASS_PLAYER, CLASS_MANHACK, D_HT, 0);
			CBaseCombatCharacter::SetDefaultRelationship(CLASS_PLAYER, CLASS_METROPOLICE, D_HT, 0);
			CBaseCombatCharacter::SetDefaultRelationship(CLASS_PLAYER, CLASS_ALIEN_MILITARY, D_HT, 0);
			CBaseCombatCharacter::SetDefaultRelationship(CLASS_PLAYER, CLASS_SCANNER, D_HT, 0);
			CBaseCombatCharacter::SetDefaultRelationship(CLASS_PLAYER, CLASS_ZOMBIE, D_HT, 0);
			CBaseCombatCharacter::SetDefaultRelationship(CLASS_PLAYER, CLASS_CITIZEN_PASSIVE, D_HT, 0);
			CBaseCombatCharacter::SetDefaultRelationship(CLASS_PLAYER, CLASS_CITIZEN_REBEL, D_HT, 0);
			//compile the code and remove this comment. get alien controller working.
		}
		else
		{
			for (i = 0; i<NUM_AI_CLASSES; i++)
			{
				for (j = 0; j<NUM_AI_CLASSES; j++)
				{
					if (i == j)
					{
						CBaseCombatCharacter::SetDefaultRelationship((Class_T)i, (Class_T)j, D_LI, 0);
					}
					else if (i == CLASS_NONE || i == CLASS_BULLSEYE || i == CLASS_FLARE)
					{
						CBaseCombatCharacter::SetDefaultRelationship((Class_T)i, (Class_T)j, D_NU, 0);
					}
					else if (j == CLASS_NONE || j == CLASS_BULLSEYE || j == CLASS_FLARE)
					{
						CBaseCombatCharacter::SetDefaultRelationship((Class_T)i, (Class_T)j, D_NU, 0);
					}
					else
					{
						CBaseCombatCharacter::SetDefaultRelationship((Class_T)i, (Class_T)j, D_HT, 0);
					}
				}
			}
			//add important relationships
			//combine enemies on the same faction like each other.
			CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE, CLASS_COMBINE_GUNSHIP, D_LI, 0);
			CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE, CLASS_COMBINE_HUNTER, D_LI, 0);
			CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE, CLASS_MANHACK, D_LI, 0);
			CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE, CLASS_METROPOLICE, D_LI, 0);
			CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE, CLASS_SCANNER, D_LI, 0);
			CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE, CLASS_COMBINE_CREMATOR, D_LI, 0);
			CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE_GUNSHIP, CLASS_COMBINE, D_LI, 0);
			CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE_GUNSHIP, CLASS_COMBINE_HUNTER, D_LI, 0);
			CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE_GUNSHIP, CLASS_MANHACK, D_LI, 0);
			CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE_GUNSHIP, CLASS_METROPOLICE, D_LI, 0);
			CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE_GUNSHIP, CLASS_SCANNER, D_LI, 0);
			CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE_GUNSHIP, CLASS_COMBINE_CREMATOR, D_LI, 0);
			CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE_HUNTER, CLASS_COMBINE, D_LI, 0);
			CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE_HUNTER, CLASS_COMBINE_GUNSHIP, D_LI, 0);
			CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE_HUNTER, CLASS_MANHACK, D_LI, 0);
			CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE_HUNTER, CLASS_METROPOLICE, D_LI, 0);
			CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE_HUNTER, CLASS_SCANNER, D_LI, 0);
			CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE_HUNTER, CLASS_COMBINE_CREMATOR, D_LI, 0);
			CBaseCombatCharacter::SetDefaultRelationship(CLASS_MANHACK, CLASS_COMBINE, D_LI, 0);
			CBaseCombatCharacter::SetDefaultRelationship(CLASS_MANHACK, CLASS_COMBINE_GUNSHIP, D_LI, 0);
			CBaseCombatCharacter::SetDefaultRelationship(CLASS_MANHACK, CLASS_COMBINE_HUNTER, D_LI, 0);
			CBaseCombatCharacter::SetDefaultRelationship(CLASS_MANHACK, CLASS_METROPOLICE, D_LI, 0);
			CBaseCombatCharacter::SetDefaultRelationship(CLASS_MANHACK, CLASS_SCANNER, D_LI, 0);
			CBaseCombatCharacter::SetDefaultRelationship(CLASS_MANHACK, CLASS_COMBINE_CREMATOR, D_LI, 0);
			CBaseCombatCharacter::SetDefaultRelationship(CLASS_METROPOLICE, CLASS_COMBINE, D_LI, 0);
			CBaseCombatCharacter::SetDefaultRelationship(CLASS_METROPOLICE, CLASS_COMBINE_GUNSHIP, D_LI, 0);
			CBaseCombatCharacter::SetDefaultRelationship(CLASS_METROPOLICE, CLASS_COMBINE_HUNTER, D_LI, 0);
			CBaseCombatCharacter::SetDefaultRelationship(CLASS_METROPOLICE, CLASS_MANHACK, D_LI, 0);
			CBaseCombatCharacter::SetDefaultRelationship(CLASS_METROPOLICE, CLASS_SCANNER, D_LI, 0);
			CBaseCombatCharacter::SetDefaultRelationship(CLASS_METROPOLICE, CLASS_COMBINE_CREMATOR, D_LI, 0);
			CBaseCombatCharacter::SetDefaultRelationship(CLASS_SCANNER, CLASS_COMBINE, D_LI, 0);
			CBaseCombatCharacter::SetDefaultRelationship(CLASS_SCANNER, CLASS_COMBINE_GUNSHIP, D_LI, 0);
			CBaseCombatCharacter::SetDefaultRelationship(CLASS_SCANNER, CLASS_COMBINE_HUNTER, D_LI, 0);
			CBaseCombatCharacter::SetDefaultRelationship(CLASS_SCANNER, CLASS_MANHACK, D_LI, 0);
			CBaseCombatCharacter::SetDefaultRelationship(CLASS_SCANNER, CLASS_COMBINE_CREMATOR, D_LI, 0);
			CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE_CREMATOR, CLASS_COMBINE, D_LI, 0);
			CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE_CREMATOR, CLASS_COMBINE_GUNSHIP, D_LI, 0);
			CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE_CREMATOR, CLASS_COMBINE_HUNTER, D_LI, 0);
			CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE_CREMATOR, CLASS_MANHACK, D_LI, 0);
			CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE_CREMATOR, CLASS_SCANNER, D_LI, 0);
			//zombies shouldn't attack headcrabs.
			CBaseCombatCharacter::SetDefaultRelationship(CLASS_HEADCRAB, CLASS_ZOMBIE, D_LI, 0);
			CBaseCombatCharacter::SetDefaultRelationship(CLASS_ZOMBIE, CLASS_HEADCRAB, D_LI, 0);
			CBaseCombatCharacter::SetDefaultRelationship(CLASS_CITIZEN_PASSIVE, CLASS_CITIZEN_REBEL, D_LI, 0);
			CBaseCombatCharacter::SetDefaultRelationship(CLASS_CITIZEN_REBEL, CLASS_CITIZEN_PASSIVE, D_LI, 0);
		}
	}


	//------------------------------------------------------------------------------
	// Purpose : Return classify text for classify type
	// Input   :
	// Output  :
	//------------------------------------------------------------------------------
	const char* CHalfLife2::AIClassText(int classType)
	{
		switch (classType)
		{
			case CLASS_NONE:			return "CLASS_NONE";
			case CLASS_PLAYER:			return "CLASS_PLAYER";
			case CLASS_ALIEN_MILITARY:	return "CLASS_ALIEN_MILITARY";
			case CLASS_ANTLION:			return "CLASS_ANTLION";
			case CLASS_BARNACLE:		return "CLASS_BARNACLE";
			case CLASS_BULLSEYE:		return "CLASS_BULLSEYE";
			case CLASS_BULLSQUID:		return "CLASS_BULLSQUID";	
			case CLASS_CITIZEN_PASSIVE: return "CLASS_CITIZEN_PASSIVE";		
			case CLASS_CITIZEN_REBEL:	return "CLASS_CITIZEN_REBEL";
			case CLASS_COMBINE:			return "CLASS_COMBINE";
			case CLASS_COMBINE_CREMATOR: return "CLASS_COMBINE_CREMATOR";
			case CLASS_COMBINE_GUNSHIP:	return "CLASS_COMBINE_GUNSHIP";
			case CLASS_COMBINE_HUNTER:	return "CLASS_COMBINE_HUNTER";
			case CLASS_CONSCRIPT:		return "CLASS_CONSCRIPT";
			case CLASS_HEADCRAB:		return "CLASS_HEADCRAB";
			case CLASS_HOUNDEYE:		return "CLASS_HOUNDEYE";
			case CLASS_MANHACK:			return "CLASS_MANHACK";
			case CLASS_METROPOLICE:		return "CLASS_METROPOLICE";
			case CLASS_MILITARY:		return "CLASS_MILITARY";	
			case CLASS_SCANNER:			return "CLASS_SCANNER";		
			case CLASS_STALKER:			return "CLASS_STALKER";		
			case CLASS_VORTIGAUNT:		return "CLASS_VORTIGAUNT";
			case CLASS_ZOMBIE:			return "CLASS_ZOMBIE";
			case CLASS_PROTOSNIPER:		return "CLASS_PROTOSNIPER";
			case CLASS_MISSILE:			return "CLASS_MISSILE";
			case CLASS_FLARE:			return "CLASS_FLARE";
			case CLASS_EARTH_FAUNA:		return "CLASS_EARTH_FAUNA";

			default:					return "MISSING CLASS in ClassifyText()";
		}
	}

	void CHalfLife2::PlayerThink( CBasePlayer *pPlayer )
	{
	}

	void CHalfLife2::Think( void )
	{
		BaseClass::Think();

		if( physcannon_mega_enabled.GetBool() == true )
		{
			m_bMegaPhysgun = true;
		}

		if (IsMultiplayer())
		{
			if (GetMapRemainingTime() == 0)
			{
				GoToIntermission();
			}

			if (g_fGameOver)   // someone else quit the game already
			{
				// check to see if we should change levels now
				if (m_flIntermissionEndTime < gpGlobals->curtime)
				{
					ChangeLevel();
				}

				return;
			}
		}
	}

	int CHalfLife2::GetNumberOfPlayers(void)
	{
		int num = 0;
		
		for (int i = 1; i <= gpGlobals->maxClients; i++)
		{
			CHL2_Player *pPlayer = (CHL2_Player*)UTIL_PlayerByIndex(i);

			if (!pPlayer || !pPlayer->IsConnected())
				continue;

			num++;
		}

		return num;
	}

	bool CHalfLife2::IsIntermission(void)
	{
		return m_flIntermissionEndTime > gpGlobals->curtime;
	}

	void CHalfLife2::PlayerKilled(CBasePlayer *pVictim, const CTakeDamageInfo &info)
	{
		if (IsIntermission() && IsMultiplayer())
			return;

		BaseClass::PlayerKilled(pVictim, info);
	}

	void CHalfLife2::GoToIntermission(void)
	{
		if (g_fGameOver)
			return;

		g_fGameOver = true;

		m_flIntermissionEndTime = gpGlobals->curtime + mp_chattime.GetInt();

		for (int i = 0; i < MAX_PLAYERS; i++)
		{
			CBasePlayer *pPlayer = UTIL_PlayerByIndex(i);

			if (!pPlayer)
				continue;

			//no scoreboard yet
			pPlayer->ShowViewPortPanel(PANEL_SCOREBOARD);
			pPlayer->AddFlag(FL_FROZEN);
			pPlayer->AddFlag(FL_NOTARGET);
		}
	}

	void StripCharMP(char *szBuffer, const char cWhiteSpace)
	{
		while (char *pSpace = strchr(szBuffer, cWhiteSpace))
		{
			char *pNextChar = pSpace + sizeof(char);
			V_strcpy(pSpace, pNextChar);
		}
	}

	void CHalfLife2::GetNextLevelName(char *pszNextMap, int bufsize, bool bRandom /* = false */)
	{
		char mapcfile[256];
		DetermineMapCycleFilename(mapcfile, sizeof(mapcfile), false);

		// Check the time of the mapcycle file and re-populate the list of level names if the file has been modified
		const int nMapCycleTimeStamp = filesystem->GetPathTime(mapcfile, "GAME");

		if (0 == nMapCycleTimeStamp)
		{
			// Map cycle file does not exist, make a list containing only the current map
			char *szCurrentMapName = new char[MAX_MAP_NAME];
			Q_strncpy(szCurrentMapName, STRING(gpGlobals->mapname), MAX_MAP_NAME);
			m_MapList.AddToTail(szCurrentMapName);
		}
		else
		{
			// If map cycle file has changed or this is the first time through ...
			if (m_nMapCycleTimeStamp != nMapCycleTimeStamp)
			{
				// Reset map index and map cycle timestamp
				m_nMapCycleTimeStamp = nMapCycleTimeStamp;
				m_nMapCycleindex = 0;

				LoadMapCycleFile();
			}
		}

		// If somehow we have no maps in the list then add the current one
		if (0 == m_MapList.Count())
		{
			char *szDefaultMapName = new char[MAX_MAP_NAME];
			Q_strncpy(szDefaultMapName, STRING(gpGlobals->mapname), MAX_MAP_NAME);
			m_MapList.AddToTail(szDefaultMapName);
		}

		if (bRandom)
		{
			m_nMapCycleindex = RandomInt(0, m_MapList.Count() - 1);
		}

		// Here's the return value
		Q_strncpy(pszNextMap, m_MapList[m_nMapCycleindex], bufsize);
	}

	void CHalfLife2::DetermineMapCycleFilename(char *pszResult, int nSizeResult, bool bForceSpew)
	{
		static char szLastResult[256];

		const char *pszVar = mapcyclefile.GetString();
		if (*pszVar == '\0')
		{
			if (bForceSpew || V_stricmp(szLastResult, "__novar"))
			{
				Msg("mapcyclefile convar not set.\n");
				V_strcpy_safe(szLastResult, "__novar");
			}
			*pszResult = '\0';
			return;
		}

		char szRecommendedName[256];
		V_sprintf_safe(szRecommendedName, "cfg/%s", pszVar);

		// First, look for a mapcycle file in the cfg directory, which is preferred
		V_strncpy(pszResult, szRecommendedName, nSizeResult);
		if (filesystem->FileExists(pszResult, "GAME"))
		{
			if (bForceSpew || V_stricmp(szLastResult, pszResult))
			{
				Msg("Using map cycle file '%s'.\n", pszResult);
				V_strcpy_safe(szLastResult, pszResult);
			}
			return;
		}

		// Nope?  Try the root.
		V_strncpy(pszResult, pszVar, nSizeResult);
		if (filesystem->FileExists(pszResult, "GAME"))
		{
			if (bForceSpew || V_stricmp(szLastResult, pszResult))
			{
				Msg("Using map cycle file '%s'.  ('%s' was not found.)\n", pszResult, szRecommendedName);
				V_strcpy_safe(szLastResult, pszResult);
			}
			return;
		}

		// Nope?  Use the default.
		if (!V_stricmp(pszVar, "mapcycle.txt"))
		{
			V_strncpy(pszResult, "cfg/mapcycle_default.txt", nSizeResult);
			if (filesystem->FileExists(pszResult, "GAME"))
			{
				if (bForceSpew || V_stricmp(szLastResult, pszResult))
				{
					Msg("Using map cycle file '%s'.  ('%s' was not found.)\n", pszResult, szRecommendedName);
					V_strcpy_safe(szLastResult, pszResult);
				}
				return;
			}
		}

		// Failed
		*pszResult = '\0';
		if (bForceSpew || V_stricmp(szLastResult, "__notfound"))
		{
			Msg("Map cycle file '%s' was not found.\n", szRecommendedName);
			V_strcpy_safe(szLastResult, "__notfound");
		}
	}

	void CHalfLife2::LoapMapCycleFileIntoVector(const char *pszMapCycleFile, CUtlVector<char *> &mapList)
	{
		CUtlBuffer buf;
		if (!filesystem->ReadFile(pszMapCycleFile, "GAME", buf))
			return;
		buf.PutChar(0);
		V_SplitString((char*)buf.Base(), "\n", mapList);

		for (int i = 0; i < mapList.Count(); i++)
		{
			bool bIgnore = false;

			// Strip out the spaces in the name
			StripCharMP(mapList[i], '\r');
			StripCharMP(mapList[i], ' ');

			if (!Q_strncmp(mapList[i], "//", 2) || mapList[i][0] == '\0')
			{
				bIgnore = true;
			}
			else if (!engine->IsMapValid(mapList[i]))
			{
				bIgnore = true;

				// If the engine doesn't consider it a valid map remove it from the lists
				Warning("Invalid map '%s' included in map cycle file. Ignored.\n", mapList[i]);
			}

			if (bIgnore)
			{
				delete[] mapList[i];
				mapList.Remove(i);
				--i;
			}
		}
	}

	void CHalfLife2::FreeMapCycleFileVector(CUtlVector<char *> &mapList)
	{
		// Clear out existing map list. Not using Purge() or PurgeAndDeleteAll() because they won't delete [] each element.
		for (int i = 0; i < mapList.Count(); i++)
		{
			delete[] mapList[i];
		}

		mapList.RemoveAll();
	}

	bool CHalfLife2::IsMapInMapCycle(const char *pszName)
	{
		for (int i = 0; i < m_MapList.Count(); i++)
		{
			if (V_stricmp(pszName, m_MapList[i]) == 0)
			{
				return true;
			}
		}

		return false;
	}

	void CHalfLife2::ChangeLevel(void)
	{
		char szNextMap[MAX_MAP_NAME];

		if (nextlevel.GetString() && *nextlevel.GetString() && engine->IsMapValid(nextlevel.GetString()))
		{
			Q_strncpy(szNextMap, nextlevel.GetString(), sizeof(szNextMap));
		}
		else
		{
			GetNextLevelName(szNextMap, sizeof(szNextMap));
			IncrementMapCycleIndex();
		}

		ChangeLevelToMap(szNextMap);
	}

	void CHalfLife2::LoadMapCycleFile(void)
	{
		char mapcfile[256];
		DetermineMapCycleFilename(mapcfile, sizeof(mapcfile), false);

		FreeMapCycleFileVector(m_MapList);

		// Repopulate map list from mapcycle file
		LoapMapCycleFileIntoVector(mapcfile, m_MapList);

		// Load server's mapcycle into network string table for client-side voting
		if (g_pStringTableServerMapCycle)
		{
			CUtlString sFileList;
			for (int i = 0; i < m_MapList.Count(); i++)
			{
				sFileList += m_MapList[i];
				sFileList += '\n';
			}

			g_pStringTableServerMapCycle->AddString(CBaseEntity::IsServer(), "ServerMapCycle", sFileList.Length() + 1, sFileList.String());
		}

		// If the current map selection is in the list, set m_nMapCycleindex to the map that follows it.
		for (int i = 0; i < m_MapList.Count(); i++)
		{
			if (V_strcmp(STRING(gpGlobals->mapname), m_MapList[i]) == 0)
			{
				m_nMapCycleindex = i;
				IncrementMapCycleIndex();
				break;
			}
		}
	}

	void CHalfLife2::ChangeLevelToMap(const char *pszMap)
	{
		g_fGameOver = true;
		m_flTimeLastMapChangeOrPlayerWasConnected = 0.0f;
		Msg("CHANGE LEVEL: %s\n", pszMap);
		engine->ChangeLevel(pszMap, NULL);
	}

	void CHalfLife2::SkipNextMapInCycle()
	{
		char szSkippedMap[MAX_MAP_NAME];
		char szNextMap[MAX_MAP_NAME];

		GetNextLevelName(szSkippedMap, sizeof(szSkippedMap));
		IncrementMapCycleIndex();
		GetNextLevelName(szNextMap, sizeof(szNextMap));

		Msg("Skipping: %s\tNext map: %s\n", szSkippedMap, szNextMap);

		if (nextlevel.GetString() && *nextlevel.GetString() && engine->IsMapValid(nextlevel.GetString()))
		{
			Msg("Warning! \"nextlevel\" is set to \"%s\" and will override the next map to be played.\n", nextlevel.GetString());
		}
	}

	void CHalfLife2::IncrementMapCycleIndex()
	{
		// Reset index if we've passed the end of the map list
		if (++m_nMapCycleindex >= m_MapList.Count())
		{
			m_nMapCycleindex = 0;
		}
	}

	//=========================================================
	//=========================================================
	void CHalfLife2::FrameUpdatePostEntityThink()
	{
		BaseClass::FrameUpdatePostEntityThink();

		float flNow = Plat_FloatTime();

		// Update time when client was last connected
		if (m_flTimeLastMapChangeOrPlayerWasConnected <= 0.0f)
		{
			m_flTimeLastMapChangeOrPlayerWasConnected = flNow;
		}
		else
		{
			for (int iPlayerIndex = 1; iPlayerIndex <= MAX_PLAYERS; iPlayerIndex++)
			{
				player_info_t pi;
				if (!engine->GetPlayerInfo(iPlayerIndex, &pi))
					continue;
#if defined( REPLAY_ENABLED )
				if (pi.ishltv || pi.isreplay || pi.fakeplayer)
#else
				if (pi.ishltv || pi.fakeplayer)
#endif
					continue;

				m_flTimeLastMapChangeOrPlayerWasConnected = flNow;
				break;
			}
		}

		// Check if we should cycle the map because we've been empty
		// for long enough
		if (mp_mapcycle_empty_timeout_seconds.GetInt() > 0)
		{
			int iIdleSeconds = (int)(flNow - m_flTimeLastMapChangeOrPlayerWasConnected);
			if (iIdleSeconds >= mp_mapcycle_empty_timeout_seconds.GetInt())
			{
				Log("Server has been empty for %d seconds on this map, cycling map as per mp_mapcycle_empty_timeout_seconds\n", iIdleSeconds);
				ChangeLevel();
			}
		}
	}

	//-----------------------------------------------------------------------------
	// Purpose: Returns how much damage the given ammo type should do to the victim
	//			when fired by the attacker.
	// Input  : pAttacker - Dude what shot the gun.
	//			pVictim - Dude what done got shot.
	//			nAmmoType - What been shot out.
	// Output : How much hurt to put on dude what done got shot (pVictim).
	//-----------------------------------------------------------------------------
	float CHalfLife2::GetAmmoDamage( CBaseEntity *pAttacker, CBaseEntity *pVictim, int nAmmoType )
	{
		float flDamage = 0.0f;
		CAmmoDef *pAmmoDef = GetAmmoDef();

		if ( pAmmoDef->DamageType( nAmmoType ) & DMG_SNIPER )
		{
			// If this damage is from a SNIPER, we do damage based on what the bullet
			// HITS, not who fired it. All other bullets have their damage values
			// arranged according to the owner of the bullet, not the recipient.
			if ( pVictim->IsPlayer() )
			{
				// Player
				flDamage = pAmmoDef->PlrDamage( nAmmoType );
			}
			else
			{
				// NPC or breakable
				flDamage = pAmmoDef->NPCDamage( nAmmoType );
			}
		}
		else
		{
			flDamage = BaseClass::GetAmmoDamage( pAttacker, pVictim, nAmmoType );
		}

		if( pAttacker->IsPlayer() && pVictim->IsNPC() )
		{
			if( pVictim->MyCombatCharacterPointer() )
			{
				// Player is shooting an NPC. Adjust the damage! This protects breakables
				// and other 'non-living' entities from being easier/harder to break
				// in different skill levels.
				flDamage = pAmmoDef->PlrDamage( nAmmoType );
				flDamage = AdjustPlayerDamageInflicted( flDamage );
			}
		}

		return flDamage;
	}

   	//-----------------------------------------------------------------------------
  	//-----------------------------------------------------------------------------
 	bool CHalfLife2::AllowDamage( CBaseEntity *pVictim, const CTakeDamageInfo &info )
  	{
#ifndef CLIENT_DLL
	if( (info.GetDamageType() & DMG_CRUSH) && info.GetInflictor() && pVictim->MyNPCPointer() )
	{
		if( pVictim->MyNPCPointer()->IsPlayerAlly() )
		{
			// A physics object has struck a player ally. Don't allow damage if it
			// came from the player's physcannon. 
			for (int i = 1; i <= gpGlobals->maxClients; i++)//
			{
				CBasePlayer *pPlayer = UTIL_PlayerByIndex(i);
				if (!pPlayer)//AI Patch Removal
					continue;

				CBaseEntity *pWeapon = pPlayer->HasNamedPlayerItem("weapon_physcannon");

				if( pWeapon )
				{
					CBaseCombatWeapon *pCannon = assert_cast <CBaseCombatWeapon*>(pWeapon);

					if( pCannon )
					{
						if( PhysCannonAccountableForObject(pCannon, info.GetInflictor() ) )
						{
							// Antlions can always be squashed!
							if ( pVictim->Classify() == CLASS_ANTLION )
								return true;

  							return false;
						}
					}
				}
			}
		}
	}
#endif
  		return true;
  	}
	//-----------------------------------------------------------------------------
	// Purpose: Whether or not the NPC should drop a health vial
	// Output : Returns true on success, false on failure.
	//-----------------------------------------------------------------------------
	bool CHalfLife2::NPC_ShouldDropHealth( CBasePlayer *pRecipient )
	{
		// Can only do this every so often
		if ( m_flLastHealthDropTime > gpGlobals->curtime )
			return false;

		//Try to throw dynamic health
		float healthPerc = ( (float) pRecipient->m_iHealth / (float) pRecipient->m_iMaxHealth );

		if ( random->RandomFloat( 0.0f, 1.0f ) > healthPerc*1.5f )
			return true;

		return false;
	}

	//-----------------------------------------------------------------------------
	// Purpose: Whether or not the NPC should drop a health vial
	// Output : Returns true on success, false on failure.
	//-----------------------------------------------------------------------------
	bool CHalfLife2::NPC_ShouldDropGrenade( CBasePlayer *pRecipient )
	{
		// Can only do this every so often
		if ( m_flLastGrenadeDropTime > gpGlobals->curtime )
			return false;
		
		int grenadeIndex = GetAmmoDef()->Index( "grenade" );
		int numGrenades = pRecipient->GetAmmoCount( grenadeIndex );

		// If we're not maxed out on grenades and we've randomly okay'd it
		if ( ( numGrenades < GetAmmoDef()->MaxCarry( grenadeIndex ) ) && ( random->RandomInt( 0, 2 ) == 0 ) )
			return true;

		return false;
	}

	//-----------------------------------------------------------------------------
	// Purpose: Update the drop counter for health
	//-----------------------------------------------------------------------------
	void CHalfLife2::NPC_DroppedHealth( void )
	{
		m_flLastHealthDropTime = gpGlobals->curtime + sk_plr_health_drop_time.GetFloat();
	}

	//-----------------------------------------------------------------------------
	// Purpose: Update the drop counter for grenades
	//-----------------------------------------------------------------------------
	void CHalfLife2::NPC_DroppedGrenade( void )
	{
		m_flLastGrenadeDropTime = gpGlobals->curtime + sk_plr_grenade_drop_time.GetFloat();
	}

	void CHalfLife2::SetMegaPhyscannonActive(void)
	{
		m_bMegaPhysgun = true;
	}

	void CHalfLife2::SetMegaPhyscannonInActive(void)
	{
		m_bMegaPhysgun = false;
	}

	const char *CHalfLife2::GetGameDescription(void)
	{
		return GetGamemodeName_ServerBrowser(bHasRandomized);
	}

#endif //} !CLIENT_DLL

// ------------------------------------------------------------------------------------ //
// Shared CHalfLife2 implementation.
// ------------------------------------------------------------------------------------ //
bool CHalfLife2::ShouldCollide( int collisionGroup0, int collisionGroup1 )
{
	// The smaller number is always first
	if ( collisionGroup0 > collisionGroup1 )
	{
		// swap so that lowest is always first
		int tmp = collisionGroup0;
		collisionGroup0 = collisionGroup1;
		collisionGroup1 = tmp;
	}
	
	// Prevent the player movement from colliding with spit globs (caused the player to jump on top of globs while in water)
	if ( collisionGroup0 == COLLISION_GROUP_PLAYER_MOVEMENT && collisionGroup1 == HL2COLLISION_GROUP_SPIT )
		return false;

	// HL2 treats movement and tracing against players the same, so just remap here
	if ( collisionGroup0 == COLLISION_GROUP_PLAYER_MOVEMENT )
	{
		collisionGroup0 = COLLISION_GROUP_PLAYER;
	}

	if( collisionGroup1 == COLLISION_GROUP_PLAYER_MOVEMENT )
	{
		collisionGroup1 = COLLISION_GROUP_PLAYER;
	}

	//If collisionGroup0 is not a player then NPC_ACTOR behaves just like an NPC.
	if ( collisionGroup1 == COLLISION_GROUP_NPC_ACTOR && collisionGroup0 != COLLISION_GROUP_PLAYER )
	{
		collisionGroup1 = COLLISION_GROUP_NPC;
	}

	// This is only for the super physcannon
	if ( m_bMegaPhysgun )
	{
		if ( collisionGroup0 == COLLISION_GROUP_INTERACTIVE_DEBRIS && collisionGroup1 == COLLISION_GROUP_PLAYER )
			return false;
	}

	if ( collisionGroup0 == HL2COLLISION_GROUP_COMBINE_BALL )
	{
		if ( collisionGroup1 == HL2COLLISION_GROUP_COMBINE_BALL )
			return false;
	}

	if ( collisionGroup0 == HL2COLLISION_GROUP_COMBINE_BALL && collisionGroup1 == HL2COLLISION_GROUP_COMBINE_BALL_NPC )
		return false;

	if ( ( collisionGroup0 == COLLISION_GROUP_WEAPON ) ||
		( collisionGroup0 == COLLISION_GROUP_PLAYER ) ||
		( collisionGroup0 == COLLISION_GROUP_PROJECTILE ) )
	{
		if ( collisionGroup1 == HL2COLLISION_GROUP_COMBINE_BALL )
			return false;
	}

	if ( collisionGroup0 == COLLISION_GROUP_DEBRIS )
	{
		if ( collisionGroup1 == HL2COLLISION_GROUP_COMBINE_BALL )
			return true;
	}

	if (collisionGroup0 == HL2COLLISION_GROUP_HOUNDEYE && collisionGroup1 == HL2COLLISION_GROUP_HOUNDEYE )
		return false;

	if (collisionGroup0 == HL2COLLISION_GROUP_HOMING_MISSILE && collisionGroup1 == HL2COLLISION_GROUP_HOMING_MISSILE )
		return false;

	if ( collisionGroup1 == HL2COLLISION_GROUP_CROW )
	{
		if ( collisionGroup0 == COLLISION_GROUP_PLAYER || collisionGroup0 == COLLISION_GROUP_NPC ||
			 collisionGroup0 == HL2COLLISION_GROUP_CROW )
			return false;
	}

	if ( ( collisionGroup0 == HL2COLLISION_GROUP_HEADCRAB ) && ( collisionGroup1 == HL2COLLISION_GROUP_HEADCRAB ) )
		return false;

	// striders don't collide with other striders
	if ( collisionGroup0 == HL2COLLISION_GROUP_STRIDER && collisionGroup1 == HL2COLLISION_GROUP_STRIDER )
		return false;

	// gunships don't collide with other gunships
	if ( collisionGroup0 == HL2COLLISION_GROUP_GUNSHIP && collisionGroup1 == HL2COLLISION_GROUP_GUNSHIP )
		return false;

	// weapons and NPCs don't collide
	if ( collisionGroup0 == COLLISION_GROUP_WEAPON && (collisionGroup1 >= HL2COLLISION_GROUP_FIRST_NPC && collisionGroup1 <= HL2COLLISION_GROUP_LAST_NPC ) )
		return false;

	//players don't collide against NPC Actors.
	//I could've done this up where I check if collisionGroup0 is NOT a player but I decided to just
	//do what the other checks are doing in this function for consistency sake.
	if ( collisionGroup1 == COLLISION_GROUP_NPC_ACTOR && collisionGroup0 == COLLISION_GROUP_PLAYER )
		return false;
		
	// In cases where NPCs are playing a script which causes them to interpenetrate while riding on another entity,
	// such as a train or elevator, you need to disable collisions between the actors so the mover can move them.
	if ( collisionGroup0 == COLLISION_GROUP_NPC_SCRIPTED && collisionGroup1 == COLLISION_GROUP_NPC_SCRIPTED )
		return false;

	// Spit doesn't touch other spit
	if ( collisionGroup0 == HL2COLLISION_GROUP_SPIT && collisionGroup1 == HL2COLLISION_GROUP_SPIT )
		return false;

	return BaseClass::ShouldCollide( collisionGroup0, collisionGroup1 ); 
}

#ifndef CLIENT_DLL
//---------------------------------------------------------
//---------------------------------------------------------
void CHalfLife2::AdjustPlayerDamageTaken( CTakeDamageInfo *pInfo )
{
	if( pInfo->GetDamageType() & (DMG_DROWN|DMG_CRUSH|DMG_FALL|DMG_POISON|DMG_SNIPER) )
	{
		// Skill level doesn't affect these types of damage.
		return;
	}

	switch( GetSkillLevel() )
	{
	case SKILL_EASY:
		pInfo->ScaleDamage( sk_dmg_take_scale1.GetFloat() );
		break;

	case SKILL_MEDIUM:
		pInfo->ScaleDamage( sk_dmg_take_scale2.GetFloat() );
		break;

	case SKILL_HARD:
		pInfo->ScaleDamage( sk_dmg_take_scale3.GetFloat() );
		break;

	case SKILL_VERYHARD:
		pInfo->ScaleDamage(sk_dmg_take_scale4.GetFloat());
		break;

	case SKILL_NIGHTMARE:
		pInfo->ScaleDamage(sk_dmg_take_scale5.GetFloat());
		break;
	}
}

//---------------------------------------------------------
//---------------------------------------------------------
float CHalfLife2::AdjustPlayerDamageInflicted( float damage )
{
	switch( GetSkillLevel() ) 
	{
	case SKILL_EASY:
		return damage * sk_dmg_inflict_scale1.GetFloat();
		break;

	case SKILL_MEDIUM:
		return damage * sk_dmg_inflict_scale2.GetFloat();
		break;

	case SKILL_HARD:
		return damage * sk_dmg_inflict_scale3.GetFloat();
		break;

	case SKILL_VERYHARD:
		return damage * sk_dmg_inflict_scale4.GetFloat();
		break;

	case SKILL_NIGHTMARE:
		return damage * sk_dmg_inflict_scale5.GetFloat();
		break;

	default:
		return damage;
		break;
	}
}

//---------------------------------------------------------
//---------------------------------------------------------
void CHalfLife2::AdjustPlayerDamageTakenCombineAce(CTakeDamageInfo *pInfo)
{
	if (pInfo->GetDamageType() & (DMG_DROWN | DMG_CRUSH | DMG_FALL | DMG_POISON | DMG_SNIPER))
	{
		// Skill level doesn't affect these types of damage.
		return;
	}

	switch (GetSkillLevel())
	{
	case SKILL_EASY:
		pInfo->ScaleDamage(sk_dmg_take_scale1_ace.GetFloat());
		break;

	case SKILL_MEDIUM:
		pInfo->ScaleDamage(sk_dmg_take_scale2_ace.GetFloat());
		break;

	case SKILL_HARD:
		pInfo->ScaleDamage(sk_dmg_take_scale3_ace.GetFloat());
		break;

	case SKILL_VERYHARD:
		pInfo->ScaleDamage(sk_dmg_take_scale4_ace.GetFloat());
		break;

	case SKILL_NIGHTMARE:
		pInfo->ScaleDamage(sk_dmg_take_scale5_ace.GetFloat());
		break;
	}
}
#endif//CLIENT_DLL

//---------------------------------------------------------
//---------------------------------------------------------
bool CHalfLife2::ShouldUseRobustRadiusDamage(CBaseEntity *pEntity)
{
#ifdef CLIENT_DLL
	return false;
#endif

	if( !sv_robust_explosions.GetBool() )
		return false;

	if( !pEntity->IsNPC() )
	{
		// Only NPC's
		return false;
	}

#ifndef CLIENT_DLL
	CAI_BaseNPC *pNPC = pEntity->MyNPCPointer();
	if( pNPC->CapabilitiesGet() & bits_CAP_SIMPLE_RADIUS_DAMAGE )
	{
		// This NPC only eligible for simple radius damage.
		return false;
	}
#endif//CLIENT_DLL

	return true;
}

#ifndef CLIENT_DLL
//---------------------------------------------------------
//---------------------------------------------------------
bool CHalfLife2::ShouldAutoAim( CBasePlayer *pPlayer, edict_t *target )
{
	return sk_allow_autoaim.GetBool() != 0;
}

//---------------------------------------------------------
//---------------------------------------------------------
float CHalfLife2::GetAutoAimScale( CBasePlayer *pPlayer )
{
#ifdef _X360
	return 1.0f;
#else
	switch( GetSkillLevel() )
	{
	case SKILL_EASY:
		return sk_autoaim_scale1.GetFloat();

	case SKILL_MEDIUM:
		return sk_autoaim_scale2.GetFloat();

	default:
		return 0.0f;
	}
#endif
}

//---------------------------------------------------------
//---------------------------------------------------------
float CHalfLife2::GetAmmoQuantityScale( int iAmmoIndex )
{
	switch( GetSkillLevel() )
	{
	case SKILL_EASY:
		return sk_ammo_qty_scale1.GetFloat();

	case SKILL_MEDIUM:
		return sk_ammo_qty_scale2.GetFloat();

	case SKILL_HARD:
		return sk_ammo_qty_scale3.GetFloat();

	case SKILL_VERYHARD:
		return sk_ammo_qty_scale4.GetFloat();

	case SKILL_NIGHTMARE:
		return sk_ammo_qty_scale5.GetFloat();

	default:
		return 0.0f;
	}
}

void CHalfLife2::LevelInitPreEntity()
{
	// Remove this if you fix the bug in ep1 where the striders need to touch
	// triggers using their absbox instead of their bbox
#ifdef HL2_EPISODIC
	if ( !Q_strnicmp( gpGlobals->mapname.ToCStr(), "ep1_", 4 ) )
	{
		// episode 1 maps use the surrounding box trigger behavior
		CBaseEntity::sm_bAccurateTriggerBboxChecks = false;
	}
#endif

	BaseClass::LevelInitPreEntity();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
const char *CHalfLife2::GetChatFormat(bool bTeamOnly, CBasePlayer *pPlayer)
{
	if (!pPlayer)  // dedicated server output
	{
		return "FR_Chat_DS";
	}

	CHL2_Player *pHL2MPPlayer = ToHL2Player(pPlayer);
	const char *pszFormat = NULL;

	// team only
	if (pHL2MPPlayer->m_bIsPlayerADev)
	{
		if (pHL2MPPlayer->IsDead())
		{
			pszFormat = "FR_Chat_DevDead";
		}
		else
		{
			pszFormat = "FR_Chat_Dev";
		}
	}
	else if (pHL2MPPlayer->m_bIsPlayerAVIP)
	{
		if (pHL2MPPlayer->IsDead())
		{
			pszFormat = "FR_Chat_VIPDead";
		}
		else
		{
			pszFormat = "FR_Chat_VIP";
		}
	}
	else
	{
		if (pHL2MPPlayer->IsDead())
		{
			pszFormat = "FR_Chat_AllDead";
		}
		else
		{
			pszFormat = "FR_Chat_All";
		}
	}

	return pszFormat;
}

//-----------------------------------------------------------------------------
// Returns whether or not Alyx cares about light levels in order to see.
//-----------------------------------------------------------------------------
bool CHalfLife2::IsAlyxInDarknessMode()
{
#ifdef HL2_EPISODIC
	if ( alyx_darkness_force.GetBool() )
		return true;

	return ( GlobalEntity_GetState( "ep_alyx_darknessmode" ) == GLOBAL_ON );
#else
	return false;
#endif // HL2_EPISODIC
}


//-----------------------------------------------------------------------------
// This takes the long way around to see if a prop should emit a DLIGHT when it
// ignites, to avoid having Alyx-related code in props.cpp.
//-----------------------------------------------------------------------------
bool CHalfLife2::ShouldBurningPropsEmitLight()
{
#ifdef HL2_EPISODIC
	return IsAlyxInDarknessMode();
#else
	return false;
#endif // HL2_EPISODIC
}


#endif//CLIENT_DLL

// ------------------------------------------------------------------------------------ //
// Global functions.
// ------------------------------------------------------------------------------------ //

#ifndef HL2MP
#ifndef PORTAL

// shared ammo definition
// JAY: Trying to make a more physical bullet response
#define BULLET_MASS_GRAINS_TO_LB(grains)	(0.002285*(grains)/16.0f)
#define BULLET_MASS_GRAINS_TO_KG(grains)	lbs2kg(BULLET_MASS_GRAINS_TO_LB(grains))

// exaggerate all of the forces, but use real numbers to keep them consistent
#define BULLET_IMPULSE_EXAGGERATION			3.5
// convert a velocity in ft/sec and a mass in grains to an impulse in kg in/s
#define BULLET_IMPULSE(grains, ftpersec)	((ftpersec)*12*BULLET_MASS_GRAINS_TO_KG(grains)*BULLET_IMPULSE_EXAGGERATION)


CAmmoDef *GetAmmoDef()
{
	static CAmmoDef def;
	static bool bInitted = false;
	
	if ( !bInitted )
	{
		bInitted = true;

		def.AddAmmoType("AR2",				DMG_BULLET,					TRACER_LINE_AND_WHIZ,	"sk_plr_dmg_ar2",			"sk_npc_dmg_ar2",			"sk_max_ar2",			BULLET_IMPULSE(200, 1225), 0 );
		def.AddAmmoType("AlyxGun",			DMG_BULLET,					TRACER_LINE,			"sk_plr_dmg_alyxgun",		"sk_npc_dmg_alyxgun",		"sk_max_alyxgun",		BULLET_IMPULSE(200, 1225), 0 );
		def.AddAmmoType("Pistol",			DMG_BULLET,					TRACER_LINE_AND_WHIZ,	"sk_plr_dmg_pistol",		"sk_npc_dmg_pistol",		"sk_max_pistol",		BULLET_IMPULSE(200, 1225), 0 );
		def.AddAmmoType("SMG1",				DMG_BULLET,					TRACER_LINE_AND_WHIZ,	"sk_plr_dmg_smg1",			"sk_npc_dmg_smg1",			"sk_max_smg1",			BULLET_IMPULSE(200, 1225), 0 );
		def.AddAmmoType("357",				DMG_BULLET | DMG_SNIPER, TRACER_LINE_AND_WHIZ, "sk_plr_dmg_357", "sk_npc_dmg_357", "sk_max_357", BULLET_IMPULSE(800, 5000), 0);
		def.AddAmmoType("XBowBolt",			DMG_BULLET,					TRACER_LINE,			"sk_plr_dmg_crossbow",		"sk_npc_dmg_crossbow",		"sk_max_crossbow",		BULLET_IMPULSE(800, 8000), 0 );

		def.AddAmmoType("Buckshot",			DMG_BULLET | DMG_BUCKSHOT | DMG_ALWAYSGIB | DMG_SNIPER, TRACER_LINE, "sk_plr_dmg_buckshot", "sk_npc_dmg_buckshot", "sk_max_buckshot", BULLET_IMPULSE(400, 1200), 0);
		def.AddAmmoType("RPG_Round",		DMG_BURN,					TRACER_NONE,			"sk_plr_dmg_rpg_round",		"sk_npc_dmg_rpg_round",		"sk_max_rpg_round",		0, 0 );
		def.AddAmmoType("SMG1_Grenade",		DMG_BURN | DMG_ALWAYSGIB, TRACER_NONE, "sk_plr_dmg_smg1_grenade", "sk_npc_dmg_smg1_grenade", "sk_max_smg1_grenade", 0, 0);
		def.AddAmmoType("SniperRound",		DMG_BULLET | DMG_SNIPER,	TRACER_NONE,			"sk_plr_dmg_sniper_round",	"sk_npc_dmg_sniper_round",	"sk_max_sniper_round",	BULLET_IMPULSE(650, 6000), 0 );
		def.AddAmmoType("SniperPenetratedRound", DMG_BULLET | DMG_SNIPER, TRACER_NONE,			"sk_dmg_sniper_penetrate_plr", "sk_dmg_sniper_penetrate_npc", "sk_max_sniper_round", BULLET_IMPULSE(150, 6000), 0 );
		def.AddAmmoType("Grenade",			DMG_BURN | DMG_ALWAYSGIB, TRACER_NONE, "sk_plr_dmg_grenade", "sk_npc_dmg_grenade", "sk_max_grenade", 0, 0);
		def.AddAmmoType("Thumper",			DMG_SONIC,					TRACER_NONE,			10, 10, 2, 0, 0 );
		def.AddAmmoType("Gravity",			DMG_CLUB,					TRACER_NONE,			0,	0, 8, 0, 0 );
//		def.AddAmmoType("Extinguisher",		DMG_BURN,					TRACER_NONE,			0,	0, 100, 0, 0 );
		def.AddAmmoType("Battery",			DMG_CLUB,					TRACER_NONE,			NULL, NULL, NULL, 0, 0 );
		def.AddAmmoType("GaussEnergy",		DMG_SHOCK | DMG_ALWAYSGIB, TRACER_NONE, "sk_plr_dmg_gauss", "sk_plr_dmg_gauss", "sk_max_gauss_round", BULLET_IMPULSE(650, 8000), 0); // hit like a 10kg weight at 400 in/s
		def.AddAmmoType("CombineCannon",	DMG_BULLET,					TRACER_LINE,			"sk_npc_dmg_gunship_to_plr", "sk_npc_dmg_gunship", NULL, 1.5 * 750 * 12, 0 ); // hit like a 1.5kg weight at 750 ft/s
		def.AddAmmoType("AirboatGun",		DMG_AIRBOAT,				TRACER_LINE,			"sk_plr_dmg_airboat",		"sk_npc_dmg_airboat",		NULL,					BULLET_IMPULSE(10, 600), 0 );

		//=====================================================================
		// STRIDER MINIGUN DAMAGE - Pull up a chair and I'll tell you a tale.
		//
		// When we shipped Half-Life 2 in 2004, we were unaware of a bug in
		// CAmmoDef::NPCDamage() which was returning the MaxCarry field of
		// an ammotype as the amount of damage that should be done to a NPC
		// by that type of ammo. Thankfully, the bug only affected Ammo Types 
		// that DO NOT use ConVars to specify their parameters. As you can see,
		// all of the important ammotypes use ConVars, so the effect of the bug
		// was limited. The Strider Minigun was affected, though.
		//
		// According to my perforce Archeology, we intended to ship the Strider
		// Minigun ammo type to do 15 points of damage per shot, and we did. 
		// To achieve this we, unaware of the bug, set the Strider Minigun ammo 
		// type to have a maxcarry of 15, since our observation was that the 
		// number that was there before (8) was indeed the amount of damage being
		// done to NPC's at the time. So we changed the field that was incorrectly
		// being used as the NPC Damage field.
		//
		// The bug was fixed during Episode 1's development. The result of the 
		// bug fix was that the Strider was reduced to doing 5 points of damage
		// to NPC's, since 5 is the value that was being assigned as NPC damage
		// even though the code was returning 15 up to that point.
		//
		// Now as we go to ship Orange Box, we discover that the Striders in 
		// Half-Life 2 are hugely ineffective against citizens, causing big
		// problems in maps 12 and 13. 
		//
		// In order to restore balance to HL2 without upsetting the delicate 
		// balance of ep2_outland_12, I have chosen to build Episodic binaries
		// with 5 as the Strider->NPC damage, since that's the value that has
		// been in place for all of Episode 2's development. Half-Life 2 will
		// build with 15 as the Strider->NPC damage, which is how HL2 shipped
		// originally, only this time the 15 is located in the correct field
		// now that the AmmoDef code is behaving correctly.
		//
		//=====================================================================
#ifdef HL2_EPISODIC
		def.AddAmmoType("StriderMinigun",	DMG_BULLET,					TRACER_LINE,			5, 5, 15, 1.0 * 750 * 12, AMMO_FORCE_DROP_IF_CARRIED ); // hit like a 1.0kg weight at 750 ft/s
#else
		def.AddAmmoType("StriderMinigun",	DMG_BULLET,					TRACER_LINE,			5, 15,15, 1.0 * 750 * 12, AMMO_FORCE_DROP_IF_CARRIED ); // hit like a 1.0kg weight at 750 ft/s
#endif//HL2_EPISODIC

		def.AddAmmoType("StriderMinigunDirect",	DMG_BULLET,				TRACER_LINE,			2, 2, 15, 1.0 * 750 * 12, AMMO_FORCE_DROP_IF_CARRIED ); // hit like a 1.0kg weight at 750 ft/s
		def.AddAmmoType("HelicopterGun",	DMG_BULLET,					TRACER_LINE_AND_WHIZ,	"sk_npc_dmg_helicopter_to_plr", "sk_npc_dmg_helicopter",	"sk_max_smg1",	BULLET_IMPULSE(400, 1225), AMMO_FORCE_DROP_IF_CARRIED | AMMO_INTERPRET_PLRDAMAGE_AS_DAMAGE_TO_PLAYER );
		def.AddAmmoType("AR2AltFire",		DMG_DISSOLVE,				TRACER_NONE,			0, 0, "sk_max_ar2_altfire", 0, 0 );
		def.AddAmmoType("Grenade",			DMG_BURN | DMG_ALWAYSGIB, TRACER_NONE, "sk_plr_dmg_grenade", "sk_npc_dmg_grenade", "sk_max_grenade", 0, 0);
		def.AddAmmoType("Sniper",			DMG_BULLET | DMG_SNIPER, TRACER_LINE_AND_WHIZ, "sk_plr_dmg_sniper", "sk_npc_dmg_sniper", "sk_max_sniper", BULLET_IMPULSE(800, 5000), 0);
		def.AddAmmoType("Deagle",			DMG_BULLET | DMG_SNIPER, TRACER_LINE_AND_WHIZ, "sk_plr_dmg_deagle", "sk_npc_dmg_deagle", "sk_max_deagle", BULLET_IMPULSE(800, 5000), 0);
		def.AddAmmoType("M249",				DMG_BULLET | DMG_SNIPER, TRACER_LINE_AND_WHIZ, "sk_plr_dmg_m249para", "sk_npc_dmg_m249para", "sk_max_m249para", BULLET_IMPULSE(200, 1225), 0);
		def.AddAmmoType("slam",				DMG_BURN | DMG_ALWAYSGIB, TRACER_NONE, "sk_plr_dmg_slam", "sk_npc_dmg_slam", "sk_max_slam", 0, 0);
		def.AddAmmoType("OICW",				DMG_BULLET, TRACER_LINE_AND_WHIZ, "sk_plr_dmg_oicw", "sk_npc_dmg_oicw", "sk_max_oicw", BULLET_IMPULSE(200, 1225), 0);
		def.AddAmmoType("OICW_Grenade",		DMG_BURN | DMG_ALWAYSGIB, TRACER_NONE, "sk_plr_dmg_oicw_grenade", "sk_npc_dmg_oicw_grenade", "sk_max_oicw_grenade", 0, 0);
		def.AddAmmoType("FlareRound",		DMG_BURN, TRACER_LINE, "sk_plr_dmg_flare_round", "sk_npc_dmg_flare_round", "sk_max_flare_round", BULLET_IMPULSE(1500, 600), 0);
		def.AddAmmoType("EgonEnergy",		DMG_SHOCK | DMG_ALWAYSGIB, TRACER_NONE, "sk_plr_dmg_egon_narrow", "sk_plr_dmg_egon_narrow", "sk_max_egon_round", BULLET_IMPULSE(650, 8000), 0);
		def.AddAmmoType("MP5Ammo", DMG_BULLET, TRACER_LINE_AND_WHIZ, "sk_plr_dmg_mp5", "sk_npc_dmg_mp5", "sk_max_mp5", BULLET_IMPULSE(200, 1225), 0);
		def.AddAmmoType("Katana", DMG_SLASH, TRACER_NONE, "sk_plr_dmg_katana", "sk_npc_dmg_katana", 1, BULLET_IMPULSE(200, 1225), 0);
		//CUSTOM AMMO TYPES HERE.
		def.AddAmmoType("CustomBullet1_Normal", DMG_BULLET, TRACER_LINE_AND_WHIZ, "sk_plr_dmg_custom_normal", "sk_npc_dmg_custom_normal", "sk_max_custom_normal", BULLET_IMPULSE(800, 5000), 0);
		def.AddAmmoType("CustomBullet2_NormalBurn", DMG_BULLET | DMG_BURN, TRACER_LINE_AND_WHIZ, "sk_plr_dmg_custom_normal", "sk_npc_dmg_custom_normal", "sk_max_custom_normal", BULLET_IMPULSE(800, 5000), 0);
		def.AddAmmoType("CustomBullet3_NormalBurnGib", DMG_BULLET | DMG_BURN | DMG_ALWAYSGIB, TRACER_LINE_AND_WHIZ, "sk_plr_dmg_custom_normal", "sk_npc_dmg_custom_normal", "sk_max_custom_normal", BULLET_IMPULSE(800, 5000), 0);
		def.AddAmmoType("CustomBullet4_NormalAlwaysGib", DMG_BULLET | DMG_ALWAYSGIB, TRACER_LINE_AND_WHIZ, "sk_plr_dmg_custom_normal", "sk_npc_dmg_custom_normal", "sk_max_custom_normal", BULLET_IMPULSE(800, 5000), 0);
		def.AddAmmoType("CustomBullet5_NormalDissolve", DMG_BULLET | DMG_DISSOLVE, TRACER_LINE_AND_WHIZ, "sk_plr_dmg_custom_normal", "sk_npc_dmg_custom_normal", "sk_max_custom_normal", BULLET_IMPULSE(800, 5000), 0);
		def.AddAmmoType("CustomBullet6_NormalDissolveGib", DMG_BULLET | DMG_DISSOLVE | DMG_ALWAYSGIB, TRACER_LINE_AND_WHIZ, "sk_plr_dmg_custom_normal", "sk_npc_dmg_custom_normal", "sk_max_custom_normal", BULLET_IMPULSE(800, 5000), 0);

		def.AddAmmoType("CustomBullet7_Buckshot", DMG_BULLET | DMG_BUCKSHOT, TRACER_LINE_AND_WHIZ, "sk_plr_dmg_custom_buckshot", "sk_npc_dmg_custom_buckshot", "sk_max_custom_buckshot", BULLET_IMPULSE(800, 5000), 0);
		def.AddAmmoType("CustomBullet8_BuckshotBurn", DMG_BULLET | DMG_BUCKSHOT | DMG_BURN, TRACER_LINE_AND_WHIZ, "sk_plr_dmg_custom_buckshot", "sk_npc_dmg_custom_buckshot", "sk_max_custom_buckshot", BULLET_IMPULSE(800, 5000), 0);
		def.AddAmmoType("CustomBullet9_BuckshotBurnGib", DMG_BULLET | DMG_BUCKSHOT | DMG_BURN | DMG_ALWAYSGIB, TRACER_LINE_AND_WHIZ, "sk_plr_dmg_custom_buckshot", "sk_npc_dmg_custom_buckshot", "sk_max_custom_buckshot", BULLET_IMPULSE(800, 5000), 0);
		def.AddAmmoType("CustomBullet10_BuckshotAlwaysGib", DMG_BULLET | DMG_BUCKSHOT | DMG_ALWAYSGIB, TRACER_LINE_AND_WHIZ, "sk_plr_dmg_custom_buckshot", "sk_npc_dmg_custom_buckshot", "sk_max_custom_buckshot", BULLET_IMPULSE(800, 5000), 0);
		def.AddAmmoType("CustomBullet11_BuckshotDissolve", DMG_BULLET | DMG_BUCKSHOT | DMG_DISSOLVE, TRACER_LINE_AND_WHIZ, "sk_plr_dmg_custom_buckshot", "sk_npc_dmg_custom_buckshot", "sk_max_custom_buckshot", BULLET_IMPULSE(800, 5000), 0);
		def.AddAmmoType("CustomBullet12_BuckshotDissolveGib", DMG_BULLET | DMG_BUCKSHOT | DMG_DISSOLVE | DMG_ALWAYSGIB, TRACER_LINE_AND_WHIZ, "sk_plr_dmg_custom_buckshot", "sk_npc_dmg_custom_buckshot", "sk_max_custom_buckshot", BULLET_IMPULSE(800, 5000), 0);

		def.AddAmmoType("CustomBullet13_Sniper", DMG_BULLET | DMG_SNIPER, TRACER_LINE_AND_WHIZ, "sk_plr_dmg_custom_sniper", "sk_npc_dmg_custom_sniper", "sk_max_custom_sniper", BULLET_IMPULSE(800, 5000), 0);
		def.AddAmmoType("CustomBullet14_SniperBurn", DMG_BULLET | DMG_SNIPER | DMG_BURN, TRACER_LINE_AND_WHIZ, "sk_plr_dmg_custom_sniper", "sk_npc_dmg_custom_sniper", "sk_max_custom_sniper", BULLET_IMPULSE(800, 5000), 0);
		def.AddAmmoType("CustomBullet15_SniperBurnGib", DMG_BULLET | DMG_SNIPER | DMG_BURN | DMG_ALWAYSGIB, TRACER_LINE_AND_WHIZ, "sk_plr_dmg_custom_sniper", "sk_npc_dmg_custom_sniper", "sk_max_custom_sniper", BULLET_IMPULSE(800, 5000), 0);
		def.AddAmmoType("CustomBullet16_SniperAlwaysGib", DMG_BULLET | DMG_SNIPER | DMG_ALWAYSGIB, TRACER_LINE_AND_WHIZ, "sk_plr_dmg_custom_sniper", "sk_npc_dmg_custom_sniper", "sk_max_custom_sniper", BULLET_IMPULSE(800, 5000), 0);
		def.AddAmmoType("CustomBullet17_SniperDissolve", DMG_BULLET | DMG_SNIPER | DMG_DISSOLVE, TRACER_LINE_AND_WHIZ, "sk_plr_dmg_custom_sniper", "sk_npc_dmg_custom_sniper", "sk_max_custom_sniper", BULLET_IMPULSE(800, 5000), 0);
		def.AddAmmoType("CustomBullet18_SniperDissolveGib", DMG_BULLET | DMG_SNIPER | DMG_DISSOLVE | DMG_ALWAYSGIB, TRACER_LINE_AND_WHIZ, "sk_plr_dmg_custom_sniper", "sk_npc_dmg_custom_sniper", "sk_max_custom_sniper", BULLET_IMPULSE(800, 5000), 0);

		def.AddAmmoType("CustomBullet19_BuckshotSniper", DMG_BULLET | DMG_BUCKSHOT | DMG_SNIPER, TRACER_LINE_AND_WHIZ, "sk_plr_dmg_custom_buckshotsniper", "sk_npc_dmg_custom_buckshotsniper", "sk_max_custom_buckshotsniper", BULLET_IMPULSE(800, 5000), 0);
		def.AddAmmoType("CustomBullet20_BuckshotSniperBurn", DMG_BULLET | DMG_BUCKSHOT | DMG_SNIPER | DMG_BURN, TRACER_LINE_AND_WHIZ, "sk_plr_dmg_custom_buckshotsniper", "sk_npc_dmg_custom_buckshotsniper", "sk_max_custom_buckshotsniper", BULLET_IMPULSE(800, 5000), 0);
		def.AddAmmoType("CustomBullet21_BuckshotSniperBurnGib", DMG_BULLET | DMG_BUCKSHOT | DMG_SNIPER | DMG_BURN | DMG_ALWAYSGIB, TRACER_LINE_AND_WHIZ, "sk_plr_dmg_custom_buckshotsniper", "sk_npc_dmg_custom_buckshotsniper", "sk_max_custom_buckshotsniper", BULLET_IMPULSE(800, 5000), 0);
		def.AddAmmoType("CustomBullet22_BuckshotSniperAlwaysGib", DMG_BULLET | DMG_BUCKSHOT | DMG_SNIPER | DMG_ALWAYSGIB, TRACER_LINE_AND_WHIZ, "sk_plr_dmg_custom_buckshotsniper", "sk_npc_dmg_custom_buckshotsniper", "sk_max_custom_buckshotsniper", BULLET_IMPULSE(800, 5000), 0);
		def.AddAmmoType("CustomBullet23_BuckshotSniperDissolve", DMG_BULLET | DMG_BUCKSHOT | DMG_SNIPER | DMG_DISSOLVE, TRACER_LINE_AND_WHIZ, "sk_plr_dmg_custom_buckshotsniper", "sk_npc_dmg_custom_buckshotsniper", "sk_max_custom_buckshotsniper", BULLET_IMPULSE(800, 5000), 0);
		def.AddAmmoType("CustomBullet24_BuckshotSniperDissolveGib", DMG_BULLET | DMG_BUCKSHOT | DMG_SNIPER | DMG_DISSOLVE | DMG_ALWAYSGIB, TRACER_LINE_AND_WHIZ, "sk_plr_dmg_custom_buckshotsniper", "sk_npc_dmg_custom_buckshotsniper", "sk_max_custom_buckshotsniper", BULLET_IMPULSE(800, 5000), 0);

		def.AddAmmoType("CustomBullet25_NormalKnockback", DMG_BULLET | DMG_KNOCKBACK, TRACER_LINE_AND_WHIZ, "sk_plr_dmg_custom_normal", "sk_npc_dmg_custom_normal", "sk_max_custom_normal", BULLET_IMPULSE(800, 5000), 0);
		def.AddAmmoType("CustomBullet26_BuckshotKnockback", DMG_BULLET | DMG_BUCKSHOT | DMG_KNOCKBACK, TRACER_LINE_AND_WHIZ, "sk_plr_dmg_custom_buckshot", "sk_npc_dmg_custom_buckshot", "sk_max_custom_buckshot", BULLET_IMPULSE(800, 5000), 0);
		def.AddAmmoType("CustomBullet27_SniperKnockback", DMG_BULLET | DMG_SNIPER | DMG_KNOCKBACK, TRACER_LINE_AND_WHIZ, "sk_plr_dmg_custom_sniper", "sk_npc_dmg_custom_sniper", "sk_max_custom_sniper", BULLET_IMPULSE(800, 5000), 0);
		def.AddAmmoType("CustomBullet28_BuckshotSniperKnockback", DMG_BULLET | DMG_BUCKSHOT | DMG_SNIPER | DMG_KNOCKBACK, TRACER_LINE_AND_WHIZ, "sk_plr_dmg_custom_buckshotsniper", "sk_npc_dmg_custom_buckshotsniper", "sk_max_custom_buckshotsniper", BULLET_IMPULSE(800, 5000), 0);
		//STOP HERE.
#ifdef HL2_EPISODIC
		def.AddAmmoType("Hopwire",			DMG_BLAST,					TRACER_NONE,			"sk_plr_dmg_grenade",		"sk_npc_dmg_grenade",		"sk_max_hopwire",		0, 0);
		def.AddAmmoType("CombineHeavyCannon",	DMG_BULLET,				TRACER_LINE,			40,	40, NULL, 10 * 750 * 12, AMMO_FORCE_DROP_IF_CARRIED ); // hit like a 10 kg weight at 750 ft/s
		def.AddAmmoType("ammo_proto1",			DMG_BULLET,				TRACER_LINE,			0, 0, 10, 0, 0 );
#endif // HL2_EPISODIC
	}

	return &def;
}

#endif
#endif
