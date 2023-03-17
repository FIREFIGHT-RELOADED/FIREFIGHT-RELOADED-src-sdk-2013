#include "cbase.h"
#include "c_weapon_knife.h"
#include "c_weapon__stubs.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

STUB_WEAPON_CLASS_IMPLEMENT( weapon_knife, C_WeaponKnife );

IMPLEMENT_CLIENTCLASS_DT( C_WeaponKnife, DT_WeaponKnife, CWeaponKnife )
RecvPropEHandle( RECVINFO( m_hStuckRagdoll ) ),
END_RECV_TABLE()