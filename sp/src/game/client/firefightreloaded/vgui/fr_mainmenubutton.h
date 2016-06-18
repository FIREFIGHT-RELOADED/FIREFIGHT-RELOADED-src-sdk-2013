#ifndef FR_MAINMENUBUTTON_H
#define FR_MAINMENUBUTTON_H
#ifdef _WIN32
#pragma once
#endif

#include <vgui_controls/EditablePanel.h>
#include "fr_imagepanel.h"
#include "fr_mainmenubuttonbase.h"

using namespace vgui;

class CFRButton;

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class CFRMainMenuButton : public CFRMainMenuButtonBase
{
	friend class CFRButton;
public:
	DECLARE_CLASS_SIMPLE(CFRMainMenuButton, CFRMainMenuButtonBase);

	CFRMainMenuButton(vgui::Panel *parent, const char *panelName, const char *text);
	~CFRMainMenuButton();
	void Init();

	void ApplySettings(KeyValues *inResourceData);
	void ApplySchemeSettings(vgui::IScheme *pScheme);
	void PerformLayout();

	void SendAnimation(MouseState flag);
	void SetDefaultAnimation();
	void SetText(const char *tokenName);
	void SetCommand(const char *command);

	void OnTick();

protected:
	CFRButton		*pButton;
	float			m_fXShift;
	float			m_fYShift;
};


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class CFRButton : public CFRButtonBase
{
public:
	DECLARE_CLASS_SIMPLE(CFRButton, CFRButtonBase);

	CFRButton(vgui::Panel *parent, const char *panelName, const char *text);

	void OnCursorExited();
	void OnCursorEntered();
	void OnMousePressed(vgui::MouseCode code);
	void OnMouseReleased(vgui::MouseCode code);
	void SetMouseEnteredState(MouseState flag);
	void SetParent(CFRMainMenuButton *m_pButton) { m_pParent = m_pButton; };
	char *GetCommandStr() { return m_pParent->m_szCommand; };

private:
	CFRMainMenuButton *m_pParent;
};


#endif // FR_MAINMENUBUTTON_H
