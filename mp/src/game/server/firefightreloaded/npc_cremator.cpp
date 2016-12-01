//========= Copyright © 2015, Dark Interval.  =================================//
//
// Purpose: Keep it clean. OR... HE... WILL!..
//
// $NoKeywords: $
//=============================================================================//
#pragma warning(disable:4706)
#include "cbase.h"
#include "npc_cremator.h"

/*------------Feb 25th '15 code revision------------*/
/*================CNPC_Cremator===============*/
/*--------------------------------------------*/
#define SF_CREMATOR_NO_GRENADES					1<<23
#define SF_CREMATOR_NO_FUEL_SPILLING			1<<24 
#define SF_CREMATOR_NO_PATROL_BEHAVIOUR			1<<25
#define CREMATOR_SKIN_ALERT				0 // yellow eyes
#define CREMATOR_SKIN_CALM				1 // blue eyes
#define CREMATOR_SKIN_ANGRY				2 // red eyes
#define CREMATOR_SKIN_DEAD				3 // black eyes
#define CREMATOR_IMMOLATOR_RANGE		300
#define	CREMATOR_AE_IMMO_START			( 6 )
#define	CREMATOR_AE_IMMO_PARTICLE		( 7 )
#define	CREMATOR_AE_IMMO_PARTICLEOFF	( 8 )
#define CREMATOR_AE_THROWGRENADE		( 9 )
#define CREMATOR_AE_SPECIAL_START		( 10 )
#define CREMATOR_AE_SPECIAL_MIDDLE		( 11 ) // bad name?
#define CREMATOR_AE_SPECIAL_END			( 12 )
#define CREMATOR_AE_RELOAD				( 15 )
#define CREMATOR_AE_FLLEFT				( 98 )
#define CREMATOR_AE_FLRIGHT				( 99 )
#define	CREMATOR_BEAM_ATTACH			1
#define CREMATOR_BURN_TIME				20
#define CREMATOR_DETECT_CORPSE_RANGE	300.0

enum
{
	SCHED_CREMATOR_RANGE_ATTACK1 = LAST_SHARED_SCHEDULE + 100,
	SCHED_CREMATOR_RANGE_ATTACK2,
	SCHED_CREMATOR_CHASE_ENEMY,
	SCHED_CREMATOR_PATROL,
	SCHED_CREMATOR_INVESTIGATE_CORPSE,
};
enum
{
	TASK_CREMATOR_RANGE_ATTACK1 = LAST_SHARED_TASK + 1,
	TASK_CREMATOR_RANGE_ATTACK2,
	TASK_CREMATOR_RELOAD,
	TASK_CREMATOR_INVESTIGATE_CORPSE,
	TASK_CREMATOR_BURN_CORPSE,
};
enum
{
	COND_CREMATOR_OUT_OF_AMMO = LAST_SHARED_CONDITION + 1,
	COND_CREMATOR_ENEMY_WITH_HIGHER_PRIORITY,
	COND_CREMATOR_FOUND_CORPSE, // Cremator was patrolling the streets and found a corpse
};

CSound *pInterestSound;
CBaseEntity *pCorpse;

LINK_ENTITY_TO_CLASS(npc_cremator, CNPC_Cremator);

ConVar	sk_cremator_health("sk_cremator_health", "180");
ConVar	sk_cremator_firedamage("sk_cremator_firedamage", "15");
ConVar	sk_cremator_radiusdamage("sk_cremator_radiusdamage", "5");
ConVar  sk_cremator_corpse_search_radius("sk_cremator_corpse_search_radius", "320");
ConVar  sk_cremator_attackplayeronsight("sk_cremator_attackplayeronsight", "0");

Activity ACT_FIRESPREAD;
Activity ACT_CREMATOR_ARM;
Activity ACT_CREMATOR_DISARM;
Activity ACT_CREMATOR_RELOAD;

BEGIN_DATADESC(CNPC_Cremator)
DEFINE_FIELD(m_iAmmo, FIELD_INTEGER),
DEFINE_FIELD(m_bHeadshot, FIELD_BOOLEAN),
DEFINE_FIELD(m_bIsPlayerEnemy, FIELD_BOOLEAN),
DEFINE_FIELD(m_bIsNPCEnemy, FIELD_BOOLEAN),
DEFINE_FIELD(m_bPlayAngrySound, FIELD_BOOLEAN),
END_DATADESC();

