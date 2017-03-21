//========= Copyright Valve Corporation, All rights reserved. ============//
#ifndef WEAPON_BASEFRCOMBATWEAPON_SHARED_H
#define WEAPON_BASEFRCOMBATWEAPON_SHARED_H
#ifdef _WIN32
#pragma once
#endif

#include "hl2_player_shared.h"
#include "weapon_frbase.h"

#if defined( CLIENT_DLL )
#define CBaseFRCombatWeapon C_BaseFRCombatWeapon
#endif

class CBaseFRCombatWeapon : public CWeaponFRBase
{
#if !defined( CLIENT_DLL )
	DECLARE_DATADESC();
#endif

	DECLARE_CLASS( CBaseFRCombatWeapon, CWeaponFRBase );
public:
	DECLARE_NETWORKCLASS();
	DECLARE_PREDICTABLE();

	CBaseFRCombatWeapon();

	virtual bool	WeaponShouldBeLowered( void );

	virtual bool	Ready( void );
	virtual bool	Lower( void );
	virtual bool	Deploy( void );
	virtual bool	Holster( CBaseCombatWeapon *pSwitchingTo );
	virtual void	WeaponIdle( void );

	virtual void	AddViewmodelBob( CBaseViewModel *viewmodel, Vector &origin, QAngle &angles );
	virtual	float	CalcViewmodelBob( void );

	virtual Vector	GetBulletSpread( WeaponProficiency_t proficiency );
	virtual float	GetSpreadBias( WeaponProficiency_t proficiency );

	virtual const	WeaponProficiencyInfo_t *GetProficiencyValues();
	static const	WeaponProficiencyInfo_t *GetDefaultProficiencyValues();

	virtual void	ItemHolsterFrame( void );

protected:

	bool			m_bLowered;			// Whether the viewmodel is raised or lowered
	float			m_flRaiseTime;		// If lowered, the time we should raise the viewmodel
	float			m_flHolsterTime;	// When the weapon was holstered

private:
	
	CBaseFRCombatWeapon( const CBaseFRCombatWeapon & );
};

#endif // WEAPON_BASEFRCOMBATWEAPON_SHARED_H
