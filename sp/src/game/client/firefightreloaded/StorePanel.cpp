#include "cbase.h"
#include "IStorePanel.h"
using namespace vgui;
#include <vgui/IVGui.h>
#include <vgui_controls/Frame.h>
#include <vgui_controls/Label.h>
#include "filesystem.h"
//#include "gamestringpool.h"
#include "c_baseplayer.h"
#include "fmtstr.h"

ConVar cl_storemenu_visible("cl_storemenu_visible", "0", FCVAR_CLIENTDLL);
 
class CStorePanel : public vgui::Frame
{
	DECLARE_CLASS_SIMPLE(CStorePanel, vgui::Frame); 
	//CStorePanel : This Class / vgui::Frame : BaseClass
 
	CStorePanel(vgui::VPANEL parent); 	// Constructor
 	~CStorePanel(){};				// Destructor

 public:
	 //virtual void SetItemsForCommands(const char *pcCommand);
 
 protected:
 	virtual void OnTick();
 	virtual void OnCommand(const char* pcCommand);
 
 private:
	 //none
 
};
 
// Constuctor: Initializes the Panel
CStorePanel::CStorePanel(vgui::VPANEL parent)
: BaseClass(NULL, "StorePanel")
{
	SetParent( parent );
 
	SetKeyBoardInputEnabled( true );
	SetMouseInputEnabled( true );
 
	SetProportional( false );
	SetTitleBarVisible( true );
	SetMinimizeButtonVisible( false );
	SetMaximizeButtonVisible( false );
	SetCloseButtonVisible( false );
	SetSizeable( false );
	SetMoveable( true );
	SetVisible( true );
 
 
	SetScheme(vgui::scheme()->LoadSchemeFromFile("resource/SourceScheme.res", "SourceScheme"));
 
	LoadControlSettings("resource/StorePanel.res");
 
	vgui::ivgui()->AddTickSignal( GetVPanel(), 100 );
}

void CStorePanel::OnTick()
{
	BaseClass::OnTick();
	SetVisible(cl_storemenu_visible.GetBool());
}

