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
// implementation of CHudMoney class
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
#include <igameresources.h>

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

#define INIT_MONEY	-1

//-----------------------------------------------------------------------------
// Purpose: Displays suit power (armor) on hud
//-----------------------------------------------------------------------------
class CHudMoney : public CHudNumericDisplay, public CHudElement
{
	DECLARE_CLASS_SIMPLE( CHudMoney, CHudNumericDisplay );

public:
	CHudMoney( const char *pElementName );
	void Init( void );
	void Reset( void );
	void VidInit( void );
	void OnThink( void );
	bool ShouldDraw();

private:
	int		m_iMoney;
};

DECLARE_HUDELEMENT( CHudMoney );

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CHudMoney::CHudMoney( const char *pElementName ) : BaseClass(NULL, "HudMoney"), CHudElement( pElementName )
{
	SetShouldDisplayValue(false);
	SetShouldDisplaySmallValueMoney(true);
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHudMoney::Init( void )
{
	Reset();
	m_iMoney = INIT_MONEY;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHudMoney::Reset(void)
{
	wchar_t *tempString = g_pVGuiLocalize->Find("#Valve_Hud_MONEY");

	if (tempString)
	{
		SetLabelText(tempString);
	}
	else
	{
		SetLabelText(L"MONEY");
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHudMoney::VidInit( void )
{
	Reset();
}

bool CHudMoney::ShouldDraw(void)
{
	bool bNeedsDraw = (!g_fr_classic.GetBool() && g_fr_economy.GetBool()) || (GetAlpha() > 0);

	return (bNeedsDraw && CHudElement::ShouldDraw());
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHudMoney::OnThink(void)
{
	C_BasePlayer *pPlayer = C_BasePlayer::GetLocalPlayer();
	if (pPlayer)
	{
		m_iMoney = pPlayer->GetMoney();

		SetDisplayValue(m_iMoney);
	}

	if (!g_fr_classic.GetBool())
	{
		if (g_fr_economy.GetBool())
		{
			SetAlpha(255);
		}
		else
		{
			SetAlpha(0);
		}
	}
	else
	{
		SetAlpha(0);
	}
}
