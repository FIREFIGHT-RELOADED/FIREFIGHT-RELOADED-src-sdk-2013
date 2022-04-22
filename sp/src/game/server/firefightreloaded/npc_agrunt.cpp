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
//=========================================================
// Agrunt - Dominant, warlike alien grunt monster
//=========================================================

#include	"cbase.h"
#include	"ai_default.h"
#include	"ai_task.h"
#include	"ai_schedule.h"
#include	"ai_node.h"
#include	"ai_hull.h"
#include	"ai_hint.h"
#include	"ai_navigator.h"
#include	"ai_route.h"
#include	"ai_squad.h"
#include	"ai_squadslot.h"
#include	"ai_memory.h"
#include	"ai_senses.h"
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
#include    "te.h"
#include 	"ai_basenpc.h"
#include	"npc_agrunt.h"

int iHornetTrail;
int iHornetPuff;

ConVar sk_agrunt_health( "sk_agrunt_health", "0" );
ConVar sk_agrunt_dmg_punch( "sk_agrunt_dmg_punch", "0" );
ConVar sk_npc_dmg_hornet("sk_npc_dmg_hornet", "0", FCVAR_REPLICATED);

void TE_BeamFollow(IRecipientFilter& filter, float delay,
	int iEntIndex, int modelIndex, int haloIndex, float life, float width, float endWidth,
	float fadeLength, float r, float g, float b, float a);

LINK_ENTITY_TO_CLASS( hornet, CHornet );

//=========================================================
// Save/Restore
//=========================================================
BEGIN_DATADESC( CHornet )
	DEFINE_FIELD( m_flStopAttack, FIELD_TIME ),
	DEFINE_FIELD( m_iHornetType, FIELD_INTEGER ),
	DEFINE_FIELD( m_flFlySpeed, FIELD_FLOAT ),
	
	DEFINE_ENTITYFUNC( DieTouch ),
	DEFINE_THINKFUNC( StartDart ),
	DEFINE_THINKFUNC( StartTrack ),
	DEFINE_ENTITYFUNC( DartTouch ),
	DEFINE_ENTITYFUNC( TrackTouch ),
	DEFINE_THINKFUNC( TrackTarget ),
END_DATADESC()

//=========================================================
//=========================================================
void CHornet::Spawn( void )
{
	Precache();

	SetMoveType( MOVETYPE_FLY );
	SetSolid( SOLID_BBOX );
	m_takedamage = DAMAGE_YES;
	AddFlag( FL_NPC );
	SetHealth(1);// weak!
	SetBloodColor(DONT_BLEED);
	
	// hornets don't live as long in FR, period.
	m_flStopAttack = gpGlobals->curtime + 3.5;

	m_flFieldOfView = 0.9; // +- 25 degrees

	if ( random->RandomInt( 1, 5 ) <= 2 )
	{
		m_iHornetType = HORNET_TYPE_RED;
		m_flFlySpeed = HORNET_RED_SPEED;
	}
	else
	{
		m_iHornetType = HORNET_TYPE_ORANGE;
		m_flFlySpeed = HORNET_ORANGE_SPEED;
	}

	SetModel( "models/hornet.mdl" );
	UTIL_SetSize( this, Vector( -4, -4, -4 ), Vector( 4, 4, 4 ) );

	SetTouch( &CHornet::DieTouch );
	SetThink( &CHornet::StartTrack );
	
	SetNextThink( gpGlobals->curtime + 0.1f );
	ResetSequenceInfo();
}

void CHornet::Precache()
{
	PrecacheModel("models/hornet.mdl");

	PrecacheScriptSound( "Hornet.Die" );
	PrecacheScriptSound( "Hornet.Buzz" );

	iHornetPuff = PrecacheModel( "sprites/muz1.vmt" );
	iHornetTrail = PrecacheModel("sprites/laserbeam.vmt");
}	

//=========================================================
// hornets will never get mad at each other, no matter who the owner is.
//=========================================================
Disposition_t CHornet::IRelationType( CBaseEntity *pTarget )
{
	if (FClassnameIs(pTarget, GetClassname()))
	{
		return D_NU;
	}

	return BaseClass::IRelationType( pTarget );
}

//=========================================================
// ID's Hornet as their owner
//=========================================================
Class_T	CHornet::Classify ( void )
{
	return CLASS_ALIEN_BIOWEAPON;
}

//=========================================================
// StartTrack - starts a hornet out tracking its target
//=========================================================
void CHornet :: StartTrack ( void )
{
	IgniteTrail();

	SetTouch( &CHornet::TrackTouch );
	SetThink( &CHornet::TrackTarget );

	SetNextThink( gpGlobals->curtime + 0.1f );
}

void CHornet::Event_Killed(const CTakeDamageInfo &info)
{
	EmitSound("Hornet.Die");

	m_takedamage = DAMAGE_NO;

	BaseClass::Event_Killed(info);
}

//=========================================================
// StartDart - starts a hornet out just flying straight.
//=========================================================
void CHornet :: StartDart ( void )
{
	IgniteTrail();

	SetTouch( &CHornet::DartTouch );

	SetThink( &CHornet::SUB_Remove );
	SetNextThink( gpGlobals->curtime + 4 );
}

