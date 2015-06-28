//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: Rich Presence support
//
//=====================================================================================//

#include "cbase.h"
#include "fr_tips.h"
#include "tier3/tier3.h"
#include <vgui/ILocalize.h>
#include "cdll_util.h"
#include "fmtstr.h"

//-----------------------------------------------------------------------------
// Purpose: constructor
//-----------------------------------------------------------------------------
CFRTips::CFRTips() : CAutoGameSystem("CFRTips")
{
	m_iTipCountAll = 0;
	m_iCurrentClassTip = 0;
	m_bInited = false;
}

//-----------------------------------------------------------------------------
// Purpose: Initializer
//-----------------------------------------------------------------------------
bool CFRTips::Init()
{
	if ( !m_bInited )
	{
		// count how many tips there are for each class and in total
		m_iTipCountAll = 0;
		wchar_t *wzTipCount = g_pVGuiLocalize->Find(CFmtStr("FR_Tip_Count"));
		int iTipCount = wzTipCount ? _wtoi(wzTipCount) : 0;
		m_iTipCountAll += iTipCount;
		m_bInited = true;
	}

	return true;
}

//-----------------------------------------------------------------------------
// Purpose: Returns a random tip, selected from tips for all classes
//-----------------------------------------------------------------------------
const wchar_t *CFRTips::GetRandomTip()
{
	Init();

	// pick a random tip
	int iTip = RandomInt( 0, m_iTipCountAll-1 );
	return GetTip(iTip + 1);
}

//-----------------------------------------------------------------------------
// Purpose: Returns specified tip index for specified class
//-----------------------------------------------------------------------------
const wchar_t *CFRTips::GetTip(int iTip)
{
	static wchar_t wzTip[512] = L"";

	// get the tip
	const wchar_t *wzFmt = g_pVGuiLocalize->Find( CFmtStr( "#FR_Tip_%d", iTip ) );
	// replace any commands with their bound keys
	UTIL_ReplaceKeyBindings( wzFmt, 0, wzTip, sizeof( wzTip ) );

	return wzTip;
}

// global instance
CFRTips g_FRTips;