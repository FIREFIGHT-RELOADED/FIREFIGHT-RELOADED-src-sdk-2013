#include "cbase.h"
#include "fr_mainmenubuttonbase.h"
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

DECLARE_BUILD_FACTORY_DEFAULT_TEXT(CFRMainMenuButtonBase, CFRMainMenuButtonBase);

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CFRMainMenuButtonBase::CFRMainMenuButtonBase(vgui::Panel *parent, const char *panelName, const char *text) : CExButton(parent, panelName, text)
{
	SetProportional(true);
	pImage = new EditablePanel(this, "BackgroundImage");
	Init();
	vgui::ivgui()->AddTickSignal(GetVPanel(), 100);
}	

//-----------------------------------------------------------------------------
// Purpose: Destructor
//-----------------------------------------------------------------------------
CFRMainMenuButtonBase::~CFRMainMenuButtonBase()
{
	delete pImage;
}


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CFRMainMenuButtonBase::Init()
{ 
	SetTall(50);
	SetWide(100);
	Q_strncpy(pDefaultImage, DEFAULT_IMAGE, sizeof(pDefaultImage));
	Q_strncpy(pArmedImage, ARMED_IMAGE, sizeof(pArmedImage));
	Q_strncpy(pDepressedImage, DEPRESSED_IMAGE, sizeof(pDepressedImage));
	Q_strncpy(pDefaultBorder, DEFAULT_BORDER, sizeof(pDefaultBorder));
	Q_strncpy(pArmedBorder, ARMED_BORDER, sizeof(pArmedBorder));
	Q_strncpy(pDepressedBorder, DEPRESSED_BORDER, sizeof(pDepressedBorder));
	Q_strncpy(pDefaultText, DEFAULT_TEXT, sizeof(pDefaultText));
	Q_strncpy(pArmedText, ARMED_TEXT, sizeof(pArmedText));
	Q_strncpy(pDepressedText, DEPRESSED_TEXT, sizeof(pDepressedText));
	Q_strncpy(m_szFont, DEFAULT_FONT, sizeof(m_szFont));
	Q_strncpy(m_szCommand, EMPTY_STRING, sizeof(m_szCommand));
	Q_strncpy(m_szTextAlignment, "west", sizeof(m_szCommand));
	m_bImageVisible = false;
	m_bBorderVisible = false;
	m_bAutoChange = false;
	m_bDisabled = false;
	pFont = NULL;
	//Q_strncpy(m_szCommand, GetCommand()->GetString(), sizeof(m_szCommand));
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CFRMainMenuButtonBase::ApplySettings(KeyValues *inResourceData)
{
	BaseClass::ApplySettings(inResourceData);

	Q_strncpy(pDefaultImage, inResourceData->GetString("DefaultBG", DEFAULT_IMAGE), sizeof(pDefaultImage));
	Q_strncpy(pArmedImage, inResourceData->GetString("ArmedBG", ARMED_IMAGE), sizeof(pArmedImage));
	Q_strncpy(pDepressedImage, inResourceData->GetString("DepressedBG", DEPRESSED_IMAGE), sizeof(pDepressedImage));

	Q_strncpy(pDefaultBorder, inResourceData->GetString("DefaultBorder", DEFAULT_BORDER), sizeof(pDefaultBorder));
	Q_strncpy(pArmedBorder, inResourceData->GetString("ArmedBorder", ARMED_BORDER), sizeof(pArmedBorder));
	Q_strncpy(pDepressedBorder, inResourceData->GetString("DepressedBorder", DEPRESSED_BORDER), sizeof(pDepressedBorder));

	Q_strncpy(pDefaultText, inResourceData->GetString("DefaultText", DEFAULT_TEXT), sizeof(pDefaultText));
	Q_strncpy(pArmedText, inResourceData->GetString("ArmedText", ARMED_TEXT), sizeof(pArmedText));
	Q_strncpy(pDepressedText, inResourceData->GetString("DepressedText", DEPRESSED_TEXT), sizeof(pDepressedText));

	Q_strncpy(m_szCommand, inResourceData->GetString("command", EMPTY_STRING), sizeof(m_szCommand));
	Q_strncpy(m_szTextAlignment, inResourceData->GetString("textAlignment", "center"), sizeof(m_szTextAlignment));		
	Q_strncpy(m_szFont, inResourceData->GetString("font", DEFAULT_FONT), sizeof(m_szFont));

	m_bImageVisible = inResourceData->GetBool("bgvisible", false);	
	m_bBorderVisible = inResourceData->GetBool("bordervisible", false);

	m_bOnlyInGame = inResourceData->GetBool("onlyingame", false);
	m_bOnlyAtMenu = inResourceData->GetBool("onlyatmenu", false);

	InvalidateLayout(false, true); // force ApplySchemeSettings to run
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CFRMainMenuButtonBase::ApplySchemeSettings(vgui::IScheme *pScheme)
{
	BaseClass::ApplySchemeSettings(pScheme);

	pFont = pScheme->GetFont(m_szFont);
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CFRMainMenuButtonBase::PerformLayout()
{
	BaseClass::PerformLayout();

	pImage->SetBorder(GETSCHEME()->GetBorder(pDefaultImage));
	pImage->SetVisible(m_bImageVisible);
	pImage->SetEnabled(true);
	pImage->SetPos(0, 0);
	pImage->SetZPos(1);
	pImage->SetWide(GetWide());
	pImage->SetTall(GetTall());

	SetDefaultColor(Color(0, 0, 0, 0), Color(0, 0, 0, 0));
	SetArmedColor(Color(0, 0, 0, 0), Color(0, 0, 0, 0));
	SetDepressedColor(Color(0, 0, 0, 0), Color(0, 0, 0, 0));
	SetSelectedColor(Color(0, 0, 0, 0), Color(0, 0, 0, 0));
}

const char* CFRMainMenuButtonBase::GetCommandString()
{
	KeyValues *pCommands = GetCommand();
	if (pCommands)
		return pCommands->FindKey("command")->GetString();
	return "";
}

void CFRMainMenuButtonBase::SetFont(const char *sFont)
{
	Q_strncpy(m_szFont, sFont, sizeof(m_szFont));
	PerformLayout();
}

void CFRMainMenuButtonBase::SetBorder(const char *sBorder)
{
	Q_strncpy(pDefaultBorder, sBorder, sizeof(pDefaultBorder));
	if (m_bDisabled)
	{
		Q_strncpy(pArmedBorder, sBorder, sizeof(pArmedBorder));
		Q_strncpy(pDepressedBorder, sBorder, sizeof(pDepressedBorder));
	}
	PerformLayout();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CFRMainMenuButtonBase::OnThink()
{
	BaseClass::OnThink();
}


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CFRMainMenuButtonBase::SetDefaultAnimation()
{
	pImage->SetBorder(GETSCHEME()->GetBorder(pDefaultImage));
}


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CFRMainMenuButtonBase::SendAnimation(MouseState flag)
{
	switch (flag)
	{
	//We can add additional stuff like animation here
	case MOUSE_DEFAULT:
		pImage->SetBorder(GETSCHEME()->GetBorder(pDefaultImage));
		break;
	case MOUSE_ENTERED:
		pImage->SetBorder(GETSCHEME()->GetBorder(pArmedImage));
		break;
	case MOUSE_EXITED:
		pImage->SetBorder(GETSCHEME()->GetBorder(pDefaultImage));
		break;
	case MOUSE_PRESSED:
		pImage->SetBorder(GETSCHEME()->GetBorder(pDepressedImage));
		break;
	default:
		pImage->SetBorder(GETSCHEME()->GetBorder(pDefaultImage));
		break;
	}
}

///
//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CFRButtonBase::CFRButtonBase(vgui::Panel *parent, const char *panelName, const char *text) : CExButton(parent, panelName, text)
{
	iState = MOUSE_DEFAULT;
	vgui::ivgui()->AddTickSignal(GetVPanel());
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CFRButtonBase::ApplySettings(KeyValues *inResourceData)
{
	BaseClass::ApplySettings(inResourceData);
}


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CFRButtonBase::OnCursorEntered()
{
	BaseClass::OnCursorEntered();
	if (iState != MOUSE_ENTERED)
	{
		SetMouseEnteredState(MOUSE_ENTERED);
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CFRButtonBase::OnCursorExited()
{
	BaseClass::OnCursorExited();
	if (iState != MOUSE_EXITED)
	{
		SetMouseEnteredState(MOUSE_EXITED);
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CFRButtonBase::OnMousePressed(vgui::MouseCode code)
{
	BaseClass::OnMousePressed(code);
	if (code == MOUSE_LEFT && iState != MOUSE_PRESSED)
	{
		SetMouseEnteredState(MOUSE_PRESSED);
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CFRButtonBase::OnMouseReleased(vgui::MouseCode code)
{
	BaseClass::OnMouseReleased(code);
	if (code == MOUSE_LEFT && (iState == MOUSE_ENTERED || iState == MOUSE_PRESSED))
	{
		//Set this to do something
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
void CFRButtonBase::SetMouseEnteredState(MouseState flag)
{
	iState = flag;
}


//-----------------------------------------------------------------------------
// Purpose: Set armed button border attributes.
//-----------------------------------------------------------------------------
void CFRButtonBase::SetArmedBorder(IBorder *border)
{
	_armedBorder = border;
	InvalidateLayout(false);
}

//-----------------------------------------------------------------------------
// Purpose: Set selected button border attributes.
//-----------------------------------------------------------------------------
void CFRButtonBase::SetSelectedBorder(IBorder *border)
{
	_selectedBorder = border;
	InvalidateLayout(false);
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CFRButtonBase::ApplySchemeSettings(IScheme *pScheme)
{
	BaseClass::ApplySchemeSettings(pScheme);

	_armedBorder = pScheme->GetBorder("ButtonArmedBorder");
	_selectedBorder = pScheme->GetBorder("ButtonSelectedBorder");
	InvalidateLayout();
}

//-----------------------------------------------------------------------------
// Purpose: Get button border attributes.
//-----------------------------------------------------------------------------
IBorder *CFRButtonBase::GetBorder(bool depressed, bool armed, bool selected, bool keyfocus)
{
	if (depressed)
		return _depressedBorder;
	if (armed)
		return _armedBorder;
	if (selected)
		return _selectedBorder;

	return _defaultBorder;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
vgui::Label::Alignment CFRMainMenuButtonBase::GetAlignment(char* m_szAlignment)
{
	// text alignment
	const char *alignmentString = m_szAlignment;
	int align = -1;

	if (!stricmp(alignmentString, "north-west"))
	{
		align = a_northwest;
	}
	else if (!stricmp(alignmentString, "north"))
	{
		align = a_north;
	}
	else if (!stricmp(alignmentString, "north-east"))
	{
		align = a_northeast;
	}
	else if (!stricmp(alignmentString, "west"))
	{
		align = a_west;
	}
	else if (!stricmp(alignmentString, "center"))
	{
		align = a_center;
	}
	else if (!stricmp(alignmentString, "east"))
	{
		align = a_east;
	}
	else if (!stricmp(alignmentString, "south-west"))
	{
		align = a_southwest;
	}
	else if (!stricmp(alignmentString, "south"))
	{
		align = a_south;
	}
	else if (!stricmp(alignmentString, "south-east"))
	{
		align = a_southeast;
	}

	if (align != -1)
	{
		return (Alignment)align;
	}

	return a_center;
}