void CNPC_Cremator::Precache()
{
	PrecacheModel("models/Cremator.mdl");
	PrecacheModel("models/Cremator_headprop.mdl");

	PrecacheParticleSystem("vapor_ray");

	PrecacheScriptSound("NPC_Cremator.NPCAlert");
	PrecacheScriptSound("NPC_Cremator.PlayerAlert");
	PrecacheScriptSound("NPC_Cremator.BreathingAmb");
	PrecacheScriptSound("NPC_Cremator.AngryAmb");
	PrecacheScriptSound("NPC_Cremator.DeathAmb");
	PrecacheScriptSound("Weapon_Immolator.Single");
	PrecacheScriptSound("Weapon_Immolator.Stop");

	//	PrecacheModel( "sprites/glow02.vmt" );

	//	UTIL_PrecacheOther( "env_sprite" );
	//	UTIL_PrecacheOther( "crematorlight" );

	BaseClass::Precache();
}
void CNPC_Cremator::Spawn(void)
{
	Precache();
	SetModel("models/Cremator.mdl");
	SetHullType(HULL_MEDIUM_TALL);
	SetHullSizeNormal();

	SetBodygroup(1, 0); // the gun
	SetBodygroup(2, 0); // the head

	SetSolid(SOLID_BBOX);
	AddSolidFlags(FSOLID_NOT_STANDABLE);
	SetMoveType(MOVETYPE_STEP);

	m_bloodColor = BLOOD_COLOR_MECH; // TODO: basically turns blood into sparks. Need something more special.
	m_iHealth = sk_cremator_health.GetFloat();
	m_flFieldOfView = VIEW_FIELD_WIDE;

	m_NPCState = NPC_STATE_NONE;

	CapabilitiesClear();
	CapabilitiesAdd(bits_CAP_MOVE_GROUND | bits_CAP_INNATE_RANGE_ATTACK1 | bits_CAP_DOORS_GROUP); // TODO: cremator thus can open doors but he is too tall to fit normal doors?

	NPCInit();

	SetDistLook(1280);

	pCorpse = NULL;
}

Disposition_t CNPC_Cremator::IRelationType(CBaseEntity *pTarget)
{
	Disposition_t disp = BaseClass::IRelationType(pTarget);

	if (pTarget == NULL)
		return disp;
	if (pTarget->Classify() == CLASS_PLAYER)
	{
		if (sk_cremator_attackplayeronsight.GetBool())
		{
			return D_HT;
		}
		else
		{
			return m_bIsPlayerEnemy ? D_HT : D_NU;
		}
	}
	else if (pTarget->Classify() != (CLASS_PLAYER && Classify()))
	{
		return m_bIsNPCEnemy ? D_HT : D_NU;
	}
	return disp;
}
#if 0
void CNPC_Cremator::OnListened(void)
{
	AISoundIter_t iter;

	CSound *pCurrentSound = GetSenses()->GetFirstHeardSound(&iter);

	static int ConditionsToClear[] =
	{
		COND_CREMATOR_DETECT_INTEREST,
		COND_CREMATOR_DETECT_NEW_INTEREST,
	};

	ClearConditions(ConditionsToClear, ARRAYSIZE(ConditionsToClear));

	while (pCurrentSound)
	{
		if (!pCurrentSound->FIsSound())
		{
			if (pCurrentSound->m_iType & SOUND_CARCASS | SOUND_MEAT)
			{
				pInterestSound = pCurrentSound;
				Msg("Cremator smells a carcass\n");
				SetCondition(COND_CREMATOR_FOUND_CORPSE);
			}
		}

		pCurrentSound = GetSenses()->GetNextHeardSound(&iter);
	}

	BaseClass::OnListened();
}
#endif
void CNPC_Cremator::SelectSkin(void)
{
	switch (m_NPCState)
	{
	case NPC_STATE_COMBAT:
	{
		m_nSkin = CREMATOR_SKIN_ANGRY;
		break;
	}
	case NPC_STATE_ALERT:
	{
		m_nSkin = CREMATOR_SKIN_ALERT;
		break;
	}
	case NPC_STATE_IDLE:
	{
		m_nSkin = CREMATOR_SKIN_CALM;
		break;
	}
	case NPC_STATE_DEAD:
	{
		m_nSkin = CREMATOR_SKIN_DEAD;
		break;
	}
	default:
		m_nSkin = CREMATOR_SKIN_CALM;
		break;
	}
}
float CNPC_Cremator::MaxYawSpeed(void)
{
	float flYS = 0;

	switch (GetActivity())
	{
	case	ACT_WALK_HURT:		flYS = 30;	break;
	case	ACT_RUN:			flYS = 90;	break;
	case	ACT_IDLE:			flYS = 90;	break;
	case	ACT_RANGE_ATTACK1:	flYS = 30;	break;
	default:
		flYS = 90;
		break;
	}
	return flYS;
}
void CNPC_Cremator::AlertSound(void)
{
	switch (random->RandomInt(0, 1))
	{
	case 0:
		EmitSound("NPC_Cremator.NPCAlert");
		break;
	case 1:
		EmitSound("NPC_Cremator.PlayerAlert");
		break;
	}
}
void CNPC_Cremator::IdleSound(void)
{
	int randSay = random->RandomInt(0, 2);
	if (randSay == 2)
	{
		if (m_bPlayAngrySound)
		{
			EmitSound("NPC_Cremator.AngryAmb");
		}
		else
		{
			EmitSound("NPC_Cremator.BreathingAmb");
		}
	}
	EmitSound("NPC_Cremator.ClothAmb");
}
void CNPC_Cremator::DeathSound(const CTakeDamageInfo &info)
{
	EmitSound("NPC_Cremator.DeathAmb");
}

