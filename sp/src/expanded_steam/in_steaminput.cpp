//=============================================================================//
//
// Purpose: Community integration of Steam Input on Source SDK 2013.
//
// Author: Blixibon
//
// $NoKeywords: $
//=============================================================================//

#include "expanded_steam/isteaminput.h"

#include "inputsystem/iinputsystem.h"
#include "GameUI/IGameUI.h"
#include "IGameUIFuncs.h"
#include "ienginevgui.h"
#include <vgui/IInput.h>
#include <vgui/ILocalize.h>
#include <vgui_controls/Controls.h>
#include "steam/isteaminput.h"
#include "steam/isteamutils.h"
#include "icommandline.h"
#include "cdll_int.h"
#include "tier1/convar.h"
#include "tier1/strtools.h"
#include "tier1/utlbuffer.h"
#include "tier2/tier2.h"
#include "tier3/tier3.h"
#include "filesystem.h"

//#include "libpng/png.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

//-------------------------------------------

IVEngineClient *g_pEngineClient = NULL;
IEngineVGui *g_pEngineVGui = NULL;

// Copied from cdll_util.cpp
char *SteamInput_VarArgs( const char *format, ... )
{
	va_list		argptr;
	static char		string[1024];
	
	va_start (argptr, format);
	Q_vsnprintf (string, sizeof( string ), format,argptr);
	va_end (argptr);

	return string;	
}

//-------------------------------------------

#define USE_HL2_INSTALLATION 0 // This attempts to obtain HL2's action manifest from the user's own HL2 or Portal installations

InputActionSetHandle_t g_AS_GameControls;
InputActionSetHandle_t g_AS_VehicleControls;
InputActionSetHandle_t g_AS_MenuControls;

//-------------------------------------------

struct InputDigitalActionCommandBind_t : public InputDigitalActionBind_t
{
	InputDigitalActionCommandBind_t()
	{
		pszActionName = NULL;
		pszBindCommand = NULL;
	}

	InputDigitalActionCommandBind_t( const char *_pszActionName, const char *_pszBindCommand )
	{
		pszActionName = _pszActionName;
		pszBindCommand = _pszBindCommand;
	}

	const char *pszActionName;
	const char *pszBindCommand;

	void OnDown() override
	{
		g_pEngineClient->ClientCmd_Unrestricted( SteamInput_VarArgs( "%s\n", pszBindCommand ) );
	}

	void OnUp() override
	{
		if (pszBindCommand[0] == '+')
		{
			// Unpress the bind
			g_pEngineClient->ClientCmd_Unrestricted( SteamInput_VarArgs( "-%s\n", pszBindCommand+1 ) );
		}
	}
};

static CUtlVector<InputDigitalActionCommandBind_t> g_DigitalActionBinds;

// Stores strings parsed for action binds
static CUtlStringList g_DigitalActionBindNames;

// Special cases from the above
InputDigitalActionBind_t *g_DAB_Zoom;
InputDigitalActionBind_t *g_DAB_Brake;
InputDigitalActionBind_t *g_DAB_Duck;
InputDigitalActionBind_t *g_DAB_MenuPause;
InputDigitalActionBind_t *g_DAB_Toggle_Zoom;
InputDigitalActionBind_t *g_DAB_Toggle_Duck;

//-------------------------------------------

InputAnalogActionHandle_t g_AA_Move;
InputAnalogActionHandle_t g_AA_Camera;
InputAnalogActionHandle_t g_AA_JoystickCamera;

//-------------------------------------------

InputAnalogActionHandle_t g_AA_Steer;
InputAnalogActionHandle_t g_AA_Accelerate;
InputAnalogActionHandle_t g_AA_Brake;

//-------------------------------------------

InputDigitalActionBind_t g_DAB_MenuUp;
InputDigitalActionBind_t g_DAB_MenuDown;
InputDigitalActionBind_t g_DAB_MenuLeft;
InputDigitalActionBind_t g_DAB_MenuRight;
InputDigitalActionBind_t g_DAB_MenuSelect;
InputDigitalActionBind_t g_DAB_MenuCancel;
InputDigitalActionBind_t g_DAB_MenuLB;
InputDigitalActionBind_t g_DAB_MenuRB;
InputDigitalActionBind_t g_DAB_MenuX;
InputDigitalActionBind_t g_DAB_MenuY;

InputAnalogActionHandle_t g_AA_Mouse;

//-------------------------------------------

CON_COMMAND( pause_menu, "Shortcut to toggle pause menu" )
{
	if (g_pEngineVGui->IsGameUIVisible())
	{
		g_pEngineClient->ClientCmd_Unrestricted( "gameui_hide" );
	}
	else
	{
		g_pEngineClient->ClientCmd_Unrestricted( "gameui_activate" );
	}
}

//-------------------------------------------

static ConVar si_current_cfg( "si_current_cfg", "0", FCVAR_ARCHIVE, "Steam Input's current controller." );

static ConVar si_force_glyph_controller( "si_force_glyph_controller", "-1", FCVAR_NONE, "Forces glyphs to translate to the specified ESteamInputType." );

static ConVar si_use_glyphs( "si_use_glyphs", "0", FCVAR_NONE, "Whether or not to use controller glyphs for hints." );

static ConVar si_enable_rumble( "si_enable_rumble", "1", FCVAR_NONE, "Enables controller rumble triggering vibration events in Steam Input. If disabled, rumble is directed back to the input system as before." );

static ConVar si_enable_dualsense_adaptive_triggers( "si_enable_dualsense_adaptive_triggers", "1", FCVAR_ARCHIVE, "Enables Dualsense adaptive trigger support." );

static ConVar si_hintremap( "si_hintremap", "1", FCVAR_NONE, "Enables the hint remap system, which remaps HUD hints based on the current controller configuration." );

static ConVar si_print_action_set( "si_print_action_set", "0" );
static ConVar si_print_joy_src( "si_print_joy_src", "0" );
static ConVar si_print_rumble( "si_print_rumble", "0" );
static ConVar si_print_hintremap( "si_print_hintremap", "0" );

//-------------------------------------------

class CSource2013SteamInput : public ISource2013SteamInput
{
public:

	CSource2013SteamInput()
	{

	}

	~CSource2013SteamInput();

	void Initialize( CreateInterfaceFn factory ) override;

	void InitSteamInput() override;
	void InitActionManifest();

	void PostInit() override;

	void LevelInitPreEntity() override;

	void Shutdown() override;

	void RunFrame( ActionSet_t &iActionSet ) override;
	
	bool IsEnabled() override;

	//-------------------------------------------

	bool IsSteamRunningOnSteamDeck() override;
	void SetGamepadUI( bool bToggle ) override;
	
	InputHandle_t GetActiveController() override;
	int GetConnectedControllers( InputHandle_t *nOutHandles ) override;

	const char *GetControllerName( InputHandle_t nController ) override;
	int GetControllerType( InputHandle_t nController ) override;

	bool ShowBindingPanel( InputHandle_t nController ) override;

	//-----------------------------------------------------------------------------

	bool ShowGamepadTextInput( const char *pszDescription, uint32 nMaxChars, bool bMultiline = false, const char *pszExistingText = "" ) override;
	bool ShowFloatingGamepadTextInput( bool bMultiline, int nX, int nY, int nW, int nH ) override;

	bool GetEnteredGamepadTextInput( char *pszText, uint32 nTextSize ) override;
	uint32 GetEnteredGamepadTextLength() override;

	//-------------------------------------------

	bool LoadActionBinds( const char *pszFileName );
	InputDigitalActionCommandBind_t *FindActionBind( const char *pszActionName );

	bool TestActions( int iActionSet, InputHandle_t nController );
	void DoActions( int iActionSet );
	void TransitionActionSet( int iNewActionSet, int iOldActionSet );

	void TestDigitalActionBind( InputHandle_t nController, InputDigitalActionBind_t &DigitalAction, bool &bActiveInput );
	void SendDigitalActionBind( InputDigitalActionBind_t &DigitalAction );
	void DeactivateDigitalActionBind( InputDigitalActionBind_t &DigitalAction );

