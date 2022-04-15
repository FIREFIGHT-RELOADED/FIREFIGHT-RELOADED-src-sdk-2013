//========= Copyright © 1996-2007, Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//=============================================================================//

#include "cbase.h"

#include <vgui_controls/ScrollBarSlider.h>
#include "vgui/ILocalize.h"
#include "vgui/ISurface.h"
#include "fr_controls.h"

using namespace vgui;

DECLARE_BUILD_FACTORY_DEFAULT_TEXT( CExButton, CExButton );
DECLARE_BUILD_FACTORY_DEFAULT_TEXT( CExLabel, CExLabel );

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CExButton::CExButton(Panel *parent, const char *name, const char *text) : Button(parent, name, text)
{
	m_szFont[0] = '\0';
	m_szColor[0] = '\0';
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CExButton::CExButton(Panel *parent, const char *name, const wchar_t *wszText) : Button(parent, name, wszText)
{
	m_szFont[0] = '\0';
	m_szColor[0] = '\0';
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CExButton::ApplySettings(KeyValues *inResourceData)
{
	BaseClass::ApplySettings( inResourceData );

	Q_strncpy( m_szFont, inResourceData->GetString( "font", "Default" ), sizeof( m_szFont ) );
	Q_strncpy( m_szColor, inResourceData->GetString( "fgcolor", "Button.TextColor" ), sizeof( m_szColor ) );

	InvalidateLayout( false, true ); // force ApplySchemeSettings to run
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CExButton::ApplySchemeSettings(IScheme *pScheme)
{
	BaseClass::ApplySchemeSettings( pScheme );

	SetFont( pScheme->GetFont( m_szFont, true ) );
	SetFgColor( pScheme->GetColor( m_szColor, Color( 255, 255, 255, 255 ) ) );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CExLabel::CExLabel(Panel *parent, const char *name, const char *text) : Label(parent, name, text)
{
	m_szColor[0] = '\0';
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CExLabel::CExLabel(Panel *parent, const char *name, const wchar_t *wszText) : Label(parent, name, wszText)
{
	m_szColor[0] = '\0';
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CExLabel::ApplySettings(KeyValues *inResourceData)
{
	BaseClass::ApplySettings( inResourceData );

	Q_strncpy( m_szColor, inResourceData->GetString( "fgcolor", "Label.TextColor" ), sizeof( m_szColor ) );

	InvalidateLayout( false, true ); // force ApplySchemeSettings to run
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CExLabel::ApplySchemeSettings(IScheme *pScheme)
{
	BaseClass::ApplySchemeSettings( pScheme );

	SetFgColor( pScheme->GetColor( m_szColor, Color( 255, 255, 255, 255 ) ) );
}