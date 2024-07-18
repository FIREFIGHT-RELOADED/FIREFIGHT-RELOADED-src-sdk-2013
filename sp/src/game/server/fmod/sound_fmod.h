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
	CNetworkHandle( CBaseEntity, m_hSoundSource );	// Entity from which the sound will play if set.
};

#endif // SOUND_FMOD_H