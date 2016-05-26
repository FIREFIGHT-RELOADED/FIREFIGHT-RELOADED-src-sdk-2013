//========= Copyright © 1996-2006, Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//=============================================================================//


#include "cbase.h"
#include <KeyValues.h>
#include <vgui/IScheme.h>
#include <vgui/ISurface.h>
#include <vgui/ISystem.h>
#include <vgui/IScheme.h>
#include <vgui_controls/AnimationController.h>
#include <vgui_controls/EditablePanel.h>
#include <vgui_controls/ImagePanel.h>
#include <vgui/ISurface.h>
#include <vgui/IImage.h>
#include <vgui_controls/Label.h>

#include "fr_imagepanel.h"

using namespace vgui;

DECLARE_BUILD_FACTORY( CFRImagePanel );

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CFRImagePanel::CFRImagePanel(Panel *parent, const char *name) : ImagePanel(parent, name)
{
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CFRImagePanel::ApplySettings(KeyValues *inResourceData)
{
	BaseClass::ApplySettings( inResourceData );
}

void CFRImagePanel::FireGameEvent(IGameEvent * event)
{
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
Color CFRImagePanel::GetDrawColor(void)
{
	Color tempColor = GetFgColor();
	tempColor[3] = GetAlpha();

	return tempColor;
}