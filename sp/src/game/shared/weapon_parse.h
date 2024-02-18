//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: Weapon data file parsing, shared by game & client dlls.
//
// $NoKeywords: $
//=============================================================================//

#ifndef WEAPON_PARSE_H
#define WEAPON_PARSE_H
#ifdef _WIN32
#pragma once
#endif

#include "shareddefs.h"

class IFileSystem;

typedef unsigned short WEAPON_FILE_INFO_HANDLE;

// -----------------------------------------------------------
// Weapon sound types
// Used to play sounds defined in the weapon's classname.txt file
// This needs to match pWeaponSoundCategories in weapon_parse.cpp
// ------------------------------------------------------------
typedef enum {
	EMPTY,
	SINGLE,
	SINGLE_NPC,
	WPN_DOUBLE, // Can't be "DOUBLE" because windows.h uses it.
	DOUBLE_NPC,
	BURST,
	RELOAD,
	RELOAD_NPC,
	MELEE_MISS,
	MELEE_HIT,
	MELEE_HIT_WORLD,
	SPECIAL1,
	SPECIAL2,
	SPECIAL3,
	TAUNT,
	DEPLOY,

	// Add new shoot sound types here

	NUM_SHOOT_SOUND_TYPES,
} WeaponSound_t;

int GetWeaponSoundFromString( const char *pszString );

#define MAX_SHOOT_SOUNDS	16			// Maximum number of shoot sounds per shoot type

#define MAX_WEAPON_STRING	80
#define MAX_WEAPON_PREFIX	16
#define MAX_WEAPON_AMMO_NAME		32

#define WEAPON_PRINTNAME_MISSING "!!! Missing printname on weapon"

class CHudTexture;
class KeyValues;

//-----------------------------------------------------------------------------
// Purpose: Contains the data read from the weapon's script file. 
// It's cached so we only read each weapon's script file once.
// Each game provides a CreateWeaponInfo function so it can have game-specific
// data (like CS move speeds) in the weapon script.
//-----------------------------------------------------------------------------
class FileWeaponInfo_t
{
public:

	FileWeaponInfo_t();
	
	// Each game can override this to get whatever values it wants from the script.
	virtual void Parse( KeyValues *pKeyValuesData, const char *szWeaponName );

	
public:	
	bool					bParsedScript;
	bool					bLoadedHudElements;

// SHARED
	char					szClassName[MAX_WEAPON_STRING];
	char					szPrintName[MAX_WEAPON_STRING];			// Name for showing in HUD, etc.

	char					szViewModel[MAX_WEAPON_STRING];			// View model of this weapon
	char					szWorldModel[MAX_WEAPON_STRING];		// Model of this weapon seen carried by the player
	char					szAnimationPrefix[MAX_WEAPON_PREFIX];	// Prefix of the animations that should be used by the player carrying this weapon
	char					szWeaponType[MAX_WEAPON_STRING];	// Defines the weapon's "type"
	int						iSlot;									// inventory slot.
	int						iPosition;								// position in the inventory slot.
#ifdef STEAM_INPUT
	// Steam Input needs these to be interchangeable
	int						iSlot360;								// inventory slot for hud_quickswitch 2.
	int						iPosition360;							// position in the inventory slot for hud_quickswitch 2.
#endif
	int						iMaxClip1;								// max primary clip size (-1 if no clip)
	int						iMaxClip2;								// max secondary clip size (-1 if no clip)
	int						iDefaultClip1;							// amount of primary ammo in the gun when it's created
	int						iDefaultClip2;							// amount of secondary ammo in the gun when it's created
	int						iWeight;								// this value used to determine this weapon's importance in autoselection.
	int						iRumbleEffect;							// Which rumble effect to use when fired? (xbox)
	bool					bAutoSwitchTo;							// whether this weapon should be considered for autoswitching to
	bool					bAutoSwitchFrom;						// whether this weapon can be autoswitched away from when picking up another weapon or ammo
	int						iFlags;									// miscellaneous weapon flags
	char					szAmmo1[MAX_WEAPON_AMMO_NAME];			// "primary" ammo type
	char					szAmmo2[MAX_WEAPON_AMMO_NAME];			// "secondary" ammo type