	void SendKeyFromDigitalActionHandle( InputDigitalActionBind_t &nHandle, ButtonCode_t nKey );

	bool UsingJoysticks() override;
	void GetJoystickValues( float &flForward, float &flSide, float &flPitch, float &flYaw,
		bool &bRelativeForward, bool &bRelativeSide, bool &bRelativePitch, bool &bRelativeYaw ) override;

	void SetRumble( InputHandle_t nController, float fLeftMotor, float fRightMotor, int userId = INVALID_USER_ID ) override;
	void StopRumble() override;

	//-------------------------------------------
	
	void SetLEDColor( InputHandle_t nController, byte r, byte g, byte b ) override;
	void ResetLEDColor( InputHandle_t nController ) override;
    void SetDualSenseTriggerEffect(InputHandle_t nController, const ScePadTriggerEffectParam *pParam) override;

	//-------------------------------------------

	int FindDigitalActionsForCommand( const char *pszCommand, InputDigitalActionHandle_t *pHandles );
	int FindAnalogActionsForCommand( const char *pszCommand, InputAnalogActionHandle_t *pHandles );
	void GetInputActionOriginsForCommand( const char *pszCommand, CUtlVector<EInputActionOrigin> &actionOrigins, int iActionSetOverride = -1 );

	//void GetGlyphPNGsForCommand( CUtlVector<const char*> &szImgList, const char *pszCommand, int &iSize, int iStyle ) override;
	//void GetGlyphSVGsForCommand( CUtlVector<const char*> &szImgList, const char *pszCommand ) override;

	virtual bool UseGlyphs() override { return si_use_glyphs.GetBool(); };
	void GetButtonStringsForCommand( const char *pszCommand, CUtlVector<const char *> &szStringList, int iActionSet = -1 ) override;

	//-------------------------------------------

	void LoadHintRemap( const char *pszFileName );
	void RemapHudHint( const char **pszInputHint ) override;

private:
	const char *IdentifyControllerParam( ESteamInputType inputType );

	void InputDeviceConnected( InputHandle_t nDeviceHandle );
	void InputDeviceDisconnected( InputHandle_t nDeviceHandle );
	void InputDeviceChanged( InputHandle_t nOldHandle, InputHandle_t nNewHandle );
	void DeckConnected( InputHandle_t nDeviceHandle );

	//-------------------------------------------

	// Provides a description for the specified action using GetStringForActionOrigin()
	const char *LookupDescriptionForActionOrigin( EInputActionOrigin eAction );

	//-------------------------------------------

	bool m_bEnabled;

	// Handle to the active controller (may change depending on last input)
	InputHandle_t m_nControllerHandle = 0;

	InputAnalogActionData_t m_analogMoveData, m_analogCameraData;

	InputActionSetHandle_t m_iLastActionSet;

	bool m_bIsGamepadUI;

	// If true, Steam Input won't check for a controller during gameplay
	bool m_bInvalidSteamInputAction;

	//-------------------------------------------

	enum
	{
		HINTREMAPCOND_NONE,
		HINTREMAPCOND_INPUT_TYPE,		// Only for the specified type of controller
		HINTREMAPCOND_ACTION_BOUND,		// Only if the specified action is bound
	};

	struct HintRemapCondition_t
	{
		int iType;
		bool bNot;
		char szParam[32];
	};

	struct HintRemap_t
	{
		const char *pszOldHint;
		const char *pszNewHint;

		CUtlVector<HintRemapCondition_t> nRemapConds;
	};

	CUtlVector< HintRemap_t >	m_HintRemaps;
};

//EXPOSE_SINGLE_INTERFACE( CSource2013SteamInput, ISource2013SteamInput, SOURCE2013STEAMINPUT_INTERFACE_VERSION );

// TODO: Replace with proper singleton interface in the future
ISource2013SteamInput *CreateSource2013SteamInput()
{
	static CSource2013SteamInput g_SteamInput;
	return &g_SteamInput;
}

//-------------------------------------------

CSource2013SteamInput::~CSource2013SteamInput()
{
	SteamInput()->Shutdown();
}

//-------------------------------------------

void CSource2013SteamInput::Initialize( CreateInterfaceFn factory )
{
	g_pEngineClient = (IVEngineClient *)factory( VENGINE_CLIENT_INTERFACE_VERSION, NULL );
	g_pEngineVGui = (IEngineVGui *)factory( VENGINE_VGUI_VERSION, NULL );
}

//-------------------------------------------

void CSource2013SteamInput::InitSteamInput()
{
	bool bInit = false;

	if (CommandLine()->CheckParm( "-nosteamcontroller" ) == 0 && SteamUtils()->IsOverlayEnabled())
	{
		// Do this before initializing SteamInput()
		InitActionManifest();

		bInit = SteamInput()->Init( true );
	}

	if (!bInit)
	{
		Msg( "SteamInput didn't initialize\n" );

		if (si_current_cfg.GetString()[0] != '0')
		{
			Msg("Reverting leftover Steam Input cvars\n");
			g_pEngineClient->ClientCmd_Unrestricted( "exec steam_uninput.cfg" );
			g_pEngineClient->ClientCmd_Unrestricted( SteamInput_VarArgs( "exec steam_uninput_%s.cfg", si_current_cfg.GetString() ) );
		}

		return;
	}

	Msg( "SteamInput initialized\n" );

	m_bEnabled = false;
	//SteamInput()->EnableDeviceCallbacks();
	m_bInvalidSteamInputAction = false;

	g_AS_GameControls		= SteamInput()->GetActionSetHandle( "GameControls" );
	g_AS_VehicleControls	= SteamInput()->GetActionSetHandle( "VehicleControls" );
	g_AS_MenuControls		= SteamInput()->GetActionSetHandle( "MenuControls" );

	SteamInput()->RunFrame();

	// Load the KV
	LoadActionBinds( "scripts/steaminput_actionbinds.txt" );

	if (g_DigitalActionBinds.Count() > 0)
	{
		// Fill out special cases
		g_DAB_Zoom				= FindActionBind("zoom");
		g_DAB_Brake				= FindActionBind("jump");
		g_DAB_Duck				= FindActionBind("duck");
		g_DAB_MenuPause			= FindActionBind("pause_menu");
		g_DAB_Toggle_Zoom		= FindActionBind("toggle_zoom");
		g_DAB_Toggle_Duck		= FindActionBind("toggle_duck");

		g_AA_Move				= SteamInput()->GetAnalogActionHandle( "Move" );
		g_AA_Camera				= SteamInput()->GetAnalogActionHandle( "Camera" );
		g_AA_JoystickCamera		= SteamInput()->GetAnalogActionHandle( "JoystickCamera" );
		g_AA_Steer				= SteamInput()->GetAnalogActionHandle( "Steer" );
		g_AA_Accelerate			= SteamInput()->GetAnalogActionHandle( "Accelerate" );
		g_AA_Brake				= SteamInput()->GetAnalogActionHandle( "Brake" );
		g_AA_Mouse				= SteamInput()->GetAnalogActionHandle( "Mouse" );

		g_DAB_MenuUp.handle		= SteamInput()->GetDigitalActionHandle( "menu_up" );
		g_DAB_MenuDown.handle	= SteamInput()->GetDigitalActionHandle( "menu_down" );
		g_DAB_MenuLeft.handle	= SteamInput()->GetDigitalActionHandle( "menu_left" );
		g_DAB_MenuRight.handle	= SteamInput()->GetDigitalActionHandle( "menu_right" );
		g_DAB_MenuSelect.handle	= SteamInput()->GetDigitalActionHandle( "menu_select" );
		g_DAB_MenuCancel.handle	= SteamInput()->GetDigitalActionHandle( "menu_cancel" );
		g_DAB_MenuX.handle		= SteamInput()->GetDigitalActionHandle( "menu_x" );
		g_DAB_MenuY.handle		= SteamInput()->GetDigitalActionHandle( "menu_y" );
		g_DAB_MenuLB.handle		= SteamInput()->GetDigitalActionHandle( "menu_lb" );
		g_DAB_MenuRB.handle		= SteamInput()->GetDigitalActionHandle( "menu_rb" );
	}
	else
	{
		Msg( "SteamInput has no action binds, will not run\n" );
	}

	LoadHintRemap( "scripts/steaminput_hintremap.txt" );

	// Also load mod remap script
	LoadHintRemap( "scripts/mod_hintremap.txt" );

	g_pVGuiLocalize->AddFile( "resource/steaminput_%language%.txt" );
}