int CNPC_Cremator::OnTakeDamage_Alive( const CTakeDamageInfo &info )
{
	if (info.GetAttacker())
	{
		if (info.GetAttacker()->IsPlayer())
		{
			m_bIsPlayerEnemy = true;
			m_bPlayAngrySound = true;
		}
		else if (info.GetAttacker()->IsNPC() && info.GetAttacker()->Classify() != Classify())
		{
			m_bIsNPCEnemy = true;
			m_bPlayAngrySound = true;
		}
	}

	return BaseClass::OnTakeDamage_Alive(info);
}

void CNPC_Cremator::TraceAttack(const CTakeDamageInfo &info, const Vector &vecDir, trace_t *ptr, CDmgAccumulator *pAccumulator)
{
	CTakeDamageInfo infoCopy = info;

	if (ptr->hitgroup == HITGROUP_HEAD)
	{
		m_bHeadshot = true;
	}

	if (infoCopy.GetDamageType() & DMG_BUCKSHOT)
	{
		infoCopy.ScaleDamage(0.625);
	}

	BaseClass::TraceAttack(infoCopy, vecDir, ptr, pAccumulator);
}
void CNPC_Cremator::Event_Killed(const CTakeDamageInfo &info)
{
	if (m_bHeadshot && ((info.GetAmmoType() == GetAmmoDef()->Index(".50BMG")) // sniper ammo
		|| (info.GetAmmoType() == GetAmmoDef()->Index("Buckshot")) // shotgun ammo
		|| (info.GetAmmoType() == GetAmmoDef()->Index("Gauss")) // gauss ammo
		|| (info.GetAmmoType() == GetAmmoDef()->Index("XBowBolt")) // crossbow ammo
		|| (info.GetAmmoType() == GetAmmoDef()->Index("357")))) // revolver ammo
	{
		SetBodygroup(2, 1); // turn the head off
		Vector vecDamageDir = info.GetDamageForce();
		VectorNormalize(vecDamageDir);
		DropHead(50, vecDamageDir); // spawn headprop
	}

	else if (info.GetDamageType() == DMG_BLAST) // blown up
	{
		SetBodygroup(2, 1);
		DropHead(300, Vector(0, 0, 1)); // spawn headprop
	}
	StopParticleEffects(this);
	m_nSkin = CREMATOR_SKIN_DEAD; // turn the eyes black
	SetBodygroup(1, 1); // turn the gun off

	BaseClass::Event_Killed(info);
}
const char *CNPC_Cremator::GetHeadpropModel(void)
{
	return "models/cremator_headprop.mdl";
}
void CNPC_Cremator::DropHead(int iVelocity, Vector &vecVelocity)
{
	CPhysicsProp *pGib = assert_cast<CPhysicsProp*>(CreateEntityByName("prop_physics"));
	pGib->SetModel(GetHeadpropModel());
	pGib->SetAbsOrigin(EyePosition());
	pGib->SetAbsAngles(EyeAngles());
	pGib->SetMoveType(MOVETYPE_VPHYSICS);
	pGib->SetCollisionGroup(COLLISION_GROUP_INTERACTIVE);
	pGib->SetOwnerEntity(this);
	pGib->Spawn();
	pGib->SetAbsVelocity(vecVelocity * (iVelocity + RandomFloat(-10, 10)));
	/*
	float flRandomVel = random->RandomFloat( -10, 10 );
	pGib->GetMassCenter( &vecDir );
	vecDir *= (iVelocity + flRandomVel) / 15;
	vecDir.z += 30.0f;
	AngularImpulse angImpulse = RandomAngularImpulse( -500, 500 );

	IPhysicsObject *pObj = pGib->VPhysicsGetObject();
	if ( pObj != NULL )
	{
	pObj->AddVelocity( &vecDir, &angImpulse );
	}
	*/
}
void CNPC_Cremator::HandleAnimEvent(animevent_t *pEvent)
{
	switch (pEvent->event)
	{
	case CREMATOR_AE_FLLEFT:
	{
		LeftFootHit(pEvent->eventtime);
	}
	break;
	case CREMATOR_AE_FLRIGHT:
	{
		RightFootHit(pEvent->eventtime);
	}
	break;
	case CREMATOR_AE_IMMO_START: // for combat
	{
		CBaseEntity *pEnemy = GetEnemyCombatCharacterPointer();
		Assert(pEnemy != NULL);
		DispatchSpray(pEnemy);
		DevMsg("%i ammo left\n", m_iAmmo);

		Vector flEnemyLKP = GetEnemyLKP();
		GetMotor()->SetIdealYawToTargetAndUpdate(flEnemyLKP);
	}
	break;
	case CREMATOR_AE_IMMO_PARTICLE: // Маркер для запуска системы частиц огнемета
	{
		DispatchParticleEffect("vapor_ray", PATTACH_POINT_FOLLOW, this, "muzzle");
		EmitSound("Weapon_Immolator.Single");
	}
	break;
	case CREMATOR_AE_IMMO_PARTICLEOFF:
	{
		StopParticleEffects(this);
		StopSound("Weapon_Immolator.Single");
		EmitSound("Weapon_Immolator.Stop");
	}
	break;
	case CREMATOR_AE_RELOAD:
	{
		ClearCondition(COND_CREMATOR_OUT_OF_AMMO);

		// Put your own ints here. This defines for how long a cremator would be able to fire at an enemy
		// Cremator gets shorter bursts on lower difficulty. On Hard, it can continously fire 60 attack cycles (1 ammo per cycle)
		m_iAmmo += 100;
	}
	break;
	case CREMATOR_AE_SPECIAL_START: // for corpse removal routine
	{
		DispatchParticleEffect("vapor_ray", PATTACH_POINT_FOLLOW, this, "muzzle");
		EmitSound("Weapon_Immolator.Single");
	}
	break;
	case CREMATOR_AE_SPECIAL_MIDDLE:
	{
		if (pCorpse && pCorpse != NULL) // double, triple, quadruple check!
		{
			IncinerateCorpse(pCorpse); // start the corpse burn
		}
	}
	break;
	case CREMATOR_AE_SPECIAL_END:
	{
		StopParticleEffects(this);
		StopSound("Weapon_Immolator.Single");
		EmitSound("Weapon_Immolator.Stop");
	}
	break;
#if 0
	case CREMATOR_AE_THROWGRENADE:
	{
		//	DevMsg( "Throwing incendiary grenade!\n" );
		ThrowIncendiaryGrenade();

		if (g_pGameRules->IsSkillLevel(SKILL_EASY))
		{
			m_flNextRangeAttack2Time = gpGlobals->curtime + random->RandomFloat(15.0f, 30.0f);
		}
		else if (g_pGameRules->IsSkillLevel(SKILL_HARD))
		{
			m_flNextRangeAttack2Time = gpGlobals->curtime + random->RandomFloat(5.0f, 10.0f);
		}
		else
		{
			m_flNextRangeAttack2Time = gpGlobals->curtime + random->RandomFloat(10.0f, 20.0f);
		}
	}
	break;
#endif
	default:
		BaseClass::HandleAnimEvent(pEvent);
		break;
	}
}
Vector CNPC_Cremator::LeftFootHit(float eventtime)
{
	Vector footPosition;

	GetAttachment("footleft", footPosition);
	CPASAttenuationFilter filter(this);
	EmitSound(filter, entindex(), "NPC_Cremator.FootstepLeft", &footPosition, eventtime);

	FootstepEffect(footPosition);
	return footPosition;
}
Vector CNPC_Cremator::RightFootHit(float eventtime)
{
	Vector footPosition;

	GetAttachment("footright", footPosition);
	CPASAttenuationFilter filter(this);
	EmitSound(filter, entindex(), "NPC_Cremator.FootstepRight", &footPosition, eventtime);

	FootstepEffect(footPosition);
	return footPosition;
}
void CNPC_Cremator::FootstepEffect(const Vector &origin)
{
	trace_t tr;
	AI_TraceLine(origin, origin - Vector(0, 0, 0), MASK_SOLID_BRUSHONLY, this, COLLISION_GROUP_NONE, &tr);
	float yaw = random->RandomInt(0, 0);
	for (int i = 0; i < 2; i++)
	{
		if (UTIL_PointContents(tr.endpos + Vector(0, 0, 1)) & MASK_WATER)
		{
			float flWaterZ = UTIL_FindWaterSurface(tr.endpos, tr.endpos.z, tr.endpos.z + 100.0f);

			CEffectData	data;
			data.m_fFlags = 0;
			data.m_vOrigin = tr.endpos;
			data.m_vOrigin.z = flWaterZ;
			data.m_vNormal = Vector(0, 0, 1);
			data.m_flScale = random->RandomFloat(10.0, 14.0);

			DispatchEffect("watersplash", data);
		}
		else
		{
			Vector dir = UTIL_YawToVector(yaw + i * 180) * 10;
			VectorNormalize(dir);
			dir.z = 0.25;
			VectorNormalize(dir);
			g_pEffects->Dust(tr.endpos, dir, 12, 50);
		}
	}
}
void CNPC_Cremator::DispatchSpray(CBaseEntity *pEntity)
{
	Vector vecSrc, vecAim;
	trace_t tr;

	Vector forward, right, up;
	AngleVectors(GetAbsAngles(), &forward, &right, &up);

	vecSrc = GetAbsOrigin() + up * 36; // hardcode FTW!!
	vecAim = GetShootEnemyDir(vecSrc);
	float deflection = 0.01;
	vecAim = vecAim + 1 * right * random->RandomFloat(0, deflection) + up * random->RandomFloat(-deflection, deflection);
	UTIL_TraceLine(vecSrc, vecSrc + vecAim * CREMATOR_IMMOLATOR_RANGE * 1.5, MASK_SOLID, this, COLLISION_GROUP_NONE, &tr);

	pEntity = tr.m_pEnt;

	if (pEntity != NULL && m_takedamage)
	{
		CTakeDamageInfo firedamage(this, this, sk_cremator_firedamage.GetFloat(), DMG_BURN);
		CTakeDamageInfo radiusdamage(this, this, sk_cremator_radiusdamage.GetFloat(), DMG_PLASMA);
		CalculateMeleeDamageForce(&firedamage, vecAim, tr.endpos);
		RadiusDamage(CTakeDamageInfo(this, this, 2, DMG_PLASMA), // AOE; this makes cremators absurdly powerfull sometimes btw
			tr.endpos,
			64.0f,
			CLASS_NONE,
			NULL);

		pEntity->DispatchTraceAttack((firedamage), vecAim, &tr);

		// This is bugged. This screenfades combines with default 'pain' screenfade and makes the screen go darker.
		//	if( pEntity->IsPlayer() )
		//	{
		//		color32 flamescreen = {250,200,40,50};
		//		UTIL_ScreenFade( pEntity, flamescreen, 0.5f, 0.1f, FFADE_IN|FFADE_PURGE );
		//	}

		ClearMultiDamage();
	}
#if 0	
	if (m_flFireDropletTimer < gpGlobals->curtime)
	{
		FireDroplet(100, 400, 25, -30, 30, random->RandomFloat(2.0, 6.0), 1);
		m_flFireDropletTimer = gpGlobals->curtime + random->RandomFloat(0.2f, 0.8f);
	}
#endif
	m_iAmmo--;
}
void CNPC_Cremator::RunAI(void)
{
	if (!HasCondition(COND_CREMATOR_FOUND_CORPSE))
		SearchForCorpses(); // FIXME: is it the best place for it?
	if (this->IsCurSchedule(SCHED_CREMATOR_INVESTIGATE_CORPSE) && pCorpse->WorldSpaceCenter().DistToSqr(WorldSpaceCenter()) <= 128 * 128)
	{
		GetNavigator()->StopMoving();
	}
	BaseClass::RunAI();
}
void CNPC_Cremator::StartTask(const Task_t *pTask)
{
	switch (pTask->iTask)
	{
	case TASK_CREMATOR_INVESTIGATE_CORPSE:
	{
		DevMsg("Cremator is investigating a corpse he saw\n");

		AssertMsg(pCorpse != NULL, "The corpse the cremator was after, it's gone!\n");

		if (pCorpse != NULL)
		{
			GetNavigator()->SetGoal(pCorpse->WorldSpaceCenter());

			if (IsUnreachable(pCorpse))
			{
				DevMsg("The corpse is unreachable, marked as such\n");
				pCorpse->SetOwnerEntity(this);	//HACKHACK: set the owner to prevent this unreachable corpse from being detected again. 
												//TODO: find a better solution. How to do it other than implementing a new EFlag?

				pCorpse = NULL;						//forget about this corpse.
				ClearCondition(COND_CREMATOR_FOUND_CORPSE);
				TaskFail(FAIL_NO_ROUTE);
			}
			TaskComplete();
		}
		break;
	}
	case TASK_CREMATOR_BURN_CORPSE:
	{
		if (!pCorpse)
		{
			TaskFail(FAIL_NO_TARGET);
			ClearCondition(COND_CREMATOR_FOUND_CORPSE);
			DevMsg("The corpse has been dealt with.\n");
		}
		else
		{
			GetMotor()->SetIdealYawToTarget(pCorpse->GetAbsOrigin(), 0, 0);
			SetActivity(ACT_FIRESPREAD);

			// IncinerateCorpse( pCorpse ); // Moved to HandleAnimEvent( ... )

			// TODO: Handle multiple corpses in a pile at once
		}
		break;
	}
	default:
		BaseClass::StartTask(pTask);
		break;
	}
}
void CNPC_Cremator::RunTask(const Task_t *pTask)
{
	switch (pTask->iTask)
	{
	case TASK_CREMATOR_INVESTIGATE_CORPSE: // FIXME: that never runs!
	{
		Msg("Running TASK_CREMATOR_INVESTIGATE_CORPSE\n");
		if (WorldSpaceCenter().DistToSqr(pCorpse->WorldSpaceCenter()) <= pTask->flTaskData * pTask->flTaskData) // Stop when we're close enough to the corpse.
		{
			GetNavigator()->StopMoving();
			DevMsg("Close enough to the corpse\n");
			TaskComplete();
		}
		break;
	}
	case TASK_CREMATOR_BURN_CORPSE:
	{
		if (IsActivityFinished())
		{
			TaskComplete();
			ClearCondition(COND_CREMATOR_FOUND_CORPSE);
		}
		break;
	}
	case TASK_CREMATOR_RANGE_ATTACK1:
	{
		Assert(GetEnemy() != NULL);
		SetActivity(ACT_RANGE_ATTACK1);

		Vector flEnemyLKP = GetEnemyLKP();
		GetMotor()->SetIdealYawToTargetAndUpdate(flEnemyLKP);

		if (m_iAmmo < 1 && IsActivityFinished())
		{
			SetCondition(COND_CREMATOR_OUT_OF_AMMO);
			StopParticleEffects(this);
			StopSound("Weapon_Immolator.Single");
			EmitSound("Weapon_Immolator.Stop");

			SetActivity(ACT_CREMATOR_RELOAD);
			TaskComplete();
			SetNextThink(gpGlobals->curtime + 0.1f);
		}
		break;
	}
	default:
		BaseClass::RunTask(pTask);
		break;
	}
}
int CNPC_Cremator::RangeAttack1Conditions(float flDot, float flDist)
{
	if (flDot < 0.7)
	{
		return COND_NOT_FACING_ATTACK;
	}

	else if (flDist > CREMATOR_IMMOLATOR_RANGE - 100) // create a buffer between us and the target
	{
		return COND_TOO_FAR_TO_ATTACK;
	}

	return COND_CAN_RANGE_ATTACK1;
}
NPC_STATE CNPC_Cremator::SelectIdealState(void)
{
	switch (m_NPCState)
	{
	case NPC_STATE_COMBAT:
	{
		// COMBAT goes to ALERT upon death of enemy
		if (GetEnemy() == NULL)
		{
			m_bPlayAngrySound = false;
			m_nSkin = CREMATOR_SKIN_CALM;
			return NPC_STATE_IDLE;
		}
		break;
	}
	case NPC_STATE_IDLE:
	{
		if (HasCondition(COND_CREMATOR_FOUND_CORPSE))
		{
			return NPC_STATE_ALERT;
		}
		break;
	}
	}
	return BaseClass::SelectIdealState();
}
Activity CNPC_Cremator::TranslateActivity(Activity activity)
{
	Assert(activity != ACT_INVALID);

	switch (activity)
	{
		case ACT_RUN:
		{
			return (Activity)ACT_WALK;
		}
	}

	return activity;
}
int CNPC_Cremator::TranslateSchedule(int scheduleType)
{
	switch (scheduleType)
	{
	case SCHED_RANGE_ATTACK1:
	{
		return SCHED_CREMATOR_RANGE_ATTACK1;
		break;
	}
	case SCHED_RANGE_ATTACK2:
	{
		return SCHED_CREMATOR_RANGE_ATTACK2;
		break;
	}
	case SCHED_MOVE_TO_WEAPON_RANGE:
	{
		return SCHED_CREMATOR_CHASE_ENEMY;
		break;
	}
	}
	return BaseClass::TranslateSchedule(scheduleType);
}
int CNPC_Cremator::SelectSchedule(void)
{
	switch (m_NPCState)
	{
	case NPC_STATE_IDLE:
	{
		if (!HasSpawnFlags(SF_CREMATOR_NO_PATROL_BEHAVIOUR))
			return SCHED_CREMATOR_PATROL;
		else
			return SCHED_PATROL_WALK_LOOP;
	}
	case NPC_STATE_ALERT:
	{
		if (HasCondition(COND_CREMATOR_FOUND_CORPSE) && !HasCondition(COND_LIGHT_DAMAGE | COND_HEAVY_DAMAGE) && GetEnemy() == NULL)
		{
			return SCHED_CREMATOR_INVESTIGATE_CORPSE;
		}
		if (!HasSpawnFlags(SF_CREMATOR_NO_PATROL_BEHAVIOUR))
			return SCHED_CREMATOR_PATROL;
		else
			return SCHED_PATROL_WALK_LOOP;
	}
	case NPC_STATE_COMBAT:
	{
		if (HasCondition(COND_CAN_RANGE_ATTACK1))
		{
			return SCHED_RANGE_ATTACK1;
		}
		else
		{
			return SCHED_CREMATOR_CHASE_ENEMY;
		}
#if 0
		if (HasCondition(COND_CAN_RANGE_ATTACK2))
		{
			return SCHED_CREMATOR_RANGE_ATTACK2;
		}
		if (HasCondition(COND_ENEMY_UNREACHABLE))
		{
			return SCHED_CREMATOR_RANGE_ATTACK2;
		}
#endif
		if (HasCondition(COND_ENEMY_DEAD))
		{
			m_bPlayAngrySound = false;
			m_nSkin = CREMATOR_SKIN_CALM;
			return BaseClass::SelectSchedule();
		}
	}
	}
	return BaseClass::SelectSchedule();
}
void CNPC_Cremator::OnScheduleChange(void)
{
	SelectSkin();
	StopParticleEffects(this);
	StopSound("Weapon_Immolator.Single");
	BaseClass::OnScheduleChange();
}
void CNPC_Cremator::PrescheduleThink(void)
{
	BaseClass::PrescheduleThink();
#if 0
	if (pInterestSound && pInterestSound->m_iType & GetSoundInterests())
	{
		if (!HasCondition(COND_CREMATOR_FOUND_CORPSE))
			SetCondition(COND_CREMATOR_FOUND_CORPSE);
	}
#endif

	switch (m_NPCState)
	{
	case NPC_STATE_ALERT:
	{
		if (HasCondition(COND_CREMATOR_FOUND_CORPSE))
		{
			if (HasCondition(COND_LIGHT_DAMAGE | COND_HEAVY_DAMAGE | COND_REPEATED_DAMAGE))
			{
				ClearCondition(COND_CREMATOR_FOUND_CORPSE); // stop caring about stinks if we've been hit
			}
		}
		break;
	}
	}
}


