//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: An entity that creates NPCs in the game.
//
//=============================================================================//

#include "cbase.h"
#include "datacache/imdlcache.h"
#include "entityapi.h"
#include "entityoutput.h"
#include "ai_basenpc.h"
#include "itemmaker_firefight.h"
#include "TemplateEntities.h"
#include "ndebugoverlay.h"
#include "mapentities.h"
#include "IEffects.h"
#include "props.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

const char *g_charWeaponsCommon[] =
{
	"weapon_smg1",
	"weapon_ar2",
	"weapon_pistol",
	"weapon_shotgun",
};

const char *g_charWeaponsRare[] =
{
	"weapon_357",
	"weapon_frag",
	"weapon_crossbow",
};

const char *g_charItemsCommon[] =
{
	"item_healthkit",
	"item_healthvial",
	"item_ammo_ar2",
	"item_ammo_pistol",
	"item_ammo_smg1",
	"item_battery",
	"item_box_buckshot",
};

const char *g_charItemsRare[] =
{
	"item_ammo_357",
	"item_ammo_ar2_altfire",
	"item_ammo_crossbow",
	"item_ammo_smg1_grenade",
	"item_ammo_ar2_altfire",
};

static void DispatchActivate( CBaseEntity *pEntity )
{
	bool bAsyncAnims = mdlcache->SetAsyncLoad( MDLCACHE_ANIMBLOCK, false );
	pEntity->Activate();
	mdlcache->SetAsyncLoad( MDLCACHE_ANIMBLOCK, bAsyncAnims );
}

LINK_ENTITY_TO_CLASS(item_maker_firefight, CItemMakerFirefight);

//-------------------------------------
BEGIN_DATADESC(CItemMakerFirefight)
	DEFINE_KEYFIELD( m_bDisabled,			FIELD_BOOLEAN,	"StartDisabled" ),
	DEFINE_KEYFIELD( m_ChildTargetName, FIELD_STRING, "ItemTargetname" ),

	// Inputs
	DEFINE_INPUTFUNC( FIELD_VOID,	"SpawnItem",	InputSpawnItem ),
	DEFINE_INPUTFUNC( FIELD_VOID,	"SpawnRareItem", InputSpawnRareItem ),
	DEFINE_INPUTFUNC(FIELD_VOID,	"SpawnWeapon", InputSpawnWeapon ),
	DEFINE_INPUTFUNC(FIELD_VOID,	"SpawnRareWeapon", InputSpawnRareWeapon ),
	DEFINE_INPUTFUNC( FIELD_VOID,	"Enable",	InputEnable ),
	DEFINE_INPUTFUNC( FIELD_VOID,	"Disable",	InputDisable ),
	DEFINE_INPUTFUNC( FIELD_VOID,	"Toggle",	InputToggle ),

	// Outputs
	DEFINE_OUTPUT( m_OnSpawnItem,		"OnSpawnItem" ),
	DEFINE_OUTPUT( m_OnSpawnRareItem,	"OnSpawnRareItem" ),
	DEFINE_OUTPUT( m_OnSpawnWeapon,		"OnSpawnWeapon"),
	DEFINE_OUTPUT( m_OnSpawnRareWeapon,		"OnSpawnRareWeapon"),

	// Function Pointers
	DEFINE_THINKFUNC( MakerThink ),

	DEFINE_FIELD( m_hIgnoreEntity, FIELD_EHANDLE ),
	DEFINE_KEYFIELD( m_iszIngoreEnt, FIELD_STRING, "IgnoreEntity" ), 
END_DATADESC()

//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------
CItemMakerFirefight::CItemMakerFirefight(void)
{
	m_bCanSpawn = true;
}

//-----------------------------------------------------------------------------
// Purpose: Spawn
//-----------------------------------------------------------------------------
void CItemMakerFirefight::Spawn(void)
{
	SetSolid( SOLID_NONE );
	Precache();

	//Start on?
	if ( m_bDisabled == false )
	{
		SetThink(&CItemMakerFirefight::MakerThink);
		SetNextThink( gpGlobals->curtime + 0.1f );
	}
	else
	{
		//wait to be activated.
		SetThink(&CItemMakerFirefight::SUB_DoNothing);
	}
}

//-----------------------------------------------------------------------------
// Purpose: Precache the target NPC
//-----------------------------------------------------------------------------
void CItemMakerFirefight::Precache(void)
{
	BaseClass::Precache();

	int nItems = ARRAYSIZE(g_charItemsCommon);
	int nItemsRare = ARRAYSIZE(g_charItemsRare);
	int nWeapons = ARRAYSIZE(g_charWeaponsCommon);
	int nWeaponsRare = ARRAYSIZE(g_charWeaponsRare);
	int i;
	for (i = 0; i < nItems; ++i)
		UTIL_PrecacheOther(g_charItemsCommon[nItems]);
	for (i = 0; i < nItemsRare; ++i)
		UTIL_PrecacheOther(g_charItemsRare[nItemsRare]);
	for (i = 0; i < nWeapons; ++i)
		UTIL_PrecacheOther(g_charWeaponsCommon[nWeapons]);
	for (i = 0; i < nWeaponsRare; ++i)
		UTIL_PrecacheOther(g_charWeaponsRare[nWeaponsRare]);
}

