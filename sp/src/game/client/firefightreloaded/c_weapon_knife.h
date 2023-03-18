#ifndef C_WEAPON_KNIFE_H
#define C_WEAPON_KNIFE_H

#include "c_basehlcombatweapon.h"

class C_WeaponKnife : public C_BaseHLBludgeonWeapon
{
	DECLARE_CLASS( C_WeaponKnife, C_BaseHLBludgeonWeapon );

public:
	DECLARE_CLIENTCLASS();
	DECLARE_PREDICTABLE();

	C_WeaponKnife( void ) { };

	EHANDLE m_hStuckRagdoll;

private:

	C_WeaponKnife( const C_WeaponKnife & ); // not defined, not accessible
};

#endif