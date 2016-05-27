//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#include "cbase.h"
#include "PlayerModelDialog.h"

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
CPlayerModelDialog::CPlayerModelDialog(vgui::Panel *parent) : BaseClass(NULL, "PlayerModelDialog")
{
	SetSize(348, 460);
	//SetOKButtonText("#GameUI_Start");
	
	// we can use this if we decide we want to put "listen server" at the end of the game name
	m_pModelList = new ComboBox(this, "ModelList", 12, false);

	m_lcurrentModel = new Label(this, "CurrentModelPanel", "");

	LoadControlSettings("Resource/PlayerModelDialog.res");

	// create KeyValues object to load/save config options

	LoadModelList();
	DialogInit();
	SetSizeable(false);
	SetDeleteSelfOnClose(true);
	MoveToCenterOfScreen();
}

//-----------------------------------------------------------------------------
// Purpose: Destructor
//-----------------------------------------------------------------------------
CPlayerModelDialog::~CPlayerModelDialog()
{
}

void CPlayerModelDialog::DialogInit()
{
	static ConVarRef cl_playermodel("cl_playermodel");
	const char* playermodel = cl_playermodel.GetString();
	char szModelName[1024];
	Q_snprintf(szModelName, sizeof(szModelName), "Current: %s\n", playermodel);
	m_lcurrentModel->SetText(szModelName);
};

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CPlayerModelDialog::OnClose()
{
	BaseClass::OnClose();
	MarkForDeletion();
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : *command - 
//-----------------------------------------------------------------------------
void CPlayerModelDialog::OnCommand(const char *command)
{
	if ( !stricmp( command, "Ok" ) )
	{
		char szModelName[64];
		Q_strncpy(szModelName, GetModelName(), sizeof( szModelName ));

		char szModelCommand[1024];

		// create the command to execute
		Q_snprintf(szModelCommand, sizeof( szModelCommand ), "cl_playermodel models/player/playermodels/%s\n", szModelName);

		// exec
		engine->ClientCmd_Unrestricted(szModelCommand);
		OnClose();
		return;
	}

	BaseClass::OnCommand( command );
}

void CPlayerModelDialog::OnKeyCodeTyped(KeyCode code)
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
void CPlayerModelDialog::LoadModels()
{
	FileFindHandle_t findHandle = NULL;

	const char *pszFilename = g_pFullFileSystem->FindFirst("models/player/playermodels/*.mdl", &findHandle);
	while (pszFilename)
	{
		m_pModelList->AddItem(pszFilename, new KeyValues("data", "modelname", pszFilename));
		pszFilename = g_pFullFileSystem->FindNext(findHandle);
	}
	g_pFullFileSystem->FindClose(findHandle);
}

//-----------------------------------------------------------------------------
// Purpose: loads the list of available maps into the map list
//-----------------------------------------------------------------------------
void CPlayerModelDialog::LoadModelList()
{
	// clear the current list (if any)
	m_pModelList->DeleteAllItems();

	// Load the GameDir maps
	LoadModels();

	// set the first item to be selected
	m_pModelList->ActivateItem( 0 );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
const char *CPlayerModelDialog::GetModelName()
{
	int count = m_pModelList->GetItemCount();

	// if there is only one entry it's the special "select random map" entry
	if( count <= 1 )
		return NULL;

	const char *modelname = NULL;
	modelname = m_pModelList->GetActiveItemUserData()->GetString("modelname");

	return modelname;
}

//-----------------------------------------------------------------------------
// Purpose: Sets currently selected map in the map combobox
//-----------------------------------------------------------------------------
void CPlayerModelDialog::SetModel(const char *name)
{
	for (int i = 0; i < m_pModelList->GetItemCount(); i++)
	{
		if (!m_pModelList->IsItemIDValid(i))
			continue;

		if (!stricmp(m_pModelList->GetItemUserData(i)->GetString("modelname"), name))
		{
			m_pModelList->ActivateItem(i);
			break;
		}
	}
}