//-----------------------------------------------------------------------------
// Purpose: Creates a new NPC every so often.
//-----------------------------------------------------------------------------
void CItemMakerFirefight::MakerThink(void)
{
	SetNextThink(15.0);
	if (HasSpawnFlags(SF_ITEMMAKER_ONLYWEAPONS))
	{
		if (HasSpawnFlags(SF_ITEMMAKER_RAREWEAPONS))
		{
			int rareweaponrandom = random->RandomInt(0, 15);

			if (rareweaponrandom == 0)
			{
				MakeRareWeapon();
			}
			else
			{
				MakeWeapon();
			}
		}
		else
		{
			MakeWeapon();
		}
	}
	else if (HasSpawnFlags(SF_ITEMMAKER_ONLYITEMS))
	{
		if (HasSpawnFlags(SF_ITEMMAKER_RAREITEMS))
		{
			int rareitemrandom = random->RandomInt(0, 15);

			if (rareitemrandom == 0)
			{
				MakeRareItem();
			}
			else
			{
				MakeItem();
			}
		}
		else
		{
			MakeItem();
		}
	}
	else
	{
		int itemrandom = random->RandomInt(0, 1);

		if (itemrandom == 1)
		{
			if (HasSpawnFlags(SF_ITEMMAKER_RAREWEAPONS))
			{
				int rareweaponrandom = random->RandomInt(0, 15);

				if (rareweaponrandom == 0)
				{
					MakeRareWeapon();
				}
				else
				{
					MakeWeapon();
				}
			}
			else
			{
				MakeWeapon();
			}
		}
		else
		{
			if (HasSpawnFlags(SF_ITEMMAKER_RAREITEMS))
			{
				int rareitemrandom = random->RandomInt(0, 15);

				if (rareitemrandom == 0)
				{
					MakeRareItem();
				}
				else
				{
					MakeItem();
				}
			}
			else
			{
				MakeItem();
			}
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: Creates the NPC.
//-----------------------------------------------------------------------------
void CItemMakerFirefight::MakeItem(void)
{
	int nItems = ARRAYSIZE(g_charItemsCommon);
	int randomChoice = rand() % nItems;
	const char *pRandomName = g_charItemsCommon[randomChoice];

	CBaseEntity	*pent = CreateEntityByName(pRandomName);

	if (!pent)
	{
		Warning("item_maker_firefight: Entity classname does not exist in database.\n");
		return;
	}

	// ------------------------------------------------
	//  Intialize spawned NPC's relationships
	// ------------------------------------------------

	m_OnSpawnItem.Set(pent, pent, this);

	pent->SetAbsOrigin(GetAbsOrigin());

	// Strip pitch and roll from the spawner's angles. Pass only yaw to the spawned NPC.
	QAngle angles = GetAbsAngles();
	angles.x = 0.0;
	angles.z = 0.0;
	pent->SetAbsAngles(angles);

	pent->m_isRareEntity = false;

	DispatchSpawn(pent);
	pent->SetOwnerEntity(this);
	DispatchActivate(pent);

	if (m_ChildTargetName != NULL_STRING)
	{
		// if I have a netname (overloaded), give the child NPC that name as a targetname
		pent->SetName(m_ChildTargetName);
	}

	m_bCanSpawn = false;
}

//-----------------------------------------------------------------------------
// Purpose: Creates the NPC.
//-----------------------------------------------------------------------------
void CItemMakerFirefight::MakeRareItem(void)
{
	int nItems = ARRAYSIZE(g_charItemsRare);
	int randomChoice = rand() % nItems;
	const char *pRandomName = g_charItemsRare[randomChoice];

	CBaseEntity	*pent = CreateEntityByName(pRandomName);

	if (!pent)
	{
		Warning("item_maker_firefight: Entity classname does not exist in database.\n");
		return;
	}

	// ------------------------------------------------
	//  Intialize spawned NPC's relationships
	// ------------------------------------------------

	m_OnSpawnRareItem.Set(pent, pent, this);

	pent->SetAbsOrigin(GetAbsOrigin());

	// Strip pitch and roll from the spawner's angles. Pass only yaw to the spawned NPC.
	QAngle angles = GetAbsAngles();
	angles.x = 0.0;
	angles.z = 0.0;
	pent->SetAbsAngles(angles);

	pent->m_isRareEntity = true;

	DispatchSpawn(pent);
	pent->SetOwnerEntity(this);
	DispatchActivate(pent);

	if (m_ChildTargetName != NULL_STRING)
	{
		// if I have a netname (overloaded), give the child NPC that name as a targetname
		pent->SetName(m_ChildTargetName);
	}

	m_bCanSpawn = false;
}

//-----------------------------------------------------------------------------
// Purpose: Creates the NPC.
//-----------------------------------------------------------------------------
void CItemMakerFirefight::MakeWeapon(void)
{
	int nWeapons = ARRAYSIZE(g_charWeaponsCommon);
	int randomChoice = rand() % nWeapons;
	const char *pRandomName = g_charWeaponsCommon[randomChoice];

	CBaseEntity	*pent = CreateEntityByName(pRandomName);

	if (!pent)
	{
		Warning("item_maker_firefight: Entity classname does not exist in database.\n");
		return;
	}

	// ------------------------------------------------
	//  Intialize spawned NPC's relationships
	// ------------------------------------------------

	m_OnSpawnWeapon.Set(pent, pent, this);

	pent->SetAbsOrigin(GetAbsOrigin());

	// Strip pitch and roll from the spawner's angles. Pass only yaw to the spawned NPC.
	QAngle angles = GetAbsAngles();
	angles.x = 0.0;
	angles.z = 0.0;
	pent->SetAbsAngles(angles);

	pent->m_isRareEntity = false;

	DispatchSpawn(pent);
	pent->SetOwnerEntity(this);
	DispatchActivate(pent);

	if (m_ChildTargetName != NULL_STRING)
	{
		// if I have a netname (overloaded), give the child NPC that name as a targetname
		pent->SetName(m_ChildTargetName);
	}

	m_bCanSpawn = false;
}

//-----------------------------------------------------------------------------
// Purpose: Creates the NPC.
//-----------------------------------------------------------------------------
void CItemMakerFirefight::MakeRareWeapon(void)
{
	int nWeapons = ARRAYSIZE(g_charWeaponsRare);
	int randomChoice = rand() % nWeapons;
	const char *pRandomName = g_charWeaponsRare[randomChoice];

	CBaseEntity	*pent = CreateEntityByName(pRandomName);

	if (!pent)
	{
		Warning("item_maker_firefight: Entity classname does not exist in database.\n");
		return;
	}

	// ------------------------------------------------
	//  Intialize spawned NPC's relationships
	// ------------------------------------------------

	m_OnSpawnWeapon.Set(pent, pent, this);

	pent->SetAbsOrigin(GetAbsOrigin());

	// Strip pitch and roll from the spawner's angles. Pass only yaw to the spawned NPC.
	QAngle angles = GetAbsAngles();
	angles.x = 0.0;
	angles.z = 0.0;
	pent->SetAbsAngles(angles);

	pent->m_isRareEntity = true;

	DispatchSpawn(pent);
	pent->SetOwnerEntity(this);
	DispatchActivate(pent);

	if (m_ChildTargetName != NULL_STRING)
	{
		// if I have a netname (overloaded), give the child NPC that name as a targetname
		pent->SetName(m_ChildTargetName);
	}

	m_bCanSpawn = false;
}

//-----------------------------------------------------------------------------
// Purpose: Toggle the spawner's state
//-----------------------------------------------------------------------------
void CItemMakerFirefight::Toggle(void)
{
	if ( m_bDisabled )
	{
		Enable();
	}
	else
	{
		Disable();
	}
}

//-----------------------------------------------------------------------------
// Purpose: Start the spawner
//-----------------------------------------------------------------------------
void CItemMakerFirefight::Enable(void)
{
	m_bDisabled = false;
	SetThink(&CItemMakerFirefight::MakerThink);
	SetNextThink( gpGlobals->curtime );
}


//-----------------------------------------------------------------------------
// Purpose: Stop the spawner
//-----------------------------------------------------------------------------
void CItemMakerFirefight::Disable(void)
{
	m_bDisabled = true;
	SetThink ( NULL );
}

//-----------------------------------------------------------------------------
// Purpose: Input handler that spawns an NPC.
//-----------------------------------------------------------------------------
void CItemMakerFirefight::InputSpawnItem(inputdata_t &inputdata)
{
	MakeItem();
}

//-----------------------------------------------------------------------------
// Purpose: Input handler that spawns an NPC.
//-----------------------------------------------------------------------------
void CItemMakerFirefight::InputSpawnRareItem(inputdata_t &inputdata)
{
	MakeRareItem();
}

//-----------------------------------------------------------------------------
// Purpose: Input handler that spawns an NPC.
//-----------------------------------------------------------------------------
void CItemMakerFirefight::InputSpawnWeapon(inputdata_t &inputdata)
{
	MakeWeapon();
}

//-----------------------------------------------------------------------------
// Purpose: Input handler that spawns an NPC.
//-----------------------------------------------------------------------------
void CItemMakerFirefight::InputSpawnRareWeapon(inputdata_t &inputdata)
{
	MakeRareWeapon();
}

//-----------------------------------------------------------------------------
// Purpose: Input hander that starts the spawner
//-----------------------------------------------------------------------------
void CItemMakerFirefight::InputEnable(inputdata_t &inputdata)
{
	Enable();
}


//-----------------------------------------------------------------------------
// Purpose: Input hander that stops the spawner
//-----------------------------------------------------------------------------
void CItemMakerFirefight::InputDisable(inputdata_t &inputdata)
{
	Disable();
}


//-----------------------------------------------------------------------------
// Purpose: Input hander that toggles the spawner
//-----------------------------------------------------------------------------
void CItemMakerFirefight::InputToggle(inputdata_t &inputdata)
{
	Toggle();
}