void CHornet::IgniteTrail( void )
{
/*

  ted's suggested trail colors:

r161
g25
b97

r173
g39
b14

old colors
		case HORNET_TYPE_RED:
			WRITE_BYTE( 255 );   // r, g, b
			WRITE_BYTE( 128 );   // r, g, b
			WRITE_BYTE( 0 );   // r, g, b
			break;
		case HORNET_TYPE_ORANGE:
			WRITE_BYTE( 0   );   // r, g, b
			WRITE_BYTE( 100 );   // r, g, b
			WRITE_BYTE( 255 );   // r, g, b
			break;
	
*/

	// trail
	Vector vColor;

	switch (m_iHornetType)
	{
		case HORNET_TYPE_RED:
			vColor = Vector ( 179, 39, 14 );
			break;
		default:
		case HORNET_TYPE_ORANGE:
			vColor = Vector ( 255, 128, 0 );
			break;
	}

	CBroadcastRecipientFilter filter;
	TE_BeamFollow( filter, 0.0,
		entindex(),
		iHornetTrail,
		0,
		1,
		2,
		0.5,
		0.5,
		vColor.x,
		vColor.y,
		vColor.z,
		128 );
}

//=========================================================
// Hornet is flying, gently tracking target
//=========================================================
void CHornet::TrackTarget(void)
{
	Vector	vecFlightDir;
	Vector	vecDirToEnemy;
	float	flDelta;

	StudioFrameAdvance();

	if (gpGlobals->curtime > m_flStopAttack)
	{
		SetTouch(NULL);
		SetThink(&CBaseEntity::SUB_Remove);
		SetNextThink(gpGlobals->curtime + 0.1f);
		return;
	}

	// UNDONE: The player pointer should come back after returning from another level
	if (GetEnemy() == NULL)
	{// enemy is dead.
		GetSenses()->Look(512);
		SetEnemy(BestEnemy());
	}

	Vector m_vecEnemyLKP = vec3_origin;

	if (GetEnemy() != NULL && FVisible(GetEnemy()))
	{
		m_vecEnemyLKP = GetEnemy()->BodyTarget(GetAbsOrigin());
	}
	else
	{
		m_vecEnemyLKP = m_vecEnemyLKP + GetAbsVelocity() * m_flFlySpeed * 0.1;
	}

	vecDirToEnemy = (m_vecEnemyLKP - GetAbsOrigin());
	VectorNormalize(vecDirToEnemy);

	if (GetAbsVelocity().Length() < 0.1)
	{
		vecFlightDir = vecDirToEnemy;
	}
	else
	{
		vecFlightDir = GetAbsVelocity();
		VectorNormalize(vecFlightDir);
	}

	SetAbsVelocity(vecFlightDir + vecDirToEnemy);

	// measure how far the turn is, the wider the turn, the slow we'll go this time.
	flDelta = DotProduct(vecFlightDir, vecDirToEnemy);

	if (flDelta < 0.5)
	{// hafta turn wide again. play sound
		EmitSound("Hornet.Buzz");
	}

	if (flDelta <= 0 && m_iHornetType == HORNET_TYPE_RED)
	{// no flying backwards, but we don't want to invert this, cause we'd go fast when we have to turn REAL far.
		flDelta = 0.25;
	}

	Vector vecVel = vecFlightDir + vecDirToEnemy;
	VectorNormalize(vecVel);

	if (GetOwnerEntity() && (GetOwnerEntity()->GetFlags() & FL_NPC))
	{
		// random pattern only applies to hornets fired by monsters, not players. 

		vecVel.x += random->RandomFloat(-0.10, 0.10);// scramble the flight dir a bit.
		vecVel.y += random->RandomFloat(-0.10, 0.10);
		vecVel.z += random->RandomFloat(-0.10, 0.10);
	}

	switch (m_iHornetType)
	{
	case HORNET_TYPE_RED:
		SetAbsVelocity(vecVel * (m_flFlySpeed * flDelta));// scale the dir by the ( speed * width of turn )
		SetNextThink(gpGlobals->curtime + random->RandomFloat(0.1, 0.3));
		break;
	case HORNET_TYPE_ORANGE:
	default:
		SetAbsVelocity(vecVel * m_flFlySpeed);// do not have to slow down to turn.
		SetNextThink(gpGlobals->curtime + 0.1);// fixed think time
		break;
	}

	QAngle angNewAngles;
	VectorAngles(GetAbsVelocity(), angNewAngles);
	SetAbsAngles(angNewAngles);

	SetSolid(SOLID_BBOX);

	// if hornet is close to the enemy, jet in a straight line for a half second.
	// (only in the single player game)
	if (GetEnemy() != NULL)
	{
		if (flDelta >= 0.4 && (GetAbsOrigin() - m_vecEnemyLKP).Length() <= 300)
		{
			CPVSFilter filter(GetAbsOrigin());
			te->Sprite(filter, 0.0,
				&GetAbsOrigin(), // pos
				iHornetPuff,	// model
				0.2,				//size
				128				// brightness
			);

			EmitSound("Hornet.Buzz");
			SetAbsVelocity(GetAbsVelocity() * 2);
			SetNextThink(gpGlobals->curtime + 1.0f);
			// don't attack again
			m_flStopAttack = gpGlobals->curtime;
		}
	}
}

unsigned int CHornet::PhysicsSolidMaskForEntity() const
{
	return (BaseClass::PhysicsSolidMaskForEntity() | CONTENTS_HITBOX) 
		& ~CONTENTS_MONSTERCLIP
		& ~CONTENTS_GRATE;
}

