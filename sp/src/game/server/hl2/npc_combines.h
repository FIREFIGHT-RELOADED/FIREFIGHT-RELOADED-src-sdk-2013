//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#ifndef NPC_COMBINES_H
#define NPC_COMBINES_H
#ifdef _WIN32
#pragma once
#endif

#include "npc_combine.h"

#define SF_COMBINE_S_ELITE ( 1 << 19 )
#define SF_COMBINE_S_SHOTGUNNER ( 1 << 20 )
#define SF_COMBINE_S_PRISONGUARD ( 1 << 21 )
#define SF_COMBINE_S_FRIENDLY ( 1 << 22 )

//=========================================================
//	>> CNPC_CombineS
//=========================================================
class CNPC_CombineS : public CNPC_Combine
{
	DECLARE_CLASS( CNPC_CombineS, CNPC_Combine );
#if HL2_EPISODIC
	DECLARE_DATADESC();
#endif

public: 
	void		Spawn( void );
	void		Precache( void );
	Class_T		Classify(void);
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

	void GetSoldierModel();
	const char* GetGibModel(appendage_t appendage);

private:
	bool		ShouldHitPlayer( const Vector &targetDir, float targetDist );

#if HL2_EPISODIC
public:
	Activity	NPC_TranslateActivity( Activity eNewActivity );

protected:
	/// whether to use the more casual march anim in ep2_outland_05
	int			m_iUseMarch;
#endif
};

#endif // NPC_COMBINES_H
