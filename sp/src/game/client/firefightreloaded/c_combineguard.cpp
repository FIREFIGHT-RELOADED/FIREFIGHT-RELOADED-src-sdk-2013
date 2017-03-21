//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//
#include "cbase.h"
#include "c_ai_basenpc.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class C_CombineGuard : public C_AI_BaseNPC
{
public:
	DECLARE_CLASS( C_CombineGuard, C_AI_BaseNPC );
	DECLARE_CLIENTCLASS();

					C_CombineGuard();
	virtual			~C_CombineGuard();

private:
	C_CombineGuard( const C_CombineGuard & ); // not defined, not accessible
};

IMPLEMENT_CLIENTCLASS_DT(C_CombineGuard, DT_NPC_CombineGuard, CNPC_CombineGuard)
END_RECV_TABLE()

C_CombineGuard::C_CombineGuard()
{
}


C_CombineGuard::~C_CombineGuard()
{
}


