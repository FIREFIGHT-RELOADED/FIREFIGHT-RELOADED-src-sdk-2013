#ifndef TFMAINMENU_H
#define TFMAINMENU_H

#include "vgui_controls/Frame.h"
#include "vgui_controls/Panel.h"
#include "GameUI/IGameUI.h"
#include "fr_mainmenupanel.h"

enum MenuPanel 
{
	NONE_MENU,
	MAIN_MENU,
	COUNT_MENU
};

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class CFRMainMenu : public vgui::EditablePanel
{
	DECLARE_CLASS_SIMPLE(CFRMainMenu, vgui::EditablePanel);

public:
	CFRMainMenu(vgui::VPANEL parent);
	virtual ~CFRMainMenu();
	IGameUI*	 GetGameUI();
	virtual void PerformLayout();
	virtual void ApplySchemeSettings(vgui::IScheme *pScheme);
	virtual void PaintBackground();
	virtual void OnCommand(const char* command);
	virtual void OnThink();
	virtual void OnTick();
	virtual void ShowPanel(MenuPanel iPanel);
	virtual void HidePanel(MenuPanel iPanel);

private:
	CFRMainMenuPanel	*MainMenuPanel;
	bool				LoadGameUI();
	bool				InGame();
	IGameUI*			gameui;
	int					width;
	int					height;
};

extern CFRMainMenu *guiroot;

#endif // TFMAINMENU_H