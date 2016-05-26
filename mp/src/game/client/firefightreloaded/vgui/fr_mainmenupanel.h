#ifndef FRMAINMENUPANEL_H
#define FRMAINMENUPANEL_H

#include "vgui_controls/Panel.h"
#include "fr_mainmenupanelbase.h"

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class CFRMainMenuPanel : public CFRMainMenuPanelBase
{
	DECLARE_CLASS_SIMPLE(CFRMainMenuPanel, CFRMainMenuPanelBase);

public:
	CFRMainMenuPanel(vgui::Panel* parent);
	virtual ~CFRMainMenuPanel();

	void PerformLayout();
	void ApplySchemeSettings(vgui::IScheme *pScheme);
	void OnThink();
	void OnTick();
	void OnCommand(const char* command);
	void DefaultLayout();
	void GameLayout();
	void SetVersionLabel();
	void SetHintLabel();

private:
	CExLabel			*m_pVersionLabel;
	CExLabel			*m_pHintLabel;
	CFRMainMenuButton	*m_pDisconnectButton;
	CFRMainMenuButton	*m_pResumeGameButton;
	CFRMainMenuButton	*m_pPlayerListButton;
	CFRImagePanel		*m_pBackground;
	CFRImagePanel		*m_pLogo;
	EditablePanel		*m_pWeaponIcon;
	CFRVideoPanel		*m_pVideo;
	char				m_pzVideoLink[64];
	bool				b_ShowVideo;
	float				m_flActionThink;
	float				m_flAnimationThink;
	bool				m_bAnimationIn;
	char*				GetRandomVideo();
};

#endif // TFMAINMENUPANEL_H