#define ACTION_MANIFEST_MOD					"steam_input/action_manifest_mod.vdf"
#define ACTION_MANIFEST_RELATIVE_HL2		"%s/../Half-Life 2/steam_input/action_manifest_hl2.vdf"
#define ACTION_MANIFEST_RELATIVE_PORTAL		"%s/../Portal/steam_input/action_manifest_hl2.vdf"

void CSource2013SteamInput::InitActionManifest()
{
	// First, check for a mod-specific action manifest
	if (g_pFullFileSystem->FileExists( ACTION_MANIFEST_MOD, "MOD" ))
	{
		char szFullPath[MAX_PATH];
		g_pFullFileSystem->RelativePathToFullPath( ACTION_MANIFEST_MOD, "MOD", szFullPath, sizeof( szFullPath ) );
		V_FixSlashes( szFullPath );

		Msg( "Loading mod action manifest file at \"%s\"\n", szFullPath );
		SteamInput()->SetInputActionManifestFilePath( szFullPath );
	}
#if USE_HL2_INSTALLATION
	else if (SteamUtils()->GetAppID() == 243730 || SteamUtils()->GetAppID() == 243750)
	{
		char szCurDir[MAX_PATH];
		g_pFullFileSystem->GetCurrentDirectory( szCurDir, sizeof( szCurDir ) );

		char szTargetApp[MAX_PATH];
		Q_snprintf( szTargetApp, sizeof( szTargetApp ), ACTION_MANIFEST_RELATIVE_HL2, szCurDir );
		V_FixSlashes( szTargetApp );

		if (g_pFullFileSystem->FileExists( szTargetApp ))
		{
			Msg( "Loading Half-Life 2 action manifest file at \"%s\"\n", szTargetApp );
			SteamInput()->SetInputActionManifestFilePath( szTargetApp );
		}
		else
		{
			// If Half-Life 2 is not installed, check if Portal has it
			Q_snprintf( szTargetApp, sizeof( szTargetApp ), ACTION_MANIFEST_RELATIVE_PORTAL, szCurDir );
			V_FixSlashes( szTargetApp );

			if (g_pFullFileSystem->FileExists( szTargetApp ))
			{
				Msg( "Loading Portal's copy of HL2 action manifest file at \"%s\"\n", szTargetApp );
				SteamInput()->SetInputActionManifestFilePath( szTargetApp );
			}
		}
	}
#endif
}

bool CSource2013SteamInput::LoadActionBinds( const char *pszFileName )
{
	{
		static InputHandle_t inputHandles[STEAM_INPUT_MAX_COUNT];
		if (SteamInput()->GetConnectedControllers( inputHandles ) <= 0)
			return false;
	}

	KeyValues *pKV = new KeyValues("ActionBinds");
	if ( pKV->LoadFromFile( g_pFullFileSystem, pszFileName ) )
	{
		// Parse each action bind
		KeyValues *pKVAction = pKV->GetFirstSubKey();
		while ( pKVAction )
		{
			InputDigitalActionHandle_t action = SteamInput()->GetDigitalActionHandle( pKVAction->GetName() );
			if ( action != 0 )
			{
				int i = g_DigitalActionBinds.AddToTail();
				g_DigitalActionBinds[i].handle = action;

				g_DigitalActionBindNames.CopyAndAddToTail( pKVAction->GetName() );
				g_DigitalActionBinds[i].pszActionName = g_DigitalActionBindNames.Tail();

				g_DigitalActionBindNames.CopyAndAddToTail( pKVAction->GetString() );
				g_DigitalActionBinds[i].pszBindCommand = g_DigitalActionBindNames.Tail();
			}
			else
			{
				Warning("Invalid Steam Input action \"%s\", won't run Steam Input without manual restart\n", pKVAction->GetName());
				m_bInvalidSteamInputAction = true;
				pKV->deleteThis();
				return false;
			}

			pKVAction = pKVAction->GetNextKey();
		}
		pKV->deleteThis();
		return true;
	}
	else
	{
		Msg( "SteamInput action bind file \"%s\" failed to load\n", pszFileName );
		pKV->deleteThis();
		return false;
	}
}

InputDigitalActionCommandBind_t *CSource2013SteamInput::FindActionBind( const char *pszActionName )
{
	for (int i = 0; i < g_DigitalActionBinds.Count(); i++)
	{
		if (!V_strcmp( pszActionName, g_DigitalActionBinds[i].pszActionName ))
			return &g_DigitalActionBinds[i];
	}

	return NULL;
}

//-------------------------------------------

void CSource2013SteamInput::PostInit()
{
	if (!m_bEnabled)
	{
		if (SteamUtils()->IsSteamRunningOnSteamDeck())
		{
			InputHandle_t inputHandles[STEAM_INPUT_MAX_COUNT];
			int iNumHandles = SteamInput()->GetConnectedControllers( inputHandles );
			Msg( "On Steam Deck and number of controllers is %i\n", iNumHandles );

			if (iNumHandles > 0)
			{
				DeckConnected( inputHandles[0] );
			}
		}
		else if (si_current_cfg.GetString()[0] != '0')
		{
			Msg("Reverting leftover Steam Input cvars\n");
			g_pEngineClient->ClientCmd_Unrestricted( "exec steam_uninput.cfg" );
			g_pEngineClient->ClientCmd_Unrestricted( SteamInput_VarArgs( "exec steam_uninput_%s.cfg", si_current_cfg.GetString() ) );

			si_current_cfg.SetValue( "0" );
		}
	}
	else
	{
		// Sometimes, the archived value overwrites the cvar. This is a compromise to make sure that doesn't happen
		ESteamInputType inputType = SteamInput()->GetInputTypeForHandle( m_nControllerHandle );
		si_current_cfg.SetValue( IdentifyControllerParam( inputType ) );
	}
}

void CSource2013SteamInput::LevelInitPreEntity()
{
}

void CSource2013SteamInput::Shutdown()
{
	SteamInput()->Shutdown();
	m_nControllerHandle = 0;

	g_DigitalActionBindNames.PurgeAndDeleteElements();
	g_DigitalActionBinds.RemoveAll();
}

//-------------------------------------------

const char *CSource2013SteamInput::IdentifyControllerParam( ESteamInputType inputType )
{
	switch (inputType)
	{
		case k_ESteamInputType_SteamDeckController:
			return "deck";
			break;
		case k_ESteamInputType_SteamController:
			return "steamcontroller";
			break;
		case k_ESteamInputType_XBox360Controller:
			return "xbox360";
			break;
		case k_ESteamInputType_XBoxOneController:
			return "xboxone";
			break;
		case k_ESteamInputType_PS3Controller:
			return "ps3";
			break;
		case k_ESteamInputType_PS4Controller:
			return "ps4";
			break;
		case k_ESteamInputType_PS5Controller:
			return "ps5";
			break;
		case k_ESteamInputType_SwitchProController:
			return "switchpro";
			break;
		case k_ESteamInputType_SwitchJoyConPair:
			return "joyconpair";
			break;
		case k_ESteamInputType_SwitchJoyConSingle:
			return "joyconsingle";
			break;
	}

	return NULL;
}