void CNPC_Cremator::SearchForCorpses(void)
{
//	float flSearchRadius = CREMATOR_DETECT_CORPSE_RANGE;

	// I had to do this by making the cremator look through entlist because OnListened() (stink) method is flawed. 
	// Originally this search was done with gEntList.FindEntityByClassnameNearest( "prop_ragdoll", GetAbsOrigin(), flSearchRadius ).
	// Naturally, a check for prop_ragdoll would return any prop_ragdoll and we know that it can include things like cars and matrasses...
	// For now I don't see a better way for it. Other than naming and/or flagging entities. I suppose I'll have to use scripted corpses after all.

	// TODO: it's better to have the cremator search the names provided by a filter_name entity associated with it, akin to enemy filters.
	CBaseEntity *pEnt = gEntList.FindEntityGenericWithin(this, "corpse", GetLocalOrigin(), sk_cremator_corpse_search_radius.GetFloat());

	//NDebugOverlay::Circle(GetLocalOrigin(), sk_cremator_corpse_search_radius.GetFloat(), 255, 255, 0, 150, true, 0.1f);

	if (pEnt && pEnt->GetOwnerEntity() != this)
	{
		DevMsg("Cremator has found a corpse\n");
		pCorpse = pEnt;

		//NDebugOverlay::Sphere(pEnt->GetLocalOrigin(), 72.0f, 0, 255, 50, true, 3.0f);

		SetCondition(COND_CREMATOR_FOUND_CORPSE);
	}
}
void CNPC_Cremator::IncinerateCorpse(CBaseEntity *pTarget)
{
	if (pTarget)
	{
		CEntityFlame *pFlame = CEntityFlame::Create(this);

		if (pFlame)
		{
			SetEffectEntity(NULL);
			pFlame->SetAbsOrigin(pTarget->GetAbsOrigin());
			pFlame->AttachToEntity(pTarget);
			pFlame->AddEFlags(EFL_FORCE_CHECK_TRANSMIT);
			pFlame->AddEffects(EF_BRIGHTLIGHT); // create light from the fire
			pFlame->SetLifetime(20.0); // burn for 20 seconds

			pTarget->AddFlag(FL_ONFIRE);
			pTarget->SetEffectEntity(pFlame);
			pTarget->SetRenderColor(50, 50, 50);

			pTarget->SetOwnerEntity(this); // HACKHACK - we're marking this corpse so that it won't be picked again in the future.
			DevMsg("This corpse has been handled. Moving on\n");
		}
	}
}

