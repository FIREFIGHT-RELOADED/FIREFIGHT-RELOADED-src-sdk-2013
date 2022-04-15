//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: Tips
//
//=====================================================================================//

#include "cbase.h"
#include "fr_tips.h"
#include "tier3/tier3.h"
#include "vgui/ILocalize.h"
#include "cdll_util.h"
#include "fmtstr.h"

//-----------------------------------------------------------------------------
// Purpose: constructor
//-----------------------------------------------------------------------------
CFRTips::CFRTips() : CAutoGameSystem("CFRTips")
{
	m_iTipCountAll = 0;
	m_iSplashCountAll = 0;
	m_iCurrentTip = 0;
	m_bInited = false;
}

//-----------------------------------------------------------------------------
// Purpose: Initializer
//-----------------------------------------------------------------------------
bool CFRTips::Init()
{
	if ( !m_bInited )
	{
		// count how many tips there are
		m_iTipCountAll = 0;
		wchar_t *wzTipCount = g_pVGuiLocalize->Find(CFmtStr("#FR_Tip_Count"));
		int iTipCount = wzTipCount ? _wtoi(wzTipCount) : 0;
		m_iTipCountAll += iTipCount;

		// count how many splashes there are
		m_iSplashCountAll = 0;
		wchar_t* wzSplashCount = g_pVGuiLocalize->Find(CFmtStr("#FR_Splash_Count"));
		int iSplashCount = wzSplashCount ? _wtoi(wzSplashCount) : 0;
		m_iSplashCountAll += iSplashCount;

		m_bInited = true;
	}

	return m_bInited;
}

//-----------------------------------------------------------------------------
// Purpose: Returns a random tip
//-----------------------------------------------------------------------------
const wchar_t *CFRTips::GetRandomTip()
{
	Init();

	//we choose if we want splashes or tips.
	int randChoice = RandomInt(0, 1);
	int iTip = 0;

	switch (randChoice)
	{
		case 1:
			// pick a random spalsh
			iTip = RandomInt(0, m_iSplashCountAll - 1);
			return GetLocalizedString(iTip + 1, CFmtStr("#FR_Splash_%i", iTip));
			break;
		case 0:
		default:
			// pick a random tip
			iTip = RandomInt(0, m_iTipCountAll - 1);
			return GetLocalizedString(iTip + 1, CFmtStr("#FR_Tip_%i", iTip));
			break;
	}

	return NULL;
}

//-----------------------------------------------------------------------------
// Purpose: Returns specified tip index from string
//-----------------------------------------------------------------------------
const wchar_t *CFRTips::GetLocalizedString(int iTip, const char* token)
{
	static wchar_t wzTip[512] = L"";

	// get the tip
	const wchar_t *wzFmt = g_pVGuiLocalize->Find( token );
	// replace any commands with their bound keys
	UTIL_ReplaceKeyBindings( wzFmt, 0, wzTip, sizeof( wzTip ) );

	return wzTip;
}

// global instance
CFRTips *g_FRTips;