/***
*
*	Copyright (c) 1996-2001, Valve LLC. All rights reserved.
*	
*	This product contains software technology licensed from Id 
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc. 
*	All Rights Reserved.
*
*   This source code contains proprietary and confidential information of
*   Valve LLC and its suppliers.  Access to this code is restricted to
*   persons who have executed a written SDK license with Valve.  Any access,
*   use or distribution of this code by or to any unlicensed person is illegal.
*
****/
//#if !defined( OEM_BUILD ) && !defined( HLDEMO_BUILD )

//=========================================================
// CONTROLLER
//=========================================================

#include	"cbase.h"
#include	"ai_default.h"
#include	"ai_task.h"
#include	"ai_schedule.h"
#include	"ai_node.h"
#include	"ai_hull.h"
#include	"ai_hint.h"
#include	"ai_route.h"
#include	"ai_navigator.h"
#include	"ai_basenpc_flyer.h"
#include	"soundent.h"
#include	"game.h"
#include	"npcevent.h"
#include	"entitylist.h"
#include	"activitylist.h"
#include	"animation.h"
#include	"basecombatweapon.h"
#include	"IEffects.h"
#include	"vstdlib/random.h"
#include	"engine/IEngineSound.h"
#include	"ammodef.h"
#include	"Sprite.h"
#include	"ai_moveprobe.h"

//=========================================================
// Monster's Anim Events Go Here
//=========================================================
#define	CONTROLLER_AE_HEAD_OPEN		1
#define	CONTROLLER_AE_BALL_SHOOT	2
#define	CONTROLLER_AE_SMALL_SHOOT	3
#define CONTROLLER_AE_POWERUP_FULL	4
#define CONTROLLER_AE_POWERUP_HALF	5

#define CONTROLLER_FLINCH_DELAY			2		// at most one flinch every n secs

#define DIST_TO_CHECK	200

ConVar sk_controller_health ( "sk_controller_health", "0" );
ConVar sk_controller_dmgzap ( "sk_controller_dmgzap", "0" );
ConVar sk_controller_speedball ( "sk_controller_speedball", "750", FCVAR_ARCHIVE );
ConVar sk_controller_dmgball ( "sk_controller_dmgball", "0" );

class CSprite;
class CNPC_Controller;

enum
{
	TASK_CONTROLLER_CHASE_ENEMY = LAST_SHARED_TASK,
	TASK_CONTROLLER_STRAFE,
	TASK_CONTROLLER_TAKECOVER,
	TASK_CONTROLLER_FAIL,
};

enum
{
	SCHED_CONTROLLER_CHASE_ENEMY = LAST_SHARED_SCHEDULE,
	SCHED_CONTROLLER_STRAFE,
	SCHED_CONTROLLER_TAKECOVER,
	SCHED_CONTROLLER_FAIL,
};

class CAlienController : public CAI_BaseFlyingBot
{
public:
	DECLARE_CLASS( CAlienController, CAI_BaseFlyingBot );
	DEFINE_CUSTOM_AI;
	DECLARE_DATADESC();

	void Spawn( void );
	void Precache( void );
	float SetYawSpeed( void );
	Class_T	Classify(void) { return CLASS_ALIEN_MILITARY; }
	void HandleAnimEvent( animevent_t *pEvent );
	void RunAI( void );
	int RangeAttack1Conditions ( float flDot, float flDist );	// balls
	int RangeAttack2Conditions ( float flDot, float flDist );	// head
	int TranslateSchedule( int Type );
	void RunTask ( const Task_t *pTask );
	void Stop( void );
	bool OverridePathMove( float flInterval );
	bool OverrideMove( float flInterval );
	void MoveToTarget(float flInterval, const Vector& vecMoveTarget);
	Activity NPC_TranslateActivity( Activity eNewActivity );
	void SetActivity ( Activity NewActivity );
	int LookupFloat( );
	
	class CNavigator : public CAI_ComponentWithOuter<CAlienController, CAI_Navigator>
	{
		typedef CAI_ComponentWithOuter<CAlienController, CAI_Navigator> BaseClass;
	public:
		CNavigator(CAlienController* pOuter)
			: BaseClass(pOuter)
		{
		}

		bool ActivityIsLocomotive(Activity activity) { return true; }
	};
	CAI_Navigator* CreateNavigator() { return new CNavigator(this); }
	
	bool ShouldGib( const CTakeDamageInfo &info ) {return false;};
	void PainSound( const CTakeDamageInfo &info );
	void AlertSound( void );
	void IdleSound( void );
	void AttackSound( void );
	void DeathSound( const CTakeDamageInfo &info );
	int OnTakeDamage_Alive( const CTakeDamageInfo &info );
	void Event_Killed( const CTakeDamageInfo &info );
	void SetSequence( int nSequence );
	int IRelationPriority( CBaseEntity *pTarget );

public:
	float m_flNextFlinch;

