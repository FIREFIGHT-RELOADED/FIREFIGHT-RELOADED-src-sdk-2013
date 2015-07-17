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
// implementation of CHudMoneyClassic class
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
#define MAX_LEVEL 50

//-----------------------------------------------------------------------------
// Purpose: Displays suit power (armor) on hud
//-----------------------------------------------------------------------------
class CHudMoneyClassic : public CHudNumericDisplay, public CHudElement
{
	DECLARE_CLASS_SIMPLE( CHudMoneyClassic, CHudNumericDisplay );

public:
	CHudMoneyClassic( const char *pElementName );
	void Init( void );
	void Reset( void );
	void VidInit( void );
	void OnThink( void );
	bool ShouldDraw();

private:
	int		m_iMoney;
};

DECLARE_HUDELEMENT( CHudMoneyClassic );

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CHudMoneyClassic::CHudMoneyClassic( const char *pElementName ) : BaseClass(NULL, "HudMoneyClassic"), CHudElement( pElementName )
{
	SetShouldDisplayValue(true);
	SetShouldDisplaySmallValueMoney(false);
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHudMoneyClassic::Init( void )
{
	Reset();
	m_iMoney = INIT_MONEY;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHudMoneyClassic::Reset(void)
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
void CHudMoneyClassic::VidInit( void )
{
	Reset();
}

bool CHudMoneyClassic::ShouldDraw(void)
{
	bool bNeedsDraw = (g_fr_classic.GetBool() && g_fr_economy.GetBool()) || (GetAlpha() > 0);

	return (bNeedsDraw && CHudElement::ShouldDraw());
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHudMoneyClassic::OnThink(void)
{
	C_BasePlayer *pPlayer = C_BasePlayer::GetLocalPlayer();
	if (pPlayer)
	{
		m_iMoney = pPlayer->GetMoney();

		SetDisplayValue(m_iMoney);
	}

	if (pPlayer->GetLevel() == MAX_LEVEL && !g_fr_classic.GetBool())
	{
		if (g_fr_economy.GetBool())
		{
			SetPos(250, 424);
			SetAlpha(255);
		}
		else
		{
			SetAlpha(0);
		}
	}
	else if (g_fr_classic.GetBool())
	{
		if (g_fr_economy.GetBool())
		{
			SetPos(184, 424);
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
