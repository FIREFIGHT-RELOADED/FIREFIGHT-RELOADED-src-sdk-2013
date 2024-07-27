//========= Copyright Valve Corporation, All rights reserved. ============//
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
//=============================================================================//

#ifndef	WEAPONAR2_H
#define	WEAPONAR2_H

#include "basegrenade_shared.h"
#include "basehlcombatweapon.h"

class CWeaponAR2 : public CHLMachineGun
{
public:
	DECLARE_CLASS( CWeaponAR2, CHLMachineGun );

	CWeaponAR2();

	DECLARE_SERVERCLASS();

	void	PrimaryAttack(void);
	void	ItemPostFrame( void );
	void	Precache( void );
	void	BallAttack(void);
	void	DelayedAttack( void );

	const char *GetTracerType( void ) { return "AR2Tracer"; }

	void	AddViewKick( void );

	void	FireNPCPrimaryAttack( CBaseCombatCharacter *pOperator, bool bUseWeaponAngles );
	void	FireNPCSecondaryAttack( CBaseCombatCharacter *pOperator, bool bUseWeaponAngles );
	void	Operator_ForceNPCFire( CBaseCombatCharacter  *pOperator, bool bSecondary );
	void	Operator_HandleAnimEvent( animevent_t *pEvent, CBaseCombatCharacter *pOperator );

	int		GetMinBurst( void ) { return 2; }
	int		GetMaxBurst( void ) { return 5; }
	float	GetFireRate(void);

	bool	CanHolster( void );
	bool	Holster(CBaseCombatWeapon* pSwitchingTo = NULL);
	bool	Reload( void );

	int		CapabilitiesGet( void ) { return bits_CAP_WEAPON_RANGE_ATTACK1; }

	Activity	GetPrimaryAttackActivity( void );
	
	void	DoImpactEffect( trace_t &tr, int nDamageType );

	virtual const Vector& GetBulletSpread(void)
	{
		static Vector cone;

		if (GetOwner() && GetOwner()->IsPlayer())
		{
			if (IsIronsighted())
			{
				cone = VECTOR_CONE_2DEGREES;
			}
            else if (m_bZoomed)
			{
				cone = VECTOR_CONE_1DEGREES;
			}
		}
		else
		{
			cone = VECTOR_CONE_3DEGREES;
		}

		return cone;
	}

	virtual bool	Deploy(void);
	virtual void	Drop(const Vector& velocity);

	const WeaponProficiencyInfo_t *GetProficiencyValues();

protected:
	void				Zoom(void);

	bool				m_bZoomed;

	int					m_iFireMode;

	float					m_flDelayedFire;
	bool					m_bShotDelayed;
	int						m_nVentPose;
	
	DECLARE_ACTTABLE();
	DECLARE_DATADESC();
};


#endif	//WEAPONAR2_H
