//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Small, fast version of the strider. Goes where striders cannot, such
// as into buildings. Best killed with physics objects and explosives.
//
//=============================================================================

#include "cbase.h"
#include "npc_strider.h"
#include "npc_ministrider.h"
#include "ai_behavior_follow.h"
#include "ai_moveprobe.h"
#include "ai_senses.h"
#include "ai_speech.h"
#include "ai_task.h"
#include "ai_default.h"
#include "ai_schedule.h"
#include "ai_hull.h"
#include "ai_baseactor.h"
#include "ai_waypoint.h"
#include "ai_link.h"
#include "ai_hint.h"
#include "ai_squadslot.h"
#include "ai_squad.h"
#include "ai_tacticalservices.h"
#include "beam_shared.h"
#include "datacache/imdlcache.h"
#include "eventqueue.h"
#include "gib.h"
#include "globalstate.h"
#include "hierarchy.h"
#include "movevars_shared.h"
#include "npcevent.h"
#include "saverestore_utlvector.h"
#include "particle_parse.h"
#include "te_particlesystem.h"
#include "sceneentity.h"
#include "shake.h"
#include "soundenvelope.h"
#include "soundent.h"
#include "SpriteTrail.h"
#include "IEffects.h"
#include "engine/IEngineSound.h"
#include "bone_setup.h"
#include "studio.h"
#include "ai_route.h"
#include "ammodef.h"
#include "npc_bullseye.h"
#include "physobj.h"
#include "ai_memory.h"
#include "collisionutils.h"
#include "shot_manipulator.h"
#include "steamjet.h"
#include "physics_prop_ragdoll.h"
#include "vehicle_base.h"
#include "coordsize.h"
#include "hl2_shareddefs.h"
#include "te_effect_dispatch.h"
#include "beam_flags.h"
#include "prop_combine_ball.h"
#include "explode.h"
#include "weapon_physcannon.h"
#include "weapon_striderbuster.h"
#include "monstermaker.h"
#include "weapon_rpg.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

class CNPC_MiniStrider;

// Think contexts
static const char *MINISTRIDER_BLEED_THINK = "MiniStriderBleed";
static const char *MINISTRIDER_ZAP_THINK = "MiniStriderZap";
static const char *MINISTRIDER_JOSTLE_VEHICLE_THINK = "MiniStriderJostle";

ConVar sk_ministrider_health( "sk_ministrider_health", "150" );

// Melee attacks
ConVar sk_ministrider_dmg_one_slash( "sk_ministrider_dmg_one_slash", "20" );
ConVar sk_ministrider_dmg_charge( "sk_ministrider_dmg_charge", "20" );

// Flechette volley attack
ConVar ministrider_ar2round_max_range( "ministrider_ar2round_max_range", "1200" );
ConVar ministrider_ar2round_min_range( "ministrider_ar2round_min_range", "100" );
ConVar ministrider_ar2round_volley_size( "ministrider_ar2round_volley_size", "8" );
ConVar ministrider_ar2round_speed( "ministrider_ar2round_speed", "2000" );
ConVar sk_ministrider_dmg_ar2round( "sk_ministrider_dmg_ar2round", "5.0" );
ConVar ministrider_ar2round_delay("ministrider_ar2round_delay", "0.1");
ConVar ministrider_first_ar2round_delay("ministrider_first_ar2round_delay", "0.5");
ConVar ministrider_ar2round_max_concurrent_volleys( "ministrider_ar2round_max_concurrent_volleys", "2" );
ConVar ministrider_ar2round_volley_start_min_delay( "ministrider_ar2round_volley_start_min_delay", ".25" );
ConVar ministrider_ar2round_volley_start_max_delay( "ministrider_ar2round_volley_start_max_delay", ".95" );
ConVar ministrider_ar2round_volley_end_min_delay( "ministrider_ar2round_volley_end_min_delay", "1" );
ConVar ministrider_ar2round_volley_end_max_delay( "ministrider_ar2round_volley_end_max_delay", "2" );
ConVar ministrider_ar2round_test( "ministrider_ar2round_test", "0" );
ConVar ministrider_clamp_shots( "ministrider_clamp_shots", "1" );

// Damage received
ConVar sk_ministrider_bullet_damage_scale( "sk_ministrider_bullet_damage_scale", "0.6" );
ConVar sk_ministrider_charge_damage_scale( "sk_ministrider_charge_damage_scale", "2.0" );
ConVar sk_ministrider_buckshot_damage_scale( "sk_ministrider_buckshot_damage_scale", "0.5" );
ConVar sk_ministrider_vehicle_damage_scale( "sk_ministrider_vehicle_damage_scale", "2.2" );
ConVar sk_ministrider_dmg_from_striderbuster( "sk_ministrider_dmg_from_striderbuster", "150" );
ConVar sk_ministrider_citizen_damage_scale( "sk_ministrider_citizen_damage_scale", "0.3" );

ConVar ministrider_allow_dissolve( "ministrider_allow_dissolve", "1" );
ConVar ministrider_random_expressions( "ministrider_random_expressions", "0" );
ConVar ministrider_show_weapon_los_z( "ministrider_show_weapon_los_z", "0" );
ConVar ministrider_show_weapon_los_condition( "ministrider_show_weapon_los_condition", "0" );

ConVar ministrider_melee_delay( "ministrider_melee_delay", "2.0" );

// Bullrush charge.
ConVar ministrider_charge( "ministrider_charge", "1" );
ConVar ministrider_charge_min_delay( "ministrider_charge_min_delay", "10.0" );
ConVar ministrider_charge_pct( "ministrider_charge_pct", "25" );
ConVar ministrider_charge_test( "ministrider_charge_test", "0" );

// Vehicle dodging.
ConVar ministrider_dodge_warning( "ministrider_dodge_warning", "1.1" );
ConVar ministrider_dodge_warning_width( "ministrider_dodge_warning_width", "180" );
ConVar ministrider_dodge_warning_cone( "ministrider_dodge_warning_cone", ".5" );
ConVar ministrider_dodge_debug( "ministrider_dodge_debug", "0" );

// Jostle vehicles when hit by them
ConVar ministrider_jostle_car_min_speed( "ministrider_jostle_car_min_speed", "100" ); // If hit by a car going at least this fast, jostle the car
ConVar ministrider_jostle_car_max_speed( "ministrider_jostle_car_max_speed", "600" ); // Used for determining jostle scale

ConVar ministrider_free_knowledge( "ministrider_free_knowledge", "10.0" );
ConVar ministrider_plant_adjust_z( "ministrider_plant_adjust_z", "12" );

ConVar ministrider_disable_patrol( "ministrider_disable_patrol", "0" );

// Dealing with striderbusters
ConVar ministrider_hate_held_striderbusters( "ministrider_hate_held_striderbusters", "1" );
ConVar ministrider_hate_thrown_striderbusters( "ministrider_hate_thrown_striderbusters", "1" );
ConVar ministrider_hate_attached_striderbusters( "ministrider_hate_attached_striderbusters", "1" );
ConVar ministrider_hate_held_striderbusters_delay( "ministrider_hate_held_striderbusters_delay", "0.5" );
ConVar ministrider_hate_held_striderbusters_tolerance( "ministrider_hate_held_striderbusters_tolerance", "2000.0" );
ConVar ministrider_hate_thrown_striderbusters_tolerance( "ministrider_hate_thrown_striderbusters_tolerance", "300.0" );
ConVar ministrider_seek_thrown_striderbusters_tolerance( "ministrider_seek_thrown_striderbusters_tolerance", "400.0" );
ConVar ministrider_retreat_striderbusters( "ministrider_retreat_striderbusters", "1", FCVAR_NONE, "If true, the ministrider will retreat when a buster is glued to him." );

ConVar ministrider_allow_nav_jump( "ministrider_allow_nav_jump", "0" );
ConVar g_debug_ministrider_charge( "g_debug_ministrider_charge", "0" );

ConVar ministrider_stand_still( "ministrider_stand_still", "0" ); // used for debugging, keeps them rooted in place

ConVar ministrider_siege_frequency( "ministrider_siege_frequency", "12" );

extern ConVar hunter_laugh;
extern ConVar hunter_laugh_frequency;
extern ConVar hunter_laugh_healthvalue;

#define MINISTRIDER_FOV_DOT					0.0		// 180 degree field of view
#define MINISTRIDER_CHARGE_MIN				256
#define MINISTRIDER_CHARGE_MAX				1024
#define MINISTRIDER_FACE_ENEMY_DIST			512.0f
#define MINISTRIDER_MELEE_REACH				80
#define MINISTRIDER_BLOOD_LEFT_FOOT			0
#define MINISTRIDER_IGNORE_ENEMY_TIME		5		// How long the ministrider will ignore another enemy when distracted by the player.

#define MINISTRIDER_FACING_DOT				0.8		// The angle within which we start shooting
#define MINISTRIDER_SHOOT_MAX_YAW_DEG		60.0f	// Once shooting, clamp to +/- these degrees of yaw deflection as our target moves
#define MINISTRIDER_SHOOT_MAX_YAW_COS		0.5f	// The cosine of the above angle

#define MINISTRIDER_FLECHETTE_WARN_TIME		1.0f

#define MINISTRIDER_SEE_ENEMY_TIME_INVALID	-1

#define NUM_FLECHETTE_VOLLEY_ON_FOLLOW 4

#define MINISTRIDER_SIEGE_MAX_DIST_MODIFIER 2.0f

#define MINISTRIDER_SKIN_DEFAULT		0
#define MINISTRIDER_SKIN_DEAD		1

//-----------------------------------------------------------------------------
// Animation events
//-----------------------------------------------------------------------------
int AE_MINTER_FOOTSTEP_LEFT;
int AE_MINTER_FOOTSTEP_RIGHT;
int AE_MINTER_FOOTSTEP_BACK;
int AE_MINTER_MELEE_ANNOUNCE;
int AE_MINTER_MELEE_ATTACK_LEFT;
int AE_MINTER_MELEE_ATTACK_RIGHT;
int AE_MINTER_DIE;
int AE_MINTER_SPRAY_BLOOD;
int AE_MINTER_START_EXPRESSION;
int AE_MINTER_END_EXPRESSION;

//-----------------------------------------------------------------------------
// Interactions.
//-----------------------------------------------------------------------------
int g_interactionMiniStriderFoundEnemy = 0;

//-----------------------------------------------------------------------------
// Local stuff.
//-----------------------------------------------------------------------------
static string_t s_iszStriderClassname;
static string_t s_iszStriderBusterClassname;
static string_t s_iszMagnadeClassname;
static string_t s_iszPhysPropClassname;
static string_t s_iszMiniStridersToRunOver;


//-----------------------------------------------------------------------------
// Custom Activities
//-----------------------------------------------------------------------------
Activity ACT_MINTER_DEPLOYRA2;
Activity ACT_MINTER_DODGER;
Activity ACT_MINTER_DODGEL;
Activity ACT_MINTER_GESTURE_SHOOT;
Activity ACT_MINTER_FLINCH_STICKYBOMB;
Activity ACT_MINTER_STAGGER;
Activity ACT_MINTER_MELEE_ATTACK1_VS_PLAYER;
Activity ACT_DI_MINTER_MELEE;
Activity ACT_DI_MINTER_THROW;
Activity ACT_MINTER_ANGRY;
Activity ACT_MINTER_WALK_ANGRY;
Activity ACT_MINTER_FOUND_ENEMY;
Activity ACT_MINTER_FOUND_ENEMY_ACK;
Activity ACT_MINTER_CHARGE_START;
Activity ACT_MINTER_CHARGE_RUN;
Activity ACT_MINTER_CHARGE_STOP;
Activity ACT_MINTER_CHARGE_CRASH;
Activity ACT_MINTER_CHARGE_HIT;
Activity ACT_MINTER_RANGE_ATTACK2_UNPLANTED;
Activity ACT_MINTER_IDLE_PLANTED;
Activity ACT_MINTER_FLINCH_N;
Activity ACT_MINTER_FLINCH_S;
Activity ACT_MINTER_FLINCH_E;
Activity ACT_MINTER_FLINCH_W;


//-----------------------------------------------------------------------------
//	Squad slots
//-----------------------------------------------------------------------------
enum SquadSlot_t
{	
	SQUAD_SLOT_MINISTRIDER_CHARGE = LAST_SHARED_SQUADSLOT,
	SQUAD_SLOT_MINISTRIDER_FLANK_FIRST,
	SQUAD_SLOT_MINISTRIDER_FLANK_LAST = SQUAD_SLOT_MINISTRIDER_FLANK_FIRST,
	SQUAD_SLOT_RUN_SHOOT,
};

#define	MINISTRIDER_FOLLOW_DISTANCE	2000.0f
#define	MINISTRIDER_FOLLOW_DISTANCE_SQR	(MINISTRIDER_FOLLOW_DISTANCE * MINISTRIDER_FOLLOW_DISTANCE)

#define MINISTRIDER_RUNDOWN_SQUADDATA 0


//-----------------------------------------------------------------------------
// We're doing this quite a lot, so this makes the check a lot faster since
// we don't have to compare strings.
//-----------------------------------------------------------------------------
extern bool IsStriderBuster(CBaseEntity *pEntity);
extern bool HateThisStriderBuster(CBaseEntity *pTarget);

//-----------------------------------------------------------------------------
// Calculate & apply damage & force for a charge to a target.
// Done outside of the ministrider because we need to do this inside a trace filter.
//-----------------------------------------------------------------------------
void MiniStrider_ApplyChargeDamage( CBaseEntity *pMiniStrider, CBaseEntity *pTarget, float flDamage )
{
	Vector attackDir = ( pTarget->WorldSpaceCenter() - pMiniStrider->WorldSpaceCenter() );
	VectorNormalize( attackDir );
	Vector offset = RandomVector( -32, 32 ) + pTarget->WorldSpaceCenter();

	// Generate enough force to make a 75kg guy move away at 700 in/sec
	Vector vecForce = attackDir * ImpulseScale( 75, 700 );

	// Deal the damage
	CTakeDamageInfo	info( pMiniStrider, pMiniStrider, vecForce, offset, flDamage, DMG_CLUB );
	pTarget->TakeDamage( info );
}


//-----------------------------------------------------------------------------
// A simple trace filter class to skip small moveable physics objects
//-----------------------------------------------------------------------------
class CMiniStriderTraceFilterSkipPhysics : public CTraceFilter
{
public:
	// It does have a base, but we'll never network anything below here..
	DECLARE_CLASS_NOBASE( CMiniStriderTraceFilterSkipPhysics );
	
	CMiniStriderTraceFilterSkipPhysics( const IHandleEntity *passentity, int collisionGroup, float minMass )
		: m_pPassEnt(passentity), m_collisionGroup(collisionGroup), m_minMass(minMass)
	{
	}
	virtual bool ShouldHitEntity( IHandleEntity *pHandleEntity, int contentsMask )
	{
		if ( !StandardFilterRules( pHandleEntity, contentsMask ) )
			return false;

		if ( !PassServerEntityFilter( pHandleEntity, m_pPassEnt ) )
			return false;

		// Don't test if the game code tells us we should ignore this collision...
		CBaseEntity *pEntity = EntityFromEntityHandle( pHandleEntity );
		if ( pEntity )
		{
			if ( !pEntity->ShouldCollide( m_collisionGroup, contentsMask ) )
				return false;
			
			if ( !g_pGameRules->ShouldCollide( m_collisionGroup, pEntity->GetCollisionGroup() ) )
				return false;

			// don't test small moveable physics objects (unless it's an NPC)
			if ( !pEntity->IsNPC() && pEntity->GetMoveType() == MOVETYPE_VPHYSICS )
			{
				float entMass = PhysGetEntityMass( pEntity ) ;
				if ( entMass < m_minMass )
				{
					if ( entMass < m_minMass * 0.666f || pEntity->CollisionProp()->BoundingRadius() < (assert_cast<const CAI_BaseNPC *>(EntityFromEntityHandle( m_pPassEnt )))->GetHullHeight() )
					{
						return false;
					}
				}
			}

			// If we hit an antlion, don't stop, but kill it
			if ( pEntity->Classify() == CLASS_ANTLION  && g_pGameRules->IsSkillLevel( SKILL_EASY ) )
			{
				CBaseEntity *pMiniStrider = (CBaseEntity *)EntityFromEntityHandle( m_pPassEnt );
				MiniStrider_ApplyChargeDamage( pMiniStrider, pEntity, pEntity->GetHealth() );
				return false;
			}
		}

		return true;
	}

private:
	const IHandleEntity *m_pPassEnt;
	int m_collisionGroup;
	float m_minMass;
};


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
inline void MiniStriderTraceHull_SkipPhysics( const Vector &vecAbsStart, const Vector &vecAbsEnd, const Vector &hullMin, 
					 const Vector &hullMax,	unsigned int mask, const CBaseEntity *ignore, 
					 int collisionGroup, trace_t *ptr, float minMass )
{
	Ray_t ray;
	ray.Init( vecAbsStart, vecAbsEnd, hullMin, hullMax );
	CMiniStriderTraceFilterSkipPhysics traceFilter( ignore, collisionGroup, minMass );
	enginetrace->TraceRay( ray, mask, &traceFilter, ptr );
}


//-----------------------------------------------------------------------------
// MiniStrider follow behavior
//-----------------------------------------------------------------------------
class CAI_MiniStriderEscortBehavior : public CAI_FollowBehavior
{	
public:
	DECLARE_CLASS( CAI_MiniStriderEscortBehavior, CAI_FollowBehavior );

	CAI_MiniStriderEscortBehavior() :
		BaseClass( AI_FollowParams_t( AIF_HUNTER, true ) ),
		m_flTimeEscortReturn( 0 ),
		m_bEnabled( false )
	{
	}

	CNPC_MiniStrider *GetOuter() { return (CNPC_MiniStrider *)( BaseClass::GetOuter() ); }

	void SetEscortTarget( CNPC_Strider *pLeader, bool fFinishCurSchedule = false );
	CNPC_Strider * GetEscortTarget() { return (CNPC_Strider *)GetFollowTarget(); }

	bool FarFromFollowTarget()
	{ 
		return ( GetFollowTarget() && (GetAbsOrigin() - GetFollowTarget()->GetAbsOrigin()).LengthSqr() > MINISTRIDER_FOLLOW_DISTANCE_SQR ); 
	}

	void DrawDebugGeometryOverlays();
	bool ShouldFollow();
	void BuildScheduleTestBits();

	void BeginScheduleSelection();

	void GatherConditions();
	void GatherConditionsNotActive();
	int SelectSchedule();
	int	FollowCallBaseSelectSchedule();
	void StartTask( const Task_t *pTask );
	void RunTask( const Task_t *pTask );

	void CheckBreakEscort();

	void OnDamage( const CTakeDamageInfo &info );
	static void DistributeFreeMiniStriders();
	static void FindFreeMiniStriders( CUtlVector<CNPC_MiniStrider *> *pFreeMiniStriders );

	float			m_flTimeEscortReturn;
	CSimpleSimTimer	m_FollowAttackTimer;
	bool			m_bEnabled;

	static float	gm_flLastDefendSound; // not saved and loaded, it's okay to yell again after a load

	//---------------------------------

	DECLARE_DATADESC();
};


BEGIN_DATADESC( CAI_MiniStriderEscortBehavior )
	DEFINE_FIELD( m_flTimeEscortReturn, FIELD_TIME ),
	DEFINE_EMBEDDED( m_FollowAttackTimer ),
	DEFINE_FIELD( m_bEnabled, FIELD_BOOLEAN ),
END_DATADESC();

float CAI_MiniStriderEscortBehavior::gm_flLastDefendSound;

//-----------------------------------------------------------------------------
// MiniStrider PHYSICS DAMAGE TABLE
//-----------------------------------------------------------------------------
#define MINISTRIDER_MIN_PHYSICS_DAMAGE 10

static impactentry_t s_MiniStriderLinearTable[] =
{
	{ 150*150, 75 },
	{ 350*350, 105 },
	{ 1000*1000, 300 },
};

static impactentry_t s_MiniStriderAngularTable[] =
{
	{ 100*100, 75 },
	{ 200*200, 105 },
	{ 300*300, 300 },
};

impactdamagetable_t s_MiniStriderImpactDamageTable =
{
	s_MiniStriderLinearTable,
	s_MiniStriderAngularTable,
	
	ARRAYSIZE(s_MiniStriderLinearTable),
	ARRAYSIZE(s_MiniStriderAngularTable),

	24*24,		// minimum linear speed squared
	360*360,	// minimum angular speed squared (360 deg/s to cause spin/slice damage)
	5,			// can't take damage from anything under 5kg

	10,			// anything less than 10kg is "small"
	MINISTRIDER_MIN_PHYSICS_DAMAGE,			// never take more than 10 pts of damage from anything under 10kg
	36*36,		// <10kg objects must go faster than 36 in/s to do damage

	VPHYSICS_LARGE_OBJECT_MASS,		// large mass in kg 
	4,			// large mass scale (anything over 500kg does 4X as much energy to read from damage table)
	5,			// large mass falling scale (emphasize falling/crushing damage over sideways impacts since the stress will kill you anyway)
	0.0f,		// min vel
};


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
class CNPC_MiniStrider : public CAI_BaseActor
{
	DECLARE_CLASS( CNPC_MiniStrider, CAI_BaseActor );

public:
	CNPC_MiniStrider();
	~CNPC_MiniStrider();

	//---------------------------------

	void			Precache();
	void			Spawn();
	void			PostNPCInit();
	void			Activate();
	void			UpdateOnRemove();
	void			OnRestore();
	bool			CreateBehaviors();
	void			IdleSound();
	bool			ShouldPlayIdleSound();
	bool			CanBecomeRagdoll();
	Activity		GetDeathActivity();
	void			StopLoopingSounds();

	const impactdamagetable_t &GetPhysicsImpactDamageTable();

	Class_T			Classify();
	Vector			BodyTarget( const Vector &posSrc, bool bNoisy /*= true*/ );

	int				DrawDebugTextOverlays();
	void			DrawDebugGeometryOverlays();

	void			UpdateEfficiency( bool bInPVS );

	//---------------------------------

	virtual Vector	GetNodeViewOffset()					{ return BaseClass::GetDefaultEyeOffset();		}

	int				GetSoundInterests();

	bool			IsInLargeOutdoorMap();

	//---------------------------------
	// CAI_BaseActor
	//---------------------------------
	const char *SelectRandomExpressionForState( NPC_STATE state );
	void PlayExpressionForState( NPC_STATE state );

	//---------------------------------
	// CBaseAnimating
	//---------------------------------
	float			GetIdealAccel() const { return GetIdealSpeed(); }

	//---------------------------------
	// Behavior
	//---------------------------------
	void			NPCThink();
	void			PrescheduleThink();
	void			GatherConditions();
	void			CollectSiegeTargets();
	void			ManageSiegeTargets();
	void			KillCurrentSiegeTarget();
	bool			QueryHearSound( CSound *pSound );
	void			OnSeeEntity( CBaseEntity *pEntity );
	void			CheckFlinches() {} // MiniStrider handles on own
	void			BuildScheduleTestBits();
	NPC_STATE		SelectIdealState();
	int				SelectSchedule();
	int				SelectCombatSchedule();
	int				SelectSiegeSchedule();
	int				TranslateSchedule( int scheduleType );
	void			StartTask( const Task_t *pTask );
	void			RunTask( const Task_t *pTask );
	Activity		NPC_TranslateActivity( Activity baseAct );
	void			OnChangeActivity( Activity eNewActivity );
	
	void			HandleAnimEvent( animevent_t *pEvent );
	bool			HandleInteraction(int interactionType, void *data, CBaseCombatCharacter *pSourceEnt);

	void			PlayerHasIlluminatedNPC( CBasePlayer *pPlayer, float flDot );

	void			AddEntityRelationship( CBaseEntity *pEntity, Disposition_t nDisposition, int nPriority );
	float			EnemyDistTolerance() { return 100.0f; }

	bool			ScheduledMoveToGoalEntity( int scheduleType, CBaseEntity *pGoalEntity, Activity movementActivity );

	void			OnChangeHintGroup( string_t oldGroup, string_t newGroup );

	bool			IsUsingSiegeTargets()	{ return m_iszSiegeTargetName != NULL_STRING; }
	
	//---------------------------------
	// Inputs
	//---------------------------------
	void			InputDodge( inputdata_t &inputdata );
	void			InputFlankEnemy( inputdata_t &inputdata );
	void			InputDisableShooting( inputdata_t &inputdata );
	void			InputEnableShooting( inputdata_t &inputdata );
	void			InputFollowStrider( inputdata_t &inputdata );
	void			InputUseSiegeTargets( inputdata_t &inputdata );
	void			InputEnableSquadShootDelay( inputdata_t &inputdata );
	void			InputDisableSquadShootDelay( inputdata_t &inputdata );
	void			InputEnableUnplantedShooting( inputdata_t &inputdata );
	void			InputDisableUnplantedShooting( inputdata_t &inputdata );

	//---------------------------------
	// Combat
	//---------------------------------
	bool			FVisible( CBaseEntity *pEntity, int traceMask = MASK_BLOCKLOS, CBaseEntity **ppBlocker = NULL );
	bool			IsValidEnemy( CBaseEntity *pEnemy );
	
	Disposition_t	IRelationType( CBaseEntity *pTarget );
	int				IRelationPriority( CBaseEntity *pTarget );

	void			SetSquad( CAI_Squad *pSquad );

	bool			UpdateEnemyMemory( CBaseEntity *pEnemy, const Vector &position, CBaseEntity *pInformer = NULL );

	int				RangeAttack1Conditions( float flDot, float flDist );
	int				RangeAttack2Conditions( float flDot, float flDist );

	int				MeleeAttack1Conditions ( float flDot, float flDist );
	int				MeleeAttack1ConditionsVsEnemyInVehicle( CBaseCombatCharacter *pEnemy, float flDot );

	int				MeleeAttack2Conditions( float flDot, float flDist );

	bool			WeaponLOSCondition(const Vector &ownerPos, const Vector &targetPos, bool bSetConditions);
	bool			TestShootPosition(const Vector &vecShootPos, const Vector &targetPos );

	Vector			Weapon_ShootPosition();

	CBaseEntity *	MeleeAttack( float flDist, int iDamage, QAngle &qaViewPunch, Vector &vecVelocityPunch, int BloodOrigin );

	void			MakeTracer( const Vector &vecTracerSrc, const trace_t &tr, int iTracerType );
	void			DoMuzzleFlash( int nAttachment );

	bool			CanShootThrough( const trace_t &tr, const Vector &vecTarget );

	int				CountRangedAttackers();
	void 			DelayRangedAttackers( float minDelay, float maxDelay, bool bForced = false );

	//---------------------------------
	//	Sounds & speech
	//---------------------------------
	void			AlertSound();
	void			PainSound( const CTakeDamageInfo &info );
	void			DeathSound( const CTakeDamageInfo &info );

	//---------------------------------
	// Damage handling
	//---------------------------------
	void			TraceAttack( const CTakeDamageInfo &info, const Vector &vecDir, trace_t *ptr, CDmgAccumulator *pAccumulator );
	bool			IsHeavyDamage( const CTakeDamageInfo &info );
	int				OnTakeDamage( const CTakeDamageInfo &info );
	int				OnTakeDamage_Alive( const CTakeDamageInfo &info );
	void			Event_Killed( const CTakeDamageInfo &info );

	void			StartBleeding();
	inline bool		IsBleeding() { return m_bIsBleeding; }
	void			Explode();

	void			SetupGlobalModelData();
	
	//---------------------------------
	// Navigation & Movement
	//---------------------------------
	bool			OverrideMoveFacing( const AILocalMoveGoal_t &move, float flInterval );
	float			MaxYawSpeed();
	bool			IsJumpLegal(const Vector &startPos, const Vector &apex, const Vector &endPos) const;
	float			GetJumpGravity() const		{ return 3.0f; }
	bool			ShouldProbeCollideAgainstEntity( CBaseEntity *pEntity );
	void            TaskFail( AI_TaskFailureCode_t code );
	void			TaskFail( const char *pszGeneralFailText )	{ TaskFail( MakeFailCode( pszGeneralFailText ) ); }

	CAI_BaseNPC *	GetEntity() { return this; }

	//---------------------------------
	// Magnade
	//---------------------------------	
	void	StriderBusterAttached( CBaseEntity *pAttached );
	void	StriderBusterDetached( CBaseEntity *pAttached );

private:

	void ConsiderFlinching( const CTakeDamageInfo &info );

	void TaskFindDodgeActivity();

	void GatherChargeConditions();
	void GatherIndoorOutdoorConditions();

	// Charge attack.
	bool ShouldCharge( const Vector &startPos, const Vector &endPos, bool useTime, bool bCheckForCancel );
	void ChargeLookAhead();
	float ChargeSteer();
	bool EnemyIsRightInFrontOfMe( CBaseEntity **pEntity );
	void ChargeDamage( CBaseEntity *pTarget );
	bool HandleChargeImpact( Vector vecImpact, CBaseEntity *pEntity );

	void BeginVolley( int nNum, float flStartTime );
	bool ShootFlechette( CBaseEntity *pTargetEntity, bool bSingleShot );
	bool ShouldSeekTarget( CBaseEntity *pTargetEntity, bool bStriderBuster );
	void GetShootDir( Vector &vecDir, const Vector &vecSrc, CBaseEntity *pTargetEntity, bool bStriderbuster, int nShotNum, bool bSingleShot );
	bool ClampShootDir( Vector &vecDir );

	void SetAim( const Vector &aimDir, float flInterval );
	void RelaxAim( float flInterval );
	void UpdateAim();
	void UpdateEyes();
	void LockBothEyes( float flDuration );
	void UnlockBothEyes( float flDuration );

	void TeslaThink();
	void BleedThink();
	void JostleVehicleThink();

	void FollowStrider( const char *szStrider );
	void FollowStrider( CNPC_Strider * pStrider );
	int NumMiniStridersInMySquad();

	bool CanPlantHere( const Vector &vecPos );

	//---------------------------------
	// Foot handling
	//---------------------------------
	Vector LeftFootHit( float eventtime );
	Vector RightFootHit( float eventtime );
	Vector BackFootHit( float eventtime );

	void FootFX( const Vector &origin );

	CBaseEntity *GetEnemyVehicle();
	bool IsCorporealEnemy( CBaseEntity *pEnemy );

	void PhysicsDamageEffect( const Vector &vecPos, const Vector &vecDir );
	bool PlayerFlashlightOnMyEyes( CBasePlayer *pPlayer );

	//-----------------------------------------------------
	// Conditions, Schedules, Tasks
	//-----------------------------------------------------
	enum 
	{
		SCHED_MINISTRIDER_RANGE_ATTACK1 = BaseClass::NEXT_SCHEDULE,
		SCHED_MINISTRIDER_RANGE_ATTACK2,
		SCHED_MINISTRIDER_MELEE_ATTACK1,
		SCHED_MINISTRIDER_DODGE,
		SCHED_MINISTRIDER_CHASE_ENEMY,
		SCHED_MINISTRIDER_CHASE_ENEMY_MELEE,
		SCHED_MINISTRIDER_COMBAT_FACE,
		SCHED_MINISTRIDER_FLANK_ENEMY,
		SCHED_MINISTRIDER_CHANGE_POSITION,
		SCHED_MINISTRIDER_CHANGE_POSITION_FINISH,
		SCHED_MINISTRIDER_SIDESTEP,
		SCHED_MINISTRIDER_PATROL,
		SCHED_MINISTRIDER_FLINCH_STICKYBOMB,
		SCHED_MINISTRIDER_STAGGER,
		SCHED_MINISTRIDER_PATROL_RUN,
		SCHED_MINISTRIDER_TAKE_COVER_FROM_ENEMY,
		SCHED_MINISTRIDER_HIDE_UNDER_COVER,
		SCHED_MINISTRIDER_FAIL_IMMEDIATE, // instant fail without waiting
		SCHED_MINISTRIDER_CHARGE_ENEMY,
		SCHED_MINISTRIDER_FAIL_CHARGE_ENEMY,
		SCHED_MINISTRIDER_FOUND_ENEMY,
		SCHED_MINISTRIDER_FOUND_ENEMY_ACK,
		SCHED_MINISTRIDER_RANGE_ATTACK2_VS_STRIDERBUSTER,
		SCHED_MINISTRIDER_RANGE_ATTACK2_VS_STRIDERBUSTER_LATENT,
		SCHED_MINISTRIDER_GOTO_HINT,
		SCHED_MINISTRIDER_CLEAR_HINTNODE,
		SCHED_MINISTRIDER_FAIL_DODGE,
		SCHED_MINISTRIDER_SIEGE_STAND,
		SCHED_MINISTRIDER_CHANGE_POSITION_SIEGE,