	float m_flShootTime;
	float m_flShootEnd;

	CSprite *m_pBall[2];	// hand balls
	int m_iBall[2];			// how bright it should be
	float m_iBallTime[2];	// when it should be that color
	int m_iBallCurrent[2];	// current brightness

	Vector m_vecEstVelocity;

	Vector m_velocity;
	bool m_fInCombat;
};

LINK_ENTITY_TO_CLASS( npc_acontroller, CAlienController );

BEGIN_DATADESC( CAlienController )
	DEFINE_ARRAY( m_pBall, FIELD_CLASSPTR, 2 ),
	DEFINE_ARRAY( m_iBall, FIELD_INTEGER, 2 ),
	DEFINE_ARRAY( m_iBallTime, FIELD_TIME, 2 ),
	DEFINE_ARRAY( m_iBallCurrent, FIELD_INTEGER, 2 ),
	DEFINE_FIELD( m_vecEstVelocity, FIELD_VECTOR ),
	DEFINE_FIELD( m_velocity, FIELD_VECTOR ),
	DEFINE_FIELD( m_fInCombat, FIELD_BOOLEAN ),
	DEFINE_FIELD( m_flShootTime, FIELD_TIME ),
	DEFINE_FIELD( m_flShootEnd, FIELD_TIME ),
END_DATADESC()

//=========================================================
// SetYawSpeed - allows each sequence to have a different
// turn rate associated with it.
//=========================================================
float CAlienController::SetYawSpeed ( void )
{
	if (m_fInCombat)
	{
		return 150.0f;
	}
	else
	{
		return 120.0f;
	}
}

int CAlienController::OnTakeDamage_Alive( const CTakeDamageInfo &info )
{
	// HACK HACK -- until we fix this.
	PainSound( info );
	return BaseClass::OnTakeDamage_Alive( info );
}

int CAlienController::IRelationPriority( CBaseEntity *pTarget )
{
	if ( pTarget->Classify() == CLASS_PLAYER )
	{
		 return BaseClass::IRelationPriority ( pTarget ) + 1;
	}

	return BaseClass::IRelationPriority( pTarget );
}

void CAlienController::Event_Killed( const CTakeDamageInfo &info )
{
	// shut off balls
	/*
	m_iBall[0] = 0;
	m_iBallTime[0] = gpGlobals->time + 4.0;
	m_iBall[1] = 0;
	m_iBallTime[1] = gpGlobals->time + 4.0;
	*/

	// fade balls
	if (m_pBall[0])
	{
		m_pBall[0]->FadeAndDie( 2 );
		m_pBall[0] = NULL;
	}
	if (m_pBall[1])
	{
		m_pBall[1]->FadeAndDie( 2 );
		m_pBall[1] = NULL;
	}

	BaseClass::Event_Killed( info );
}

void CAlienController::PainSound(const CTakeDamageInfo& info)
{
	if (random->RandomInt(0, 5) < 2)
	{
		CPASAttenuationFilter filter(this);
		EmitSound(filter, entindex(), "Controller.Pain");
	}
}	

void CAlienController::AlertSound( void )
{
	CPASAttenuationFilter filter(this);
	EmitSound(filter, entindex(), "Controller.Alert");
}

void CAlienController::IdleSound( void )
{
	CPASAttenuationFilter filter(this);
	EmitSound(filter, entindex(), "Controller.Idle");
}

void CAlienController::AttackSound( void )
{
	CPASAttenuationFilter filter(this);
	EmitSound(filter, entindex(), "Controller.Attack");
}

void CAlienController::DeathSound(const CTakeDamageInfo& info)
{
	CPASAttenuationFilter filter(this);
	EmitSound(filter, entindex(), "Controller.Die");
}