void CSource2013SteamInput::InputDeviceConnected( InputHandle_t nDeviceHandle )
{
	m_nControllerHandle = nDeviceHandle;
	m_bEnabled = true;

	g_pEngineClient->ClientCmd_Unrestricted( "exec steam_input.cfg" );

	ESteamInputType inputType = SteamInput()->GetInputTypeForHandle( m_nControllerHandle );
	const char *pszInputPrintType = IdentifyControllerParam( inputType );

	Msg( "Steam Input running with a controller (%i: %s)\n", inputType, pszInputPrintType );

	if (pszInputPrintType)
	{
		g_pEngineClient->ClientCmd_Unrestricted( SteamInput_VarArgs( "exec steam_input_%s.cfg", pszInputPrintType ) );
		si_current_cfg.SetValue( pszInputPrintType );
	}

	if (g_pEngineClient->IsConnected() )
	{
		// Refresh weapon buckets
		g_pEngineClient->ClientCmd_Unrestricted( "weapon_precache_weapon_info_database\n" );
	}
}

void CSource2013SteamInput::InputDeviceDisconnected( InputHandle_t nDeviceHandle )
{
	Msg( "Steam Input controller disconnected\n" );

	m_nControllerHandle = 0;
	m_bEnabled = false;

	g_pEngineClient->ClientCmd_Unrestricted( "exec steam_uninput.cfg" );

	const char *pszInputPrintType = NULL;
	ESteamInputType inputType = SteamInput()->GetInputTypeForHandle( nDeviceHandle );
	pszInputPrintType = IdentifyControllerParam( inputType );

	if (pszInputPrintType)
	{
		g_pEngineClient->ClientCmd_Unrestricted( SteamInput_VarArgs( "exec steam_uninput_%s.cfg", pszInputPrintType ) );
	}

	si_current_cfg.SetValue( "0" );

	if ( g_pEngineClient->IsConnected() )
	{
		// Refresh weapon buckets
		g_pEngineClient->ClientCmd_Unrestricted( "weapon_precache_weapon_info_database\n" );
	}
}

void CSource2013SteamInput::InputDeviceChanged( InputHandle_t nOldHandle, InputHandle_t nNewHandle )
{
	// Disconnect previous controller
	const char *pszInputPrintType = NULL;
	ESteamInputType inputType = SteamInput()->GetInputTypeForHandle( nOldHandle );
	pszInputPrintType = IdentifyControllerParam( inputType );

	if (pszInputPrintType)
	{
		g_pEngineClient->ClientCmd_Unrestricted( SteamInput_VarArgs( "exec steam_uninput_%s.cfg", pszInputPrintType ) );
	}

	// Connect new controller
	m_nControllerHandle = nNewHandle;

	ESteamInputType newInputType = SteamInput()->GetInputTypeForHandle( m_nControllerHandle );
	const char *pszNewInputPrintType = IdentifyControllerParam( newInputType );

	Msg( "Steam Input changing controller from %i/%s to %i/%s\n", inputType, pszInputPrintType, newInputType, pszNewInputPrintType );

	if (pszNewInputPrintType)
	{
		g_pEngineClient->ClientCmd_Unrestricted( SteamInput_VarArgs( "exec steam_input_%s.cfg", pszNewInputPrintType ) );
		si_current_cfg.SetValue( pszNewInputPrintType );
	}
}

void CSource2013SteamInput::DeckConnected( InputHandle_t nDeviceHandle )
{
	Msg( "Steam Input running with a Steam Deck\n" );

	m_nControllerHandle = nDeviceHandle;
	m_bEnabled = true;

	g_pEngineClient->ClientCmd_Unrestricted( "exec steam_input.cfg" );
	g_pEngineClient->ClientCmd_Unrestricted( "exec steam_input_deck.cfg" );
	si_current_cfg.SetValue( "deck" );
}

//-------------------------------------------

void CSource2013SteamInput::RunFrame( ActionSet_t &iActionSet )
{
	SteamInput()->RunFrame();

	static InputHandle_t inputHandles[STEAM_INPUT_MAX_COUNT];
	int iNumHandles = SteamInput()->GetConnectedControllers( inputHandles );

	//Msg( "Number of handles is %i!!! (inputHandles[0] is %llu, m_nControllerHandle is %llu)\n", iNumHandles, inputHandles[0], m_nControllerHandle );

	if (iNumHandles <= 0)
	{
		if (IsEnabled())
		{
			// No controllers available, disable Steam Input
			InputDeviceDisconnected( m_nControllerHandle );
		}
		return;
	}

	if (g_DigitalActionBinds.Count() == 0)
	{
		if (!m_bInvalidSteamInputAction && iNumHandles > 0 && m_nControllerHandle == 0)
		{
			// A new controller may have been connected without proper Steam Input initialization, so restart it
			Shutdown();
			InitSteamInput();
		}
		return;
	}

	//if (!SteamInput()->BNewDataAvailable())
	//	return;

	// Reset the analog data
	m_analogMoveData = m_analogCameraData = InputAnalogActionData_t();

	InputHandle_t iFirstActive = m_nControllerHandle;
	bool bActiveInput = false;
	for (int i = 0; i < iNumHandles; i++)
	{
		if (TestActions( iActionSet, inputHandles[i] ))
		{
			bActiveInput = true;
			if (iFirstActive == m_nControllerHandle)
				iFirstActive = inputHandles[i];
		}
	}

	if (iFirstActive != m_nControllerHandle)
	{
		// Disconnect previous controller if its inputs are not active
		if (m_nControllerHandle != 0)
		{
			InputDeviceChanged( m_nControllerHandle, iFirstActive );
		}
		else
		{
			// Register the new controller
			InputDeviceConnected( iFirstActive );
		}
	}

	if (bActiveInput)
	{
		if (m_iLastActionSet != iActionSet)
		{
			TransitionActionSet( iActionSet, m_iLastActionSet );
		}
		else
		{
			DoActions( iActionSet );
		}

		m_iLastActionSet = iActionSet;

		if (si_print_action_set.GetBool())
		{
			switch (iActionSet)
			{
				case AS_GameControls:
					Msg( "Steam Input: GameControls\n" );
					break;

				case AS_VehicleControls:
					Msg( "Steam Input: VehicleControls\n" );
					break;

				case AS_MenuControls:
					Msg( "Steam Input: MenuControls\n" );
					break;
			}
		}
	}
}

bool CSource2013SteamInput::IsEnabled()
{
	return m_bEnabled;
}

bool CSource2013SteamInput::IsSteamRunningOnSteamDeck()
{
	return SteamUtils()->IsSteamRunningOnSteamDeck();
}

void CSource2013SteamInput::SetGamepadUI( bool bToggle )
{
	m_bIsGamepadUI = bToggle;
}

InputHandle_t CSource2013SteamInput::GetActiveController()
{
	return m_nControllerHandle;
}

int CSource2013SteamInput::GetConnectedControllers( InputHandle_t *nOutHandles )
{
	if (_ARRAYSIZE( nOutHandles ) < STEAM_INPUT_MAX_COUNT)
	{
		Warning( "ISource2013SteamInput::GetConnectedControllers requires an array greater than or equal to STEAM_INPUT_MAX_COUNT (%i) in size\n", STEAM_INPUT_MAX_COUNT );
		return 0;
	}

	return SteamInput()->GetConnectedControllers( nOutHandles );
}

const char *CSource2013SteamInput::GetControllerName( InputHandle_t nController )
{
	ESteamInputType inputType = SteamInput()->GetInputTypeForHandle( nController );
	return IdentifyControllerParam( inputType );
}

int CSource2013SteamInput::GetControllerType( InputHandle_t nController )
{
	return SteamInput()->GetInputTypeForHandle( nController );
}

bool CSource2013SteamInput::ShowBindingPanel( InputHandle_t nController )
{
	return SteamInput()->ShowBindingPanel( nController );
}

bool CSource2013SteamInput::ShowGamepadTextInput( const char *pszDescription, uint32 nMaxChars, bool bMultiline, const char *pszExistingText )
{
	return SteamUtils()->ShowGamepadTextInput( k_EGamepadTextInputModeNormal, bMultiline ? k_EGamepadTextInputLineModeMultipleLines : k_EGamepadTextInputLineModeSingleLine, pszDescription, nMaxChars, pszExistingText );
}

