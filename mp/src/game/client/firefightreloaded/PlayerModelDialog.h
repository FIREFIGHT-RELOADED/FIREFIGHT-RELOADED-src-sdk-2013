//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#ifndef PLAYERMODELDIALOG_H
#define PLAYERMODELDIALOG_H
#ifdef _WIN32
#pragma once
#endif

#include <vgui_controls/Frame.h>

//-----------------------------------------------------------------------------
// Purpose: dialog for launching a listenserver
//-----------------------------------------------------------------------------
class CPlayerModelDialog : public vgui::Frame
{
	DECLARE_CLASS_SIMPLE( CPlayerModelDialog,  vgui::Frame );

public:
	CPlayerModelDialog(vgui::Panel *parent);
	~CPlayerModelDialog();
	
	// returns currently entered information about the server
	void SetModel(const char *name);
	const char *GetModelName();
	void DialogInit();

private:
	virtual void OnCommand( const char *command );
	virtual void OnClose();
	virtual void OnKeyCodeTyped(vgui::KeyCode code);
	
	void LoadModelList();
	void LoadModels();

	vgui::ComboBox *m_pModelList;
	vgui::Label *m_lcurrentModel;
};


#endif