//=========================================================
// HandleAnimEvent - catches the monster-specific messages
// that occur when tagged animation frames are played.
//=========================================================
void CAlienController::HandleAnimEvent( animevent_t *pEvent )
{
	switch( pEvent->event )
	{
		case CONTROLLER_AE_HEAD_OPEN:
		{
			Vector vecStart;
			QAngle angleGun;

			GetAttachment(0, vecStart, angleGun);

			CBroadcastRecipientFilter filter;
			te->DynamicLight(filter, 0.0, &vecStart, 255, 192, 64, 0, 1, 0.2, 32);

			m_iBall[0] = 192;
			m_iBallTime[0] = gpGlobals->curtime + atoi(pEvent->options) / 15.0;
			m_iBall[1] = 255;
			m_iBallTime[1] = gpGlobals->curtime + atoi(pEvent->options) / 15.0;
		}
		break;

		case CONTROLLER_AE_BALL_SHOOT:
		{
			Vector vecStart;
			QAngle angleGun;

			GetAttachment(1, vecStart, angleGun);

			CBroadcastRecipientFilter filter;
			te->DynamicLight(filter, 0.0, &vecStart, 255, 192, 64, 0, 1, 0.1, 32);

			CAI_BaseNPC* pBall = (CAI_BaseNPC*)Create("controller_head_ball", vecStart, angleGun);

			pBall->SetAbsVelocity(Vector(0, 0, 32));
			pBall->SetEnemy(GetEnemy());

			m_iBall[0] = 0;
			m_iBall[1] = 0;
		}
		break;

		case CONTROLLER_AE_SMALL_SHOOT:
			AttackSound( );
			m_flShootTime = gpGlobals->curtime;
			m_flShootEnd = m_flShootTime + atoi( pEvent->options ) / 15.0;
			break;
		case CONTROLLER_AE_POWERUP_FULL:
			m_iBall[0] = 255;
			m_iBallTime[0] = gpGlobals->curtime + atoi( pEvent->options ) / 15.0;
			m_iBall[1] = 255;
			m_iBallTime[1] = gpGlobals->curtime + atoi( pEvent->options ) / 15.0;
			break;
		case CONTROLLER_AE_POWERUP_HALF:
			m_iBall[0] = 192;
			m_iBallTime[0] = gpGlobals->curtime + atoi( pEvent->options ) / 15.0;
			m_iBall[1] = 192;
			m_iBallTime[1] = gpGlobals->curtime + atoi( pEvent->options ) / 15.0;
			break;
		default:
			BaseClass::HandleAnimEvent( pEvent );
			break;
	}
}

//=========================================================
// Spawn
//=========================================================
void CAlienController::Spawn()
{
	Precache( );

	SetModel( "models/controller.mdl" );
	UTIL_SetSize( this, Vector( -32, -32, 0 ), Vector( 32, 32, 64 ));

	SetSolid( SOLID_BBOX );
	AddSolidFlags( FSOLID_NOT_STANDABLE );
	SetMoveType( MOVETYPE_STEP );
	SetGravity(0.001);
	AddFlag( FL_FLY );
	SetNavType( NAV_FLY );
	//FIXME: SetBloodType(); is not available???
	m_bloodColor = BLOOD_COLOR_GREEN;
	SetHealth(sk_controller_health.GetFloat());
	SetDefaultEyeOffset();// position of the eyes relative to monster's origin.
	m_flFieldOfView		= VIEW_FIELD_FULL;// indicates the width of this monster's forward view cone ( as a dotproduct result )
	m_NPCState			= NPC_STATE_NONE;
	
	CapabilitiesClear();
	CapabilitiesAdd( bits_CAP_MOVE_FLY | bits_CAP_INNATE_RANGE_ATTACK1 | bits_CAP_INNATE_RANGE_ATTACK2 | bits_CAP_MOVE_SHOOT);

	NPCInit();
}

//=========================================================
// Precache - precaches all resources this monster needs
//=========================================================
void CAlienController::Precache()
{
	PrecacheModel("models/controller.mdl");

	PrecacheScriptSound( "Controller.Pain" );
	PrecacheScriptSound( "Controller.Alert" );
	PrecacheScriptSound( "Controller.Die" );
	PrecacheScriptSound( "Controller.Idle" );
	PrecacheScriptSound( "Controller.Attack" );

	PrecacheModel( "sprites/xspark4.vmt");

	UTIL_PrecacheOther( "controller_energy_ball" );	
	UTIL_PrecacheOther( "controller_head_ball" );
}

Vector Intersect( Vector vecSrc, Vector vecDst, Vector vecMove, float flSpeed )
{
	Vector vecTo = vecDst - vecSrc;

	float a = DotProduct( vecMove, vecMove ) - flSpeed * flSpeed;
	float b = 0 * DotProduct(vecTo, vecMove); // why does this work?
	float c = DotProduct( vecTo, vecTo );

	float t;
	if (a == 0)
	{
		t = c / (flSpeed * flSpeed);
	}
	else
	{
		t = b * b - 4 * a * c;
		t = sqrt( t ) / (2.0 * a);
		float t1 = -b +t;
		float t2 = -b -t;

		if (t1 < 0 || t2 < t1)
			t = t2;
		else
			t = t1;
	}
	
	if (t < 0.1)
		t = 0.1;
	if (t > 10.0)
		t = 10.0;

	Vector vecHit = vecTo + vecMove * t;
	VectorNormalize( vecHit );
	return vecHit * flSpeed;
}


int CAlienController::LookupFloat( )
{
	if (m_velocity.Length( ) < 32.0)
	{
		return LookupSequence( "up" );
	}
	
	Vector vecForward, vecRight, vecUp;
	AngleVectors( GetAbsAngles(), &vecForward, &vecRight, &vecUp );

	float x = DotProduct( vecForward, m_velocity );
	float y = DotProduct( vecRight, m_velocity );
	float z = DotProduct( vecUp, m_velocity );

	if (fabs(x) > fabs(y) && fabs(x) > fabs(z))
	{
		if (x > 0)
			return LookupSequence( "forward");
		else
			return LookupSequence( "backward");
	}
	else if (fabs(y) > fabs(z))
	{
		if (y > 0)
			return LookupSequence( "right");
		else
			return LookupSequence( "left");
	}
	else
	{
		if (z > 0)
			return LookupSequence( "up");
		else
			return LookupSequence( "down");
	}
}

