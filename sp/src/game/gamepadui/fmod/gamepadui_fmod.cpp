#include "gamepadui_interface.h"
#include "gamepadui_fmod.h"
#include "filesystem.h"

#include <fmod_errors.h>

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

// This is the global volume ConVar, this is a bit of a hack so we don't have to use ConVarRef
ConVar volume( "volume", "1.0", FCVAR_ARCHIVE );

// This is the global music volume ConVar, this is a bit of a hack so we don't have to use ConVarRef
ConVar snd_musicvolume( "snd_musicvolume", "1.0", FCVAR_ARCHIVE );

FMOD_RESULT F_CALLBACK USER_FMOD_FILE_OPEN_CALLBACK( const char *name, unsigned int *filesize, void **handle, void *userdata )
{
	FileHandle_t fileHandle = g_pFullFileSystem->Open( name, "rb", nullptr );

	// Same as checking if NULL
	if ( fileHandle == FILESYSTEM_INVALID_HANDLE )
		return CGamepadUIFMODManager::CheckError( FMOD_ERR_FILE_NOTFOUND );

	*handle = fileHandle;
	*filesize = g_pFullFileSystem->Size( fileHandle );

	return FMOD_OK;
}

FMOD_RESULT F_CALLBACK USER_FMOD_FILE_CLOSE_CALLBACK( void *handle, void *userdata )
{
	FileHandle_t fileHandle = handle;
	g_pFullFileSystem->Close( fileHandle );

	return FMOD_OK;
}

FMOD_RESULT F_CALLBACK USER_FMOD_FILE_READ_CALLBACK( void *handle, void *buffer, unsigned int sizebytes, unsigned int *bytesread, void *userdata )
{
	// We shouldn't get to the read callback if the file handle is invalid, so we shouldn't worry about checking it
	FileHandle_t fileHandle = handle;

	*bytesread = ( unsigned int )g_pFullFileSystem->Read( buffer, sizebytes, fileHandle );

	if ( *bytesread == 0 )
		return CGamepadUIFMODManager::CheckError( FMOD_ERR_FILE_EOF );

	return FMOD_OK;
}

FMOD_RESULT F_CALLBACK USER_FMOD_FILE_SEEK_CALLBACK( void *handle, unsigned int pos, void *userdata )
{
	// We shouldn't get to the seek callback if the file handle is invalid, so we shouldn't worry about checking it
	FileHandle_t fileHandle = handle;

	g_pFullFileSystem->Seek( fileHandle, pos, FILESYSTEM_SEEK_HEAD );

	return FMOD_OK;
}

CGamepadUIFMODManager::CGamepadUIFMODManager()
{
	m_pSystem = nullptr;

	for ( unsigned int i = 0; i < NUM_CHANNELGROUPS; i++ )
		m_pChannelGroups[i] = nullptr;

	m_pMasterChannelGroup = nullptr;
}

bool CGamepadUIFMODManager::Init()
{
	// Create the FMOD system, if it fails, the client does not load
	if ( CheckError( FMOD::System_Create( &m_pSystem ) ) != FMOD_OK )
		return false;

	// Initialize the FMOD system, if it fails, the client does not load
	if ( CheckError( m_pSystem->init( 32, FMOD_INIT_NORMAL, nullptr ) ) != FMOD_OK )
		return false;

	// Connect FMOD to Source's filesystem for better support, including using VPKs
	// If this fails, the client does not load
	if ( CheckError( m_pSystem->setFileSystem( 
		USER_FMOD_FILE_OPEN_CALLBACK,
		USER_FMOD_FILE_CLOSE_CALLBACK,
		USER_FMOD_FILE_READ_CALLBACK,
		USER_FMOD_FILE_SEEK_CALLBACK,
		nullptr,
		nullptr,
		-1 ) ) != FMOD_OK )
		return false;

	// Store a pointer of FMOD's master channel group to add ours to
	if ( CheckError( m_pSystem->getMasterChannelGroup( &m_pMasterChannelGroup ) ) != FMOD_OK )
		return false;

	for ( unsigned int i = 0; i < NUM_CHANNELGROUPS; i++ )
	{
		// Create our channel groups
		if ( CheckError( m_pSystem->createChannelGroup( g_pChannelGroupNames[i], &m_pChannelGroups[i] ) ) != FMOD_OK )
			return false;

		// Add our channel groups to the master channel group
		if ( CheckError( m_pMasterChannelGroup->addGroup( m_pChannelGroups[i] ) ) != FMOD_OK )
			return false;

		// Mute them until we're ready to use them
		m_pChannelGroups[i]->setMute( true );
	}

	// 52.4590163 units roughly translates to one meter in Source.
	if ( CheckError( m_pSystem->set3DSettings( 1.0f, 52.4590163f, 1.0f ) ) != FMOD_OK )
		return false;

	// Everything worked, yay! :D
	return true;
}