	Vector					vecIronsightPosOffset;
	QAngle					angIronsightAngOffset;
	float					flIronsightFOVOffset;
	Vector					vecAdjustPosOffset;
	QAngle					angAdjustAngOffset;
	bool					m_bUseIronsight;
	bool					m_bUseMagStyleReloads;
	bool					m_bUseMuzzleSmoke;
	bool					m_bLowerWeapon;
	bool					m_bUseIronsightCrosshair;

	//SMMOD Custom Weapons!
	//bool	m_sUsesShotgunStyleReloads;
	bool	m_sCanReloadSingly;
	bool	m_sDualWeapons;
	bool	m_sWeaponOptions;
	bool	m_sCustomMelee;
	bool	m_sCustomMeleeSecondary;

	bool	m_sPrimaryBulletEnabled;
	bool	m_sPrimaryMissleEnabled;
	char	m_sPrimaryAmmoType;
	float	m_sPrimaryDamage;
	int m_sPrimaryShotCount;
	float m_sPrimaryFireRate;
	Vector m_vPrimarySpread;
	bool	m_sPrimaryInfiniteAmmoEnabled;
	bool	m_sPrimarySMGGrenadeEnabled;
	float	m_sSMGGrenadePrimaryDamage;
	bool	m_sPrimaryAR2EnergyBallEnabled;
	float	m_sPrimaryCombineBallRadius;
	float	m_sPrimaryCombineBallMass;
	float	m_sPrimaryCombineBallDuration;
	float	m_sPrimaryRecoilEasyDampen;
	float	m_sPrimaryRecoilDegrees;
	float	m_sPrimaryRecoilSeconds;
	Vector	m_vPrimaryIronsightSpread;
	Vector	m_vPrimaryZoomSpread;
	float	m_sPrimaryIronsightFireRate;
	float	m_sPrimaryZoomFireRate;
	bool	m_sHasPrimaryFire;
	bool	m_sPrimaryHasRecoilSMGGrenade;
	bool	m_sPrimaryHasRecoilRPGMissle;
	int		m_sPrimaryMinRange;
	int		m_sPrimaryMaxRange;
	bool	m_sCanPrimaryFireUnderwater;
	bool	m_sFireBothOnPrimary;
	//float	m_sPrimaryMeleeDamage;

	bool	m_sSecondaryBulletEnabled;
	bool	m_sSecondaryMissleEnabled;
	bool	 m_sUsePrimaryAmmo;
	char	m_sSecondaryAmmoType;
	float	m_sSecondaryDamage;
	int m_sSecondaryShotCount;
	float m_sSecondaryFireRate;
	Vector m_vSecondarySpread;
	bool	m_sSecondaryInfiniteAmmoEnabled;
	bool	m_sSecondarySMGGrenadeEnabled;
	float	m_sSMGGrenadeSecondaryDamage;
	bool	m_sSecondaryAR2EnergyBallEnabled;
	float	m_sSecondaryCombineBallRadius;
	float	m_sSecondaryCombineBallMass;
	float	m_sSecondaryCombineBallDuration;
	float	m_sSecondaryRecoilEasyDampen;
	float	m_sSecondaryRecoilDegrees;
	float	m_sSecondaryRecoilSeconds;
	Vector	m_vSecondaryIronsightSpread;
	Vector	m_vSecondaryZoomSpread;
	float	m_sSecondaryIronsightFireRate;
	float	m_sSecondaryZoomFireRate;
	bool	m_sHasSecondaryFire;
	bool	m_sSecondaryHasRecoilSMGGrenade;
	bool	m_sSecondaryHasRecoilRPGMissle;
	int		m_sSecondaryMinRange;
	int		m_sSecondaryMaxRange;
	bool	m_sCanSecondaryFireUnderwater;
	bool	m_sFireBothOnSecondary;

