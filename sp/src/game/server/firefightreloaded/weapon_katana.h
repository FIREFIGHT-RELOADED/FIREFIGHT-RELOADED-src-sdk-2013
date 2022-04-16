//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//=============================================================================//

#ifndef WEAPON_KATANA_H
#define WEAPON_KATANA_H

#include "basebludgeonweapon.h"

#if defined( _WIN32 )
#pragma once
#endif

#ifdef HL2MP
#error weapon_katana.h must not be included in hl2mp. The windows compiler will use the wrong class elsewhere if it is.
#endif

#define	KATANA_RANGE	90.0f
#define	KATANA_REFIRE	0.45f

//-----------------------------------------------------------------------------
// CWeaponKatana
//-----------------------------------------------------------------------------

class CWeaponKatana : public CBaseHLBludgeonWeapon
{
public:
	DECLARE_CLASS( CWeaponKatana, CBaseHLBludgeonWeapon );

	DECLARE_SERVERCLASS();
	DECLARE_ACTTABLE();

	CWeaponKatana();

	float		GetRange( void )		{	return	KATANA_RANGE;	}
	float		GetFireRate( void )		{	return	KATANA_REFIRE;	}

	void		AddViewKick( void );

	void		PrimaryAttack(void);
	void		SecondaryAttack( void )	{	return;	}
};

#endif // WEAPON_KATANA_H