void CStorePanel::OnCommand(const char* pcCommand)
{
	//SetItemsForCommands(pcCommand);

	C_BasePlayer *pPlayer = C_BasePlayer::GetLocalPlayer();

	if (!Q_stricmp(pcCommand, "turnoff"))
	{
		cl_storemenu_visible.SetValue(0);
	}
	else if (!Q_stricmp(pcCommand, "buysuitbattery"))
	{
		if (!pPlayer)
		{
			engine->ClientCmd("messagebox #GameUI_Store_Title #GameUI_Store_PlayerIsDead");
			cl_storemenu_visible.SetValue(0);
		}
		else
		{
			engine->ServerCmd("buyitem item_battery 20 1");
		}
	}
	else if (!Q_stricmp(pcCommand, "buyhealthkit"))
	{
		if (!pPlayer)
		{
			engine->ClientCmd("messagebox #GameUI_Store_Title #GameUI_Store_PlayerIsDead");
			cl_storemenu_visible.SetValue(0);
		}
		else
		{
			engine->ServerCmd("buyitem item_healthkit 10 1");
		}
	}
	else if (!Q_stricmp(pcCommand, "buy357ammo"))
	{
		if (!pPlayer)
		{
			engine->ClientCmd("messagebox #GameUI_Store_Title #GameUI_Store_PlayerIsDead");
			cl_storemenu_visible.SetValue(0);
		}
		else
		{
			engine->ServerCmd("buyitem item_ammo_357 25 1");
		}
	}
	else if (!Q_stricmp(pcCommand, "buyar2ammo"))
	{
		if (!pPlayer)
		{
			engine->ClientCmd("messagebox #GameUI_Store_Title #GameUI_Store_PlayerIsDead");
			cl_storemenu_visible.SetValue(0);
		}
		else
		{
			engine->ServerCmd("buyitem item_ammo_ar2 15 1");
		}
	}
	else if (!Q_stricmp(pcCommand, "buycrossbowammo"))
	{
		if (!pPlayer)
		{
			engine->ClientCmd("messagebox #GameUI_Store_Title #GameUI_Store_PlayerIsDead");
			cl_storemenu_visible.SetValue(0);
		}
		else
		{
			engine->ServerCmd("buyitem item_ammo_crossbow 30 1");
		}
	}
	else if (!Q_stricmp(pcCommand, "buypistolammo"))
	{
		if (!pPlayer)
		{
			engine->ClientCmd("messagebox #GameUI_Store_Title #GameUI_Store_PlayerIsDead");
			cl_storemenu_visible.SetValue(0);
		}
		else
		{
			engine->ServerCmd("buyitem item_ammo_pistol 10 1");
		}
	}
	else if (!Q_stricmp(pcCommand, "buysmg1ammo"))
	{
		if (!pPlayer)
		{
			engine->ClientCmd("messagebox #GameUI_Store_Title #GameUI_Store_PlayerIsDead");
			cl_storemenu_visible.SetValue(0);
		}
		else
		{
			engine->ServerCmd("buyitem item_ammo_smg1 15 1");
		}
	}
	else if (!Q_stricmp(pcCommand, "buyshotgunammo"))
	{
		if (!pPlayer)
		{
			engine->ClientCmd("messagebox #GameUI_Store_Title #GameUI_Store_PlayerIsDead");
			cl_storemenu_visible.SetValue(0);
		}
		else
		{
			engine->ServerCmd("buyitem item_box_buckshot 25 1");
		}
	}
	else if (!Q_stricmp(pcCommand, "buysmg1grenade"))
	{
		if (!pPlayer)
		{
			engine->ClientCmd("messagebox #GameUI_Store_Title #GameUI_Store_PlayerIsDead");
			cl_storemenu_visible.SetValue(0);
		}
		else
		{
			engine->ServerCmd("buyitem item_ammo_smg1_grenade 45 1");
		}
	}
	else if (!Q_stricmp(pcCommand, "buyar2darkenergyball"))
	{
		if (!pPlayer)
		{
			engine->ClientCmd("messagebox #GameUI_Store_Title #GameUI_Store_PlayerIsDead");
			cl_storemenu_visible.SetValue(0);
		}
		else
		{
			engine->ServerCmd("buyitem item_ammo_ar2_altfire 50 1");
		}
	}
	else if (!Q_stricmp(pcCommand, "buyfraggrenade"))
	{
		if (!pPlayer)
		{
			engine->ClientCmd("messagebox #GameUI_Store_Title #GameUI_Store_PlayerIsDead");
			cl_storemenu_visible.SetValue(0);
		}
		else
		{
			engine->ServerCmd("buyitem item_ammo_grenade 45 1");
		}
	}
	else if (!Q_stricmp(pcCommand, "buy357magnum"))
	{
		if (!pPlayer)
		{
			engine->ClientCmd("messagebox #GameUI_Store_Title #GameUI_Store_PlayerIsDead");
			cl_storemenu_visible.SetValue(0);
		}
		else
		{
			engine->ServerCmd("buyitem weapon_357 35 0");
		}
	}
	else if (!Q_stricmp(pcCommand, "buyar2"))
	{
		if (!pPlayer)
		{
			engine->ClientCmd("messagebox #GameUI_Store_Title #GameUI_Store_PlayerIsDead");
			cl_storemenu_visible.SetValue(0);
		}
		else
		{
			engine->ServerCmd("buyitem weapon_ar2 45 0");
		}
	}
	else if (!Q_stricmp(pcCommand, "buycrossbow"))
	{
		if (!pPlayer)
		{
			engine->ClientCmd("messagebox #GameUI_Store_Title #GameUI_Store_PlayerIsDead");
			cl_storemenu_visible.SetValue(0);
		}
		else
		{
			engine->ServerCmd("buyitem weapon_crossbow 55 0");
		}
	}
	else if (!Q_stricmp(pcCommand, "buyshotgun"))
	{
		if (!pPlayer)
		{
			engine->ClientCmd("messagebox #GameUI_Store_Title #GameUI_Store_PlayerIsDead");
			cl_storemenu_visible.SetValue(0);
		}
		else
		{
			engine->ServerCmd("buyitem weapon_shotgun 50 0");
		}
	}
	else if (!Q_stricmp(pcCommand, "buyrpg"))
	{
		if (!pPlayer)
		{
			engine->ClientCmd("messagebox #GameUI_Store_Title #GameUI_Store_PlayerIsDead");
			cl_storemenu_visible.SetValue(0);
		}
		else
		{
			engine->ServerCmd("buyitem weapon_rpg 65 0");
		}
	}
	else if (!Q_stricmp(pcCommand, "buyrpgammo"))
	{
		if (!pPlayer)
		{
			engine->ClientCmd("messagebox #GameUI_Store_Title #GameUI_Store_PlayerIsDead");
			cl_storemenu_visible.SetValue(0);
		}
		else
		{
			engine->ServerCmd("buyitem item_rpg_round 55 0");
		}
	}
	else if (!Q_stricmp(pcCommand, "buysniperrifleammo"))
	{
		if (!pPlayer)
		{
			engine->ClientCmd("messagebox #GameUI_Store_Title #GameUI_Store_PlayerIsDead");
			cl_storemenu_visible.SetValue(0);
		}
		else
		{
			engine->ServerCmd("buyitem item_ammo_sniperrifle 45 1");
		}
	}
	else if (!Q_stricmp(pcCommand, "buysniperrifle"))
	{
		if (!pPlayer)
		{
			engine->ClientCmd("messagebox #GameUI_Store_Title #GameUI_Store_PlayerIsDead");
			cl_storemenu_visible.SetValue(0);
		}
		else
		{
			engine->ServerCmd("buyitem weapon_sniper_rifle 60 0");
		}
	}
	else if (!Q_stricmp(pcCommand, "buymaxhealth"))
	{
		if (!pPlayer)
		{
			engine->ClientCmd("messagebox #GameUI_Store_Title #GameUI_Store_PlayerIsDead");
			cl_storemenu_visible.SetValue(0);
		}
		else
		{
			engine->ServerCmd("buyupgrade 0 40");
		}
	}
	else if (!Q_stricmp(pcCommand, "buymaxarmor"))
	{
		if (!pPlayer)
		{
			engine->ClientCmd("messagebox #GameUI_Store_Title #GameUI_Store_PlayerIsDead");
			cl_storemenu_visible.SetValue(0);
		}
		else
		{
			engine->ServerCmd("buyupgrade 1 40");
		}
	}
}