		TASK_MINISTRIDER_AIM = BaseClass::NEXT_TASK,
		TASK_MINISTRIDER_FIND_DODGE_POSITION,
		TASK_MINISTRIDER_DODGE,
		TASK_MINISTRIDER_PRE_RANGE_ATTACK2,
		TASK_MINISTRIDER_SHOOT_COMMIT,
		TASK_MINISTRIDER_BEGIN_FLANK,
		TASK_MINISTRIDER_ANNOUNCE_FLANK,
		TASK_MINISTRIDER_STAGGER,
		TASK_MINISTRIDER_CORNERED_TIMER,
		TASK_MINISTRIDER_FIND_SIDESTEP_POSITION,
		TASK_MINISTRIDER_CHARGE,
		TASK_MINISTRIDER_CHARGE_DELAY,
		TASK_MINISTRIDER_FINISH_RANGE_ATTACK,
		TASK_MINISTRIDER_WAIT_FOR_MOVEMENT_FACING_ENEMY,

		COND_MINISTRIDER_SHOULD_PATROL = BaseClass::NEXT_CONDITION,
		COND_MINISTRIDER_FORCED_FLANK_ENEMY,
		COND_MINISTRIDER_FORCED_DODGE,
		COND_MINISTRIDER_CAN_CHARGE_ENEMY,
		COND_MINISTRIDER_HIT_BY_STICKYBOMB,
		COND_MINISTRIDER_STAGGERED,
		COND_MINISTRIDER_IS_INDOORS,
		COND_MINISTRIDER_SEE_STRIDERBUSTER,
		COND_MINISTRIDER_INCOMING_VEHICLE,
		COND_MINISTRIDER_NEW_HINTGROUP,
		COND_MINISTRIDER_CANT_PLANT,
		COND_MINISTRIDER_SQUADMATE_FOUND_ENEMY,
	};

	enum MiniStriderEyeStates_t
	{
		MINISTRIDER_EYE_STATE_TOP_LOCKED = 0,
		MINISTRIDER_EYE_STATE_BOTTOM_LOCKED,
		MINISTRIDER_EYE_STATE_BOTH_LOCKED,
		MINISTRIDER_EYE_STATE_BOTH_UNLOCKED,
	};

	string_t		m_iszFollowTarget;		// Name of the strider we should follow.
	CSimpleStopwatch m_BeginFollowDelay;

	int				m_nKillingDamageType;
	MiniStriderEyeStates_t m_eEyeState;

	float			m_aimYaw;
	float			m_aimPitch;

	float			m_flShootAllowInterruptTime;
	float			m_flNextChargeTime;				// Prevents us from doing our threat display too often.
	float			m_flNextDamageTime;
	float			m_flNextSideStepTime;

	CSimpleSimTimer m_HeavyDamageDelay;
	CSimpleSimTimer m_FlinchTimer;
	CSimpleSimTimer m_EyeSwitchTimer;		// Controls how often we switch which eye is focusing on our enemy.

	bool			m_bTopMuzzle;	// Used to alternate between top muzzle FX and bottom muzzle FX.
	bool			m_bEnableSquadShootDelay;
	bool			m_bIsBleeding;

	Activity		m_eDodgeActivity;
	CSimpleSimTimer m_RundownDelay;
	CSimpleSimTimer m_IgnoreVehicleTimer;

	bool m_bDisableShooting;	// Range attack disabled via an input. Used for scripting melee attacks.
	
	bool m_bFlashlightInEyes;	// The player is shining the flashlight on our eyes.
	float m_flPupilDilateTime;	// When to dilate our pupils if the flashlight is no longer on our eyes.

	Vector m_vecEnemyLastSeen;
	Vector m_vecLastCanPlantHerePos;
	Vector m_vecStaggerDir;

	bool m_bPlanted;
	bool m_bLastCanPlantHere;
	bool m_bMissLeft;
	bool m_bEnableUnplantedShooting;

	static float	gm_flMinigunDistZ;
	static Vector	gm_vecLocalRelativePositionMinigun;

	static int gm_nTopGunAttachment;
	static int gm_nBottomGunAttachment;
	static int gm_nAimYawPoseParam;
	static int gm_nAimPitchPoseParam;
	static int gm_nBodyYawPoseParam;
	static int gm_nBodyPitchPoseParam;
	static int gm_nStaggerYawPoseParam;
	static int gm_nHeadCenterAttachment;
	static int gm_nHeadBottomAttachment;
	static float gm_flHeadRadius;

	static int gm_nUnplantedNode;
	static int gm_nPlantedNode;

	CAI_MiniStriderEscortBehavior m_EscortBehavior;

	int m_nFlechettesQueued;
	int m_nClampedShots;				// The number of consecutive shots fired at an out-of-max yaw target.

	float m_flNextRangeAttack2Time;		// Time when we can fire another volley of ar2rounds.
	float m_flNextFlechetteTime;		// Time to fire the next ar2round in this volley.
	
	float m_flNextMeleeTime;
	float m_flTeslaStopTime;

	string_t m_iszCurrentExpression;

	// buster fu	
	CUtlVector< EHANDLE >	m_hAttachedBusters;		// List of busters attached to us
	float m_fCorneredTimer; ///< ministrider was cornered when fleeing player; it won't flee again until this time

	CSimpleSimTimer m_CheckHintGroupTimer;

	DEFINE_CUSTOM_AI;

	DECLARE_DATADESC();

	friend class CAI_MiniStriderEscortBehavior;
	friend class CMiniStriderMaker;

	bool	m_bInLargeOutdoorMap;
	float	m_flTimeSawEnemyAgain;
	bool	m_bIsLaughing;

	// Sounds
	//CSoundPatch	*m_pGunFiringSound;

	CUtlVector<EHANDLE> m_pSiegeTargets;
	string_t	m_iszSiegeTargetName;
	float		m_flTimeNextSiegeTargetAttack;
	EHANDLE		m_hCurrentSiegeTarget;
	
	EHANDLE		m_hHitByVehicle;
};


LINK_ENTITY_TO_CLASS( npc_ministrider, CNPC_MiniStrider );


BEGIN_DATADESC( CNPC_MiniStrider )

	DEFINE_KEYFIELD( m_iszFollowTarget, FIELD_STRING, "FollowTarget" ),

	DEFINE_FIELD( m_aimYaw,				FIELD_FLOAT ),
	DEFINE_FIELD( m_aimPitch,				FIELD_FLOAT ),

	DEFINE_FIELD( m_flShootAllowInterruptTime, FIELD_TIME ),
	DEFINE_FIELD( m_flNextChargeTime, FIELD_TIME ),
	//DEFINE_FIELD( m_flNextDamageTime, FIELD_TIME ),
	DEFINE_FIELD( m_flNextSideStepTime, FIELD_TIME ),

	DEFINE_EMBEDDED( m_HeavyDamageDelay ),
	DEFINE_EMBEDDED( m_FlinchTimer ),

	DEFINE_FIELD( m_eEyeState, FIELD_INTEGER ),

	DEFINE_FIELD( m_bTopMuzzle, FIELD_BOOLEAN ),
	DEFINE_FIELD( m_bEnableSquadShootDelay, FIELD_BOOLEAN ),
	DEFINE_FIELD( m_bIsBleeding, FIELD_BOOLEAN ),

	DEFINE_FIELD( m_bDisableShooting, FIELD_BOOLEAN ),
	DEFINE_FIELD( m_bFlashlightInEyes, FIELD_BOOLEAN ),
	DEFINE_FIELD( m_flPupilDilateTime, FIELD_TIME ),

	DEFINE_FIELD( m_vecEnemyLastSeen, FIELD_POSITION_VECTOR ),
	DEFINE_FIELD( m_vecLastCanPlantHerePos, FIELD_POSITION_VECTOR ),
	DEFINE_FIELD( m_vecStaggerDir, FIELD_VECTOR ),
	
	DEFINE_FIELD( m_bPlanted, FIELD_BOOLEAN ),
	DEFINE_FIELD( m_bLastCanPlantHere, FIELD_BOOLEAN ),
	//DEFINE_FIELD( m_bMissLeft, FIELD_BOOLEAN ),
	DEFINE_FIELD( m_bEnableUnplantedShooting, FIELD_BOOLEAN ),

	DEFINE_FIELD( m_nKillingDamageType, FIELD_INTEGER ),
	DEFINE_FIELD( m_eDodgeActivity, FIELD_INTEGER ),
	DEFINE_EMBEDDED( m_RundownDelay ),
	DEFINE_EMBEDDED( m_IgnoreVehicleTimer ),

	DEFINE_FIELD( m_flNextMeleeTime, FIELD_TIME ),
	DEFINE_FIELD( m_flTeslaStopTime, FIELD_TIME ),

	// (auto saved by AI)
	//DEFINE_FIELD( m_EscortBehavior, FIELD_EMBEDDED ),	

	DEFINE_FIELD( m_iszCurrentExpression, FIELD_STRING ),

	DEFINE_FIELD( m_fCorneredTimer, FIELD_TIME),

	DEFINE_EMBEDDED( m_CheckHintGroupTimer ),

	// (Recomputed in Precache())
	//DEFINE_FIELD( m_bInLargeOutdoorMap, FIELD_BOOLEAN ), 
	DEFINE_FIELD( m_flTimeSawEnemyAgain, FIELD_TIME ),

	//DEFINE_SOUNDPATCH( m_pGunFiringSound ),

	//DEFINE_UTLVECTOR( m_pSiegeTarget, FIELD_EHANDLE ),
	DEFINE_FIELD( m_iszSiegeTargetName, FIELD_STRING ),
	DEFINE_FIELD( m_flTimeNextSiegeTargetAttack, FIELD_TIME ),
	DEFINE_FIELD( m_hCurrentSiegeTarget, FIELD_EHANDLE ),
	DEFINE_FIELD( m_hHitByVehicle, FIELD_EHANDLE ),

	DEFINE_EMBEDDED( m_BeginFollowDelay ),
	DEFINE_EMBEDDED( m_EyeSwitchTimer ),

	DEFINE_FIELD( m_nFlechettesQueued, FIELD_INTEGER ),
	DEFINE_FIELD( m_nClampedShots, FIELD_INTEGER ),
	DEFINE_FIELD( m_flNextRangeAttack2Time, FIELD_TIME ),
	DEFINE_FIELD( m_flNextFlechetteTime, FIELD_TIME ),
	DEFINE_UTLVECTOR( m_hAttachedBusters, FIELD_EHANDLE ),
	DEFINE_UTLVECTOR( m_pSiegeTargets, FIELD_EHANDLE ),

	// inputs
	DEFINE_INPUTFUNC( FIELD_VOID, "Dodge", InputDodge ),
	DEFINE_INPUTFUNC( FIELD_VOID, "FlankEnemy", InputFlankEnemy ),
	DEFINE_INPUTFUNC( FIELD_STRING, "DisableShooting", InputDisableShooting ),
	DEFINE_INPUTFUNC( FIELD_STRING, "EnableShooting", InputEnableShooting ),
	DEFINE_INPUTFUNC( FIELD_STRING, "FollowStrider", InputFollowStrider ),
	DEFINE_INPUTFUNC( FIELD_STRING, "UseSiegeTargets", InputUseSiegeTargets ),
	DEFINE_INPUTFUNC( FIELD_VOID, "EnableSquadShootDelay", InputEnableSquadShootDelay ),
	DEFINE_INPUTFUNC( FIELD_VOID, "DisableSquadShootDelay", InputDisableSquadShootDelay ),
	DEFINE_INPUTFUNC( FIELD_VOID, "EnableUnplantedShooting", InputEnableUnplantedShooting ),
	DEFINE_INPUTFUNC( FIELD_VOID, "DisableUnplantedShooting", InputDisableUnplantedShooting ),

	// Function Pointers
	DEFINE_THINKFUNC( TeslaThink ),
	DEFINE_THINKFUNC( BleedThink ),
	DEFINE_THINKFUNC( JostleVehicleThink ),

END_DATADESC()

//-----------------------------------------------------------------------------

int CNPC_MiniStrider::gm_nUnplantedNode = 0;
int CNPC_MiniStrider::gm_nPlantedNode = 0;

int CNPC_MiniStrider::gm_nAimYawPoseParam = -1;
int CNPC_MiniStrider::gm_nAimPitchPoseParam = -1;
int CNPC_MiniStrider::gm_nBodyYawPoseParam = -1;
int CNPC_MiniStrider::gm_nBodyPitchPoseParam = -1;
int CNPC_MiniStrider::gm_nStaggerYawPoseParam = -1;
int CNPC_MiniStrider::gm_nHeadCenterAttachment = -1;
int CNPC_MiniStrider::gm_nHeadBottomAttachment = -1;
float CNPC_MiniStrider::gm_flHeadRadius = 0;

int CNPC_MiniStrider::gm_nTopGunAttachment = -1;
int CNPC_MiniStrider::gm_nBottomGunAttachment = -1;

float CNPC_MiniStrider::gm_flMinigunDistZ;
Vector CNPC_MiniStrider::gm_vecLocalRelativePositionMinigun;

//-----------------------------------------------------------------------------