AI_BEGIN_CUSTOM_NPC(npc_cremator, CNPC_Cremator)

DECLARE_ACTIVITY(ACT_FIRESPREAD)
DECLARE_ACTIVITY(ACT_CREMATOR_ARM)
DECLARE_ACTIVITY(ACT_CREMATOR_DISARM)
DECLARE_ACTIVITY(ACT_CREMATOR_RELOAD)

DECLARE_CONDITION(COND_CREMATOR_OUT_OF_AMMO)
DECLARE_CONDITION(COND_CREMATOR_ENEMY_WITH_HIGHER_PRIORITY)
DECLARE_CONDITION(COND_CREMATOR_FOUND_CORPSE)

DECLARE_TASK(TASK_CREMATOR_RANGE_ATTACK1)
DECLARE_TASK(TASK_CREMATOR_RELOAD)
DECLARE_TASK(TASK_CREMATOR_INVESTIGATE_CORPSE)
DECLARE_TASK(TASK_CREMATOR_BURN_CORPSE)

DEFINE_SCHEDULE(
	SCHED_CREMATOR_CHASE_ENEMY,
	"	Tasks"
	"	TASK_GET_CHASE_PATH_TO_ENEMY 1024"
	"	TASK_SET_TOLERANCE_DISTANCE 250"
	"	TASK_WALK_PATH 0"
	"	TASK_WAIT_FOR_MOVEMENT 0"
	"	TASK_FACE_ENEMY 0"
	""
	"	Interrupts"
	"	COND_CAN_RANGE_ATTACK1"
	"	COND_ENEMY_DEAD"
	"	COND_LOST_ENEMY"
	"	COND_CREMATOR_ENEMY_WITH_HIGHER_PRIORITY"
)