bool CSource2013SteamInput::ShowFloatingGamepadTextInput( bool bMultiline, int nX, int nY, int nW, int nH )
{
	return SteamUtils()->ShowFloatingGamepadTextInput( bMultiline ? k_EFloatingGamepadTextInputModeModeMultipleLines : k_EFloatingGamepadTextInputModeModeSingleLine, nX, nY, nW, nH );
}

bool CSource2013SteamInput::GetEnteredGamepadTextInput( char *pszText, uint32 nTextSize )
{
	return SteamUtils()->GetEnteredGamepadTextInput( pszText, nTextSize );
}

uint32 CSource2013SteamInput::GetEnteredGamepadTextLength()
{
	return SteamUtils()->GetEnteredGamepadTextLength();
}

bool CSource2013SteamInput::TestActions( int iActionSet, InputHandle_t nController )
{
	bool bActiveInput = false;

	switch (iActionSet)
	{
		case AS_GameControls:
		{
			SteamInput()->ActivateActionSet( nController, g_AS_GameControls );
			
			// Run commands for all digital actions
			for (int i = 0; i < g_DigitalActionBinds.Count(); i++)
			{
				TestDigitalActionBind( nController, g_DigitalActionBinds[i], bActiveInput );
			}

			InputAnalogActionData_t moveData = SteamInput()->GetAnalogActionData( nController, g_AA_Move );
			if (!m_analogMoveData.bActive)
			{
				m_analogMoveData = moveData;
			}
			else if (m_analogMoveData.eMode == moveData.eMode)
			{
				// Just add on to existing input
				m_analogMoveData.x += moveData.x;
				m_analogMoveData.y += moveData.y;
			}

			if (moveData.x != 0.0f || moveData.y != 0.0f)
			{
				bActiveInput = true;
			}

		} break;

		case AS_VehicleControls:
		{
			SteamInput()->ActivateActionSet( nController, g_AS_VehicleControls );
			
			// Run commands for all digital actions
			for (int i = 0; i < g_DigitalActionBinds.Count(); i++)
			{
				TestDigitalActionBind( nController, g_DigitalActionBinds[i], bActiveInput );
			}

			InputAnalogActionData_t moveData = SteamInput()->GetAnalogActionData( nController, g_AA_Move );
			if (!m_analogMoveData.bActive)
			{
				m_analogMoveData = moveData;
			}
			else if (m_analogMoveData.eMode == moveData.eMode)
			{
				// Just add on to existing input
				m_analogMoveData.x += moveData.x;
				m_analogMoveData.y += moveData.y;
			}

			// Add steer data to the X value
			InputAnalogActionData_t steerData = SteamInput()->GetAnalogActionData( nController, g_AA_Steer );
			m_analogMoveData.x += steerData.x;

			// Add acceleration to the Y value
			steerData = SteamInput()->GetAnalogActionData( nController, g_AA_Accelerate );
			m_analogMoveData.y += steerData.x;

			if (g_DAB_Brake->bDown == false)
			{
				// For now, braking is equal to the digital action
				steerData = SteamInput()->GetAnalogActionData( nController, g_AA_Brake );
				if (steerData.x >= 0.25f)
				{
					g_pEngineClient->ClientCmd_Unrestricted( "+jump" );
				}
				else
				{
					g_pEngineClient->ClientCmd_Unrestricted( "-jump" );
				}
			}

			if (moveData.x != 0.0f || moveData.y != 0.0f ||
				steerData.x != 0.0f || steerData.y != 0.0f)
			{
				bActiveInput = true;
			}

		} break;

		case AS_MenuControls:
		{
			SteamInput()->ActivateActionSet( nController, g_AS_MenuControls );

			TestDigitalActionBind( nController, g_DAB_MenuUp, bActiveInput );
			TestDigitalActionBind( nController, g_DAB_MenuDown, bActiveInput );
			TestDigitalActionBind( nController, g_DAB_MenuLeft, bActiveInput );
			TestDigitalActionBind( nController, g_DAB_MenuRight, bActiveInput );
			TestDigitalActionBind( nController, g_DAB_MenuSelect, bActiveInput );
			TestDigitalActionBind( nController, g_DAB_MenuCancel, bActiveInput );
			TestDigitalActionBind( nController, g_DAB_MenuX, bActiveInput );
			TestDigitalActionBind( nController, g_DAB_MenuY, bActiveInput );
			TestDigitalActionBind( nController, g_DAB_MenuLB, bActiveInput );
			TestDigitalActionBind( nController, g_DAB_MenuRB, bActiveInput );

			TestDigitalActionBind( nController, *g_DAB_MenuPause, bActiveInput );

		} break;
	}

	if (iActionSet != AS_MenuControls)
	{
		InputAnalogActionData_t cameraData = SteamInput()->GetAnalogActionData( nController, g_AA_Camera );
		InputAnalogActionData_t cameraJoystickData = SteamInput()->GetAnalogActionData( nController, g_AA_JoystickCamera );

		if (cameraJoystickData.bActive)
		{
			cameraData = cameraJoystickData;
		}

		if (!m_analogCameraData.bActive)
		{
			m_analogCameraData = cameraData;
		}
		else if (m_analogCameraData.eMode == cameraData.eMode)
		{
			// Just add on to existing input
			m_analogCameraData.x += cameraData.x;
			m_analogCameraData.y += cameraData.y;
		}

		if (cameraData.x != 0.0f || cameraData.y != 0.0f)
		{
			bActiveInput = true;
		}
	}

	//Msg( "Active input on %llu is %s\n", nController, bActiveInput ? "true" : "false" );

	return bActiveInput;
}

void CSource2013SteamInput::DoActions( int iActionSet )
{
	switch (iActionSet)
	{
		case AS_VehicleControls:
		case AS_GameControls:
		{
			// Run commands for all digital actions
			for (int i = 0; i < g_DigitalActionBinds.Count(); i++)
			{
				SendDigitalActionBind( g_DigitalActionBinds[i] );
			}
		} break;

		case AS_MenuControls:
		{
			if (m_bIsGamepadUI)
			{
				SendKeyFromDigitalActionHandle( g_DAB_MenuUp, KEY_XBUTTON_UP );
				SendKeyFromDigitalActionHandle( g_DAB_MenuDown, KEY_XBUTTON_DOWN );
				SendKeyFromDigitalActionHandle( g_DAB_MenuLeft, KEY_XBUTTON_LEFT );
				SendKeyFromDigitalActionHandle( g_DAB_MenuRight, KEY_XBUTTON_RIGHT );
				SendKeyFromDigitalActionHandle( g_DAB_MenuSelect, KEY_XBUTTON_A );
				SendKeyFromDigitalActionHandle( g_DAB_MenuCancel, KEY_XBUTTON_B );
				SendKeyFromDigitalActionHandle( g_DAB_MenuX, KEY_XBUTTON_X );
				SendKeyFromDigitalActionHandle( g_DAB_MenuY, KEY_XBUTTON_Y );
				SendKeyFromDigitalActionHandle( g_DAB_MenuLB, KEY_XBUTTON_LEFT_SHOULDER );
				SendKeyFromDigitalActionHandle( g_DAB_MenuRB, KEY_XBUTTON_RIGHT_SHOULDER );
			}
			else
			{
				SendKeyFromDigitalActionHandle( g_DAB_MenuUp, KEY_UP ); // KEY_XBUTTON_UP
				SendKeyFromDigitalActionHandle( g_DAB_MenuDown, KEY_DOWN ); // KEY_XBUTTON_DOWN
				SendKeyFromDigitalActionHandle( g_DAB_MenuLeft, KEY_LEFT ); // KEY_XBUTTON_LEFT
				SendKeyFromDigitalActionHandle( g_DAB_MenuRight, KEY_RIGHT ); // KEY_XBUTTON_RIGHT
				SendKeyFromDigitalActionHandle( g_DAB_MenuSelect, KEY_XBUTTON_A );
				SendKeyFromDigitalActionHandle( g_DAB_MenuCancel, KEY_XBUTTON_B );
				SendKeyFromDigitalActionHandle( g_DAB_MenuLB, KEY_XBUTTON_LEFT ); // KEY_XBUTTON_LEFT_SHOULDER
				SendKeyFromDigitalActionHandle( g_DAB_MenuRB, KEY_XBUTTON_RIGHT ); // KEY_XBUTTON_RIGHT_SHOULDER
			}

			SendDigitalActionBind( *g_DAB_MenuPause );

			if (!m_bIsGamepadUI)
			{
				if (g_DAB_MenuY.bDown && g_DAB_MenuY.bQueue)
				{
					g_pEngineClient->ClientCmd_Unrestricted( "gamemenucommand OpenOptionsDialog\n" );
					g_DAB_MenuY.bQueue = false;
				}
			}

		} break;
	}
}

