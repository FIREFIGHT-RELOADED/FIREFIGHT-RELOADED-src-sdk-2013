//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: Weapon data file parsing, shared by game & client dlls.
//
// $NoKeywords: $
//=============================================================================//
#include "cbase.h"
#include <KeyValues.h>
#include <tier0/mem.h>
#include "filesystem.h"
#include "utldict.h"
#include "ammodef.h"
#if defined(CLIENT_DLL)
#include "hl2/c_basehlcombatweapon.h"
#include "SMMOD/c_weapon_custom.h"
#include "c_baseentity.h"
#else
#include "SMMOD/weapon_custom.h"
#endif

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

// The sound categories found in the weapon classname.txt files
// This needs to match the WeaponSound_t enum in weapon_parse.h
#if !defined(_STATIC_LINKED) || defined(CLIENT_DLL)
const char *pWeaponSoundCategories[ NUM_SHOOT_SOUND_TYPES ] = 
{
	"empty",
	"single_shot",
	"single_shot_npc",
	"double_shot",
	"double_shot_npc",
	"burst",
	"reload",
	"reload_npc",
	"melee_miss",
	"melee_hit",
	"melee_hit_world",
	"special1",
	"special2",
	"special3",
	"taunt",
	"deploy"
};
#else
extern const char *pWeaponSoundCategories[ NUM_SHOOT_SOUND_TYPES ];
#endif

int GetWeaponSoundFromString( const char *pszString )
{
	for ( int i = EMPTY; i < NUM_SHOOT_SOUND_TYPES; i++ )
	{
		if ( !Q_stricmp(pszString,pWeaponSoundCategories[i]) )
			return (WeaponSound_t)i;
	}
	return -1;
}


// Item flags that we parse out of the file.
typedef struct
{
	const char *m_pFlagName;
	int m_iFlagValue;
} itemFlags_t;
#if !defined(_STATIC_LINKED) || defined(CLIENT_DLL)
itemFlags_t g_ItemFlags[9] =
{
	{ "ITEM_FLAG_SELECTONEMPTY",	ITEM_FLAG_SELECTONEMPTY },
	{ "ITEM_FLAG_NOAUTORELOAD",		ITEM_FLAG_NOAUTORELOAD },
	{ "ITEM_FLAG_NOAUTOSWITCHEMPTY", ITEM_FLAG_NOAUTOSWITCHEMPTY },
	{ "ITEM_FLAG_LIMITINWORLD",		ITEM_FLAG_LIMITINWORLD },
	{ "ITEM_FLAG_EXHAUSTIBLE",		ITEM_FLAG_EXHAUSTIBLE },
	{ "ITEM_FLAG_DOHITLOCATIONDMG", ITEM_FLAG_DOHITLOCATIONDMG },
	{ "ITEM_FLAG_NOAMMOPICKUPS",	ITEM_FLAG_NOAMMOPICKUPS },
	{ "ITEM_FLAG_NOITEMPICKUP",		ITEM_FLAG_NOITEMPICKUP },
	{ "ITEM_FLAG_HIDESELECTION", ITEM_FLAG_HIDESELECTION }
};
#else
extern itemFlags_t g_ItemFlags[8];
#endif


static CUtlDict< FileWeaponInfo_t*, unsigned short > m_WeaponInfoDatabase;

#ifdef _DEBUG
// used to track whether or not two weapons have been mistakenly assigned the wrong slot
bool g_bUsedWeaponSlots[MAX_WEAPON_SLOTS][MAX_WEAPON_POSITIONS] = { 0 };

#endif

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : *name - 
// Output : FileWeaponInfo_t
//-----------------------------------------------------------------------------
static WEAPON_FILE_INFO_HANDLE FindWeaponInfoSlot( const char *name )
{
	// Complain about duplicately defined metaclass names...
	unsigned short lookup = m_WeaponInfoDatabase.Find( name );
	if ( lookup != m_WeaponInfoDatabase.InvalidIndex() )
	{
		return lookup;
	}

	FileWeaponInfo_t *insert = CreateWeaponInfo();

	lookup = m_WeaponInfoDatabase.Insert( name, insert );
	Assert( lookup != m_WeaponInfoDatabase.InvalidIndex() );
	return lookup;
}

// Find a weapon slot, assuming the weapon's data has already been loaded.
WEAPON_FILE_INFO_HANDLE LookupWeaponInfoSlot( const char *name )
{
	return m_WeaponInfoDatabase.Find( name );
}



// FIXME, handle differently?
static FileWeaponInfo_t gNullWeaponInfo;


//-----------------------------------------------------------------------------
// Purpose: 
// Input  : handle - 
// Output : FileWeaponInfo_t
//-----------------------------------------------------------------------------
FileWeaponInfo_t *GetFileWeaponInfoFromHandle( WEAPON_FILE_INFO_HANDLE handle )
{
	if ( handle < 0 || handle >= m_WeaponInfoDatabase.Count() )
	{
		return &gNullWeaponInfo;
	}

	if ( handle == m_WeaponInfoDatabase.InvalidIndex() )
	{
		return &gNullWeaponInfo;
	}

	return m_WeaponInfoDatabase[ handle ];
}

//-----------------------------------------------------------------------------
// Purpose: 
// Output : WEAPON_FILE_INFO_HANDLE
//-----------------------------------------------------------------------------
WEAPON_FILE_INFO_HANDLE GetInvalidWeaponInfoHandle( void )
{
	return (WEAPON_FILE_INFO_HANDLE)m_WeaponInfoDatabase.InvalidIndex();
}

