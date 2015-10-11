//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#ifndef NPC_COMBINEACE_H
#define NPC_COMBINEACE_H
#ifdef _WIN32
#pragma once
#endif

#include "npc_combine.h"
#include "Sprite.h"
#include "SpriteTrail.h"

class CArmorPiece : public CBaseAnimating
{
	DECLARE_CLASS(CArmorPiece, CBaseAnimating);
public:
	void Spawn(void);
	void Precache(void);
	bool CreateVPhysics(void);
};

enum aceEyeState_t
{
	ACE_EYE_ACTIVATE,		//active
	ACE_EYE_DORMANT,			//Not active
	ACE_EYE_DEAD,				//Completely invisible
};

//=========================================================
//	>> CNPC_CombineS
//=========================================================
class CNPC_CombineAce : public CNPC_Combine
{
	DECLARE_CLASS( CNPC_CombineAce, CNPC_Combine );
#if HL2_EPISODIC
	DECLARE_DATADESC();
#endif

public: 
	void		Spawn( void );
	void		Precache( void );
	void		DeathSound( const CTakeDamageInfo &info );
	void		PrescheduleThink( void );
	void		BuildScheduleTestBits( void );
	int			SelectSchedule ( void );
	float		GetHitgroupDamageMultiplier( int iHitGroup, const CTakeDamageInfo &info );
	void		HandleAnimEvent( animevent_t *pEvent );
	void		OnChangeActivity( Activity eNewActivity );
	void		Event_Killed( const CTakeDamageInfo &info );
	void		OnListened();

	void		ClearAttackConditions( void );

	bool		m_fIsBlocking;

	bool		IsLightDamage( const CTakeDamageInfo &info );
	bool		IsHeavyDamage( const CTakeDamageInfo &info );

	void		SpawnArmorPieces(void);

private:
	bool		ShouldHitPlayer( const Vector &targetDir, float targetDist );
	void		SetEyeState(aceEyeState_t state);

	CSprite				*m_pEyeSprite;
	CSpriteTrail		*m_pEyeTrail;
	CArmorPiece			*pArmor;
	bool					m_bNoArmor;

#if HL2_EPISODIC
public:
	Activity	NPC_TranslateActivity( Activity eNewActivity );

protected:
	/// whether to use the more casual march anim in ep2_outland_05
	int			m_iUseMarch;
#endif

};

#endif // NPC_COMBINES_H
