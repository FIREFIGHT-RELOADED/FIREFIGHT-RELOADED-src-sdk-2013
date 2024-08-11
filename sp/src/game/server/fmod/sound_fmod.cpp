#include "cbase.h"
#include "sound_fmod.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

extern ConVar volume;
extern ConVar snd_musicvolume;

#define SF_AMBIENT_SOUND_EVERYWHERE			1
#define SF_AMBIENT_SOUND_START_SILENT		16
#define SF_AMBIENT_SOUND_NOT_LOOPING		32
#define SF_AMBIENT_SOUND_MUSIC				64

LINK_ENTITY_TO_CLASS( ambient_fmod, CAmbientFMOD );

BEGIN_DATADESC( CAmbientFMOD )

	// Inputs
	DEFINE_INPUTFUNC( FIELD_VOID, "PlaySound", InputPlaySound ),
	DEFINE_INPUTFUNC( FIELD_VOID, "StopSound", InputStopSound ),
	DEFINE_INPUTFUNC( FIELD_FLOAT, "SetVolume", InputSetVolume ),
	DEFINE_INPUTFUNC( FIELD_FLOAT, "IncreaseVolume", InputIncreaseVolume ),
	DEFINE_INPUTFUNC( FIELD_FLOAT, "DecreaseVolume", InputDecreaseVolume ),
	DEFINE_INPUTFUNC( FIELD_FLOAT, "SetMusicSpeed", InputSetMusicSpeed ),
	DEFINE_INPUTFUNC( FIELD_FLOAT, "Pitch", InputPitch ),
	DEFINE_INPUTFUNC( FIELD_FLOAT, "IncreasePitch", InputIncreasePitch ),
	DEFINE_INPUTFUNC( FIELD_FLOAT, "DecreasePitch", InputDecreasePitch ),

	// Fields
	DEFINE_KEYFIELD( m_iszSound, FIELD_SOUNDNAME, "message" ),
	DEFINE_KEYFIELD( m_flMaxDistance,	FIELD_FLOAT, "maxdistance" ),
	DEFINE_KEYFIELD( m_flVolume, FIELD_FLOAT, "volume" ),
	DEFINE_KEYFIELD( m_flPitch, FIELD_FLOAT, "pitch" ),
	DEFINE_KEYFIELD( m_sSourceEntName,	FIELD_STRING, "SourceEntityName" ),

	//save field
	DEFINE_FIELD( m_bLooping, FIELD_BOOLEAN ),

END_DATADESC()

IMPLEMENT_SERVERCLASS_ST( CAmbientFMOD, DT_AmbientFMOD )
	SendPropStringT ( SENDINFO( m_iszSound ) ),
	SendPropFloat( SENDINFO( m_flMaxDistance ), 8, SPROP_NOSCALE ),
	SendPropFloat( SENDINFO( m_flVolume ), 8, SPROP_NOSCALE ),
	SendPropFloat( SENDINFO( m_flPitch ), 8, SPROP_NOSCALE ),
	SendPropFloat( SENDINFO( m_flMusicSpeed ), 8, SPROP_NOSCALE ),
	SendPropBool( SENDINFO( m_bGlobal ) ),
	SendPropBool( SENDINFO( m_bActive ) ),
	SendPropBool( SENDINFO( m_bLooping ) ),
	SendPropBool( SENDINFO( m_bMusic ) ),
	SendPropEHandle( SENDINFO( m_hSoundSource ) ),
END_SEND_TABLE()

CAmbientFMOD::CAmbientFMOD()
{
	m_iszSound = NULL_STRING;

	m_flVolume = 1.0f;
	m_flPitch = 100.0f;
	m_flMusicSpeed = 1.0f;

	m_bGlobal = false;
	m_bActive = false;
	m_bLooping = false;
	m_bMusic = false;
}