#if 0
void ResetFileWeaponInfoDatabase( void )
{
	int c = m_WeaponInfoDatabase.Count(); 
	for ( int i = 0; i < c; ++i )
	{
		delete m_WeaponInfoDatabase[ i ];
	}
	m_WeaponInfoDatabase.RemoveAll();

#ifdef _DEBUG
	memset(g_bUsedWeaponSlots, 0, sizeof(g_bUsedWeaponSlots));
#endif
}
#endif

/*
#ifdef CLIENT_DLL


#define CWeaponCustom C_WeaponCustom 
static C_BaseEntity *CCHL2MPScriptedWeaponFactory(void)
{
	return static_cast< C_BaseEntity * >(new CWeaponCustom);
};
#endif

#ifndef CLIENT_DLL
static CUtlDict< CEntityFactory<CWeaponCustom>*, unsigned short > m_WeaponFactoryDatabase;
#endif

void RegisterScriptedWeapon(const char *className)
{
#ifdef CLIENT_DLL
	if (GetClassMap().FindFactory(className))
	{
		return;
	}
	GetClassMap().Add(className, "CWeaponCustom", sizeof(CWeaponCustom),
		&CCHL2MPScriptedWeaponFactory, true);
	//GetClassMap().Add( className, "CWeaponCustom", sizeof( C_HLSelectFireMachineGun));
#else
	if (EntityFactoryDictionary()->FindFactory(className))
	{
		return;
	}

	unsigned short lookup = m_WeaponFactoryDatabase.Find(className);
	if (lookup != m_WeaponFactoryDatabase.InvalidIndex())
	{
		return;
	}

	// Andrew; This fixes months worth of pain and anguish.
	CEntityFactory<CWeaponCustom> *pFactory = new CEntityFactory<CWeaponCustom>(className);

	lookup = m_WeaponFactoryDatabase.Insert(className, pFactory);
	Assert(lookup != m_WeaponFactoryDatabase.InvalidIndex());
#endif
	// BUGBUG: When attempting to precache weapons registered during runtime,
	// they don't appear as valid registered entities.
	// static CPrecacheRegister precache_weapon_(&CPrecacheRegister::PrecacheFn_Other, className);
}
void InitCustomWeapon(void)
{
	FileFindHandle_t findHandle; // note: FileFINDHandle

	const char *pFilename = filesystem->FindFirstEx("scripts/*.txt", "MOD", &findHandle);
	while (pFilename)
	{
		if (Q_strncmp(pFilename, "weapon_custom_", strlen("weapon_custom_")) == 0)
		{
#ifdef CLIENT_DLL
			Msg("'%s' added to Custom Weapon list on Client!\n", pFilename);
#else
			Msg("'%s' added to Custom Weapon list on Server!\n", pFilename);
#endif

#if !defined(CLIENT_DLL)
			//	CEntityFactory<CWeaponCustom> weapon_custom( pFilename );
			//	UTIL_PrecacheOther(pFilename);
#endif
			char fileBase[512] = "";
			Q_FileBase(pFilename, fileBase, sizeof(fileBase));
			RegisterScriptedWeapon(fileBase);
			//CEntityFactory<CWeaponCustom>(CEntityFactory<CWeaponCustom> &);
			//LINK_ENTITY_TO_CLASS2(pFilename,CWeaponCustom);

			WEAPON_FILE_INFO_HANDLE tmp;
#ifdef CLIENT_DLL
			if (ReadWeaponDataFromFileForSlot(filesystem, fileBase, &tmp))
			{
				gWR.LoadWeaponSprites(tmp);
			}
#else
			ReadWeaponDataFromFileForSlot(filesystem, fileBase, &tmp);
#endif


			pFilename = filesystem->FindNext(findHandle);
		}
		else
		{
			pFilename = filesystem->FindNext(findHandle);
		}
	}

	filesystem->FindClose(findHandle);

}
*/

void PrecacheFileWeaponInfoDatabase( IFileSystem *filesystem, const unsigned char *pICEKey )
{
	if ( m_WeaponInfoDatabase.Count() )
		return;

	KeyValues *manifest = new KeyValues( "weaponscripts" );
	if ( manifest->LoadFromFile( filesystem, "scripts/weapon_manifest.txt", "GAME" ) )
	{
		for ( KeyValues *sub = manifest->GetFirstSubKey(); sub != NULL ; sub = sub->GetNextKey() )
		{
			if ( !Q_stricmp( sub->GetName(), "file" ) )
			{
				char fileBase[512];
				Q_FileBase( sub->GetString(), fileBase, sizeof(fileBase) );
				WEAPON_FILE_INFO_HANDLE tmp;
#ifdef CLIENT_DLL
				if ( ReadWeaponDataFromFileForSlot( filesystem, fileBase, &tmp, pICEKey ) )
				{
					gWR.LoadWeaponSprites( tmp );
				}
#else
				ReadWeaponDataFromFileForSlot( filesystem, fileBase, &tmp, pICEKey );
#endif
			}
			else
			{
				Error( "Expecting 'file', got %s\n", sub->GetName() );
			}
		}
	}
	manifest->deleteThis();
}

