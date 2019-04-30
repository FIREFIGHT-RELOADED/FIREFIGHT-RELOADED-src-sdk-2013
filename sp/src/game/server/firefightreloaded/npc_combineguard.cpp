//========= Copyright © LOLOLOL, All rights reserved. ============//
//
// Purpose: A big heavily-armored monstrosity who works for the combine and carries the combine 
// equivalent to the BFG.
//
// FIXED DAMAGE! EXPLOSION ON DEATH NO LONGER NECESSARY!!!!
//=============================================================================//

#include "cbase.h"
#include "ai_task.h"
#include "ai_default.h"
#include "ai_schedule.h"
#include "ai_hull.h"
#include "ai_motor.h"
#include "ai_memory.h"
#include "npc_combine.h"
#include "physics.h"
#include "bitstring.h"
#include "activitylist.h"
#include "game.h"
#include "npcevent.h"
#include "player.h"
#include "entitylist.h"
#include "ai_interactions.h"
#include "soundent.h"
#include "gib.h"
#include "shake.h"
#include "Sprite.h"
#include "explode.h"
#include "grenade_homer.h"
#include "ai_basenpc.h"
#include "soundenvelope.h"
#include "IEffects.h"
#include "vstdlib/random.h"
#include "engine/IEngineSound.h"
#include "prop_combine_ball.h"
#include "props.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

ConVar sk_combineguard_health( "sk_combineguard_health", "0" );

class CSprite;

extern void CreateConcussiveBlast( const Vector &origin, const Vector &surfaceNormal, CBaseEntity *pOwner, float magnitude );

#define	COMBINEGUARD_MODEL	"models/combine_guard.mdl"
#define	COMBINEGUARD_GUN_MODEL	"models/combine_guard_gun.mdl"

#define CGUARD_MSG_SHOT	1
#define CGUARD_MSG_SHOT_START 2

enum 
{
	CGUARD_REF_INVALID = 0,
	CGUARD_REF_MUZZLE,
	CGUARD_REF_LEFT_SHOULDER,
	CGUARD_REF_HUMAN_HEAD,
	CGUARD_REF_RIGHT_ARM_HIGH,
	CGUARD_REF_RIGHT_ARM_LOW,
	CGUARD_REF_LEFT_ARM_HIGH,
	CGUARD_REF_LEFT_ARM_LOW,
	CGUARD_REF_TORSO,
	CGUARD_REF_LOWER_TORSO,
	CGUARD_REF_RIGHT_THIGH_HIGH,
	CGUARD_REF_RIGHT_THIGH_LOW,
	CGUARD_REF_LEFT_THIGH_HIGH,
	CGUARD_REF_LEFT_THIGH_LOW,
//	CGUARD_SHOVE,
	CGUARD_REF_RIGHT_SHIN_HIGH,
	CGUARD_REF_RIGHT_SHIN_LOW,
	CGUARD_REF_LEFT_SHIN_HIGH,
	CGUARD_REF_LEFT_SHIN_LOW,
	CGUARD_REF_RIGHT_SHOULDER,
	
	NUM_CGUARD_ATTACHMENTS,
};

enum
{
	CGUARD_BGROUP_INVALID = -1,
	CGUARD_BGROUP_MAIN,
	CGUARD_BGROUP_GUN,
	CGUARD_BGROUP_RIGHT_SHOULDER,
	CGUARD_BGROUP_LEFT_SHOULDER,
	CGUARD_BGROUP_HEAD,
	CGUARD_BGROUP_RIGHT_ARM,
	CGUARD_BGROUP_LEFT_ARM,
	CGUARD_BGROUP_TORSO,
	CGUARD_BGROUP_LOWER_TORSO,
	CGUARD_BGROUP_RIGHT_THIGH,
	CGUARD_BGROUP_LEFT_THIGH,
	CGUARD_BGROUP_RIGHT_SHIN,
	CGUARD_BGROUP_LEFT_SHIN,

	NUM_CGUARD_BODYGROUPS,
};

struct armorPiece_t
{
	DECLARE_DATADESC();

	bool destroyed;
	int health;
};

class CNPC_CombineGuard : public CAI_BaseNPC
{
	DECLARE_CLASS( CNPC_CombineGuard, CAI_BaseNPC );
public:

	DECLARE_SERVERCLASS();

	CNPC_CombineGuard( void );

//	int	OnTakeDamage( const CTakeDamageInfo &info );
	int	OnTakeDamage_Alive( const CTakeDamageInfo &info );
	void Event_Killed( const CTakeDamageInfo &info );
	int	TranslateSchedule( int type );
	int	MeleeAttack1Conditions( float flDot, float flDist );
	int	RangeAttack1Conditions( float flDot, float flDist );
	void DropGun(int iVelocity, const Vector &vecVelocity);

	void Precache( void );
	void Spawn( void );
	void PrescheduleThink( void );
	void TraceAttack( CBaseEntity *pAttacker, float flDamage, const Vector &vecDir, trace_t *ptr, int bitsDamageType );
	void HandleAnimEvent( animevent_t *pEvent );
	void StartTask( const Task_t *pTask );
	void RunTask( const Task_t *pTask );

