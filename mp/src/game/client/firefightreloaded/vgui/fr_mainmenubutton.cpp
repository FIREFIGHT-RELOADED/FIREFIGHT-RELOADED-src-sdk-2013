#include "cbase.h"
#include "fr_mainmenubutton.h"
#include "vgui_controls/Frame.h"
#include <vgui/ISurface.h>
#include <vgui/IVGui.h>
#include <vgui/IInput.h>
#include "vgui_controls/Button.h"
#include "vgui_controls/ImagePanel.h"
#include "fr_controls.h"
#include <filesystem.h>
#include <vgui_controls/AnimationController.h>
#include "basemodelpanel.h"

using namespace vgui;

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

DECLARE_BUILD_FACTORY_DEFAULT_TEXT(CFRMainMenuButton, CFRMainMenuButtonBase);

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CFRMainMenuButton::CFRMainMenuButton(vgui::Panel *parent, const char *panelName, const char *text) : CFRMainMenuButtonBase(parent, panelName, text)
{
	pButton = new CFRButton(this, "ButtonNew", text);
	pButton->SetParent(this);
	Init();
}

//-----------------------------------------------------------------------------
// Purpose: Destructor
//-----------------------------------------------------------------------------
CFRMainMenuButton::~CFRMainMenuButton()
{
	delete pButton;
}


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CFRMainMenuButton::Init()
{
	BaseClass::Init();
	m_bImageVisible = true;
	m_bBorderVisible = false;
	m_fXShift = 0.0;
	m_fYShift = 0.0;
}