KeyValues* ReadEncryptedKVFile( IFileSystem *filesystem, const char *szFilenameWithoutExtension, const unsigned char *pICEKey, bool bForceReadEncryptedFile /*= false*/ )
{
	Assert( strchr( szFilenameWithoutExtension, '.' ) == NULL );
	char szFullName[512];

	const char *pSearchPath = "MOD";

	if ( pICEKey == NULL )
	{
		pSearchPath = "GAME";
	}

	// Open the weapon data file, and abort if we can't
	KeyValues *pKV = new KeyValues( "WeaponDatafile" );

	Q_snprintf(szFullName,sizeof(szFullName), "%s.txt", szFilenameWithoutExtension);

	if ( bForceReadEncryptedFile || !pKV->LoadFromFile( filesystem, szFullName, pSearchPath ) ) // try to load the normal .txt file first
	{
#ifndef _XBOX
		if ( pICEKey )
		{
			Q_snprintf(szFullName,sizeof(szFullName), "%s.ctx", szFilenameWithoutExtension); // fall back to the .ctx file

			FileHandle_t f = filesystem->Open( szFullName, "rb", pSearchPath );

			if (!f)
			{
				pKV->deleteThis();
				return NULL;
			}
			// load file into a null-terminated buffer
			int fileSize = filesystem->Size(f);
			char *buffer = (char*)MemAllocScratch(fileSize + 1);
		
			Assert(buffer);
		
			filesystem->Read(buffer, fileSize, f); // read into local buffer
			buffer[fileSize] = 0; // null terminate file as EOF
			filesystem->Close( f );	// close file after reading

			UTIL_DecodeICE( (unsigned char*)buffer, fileSize, pICEKey );

			bool retOK = pKV->LoadFromBuffer( szFullName, buffer, filesystem );

			MemFreeScratch();

			if ( !retOK )
			{
				pKV->deleteThis();
				return NULL;
			}
		}
		else
		{
			pKV->deleteThis();
			return NULL;
		}
#else
		pKV->deleteThis();
		return NULL;
#endif
	}

	return pKV;
}


//-----------------------------------------------------------------------------
// Purpose: Read data on weapon from script file
// Output:  true  - if data2 successfully read
//			false - if data load fails
//-----------------------------------------------------------------------------

bool ReadWeaponDataFromFileForSlot(IFileSystem* filesystem, const char *szWeaponName, WEAPON_FILE_INFO_HANDLE *phandle, const unsigned char *pICEKey)
{
	if (!phandle)
	{
		Assert(0);
		return false;
	}

	*phandle = FindWeaponInfoSlot(szWeaponName);
	FileWeaponInfo_t *pFileInfo = GetFileWeaponInfoFromHandle(*phandle);
	Assert(pFileInfo);

	if (pFileInfo->bParsedScript)
		return true;

	char sz[1024];
	Q_snprintf(sz, sizeof(sz), "scripts/%s", szWeaponName);

	KeyValues *pKV = ReadEncryptedKVFile(filesystem, sz, pICEKey,
#if defined( DOD_DLL )
		true			// Only read .ctx files!
#else
		false
#endif
		);

	if (!pKV)
		return false;

	pFileInfo->Parse(pKV, szWeaponName);

	pKV->deleteThis();

	return true;
}


//-----------------------------------------------------------------------------
// FileWeaponInfo_t implementation.
//-----------------------------------------------------------------------------

FileWeaponInfo_t::FileWeaponInfo_t()
{
	bParsedScript = false;
	bLoadedHudElements = false;
	szClassName[0] = 0;
	szPrintName[0] = 0;

	szViewModel[0] = 0;
	szWorldModel[0] = 0;
	szAnimationPrefix[0] = 0;
	szWeaponType[0] = 0;
	iSlot = 0;
	iPosition = 0;
	iMaxClip1 = 0;
	iMaxClip2 = 0;
	iDefaultClip1 = 0;
	iDefaultClip2 = 0;
	iWeight = 0;
	iRumbleEffect = -1;
	bAutoSwitchTo = false;
	bAutoSwitchFrom = false;
	iFlags = 0;
	szAmmo1[0] = 0;
	szAmmo2[0] = 0;
	memset( aShootSounds, 0, sizeof( aShootSounds ) );
	iAmmoType = 0;
	iAmmo2Type = 0;
	m_bMeleeWeapon = false;
	iSpriteCount = 0;
	iconActive = 0;
	iconInactive = 0;
	iconAmmo = 0;
	iconAmmo2 = 0;
	iconCrosshair = 0;
	iconAutoaim = 0;
	iconZoomedCrosshair = 0;
	iconZoomedAutoaim = 0;
	bShowUsageHint = false;
	m_bAllowFlipping = true;
	m_bBuiltRightHanded = true;
}

#ifdef CLIENT_DLL
extern ConVar hud_fastswitch;
#endif

