#include "cbase.h"
#include "fmodmanager.h"
#include "filesystem.h"
#include "c_baseplayer.h"
#include "firefightreloaded/mapinfo.h"
#include "tier3/tier3.h"
#include "tier1/iconvar.h"
#include <vgui/ILocalize.h>

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

void CFMODManager::LevelInitPreEntity()
{
	for (unsigned int i = 0; i < NUM_CHANNELGROUPS; i++)
	{
		m_pChannelGroups[i]->setMute(false);
	}
}

void CFMODManager::LevelShutdownPreEntity()
{
	for (unsigned int i = 0; i < NUM_CHANNELGROUPS; i++)
	{
		m_pChannelGroups[i]->setMute(true);
	}
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
	C_BasePlayer* pLocalPlayer = C_BasePlayer::GetLocalPlayer();

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
ConVar snd_fmod_musicsystem_forceshuffle("snd_fmod_musicsystem_forceshuffle", "0", FCVAR_ARCHIVE);
ConVar snd_fmod_musicsystem_playlist("snd_fmod_musicsystem_playlist", "scripts/playlists/default.txt", FCVAR_ARCHIVE, "");

void CC_ReloadSystem(void)
{
	if (GetMusicSystem())
	{
		GetMusicSystem()->Kill(true);
		GetMusicSystem()->m_bStart = true;
	}
}
static ConCommand snd_fmod_musicsystem_reload("snd_fmod_musicsystem_reload", CC_ReloadSystem, "");

void CC_MoveForwardInPlaylist(void)
{
	if (GetMusicSystem())
	{
		GetMusicSystem()->m_bManualControl = true;
		GetMusicSystem()->tracktime = gpGlobals->curtime;
	}
}
static ConCommand snd_fmod_musicsystem_forward("snd_fmod_musicsystem_forward", CC_MoveForwardInPlaylist, "");

void CC_MoveBackInPlaylist(void)
{
	if (GetMusicSystem())
	{
		//this goes back before the previous and current track. 
		//The system will automatically go to the previous track as it adds +1 to this value.
		GetMusicSystem()->curID -= 2;
		GetMusicSystem()->m_bManualControl = true;
		GetMusicSystem()->tracktime = gpGlobals->curtime;
	}
}
static ConCommand snd_fmod_musicsystem_backward("snd_fmod_musicsystem_backward", CC_MoveBackInPlaylist, "");

CFMODMusicSystem::CFMODMusicSystem() : CAutoGameSystemPerFrame("fmod_music_system")
{
	m_pChannel = nullptr;
	m_pSong = nullptr;
	m_bDisabled = false;
	m_bStart = false;
	m_bPlaylistLoaded = false;
	curID = 0;
	tracktime = 0.0f;
	m_bJustShuffled = false;
	m_sSettings = CreateNullSettings();
	m_sCurSong = CreateNullSong();
}

bool CFMODMusicSystem::Init()
{
	if (!GetFMODManager())
	{
		return false;
	}

	return true;
}

//kill him. do it now.
void CFMODMusicSystem::Kill(bool full)
{
	if (m_pChannel != nullptr)
	{
		m_pChannel->stop();
		m_pChannel = nullptr;
	}

	if (m_pSong != nullptr)
	{
		m_pSong->release();
		m_pSong = nullptr;
	}

	if (full)
	{
		m_bStart = false;
	}
	m_bJustShuffled = false;
	m_bPlaylistLoaded = false;
	curID = 0;
	tracktime = 0.0f;
	m_sSettings = CreateNullSettings();
	m_sCurSong = CreateNullSong();
	m_Songs.Purge();
}

void CFMODMusicSystem::Shutdown()
{
	Kill();
}

void CFMODMusicSystem::ReadPlaylist()
{
	if (m_bPlaylistLoaded)
		return;

	KeyValues* pInfo = CMapInfo::GetMapInfoData();
	bool allowMusicSystem = (pInfo != NULL) ? pInfo->GetBool("AllowMusicSystem", true) : true;

	if (!allowMusicSystem)
	{
		m_bDisabled = true;
		if (pInfo != NULL)
		{
			pInfo->deleteThis();
		}
		return;
	}

	//this is where we init the playlist.
	KeyValues* pKV = new KeyValues("");
	if (pKV->LoadFromFile(filesystem, snd_fmod_musicsystem_playlist.GetString()))
	{
		KeyValues* settings = pKV->FindKey("settings");
		if (settings)
		{
			m_sSettings.Shuffle = settings->GetBool("shuffle");
		}
		else
		{
			m_sSettings.Shuffle = false;
		}

		int num = 0;
		bool failed = false;
		for (auto iter = pKV->GetFirstSubKey(); iter != NULL; iter = iter->GetNextKey())
		{
			if (!strcmp(iter->GetName(), "settings"))
				continue;

			auto newKV = iter->MakeCopy();
			Song_t entry;
			Song_t nullSong = CreateNullSong();
			entry.Path = newKV->GetString("path", NULL);
			entry.Title = newKV->GetString("title", nullSong.Title);
			entry.Artist = newKV->GetString("artist", nullSong.Artist);
			entry.Album = newKV->GetString("album", nullSong.Album);
			entry.Volume = newKV->GetFloat("volume", nullSong.Volume);

			if (entry.Path == NULL)
			{
				failed = true;
				break;
			}

			m_Songs.AddToTail(entry);
			num++;
		}

		if (failed)
		{
			DevWarning("CFMODMusicSystem: Failed to load playlist! File failed to load because entries are missing paths.\n");
			return;
		}

		DevMsg("CFMODMusicSystem: User-specified playlist loaded.\n");
		//play stuff as soon as ents have finished initalizing.
		tracktime = gpGlobals->curtime + 0.1f;
		m_bPlaylistLoaded = true;
		m_bJustShuffled = false;
	}
	else
	{
		DevWarning("CFMODMusicSystem: Failed to load playlist! File may not exist.\n");
	}
}

void CFMODMusicSystem::LevelInitPostEntity()
{
	m_bStart = true;
}

void CFMODMusicSystem::LevelShutdownPreEntity()
{
	//FMOD manager will handle any FMOD related things here.
	Kill();
}

void CFMODMusicSystem::Update(float frametime)
{
	if (!m_bStart)
	{
		if (!m_bDisabled)
		{
			m_bDisabled = true;
		}
		return;
	}

	if (!snd_fmod_musicsystem.GetBool())
	{
		if (!m_bDisabled)
		{
			m_bDisabled = true;
			Kill(false);
		}
		return;
	}
	
	if (m_bDisabled)
	{
		m_bDisabled = false;
	}

	if (!m_bDisabled)
	{
		ReadPlaylist();
	}

	if ((m_sSettings.Shuffle || snd_fmod_musicsystem_forceshuffle.GetBool()) && !m_bJustShuffled)
	{
		//we might need to handle shuffling here
		m_Songs.Shuffle(random);
		m_bJustShuffled = true;
	}

	//this is where we handle playback
	if (gpGlobals->curtime >= tracktime)
	{
		// We could just return, but maybe there's a legitimate reason to re-set the sound
		// This should be handled elsewhere though.
		if (m_pChannel)
		{
			m_pChannel->stop();
			m_pChannel = NULL;
		}

		if (m_pSong)
		{
			m_pSong->release();
			m_pSong = NULL;
		}

		PlaySong();
	}
	else
	{
		//since our music system is reliant on in-game time which is paused on engine pause, 
		//we have to forcefully pause it if certain values are disabled.
		//This is to ensure that the playlist will work effectively.

		if (m_pChannel)
		{
			for (unsigned int i = 0; i < NUM_CHANNELGROUPS; i++)
			{
				m_pChannel->setPaused(!engine->IsActiveApp());
				m_pChannel->setMute(!engine->IsActiveApp());
				m_pChannel->setPaused(engine->IsPaused());
			}
		}
	}
}

void CFMODMusicSystem::PlaySong()
{
	if (m_bDisabled)
		return;

	DevMsg("CFMODMusicSystem: TRYING TO LOAD SONG: %i\n", curID);

	int lastSong = (m_Songs.Count() - 1);

	if (curID < 0)
	{
		curID = lastSong;
	}
	else if (curID > lastSong)
	{
		curID = 0;
	}

	DevMsg("CFMODMusicSystem: CORRECTED ID TO: %i\n", curID);

	//then search for the song we want
	for (int i = 0; i < m_Songs.Count(); ++i)
	{
		const Song_t& song = m_Songs[i];
		DevMsg("CFMODMusicSystem: SEARCHING FOR SONG: %i (CAND: %s, %i)\n", curID, song.Title, i);
		if (curID == i)
		{
			m_sCurSong = song;
			DevMsg("CFMODMusicSystem: SELECTED %s\n", GetTitleString());
		}
	}

	const char* szSound = GetFMODManager()->GetFullPathToSound(m_sCurSong.Path);
	CFMODManager::CheckError(GetFMODManager()->GetSystem()->createSound(szSound, FMOD_DEFAULT | FMOD_CREATESTREAM | FMOD_ACCURATETIME | FMOD_IGNORETAGS | FMOD_LOWMEM, 0, &m_pSong));
	CFMODManager::CheckError(GetFMODManager()->GetSystem()->playSound(m_pSong, GetFMODManager()->GetChannelGroup(CHANNELGROUP_MUSIC), true, &m_pChannel));

	if (m_pChannel)
	{
		CFMODManager::CheckError(m_pChannel->setPitch(1.0f));
		CFMODManager::CheckError(m_pChannel->setVolume(m_sCurSong.Volume));
		CFMODManager::CheckError(m_pChannel->setPaused(false));
	}

	DevMsg("CFMODMusicSystem: PLAYING: %s\n", GetTitleString());

	unsigned int songTime = 5000;
	CFMODManager::CheckError(m_pSong->getLength(&songTime, FMOD_TIMEUNIT_MS));
	DevMsg("CFMODMusicSystem: LENGTH (MS): %i\n", songTime);

	//convert MS into seconds
	songTime = (songTime / 1000);
	DevMsg("CFMODMusicSystem: LENGTH (SEC): %i\n", songTime);

	tracktime = gpGlobals->curtime + songTime;

	//DEBUGGING
	if (!m_bManualControl)
	{
		DevMsg("CFMODMusicSystem: MANUAL CONTROL OFF\n");
	}
	else
	{
		DevMsg("CFMODMusicSystem: MANUAL CONTROL ON\n");
	}

	//increment curID.
	curID++;

	if (!m_bManualControl)
	{
		if (curID > lastSong)
		{
			//RESET!
			curID = 0;
			//tell the shuffle system we need to shuffle again.
			m_bJustShuffled = false;
		}
	}
	else
	{
		m_bManualControl = false;
	}
}

const char* CFMODMusicSystem::GetTitleString()
{
	char szTitleString[2048];
	Q_snprintf(szTitleString, sizeof(szTitleString), "#Song_Title_%s", m_sCurSong.Title);

	if (UTIL_CanGetLanguageString(szTitleString))
	{
		wchar_t* convertedTitle = UTIL_GetTextForLanguage(szTitleString);
		g_pVGuiLocalize->ConvertUnicodeToANSI(convertedTitle, szTitleString, sizeof(szTitleString));
	}
	else
	{
		Q_snprintf(szTitleString, sizeof(szTitleString), "%s", m_sCurSong.Title);
	}

	const char* result = szTitleString;
	return result;
}

const char* CFMODMusicSystem::GetArtistString()
{
	char szArtistString[2048];
	Q_snprintf(szArtistString, sizeof(szArtistString), "#Song_Artist_%s", m_sCurSong.Artist);

	if (UTIL_CanGetLanguageString(szArtistString))
	{
		wchar_t* convertedArtist = UTIL_GetTextForLanguage(szArtistString);
		g_pVGuiLocalize->ConvertUnicodeToANSI(convertedArtist, szArtistString, sizeof(szArtistString));
	}
	else
	{
		Q_snprintf(szArtistString, sizeof(szArtistString), "%s", m_sCurSong.Artist);
	}

	const char* result = szArtistString;
	return result;
}

const char* CFMODMusicSystem::GetAlbumString()
{
	char szAlbumString[2048];
	Q_snprintf(szAlbumString, sizeof(szAlbumString), "#Song_Album_%s", m_sCurSong.Album);

	if (UTIL_CanGetLanguageString(szAlbumString))
	{
		wchar_t* convertedAlbum = UTIL_GetTextForLanguage(szAlbumString);
		g_pVGuiLocalize->ConvertUnicodeToANSI(convertedAlbum, szAlbumString, sizeof(szAlbumString));
	}
	else
	{
		Q_snprintf(szAlbumString, sizeof(szAlbumString), "%s", m_sCurSong.Album);
	}

	const char* result = szAlbumString;
	return result;
}

static CFMODMusicSystem s_FMODMusicSystem;
CFMODMusicSystem* GetMusicSystem() { return &s_FMODMusicSystem; }