//=========================================================
// Tracking Hornet hit something
//=========================================================
void CHornet::TrackTouch ( CBaseEntity *pOther )
{
	if ( (pOther->IsSolid() || 
		!pOther->IsSolidFlagSet(FSOLID_VOLUME_CONTENTS)) && 
		(pOther != GetOwnerEntity() || 
		!FClassnameIs(pOther, GetClassname())))
	{
		int nRelationship = IRelationType( pOther );
		if ( nRelationship > D_HT )
		{
			// hit something we don't want to hurt, so turn around.
			
			Vector vecVel = GetAbsVelocity();

			VectorNormalize( vecVel );

			vecVel.x *= -1;
			vecVel.y *= -1;

			SetAbsOrigin( GetAbsOrigin() + vecVel * 4 ); // bounce the hornet off a bit.
			SetAbsVelocity( vecVel * m_flFlySpeed );

			return;
		}
	}

	DieTouch(pOther);
}

void CHornet::DartTouch( CBaseEntity *pOther )
{
	DieTouch( pOther );
}

void CHornet::DieTouch( CBaseEntity *pOther )
{
	if ( pOther && (pOther->IsSolid() || !pOther->IsSolidFlagSet(FSOLID_VOLUME_CONTENTS)))
	{
		CTakeDamageInfo info( this, GetOwnerEntity(), sk_npc_dmg_hornet.GetFloat(), DMG_BULLET );
		CalculateBulletDamageForce(&info, GetAmmoDef()->Index("SMG1"), GetAbsVelocity(), GetAbsOrigin());
		pOther->TakeDamage( info );
		Event_Killed(info);

		SetThink(&CHornet::SUB_Remove);
		SetNextThink(gpGlobals->curtime);
	}
}

int iAgruntMuzzleFlash;
int ACT_THREAT_DISPLAY;

class CAGrunt : public CAI_BaseNPC
{
	DECLARE_CLASS(CAGrunt, CAI_BaseNPC);
public:
	DECLARE_DATADESC();
	DEFINE_CUSTOM_AI;

	void Spawn( void );
	void Precache( void );
	float MaxYawSpeed(void);
	Class_T	Classify ( void );
	void HandleAnimEvent( animevent_t *pEvent );
	int GetSoundInterests( void );
	bool FCanCheckAttacks ( void );
	int MeleeAttack1Conditions ( float flDot, float flDist );
	int RangeAttack1Conditions ( float flDot, float flDist );
	void StartTask ( const Task_t *pTask );
	void AlertSound( void );
	void DeathSound (const CTakeDamageInfo &info);
	void PainSound (const CTakeDamageInfo &info);
	void AttackSound ( void );
	void PrescheduleThink ( void );
	void TraceAttack(const CTakeDamageInfo &info, const Vector &vecDir, trace_t *ptr);
	int	 IRelationPriority( CBaseEntity *pTarget );
	void StopTalking ( void );
	void PunchEnemy(bool right);
	bool ShouldSpeak( void );
	int TranslateSchedule( int iType );
	int SelectSchedule( void );

public: 
	bool	m_fCanHornetAttack;
	float	m_flNextHornetAttackCheck;

	float m_flNextPainTime;

	// three hacky fields for speech stuff. These don't really need to be saved.
	float	m_flNextSpeakTime;
	float	m_flNextWordTime;
	float	m_flLastDamaged;
};
LINK_ENTITY_TO_CLASS( npc_agrunt, CAGrunt );

BEGIN_DATADESC(CAGrunt)
	DEFINE_FIELD( m_fCanHornetAttack, FIELD_BOOLEAN ),
	DEFINE_FIELD( m_flNextHornetAttackCheck, FIELD_TIME ),
	DEFINE_FIELD( m_flNextPainTime, FIELD_TIME ),
	DEFINE_FIELD( m_flNextSpeakTime, FIELD_TIME ),
	DEFINE_FIELD( m_flNextWordTime, FIELD_TIME ),
	DEFINE_FIELD( m_flLastDamaged, FIELD_TIME ),
END_DATADESC()

//=========================================================
// IRelationPriority - overridden because Human Grunts are 
// Alien Grunt's nemesis.
// In FR, we use the Combine as the nemesis.
// The Combine ate the plumbuses.
//=========================================================
int	 CAGrunt::IRelationPriority(CBaseEntity* pTarget)
{
	if ( pTarget->Classify() == CLASS_COMBINE || pTarget->Classify() == CLASS_PLAYER )
	{
		return ( BaseClass::IRelationPriority ( pTarget ) + 1 );
	}

	return BaseClass::IRelationPriority (pTarget );
}

//=========================================================
// TraceAttack
//=========================================================
void CAGrunt::TraceAttack(const CTakeDamageInfo &info, const Vector &vecDir, trace_t *ptr)
{
	CTakeDamageInfo newinfo = info;
	
	float flDamage = newinfo.GetDamage();
	
	//hitgroup gear is 10. - Bitl
	if ( ptr->hitgroup == HITGROUP_GEAR && (newinfo.GetDamageType() & (DMG_BULLET | DMG_SLASH | DMG_CLUB)))
	{
		// hit armor
		if ( m_flLastDamaged != gpGlobals->curtime || (random->RandomInt(0,10) < 1) )
		{
			CPVSFilter filter( ptr->endpos );
			te->ArmorRicochet( filter, 0.0, &ptr->endpos, &ptr->plane.normal );
			m_flLastDamaged = gpGlobals->curtime;
		}

		if ( random->RandomInt( 0, 1 ) == 0 )
		{
			Vector vecTracerDir = vecDir;

			vecTracerDir.x += random->RandomFloat( -0.3, 0.3 );
			vecTracerDir.y += random->RandomFloat( -0.3, 0.3 );
			vecTracerDir.z += random->RandomFloat( -0.3, 0.3 );

			vecTracerDir = vecTracerDir * -512;

			Vector vEndPos = ptr->endpos + vecTracerDir;

			UTIL_Tracer( ptr->endpos, vEndPos, ENTINDEX( edict() ) );
		}

		flDamage -= 20;
		if (flDamage <= 0)
			flDamage = 0.1;// don't hurt the monster much, but allow bits_COND_LIGHT_DAMAGE to be generated
		
		newinfo.SetDamage(flDamage);
	}
	else
	{
		SpawnBlood(ptr->endpos, vecDir, BloodColor(), flDamage);// a little surface blood.
		TraceBleed( flDamage, vecDir, ptr, newinfo.GetDamageType() );
	}

	AddMultiDamage( newinfo, this );
}

