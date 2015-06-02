//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#include "cbase.h"
#include <time.h>

#include "SingleplayerAdvancedDialog.h"

#include <vgui/ILocalize.h>
#include <vgui/ISurface.h>
#include <vgui_controls/ListPanel.h>
#include <KeyValues.h>
#include <vgui_controls/Label.h>
#include <vgui_controls/Button.h>
#include <vgui_controls/MessageBox.h>
#include <vgui_controls/CheckButton.h>
#include <vgui_controls/ComboBox.h>
#include <vgui_controls/TextEntry.h>
#include <vgui_controls/Slider.h>
#include "PanelListPanel.h"
#include <vgui/IInput.h>

#include "FileSystem.h"

#include <tier0/vcrmode.h>

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

using namespace vgui;

#define OPTIONS_DIR "cfg"
#define DEFAULT_OPTIONS_FILE OPTIONS_DIR "/user_default_sp.scr"
#define OPTIONS_FILE OPTIONS_DIR "/user_sp.scr"

ConVar singleadvanced_listlines("singleadvanced_listlines", "100", FCVAR_ARCHIVE);

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CSingleplayerAdvancedDialog::CSingleplayerAdvancedDialog(vgui::Panel *parent) : BaseClass(NULL, "SingleplayerAdvancedDialog")
{
	SetBounds(0, 0, 855, 450);
	SetMinimumSize(368, 193);

	SetTitle("#GameUI_SingleplayerAdvanced", true);

	Button *cancel = new Button( this, "Cancel", "#GameUI_Cancel" );
	cancel->SetCommand( "Close" );

	Button *ok = new Button( this, "OK", "#GameUI_OK" );
	ok->SetCommand( "Ok" );

	m_pListPanel = new CPanelListPanel( this, "PanelListPanel" );

	m_pList = NULL;

	m_pDescription = new CInfoDescription( m_pListPanel );
	m_pDescription->InitFromFile( DEFAULT_OPTIONS_FILE );
	m_pDescription->InitFromFile( OPTIONS_FILE );
	m_pDescription->TransferCurrentValues( NULL );

	LoadControlSettings("Resource\\SingleplayerAdvancedDialog.res");
	CreateControls();

	MoveToCenterOfScreen();
	SetSizeable( true );
	SetDeleteSelfOnClose( true );
}