void FileWeaponInfo_t::Parse( KeyValues *pKeyValuesData, const char *szWeaponName )
{
	// Okay, we tried at least once to look this up...
	bParsedScript = true;

	// Classname
	Q_strncpy( szClassName, szWeaponName, MAX_WEAPON_STRING );
	// Printable name
	Q_strncpy( szPrintName, pKeyValuesData->GetString( "printname", WEAPON_PRINTNAME_MISSING ), MAX_WEAPON_STRING );
	// View model & world model
	Q_strncpy( szViewModel, pKeyValuesData->GetString( "viewmodel" ), MAX_WEAPON_STRING );
	Q_strncpy( szWorldModel, pKeyValuesData->GetString( "playermodel" ), MAX_WEAPON_STRING );
	Q_strncpy( szAnimationPrefix, pKeyValuesData->GetString( "anim_prefix" ), MAX_WEAPON_PREFIX );
	Q_strncpy( szWeaponType, pKeyValuesData->GetString("weapon_type"), MAX_WEAPON_STRING );
	iSlot = pKeyValuesData->GetInt( "bucket", 0 );
	iPosition = pKeyValuesData->GetInt( "bucket_position", 0 );
	
	// Use the console (X360) buckets if hud_fastswitch is set to 2.
#ifdef CLIENT_DLL
	if ( hud_fastswitch.GetInt() == 2 )
#else
	if ( IsX360() )
#endif
	{
		iSlot = pKeyValuesData->GetInt( "bucket_360", iSlot );
		iPosition = pKeyValuesData->GetInt( "bucket_position_360", iPosition );
	}
	iMaxClip1 = pKeyValuesData->GetInt( "clip_size", WEAPON_NOCLIP );					// Max primary clips gun can hold (assume they don't use clips by default)
	iMaxClip2 = pKeyValuesData->GetInt( "clip2_size", WEAPON_NOCLIP );					// Max secondary clips gun can hold (assume they don't use clips by default)
	iDefaultClip1 = pKeyValuesData->GetInt( "default_clip", iMaxClip1 );		// amount of primary ammo placed in the primary clip when it's picked up
	iDefaultClip2 = pKeyValuesData->GetInt( "default_clip2", iMaxClip2 );		// amount of secondary ammo placed in the secondary clip when it's picked up
	iWeight = pKeyValuesData->GetInt( "weight", 0 );

	iRumbleEffect = pKeyValuesData->GetInt( "rumble", -1 );
	
	// LAME old way to specify item flags.
	// Weapon scripts should use the flag names.
	iFlags = pKeyValuesData->GetInt( "item_flags", ITEM_FLAG_LIMITINWORLD );

	for ( int i=0; i < ARRAYSIZE( g_ItemFlags ); i++ )
	{
		int iVal = pKeyValuesData->GetInt( g_ItemFlags[i].m_pFlagName, -1 );
		if ( iVal == 0 )
		{
			iFlags &= ~g_ItemFlags[i].m_iFlagValue;
		}
		else if ( iVal == 1 )
		{
			iFlags |= g_ItemFlags[i].m_iFlagValue;
		}
	}


	bShowUsageHint = ( pKeyValuesData->GetInt( "showusagehint", 0 ) != 0 ) ? true : false;
	bAutoSwitchTo = ( pKeyValuesData->GetInt( "autoswitchto", 1 ) != 0 ) ? true : false;
	bAutoSwitchFrom = ( pKeyValuesData->GetInt( "autoswitchfrom", 1 ) != 0 ) ? true : false;
	m_bBuiltRightHanded = ( pKeyValuesData->GetInt( "BuiltRightHanded", 1 ) != 0 ) ? true : false;
	m_bAllowFlipping = ( pKeyValuesData->GetInt( "AllowFlipping", 1 ) != 0 ) ? true : false;
	m_bMeleeWeapon = ( pKeyValuesData->GetInt( "MeleeWeapon", 0 ) != 0 ) ? true : false;
	m_bUseMagStyleReloads = (pKeyValuesData->GetInt("MagazineStyledReloads", 0) != 0) ? true : false;
	m_bUseMuzzleSmoke = (pKeyValuesData->GetInt("UseMuzzleSmoke", 0) != 0) ? true : false;
	m_bUseIronsight = (pKeyValuesData->GetInt("useironsights", 1) != 0) ? true : false;
	m_bLowerWeapon = (pKeyValuesData->GetInt("LowerWeapon", 1) != 0) ? true : false;
	m_bUseIronsightCrosshair = (pKeyValuesData->GetInt("useironsightcrosshair", 1) != 0) ? true : false;

#if defined(_DEBUG) && defined(HL2_CLIENT_DLL)
	// make sure two weapons aren't in the same slot & position
	if ( iSlot >= MAX_WEAPON_SLOTS ||
		iPosition >= MAX_WEAPON_POSITIONS )
	{
		Warning( "Invalid weapon slot or position [slot %d/%d max], pos[%d/%d max]\n",
			iSlot, MAX_WEAPON_SLOTS - 1, iPosition, MAX_WEAPON_POSITIONS - 1 );
	}
	else
	{
		if (g_bUsedWeaponSlots[iSlot][iPosition])
		{
			Warning( "Duplicately assigned weapon slots in selection hud:  %s (%d, %d)\n", szPrintName, iSlot, iPosition );
		}
		g_bUsedWeaponSlots[iSlot][iPosition] = true;
	}
#endif

	// Primary ammo used 
	cAmmoType = pKeyValuesData->GetString("primary_ammo", "None");
	if (strcmp("None", cAmmoType) == 0)
		Q_strncpy( szAmmo1, "", sizeof( szAmmo1 ) );
	else
		Q_strncpy(szAmmo1, cAmmoType, sizeof(szAmmo1));
	iAmmoType = GetAmmoDef()->Index( szAmmo1 );
	
	// Secondary ammo used
	cAmmo2Type = pKeyValuesData->GetString("secondary_ammo", "None");
	if (strcmp("None", cAmmo2Type) == 0)
		Q_strncpy( szAmmo2, "", sizeof( szAmmo2 ) );
	else
		Q_strncpy(szAmmo2, cAmmo2Type, sizeof(szAmmo2));
	iAmmo2Type = GetAmmoDef()->Index( szAmmo2 );

	//ironsights
	KeyValues *pSights = pKeyValuesData->FindKey("IronSight");
	if (pSights)
	{
		vecIronsightPosOffset.x = pSights->GetFloat("forward", 0.0f);
		vecIronsightPosOffset.y = pSights->GetFloat("right", 0.0f);
		vecIronsightPosOffset.z = pSights->GetFloat("up", 0.0f);

		angIronsightAngOffset[PITCH] = pSights->GetFloat("pitch", 0.0f);
		angIronsightAngOffset[YAW] = pSights->GetFloat("yaw", 0.0f);
		angIronsightAngOffset[ROLL] = pSights->GetFloat("roll", 0.0f);

		flIronsightFOVOffset = pSights->GetFloat("fov", 0.0f);
	}
	else
	{
		//m_bUseIronsight = false;
		vecIronsightPosOffset = vec3_origin;
		angIronsightAngOffset.Init();
	}

	//Adjust
	KeyValues *pAdjust = pKeyValuesData->FindKey("Adjust");
	if (pAdjust)
	{
		vecAdjustPosOffset.x = pAdjust->GetFloat("forward", 0.0f);
		vecAdjustPosOffset.y = pAdjust->GetFloat("right", 0.0f);
		vecAdjustPosOffset.z = pAdjust->GetFloat("up", 0.0f);

		angAdjustAngOffset[PITCH] = pAdjust->GetFloat("pitch", 0.0f);
		angAdjustAngOffset[YAW] = pAdjust->GetFloat("yaw", 0.0f);
		angAdjustAngOffset[ROLL] = pAdjust->GetFloat("roll", 0.0f);
	}
	else
	{
		vecAdjustPosOffset = vec3_origin;
		angAdjustAngOffset.Init();
	}

	KeyValues *pWeaponSpec = pKeyValuesData->FindKey("WeaponSpec");
	if (pWeaponSpec)
	{
		KeyValues *pWeaponOptions = pWeaponSpec->FindKey("WeaponOptions");
		if (pWeaponOptions)
		{
			m_sWeaponOptions = true;
			m_sCanReloadSingly = (pWeaponOptions->GetInt("CanReloadSingly", 1) != 0) ? true : false;
			m_sDualWeapons = (pWeaponOptions->GetInt("DualWeapons", 1) != 0) ? true : false;
			m_sCustomMelee = (pWeaponOptions->GetInt("IsMelee", 1) != 0) ? true : false;
			m_sCustomMeleeSecondary = (pWeaponOptions->GetInt("SecondaryCanMelee", 1) != 0) ? true : false;
		}
		else
		{
			m_sWeaponOptions = false;
		}

		KeyValues *pPrimaryFire = pWeaponSpec->FindKey("PrimaryFire");
		if (pPrimaryFire)
		{
			m_sHasPrimaryFire = true;
			m_sPrimaryFireRate = pPrimaryFire->GetFloat("FireRate", 1.0f);
			m_sPrimaryIronsightFireRate = pPrimaryFire->GetFloat("IronsightFireRate", m_sPrimaryFireRate);
			m_sPrimaryZoomFireRate = pPrimaryFire->GetFloat("ZoomFireRate", m_sPrimaryFireRate);
			m_sPrimaryInfiniteAmmoEnabled = (pPrimaryFire->GetInt("InfiniteAmmo", 1) != 0) ? true : false;
			m_sPrimaryMinRange = pPrimaryFire->GetInt("MinRange", 0);
			m_sPrimaryMinRange = pPrimaryFire->GetInt("MaxRange", 0);
			m_sCanPrimaryFireUnderwater = (pPrimaryFire->GetInt("CanFireUnderwater", 1) != 0) ? true : false;
			m_sFireBothOnPrimary = (pWeaponOptions->GetInt("FireBothGuns", 1) != 0) ? true : false;
			KeyValues *pBullet1 = pPrimaryFire->FindKey("Bullet");
			if (pBullet1)
			{
				m_sPrimaryBulletEnabled = true;
				m_sPrimaryDamage = pBullet1->GetFloat("Damage", 0);
				m_sPrimaryShotCount = pBullet1->GetInt("ShotCount", 0);

				KeyValues *pSpread1 = pBullet1->FindKey("Spread");
				if (pSpread1)
				{
					m_vPrimarySpread.x = sin((pSpread1->GetFloat("x", 0.0f) / 2.0f));
					m_vPrimarySpread.y = sin((pSpread1->GetFloat("y", 0.0f) / 2.0f));
					m_vPrimarySpread.z = sin((pSpread1->GetFloat("z", 0.0f) / 2.0f));
				}
				else
				{
					m_vPrimarySpread.x = 0.0f;
					m_vPrimarySpread.y = 0.0f;
					m_vPrimarySpread.z = 0.0f;
				}

				KeyValues *pIronsightSpread1 = pBullet1->FindKey("IronsightSpread");
				if (pIronsightSpread1)
				{
					m_vPrimaryIronsightSpread.x = sin((pIronsightSpread1->GetFloat("x", 0.0f) / 2.0f));
					m_vPrimaryIronsightSpread.y = sin((pIronsightSpread1->GetFloat("y", 0.0f) / 2.0f));
					m_vPrimaryIronsightSpread.z = sin((pIronsightSpread1->GetFloat("z", 0.0f) / 2.0f));
				}
				else
				{
					m_vPrimaryIronsightSpread.x = m_vPrimarySpread.x;
					m_vPrimaryIronsightSpread.y = m_vPrimarySpread.y;
					m_vPrimaryIronsightSpread.z = m_vPrimarySpread.z;
				}

				KeyValues *pZoomSpread1 = pBullet1->FindKey("ZoomSpread");
				if (pZoomSpread1)
				{
					m_vPrimaryZoomSpread.x = sin((pZoomSpread1->GetFloat("x", 0.0f) / 2.0f));
					m_vPrimaryZoomSpread.y = sin((pZoomSpread1->GetFloat("y", 0.0f) / 2.0f));
					m_vPrimaryZoomSpread.z = sin((pZoomSpread1->GetFloat("z", 0.0f) / 2.0f));
				}
				else
				{
					m_vPrimaryZoomSpread.x = m_vPrimarySpread.x;
					m_vPrimaryZoomSpread.y = m_vPrimarySpread.y;
					m_vPrimaryZoomSpread.z = m_vPrimarySpread.z;
				}
			}
			else
			{
				m_sPrimaryDamage = 0.0f;
				m_sPrimaryShotCount = 0;
				m_sPrimaryBulletEnabled = false;
			}

			KeyValues *pMissle1 = pPrimaryFire->FindKey("Missle");
			if (pMissle1) //No params yet, but setting this will enable missles
			{
				m_sPrimaryMissleEnabled = true;
				m_sPrimaryHasRecoilRPGMissle = (pMissle1->GetInt("UseRecoil", 1) != 0) ? true : false;
			}
			else
			{
				m_sPrimaryMissleEnabled = false;
			}

			KeyValues *pSMGGrenade1 = pPrimaryFire->FindKey("SMGGrenade");
			if (pSMGGrenade1) //No params yet, but setting this will enable missles
			{
				m_sPrimarySMGGrenadeEnabled = true;
				m_sSMGGrenadePrimaryDamage = pSMGGrenade1->GetFloat("Damage", 0);
				m_sPrimaryHasRecoilSMGGrenade = (pSMGGrenade1->GetInt("UseRecoil", 1) != 0) ? true : false;
			}
			else
			{
				m_sPrimarySMGGrenadeEnabled = false;
				m_sSMGGrenadePrimaryDamage = 0.0;
			}

			KeyValues *pAR2EnergyBall1 = pPrimaryFire->FindKey("AR2EnergyBall");
			if (pAR2EnergyBall1) //No params yet, but setting this will enable missles
			{
				m_sPrimaryAR2EnergyBallEnabled = true;
				m_sPrimaryCombineBallRadius = pAR2EnergyBall1->GetFloat("Radius", 0);
				m_sPrimaryCombineBallMass = pAR2EnergyBall1->GetFloat("Mass", 0);
				m_sPrimaryCombineBallDuration = pAR2EnergyBall1->GetFloat("Duration", 0);
			}
			else
			{
				m_sPrimaryAR2EnergyBallEnabled = false;
				m_sPrimaryCombineBallRadius = 0.0;
				m_sPrimaryCombineBallMass = 0.0;
				m_sPrimaryCombineBallDuration = 0.0;
			}

			KeyValues *pRecoil1 = pPrimaryFire->FindKey("Recoil");
			if (pRecoil1) //No params yet, but setting this will enable missles
			{
				m_sPrimaryRecoilEasyDampen = pRecoil1->GetFloat("EasyDampen", 0);
				m_sPrimaryRecoilDegrees = pRecoil1->GetFloat("Degrees", 0);
				m_sPrimaryRecoilSeconds = pRecoil1->GetFloat("Seconds", 0);
			}
			else
			{
				m_sPrimaryRecoilEasyDampen = 0.0;
				m_sPrimaryRecoilDegrees = 0.0;
				m_sPrimaryRecoilSeconds = 0.0;
			}
		}
		else
		{
			m_sHasPrimaryFire = false;
		}

		KeyValues *pSecondaryFire = pWeaponSpec->FindKey("SecondaryFire");
		if (pSecondaryFire)
		{
			m_sHasSecondaryFire = true;
			m_sSecondaryFireRate = pSecondaryFire->GetFloat("FireRate", 1.0f);
			m_sSecondaryIronsightFireRate = pSecondaryFire->GetFloat("IronsightFireRate", m_sSecondaryFireRate);
			m_sSecondaryZoomFireRate = pSecondaryFire->GetFloat("ZoomFireRate", m_sSecondaryFireRate);
			m_sUsePrimaryAmmo = (pSecondaryFire->GetInt("UsePrimaryAmmo", 0) != 0) ? true : false;
			m_sSecondaryInfiniteAmmoEnabled = (pSecondaryFire->GetInt("InfiniteAmmo", 1) != 0) ? true : false;
			m_sSecondaryMinRange = pSecondaryFire->GetInt("MinRange", 0);
			m_sSecondaryMinRange = pSecondaryFire->GetInt("MaxRange", 0);
			m_sCanSecondaryFireUnderwater = (pSecondaryFire->GetInt("CanFireUnderwater", 1) != 0) ? true : false;
			m_sFireBothOnSecondary = (pWeaponOptions->GetInt("FireBothGuns", 1) != 0) ? true : false;
			KeyValues *pBullet2 = pSecondaryFire->FindKey("Bullet");
			if (pBullet2)
			{
				m_sSecondaryBulletEnabled = true;
				m_sSecondaryDamage = pBullet2->GetFloat("Damage", 0);
				m_sSecondaryShotCount = pBullet2->GetInt("ShotCount", 0);

				KeyValues *pSpread2 = pBullet2->FindKey("Spread");
				if (pSpread2)
				{
					m_vSecondarySpread.x = sin(pSpread2->GetFloat("x", 0.0f) / 2.0f);
					m_vSecondarySpread.y = sin(pSpread2->GetFloat("y", 0.0f) / 2.0f);
					m_vSecondarySpread.z = sin(pSpread2->GetFloat("z", 0.0f) / 2.0f);
				}
				else
				{
					m_vSecondarySpread.x = 0.0f;
					m_vSecondarySpread.y = 0.0f;
					m_vSecondarySpread.z = 0.0f;
				}

				KeyValues *pIronsightSpread2 = pBullet2->FindKey("IronsightSpread");
				if (pIronsightSpread2)
				{
					m_vSecondaryIronsightSpread.x = sin((pIronsightSpread2->GetFloat("x", 0.0f) / 2.0f));
					m_vSecondaryIronsightSpread.y = sin((pIronsightSpread2->GetFloat("y", 0.0f) / 2.0f));
					m_vSecondaryIronsightSpread.z = sin((pIronsightSpread2->GetFloat("z", 0.0f) / 2.0f));
				}
				else
				{
					m_vSecondaryIronsightSpread.x = m_vSecondarySpread.x;
					m_vSecondaryIronsightSpread.y = m_vSecondarySpread.y;
					m_vSecondaryIronsightSpread.z = m_vSecondarySpread.z;
				}

				KeyValues *pZoomSpread2 = pBullet2->FindKey("ZoomSpread");
				if (pZoomSpread2)
				{
					m_vSecondaryZoomSpread.x = sin((pZoomSpread2->GetFloat("x", 0.0f) / 2.0f));
					m_vSecondaryZoomSpread.y = sin((pZoomSpread2->GetFloat("y", 0.0f) / 2.0f));
					m_vSecondaryZoomSpread.z = sin((pZoomSpread2->GetFloat("z", 0.0f) / 2.0f));
				}
				else
				{
					m_vSecondaryZoomSpread.x = m_vSecondarySpread.x;
					m_vSecondaryZoomSpread.y = m_vSecondarySpread.y;
					m_vSecondaryZoomSpread.z = m_vSecondarySpread.z;
				}
			}
			else
			{
				m_sSecondaryDamage = 0.0f;
				m_sSecondaryShotCount = 0;
				m_sSecondaryBulletEnabled = false;
			}
			KeyValues *pMissle2 = pSecondaryFire->FindKey("Missle");
			if (pMissle2) //No params yet, but setting this will enable missles
			{
				m_sSecondaryMissleEnabled = true;
				m_sSecondaryHasRecoilRPGMissle = (pMissle2->GetInt("UseRecoil", 1) != 0) ? true : false;
			}
			else
			{
				m_sSecondaryMissleEnabled = false;
			}

			KeyValues *pSMGGrenade2 = pSecondaryFire->FindKey("SMGGrenade");
			if (pSMGGrenade2) //No params yet, but setting this will enable missles
			{
				m_sSecondarySMGGrenadeEnabled = true;
				m_sSMGGrenadeSecondaryDamage = pSMGGrenade2->GetFloat("Damage", 0);
				m_sSecondaryHasRecoilSMGGrenade = (pSMGGrenade2->GetInt("UseRecoil", 1) != 0) ? true : false;
			}
			else
			{
				m_sSecondarySMGGrenadeEnabled = false;
				m_sSMGGrenadeSecondaryDamage = 0.0;
			}

			KeyValues *pAR2EnergyBall2 = pSecondaryFire->FindKey("AR2EnergyBall");
			if (pAR2EnergyBall2) //No params yet, but setting this will enable missles
			{
				m_sSecondaryAR2EnergyBallEnabled = true;
				m_sSecondaryCombineBallRadius = pAR2EnergyBall2->GetFloat("Radius", 0);
				m_sSecondaryCombineBallMass = pAR2EnergyBall2->GetFloat("Mass", 0);
				m_sSecondaryCombineBallDuration = pAR2EnergyBall2->GetFloat("Duration", 0);
			}
			else
			{
				m_sSecondaryAR2EnergyBallEnabled = false;
				m_sSecondaryCombineBallRadius = 0.0;
				m_sSecondaryCombineBallMass = 0.0;
				m_sSecondaryCombineBallDuration = 0.0;
			}

			KeyValues *pRecoil2 = pSecondaryFire->FindKey("Recoil");
			if (pRecoil2) //No params yet, but setting this will enable missles
			{
				m_sSecondaryRecoilEasyDampen = pRecoil2->GetFloat("EasyDampen", 0);
				m_sSecondaryRecoilDegrees = pRecoil2->GetFloat("Degrees", 0);
				m_sSecondaryRecoilSeconds = pRecoil2->GetFloat("Seconds", 0);
			}
			else
			{
				m_sSecondaryRecoilEasyDampen = 0.0;
				m_sSecondaryRecoilDegrees = 0.0;
				m_sSecondaryRecoilSeconds = 0.0;
			}
		}
		else
		{
			m_sHasSecondaryFire = false;
		}

		KeyValues *pMeleeOptions = pWeaponSpec->FindKey("Melee");
		if (pMeleeOptions)
		{
			m_sHasMeleeOptions = true;
			m_sMeleeDamage = pMeleeOptions->GetFloat("Damage", 0);
			m_sMeleeRange = pMeleeOptions->GetFloat("Range", 0);

			KeyValues *pRecoilM = pMeleeOptions->FindKey("Kick");
			if (pRecoilM) //No params yet, but setting this will enable missles
			{
				m_sMeleeKickEasyDampen = pRecoilM->GetFloat("EasyDampen", 0);
				m_sMeleeKickDegrees = pRecoilM->GetFloat("Degrees", 0);
				m_sMeleeKickSeconds = pRecoilM->GetFloat("Seconds", 0);
			}
			else
			{
				m_sMeleeKickEasyDampen = 0.0;
				m_sMeleeKickDegrees = 0.0;
				m_sMeleeKickSeconds = 0.0;
			}
		}
		else
		{
			m_sHasMeleeOptions = false;
		}

		
		KeyValues *pZoom = pWeaponSpec->FindKey("Zoom");
		if (pZoom)
		{
			m_sUsesZoom = true;
			m_sUseZoomOnPrimaryFire = (pZoom->GetInt("UseOnPrimaryFire", 1) != 0) ? true : false;
			m_sUsesZoomSound = (pZoom->GetInt("UsesSound", 1) != 0) ? true : false;
			m_sUsesZoomColor = (pZoom->GetInt("UsesColor", 1) != 0) ? true : false;
			KeyValues *pZoomColor = pZoom->FindKey("ZoomColor");
			{
				if (pZoomColor)
				{
					m_sZoomColorRed = pZoomColor->GetInt("Red", 0);
					m_sZoomColorGreen = pZoomColor->GetInt("Green", 0);
					m_sZoomColorBlue = pZoomColor->GetInt("Blue", 0);
					m_sZoomColorAlpha = pZoomColor->GetInt("Alpha", 0);
				}
			}
		}
		else
		{
			m_sUsesZoom = false;
		}

		KeyValues *pCustomization = pWeaponSpec->FindKey("Customization");
		if (pCustomization)
		{
			m_sUsesCustomization = true;
			m_sWeaponSkin = pCustomization->GetInt("Skin", 0);
			KeyValues *pBodygroup1 = pWeaponSpec->FindKey("Bodygroup1");
			{
				if (pBodygroup1)
				{
					m_sBodygroup1 = pBodygroup1->GetInt("Bodygroup", 0);
					m_sSubgroup1 = pBodygroup1->GetInt("Subgroup", 0);
				}
			}

			KeyValues *pBodygroup2 = pWeaponSpec->FindKey("Bodygroup2");
			{
				if (pBodygroup2)
				{
					m_sBodygroup2 = pBodygroup2->GetInt("Bodygroup", 0);
					m_sSubgroup2 = pBodygroup2->GetInt("Subgroup", 0);
				}
			}

			KeyValues *pBodygroup3 = pWeaponSpec->FindKey("Bodygroup3");
			{
				if (pBodygroup3)
				{
					m_sBodygroup3 = pBodygroup3->GetInt("Bodygroup", 0);
					m_sSubgroup3 = pBodygroup3->GetInt("Subgroup", 0);
				}
			}

			KeyValues *pBodygroup4 = pWeaponSpec->FindKey("Bodygroup4");
			{
				if (pBodygroup4)
				{
					m_sBodygroup4 = pBodygroup4->GetInt("Bodygroup", 0);
					m_sSubgroup4 = pBodygroup4->GetInt("Subgroup", 0);
				}
			}

			KeyValues *pBodygroup5 = pWeaponSpec->FindKey("Bodygroup5");
			{
				if (pBodygroup5)
				{
					m_sBodygroup5 = pBodygroup5->GetInt("Bodygroup", 0);
					m_sSubgroup5 = pBodygroup5->GetInt("Subgroup", 0);
				}
			}

			KeyValues *pBodygroup6 = pWeaponSpec->FindKey("Bodygroup6");
			{
				if (pBodygroup6)
				{
					m_sBodygroup6 = pBodygroup6->GetInt("Bodygroup", 0);
					m_sSubgroup6 = pBodygroup6->GetInt("Subgroup", 0);
				}
			}
		}
		else
		{
			m_sUsesCustomization = false;
		}
	}

	// Now read the weapon sounds
	memset( aShootSounds, 0, sizeof( aShootSounds ) );
	KeyValues *pSoundData = pKeyValuesData->FindKey( "SoundData" );
	if ( pSoundData )
	{
		for ( int i = EMPTY; i < NUM_SHOOT_SOUND_TYPES; i++ )
		{
			const char *soundname = pSoundData->GetString( pWeaponSoundCategories[i] );
			if ( soundname && soundname[0] )
			{
				Q_strncpy( aShootSounds[i], soundname, MAX_WEAPON_STRING );
			}
		}
	}
}

