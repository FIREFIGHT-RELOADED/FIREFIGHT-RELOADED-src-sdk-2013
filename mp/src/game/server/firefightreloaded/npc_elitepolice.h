//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#ifndef NPC_ELITEPOLICE_H
#define NPC_ELITEPOLICE_H
#ifdef _WIN32
#pragma once
#endif

#include "npc_combine.h"

//=========================================================
//	>> CNPC_CombineS
//=========================================================
class CNPC_ElitePolice : public CNPC_Combine
{
	DECLARE_CLASS(CNPC_ElitePolice, CNPC_Combine);
#if HL2_EPISODIC
	DECLARE_DATADESC();
#endif

public:
	virtual bool CreateComponents();
	void		Spawn(void);
	void		Precache(void);
	void		DeathSound(const CTakeDamageInfo &info);
	void		PrescheduleThink(void);
	void		BuildScheduleTestBits(void);
	int			SelectSchedule(void);
	float		GetHitgroupDamageMultiplier(int iHitGroup, const CTakeDamageInfo &info);
	void		HandleAnimEvent(animevent_t *pEvent);
	void		OnChangeActivity(Activity eNewActivity);
	void		Event_Killed(const CTakeDamageInfo &info);
	int			MeleeAttack1Conditions(float flDot, float flDist); // For kick/punch
	void		OnListened();
	void		AnnounceAssault(void);
	void		AnnounceEnemyType(CBaseEntity *pEnemy);
	void		AnnounceEnemyKill(CBaseEntity *pEnemy);
	void		NotifyDeadFriend(CBaseEntity* pFriend);
	void		PainSound(void);
	void		IdleSound(void);
	void		AlertSound(void);
	void		LostEnemySound(void);
	void		FoundEnemySound(void);

	void		ClearAttackConditions(void);

	bool		m_fIsBlocking;

	bool		IsLightDamage(const CTakeDamageInfo &info);
	bool		IsHeavyDamage(const CTakeDamageInfo &info);

private:
	bool		ShouldHitPlayer(const Vector &targetDir, float targetDist);

#if HL2_EPISODIC
public:
	Activity	NPC_TranslateActivity(Activity eNewActivity);

protected:
	/// whether to use the more casual march anim in ep2_outland_05
	int			m_iUseMarch;
#endif

};

#endif // NPC_COMBINES_H