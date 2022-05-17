//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#ifndef SPAWNLISTDIALOG_H
#define SPAWNLISTDIALOG_H
#ifdef _WIN32
#pragma once
#endif

#include <vgui_controls/Frame.h>

//-----------------------------------------------------------------------------
// Purpose: dialog for launching a listenserver
//-----------------------------------------------------------------------------
class CSpawnlistDialog : public vgui::Frame
{
	DECLARE_CLASS_SIMPLE( CSpawnlistDialog,  vgui::Frame );

public:
	CSpawnlistDialog(vgui::Panel *parent);
	~CSpawnlistDialog();
	
	// returns currently entered information about the server
	void SetSpawnScript(const char *name);
	const char *GetSpawnScriptName();
	void DialogInit();

private:
	virtual void OnCommand( const char *command );
	virtual void OnClose();
	virtual void OnOK();
	virtual void OnKeyCodeTyped(vgui::KeyCode code);
	
	void LoadSpawnScriptList();
	void LoadSpawnScripts();

	vgui::ComboBox *m_pSpawnScriptList;
	vgui::Label *m_lcurrentSpawnScript;
};


#endif
