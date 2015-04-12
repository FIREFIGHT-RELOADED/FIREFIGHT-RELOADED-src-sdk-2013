//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#ifndef ITEMMAKER_FIREFIGHT_H
#define ITEMMAKER_FIREFIGHT_H
#ifdef _WIN32
#pragma once
#endif

#include "cbase.h"


//-----------------------------------------------------------------------------
// Spawnflags
//-----------------------------------------------------------------------------
#define SF_ITEMMAKER_RAREWEAPONS			1
#define SF_ITEMMAKER_RAREITEMS				8
#define SF_ITEMMAKER_ONLYWEAPONS			16
#define SF_ITEMMAKER_ONLYITEMS				32

class CItemMakerFirefight : public CBaseEntity
{
public:
	DECLARE_CLASS(CItemMakerFirefight, CBaseEntity);

	CItemMakerFirefight(void);

	void Spawn( void );
	void Precache(void);
	virtual int	ObjectCaps( void ) { return BaseClass::ObjectCaps() & ~FCAP_ACROSS_TRANSITION; }
	void MakerThink( void );

	virtual void MakeItem(void);
	virtual void MakeRareItem(void);
	virtual void MakeWeapon(void);
	virtual void MakeRareWeapon(void);

	// Input handlers
	void InputSpawnItem(inputdata_t &inputdata);
	void InputSpawnRareItem(inputdata_t &inputdata);
	void InputSpawnWeapon(inputdata_t &inputdata);
	void InputSpawnRareWeapon(inputdata_t &inputdata);
	void InputEnable( inputdata_t &inputdata );
	void InputDisable( inputdata_t &inputdata );
	void InputToggle( inputdata_t &inputdata );

	// State changers
	void Toggle( void );
	virtual void Enable( void );
	virtual void Disable( void );

	DECLARE_DATADESC();

	COutputEHANDLE m_OnSpawnItem;
	COutputEHANDLE m_OnSpawnRareItem;
	COutputEHANDLE m_OnSpawnWeapon;
	COutputEHANDLE m_OnSpawnRareWeapon;

	bool	m_bDisabled;
	bool	m_bCanSpawn;

	EHANDLE m_hIgnoreEntity;
	string_t m_iszIngoreEnt;
	
	string_t m_ChildTargetName;
};

#endif // MONSTERMAKER_H
