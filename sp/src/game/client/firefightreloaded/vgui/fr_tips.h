//====== Copyright © 1996-2005, Valve Corporation, All rights reserved. =======
//
// Purpose: TF implementation of the IPresence interface
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
	const wchar_t *GetTip( int iTip );

	int m_iTipCountAll;								// how many tips there are total
	int m_iCurrentClassTip;							// index of current per-class tip
	bool m_bInited;									// have we been initialized
};

extern CFRTips g_FRTips;
#endif // TF_TIPS_H
