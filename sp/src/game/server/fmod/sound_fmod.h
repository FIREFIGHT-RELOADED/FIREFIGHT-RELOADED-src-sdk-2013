#ifndef SOUND_FMOD_H
#define SOUND_FMOD_H

#include "baseentity.h"

class CAmbientFMOD : public CBaseEntity
{
public:
	DECLARE_CLASS( CAmbientFMOD, CBaseEntity );
	DECLARE_SERVERCLASS();

	DECLARE_DATADESC();

	CAmbientFMOD();

	virtual void Spawn( void );

	inline int UpdateTransmitState() { return SetTransmitState( FL_EDICT_ALWAYS ); }

	void SetSound(string_t szSoundFile) { m_iszSound = szSoundFile; }
	void SetSound(const char *szSoundFile) { m_iszSound = MAKE_STRING(szSoundFile); }
	void SoundPlayingState(bool bActive) { m_bActive = bActive; }
	void SetVolume(float flVol);
	void IncreaseVolume(float flVol);
	void DecreaseVolume(float flVol);
	void SetMusicSpeed(float flSpeed) { m_flMusicSpeed = flSpeed; }
	void SetPitch(float flPitch) { m_flPitch = flPitch; }
	void IncreasePitch(float flPitch);
	void DecreasePitch(float flPitch);

	void SetGlobalState(bool bGlobal) { m_bGlobal = bGlobal; };
	void SetLoopState(bool bLooping) { m_bLooping = bLooping; };
	void SetMusic(bool bMusic) { m_bMusic = bMusic; };

	bool IsPlaying(void) { return (m_bActive == true); }

	// Input handlers
	void InputPlaySound( inputdata_t &inputData );
	void InputStopSound( inputdata_t &inputData );
	void InputSetVolume( inputdata_t &inputData );
	void InputIncreaseVolume( inputdata_t &inputData );
	void InputDecreaseVolume( inputdata_t &inputData );
	void InputSetMusicSpeed( inputdata_t &inputData );
	void InputPitch( inputdata_t &inputData );
	void InputIncreasePitch( inputdata_t &inputData );
	void InputDecreasePitch( inputdata_t &inputData );

private:

	CNetworkVar( string_t, m_iszSound );			// Path/filename of sound file to play
	string_t m_sSourceEntName;						// Name of the m_hSoundSource entity

	CNetworkVar ( float, m_flMaxDistance );			// The maximum distance, dependent on the rolloff method used
	CNetworkVar ( float, m_flVolume );				// Sound volume
	CNetworkVar ( float, m_flPitch );				// Pitch
	CNetworkVar ( float, m_flMusicSpeed );			// Music speed, only works on sequenced music (tracker modules, midi)
	CNetworkVar ( bool, m_bGlobal );				// True when the sound is a global sound, it plays everywhere.
	CNetworkVar ( bool, m_bActive );				// True when sound is to be playing.
	CNetworkVar ( bool, m_bLooping );				// True when the sound played will loop
	CNetworkVar ( bool, m_bMusic );					// True when enforcing the use of the music volume slider.
	CNetworkVar(int, m_iLength);
	CNetworkHandle( CBaseEntity, m_hSoundSource );	// Entity from which the sound will play if set.
};
#endif // SOUND_FMOD_H