static CUtlVector<CNPC_MiniStrider *> g_MiniStriders;
float g_TimeLastDistributeFreeMiniStriders = -1;
const float FREE_MINISTRIDER_DISTRIBUTE_INTERVAL = 2;

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
CNPC_MiniStrider::CNPC_MiniStrider()
{
	g_MiniStriders.AddToTail( this );
	g_TimeLastDistributeFreeMiniStriders = -1;
	m_flTimeSawEnemyAgain = MINISTRIDER_SEE_ENEMY_TIME_INVALID;
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
CNPC_MiniStrider::~CNPC_MiniStrider()
{
	g_MiniStriders.FindAndRemove( this );
	g_TimeLastDistributeFreeMiniStriders = -1;
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CNPC_MiniStrider::Precache()
{
	PrecacheModel( "models/ministrider.mdl" );
	PropBreakablePrecacheAll( MAKE_STRING("models/ministrider.mdl") );

	PrecacheScriptSound("NPC_Hunter.Idle");
	PrecacheScriptSound("NPC_Hunter.Scan");
	PrecacheScriptSound("NPC_Hunter.Alert");
	PrecacheScriptSound("NPC_Hunter.Pain");
	PrecacheScriptSound("NPC_Hunter.PreCharge");
	PrecacheScriptSound("NPC_Hunter.Angry");
	PrecacheScriptSound("NPC_Hunter.Death");
	PrecacheScriptSound("NPC_Hunter.FireMinigun");
	PrecacheScriptSound("NPC_Hunter.Footstep");
	PrecacheScriptSound("NPC_Hunter.BackFootstep");
	PrecacheScriptSound("NPC_Hunter.FlechetteVolleyWarn");
	PrecacheScriptSound("NPC_Hunter.FlechetteShoot");
	PrecacheScriptSound("NPC_Hunter.FlechetteShootLoop");
	PrecacheScriptSound("NPC_Hunter.FlankAnnounce");
	PrecacheScriptSound("NPC_Hunter.MeleeAnnounce");
	PrecacheScriptSound("NPC_Hunter.MeleeHit");
	PrecacheScriptSound("NPC_Hunter.TackleAnnounce");
	PrecacheScriptSound("NPC_Hunter.TackleHit");
	PrecacheScriptSound("NPC_Hunter.ChargeHitEnemy");
	PrecacheScriptSound("NPC_Hunter.ChargeHitWorld");
	PrecacheScriptSound("NPC_Hunter.FoundEnemy");
	PrecacheScriptSound("NPC_Hunter.FoundEnemyAck");
	PrecacheScriptSound("NPC_Hunter.DefendStrider");
	PrecacheScriptSound("NPC_Hunter.HitByVehicle");

	PrecacheParticleSystem( "ministrider_muzzle_flash" );
	PrecacheParticleSystem( "blood_impact_synth_01" );
	PrecacheParticleSystem( "blood_impact_synth_01_arc_parent" );
	PrecacheParticleSystem( "blood_spurt_synth_01" );
	PrecacheParticleSystem( "blood_drip_synth_01" );

	PrecacheInstancedScene("scenes/npc/hunter/hunter_scan.vcd");
	PrecacheInstancedScene("scenes/npc/hunter/hunter_eyeclose.vcd");
	PrecacheInstancedScene("scenes/npc/hunter/hunter_roar.vcd");
	PrecacheInstancedScene("scenes/npc/hunter/hunter_pain.vcd");
	PrecacheInstancedScene("scenes/npc/hunter/hunter_eyedarts_top.vcd");
	PrecacheInstancedScene("scenes/npc/hunter/hunter_eyedarts_bottom.vcd");
	
	PrecacheMaterial( "effects/water_highlight" );
	
	UTIL_PrecacheOther( "sparktrail" );

	m_bInLargeOutdoorMap = false;
	if( !Q_strnicmp( STRING(gpGlobals->mapname), "ep2_outland_12", 14) )
	{
		m_bInLargeOutdoorMap = true;
	}
	m_bIsLaughing = false;

	BaseClass::Precache();
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CNPC_MiniStrider::Spawn()
{
	Precache();

	SetModel( "models/ministrider.mdl" );
	BaseClass::Spawn();

	//m_debugOverlays |= OVERLAY_NPC_ROUTE_BIT | OVERLAY_BBOX_BIT | OVERLAY_PIVOT_BIT;

	SetHullType( HULL_MEDIUM_TALL );
	SetHullSizeNormal();
	SetDefaultEyeOffset();
	
	SetNavType( NAV_GROUND );
	m_flGroundSpeed	= 500;
	m_NPCState = NPC_STATE_NONE;

	SetBloodColor( DONT_BLEED );
	
	m_iHealth = m_iMaxHealth = sk_ministrider_health.GetInt();

	m_flFieldOfView = MINISTRIDER_FOV_DOT;

	SetSolid( SOLID_BBOX );
	AddSolidFlags( FSOLID_NOT_STANDABLE );
	SetMoveType( MOVETYPE_STEP );

	SetupGlobalModelData();
	
	CapabilitiesAdd( bits_CAP_MOVE_GROUND | bits_CAP_SQUAD | bits_CAP_ANIMATEDFACE );
	CapabilitiesAdd( bits_CAP_INNATE_RANGE_ATTACK1 | bits_CAP_INNATE_RANGE_ATTACK2 | bits_CAP_INNATE_MELEE_ATTACK1 );
	CapabilitiesAdd( bits_CAP_SKIP_NAV_GROUND_CHECK );

	if ( !ministrider_allow_dissolve.GetBool() )
	{
		AddEFlags( EFL_NO_DISSOLVE );
	}

	if( ministrider_allow_nav_jump.GetBool() )
	{
		CapabilitiesAdd( bits_CAP_MOVE_JUMP );
	}

	NPCInit();

	m_bEnableSquadShootDelay = true;

	m_flDistTooFar = ministrider_ar2round_max_range.GetFloat();

	// Discard time must be greater than free knowledge duration. Make it double.
	float freeKnowledge = ministrider_free_knowledge.GetFloat();
	if ( freeKnowledge < GetEnemies()->GetEnemyDiscardTime() )
	{
		GetEnemies()->SetEnemyDiscardTime( MAX( freeKnowledge + 0.1, AI_DEF_ENEMY_DISCARD_TIME ) );
	}
	GetEnemies()->SetFreeKnowledgeDuration( freeKnowledge );

	// Find out what strider we should follow, if any.
	if ( m_iszFollowTarget != NULL_STRING )
	{
		m_BeginFollowDelay.Set( .1 ); // Allow time for strider to spawn
	}
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CNPC_MiniStrider::UpdateEfficiency( bool bInPVS )
{
	SetEfficiency( ( GetSleepState() != AISS_AWAKE ) ? AIE_DORMANT : AIE_NORMAL );
	SetMoveEfficiency( AIME_NORMAL );
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
bool CNPC_MiniStrider::CreateBehaviors()
{
	AddBehavior( &m_EscortBehavior );

	return BaseClass::CreateBehaviors();
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CNPC_MiniStrider::SetupGlobalModelData()
{
	if ( gm_nBodyYawPoseParam != -1 )
		return;

	gm_nAimYawPoseParam = LookupPoseParameter( "aim_yaw" );
	gm_nAimPitchPoseParam = LookupPoseParameter( "aim_pitch" );

	gm_nBodyYawPoseParam = LookupPoseParameter( "body_yaw" );
	gm_nBodyPitchPoseParam = LookupPoseParameter( "body_pitch" );

	gm_nTopGunAttachment = LookupAttachment( "top_eye" );
	gm_nBottomGunAttachment = LookupAttachment( "bottom_eye" );
	gm_nStaggerYawPoseParam = LookupAttachment( "stagger_yaw" );
	
	gm_nHeadCenterAttachment = LookupAttachment( "head_center" );
	gm_nHeadBottomAttachment = LookupAttachment( "head_radius_measure" );

	// Measure the radius of the head.	
	Vector vecHeadCenter;
	Vector vecHeadBottom;
	GetAttachment( gm_nHeadCenterAttachment, vecHeadCenter );
	GetAttachment( gm_nHeadBottomAttachment, vecHeadBottom );
	gm_flHeadRadius = ( vecHeadCenter - vecHeadBottom ).Length();

	int nSequence = SelectWeightedSequence( ACT_MINTER_RANGE_ATTACK2_UNPLANTED );
	gm_nUnplantedNode = GetEntryNode( nSequence );

	nSequence = SelectWeightedSequence( ACT_RANGE_ATTACK2 );
	gm_nPlantedNode = GetEntryNode( nSequence );

	CollisionProp()->SetSurroundingBoundsType( USE_HITBOXES );
}


//-----------------------------------------------------------------------------
// Shuts down looping sounds when we are killed in combat or deleted.
//-----------------------------------------------------------------------------
void CNPC_MiniStrider::StopLoopingSounds()
{
	BaseClass::StopLoopingSounds();

	//if ( m_pGunFiringSound )
	//{
	//	CSoundEnvelopeController &controller = CSoundEnvelopeController::GetController();
	//	controller.SoundDestroy( m_pGunFiringSound );
	//	m_pGunFiringSound = NULL;
	//}
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CNPC_MiniStrider::OnRestore()
{
	BaseClass::OnRestore();
	SetupGlobalModelData();
	CreateVPhysics();
	
	if ( IsBleeding() )
	{
		StartBleeding();
	}
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CNPC_MiniStrider::IdleSound()
{
	if ( HasCondition( COND_LOST_ENEMY ) )
	{
		EmitSound("NPC_Hunter.Scan");
	}
	else
	{
		EmitSound("NPC_Hunter.Idle");
	}
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
bool CNPC_MiniStrider::ShouldPlayIdleSound()
{
	if (random->RandomInt(0, 99) == 0 && !HasSpawnFlags(SF_NPC_GAG) && !m_bIsLaughing)
		return true;
	
	return false;
}


//-----------------------------------------------------------------------------
// Stay facing our enemy when close enough.
//-----------------------------------------------------------------------------
bool CNPC_MiniStrider::OverrideMoveFacing( const AILocalMoveGoal_t &move, float flInterval )
{
	if ( GetActivity() == ACT_TRANSITION )
	{
		// No turning while in transitions.
		return true;
	}

 	bool bSideStepping = IsCurSchedule( SCHED_MINISTRIDER_SIDESTEP, false );
 	
  	// FIXME: this will break scripted sequences that walk when they have an enemy
  	if ( GetEnemy() &&
  		( bSideStepping ||
  		( ( ( GetNavigator()->GetMovementActivity() == ACT_RUN ) || ( GetNavigator()->GetMovementActivity() == ACT_WALK ) ) &&
		  !IsCurSchedule( SCHED_MINISTRIDER_TAKE_COVER_FROM_ENEMY, false ) ) ) )
	{
		Vector vecEnemyLKP = GetEnemyLKP();
		
		// Face my enemy if we're close enough
		if ( bSideStepping || UTIL_DistApprox( vecEnemyLKP, GetAbsOrigin() ) < MINISTRIDER_FACE_ENEMY_DIST )
		{
			AddFacingTarget( GetEnemy(), vecEnemyLKP, 1.0, 0.2 );
		}
	}

	return BaseClass::OverrideMoveFacing( move, flInterval );
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CNPC_MiniStrider::PostNPCInit()
{
	BaseClass::PostNPCInit();

	IPhysicsObject *pPhysObject = VPhysicsGetObject();
	Assert( pPhysObject );
	if ( pPhysObject )
	{
		pPhysObject->SetMass( 600.0 );
	}
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CNPC_MiniStrider::Activate()
{
	BaseClass::Activate();

	s_iszStriderBusterClassname = AllocPooledString( "weapon_striderbuster" );
	s_iszStriderClassname  = AllocPooledString( "npc_strider" );
	s_iszMagnadeClassname = AllocPooledString( "npc_grenade_magna" );
	s_iszPhysPropClassname = AllocPooledString( "prop_physics" );
	s_iszMiniStridersToRunOver = AllocPooledString( "ministriders_to_run_over" );
	
	// If no one has initialized the ministriders to run over counter, just zero it out.
	if ( !GlobalEntity_IsInTable( s_iszMiniStridersToRunOver ) )
	{
		GlobalEntity_Add( s_iszMiniStridersToRunOver, gpGlobals->mapname, GLOBAL_ON );
		GlobalEntity_SetCounter( s_iszMiniStridersToRunOver, 0 );
	}

	CMissile::AddCustomDetonator( this, ( GetHullMaxs().AsVector2D() - GetHullMins().AsVector2D() ).Length() * 0.5, GetHullHeight() );

	SetupGlobalModelData();
	
	if ( gm_flMinigunDistZ == 0 )
	{
		// Have to create a virgin ministrider to ensure proper pose
		CNPC_MiniStrider *pMiniStrider = (CNPC_MiniStrider *)CreateEntityByName( "npc_ministrider" );
		Assert(pMiniStrider);
		pMiniStrider->Spawn();

		pMiniStrider->SetActivity( ACT_WALK );
		pMiniStrider->InvalidateBoneCache();

		// Currently just using the gun for the vertical component!
		Vector defEyePos;
		pMiniStrider->GetAttachment( "minigunbase", defEyePos );
		gm_flMinigunDistZ = defEyePos.z - pMiniStrider->GetAbsOrigin().z;

		Vector position;
		pMiniStrider->GetAttachment( gm_nTopGunAttachment, position );
		VectorITransform( position, pMiniStrider->EntityToWorldTransform(), gm_vecLocalRelativePositionMinigun );
		UTIL_Remove( pMiniStrider );
	}
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CNPC_MiniStrider::UpdateOnRemove()
{
	CMissile::RemoveCustomDetonator( this );
	BaseClass::UpdateOnRemove();
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
Class_T CNPC_MiniStrider::Classify()
{
	return CLASS_COMBINE_HUNTER;
}

//-----------------------------------------------------------------------------
// Compensate for the ministrider's long legs by moving the bodytarget up to his head.
//-----------------------------------------------------------------------------
Vector CNPC_MiniStrider::BodyTarget( const Vector &posSrc, bool bNoisy /*= true*/ )
{ 
	Vector vecResult;
	QAngle vecAngle;
	GetAttachment( gm_nHeadCenterAttachment, vecResult, vecAngle );

	if ( bNoisy )
	{
		float rand1 = random->RandomFloat( 0, gm_flHeadRadius ) + random->RandomFloat( 0, gm_flHeadRadius );
		return vecResult + RandomVector( -rand1, rand1 );
	}

	return vecResult;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
int	CNPC_MiniStrider::DrawDebugTextOverlays()
{
	int text_offset = BaseClass::DrawDebugTextOverlays();

	if (m_debugOverlays & OVERLAY_TEXT_BIT)
	{
		EntityText( text_offset, CFmtStr("%s", m_bPlanted ? "Planted" : "Unplanted" ), 0 );
		text_offset++;

		EntityText( text_offset, CFmtStr("Eye state: %d", m_eEyeState ), 0 );
		text_offset++;

		if( IsUsingSiegeTargets() )
		{
			EntityText( text_offset, CFmtStr("Next Siege Attempt:%f", m_flTimeNextSiegeTargetAttack - gpGlobals->curtime ), 0 );
			text_offset++;
		}
	}

	return text_offset;
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CNPC_MiniStrider::LockBothEyes( float flDuration )
{
	m_eEyeState = MINISTRIDER_EYE_STATE_BOTH_LOCKED;
	m_EyeSwitchTimer.Set( flDuration );
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CNPC_MiniStrider::UnlockBothEyes( float flDuration )
{
	m_eEyeState = MINISTRIDER_EYE_STATE_BOTH_UNLOCKED;
	m_EyeSwitchTimer.Set( flDuration );
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CNPC_MiniStrider::OnChangeActivity( Activity eNewActivity )
{
	m_EyeSwitchTimer.Force();
	
	BaseClass::OnChangeActivity( eNewActivity );
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CNPC_MiniStrider::UpdateEyes()
{
	// If the eyes are controlled by a script, do nothing.
	if ( GetState() == NPC_STATE_SCRIPT )
		return;

	if ( m_EyeSwitchTimer.Expired() )
	{
		RemoveActorFromScriptedScenes( this, false );
		
		if ( GetActivity() == ACT_IDLE )
		{
			// Idles have eye motion baked in.
			m_eEyeState = MINISTRIDER_EYE_STATE_BOTH_LOCKED;
		}
		else if ( GetEnemy() == NULL )
		{
			m_eEyeState = MINISTRIDER_EYE_STATE_BOTH_UNLOCKED;
		}
		else if ( m_eEyeState == MINISTRIDER_EYE_STATE_BOTH_LOCKED )
		{
			if ( random->RandomInt( 0, 1 ) == 0 )
			{
				m_eEyeState = MINISTRIDER_EYE_STATE_TOP_LOCKED;
			}
			else
			{
				m_eEyeState = MINISTRIDER_EYE_STATE_BOTTOM_LOCKED;
			}
		}
		else if ( m_eEyeState == MINISTRIDER_EYE_STATE_TOP_LOCKED )
		{
			m_eEyeState = MINISTRIDER_EYE_STATE_BOTTOM_LOCKED;
		}
		else if ( m_eEyeState == MINISTRIDER_EYE_STATE_BOTTOM_LOCKED )
		{
			m_eEyeState = MINISTRIDER_EYE_STATE_TOP_LOCKED;
		}

		if ( ( m_eEyeState == MINISTRIDER_EYE_STATE_BOTTOM_LOCKED ) || ( m_eEyeState == MINISTRIDER_EYE_STATE_BOTH_UNLOCKED ) )
		{
			SetExpression("scenes/npc/hunter/hunter_eyedarts_top.vcd");
		}

		if ( ( m_eEyeState == MINISTRIDER_EYE_STATE_TOP_LOCKED ) || ( m_eEyeState == MINISTRIDER_EYE_STATE_BOTH_UNLOCKED ) )
		{
			SetExpression("scenes/npc/hunter/hunter_eyedarts_bottom.vcd");
		}

		m_EyeSwitchTimer.Set( random->RandomFloat( 1.0f, 3.0f ) );
	}

	/*Vector vecEyePos;
	Vector vecEyeDir;

	GetAttachment( gm_nTopGunAttachment, vecEyePos, &vecEyeDir );
	NDebugOverlay::Line( vecEyePos, vecEyePos + vecEyeDir * 36, 255, 0, 0, 0, 0.1 );

	GetAttachment( gm_nBottomGunAttachment, vecEyePos, &vecEyeDir );
	NDebugOverlay::Line( vecEyePos, vecEyePos + vecEyeDir * 36, 255, 0, 0, 0, 0.1 );*/
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CNPC_MiniStrider::NPCThink()
{
	BaseClass::NPCThink();

	// Update our planted/unplanted state.
	m_bPlanted = ( GetEntryNode( GetSequence() ) == gm_nPlantedNode );

	UpdateAim();
	UpdateEyes();

	CBasePlayer *pPlayer = UTIL_PlayerByIndex(1);

	if (hunter_laugh.GetBool() && !pPlayer->IsDead() && !m_bIsLaughing)
	{
		if (random->RandomInt(0, hunter_laugh_frequency.GetInt()) == 0 && pPlayer->GetHealth() < hunter_laugh_healthvalue.GetInt() && !m_bIsLaughing)
		{
			//laugh at the player's own stupidity while idle.
			m_bIsLaughing = true;
			EmitSound("NPC_Hunter.Gloat");
		}
		else
		{
			m_bIsLaughing = false;
		}
	}
	else
	{
		m_bIsLaughing = false;
	}
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CNPC_MiniStrider::PrescheduleThink()
{
	BaseClass::PrescheduleThink();
	
	if ( m_BeginFollowDelay.Expired() )
	{
		FollowStrider( STRING( m_iszFollowTarget ) );
		m_BeginFollowDelay.Stop();
	}

	m_EscortBehavior.CheckBreakEscort();

	// If we're being blinded by the flashlight, see if we should stop
	if ( m_bFlashlightInEyes )
	{
		if ( m_flPupilDilateTime < gpGlobals->curtime )
		{
 			CBasePlayer *pPlayer = UTIL_PlayerByIndex( 1 );
 			if ( ( pPlayer && !pPlayer->IsIlluminatedByFlashlight( this, NULL ) ) || !PlayerFlashlightOnMyEyes( pPlayer ) )
			{
				//Msg( "NOT SHINING FLASHLIGHT ON ME\n" );
			
				// Remove the actor from the flashlight scene
				RemoveActorFromScriptedScenes(this, true, false, "scenes/npc/hunter/hunter_eyeclose.vcd");
				m_bFlashlightInEyes = false;
			}
		}
	}
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CNPC_MiniStrider::GatherChargeConditions()
{
	ClearCondition( COND_MINISTRIDER_CAN_CHARGE_ENEMY );

	if ( !ministrider_charge.GetBool() )
		return;
		
	if ( !GetEnemy() )
		return;

	if ( GetHintGroup() != NULL_STRING )
		return;
	
	if ( !HasCondition( COND_SEE_ENEMY ) )
		return;
	
	if ( !ministrider_charge_test.GetBool() && gpGlobals->curtime < m_flNextChargeTime )
		return;

	// No charging Alyx or Barney
	if( GetEnemy()->Classify() == CLASS_PLAYER_ALLY_VITAL )
		return;

	if ( m_EscortBehavior.GetEscortTarget() && GetEnemy()->MyCombatCharacterPointer() && !GetEnemy()->MyCombatCharacterPointer()->FInViewCone( this ) )
		return;

	if ( ShouldCharge( GetAbsOrigin(), GetEnemy()->GetAbsOrigin(), true, false ) )
	{
		SetCondition( COND_MINISTRIDER_CAN_CHARGE_ENEMY );
	}
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CNPC_MiniStrider::GatherConditions()
{
	GatherIndoorOutdoorConditions();
	GatherChargeConditions();

	BaseClass::GatherConditions();

	// Enemy LKP that doesn't get updated by the free knowledge code.
	// Used for shooting at where our enemy was when we can't see them.
	ClearCondition( COND_MINISTRIDER_INCOMING_VEHICLE );
	if ( m_IgnoreVehicleTimer.Expired() && GetEnemy() && HasCondition( COND_SEE_ENEMY ) )
	{
		CBaseEntity *pVehicle = GetEnemyVehicle();
		if ( ( pVehicle ) && ( GlobalEntity_GetCounter( s_iszMiniStridersToRunOver ) <= 0 ) )
		{
			static float timeDrawnArrow;

			// Extrapolate the position of the vehicle and see if it's heading toward us.
			float predictTime = ministrider_dodge_warning.GetFloat();
			Vector2D vecFuturePos = pVehicle->GetAbsOrigin().AsVector2D() + pVehicle->GetSmoothedVelocity().AsVector2D() * predictTime;
			if ( pVehicle->GetSmoothedVelocity().LengthSqr() > Square( 200 ) )
			{
				float t = 0;
				Vector2D vDirMovement = pVehicle->GetSmoothedVelocity().AsVector2D();
				if ( ministrider_dodge_debug.GetBool() )
				{
					NDebugOverlay::Line( pVehicle->GetAbsOrigin(), pVehicle->GetAbsOrigin() + pVehicle->GetSmoothedVelocity(), 255, 255, 255, true, .1 );
				}
				vDirMovement.NormalizeInPlace();
				Vector2D vDirToMiniStrider = GetAbsOrigin().AsVector2D() - pVehicle->GetAbsOrigin().AsVector2D();
				vDirToMiniStrider.NormalizeInPlace();
				if ( DotProduct2D( vDirMovement, vDirToMiniStrider ) > ministrider_dodge_warning_cone.GetFloat() && 
					 CalcDistanceSqrToLine2D( GetAbsOrigin().AsVector2D(), pVehicle->GetAbsOrigin().AsVector2D(), vecFuturePos, &t ) < Square( ministrider_dodge_warning_width.GetFloat() * .5 ) && 
					 t > 0.0 && t < 1.0 )
				{
					if ( fabs( predictTime - ministrider_dodge_warning.GetFloat() ) < .05 || random->RandomInt( 0, 3 ) )
					{
						SetCondition( COND_MINISTRIDER_INCOMING_VEHICLE );
					}
					else
					{
						if ( ministrider_dodge_debug. GetBool() )
						{
							Msg( "MiniStrider %d failing dodge (ignore)\n", entindex() );
						}
					}

					if ( ministrider_dodge_debug. GetBool() )
					{
						NDebugOverlay::Cross3D( EyePosition(), 100, 255, 255, 255, true, .1 );
						if ( timeDrawnArrow != gpGlobals->curtime )
						{
							timeDrawnArrow = gpGlobals->curtime;
							Vector vEndpoint( vecFuturePos.x, vecFuturePos.y, UTIL_GetLocalPlayer()->WorldSpaceCenter().z - 24 );
							NDebugOverlay::HorzArrow( UTIL_GetLocalPlayer()->WorldSpaceCenter() - Vector(0, 0, 24), vEndpoint, ministrider_dodge_warning_width.GetFloat(), 255, 0, 0, 64, true, .1 );
						}
					}
				}
				else if ( ministrider_dodge_debug.GetBool() )
				{
					if ( t <= 0 )
					{
						NDebugOverlay::Cross3D( EyePosition(), 100, 0, 0, 255, true, .1 );
					}
					else
					{
						NDebugOverlay::Cross3D( EyePosition(), 100, 0, 255, 255, true, .1 );
					}
				}
			}
			else if ( ministrider_dodge_debug.GetBool() )
			{
				NDebugOverlay::Cross3D( EyePosition(), 100, 0, 255, 0, true, .1 );
			}
			if ( ministrider_dodge_debug. GetBool() )
			{
				if ( timeDrawnArrow != gpGlobals->curtime )
				{
					timeDrawnArrow = gpGlobals->curtime;
					Vector vEndpoint( vecFuturePos.x, vecFuturePos.y, UTIL_GetLocalPlayer()->WorldSpaceCenter().z - 24 );
					NDebugOverlay::HorzArrow( UTIL_GetLocalPlayer()->WorldSpaceCenter() - Vector(0, 0, 24), vEndpoint, ministrider_dodge_warning_width.GetFloat(), 127, 127, 127, 64, true, .1 );
				}
			}

		}

		m_vecEnemyLastSeen = GetEnemy()->GetAbsOrigin();
	}

	if( !HasCondition(COND_ENEMY_OCCLUDED) )
	{
		// m_flTimeSawEnemyAgain always tells us what time I first saw this
		// enemy again after some period of not seeing them. This is used to
		// compute how long the enemy has been visible to me THIS TIME. 
		// Every time I lose sight of the enemy this time is set invalid until
		// I see the enemy again and record that time.
		if( m_flTimeSawEnemyAgain == MINISTRIDER_SEE_ENEMY_TIME_INVALID )
		{
			m_flTimeSawEnemyAgain = gpGlobals->curtime;
		}
	}
	else
	{
		m_flTimeSawEnemyAgain = MINISTRIDER_SEE_ENEMY_TIME_INVALID;
	}

	ManageSiegeTargets();
}

//-----------------------------------------------------------------------------
// Search all entities in the map
//-----------------------------------------------------------------------------
void CNPC_MiniStrider::CollectSiegeTargets()
{
	CBaseEntity *pTarget = gEntList.FindEntityByName( NULL, m_iszSiegeTargetName );

	while( pTarget != NULL )
	{
		if( pTarget->Classify() == CLASS_BULLSEYE )
		{
			m_pSiegeTargets.AddToTail( pTarget );
		}

		pTarget = gEntList.FindEntityByName( pTarget, m_iszSiegeTargetName );
	};

	if( m_pSiegeTargets.Count() < 1 )
	{
		m_iszSiegeTargetName = NULL_STRING;		// And stop trying!
	}
}

//-----------------------------------------------------------------------------
// For use when MiniStriders are outside and the player is inside a structure
// Create a temporary bullseye in a location that makes it seem like
// I am aware of the location of a player I cannot see. (Then fire at
// at this bullseye, thus laying 'siege' to the part of the building he 
// is in.) The locations are copied from suitable info_target entities.
// (these should be placed in exterior windows and doorways so that 
// the MiniStrider fires into the building through these apertures)
//-----------------------------------------------------------------------------
void CNPC_MiniStrider::ManageSiegeTargets()
{
	if( gpGlobals->curtime < m_flTimeNextSiegeTargetAttack )
		return;

	if( m_pSiegeTargets.Count() == 0 )
	{
		// If my list of siege targets is empty, go and cache all of them now
		// so that I don't have to search the world every time.
		CollectSiegeTargets();

		if( m_pSiegeTargets.Count() == 0 )
			return;
	}

	m_flTimeNextSiegeTargetAttack = gpGlobals->curtime + (ministrider_siege_frequency.GetFloat() * RandomFloat( 0.8f, 1.2f) );
	CBasePlayer *pPlayer = UTIL_GetNearestPlayer(GetAbsOrigin());

	// Start by assuming we are not going to create a siege target
	bool bCreateSiegeTarget = false;
	if( GetEnemy() == NULL )
	{
		// If I have no enemy at all, give it a try.
		bCreateSiegeTarget = true;
	}

	if( bCreateSiegeTarget )
	{
		// We've decided that the situation calls for a siege target. So, we dig through all of my siege targets and
		// take the closest one to the player that the player can see! (Obey they bullseye's FOV)
		float flClosestDistSqr = Square( 1200.0f ); // Only use siege targets within 100 feet of player
		CBaseEntity *pSiegeTargetLocation = NULL;
		int iTraces = 0;
		for( int i = 0 ; i < m_pSiegeTargets.Count() ; i++ )
		{
			CBaseEntity *pCandidate = m_pSiegeTargets[i];
			if ( pCandidate == NULL )
				continue;

			float flDistSqr = pCandidate->GetAbsOrigin().DistToSqr(pPlayer->GetAbsOrigin());

			if( flDistSqr < flClosestDistSqr )
			{
				// CollectSiegeTargets() guarantees my list is populated only with bullseye entities.
				CNPC_Bullseye *pBullseye = dynamic_cast<CNPC_Bullseye*>(pCandidate);
				if( !pBullseye->FInViewCone(this) )
					continue;

				if( pPlayer->FVisible(pCandidate) )
				{
					iTraces++;// Only counting these as a loose perf measurement
					flClosestDistSqr = flDistSqr;
					pSiegeTargetLocation = pCandidate;
				}
			}
		}

		if( pSiegeTargetLocation != NULL )
		{
			// Ditch any leftover siege target.
			KillCurrentSiegeTarget();

			// Create a bullseye that will live for 20 seconds. If we can't attack it within 20 seconds, it's probably
			// out of reach anyone, so have it clean itself up after that long.
			CBaseEntity *pSiegeTarget = CreateCustomTarget( pSiegeTargetLocation->GetAbsOrigin(), 20.0f );
			pSiegeTarget->SetName( MAKE_STRING("siegetarget") );

			m_hCurrentSiegeTarget.Set( pSiegeTarget );

			AddEntityRelationship( pSiegeTarget, D_HT, 1 );
			GetEnemies()->UpdateMemory( GetNavigator()->GetNetwork(), pSiegeTarget, pSiegeTarget->GetAbsOrigin(), 0.0f, true );
			AI_EnemyInfo_t *pMemory = GetEnemies()->Find( pSiegeTarget );

			if( pMemory )
			{
				// Pretend we've known about this target longer than we really have so that our AI doesn't waste time running ALERT schedules.
				pMemory->timeFirstSeen = gpGlobals->curtime - 5.0f;
				pMemory->timeLastSeen = gpGlobals->curtime - 1.0f;
			}
		}
	}
}

//-----------------------------------------------------------------------------
// Destroy the bullseye that we're using as a temporary target
//-----------------------------------------------------------------------------
void CNPC_MiniStrider::KillCurrentSiegeTarget()
{
	if ( m_hCurrentSiegeTarget )
	{
		GetEnemies()->ClearMemory( m_hCurrentSiegeTarget );

		UTIL_Remove( m_hCurrentSiegeTarget );
		m_hCurrentSiegeTarget.Set( NULL );
	}
}

//-----------------------------------------------------------------------------
// Return true if this NPC can hear the specified sound
//-----------------------------------------------------------------------------
bool CNPC_MiniStrider::QueryHearSound( CSound *pSound )
{
	if ( pSound->SoundContext() & SOUND_CONTEXT_EXCLUDE_COMBINE )
		return false;

	if ( pSound->SoundContext() & SOUND_CONTEXT_PLAYER_VEHICLE )
		return false;

	return BaseClass::QueryHearSound( pSound );
}


//-----------------------------------------------------------------------------
// This is a fairly bogus heuristic right now, but it works on 06a and 12 (sjb)
//
// Better options:	Trace infinitely and check the material we hit for sky
//					Put some leaf info in the BSP
//					Use volumes in the levels? (yucky for designers)
//-----------------------------------------------------------------------------
// TODO: use this or nuke it!
void CNPC_MiniStrider::GatherIndoorOutdoorConditions()
{
	// Check indoor/outdoor before calling base class, since base class calls our
	// RangeAttackConditions() functions, and we want those functions to know 
	// whether we're indoors or out.
	trace_t tr;

	UTIL_TraceLine( GetAbsOrigin(), GetAbsOrigin() + Vector( 0, 0, 40.0f * 12.0f ), MASK_SHOT, this, COLLISION_GROUP_NONE, &tr );
	if( tr.fraction < 1.0f )
	{
		SetCondition( COND_MINISTRIDER_IS_INDOORS );
	}
	else
	{
		ClearCondition( COND_MINISTRIDER_IS_INDOORS );
	}
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CNPC_MiniStrider::BuildScheduleTestBits()
{
	BaseClass::BuildScheduleTestBits();

	if ( m_lifeState != LIFE_ALIVE )
	{
		return;
	}

	// Our range attack is uninterruptable for the first few seconds.
	if ( IsCurSchedule( SCHED_MINISTRIDER_RANGE_ATTACK2, false ) && ( gpGlobals->curtime < m_flShootAllowInterruptTime ) )
	{
		ClearCustomInterruptConditions();
		SetCustomInterruptCondition( COND_HEAVY_DAMAGE );
	}
	else if ( IsCurSchedule( SCHED_MINISTRIDER_RANGE_ATTACK2, false ) && ( GetActivity() == ACT_TRANSITION ) )
	{
		// Don't stop unplanting just because we can range attack again.
		ClearCustomInterruptCondition( COND_CAN_RANGE_ATTACK1 );
		ClearCustomInterruptCondition( COND_CAN_RANGE_ATTACK2 );
	}
	else if ( !IsInLargeOutdoorMap() && IsCurSchedule( SCHED_MINISTRIDER_FLANK_ENEMY, false ) && GetEnemy() != NULL )
	{
		if( HasCondition(COND_CAN_RANGE_ATTACK2) && m_flTimeSawEnemyAgain != MINISTRIDER_SEE_ENEMY_TIME_INVALID )
		{
			if( (gpGlobals->curtime - m_flTimeSawEnemyAgain) >= 2.0f )
			{
				// When we're running flank behavior, wait a moment AFTER being able to see the enemy before
				// breaking my schedule to range attack. This helps assure that the ministrider gets well inside
				// the room before stopping to attack. Otherwise the MiniStrider may stop immediately in the doorway
				// and stop the progress of any ministriders behind it.
				SetCustomInterruptCondition( COND_CAN_RANGE_ATTACK2 );
			}
		}
	}

	// If our enemy is anything but a striderbuster, drop everything if we see one.
	if ( !IsStriderBuster( GetEnemy() ) )
	{
		SetCustomInterruptCondition( COND_MINISTRIDER_SEE_STRIDERBUSTER );
	}

	// If we're not too busy, allow ourselves to ACK found enemy signals.
	if ( !GetEnemy() )
	{
		SetCustomInterruptCondition( COND_MINISTRIDER_SQUADMATE_FOUND_ENEMY );
	}

	// Interrupt everything if we need to dodge.
	if ( !IsCurSchedule( SCHED_MINISTRIDER_DODGE, false ) && 
		 !IsCurSchedule( SCHED_MINISTRIDER_STAGGER, false ) &&
		 !IsCurSchedule( SCHED_ALERT_FACE_BESTSOUND, false ) )
	{
		SetCustomInterruptCondition( COND_MINISTRIDER_INCOMING_VEHICLE );
		SetCustomInterruptCondition( COND_HEAR_PHYSICS_DANGER );
		SetCustomInterruptCondition( COND_MINISTRIDER_FORCED_DODGE );
	}

	// Always interrupt on a flank command.	
	SetCustomInterruptCondition( COND_MINISTRIDER_FORCED_FLANK_ENEMY );

	// Always interrupt if staggered.
	SetCustomInterruptCondition( COND_MINISTRIDER_STAGGERED );
	
	// Always interrupt if hit by a sticky bomb.
	SetCustomInterruptCondition( COND_MINISTRIDER_HIT_BY_STICKYBOMB );
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
static bool IsMovablePhysicsObject( CBaseEntity *pEntity )
{
	return pEntity && pEntity->GetMoveType() == MOVETYPE_VPHYSICS && pEntity->VPhysicsGetObject() && pEntity->VPhysicsGetObject()->IsMoveable();
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
NPC_STATE CNPC_MiniStrider::SelectIdealState()
{
	switch ( m_NPCState )
	{
		case NPC_STATE_COMBAT:
		{
			if ( GetEnemy() == NULL )
			{
				if ( !HasCondition( COND_ENEMY_DEAD ) && !ministrider_disable_patrol.GetBool() )
				{
					// Lost track of my enemy. Patrol.
					SetCondition( COND_MINISTRIDER_SHOULD_PATROL );
				}

				return NPC_STATE_ALERT;
			}
			else if ( HasCondition( COND_ENEMY_DEAD ) )
			{
				// dvs: TODO: announce enemy kills?
				//AnnounceEnemyKill(GetEnemy());
			}
		}

		default:
		{
			return BaseClass::SelectIdealState();
		}
	}

	return GetIdealState();
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
bool CNPC_MiniStrider::ShouldCharge( const Vector &startPos, const Vector &endPos, bool useTime, bool bCheckForCancel )
{
	// Must have a target
	if ( !GetEnemy() )
		return false;

	// Don't check the distance once we start charging
	if ( !bCheckForCancel && !ministrider_charge_test.GetBool() )
	{
		float distance = ( startPos.AsVector2D() - endPos.AsVector2D() ).LengthSqr();

		// Must be within our tolerance range
		if ( ( distance < Square(MINISTRIDER_CHARGE_MIN) ) || ( distance > Square(MINISTRIDER_CHARGE_MAX) ) )
			return false;
	}

	// FIXME: We'd like to exclude small physics objects from this check!

	// We only need to hit the endpos with the edge of our bounding box
	Vector vecDir = endPos - startPos;
	VectorNormalize( vecDir );
	float flWidth = WorldAlignSize().x * 0.5;
	Vector vecTargetPos = endPos - (vecDir * flWidth);

	// See if we can directly move there
	AIMoveTrace_t moveTrace;
	GetMoveProbe()->MoveLimit( NAV_GROUND, startPos, vecTargetPos, MASK_NPCSOLID_BRUSHONLY, GetEnemy(), &moveTrace );
	
	// Draw the probe
	if ( g_debug_ministrider_charge.GetInt() == 1 )
	{
		Vector	enemyDir	= (vecTargetPos - startPos);
		float	enemyDist	= VectorNormalize( enemyDir );

		NDebugOverlay::BoxDirection( startPos, GetHullMins(), GetHullMaxs() + Vector(enemyDist,0,0), enemyDir, 0, 255, 0, 8, 1.0f );
	}

	// If we're not blocked, charge
	if ( IsMoveBlocked( moveTrace ) )
	{
		// Don't allow it if it's too close to us
		if ( UTIL_DistApprox( WorldSpaceCenter(), moveTrace.vEndPosition ) < MINISTRIDER_CHARGE_MIN )
			return false;

		// Allow some special cases to not block us
		if ( moveTrace.pObstruction != NULL )
		{
			// If we've hit the world, see if it's a cliff
			if ( moveTrace.pObstruction == GetContainingEntity( INDEXENT(0) ) )
			{	
				// Can't be too far above/below the target
				if ( fabs( moveTrace.vEndPosition.z - vecTargetPos.z ) > StepHeight() )
					return false;

				// Allow it if we got pretty close
				if ( UTIL_DistApprox( moveTrace.vEndPosition, vecTargetPos ) < 64 )
					return true;
			}

			// Hit things that will take damage
			if ( moveTrace.pObstruction->m_takedamage != DAMAGE_NO )
				return true;

			// Hit things that will move
			if ( moveTrace.pObstruction->GetMoveType() == MOVETYPE_VPHYSICS )
				return true;
		}

		return false;
	}

	float zDelta = endPos.z - moveTrace.vEndPosition.z;
	if ( fabsf(zDelta) > GetHullHeight() * 0.7)
	{
		return false;
	}

	return true;
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
bool CNPC_MiniStrider::HandleInteraction(int interactionType, void *data, CBaseCombatCharacter *pSourceEnt)
{
	if ( ( pSourceEnt != this ) && ( interactionType == g_interactionMiniStriderFoundEnemy ) )
	{
		SetCondition( COND_MINISTRIDER_SQUADMATE_FOUND_ENEMY );
		return true;
	}

	return BaseClass::HandleInteraction( interactionType, data, pSourceEnt );
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
int CNPC_MiniStrider::SelectCombatSchedule()
{
	// If we're here with no enemy, patrol and hope we find one.
	CBaseEntity *pEnemy = GetEnemy();
	if ( pEnemy == NULL )
	{
		if ( !ministrider_disable_patrol.GetBool() )
			return SCHED_MINISTRIDER_PATROL_RUN;
		else
			return SCHED_ALERT_STAND;
	}

	if ( ministrider_ar2round_test.GetBool() )
	{
		if ( HasCondition( COND_CAN_RANGE_ATTACK2 ) )
		{
			return SCHED_MINISTRIDER_RANGE_ATTACK2;
		}
		return SCHED_COMBAT_FACE;
	}

	bool bStriderBuster = IsStriderBuster( pEnemy );
	if ( bStriderBuster )
	{
		if ( gpGlobals->curtime - CAI_MiniStriderEscortBehavior::gm_flLastDefendSound > 10.0 )
		{
			EmitSound( "NPC_Hunter.DefendStrider" );
			CAI_MiniStriderEscortBehavior::gm_flLastDefendSound = gpGlobals->curtime;
		}

		if ( HasCondition( COND_CAN_RANGE_ATTACK2 ) || HasCondition( COND_NOT_FACING_ATTACK ) )
		{
			return SCHED_MINISTRIDER_RANGE_ATTACK2;
		}
		return SCHED_ESTABLISH_LINE_OF_FIRE;
	}

	// Certain behaviors, like flanking and melee attacks, only make sense on visible,
	// corporeal enemies (NOT bullseyes).
	bool bIsCorporealEnemy = IsCorporealEnemy( pEnemy );

	// Take a quick swipe at our enemy if able to do so.
	if ( bIsCorporealEnemy && HasCondition( COND_CAN_MELEE_ATTACK1 ) )
	{
		return SCHED_MINISTRIDER_MELEE_ATTACK1;
	}

	// React to newly acquired enemies.
	if ( bIsCorporealEnemy && HasCondition( COND_NEW_ENEMY ) )
	{
		AI_EnemyInfo_t *pEnemyInfo = GetEnemies()->Find( pEnemy );

		if ( GetSquad() && pEnemyInfo && ( pEnemyInfo->timeFirstSeen == pEnemyInfo->timeAtFirstHand ) )
		{
			GetSquad()->BroadcastInteraction( g_interactionMiniStriderFoundEnemy, NULL, this );

			// First contact for my squad.
			return SCHED_MINISTRIDER_FOUND_ENEMY;
		}
	}

	if ( HasCondition( COND_MINISTRIDER_SQUADMATE_FOUND_ENEMY ) )
	{
		// A squadmate found an enemy. Respond to their call.
		return SCHED_MINISTRIDER_FOUND_ENEMY_ACK;
	}

	// Fire a ar2round volley. Ignore squad slots if we're attacking a striderbuster.
	// See if there is an opportunity to charge.
	if ( !bStriderBuster && bIsCorporealEnemy && HasCondition( COND_MINISTRIDER_CAN_CHARGE_ENEMY ) )
	{
		if ( ministrider_charge_test.GetBool() || random->RandomInt( 1, 100 ) < ministrider_charge_pct.GetInt() )
		{
			if ( ministrider_charge_test.GetBool() || OccupyStrategySlot( SQUAD_SLOT_MINISTRIDER_CHARGE ) )
			{
				return SCHED_MINISTRIDER_CHARGE_ENEMY;
			}
		}
	}

	if ( HasCondition( COND_CAN_RANGE_ATTACK2 ) )
	{
		if ( bStriderBuster || CountRangedAttackers() < ministrider_ar2round_max_concurrent_volleys.GetInt() )
		{
			DelayRangedAttackers( ministrider_ar2round_volley_start_min_delay.GetFloat(), ministrider_ar2round_volley_start_max_delay.GetFloat(), true );
			return SCHED_MINISTRIDER_RANGE_ATTACK2;
		}
	}

	if ( pEnemy->GetGroundEntity() == this )
	{
		return SCHED_MINISTRIDER_MELEE_ATTACK1;
	}

	if ( HasCondition( COND_TOO_CLOSE_TO_ATTACK ) )
	{
		return SCHED_MOVE_AWAY_FROM_ENEMY;
	}

	// Sidestep every so often if my enemy is nearby and facing me.
/*
	if ( gpGlobals->curtime > m_flNextSideStepTime )
	{
		if ( HasCondition( COND_ENEMY_FACING_ME ) && ( UTIL_DistApprox( GetEnemy()->GetAbsOrigin(), GetAbsOrigin() ) < MINISTRIDER_FACE_ENEMY_DIST ) )
		{
			m_flNextSideStepTime = gpGlobals->curtime + random->RandomFloat( 1.0f, 3.0f );
			return SCHED_MINISTRIDER_SIDESTEP;
		}
	}
*/
	if ( HasCondition( COND_HEAVY_DAMAGE ) && ( gpGlobals->curtime > m_flNextSideStepTime ) )
	{
		m_flNextSideStepTime = gpGlobals->curtime + random->RandomFloat( 1.0f, 3.0f );
		return SCHED_MINISTRIDER_SIDESTEP;
	}

	if ( !bStriderBuster && bIsCorporealEnemy )
	{
		if ( HasCondition( COND_MINISTRIDER_CAN_CHARGE_ENEMY ) )
		{
			if ( OccupyStrategySlot( SQUAD_SLOT_MINISTRIDER_CHARGE ) )
			{
				return SCHED_MINISTRIDER_CHARGE_ENEMY;
			}
/*
			else
			{
				return SCHED_MINISTRIDER_SIDESTEP;
			}
*/
		}

		// Try to be a flanker.
		if ( ( NumMiniStridersInMySquad() > 1 ) && OccupyStrategySlotRange( SQUAD_SLOT_MINISTRIDER_FLANK_FIRST, SQUAD_SLOT_MINISTRIDER_FLANK_LAST ) )
		{
			return SCHED_MINISTRIDER_FLANK_ENEMY;
		}
	}
	
	// Can't see my enemy.
	if ( HasCondition( COND_ENEMY_OCCLUDED ) || HasCondition( COND_ENEMY_TOO_FAR ) || HasCondition( COND_TOO_FAR_TO_ATTACK ) || HasCondition( COND_NOT_FACING_ATTACK ) )
	{
		return SCHED_MINISTRIDER_CHASE_ENEMY;
	}

	if ( HasCondition( COND_MINISTRIDER_CANT_PLANT ) )
	{
		return SCHED_ESTABLISH_LINE_OF_FIRE;
	}

	//if ( HasCondition( COND_ENEMY_OCCLUDED ) && IsCurSchedule( SCHED_RANGE_ATTACK1, false ) )
	//{
	//	return SCHED_MINISTRIDER_COMBAT_FACE;
	//}

 	return SCHED_MINISTRIDER_CHANGE_POSITION;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
int CNPC_MiniStrider::SelectSiegeSchedule()
{
	bool bHasEnemy = (GetEnemy() != NULL);

	if( bHasEnemy )
	{
		// We have an enemy, so we should be making every effort to attack it.
		if( !HasCondition(COND_SEE_ENEMY) || !HasCondition(COND_CAN_RANGE_ATTACK2) )
			return SCHED_ESTABLISH_LINE_OF_FIRE;

		if( HasCondition(COND_CAN_RANGE_ATTACK2) )
			return SCHED_MINISTRIDER_RANGE_ATTACK2;

		return SCHED_MINISTRIDER_SIEGE_STAND;
	}
	else
	{
		// Otherwise we are loitering in siege mode. Break line of sight with the player
		// if they expose our position.
		if( HasCondition( COND_SEE_PLAYER ) )
			return SCHED_MINISTRIDER_CHANGE_POSITION_SIEGE;
	}

	return SCHED_MINISTRIDER_SIEGE_STAND;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
int CNPC_MiniStrider::SelectSchedule()
{
	if ( ministrider_stand_still.GetBool() )
	{
		m_bPlanted = false;
		return SCHED_IDLE_STAND;
	}
	
	if ( HasCondition( COND_MINISTRIDER_FORCED_DODGE ) )
		return SCHED_MINISTRIDER_DODGE;

	if ( HasCondition( COND_MINISTRIDER_NEW_HINTGROUP ) || ( GetHintGroup() != NULL_STRING && m_CheckHintGroupTimer.Expired() ) )
	{
		CAI_Hint *pHint;
		CHintCriteria criteria;
		criteria.SetGroup( GetHintGroup() );
		criteria.SetFlag( bits_HINT_NODE_NEAREST );

		if ( HasCondition( COND_MINISTRIDER_NEW_HINTGROUP ) )
		{
			ClearCondition( COND_MINISTRIDER_NEW_HINTGROUP );
			if ( GetEnemy() )
			{
				pHint = CAI_HintManager::FindHint( NULL, GetEnemy()->GetAbsOrigin(), criteria );
			}
			else
			{
				pHint = CAI_HintManager::FindHint( GetAbsOrigin(), criteria );
			}

			if ( pHint )
			{
				pHint->Lock( this );
			}
		} 
		else
		{
			pHint = CAI_HintManager::FindHint( GetAbsOrigin(), criteria );
			if ( pHint )
			{
				if ( (pHint->GetAbsOrigin() - GetAbsOrigin()).Length2DSqr() < Square( 20*12 ) )
				{
					m_CheckHintGroupTimer.Set( 5 );
					pHint = NULL;
				}
				else
				{
					m_CheckHintGroupTimer.Set( 15 );
				}
			}
		}

		if ( pHint )
		{
			SetHintNode( pHint );
			return SCHED_MINISTRIDER_GOTO_HINT;
		}
	}

	if ( HasCondition( COND_MINISTRIDER_INCOMING_VEHICLE ) )
	{
		if ( m_RundownDelay.Expired() )
		{
			int iRundownCounter = 0;
			if ( GetSquad() )
			{
				GetSquad()->GetSquadData( MINISTRIDER_RUNDOWN_SQUADDATA, &iRundownCounter );
			}

			if ( iRundownCounter % 2 == 0 )
			{
				for ( int i = 0; i < g_MiniStriders.Count(); i++ )
				{
					if ( g_MiniStriders[i] != this )
					{
						g_MiniStriders[i]->m_RundownDelay.Set( 3 );
						g_MiniStriders[i]->m_IgnoreVehicleTimer.Force();
					}
				}
				m_IgnoreVehicleTimer.Set( ministrider_dodge_warning.GetFloat() * 4 );
				if ( ministrider_dodge_debug.GetBool() )
				{
					Msg( "MiniStrider %d rundown\n", entindex() );
				}

				if ( HasCondition( COND_SEE_ENEMY ) )
				{
					if ( m_bPlanted && HasCondition( COND_CAN_RANGE_ATTACK2 ) )
					{
						return SCHED_MINISTRIDER_RANGE_ATTACK2;
					}
					else if ( random->RandomInt( 0, 1 ) )
					{
						return SCHED_MINISTRIDER_CHARGE_ENEMY;
					}
					else
					{
						return SCHED_MOVE_AWAY;
					}
				}
				else
				{
					SetTarget( UTIL_GetLocalPlayer() );
					return SCHED_TARGET_FACE;
				}
			}
			else
			{
				if ( ministrider_dodge_debug.GetBool() )
				{
					Msg( "MiniStrider %d safe from rundown\n", entindex() );
				}
				for ( int i = 0; i < g_MiniStriders.Count(); i++ )
				{
					g_MiniStriders[i]->m_RundownDelay.Set( 4 );
					g_MiniStriders[i]->m_IgnoreVehicleTimer.Force();
				}
				if ( GetSquad() )
				{
					GetSquad()->SetSquadData( MINISTRIDER_RUNDOWN_SQUADDATA, iRundownCounter + 1 );
				}
			}
		}

		if ( HasCondition( COND_SEE_ENEMY ) )
		{
			if ( ministrider_dodge_debug.GetBool() )
			{
				Msg( "MiniStrider %d try dodge\n", entindex() );
			}
			return SCHED_MINISTRIDER_DODGE;
		}
		else
		{
			SetTarget( UTIL_GetLocalPlayer() );
			return SCHED_TARGET_FACE;
		}

		CSound *pBestSound = GetBestSound( SOUND_PHYSICS_DANGER );
		if ( pBestSound && ( pBestSound->SoundContext() & SOUND_CONTEXT_PLAYER_VEHICLE ) )
		{
			return SCHED_ALERT_FACE_BESTSOUND;
		}
	}

	if ( HasCondition( COND_MINISTRIDER_FORCED_FLANK_ENEMY ) )
	{
		return SCHED_MINISTRIDER_FLANK_ENEMY;
	}
	
	if ( HasCondition( COND_MINISTRIDER_STAGGERED ) /*|| HasCondition( COND_MINISTRIDER_HIT_BY_STICKYBOMB )*/ )
	{
		return SCHED_MINISTRIDER_STAGGER;
	}

	// Now that we're past all of the forced reactions to things, if we're running the siege
	// behavior, go pick an appropriate siege schedule UNLESS we have an enemy. If we have
	// an enemy, we should focus on attacking that enemy.
	if( IsUsingSiegeTargets() )
	{
		return SelectSiegeSchedule();
	}

	// back away if there's a magnade glued to my head.
	if ( ministrider_retreat_striderbusters.GetBool() /*&& GetEnemy() && ( GetEnemy()->IsPlayer() )*/ 
		&& (m_hAttachedBusters.Count() > 0)
		&& m_fCorneredTimer < gpGlobals->curtime)
	{
		return SCHED_MINISTRIDER_TAKE_COVER_FROM_ENEMY;
	}

	if ( !BehaviorSelectSchedule() )
	{
		switch ( GetState() )
		{
			case NPC_STATE_IDLE:
			{
				return SCHED_MINISTRIDER_PATROL;
			}

			case NPC_STATE_ALERT:
			{
				if ( HasCondition( COND_MINISTRIDER_SHOULD_PATROL ) )
					return SCHED_MINISTRIDER_PATROL;
					
				break;
			}

			case NPC_STATE_COMBAT:
			{
				return SelectCombatSchedule();
			}
		}
	}
		
	return BaseClass::SelectSchedule();
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
int CNPC_MiniStrider::TranslateSchedule( int scheduleType )
{
	switch ( scheduleType )
	{
		case SCHED_RANGE_ATTACK1:
		{
			return SCHED_MINISTRIDER_RANGE_ATTACK1;
		}

		case SCHED_RANGE_ATTACK2:
		case SCHED_MINISTRIDER_RANGE_ATTACK2:
		{
			if ( scheduleType == SCHED_RANGE_ATTACK2 )
			{
				Msg( "MINISTRIDER IGNORING SQUAD SLOTS\n" );
			}

			if ( IsStriderBuster( GetEnemy() ) )
			{
				// Attack as FAST as possible. The point is to shoot down the buster.
				return SCHED_MINISTRIDER_RANGE_ATTACK2_VS_STRIDERBUSTER;
			}

			return SCHED_MINISTRIDER_RANGE_ATTACK2;
		}

		case SCHED_MELEE_ATTACK1:
		{
			return SCHED_MINISTRIDER_MELEE_ATTACK1;
		}
	
		case SCHED_ESTABLISH_LINE_OF_FIRE_FALLBACK:
		{
			return SCHED_MINISTRIDER_CHANGE_POSITION;
		}

		case SCHED_ALERT_STAND:
		{
			if ( !ministrider_disable_patrol.GetBool() )
				return SCHED_MINISTRIDER_PATROL_RUN;
			break;
		}

		case SCHED_COMBAT_FACE:
		{
			return SCHED_MINISTRIDER_COMBAT_FACE;
		}

		case SCHED_MINISTRIDER_PATROL:
		{
			if ( ministrider_disable_patrol.GetBool() )
			{
				return SCHED_IDLE_STAND;
			}
			break;
		}
	}

	return BaseClass::TranslateSchedule( scheduleType );
}


//-----------------------------------------------------------------------------
// catch blockage while escaping magnade
//-----------------------------------------------------------------------------
void CNPC_MiniStrider::TaskFail( AI_TaskFailureCode_t code )
{
	if ( IsCurSchedule( SCHED_MINISTRIDER_TAKE_COVER_FROM_ENEMY, false ) && ( code == FAIL_NO_ROUTE_BLOCKED  ) )
	{
		// cornered!
		if ( m_fCorneredTimer < gpGlobals->curtime )
		{
			m_fCorneredTimer = gpGlobals->curtime + 6.0f;
		}
	}

	BaseClass::TaskFail( code );
}


//-----------------------------------------------------------------------------
// The player is speeding toward us in a vehicle! Find a good activity for dodging.
//-----------------------------------------------------------------------------
void CNPC_MiniStrider::TaskFindDodgeActivity()
{
	if ( GetEnemy() == NULL )
	{
		TaskFail( "No enemy to dodge" );
		return;
	}

	Vector vecUp;
	Vector vecRight;
	GetVectors( NULL, &vecRight, &vecUp );

	// TODO: find most perpendicular 8-way dodge when we get the anims
	Vector vecEnemyDir = GetEnemy()->GetAbsOrigin() - GetAbsOrigin();
	//Vector vecDir = CrossProduct( vecEnemyDir, vecUp );
	VectorNormalize( vecEnemyDir );
	if ( fabs( DotProduct( vecEnemyDir, vecRight ) ) > 0.7 )
	{
		TaskFail( "Can't dodge, enemy approaching perpendicularly" );
		return;
	}

	// Check left or right randomly first.
	bool bDodgeLeft = false;
	CBaseEntity *pVehicle = GetEnemyVehicle();
	if ( pVehicle  )
	{
		Ray_t enemyRay;
		Ray_t perpendicularRay;
		enemyRay.Init( pVehicle->GetAbsOrigin(), pVehicle->GetAbsOrigin() + pVehicle->GetSmoothedVelocity() );
		Vector vPerpendicularPt = vecEnemyDir;
		vPerpendicularPt.y = -vPerpendicularPt.y;
		perpendicularRay.Init( GetAbsOrigin(), GetAbsOrigin() + vPerpendicularPt );

		enemyRay.m_Start.z = enemyRay.m_Delta.z = enemyRay.m_StartOffset.z;
		perpendicularRay.m_Start.z = perpendicularRay.m_Delta.z = perpendicularRay.m_StartOffset.z;

		float t, s;

		IntersectRayWithRay( perpendicularRay, enemyRay, t, s );

		if ( t > 0 )
		{
			bDodgeLeft = true;
		}
	}
	else if ( random->RandomInt( 0, 1 ) == 0 )
	{
		bDodgeLeft = true;
	}

	bool bFoundDir = false;
	int nTries = 0;

	while ( !bFoundDir && ( nTries < 2 ) )
	{
		// Pick a dodge activity to try.
		if ( bDodgeLeft )
		{
			m_eDodgeActivity = ACT_MINTER_DODGEL;
		}
		else
		{
			m_eDodgeActivity = ACT_MINTER_DODGER;
		}

		// See where the dodge will put us.
		Vector vecLocalDelta;
		int nSeq = SelectWeightedSequence( m_eDodgeActivity );
		GetSequenceLinearMotion( nSeq, &vecLocalDelta );

		// Transform the sequence delta into local space.
		matrix3x4_t fRotateMatrix;
		AngleMatrix( GetLocalAngles(), fRotateMatrix );
		Vector vecDelta;
		VectorRotate( vecLocalDelta, fRotateMatrix, vecDelta );

		// Trace a bit high so this works better on uneven terrain.
		Vector testHullMins = GetHullMins();
		testHullMins.z += ( StepHeight() * 2 );

		// See if all is clear in that direction.
		trace_t tr;
		MiniStriderTraceHull_SkipPhysics( GetAbsOrigin(), GetAbsOrigin() + vecDelta, testHullMins, GetHullMaxs(), MASK_NPCSOLID, this, GetCollisionGroup(), &tr, VPhysicsGetObject()->GetMass() * 0.5f );

		// TODO: dodge anyway if we'll make it a certain percentage of the way through the dodge?
		if ( tr.fraction == 1.0f )
		{
			//NDebugOverlay::SweptBox( GetAbsOrigin(), GetAbsOrigin() + vecDelta, testHullMins, GetHullMaxs(), QAngle( 0, 0, 0 ), 0, 255, 0, 128, 5 );
			bFoundDir = true;
			TaskComplete();
		}
		else
		{
			//NDebugOverlay::SweptBox( GetAbsOrigin(), GetAbsOrigin() + vecDelta, testHullMins, GetHullMaxs(), QAngle( 0, 0, 0 ), 255, 0, 0, 128, 5 );
			nTries++;
			bDodgeLeft = !bDodgeLeft;
		}
	}

	if ( nTries < 2 )
	{
		TaskComplete();
	}
	else
	{
		TaskFail( "Couldn't find dodge position\n" );
	}
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CNPC_MiniStrider::StartTask( const Task_t *pTask )
{
	switch ( pTask->iTask )
	{
		case TASK_MINISTRIDER_FINISH_RANGE_ATTACK:
		{
			if( GetEnemy() != NULL && GetEnemy()->Classify() == CLASS_PLAYER_ALLY_VITAL )
			{
				// Just finished shooting at Alyx! So forget her for a little while and get back on the player
				// !!!LATER - make sure there's someone else in enemy memory to go bother.
				GetEnemies()->SetTimeValidEnemy( GetEnemy(), gpGlobals->curtime + 10.0f );
			}

			if( m_hCurrentSiegeTarget )
			{
				// We probably just fired at our siege target, so dump it.
				KillCurrentSiegeTarget();
			}

			TaskComplete();
		}

		case TASK_MINISTRIDER_WAIT_FOR_MOVEMENT_FACING_ENEMY:
		{
			ChainStartTask( TASK_WAIT_FOR_MOVEMENT, pTask->flTaskData );
			break;
		}

		case TASK_MINISTRIDER_BEGIN_FLANK:
		{
			if ( IsInSquad() && GetSquad()->NumMembers() > 1 )
			{
				// Flank relative to the other shooter in our squad.
				// If there's no other shooter, just flank relative to any squad member.
				AISquadIter_t iter;
				CAI_BaseNPC *pNPC = GetSquad()->GetFirstMember( &iter );
				while ( pNPC == this )
				{
					pNPC = GetSquad()->GetNextMember( &iter );
				}

				m_vSavePosition = pNPC->GetAbsOrigin();
			}
			else
			{
				// Flank relative to our current position.
				m_vSavePosition = GetAbsOrigin();
			}
			
			TaskComplete();
			break;
		}

		case TASK_MINISTRIDER_ANNOUNCE_FLANK:
		{
			EmitSound( "NPC_Hunter.FlankAnnounce" );
			TaskComplete();
			break;
		}

		case TASK_MINISTRIDER_DODGE:
		{
			if ( ministrider_dodge_debug. GetBool() )
			{
				Msg( "MiniStrider %d dodging\n", entindex() );
			}
			SetIdealActivity( m_eDodgeActivity );
			break;
		}
		
		// Guarantee a certain delay between volleys. If we aren't already planted,
		// the plant transition animation will take care of that.
		case TASK_MINISTRIDER_PRE_RANGE_ATTACK2:
		{
			if ( !m_bPlanted  || ( GetEnemy() && IsStriderBuster( GetEnemy() ) ) )
			{
				TaskComplete();
			}
			else
			{
				SetIdealActivity( ACT_MINTER_ANGRY );
			}
			break;
		}

		case TASK_MINISTRIDER_SHOOT_COMMIT:
		{
			// We're committing to shooting. Don't allow interrupts until after we've shot a bit (see TASK_RANGE_ATTACK1).
			m_flShootAllowInterruptTime = gpGlobals->curtime + 100.0f;
			TaskComplete();
			break;
		}
		
		case TASK_RANGE_ATTACK2:
		{
			if ( GetEnemy() )
			{
				bool bIsBuster = IsStriderBuster( GetEnemy() );
				if ( bIsBuster )
				{
					AddFacingTarget( GetEnemy(), GetEnemy()->GetAbsOrigin() + GetEnemy()->GetSmoothedVelocity() * .5, 1.0, 0.8 );
				}

				// Start the firing sound.
				//CSoundEnvelopeController &controller = CSoundEnvelopeController::GetController();
				//controller.SoundChangeVolume( m_pGunFiringSound, 1.0, ministrider_first_ar2round_delay.GetFloat() );

				SetIdealActivity( ACT_RANGE_ATTACK2 );

				// Decide how many shots to fire.
				int nShots = ministrider_ar2round_volley_size.GetInt();
				if ( g_pGameRules->IsSkillLevel( SKILL_EASY ) )
				{
					nShots--;
				}

				// Decide when to fire the first shot.
				float initialDelay = ministrider_first_ar2round_delay.GetFloat();
				if ( bIsBuster )
				{
					initialDelay = 0; //*= 0.5;
				}

				BeginVolley( nShots, gpGlobals->curtime + initialDelay );

				// In case we need to miss on purpose, pick a direction now.
				m_bMissLeft = false;
				if ( random->RandomInt( 0, 1 ) == 0 )
				{
					m_bMissLeft = true;
				}

				LockBothEyes( initialDelay + ( nShots * ministrider_ar2round_delay.GetFloat() ) );
			}
			else
			{
				TaskFail( FAIL_NO_ENEMY );
			}
			
			break;
		}

		case TASK_MINISTRIDER_STAGGER:
		{
			// Stagger in the direction the impact force would push us.
			VMatrix worldToLocalRotation = EntityToWorldTransform();
			Vector vecLocalStaggerDir = worldToLocalRotation.InverseTR().ApplyRotation( m_vecStaggerDir );
			
			float flStaggerYaw = VecToYaw( vecLocalStaggerDir );
			SetPoseParameter( gm_nStaggerYawPoseParam, flStaggerYaw );

			// Go straight there!
			SetActivity( ACT_RESET );
			SetActivity( ( Activity )ACT_MINTER_STAGGER );
			break;
		}
	
		case TASK_MELEE_ATTACK1:
		{
			SetLastAttackTime( gpGlobals->curtime );
			
			if ( GetEnemy() && GetEnemy()->IsPlayer() )
			{
				ResetIdealActivity( ( Activity )ACT_MINTER_MELEE_ATTACK1_VS_PLAYER );
			}
			else
			{
				ResetIdealActivity( ACT_MELEE_ATTACK1 );
			}
			
			break;
		}

		case TASK_MINISTRIDER_CORNERED_TIMER:
		{
			m_fCorneredTimer = gpGlobals->curtime + pTask->flTaskData;

			break;
		}

		case TASK_MINISTRIDER_FIND_SIDESTEP_POSITION:
		{
			if ( GetEnemy() == NULL )
			{
				TaskFail( "No enemy to sidestep" );
			}
			else
			{
				Vector vecUp;
				GetVectors( NULL, NULL, &vecUp );
				
				Vector vecEnemyDir = GetEnemy()->GetAbsOrigin() - GetAbsOrigin();
				Vector vecDir = CrossProduct( vecEnemyDir, vecUp );
				VectorNormalize( vecDir );

				// Sidestep left or right randomly.				
				if ( random->RandomInt( 0, 1 ) == 0 )
				{
					vecDir *= -1;
				}

				// Start high and then trace down so that it works on uneven terrain.
				Vector vecPos = GetAbsOrigin() + Vector( 0, 0, 64 ) + random->RandomFloat( 120, 200 ) * vecDir;
				
				// Try to find the ground at the sidestep position.
				trace_t tr;
				UTIL_TraceLine( vecPos, vecPos + Vector( 0, 0, -128 ), MASK_NPCSOLID, NULL, COLLISION_GROUP_NONE, &tr );
				if ( tr.fraction < 1.0f )
				{
					//NDebugOverlay::Line( vecPos, tr.endpos, 0, 255, 0, true, 10 ); 
		
					m_vSavePosition = tr.endpos;
									
					TaskComplete();
				}
				else
				{
					TaskFail( "Couldn't find sidestep position\n" );
				}
			}
			
			break;
		}

		case TASK_MINISTRIDER_FIND_DODGE_POSITION:
		{
			TaskFindDodgeActivity();
			break;
		}

		case TASK_MINISTRIDER_CHARGE:
		{
			SetIdealActivity( ( Activity )ACT_MINTER_CHARGE_START );
			break;
		}

		case TASK_MINISTRIDER_CHARGE_DELAY:
		{
			m_flNextChargeTime = gpGlobals->curtime + pTask->flTaskData;
			TaskComplete();
			break;
		}
	
		case TASK_DIE:
		{
			GetNavigator()->StopMoving();	
			ResetActivity();
			SetIdealActivity( GetDeathActivity() );
			m_lifeState = LIFE_DYING;

			break;
		}

		//case TASK_MINISTRIDER_END_FLANK:
		//{
		//	
		//}
	
		default:
		{
			BaseClass::StartTask( pTask );
			break;
		}
	}
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CNPC_MiniStrider::RunTask( const Task_t *pTask )
{
	switch ( pTask->iTask )
	{
		case TASK_MINISTRIDER_PRE_RANGE_ATTACK2:
		{
			if ( IsActivityFinished() )
			{
				TaskComplete();
			}
			break;
		}

		case TASK_RANGE_ATTACK2:
		{
			if( !ministrider_hate_thrown_striderbusters.GetBool() && GetEnemy() != NULL && IsStriderBuster( GetEnemy() ) )
			{
				if( !IsValidEnemy(GetEnemy()) )
				{
					TaskFail("No longer hate this StriderBuster");
				}
			}

			bool bIsBuster = IsStriderBuster( GetEnemy() );
			if ( bIsBuster )
			{
				Vector vFuturePosition = GetEnemy()->GetAbsOrigin() + GetEnemy()->GetSmoothedVelocity() * .3;
				AddFacingTarget( GetEnemy(), vFuturePosition, 1.0, 0.8 );

				Vector2D vToFuturePositon = ( vFuturePosition.AsVector2D() - GetAbsOrigin().AsVector2D() );
				vToFuturePositon.NormalizeInPlace();
				Vector2D facingDir = BodyDirection2D().AsVector2D();

				float flDot = DotProduct2D( vToFuturePositon, facingDir );

				if ( flDot < .4 )
				{
					GetMotor()->SetIdealYawToTarget( vFuturePosition );
					GetMotor()->UpdateYaw();
					break;
				}
			}

			if ( gpGlobals->curtime >= m_flNextFlechetteTime )
			{
				// Must have an enemy and a shot queued up.
				bool bDone = false;
				if ( GetEnemy() != NULL && m_nFlechettesQueued > 0 )
				{
					if ( ShootFlechette( GetEnemy(), false ) )
					{
						m_nClampedShots++;
					}
					else
					{
						m_nClampedShots = 0;
					}

					m_nFlechettesQueued--;

					// If we fired three or more clamped shots in a row, call it quits so we don't look dumb.
					if ( ( m_nClampedShots >= 3 ) || ( m_nFlechettesQueued == 0 ) )
					{
						bDone = true;
					}
					else
					{
						// More shooting to do. Schedule our next ar2round.
						m_flNextFlechetteTime = gpGlobals->curtime + ministrider_ar2round_delay.GetFloat();
					}
				}
				else
				{
					bDone = true;
				}
				
				if ( bDone )     
				{
					// Stop the firing sound.
					//CSoundEnvelopeController &controller = CSoundEnvelopeController::GetController();
					//controller.SoundChangeVolume( m_pGunFiringSound, 0.0f, 0.1f );

					DelayRangedAttackers( ministrider_ar2round_volley_end_min_delay.GetFloat(), ministrider_ar2round_volley_end_max_delay.GetFloat(), true );
					TaskComplete();
				}
			}

			break;
		}
	
		case TASK_GET_PATH_TO_ENEMY_LOS:
		{
			ChainRunTask( TASK_GET_PATH_TO_ENEMY_LKP_LOS, pTask->flTaskData );
			break;
		}

		case TASK_MINISTRIDER_DODGE:
		{
			AutoMovement();

			if ( IsActivityFinished() )
			{
				TaskComplete();
			}
			break;
		}

		case TASK_MINISTRIDER_CORNERED_TIMER:
		{
			TaskComplete();
			break;
		}
		
		case TASK_MINISTRIDER_STAGGER:
		{
			if ( IsActivityFinished() )
			{
				TaskComplete();
			}
			break;
		}
				
		case TASK_MINISTRIDER_CHARGE:
		{
			Activity eActivity = GetActivity();

			// See if we're trying to stop after hitting/missing our target
			if ( eActivity == ACT_MINTER_CHARGE_STOP || eActivity == ACT_MINTER_CHARGE_CRASH ) 
			{
				if ( IsActivityFinished() )
				{
					m_flNextChargeTime = gpGlobals->curtime + ministrider_charge_min_delay.GetFloat() + random->RandomFloat( 0, 2.5 ) + random->RandomFloat( 0, 2.5 );
					float delayMultiplier = ( g_pGameRules->IsSkillLevel( SKILL_EASY ) ) ? 1.5 : 1.0;
					float groupDelay = gpGlobals->curtime +  ( 2.0  + random->RandomFloat( 0, 2 ) ) * delayMultiplier;
					for ( int i = 0; i < g_MiniStriders.Count(); i++ )
					{
						if ( g_MiniStriders[i] != this && g_MiniStriders[i]->m_flNextChargeTime < groupDelay )
						{
							g_MiniStriders[i]->m_flNextChargeTime = groupDelay;
						}
					}
					TaskComplete();
					return;
				}

				// Still in the process of slowing down. Run movement until it's done.
				AutoMovement();
				return;
			}

			// Check for manual transition
			if ( ( eActivity == ACT_MINTER_CHARGE_START ) && ( IsActivityFinished() ) )
			{
				SetIdealActivity( ACT_MINTER_CHARGE_RUN );
			}

			// See if we're still running
			if ( eActivity == ACT_MINTER_CHARGE_RUN || eActivity == ACT_MINTER_CHARGE_START ) 
			{
				if ( HasCondition( COND_NEW_ENEMY ) || HasCondition( COND_LOST_ENEMY ) || HasCondition( COND_ENEMY_DEAD ) )
				{
					SetIdealActivity( ACT_MINTER_CHARGE_STOP );
					return;
				}
				else 
				{
					if ( GetEnemy() != NULL )
					{
						Vector	goalDir = ( GetEnemy()->GetAbsOrigin() - GetAbsOrigin() );
						VectorNormalize( goalDir );

						if ( DotProduct( BodyDirection2D(), goalDir ) < 0.25f )
						{
							SetIdealActivity( ACT_MINTER_CHARGE_STOP );
						}
					}
				}
			}

			// Steer towards our target
			float idealYaw;
			if ( GetEnemy() == NULL )
			{
				idealYaw = GetMotor()->GetIdealYaw();
			}
			else
			{
				idealYaw = CalcIdealYaw( GetEnemy()->GetAbsOrigin() );
			}

			// Add in our steering offset
			idealYaw += ChargeSteer();
			
			// Turn to face
			GetMotor()->SetIdealYawAndUpdate( idealYaw );

			// See if we're going to run into anything soon
			ChargeLookAhead();

			// Let our animations simply move us forward. Keep the result
			// of the movement so we know whether we've hit our target.
			AIMoveTrace_t moveTrace;
			if ( AutoMovement( GetEnemy(), &moveTrace ) == false )
			{
				// Only stop if we hit the world
				if ( HandleChargeImpact( moveTrace.vEndPosition, moveTrace.pObstruction ) )
				{
					// If we're starting up, this is an error
					if ( eActivity == ACT_MINTER_CHARGE_START )
					{
						TaskFail( "Unable to make initial movement of charge\n" );
						return;
					}

					// Crash unless we're trying to stop already
					if ( eActivity != ACT_MINTER_CHARGE_STOP )
					{
						if ( moveTrace.fStatus == AIMR_BLOCKED_WORLD && moveTrace.vHitNormal == vec3_origin )
						{
							SetIdealActivity( ACT_MINTER_CHARGE_STOP );
						}
						else
						{
							// Shake the screen
							if ( moveTrace.fStatus != AIMR_BLOCKED_NPC )
							{
								EmitSound( "NPC_Hunter.ChargeHitWorld" );
								UTIL_ScreenShake( GetAbsOrigin(), 16.0f, 4.0f, 1.0f, 400.0f, SHAKE_START );
							}
							SetIdealActivity( ACT_MINTER_CHARGE_CRASH );
						}
					}
				}
				else if ( moveTrace.pObstruction )
				{
					// If we hit another ministrider, stop
					if (moveTrace.pObstruction->Classify() == CLASS_COMBINE_HUNTER)
					{
						// Crash unless we're trying to stop already
						if ( eActivity != ACT_MINTER_CHARGE_STOP )
						{
							SetIdealActivity( ACT_MINTER_CHARGE_STOP );
						}
					}
					// If we hit an antlion, don't stop, but kill it
					// We never have ministriders and antlions together, but you never know.
					else if (moveTrace.pObstruction->Classify() == CLASS_ANTLION )
					{
						if ( FClassnameIs( moveTrace.pObstruction, "npc_antlionguard" ) )
						{
							// Crash unless we're trying to stop already
							if ( eActivity != ACT_MINTER_CHARGE_STOP )
							{
								SetIdealActivity( ACT_MINTER_CHARGE_STOP );
							}
						}
						else
						{
							MiniStrider_ApplyChargeDamage( this, moveTrace.pObstruction, moveTrace.pObstruction->GetHealth() );
						}
					}
				}
			}

			break;
		}
				
		case TASK_MINISTRIDER_WAIT_FOR_MOVEMENT_FACING_ENEMY:
		{
			if ( GetEnemy() )
			{
				Vector vecEnemyLKP = GetEnemyLKP();
				AddFacingTarget( GetEnemy(), vecEnemyLKP, 1.0, 0.8 );
			}
			ChainRunTask( TASK_WAIT_FOR_MOVEMENT, pTask->flTaskData );
			break;
		}

		default:
		{
			BaseClass::RunTask( pTask );
			break;		
		}
	}
}


//-----------------------------------------------------------------------------
// Return true if our charge target is right in front of the ministrider.
//-----------------------------------------------------------------------------
bool CNPC_MiniStrider::EnemyIsRightInFrontOfMe( CBaseEntity **pEntity )
{
	if ( !GetEnemy() )
		return false;

	if ( (GetEnemy()->WorldSpaceCenter() - WorldSpaceCenter()).LengthSqr() < (156*156) )
	{
		Vector vecLOS = ( GetEnemy()->GetAbsOrigin() - GetAbsOrigin() );
		vecLOS.z = 0;
		VectorNormalize( vecLOS );
		Vector vBodyDir = BodyDirection2D();
		if ( DotProduct( vecLOS, vBodyDir ) > 0.8 )
		{
			// He's in front of me, and close. Make sure he's not behind a wall.
			trace_t tr;
			UTIL_TraceHull( WorldSpaceCenter(), GetEnemy()->WorldSpaceCenter(), GetHullMins() * 0.5, GetHullMaxs() * 0.5, MASK_SOLID, this, COLLISION_GROUP_NONE, &tr );
			if ( tr.m_pEnt == GetEnemy() )
			{
				*pEntity = tr.m_pEnt;
				return true;
			}
		}
	}

	return false;
}


//-----------------------------------------------------------------------------
// While charging, look ahead and see if we're going to run into anything.
// If we are, start the gesture so it looks like we're anticipating the hit.
//-----------------------------------------------------------------------------
void CNPC_MiniStrider::ChargeLookAhead( void )
{
#if 0
	trace_t	tr;
	Vector vecForward;
	GetVectors( &vecForward, NULL, NULL );
	Vector vecTestPos = GetAbsOrigin() + ( vecForward * m_flGroundSpeed * 0.75 );
	Vector testHullMins = GetHullMins();
	testHullMins.z += (StepHeight() * 2);
	MiniStriderTraceHull_SkipPhysics( GetAbsOrigin(), vecTestPos, testHullMins, GetHullMaxs(), MASK_SHOT_HULL, this, COLLISION_GROUP_NONE, &tr, VPhysicsGetObject()->GetMass() * 0.5 );

	//NDebugOverlay::Box( tr.startpos, testHullMins, GetHullMaxs(), 0, 255, 0, true, 0.1f );
	//NDebugOverlay::Box( vecTestPos, testHullMins, GetHullMaxs(), 255, 0, 0, true, 0.1f );

	if ( tr.fraction != 1.0 )
	{
		// dvs: TODO:
		// Start playing the hit animation
		//AddGesture( ACT_MINTER_CHARGE_ANTICIPATION );
	}
#endif
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
float CNPC_MiniStrider::ChargeSteer()
{
	trace_t	tr;
	Vector	testPos, steer, forward, right;
	QAngle	angles;
	const float	testLength = m_flGroundSpeed * 0.15f;

	//Get our facing
	GetVectors( &forward, &right, NULL );

	steer = forward;

	const float faceYaw	= UTIL_VecToYaw( forward );

	//Offset right
	VectorAngles( forward, angles );
	angles[YAW] += 45.0f;
	AngleVectors( angles, &forward );

	// Probe out
	testPos = GetAbsOrigin() + ( forward * testLength );

	// Offset by step height
	Vector testHullMins = GetHullMins();
	testHullMins.z += (StepHeight() * 2);

	// Probe
	MiniStriderTraceHull_SkipPhysics( GetAbsOrigin(), testPos, testHullMins, GetHullMaxs(), MASK_NPCSOLID, this, COLLISION_GROUP_NONE, &tr, VPhysicsGetObject()->GetMass() * 0.5f );

	// Debug info
	if ( g_debug_ministrider_charge.GetInt() == 1 )
	{
		if ( tr.fraction == 1.0f )
		{
  			NDebugOverlay::BoxDirection( GetAbsOrigin(), testHullMins, GetHullMaxs() + Vector(testLength,0,0), forward, 0, 255, 0, 8, 0.1f );
   		}
   		else
   		{
  			NDebugOverlay::BoxDirection( GetAbsOrigin(), testHullMins, GetHullMaxs() + Vector(testLength,0,0), forward, 255, 0, 0, 8, 0.1f );
		}
	}

	// Add in this component
	steer += ( right * 0.5f ) * ( 1.0f - tr.fraction );

	// Offset left
	angles[YAW] -= 90.0f;
	AngleVectors( angles, &forward );

	// Probe out
	testPos = GetAbsOrigin() + ( forward * testLength );
	MiniStriderTraceHull_SkipPhysics( GetAbsOrigin(), testPos, testHullMins, GetHullMaxs(), MASK_NPCSOLID, this, COLLISION_GROUP_NONE, &tr, VPhysicsGetObject()->GetMass() * 0.5f );

	// Debug
	if ( g_debug_ministrider_charge.GetInt() == 1 )
	{
		if ( tr.fraction == 1.0f )
		{
			NDebugOverlay::BoxDirection( GetAbsOrigin(), testHullMins, GetHullMaxs() + Vector(testLength,0,0), forward, 0, 255, 0, 8, 0.1f );
		}
		else
		{
			NDebugOverlay::BoxDirection( GetAbsOrigin(), testHullMins, GetHullMaxs() + Vector(testLength,0,0), forward, 255, 0, 0, 8, 0.1f );
		}
	}

	// Add in this component
	steer -= ( right * 0.5f ) * ( 1.0f - tr.fraction );

	// Debug
	if ( g_debug_ministrider_charge.GetInt() == 1 )
	{
		NDebugOverlay::Line( GetAbsOrigin(), GetAbsOrigin() + ( steer * 512.0f ), 255, 255, 0, true, 0.1f );
		NDebugOverlay::Cross3D( GetAbsOrigin() + ( steer * 512.0f ), Vector(2,2,2), -Vector(2,2,2), 255, 255, 0, true, 0.1f );

		NDebugOverlay::Line( GetAbsOrigin(), GetAbsOrigin() + ( BodyDirection3D() * 256.0f ), 255, 0, 255, true, 0.1f );
		NDebugOverlay::Cross3D( GetAbsOrigin() + ( BodyDirection3D() * 256.0f ), Vector(2,2,2), -Vector(2,2,2), 255, 0, 255, true, 0.1f );
	}

	return UTIL_AngleDiff( UTIL_VecToYaw( steer ), faceYaw );
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CNPC_MiniStrider::ChargeDamage( CBaseEntity *pTarget )
{
	if ( pTarget == NULL )
		return;

	CBasePlayer *pPlayer = ToBasePlayer( pTarget );

	if ( pPlayer != NULL )
	{
		//Kick the player angles
		pPlayer->ViewPunch( QAngle( 20, 20, -30 ) );	

		Vector	dir = pPlayer->WorldSpaceCenter() - WorldSpaceCenter();
		VectorNormalize( dir );
		dir.z = 0.0f;
		
		Vector vecNewVelocity = dir * 250.0f;
		vecNewVelocity[2] += 128.0f;
		pPlayer->SetAbsVelocity( vecNewVelocity );

		color32 red = {128,0,0,128};
		UTIL_ScreenFade( pPlayer, red, 1.0f, 0.1f, FFADE_IN );
	}
	
	// Player takes less damage
	float flDamage = ( pPlayer == NULL ) ? 250 : sk_ministrider_dmg_charge.GetFloat();
	
	// If it's being held by the player, break that bond
	Pickup_ForcePlayerToDropThisObject( pTarget );

	// Calculate the physics force
	MiniStrider_ApplyChargeDamage( this, pTarget, flDamage );
}


//-----------------------------------------------------------------------------
// Handles the ministrider charging into something. Returns true if it hit the world.
//-----------------------------------------------------------------------------
bool CNPC_MiniStrider::HandleChargeImpact( Vector vecImpact, CBaseEntity *pEntity )
{
	// Cause a shock wave from this point which will disrupt nearby physics objects
	//ImpactShock( vecImpact, 128, 350 );

	// Did we hit anything interesting?
	if ( !pEntity || pEntity->IsWorld() )
	{
		// Robin: Due to some of the finicky details in the motor, the ministrider will hit
		//		  the world when it is blocked by our enemy when trying to step up 
		//		  during a moveprobe. To get around this, we see if the enemy's within
		//		  a volume in front of the ministrider when we hit the world, and if he is,
		//		  we hit him anyway.
		EnemyIsRightInFrontOfMe( &pEntity );

		// Did we manage to find him? If not, increment our charge miss count and abort.
		if ( pEntity->IsWorld() )
		{
			return true;
		}
	}

	// Hit anything we don't like
	if ( IRelationType( pEntity ) == D_HT && ( GetNextAttack() < gpGlobals->curtime ) )
	{
		EmitSound( "NPC_Hunter.ChargeHitEnemy" );

		// dvs: TODO:
		//if ( !IsPlayingGesture( ACT_MINTER_CHARGE_HIT ) )
		//{
		//	RestartGesture( ACT_MINTER_CHARGE_HIT );
		//}
		
		ChargeDamage( pEntity );

		if ( !pEntity->IsNPC() )
		{
			pEntity->ApplyAbsVelocityImpulse( ( BodyDirection2D() * 400 ) + Vector( 0, 0, 200 ) );
		}

		if ( !pEntity->IsAlive() && GetEnemy() == pEntity )
		{
			SetEnemy( NULL );
		}

		SetNextAttack( gpGlobals->curtime + 2.0f );

		if ( !pEntity->IsAlive() || !pEntity->IsNPC() )
		{
			SetIdealActivity( ACT_MINTER_CHARGE_STOP );
			return false;
		}
		else
			return true;

	}

	// Hit something we don't hate. If it's not moveable, crash into it.
	if ( pEntity->GetMoveType() == MOVETYPE_NONE || pEntity->GetMoveType() == MOVETYPE_PUSH )
	{		
		CBreakable *pBreakable = dynamic_cast<CBreakable *>(pEntity);
		if ( pBreakable  && pBreakable->IsBreakable() && pBreakable->m_takedamage == DAMAGE_YES && pBreakable->GetHealth() > 0 )
		{
			ChargeDamage( pEntity );
		}
		return true;
	}

	// If it's a vphysics object that's too heavy, crash into it too.
	if ( pEntity->GetMoveType() == MOVETYPE_VPHYSICS )
	{
		IPhysicsObject *pPhysics = pEntity->VPhysicsGetObject();
		if ( pPhysics )
		{
			// If the object is being held by the player, knock it out of his hands
			if ( pPhysics->GetGameFlags() & FVPHYSICS_PLAYER_HELD )
			{
				Pickup_ForcePlayerToDropThisObject( pEntity );
				return false;
			}

			if ( !pPhysics->IsMoveable() )
				return true;

			float entMass = PhysGetEntityMass( pEntity ) ;
			float minMass = VPhysicsGetObject()->GetMass() * 0.5f;
			if ( entMass < minMass )
			{
				if ( entMass < minMass * 0.666f || pEntity->CollisionProp()->BoundingRadius() < GetHullHeight() )
				{
					if ( pEntity->GetHealth() > 0 )
					{
						CBreakableProp *pBreakable = dynamic_cast<CBreakableProp *>(pEntity);
						if ( pBreakable && pBreakable->m_takedamage == DAMAGE_YES && pBreakable->GetHealth() > 0 && pBreakable->GetHealth() <= 50 )
						{
							ChargeDamage( pEntity );
						}
					}
					pEntity->SetNavIgnore( 2.0 );
					return false;
				}
			}
			return true;

		}
	}

	return false;
}


//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
void CNPC_MiniStrider::Explode()
{
	Vector			velocity = vec3_origin;
	AngularImpulse	angVelocity = RandomAngularImpulse( -150, 150 );

	PropBreakableCreateAll( GetModelIndex(), NULL, EyePosition(), GetAbsAngles(), velocity, angVelocity, 1.0, 150, COLLISION_GROUP_NPC, this );

	ExplosionCreate( EyePosition(), GetAbsAngles(), this, 500, 256, (SF_ENVEXPLOSION_NOPARTICLES|SF_ENVEXPLOSION_NOSPARKS|SF_ENVEXPLOSION_NODLIGHTS|SF_ENVEXPLOSION_NODAMAGE|SF_ENVEXPLOSION_NOSMOKE), false );

	// Create liquid fountain gushtacular effect here!
	CEffectData	data;

	data.m_vOrigin = EyePosition();
	data.m_vNormal = Vector( 0, 0, 1 );
	data.m_flScale = 4.0f;

	DispatchEffect( "StriderBlood", data );
	
	// Go away
	m_lifeState = LIFE_DEAD;

	SetThink( &CNPC_MiniStrider::SUB_Remove );
	SetNextThink( gpGlobals->curtime + 0.1f );

	AddEffects( EF_NODRAW );
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
Activity CNPC_MiniStrider::NPC_TranslateActivity( Activity baseAct )
{
	if ( ( baseAct == ACT_WALK ) || ( baseAct == ACT_RUN ) )
	{
  		if ( GetEnemy() )
  		{
			Vector vecEnemyLKP = GetEnemyLKP();
			
			// Only start facing when we're close enough
			if ( UTIL_DistApprox( vecEnemyLKP, GetAbsOrigin() ) < MINISTRIDER_FACE_ENEMY_DIST )
			{
				return (Activity)ACT_MINTER_WALK_ANGRY;
			}
		}
	}
	else if ( ( baseAct == ACT_IDLE ) && m_bPlanted )
	{
		return ( Activity )ACT_MINTER_IDLE_PLANTED;
	}
	else if ( baseAct == ACT_RANGE_ATTACK2 )
	{
		if ( !m_bPlanted && ( m_bEnableUnplantedShooting || IsStriderBuster( GetEnemy() ) ) )
		{
			return (Activity)ACT_MINTER_RANGE_ATTACK2_UNPLANTED;
		}
	}
	
	return BaseClass::NPC_TranslateActivity( baseAct );
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CNPC_MiniStrider::HandleAnimEvent( animevent_t *pEvent )
{
	Vector footPosition;
	QAngle angles;
	
	if ( pEvent->event == AE_MINTER_FOOTSTEP_LEFT )
	{
		LeftFootHit( pEvent->eventtime );
		return;
	}

	if ( pEvent->event == AE_MINTER_FOOTSTEP_RIGHT )
	{
		RightFootHit( pEvent->eventtime );
		return;
	}

	if ( pEvent->event == AE_MINTER_FOOTSTEP_BACK )
	{
		BackFootHit( pEvent->eventtime );
		return;
	}
	
	if ( pEvent->event == AE_MINTER_START_EXPRESSION )
	{
		if ( pEvent->options && Q_strlen( pEvent->options ) )
		{
			//m_iszCurrentExpression = AllocPooledString( pEvent->options );
			//SetExpression( pEvent->options );
		}
		return;
	}

	if ( pEvent->event == AE_MINTER_END_EXPRESSION )
	{
		if ( pEvent->options && Q_strlen( pEvent->options ) )
		{
			//m_iszCurrentExpression = NULL_STRING;
			//RemoveActorFromScriptedScenes( this, true, false, pEvent->options );
		}
		return;
	}

	if ( pEvent->event == AE_MINTER_MELEE_ANNOUNCE )
	{
		EmitSound( "NPC_Hunter.MeleeAnnounce" );
		return;
	}
		
	if ( pEvent->event == AE_MINTER_MELEE_ATTACK_LEFT )
	{
		Vector right, forward, dir;
		AngleVectors( GetLocalAngles(), &forward, &right, NULL );

		right = right * -100;
		forward = forward * 600;
		dir = right + forward;
		QAngle angle( 25, 30, -20 );

		MeleeAttack( MINISTRIDER_MELEE_REACH, sk_ministrider_dmg_one_slash.GetFloat(), angle, dir, MINISTRIDER_BLOOD_LEFT_FOOT );
		return;
	}

	if ( pEvent->event == AE_MINTER_MELEE_ATTACK_RIGHT )
	{
		Vector right, forward,dir;
		AngleVectors( GetLocalAngles(), &forward, &right, NULL );

		right = right * 100;
		forward = forward * 600;
		dir = right + forward;
		
		QAngle angle( 25, -30, 20 );

		MeleeAttack( MINISTRIDER_MELEE_REACH, sk_ministrider_dmg_one_slash.GetFloat(), angle, dir, MINISTRIDER_BLOOD_LEFT_FOOT );
		return;
	}

	if ( pEvent->event == AE_MINTER_SPRAY_BLOOD )
	{
		Vector vecOrigin;
		Vector vecDir;
	
		// spray blood from the attachment point
		bool bGotAttachment = false;
		if ( pEvent->options )
		{
			QAngle angDir;
			if ( GetAttachment( pEvent->options, vecOrigin, angDir ) )
			{
				bGotAttachment = true;
				AngleVectors( angDir, &vecDir, NULL, NULL );
			}
		}

		// fall back to our center, tracing forward
		if ( !bGotAttachment )
		{	
			vecOrigin = WorldSpaceCenter();
			GetVectors( &vecDir, NULL, NULL );
		}
		
		UTIL_BloodSpray( vecOrigin, vecDir, BLOOD_COLOR_RED, 4, FX_BLOODSPRAY_ALL );

		for ( int i = 0 ; i < 3 ; i++ )
		{
			Vector vecTraceDir = vecDir;
			vecTraceDir.x += random->RandomFloat( -0.1, 0.1 );
			vecTraceDir.y += random->RandomFloat( -0.1, 0.1 );
			vecTraceDir.z += random->RandomFloat( -0.1, 0.1 );

			trace_t tr;
			AI_TraceLine( vecOrigin, vecOrigin + ( vecTraceDir * 192.0f ), MASK_SOLID_BRUSHONLY, this, COLLISION_GROUP_NONE, &tr );
			if ( tr.fraction != 1.0 )
			{
				UTIL_BloodDecalTrace( &tr, BLOOD_COLOR_RED );
			}
		}

		return;
	}

	BaseClass::HandleAnimEvent( pEvent );
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CNPC_MiniStrider::AddEntityRelationship( CBaseEntity *pEntity, Disposition_t nDisposition, int nPriority )
{
	if ( nDisposition ==  D_HT && pEntity->ClassMatches("npc_bullseye") )
		UpdateEnemyMemory( pEntity, pEntity->GetAbsOrigin() );
	BaseClass::AddEntityRelationship( pEntity, nDisposition, nPriority );
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
bool CNPC_MiniStrider::ScheduledMoveToGoalEntity( int scheduleType, CBaseEntity *pGoalEntity, Activity movementActivity )
{
	if ( IsCurSchedule( SCHED_MINISTRIDER_RANGE_ATTACK1, false ) )
	{
		SetGoalEnt( pGoalEntity );
		return true;
	}
	return BaseClass::ScheduledMoveToGoalEntity( scheduleType, pGoalEntity, movementActivity );
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CNPC_MiniStrider::OnChangeHintGroup( string_t oldGroup, string_t newGroup )
{
	SetCondition( COND_MINISTRIDER_NEW_HINTGROUP );
	m_CheckHintGroupTimer.Set( 10 );
}


//-----------------------------------------------------------------------------
// Tells whether any given ministrider is in a squad that contains other ministriders.
// This is useful for preventing timid behavior for MiniStriders that are not 
// supported by other ministriders.
//
// NOTE:	This counts the self! So a ministrider that is alone in his squad
//			receives a result of 1.
//-----------------------------------------------------------------------------
int CNPC_MiniStrider::NumMiniStridersInMySquad()
{
	AISquadIter_t iter;
	CAI_BaseNPC *pSquadmate = m_pSquad ? m_pSquad->GetFirstMember( &iter ) : NULL;

	if( !pSquadmate )
	{
		// Not in a squad at all, but the caller is not concerned with that. Just
		// tell them that we're in a squad of one (ourself)
		return 1;
	}
	
	int count = 0;

	while ( pSquadmate )
	{
		if( pSquadmate->m_iClassname == m_iClassname )
			count++;

		pSquadmate = m_pSquad->GetNextMember( &iter );
	}

	return count;
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CNPC_MiniStrider::FollowStrider( const char *szStrider )
{
	if ( !szStrider )
		return;

	CBaseEntity *pEnt = gEntList.FindEntityByName( NULL, szStrider, this );
	CNPC_Strider *pStrider = dynamic_cast <CNPC_Strider *>( pEnt );
	FollowStrider(pStrider);
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CNPC_MiniStrider::FollowStrider( CNPC_Strider * pStrider )
{
	if ( !IsAlive() )
	{
		return;
	}

	if ( pStrider )
	{
		if ( m_EscortBehavior.GetFollowTarget() != pStrider )
		{
			m_iszFollowTarget = pStrider->GetEntityName();
			if ( m_iszFollowTarget == NULL_STRING )
			{
				m_iszFollowTarget = AllocPooledString( "unnamed_strider" );
			}
			m_EscortBehavior.SetEscortTarget( pStrider );
		}
	}
	else
	{
		DevWarning("MiniStrider set to follow entity %s that is not a strider\n", STRING( m_iszFollowTarget ) );
		m_iszFollowTarget = AllocPooledString( "unknown_strider" );
	}
}

void CAI_MiniStriderEscortBehavior::SetEscortTarget( CNPC_Strider *pStrider, bool fFinishCurSchedule )
{
	m_bEnabled = true;

	if ( GetOuter()->GetSquad() )
	{
		GetOuter()->GetSquad()->RemoveFromSquad( GetOuter() );
	}

	for ( int i = 0; i < g_MiniStriders.Count(); i++ )
	{
		if ( g_MiniStriders[i]->m_EscortBehavior.GetFollowTarget() == pStrider )
		{
			Assert( g_MiniStriders[i]->GetSquad() );
			g_MiniStriders[i]->GetSquad()->AddToSquad( GetOuter() );
			break;
		}
	}

	if ( !GetOuter()->GetSquad() )
	{
		GetOuter()->AddToSquad( AllocPooledString( CFmtStr( "%s_ministrider_squad", STRING( pStrider->GetEntityName() ) ) ) );
	}

	BaseClass::SetFollowTarget( pStrider );
	m_flTimeEscortReturn = gpGlobals->curtime;
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CNPC_MiniStrider::InputEnableUnplantedShooting( inputdata_t &inputdata )
{
	m_bEnableUnplantedShooting = true;
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CNPC_MiniStrider::InputDisableUnplantedShooting( inputdata_t &inputdata )
{
	m_bEnableUnplantedShooting = false;
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CNPC_MiniStrider::InputFollowStrider( inputdata_t &inputdata )
{
	m_iszFollowTarget = inputdata.value.StringID();
	if ( m_iszFollowTarget == s_iszStriderClassname )
	{
		m_EscortBehavior.m_bEnabled = true;
		m_iszFollowTarget = NULL_STRING;
	}
	m_BeginFollowDelay.Start( .1 ); // Allow time for strider to spawn
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CNPC_MiniStrider::InputUseSiegeTargets( inputdata_t &inputdata )
{
	m_iszSiegeTargetName = inputdata.value.StringID();
	m_flTimeNextSiegeTargetAttack = gpGlobals->curtime + random->RandomFloat( 1, ministrider_siege_frequency.GetFloat() );

	if( m_iszSiegeTargetName == NULL_STRING )
	{
		// Turning the feature off. Restore m_flDistTooFar to default.
		m_flDistTooFar = ministrider_ar2round_max_range.GetFloat();
		m_pSiegeTargets.RemoveAll();
	}
	else
	{
		// We're going into siege mode. Adjust range accordingly.
		m_flDistTooFar = ministrider_ar2round_max_range.GetFloat() * MINISTRIDER_SIEGE_MAX_DIST_MODIFIER;
	}
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CNPC_MiniStrider::InputDodge( inputdata_t &inputdata )
{
	SetCondition( COND_MINISTRIDER_FORCED_DODGE );
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CNPC_MiniStrider::InputFlankEnemy( inputdata_t &inputdata )
{
	SetCondition( COND_MINISTRIDER_FORCED_FLANK_ENEMY );
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CNPC_MiniStrider::InputDisableShooting( inputdata_t &inputdata )
{
	m_bDisableShooting = true;
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CNPC_MiniStrider::InputEnableShooting( inputdata_t &inputdata )
{
	m_bDisableShooting = false;
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CNPC_MiniStrider::InputEnableSquadShootDelay( inputdata_t &inputdata )
{
	m_bEnableSquadShootDelay = true;
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CNPC_MiniStrider::InputDisableSquadShootDelay( inputdata_t &inputdata )
{
	m_bEnableSquadShootDelay = false;
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
bool CNPC_MiniStrider::FVisible( CBaseEntity *pEntity, int traceMask, CBaseEntity **ppBlocker )
{
	return BaseClass::FVisible( pEntity, traceMask, ppBlocker );
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
bool CNPC_MiniStrider::IsValidEnemy( CBaseEntity *pTarget )
{
	if ( IsStriderBuster( pTarget) )
	{
		if ( !m_EscortBehavior.m_bEnabled || !m_EscortBehavior.GetEscortTarget() )
		{
			// We only hate striderbusters when we are actively protecting a strider.
			return false;
		}

		if ( pTarget->VPhysicsGetObject() )
		{
			if ( ( pTarget->VPhysicsGetObject()->GetGameFlags() & FVPHYSICS_PLAYER_HELD ) &&
				ministrider_hate_held_striderbusters.GetBool() )
			{
				if ( gpGlobals->curtime - StriderBuster_GetPickupTime( pTarget ) > ministrider_hate_held_striderbusters_delay.GetFloat())
				{
					if ( StriderBuster_NumFlechettesAttached( pTarget ) <= 2 )
					{
						if ( m_EscortBehavior.GetEscortTarget() && 
							( m_EscortBehavior.GetEscortTarget()->GetAbsOrigin().AsVector2D() - pTarget->GetAbsOrigin().AsVector2D() ).LengthSqr() < Square( ministrider_hate_held_striderbusters_tolerance.GetFloat() ) )
						{
							return true;
						}
					}
				}
				return false;
			}

			bool bThrown = ( pTarget->VPhysicsGetObject()->GetGameFlags() & FVPHYSICS_WAS_THROWN ) != 0;
			bool bAttached = StriderBuster_IsAttachedStriderBuster( pTarget );

			if ( ( bThrown && !bAttached ) && ministrider_hate_thrown_striderbusters.GetBool() )
			{
				float t;
				float dist = CalcDistanceSqrToLineSegment2D( m_EscortBehavior.GetEscortTarget()->GetAbsOrigin().AsVector2D(), 
					pTarget->GetAbsOrigin().AsVector2D(), 
					pTarget->GetAbsOrigin().AsVector2D() + pTarget->GetSmoothedVelocity().AsVector2D(), &t );

				if ( t > 0 && dist < Square( ministrider_hate_thrown_striderbusters_tolerance.GetFloat() ))
				{
					return true;
				}
				return false;
			}

			if ( bAttached && StriderBuster_IsAttachedStriderBuster( pTarget, m_EscortBehavior.GetEscortTarget() ) && ministrider_hate_attached_striderbusters.GetBool() )
			{
				return true;
			}
		}
		return false;
	}

	return BaseClass::IsValidEnemy( pTarget );
}



//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
Disposition_t CNPC_MiniStrider::IRelationType( CBaseEntity *pTarget )
{
	if ( !pTarget )
		return D_NU;

	if ( IsStriderBuster( pTarget ) )
	{
		if ( HateThisStriderBuster( pTarget ) )
			return D_HT;

		return D_NU;
	}

	if ( ministrider_retreat_striderbusters.GetBool() )
	{
		if ( pTarget->IsPlayer() && (m_hAttachedBusters.Count() > 0) )
		{
			return D_FR;
		}
	}
		
	return BaseClass::IRelationType( pTarget );
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
int CNPC_MiniStrider::IRelationPriority( CBaseEntity *pTarget )
{
	if ( IsStriderBuster( pTarget ) )
	{
		// If we're here, we already know that we hate striderbusters.
		return 1000.0f;
	}

	return BaseClass::IRelationPriority( pTarget );
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CNPC_MiniStrider::SetSquad( CAI_Squad *pSquad )
{
	BaseClass::SetSquad( pSquad );
	if ( pSquad && pSquad->NumMembers() == 1 )
	{
		pSquad->SetSquadData( MINISTRIDER_RUNDOWN_SQUADDATA, 0 );
	}
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CNPC_MiniStrider::OnSeeEntity( CBaseEntity *pEntity )
{
	BaseClass::OnSeeEntity(pEntity);

	if ( IsStriderBuster( pEntity ) && IsValidEnemy( pEntity ) )
	{
		SetCondition( COND_MINISTRIDER_SEE_STRIDERBUSTER );
	}
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
bool CNPC_MiniStrider::UpdateEnemyMemory( CBaseEntity *pEnemy, const Vector &position, CBaseEntity *pInformer )
{
	//EmitSound( "NPC_Hunter.Alert" );
	return BaseClass::UpdateEnemyMemory( pEnemy, position, pInformer );
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
bool CNPC_MiniStrider::CanPlantHere( const Vector &vecPos )
{
	// TODO: cache results?
	//if ( vecPos == m_vecLastCanPlantHerePos )
	//{
	//	return m_bLastCanPlantHere;
	//}

	Vector vecMins = GetHullMins();
	Vector vecMaxs = GetHullMaxs();
	
	vecMins.x -= 16;
	vecMins.y -= 16;

	vecMaxs.x += 16;
	vecMaxs.y += 16;
	vecMaxs.z -= ministrider_plant_adjust_z.GetInt();
	
	bool bResult = false;

	trace_t tr;
	UTIL_TraceHull( vecPos, vecPos, vecMins, vecMaxs, MASK_NPCSOLID, this, COLLISION_GROUP_NONE, &tr );
	if ( tr.startsolid )
	{
		// Try again, tracing down from above.
		Vector vecStart = vecPos;
		vecStart.z += ministrider_plant_adjust_z.GetInt();
		
		UTIL_TraceHull( vecStart, vecPos, vecMins, vecMaxs, MASK_NPCSOLID, this, COLLISION_GROUP_NONE, &tr );
	}
	
	if ( tr.startsolid )
	{
		//NDebugOverlay::Box( vecPos, vecMins, vecMaxs, 255, 0, 0, 0, 0 );
	}
	else
	{	
		//NDebugOverlay::Box( vecPos, vecMins, vecMaxs, 0, 255, 0, 0, 0 );
		bResult = true;
	}

	// Cache the results in case we ask again for the same spot.	
	//m_vecLastCanPlantHerePos = vecPos;
	//m_bLastCanPlantHere = bResult;

	return bResult;
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
int CNPC_MiniStrider::MeleeAttack1ConditionsVsEnemyInVehicle( CBaseCombatCharacter *pEnemy, float flDot )
{
	if( !IsCorporealEnemy( GetEnemy() ) )
		return COND_NONE;

	// Try and trace a box to the player, and if I hit the vehicle, attack it
	Vector vecDelta = (pEnemy->WorldSpaceCenter() - WorldSpaceCenter());
	VectorNormalize( vecDelta );
	trace_t	tr;
	AI_TraceHull( WorldSpaceCenter(), WorldSpaceCenter() + (vecDelta * 64), -Vector(8,8,8), Vector(8,8,8), MASK_SHOT, this, COLLISION_GROUP_NONE, &tr );
	if ( tr.fraction != 1.0 && tr.m_pEnt == pEnemy->GetVehicleEntity() )
	{
		// We're near the vehicle. Are we facing it?
		if (flDot < 0.7)
			return COND_NOT_FACING_ATTACK;

		return COND_CAN_MELEE_ATTACK1;
	}

	return COND_TOO_FAR_TO_ATTACK;
}


//-----------------------------------------------------------------------------
// For innate melee attack
//-----------------------------------------------------------------------------
int CNPC_MiniStrider::MeleeAttack1Conditions ( float flDot, float flDist )
{
	if ( !IsCorporealEnemy( GetEnemy() ) )
		return COND_NONE;
		
	if ( ( gpGlobals->curtime < m_flNextMeleeTime ) && // allow berzerk bashing if cornered
		!( m_hAttachedBusters.Count() > 0 && gpGlobals->curtime < m_fCorneredTimer ) )
	{
		return COND_NONE;
	}

	if ( GetEnemy()->Classify() == CLASS_PLAYER_ALLY_VITAL )
	{
		return COND_NONE;
	}
		
	if ( flDist > MINISTRIDER_MELEE_REACH )
	{
		// Translate a hit vehicle into its passenger if found
		if ( GetEnemy() != NULL )
		{
			CBaseCombatCharacter *pCCEnemy = GetEnemy()->MyCombatCharacterPointer();
			if ( pCCEnemy != NULL && pCCEnemy->IsInAVehicle() )
			{
				return MeleeAttack1ConditionsVsEnemyInVehicle( pCCEnemy, flDot );
			}

#if defined(HL2_DLL) && !defined(HL2MP)
			// If the player is holding an object, knock it down.
			if ( GetEnemy()->IsPlayer() )
			{
				CBasePlayer *pPlayer = ToBasePlayer( GetEnemy() );

				Assert( pPlayer != NULL );

				// Is the player carrying something?
				CBaseEntity *pObject = GetPlayerHeldEntity(pPlayer);

				if ( !pObject )
				{
					pObject = PhysCannonGetHeldEntity( pPlayer->GetActiveWeapon() );
				}

				if ( pObject )
				{
					float flDist = pObject->WorldSpaceCenter().DistTo( WorldSpaceCenter() );

					if ( flDist <= MINISTRIDER_MELEE_REACH )
					{
						return COND_CAN_MELEE_ATTACK1;
					}
				}
			}
#endif
		}
	
		return COND_TOO_FAR_TO_ATTACK;
	}

	if (flDot < 0.7)
	{
		return COND_NOT_FACING_ATTACK;
	}

	// Build a cube-shaped hull, the same hull that MeleeAttack is going to use.
	Vector vecMins = GetHullMins();
	Vector vecMaxs = GetHullMaxs();
	vecMins.z = vecMins.x;
	vecMaxs.z = vecMaxs.x;

	Vector forward;
	GetVectors( &forward, NULL, NULL );

	trace_t	tr;
	AI_TraceHull( WorldSpaceCenter(), WorldSpaceCenter() + forward * MINISTRIDER_MELEE_REACH, vecMins, vecMaxs, MASK_NPCSOLID, this, COLLISION_GROUP_NONE, &tr );

	if ( tr.fraction == 1.0 || !tr.m_pEnt )
	{
		// This attack would miss completely. Trick the ministrider into moving around some more.
		return COND_TOO_FAR_TO_ATTACK;
	}

	if ( tr.m_pEnt == GetEnemy() || tr.m_pEnt->IsNPC() || (tr.m_pEnt->m_takedamage == DAMAGE_YES && (dynamic_cast<CBreakableProp*>(tr.m_pEnt))) )
	{
		// Let the ministrider swipe at his enemy if he's going to hit them.
		// Also let him swipe at NPC's that happen to be between the ministrider and the enemy. 
		// This makes mobs of ministriders seem more rowdy since it doesn't leave guys in the back row standing around.
		// Also let him swipe at things that takedamage, under the assumptions that they can be broken.
		return COND_CAN_MELEE_ATTACK1;
	}

	// dvs TODO: incorporate this
	/*if ( tr.m_pEnt->IsBSPModel() )
	{
		// The trace hit something solid, but it's not the enemy. If this item is closer to the ministrider than
		// the enemy is, treat this as an obstruction.
		Vector vecToEnemy = GetEnemy()->WorldSpaceCenter() - WorldSpaceCenter();
		Vector vecTrace = tr.endpos - tr.startpos;

		if ( vecTrace.Length2DSqr() < vecToEnemy.Length2DSqr() )
		{
			return COND_MINISTRIDER_LOCAL_MELEE_OBSTRUCTION;
		}
	}*/

	if ( !tr.m_pEnt->IsWorld() && GetEnemy() && GetEnemy()->GetGroundEntity() == tr.m_pEnt )
	{
		// Try to swat whatever the player is standing on instead of acting like a dill.
		return COND_CAN_MELEE_ATTACK1;
	}

	// Move around some more
	return COND_TOO_FAR_TO_ATTACK;
}


//-----------------------------------------------------------------------------
// For innate melee attack
//-----------------------------------------------------------------------------
int CNPC_MiniStrider::MeleeAttack2Conditions ( float flDot, float flDist )
{
	return COND_NONE;
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
bool CNPC_MiniStrider::IsCorporealEnemy( CBaseEntity *pEnemy ) 
{
	if( !pEnemy )
		return false;

	// Generally speaking, don't melee attack anything the player can't see.
	if( pEnemy->IsEffectActive( EF_NODRAW ) )
		return false;

	// Don't flank, melee attack striderbusters.
	if ( IsStriderBuster( pEnemy ) )
		return false;

	return true;
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
int CNPC_MiniStrider::RangeAttack1Conditions( float flDot, float flDist )
{
	return COND_NONE;
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
int CNPC_MiniStrider::RangeAttack2Conditions( float flDot, float flDist )
{
	bool bIsBuster = IsStriderBuster( GetEnemy() );
	bool bIsPerfectBullseye = ( GetEnemy() && dynamic_cast<CNPC_Bullseye *>(GetEnemy()) && ((CNPC_Bullseye *)GetEnemy())->UsePerfectAccuracy() );

	if ( !bIsPerfectBullseye && !bIsBuster && !ministrider_ar2round_test.GetBool() && ( gpGlobals->curtime < m_flNextRangeAttack2Time ) )
	{
		return COND_NONE;
	}

	if ( m_bDisableShooting )
	{
		return COND_NONE;
	}

	if ( !HasCondition( COND_SEE_ENEMY ) )
	{
		return COND_NONE;
	}

	float flMaxFlechetteRange = ministrider_ar2round_max_range.GetFloat();

	if ( IsUsingSiegeTargets() )
	{
		flMaxFlechetteRange *= MINISTRIDER_SIEGE_MAX_DIST_MODIFIER;
	}

 	if ( !bIsBuster && ( flDist > flMaxFlechetteRange ) )
	{
		return COND_TOO_FAR_TO_ATTACK;
	}
	else if ( !bIsBuster && ( !GetEnemy() || !GetEnemy()->ClassMatches( "npc_bullseye" ) ) && flDist < ministrider_ar2round_min_range.GetFloat() )
	{
		return COND_TOO_CLOSE_TO_ATTACK;
	}
	else if ( flDot < MINISTRIDER_FACING_DOT )
	{
		return COND_NOT_FACING_ATTACK;
	}
	
	if ( !bIsBuster && !m_bEnableUnplantedShooting && !ministrider_ar2round_test.GetBool() && !CanPlantHere( GetAbsOrigin() ) )
	{
		return COND_MINISTRIDER_CANT_PLANT;
	}

	return COND_CAN_RANGE_ATTACK2;
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
bool CNPC_MiniStrider::WeaponLOSCondition(const Vector &ownerPos, const Vector &targetPos, bool bSetConditions)
{
	CBaseEntity *pTargetEnt;

	pTargetEnt = GetEnemy();

	trace_t tr;
	Vector vFrom = ownerPos + GetViewOffset();
	AI_TraceLine( vFrom, targetPos, MASK_SHOT, this, COLLISION_GROUP_NONE, &tr );

	if ( ( pTargetEnt && tr.m_pEnt == pTargetEnt) || tr.fraction == 1.0 || CanShootThrough( tr, targetPos ) )
	{
		static Vector vMins( -2.0, -2.0, -2.0 );
		static Vector vMaxs( -vMins);
		// Hit the enemy, or hit nothing (traced all the way to a nonsolid enemy like a bullseye)
		AI_TraceHull( vFrom - Vector( 0, 0, 18 ), targetPos, vMins, vMaxs, MASK_SHOT, this, COLLISION_GROUP_NONE, &tr);

		if ( ( pTargetEnt && tr.m_pEnt == pTargetEnt) || tr.fraction == 1.0 || CanShootThrough( tr, targetPos ) )
		{
			if ( ministrider_show_weapon_los_condition.GetBool() )
			{
				NDebugOverlay::Line( vFrom, targetPos, 255, 0, 255, false, 0.1 );
				NDebugOverlay::Line( vFrom - Vector( 0, 0, 18 ), targetPos, 0, 0, 255, false, 0.1 );
			}
			return true;
		}
	}
	else if ( bSetConditions )
	{
		SetCondition( COND_WEAPON_SIGHT_OCCLUDED );
		SetEnemyOccluder( tr.m_pEnt );
	}

	return false;
}

//-----------------------------------------------------------------------------
// Look in front and see if the claw hit anything.
//
// Input  :	flDist				distance to trace		
//			iDamage				damage to do if attack hits
//			vecViewPunch		camera punch (if attack hits player)
//			vecVelocityPunch	velocity punch (if attack hits player)
//
// Output : The entity hit by claws. NULL if nothing.
//-----------------------------------------------------------------------------
CBaseEntity *CNPC_MiniStrider::MeleeAttack( float flDist, int iDamage, QAngle &qaViewPunch, Vector &vecVelocityPunch, int BloodOrigin  )
{
	// Added test because claw attack anim sometimes used when for cases other than melee
	if ( GetEnemy() )
	{
		trace_t	tr;
		AI_TraceHull( WorldSpaceCenter(), GetEnemy()->WorldSpaceCenter(), -Vector(8,8,8), Vector(8,8,8), MASK_SOLID_BRUSHONLY, this, COLLISION_GROUP_NONE, &tr );

		if ( tr.fraction < 1.0f )
			return NULL;
	}

	//
	// Trace out a cubic section of our hull and see what we hit.
	//
	Vector vecMins = GetHullMins();
	Vector vecMaxs = GetHullMaxs();
	vecMins.z = vecMins.x;
	vecMaxs.z = vecMaxs.x;

	CBaseEntity *pHurt = CheckTraceHullAttack( flDist, vecMins, vecMaxs, iDamage, DMG_SLASH );

	if ( pHurt )
	{
		EmitSound( "NPC_Hunter.MeleeHit" );
		EmitSound( "NPC_Hunter.TackleHit" );

		CBasePlayer *pPlayer = ToBasePlayer( pHurt );

		if ( pPlayer != NULL && !(pPlayer->GetFlags() & FL_GODMODE ) )
		{
			pPlayer->ViewPunch( qaViewPunch );
			pPlayer->VelocityPunch( vecVelocityPunch );
			
			// Shake the screen
			UTIL_ScreenShake( pPlayer->GetAbsOrigin(), 100.0, 1.5, 1.0, 2, SHAKE_START );

			// Red damage indicator
			color32 red = { 128, 0, 0, 128 };
			UTIL_ScreenFade( pPlayer, red, 1.0f, 0.1f, FFADE_IN );

			/*if ( UTIL_ShouldShowBlood( pPlayer->BloodColor() ) )
			{
				// Spray some of the player's blood on the ministrider.			
				trace_t tr;
				
				Vector vecMiniStriderEyePos; // = EyePosition();
				QAngle angDiscard;
				GetBonePosition( LookupBone( "MiniStrider.top_eye_bone" ), vecMiniStriderEyePos, angDiscard );

				Vector vecPlayerEyePos = pPlayer->EyePosition();
				
				Vector vecDir = vecMiniStriderEyePos - vecPlayerEyePos;
				float flLen = VectorNormalize( vecDir );
				
				Vector vecStart = vecPlayerEyePos - ( vecDir * 64 );
				Vector vecEnd = vecPlayerEyePos + ( vecDir * ( flLen + 64 ) );
				
				NDebugOverlay::HorzArrow( vecStart, vecEnd, 16, 255, 255, 0, 255, false, 10 );
				
				UTIL_TraceLine( vecStart, vecEnd, MASK_SHOT, pPlayer, COLLISION_GROUP_NONE, &tr );
	
				if ( tr.m_pEnt )
				{
					Msg( "Hit %s!!!\n", tr.m_pEnt->GetDebugName() );
					UTIL_DecalTrace( &tr, "Blood" );
				}
			}*/
		}
		else if ( !pPlayer )
		{
			if ( IsMovablePhysicsObject( pHurt ) )
			{
				// If it's a vphysics object that's too heavy, crash into it too.
				IPhysicsObject *pPhysics = pHurt->VPhysicsGetObject();
				if ( pPhysics )
				{
					// If the object is being held by the player, break it or make them drop it.
					if ( pPhysics->GetGameFlags() & FVPHYSICS_PLAYER_HELD )
					{
						// If it's breakable, break it.
						if ( pHurt->m_takedamage == DAMAGE_YES )
						{
							CBreakableProp *pBreak = dynamic_cast<CBreakableProp*>(pHurt);
							if ( pBreak )
							{
								CTakeDamageInfo info( this, this, 20, DMG_SLASH );
								pBreak->Break( this, info );
							}
						}
					}
				}
			}		
		
			if ( UTIL_ShouldShowBlood(pHurt->BloodColor()) )
			{
				// Hit an NPC. Bleed them!
				Vector vecBloodPos;

				switch ( BloodOrigin )
				{
					case MINISTRIDER_BLOOD_LEFT_FOOT:
					{
						if ( GetAttachment( "blood_left", vecBloodPos ) )
						{
							SpawnBlood( vecBloodPos, g_vecAttackDir, pHurt->BloodColor(), MIN( iDamage, 30 ) );
						}
						
						break;
					}
				}
			}
		}
	}
	else 
	{
		// TODO:
		//AttackMissSound();
	}

	m_flNextMeleeTime = gpGlobals->curtime + ministrider_melee_delay.GetFloat();

	return pHurt;
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
bool CNPC_MiniStrider::TestShootPosition(const Vector &vecShootPos, const Vector &targetPos )	
{ 
	if ( !CanPlantHere(vecShootPos ) )
	{
		return false;
	}

	return BaseClass::TestShootPosition( vecShootPos, targetPos );
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
Vector CNPC_MiniStrider::Weapon_ShootPosition( )
{
	matrix3x4_t gunMatrix;
	GetAttachment( gm_nTopGunAttachment, gunMatrix );

	Vector vecShootPos;
	MatrixGetColumn( gunMatrix, 3, vecShootPos );

	return vecShootPos;
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CNPC_MiniStrider::MakeTracer( const Vector &vecTracerSrc, const trace_t &tr, int iTracerType )
{
	float flTracerDist;
	Vector vecDir;
	Vector vecEndPos;

	vecDir = tr.endpos - vecTracerSrc;

	flTracerDist = VectorNormalize( vecDir );

	int nAttachment = LookupAttachment( "MiniGun" );

	UTIL_Tracer( vecTracerSrc, tr.endpos, nAttachment, TRACER_FLAG_USEATTACHMENT, 5000, true, "MiniStriderTracer" );
}


//-----------------------------------------------------------------------------
// Trace didn't hit the intended target, but should the ministrider
// shoot anyway? We use this to get the ministrider to destroy 
// breakables that are between him and his target.
//-----------------------------------------------------------------------------
bool CNPC_MiniStrider::CanShootThrough( const trace_t &tr, const Vector &vecTarget )
{
	if ( !tr.m_pEnt )
	{
		return false;
	}

	if ( !tr.m_pEnt->GetHealth() )
	{
		return false;
	}
	
	// Don't try to shoot through allies.
	CAI_BaseNPC *pNPC = tr.m_pEnt->MyNPCPointer();
	if ( pNPC && ( IRelationType( pNPC ) == D_LI ) )
	{
		return false;
	}

	// Would a trace ignoring this entity continue to the target?
	trace_t continuedTrace;
	AI_TraceLine( tr.endpos, vecTarget, MASK_SHOT, tr.m_pEnt, COLLISION_GROUP_NONE, &continuedTrace );

	if ( continuedTrace.fraction != 1.0 )
	{
		if ( continuedTrace.m_pEnt != GetEnemy() )
		{
			return false;
		}
	}

	return true;
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
int CNPC_MiniStrider::GetSoundInterests()
{
	return SOUND_WORLD | SOUND_COMBAT | SOUND_PLAYER | SOUND_DANGER | SOUND_PHYSICS_DANGER | SOUND_PLAYER_VEHICLE | SOUND_BULLET_IMPACT | SOUND_MOVE_AWAY;
}

//-----------------------------------------------------------------------------
// Tells us whether the MiniStrider is acting in a large, outdoor map, 
// currently only ep2_outland_12. This allows us to create logic
// branches here in the AI code so that we can make choices that
// tailor behavior to larger and smaller maps.
//-----------------------------------------------------------------------------
bool CNPC_MiniStrider::IsInLargeOutdoorMap()
{
	return m_bInLargeOutdoorMap;
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CNPC_MiniStrider::AlertSound()
{
	EmitSound( "NPC_Hunter.Alert" );
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CNPC_MiniStrider::PainSound( const CTakeDamageInfo &info )
{
	if (IsOnFire())
		return;

	if ( gpGlobals->curtime > m_flNextDamageTime )
	{
		EmitSound( "NPC_Hunter.Pain" );
		m_flNextDamageTime = gpGlobals->curtime + random->RandomFloat( 0.5, 1.2 ); 
	}
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CNPC_MiniStrider::DeathSound( const CTakeDamageInfo &info )
{
	if (IsOnFire())
		return;

	EmitSound( "NPC_Hunter.Death" );
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CNPC_MiniStrider::TraceAttack( const CTakeDamageInfo &inputInfo, const Vector &vecDir, trace_t *ptr, CDmgAccumulator *pAccumulator )
{
	CTakeDamageInfo info = inputInfo;

	// Even though the damage might not hurt us, we want to react to it
	// if it's from the player.
	if ( info.GetAttacker()->IsPlayer() )
	{
		if ( !HasMemory( bits_MEMORY_PROVOKED ) )
		{
			GetEnemies()->ClearMemory( info.GetAttacker() );
			Remember( bits_MEMORY_PROVOKED );
			SetCondition( COND_LIGHT_DAMAGE );
		}
	}

	// HUnters have special resisitance to some types of damage.
	if ( ( info.GetDamageType() & DMG_BULLET ) ||
		 ( info.GetDamageType() & DMG_BUCKSHOT ) ||
		 ( info.GetDamageType() & DMG_CLUB ) ||
		 ( info.GetDamageType() & DMG_NEVERGIB ) )
	{
		float flScale = 1.0;
		
		if ( info.GetDamageType() & DMG_BUCKSHOT )
		{
			flScale = sk_ministrider_buckshot_damage_scale.GetFloat();
		}
		else if ( ( info.GetDamageType() & DMG_BULLET ) || ( info.GetDamageType() & DMG_NEVERGIB ) )
		{
			// MiniStriders resist most bullet damage, but they are actually vulnerable to .357 rounds, 
			// since players regard that weapon as one of the game's truly powerful weapons.
			if( info.GetAmmoType() == GetAmmoDef()->Index("357") )
			{
				flScale = 1.16f;
			}
			else
			{
				flScale = sk_ministrider_bullet_damage_scale.GetFloat();
			}
		}

		if ( GetActivity() == ACT_MINTER_CHARGE_RUN )
		{
			flScale *= sk_ministrider_charge_damage_scale.GetFloat();
		}
		
		if ( flScale != 0 )
		{
			float flDamage = info.GetDamage() * flScale;
			info.SetDamage( flDamage );
		}
		
		QAngle vecAngles;
		VectorAngles( ptr->plane.normal, vecAngles );
		DispatchParticleEffect( "blood_impact_synth_01", ptr->endpos, vecAngles );
		DispatchParticleEffect( "blood_impact_synth_01_arc_parent", PATTACH_POINT_FOLLOW, this, gm_nHeadCenterAttachment );
	}

	BaseClass::TraceAttack( info, vecDir, ptr, pAccumulator );
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
const impactdamagetable_t &CNPC_MiniStrider::GetPhysicsImpactDamageTable()
{
	return s_MiniStriderImpactDamageTable;
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CNPC_MiniStrider::PhysicsDamageEffect( const Vector &vecPos, const Vector &vecDir )
{
	CEffectData data;
	data.m_vOrigin = vecPos;
	data.m_vNormal = vecDir;
	DispatchEffect( "MiniStriderDamage", data );

	if ( random->RandomInt( 0, 1 ) == 0 )
	{
		CBaseEntity *pTrail = CreateEntityByName( "sparktrail" );
		pTrail->SetOwnerEntity( this );
		pTrail->Spawn();
	}
}


//-----------------------------------------------------------------------------
// We were hit by a strider buster. Do the tesla effect on our hitboxes.
//-----------------------------------------------------------------------------
void CNPC_MiniStrider::TeslaThink()
{
	CEffectData data;
	data.m_nEntIndex = entindex();
	data.m_flMagnitude = 3;
	data.m_flScale = 0.5f;
	DispatchEffect( "TeslaHitboxes", data );
	EmitSound( "RagdollBoogie.Zap" );

	if ( gpGlobals->curtime < m_flTeslaStopTime )
	{
		SetContextThink( &CNPC_MiniStrider::TeslaThink, gpGlobals->curtime + random->RandomFloat( 0.1f, 0.3f ), MINISTRIDER_ZAP_THINK ); 
	}
}


//-----------------------------------------------------------------------------
// Our health is low. Show damage effects.
//-----------------------------------------------------------------------------
void CNPC_MiniStrider::BleedThink()
{
	// Spurt blood from random points on the ministrider's head.
	Vector vecOrigin;
	QAngle angDir;
	GetAttachment( gm_nHeadCenterAttachment, vecOrigin, angDir );
	
	Vector vecDir = RandomVector( -1, 1 );
	VectorNormalize( vecDir );
	VectorAngles( vecDir, Vector( 0, 0, 1 ), angDir );

	vecDir *= gm_flHeadRadius;
	DispatchParticleEffect( "blood_spurt_synth_01", vecOrigin + vecDir, angDir );

	SetNextThink( gpGlobals->curtime + random->RandomFloat( 0.6, 1.5 ), MINISTRIDER_BLEED_THINK );
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
bool CNPC_MiniStrider::IsHeavyDamage( const CTakeDamageInfo &info )
{
	if ( info.GetDamage() < 45 )
	{
		return false;
	}

	if ( info.GetDamage() < 180 )
	{
		if ( !m_HeavyDamageDelay.Expired() || !BaseClass::IsHeavyDamage( info ) )
		{
			return false;
		}
	}

	m_HeavyDamageDelay.Set( 15, 25 );
	return true;

}


//-----------------------------------------------------------------------------
// We've taken some damage. Maybe we should flinch because of it.
//-----------------------------------------------------------------------------
void CNPC_MiniStrider::ConsiderFlinching( const CTakeDamageInfo &info )
{
	if ( !m_FlinchTimer.Expired() )
	{
		// Someone is whaling on us. Push out the timer so we don't keep flinching.
		m_FlinchTimer.Set( random->RandomFloat( 0.3 ) );
		return;
	}

	if ( GetState() == NPC_STATE_SCRIPT )
	{
		return;
	}

	Activity eGesture = ACT_MINTER_FLINCH_N;

	Vector forward;
	GetVectors( &forward, NULL, NULL );
	
	Vector vecForceDir = info.GetDamageForce();
	VectorNormalize( vecForceDir );
	
	float flDot = DotProduct( forward, vecForceDir );
	
	if ( flDot > 0.707 )
	{
		// flinch forward
		eGesture = ACT_MINTER_FLINCH_N;
	}
	else if ( flDot < -0.707 )
	{
		// flinch back
		eGesture = ACT_MINTER_FLINCH_S;
	}
	else
	{
		// flinch left or right
		Vector cross = CrossProduct( forward, vecForceDir );
		
		if ( cross.z > 0 )
		{
			eGesture = ACT_MINTER_FLINCH_W;
		}
		else
		{
			eGesture = ACT_MINTER_FLINCH_E;
		}
	}

	if ( !IsPlayingGesture( eGesture ) )
	{
		RestartGesture( eGesture );
		m_FlinchTimer.Set( random->RandomFloat( 0.3, 1.0 ) );
	}
}


//-----------------------------------------------------------------------------
// This is done from a think function because when the ministrider is killed,
// the physics code puts the vehicle's pre-collision velocity back so the jostle
// is basically discared.
//-----------------------------------------------------------------------------
void CNPC_MiniStrider::JostleVehicleThink()
{
	CBaseEntity *pInflictor = m_hHitByVehicle;
	if ( !pInflictor )
		return;

	Vector vecVelDir = pInflictor->GetSmoothedVelocity();
	float flSpeed = VectorNormalize( vecVelDir );
	Vector vecForce = CrossProduct( vecVelDir, Vector( 0, 0, 1 ) );
	if ( DotProduct( vecForce, GetAbsOrigin() ) < DotProduct( vecForce, pInflictor->GetAbsOrigin() ) )
	{
		// We're to the left of the vehicle that's hitting us.
		vecForce *= -1;
	}

	VectorNormalize( vecForce );
	vecForce.z = 1.0;

	float flForceScale = RemapValClamped( flSpeed, ministrider_jostle_car_min_speed.GetFloat(), ministrider_jostle_car_max_speed.GetFloat(), 50.0f, 150.0f );

	vecForce *= ( flForceScale * pInflictor->VPhysicsGetObject()->GetMass() );

	pInflictor->VPhysicsGetObject()->ApplyForceOffset( vecForce, WorldSpaceCenter() );
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
int CNPC_MiniStrider::OnTakeDamage( const CTakeDamageInfo &info )
{
	CTakeDamageInfo myInfo = info;
	
	return BaseClass::OnTakeDamage( myInfo );
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
int CNPC_MiniStrider::OnTakeDamage_Alive( const CTakeDamageInfo &info )
{
	CTakeDamageInfo myInfo = info;

	// don't take damage from my own weapons!!!
	// Exception: I "own" a magnade if it's glued to me.
	CBaseEntity *pInflictor = info.GetInflictor();
	CBaseEntity *pAttacker = info.GetAttacker();
	if ( pInflictor )
	{
		if ( IsStriderBuster( pInflictor ) )
		{
			// Get a tesla effect on our hitboxes for a little while.
			SetContextThink( &CNPC_MiniStrider::TeslaThink, gpGlobals->curtime, MINISTRIDER_ZAP_THINK ); 
			m_flTeslaStopTime = gpGlobals->curtime + 2.0f;
			
			myInfo.SetDamage( sk_ministrider_dmg_from_striderbuster.GetFloat() ) ;

			SetCondition( COND_MINISTRIDER_STAGGERED );
		}
		else if ( pInflictor->ClassMatches( GetClassname() ) && !( info.GetDamageType() == DMG_GENERIC ) )
		{
			return 0;
		}
	}

	if ( m_EscortBehavior.GetFollowTarget() && m_EscortBehavior.GetFollowTarget() == pAttacker )
	{
		return 0;
	}

	bool bHitByUnoccupiedCar = false;
	if ( ( ( info.GetDamageType() & DMG_CRUSH ) && ( pAttacker && pAttacker->IsPlayer() ) ) ||
		 ( info.GetDamageType() & DMG_VEHICLE ) )
	{
		// myInfo, not info! it may have been modified above.
		float flDamage = myInfo.GetDamage();
		if ( flDamage < MINISTRIDER_MIN_PHYSICS_DAMAGE )
		{
			//DevMsg( "ministrider: <<<< ZERO PHYSICS DAMAGE: %f\n", flDamage );
			myInfo.SetDamage( 0 );
		}
		else
		{
			CBaseEntity *pInflictor = info.GetInflictor();
			if ( ( info.GetDamageType() & DMG_VEHICLE ) || 
				 ( pInflictor && pInflictor->GetServerVehicle() && 
				   ( ( bHitByUnoccupiedCar = ( dynamic_cast<CPropVehicleDriveable *>(pInflictor) && static_cast<CPropVehicleDriveable *>(pInflictor)->GetDriver() == NULL ) )  == false ) ) )
			{
				// Adjust the damage from vehicles.
				flDamage *= sk_ministrider_vehicle_damage_scale.GetFloat();
				myInfo.SetDamage( flDamage );

				// Apply a force to jostle the vehicle that hit us.
				// Pick a force direction based on which side we're on relative to the vehicle's motion.
				Vector vecVelDir = pInflictor->GetSmoothedVelocity();
				if ( vecVelDir.Length() >= ministrider_jostle_car_min_speed.GetFloat() )
				{
					EmitSound( "NPC_Hunter.HitByVehicle" );
					m_hHitByVehicle = pInflictor;
					SetContextThink( &CNPC_MiniStrider::JostleVehicleThink, gpGlobals->curtime, MINISTRIDER_JOSTLE_VEHICLE_THINK );
				}
			}

			if ( !bHitByUnoccupiedCar )
			{
				SetCondition( COND_MINISTRIDER_STAGGERED );
			}
		}
		
		//DevMsg( "ministrider: >>>> PHYSICS DAMAGE: %f (was %f)\n", flDamage, info.GetDamage() );
	}

	// Show damage effects if we actually took damage.
	if ( ( myInfo.GetDamageType() & ( DMG_CRUSH | DMG_BLAST ) ) && ( myInfo.GetDamage() > 0 ) )
	{
		if ( !bHitByUnoccupiedCar )
			SetCondition( COND_MINISTRIDER_STAGGERED );
	}
	
	if ( HasCondition( COND_MINISTRIDER_STAGGERED ) )
	{
		// Throw a bunch of gibs out
		Vector vecForceDir = -myInfo.GetDamageForce();
		VectorNormalize( vecForceDir );
		PhysicsDamageEffect( myInfo.GetDamagePosition(), vecForceDir );

		// Stagger away from the direction the damage came from.
		m_vecStaggerDir = myInfo.GetDamageForce();
		VectorNormalize( m_vecStaggerDir );
	}

	// Take less damage from citizens and Alyx, otherwise ministriders go down too easily.
	float flScale = 1.0;

	if ( pAttacker &&
		( ( pAttacker->Classify() == CLASS_CITIZEN_REBEL ) ||
		  ( pAttacker->Classify() == CLASS_PLAYER_ALLY ) || 
		  ( pAttacker->Classify() == CLASS_PLAYER_ALLY_VITAL ) ) )
	{
		flScale *= sk_ministrider_citizen_damage_scale.GetFloat();
	}
	
	if ( flScale != 0 )
	{
		// We're taking a nonzero amount of damage.

		// If we're not staggering, consider flinching!
		if ( !HasCondition( COND_MINISTRIDER_STAGGERED ) )
		{
			ConsiderFlinching( info );
		}
		
		if( pAttacker && pAttacker->IsPlayer() )
		{
			// This block of code will distract the MiniStrider back to the player if the 
			// player does harm to the MiniStrider but is not the MiniStrider's current enemy.
			// This is achieved by updating the MiniStrider's enemy memory of the player and 
			// making the MiniStrider's current enemy invalid for a short time.
			if( !GetEnemy() || !GetEnemy()->IsPlayer() )
			{
				UpdateEnemyMemory( pAttacker, pAttacker->GetAbsOrigin(), this );

				if( GetEnemy() )
				{
					// Gotta forget about this person for a little bit.
					GetEnemies()->SetTimeValidEnemy( GetEnemy(), gpGlobals->curtime + MINISTRIDER_IGNORE_ENEMY_TIME );
				}
			}
		}

		float flDamage = myInfo.GetDamage() * flScale;
		myInfo.SetDamage( flDamage );
	}

	int nRet = BaseClass::OnTakeDamage_Alive( myInfo );

	m_EscortBehavior.OnDamage( myInfo );

	// Spark at 30% health.
	if ( !IsBleeding() && ( GetHealth() <= sk_ministrider_health.GetInt() * 0.3 ) )
	{
		StartBleeding();
	}
	
	if ( IsUsingSiegeTargets() && info.GetAttacker() != NULL && info.GetAttacker()->IsPlayer() )
	{
		// Defend myself. Try to siege attack immediately.
		m_flTimeNextSiegeTargetAttack = gpGlobals->curtime;
	}
	
	return nRet;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CNPC_MiniStrider::Event_Killed( const CTakeDamageInfo &info )
{
	// Remember the killing blow to make decisions about ragdolling.
	m_nKillingDamageType = info.GetDamageType();

	if ( m_EscortBehavior.GetFollowTarget() )
	{
		if ( AIGetNumFollowers( m_EscortBehavior.GetFollowTarget(), m_iClassname ) == 1 )
		{
			m_EscortBehavior.GetEscortTarget()->AlertSound();
			if ( info.GetAttacker() && info.GetAttacker()->IsPlayer() )
			{
				m_EscortBehavior.GetEscortTarget()->UpdateEnemyMemory( UTIL_GetLocalPlayer(), UTIL_GetLocalPlayer()->GetAbsOrigin(), this );
			}
		}
	}
	
	if ( info.GetDamageType() & DMG_VEHICLE )
	{
		bool bWasRunDown = false;
		int iRundownCounter = 0;
		if ( GetSquad() )
		{
			if ( !m_IgnoreVehicleTimer.Expired() )
			{
				GetSquad()->GetSquadData( MINISTRIDER_RUNDOWN_SQUADDATA, &iRundownCounter );
				GetSquad()->SetSquadData( MINISTRIDER_RUNDOWN_SQUADDATA, iRundownCounter + 1 );
				bWasRunDown = true;
			}
		}

		if ( ministrider_dodge_debug.GetBool() )
			Msg( "MiniStrider %d was%s run down\n", entindex(), ( bWasRunDown ) ? "" : " not" );

		// Death by vehicle! Decrement the ministriders to run over counter.
		// When the counter reaches zero ministriders will start dodging.
		if ( GlobalEntity_GetCounter( s_iszMiniStridersToRunOver ) > 0 )
		{
			GlobalEntity_AddToCounter( s_iszMiniStridersToRunOver, -1 );
		}
	}

	// Stop all our thinks
	SetContextThink( NULL, 0, MINISTRIDER_BLEED_THINK );

	StopParticleEffects( this );

	if (m_nSkin != MINISTRIDER_SKIN_DEAD)
	{
		m_nSkin = MINISTRIDER_SKIN_DEAD;
	}

	BaseClass::Event_Killed( info );
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CNPC_MiniStrider::StartBleeding()
{
	// Do this even if we're already bleeding (see OnRestore).
	m_bIsBleeding = true;

	// Start gushing blood from our... anus or something.
	DispatchParticleEffect( "blood_drip_synth_01", PATTACH_POINT_FOLLOW, this, gm_nHeadBottomAttachment );

	// Emit spurts of our blood
	SetContextThink( &CNPC_MiniStrider::BleedThink, gpGlobals->curtime + 0.1, MINISTRIDER_BLEED_THINK );
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
float CNPC_MiniStrider::MaxYawSpeed()
{
	if ( IsStriderBuster( GetEnemy() ) )
	{
		return 60;
	}

	if ( GetActivity() == ACT_MINTER_ANGRY )
		return 0;

	if ( GetActivity() == ACT_MINTER_CHARGE_RUN )
		return 5;

	if ( GetActivity() == ACT_MINTER_IDLE_PLANTED )
		return 0;

	if ( GetActivity() == ACT_MINTER_RANGE_ATTACK2_UNPLANTED )
		return 180;

	switch ( GetActivity() )
	{
		case ACT_RANGE_ATTACK2:
		{
			return 0;
		}
		
		case ACT_90_LEFT:
		case ACT_90_RIGHT:
		{
			return 45;
		}
		
		case ACT_TURN_LEFT:
		case ACT_TURN_RIGHT:
		{
			return 45;
		}
		
		case ACT_WALK:
		{
			return 25;
		}
		
		default:
		{
			return 35;
		}
	}
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
bool CNPC_MiniStrider::IsJumpLegal(const Vector &startPos, const Vector &apex, const Vector &endPos) const
{
	float MAX_JUMP_RISE		= 220.0f;
	float MAX_JUMP_DISTANCE	= 512.0f;
	float MAX_JUMP_DROP		= 384.0f;

	trace_t tr;	
	UTIL_TraceHull( startPos, startPos, GetHullMins(), GetHullMaxs(), MASK_NPCSOLID, this, COLLISION_GROUP_NONE, &tr );
	if ( tr.startsolid )
	{
		// Trying to start a jump in solid! Consider checking for this in CAI_MoveProbe::JumpMoveLimit.
		Assert( 0 );
		return false;
	}

	if ( BaseClass::IsJumpLegal( startPos, apex, endPos, MAX_JUMP_RISE, MAX_JUMP_DROP, MAX_JUMP_DISTANCE ) )
	{
		return true;
	}
	return false;
}


//-----------------------------------------------------------------------------
// Let the probe know I can run through small debris
// Stolen shamelessly from the Antlion Guard
//-----------------------------------------------------------------------------
bool CNPC_MiniStrider::ShouldProbeCollideAgainstEntity( CBaseEntity *pEntity )
{
	if ( s_iszPhysPropClassname != pEntity->m_iClassname )
		return BaseClass::ShouldProbeCollideAgainstEntity( pEntity );

	if ( pEntity->GetMoveType() == MOVETYPE_VPHYSICS )
	{
		IPhysicsObject *pPhysObj = pEntity->VPhysicsGetObject();

		if( pPhysObj && pPhysObj->GetMass() <= 500.0f )
		{
			return false;
		}
	}

	return BaseClass::ShouldProbeCollideAgainstEntity( pEntity );
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CNPC_MiniStrider::DoMuzzleFlash( int nAttachment )
{
	BaseClass::DoMuzzleFlash();
	
	DispatchParticleEffect( "ministrider_muzzle_flash", PATTACH_POINT_FOLLOW, this, nAttachment );

	// Dispatch the elight	
	CEffectData data;
	data.m_nAttachmentIndex = nAttachment;
	data.m_nEntIndex = entindex();
	DispatchEffect( "MiniStriderMuzzleFlash", data );
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
int CNPC_MiniStrider::CountRangedAttackers()
{
	CBaseEntity *pEnemy = GetEnemy();
	if ( !pEnemy )
	{
		return 0;
	}

	int nAttackers = 0;
	for ( int i = 0; i < g_MiniStriders.Count(); i++ )
	{
		CNPC_MiniStrider *pOtherMiniStrider = g_MiniStriders[i];
		if ( pOtherMiniStrider->GetEnemy() == pEnemy )
		{
			if ( pOtherMiniStrider->IsCurSchedule( SCHED_MINISTRIDER_RANGE_ATTACK2 ) )
			{
				nAttackers++;
			}
		}
	}
	return nAttackers;
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CNPC_MiniStrider::DelayRangedAttackers( float minDelay, float maxDelay, bool bForced )
{
	float delayMultiplier = ( g_pGameRules->IsSkillLevel( SKILL_EASY ) ) ? 1.25 : 1.0;
	if ( !m_bEnableSquadShootDelay && !bForced )
	{
		m_flNextRangeAttack2Time = gpGlobals->curtime + random->RandomFloat( minDelay, maxDelay ) * delayMultiplier;
		return;
	}

	CBaseEntity *pEnemy = GetEnemy();
	for ( int i = 0; i < g_MiniStriders.Count(); i++ )
	{
		CNPC_MiniStrider *pOtherMiniStrider = g_MiniStriders[i];
		if ( pOtherMiniStrider->GetEnemy() == pEnemy )
		{
			float nextTime = gpGlobals->curtime + random->RandomFloat( minDelay, maxDelay ) * delayMultiplier;
			if ( nextTime > pOtherMiniStrider->m_flNextRangeAttack2Time )
				pOtherMiniStrider->m_flNextRangeAttack2Time = nextTime;
		}
	}
}


//-----------------------------------------------------------------------------
// Given a target to shoot at, decide where to aim.
//-----------------------------------------------------------------------------
void CNPC_MiniStrider::GetShootDir( Vector &vecDir, const Vector &vecSrc, CBaseEntity *pTargetEntity, bool bStriderBuster, int nShotNum, bool bSingleShot )
{
	//RestartGesture( ACT_MINTER_GESTURE_SHOOT );

	EmitSound( "NPC_Hunter.FlechetteShoot" );

	Vector vecBodyTarget;

	if( pTargetEntity->Classify() == CLASS_PLAYER_ALLY_VITAL )
	{
		// Shooting at Alyx, most likely (in EP2). The attack is designed to displace
		// her, not necessarily actually harm her. So shoot at the area around her feet.
		vecBodyTarget = pTargetEntity->GetAbsOrigin();
	}
	else
	{
		vecBodyTarget = pTargetEntity->BodyTarget( vecSrc );
	}

	Vector vecTarget = vecBodyTarget;

	Vector vecDelta = pTargetEntity->GetAbsOrigin() - GetAbsOrigin();
	float flDist = vecDelta.Length();

	if ( !bStriderBuster )
	{
		// If we're not firing at a strider buster, miss in an entertaining way for the 
		// first three shots of each volley.
		if ( ( nShotNum < 3 ) && ( flDist > 200 ) )
		{
			Vector vecTargetForward;
			Vector vecTargetRight;
			pTargetEntity->GetVectors( &vecTargetForward, &vecTargetRight, NULL );

			Vector vecForward;
			GetVectors( &vecForward, NULL, NULL );

			float flDot = DotProduct( vecTargetForward, vecForward );

			if ( flDot < -0.8f )
			{
				// Our target is facing us, shoot the ground between us.
				float flPerc = 0.7 + ( 0.1 * nShotNum );
				vecTarget = GetAbsOrigin() + ( flPerc * ( pTargetEntity->GetAbsOrigin() - GetAbsOrigin() ) );
			}
			else if ( flDot > 0.8f )
			{
				// Our target is facing away from us, shoot to the left or right.
				Vector vecMissDir = vecTargetRight;
				if ( m_bMissLeft )
				{
					vecMissDir *= -1.0f;
				}

				vecTarget = pTargetEntity->EyePosition() +  ( 36.0f * ( 3 - nShotNum ) ) * vecMissDir;
			}
			else
			{
				// Our target is facing vaguely perpendicular to us, shoot across their view.
				vecTarget = pTargetEntity->EyePosition() +  ( 36.0f * ( 3 - nShotNum ) ) * vecTargetForward;
			}
		}	
		// If we can't see them, shoot where we last saw them.
		else if ( !HasCondition( COND_SEE_ENEMY ) )
		{
			Vector vecDelta = vecTarget - pTargetEntity->GetAbsOrigin();
			vecTarget = m_vecEnemyLastSeen + vecDelta;
		}
	}
	else
	{
		// If we're firing at a striderbuster, lead it.
		float flSpeed = ministrider_ar2round_speed.GetFloat();
		if ( !flSpeed )
		{
			flSpeed = 2500.0f;
		}

		flSpeed *= 1.5;

		float flDeltaTime = flDist / flSpeed;
		vecTarget = vecTarget + flDeltaTime * pTargetEntity->GetSmoothedVelocity();
	}

	vecDir = vecTarget - vecSrc;
	VectorNormalize( vecDir );
}


//-----------------------------------------------------------------------------
// Ensures that we don't exceed our pitch/yaw limits when shooting ar2rounds.
// Returns true if we had to clamp, false if not.
//-----------------------------------------------------------------------------
bool CNPC_MiniStrider::ClampShootDir( Vector &vecDir )
{
	Vector vecDir2D = vecDir;
	vecDir2D.z = 0;
	
	Vector vecForward;
	GetVectors( &vecForward, NULL, NULL );

	Vector vecForward2D = vecForward;
	vecForward2D.z = 0;
	
	float flDot = DotProduct( vecForward2D, vecDir2D );
	if ( flDot >= MINISTRIDER_SHOOT_MAX_YAW_COS )
	{
		// No need to clamp.
		return false;		
	}
	
	Vector vecAxis;
	CrossProduct( vecDir, vecForward, vecAxis );
	VectorNormalize( vecAxis );

	Quaternion q;
	AxisAngleQuaternion( vecAxis, -MINISTRIDER_SHOOT_MAX_YAW_DEG, q );

	matrix3x4_t rot;
	QuaternionMatrix( q, rot );
	VectorRotate( vecForward, rot, vecDir );
	VectorNormalize( vecDir );
	
	return true;
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
bool CNPC_MiniStrider::ShouldSeekTarget( CBaseEntity *pTargetEntity, bool bStriderBuster )
{
	bool bSeek = false;

	if ( bStriderBuster )
	{
		bool bSeek = false;

		if ( pTargetEntity->VPhysicsGetObject() && ( pTargetEntity->VPhysicsGetObject()->GetGameFlags() & FVPHYSICS_PLAYER_HELD ) )
		{
			bSeek = true;
		}
		else if ( StriderBuster_NumFlechettesAttached( pTargetEntity ) == 0 )
		{
			if ( StriderBuster_IsAttachedStriderBuster(pTargetEntity) )
			{
				bSeek = true;
			}
			else if ( ministrider_seek_thrown_striderbusters_tolerance.GetFloat() > 0.0 )
			{
				CNPC_Strider *pEscortTarget = m_EscortBehavior.GetEscortTarget();
				if ( pEscortTarget && ( pEscortTarget->GetAbsOrigin() - pTargetEntity->GetAbsOrigin() ).LengthSqr() < Square( ministrider_seek_thrown_striderbusters_tolerance.GetFloat() ) )
				{
					bSeek = true;
				}
			}
		}
	}

	return bSeek;
}	


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CNPC_MiniStrider::BeginVolley( int nNum, float flStartTime )
{
	m_nFlechettesQueued = nNum;
	m_nClampedShots = 0;
	m_flNextFlechetteTime = flStartTime;
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
bool CNPC_MiniStrider::ShootFlechette(CBaseEntity *pTargetEntity, bool bSingleShot)
{
	if (!pTargetEntity)
	{
		Assert(false);
		return false;
	}

	int nShotNum = ministrider_ar2round_volley_size.GetInt() - m_nFlechettesQueued;

	bool bStriderBuster = IsStriderBuster(pTargetEntity);

	// Choose the next muzzle to shoot from.
	Vector vecSrc;
	QAngle angMuzzle;

	if (m_bTopMuzzle)
	{
		GetAttachment(gm_nTopGunAttachment, vecSrc, angMuzzle);
		DoMuzzleFlash(gm_nTopGunAttachment);
	}
	else
	{
		GetAttachment(gm_nBottomGunAttachment, vecSrc, angMuzzle);
		DoMuzzleFlash(gm_nBottomGunAttachment);
	}

	m_bTopMuzzle = !m_bTopMuzzle;

	Vector vecDir;
	GetShootDir(vecDir, vecSrc, pTargetEntity, bStriderBuster, nShotNum, bSingleShot);

	bool bClamped = false;
	if (ministrider_clamp_shots.GetBool())
	{
		bClamped = ClampShootDir(vecDir);
	}

	CShotManipulator manipulator(vecDir);
	Vector vecShoot;

	if (IsUsingSiegeTargets() && nShotNum >= 2 && (nShotNum % 2) == 0)
	{
		// Near perfect accuracy for these three shots, so they are likely to fly right into the windows.
		// NOTE! In siege behavior in the map that this behavior was designed for (ep2_outland_10), the
		// MiniStriders will only ever shoot at siege targets in siege mode. If you allow MiniStriders in siege mode
		// to attack players or other NPCs, this accuracy bonus will apply unless we apply a bit more logic to it.
		vecShoot = manipulator.ApplySpread(VECTOR_CONE_1DEGREES * 0.5, 1.0f);
	}
	else
	{
		vecShoot = manipulator.ApplySpread(VECTOR_CONE_4DEGREES, 1.0f);
	}

	QAngle angShoot;
	VectorAngles(vecShoot, angShoot);

	trace_t tr;
	// Trace the initial shot from the weapon
	UTIL_TraceLine(vecSrc, vecSrc + vecDir * MAX_COORD_INTEGER, MASK_SHOT, this, COLLISION_GROUP_NONE, &tr);

	CEffectData data;
	data.m_vOrigin = tr.endpos + (tr.plane.normal * 1.0f);
	data.m_vNormal = tr.plane.normal;
	DispatchEffect("AR2Impact", data);
	BaseClass::DoImpactEffect(tr, DMG_DISSOLVE);
	
	UTIL_Tracer(vecSrc, tr.endpos, 0, TRACER_DONT_USE_ATTACHMENT, 5000, true, "StriderTracer");

	CTakeDamageInfo damageInfo(this, this, sk_ministrider_dmg_ar2round.GetFloat(), DMG_GENERIC | DMG_DISSOLVE); //tr.m_pEnt->GetHealth()
	Vector force(0, 0, 1.0f);
	damageInfo.SetDamageForce(force);
	damageInfo.SetDamagePosition(tr.endpos);
	tr.m_pEnt->TakeDamage(damageInfo);

	if (nShotNum == 1 && pTargetEntity->Classify() == CLASS_PLAYER_ALLY_VITAL)
	{
		// Make this person afraid and react to ME, not to the ar2rounds. 
		// Otherwise they could be scared into running towards the ministrider.
		CSoundEnt::InsertSound(SOUND_DANGER | SOUND_CONTEXT_REACT_TO_SOURCE | SOUND_CONTEXT_EXCLUDE_COMBINE, pTargetEntity->EyePosition(), 180.0f, 2.0f, this);
	}

	return bClamped;
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
Vector CNPC_MiniStrider::LeftFootHit( float eventtime )
{
	Vector footPosition;

	GetAttachment( "left foot", footPosition );
	CPASAttenuationFilter filter( this );
	EmitSound( filter, entindex(), "NPC_Hunter.Footstep", &footPosition, eventtime );

	FootFX( footPosition );

	return footPosition;
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
Vector CNPC_MiniStrider::RightFootHit( float eventtime )
{
	Vector footPosition;

	GetAttachment( "right foot", footPosition );
	CPASAttenuationFilter filter( this );
	EmitSound( filter, entindex(), "NPC_Hunter.Footstep", &footPosition, eventtime );
	FootFX( footPosition );

	return footPosition;
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
Vector CNPC_MiniStrider::BackFootHit( float eventtime )
{
	Vector footPosition;

	GetAttachment( "back foot", footPosition );
	CPASAttenuationFilter filter( this );
	EmitSound( filter, entindex(), "NPC_Hunter.BackFootstep", &footPosition, eventtime );
	FootFX( footPosition );

	return footPosition;
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CNPC_MiniStrider::FootFX( const Vector &origin )
{
	return;

	// dvs TODO: foot dust? probably too expensive for these guys
	/*trace_t tr;
	AI_TraceLine( origin, origin - Vector(0,0,100), MASK_SOLID_BRUSHONLY, this, COLLISION_GROUP_NONE, &tr );
	float yaw = random->RandomInt(0,120);
	for ( int i = 0; i < 3; i++ )
	{
		Vector dir = UTIL_YawToVector( yaw + i*120 ) * 10;
		VectorNormalize( dir );
		dir.z = 0.25;
		VectorNormalize( dir );
		g_pEffects->Dust( tr.endpos, dir, 12, 50 );
	}*/
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
CBaseEntity *CNPC_MiniStrider::GetEnemyVehicle()
{
	if ( GetEnemy() == NULL )
		return NULL;

	CBaseCombatCharacter *pCCEnemy = GetEnemy()->MyCombatCharacterPointer();
	if ( pCCEnemy != NULL )
		return pCCEnemy->GetVehicleEntity();

	return NULL;
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CNPC_MiniStrider::DrawDebugGeometryOverlays()
{
	if (m_debugOverlays & OVERLAY_BBOX_BIT) 
	{	
		float flViewRange	= acos(0.8);
		Vector vEyeDir = EyeDirection2D( );
		Vector vLeftDir, vRightDir;
		float fSin, fCos;
		SinCos( flViewRange, &fSin, &fCos );

		vLeftDir.x			= vEyeDir.x * fCos - vEyeDir.y * fSin;
		vLeftDir.y			= vEyeDir.x * fSin + vEyeDir.y * fCos;
		vLeftDir.z			=  vEyeDir.z;
		fSin				= sin(-flViewRange);
		fCos				= cos(-flViewRange);
		vRightDir.x			= vEyeDir.x * fCos - vEyeDir.y * fSin;
		vRightDir.y			= vEyeDir.x * fSin + vEyeDir.y * fCos;
		vRightDir.z			=  vEyeDir.z;

		int nSeq = GetSequence();
		if ( ( GetEntryNode( nSeq ) == gm_nPlantedNode ) && ( GetExitNode( nSeq ) == gm_nPlantedNode ) )
		{
			// planted - green
			NDebugOverlay::Box( GetAbsOrigin(), GetHullMins(), GetHullMaxs(), 0, 255, 0, 128, 0 );
		}
		else if ( ( GetEntryNode( nSeq ) == gm_nUnplantedNode ) && ( GetExitNode( nSeq ) == gm_nUnplantedNode ) )
		{
			// unplanted - blue
			NDebugOverlay::Box( GetAbsOrigin(), GetHullMins(), GetHullMaxs(), 0, 0, 255, 128, 0 );
		}
		else if ( ( GetEntryNode( nSeq ) == gm_nUnplantedNode ) && ( GetExitNode( nSeq ) == gm_nPlantedNode ) )
		{
			// planting transition - cyan
			NDebugOverlay::Box( GetAbsOrigin(), GetHullMins(), GetHullMaxs(), 0, 255, 255, 128, 0 );
		}
		else if ( ( GetEntryNode( nSeq ) == gm_nPlantedNode ) && ( GetExitNode( nSeq ) == gm_nUnplantedNode ) ) 
		{
			// unplanting transition - purple
			NDebugOverlay::Box( GetAbsOrigin(), GetHullMins(), GetHullMaxs(), 255, 0, 255, 128, 0 );
		}
		else
		{
			// unknown / other node - red
			Msg( "UNKNOWN: %s\n", GetSequenceName( GetSequence() ) );
			NDebugOverlay::Box( GetAbsOrigin(), GetHullMins(), GetHullMaxs(), 255, 0, 0, 128, 0 );
		}

		NDebugOverlay::BoxDirection(EyePosition(), Vector(0,0,-1), Vector(200,0,1), vLeftDir, 255, 0, 0, 50, 0 );
		NDebugOverlay::BoxDirection(EyePosition(), Vector(0,0,-1), Vector(200,0,1), vRightDir, 255, 0, 0, 50, 0 );
		NDebugOverlay::BoxDirection(EyePosition(), Vector(0,0,-1), Vector(200,0,1), vEyeDir, 0, 255, 0, 50, 0 );
		NDebugOverlay::Box(EyePosition(), -Vector(2,2,2), Vector(2,2,2), 0, 255, 0, 128, 0 );
	}	
	
	m_EscortBehavior.DrawDebugGeometryOverlays();
	
	BaseClass::DrawDebugGeometryOverlays();
}


//-----------------------------------------------------------------------------
// Player has illuminated this NPC with the flashlight
//-----------------------------------------------------------------------------
void CNPC_MiniStrider::PlayerHasIlluminatedNPC( CBasePlayer *pPlayer, float flDot )
{
	if ( m_bFlashlightInEyes )
		return;

	// Ignore the flashlight if it's not shining at my eyes
	if ( PlayerFlashlightOnMyEyes( pPlayer ) )
	{
		//Msg( ">>>> SHINING FLASHLIGHT ON ME\n" );
		m_bFlashlightInEyes = true;
		SetExpression("scenes/npc/hunter/hunter_eyeclose.vcd");
		m_flPupilDilateTime = gpGlobals->curtime + 0.2f;
	}
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
bool CNPC_MiniStrider::PlayerFlashlightOnMyEyes( CBasePlayer *pPlayer )
{
	Vector vecEyes, vecEyeForward, vecPlayerForward;
 	GetAttachment( gm_nTopGunAttachment, vecEyes, &vecEyeForward );
 	pPlayer->EyeVectors( &vecPlayerForward );

	Vector vecToEyes = (vecEyes - pPlayer->EyePosition());
	//float flDist = VectorNormalize( vecToEyes ); 

	float flDot = DotProduct( vecPlayerForward, vecToEyes );
	if ( flDot < 0.98 )
		return false;

	// Check facing to ensure we're in front of her
 	Vector los = ( pPlayer->EyePosition() - EyePosition() );
	los.z = 0;
	VectorNormalize( los );
	Vector facingDir = EyeDirection2D();
 	flDot = DotProduct( los, facingDir );
	return ( flDot > 0.3 );
}


//-----------------------------------------------------------------------------
// Return a random expression for the specified state to play over 
// the state's expression loop.
//-----------------------------------------------------------------------------
const char *CNPC_MiniStrider::SelectRandomExpressionForState( NPC_STATE state )
{
	if ( m_bFlashlightInEyes )
		return NULL;
		
	if ( !ministrider_random_expressions.GetBool() )
		return NULL;

	char *szExpressions[4] =
	{
		"scenes/npc/hunter/hunter_scan.vcd",
		"scenes/npc/hunter/hunter_eyeclose.vcd",
		"scenes/npc/hunter/hunter_roar.vcd",
		"scenes/npc/hunter/hunter_pain.vcd"
	};
	
	int nIndex = random->RandomInt( 0, 3 );
	//Msg( "RANDOM Expression: %s\n", szExpressions[nIndex] );
	return szExpressions[nIndex];
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CNPC_MiniStrider::PlayExpressionForState( NPC_STATE state )
{
	if ( m_bFlashlightInEyes )
	{
		return;
	}

	BaseClass::PlayExpressionForState( state );
}


//-----------------------------------------------------------------------------
// TODO: remove if we're not doing striderbuster stuff
//-----------------------------------------------------------------------------
void CNPC_MiniStrider::StriderBusterAttached( CBaseEntity *pAttached )
{
	// Add another to the list
	m_hAttachedBusters.AddToTail( pAttached );

	SetCondition( COND_MINISTRIDER_HIT_BY_STICKYBOMB );
	if (m_hAttachedBusters.Count() == 1)
	{
		EmitSound( "NPC_Hunter.Alert" );
	}
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CNPC_MiniStrider::StriderBusterDetached( CBaseEntity *pAttached )
{
	int elem = m_hAttachedBusters.Find(pAttached);
	if (elem >= 0)
	{
		m_hAttachedBusters.FastRemove(elem);
	}
}


//-----------------------------------------------------------------------------
// Set direction that the ministrider aims his body and eyes (guns).
//-----------------------------------------------------------------------------
void CNPC_MiniStrider::SetAim( const Vector &aimDir, float flInterval )
{
	QAngle angDir;
	VectorAngles( aimDir, angDir );
	float curPitch = GetPoseParameter( gm_nBodyPitchPoseParam );
	float curYaw = GetPoseParameter( gm_nBodyYawPoseParam );

	float newPitch;
	float newYaw;

	if ( GetEnemy() )
	{
		// clamp and dampen movement
		newPitch = curPitch + 0.8 * UTIL_AngleDiff( UTIL_ApproachAngle( angDir.x, curPitch, 20 ), curPitch );

		float flRelativeYaw = UTIL_AngleDiff( angDir.y, GetAbsAngles().y );
		newYaw = curYaw + UTIL_AngleDiff( flRelativeYaw, curYaw );
	}
	else
	{
		// Sweep your weapon more slowly if you're not fighting someone
		newPitch = curPitch + 0.6 * UTIL_AngleDiff( UTIL_ApproachAngle( angDir.x, curPitch, 20 ), curPitch );

		float flRelativeYaw = UTIL_AngleDiff( angDir.y, GetAbsAngles().y );
		newYaw = curYaw + 0.6 * UTIL_AngleDiff( flRelativeYaw, curYaw );
	}

	newPitch = AngleNormalize( newPitch );
	newYaw = AngleNormalize( newYaw );

	//Msg( "pitch=%f, yaw=%f\n", newPitch, newYaw );

	SetPoseParameter( gm_nAimPitchPoseParam, 0 );
	SetPoseParameter( gm_nAimYawPoseParam, 0 );

	SetPoseParameter( gm_nBodyPitchPoseParam, clamp( newPitch, -45, 45 ) );
	SetPoseParameter( gm_nBodyYawPoseParam, clamp( newYaw, -45, 45 ) );
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CNPC_MiniStrider::RelaxAim( float flInterval )
{
	float curPitch = GetPoseParameter( gm_nBodyPitchPoseParam );
	float curYaw = GetPoseParameter( gm_nBodyYawPoseParam );

	// dampen existing aim
	float newPitch = AngleNormalize( UTIL_ApproachAngle( 0, curPitch, 3 ) );
	float newYaw = AngleNormalize( UTIL_ApproachAngle( 0, curYaw, 2 ) );

	SetPoseParameter( gm_nAimPitchPoseParam, 0 );
	SetPoseParameter( gm_nAimYawPoseParam, 0 );

	SetPoseParameter( gm_nBodyPitchPoseParam, clamp( newPitch, -45, 45 ) );
	SetPoseParameter( gm_nBodyYawPoseParam, clamp( newYaw, -45, 45 ) );
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CNPC_MiniStrider::UpdateAim()
{
	if ( !GetModelPtr() || !GetModelPtr()->SequencesAvailable() )
		return;
		
	float flInterval = GetAnimTimeInterval();

	// Some activities look bad if we're giving our enemy the stinkeye.
	int eActivity = GetActivity();

	if ( GetEnemy() &&
		 GetState() != NPC_STATE_SCRIPT && 
		 ( eActivity != ACT_MINTER_CHARGE_CRASH ) &&
		 ( eActivity != ACT_MINTER_CHARGE_HIT ) )
	{
		Vector vecShootOrigin;

		vecShootOrigin = Weapon_ShootPosition();
		Vector vecShootDir = GetShootEnemyDir( vecShootOrigin, false );

		SetAim( vecShootDir, flInterval );
	}
	else
	{
		RelaxAim( flInterval );
	}
}


//-----------------------------------------------------------------------------
// Don't become a ragdoll until we've finished our death anim
//-----------------------------------------------------------------------------
bool CNPC_MiniStrider::CanBecomeRagdoll()
{
	return ( m_nKillingDamageType & DMG_CRUSH ) ||
		IsCurSchedule( SCHED_DIE, false ) ||								// Finished playing death anim, time to ragdoll
		IsCurSchedule( SCHED_MINISTRIDER_CHARGE_ENEMY, false ) ||				// While moving, it looks better to ragdoll instantly
		IsCurSchedule( SCHED_SCRIPTED_RUN, false ) ||
		( GetActivity() == ACT_WALK ) || ( GetActivity() == ACT_RUN ) ||
		GetCurSchedule() == NULL;											// Failsafe
}


//-----------------------------------------------------------------------------
// Determines the best type of death anim to play based on how we died.
//-----------------------------------------------------------------------------
Activity CNPC_MiniStrider::GetDeathActivity()
{
	return ACT_DIESIMPLE;
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CAI_MiniStriderEscortBehavior::OnDamage( const CTakeDamageInfo &info )
{
	if ( info.GetDamage() > 0 && info.GetAttacker()->IsPlayer() &&
		GetFollowTarget() && ( AIGetNumFollowers( GetFollowTarget() ) > 1 ) &&
		( GetOuter()->GetSquad()->GetSquadSoundWaitTime() <= gpGlobals->curtime ) ) // && !FarFromFollowTarget()
	{
		// Start the clock ticking. We'll return the the strider when the timer elapses.
		m_flTimeEscortReturn = gpGlobals->curtime + random->RandomFloat( 15.0f, 25.0f );
		GetOuter()->GetSquad()->SetSquadSoundWaitTime( m_flTimeEscortReturn + 1.0 ); // prevent others from breaking escort
	}
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CAI_MiniStriderEscortBehavior::BuildScheduleTestBits()
{
	BaseClass::BuildScheduleTestBits();

	if ( ( m_flTimeEscortReturn != 0 ) && ( gpGlobals->curtime > m_flTimeEscortReturn ) )
	{
		// We're delinquent! Return to strider!
		GetOuter()->ClearCustomInterruptCondition( COND_NEW_ENEMY );
		GetOuter()->ClearCustomInterruptCondition( COND_SEE_ENEMY );
		GetOuter()->ClearCustomInterruptCondition( COND_SEE_HATE );
		GetOuter()->ClearCustomInterruptCondition( COND_CAN_RANGE_ATTACK1 );
		GetOuter()->ClearCustomInterruptCondition( COND_CAN_RANGE_ATTACK2 );
	}
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CAI_MiniStriderEscortBehavior::CheckBreakEscort()
{
	if ( m_flTimeEscortReturn != 0 && ( FarFromFollowTarget() || gpGlobals->curtime >= m_flTimeEscortReturn ) )
	{
		if ( FarFromFollowTarget() )
		{
			m_flTimeEscortReturn = gpGlobals->curtime;
		}
		else
		{
			m_flTimeEscortReturn = 0;
		}
		if ( GetOuter()->GetSquad() )
		{
			GetOuter()->GetSquad()->SetSquadSoundWaitTime( gpGlobals->curtime + random->RandomFloat( 5.0f, 12.0f ) );
		}
	}
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CAI_MiniStriderEscortBehavior::GatherConditionsNotActive( void )
{
	if ( m_bEnabled )
	{
		DistributeFreeMiniStriders();
	}

	BaseClass::GatherConditionsNotActive();
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CAI_MiniStriderEscortBehavior::GatherConditions( void )
{
	m_bEnabled = true;

	DistributeFreeMiniStriders();

	BaseClass::GatherConditions();

	if ( GetEnemy() && GetEnemy()->IsPlayer() && HasCondition( COND_SEE_ENEMY ) )
	{
		if ( GetOuter()->GetSquad()->GetSquadSoundWaitTime() <= gpGlobals->curtime && ((CBasePlayer *)GetEnemy())->IsInAVehicle() )
		{
			m_flTimeEscortReturn = gpGlobals->curtime + random->RandomFloat( 15.0f, 25.0f );
			GetOuter()->GetSquad()->SetSquadSoundWaitTime( m_flTimeEscortReturn + 1.0 ); // prevent others from breaking escort
		}
	}
}

	
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
bool CAI_MiniStriderEscortBehavior::ShouldFollow()
{
	if ( IsStriderBuster( GetEnemy() ) )
		return false;

	if ( HasCondition( COND_HEAR_PHYSICS_DANGER ) )
		return false;

	if ( m_flTimeEscortReturn <= gpGlobals->curtime )
	{
		return CAI_FollowBehavior::ShouldFollow();
	}

	return false;
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CAI_MiniStriderEscortBehavior::BeginScheduleSelection()
{
	BaseClass::BeginScheduleSelection();
	Assert( m_SavedDistTooFar == GetOuter()->m_flDistTooFar );
	GetOuter()->m_flDistTooFar *= 2;
}	


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
int CAI_MiniStriderEscortBehavior::SelectSchedule()
{
	if( m_FollowDelay.IsRunning() && !m_FollowDelay.Expired() )
	{
		return FollowCallBaseSelectSchedule();
	}
	return BaseClass::SelectSchedule();
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
int CAI_MiniStriderEscortBehavior::FollowCallBaseSelectSchedule() 
{
	if ( GetOuter()->GetState() == NPC_STATE_COMBAT )
	{
		return GetOuter()->SelectCombatSchedule();
	}

	return BaseClass::FollowCallBaseSelectSchedule(); 
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CAI_MiniStriderEscortBehavior::StartTask( const Task_t *pTask )
{
	switch( pTask->iTask )
	{
	case TASK_MOVE_TO_FOLLOW_POSITION:
		{
			if ( GetEnemy() )
			{
				if ( GetOuter()->OccupyStrategySlot( SQUAD_SLOT_RUN_SHOOT ) )
				{
					if ( GetOuter()->GetSquad()->GetSquadMemberNearestTo( GetEnemy()->GetAbsOrigin() ) == GetOuter() )
					{
						GetOuter()->BeginVolley( NUM_FLECHETTE_VOLLEY_ON_FOLLOW, gpGlobals->curtime + 1.0 + random->RandomFloat( 0, .25 ) + random->RandomFloat( 0, .25 ) );
					}
					else
					{
						GetOuter()->VacateStrategySlot();
					}
				}
			}
			BaseClass::StartTask( pTask );
			break;
		}

	default:
		BaseClass::StartTask( pTask );
	}
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CAI_MiniStriderEscortBehavior::RunTask( const Task_t *pTask )
{
	switch( pTask->iTask )
	{
	case TASK_MOVE_TO_FOLLOW_POSITION:
		{
			if ( !GetFollowTarget() )
			{
				TaskFail( FAIL_NO_TARGET );
			}
			else
			{
				if ( GetEnemy() )
				{
					CNPC_MiniStrider *pMiniStrider = GetOuter();
					Vector vecEnemyLKP = pMiniStrider->GetEnemyLKP();
					pMiniStrider->AddFacingTarget( pMiniStrider->GetEnemy(), vecEnemyLKP, 1.0, 0.8 );
					bool bVacate = false;

					bool bHasSlot = pMiniStrider->HasStrategySlot( SQUAD_SLOT_RUN_SHOOT );
					if ( HasCondition( COND_SEE_ENEMY ) )
					{
						float maxDist = ministrider_ar2round_max_range.GetFloat() * 3;
						float distSq = ( pMiniStrider->GetAbsOrigin() - pMiniStrider->GetEnemy()->GetAbsOrigin() ).Length2DSqr();

						if ( distSq < Square( maxDist ) )
						{
							if ( gpGlobals->curtime >= pMiniStrider->m_flNextFlechetteTime )
							{
								if ( !bHasSlot )
								{
									if ( GetOuter()->OccupyStrategySlot( SQUAD_SLOT_RUN_SHOOT ) )
									{
										if ( GetOuter()->GetSquad()->GetSquadMemberNearestTo( GetEnemy()->GetAbsOrigin() ) == GetOuter() )
										{
											bHasSlot = true;
										}
										else
										{
											GetOuter()->VacateStrategySlot();
										}
									}
								}

								if ( bHasSlot )
								{
									// Start the firing sound.
									//CSoundEnvelopeController &controller = CSoundEnvelopeController::GetController();
									//if ( controller.SoundGetVolume( pMiniStrider->m_pGunFiringSound ) == 0.0f )
									//{
									//	controller.SoundChangeVolume( pMiniStrider->m_pGunFiringSound, 1.0f, 0.0f );
									//}
									
									pMiniStrider->ShootFlechette( GetEnemy(), true );

									if ( --pMiniStrider->m_nFlechettesQueued > 0 )
									{
										pMiniStrider->m_flNextFlechetteTime = gpGlobals->curtime + ministrider_ar2round_delay.GetFloat();
									}
									else
									{
										// Stop the firing sound.
										//CSoundEnvelopeController &controller = CSoundEnvelopeController::GetController();
										//controller.SoundChangeVolume( pMiniStrider->m_pGunFiringSound, 0, 0.01f );

										bVacate = true;
										pMiniStrider->BeginVolley( NUM_FLECHETTE_VOLLEY_ON_FOLLOW, gpGlobals->curtime + 1.0 + random->RandomFloat( 0, .25 ) + random->RandomFloat( 0, .25 ) );
									}
								}
							}
						}
						else if ( bHasSlot )
						{
							bVacate = true;
						}
					}
					else if ( bHasSlot )
					{
						bVacate = true;
					}

					if ( bVacate )
					{
						pMiniStrider->VacateStrategySlot();
					}
				}

				if ( m_FollowAttackTimer.Expired() && IsFollowTargetInRange( .8 )) 
				{
					m_FollowAttackTimer.Set( 8, 24 );
					TaskComplete();
				}
				else
				{
					BaseClass::RunTask( pTask );
				}
			}
			break;
		}

	default:
		BaseClass::RunTask( pTask );
	}
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CAI_MiniStriderEscortBehavior::FindFreeMiniStriders( CUtlVector<CNPC_MiniStrider *> *pFreeMiniStriders )
{
	pFreeMiniStriders->EnsureCapacity( g_MiniStriders.Count() );
	int i;

	for ( i = 0; i < g_MiniStriders.Count(); i++ )
	{
		CNPC_MiniStrider *pMiniStrider = g_MiniStriders[i];
		if ( pMiniStrider->IsAlive() && pMiniStrider->m_EscortBehavior.m_bEnabled )
		{
			if ( pMiniStrider->m_EscortBehavior.GetFollowTarget() == NULL || !pMiniStrider->m_EscortBehavior.GetFollowTarget()->IsAlive() )
			{
				pFreeMiniStriders->AddToTail( pMiniStrider);
			}
		}
	}
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CAI_MiniStriderEscortBehavior::DistributeFreeMiniStriders()
{
	if ( g_TimeLastDistributeFreeMiniStriders != -1 && gpGlobals->curtime - g_TimeLastDistributeFreeMiniStriders < FREE_MINISTRIDER_DISTRIBUTE_INTERVAL )
	{
		return;
	}

	g_TimeLastDistributeFreeMiniStriders = gpGlobals->curtime;

	CUtlVector<CNPC_MiniStrider *> freeMiniStriders;
	int i;
	FindFreeMiniStriders( &freeMiniStriders );

	CAI_BaseNPC **ppNPCs = g_AI_Manager.AccessAIs();
	for ( i = 0; i < g_AI_Manager.NumAIs() && freeMiniStriders.Count(); i++ )
	{
		int nToAdd;
		CNPC_Strider *pStrider = ( ppNPCs[i]->IsAlive() ) ? dynamic_cast<CNPC_Strider *>( ppNPCs[i] ) : NULL;
		if ( pStrider && !pStrider->CarriedByDropship() )
		{
			if ( ( nToAdd = 3 - AIGetNumFollowers( pStrider ) ) > 0 )
			{
				for ( int j = freeMiniStriders.Count() - 1; j >= 0 && nToAdd > 0; --j )
				{
					DevMsg( "npc_ministrider %d assigned to npc_strider %d\n", freeMiniStriders[j]->entindex(), pStrider->entindex() );
					freeMiniStriders[j]->FollowStrider( pStrider );
					freeMiniStriders.FastRemove( j );
					nToAdd--;
				}
			}
		}
	}

	for ( i = 0; i < freeMiniStriders.Count(); i++ )
	{
		//DevMsg( "npc_ministrider %d assigned to free_ministriders_squad\n", freeMiniStriders[i]->entindex() );
		freeMiniStriders[i]->m_EscortBehavior.SetFollowTarget( NULL );
		freeMiniStriders[i]->AddToSquad( AllocPooledString( "free_ministriders_squad" ) );
	}

#if 0
	CBaseEntity *pMiniStriderMaker = gEntList.FindEntityByClassname( NULL, "npc_ministrider_maker" ); // TODO: this picks the same one every time!
	if ( pMiniStriderMaker )
	{
		for ( i = 0; i < freeMiniStriders.Count(); i++ )
		{
			freeMiniStriders[i]->m_EscortBehavior.SetFollowTarget( pMiniStriderMaker );
		}
	}
#endif
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CAI_MiniStriderEscortBehavior::DrawDebugGeometryOverlays()
{
	if ( !GetFollowTarget() )
		return;

	Vector vecFollowPos = GetGoalPosition();
	if ( FarFromFollowTarget() )
	{
		if ( gpGlobals->curtime >= m_flTimeEscortReturn )
		{
			NDebugOverlay::HorzArrow( GetOuter()->GetAbsOrigin(), vecFollowPos, 16.0f, 255, 0, 0, 0, true, 0 );
		}
		else
		{
			NDebugOverlay::HorzArrow( GetOuter()->GetAbsOrigin(), vecFollowPos, 16.0f, 255, 255, 0, 0, true, 0 );
		}
	}
	else
	{
		NDebugOverlay::HorzArrow( GetOuter()->GetAbsOrigin(), vecFollowPos, 16.0f, 0, 255, 0, 0, true, 0 );
	}
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
bool MiniStrider_IsMiniStrider(CBaseEntity *pEnt)
{
	return dynamic_cast<CNPC_MiniStrider *>(pEnt) != NULL;
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void MiniStrider_StriderBusterLaunched( CBaseEntity *pBuster )
{
	CAI_BaseNPC **ppAIs = g_AI_Manager.AccessAIs();
	int nAIs = g_AI_Manager.NumAIs();

	for ( int i = 0; i < nAIs; i++ )
	{
		CAI_BaseNPC *pNPC = ppAIs[ i ];
		if (pNPC && (pNPC->Classify() == CLASS_COMBINE_HUNTER) && pNPC->m_lifeState == LIFE_ALIVE)
		{
			if ( !pNPC->GetEnemy() || !IsStriderBuster( pNPC->GetEnemy() ) )
			{
				Vector vecDelta = pNPC->GetAbsOrigin() - pBuster->GetAbsOrigin();
				if ( vecDelta.Length2DSqr() < 9437184.0f ) // 3072 * 3072
				{
					pNPC->SetEnemy( pBuster );
					pNPC->SetState( NPC_STATE_COMBAT );
					pNPC->UpdateEnemyMemory( pBuster, pBuster->GetAbsOrigin() );

					// Stop whatever we're doing.
					pNPC->SetCondition( COND_SCHEDULE_DONE );
				}
			}
		}
	}
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void MiniStrider_StriderBusterAttached( CBaseEntity *pMiniStrider, CBaseEntity *pAttached )
{
	Assert(dynamic_cast<CNPC_MiniStrider *>(pMiniStrider));

	static_cast<CNPC_MiniStrider *>(pMiniStrider)->StriderBusterAttached(pAttached);
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void MiniStrider_StriderBusterDetached( CBaseEntity *pMiniStrider, CBaseEntity *pAttached )
{
	Assert(dynamic_cast<CNPC_MiniStrider *>(pMiniStrider));

	static_cast<CNPC_MiniStrider *>(pMiniStrider)->StriderBusterDetached(pAttached);
}

//-------------------------------------------------------------------------------------------------
//
// ep2_outland_12 custom npc makers
//
//-------------------------------------------------------------------------------------------------

class CMiniStriderMaker : public CTemplateNPCMaker
{
	typedef CTemplateNPCMaker BaseClass;
public:
	void MakeMultipleNPCS( int nNPCs )
	{
		const float MIN_HEALTH_PCT = 0.2;

		CUtlVector<CNPC_MiniStrider *> candidates;
		CUtlVectorFixed<CNPC_MiniStrider *, 3> freeMiniStriders;
		CAI_MiniStriderEscortBehavior::FindFreeMiniStriders( &candidates );

		freeMiniStriders.EnsureCapacity( 3 );
		int i;

		for ( i = 0; i < candidates.Count() && freeMiniStriders.Count() < 3; i++ )
		{
			if ( candidates[i]->GetHealth() > candidates[i]->GetMaxHealth() * MIN_HEALTH_PCT )
			{
				freeMiniStriders.AddToTail( candidates[i] );
			}
		}

		int nRequested = nNPCs;
		if ( nNPCs < 3 )
		{
			nNPCs = MIN( 3, nNPCs + freeMiniStriders.Count() );
		}

		int nSummoned = 0;
		for ( i = 0; i < freeMiniStriders.Count() && nNPCs; i++ )
		{
			freeMiniStriders[i]->m_EscortBehavior.SetFollowTarget( this ); // this will make them not "free"
			freeMiniStriders[i]->SetName( m_iszTemplateName ); // this will force the ministrider to get the FollowStrider input
			nNPCs--;
			nSummoned++;
		}

		DevMsg( "Requested %d to spawn, Summoning %d free ministriders, spawning %d new ministriders\n", nRequested, nSummoned, nNPCs );
		if ( nNPCs )
		{
			BaseClass::MakeMultipleNPCS( nNPCs );
		}
	}
};

LINK_ENTITY_TO_CLASS( npc_ministrider_maker, CMiniStriderMaker );


//-------------------------------------------------------------------------------------------------
//
// Schedules
//
//-------------------------------------------------------------------------------------------------
AI_BEGIN_CUSTOM_NPC( npc_ministrider, CNPC_MiniStrider )

	DECLARE_TASK( TASK_MINISTRIDER_AIM )
	DECLARE_TASK( TASK_MINISTRIDER_FIND_DODGE_POSITION )
	DECLARE_TASK( TASK_MINISTRIDER_DODGE )
	DECLARE_TASK( TASK_MINISTRIDER_PRE_RANGE_ATTACK2 )
	DECLARE_TASK( TASK_MINISTRIDER_SHOOT_COMMIT )
	DECLARE_TASK( TASK_MINISTRIDER_ANNOUNCE_FLANK )
	DECLARE_TASK( TASK_MINISTRIDER_BEGIN_FLANK )
	DECLARE_TASK( TASK_MINISTRIDER_STAGGER )
	DECLARE_TASK( TASK_MINISTRIDER_CORNERED_TIMER )
	DECLARE_TASK( TASK_MINISTRIDER_FIND_SIDESTEP_POSITION )
	DECLARE_TASK( TASK_MINISTRIDER_CHARGE )
	DECLARE_TASK( TASK_MINISTRIDER_FINISH_RANGE_ATTACK )
	DECLARE_TASK( TASK_MINISTRIDER_WAIT_FOR_MOVEMENT_FACING_ENEMY )
	DECLARE_TASK( TASK_MINISTRIDER_CHARGE_DELAY )

	DECLARE_ACTIVITY( ACT_MINTER_DEPLOYRA2 )
	DECLARE_ACTIVITY( ACT_MINTER_DODGER )
	DECLARE_ACTIVITY( ACT_MINTER_DODGEL )
	DECLARE_ACTIVITY( ACT_MINTER_GESTURE_SHOOT )
	DECLARE_ACTIVITY( ACT_MINTER_FLINCH_STICKYBOMB )
	DECLARE_ACTIVITY( ACT_MINTER_STAGGER )
	DECLARE_ACTIVITY(ACT_DI_MINTER_MELEE)
	DECLARE_ACTIVITY(ACT_DI_MINTER_THROW)
	DECLARE_ACTIVITY( ACT_MINTER_MELEE_ATTACK1_VS_PLAYER )
	DECLARE_ACTIVITY( ACT_MINTER_ANGRY )
	DECLARE_ACTIVITY( ACT_MINTER_WALK_ANGRY )
	DECLARE_ACTIVITY( ACT_MINTER_FOUND_ENEMY )
	DECLARE_ACTIVITY( ACT_MINTER_FOUND_ENEMY_ACK )
	DECLARE_ACTIVITY( ACT_MINTER_CHARGE_START )
	DECLARE_ACTIVITY( ACT_MINTER_CHARGE_RUN )
	DECLARE_ACTIVITY( ACT_MINTER_CHARGE_STOP )
	DECLARE_ACTIVITY( ACT_MINTER_CHARGE_CRASH )
	DECLARE_ACTIVITY( ACT_MINTER_CHARGE_HIT )
	DECLARE_ACTIVITY( ACT_MINTER_RANGE_ATTACK2_UNPLANTED )
	DECLARE_ACTIVITY( ACT_MINTER_IDLE_PLANTED )
	DECLARE_ACTIVITY( ACT_MINTER_FLINCH_N )
	DECLARE_ACTIVITY( ACT_MINTER_FLINCH_S )
	DECLARE_ACTIVITY( ACT_MINTER_FLINCH_E )
	DECLARE_ACTIVITY( ACT_MINTER_FLINCH_W )

	DECLARE_INTERACTION( g_interactionMiniStriderFoundEnemy );

	DECLARE_SQUADSLOT( SQUAD_SLOT_MINISTRIDER_CHARGE )
	DECLARE_SQUADSLOT( SQUAD_SLOT_MINISTRIDER_FLANK_FIRST )
	DECLARE_SQUADSLOT( SQUAD_SLOT_RUN_SHOOT )

	DECLARE_CONDITION( COND_MINISTRIDER_SHOULD_PATROL )
	DECLARE_CONDITION( COND_MINISTRIDER_FORCED_FLANK_ENEMY )
	DECLARE_CONDITION( COND_MINISTRIDER_CAN_CHARGE_ENEMY )
	DECLARE_CONDITION( COND_MINISTRIDER_STAGGERED )
	DECLARE_CONDITION( COND_MINISTRIDER_IS_INDOORS )
	DECLARE_CONDITION( COND_MINISTRIDER_HIT_BY_STICKYBOMB )
	DECLARE_CONDITION( COND_MINISTRIDER_SEE_STRIDERBUSTER )
	DECLARE_CONDITION( COND_MINISTRIDER_FORCED_DODGE )
	DECLARE_CONDITION( COND_MINISTRIDER_INCOMING_VEHICLE )
	DECLARE_CONDITION( COND_MINISTRIDER_NEW_HINTGROUP )
	DECLARE_CONDITION( COND_MINISTRIDER_CANT_PLANT )
	DECLARE_CONDITION( COND_MINISTRIDER_SQUADMATE_FOUND_ENEMY )
	
	DECLARE_ANIMEVENT( AE_MINTER_FOOTSTEP_LEFT )
	DECLARE_ANIMEVENT( AE_MINTER_FOOTSTEP_RIGHT )
	DECLARE_ANIMEVENT( AE_MINTER_FOOTSTEP_BACK )
	DECLARE_ANIMEVENT( AE_MINTER_MELEE_ANNOUNCE )
	DECLARE_ANIMEVENT( AE_MINTER_MELEE_ATTACK_LEFT )
	DECLARE_ANIMEVENT( AE_MINTER_MELEE_ATTACK_RIGHT )
	DECLARE_ANIMEVENT( AE_MINTER_DIE )
	DECLARE_ANIMEVENT( AE_MINTER_SPRAY_BLOOD )
	DECLARE_ANIMEVENT( AE_MINTER_START_EXPRESSION )
	DECLARE_ANIMEVENT( AE_MINTER_END_EXPRESSION )

	//=========================================================
	// Attack (Deploy/shoot/finish)
	//=========================================================
	DEFINE_SCHEDULE
	(
		SCHED_MINISTRIDER_RANGE_ATTACK1,

		"	Tasks"
		"		TASK_STOP_MOVING					0"
		"		TASK_MINISTRIDER_SHOOT_COMMIT			0"
		"		TASK_RANGE_ATTACK1					0"
		"	"
		"	Interrupts"
		"		COND_NEW_ENEMY"
		"		COND_ENEMY_DEAD"
		"		COND_LOST_ENEMY"
		"		COND_ENEMY_OCCLUDED"
		"		COND_WEAPON_SIGHT_OCCLUDED"
		"		COND_TOO_CLOSE_TO_ATTACK"
		"		COND_TOO_FAR_TO_ATTACK"
		"		COND_NOT_FACING_ATTACK"
	)

	//=========================================================
	// Attack (Deploy/shoot/finish)
	//=========================================================
	DEFINE_SCHEDULE
	(
		SCHED_MINISTRIDER_RANGE_ATTACK2,

		"	Tasks"
		"		TASK_STOP_MOVING				0"
		"		TASK_MINISTRIDER_PRE_RANGE_ATTACK2	0"
		"		TASK_MINISTRIDER_SHOOT_COMMIT		0"
		"		TASK_RANGE_ATTACK2				0"
		"		TASK_MINISTRIDER_FINISH_RANGE_ATTACK	0"
		"		TASK_SET_ACTIVITY				ACTIVITY:ACT_IDLE"
		"		TASK_WAIT        				0.4"
		"		TASK_WAIT_RANDOM				0.2"
		"	"
		"	Interrupts"
		"		COND_NEW_ENEMY"
	)

	//=========================================================
	// Shoot at striderbuster. Distinct from generic range attack
	// because of BuildScheduleTestBits.
	//=========================================================
	DEFINE_SCHEDULE
	(
		SCHED_MINISTRIDER_RANGE_ATTACK2_VS_STRIDERBUSTER,

		"	Tasks"
		"		TASK_STOP_MOVING				0"
		"		TASK_MINISTRIDER_SHOOT_COMMIT		0"
		"		TASK_RANGE_ATTACK2				0"
		"	"
		"	Interrupts"
	)

	//=========================================================
	// Shoot at striderbuster with a little latency beforehand
	//=========================================================
	DEFINE_SCHEDULE
	(
		SCHED_MINISTRIDER_RANGE_ATTACK2_VS_STRIDERBUSTER_LATENT,

		"	Tasks"
		"		TASK_STOP_MOVING				0"
		"		TASK_MINISTRIDER_SHOOT_COMMIT		0"
		"		TASK_WAIT						0.2"
		"		TASK_PLAY_SEQUENCE_FACE_ENEMY	ACTIVITY:ACT_RANGE_ATTACK2"
		"		TASK_RANGE_ATTACK2				0"
		"	"
		"	Interrupts"
	)

	//=========================================================
	// Dodge Incoming vehicle
	//=========================================================
	DEFINE_SCHEDULE
	(
		SCHED_MINISTRIDER_DODGE,

		"	Tasks"
		"		TASK_SET_FAIL_SCHEDULE			SCHEDULE:SCHED_MINISTRIDER_FAIL_DODGE"
		"		TASK_MINISTRIDER_FIND_DODGE_POSITION			0"
		"		TASK_MINISTRIDER_DODGE						0"
		""
		"	Interrupts"
	)

	//=========================================================
	// Dodge Incoming vehicle
	//=========================================================
	DEFINE_SCHEDULE
	(
		SCHED_MINISTRIDER_FAIL_DODGE,

		"	Tasks"
		"		TASK_STOP_MOVING		0"
		"		TASK_SET_ACTIVITY		ACTIVITY:ACT_IDLE"
		"		TASK_FACE_ENEMY			0"
		""
		"	Interrupts"
	)

	//==================================================
	// > SCHED_MINISTRIDER_CHARGE_ENEMY
	// Rush at my enemy and head-butt them.
	//==================================================
	DEFINE_SCHEDULE
	(
		SCHED_MINISTRIDER_CHARGE_ENEMY,

		"	Tasks"
		"		TASK_SET_FAIL_SCHEDULE			SCHEDULE:SCHED_MINISTRIDER_FAIL_CHARGE_ENEMY"
		"		TASK_STOP_MOVING				0"
		"		TASK_FACE_ENEMY					0"
		"		TASK_MINISTRIDER_CHARGE				0"
		""
		"	Interrupts"
		"		COND_TASK_FAILED"
		"		COND_ENEMY_DEAD"
	)

	DEFINE_SCHEDULE
	(
		SCHED_MINISTRIDER_FAIL_CHARGE_ENEMY,

		"	Tasks"
		"		TASK_MINISTRIDER_CHARGE_DELAY		10"
	)

	//=========================================================
	// Chase the enemy with intent to claw
	//=========================================================
	DEFINE_SCHEDULE
	(
		SCHED_MINISTRIDER_CHASE_ENEMY_MELEE,

		"	Tasks"
		"		TASK_SET_FAIL_SCHEDULE			SCHEDULE:SCHED_ESTABLISH_LINE_OF_FIRE"
		"		TASK_STOP_MOVING				0"
		"		TASK_GET_CHASE_PATH_TO_ENEMY	300"
		"		TASK_RUN_PATH					0"
		"		TASK_WAIT_FOR_MOVEMENT			0"
		"		TASK_FACE_ENEMY					0"
		""
		"	Interrupts"
		"		COND_NEW_ENEMY"
		"		COND_ENEMY_DEAD"
		"		COND_ENEMY_UNREACHABLE"
		"		COND_CAN_MELEE_ATTACK1"
		"		COND_CAN_MELEE_ATTACK2"
		//"		COND_TOO_CLOSE_TO_ATTACK"
		"		COND_LOST_ENEMY"
	)

	//=========================================================
	// Chase my enemy, shoot or claw when possible to do so.
	//=========================================================
	DEFINE_SCHEDULE
	(
		SCHED_MINISTRIDER_CHASE_ENEMY,

		"	Tasks"
		"		TASK_SET_FAIL_SCHEDULE			SCHEDULE:SCHED_ESTABLISH_LINE_OF_FIRE"
		"		TASK_STOP_MOVING				0"
		"		TASK_GET_CHASE_PATH_TO_ENEMY	300"
		"		TASK_RUN_PATH					0"
		"		TASK_WAIT_FOR_MOVEMENT			0"
		"		TASK_FACE_ENEMY					0"
		""
		"	Interrupts"
		"		COND_NEW_ENEMY"
		"		COND_ENEMY_DEAD"
		"		COND_ENEMY_UNREACHABLE"
		"		COND_CAN_RANGE_ATTACK1"
		"		COND_CAN_RANGE_ATTACK2"
		"		COND_CAN_MELEE_ATTACK1"
		"		COND_CAN_MELEE_ATTACK2"
		"		COND_TOO_CLOSE_TO_ATTACK"
		"		COND_LOST_ENEMY"
	)

	//=========================================================
	// Move to a flanking position, then shoot if possible.
	//=========================================================
	DEFINE_SCHEDULE
	(
		SCHED_MINISTRIDER_FLANK_ENEMY,

		"	Tasks"
		"		TASK_SET_FAIL_SCHEDULE					SCHEDULE:SCHED_ESTABLISH_LINE_OF_FIRE"
		"		TASK_STOP_MOVING						0"
		"		TASK_MINISTRIDER_BEGIN_FLANK					0"
		"		TASK_GET_FLANK_ARC_PATH_TO_ENEMY_LOS	30"
		"		TASK_MINISTRIDER_ANNOUNCE_FLANK				0"
		"		TASK_RUN_PATH							0"
		"		TASK_WAIT_FOR_MOVEMENT					0"
		"		TASK_FACE_ENEMY							0"
		//"		TASK_MINISTRIDER_END_FLANK					0"
		""
		"	Interrupts"
		"		COND_NEW_ENEMY"
		//"		COND_CAN_RANGE_ATTACK1"
		//"		COND_CAN_RANGE_ATTACK2"
		"		COND_CAN_MELEE_ATTACK1"
		"		COND_CAN_MELEE_ATTACK2"
		"		COND_ENEMY_DEAD"
		"		COND_ENEMY_UNREACHABLE"
		"		COND_TOO_CLOSE_TO_ATTACK"
		"		COND_LOST_ENEMY"
	)

	//=========================================================
	//=========================================================
	DEFINE_SCHEDULE
	(
		SCHED_MINISTRIDER_COMBAT_FACE,

		"	Tasks"
		"		TASK_STOP_MOVING		0"
		"		TASK_SET_ACTIVITY		ACTIVITY:ACT_IDLE"
		"		TASK_WAIT_FACE_ENEMY	1"
		""
		"	Interrupts"
		"		COND_CAN_RANGE_ATTACK1"
		"		COND_CAN_RANGE_ATTACK2"
		"		COND_CAN_MELEE_ATTACK1"
		"		COND_CAN_MELEE_ATTACK2"
		"		COND_NEW_ENEMY"
		"		COND_ENEMY_DEAD"
	)
	
	//=========================================================
	// Like the base class, only don't stop in the middle of 
	// swinging if the enemy is killed, hides, or new enemy.
	//=========================================================
	DEFINE_SCHEDULE
	(
		SCHED_MINISTRIDER_MELEE_ATTACK1,

		"	Tasks"
		"		TASK_STOP_MOVING		0"
		"		TASK_FACE_ENEMY			0"
		"		TASK_MELEE_ATTACK1		0"
		//"		TASK_SET_SCHEDULE		SCHEDULE:SCHED_MINISTRIDER_POST_MELEE_WAIT"
		""
		"	Interrupts"
	)

	//=========================================================
	// In a fight with nothing to do. Make busy!
	//=========================================================
	DEFINE_SCHEDULE	
	(
		SCHED_MINISTRIDER_CHANGE_POSITION,

		"	Tasks"
		"		TASK_STOP_MOVING				0"
		"		TASK_WANDER						720432" // 6 feet to 36 feet
		"		TASK_RUN_PATH					0"
		"		TASK_MINISTRIDER_WAIT_FOR_MOVEMENT_FACING_ENEMY	0"
		"		TASK_STOP_MOVING				0"
		"		TASK_SET_SCHEDULE				SCHEDULE:SCHED_MINISTRIDER_CHANGE_POSITION_FINISH"
		""
		"	Interrupts"
		"		COND_ENEMY_DEAD"
		"		COND_CAN_MELEE_ATTACK1"
		"		COND_CAN_MELEE_ATTACK2"
		"		COND_HEAR_DANGER"
		"		COND_HEAR_MOVE_AWAY"
		"		COND_NEW_ENEMY"
	)

	//=========================================================
	// In a fight with nothing to do. Make busy!
	//=========================================================
	DEFINE_SCHEDULE	
	(
		SCHED_MINISTRIDER_CHANGE_POSITION_FINISH,

		"	Tasks"
		"		TASK_FACE_ENEMY					0"
		"		TASK_WAIT_FACE_ENEMY_RANDOM		5"
		""
		"	Interrupts"
		"		COND_ENEMY_DEAD"
		"		COND_CAN_RANGE_ATTACK1"
		"		COND_CAN_RANGE_ATTACK2"
		"		COND_CAN_MELEE_ATTACK1"
		"		COND_CAN_MELEE_ATTACK2"
		"		COND_HEAR_DANGER"
		"		COND_HEAR_MOVE_AWAY"
		"		COND_NEW_ENEMY"
	)

	//=========================================================
	// In a fight with nothing to do. Make busy!
	//=========================================================
	DEFINE_SCHEDULE	
	(
		SCHED_MINISTRIDER_SIDESTEP,

		"	Tasks"
		"		TASK_SET_FAIL_SCHEDULE			SCHEDULE:SCHED_MINISTRIDER_FAIL_IMMEDIATE" // used because sched_fail includes a one second pause. ick!
		"		TASK_STOP_MOVING						0"
		"		TASK_MINISTRIDER_FIND_SIDESTEP_POSITION		0"
		"		TASK_GET_PATH_TO_SAVEPOSITION			0"
		"		TASK_RUN_PATH							0"
		"		TASK_WAIT_FOR_MOVEMENT					0"
		"		TASK_FACE_ENEMY							0"
		""
		"	Interrupts"
	)

	//=========================================================
	//=========================================================
	DEFINE_SCHEDULE	
	(
		SCHED_MINISTRIDER_PATROL,

		"	Tasks"
		"		TASK_STOP_MOVING				0"
		"		TASK_WANDER						720432" // 6 feet to 36 feet
		"		TASK_WALK_PATH					0"
		"		TASK_WAIT_FOR_MOVEMENT			0"
		"		TASK_STOP_MOVING				0"
		"		TASK_FACE_REASONABLE			0"
		"		TASK_WAIT_RANDOM				3"
		""
		"	Interrupts"
		"		COND_ENEMY_DEAD"
		"		COND_LIGHT_DAMAGE"
		"		COND_HEAVY_DAMAGE"
		"		COND_HEAR_DANGER"
		"		COND_HEAR_COMBAT"
		"		COND_HEAR_PLAYER"
		"		COND_HEAR_BULLET_IMPACT"
		"		COND_HEAR_MOVE_AWAY"
		"		COND_NEW_ENEMY"
		"		COND_SEE_ENEMY"
		"		COND_CAN_RANGE_ATTACK1"
		"		COND_CAN_RANGE_ATTACK2"
		"		COND_CAN_MELEE_ATTACK1"
		"		COND_CAN_MELEE_ATTACK2"
	)

	//=========================================================
	// Stagger because I got hit by something heavy
	//=========================================================
	DEFINE_SCHEDULE
	(
		SCHED_MINISTRIDER_STAGGER,

		"	Tasks"
		"		TASK_MINISTRIDER_STAGGER			0"
		""
		"	Interrupts"
	)

	//=========================================================
	// Run around randomly until we detect an enemy
	//=========================================================
	DEFINE_SCHEDULE
	(
		SCHED_MINISTRIDER_PATROL_RUN,

		"	Tasks"
		"		TASK_SET_FAIL_SCHEDULE			SCHEDULE:SCHED_COMBAT_FACE"
		"		TASK_SET_ROUTE_SEARCH_TIME		5"	// Spend 5 seconds trying to build a path if stuck
		"		TASK_GET_PATH_TO_RANDOM_NODE	200"
		"		TASK_RUN_PATH					0"
		"		TASK_WAIT_FOR_MOVEMENT			0"
		""
		"	Interrupts"
		"		COND_CAN_RANGE_ATTACK1 "
		"		COND_CAN_RANGE_ATTACK2 "
		"		COND_CAN_MELEE_ATTACK1 "
		"		COND_CAN_MELEE_ATTACK2"
		"		COND_GIVE_WAY"
		"		COND_NEW_ENEMY"
		"		COND_HEAR_COMBAT"
		"		COND_HEAR_DANGER"
		"		COND_HEAR_PLAYER"
		"		COND_LIGHT_DAMAGE"
		"		COND_HEAVY_DAMAGE"
	)

	//=========================================================
	//=========================================================
	DEFINE_SCHEDULE
	(
		SCHED_MINISTRIDER_TAKE_COVER_FROM_ENEMY,

		"	Tasks"
		"		TASK_SET_FAIL_SCHEDULE			SCHEDULE:SCHED_MINISTRIDER_CHASE_ENEMY_MELEE"
		"		TASK_MINISTRIDER_CORNERED_TIMER		10.0"
		"		TASK_WAIT						0.0"
	//	"		TASK_SET_TOLERANCE_DISTANCE		24"
	//	"		TASK_FIND_COVER_FROM_ENEMY		0"
		"		TASK_FIND_FAR_NODE_COVER_FROM_ENEMY 200.0"
		"		TASK_RUN_PATH					0"
		"		TASK_MINISTRIDER_CORNERED_TIMER		0.0"
	//	"		TASK_CLEAR_FAIL_SCHEDULE		0" // not used because sched_fail includes a one second pause. ick!
		"		TASK_SET_FAIL_SCHEDULE			SCHEDULE:SCHED_MINISTRIDER_FAIL_IMMEDIATE"
		"		TASK_WAIT_FOR_MOVEMENT			0"
		"		TASK_REMEMBER					MEMORY:INCOVER"
		"		TASK_SET_SCHEDULE				SCHEDULE:SCHED_MINISTRIDER_HIDE_UNDER_COVER"
		/*
		"		TASK_FACE_ENEMY					0"
		"		TASK_SET_ACTIVITY				ACTIVITY:ACT_IDLE"	// Translated to cover
		"		TASK_WAIT						1"
		*/
		""
		"	Interrupts"
		"		COND_NEW_ENEMY"
		"		COND_HEAR_DANGER"
	)

	//=========================================================
	//=========================================================
	DEFINE_SCHEDULE
	(
		SCHED_MINISTRIDER_HIDE_UNDER_COVER,

		"	Tasks"
		"		TASK_SET_FAIL_SCHEDULE			SCHEDULE:SCHED_MINISTRIDER_FAIL_IMMEDIATE" // used because sched_fail includes a one second pause. ick!
		"		TASK_FACE_ENEMY					0"
		"		TASK_SET_ACTIVITY				ACTIVITY:ACT_IDLE"	// Translated to cover
		"		TASK_WAIT						1"
		""
		"	Interrupts"
		"		COND_NEW_ENEMY"
		"		COND_HEAR_DANGER"
		"		COND_HAVE_ENEMY_LOS"
	)
	
	//=========================================================
	//=========================================================
	DEFINE_SCHEDULE
	(
		SCHED_MINISTRIDER_FOUND_ENEMY,

		"	Tasks"
		"		TASK_STOP_MOVING				0"
		"		TASK_FACE_ENEMY					0"
		"		TASK_PLAY_SEQUENCE_FACE_ENEMY	ACTIVITY:ACT_MINTER_FOUND_ENEMY"
		""
		"	Interrupts"
		"		COND_LIGHT_DAMAGE"
		"		COND_HEAVY_DAMAGE"
	)
	
	//=========================================================
	//=========================================================
	DEFINE_SCHEDULE
	(
		SCHED_MINISTRIDER_FOUND_ENEMY_ACK,

		"	Tasks"
		"		TASK_STOP_MOVING				0"
		"		TASK_WAIT_RANDOM				0.75"
		"		TASK_FACE_ENEMY					0"
		"		TASK_PLAY_SEQUENCE_FACE_ENEMY	ACTIVITY:ACT_MINTER_FOUND_ENEMY_ACK"
		""
		"	Interrupts"
		"		COND_LIGHT_DAMAGE"
		"		COND_HEAVY_DAMAGE"
	)
	
	//=========================================================
	// An empty schedule that immediately bails out, faster than
	// SCHED_FAIL which stops moving and waits for one second.
	//=========================================================
	DEFINE_SCHEDULE
	(
		SCHED_MINISTRIDER_FAIL_IMMEDIATE,

		"	Tasks"
		"		TASK_WAIT			0"

	)

	DEFINE_SCHEDULE
	( 
		SCHED_MINISTRIDER_GOTO_HINT,
		"	Tasks"
		"		TASK_SET_FAIL_SCHEDULE			SCHEDULE:SCHED_MINISTRIDER_CLEAR_HINTNODE" // used because sched_fail includes a one second pause. ick!
		"		TASK_GET_PATH_TO_HINTNODE		1"
		"		TASK_WAIT_FOR_MOVEMENT			0"
		"		TASK_CLEAR_HINTNODE				0"
		""
		""
		"	Interrupts"
	)

	DEFINE_SCHEDULE
	( 
		SCHED_MINISTRIDER_CLEAR_HINTNODE,
		"	Tasks"
		"		TASK_CLEAR_HINTNODE				0"
		""
		""
		"	Interrupts"
	)

	DEFINE_SCHEDULE
	(
		SCHED_MINISTRIDER_SIEGE_STAND,
		"	Tasks"
		"		TASK_STOP_MOVING				0"
		"		TASK_SET_ACTIVITY				ACTIVITY:ACT_IDLE"
		"		TASK_FACE_PLAYER				0"
		"		TASK_WAIT						10"
		"		TASK_WAIT_RANDOM				2"
		"		TASK_SET_SCHEDULE				SCHEDULE:SCHED_MINISTRIDER_CHANGE_POSITION_SIEGE"
		""
		""
		"	Interrupts"
		"		COND_SEE_PLAYER"
		"		COND_NEW_ENEMY"
	)

	DEFINE_SCHEDULE	
	(
		SCHED_MINISTRIDER_CHANGE_POSITION_SIEGE,

		"	Tasks"
		"		TASK_STOP_MOVING				0"
		"		TASK_WANDER						2400480"
		"		TASK_RUN_PATH					0"
		"		TASK_WAIT_FOR_MOVEMENT			0"
		"		TASK_STOP_MOVING				0"
		"		TASK_SET_ACTIVITY				ACTIVITY:ACT_IDLE"
		"		TASK_FACE_PLAYER				0"
		""
		"	Interrupts"
		"		COND_NEW_ENEMY"
	)

	// formula is MIN_DIST * 10000 + MAX_DIST

AI_END_CUSTOM_NPC()
