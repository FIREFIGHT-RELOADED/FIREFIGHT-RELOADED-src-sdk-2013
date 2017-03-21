//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//=============================================================================//

#ifndef WEAPON_KNIFE_H
#define WEAPON_KNIFE_H

#include "basebludgeonweapon.h"

#if defined( _WIN32 )
#pragma once
#endif

#ifdef HL2MP
#error weapon_knife.h must not be included in hl2mp. The windows compiler will use the wrong class elsewhere if it is.
#endif

#define	KNIFE_RANGE	75.0f
#define	KNIFE_REFIRE	0.2f

//-----------------------------------------------------------------------------
// CWeaponKnife
//-----------------------------------------------------------------------------

class CWeaponKnife : public CBaseHLBludgeonWeapon
{
public:
	DECLARE_CLASS( CWeaponKnife, CBaseHLBludgeonWeapon );

	DECLARE_SERVERCLASS();
	DECLARE_ACTTABLE();

	CWeaponKnife();

	float		GetRange( void )		{	return	KNIFE_RANGE;	}
	float		GetFireRate( void )		{	return	KNIFE_REFIRE;	}

	void		AddViewKick( void );
	float		GetDamageForActivity( Activity hitActivity );

	virtual int WeaponMeleeAttack1Condition( float flDot, float flDist );
	void		SecondaryAttack( void )	{	return;	}

	// Animation event
	virtual void Operator_HandleAnimEvent( animevent_t *pEvent, CBaseCombatCharacter *pOperator );
	void			Hit(trace_t &traceHit, Activity nHitActivity, bool bIsSecondary);
	void			HandleAnimEventMeleeHit(animevent_t *pEvent, CBaseCombatCharacter *pOperator);

protected:
	virtual	void	ImpactEffect(trace_t &trace);
};

#endif // WEAPON_KNIFE_H