//=========================================================
// RunTask 
//=========================================================
void CAlienController::RunTask ( const Task_t *pTask )
{
	if (m_flShootEnd > gpGlobals->curtime)
	{
		Vector vecHand;
		QAngle vecAngle;
		
		GetAttachment( 2, vecHand, vecAngle );
	
		while (m_flShootTime < m_flShootEnd && m_flShootTime < gpGlobals->curtime)
		{
			Vector vecSrc = vecHand + GetAbsVelocity() * (m_flShootTime - gpGlobals->curtime);
			Vector vecDir;
			
			if (GetEnemy() != NULL)
			{
				if (HasCondition( COND_SEE_ENEMY ))
				{
					m_vecEstVelocity = m_vecEstVelocity * 0.5 + GetEnemy()->GetAbsVelocity() * 0.5;
				}
				else
				{
					m_vecEstVelocity = m_vecEstVelocity * 0.8;
				}
				vecDir = Intersect( vecSrc, GetEnemy()->BodyTarget( GetAbsOrigin() ), m_vecEstVelocity, sk_controller_speedball.GetFloat() );
				float delta = 0.03490; // +-2 degree
				vecDir = vecDir + Vector( random->RandomFloat( -delta, delta ), random->RandomFloat( -delta, delta ), random->RandomFloat( -delta, delta ) ) * sk_controller_speedball.GetFloat();

				vecSrc = vecSrc + vecDir * (gpGlobals->curtime - m_flShootTime);
				CAI_BaseNPC *pBall = (CAI_BaseNPC*)Create( "controller_energy_ball", vecSrc, GetAbsAngles(), this );
				pBall->SetAbsVelocity( vecDir );
			}
			m_flShootTime += 0.2;
		}

		if (m_flShootTime > m_flShootEnd)
		{
			m_iBall[0] = 64;
			m_iBallTime[0] = m_flShootEnd;
			m_iBall[1] = 64;
			m_iBallTime[1] = m_flShootEnd;
			m_fInCombat = false;
		}
	}

	switch ( pTask->iTask )
	{
	case TASK_WAIT_FOR_MOVEMENT:
	case TASK_WAIT:
	case TASK_WAIT_FACE_ENEMY:
	case TASK_WAIT_PVS:
		if( GetEnemy() )
		{
			float idealYaw = UTIL_VecToYaw( GetEnemy()->GetAbsOrigin() - GetAbsOrigin() );
			GetMotor()->SetIdealYawAndUpdate( idealYaw );
		}

		if ( IsSequenceFinished() || GetActivity() == ACT_IDLE)
		{
			m_fInCombat = false;
		}

		BaseClass::RunTask ( pTask );

		if (!m_fInCombat)
		{
			if (HasCondition(COND_CAN_RANGE_ATTACK1))
			{
				SetActivity( ACT_RANGE_ATTACK1 );
				SetCycle( 0 ); 
				ResetSequenceInfo( );
				m_fInCombat = true;
			}
			else if (HasCondition(COND_CAN_RANGE_ATTACK2))
			{
				SetActivity( ACT_RANGE_ATTACK2 );
				SetCycle( 0 );
				ResetSequenceInfo( );
				m_fInCombat = true;
			}
			else
			{
				int iFloat = LookupFloat();
				if( IsSequenceFinished() || iFloat != GetActivity() )
				{
					SetActivity( (Activity)iFloat );
				}
			}
		}
		break;
	default: 
		BaseClass::RunTask ( pTask );
		break;
	}
}

void CAlienController::SetSequence( int nSequence )
{
	BaseClass::SetSequence( nSequence );
}

//=========================================================
//=========================================================
int  CAlienController::TranslateSchedule( int Type ) 
{
	switch	( Type )
	{
	case SCHED_CHASE_ENEMY:
		return SCHED_CONTROLLER_CHASE_ENEMY;
	case SCHED_RANGE_ATTACK1:
		return SCHED_CONTROLLER_STRAFE;
	case SCHED_RANGE_ATTACK2:
	case SCHED_MELEE_ATTACK1:
	case SCHED_MELEE_ATTACK2:
	case SCHED_TAKE_COVER_FROM_ENEMY:
		return SCHED_CONTROLLER_TAKECOVER;
	case SCHED_FAIL:
		return SCHED_CONTROLLER_FAIL;
	default:
		break;
	}

	return BaseClass::TranslateSchedule(Type);
}

