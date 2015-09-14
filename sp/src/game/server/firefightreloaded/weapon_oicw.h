//=========== (C) Copyright 1999 Valve, L.L.C. All rights reserved. ===========
//
// The copyright to the contents herein is the property of Valve, L.L.C.
// The contents may be used and/or copied only with the written permission of
// Valve, L.L.C., or in accordance with the terms and conditions stipulated in
// the agreement/contract under which the contents have been supplied.
//
// Purpose:		Projectile shot from the AR2 
//
// $Workfile:     $
// $Date:         $
//
//-----------------------------------------------------------------------------
// $Log: $
//
// $NoKeywords: $
//=============================================================================

#ifndef	WEAPONOICW_H
#define	WEAPONOICW_H

#include "basegrenade_shared.h"
#include "basehlcombatweapon.h"

class CGrenadeAR2;

class CWeaponOICW : public CHLMachineGun
{
public:
	DECLARE_CLASS(CWeaponOICW, CHLMachineGun);

	CWeaponOICW();

	DECLARE_SERVERCLASS();

	bool	Holster(CBaseCombatWeapon *pSwitchingTo = NULL);
	bool	Reload(void);
	float	GetFireRate(void);
	void	ItemPostFrame(void);
	void	Precache(void);
	void	PrimaryAttack(void);
	void	GrenadeAttack(void);
	void	AddViewKick(void);
	void	Operator_HandleAnimEvent(animevent_t *pEvent, CBaseCombatCharacter *pOperator);

	int		GetMinBurst() { return 4; }
	int		GetMaxBurst() { return 7; }

	int		CapabilitiesGet(void) { return bits_CAP_WEAPON_RANGE_ATTACK1; }

	Activity	GetPrimaryAttackActivity(void);

	virtual const Vector& GetBulletSpread(void)
	{
		static Vector cone;

		if (GetOwner() && GetOwner()->IsPlayer())
		{
			cone = (m_bZoomed) ? VECTOR_CONE_1DEGREES : VECTOR_CONE_3DEGREES;
		}
		else
		{
			cone = VECTOR_CONE_8DEGREES;
		}

		return cone;
	}

	virtual void	Equip(CBaseCombatCharacter *pOwner);
	virtual bool	Deploy(void);
	virtual void	Drop(const Vector &velocity);

protected:

	void			Zoom(void);

	int				m_nShotsFired;

	bool			m_bZoomed;

	float	m_flSoonestPrimaryAttack;

	static const char *pShootSounds[];

	DECLARE_ACTTABLE();
	DECLARE_DATADESC();
};


#endif	//WEAPONOICW_H