	bool AllArmorDestroyed( void );
	bool IsArmorPiece( int iArmorPiece );

	float MaxYawSpeed( void );

	Class_T Classify( void ) { return CLASS_COMBINE; }

	Activity NPC_TranslateActivity( Activity baseAct );

	virtual int	SelectSchedule( void );

	void BuildScheduleTestBits( void );

	DECLARE_DATADESC();

private:

	bool m_fOffBalance;
	float m_flNextClobberTime;

	int	GetReferencePointForBodyGroup( int bodyGroup );

	void InitArmorPieces( void );
	void DamageArmorPiece( int pieceID, float damage, const Vector &vecOrigin, const Vector &vecDir );
	void DestroyArmorPiece( int pieceID );
	void Shove( void );
	void FireRangeWeapon( void );

	float GetLegDamage( void );

	bool AimGunAt( CBaseEntity *pEntity, float flInterval );

	CSprite *m_pGlowSprite[NUM_CGUARD_ATTACHMENTS];

	armorPiece_t m_armorPieces[NUM_CGUARD_BODYGROUPS];

	int	m_nGibModel;

	float m_flGlowTime;
	float m_flLastRangeTime;

	float m_aimYaw;
	float m_aimPitch;

	int	m_YawControl;
	int	m_PitchControl;
	int	m_MuzzleAttachment;

	DEFINE_CUSTOM_AI;
};

#define	CGUARD_DEFAULT_ARMOR_HEALTH	50

#define	COMBINEGUARD_MELEE1_RANGE	92
#define	COMBINEGUARD_MELEE1_CONE	0.5f

#define	COMBINEGUARD_RANGE1_RANGE	1024
#define	COMBINEGUARD_RANGE1_CONE	0.0f

#define	CGUARD_GLOW_TIME			0.5f

IMPLEMENT_SERVERCLASS_ST(CNPC_CombineGuard, DT_NPC_CombineGuard)
END_SEND_TABLE()

BEGIN_DATADESC_NO_BASE( armorPiece_t )

	DEFINE_FIELD( destroyed,	FIELD_BOOLEAN ),
	DEFINE_FIELD( health,		FIELD_INTEGER ),

END_DATADESC()

BEGIN_DATADESC( CNPC_CombineGuard )

	DEFINE_FIELD( m_fOffBalance, FIELD_BOOLEAN ),
	DEFINE_FIELD( m_flNextClobberTime, FIELD_TIME ),
	DEFINE_ARRAY( m_pGlowSprite, FIELD_CLASSPTR, NUM_CGUARD_ATTACHMENTS ),
	DEFINE_EMBEDDED_AUTO_ARRAY( m_armorPieces ),
	DEFINE_FIELD( m_nGibModel, FIELD_INTEGER ),
	DEFINE_FIELD( m_flGlowTime,	FIELD_TIME ),
	DEFINE_FIELD( m_flLastRangeTime, FIELD_TIME ),
	DEFINE_FIELD( m_aimYaw,	FIELD_FLOAT ),
	DEFINE_FIELD( m_aimPitch, FIELD_FLOAT ),
	DEFINE_FIELD( m_YawControl,	FIELD_INTEGER ),
	DEFINE_FIELD( m_PitchControl, FIELD_INTEGER ),
	DEFINE_FIELD( m_MuzzleAttachment, FIELD_INTEGER ),

END_DATADESC()

enum CombineGuardSchedules 
{	
	SCHED_CGUARD_RANGE_ATTACK1 = LAST_SHARED_SCHEDULE,
	SCHED_COMBINEGUARD_CLOBBERED,
	SCHED_COMBINEGUARD_TOPPLE,
	SCHED_COMBINEGUARD_HELPLESS,
};

enum CombineGuardTasks 
{	
	TASK_CGUARD_RANGE_ATTACK1 = LAST_SHARED_TASK,
	TASK_COMBINEGUARD_SET_BALANCE,
};

enum CombineGuardConditions
{	
	COND_COMBINEGUARD_CLOBBERED = LAST_SHARED_CONDITION,
};

int	ACT_CGUARD_IDLE_TO_ANGRY;
int ACT_COMBINEGUARD_CLOBBERED;
int ACT_COMBINEGUARD_TOPPLE;
int ACT_COMBINEGUARD_GETUP;
int ACT_COMBINEGUARD_HELPLESS;

#define	CGUARD_AE_SHOVE	11
#define	CGUARD_AE_FIRE 12
#define	CGUARD_AE_FIRE_START 13
#define	CGUARD_AE_GLOW 14
#define CGUARD_AE_LEFTFOOT 20
#define CGUARD_AE_RIGHTFOOT	21
#define CGUARD_AE_SHAKEIMPACT 22

CNPC_CombineGuard::CNPC_CombineGuard( void )
{
}

LINK_ENTITY_TO_CLASS( npc_combineguard, CNPC_CombineGuard );