void CSource2013SteamInput::TransitionActionSet( int iNewActionSet, int iOldActionSet )
{
	if (iOldActionSet == AS_MenuControls)
	{
		// Deactivate all menu controls
		DeactivateDigitalActionBind( g_DAB_MenuUp );
		DeactivateDigitalActionBind( g_DAB_MenuDown );
		DeactivateDigitalActionBind( g_DAB_MenuLeft );
		DeactivateDigitalActionBind( g_DAB_MenuRight );
		DeactivateDigitalActionBind( g_DAB_MenuSelect );
		DeactivateDigitalActionBind( g_DAB_MenuCancel );
		DeactivateDigitalActionBind( g_DAB_MenuX );
		DeactivateDigitalActionBind( g_DAB_MenuY );
		DeactivateDigitalActionBind( g_DAB_MenuLB );
		DeactivateDigitalActionBind( g_DAB_MenuRB );
		
		// Stop queueing any actions which were immediately pressed after unpausing (i.e. pressing A to unpause shouldn't cause the player to jump immediately after)
		for (int i = 0; i < g_DigitalActionBinds.Count(); i++)
		{
			g_DigitalActionBinds[i].bQueue = false;
		}
	}
	else if (iNewActionSet == AS_MenuControls)
	{
		// Deactivate all regular actions
		for (int i = 0; i < g_DigitalActionBinds.Count(); i++)
		{
			if (&g_DigitalActionBinds[i] != g_DAB_MenuPause)
				DeactivateDigitalActionBind( g_DigitalActionBinds[i] );
		}

		// Stop queueing any actions which were immediately pressed after unpausing (i.e. pressing A to unpause shouldn't cause the player to jump immediately after)
		g_DAB_MenuUp.bQueue = false;
		g_DAB_MenuDown.bQueue = false;
		g_DAB_MenuLeft.bQueue = false;
		g_DAB_MenuRight.bQueue = false;
		g_DAB_MenuSelect.bQueue = false;
		g_DAB_MenuCancel.bQueue = false;
		g_DAB_MenuX.bQueue = false;
		g_DAB_MenuY.bQueue = false;
		g_DAB_MenuLB.bQueue = false;
		g_DAB_MenuRB.bQueue = false;
	}
}

void CSource2013SteamInput::TestDigitalActionBind( InputHandle_t nController, InputDigitalActionBind_t &DigitalAction, bool &bActiveInput )
{
	InputDigitalActionData_t data = SteamInput()->GetDigitalActionData( nController, DigitalAction.handle );

	if (data.bState)
	{
		// Key is not down
		if (!DigitalAction.bDown)
		{
			DigitalAction.controller = nController;
			DigitalAction.bDown = true;
			DigitalAction.bQueue = true; //DigitalAction.OnDown();
		}

		if (DigitalAction.controller == nController)
			bActiveInput = true;
	}
	else if (DigitalAction.controller == nController)
	{
		// Key was already down on this controller
		if (DigitalAction.bDown)
		{
			DigitalAction.bDown = false;
			DigitalAction.bQueue = true; //DigitalAction.OnUp();
			bActiveInput = true;
		}
	}
}

void CSource2013SteamInput::SendDigitalActionBind( InputDigitalActionBind_t &DigitalAction )
{
	if (DigitalAction.bQueue)
	{
		if (DigitalAction.bDown)
			DigitalAction.OnDown();
		else
			DigitalAction.OnUp();

		DigitalAction.bQueue = false;
	}
}

void CSource2013SteamInput::DeactivateDigitalActionBind( InputDigitalActionBind_t &DigitalAction )
{
	if (DigitalAction.bDown)
	{
		DigitalAction.bDown = false;
		DigitalAction.OnUp();
		DigitalAction.bQueue = false;
	}
}

void CSource2013SteamInput::SendKeyFromDigitalActionHandle( InputDigitalActionBind_t &nHandle, ButtonCode_t nKey )
{
	if (nHandle.bQueue)
	{
		if (nHandle.bDown)
			vgui::ivgui()->PostMessage( vgui::input()->GetFocus(), new KeyValues( "KeyCodePressed", "code", nKey ), NULL );
		else
			vgui::ivgui()->PostMessage( vgui::input()->GetFocus(), new KeyValues( "KeyCodeReleased", "code", nKey ), NULL );
	}

	nHandle.bQueue = false;
}

static inline bool IsRelativeAnalog( EInputSourceMode mode )
{
	// TODO: Is there a better way of doing this?
	return mode == k_EInputSourceMode_AbsoluteMouse ||
		mode == k_EInputSourceMode_RelativeMouse ||
		mode == k_EInputSourceMode_JoystickMouse ||
		mode == k_EInputSourceMode_MouseRegion;
}

bool CSource2013SteamInput::UsingJoysticks()
{
	// For now, any controller uses joysticks
	return IsEnabled();
}

