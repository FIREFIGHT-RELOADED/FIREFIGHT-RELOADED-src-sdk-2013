#include "cbase.h"
//LoadPanel for Singleplayer Advanced Dialog.
#include "vgui\LoadPanel.h"
#include <vgui_controls/MessageBox.h>
#include "game/client/iviewport.h"

void OpenSingleplayerAdvancedDialog(void)
{
	vgui::VPANEL parent = NULL;
	CLoadPanel* pCLoadPanel = new CLoadPanel(parent);
	pCLoadPanel->Activate();
}

ConCommand singleplayeroptions("singleplayeroptions", OpenSingleplayerAdvancedDialog);

void OpenMessageBox(const CCommand &args)
{
	MessageBox *pMessageBox = new MessageBox(args[1], args[2]);
	pMessageBox->DoModal();
}

ConCommand messagebox("messagebox", OpenMessageBox);

void CC_ToggleIronSights(void)
{
	CBasePlayer* pPlayer = C_BasePlayer::GetLocalPlayer();
	if (pPlayer == NULL)
		return;

	CBaseCombatWeapon *pWeapon = pPlayer->GetActiveWeapon();
	if (pWeapon == NULL)
		return;

	pWeapon->ToggleIronsights();

	engine->ServerCmd("toggle_ironsight"); //forward to server
}

static ConCommand toggle_ironsight("toggle_ironsight", CC_ToggleIronSights);