//=========================================================
// StopTalking - won't speak again for 10-20 seconds.
//=========================================================
void CAGrunt::StopTalking( void )
{
	m_flNextWordTime = m_flNextSpeakTime = gpGlobals->curtime + 10 + random->RandomInt(0, 10);
}

//=========================================================
// ShouldSpeak - Should this agrunt be talking?
//=========================================================
bool CAGrunt::ShouldSpeak( void )
{
	if ( m_flNextSpeakTime > gpGlobals->curtime )
	{
		// my time to talk is still in the future.
		return false;
	}

	if ( m_spawnflags & SF_NPC_GAG )
	{
		if ( m_NPCState != NPC_STATE_COMBAT )
		{
			// if gagged, don't talk outside of combat.
			// if not going to talk because of this, put the talk time 
			// into the future a bit, so we don't talk immediately after 
			// going into combat
			m_flNextSpeakTime = gpGlobals->curtime + 3.0f;
			return false;
		}
	}

	return true;
}

//=========================================================
// PrescheduleThink 
//=========================================================
void CAGrunt::PrescheduleThink ( void )
{
	BaseClass::PrescheduleThink();
	
	if ( ShouldSpeak() )
	{
		if ( m_flNextWordTime < gpGlobals->curtime )
		{
			// play a new sound
			EmitSound( "AlienGrunt.Idle" );

			// is this word our last?
			if ( random->RandomInt( 1, 10 ) <= 1 )
			{
				// stop talking.
				StopTalking();
			}
			else
			{
				m_flNextWordTime = gpGlobals->curtime + random->RandomFloat( 0.5, 1 );
			}
		}
	}
}

//=========================================================
// DieSound
//=========================================================
void CAGrunt::DeathSound (const CTakeDamageInfo &info)
{
	StopTalking();

	EmitSound( "AlienGrunt.Die" );
}

//=========================================================
// AlertSound
//=========================================================
void CAGrunt::AlertSound ( void )
{
	StopTalking();

	EmitSound( "AlienGrunt.Alert" );
}

//=========================================================
// AttackSound
//=========================================================
void CAGrunt::AttackSound ( void )
{
	StopTalking();

	EmitSound( "AlienGrunt.Attack" );
}

//=========================================================
// PainSound
//=========================================================
void CAGrunt::PainSound (const CTakeDamageInfo &info)
{
	if ( m_flNextPainTime > gpGlobals->curtime )
	{
		return;
	}

	m_flNextPainTime = gpGlobals->curtime + 0.6;

	StopTalking();

	EmitSound( "AlienGrunt.Pain" );
}

//=========================================================
// Classify - indicates this monster's place in the 
// relationship table.
//=========================================================
Class_T	CAGrunt::Classify ( void )
{
	return CLASS_ALIEN_MILITARY;
}

//=========================================================
// MaxYawSpeed - allows each sequence to have a different
// turn rate associated with it.
//=========================================================
float CAGrunt::MaxYawSpeed ( void )
{
	int ys;

	switch (GetActivity())
	{
	case ACT_TURN_LEFT:
	case ACT_TURN_RIGHT:
		ys = 110;
		break;
	default:
		ys = 100;
		break;
	}

	return ys;
}

//=========================================================
// GetSoundInterests - returns a bit mask indicating which types
// of sounds this monster regards. In the base class implementation,
// monsters care about all sounds, but no scents.
//=========================================================
int CAGrunt::GetSoundInterests( void )
{
	return	(SOUND_WORLD | SOUND_COMBAT | SOUND_PLAYER | SOUND_DANGER);
}

void CAGrunt::PunchEnemy(bool right)
{
	Vector vecMins = GetHullMins();
	Vector vecMaxs = GetHullMaxs();
	vecMins.z = vecMins.x;
	vecMaxs.z = vecMaxs.x;
	CBaseEntity* pHurt = CheckTraceHullAttack(AGRUNT_MELEE_DIST, vecMins, vecMaxs, sk_agrunt_dmg_punch.GetFloat(), DMG_CLUB);

	if (pHurt)
	{
		QAngle punchAngle = QAngle((right ? 25 : -25), 8, 0);

		if (pHurt->GetFlags() & (FL_NPC | FL_CLIENT))
		{
			pHurt->ViewPunch(punchAngle);
		}

		// OK to use gpGlobals without calling MakeVectors, cause CheckTraceHullAttack called it above.
		if (pHurt->IsPlayer())
		{
			// this is a player. Knock him around.
			Vector vRight;
			AngleVectors(GetAbsAngles(), NULL, &vRight, NULL);
			pHurt->SetAbsVelocity(pHurt->GetAbsVelocity() + vRight * (right ? -250 : 250));
		}

		EmitSound("AlienGrunt.AttackHit");

		Vector vecArmPos;
		QAngle angArmAng;
		GetAttachment(0, vecArmPos, angArmAng);
		SpawnBlood(vecArmPos, g_vecAttackDir, pHurt->BloodColor(), 25);// a little surface blood.
	}
	else
	{
		// Play a random attack miss sound
		EmitSound("AlienGrunt.AttackMiss");
	}
}

