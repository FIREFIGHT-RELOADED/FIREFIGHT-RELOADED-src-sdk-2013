#include "cbase.h"
#include "game/client/iviewport.h"
#include "tier3/tier3.h"
#include "vgui/ILocalize.h"
#include "fmtstr.h"
#if !defined(MOD_VER) && !defined( _X360 ) && !defined( NO_STEAM )
#include "steam/steam_api.h"
#endif

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