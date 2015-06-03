#include "cbase.h"
#include "fr_mainmenu.h"
#include "fr_mainmenu_interface.h"
#include <filesystem.h>
#include <vgui_controls/AnimationController.h>

using namespace vgui;
// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

// See interface.h/.cpp for specifics:  basically this ensures that we actually Sys_UnloadModule the dll and that we don't call Sys_LoadModule 
//  over and over again.
static CDllDemandLoader g_GameUIDLL("GameUI");

CFRMainMenu *guiroot = NULL;

void OverrideMainMenu()
{
	if (!MainMenu->GetPanel())
	{
		MainMenu->Create(NULL);
	}
	if (guiroot->GetGameUI())
	{
		guiroot->GetGameUI()->SetMainMenuOverride(guiroot->GetVPanel());
		return;
	}
}

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CFRMainMenu::CFRMainMenu(VPANEL parent) : vgui::EditablePanel(NULL, "MainMenu")
{
	SetParent(parent);

	guiroot = this;
	gameui = NULL;
	LoadGameUI();
	SetScheme("ClientScheme");

	SetDragEnabled(false);
	SetShowDragHelper(false);
	SetProportional(false);
	SetVisible(true);

	surface()->GetScreenSize(width, height);
	SetSize(width, height);
	SetPos(0, 0);

	MainMenuPanel = new CFRMainMenuPanel(this);

	vgui::ivgui()->AddTickSignal(GetVPanel(), 100);
}

//-----------------------------------------------------------------------------
// Purpose: Destructor
//-----------------------------------------------------------------------------
CFRMainMenu::~CFRMainMenu()
{
	gameui = NULL;
	g_GameUIDLL.Unload();
}

void CFRMainMenu::ShowPanel(MenuPanel iPanel)
{
	switch (iPanel)
	{
	case MAIN_MENU:
		MainMenuPanel->SetVisible(true);
		break;
	default:
		break;
	}
}

void CFRMainMenu::HidePanel(MenuPanel iPanel)
{
	switch (iPanel)
	{
	case MAIN_MENU:
		MainMenuPanel->SetVisible(false);
		break;
	default:
		break;
	}
}

IGameUI *CFRMainMenu::GetGameUI()
{
	if (!gameui)
	{
		if (!LoadGameUI())
			return NULL;
	}

	return gameui;
}

bool CFRMainMenu::LoadGameUI()
{
	if (!gameui)
	{
		CreateInterfaceFn gameUIFactory = g_GameUIDLL.GetFactory();
		if (gameUIFactory)
		{
			gameui = (IGameUI *)gameUIFactory(GAMEUI_INTERFACE_VERSION, NULL);
			if (!gameui)
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	return true;
}


void CFRMainMenu::ApplySchemeSettings(vgui::IScheme *pScheme)
{
	BaseClass::ApplySchemeSettings(pScheme);
}

void CFRMainMenu::PerformLayout()
{
	BaseClass::PerformLayout();
};

void CFRMainMenu::OnCommand(const char* command)
{
	engine->ExecuteClientCmd(command);
}

void CFRMainMenu::OnTick()
{
	BaseClass::OnTick();
	if (!engine->IsDrawingLoadingImage() && !IsVisible())
	{
		SetVisible(true);
	}

	int newWidth, newHeight;
	surface()->GetScreenSize(newWidth, newHeight);
	if (width != newWidth || height != newHeight)
	{
		//restart the panel after applying res settings.
		width = newWidth;
		height = newHeight;
		SetSize(width, height);
		MainMenuPanel->SetParent((vgui::Panel *)NULL);
		delete MainMenuPanel;
		MainMenuPanel = new CFRMainMenuPanel(this);
	}
};

void CFRMainMenu::OnThink()
{
	BaseClass::OnThink();

	if (engine->IsDrawingLoadingImage() && IsVisible())
	{
		SetVisible(false);
	}
};

void CFRMainMenu::PaintBackground()
{
	SetPaintBackgroundType(0);
	BaseClass::PaintBackground();
}

bool CFRMainMenu::InGame()
{
	C_BasePlayer *pPlayer = C_BasePlayer::GetLocalPlayer();

	if (pPlayer && IsVisible())
	{
		return true;
	}
	else 
	{
		return false;
	}
}