//=========================================================
// CheckRangeAttack1  - shoot a bigass energy ball out of their head
//
//=========================================================
int CAlienController::RangeAttack1Conditions( float flDot, float flDist )
{
	if( flDist > 2048 )
	{
		return COND_TOO_FAR_TO_ATTACK;
	}

	if( flDist <= 256 )
	{
		return COND_TOO_CLOSE_TO_ATTACK;
	}
	
	return COND_CAN_RANGE_ATTACK1;
}


int CAlienController::RangeAttack2Conditions( float flDot, float flDist )
{
	if( flDist > 2048 )
	{
		return COND_TOO_FAR_TO_ATTACK;
	}

	if( flDist <= 64 )
	{
		return COND_TOO_CLOSE_TO_ATTACK;
	}

	return COND_CAN_RANGE_ATTACK2;
}

Activity CAlienController::NPC_TranslateActivity( Activity eNewActivity )
{
	switch ( eNewActivity)
	{
	case ACT_IDLE:
		return (Activity)LookupFloat();
		break;

	default:
		return BaseClass::NPC_TranslateActivity( eNewActivity );
	}
}

void CAlienController::SetActivity ( Activity NewActivity )
{
	BaseClass::SetActivity( NewActivity );
	m_flGroundSpeed = 100;
}

//=========================================================
// RunAI
//=========================================================
void CAlienController::RunAI( void )
{
	BaseClass::RunAI();
	Vector vecStart;
	QAngle angleGun;

	if ( !IsAlive() )
		return;

	for (int i = 0; i < 2; i++)
	{
		if (m_pBall[i] == NULL)
		{
			m_pBall[i] = CSprite::SpriteCreate( "sprites/xspark4.vmt", GetAbsOrigin(), true );
			m_pBall[i]->SetTransparency( kRenderGlow, 255, 255, 255, 255, kRenderFxNoDissipation );
			m_pBall[i]->SetAttachment( this, (i + 3) );
			m_pBall[i]->SetScale( 1.0 );
		}

		float t = m_iBallTime[i] - gpGlobals->curtime;
		if (t > 0.1)
			t = 0.1 / t;
		else
			t = 1.0;

		m_iBallCurrent[i] += (m_iBall[i] - m_iBallCurrent[i]) * t;

		m_pBall[i]->SetBrightness( m_iBallCurrent[i] );

		GetAttachment( i + 2, vecStart, angleGun );
		m_pBall[i]->SetAbsOrigin( vecStart );
		
		CBroadcastRecipientFilter filter;
		GetAttachment( i + 3, vecStart, angleGun );
		te->DynamicLight( filter, 0.0, &vecStart, 255, 192, 64, 0, m_iBallCurrent[i] / 8, 0.5, 0 );
	}
}

void CAlienController::Stop( void ) 
{ 
	SetIdealActivity( GetStoppedActivity() );
}

bool CAlienController::OverridePathMove( float flInterval )
{
	CBaseEntity *pMoveTarget = (GetTarget()) ? GetTarget() : GetEnemy();
	Vector waypointDir = GetNavigator()->GetCurWaypointPos() - GetLocalOrigin();

	float flWaypointDist = waypointDir.Length2D();
	VectorNormalize(waypointDir);
	
	// cut corner?
	if (flWaypointDist < 128)
	{
		if (m_flGroundSpeed > 100)
			m_flGroundSpeed -= 40;
	}
	else
	{
		if (m_flGroundSpeed < 400)
			m_flGroundSpeed += 10;
	}
	
	m_velocity = m_velocity * 0.8 + m_flGroundSpeed * waypointDir * 0.5;
	SetAbsVelocity( m_velocity );
	
	Vector checkPos = GetLocalOrigin() + (waypointDir * (m_flGroundSpeed * flInterval));

	AIMoveTrace_t moveTrace;
	GetMoveProbe()->MoveLimit( NAV_FLY, GetLocalOrigin(), checkPos, MASK_NPCSOLID|CONTENTS_WATER,pMoveTarget, &moveTrace);
	if (IsMoveBlocked( moveTrace ))
	{
		TaskFail(FAIL_NO_ROUTE);
		GetNavigator()->ClearGoal();
		return true;
	}
	
	Vector lastPatrolDir = GetNavigator()->GetCurWaypointPos() - GetLocalOrigin();
	
	if ( ProgressFlyPath( flInterval, pMoveTarget, MASK_NPCSOLID, false, 64 ) == AINPP_COMPLETE )
	{
		m_vLastPatrolDir = lastPatrolDir;
		VectorNormalize(m_vLastPatrolDir);
		return true;
	}
	return false;
}

