#ifndef GAMEPADUI_FMOD_H
#define GAMEPADUI_FMOD_H
#ifdef _WIN32
#pragma once
#endif

#include <fmod.hpp>

// Channel group types
enum eChannelGroupType
{
	CHANNELGROUP_STANDARD = 0,
	CHANNELGROUP_MUSIC,
	NUM_CHANNELGROUPS
};

static const char *g_pChannelGroupNames[NUM_CHANNELGROUPS] =
{
	"STANDARD",
	"MUSIC"
};

FMOD_RESULT F_CALLBACK USER_FMOD_FILE_OPEN_CALLBACK( const char *name, unsigned int *filesize, void **handle, void *userdata );
FMOD_RESULT F_CALLBACK USER_FMOD_FILE_CLOSE_CALLBACK( void *handle, void *userdata );
FMOD_RESULT F_CALLBACK USER_FMOD_FILE_READ_CALLBACK( void *handle, void *buffer, unsigned int sizebytes, unsigned int *bytesread, void *userdata );
FMOD_RESULT F_CALLBACK USER_FMOD_FILE_SEEK_CALLBACK( void *handle, unsigned int pos, void *userdata );

class CGamepadUIFMODManager
{
public:
	CGamepadUIFMODManager();

	// Initilaztion and Shutdown functions
	virtual bool Init();
	virtual void OnClose();

	virtual void Mute();
	virtual void Unmute();
	virtual void Pause();
	virtual void Unpause();

	// Per frame update function
	virtual void OnThink();

	// Get the full path to a sound relative to the mod folder
	const char *GetFullPathToSound( const char *pszSoundPath );

	// Get one of our channel groups
	FMOD::ChannelGroup *GetChannelGroup( eChannelGroupType channelgroupType ) { return m_pChannelGroups[channelgroupType]; }

	// Used to check for errors when using FMOD
	static FMOD_RESULT CheckError( FMOD_RESULT result );

	FMOD::System *GetSystem() { return m_pSystem; }

private:
	// This is our FMOD system, the core of everything FMOD related
	FMOD::System *m_pSystem;

	// These are our channel groups, used for managing channel volumes
	FMOD::ChannelGroup *m_pChannelGroups[NUM_CHANNELGROUPS];

	// This points to the master channel group which all other channel groups are a part of
	FMOD::ChannelGroup *m_pMasterChannelGroup;
};

CGamepadUIFMODManager *GetFMODManager();

#endif // GAMEPADUI_FRAME_H
