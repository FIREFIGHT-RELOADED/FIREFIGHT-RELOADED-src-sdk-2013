//========= Copyright © 1996-2007, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#ifndef FR_CONTROLS_H
#define FR_CONTROLS_H
#ifdef _WIN32
#pragma once
#endif

#include <vgui/IScheme.h>
#include <vgui/KeyCode.h>
#include <KeyValues.h>
#include <vgui/IVGui.h>
#include <vgui_controls/ScrollBar.h>
#include <vgui_controls/EditablePanel.h>
#include <vgui_controls/Button.h>
#include <vgui_controls/Label.h>
#include <vgui_controls/RichText.h>
#include "fr_imagepanel.h"
#include <vgui_controls/ImagePanel.h>

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class CExButton : public vgui::Button
{
public:
	DECLARE_CLASS_SIMPLE( CExButton, vgui::Button );

	CExButton(vgui::Panel *parent, const char *name, const char *text);
	CExButton(vgui::Panel *parent, const char *name, const wchar_t *wszText);

	virtual void ApplySettings( KeyValues *inResourceData );
	virtual void ApplySchemeSettings( vgui::IScheme *pScheme );

private:
	char		m_szFont[64];
	char		m_szColor[64];
};

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class CExLabel : public vgui::Label
{
public:
	DECLARE_CLASS_SIMPLE( CExLabel, vgui::Label );

	CExLabel(vgui::Panel *parent, const char *panelName, const char *text);
	CExLabel(vgui::Panel *parent, const char *panelName, const wchar_t *wszText);

	virtual void ApplySettings( KeyValues *inResourceData );
	virtual void ApplySchemeSettings( vgui::IScheme *pScheme );

private:
	char		m_szColor[64];
};

#endif // FR_CONTROLS_H