void CSource2013SteamInput::GetJoystickValues( float &flForward, float &flSide, float &flPitch, float &flYaw,
	bool &bRelativeForward, bool &bRelativeSide, bool &bRelativePitch, bool &bRelativeYaw )
{

	if (IsRelativeAnalog( m_analogMoveData.eMode ))
	{
		bRelativeForward = true;
		bRelativeSide = true;

		flForward = (m_analogMoveData.y / 180.0f) * MAX_BUTTONSAMPLE;
		flSide = (m_analogMoveData.x / 180.0f) * MAX_BUTTONSAMPLE;
	}
	else
	{
		bRelativeForward = false;
		bRelativeSide = false;

		flForward = m_analogMoveData.y * MAX_BUTTONSAMPLE;
		flSide = m_analogMoveData.x * MAX_BUTTONSAMPLE;
	}
	
	if (IsRelativeAnalog( m_analogCameraData.eMode ))
	{
		bRelativePitch = true;
		bRelativeYaw = true;

		flPitch = (m_analogCameraData.y / 180.0f) * MAX_BUTTONSAMPLE;
		flYaw = (m_analogCameraData.x / 180.0f) * MAX_BUTTONSAMPLE;
	}
	else
	{
		bRelativePitch = false;
		bRelativeYaw = false;

		flPitch = m_analogCameraData.y * MAX_BUTTONSAMPLE;
		flYaw = m_analogCameraData.x * MAX_BUTTONSAMPLE;
	}

	if (si_print_joy_src.GetBool())
	{
		Msg( "moveData = %i (%f, %f)\ncameraData = %i (%f, %f)\n\n",
			m_analogMoveData.eMode, m_analogMoveData.x, m_analogMoveData.y,
			m_analogCameraData.eMode, m_analogCameraData.x, m_analogCameraData.y );
	}
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

void CSource2013SteamInput::SetRumble( InputHandle_t nController, float fLeftMotor, float fRightMotor, int userId )
{
	if (!IsEnabled() || !si_enable_rumble.GetBool())
	{
		g_pInputSystem->SetRumble( fLeftMotor, fRightMotor, userId );
		return;
	}

	if (nController == 0)
		nController = m_nControllerHandle;

	SteamInput()->TriggerVibrationExtended( nController, fLeftMotor, fRightMotor, fLeftMotor, fRightMotor );

	if (si_print_rumble.GetBool())
	{
		Msg( "fLeftMotor = %f, fRightMotor = %f\n\n", fLeftMotor, fRightMotor );
	}
}

void CSource2013SteamInput::StopRumble()
{
	if (!IsEnabled())
	{
		g_pInputSystem->StopRumble();
		return;
	}

	// N/A
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

void CSource2013SteamInput::SetLEDColor( InputHandle_t nController, byte r, byte g, byte b )
{
	SteamInput()->SetLEDColor( nController, r, g, b, k_ESteamInputLEDFlag_SetColor );
}

void CSource2013SteamInput::ResetLEDColor( InputHandle_t nController )
{
	SteamInput()->SetLEDColor( nController, 0, 0, 0, k_ESteamInputLEDFlag_RestoreUserDefault );
}

void SetDualSenseTriggerEffect(InputHandle_t nController, const ScePadTriggerEffectParam *pParam)
{
    SteamInput()->SetDualSenseTriggerEffect(nController, pParam);
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

int CSource2013SteamInput::FindDigitalActionsForCommand( const char *pszCommand, InputDigitalActionHandle_t *pHandles )
{
	int iNumHandles = 0;

	if (m_iLastActionSet == AS_MenuControls)
	{
		pszCommand += 5;
		if (!V_strcmp( pszCommand, "up" ))		{ pHandles[0] = g_DAB_MenuUp.handle; return 1; }
		if (!V_strcmp( pszCommand, "down" ))	{ pHandles[0] = g_DAB_MenuDown.handle; return 1; }
		if (!V_strcmp( pszCommand, "left" ))	{ pHandles[0] = g_DAB_MenuLeft.handle; return 1; }
		if (!V_strcmp( pszCommand, "right" ))	{ pHandles[0] = g_DAB_MenuRight.handle; return 1; }
		if (!V_strcmp( pszCommand, "select" ))	{ pHandles[0] = g_DAB_MenuSelect.handle; return 1; }
		if (!V_strcmp( pszCommand, "cancel" ))	{ pHandles[0] = g_DAB_MenuCancel.handle; return 1; }
		if (!V_strcmp( pszCommand, "x" ))		{ pHandles[0] = g_DAB_MenuX.handle; return 1; }
		if (!V_strcmp( pszCommand, "y" ))		{ pHandles[0] = g_DAB_MenuY.handle; return 1; }
		if (!V_strcmp( pszCommand, "lb" ))		{ pHandles[0] = g_DAB_MenuLB.handle; return 1; }
		if (!V_strcmp( pszCommand, "rb" ))		{ pHandles[0] = g_DAB_MenuRB.handle; return 1; }
	}

	// Special cases
	if (!V_strcmp( pszCommand, "duck" ))
	{
		// Add toggle_duck
		pHandles[iNumHandles] = g_DAB_Toggle_Duck->handle;
		iNumHandles++;
	}
	else if (!V_strcmp( pszCommand, "zoom" ))
	{
		// Add toggle_zoom
		pHandles[iNumHandles] = g_DAB_Toggle_Zoom->handle;
		iNumHandles++;
	}

	// Figure out which command this is
	for (int i = 0; i < g_DigitalActionBinds.Count(); i++)
	{
		const char *pszBindCommand = g_DigitalActionBinds[i].pszBindCommand;
		if (pszBindCommand[0] == '+')
			pszBindCommand++;

		if (!V_strcmp( pszCommand, pszBindCommand ))
		{
			pHandles[iNumHandles] = g_DigitalActionBinds[i].handle;
			iNumHandles++;
			break;
		}
	}

	return iNumHandles;
}

int CSource2013SteamInput::FindAnalogActionsForCommand( const char *pszCommand, InputAnalogActionHandle_t *pHandles )
{
	int iNumHandles = 0;

	// Check pre-set analog action names
	if (!V_strcmp( pszCommand, "xlook" ))
	{
		// Add g_AA_Camera and g_AA_JoystickCamera
		pHandles[iNumHandles] = g_AA_Camera;
		iNumHandles++;
		pHandles[iNumHandles] = g_AA_JoystickCamera;
		iNumHandles++;
	}
	else if (!V_strcmp( pszCommand, "xaccel" ))
	{
		// Add g_AA_Accelerate and g_AA_Move
		pHandles[iNumHandles] = g_AA_Accelerate;
		iNumHandles++;
		pHandles[iNumHandles] = g_AA_Move;
		iNumHandles++;
	}
	else if (!V_strcmp( pszCommand, "xmove" ) )
	{
		// Add g_AA_Accelerate and g_AA_Move
		pHandles[iNumHandles] = g_AA_Accelerate;
		iNumHandles++;
		pHandles[iNumHandles] = g_AA_Move;
		iNumHandles++;
	}
	else if (!V_strcmp( pszCommand, "xsteer" ))
	{
		pHandles[iNumHandles] = g_AA_Steer;
		iNumHandles++;
	}
	else if (!V_strcmp( pszCommand, "xbrake" ))
	{
		pHandles[iNumHandles] = g_AA_Brake;
		iNumHandles++;
	}
	else if (!V_strcmp( pszCommand, "xmouse" ))
	{
		pHandles[iNumHandles] = g_AA_Mouse;
		iNumHandles++;
	}

	return iNumHandles;
}

void CSource2013SteamInput::GetInputActionOriginsForCommand( const char *pszCommand, CUtlVector<EInputActionOrigin> &actionOrigins, int iActionSetOverride )
{
	InputActionSetHandle_t actionSet = g_AS_MenuControls;

	if (iActionSetOverride != -1)
	{
		switch (iActionSetOverride)
		{
			default:
			case AS_GameControls:		actionSet = g_AS_GameControls; break;
			case AS_VehicleControls:	actionSet = g_AS_VehicleControls; break;
			//case AS_MenuControls:		actionSet = g_AS_MenuControls; break;
		}
	}
	else
	{
		switch (m_iLastActionSet)
		{
			case AS_GameControls:		actionSet = g_AS_GameControls; break;
			case AS_VehicleControls:	actionSet = g_AS_VehicleControls; break;
		}
	}

	InputDigitalActionHandle_t digitalActions[STEAM_INPUT_MAX_ORIGINS];
	int iNumActions = FindDigitalActionsForCommand( pszCommand, digitalActions );
	if (iNumActions > 0)
	{
		for (int i = 0; i < iNumActions; i++)
		{
			EInputActionOrigin actionOriginsLocal[STEAM_INPUT_MAX_ORIGINS];
			int iNumOriginsLocal = SteamInput()->GetDigitalActionOrigins( m_nControllerHandle, actionSet, digitalActions[i], actionOriginsLocal );

			if (iNumOriginsLocal > 0)
			{
				// Add them to the list
				actionOrigins.AddMultipleToTail( iNumOriginsLocal, actionOriginsLocal );

				//memcpy( actionOrigins+iNumOrigins, actionOriginsLocal, sizeof(EInputActionOrigin)*iNumOriginsLocal );
				//iNumOrigins += iNumOriginsLocal;
			}
		}
	}
	else
	{
		InputAnalogActionHandle_t analogActions[STEAM_INPUT_MAX_ORIGINS];
		iNumActions = FindAnalogActionsForCommand( pszCommand, analogActions );
		for (int i = 0; i < iNumActions; i++)
		{
			EInputActionOrigin actionOriginsLocal[STEAM_INPUT_MAX_ORIGINS];
			int iNumOriginsLocal = SteamInput()->GetAnalogActionOrigins( m_nControllerHandle, actionSet, analogActions[i], actionOriginsLocal );

			if (iNumOriginsLocal > 0)
			{
				// Add them to the list
				actionOrigins.AddMultipleToTail( iNumOriginsLocal, actionOriginsLocal );

				//memcpy( actionOrigins+iNumOrigins, actionOriginsLocal, sizeof(EInputActionOrigin)*iNumOriginsLocal );
				//iNumOrigins += iNumOriginsLocal;
			}
		}
	}
}

/*void CSource2013SteamInput::GetGlyphPNGsForCommand( CUtlVector<const char *> &szImgList, const char *pszCommand, int &iSize, int iStyle )
{
	if (pszCommand[0] == '+')
		pszCommand++;

	CUtlVector<EInputActionOrigin> actionOrigins;
	GetInputActionOriginsForCommand( pszCommand, actionOrigins );

	ESteamInputGlyphSize glyphSize;
	if (iSize <= 32)
	{
		glyphSize = k_ESteamInputGlyphSize_Small;
		iSize = 32;
	}
	else if (iSize <= 128)
	{
		glyphSize = k_ESteamInputGlyphSize_Medium;
		iSize = 128;
	}
	else
	{
		glyphSize = k_ESteamInputGlyphSize_Large;
		iSize = 256;
	}

	FOR_EACH_VEC( actionOrigins, i )
	{
		if (si_force_glyph_controller.GetInt() != -1)
		{
			EInputActionOrigin translatedOrigin = SteamInput()->TranslateActionOrigin( (ESteamInputType)si_force_glyph_controller.GetInt(), actionOrigins[i] );
			if (translatedOrigin != k_EInputActionOrigin_None)
				actionOrigins[i] = translatedOrigin;
		}

		szImgList.AddToTail( SteamInput()->GetGlyphPNGForActionOrigin( actionOrigins[i], glyphSize, (ESteamInputGlyphStyle)iStyle ) );
	}
}

void CSource2013SteamInput::GetGlyphSVGsForCommand( CUtlVector<const char *> &szImgList, const char *pszCommand )
{
	if (pszCommand[0] == '+')
		pszCommand++;

	CUtlVector<EInputActionOrigin> actionOrigins;
	GetInputActionOriginsForCommand( pszCommand, actionOrigins );

	FOR_EACH_VEC( actionOrigins, i )
	{
		if (si_force_glyph_controller.GetInt() != -1)
		{
			actionOrigins[i] = SteamInput()->TranslateActionOrigin( (ESteamInputType)si_force_glyph_controller.GetInt(), actionOrigins[i] );
		}

		szImgList.AddToTail( SteamInput()->GetGlyphSVGForActionOrigin( actionOrigins[i], 0 ) );
	}
}*/

void CSource2013SteamInput::GetButtonStringsForCommand( const char *pszCommand, CUtlVector<const char*> &szStringList, int iActionSet )
{
	if (pszCommand[0] == '+')
		pszCommand++;

	CUtlVector<EInputActionOrigin> actionOrigins;
	GetInputActionOriginsForCommand( pszCommand, actionOrigins, iActionSet );

	for (int i = 0; i < actionOrigins.Count(); i++)
	{
		szStringList.AddToTail( LookupDescriptionForActionOrigin( actionOrigins[i] ) );
	}
}

inline const char *CSource2013SteamInput::LookupDescriptionForActionOrigin( EInputActionOrigin eAction )
{
	return SteamInput()->GetStringForActionOrigin( eAction );
}

//-----------------------------------------------------------------------------

void CSource2013SteamInput::LoadHintRemap( const char *pszFileName )
{
	KeyValues *pKV = new KeyValues("HintRemap");
	if ( pKV->LoadFromFile( g_pFullFileSystem, pszFileName ) )
	{
		// Parse each hint to remap
		KeyValues *pKVHint = pKV->GetFirstSubKey();
		while ( pKVHint )
		{
			// Parse hint remaps
			KeyValues *pKVRemappedHint = pKVHint->GetFirstSubKey();
			while ( pKVRemappedHint )
			{
				int i = m_HintRemaps.AddToTail();
			
				m_HintRemaps[i].pszOldHint = pKVHint->GetName();
				if (m_HintRemaps[i].pszOldHint[0] == '#')
					m_HintRemaps[i].pszOldHint++;

				m_HintRemaps[i].pszNewHint = pKVRemappedHint->GetName();

				// Parse remap conditions
				KeyValues *pKVRemapCond = pKVRemappedHint->GetFirstSubKey();
				while ( pKVRemapCond )
				{
					int i2 = m_HintRemaps[i].nRemapConds.AddToTail();

					const char *pszParam = pKVRemapCond->GetString();
					if (pszParam[0] == '!')
					{
						m_HintRemaps[i].nRemapConds[i2].bNot = true;
						pszParam++;
					}

					Q_strncpy( m_HintRemaps[i].nRemapConds[i2].szParam, pszParam, sizeof( m_HintRemaps[i].nRemapConds[i2].szParam ) );

					if (!V_strcmp( pKVRemapCond->GetName(), "if_input_type" ))
						m_HintRemaps[i].nRemapConds[i2].iType = HINTREMAPCOND_INPUT_TYPE;
					else if (!V_strcmp( pKVRemapCond->GetName(), "if_action_bound" ))
						m_HintRemaps[i].nRemapConds[i2].iType = HINTREMAPCOND_ACTION_BOUND;
					else
						m_HintRemaps[i].nRemapConds[i2].iType = HINTREMAPCOND_NONE;

					pKVRemapCond = pKVRemapCond->GetNextKey();
				}

				pKVRemappedHint = pKVRemappedHint->GetNextKey();
			}

			pKVHint = pKVHint->GetNextKey();
		}
	}
	pKV->deleteThis();
}

void CSource2013SteamInput::RemapHudHint( const char **ppszInputHint )
{
	if (!si_hintremap.GetBool())
		return;

	const char *pszInputHint = *ppszInputHint;
	if (pszInputHint[0] == '#')
		pszInputHint++;

	int iRemap = -1;

	for (int i = 0; i < m_HintRemaps.Count(); i++)
	{
		if (V_strcmp( pszInputHint, m_HintRemaps[i].pszOldHint ))
			continue;

		// If we've already selected a remap, ignore ones without conditions
		if (iRemap != -1 && m_HintRemaps[i].nRemapConds.Count() <= 0)
			continue;

		if (si_print_hintremap.GetBool())
			Msg( "Hint Remap: Testing hint remap for %s to %s...\n", pszInputHint, m_HintRemaps[i].pszNewHint );

		bool bPass = true;

		for (int i2 = 0; i2 < m_HintRemaps[i].nRemapConds.Count(); i2++)
		{
			if (si_print_hintremap.GetBool())
				Msg( "	Hint Remap: Testing remap condition %i (param %s)\n", m_HintRemaps[i].nRemapConds[i2].iType, m_HintRemaps[i].nRemapConds[i2].szParam );

			switch (m_HintRemaps[i].nRemapConds[i2].iType)
			{
				case HINTREMAPCOND_INPUT_TYPE:
				{
					ESteamInputType inputType = SteamInput()->GetInputTypeForHandle( m_nControllerHandle );
					bPass = !V_strcmp( IdentifyControllerParam( inputType ), m_HintRemaps[i].nRemapConds[i2].szParam );
				} break;

				case HINTREMAPCOND_ACTION_BOUND:
				{
					CUtlVector<EInputActionOrigin> actionOrigins;
					GetInputActionOriginsForCommand( m_HintRemaps[i].nRemapConds[i2].szParam, actionOrigins );
					bPass = (actionOrigins.Count() > 0);
				} break;
			}

			if (m_HintRemaps[i].nRemapConds[i2].bNot)
				bPass = !bPass;

			if (!bPass)
				break;
		}

		if (bPass)
		{
			if (si_print_hintremap.GetBool())
				Msg( "Hint Remap: Hint remap for %s to %s succeeded\n", pszInputHint, m_HintRemaps[i].pszNewHint );

			iRemap = i;
		}
		else if (si_print_hintremap.GetBool())
		{
			Msg( "Hint Remap: Hint remap for %s to %s did not pass\n", pszInputHint, m_HintRemaps[i].pszNewHint );
		}
	}

	if (iRemap != -1)
	{
		if (si_print_hintremap.GetBool())
			Msg( "Hint Remap: Remapping hint %s to %s\n", pszInputHint, m_HintRemaps[iRemap].pszNewHint );

		*ppszInputHint = m_HintRemaps[iRemap].pszNewHint;
	}
	else
	{
		if (si_print_hintremap.GetBool())
			Msg( "Hint Remap: Didn't find a hint for %s to remap to\n", pszInputHint );
	}
}
