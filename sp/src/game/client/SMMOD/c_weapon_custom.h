#include "hl2/c_basehlcombatweapon.h"

#ifndef	C_WEAPONCUSTOM_H
#define	C_WEAPONCUSTOM_H
#ifdef _WIN32
#pragma once
#endif

class C_WeaponCustom : public C_HLSelectFireMachineGun			
{														
	DECLARE_CLASS( C_WeaponCustom, C_HLSelectFireMachineGun );					
public:													
	//DECLARE_PREDICTABLE();									
	DECLARE_CLIENTCLASS();									
	C_WeaponCustom() {
	};	
private:												
//	C_WeaponCustom( const C_WeaponCustom & );	
};				

#endif	//WEAPONCUSTOM_H