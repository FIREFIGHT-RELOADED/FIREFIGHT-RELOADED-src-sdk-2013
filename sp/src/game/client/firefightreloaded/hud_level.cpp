//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//
//=============================================================================//
//
// battery.cpp
//
// implementation of CHudLevel class
//
#include "cbase.h"
#include "hud.h"
#include "hudelement.h"
#include "hud_macros.h"
#include "hud_numericdisplay.h"
#include "iclientmode.h"

#include "vgui_controls/AnimationController.h"
#include "vgui/ILocalize.h"
//#include "c_baseplayer.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

#define INIT_EXP	-1

//-----------------------------------------------------------------------------
// Purpose: Displays suit power (armor) on hud
//-----------------------------------------------------------------------------
class CHudLevel : public CHudNumericDisplay, public CHudElement
{
	DECLARE_CLASS_SIMPLE( CHudLevel, CHudNumericDisplay );

public:
	CHudLevel( const char *pElementName );
	void Init( void );
	void Reset( void );
	void VidInit( void );
	void OnThink( void );
	bool ShouldDraw();

private:
	int		m_iEXP;
};

DECLARE_HUDELEMENT( CHudLevel );

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CHudLevel::CHudLevel( const char *pElementName ) : BaseClass(NULL, "HudLevel"), CHudElement( pElementName )
{
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHudLevel::Init( void )
{
	Reset();
	m_iEXP = INIT_EXP;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHudLevel::Reset(void)
{
	wchar_t *tempString = g_pVGuiLocalize->Find("#Valve_Hud_LEVEL");

	if (tempString)
	{
		SetLabelText(tempString);
	}
	else
	{
		SetLabelText(L"LEVEL");
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHudLevel::VidInit( void )
{
	Reset();
}

bool CHudLevel::ShouldDraw(void)
{
	bool bNeedsDraw = (!g_fr_classic.GetBool()) || (GetAlpha() > 0);

	return (bNeedsDraw && CHudElement::ShouldDraw());
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHudLevel::OnThink(void)
{
	C_BasePlayer *pPlayer = C_BasePlayer::GetLocalPlayer();
	if (pPlayer)
	{
		m_iEXP = pPlayer->GetLevel();

		SetDisplayValue(m_iEXP);
	}

	if (g_fr_classic.GetBool())
	{
		SetAlpha(0);
	}
	else
	{
		SetAlpha(255);
	}
}