void CGamepadUIFMODManager::OnClose()
{
	for ( unsigned int i = 0; i < NUM_CHANNELGROUPS; i++ )
	{
		if ( m_pChannelGroups[i] != nullptr )
			m_pChannelGroups[i]->release();
	}

	m_pMasterChannelGroup = nullptr;

	// Due to the way DLLs work, do NOT release the system in the destructor
	if ( m_pSystem )
	{
		m_pSystem->release();
		m_pSystem = nullptr;
	}
}

void CGamepadUIFMODManager::Mute()
{
	for (unsigned int i = 0; i < NUM_CHANNELGROUPS; i++)
		m_pChannelGroups[i]->setMute(true);
}

void CGamepadUIFMODManager::Unmute()
{
	for (unsigned int i = 0; i < NUM_CHANNELGROUPS; i++)
		m_pChannelGroups[i]->setMute(false);
}

void CGamepadUIFMODManager::OnThink()
{
	// Update the FMOD system
	m_pSystem->update();

	m_pMasterChannelGroup->setMute(!GamepadUI::GetInstance().GetEngineClient()->IsActiveApp());

	// Link the volume of our channel groups to the appropriate volume ConVar
	m_pChannelGroups[CHANNELGROUP_STANDARD]->setVolume( volume.GetFloat() );
	m_pChannelGroups[CHANNELGROUP_MUSIC]->setVolume( snd_musicvolume.GetFloat() );

	// Zero out the 3D listener if the local player is invalid
	// This really doesn't need to be done every frame though
	static const FMOD_VECTOR zero = { 0.0f, 0.0f, 0.0f };
	m_pSystem->set3DListenerAttributes( 0, &zero, &zero, &zero, &zero );
}

//-----------------------------------------------------------------------------
// Purpose: Performs a var args printf into a static return buffer
// Input  : *format - 
//			... - 
// Output : char
//-----------------------------------------------------------------------------
char* VarArgs(const char* format, ...)
{
	va_list		argptr;
	static char		string[1024];

	va_start(argptr, format);
	Q_vsnprintf(string, sizeof(string), format, argptr);
	va_end(argptr);

	return string;
}

const char *CGamepadUIFMODManager::GetFullPathToSound( const char *pszSoundPath )
{
	// Note: This probably only works because VarArgs() uses a static char array
	char *pszResultPath = VarArgs( "sound/%s", pszSoundPath );
	V_FixSlashes( pszResultPath );

	return pszResultPath;
}

FMOD_RESULT CGamepadUIFMODManager::CheckError( FMOD_RESULT result )
{
	if ( result != FMOD_OK )
		GamepadUI_Log( "[FMOD]: %s\n", FMOD_ErrorString( result ) );

	return result;
}

static CGamepadUIFMODManager s_GamepadUIFMODManager;
CGamepadUIFMODManager *GetFMODManager() { return &s_GamepadUIFMODManager; }