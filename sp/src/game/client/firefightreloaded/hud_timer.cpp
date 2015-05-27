//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//
//=============================================================================//
//
// Health.cpp
//
// implementation of CHudTimer class
//
#include "cbase.h"
#include "hud.h"
#include "hud_macros.h"
#include "view.h"

#include "iclientmode.h"

#include <KeyValues.h>
#include <vgui/ISurface.h>
#include <vgui/ISystem.h>
#include <vgui_controls/AnimationController.h>

#include <vgui/ILocalize.h>

using namespace vgui;

#include "hudelement.h"
#include "hud_basetimer.h"

#include "convar.h"

#include "engine/IEngineSound.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

#define INIT_TIMER -1

ConVar cl_fr_usetimer("cl_fr_usetimer", "1", FCVAR_ARCHIVE);

//-----------------------------------------------------------------------------
// Purpose: Health panel
//-----------------------------------------------------------------------------
class CHudTimer : public CHudElement, public CHudBaseTimer
{
	DECLARE_CLASS_SIMPLE( CHudTimer, CHudBaseTimer );

public:
	CHudTimer( const char *pElementName );
	virtual void Init( void );
	virtual void VidInit( void );
	virtual void Reset( void );
	virtual void OnThink();

private:
	// old variables
};	

DECLARE_HUDELEMENT( CHudTimer );

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CHudTimer::CHudTimer( const char *pElementName ) : CHudElement( pElementName ), CHudBaseTimer(NULL, "HudTimer")
{
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHudTimer::Init()
{
	Reset();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHudTimer::Reset()
{
	SetMinutes(INIT_TIMER);
	SetSeconds(INIT_TIMER);
	g_pClientMode->GetViewportAnimationController()->StartAnimationSequence("TimerInit");
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHudTimer::VidInit()
{
	Reset();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHudTimer::OnThink()
{
	if (cl_fr_usetimer.GetBool())
	{
		int iMinutes, iSeconds;
		int iRemain = 300;
		//iRemain += 1;
		iMinutes = iRemain / 60;
		iSeconds = iRemain % 60;
		SetMinutes(iMinutes);
		SetSeconds(iSeconds);
	}
	else
	{
		g_pClientMode->GetViewportAnimationController()->StartAnimationSequence("TimerHide");
	}
}