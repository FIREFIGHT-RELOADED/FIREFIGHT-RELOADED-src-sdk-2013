//====== Copyright © 1996-2007, Valve Corporation, All rights reserved. =======
//
// Purpose: VGUI panel which can play back video, in-engine
//
//=============================================================================

#ifndef FR_VGUI_VIDEO_H
#define FR_VGUI_VIDEO_H
#ifdef _WIN32
#pragma once
#endif

#include "vgui_video.h"

class CFRVideoPanel : public VideoPanel
{
	DECLARE_CLASS_SIMPLE( CFRVideoPanel, VideoPanel );
public:

	CFRVideoPanel( vgui::Panel *parent, const char *panelName );
	~CFRVideoPanel();

	virtual void OnClose();
	virtual void OnKeyCodePressed( vgui::KeyCode code ){}
	virtual void ApplySettings( KeyValues *inResourceData );
	
	virtual void GetPanelPos( int &xpos, int &ypos );
	virtual void Shutdown();

	float GetStartDelay(){ return m_flStartAnimDelay; }
	float GetEndDelay(){ return m_flEndAnimDelay; }

protected:
	virtual void ReleaseVideo();
	virtual void OnVideoOver();

private:
	float m_flStartAnimDelay;
	float m_flEndAnimDelay;
};

#endif // FR_VGUI_VIDEO_H