	bool	m_sUsesZoom;
	bool	m_sUsesZoomSound;
	bool	m_sUsesZoomColor;
	bool	m_sUseZoomOnPrimaryFire;
	int		m_sZoomColorRed;
	int		m_sZoomColorGreen;
	int		m_sZoomColorBlue;
	int		m_sZoomColorAlpha;

	bool	m_sHasMeleeOptions;
	float	m_sMeleeDamage;
	float	m_sMeleeRange;
	float	m_sMeleeKickEasyDampen;
	float	m_sMeleeKickDegrees;
	float	m_sMeleeKickSeconds;

	bool	m_sUsesCustomization;
	bool	m_sUsesBodygroupSection1;
	bool	m_sUsesBodygroupSection2;
	bool	m_sUsesBodygroupSection3;
	bool	m_sUsesBodygroupSection4;
	bool	m_sUsesBodygroupSection5;
	bool	m_sUsesBodygroupSection6;
	int		m_sBodygroup1;
	int		m_sBodygroup2;
	int		m_sBodygroup3;
	int		m_sBodygroup4;
	int		m_sBodygroup5;
	int		m_sBodygroup6;
	int		m_sSubgroup1;
	int		m_sSubgroup2;
	int		m_sSubgroup3;
	int		m_sSubgroup4;
	int		m_sSubgroup5;
	int		m_sSubgroup6;
	int		m_sWeaponSkin;

	// Sound blocks
	char					aShootSounds[NUM_SHOOT_SOUND_TYPES][MAX_WEAPON_STRING];	

	int						iAmmoType;
	const char	*cAmmoType;
	int						iAmmo2Type;
	const char	*cAmmo2Type;
	bool					m_bMeleeWeapon;		// Melee weapons can always "fire" regardless of ammo.

	// This tells if the weapon was built right-handed (defaults to true).
	// This helps cl_righthand make the decision about whether to flip the model or not.
	bool					m_bBuiltRightHanded;
	bool					m_bAllowFlipping;	// False to disallow flipping the model, regardless of whether
												// it is built left or right handed.

// CLIENT DLL
	// Sprite data, read from the data file
	int						iSpriteCount;
	CHudTexture						*iconActive;
	CHudTexture	 					*iconInactive;
	CHudTexture 					*iconAmmo;
	CHudTexture 					*iconAmmo2;
	CHudTexture 					*iconCrosshair;
	CHudTexture 					*iconAutoaim;
	CHudTexture 					*iconZoomedCrosshair;
	CHudTexture 					*iconZoomedAutoaim;
	CHudTexture						*iconSmall;

// TF2 specific
	bool					bShowUsageHint;							// if true, then when you receive the weapon, show a hint about it

// SERVER DLL

};

// The weapon parse function
bool ReadWeaponDataFromFileForSlot( IFileSystem* filesystem, const char *szWeaponName, 
	WEAPON_FILE_INFO_HANDLE *phandle, const unsigned char *pICEKey = NULL);

// If weapon info has been loaded for the specified class name, this returns it.
WEAPON_FILE_INFO_HANDLE LookupWeaponInfoSlot( const char *name );

FileWeaponInfo_t *GetFileWeaponInfoFromHandle( WEAPON_FILE_INFO_HANDLE handle );
WEAPON_FILE_INFO_HANDLE GetInvalidWeaponInfoHandle( void );
void PrecacheFileWeaponInfoDatabase( IFileSystem *filesystem, const unsigned char *pICEKey );
//void InitCustomWeapon(void);

// 
// Read a possibly-encrypted KeyValues file in. 
// If pICEKey is NULL, then it appends .txt to the filename and loads it as an unencrypted file.
// If pICEKey is non-NULL, then it appends .ctx to the filename and loads it as an encrypted file.
//
// (This should be moved into a more appropriate place).
//
KeyValues* ReadEncryptedKVFile( IFileSystem *filesystem, const char *szFilenameWithoutExtension, const unsigned char *pICEKey, bool bForceReadEncryptedFile = false );


// Each game implements this. It can return a derived class and override Parse() if it wants.
extern FileWeaponInfo_t* CreateWeaponInfo();


#endif // WEAPON_PARSE_H
