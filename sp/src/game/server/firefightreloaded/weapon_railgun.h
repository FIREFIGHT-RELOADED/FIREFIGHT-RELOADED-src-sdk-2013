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
#define RAIL_DECHARGE_TIME 0.055f
#define RAIL_RECHARGE_SUIT_TIME 0.2f
#define RAIL_RECHARGE_RECOVERY_TIME 1.0f
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

	void	Precache(void);
	int		CapabilitiesGet(void) { return bits_CAP_WEAPON_RANGE_ATTACK1; }
	void	FireNPCPrimaryAttack(CBaseCombatCharacter* pOperator, bool bUseWeaponAngles);
	void	Operator_ForceNPCFire(CBaseCombatCharacter* pOperator, bool bSecondary);
	void	Operator_HandleAnimEvent(animevent_t* pEvent, CBaseCombatCharacter* pOperator);
	bool	Deploy(void);
	bool	Holster(CBaseCombatWeapon* pSwitchingTo);
	void	ItemPostFrame(void);
	void	HolsterThink(void);
	void	ItemBusyFrame(void);
	void	PrimaryAttack( void );
	void	SecondaryAttack(void);
	void	Fire(void);
	void	RechargeAmmo(bool bIsHolstered);
	void	DechargeAmmo();
	void	ChargeAmmo();
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

	const WeaponProficiencyInfo_t* GetProficiencyValues();

protected:
	DECLARE_DATADESC();

private:
	void	CheckZoomToggle(void);
	void	ToggleZoom(void);

private:
	CBeam* m_pBeam;
	float m_flNextCharge;
	float m_flNextDecharge;
	float m_flNextSuitCharge;
	float m_flNextWarningBeep;
	bool m_bInZoom;
	bool m_bJustOvercharged;
	bool m_bIsLowBattery;
	bool m_bOverchargeDamageBenefits;
	bool m_bPlayedDechargingSound;
	bool m_bPlayedChargingSound;
	bool m_bFirstEquip;
	bool m_bJustRecovered;
	
	CWeaponRailgun( const CWeaponRailgun & );
};
#endif