//=========================================================
// HandleAnimEvent - catches the monster-specific messages
// that occur when tagged animation frames are played.
//
// Returns number of events handled, 0 if none.
//=========================================================
void CAGrunt :: HandleAnimEvent( animevent_t *pEvent )
{
	switch( pEvent->event )
	{
	case AGRUNT_AE_HORNET1:
	case AGRUNT_AE_HORNET2:
	case AGRUNT_AE_HORNET3:
	case AGRUNT_AE_HORNET4:
	case AGRUNT_AE_HORNET5:
		{
			// m_vecEnemyLKP should be center of enemy body
			Vector vecArmPos, vecDirToEnemy;
			QAngle angDir, angArmDir;

			if (HasCondition( COND_SEE_ENEMY) && GetEnemy())
			{
				Vector vecEnemyLKP = GetEnemy()->GetAbsOrigin();

				vecDirToEnemy = ((vecEnemyLKP) - GetAbsOrigin());
				VectorAngles(vecDirToEnemy, angDir);
				VectorNormalize(vecDirToEnemy);
			}
			else
			{
				angDir = GetAbsAngles();
				angDir.x = -angDir.x;

				Vector vForward;
				AngleVectors(angDir, &vForward);
				vecDirToEnemy = vForward;
			}

			DoMuzzleFlash();

			// make angles +-180
			if (angDir.x > 180)
			{
				angDir.x = angDir.x - 360;
			}

			GetAttachment( "0", vecArmPos, angArmDir);
			vecArmPos = vecArmPos + vecDirToEnemy * 32;

			CPVSFilter filter(GetAbsOrigin());
			te->Sprite(filter, 0.0, &vecArmPos, iAgruntMuzzleFlash, random->RandomFloat(0.4, 0.8), 128);

			CBaseEntity* pHornet = CBaseEntity::Create("hornet", vecArmPos, QAngle(0, 0, 0), this);
			Vector vForward;
			AngleVectors(angDir, &vForward);

			pHornet->SetAbsVelocity(vForward * 300);
			pHornet->SetOwnerEntity(this);

			EmitSound("Weapon_Hornetgun.Single");

			CAI_BaseNPC* pHornetMonster = (CAI_BaseNPC*)pHornet->MyNPCPointer();

			if (pHornetMonster)
			{
				pHornetMonster->SetEnemy(GetEnemy());
			}
		}
		break;

	case AGRUNT_AE_LEFT_FOOT:
		EmitSound("AlienGrunt.LeftFoot");
		break;
	case AGRUNT_AE_RIGHT_FOOT:
		EmitSound("AlienGrunt.RightFoot");
		break;
	case AGRUNT_AE_LEFT_PUNCH:
		PunchEnemy(false);
		break;
	case AGRUNT_AE_RIGHT_PUNCH:
		PunchEnemy(true);
		break;
	default:
		BaseClass::HandleAnimEvent(pEvent);
		break;
	}
}

//=========================================================
// Spawn
//=========================================================
void CAGrunt::Spawn()
{
	Precache();

	SetModel( "models/agrunt.mdl");
	SetHullType( HULL_WIDE_HUMAN );
	SetHullSizeNormal();

	SetSolid( SOLID_BBOX );
	AddSolidFlags( FSOLID_NOT_STANDABLE );
	SetMoveType( MOVETYPE_STEP );
	SetBloodColor(BLOOD_COLOR_GREEN);
	ClearEffects();
	SetHealth(sk_agrunt_health.GetInt());
	m_flFieldOfView		= 0.2;// indicates the width of this monster's forward view cone ( as a dotproduct result )
	m_NPCState			= NPC_STATE_NONE;
	CapabilitiesClear();
	CapabilitiesAdd(bits_CAP_SQUAD|bits_CAP_MOVE_GROUND);
	CapabilitiesAdd(bits_CAP_INNATE_RANGE_ATTACK1);
	CapabilitiesAdd(bits_CAP_INNATE_MELEE_ATTACK1);

	m_HackedGunPos		= Vector( 24, 64, 48 );

	m_flNextSpeakTime	= m_flNextWordTime = gpGlobals->curtime + 10 + random->RandomInt(0, 10);
	
	NPCInit();
	BaseClass::Spawn();
}

//=========================================================
// Precache - precaches all resources this monster needs
//=========================================================
void CAGrunt::Precache()
{
	PrecacheModel("models/agrunt.mdl");

	PrecacheScriptSound( "Weapon_Hornetgun.Single" );
	PrecacheScriptSound( "AlienGrunt.LeftFoot" );
	PrecacheScriptSound( "AlienGrunt.RightFoot" );
	PrecacheScriptSound( "AlienGrunt.AttackHit" );
	PrecacheScriptSound( "AlienGrunt.AttackMiss" );
	PrecacheScriptSound( "AlienGrunt.Die" );
	PrecacheScriptSound( "AlienGrunt.Alert" );
	PrecacheScriptSound( "AlienGrunt.Attack" );
	PrecacheScriptSound( "AlienGrunt.Pain" );
	PrecacheScriptSound( "AlienGrunt.Idle" );

	iAgruntMuzzleFlash = PrecacheModel( "sprites/muz4.vmt" );

	UTIL_PrecacheOther( "hornet" );
}

