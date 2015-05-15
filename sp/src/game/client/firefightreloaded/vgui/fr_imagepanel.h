//========= Copyright © 1996-2006, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#ifndef FR_IMAGEPANEL_H
#define FR_IMAGEPANEL_H
#ifdef _WIN32
#pragma once
#endif

#include <vgui/IScheme.h>
#include <vgui_controls/ImagePanel.h>
#include "GameEventListener.h"

#define MAX_BG_LENGTH		128

class CFRImagePanel : public vgui::ImagePanel, public CGameEventListener
{
public:
	DECLARE_CLASS_SIMPLE( CFRImagePanel, vgui::ImagePanel );

	CFRImagePanel(vgui::Panel *parent, const char *name);

	virtual void ApplySettings( KeyValues *inResourceData );

	virtual Color GetDrawColor( void );

public: // IGameEventListener Interface
	virtual void FireGameEvent(IGameEvent * event);
};


#endif // TF_IMAGEPANEL_H