DEFINE_SCHEDULE(
	SCHED_CREMATOR_RANGE_ATTACK1,
	"	Tasks"
	"	TASK_STOP_MOVING 0"
	"	TASK_FACE_ENEMY 0"
	"	TASK_ANNOUNCE_ATTACK 1"
	"	TASK_PLAY_SEQUENCE ACTIVITY:ACT_CREMATOR_ARM"
	"	TASK_CREMATOR_RANGE_ATTACK1 0"
	"	TASK_PLAY_SEQUENCE ACTIVITY:ACT_CREMATOR_RELOAD"
	""
	"	Interrupts"
	"	COND_HEAVY_DAMAGE"
	"	COND_REPEATED_DAMAGE"
	"	COND_ENEMY_DEAD"
	"	COND_TOO_FAR_TO_ATTACK"
	"	COND_CREMATOR_ENEMY_WITH_HIGHER_PRIORITY"
)

DEFINE_SCHEDULE(
	SCHED_CREMATOR_PATROL,
	"	Tasks"
	"	TASK_STOP_MOVING 0"
	"	TASK_GET_PATH_TO_RANDOM_NODE 1024"
	"	TASK_WALK_PATH 0"
	"	TASK_WAIT_FOR_MOVEMENT 0"
	"	TASK_WAIT 5"
	"	TASK_SET_SCHEDULE SCHEDULE:SCHED_CREMATOR_PATROL"
	""
	"	Interrupts"
	"	COND_CREMATOR_FOUND_CORPSE"
	"	COND_NEW_ENEMY"
	"	COND_LIGHT_DAMAGE"
	"	COND_HEAVY_DAMAGE"
)
DEFINE_SCHEDULE(
	SCHED_CREMATOR_INVESTIGATE_CORPSE, // we're here because we have COND_CREMATOR_FOUND_CORPSE.
	"	Tasks"
	"	TASK_WAIT_FOR_MOVEMENT 0"
	"	TASK_SOUND_WAKE 0" // Play the alert sound
	"	TASK_CREMATOR_INVESTIGATE_CORPSE 64" // analogous to TASK_GET_PATH_TO_BESTSCENT
											 //"	TASK_WALK_PATH 0"
	"	TASK_WAIT_FOR_MOVEMENT 0"
	"	TASK_CREMATOR_BURN_CORPSE 0" // Play the firespread animation
	"	TASK_WAIT 3"
	//"	TASK_SET_FAIL_SCHEDULE SCHEDULE:SCHED_CREMATOR_INVESTIGATE_CORPSE"
	"	TASK_SET_SCHEDULE SCHEDULE:SCHED_CREMATOR_PATROL" // resume patroling
	""
	"	Interrupts"
	"	COND_NEW_ENEMY"
	"	COND_SEE_ENEMY"
	"	COND_LIGHT_DAMAGE"
	"	COND_HEAVY_DAMAGE"
)
AI_END_CUSTOM_NPC()