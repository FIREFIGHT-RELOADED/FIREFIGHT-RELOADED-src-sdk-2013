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
#define	KATANA_POSTKILLHEALTHBONUSDELAY	5.0f
#define	KATANA_MAXKILLHEALTHBONUS	5
#define	KATANA_MAXGIVEHEALTH	10

//-----------------------------------------------------------------------------
// CWeaponKatana
//-----------------------------------------------------------------------------

class CWeaponKatana : public CBaseHLBludgeonWeapon
{
public:
	DECLARE_CLASS( CWeaponKatana, CBaseHLBludgeonWeapon );

	DECLARE_SERVERCLASS();
	DECLARE_ACTTABLE();
	DECLARE_DATADESC();

	CWeaponKatana();

	float		GetRange( void )		{	return	KATANA_RANGE;	}
	float		GetFireRate( void )		{	return	KATANA_REFIRE;	}

	void		AddViewKick( void );

	void		PrimaryAttack(void);
	void		SecondaryAttack( void )	{	return;	}
	bool		Holster(CBaseCombatWeapon* pSwitchingTo);
	void		ItemPostFrame(void);
private:
	int			m_iKillMultiplier;
	int			m_iKills;
	float		m_flLastKill;
	bool		m_bKillMultiplier;
};

#endif // WEAPON_KATANA_H
