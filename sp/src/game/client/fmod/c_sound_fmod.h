#ifndef C_SOUND_FMOD_H
#define C_SOUND_FMOD_H

#include "c_baseentity.h"

#include <fmod.hpp>

class C_AmbientFMOD : public C_BaseEntity
{
public:
	DECLARE_CLASS( C_AmbientFMOD, C_BaseEntity );
	DECLARE_CLIENTCLASS();

	C_AmbientFMOD();

	virtual void Spawn( void );

	virtual void UpdateOnRemove( void );

	virtual void OnDataChanged( DataUpdateType_t updateType );

	virtual void ClientThink( void );

private:
	void ToggleSound();
	void SetVolume();
	void SetMusicSpeed();
	void SetPitch();

	bool SetSound();

	float ConvertVolume();

	CNetworkVar ( float, m_flMaxDistance );			// The maximum distance, dependent on the rolloff method used
	CNetworkVar ( float, m_flVolume );				// Sound volume
	CNetworkVar ( float, m_flPitch );				// Pitch
	CNetworkVar ( float, m_flMusicSpeed );			// Music speed, only works on sequenced music (tracker modules, midi)
	CNetworkVar ( bool, m_bGlobal );				// True when the sound is a global sound, it plays everywhere.
	CNetworkVar ( bool, m_bActive );				// True when sound is to be playing.
	CNetworkVar ( bool, m_bLooping );				// True when the sound played will loop
	CNetworkVar ( bool, m_bMusic );					// True when enforcing the use of the music volume slider.
	CNetworkHandle( C_BaseEntity, m_hSoundSource );	// Entity from which the sound will play if set.

	char m_iszSound[1024];

	bool m_bInit;
	bool m_bOldActive;

	float m_flOldVolume;
	float m_flOldPitch;
	float m_flOldMusicSpeed;

	FMOD::Channel *m_pChannel;
	FMOD::Sound *m_pSound;
};

#endif // C_SOUND_FMOD_H