//=========================================================
// FCanCheckAttacks - this is overridden for alien grunts
// because they can use their smart weapons against unseen
// enemies. Base class doesn't attack anyone it can't see.
//=========================================================
bool CAGrunt::FCanCheckAttacks ( void )
{
	if ( !HasCondition( COND_ENEMY_TOO_FAR ) )
	{
		return true;
	}
	else
	{
		return false;
	}
}

//=========================================================
// MeleeAttack1Conditions - alien grunts zap the crap out of 
// any enemy that gets too close. 
//=========================================================
int CAGrunt::MeleeAttack1Conditions( float flDot, float flDist )
{
	if ( HasCondition(COND_SEE_ENEMY) 
		&& flDist <= AGRUNT_MELEE_DIST 
		&& flDot >= AGRUNT_MELEE_DOT 
		&& GetEnemy() != NULL )
	{
		return COND_CAN_MELEE_ATTACK1;
	}
	
	return COND_NONE;
}

//=========================================================
// RangeAttack1Conditions
//
// !!!LATER - we may want to load balance this. Several
// tracelines are done, so we may not want to do this every
// server frame. Definitely not while firing. 
//=========================================================
int CAGrunt::RangeAttack1Conditions( float flDot, float flDist )
{
	if ( gpGlobals->curtime < m_flNextHornetAttackCheck )
	{
		if (HasCondition(COND_SEE_ENEMY))
		{
			if (m_fCanHornetAttack)
			{
				return COND_CAN_RANGE_ATTACK1;
			}
		}
	}
	
	if ( HasCondition( COND_SEE_ENEMY ) 
		&& flDist >= AGRUNT_MELEE_DIST 
		&& flDist <= AGRUNT_RANGED_DIST 
		&& flDot >= AGRUNT_RANGED_DOT)
	{
		trace_t tr;
		Vector	vecArmPos;
		QAngle	angArmDir;
		
		// verify that a shot fired from the gun will hit the enemy before the world.
		// !!!LATER - we may wish to do something different for projectile weapons as opposed to instant-hit
		GetAttachment( "0", vecArmPos, angArmDir );
		UTIL_TraceLine( vecArmPos, GetEnemy()->BodyTarget( vecArmPos ), MASK_SOLID, this, COLLISION_GROUP_NONE, &tr);

		if ( tr.fraction == 1.0 || tr.m_pEnt == GetEnemy() )
		{
			m_flNextHornetAttackCheck = gpGlobals->curtime + random->RandomFloat( 2, 5 );
			m_fCanHornetAttack = true;
			return m_fCanHornetAttack;
		}
	}
	
	m_flNextHornetAttackCheck = gpGlobals->curtime + 0.2f;// don't check for half second if this check wasn't successful
	m_fCanHornetAttack = false;
	return COND_NONE;
}

//=========================================================
// StartTask
//=========================================================
void CAGrunt::StartTask ( const Task_t *pTask )
{
	switch ( pTask->iTask )
	{
	case TASK_AGRUNT_GET_PATH_TO_ENEMY_CORPSE:
		{
			Vector forward;
			AngleVectors( GetAbsAngles(), &forward );
			Vector flEnemyLKP = GetEnemyLKP();
			
			if ( GetNavigator()->SetGoal( flEnemyLKP - forward * 64, AIN_CLEAR_TARGET))
			{
				TaskComplete();
			}
			else
			{
				Warning("AGruntGetPathToEnemyCorpse failed!!\n");
				TaskFail(FAIL_NO_ROUTE );
			}
		}
		break;

	case TASK_AGRUNT_SETUP_HIDE_ATTACK:
		// alien grunt shoots hornets back out into the open from a concealed location. 
		// try to find a spot to throw that gives the smart weapon a good chance of finding the enemy.
		// ideally, this spot is along a line that is perpendicular to a line drawn from the agrunt to the enemy.

		CAI_BaseNPC* pEnemyMonsterPtr;
		pEnemyMonsterPtr = (CAI_BaseNPC*)GetEnemy()->MyNPCPointer();

		if ( pEnemyMonsterPtr )
		{
			Vector		vecCenter, vForward, vRight, vecEnemyLKP;
			QAngle		angTmp;
			trace_t		tr;
			bool		fSkip;

			fSkip = false;
			vecCenter = WorldSpaceCenter();
			vecEnemyLKP = GetEnemyLKP();

			VectorAngles( vecEnemyLKP - GetAbsOrigin(), angTmp );
			SetAbsAngles( angTmp );
			AngleVectors( GetAbsAngles(), &vForward, &vRight, NULL );
			
			float flMult = 128.0f;
			int checks = 0;
			
			while (!fSkip && checks < 4)
			{
				UTIL_TraceLine( WorldSpaceCenter() + vForward * flMult, vecEnemyLKP, MASK_SOLID_BRUSHONLY, this, COLLISION_GROUP_NONE, &tr);
				if ( tr.fraction == 1.0 )
				{
					GetMotor()->SetIdealYawToTargetAndUpdate ( GetAbsOrigin() + vRight * flMult );
					fSkip = true;
					checks = 0;
					TaskComplete();
				}
				else
				{
					if (checks == 2 && flMult != 256)
					{
						flMult = flMult * 2;
					}
					checks += 1;
				}
			}
			
			if (!fSkip && checks == 4)
			{
				TaskFail( FAIL_NO_COVER );
			}
		}
		else
		{
			Warning( "AGRunt - no enemy monster ptr!!!\n" );
			TaskFail(FAIL_NO_ROUTE );
		}
		break;
	default:
		BaseClass::StartTask ( pTask );
		break;
	}
}

