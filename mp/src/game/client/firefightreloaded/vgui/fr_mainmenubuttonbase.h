#ifndef FR_MAINMENUBUTTONBASE_H
#define FR_MAINMENUBUTTONBASE_H
#ifdef _WIN32
#pragma once
#endif

#include <vgui_controls/EditablePanel.h>
#include "fr_imagepanel.h"
#include "fr_controls.h"

using namespace vgui;

class CFRButtonBase;

enum MouseState
{
	MOUSE_DEFAULT,
	MOUSE_ENTERED,
	MOUSE_EXITED,
	MOUSE_PRESSED,
	MOUSE_RELEASED
};

#define DEFAULT_PATH		"../vgui/main_menu/"
#define DEFAULT_IMAGE		"MainMenuNewButtonDefault"
#define ARMED_IMAGE			"MainMenuNewButtonArmed"
#define DEPRESSED_IMAGE		"MainMenuNewButtonDepressed"
#define DEFAULT_BORDER		"RaisedBorder"
#define ARMED_BORDER		"RaisedBorder"
#define DEPRESSED_BORDER	"DepressedBorder"
#define DEFAULT_TEXT		"Button.ArmedTextColor"
#define ARMED_TEXT			"HudWhite"
#define DEPRESSED_TEXT		"Gray"
#define DEFAULT_FONT		"MenuMainTitle"
#define EMPTY_STRING		""
#define GETSCHEME()			scheme()->GetIScheme(GetScheme())

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class CFRMainMenuButtonBase : public CExButton
{
	friend class CFRButtonBase;
public:
	DECLARE_CLASS_SIMPLE(CFRMainMenuButtonBase, CExButton);

	CFRMainMenuButtonBase(vgui::Panel *parent, const char *panelName, const char *text);
	virtual ~CFRMainMenuButtonBase();
	virtual void Init();
	virtual void ApplySettings(KeyValues *inResourceData);
	virtual void ApplySchemeSettings(vgui::IScheme *pScheme);
	virtual void PerformLayout();

	virtual void SendAnimation(MouseState flag);
	virtual void SetDefaultAnimation();
	virtual const char* GetCommandString();
	virtual void SetAutoChange(bool bAutoChange) { m_bAutoChange = bAutoChange; };
	virtual bool IsAutoChange() { return m_bAutoChange; };
	virtual void SetBorderVisible(bool bVisible){ m_bBorderVisible = bVisible; };
	virtual void SetImageVisible(bool bVisible){ m_bImageVisible = bVisible; };
	virtual void SetDisabled(bool bDisabled){ m_bDisabled = bDisabled; };
	virtual bool IsDisabled() { return m_bDisabled; };
	virtual void SetFont(const char *sFont);
	virtual void SetBorder(const char *sBorder);

	virtual void OnThink();

	bool OnlyInGame(void) { return m_bOnlyInGame; };
	bool OnlyAtMenu(void) { return m_bOnlyAtMenu; };

private:
	bool			m_bOnlyInGame;
	bool			m_bOnlyAtMenu;

protected:
	bool			m_bImageVisible;
	bool			m_bBorderVisible;
	bool			m_bDisabled;
	char			pDefaultImage[64];
	char			pArmedImage[64];
	char			pDepressedImage[64];
	char			pDefaultBorder[64];
	char			pArmedBorder[64];
	char			pDepressedBorder[64];
	char			pDefaultText[64];
	char			pArmedText[64];
	char			pDepressedText[64];
	char			m_szCommand[64];
	char			m_szText[64];
	char			m_szFont[64];
	char			m_szTextAlignment[64];
	HFont			pFont;
	EditablePanel	*pImage;
	virtual			vgui::Label::Alignment GetAlignment(char* m_szAlignment);
	bool			m_bAutoChange;
	//CFRButtonBase	*pButton;
};


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class CFRButtonBase : public CExButton
{
public:
	DECLARE_CLASS_SIMPLE(CFRButtonBase, CExButton);

	CFRButtonBase(vgui::Panel *parent, const char *panelName, const char *text);

	virtual void ApplySettings(KeyValues *inResourceData);
	virtual void ApplySchemeSettings(IScheme *pScheme);

	// Set armed button border attributes.
	virtual void SetArmedBorder(vgui::IBorder *border);
	virtual void SetSelectedBorder(vgui::IBorder *border);
	// Get button border attributes.
	virtual IBorder *GetBorder(bool depressed, bool armed, bool selected, bool keyfocus);

	virtual void OnCursorExited();
	virtual void OnCursorEntered();
	virtual void OnMousePressed(vgui::MouseCode code);
	virtual void OnMouseReleased(vgui::MouseCode code);
	virtual MouseState GetState() { return iState; };
	//virtual void SetParent(CFRMainMenuButtonBase *m_pButton) { m_pParent = m_pButton; };
	//virtual char *GetCommandStr() { return m_pParent->m_szCommand; };

protected:
	virtual void	SetMouseEnteredState(MouseState flag);
	IBorder			*_armedBorder;
	IBorder			*_selectedBorder;
	MouseState		iState;

//private:
//	CFRMainMenuButtonBase *m_pParent;
};


#endif // FR_MAINMENUBUTTONBASE_H