void CAmbientFMOD::Spawn( void )
{
	BaseClass::Spawn();

	if ( FBitSet ( m_spawnflags, SF_AMBIENT_SOUND_EVERYWHERE ) )
		m_bGlobal = true;
	else
		m_bGlobal = false;

	if ( FBitSet ( m_spawnflags, SF_AMBIENT_SOUND_NOT_LOOPING ) )
		m_bLooping = false;
	else
		m_bLooping = true;

	if ( FBitSet ( m_spawnflags, SF_AMBIENT_SOUND_START_SILENT ) )
		m_bActive = false;
	else
		m_bActive = true;

	if ( FBitSet ( m_spawnflags, SF_AMBIENT_SOUND_MUSIC ) )
		m_bMusic = true;
	else
		m_bMusic = false;

	if ( m_sSourceEntName != NULL_STRING )
		m_hSoundSource = gEntList.FindEntityByName( NULL, m_sSourceEntName );

	if ( !m_hSoundSource )
		m_hSoundSource = this;

	if ( m_flVolume.Get() < 0.0f )
		m_flVolume = 0.0f;
	else
	{
		if ( m_flVolume.Get() / 10.0f > 1.0f )
			m_flVolume = 1.0f;
		else
			m_flVolume = ( m_flVolume.Get() /10.0f );
	}

	// Talk to the client class when data changes
	AddEFlags( EFL_FORCE_CHECK_TRANSMIT );
}

void CAmbientFMOD::InputPlaySound( inputdata_t &inputData )
{
	SoundPlayingState(true);
}

void CAmbientFMOD::InputStopSound( inputdata_t &inputData )
{
	SoundPlayingState(false);
}

void CAmbientFMOD::SetVolume(float flVol)
{
	if (flVol < 0.0f)
		m_flVolume = 0.0f;
	else if (flVol > 10.0f)
		m_flVolume = 1.0f;
	else if (flVol >= 0.0f && flVol <= 10.0f)
		m_flVolume = (flVol / 10.0f);
}

void CAmbientFMOD::InputSetVolume( inputdata_t &inputData )
{
	SetVolume(inputData.value.Float());
}

void CAmbientFMOD::IncreaseVolume(float flVol)
{
	float flIncrease = (flVol / 10.0f);

	if (flIncrease + m_flVolume.Get() > 1.0f)
		m_flVolume = 1.0f;
	else
		m_flVolume = m_flVolume.Get() + flIncrease;
}

void CAmbientFMOD::InputIncreaseVolume( inputdata_t &inputData )
{
	IncreaseVolume(inputData.value.Float());
}

void CAmbientFMOD::DecreaseVolume(float flVol)
{
	if ((m_flVolume.Get() - (flVol / 10.0f)) < 0.0f)
		m_flVolume = 0.0f;
	else
		m_flVolume = (m_flVolume.Get() - (flVol / 10.0f));
}

void CAmbientFMOD::InputDecreaseVolume( inputdata_t &inputData )
{
	DecreaseVolume(inputData.value.Float());
}

void CAmbientFMOD::InputSetMusicSpeed( inputdata_t &inputData )
{
	SetMusicSpeed(inputData.value.Float());
}

void CAmbientFMOD::InputPitch( inputdata_t &inputData )
{
	SetPitch(inputData.value.Float());
}

void CAmbientFMOD::IncreasePitch(float flPitch)
{
	if (m_flPitch + flPitch > 255.0f)
		m_flPitch = 255.0f;
	else
		m_flPitch += flPitch;
}

void CAmbientFMOD::DecreasePitch(float flPitch)
{
	if (m_flPitch - flPitch < 0.0f)
		m_flPitch = 0.0f;
	else
		m_flPitch -= flPitch;
}

void CAmbientFMOD::InputIncreasePitch( inputdata_t &inputData )
{
	IncreasePitch(inputData.value.Float());
}

void CAmbientFMOD::InputDecreasePitch( inputdata_t &inputData )
{
	DecreasePitch(inputData.value.Float());
}