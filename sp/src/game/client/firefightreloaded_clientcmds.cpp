#include "cbase.h"
#include "game/client/iviewport.h"
#include "tier3/tier3.h"
#include "vgui/ILocalize.h"
#include "fmtstr.h"
#if !defined(MOD_VER) && !defined( _X360 ) && !defined( NO_STEAM )
#include "steam/steam_api.h"
#endif

CON_COMMAND( list_centities, "List all client entities." )
{
	for ( auto iter = ClientEntityList().FirstHandle(); iter.IsValid(); iter = ClientEntityList().NextHandle( iter ) )
		ConMsg( "%d: %s\n", iter.GetEntryIndex(), ((C_BaseEntity*)iter.Get())->GetClassname() );
}

CON_COMMAND( report_centities, "Report all client entities." )
{
	CUtlStringMap<int> classnames_count;
	CUtlVector<CUtlString> sorted_classnames;

	for ( auto iter = ClientEntityList().FirstBaseEntity(); iter != nullptr; iter = ClientEntityList().NextBaseEntity( iter ))
	{
		auto classname = iter->GetClassname();
		if ( classnames_count.Defined( classname ) )
			++classnames_count[classname];
		else
		{
			sorted_classnames.AddToTail( classname );
			classnames_count[classname] = 1;
		}
	}

	sorted_classnames.Sort(CUtlString::SortCaseSensitive);
	for ( auto iter : sorted_classnames )
		ConMsg( "Class: %s (%d)\n", (const char*)iter, classnames_count[iter] );
}

CON_COMMAND(toggle_ironsight, "")
{
	CBasePlayer* pPlayer = C_BasePlayer::GetLocalPlayer();
	if (pPlayer == NULL)
		return;

	CBaseCombatWeapon* pWeapon = pPlayer->GetActiveWeapon();
	if (pWeapon == NULL)
		return;

	pWeapon->ToggleIronsights();

	engine->ServerCmd("toggle_ironsight"); //forward to server
}

CON_COMMAND(showworkshop, "")
{
#if !defined(MOD_VER) && !defined( _X360 ) && !defined( NO_STEAM )
	if (steamapicontext && steamapicontext->SteamFriends())
	{
		char szWorkshopURL[1024];
		Q_snprintf(szWorkshopURL, sizeof(szWorkshopURL), "https://steamcommunity.com/app/%i/workshop/\n", engine->GetAppID());
		steamapicontext->SteamFriends()->ActivateGameOverlayToWebPage(szWorkshopURL);
	}
#endif
}