void CNPC_CombineGuard::Precache( void )
{
	PrecacheModel( COMBINEGUARD_MODEL );
	PrecacheModel(COMBINEGUARD_GUN_MODEL);

	PrecacheModel( "sprites/blueflare1.vmt" ); //For some reason this appears between his feet.

	PrecacheScriptSound( "NPC_CombineGuard.FootstepLeft" );
	PrecacheScriptSound( "NPC_CombineGuard.FootstepRight" );
	PrecacheScriptSound( "NPC_CombineGuard.Fire" );
	PrecacheScriptSound( "NPC_CombineGuard.Charging" );

	BaseClass::Precache();
}

void CNPC_CombineGuard::InitArmorPieces( void )
{
	for ( int i = 1; i < NUM_CGUARD_BODYGROUPS; i++ )
	{
		SetBodygroup( i, true );

		m_armorPieces[i].health	= CGUARD_DEFAULT_ARMOR_HEALTH;


		if( IsArmorPiece( i ) )
		{
			m_armorPieces[i].destroyed = false;
		}
		else
		{
			m_armorPieces[i].destroyed = true;
		}
	}
}

void CNPC_CombineGuard::Spawn( void )
{
	Precache();

	SetModel( COMBINEGUARD_MODEL );

	SetHullType( HULL_WIDE_HUMAN );
	
	SetNavType( NAV_GROUND );
	m_NPCState = NPC_STATE_NONE;
	SetBloodColor( BLOOD_COLOR_RED );

	m_iHealth = m_iMaxHealth = sk_combineguard_health.GetFloat();
	m_flFieldOfView = 0.1f;
	
	SetSolid( SOLID_BBOX );
	AddSolidFlags( FSOLID_NOT_STANDABLE );
	SetMoveType( MOVETYPE_STEP );

	m_flGlowTime = gpGlobals -> curtime;
	m_flLastRangeTime = gpGlobals -> curtime;
	m_aimYaw = 0;
	m_aimPitch = 0;

	m_flNextClobberTime	= gpGlobals -> curtime;

	CapabilitiesClear();
	CapabilitiesAdd( bits_CAP_MOVE_GROUND | bits_CAP_INNATE_MELEE_ATTACK1 | bits_CAP_INNATE_RANGE_ATTACK1 );

	for ( int i = 1; i < NUM_CGUARD_ATTACHMENTS; i++ )
	{
		m_pGlowSprite[i] = CSprite::SpriteCreate( "sprites/blueflare1.vmt", GetAbsOrigin(), false );
		
		Assert( m_pGlowSprite[i] );

		if ( m_pGlowSprite[i] == NULL )
			return;

		m_pGlowSprite[i] -> TurnOff();
		m_pGlowSprite[i] -> SetTransparency( kRenderGlow, 16, 16, 16, 255, kRenderFxNoDissipation );
		m_pGlowSprite[i] -> SetScale( 1.0f );
		m_pGlowSprite[i] -> SetAttachment( this, i );
	}

	NPCInit();
	
	InitArmorPieces();

	m_YawControl = LookupPoseParameter( "aim_yaw" );
	m_PitchControl = LookupPoseParameter( "aim_pitch" );
	m_MuzzleAttachment = LookupAttachment( "muzzle" );

	m_fOffBalance = false;

	BaseClass::Spawn();
}

void CNPC_CombineGuard::PrescheduleThink( void )
{
	BaseClass::PrescheduleThink();
	
	if( HasCondition( COND_COMBINEGUARD_CLOBBERED ) )
	{
		Msg( "CLOBBERED!\n" );
	}

	for ( int i = 1; i < NUM_CGUARD_ATTACHMENTS; i++ )
	{
		if ( m_pGlowSprite[i] == NULL )
			continue;

		if ( m_flGlowTime > gpGlobals->curtime )
		{
			float brightness;
			float perc = ( m_flGlowTime - gpGlobals->curtime ) / CGUARD_GLOW_TIME;

			m_pGlowSprite[i] -> TurnOn();

			brightness = perc * 92.0f;
			m_pGlowSprite[i] -> SetTransparency( kRenderGlow, brightness, brightness, brightness, 255, kRenderFxNoDissipation );
			
			m_pGlowSprite[i] -> SetScale( perc * 1.0f );
		}
		else
		{
			m_pGlowSprite[i] -> TurnOff();
		}
	}

	Vector vecDamagePoint;
	QAngle vecDamageAngles;

	for ( int i = 1; i < NUM_CGUARD_BODYGROUPS; i++ )
	{
		if ( m_armorPieces[i].destroyed )
		{
			int	referencePoint = GetReferencePointForBodyGroup( i );

			if ( referencePoint == CGUARD_REF_INVALID )
				continue;

			GetAttachment( referencePoint, vecDamagePoint, vecDamageAngles );

			if ( random->RandomInt( 0, 4 ) == 0 )
			{
				if ( random->RandomInt( 0, 800 ) == 0 )
				{
			//		ExplosionCreate( vecDamagePoint, vecDamageAngles, this, 0.5f, 0, false );// Doesn't work for some reason.
				}
				else
				{
				}
			}
		}
	}
}

