//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: Handling for the armor.
//
// $NoKeywords: $
//=============================================================================//

#include "cbase.h"
#include "hl2_player.h"
#include "basecombatweapon.h"
#include "gamerules.h"
#include "items.h"
#include "engine/IEngineSound.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

class CItemLightArmor : public CItem
{
public:
	DECLARE_CLASS(CItemLightArmor, CItem);

	void Spawn( void )
	{ 
		Precache( );
		SetModel( "models/items/hevsuit_lightarmor.mdl" );
		BaseClass::Spawn( );
	}
	void Precache( void )
	{
		PrecacheModel ("models/items/hevsuit_lightarmor.mdl");

		PrecacheScriptSound( "ItemArmor.Touch" );

	}
	bool MyTouch( CBasePlayer *pPlayer )
	{
		CHL2_Player *pHL2Player = dynamic_cast<CHL2_Player *>( pPlayer );
		return (pHL2Player && pHL2Player->ApplyLightArmor());
	}
};

LINK_ENTITY_TO_CLASS(item_lightarmor, CItemLightArmor);
PRECACHE_REGISTER(item_lightarmor);

class CItemHeavyArmor : public CItem
{
public:
	DECLARE_CLASS(CItemHeavyArmor, CItem);

	void Spawn(void)
	{
		Precache();
		SetModel("models/items/hevsuit_heavyarmor.mdl");
		BaseClass::Spawn();
	}
	void Precache(void)
	{
		PrecacheModel("models/items/hevsuit_heavyarmor.mdl");

		PrecacheScriptSound("ItemArmor.Touch");

	}
	bool MyTouch(CBasePlayer *pPlayer)
	{
		CHL2_Player *pHL2Player = dynamic_cast<CHL2_Player *>(pPlayer);
		return (pHL2Player && pHL2Player->ApplyHeavyArmor());
	}
};

LINK_ENTITY_TO_CLASS(item_heavyarmor, CItemHeavyArmor);
PRECACHE_REGISTER(item_heavyarmor);

class CItemShield : public CItem
{
public:
	DECLARE_CLASS(CItemShield, CItem);

	void Spawn(void)
	{
		Precache();
		SetModel("models/armor/shield.mdl");
		AddSpawnFlags(SF_NORESPAWN);
		BaseClass::Spawn();
	}
	void Precache(void)
	{
		PrecacheModel("models/armor/shield.mdl");
		PrecacheScriptSound("ItemArmor.Touch");

	}
	bool MyTouch(CBasePlayer *pPlayer)
	{
		CHL2_Player *pHL2Player = dynamic_cast<CHL2_Player *>(pPlayer);
		return (pHL2Player && pHL2Player->ApplyShield());
	}
};

LINK_ENTITY_TO_CLASS(item_shield, CItemShield);
PRECACHE_REGISTER(item_shield);

