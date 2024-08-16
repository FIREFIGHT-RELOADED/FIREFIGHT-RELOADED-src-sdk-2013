//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#include "cbase.h"
#include "hud.h"
#include "text_message.h"
#include "hud_macros.h"
#include "iclientmode.h"
#include "view.h"
#include "KeyValues.h"
#include "vgui_controls/AnimationController.h"
#include "vgui/ILocalize.h"
#include "vgui/ISurface.h"
#include "VGuiMatSurface/IMatSystemSurface.h"
#include "materialsystem/imaterial.h"
#include "materialsystem/imesh.h"
#include "materialsystem/imaterialvar.h"
#include "IEffects.h"
#include "hudelement.h"
#include "fmodmanager.h"
#include "fmtstr.h"
#include "gamepadui_string.h"

using namespace vgui;

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

ConVar snd_fmod_musicsystem_shownowplayinghud("snd_fmod_musicsystem_shownowplayinghud", "1", FCVAR_ARCHIVE);

//-----------------------------------------------------------------------------
// Purpose: HDU Damage indication
//-----------------------------------------------------------------------------
class CHudNowPlaying : public CHudElement, public vgui::Panel
{
	DECLARE_CLASS_SIMPLE( CHudNowPlaying, vgui::Panel );

public:
	CHudNowPlaying( const char *pElementName );
	void Reset( void );
	virtual bool ShouldDraw( void );

private:
	virtual void Paint();
	virtual void ApplySchemeSettings(vgui::IScheme *pScheme);

private:
	CPanelAnimationVar( vgui::HFont, m_hTextFont, "TextFont", "Default" );
	CPanelAnimationVar( Color, m_TextColor, "TextColor", "FgColor" );
	CPanelAnimationVarAliasType( float, text_xpos, "text_xpos", "8", "proportional_float" );
	CPanelAnimationVarAliasType( float, text_ypos, "text_ypos", "8", "proportional_float" );
	CPanelAnimationVarAliasType( float, text_ygap, "text_ygap", "14", "proportional_float" );
};

DECLARE_HUDELEMENT( CHudNowPlaying );

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CHudNowPlaying::CHudNowPlaying( const char *pElementName ) : CHudElement( pElementName ), BaseClass(NULL, "HudNowPlaying")
{
	vgui::Panel *pParent = g_pClientMode->GetViewport();
	SetParent( pParent );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHudNowPlaying::Reset( void )
{
	//SetAlpha( 0 );
}

//-----------------------------------------------------------------------------
// Purpose: Save CPU cycles by letting the HUD system early cull
// costly traversal.  Called per frame, return true if thinking and 
// painting need to occur.
//-----------------------------------------------------------------------------
bool CHudNowPlaying::ShouldDraw( void )
{
	return (GetMusicSystem() && !GetMusicSystem()->m_bDisabled && snd_fmod_musicsystem_shownowplayinghud.GetBool());
}

//-----------------------------------------------------------------------------
// Purpose: Paints the damage display
//-----------------------------------------------------------------------------
void CHudNowPlaying::Paint()
{
	// draw the text
	surface()->DrawSetTextFont( m_hTextFont );
	surface()->DrawSetTextColor( m_TextColor );
	surface()->DrawSetTextPos(text_xpos, text_ypos);
	int ypos = text_ypos;

	CFMODMusicSystem* system = GetMusicSystem();

	GamepadUIString nowPlaying = "#GameUI_MusicSystem_NowPlaying";
	GamepadUIString title = system->GetTitleString();
	GamepadUIString artist = system->GetArtistString();
	GamepadUIString album = system->GetAlbumString();

	wchar_t wszBuf[4096];
#ifdef WIN32
	V_snwprintf(wszBuf, 4096, L"%s\n%s\n%s\n%s", nowPlaying.String(), title.String(), artist.String(), album.String());
#else
	V_snwprintf(wszBuf, 4096, L"%S\n%S\n%S\n%S", nowPlaying.String(), title.String(), artist.String(), album.String());
#endif

	const wchar_t* finalText = wszBuf;
	Assert(finalText);

	for (const wchar_t* wch = finalText; wch && *wch != 0; wch++)
	{
		if (*wch == '\n')
		{
			ypos += text_ygap;
			surface()->DrawSetTextPos(text_xpos, ypos);
		}
		else
		{
			surface()->DrawUnicodeChar(*wch);
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: hud scheme settings
//-----------------------------------------------------------------------------
void CHudNowPlaying::ApplySchemeSettings(vgui::IScheme *pScheme)
{
	BaseClass::ApplySchemeSettings(pScheme);
}
