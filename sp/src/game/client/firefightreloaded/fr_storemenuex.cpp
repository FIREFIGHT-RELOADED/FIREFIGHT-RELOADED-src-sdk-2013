//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#include "cbase.h"
#include <cdll_client_int.h>

#include "fr_storemenuex.h"

#include <vgui/IScheme.h>
#include <vgui/ILocalize.h>
#include <vgui/ISurface.h>
#include <KeyValues.h>
#include <vgui_controls/ImageList.h>
#include <filesystem.h>
#include "fmtstr.h"

#include <vgui_controls/RichText.h>
#include <vgui_controls/Label.h>
#include <vgui_controls/Button.h>
#include <vgui_controls/HTML.h>

#include "IGameUIFuncs.h" // for key bindings
#include <igameresources.h>
#include <game/client/iviewport.h>
#include <stdlib.h> // MAX_PATH define
#include <stdio.h>
#include "byteswap.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

extern IGameUIFuncs *gameuifuncs; // for key binding details

using namespace vgui;

void UpdateCursorState();
// void DuckMessage(const char *str);

Panel* CreateItemPanel(Panel *parent, const char *name, int price)
{
	Panel* PanelTest = new Panel(parent, "ItemPanel");
	PanelTest->SetSize(50, 120);

	Label* pLabel = new Label(PanelTest, "Title", name);
	pLabel->SetPos(10, 25);
	pLabel->SetWide(256);

	CFmtStr hint;
	hint.sprintf("%i Kash", price);
	Label* pLabel2 = new Label(PanelTest, "Price", hint.Access());
	pLabel2->SetPos(10, 50);
	pLabel2->SetWide(256);

	Button* pButton = new Button(PanelTest, "BuyButton", "Buy Item");
	pButton->SetPos(10, 75);
	pButton->SetWide(120);

	return PanelTest;
}

void CreateItemPanels(Panel* parent, CPanelListPanel *list)
{
	for (int i = 1; i < 15; i++)
	{
		CFmtStr hint;
		hint.sprintf("Test %i", i);
		Panel* item1 = CreateItemPanel(parent, hint.Access(), 10);
		list->AddItem(item1);
	}
}

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CFRStoreMenuEX::CFRStoreMenuEX(IViewPort *pViewPort) : Frame(NULL, PANEL_BUY)
{
	m_pViewPort = pViewPort;
	m_iJumpKey = BUTTON_CODE_INVALID; // this is looked up in Activate()
	m_iScoreBoardKey = BUTTON_CODE_INVALID; // this is looked up in Activate()

	// initialize dialog
	SetTitle("", true);

	// load the new scheme early!!
	SetScheme("ClientScheme");
	SetMoveable(false);
	SetSizeable(false);

	// hide the system buttons
	SetTitleBarVisible( false );
	SetProportional(true);

	m_pItemList = new CPanelListPanel(this, "ItemList");
	int x, y;
	m_pItemList->GetSize(x, y);
	CreateItemPanels(this, m_pItemList);
	m_pItemList->SetSize(x, y);
	m_pItemList->m_iScrollSpeed = 30;

	LoadControlSettings("Resource/UI/StoreMenuEX.res");
	InvalidateLayout();
}

//-----------------------------------------------------------------------------
// Purpose: Destructor
//-----------------------------------------------------------------------------
CFRStoreMenuEX::~CFRStoreMenuEX()
{
}

//-----------------------------------------------------------------------------
// Purpose: sets the text color of the map description field
//-----------------------------------------------------------------------------
void CFRStoreMenuEX::ApplySchemeSettings(IScheme *pScheme)
{
	BaseClass::ApplySchemeSettings(pScheme);
}


//-----------------------------------------------------------------------------
// Purpose: shows the team menu
//-----------------------------------------------------------------------------
void CFRStoreMenuEX::ShowPanel(bool bShow)
{
	if ( BaseClass::IsVisible() == bShow )
		return;

	if ( bShow )
	{
		Activate();

		SetMouseInputEnabled( true );

		// get key bindings if shown

		if( m_iJumpKey == BUTTON_CODE_INVALID ) // you need to lookup the jump key AFTER the engine has loaded
		{
			m_iJumpKey = gameuifuncs->GetButtonCodeForBind( "jump" );
		}

		if ( m_iScoreBoardKey == BUTTON_CODE_INVALID ) 
		{
			m_iScoreBoardKey = gameuifuncs->GetButtonCodeForBind( "showscores" );
		}
	}
	else
	{
		SetVisible( false );
		SetMouseInputEnabled( false );
	}
	m_pViewPort->ShowBackGround( bShow );
}

void CFRStoreMenuEX::OnCommand(const char *command)
{
	engine->ClientCmd(const_cast<char *>(command));
	Close();
	gViewPortInterface->ShowBackGround(false);
	BaseClass::OnCommand(command);
}

//-----------------------------------------------------------------------------
// Purpose: updates the UI with a new map name and map html page, and sets up the team buttons
//-----------------------------------------------------------------------------
void CFRStoreMenuEX::Update()
{
	MoveToCenterOfScreen();
}

//-----------------------------------------------------------------------------
// Purpose: Sets the text of a control by name
//-----------------------------------------------------------------------------
void CFRStoreMenuEX::SetLabelText(const char *textEntryName, const char *text)
{
	Label *entry = dynamic_cast<Label *>(FindChildByName(textEntryName));
	if (entry)
	{
		entry->SetText(text);
	}
}

void CFRStoreMenuEX::OnKeyCodePressed(KeyCode code)
{
	int nDir = 0;

	switch ( code )
	{
	case KEY_XBUTTON_UP:
	case KEY_XSTICK1_UP:
	case KEY_XSTICK2_UP:
	case KEY_UP:
	case KEY_XBUTTON_LEFT:
	case KEY_XSTICK1_LEFT:
	case KEY_XSTICK2_LEFT:
	case KEY_LEFT:
		nDir = -1;
		break;

	case KEY_XBUTTON_DOWN:
	case KEY_XSTICK1_DOWN:
	case KEY_XSTICK2_DOWN:
	case KEY_DOWN:
	case KEY_XBUTTON_RIGHT:
	case KEY_XSTICK1_RIGHT:
	case KEY_XSTICK2_RIGHT:
	case KEY_RIGHT:
		nDir = 1;
		break;
	}

	if ( m_iScoreBoardKey != BUTTON_CODE_INVALID && m_iScoreBoardKey == code )
	{
		gViewPortInterface->ShowPanel( PANEL_SCOREBOARD, true );
		gViewPortInterface->PostMessageToPanel( PANEL_SCOREBOARD, new KeyValues( "PollHideCode", "code", code ) );
	}
	else if ( nDir != 0 )
	{
		CUtlSortVector< SortedPanel_t, CSortedPanelYLess > vecSortedButtons;
		VguiPanelGetSortedChildButtonList( this, (void*)&vecSortedButtons, "&", 0 );

		if ( VguiPanelNavigateSortedChildButtonList( (void*)&vecSortedButtons, nDir ) != -1 )
		{
			// Handled!
			return;
		}
	}
	else
	{
		BaseClass::OnKeyCodePressed( code );
	}
}

void CFRStoreMenuEX::MoveToCenterOfScreen()
{
	int wx, wy, ww, wt;
	surface()->GetWorkspaceBounds(wx, wy, ww, wt);
	SetPos((ww - GetWide()) / 2, (wt - GetTall()) / 2);
}
