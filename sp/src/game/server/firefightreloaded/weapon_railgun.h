//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:		railgun access 
//
// $Workfile:     $
// $Date:         $
//
//-----------------------------------------------------------------------------
// $Log: $
//
// $NoKeywords: $
//=============================================================================//

#ifndef	WEAPON_RAILGUN_H
#define	WEAPON_RAILGUN_H

#include "basehlcombatweapon.h"
#include "beam_shared.h"

#define GAUSS_BEAM_SPRITE "sprites/laserbeam.vmt"
#define RAIL_RECHARGE_TIME 0.0897f
#define RAIL_RECHARGE_BACKGROUND_TIME 0.055f
#define RAIL_RECHARGE_OVERCHARGE_TIME 0.16f
#define RAIL_AMMO 25
#define RAIL_AMMO_OVERCHARGE 50

//-----------------------------------------------------------------------------
// CWeaponRailgun
//-----------------------------------------------------------------------------

class CWeaponRailgun : public CBaseHLCombatWeapon
{
	DECLARE_CLASS( CWeaponRailgun, CBaseHLCombatWeapon);
public:

	DECLARE_SERVERCLASS();
	DECLARE_ACTTABLE();

	CWeaponRailgun( void );

	void	Equip(CBaseCombatCharacter* pOwner);
	bool	Deploy(void);
	bool	Holster(CBaseCombatWeapon* pSwitchingTo);
	void	ItemPostFrame(void);
	void	HolsterThink(void);
	void	ItemBusyFrame(void);
	void	PrimaryAttack( void );
	void	SecondaryAttack(void);
	void	Fire(void);
	void	RechargeAmmo(bool bIsHolstered);
	void	DrawBeam(const Vector& startPos, const Vector& endPos);
	virtual const Vector& GetBulletSpread(void)
	{
		static Vector cone = VECTOR_CONE_1DEGREES;
		return cone;
	}
	float GetFireRate(void)
	{
		return 1.0f;
	}

	bool IsOvercharged()
	{
		return m_bJustOvercharged;
	}

protected:
	DECLARE_DATADESC();

private:
	void	CheckZoomToggle(void);
	void	ToggleZoom(void);

private:
	CBeam* m_pBeam;
	float m_flNextCharge;
	bool m_bInZoom;
	bool m_bJustOvercharged;
	bool m_bIsLowBattery;
	bool m_bOverchargeDamageBenefits;
	
	CWeaponRailgun( const CWeaponRailgun & );
};
#endif
