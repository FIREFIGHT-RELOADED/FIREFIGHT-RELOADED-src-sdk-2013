//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#ifndef GRENADE_HOPWIRE_H
#define GRENADE_HOPWIRE_H
#ifdef _WIN32
#pragma once
#endif

#include "basegrenade_shared.h"
#include "Sprite.h"

extern ConVar hopwire_trap;

class CGravityVortexController : public CBaseEntity
{
	DECLARE_CLASS(CGravityVortexController, CBaseEntity);
	DECLARE_DATADESC();

public:

	CGravityVortexController(void) : m_flEndTime(0.0f), m_flRadius(256), m_flStrength(256), m_flMass(0.0f) {}
	float	GetConsumedMass(void) const;

	static CGravityVortexController* Create(const Vector& origin, float radius, float strength, float duration, bool isTeleporter = false);

private:

	void	ConsumeEntity(CBaseEntity* pEnt);
	void	PullPlayersInRange(void);
	bool	KillNPCInRange(CBaseEntity* pVictim, IPhysicsObject** pPhysObj);
	void	CreateDenseBall(void);
	void	PullThink(void);
	void	StartPull(const Vector& origin, float radius, float strength, float duration);

public:

	bool m_bIsBossTeleporter;

private:

	float	m_flMass;		// Mass consumed by the vortex
	float	m_flEndTime;	// Time when the vortex will stop functioning
	float	m_flRadius;		// Area of effect for the vortex
	float	m_flStrength;	// Pulling strength of the vortex
};

class CGrenadeHopwire : public CBaseGrenade
{
	DECLARE_CLASS( CGrenadeHopwire, CBaseGrenade );
	DECLARE_DATADESC();
	DECLARE_SERVERCLASS();

public:
	void	Spawn( void );
	void	Precache( void );
	bool	CreateVPhysics( void );
	void	SetTimer( float timer );
	void	SetVelocity( const Vector &velocity, const AngularImpulse &angVelocity );
	void	Detonate( void );
	
	void	EndThink( void );		// Last think before going away
	void	CombatThink( void );	// Makes the main explosion go off

protected:

	void	KillStriders( void );

	CHandle<CGravityVortexController>	m_hVortexController;

public:
	bool m_bIsBossTeleporter;
};

extern CBaseGrenade *HopWire_Create( const Vector &position, const QAngle &angles, const Vector &velocity, const AngularImpulse &angVelocity, CBaseEntity *pOwner, float timer, bool isTeleporter = false );
extern CBaseGrenade* HopWire_Create_Simple(const Vector& position, const QAngle& angles, CBaseEntity* pOwner, float timer, bool isTeleporter = false);

#endif // GRENADE_HOPWIRE_H