//-----------------------------------------------------------------------------
// Purpose: Destructor
//-----------------------------------------------------------------------------
CSingleplayerAdvancedDialog::~CSingleplayerAdvancedDialog()
{
	delete m_pDescription;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CSingleplayerAdvancedDialog::Activate()
{
	BaseClass::Activate();
}


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CSingleplayerAdvancedDialog::OnClose()
{
	BaseClass::OnClose();
	MarkForDeletion();
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : *command - 
//-----------------------------------------------------------------------------
void CSingleplayerAdvancedDialog::OnCommand(const char *command)
{
	if ( !stricmp( command, "Ok" ) )
	{
		// OnApplyChanges();
		SaveValues();
		OnClose();
		return;
	}

	BaseClass::OnCommand( command );
}

void CSingleplayerAdvancedDialog::OnKeyCodeTyped(KeyCode code)
{
	// force ourselves to be closed if the escape key it pressed
	if (code == KEY_ESCAPE)
	{
		Close();
	}
	else
	{
		BaseClass::OnKeyCodeTyped(code);
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CSingleplayerAdvancedDialog::GatherCurrentValues()
{
	if ( !m_pDescription )
		return;

	// OK
	CheckButton *pBox;
	TextEntry *pEdit;
	ComboBox *pCombo;
	Slider *pSlider;

	mpcontrol_t *pList;

	CScriptObject *pObj;
	CScriptListItem *pItem;

	char szValue[256];
	char strValue[ 256 ];
	int iValue;

	pList = m_pList;
	while ( pList )
	{
		pObj = pList->pScrObj;

		if ( !pList->pControl )
		{
			pObj->SetCurValue( pObj->defValue );
			pList = pList->next;
			continue;
		}

		switch ( pObj->type )
		{
		case O_BOOL:
			pBox = (CheckButton *)pList->pControl;
			sprintf( szValue, "%s", pBox->IsSelected() ? "1" : "0" );
			break;
		case O_NUMBER:
			pEdit = ( TextEntry * )pList->pControl;
			pEdit->GetText( strValue, sizeof( strValue ) );
			sprintf( szValue, "%s", strValue );
			break;
		case O_SLIDER:
			pSlider = (Slider *)pList->pControl;
			iValue = pSlider->GetValue();
			sprintf(szValue, "%i", iValue);
			break;
		case O_STRING:
			pEdit = ( TextEntry * )pList->pControl;
			pEdit->GetText( strValue, sizeof( strValue ) );
			sprintf( szValue, "%s", strValue );
			break;
		case O_CATEGORY:
			break;
		case O_LIST:
			pCombo = (ComboBox *)pList->pControl;
			// pCombo->GetText( strValue, sizeof( strValue ) );
			int activeItem = pCombo->GetActiveItem();
			
			pItem = pObj->pListItems;
//			int n = (int)pObj->fdefValue;

			while ( pItem )
			{
				if (!activeItem--)
					break;

				pItem = pItem->pNext;
			}

			if ( pItem )
			{
				sprintf( szValue, "%s", pItem->szValue );
			}
			else  // Couln't find index
			{
				//assert(!("Couldn't find string in list, using default value"));
				sprintf( szValue, "%s", pObj->defValue );
			}
			break;
		}

		// Remove double quotes and % characters
		UTIL_StripInvalidCharacters( szValue, sizeof(szValue) );

		strcpy( strValue, szValue );

		pObj->SetCurValue( strValue );

		pList = pList->next;
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CSingleplayerAdvancedDialog::CreateControls()
{
	DestroyControls();

	// Go through desciption creating controls
	CScriptObject *pObj;

	pObj = m_pDescription->pObjList;

	mpcontrol_t	*pCtrl;

	CheckButton *pBox;
	TextEntry *pEdit;
	ComboBox *pCombo;
	Label *pLabel;
	Slider *pSlider;
	CScriptListItem *pListItem;

	Panel *objParent = m_pListPanel;

	while ( pObj )
	{
		if ( pObj->type == O_OBSOLETE )
		{
			pObj = pObj->pNext;
			continue;
		}

		pCtrl = new mpcontrol_t( objParent, "mpcontrol_t" );
		pCtrl->type = pObj->type;

		switch ( pCtrl->type )
		{
		case O_BOOL:
			pBox = new CheckButton( pCtrl, "DescCheckButton", pObj->prompt );
			pBox->SetSelected( pObj->fdefValue != 0.0f ? true : false );
			
			pCtrl->pControl = (Panel *)pBox;
			break;
		case O_STRING:
		case O_NUMBER:
			pEdit = new TextEntry( pCtrl, "DescTextEntry");
			pEdit->InsertString(pObj->defValue);
			pCtrl->pControl = (Panel *)pEdit;
			break;
		case O_SLIDER:
			pSlider = new Slider(pCtrl, "DescScrollEntry");
			pSlider->SetValue(pObj->fdefValue);
			pSlider->SetRange(pObj->fMin, pObj->fMax);
			pCtrl->pControl = (Panel *)pSlider;
			break;
		case O_LIST:
			pCombo = new ComboBox(pCtrl, "DescComboBox", singleadvanced_listlines.GetInt(), false);

			pListItem = pObj->pListItems;
			while ( pListItem )
			{
				pCombo->AddItem( pListItem->szItemText, NULL );
				pListItem = pListItem->pNext;
			}

			pCombo->ActivateItemByRow((int)pObj->fdefValue);

			pCtrl->pControl = (Panel *)pCombo;
			break;
		case O_CATEGORY:
			pLabel = new Label(pCtrl, "DescTextTitle", pObj->prompt);
			pCtrl->pControl = (Panel *)pLabel;
			break;
		default:
			break;
		}

		if (pCtrl->type != O_BOOL && pCtrl->type != O_CATEGORY && pCtrl->type != O_SLIDER)
		{
			pCtrl->pPrompt = new vgui::Label( pCtrl, "DescLabel", "" );
			pCtrl->pPrompt->SetContentAlignment( vgui::Label::a_west );
			pCtrl->pPrompt->SetTextInset( 5, 0 );
			pCtrl->pPrompt->SetText( pObj->prompt );
		}

		pCtrl->pScrObj = pObj;
		pCtrl->SetSize( 100, 28 );
		//pCtrl->SetBorder( scheme()->GetBorder(1, "DepressedButtonBorder") );
		m_pListPanel->AddItem( pCtrl );

		// Link it in
		if ( !m_pList )
		{
			m_pList = pCtrl;
			pCtrl->next = NULL;
		}
		else
		{
			mpcontrol_t *p;
			p = m_pList;
			while ( p )
			{
				if ( !p->next )
				{
					p->next = pCtrl;
					pCtrl->next = NULL;
					break;
				}
				p = p->next;
			}
		}

		pObj = pObj->pNext;
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CSingleplayerAdvancedDialog::DestroyControls()
{
	mpcontrol_t *p, *n;

	p = m_pList;
	while ( p )
	{
		n = p->next;
		//
		delete p->pControl;
		delete p->pPrompt;
		delete p;
		p = n;
	}

	m_pList = NULL;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CSingleplayerAdvancedDialog::SaveValues()
{
	// Get the values from the controls:
	GatherCurrentValues();

	// Create the game.cfg file
	if ( m_pDescription )
	{
		FileHandle_t fp;

		// Add settings to config.cfg
		m_pDescription->WriteToConfig();

		g_pFullFileSystem->CreateDirHierarchy( OPTIONS_DIR );
		fp = g_pFullFileSystem->Open( OPTIONS_FILE, "wb" );
		if ( fp )
		{
			m_pDescription->WriteToScriptFile( fp );
			g_pFullFileSystem->Close( fp );
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: Constructor, load/save client settings object
//-----------------------------------------------------------------------------
CInfoDescription::CInfoDescription( CPanelListPanel *panel )
: CDescription( panel )
{
	setHint( "// NOTE:  THIS FILE IS AUTOMATICALLY REGENERATED, \r\n\
//DO NOT EDIT THIS HEADER, YOUR COMMENTS WILL BE LOST IF YOU DO\r\n\
// User options script\r\n\
//\r\n\
// Format:\r\n\
//  Version [float]\r\n\
//  Options description followed by \r\n\
//  Options defaults\r\n\
//\r\n\
// Option description syntax:\r\n\
//\r\n\
//  \"cvar\" { \"Prompt\" { type [ type info ] } { default } }\r\n\
//\r\n\
//  type = \r\n\
//   BOOL   (a yes/no toggle)\r\n\
//   STRING\r\n\
//   NUMBER\r\n\
//   SLIDER\r\n\
//   LIST\r\n\
//   CATEGORY\r\n\
//\r\n\
// type info:\r\n\
// BOOL                 no type info\r\n\
// NUMBER       min max range, use -1 -1 for no limits\r\n\
// SLIDER       min max range, use -1 -1 for no limits\r\n\
// STRING       no type info\r\n\
// LIST         "" delimited list of options value pairs\r\n\
// CATEGORY		no type info\r\n\
//\r\n\
//\r\n\
// default depends on type\r\n\
// BOOL is \"0\" or \"1\"\r\n\
// NUMBER is \"value\"\r\n\
// SLIDER is \"value\"\r\n\
// STRING is \"value\"\r\n\
// LIST is \"index\", where index \"0\" is the first element of the list\r\n\
// CATEGORY	is none\r\n\r\n\r\n");

	setDescription( "INFO_SINGLEPLAYEROPTIONS" );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CInfoDescription::WriteScriptHeader( FileHandle_t fp )
{
    char am_pm[] = "AM";
	tm newtime;
	VCRHook_LocalTime( &newtime );

	g_pFullFileSystem->FPrintf( fp, (char *)getHint() );

// Write out the comment and Cvar Info:
	g_pFullFileSystem->FPrintf( fp, "// Half-Life User Info Configuration Layout Script (stores last settings chosen, too)\r\n" );
	g_pFullFileSystem->FPrintf( fp, "// File generated:  %.19s %s\r\n", asctime( &newtime ), am_pm );
	g_pFullFileSystem->FPrintf( fp, "//\r\n//\r\n// Cvar\t-\tSetting\r\n\r\n" );
	g_pFullFileSystem->FPrintf( fp, "VERSION %.1f\r\n\r\n", SCRIPT_VERSION );
	g_pFullFileSystem->FPrintf( fp, "DESCRIPTION INFO_SINGLEPLAYEROPTIONS\r\n{\r\n" );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CInfoDescription::WriteFileHeader( FileHandle_t fp )
{
    char am_pm[] = "AM";
	tm newtime;
	VCRHook_LocalTime( &newtime );

	g_pFullFileSystem->FPrintf( fp, "// Half-Life User Info Configuration Settings\r\n" );
	g_pFullFileSystem->FPrintf( fp, "// DO NOT EDIT, GENERATED BY HALF-LIFE\r\n" );
	g_pFullFileSystem->FPrintf( fp, "// File generated:  %.19s %s\r\n", asctime( &newtime ), am_pm );
	g_pFullFileSystem->FPrintf( fp, "//\r\n//\r\n// Cvar\t-\tSetting\r\n\r\n" );
}

//-----------------------------------------------------------------------------