void CNPC_CombineGuard::Event_Killed( const CTakeDamageInfo &info )
{
	SetBodygroup(1, 0);
	DropGun(25, Vector(0, 0, 1));

	BaseClass::Event_Killed(info);
}

void CNPC_CombineGuard::DropGun(int iVelocity, const Vector &vecVelocity)
{
	CPhysicsProp *pGib = assert_cast<CPhysicsProp*>(CreateEntityByName("prop_physics"));
	pGib->SetModel(COMBINEGUARD_GUN_MODEL);
	pGib->SetAbsOrigin(EyePosition());
	pGib->SetAbsAngles(EyeAngles());
	pGib->SetMoveType(MOVETYPE_VPHYSICS);
	pGib->SetCollisionGroup(COLLISION_GROUP_INTERACTIVE_DEBRIS);
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

void CNPC_CombineGuard::HandleAnimEvent( animevent_t *pEvent )
{
	switch ( pEvent -> event )
	{
	case CGUARD_AE_SHAKEIMPACT:
		Shove();
		UTIL_ScreenShake( GetAbsOrigin(), 25.0, 150.0, 1.0, 750, SHAKE_START );
		break;

	case CGUARD_AE_LEFTFOOT:
		{
			EmitSound( "NPC_CombineGuard.FootstepLeft" );
		}
		break;

	case CGUARD_AE_RIGHTFOOT:
		{
			EmitSound( "NPC_CombineGuard.FootstepRight" );
		}
		break;
	
	case CGUARD_AE_SHOVE:
		Shove();
		break;

	case CGUARD_AE_FIRE:
		{
			m_flLastRangeTime = gpGlobals->curtime + 1.5f;
			FireRangeWeapon();
			
			EmitSound( "NPC_CombineGuard.Fire" );

			EntityMessageBegin( this, true );
				WRITE_BYTE( CGUARD_MSG_SHOT );
			MessageEnd();
		}
		break;

	case CGUARD_AE_FIRE_START:
		{
			EmitSound( "NPC_CombineGuard.Charging" );

			EntityMessageBegin( this, true );
				WRITE_BYTE( CGUARD_MSG_SHOT_START );
			MessageEnd();
		}
		break;

	case CGUARD_AE_GLOW:
		m_flGlowTime = gpGlobals->curtime + CGUARD_GLOW_TIME;
		break;

	default:
		BaseClass::HandleAnimEvent( pEvent );
		return;
	}
}

void CNPC_CombineGuard::Shove( void ) // Doesn't work for some reason // It sure works now! -Dan
{
	if ( GetEnemy() == NULL )
		return;

	CBaseEntity *pHurt = NULL;

	Vector forward;
	AngleVectors( GetLocalAngles(), &forward );

	float flDist = ( GetEnemy()->GetAbsOrigin() - GetAbsOrigin() ).Length();
	Vector2D v2LOS = ( GetEnemy()->GetAbsOrigin() - GetAbsOrigin() ).AsVector2D();
	Vector2DNormalize( v2LOS );
	float flDot	= DotProduct2D ( v2LOS , forward.AsVector2D() );

	flDist -= GetEnemy() -> WorldAlignSize().x * 0.5f;

	if ( flDist < COMBINEGUARD_MELEE1_RANGE && flDot >= COMBINEGUARD_MELEE1_CONE )
	{
		Vector vStart = GetAbsOrigin();
		vStart.z += WorldAlignSize().z * 0.5;

		Vector vEnd = GetEnemy() -> GetAbsOrigin();
		vEnd.z += GetEnemy() -> WorldAlignSize().z * 0.5;

		pHurt = CheckTraceHullAttack( vStart, vEnd, Vector( -16, -16, 0 ), Vector( 16, 16, 24 ), 25, DMG_CLUB );
	}

	if ( pHurt )
	{
		pHurt -> ViewPunch( QAngle( -20, 0, 20 ) );

		UTIL_ScreenShake( pHurt -> GetAbsOrigin(), 100.0, 1.5, 1.0, 2, SHAKE_START );
		
		color32 white = { 255, 255, 255, 64 };
		UTIL_ScreenFade( pHurt, white, 0.5f, 0.1f, FFADE_IN );

		if ( pHurt->IsPlayer() )
		{
			Vector forward, up;
			AngleVectors( GetLocalAngles(), &forward, NULL, &up );
			pHurt->ApplyAbsVelocityImpulse( forward * 300 + up * 250 );
		}	
	}
}

int CNPC_CombineGuard::SelectSchedule( void )
{
	if (m_NPCState == NPC_STATE_IDLE || m_NPCState == NPC_STATE_ALERT)
	{
		return SCHED_PATROL_WALK_LOOP;
	}
	
	if( HasCondition( COND_COMBINEGUARD_CLOBBERED ) )
	{
		ClearCondition( COND_COMBINEGUARD_CLOBBERED );

		if( m_fOffBalance )
		{
			int iArmorPiece;
			do 
			{
				iArmorPiece = random->RandomInt( CGUARD_BGROUP_RIGHT_SHOULDER, CGUARD_BGROUP_LEFT_SHIN );
			}
			while( m_armorPieces[ iArmorPiece ].destroyed  );

			DestroyArmorPiece( iArmorPiece );

			DevMsg( "DESTROYING PIECE:%d\n", iArmorPiece );

			if( AllArmorDestroyed() )
			{
				DevMsg( " NO!!!!!!!! I'M DEADZ0R!!\n" );
				return SCHED_COMBINEGUARD_HELPLESS;
			//	ExplosionCreate(GetAbsOrigin(), GetAbsAngles(), NULL, random->RandomInt(30, 40), 0, true);
				UTIL_Remove(this);
				 //When I put the guard in his "helpless" state ingame by throwing props at him, he sat there in his melee animation and would not attack me. I guess the Combine Guard
				//Had a "lying on the ground dead" animation at one point.
			}
			else
			{
				return SCHED_COMBINEGUARD_TOPPLE;
			}
		}
		else
		{
			return SCHED_COMBINEGUARD_CLOBBERED;
		}
	}

	return BaseClass::SelectSchedule();
}

void CNPC_CombineGuard::StartTask( const Task_t *pTask )
{
	switch ( pTask->iTask )
	{
	case TASK_COMBINEGUARD_SET_BALANCE:
		if( pTask->flTaskData == 0.0 )
		{
			m_fOffBalance = false;
		}
		else
		{
			m_fOffBalance = true;
		}

		TaskComplete();

		break;

	case TASK_CGUARD_RANGE_ATTACK1:
		{
			Vector flEnemyLKP = GetEnemyLKP();
			GetMotor()->SetIdealYawToTarget( flEnemyLKP );
		}
		SetActivity( ACT_RANGE_ATTACK1 );
		return;

	default:
		BaseClass::StartTask( pTask );
		break;
	}
}

void CNPC_CombineGuard::RunTask( const Task_t *pTask )
{
	switch ( pTask->iTask )
	{
	case TASK_CGUARD_RANGE_ATTACK1:
		{
			if (GetEnemy() != NULL)
			{
				AimGunAt(GetEnemy(), 0.1f);
			}

			if ( IsActivityFinished() )
			{
				TaskComplete();
				return;
			}
		}
		return;
	}

	BaseClass::RunTask( pTask );
}

float CNPC_CombineGuard::GetLegDamage( void )
{
	float damageTotal = 0.0f;

	if ( m_armorPieces[CGUARD_BGROUP_RIGHT_THIGH].destroyed )
	{
		damageTotal += 0.25f;
	}

	if ( m_armorPieces[CGUARD_BGROUP_LEFT_THIGH].destroyed )
	{
		damageTotal += 0.25f;
	}

	if ( m_armorPieces[CGUARD_BGROUP_RIGHT_SHIN].destroyed )
	{
		damageTotal += 0.25f;
	}

	if ( m_armorPieces[CGUARD_BGROUP_LEFT_SHIN].destroyed )
	{
		damageTotal += 0.25f;
	}

	return damageTotal;
}

#define TRANSLATE_SCHEDULE( type, in, out ) { if ( type == in ) return out; }

Activity CNPC_CombineGuard::NPC_TranslateActivity( Activity baseAct )
{
	if ( baseAct == ACT_RUN )
	{
		float legDamage = GetLegDamage();

		if ( legDamage > 0.75f )
		{
			return ( Activity ) ACT_WALK;
		}
		else if ( legDamage > 0.5f )
		{
			return ( Activity ) ACT_WALK;
		}
	}

	if( baseAct == ACT_RUN )
	{
		return ( Activity )ACT_WALK;
	}
	
	if ( baseAct == ACT_IDLE && m_NPCState != NPC_STATE_IDLE )
	{
		return ( Activity ) ACT_IDLE_ANGRY;
	}

	return baseAct;
}

int CNPC_CombineGuard::TranslateSchedule( int type ) 
{
	switch( type )
	{
	case SCHED_RANGE_ATTACK1:
		return SCHED_CGUARD_RANGE_ATTACK1;
		break;
	}

	return BaseClass::TranslateSchedule( type );
}

int CNPC_CombineGuard::GetReferencePointForBodyGroup( int bodyGroup )
{
	switch ( bodyGroup )
	{
	case CGUARD_BGROUP_MAIN:
	case CGUARD_BGROUP_GUN:
		return CGUARD_REF_INVALID;
		break;

	case CGUARD_BGROUP_RIGHT_SHOULDER:
		return CGUARD_REF_RIGHT_SHOULDER;
		break;

	case CGUARD_BGROUP_LEFT_SHOULDER:
		return CGUARD_REF_LEFT_SHOULDER;
		break;

	case CGUARD_BGROUP_HEAD:
		return CGUARD_REF_HUMAN_HEAD;
		break;

	case CGUARD_BGROUP_RIGHT_ARM:
		if ( random->RandomInt( 0, 1 ) )
			return CGUARD_REF_RIGHT_ARM_LOW;
		else
			return CGUARD_REF_RIGHT_ARM_HIGH;
		
		break;

	case CGUARD_BGROUP_LEFT_ARM:
		if ( random->RandomInt( 0, 1 ) )
			return CGUARD_REF_LEFT_ARM_LOW;
		else
			return CGUARD_REF_LEFT_ARM_HIGH;

		break;

	case CGUARD_BGROUP_TORSO:
		return CGUARD_REF_TORSO;
		break;

	case CGUARD_BGROUP_RIGHT_THIGH:
		if ( random->RandomInt( 0, 1 ) )
			return CGUARD_REF_RIGHT_THIGH_LOW;
		else
			return CGUARD_REF_RIGHT_THIGH_HIGH;

		break;

	case CGUARD_BGROUP_LEFT_THIGH:
		if ( random->RandomInt( 0, 1 ) )
			return CGUARD_REF_LEFT_THIGH_LOW;
		else
			return CGUARD_REF_LEFT_THIGH_HIGH;

		break;

	case CGUARD_BGROUP_RIGHT_SHIN:
		if ( random->RandomInt( 0, 1 ) )
			return CGUARD_REF_RIGHT_SHIN_LOW;
		else
			return CGUARD_REF_RIGHT_SHIN_HIGH;
		break;

	case CGUARD_BGROUP_LEFT_SHIN:
		if ( random->RandomInt( 0, 1 ) )
			return CGUARD_REF_LEFT_SHIN_LOW;
		else
			return CGUARD_REF_LEFT_SHIN_HIGH;
		break;

	case CGUARD_BGROUP_LOWER_TORSO:
		return CGUARD_REF_LOWER_TORSO;
		break;
	}

	return CGUARD_REF_INVALID;
}

void CNPC_CombineGuard::DestroyArmorPiece( int pieceID )
{
	int	refPoint = GetReferencePointForBodyGroup( pieceID );

	if ( refPoint == CGUARD_REF_INVALID )
		return;

	Vector vecDamagePoint;
	QAngle vecDamageAngles;

	GetAttachment( refPoint, vecDamagePoint, vecDamageAngles );

	Vector vecVelocity, vecSize;

	vecVelocity.Random( -1.0, 1.0 );
	vecVelocity *= random -> RandomInt( 16, 64 );

	vecSize = Vector( 32, 32, 32 );

	Vector gibVelocity = RandomVector( -100, 100 ) * 10;

	CPVSFilter filter( GetAbsOrigin() );
	te->BreakModel( filter, 0.0, GetAbsOrigin(), vec3_angle, Vector( 40, 40, 40 ), gibVelocity, m_nGibModel, 100, 0, 2.5, BREAK_METAL );
	
	m_armorPieces[pieceID].destroyed = true;
	SetBodygroup( pieceID, false );

	SetCondition( COND_LIGHT_DAMAGE );
}

void CNPC_CombineGuard::DamageArmorPiece( int pieceID, float damage, const Vector &vecOrigin, const Vector &vecDir )
{
	if ( m_armorPieces[pieceID].destroyed )
	{
		if ( ( random->RandomInt( 0, 8 ) == 0 ) && ( pieceID != CGUARD_BGROUP_HEAD ) )
		{
			g_pEffects->Ricochet( vecOrigin, ( vecDir * -1.0f ) );
		}
		return;
	}

	m_armorPieces[ pieceID ].health -= damage;

	if ( m_armorPieces[ pieceID ].health <= 0.0f )
	{
		DestroyArmorPiece( pieceID );
		return;
	}

	g_pEffects->Sparks( vecOrigin );
}

void CNPC_CombineGuard::TraceAttack( CBaseEntity *pAttacker, float flDamage, const Vector &vecDir, trace_t *ptr, int bitsDamageType )
{
	Vector vecDamagePoint = ptr -> endpos;

	if ( bitsDamageType & ( DMG_BLAST ) )
	{
		Vector vecOrigin;
		QAngle vecAngles;
		float flNearestDist = 99999999.0f;
		float flDist;
		int	nReferencePoint;
		int	nNearestGroup = CGUARD_BGROUP_MAIN;
		float flAdjustedDamage = flDamage;

		for ( int i = 1; i < NUM_CGUARD_BODYGROUPS; i++ )
		{
			if ( m_armorPieces[ i ].destroyed )
			{
				flAdjustedDamage *= 0.9f;
				continue;
			}

			nReferencePoint = GetReferencePointForBodyGroup( i );

			if ( nReferencePoint == CGUARD_REF_INVALID )
				continue;

			GetAttachment( nReferencePoint, vecOrigin, vecAngles );

			flDist = ( vecOrigin - ptr -> endpos ).Length();
			
			if ( flDist < flNearestDist )
			{
				flNearestDist = flDist;
				nNearestGroup = i;
			}
		}

		if ( nNearestGroup != CGUARD_BGROUP_MAIN )
		{
			DamageArmorPiece( nNearestGroup, flAdjustedDamage, vecDamagePoint, vecDir );
			return;
		}
	}
	
	if ( ptr->hitgroup != CGUARD_BGROUP_MAIN )
	{
		DamageArmorPiece( ptr -> hitgroup, flDamage, vecDamagePoint, vecDir );
	}
	else
	{
	}
}


int CNPC_CombineGuard::OnTakeDamage_Alive( const CTakeDamageInfo &info )
{
	CTakeDamageInfo newInfo = info;
	if( newInfo.GetDamageType() & DMG_BURN)
	{
		newInfo.ScaleDamage( 0 );
		DevMsg( "Fire damage; no actual damage is taken\n" );
	}	

	int nDamageTaken = BaseClass::OnTakeDamage_Alive( newInfo );

//	m_bHeadshot = false;
//	m_bCanisterShot = false;

	return nDamageTaken;
}


int CNPC_CombineGuard::MeleeAttack1Conditions( float flDot, float flDist )
{
	if ( flDist > COMBINEGUARD_MELEE1_RANGE )
		return COND_TOO_FAR_TO_ATTACK;
	
	if ( flDot < COMBINEGUARD_MELEE1_CONE )
		return COND_NOT_FACING_ATTACK;

	return COND_CAN_MELEE_ATTACK1;
}

int CNPC_CombineGuard::RangeAttack1Conditions( float flDot, float flDist )
{
	if ( flDist > COMBINEGUARD_RANGE1_RANGE )
		return COND_TOO_FAR_TO_ATTACK;
	
	if ( flDot < COMBINEGUARD_RANGE1_CONE )
		return COND_NOT_FACING_ATTACK;

	if ( m_flLastRangeTime > gpGlobals->curtime )
		return COND_TOO_FAR_TO_ATTACK;

	return COND_CAN_RANGE_ATTACK1;
}

void CNPC_CombineGuard::FireRangeWeapon( void )
{
	if ( ( GetEnemy() != NULL ) && ( GetEnemy()->Classify() == CLASS_BULLSEYE ) )
	{
		GetEnemy()->TakeDamage( CTakeDamageInfo( this, this, 1.0f, DMG_GENERIC ) );
	}

	Vector vecSrc, vecAiming;
	Vector vecShootOrigin;

	GetVectors( &vecAiming, NULL, NULL );
	vecSrc = WorldSpaceCenter() + vecAiming * 64;
	
	Vector	impactPoint	= vecSrc + ( vecAiming * MAX_TRACE_LENGTH );

	vecShootOrigin = GetAbsOrigin() + Vector( 0, 0, 55 );
	Vector vecShootDir = GetShootEnemyDir( vecShootOrigin );
	QAngle angDir;
	
	QAngle	angShootDir;
	GetAttachment( LookupAttachment( "muzzle" ), vecShootOrigin, angShootDir );

	trace_t	tr;
	AI_TraceLine( vecSrc, impactPoint, MASK_SHOT, this, COLLISION_GROUP_NONE, &tr );
// Just using the gunship tracers for a placeholder unless a better effect can be found. Maybe use the strider cannon's tracer or something.
	UTIL_Tracer( tr.startpos, tr.endpos, 0, TRACER_DONT_USE_ATTACHMENT, 6000 + random->RandomFloat( 0, 2000 ), true, "StriderTracer" );
	UTIL_Tracer( tr.startpos, tr.endpos, 0, TRACER_DONT_USE_ATTACHMENT, 6000 + random->RandomFloat( 0, 3000 ), true, "StriderTracer" );
	UTIL_Tracer( tr.startpos, tr.endpos, 0, TRACER_DONT_USE_ATTACHMENT, 6000 + random->RandomFloat( 0, 4000 ), true, "StriderTracer" );

	CreateConcussiveBlast( tr.endpos, tr.plane.normal, this, 1.0 );
}

#define	DEBUG_AIMING 0

bool CNPC_CombineGuard::AimGunAt( CBaseEntity *pEntity, float flInterval )
{
	if ( !pEntity )
		return true;

	matrix3x4_t gunMatrix;
	GetAttachment( m_MuzzleAttachment, gunMatrix );

	Vector localEnemyPosition;
	VectorITransform( pEntity->GetAbsOrigin(), gunMatrix, localEnemyPosition );
	
	QAngle localEnemyAngles;
	VectorAngles( localEnemyPosition, localEnemyAngles );
	
	localEnemyAngles.x = UTIL_AngleDiff( localEnemyAngles.x, 0 );	
	localEnemyAngles.y = UTIL_AngleDiff( localEnemyAngles.y, 0 );

	float targetYaw = m_aimYaw + localEnemyAngles.y;
	float targetPitch = m_aimPitch + localEnemyAngles.x;
	
	QAngle unitAngles = localEnemyAngles;
	float angleDiff = sqrt( localEnemyAngles.y * localEnemyAngles.y + localEnemyAngles.x * localEnemyAngles.x );
	const float aimSpeed = 1;

	float yawSpeed = fabsf(aimSpeed*flInterval*localEnemyAngles.y);
	float pitchSpeed = fabsf(aimSpeed*flInterval*localEnemyAngles.x);

	yawSpeed = max(yawSpeed,15);
	pitchSpeed = max(pitchSpeed,15);

	m_aimYaw = UTIL_Approach( targetYaw, m_aimYaw, yawSpeed );
	m_aimPitch = UTIL_Approach( targetPitch, m_aimPitch, pitchSpeed );

	SetPoseParameter( m_YawControl, m_aimYaw );
	SetPoseParameter( m_PitchControl, m_aimPitch );

	m_aimPitch = GetPoseParameter( m_PitchControl );
	m_aimYaw = GetPoseParameter( m_YawControl );

	if ( angleDiff < 1 )
		return true;

	return false;
}

float CNPC_CombineGuard::MaxYawSpeed( void ) 
{ 	
	if ( GetActivity() == ACT_RANGE_ATTACK1 )
	{
		return 1.0f;
	}

	return 60.0f;
}

void CNPC_CombineGuard::BuildScheduleTestBits( void )
{
	SetCustomInterruptCondition( COND_COMBINEGUARD_CLOBBERED );
}

bool CNPC_CombineGuard::IsArmorPiece( int iArmorPiece )
{
	switch( iArmorPiece )
	{
		case CGUARD_BGROUP_MAIN:
		case CGUARD_BGROUP_GUN:
		case CGUARD_BGROUP_HEAD:
		case CGUARD_BGROUP_RIGHT_ARM:
		case CGUARD_BGROUP_LEFT_ARM:
		case CGUARD_BGROUP_TORSO:
		case CGUARD_BGROUP_LOWER_TORSO:
		case CGUARD_BGROUP_RIGHT_SHIN:
		case CGUARD_BGROUP_LEFT_SHIN:
			return false;
			break;

		default:
			return true;
			break;
	}
}

bool CNPC_CombineGuard::AllArmorDestroyed( void )
{
	for( int i = CGUARD_BGROUP_RIGHT_SHOULDER ; i <= CGUARD_BGROUP_LEFT_SHIN ; i++ )
	{
		if( !m_armorPieces[ i ].destroyed )
		{
			return false;
		}
	}

	return true;
}

AI_BEGIN_CUSTOM_NPC( npc_combineguard, CNPC_CombineGuard )

	DECLARE_TASK( TASK_CGUARD_RANGE_ATTACK1 )
	DECLARE_TASK( TASK_COMBINEGUARD_SET_BALANCE )
	
	DECLARE_CONDITION( COND_COMBINEGUARD_CLOBBERED )

	DECLARE_ACTIVITY( ACT_CGUARD_IDLE_TO_ANGRY )
	DECLARE_ACTIVITY( ACT_COMBINEGUARD_CLOBBERED )
	DECLARE_ACTIVITY( ACT_COMBINEGUARD_TOPPLE )
	DECLARE_ACTIVITY( ACT_COMBINEGUARD_GETUP )
	DECLARE_ACTIVITY( ACT_COMBINEGUARD_HELPLESS )

	DEFINE_SCHEDULE
	(
		SCHED_CGUARD_RANGE_ATTACK1,

		"	Tasks"
		"		TASK_STOP_MOVING			0"
		"		TASK_FACE_ENEMY				0"
		"		TASK_ANNOUNCE_ATTACK		1"
		"		TASK_CGUARD_RANGE_ATTACK1	0"
		"	"
		"	Interrupts"
		"		COND_NEW_ENEMY"
		"		COND_ENEMY_DEAD"
		"		COND_NO_PRIMARY_AMMO"
	)

	DEFINE_SCHEDULE
	(
		SCHED_COMBINEGUARD_CLOBBERED,

		"	Tasks"
		"		TASK_STOP_MOVING						0"
		"		TASK_COMBINEGUARD_SET_BALANCE			1"
		"		TASK_PLAY_SEQUENCE						ACTIVITY:ACT_COMBINEGUARD_CLOBBERED"
		"		TASK_COMBINEGUARD_SET_BALANCE			0"
		"	"
		"	Interrupts"
	)

	DEFINE_SCHEDULE
	(
		SCHED_COMBINEGUARD_TOPPLE,

		"	Tasks"
		"		TASK_STOP_MOVING				0"
		"		TASK_PLAY_SEQUENCE				ACTIVITY:ACT_COMBINEGUARD_TOPPLE"
		"		TASK_WAIT						1"
		"		TASK_PLAY_SEQUENCE				ACTIVITY:ACT_COMBINEGUARD_GETUP"
		"		TASK_COMBINEGUARD_SET_BALANCE	0"
		"	"
		"	Interrupts"
	)

	DEFINE_SCHEDULE
	(
		SCHED_COMBINEGUARD_HELPLESS,

		"	Tasks"
		"	TASK_STOP_MOVING				0"
		"	TASK_PLAY_SEQUENCE				ACTIVITY:ACT_COMBINEGUARD_TOPPLE"
		"	TASK_WAIT						2"
		"	TASK_PLAY_SEQUENCE				ACTIVITY:ACT_COMBINEGUARD_HELPLESS"
		"	TASK_WAIT_INDEFINITE			0"
		"	"
		"	Interrupts"
	)

AI_END_CUSTOM_NPC()