//=========================================================
// SelectSchedule - Decides which type of schedule best suits
// the monster's current state and conditions. Then calls
// monster's member function to get a pointer to a schedule
// of the proper type.
//=========================================================
int CAGrunt::SelectSchedule( void )
{
	if ( HasCondition( COND_HEAR_DANGER ) )
	{
		return SCHED_TAKE_COVER_FROM_BEST_SOUND;
	}

	switch	( m_NPCState )
	{
		case NPC_STATE_COMBAT:
			{
	// dead enemy
				if ( HasCondition(COND_ENEMY_DEAD) )
				{
					// call base class, all code to handle dead enemies is centralized there.
					return BaseClass::SelectSchedule();
				}

				if ( HasCondition(COND_NEW_ENEMY) )
				{
					return SCHED_WAKE_ANGRY;
				}

		// zap player!
				if ( HasCondition( COND_CAN_MELEE_ATTACK1 ) )
				{
					AttackSound();// this is a total hack. Should be parto f the schedule
					return SCHED_MELEE_ATTACK1;
				}

				if ( HasCondition( COND_HEAVY_DAMAGE ) )
				{
					return SCHED_SMALL_FLINCH;
				}

		// can attack
				if ( HasCondition( COND_CAN_RANGE_ATTACK1 ) && OccupyStrategySlotRange( AGRUNT_SQUAD_SLOT_HORNET1, AGRUNT_SQUAD_SLOT_HORNET2 ) )
				{
					return SCHED_RANGE_ATTACK1;
				}

				if ( OccupyStrategySlot(AGRUNT_SQUAD_SLOT_CHASE) )
				{
					return SCHED_CHASE_ENEMY;
				}

				return SCHED_STANDOFF;
			}
		break;
	}

	//If we are in idle, try to find the enemy by walking.
	if (m_NPCState == NPC_STATE_IDLE || m_NPCState == NPC_STATE_ALERT)
	{
		return SCHED_PATROL_WALK;
	}

	return BaseClass::SelectSchedule();
}

//=========================================================
//=========================================================
int CAGrunt::TranslateSchedule( int iType ) 
{
	switch	( iType )
	{
		case SCHED_TAKE_COVER_FROM_ENEMY:
			return SCHED_AGRUNT_TAKE_COVER_FROM_ENEMY;
			break;

		case SCHED_STANDOFF:
			return SCHED_AGRUNT_STANDOFF;
			break;

		case SCHED_VICTORY_DANCE:
			return SCHED_AGRUNT_VICTORY_DANCE;
			break;

		case SCHED_FAIL:
			// no fail schedule specified, so pick a good generic one.
			{
				if ( GetEnemy() != NULL )
				{
					// I have an enemy
					// !!!LATER - what if this enemy is really far away and i'm chasing him?
					// this schedule will make me stop, face his last known position for 2 
					// seconds, and then try to move again
					return SCHED_AGRUNT_COMBAT_FAIL;
				}

				return SCHED_AGRUNT_FAIL;
			}
			break;
	}

	return BaseClass::TranslateSchedule(iType);
}

//=========================================================
// AI Schedules Specific to this monster
//=========================================================

