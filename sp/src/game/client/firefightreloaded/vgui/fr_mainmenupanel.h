#ifndef FRMAINMENUPANEL_H
#define FRMAINMENUPANEL_H

#include "vgui_controls/Panel.h"
#include "fr_mainmenupanelbase.h"

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class CHintLabel : public vgui::Label
{
public:
	DECLARE_CLASS_SIMPLE(CHintLabel, vgui::Label);

	CHintLabel(vgui::Panel* parent, const char* panelName, const char* text);
	CHintLabel(vgui::Panel* parent, const char* panelName, const wchar_t* wszText);

	virtual void ApplySettings(KeyValues* inResourceData);
	virtual void ApplySchemeSettings(vgui::IScheme* pScheme);
	void GetRandomTip();
private:
	void ConstructorShared();
	void InitTips();
	const wchar_t* GetLocalizedString(int iTip, const char* token);
private:
	char		m_szColor[64];
	int			m_iTipCountAll;
	int			m_iSplashCountAll;
	int			m_iCurrentTip;
	bool		 m_bInited;
};

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
	CHintLabel			*m_pVersionLabel;
	CHintLabel			*m_pHintLabel;
	CFRMainMenuButton	*m_pDisconnectButton;
	CFRMainMenuButton	*m_pResumeGameButton;
	CFRMainMenuButton	*m_pSaveGameButton;
	CFRMainMenuButton	*m_pReloadMapButton;
	CFRImagePanel		*m_pBackground;
	CFRImagePanel		*m_pLogo;
	EditablePanel		*m_pWeaponIcon;
	float				m_flActionThink;
	float				m_flAnimationThink;
	bool				m_bAnimationIn;
};

#endif // TFMAINMENUPANEL_H