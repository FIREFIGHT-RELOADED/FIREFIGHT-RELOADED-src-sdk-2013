//====== Copyright © 1996-2005, Valve Corporation, All rights reserved. =======
//
// Purpose: Tips
//
//=============================================================================

#ifndef FR_TIPS_H
#define FR_TIPS_H
#ifdef _WIN32
#pragma once
#endif

#include "igamesystem.h"

//-----------------------------------------------------------------------------
// Purpose: helper class for FR tips
//-----------------------------------------------------------------------------
class CFRTips : public CAutoGameSystem
{
public:
	CFRTips();

	virtual bool Init();
	virtual char const *Name() { return "CFRTips"; }

	const wchar_t *GetRandomTip();
private:
	const wchar_t *GetLocalizedString( int iTip, const char *token );

	int m_iTipCountAll;
	int m_iSplashCountAll;
	int m_iCurrentTip;
	bool m_bInited;
};

extern CFRTips *g_FRTips;

inline CFRTips *FRTips()
{
	return g_FRTips;
}
#endif