void CFRMainMenuButton::ApplySettings(KeyValues *inResourceData)
{
	BaseClass::ApplySettings(inResourceData);

	m_fXShift = inResourceData->GetFloat("xshift", 0.0);
	m_fYShift = inResourceData->GetFloat("yshift", 0.0);

	InvalidateLayout(false, true); // force ApplySchemeSettings to run
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CFRMainMenuButton::ApplySchemeSettings(vgui::IScheme *pScheme)
{
	BaseClass::ApplySchemeSettings(pScheme);

	pButton->SetDefaultColor(pScheme->GetColor(DEFAULT_TEXT, Color(255, 255, 255, 255)), Color(0, 0, 0, 0));
	pButton->SetArmedColor(pScheme->GetColor(ARMED_TEXT, Color(255, 255, 255, 255)), Color(0, 0, 0, 0));
	pButton->SetDepressedColor(pScheme->GetColor(DEPRESSED_TEXT, Color(255, 255, 255, 255)), Color(0, 0, 0, 0));
	pButton->SetSelectedColor(pScheme->GetColor(DEPRESSED_TEXT, Color(255, 255, 255, 255)), Color(0, 0, 0, 0));
	if (m_bBorderVisible)
	{
		pButton->SetDefaultBorder(pScheme->GetBorder(pDefaultBorder));
		pButton->SetArmedBorder(pScheme->GetBorder(pArmedBorder));
		pButton->SetDepressedBorder(pScheme->GetBorder(pDepressedBorder));
		pButton->SetSelectedBorder(pScheme->GetBorder(pDepressedBorder));
	}
	else
	{
		pButton->SetDefaultBorder(pScheme->GetBorder(EMPTY_STRING));
		pButton->SetArmedBorder(pScheme->GetBorder(EMPTY_STRING));
		pButton->SetDepressedBorder(pScheme->GetBorder(EMPTY_STRING));
		pButton->SetSelectedBorder(pScheme->GetBorder(EMPTY_STRING));
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CFRMainMenuButton::PerformLayout()
{
	BaseClass::PerformLayout();

	GetText(m_szText, sizeof(m_szText));
	pButton->SetText(m_szText);
	//Msg("Text: %s\n", m_szText);
	pButton->SetCommand(GetCommandString());
	pButton->SetFont(GETSCHEME()->GetFont(m_szFont));
	pButton->SetVisible(true);
	pButton->SetEnabled(true);
	pButton->SetPos(0, 0);
	pButton->SetZPos(2);
	pButton->SetWide(GetWide());
	pButton->SetTall(GetTall());
	pButton->SetContentAlignment(GetAlignment(m_szTextAlignment));
	//pButton->SetFont(GetFont());
	pButton->SetArmedSound("ui/buttonrollover.wav");
	pButton->SetDepressedSound("ui/buttonclick.wav");
	pButton->SetReleasedSound("ui/buttonclickrelease.wav");

}

void CFRMainMenuButton::SetText(const char *tokenName)
{
	pButton->SetText(tokenName);
	BaseClass::SetText(tokenName);
}

void CFRMainMenuButton::SetCommand(const char *command)
{
	pButton->SetCommand(command);
	BaseClass::SetCommand(command);
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CFRMainMenuButton::OnTick()
{
	BaseClass::OnTick();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CFRMainMenuButton::SetDefaultAnimation()
{
	BaseClass::SetDefaultAnimation();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CFRMainMenuButton::SendAnimation(MouseState flag)
{
	BaseClass::SendAnimation(flag);

	AnimationController::PublicValue_t p_AnimLeave = { 0, 0, 0, 0 };
	AnimationController::PublicValue_t p_AnimHover = { m_fXShift, m_fYShift, 0, 0 };
	switch (flag)
	{
	//We can add additional stuff like animation here
	case MOUSE_DEFAULT:
		break;
	case MOUSE_ENTERED:
		vgui::GetAnimationController()->RunAnimationCommand(pButton, "Position", p_AnimHover, 0.0f, 0.1f, vgui::AnimationController::INTERPOLATOR_LINEAR);
		break;
	case MOUSE_EXITED:
		vgui::GetAnimationController()->RunAnimationCommand(pButton, "Position", p_AnimLeave, 0.0f, 0.1f, vgui::AnimationController::INTERPOLATOR_LINEAR);
		break;
	case MOUSE_PRESSED:
		break;
	default:
		break;
	}
}

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CFRButton::CFRButton(vgui::Panel *parent, const char *panelName, const char *text) : CFRButtonBase(parent, panelName, text)
{
	iState = MOUSE_DEFAULT;
	vgui::ivgui()->AddTickSignal(GetVPanel());
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CFRButton::OnCursorEntered()
{
	//BaseClass::OnCursorEntered();
	BaseClass::BaseClass::OnCursorEntered();
	if (iState != MOUSE_ENTERED)
	{
		SetMouseEnteredState(MOUSE_ENTERED);
	}
	
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CFRButton::OnCursorExited()
{
	BaseClass::BaseClass::OnCursorExited();
	
	if (iState != MOUSE_EXITED)
	{
		SetMouseEnteredState(MOUSE_EXITED);
	}
	
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CFRButton::OnMousePressed(vgui::MouseCode code)
{
	BaseClass::BaseClass::OnMousePressed(code);
	
	if (code == MOUSE_LEFT && iState != MOUSE_PRESSED)
	{
		SetMouseEnteredState(MOUSE_PRESSED);
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CFRButton::OnMouseReleased(vgui::MouseCode code)
{
	BaseClass::BaseClass::OnMouseReleased(code);
	
	if (code == MOUSE_LEFT && (iState == MOUSE_ENTERED || iState == MOUSE_PRESSED))
	{
		m_pParent->GetParent()->OnCommand(m_pParent->GetCommandString());
	}
	if (code == MOUSE_LEFT && iState == MOUSE_ENTERED)
	{
		SetMouseEnteredState(MOUSE_ENTERED);
	} 
	else
	{
		SetMouseEnteredState(MOUSE_EXITED);
	}
	
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CFRButton::SetMouseEnteredState(MouseState flag)
{
	BaseClass::SetMouseEnteredState(flag);
	if (!m_pParent->IsDisabled())
		m_pParent->SendAnimation(flag);
}