bool CAlienController::OverrideMove( float flInterval )
{
	CBaseEntity	*pTargetEnt;
	Vector vMoveTargetPos(0,0,0);
	
	if (m_flGroundSpeed == 0)
	{
		m_flGroundSpeed = 100;
		// TaskFail( );
		// return;
	}
	
	// if the monster is moving directly towards an entity (enemy for instance), we'll set this pointer
	// to that entity for the CheckLocalMove and Triangulate functions.
	pTargetEnt = NULL;
	
	if (GetTarget() != NULL )
	{
		pTargetEnt = GetTarget();
		vMoveTargetPos = GetTarget()->GetAbsOrigin();
	}
	else if (GetEnemy() != NULL)
	{
		// only on a PURE move to enemy ( i.e., ONLY MF_TO_ENEMY set, not MF_TO_ENEMY and DETOUR )
		pTargetEnt = GetEnemy();
		vMoveTargetPos = GetEnemy()->GetAbsOrigin();
	}
	
	if (pTargetEnt)
	{
		trace_t tr;
		UTIL_TraceEntity(this, GetAbsOrigin(), vMoveTargetPos, MASK_NPCSOLID_BRUSHONLY, pTargetEnt, GetCollisionGroup(), &tr);
	}

	// -----------------------------------------------------------------
	// If I have a route, keep it updated and move toward target
	// ------------------------------------------------------------------
	if (GetNavigator()->IsGoalActive())
	{
		if ( OverridePathMove( flInterval ) )
			return true;
	}
	else
	{
		//do nothing
		Stop();
		TaskComplete();
	}

	return true;
}

void CAlienController::MoveToTarget(float flInterval, const Vector& vecMoveTarget)
{
	MoveToLocation(flInterval, vecMoveTarget, 300.0, 600.0, 9.0);
}

//=========================================================
// AI Schedules Specific to this monster
//=========================================================

AI_BEGIN_CUSTOM_NPC(npc_acontroller, CAlienController)

	//declare our tasks
	DECLARE_TASK( TASK_CONTROLLER_CHASE_ENEMY )
	DECLARE_TASK( TASK_CONTROLLER_STRAFE )
	DECLARE_TASK( TASK_CONTROLLER_TAKECOVER )
	DECLARE_TASK( TASK_CONTROLLER_FAIL )

	//=========================================================
	// > SCHED_CONTROLLER_CHASE_ENEMY
	//=========================================================
	DEFINE_SCHEDULE
	(
		SCHED_CONTROLLER_CHASE_ENEMY,

		"	Tasks"
		"		TASK_GET_PATH_TO_ENEMY			128"
		"		TASK_WAIT_FOR_MOVEMENT				0"
		"	"
		"	Interrupts"
		"		COND_NEW_ENEMY"
		"		COND_TASK_FAILED"


	)

	//=========================================================
	// > SCHED_CONTROLLER_STRAFE
	//=========================================================
	DEFINE_SCHEDULE
	(
		SCHED_CONTROLLER_STRAFE,

		"	Tasks"
		"		TASK_WAIT						0.2"
		"		TASK_GET_PATH_TO_ENEMY			128"
		"		TASK_WAIT_FOR_MOVEMENT			  0"
		"		TASK_WAIT						  1"
		"	"
		"	Interrupts"
		"		COND_NEW_ENEMY"
	)

	//=========================================================
	// > SCHED_CONTROLLER_TAKECOVER
	//=========================================================
	DEFINE_SCHEDULE
	(
		SCHED_CONTROLLER_TAKECOVER,

		"	Tasks"
		"		TASK_WAIT						0.2"
		"		TASK_FIND_COVER_FROM_ENEMY		  0"
		"		TASK_WAIT_FOR_MOVEMENT			  0"
		"		TASK_WAIT						  1"
		"	"
		"	Interrupts"
		"		COND_NEW_ENEMY"
	)

	//=========================================================
	// > SCHED_CONTROLLER_FAIL
	//=========================================================
	DEFINE_SCHEDULE
	(
		SCHED_CONTROLLER_FAIL,

		"	Tasks"
		"		TASK_STOP_MOVING					0"
		"		TASK_SET_ACTIVITY					ACTIVITY:ACT_IDLE"
		"		TASK_WAIT							2"
		"		TASK_WAIT_PVS						0"
	)

AI_END_CUSTOM_NPC()

//=========================================================
// Controller bouncy ball attack
//=========================================================
class CAlienControllerHeadBall : public CAI_BaseNPC
{
public:
	DECLARE_CLASS( CAlienControllerHeadBall, CAI_BaseNPC );

	DECLARE_DATADESC();
	void Spawn( void );
	void Precache( void );
	void HuntThink( void );
	void DieThink( void );
	void BounceTouch( CBaseEntity *pOther );
	void MovetoTarget( Vector vecTarget );
	
public:
	float m_flSpawnTime;
	Vector m_vecIdeal;
	EHANDLE m_hOwner;
	CSprite *m_pSprite;
};
LINK_ENTITY_TO_CLASS( controller_head_ball, CAlienControllerHeadBall );

