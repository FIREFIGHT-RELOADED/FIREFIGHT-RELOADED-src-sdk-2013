//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#include "cbase.h"
#include "SpawnlistDialog.h"

#include <stdio.h>

using namespace vgui;

#include <vgui/ILocalize.h>

#include "filesystem.h"
#include <KeyValues.h>
#include <vgui_controls/ComboBox.h>
#include <vgui_controls/BitmapImagePanel.h>
#include "tier1/convar.h"

// for SRC
#include <vstdlib/random.h>

// memdbgon must be the last include file in a .cpp file!!!
#include <tier0/memdbgon.h>

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CSpawnlistDialog::CSpawnlistDialog(vgui::Panel *parent) : BaseClass(NULL, "SpawnlistDialog")
{
	SetSize(348, 460);
	//SetOKButtonText("#GameUI_Start");
	
	// we can use this if we decide we want to put "listen server" at the end of the game name
	m_pSpawnScriptList = new ComboBox(this, "SpawnScriptList", 12, false);

	m_lcurrentSpawnScript = new Label(this, "CurrentSpawnScriptPanel", "");

	LoadControlSettings("Resource/SpawnlistDialog.res");

	// create KeyValues object to load/save config options

	LoadSpawnScriptList();
	DialogInit();
	SetSizeable(false);
	SetDeleteSelfOnClose(true);
	MoveToCenterOfScreen();
}

//-----------------------------------------------------------------------------
// Purpose: Destructor
//-----------------------------------------------------------------------------
CSpawnlistDialog::~CSpawnlistDialog()
{
}

void CSpawnlistDialog::DialogInit()
{
	static ConVarRef sk_spawner_defaultspawnlist("sk_spawner_defaultspawnlist");
	const char* playermodel = sk_spawner_defaultspawnlist.GetString();
	char szModelName[1024];
	Q_snprintf(szModelName, sizeof(szModelName), "Current: %s\n", playermodel);
	m_lcurrentSpawnScript->SetText(szModelName);
};

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CSpawnlistDialog::OnClose()
{
	BaseClass::OnClose();
	MarkForDeletion();
}

void CSpawnlistDialog::OnOK()
{
	char szModelName[64];
	Q_strncpy(szModelName, GetSpawnScriptName(), sizeof(szModelName));

	char szModelCommand[1024];

	// create the command to execute
	Q_snprintf(szModelCommand, sizeof(szModelCommand), "sk_spawner_defaultspawnlist scripts/spawnlists/%s\n", szModelName);

	// exec
	engine->ClientCmd_Unrestricted(szModelCommand);

	OnClose();
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : *command - 
//-----------------------------------------------------------------------------
void CSpawnlistDialog::OnCommand(const char *command)
{
	if ( !stricmp( command, "Ok" ) )
	{
		OnOK();
		return;
	}

	BaseClass::OnCommand( command );
}

void CSpawnlistDialog::OnKeyCodeTyped(KeyCode code)
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
// Purpose: loads the list of available maps into the map list
//-----------------------------------------------------------------------------
void CSpawnlistDialog::LoadSpawnScripts()
{
	FileFindHandle_t findHandle = NULL;

	const char *pszFilename = g_pFullFileSystem->FindFirst("scripts/spawnlists/*.txt", &findHandle);
	while (pszFilename)
	{
		m_pSpawnScriptList->AddItem(pszFilename, new KeyValues("data", "listname", pszFilename));
		pszFilename = g_pFullFileSystem->FindNext(findHandle);
	}
	g_pFullFileSystem->FindClose(findHandle);
}

//-----------------------------------------------------------------------------
// Purpose: loads the list of available maps into the map list
//-----------------------------------------------------------------------------
void CSpawnlistDialog::LoadSpawnScriptList()
{
	// clear the current list (if any)
	m_pSpawnScriptList->DeleteAllItems();

	// Load the GameDir maps
	LoadSpawnScripts();

	// set the first item to be selected
	m_pSpawnScriptList->ActivateItem( 0 );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
const char *CSpawnlistDialog::GetSpawnScriptName()
{
	int count = m_pSpawnScriptList->GetItemCount();

	// if there is only one entry it's the special "select random map" entry
	if( count <= 1 )
		return NULL;

	const char *modelname = NULL;
	modelname = m_pSpawnScriptList->GetActiveItemUserData()->GetString("listname");

	return modelname;
}

//-----------------------------------------------------------------------------
// Purpose: Sets currently selected map in the map combobox
//-----------------------------------------------------------------------------
void CSpawnlistDialog::SetSpawnScript(const char *name)
{
	for (int i = 0; i < m_pSpawnScriptList->GetItemCount(); i++)
	{
		if (!m_pSpawnScriptList->IsItemIDValid(i))
			continue;

		if (!stricmp(m_pSpawnScriptList->GetItemUserData(i)->GetString("listname"), name))
		{
			m_pSpawnScriptList->ActivateItem(i);
			break;
		}
	}
}

