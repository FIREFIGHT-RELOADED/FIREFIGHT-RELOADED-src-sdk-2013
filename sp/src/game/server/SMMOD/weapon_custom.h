
#ifndef	WEAPONCUSTOM_H
#define	WEAPONCUSTOM_H

#include "basehlcombatweapon.h"
#include "weapon_rpg.h"

class CWeaponCustom : public CHLSelectFireMachineGun
{
	DECLARE_DATADESC();
public:
	DECLARE_CLASS( CWeaponCustom, CHLSelectFireMachineGun );

	CWeaponCustom();

	DECLARE_SERVERCLASS();

	DECLARE_ACTTABLE();
	
	void	Precache( void );
	void	AddViewKick(float easyDampen, float degrees, float seconds);
	void	ShootBullets( bool isPrimary = true, bool usePrimaryAmmo = true );
	void	ShootBulletsRight(bool isPrimary = true, bool usePrimaryAmmo = true);
	void	ShootBulletsLeft(bool isPrimary = true, bool usePrimaryAmmo = true);

	void	ShootProjectile( bool isPrimary, bool usePrimaryAmmo );
	void	ShootProjectileRight(bool isPrimary, bool usePrimaryAmmo);
	void	ShootProjectileLeft(bool isPrimary, bool usePrimaryAmmo);

	void	ShootAR2EnergyBall(bool isPrimary, bool usePrimaryAmmo);
	void	ShootAR2EnergyBallRight(bool isPrimary, bool usePrimaryAmmo);
	void	ShootAR2EnergyBallLeft(bool isPrimary, bool usePrimaryAmmo);

	void	ShootSMGGrenade(bool isPrimary, bool usePrimaryAmmo);
	void	ShootSMGGrenadeRight(bool isPrimary, bool usePrimaryAmmo);
	void	ShootSMGGrenadeLeft(bool isPrimary, bool usePrimaryAmmo);



	//void	ShootFragGrenadeThrow(bool isPrimary, bool usePrimaryAmmo);
	//void	ShootFragGrenadeRoll(bool isPrimary, bool usePrimaryAmmo);
	void	ItemPostFrame( void );
	void	ItemBusyFrame(void);
	void	PrimaryAttack( void );
	void	SecondaryAttack( void );

	int		GetMinBurst() { return 2; }
	int		GetMaxBurst() { return 5; }

	virtual void Equip( CBaseCombatCharacter *pOwner );
	bool	Reload( void );
	void	Drop(const Vector &vecVelocity);
	bool	Holster(CBaseCombatWeapon *pSwitchingTo = NULL);

	//void	ApplyCustomization(void);

	float	GetPrimaryFireRate( void ) 
	{ 
		if (IsIronsighted())
		{
			return this->GetWpnData().m_sPrimaryIronsightFireRate;
		}
		else if (m_bInZoom)
		{
			return this->GetWpnData().m_sPrimaryZoomFireRate;
		}
		else
		{
			return this->GetWpnData().m_sPrimaryFireRate;
		} 
	}

	float	GetSecondaryFireRate(void)
	{
		if (IsIronsighted())
		{
			return this->GetWpnData().m_sSecondaryIronsightFireRate;
		}
		else if (m_bInZoom)
		{
			return this->GetWpnData().m_sSecondaryZoomFireRate;
		}
		else
		{
			return this->GetWpnData().m_sSecondaryFireRate;
		}
	}

	int		CapabilitiesGet(void);
	int		WeaponRangeAttack2Condition( float flDot, float flDist );
	virtual	int		WeaponMeleeAttack1Condition(float flDot, float flDist);
	Activity	GetPrimaryAttackActivity( void );
	Activity	GetLeftGunActivity(void);
	Activity	GetRightGunActivity(void);

