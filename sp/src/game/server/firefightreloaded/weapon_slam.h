//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:		SLAM 
//
// $Workfile:     $
// $Date:         $
//
//-----------------------------------------------------------------------------
// $Log: $
//
// $NoKeywords: $
//=============================================================================//

#ifndef	WEAPONSLAM_H
#define	WEAPONSLAM_H

#include "basegrenade_shared.h"
#include "basehlcombatweapon.h"


enum 
{
	SLAM_TRIPMINE_READY,
	SLAM_SATCHEL_THROW,
	SLAM_SATCHEL_ATTACH,
};

class CWeapon_SLAM : public CBaseHLCombatWeapon
{
public:
	DECLARE_CLASS( CWeapon_SLAM, CBaseHLCombatWeapon );

	DECLARE_SERVERCLASS();

	int 	m_tSlamState;
	bool				m_bDetonatorArmed;
	bool				m_bNeedDetonatorDraw;
	bool				m_bNeedDetonatorHolster;
	bool				m_bNeedReload;
	bool				m_bClearReload;
	bool				m_bThrowSatchel;
	bool				m_bAttachSatchel;
	bool				m_bAttachTripmine;
	float				m_flWallSwitchTime;

	void				Spawn( void );
	void				Precache( void );

	void				PrimaryAttack( void );
	void				SecondaryAttack( void );
	void				WeaponIdle( void );
	void				Weapon_Switch( void );
	bool				CanBeSelected(void);
	void				SLAMThink( void );
	
	void				SetPickupTouch( void );
	void				SlamTouch( CBaseEntity *pOther );	// default weapon touch
	void				ItemPostFrame( void );	
	bool				Reload( void );
	void				SetSlamState( int newState );
	bool				CanAttachSLAM(void);		// In position where can attach SLAM?
	bool				AnyUndetonatedCharges(void);
	void				StartTripmineAttach( void );
	void				TripmineAttach( void );

	void				StartSatchelDetonate( void );
	void				SatchelDetonate( void );
	void				StartSatchelThrow( void );
	void				StartSatchelAttach( void );
	void				SatchelThrow( void );
	void				SatchelAttach( void );
	bool				Deploy( void );
	bool				Holster( CBaseCombatWeapon *pSwitchingTo = NULL );


	CWeapon_SLAM();

	DECLARE_ACTTABLE();
	DECLARE_DATADESC();

//private:
//	CWeapon_SLAM( const CWeapon_SLAM & );
};


#endif	//WEAPONSLAM_H
