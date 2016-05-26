#include "cbase.h"
#include "fr_mainmenupanelbase.h"
#include "fr_mainmenu.h"

using namespace vgui;
// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CFRMainMenuPanelBase::CFRMainMenuPanelBase(vgui::Panel* parent) : EditablePanel(NULL, "MainMenuPanel")
{
	SetParent(parent);
	SetScheme("ClientScheme");
	SetProportional(false);
	SetVisible(true);

	int width, height;
	surface()->GetScreenSize(width, height);
	SetSize(width, height);
	SetPos(0, 0);

	vgui::ivgui()->AddTickSignal(GetVPanel(), 100);

	bInMenu = true;
}

void CFRMainMenuPanelBase::ApplySchemeSettings(vgui::IScheme *pScheme)
{
	BaseClass::ApplySchemeSettings(pScheme);
}

void CFRMainMenuPanelBase::PerformLayout()
{
	BaseClass::PerformLayout();
};

//-----------------------------------------------------------------------------
// Purpose: Destructor
//-----------------------------------------------------------------------------
CFRMainMenuPanelBase::~CFRMainMenuPanelBase()
{

}

void CFRMainMenuPanelBase::OnCommand(const char* command)
{
	engine->ExecuteClientCmd(command);
}

void CFRMainMenuPanelBase::OnTick()
{
	BaseClass::OnTick();
};

void CFRMainMenuPanelBase::OnThink()
{
	BaseClass::OnThink();
};

void CFRMainMenuPanelBase::DefaultLayout()
{

};

void CFRMainMenuPanelBase::GameLayout()
{

};

void CFRMainMenuPanelBase::SetMainMenu(Panel *m_pPanel)
{
	m_pMainMenu = dynamic_cast<CFRMainMenu*>(m_pPanel);
};

Panel* CFRMainMenuPanelBase::GetMainMenu()
{
	return dynamic_cast<CFRMainMenu*>(m_pMainMenu);
};

void CFRMainMenuPanelBase::PaintBackground()
{
	SetPaintBackgroundType(0);
	BaseClass::PaintBackground();
}

bool CFRMainMenuPanelBase::InGame()
{
	C_BasePlayer *pPlayer = C_BasePlayer::GetLocalPlayer();

	if (pPlayer && IsVisible())
	{
		bInMenu = false;
		return true;
	}
	else 
	{
		bInMenu = true;
		return false;
	}
}