	virtual const Vector& GetBulletSpreadPrimary( void )
	{
		static const Vector cone = this->GetWpnData().m_vPrimarySpread;
		static const Vector ironsightCone = this->GetWpnData().m_vPrimaryIronsightSpread;
		static const Vector zoomCone = this->GetWpnData().m_vPrimaryZoomSpread;
		if (IsIronsighted())
		{
			return ironsightCone;
		}
		else if (m_bInZoom)
		{
			return zoomCone;
		}
		else
		{
			return cone;
		}
	}

	virtual const Vector& GetBulletSpreadSecondary( void )
	{
		static const Vector cone = this->GetWpnData().m_vSecondarySpread;
		static const Vector ironsightCone = this->GetWpnData().m_vSecondaryIronsightSpread;
		static const Vector zoomCone = this->GetWpnData().m_vSecondaryZoomSpread;
		if (IsIronsighted())
		{
			return ironsightCone;
		}
		else if (m_bInZoom)
		{
			return zoomCone;
		}
		else
		{
			return cone;
		}
	}
	bool IsPrimaryBullet( void ) {return this->GetWpnData().m_sPrimaryBulletEnabled;}
	bool IsSecondaryBullet( void ) {return this->GetWpnData().m_sSecondaryBulletEnabled;}

	const WeaponProficiencyInfo_t *GetProficiencyValues();

	//void FireNPCPrimaryAttack( CBaseCombatCharacter *pOperator, Vector &vecShootOrigin, Vector &vecShootDir );
	//void Operator_ForceNPCFire( CBaseCombatCharacter  *pOperator, bool bSecondary );
	//void Operator_HandleAnimEvent( animevent_t *pEvent, CBaseCombatCharacter *pOperator );

protected:
		virtual	void	ImpactEffect(trace_t &trace);
protected: //Why did I not put this in? I have no idea...
		CHandle<CMissile>	m_hMissile;
		CHandle<CMissile>	m_hMissile2;
private:
	void	CheckZoomToggle(void);
	void	ToggleZoom(void);
	bool			ImpactWater(const Vector &start, const Vector &end);
	void			Swing(int bIsSecondary);
	void			Hit(trace_t &traceHit, Activity nHitActivity, bool bIsSecondary);
	Activity		ChooseIntersectionPointAndActivity(trace_t &hitTrace, const Vector &mins, const Vector &maxs, CBasePlayer *pOwner);

private:
	bool				m_bInZoom;
	bool				bFlip;
};

#define CustomWeaponAdd( num )										\
class CWeaponCustom##num : public CWeaponCustom						\
{																	\
	DECLARE_DATADESC();												\
	public:															\
	DECLARE_CLASS( CWeaponCustom##num, CWeaponCustom );				\
	CWeaponCustom##num() {};										\
	DECLARE_SERVERCLASS();											\
};																	\
IMPLEMENT_SERVERCLASS_ST(CWeaponCustom##num, DT_WeaponCustom##num)	\
END_SEND_TABLE()													\
BEGIN_DATADESC( CWeaponCustom##num )										\
END_DATADESC()														\
LINK_ENTITY_TO_CLASS( weapon_custom##num, CWeaponCustom##num );		\
PRECACHE_WEAPON_REGISTER(weapon_custom##num);

#define CustomWeaponNamedAdd( customname )										\
class CWeaponCustomNamed##customname : public CWeaponCustom						\
{																	\
	DECLARE_DATADESC();												\
	public:															\
	DECLARE_CLASS( CWeaponCustomNamed##customname, CWeaponCustom );				\
	CWeaponCustomNamed##customname() {};										\
	DECLARE_SERVERCLASS();											\
};																	\
IMPLEMENT_SERVERCLASS_ST(CWeaponCustomNamed##customname, DT_WeaponCustomNamed##customname)	\
END_SEND_TABLE()													\
BEGIN_DATADESC( CWeaponCustomNamed##customname )										\
END_DATADESC()														\
LINK_ENTITY_TO_CLASS( weapon_custom_##customname, CWeaponCustomNamed##customname );		\
PRECACHE_WEAPON_REGISTER(weapon_custom_##customname);
#endif	//WEAPONCUSTOM_H