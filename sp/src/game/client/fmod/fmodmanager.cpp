#include "cbase.h"
#include "fmodmanager.h"
#include "filesystem.h"
#include "c_baseplayer.h"
#include "firefightreloaded/mapinfo.h"

#include <fmod_errors.h>

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

// This is the global volume ConVar, this is a bit of a hack so we don't have to use ConVarRef
ConVar volume("volume", "1.0", FCVAR_ARCHIVE);

// This is the global music volume ConVar, this is a bit of a hack so we don't have to use ConVarRef
ConVar snd_musicvolume("snd_musicvolume", "1.0", FCVAR_ARCHIVE);

// This is the global music volume ConVar, this is a bit of a hack so we don't have to use ConVarRef
ConVar snd_mute_losefocus("snd_mute_losefocus", "1", FCVAR_ARCHIVE);

ConVar snd_fmod_pause_losefocus("snd_fmod_pause_losefocus", "1", FCVAR_ARCHIVE);
ConVar snd_fmod_pause_gamepaused("snd_fmod_pause_gamepaused", "1", FCVAR_ARCHIVE);

FMOD_RESULT F_CALLBACK USER_FMOD_FILE_OPEN_CALLBACK(const char* name, unsigned int* filesize, void** handle, void* userdata)
{
	FileHandle_t fileHandle = g_pFullFileSystem->Open(name, "rb", nullptr);

	// Same as checking if NULL
	if (fileHandle == FILESYSTEM_INVALID_HANDLE)
		return CFMODManager::CheckError(FMOD_ERR_FILE_NOTFOUND);

	*handle = fileHandle;
	*filesize = g_pFullFileSystem->Size(fileHandle);

	return FMOD_OK;
}

FMOD_RESULT F_CALLBACK USER_FMOD_FILE_CLOSE_CALLBACK(void* handle, void* userdata)
{
	FileHandle_t fileHandle = handle;
	g_pFullFileSystem->Close(fileHandle);

	return FMOD_OK;
}

FMOD_RESULT F_CALLBACK USER_FMOD_FILE_READ_CALLBACK(void* handle, void* buffer, unsigned int sizebytes, unsigned int* bytesread, void* userdata)
{
	// We shouldn't get to the read callback if the file handle is invalid, so we shouldn't worry about checking it
	FileHandle_t fileHandle = handle;

	*bytesread = (unsigned int)g_pFullFileSystem->Read(buffer, sizebytes, fileHandle);

	if (*bytesread == 0)
		return CFMODManager::CheckError(FMOD_ERR_FILE_EOF);

	return FMOD_OK;
}

FMOD_RESULT F_CALLBACK USER_FMOD_FILE_SEEK_CALLBACK(void* handle, unsigned int pos, void* userdata)
{
	// We shouldn't get to the seek callback if the file handle is invalid, so we shouldn't worry about checking it
	FileHandle_t fileHandle = handle;

	g_pFullFileSystem->Seek(fileHandle, pos, FILESYSTEM_SEEK_HEAD);

	return FMOD_OK;
}

CFMODManager::CFMODManager() : CAutoGameSystemPerFrame( "fmod_manager" )
{
	m_pSystem = nullptr;

	for ( unsigned int i = 0; i < NUM_CHANNELGROUPS; i++ )
		m_pChannelGroups[i] = nullptr;

	m_pMasterChannelGroup = nullptr;
}

bool CFMODManager::Init()
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

void CFMODManager::Shutdown()
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

void CFMODManager::Mute()
{
	for (unsigned int i = 0; i < NUM_CHANNELGROUPS; i++)
		m_pChannelGroups[i]->setMute(true);
}

void CFMODManager::Unmute()
{
	for (unsigned int i = 0; i < NUM_CHANNELGROUPS; i++)
		m_pChannelGroups[i]->setMute(false);
}

void CFMODManager::Pause()
{
	for (unsigned int i = 0; i < NUM_CHANNELGROUPS; i++)
		m_pChannelGroups[i]->setPaused(true);
}

void CFMODManager::Unpause()
{
	for (unsigned int i = 0; i < NUM_CHANNELGROUPS; i++)
		m_pChannelGroups[i]->setPaused(false);
}

void CFMODManager::LevelInitPreEntity()
{
	Unmute();
}

void CFMODManager::LevelShutdownPreEntity()
{
	Mute();
}