//
/*
void CStorePanel::SetItemsForCommands(const char *pcCommand)
{
	const char *szName = "";
	const char *szClassName = "";
	int szPrice = 0;
	int szCanHaveMultiples = 1;

	char szFullName[512];
	Q_snprintf(szFullName, sizeof(szFullName), "scripts/storeitems.txt");

	KeyValues *pkvFile = new KeyValues("Store");
	if (pkvFile->LoadFromFile(filesystem, szFullName, "MOD"))
	{
		KeyValues *pkvItems = pkvFile->FindKey("Items");
		if (pkvItems)
		{
			KeyValues *pItemKey = pkvItems->GetFirstSubKey();

			while (pItemKey)
			{
				KeyValues *pItemKeyValues = pItemKey->FindKey(szName);
				if (pItemKeyValues)
				{
					KeyValues *pItemKeyValuesAdd = pItemKeyValues->GetFirstValue();
					while (pItemKeyValuesAdd)
					{
						if (AllocPooledString(pItemKeyValuesAdd->GetName()) == AllocPooledString("classname"))
						{
							szClassName = pItemKeyValuesAdd->GetString(NULL, "");
						}
						else if (AllocPooledString(pItemKeyValuesAdd->GetName()) == AllocPooledString("price"))
						{
							szPrice = pItemKeyValuesAdd->GetInt(NULL, 0);
						}
						else if (AllocPooledString(pItemKeyValuesAdd->GetName()) == AllocPooledString("cangetmultiple"))
						{
							szCanHaveMultiples = pItemKeyValuesAdd->GetInt(NULL, 1);
						}

						pItemKeyValuesAdd = pItemKeyValuesAdd->GetNextValue();
					}
				}
				pItemKey = pItemKey->GetNextTrueSubKey();
			}
		}
	}

	if (!Q_stricmp(pcCommand, szName))
	{
		char szCommand[512];
		Q_snprintf(szCommand, sizeof(szCommand), "buyitem %s %i %i", szClassName, szPrice, szCanHaveMultiples);
		engine->ServerCmd(szCommand);
	}
}
*/

CON_COMMAND(togglestoremenu, "Toggles the Store menu")
{
	cl_storemenu_visible.SetValue(!cl_storemenu_visible.GetBool());
};

class CStorePanelInterface : public IStorePanel
{
private:
	CStorePanel *StorePanel;
public:
	CStorePanelInterface()
	{
		StorePanel = NULL;
	}
	void Create(vgui::VPANEL parent)
	{
		StorePanel = new CStorePanel(parent);
	}
	void Destroy()
	{
		if (StorePanel)
		{
			StorePanel->SetParent( (vgui::Panel *)NULL);
			delete StorePanel;
		}
	}
};
static CStorePanelInterface g_StorePanel;
IStorePanel* storepanel = (IStorePanel*)&g_StorePanel;