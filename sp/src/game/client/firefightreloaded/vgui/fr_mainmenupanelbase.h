#ifndef FRMAINMENUPANELBASE_H
#define FRMAINMENUPANELBASE_H

#include "GameUI/IGameUI.h"
#include "vgui_controls/Frame.h"
#include "fr_mainmenubutton.h"
#include "fr_controls.h"

#include <vgui/ISurface.h>
#include "fr_vgui_video.h"

#include <filesystem.h>
#include <vgui_controls/AnimationController.h>

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class CFRMainMenuPanelBase : public vgui::EditablePanel, public CAutoGameSystem
{
	DECLARE_CLASS_SIMPLE(CFRMainMenuPanelBase, vgui::EditablePanel);

public:
	CFRMainMenuPanelBase(vgui::Panel* parent);
	virtual ~CFRMainMenuPanelBase();

	void PerformLayout();
	virtual void ApplySchemeSettings(vgui::IScheme *pScheme);
	virtual void PaintBackground();
	virtual bool InGame();
	virtual void OnCommand(const char* command);
	virtual void OnThink();
	virtual void OnTick();
	virtual void SetMainMenu(Panel *m_pPanel);
	virtual Panel* GetMainMenu();
	virtual void DefaultLayout();
	virtual void GameLayout();

protected:
	bool				InGameLayout;
	Panel				*m_pMainMenu;
	bool				bInMenu;
};

#endif // FRMAINMENUPANELBASE_H