BEGIN_DATADESC( CAlienControllerHeadBall )

	DEFINE_THINKFUNC( HuntThink ),
	DEFINE_THINKFUNC( DieThink ),
	DEFINE_ENTITYFUNC( BounceTouch ),

	DEFINE_FIELD( m_pSprite, FIELD_CLASSPTR ),

	DEFINE_FIELD( m_flSpawnTime, FIELD_TIME ),
	DEFINE_FIELD( m_vecIdeal, FIELD_VECTOR ),
	DEFINE_FIELD( m_hOwner, FIELD_EHANDLE ),

END_DATADESC()

void CAlienControllerHeadBall::Spawn( void )
{
	Precache( );
	// motor
	SetMoveType( MOVETYPE_FLY );
	SetSolid( SOLID_BBOX );
	UTIL_SetSize( this, Vector( 0, 0, 0), Vector(0, 0, 0) );

	m_pSprite = CSprite::SpriteCreate( "sprites/xspark4.vmt", GetAbsOrigin(), FALSE );
	m_pSprite->SetTransparency( kRenderTransAdd, 255, 255, 255, 255, kRenderFxNoDissipation );
	m_pSprite->SetAttachment( this, 0 );
	m_pSprite->SetScale( 2.0 );

	UTIL_SetOrigin(this, GetAbsOrigin());

	SetThink( &CAlienControllerHeadBall::HuntThink );
	SetTouch( &CAlienControllerHeadBall::BounceTouch );

	SetNextThink( gpGlobals->curtime + 0.1 );

	m_hOwner = GetOwnerEntity();

	m_flSpawnTime = gpGlobals->curtime;
}

void CAlienControllerHeadBall::Precache( void )
{
	PrecacheModel( "sprites/xspark4.vmt");
}

extern short g_sModelIndexLaser;	

void CAlienControllerHeadBall::HuntThink( void  )
{
	SetNextThink( gpGlobals->curtime + 0.1 );

	if( !m_pSprite )
	{
		return;
	}

	m_pSprite->SetBrightness( m_pSprite->GetBrightness() - 5, 0.1f );

	CBroadcastRecipientFilter filter;
	te->DynamicLight( filter, 0.0, &GetAbsOrigin(), 255, 255, 255, 0, m_pSprite->GetBrightness() / 16, 0.2, 0 );

	// check world boundaries
	if (gpGlobals->curtime - m_flSpawnTime > 5 || m_pSprite->GetBrightness() < 64 || !IsInWorld() )
	{
		SetTouch( NULL );
		SetThink( &CAlienControllerHeadBall::DieThink );
		SetNextThink( gpGlobals->curtime );
		return;
	}

	if( !GetEnemy() )
		return;

	MovetoTarget( GetEnemy()->GetAbsOrigin() );

	if ((GetEnemy()->WorldSpaceCenter() - GetAbsOrigin()).Length() < 64)
	{
		trace_t tr;

		UTIL_TraceLine( GetAbsOrigin(), GetEnemy()->WorldSpaceCenter(), MASK_ALL, this, COLLISION_GROUP_NONE, &tr );

		CBaseEntity *pEntity = tr.m_pEnt;

		if (pEntity != NULL && pEntity->m_takedamage == DAMAGE_YES)
		{
			ClearMultiDamage( );
			Vector dir = GetAbsVelocity();
			VectorNormalize( dir );
			CTakeDamageInfo info( this, this, sk_controller_dmgball.GetFloat(), DMG_SHOCK );
			CalculateMeleeDamageForce( &info, dir, tr.endpos );
			pEntity->DispatchTraceAttack( info, dir, &tr );
			ApplyMultiDamage();

			int haloindex = 0;
			int fadelength = 0;
			int amplitude = 0;
			const Vector vecEnd = tr.endpos;
			te->BeamEntPoint( filter, 0.0, entindex(), NULL, 0, &(tr.m_pEnt->GetAbsOrigin()), 
				g_sModelIndexLaser, haloindex /* no halo */, 0, 10, 3, 20, 20, fadelength, 
				amplitude, 255, 255, 255, 255, 10 );

		}

		UTIL_EmitAmbientSound( GetSoundSourceIndex(), GetAbsOrigin(), "Controller.ElectroSound", 0.5, SNDLVL_NORM, 0, 100 );

		SetNextAttack( gpGlobals->curtime + 3.0 );

		SetThink( &CAlienControllerHeadBall::DieThink );
		SetNextThink( gpGlobals->curtime + 0.3 );
	}

	// Crawl( );
}

void CAlienControllerHeadBall :: DieThink( void  )
{
	UTIL_Remove( m_pSprite );
	UTIL_Remove( this );
}

