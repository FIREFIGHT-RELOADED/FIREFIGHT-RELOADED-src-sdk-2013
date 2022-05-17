//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#ifndef NEWSINGLEPLAYERGAMEDIALOG_H
#define NEWSINGLEPLAYERGAMEDIALOG_H
#ifdef _WIN32
#pragma once
#endif

#include <vgui_controls/Frame.h>

//-----------------------------------------------------------------------------
// Purpose: dialog for launching a listenserver
//-----------------------------------------------------------------------------
class CNewSingleplayerGameDialog : public vgui::Frame
{
	DECLARE_CLASS_SIMPLE( CNewSingleplayerGameDialog,  vgui::Frame );

public:
	CNewSingleplayerGameDialog(vgui::Panel *parent);
	~CNewSingleplayerGameDialog();
	
	// returns currently entered information about the server
	void SetMap(const char *name);
	bool IsRandomMapSelected();
	const char *GetMapName();

private:
	virtual void OnCommand( const char *command );
	virtual void OnClose();
	virtual void OnKeyCodeTyped(vgui::KeyCode code);
	
	void LoadMapList();
	void LoadMaps( const char *pszPathID );

	vgui::ComboBox *m_pMapList;

	// for loading/saving game config
	KeyValues* m_pSavedData;
};


#endif
