//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Client side antlion guard. Used to create dlight for the cave guard.
//
//=============================================================================

#include "cbase.h"
#include "c_ai_basenpc.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

class C_NPC_Player : public C_AI_BaseNPC
{
public:
	C_NPC_Player() {}

	DECLARE_CLASS( C_NPC_Player, C_AI_BaseNPC );
	DECLARE_CLIENTCLASS();
 	DECLARE_DATADESC();

	virtual void OnDataChanged( DataUpdateType_t type );
	virtual void ClientThink();
	virtual void GetGlowEffectColor(float* r, float* g, float* b);
private:

	C_NPC_Player( const C_NPC_Player& );
};

//-----------------------------------------------------------------------------
// Save/restore
//-----------------------------------------------------------------------------
BEGIN_DATADESC( C_NPC_Player )
END_DATADESC()


//-----------------------------------------------------------------------------
// Networking
//-----------------------------------------------------------------------------
IMPLEMENT_CLIENTCLASS_DT(C_NPC_Player, DT_NPC_Player, CNPC_Player)
END_RECV_TABLE()

void C_NPC_Player::GetGlowEffectColor(float* r, float* g, float* b)
{
	*r = 0.1;
	*g = 0.3;
	*b = 0.6;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void C_NPC_Player::OnDataChanged( DataUpdateType_t type )
{
	if (IsAlive())
	{
		float r, g, b;
		GetGlowEffectColor(&r, &g, &b);
		UpdateGlowEffect(Vector(r, g, b), 1.0);
	}

	BaseClass::OnDataChanged( type );
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void C_NPC_Player::ClientThink()
{
	BaseClass::ClientThink();
}