void CFMODManager::Update( float frametime )
{
	// Update the FMOD system
	m_pSystem->update();

	//pause the music specifically if we're not the active app or if the engine is paused.
	m_pChannelGroups[CHANNELGROUP_MUSIC]->setPaused(
		(snd_fmod_pause_losefocus.GetBool() ? !engine->IsActiveApp() : false) ||
		(snd_fmod_pause_gamepaused.GetBool() ? engine->IsPaused() : false));

	if (snd_mute_losefocus.GetBool())
	{
		// Mute or Unmute everything depending on if we're the active app
		m_pMasterChannelGroup->setMute(!engine->IsActiveApp());
	}

	// Link the volume of our channel groups to the appropriate volume ConVar
	m_pChannelGroups[CHANNELGROUP_STANDARD]->setVolume( volume.GetFloat() );
	m_pChannelGroups[CHANNELGROUP_MUSIC]->setVolume( snd_musicvolume.GetFloat() );

	// Get the local player for updating the 3D listener
	C_BasePlayer *pLocalPlayer = C_BasePlayer::GetLocalPlayer();

	// Update the 3D listener if the local player is valid
	if ( pLocalPlayer != nullptr )
	{
		Vector playerPosition = pLocalPlayer->EyePosition();
		Vector forward, up;
		
		AngleVectors( pLocalPlayer->EyeAngles(), &forward, nullptr, &up );

		VectorNormalize( forward );
		VectorNormalize( up );

		// Source's Vector to FMOD_VECTOR is just x, z, y
		FMOD_VECTOR listenerPosition = { playerPosition.x, playerPosition.z, playerPosition.y };
		FMOD_VECTOR listenerForward = { forward.x, forward.z, forward.y };
		FMOD_VECTOR listenerUp = { up.x, up.z, up.y };

		// Set our 3D listener attributes
		m_pSystem->set3DListenerAttributes( 0, &listenerPosition, nullptr, &listenerForward, &listenerUp );
	}
	else
	{
		// Zero out the 3D listener if the local player is invalid
		// This really doesn't need to be done every frame though
		static const FMOD_VECTOR zero = { 0.0f, 0.0f, 0.0f };
		m_pSystem->set3DListenerAttributes( 0, &zero, &zero, &zero, &zero );
	}
}

const char *CFMODManager::GetFullPathToSound( const char *pszSoundPath )
{
	// Note: This probably only works because VarArgs() uses a static char array
	char *pszResultPath = VarArgs( "sound/%s", pszSoundPath );
	V_FixSlashes( pszResultPath );

	return pszResultPath;
}

FMOD_RESULT CFMODManager::CheckError( FMOD_RESULT result )
{
	if ( result != FMOD_OK )
		Warning( "[FMOD]: %s\n", FMOD_ErrorString( result ) );

	return result;
}

static CFMODManager s_FMODManager;
CFMODManager *GetFMODManager() { return &s_FMODManager; }

//CLIENT SIDE MUSIC SYSTEM

ConVar snd_fmod_musicsystem("snd_fmod_musicsystem", "1", FCVAR_ARCHIVE);
ConVar snd_fmod_musicsystem_playlist("snd_fmod_musicsystem_playlist", "scripts/playlists/default.txt", FCVAR_ARCHIVE);

CFMODMusicSystem::CFMODMusicSystem() : CAutoGameSystemPerFrame("fmod_music_system")
{
}

bool CFMODMusicSystem::Init()
{
	if (!GetFMODManager())
	{
		return false;
	}

	return true;
}

void CFMODMusicSystem::Shutdown()
{
	m_Songs.Purge();
}

void CFMODMusicSystem::LevelInitPostEntity()
{
	//this is where we init the playlist.
	KeyValues* pKV = new KeyValues("");
	if (pKV->LoadFromFile(filesystem, snd_fmod_musicsystem_playlist.GetString()))
	{
		int num = 0;
		bool failed = false;
		for (auto iter = pKV->GetFirstSubKey(); iter != NULL; iter = iter->GetNextKey())
		{
			auto newKV = iter->MakeCopy();
			Song_t entry;
			entry.Title = newKV->GetString("title", NULL);
			entry.Artist = newKV->GetString("artist", NULL);
			entry.Album = newKV->GetString("album", "Music");
			entry.ID = num;

			if (entry.Title == NULL || entry.Artist == NULL)
			{
				failed = true;
				break;
			}

			m_Songs.AddToTail(entry);
			num++;
		}

		if (failed)
		{
			DevWarning("CFMODMusicSystem: Failed to load playlist! File failed to load because entries are missing important values.\n");
			return;
		}

		DevMsg("CFMODMusicSystem: User-specified playlist loaded.\n");
	}
	else
	{
		DevWarning("CFMODMusicSystem: Failed to load playlist! File may not exist.\n");
	}
}

void CFMODMusicSystem::LevelShutdownPreEntity()
{
	//FMOD manager will handle any FMOD related things here.
	m_Songs.Purge();
}

void CFMODMusicSystem::Update(float frametime)
{
	if (!snd_fmod_musicsystem.GetBool())
		return;

	KeyValues* pInfo = CMapInfo::GetMapInfoData();
	bool allowMusicSystem = pInfo->GetBool("AllowMusicSystem", true);

	if (!allowMusicSystem)
		return;

	//this is where we handle playback
}

static CFMODMusicSystem s_FMODMusicSystem;
CFMODMusicSystem* GetMusicSystem() { return &s_FMODMusicSystem; }