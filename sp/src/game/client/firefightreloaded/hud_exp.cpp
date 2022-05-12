//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#include "cbase.h"
#include "hudelement.h"
#include "hud_numericdisplay.h"
#include <vgui_controls/Panel.h>
#include "hud.h"
#include "hud_suitpower.h"
#include "hud_macros.h"
#include "iclientmode.h"
#include <vgui_controls/AnimationController.h>
#include <vgui/ISurface.h>
#include <vgui/ILocalize.h>

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

//-----------------------------------------------------------------------------
// Purpose: Shows the flashlight icon
//-----------------------------------------------------------------------------
class CHudEXP : public CHudElement, public vgui::Panel
{
	DECLARE_CLASS_SIMPLE( CHudEXP, vgui::Panel );

public:
	CHudEXP( const char *pElementName );
	virtual void ApplySchemeSettings( vgui::IScheme *pScheme );
	int GetXpMultiplier();
	bool ShouldDraw();
	void OnThink(void);

protected:
	virtual void Paint();

private:
	void Reset( void );

	CPanelAnimationVarAliasType( float, m_flBarInsetX, "BarInsetX", "5", "proportional_float" );
	CPanelAnimationVarAliasType( float, m_flBarInsetY, "BarInsetY", "15", "proportional_float" );
	CPanelAnimationVarAliasType( float, m_flBarWidth, "BarWidth", "140", "proportional_float" );
	CPanelAnimationVarAliasType( float, m_flBarHeight, "BarHeight", "5", "proportional_float" );
	CPanelAnimationVarAliasType( float, m_flBarChunkWidth, "BarChunkWidth", "2", "proportional_float" );
	CPanelAnimationVarAliasType( float, m_flBarChunkGap, "BarChunkGap", "1", "proportional_float" );
	CPanelAnimationVar( vgui::HFont, m_hTextFont, "TextFont", "Default" );
	CPanelAnimationVarAliasType( float, text_xpos, "text_xpos", "5", "proportional_float" );
	CPanelAnimationVarAliasType( float, text_ypos, "text_ypos", "3", "proportional_float" );
	CPanelAnimationVarAliasType(float, text_xpos2, "text_xpos2", "5", "proportional_float");
	CPanelAnimationVarAliasType(float, text_ypos2, "text_ypos2", "13", "proportional_float");
};	

using namespace vgui;


DECLARE_HUDELEMENT( CHudEXP );


//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CHudEXP::CHudEXP( const char *pElementName ) : CHudElement( pElementName ), BaseClass( NULL, "HudEXP" )
{
	vgui::Panel *pParent = g_pClientMode->GetViewport();
	SetParent( pParent );
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : *pScheme - 
//-----------------------------------------------------------------------------
void CHudEXP::ApplySchemeSettings( vgui::IScheme *pScheme )
{
	BaseClass::ApplySchemeSettings(pScheme);
}

//-----------------------------------------------------------------------------
// Purpose: Start with our background off
//-----------------------------------------------------------------------------
void CHudEXP::Reset( void )
{
	g_pClientMode->GetViewportAnimationController()->StartAnimationSequence( "EXPOn" );
}

bool CHudEXP::ShouldDraw(void)
{
	bool bNeedsDraw = (!g_fr_classic.GetBool()) || (GetAlpha() > 0);

	return (bNeedsDraw && CHudElement::ShouldDraw());
}

//-----------------------------------------------------------------------------
// Purpose: draws the flashlight icon
//-----------------------------------------------------------------------------
void CHudEXP::Paint()
{
	C_BasePlayer *pPlayer = C_BasePlayer::GetLocalPlayer();
	
	int maxXP = pPlayer->GetXpToLevelUp(pPlayer->GetLevel());

	// get bar chunks
	int chunkCount = m_flBarWidth / (m_flBarChunkWidth + m_flBarChunkGap);
	int enabledChunks = (chunkCount * (pPlayer->GetXP() * 1.0f / maxXP)); //+ 0.5f);

	Color clrEXP;
	clrEXP = gHUD.m_clrYellowish;
	Color clrText;
	clrText = gHUD.m_clrYellowish;

	// Don't draw the progress bar is we're fully charged
	if (chunkCount == enabledChunks)
		return;

	// draw our name
	surface()->DrawSetTextFont(m_hTextFont);
	surface()->DrawSetTextColor(clrText);
	surface()->DrawSetTextPos(text_xpos, text_ypos);

	wchar_t *tempString = g_pVGuiLocalize->Find("#Valve_Hud_EXPERIENCE");

	if (tempString)
	{
		surface()->DrawPrintText(tempString, wcslen(tempString));
	}
	else
	{
		surface()->DrawPrintText(L"EXPERIENCE", wcslen(L"EXPERIENCE"));
	}

	// draw the suit power bar
	if (pPlayer->GetLevel() != pPlayer->GetMaxLevel())
	{
		surface()->DrawSetColor(clrEXP);
		int xpos = m_flBarInsetX, ypos = m_flBarInsetY;
		for (int i = 0; i < enabledChunks; i++)
		{
			surface()->DrawFilledRect(xpos, ypos, xpos + m_flBarChunkWidth, ypos + m_flBarHeight);
			xpos += (m_flBarChunkWidth + m_flBarChunkGap);
		}

		// Be even less transparent than we already are
		clrEXP[3] = clrEXP[3] / 8;

		// draw the exhausted portion of the bar.
		surface()->DrawSetColor(clrEXP);
		for (int i = enabledChunks; i < chunkCount; i++)
		{
			surface()->DrawFilledRect(xpos, ypos, xpos + m_flBarChunkWidth, ypos + m_flBarHeight);
			xpos += (m_flBarChunkWidth + m_flBarChunkGap);
		}
	}
	else
	{
		surface()->DrawSetTextFont(m_hTextFont);
		surface()->DrawSetTextColor(clrText);
		surface()->DrawSetTextPos(text_xpos2, text_ypos2);

		wchar_t *tempString = g_pVGuiLocalize->Find("#Valve_Hud_EXPERIENCE_MaxLevel");

		if (tempString)
		{
			surface()->DrawPrintText(tempString, wcslen(tempString));
		}
		else
		{
			surface()->DrawPrintText(L"MAXIMUM LEVEL REACHED", wcslen(L"MAXIMUM LEVEL REACHED"));
		}
	}
}

void CHudEXP::OnThink(void)
{
	if (g_fr_classic.GetBool())
	{
		SetAlpha(0);
	}
	else
	{
		SetAlpha(255);
	}
}
