//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: Implements the grapple hook weapon.
//			
//			Primary attack: fires a beam that hooks on a surface.
//			Secondary attack: switches between pull and rapple modes
//
//
//=============================================================================//
#ifndef WEAPON_GRAPPLE_H
#define WEAPON_GRAPPLE_H
 
#ifdef _WIN32
#pragma once
#endif
       
#include "Sprite.h"                            
 
#include "te_effect_dispatch.h"
#include "basehlcombatweapon.h"
#include "rope.h"
#include "props.h"
 
#include "rope_shared.h"
#include "beam_shared.h"

#define HOOK_MODEL			"models/props_junk/rock001a.mdl"

#define BOLT_AIR_VELOCITY	3500
#define BOLT_WATER_VELOCITY	1500

class CWeaponGrapple;
 
//-----------------------------------------------------------------------------
// Grapple Hook
//-----------------------------------------------------------------------------
class CGrappleHook : public CBaseCombatCharacter
{
	DECLARE_CLASS( CGrappleHook, CBaseCombatCharacter );

public:
	CGrappleHook() { };
	~CGrappleHook();
 
	Class_T Classify( void ) { return CLASS_NONE; }
	void Spawn( void );
	void Precache( void );
	void FlyThink( void );
	void HookedThink( void );
	void HookTouch( CBaseEntity *pOther );
	bool CreateVPhysics( void );
	unsigned int PhysicsSolidMaskForEntity() const;
	static CGrappleHook *HookCreate( const Vector &vecOrigin, const QAngle &angAngles, CBaseEntity *pentOwner = NULL );

protected:
	DECLARE_DATADESC();

private:

	CHandle<CWeaponGrapple>		m_hOwner;
	CHandle<CBasePlayer>		m_hPlayer;
	CHandle<CDynamicProp>		m_hBolt;
	bool						m_bPlayerWasStanding;
};
     
class CWeaponGrapple : public CBaseHLCombatWeapon            
{                                                            
	DECLARE_CLASS( CWeaponGrapple, CBaseHLCombatWeapon );

protected:
	DECLARE_SERVERCLASS();
	DECLARE_ACTTABLE();
	DECLARE_DATADESC();

public:
	CWeaponGrapple( void );

	virtual void	Precache( void );
	virtual void	PrimaryAttack( void );
	virtual bool	Deploy(void);
	virtual bool CanDeploy(void);
	bool			CanHolster( void );
	virtual bool	Holster( CBaseCombatWeapon *pSwitchingTo = NULL );
	void			Drop( const Vector &vecVelocity );
	virtual void	ItemPostFrame( void );
 
	void			NotifyHookDied( void );
 
	bool			HasAnyAmmo( void );
 
	CBaseEntity		*GetHook( void ) { return m_hHook; }

	void   	DrawBeam( const Vector &startPos, const Vector &endPos, float width );
	void	DoImpactEffect( trace_t &tr, int nDamageType );

public:
	bool                        m_bHook;
 
private:
	void	FireHook(const Vector& endPos);
 
private: 
	CHandle<CBeam>			pBeam;
	CHandle<CSprite>		m_pLightGlow;
	CHandle<CBaseEntity>	m_hHook;
	int						m_nBulletType;
};
#endif // WEAPON_GRAPPLE_H