void CAlienControllerHeadBall :: MovetoTarget( Vector vecTarget )
{
	// accelerate
	float flSpeed = m_vecIdeal.Length();
	if (flSpeed == 0)
	{
		m_vecIdeal = GetAbsVelocity();
		flSpeed = m_vecIdeal.Length();
	}
	else if (flSpeed > 400)
	{
		VectorNormalize( m_vecIdeal );
		m_vecIdeal = m_vecIdeal * 400;
	}
	
	Vector t = vecTarget - GetAbsOrigin();
	VectorNormalize(t);
	m_vecIdeal = m_vecIdeal + t * 100;
	SetAbsVelocity(m_vecIdeal);
}

void CAlienControllerHeadBall::BounceTouch( CBaseEntity *pOther )
{
	Vector vecDir = m_vecIdeal;
	VectorNormalize( vecDir );

	trace_t tr;
	tr = CBaseEntity::GetTouchTrace( );

	float n = -DotProduct(tr.plane.normal, vecDir);

	vecDir = 2.0 * tr.plane.normal * n + vecDir;

	m_vecIdeal = vecDir * m_vecIdeal.Length();
}

class CAlienControllerZapBall : public CAI_BaseNPC
{
public:
	DECLARE_CLASS( CAlienControllerZapBall, CAI_BaseNPC );

	DECLARE_DATADESC();

	void Spawn( void );
	void Precache( void );
	void AnimateThink( void );
	void ExplodeTouch( CBaseEntity *pOther );

public:
	EHANDLE m_hOwner;
	float m_flSpawnTime;
	CSprite *m_pSprite;
};
LINK_ENTITY_TO_CLASS( controller_energy_ball, CAlienControllerZapBall );

BEGIN_DATADESC( CAlienControllerZapBall )

	DEFINE_THINKFUNC( AnimateThink ),
	DEFINE_ENTITYFUNC( ExplodeTouch ),

	DEFINE_FIELD( m_hOwner, FIELD_EHANDLE ),
	DEFINE_FIELD( m_flSpawnTime, FIELD_TIME ),
	DEFINE_FIELD( m_pSprite, FIELD_CLASSPTR ),

END_DATADESC()

void CAlienControllerZapBall::Spawn( void )
{
	Precache( );
	// motor
	SetMoveType( MOVETYPE_FLY );
	SetSolid( SOLID_BBOX );
	UTIL_SetSize( this, Vector( 0, 0, 0), Vector(0, 0, 0) );

	m_pSprite = CSprite::SpriteCreate( "sprites/xspark4.vmt", GetAbsOrigin(), FALSE );
	m_pSprite->SetTransparency( kRenderTransAdd, 255, 255, 255, 255, kRenderFxNoDissipation );
	m_pSprite->SetAttachment( this, 0 );
	m_pSprite->SetScale( 0.5 );

	UTIL_SetOrigin(this, GetAbsOrigin());

	SetThink( &CAlienControllerZapBall::AnimateThink );
	SetTouch( &CAlienControllerZapBall::ExplodeTouch );

	m_hOwner = GetOwnerEntity();
	m_flSpawnTime = gpGlobals->curtime; // keep track of when ball spawned
	SetNextThink( gpGlobals->curtime + 0.1 );
}

void CAlienControllerZapBall::Precache( void )
{
	PrecacheModel( "sprites/xspark4.vmt");
	// PRECACHE_SOUND("debris/zap4.wav");
	// PRECACHE_SOUND("weapons/electro4.wav");
}

void CAlienControllerZapBall :: AnimateThink( void  )
{
	SetNextThink(gpGlobals->curtime + 0.1);
	
	SetCycle(((int)GetCycle() + 1) % 11);

	if (gpGlobals->curtime - m_flSpawnTime > 5 || GetAbsVelocity().Length() < 10)
	{
		SetTouch( NULL );
		UTIL_Remove( m_pSprite );
		UTIL_Remove( this );
	}
}


void CAlienControllerZapBall::ExplodeTouch( CBaseEntity *pOther )
{
	if (m_takedamage = DAMAGE_YES )
	{
		trace_t tr;
		tr = GetTouchTrace( );

		ClearMultiDamage( );

		Vector vecAttackDir = GetAbsVelocity();
		VectorNormalize( vecAttackDir );

		EHANDLE m_hpevOwner;
		if (m_hOwner != NULL)
		{
			m_hpevOwner = m_hOwner;
		}
		else
		{
			m_hpevOwner = this;
		}
		
		CTakeDamageInfo info( this, m_hpevOwner, sk_controller_dmgball.GetFloat(), DMG_ENERGYBEAM );
		CalculateMeleeDamageForce( &info, vecAttackDir, tr.endpos );
		pOther->DispatchTraceAttack( info, vecAttackDir, &tr );
		ApplyMultiDamage();

		UTIL_EmitAmbientSound( GetSoundSourceIndex(), tr.endpos, "Controller.ElectroSound", 0.3, SNDLVL_NORM, 0, random->RandomInt( 90, 99 ) );
	}

	UTIL_Remove( m_pSprite );
	UTIL_Remove( this );
}

//#endif		// !OEM && !HLDEMO