AI_BEGIN_CUSTOM_NPC(npc_agrunt, CAGrunt)

	DECLARE_ACTIVITY( ACT_THREAT_DISPLAY )

	DECLARE_TASK ( TASK_AGRUNT_SETUP_HIDE_ATTACK )
	DECLARE_TASK ( TASK_AGRUNT_GET_PATH_TO_ENEMY_CORPSE )

	DECLARE_SQUADSLOT( AGRUNT_SQUAD_SLOT_HORNET1 )
	DECLARE_SQUADSLOT( AGRUNT_SQUAD_SLOT_HORNET2 )
	DECLARE_SQUADSLOT( AGRUNT_SQUAD_SLOT_CHASE )
	
	//=========================================================
	// Fail Schedule
	//=========================================================
	DEFINE_SCHEDULE
	(
		SCHED_AGRUNT_FAIL,
			"	Tasks"
			"	TASK_STOP_MOVING			0"
			"	TASK_SET_ACTIVITY			ACTIVITY:ACT_IDLE"
			"	TASK_WAIT					2"
			"	TASK_WAIT_PVS				0"
			"	"
			"	Interrupts"
			"	COND_CAN_RANGE_ATTACK1"
			"	COND_CAN_MELEE_ATTACK1"
	)

	//=========================================================
	// Combat Fail Schedule
	//=========================================================
	DEFINE_SCHEDULE
	(
		SCHED_AGRUNT_COMBAT_FAIL,
			"	Tasks"
			"	TASK_STOP_MOVING			0"
			"	TASK_SET_ACTIVITY			ACTIVITY:ACT_IDLE"
			"	TASK_WAIT_FACE_ENEMY		2"
			"	TASK_WAIT_PVS				0"
			"	"
			"	Interrupts"
			"	COND_CAN_RANGE_ATTACK1"
			"	COND_CAN_MELEE_ATTACK1"
	)

	//=========================================================
	// Standoff schedule. Used in combat when a monster is 
	// hiding in cover or the enemy has moved out of sight. 
	// Should we look around in this schedule?
	//=========================================================
	DEFINE_SCHEDULE
	(
		SCHED_AGRUNT_STANDOFF,
		
			"	Tasks"
			"		TASK_STOP_MOVING			0"
			"		TASK_SET_ACTIVITY			ACTIVITY:ACT_IDLE"
			"		TASK_WAIT_FACE_ENEMY		2"
			"	"
			"	Interrupts"
			"		COND_CAN_RANGE_ATTACK1"
			"		COND_CAN_MELEE_ATTACK1"
			"		COND_SEE_ENEMY"
			"		COND_NEW_ENEMY"
			"		COND_HEAR_DANGER"
	)

	//=========================================================
	// Suppress
	//=========================================================
	DEFINE_SCHEDULE
	(
		SCHED_AGRUNT_SUPPRESS_HORNET,
			"	Tasks"
			"	TASK_STOP_MOVING			0"
			"	TASK_RANGE_ATTACK1			0"
	)

	//=========================================================
	// primary range attacks
	//=========================================================
	DEFINE_SCHEDULE
	(
		SCHED_AGRUNT_RANGE_ATTACK,
			"	Tasks"
			"	TASK_STOP_MOVING			0"
			"	TASK_FACE_ENEMY				0"
			"	TASK_RANGE_ATTACK1			0"
			"	"
			"	Interrupts"
			"	COND_NEW_ENEMY"
			"	COND_ENEMY_DEAD"
			"	COND_HEAVY_DAMAGE"
	)

	DEFINE_SCHEDULE
	(
		SCHED_AGRUNT_HIDDEN_RANGE_ATTACK,
			"	Tasks"
			"	TASK_SET_FAIL_SCHEDULE			SCHEDULE:SCHED_AGRUNT_STANDOFF"
			"	TASK_AGRUNT_SETUP_HIDE_ATTACK		0"
			"	TASK_STOP_MOVING					0"
			"	TASK_FACE_IDEAL						0"
			"	"
			"	Interrupts"
			"	COND_NEW_ENEMY"
			"	COND_HEAVY_DAMAGE"
			"	COND_HEAR_DANGER"
	)

	//=========================================================
	// Take cover from enemy! Tries lateral cover before node 
	// cover! 
	//=========================================================
	DEFINE_SCHEDULE
	(
		SCHED_AGRUNT_TAKE_COVER_FROM_ENEMY,
			"	Tasks"
			"	TASK_STOP_MOVING				0"
			"	TASK_WAIT						0.2"
			"	TASK_FIND_COVER_FROM_ENEMY		0"
			"	TASK_RUN_PATH					0"
			"	TASK_WAIT_FOR_MOVEMENT			0"
			"	TASK_REMEMBER					MEMORY:INCOVER"
			"	TASK_FACE_ENEMY					0"	
			"	"
			"	Interrupts"
			"	COND_NEW_ENEMY"
	)

	//=========================================================
	// Victory dance!
	//=========================================================
	DEFINE_SCHEDULE
	(
		SCHED_AGRUNT_VICTORY_DANCE,
			"	Tasks"
			"	TASK_STOP_MOVING							0"
			"	TASK_SET_FAIL_SCHEDULE						SCHEDULE:SCHED_AGRUNT_THREAT_DISPLAY"
			"	TASK_WAIT									0.2"
			"	TASK_AGRUNT_GET_PATH_TO_ENEMY_CORPSE		0"
			"	TASK_WALK_PATH								0"
			"	TASK_WAIT_FOR_MOVEMENT						0"
			"	TASK_FACE_ENEMY								0"
			"	TASK_PLAY_SEQUENCE							ACTIVITY:ACT_CROUCH"
			"	TASK_PLAY_SEQUENCE							ACTIVITY:ACT_VICTORY_DANCE"
			"	TASK_PLAY_SEQUENCE							ACTIVITY:ACT_VICTORY_DANCE"
			"	TASK_PLAY_SEQUENCE							ACTIVITY:ACT_STAND"
			"	TASK_PLAY_SEQUENCE							ACTIVITY:ACT_THREAT_DISPLAY"
			"	TASK_PLAY_SEQUENCE							ACTIVITY:ACT_CROUCH"
			"	TASK_PLAY_SEQUENCE							ACTIVITY:ACT_VICTORY_DANCE"
			"	TASK_PLAY_SEQUENCE							ACTIVITY:ACT_VICTORY_DANCE"
			"	TASK_PLAY_SEQUENCE							ACTIVITY:ACT_VICTORY_DANCE"
			"	TASK_PLAY_SEQUENCE							ACTIVITY:ACT_VICTORY_DANCE"
			"	TASK_PLAY_SEQUENCE							ACTIVITY:ACT_VICTORY_DANCE"
			"	TASK_PLAY_SEQUENCE							ACTIVITY:ACT_STAND"
			"	"
			"	Interrupts"
			"	COND_NEW_ENEMY"
			"	COND_LIGHT_DAMAGE"
			"	COND_HEAVY_DAMAGE"
	)

	//=========================================================
	//=========================================================
	DEFINE_SCHEDULE
	(
		SCHED_AGRUNT_THREAT_DISPLAY,
			"	Tasks"
			"	TASK_STOP_MOVING		0"
			"	TASK_FACE_ENEMY			0"
			"	TASK_PLAY_SEQUENCE		ACTIVITY:ACT_THREAT_DISPLAY"
			"	"
			"	Interrupts"
			"	COND_NEW_ENEMY"
			"	COND_LIGHT_DAMAGE"
			"	COND_HEAVY_DAMAGE"
			"	COND_HEAR_PLAYER"
			"	COND_HEAR_COMBAT"
			"	COND_HEAR_WORLD"
	)
	
AI_END_CUSTOM_NPC()