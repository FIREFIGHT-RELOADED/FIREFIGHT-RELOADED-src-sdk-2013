#include	"cbase.h"
#include	"npc_cremator.h"
#include	"particle_parse.h"

	LINK_ENTITY_TO_CLASS( npc_cremator, CNPC_Cremator );

int ACT_FIREINOUT;
Activity ACT_CREMATOR_ARM;
Activity ACT_CREMATOR_DISARM;
Activity ACT_CREMATOR_RELOAD;

	BEGIN_DATADESC( CNPC_Cremator )

	DEFINE_FIELD( m_flNextIdleSoundTime, FIELD_TIME ),
	DEFINE_FIELD( m_flImmoRadius, FIELD_FLOAT ),
	DEFINE_FIELD( m_bHeadshot, FIELD_BOOLEAN ),
	DEFINE_FIELD( m_bCanisterShot, FIELD_BOOLEAN ),
	DEFINE_FIELD( m_flNextRangeAttack2Time, FIELD_TIME ),
	DEFINE_FIELD( m_iAmmo, FIELD_INTEGER ),
	DEFINE_FIELD( m_iMaxAmmo, FIELD_INTEGER ),
//
	END_DATADESC()

// Кэширование разнообразных ассетов для нпц.
void CNPC_Cremator::Precache()
{
	BaseClass::Precache();
	PrecacheModel( "models/cremator_test2.mdl" ); // common low-poly model обычная модель
	PrecacheModel( "models/crematorhead.mdl" ); // отдельная модель головы

	enginesound->PrecacheSound("npc/cremator/amb_loop.wav");
	enginesound->PrecacheSound("npc/cremator/amb1.wav");
	enginesound->PrecacheSound("npc/cremator/amb2.wav");
	enginesound->PrecacheSound("npc/cremator/amb3.wav");
	enginesound->PrecacheSound("npc/cremator/crem_die.wav");
	enginesound->PrecacheSound("npc/cremator/alert_object.wav");
	enginesound->PrecacheSound("npc/cremator/alert_player.wav");
	PrecacheScriptSound( "NPC_Cremator.FootstepLeft" );	
	PrecacheScriptSound( "NPC_Cremator.FootstepRight" );
	PrecacheScriptSound( "Weapon_Immolator.Single" );
	PrecacheScriptSound( "Weapon_Immolator.Stop" );

	//UTIL_PrecacheOther( "grenade_molotov" );

	PrecacheParticleSystem( "flamethrower" ); // зеленое пламя для Интервала.
	PrecacheParticleSystem( "flamethrower_orange" ); // альтернативное, обычное оранжевое пламя.

	engine->PrecacheModel("sprites/crystal_beam1.vmt"); // for the beam attack
}
void CNPC_Cremator::Spawn()
{	
	Precache();
	SetModel( "models/cremator_test2.mdl" ); // a model with a bit of a tesselation (tesselated head and collar)
	
	SetHullType(HULL_HUMAN); // отключено, т.к. это новый введенный тип хулла, и вряд ли есть смысл возиться с его введением. 

	//SetHullType( HULL_MEDIUM_TALL ); // данный стандартный тип подходит для большинства ситуаций, однако крематор не сможет проходить в двери, если они ненамного выше его
	
	/*that hull type is made special for a cremator since HULL_MEDIUM is not high enough but HULL_LARGE is too big
	and a cremator cannot fit in Borealis maps with it. The hull type is enumarated in ai_hull.h under the HULL_MEDIUM_TALL
	and then MUST be enumeraten in Hull_Bits_t, again, UNDER HULL_MEDIUM_TALL with the adress of 0x00000400,
	so that the definitions in ai_hull.cpp will then find the corresponding hull type.*/
	SetHullSizeNormal();
	
	SetBodygroup( 1, 0 ); // the gun
	SetBodygroup( 2, 0 ); // the head

	SetSolid( SOLID_BBOX );
	AddSolidFlags( FSOLID_NOT_STANDABLE );
	SetMoveType( MOVETYPE_STEP );

	m_bloodColor		= DONT_BLEED;//BLOOD_COLOR_YELLOW;
	m_iHealth			= sk_cremator_health.GetFloat();
	m_flFieldOfView		= VIEW_FIELD_WIDE;// indicates the width of this NPC's forward view cone ( as a dotproduct result )
	m_NPCState			= NPC_STATE_NONE;
	m_nSkin				= CREMATOR_SKIN_ALERT; // original yellow-eyes skin // Если надо спаунить крематора с иным цветом глаз, подставь значение из npc_cremator_h. 
	m_iAmmo				= m_iMaxAmmo = 54;

	NPCInit();

	m_flDistTooFar		= 6000.0;
	GetSenses()->SetDistLook( 6000.0 -1 );
	
	m_flNextIdleSoundTime	= gpGlobals->curtime; // + random->RandomFloat( 14.0f, 28.0f );
	m_flNextRangeAttack2Time = gpGlobals->curtime; // + random->RandomFloat( 10.0f, 20.0f );

	m_MuzzleAttachment	= LookupAttachment( "muzzle" );
	m_HeadAttachment	= LookupAttachment( "headattachment" );
		
	CapabilitiesAdd( bits_CAP_MOVE_GROUND | bits_CAP_TURN_HEAD );
	CapabilitiesAdd( bits_CAP_INNATE_RANGE_ATTACK1 ); // flamethrower
	CapabilitiesAdd( bits_CAP_INNATE_RANGE_ATTACK2 );

	CapabilitiesAdd( bits_CAP_MOVE_SHOOT ); // TODO: Melee?
}
Class_T	CNPC_Cremator::Classify ( void )
{
	return	CLASS_COMBINE; // Крематор наследует общие правила для всех нпц Альянса. 
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#define CREM_ATTN_IDLE	(float) 4.5

// Звук в «простое» — дыхание, шелест плаща.
void CNPC_Cremator::IdleSound( void ) // TODO: replace with modern script-based system
{
	CPASAttenuationFilter filter( this, CREM_ATTN_IDLE );
	switch ( random->RandomInt(0,2) )
	{
	case 0:	
		enginesound->EmitSound( filter, entindex(), CHAN_VOICE, "npc/cremator/amb1.wav", 1, CREM_ATTN_IDLE );	
		break;
	case 1:	
		enginesound->EmitSound( filter, entindex(), CHAN_VOICE, "npc/cremator/amb2.wav", 1, CREM_ATTN_IDLE );	
		break;
	case 2:	
		enginesound->EmitSound( filter, entindex(), CHAN_VOICE, "npc/cremator/amb3.wav", 1, CREM_ATTN_IDLE );	
		break;
	}
	enginesound->EmitSound( filter, entindex(), CHAN_VOICE, "npc/cremator/amb_loop.wav", 1, ATTN_NORM );

	m_flNextIdleSoundTime = gpGlobals->curtime + random->RandomFloat( 14.0f, 28.0f );
}
// Звук при встревоженности.
void CNPC_Cremator::AlertSound( void )
{
	int iPitch = random->RandomInt( 90, 105 );

	CPASAttenuationFilter filter( this );
	switch ( random->RandomInt ( 0, 1  ) )
	{
	case 0:
		enginesound->EmitSound( filter, entindex(), CHAN_VOICE, "npc/cremator/alert_object.wav", 1, ATTN_NORM, 0, iPitch );	
		break;
	case 1:
		enginesound->EmitSound( filter, entindex(), CHAN_VOICE, "npc/cremator/alert_player.wav", 1, ATTN_NORM, 0, iPitch );	
		break;
	}
}
// Шипение пара при смерти.
void CNPC_Cremator::DeathSound( const CTakeDamageInfo &info )
{
	CPASAttenuationFilter filter( this );
	int iPitch = random->RandomInt( 90, 105 );
	enginesound->EmitSound( filter, entindex(), CHAN_VOICE, "npc/cremator/crem_die.wav", 1, ATTN_NORM, 0, iPitch );
}
//-----------------------------------------------------------------------------
// Определяет скорость разворота при ходьбе и атаке.
//-----------------------------------------------------------------------------
float CNPC_Cremator::MaxYawSpeed( void )
{
	float flYS = 0;

	switch ( GetActivity() )
	{
	case	ACT_WALK_HURT:		flYS = 60;	break;
	case	ACT_RUN:			flYS = 120;	break;
	case	ACT_IDLE:			flYS = 90;	break;
	case	ACT_RANGE_ATTACK1:	flYS = 60;	break;
	default:
		flYS = 90;
		break;
	}
	return flYS;
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// отдельная модель головы
const char *CNPC_Cremator::GetHeadpropModel( void )
{
	return "models/crematorhead.mdl";
}
// ф-ция вызывается при смерти крематора; «выключает» глаза, убирает иммолатор и опционально отрывает голову.
void CNPC_Cremator::Event_Killed( const CTakeDamageInfo &info  )
{	
	CTakeDamageInfo Info = info;

	if( (Info.GetAmmoType() == GetAmmoDef()->Index( ".50BMG" ))
		|| (Info.GetAmmoType() == GetAmmoDef()->Index( "Buckshot" ))
		|| (Info.GetAmmoType() == GetAmmoDef()->Index( "Gauss" ))
		|| (Info.GetAmmoType() == GetAmmoDef()->Index( "XBowBolt" ))
		|| (Info.GetAmmoType() == GetAmmoDef()->Index( "357" ))
		&& ( m_bHeadshot == 1 ) )
	{		
		SetBodygroup (2, 1); // turn the head off
		DropHead ( 65 );
	}
	else if( Info.GetDamageType() == DMG_BLAST )
	{
		SetBodygroup( 2,1 );
		DropHead( 300 );
	}
	m_nSkin = CREMATOR_SKIN_DEAD; // turn the eyes black

	SetBodygroup (1, 1); // turn the gun off
	BaseClass::Event_Killed( info );
}
// функция отрывания и выбрасывания головы
// Cremator's head is separated from the body and dropped on death. 
// Input: head's velocity.
// TODO: make it part of brickbat ammo list.
void CNPC_Cremator::DropHead( int iVelocity ) 
{
	CPhysicsProp *pGib = assert_cast<CPhysicsProp*>(CreateEntityByName( "prop_physics" ));
	pGib->SetAbsOrigin( GetAbsOrigin() );
	pGib->SetAbsAngles( GetAbsAngles() );
	pGib->SetAbsVelocity( GetAbsVelocity() );
	pGib->SetModel( GetHeadpropModel() );
	pGib->Spawn();
	pGib->SetMoveType( MOVETYPE_VPHYSICS );

	Vector vecVelocity;
	pGib->GetMassCenter( &vecVelocity );
	vecVelocity -= WorldSpaceCenter();
	vecVelocity.z = fabs(vecVelocity.z);
	VectorNormalize( vecVelocity );

	float flRandomVel = random->RandomFloat( 35, 75 );
	vecVelocity *= (iVelocity * flRandomVel) / 15;
	vecVelocity.z += 100.0f;
	AngularImpulse angImpulse = RandomAngularImpulse( -500, 500 );
			
	IPhysicsObject *pObj = pGib->VPhysicsGetObject();
	if ( pObj != NULL )
	{
		pObj->AddVelocity( &vecVelocity, &angImpulse );
	}
	pGib->SetCollisionGroup( COLLISION_GROUP_INTERACTIVE );
}
// Звук шага и пыль от поступи крематора, левый шаг.
Vector CNPC_Cremator::LeftFootHit( float eventtime )
{	
	Vector footPosition;
	
	GetAttachment( "footleft", footPosition );
	CPASAttenuationFilter filter( this );
	EmitSound( filter, entindex(), "NPC_Cremator.FootstepLeft", &footPosition, eventtime );

	FootstepEffect( footPosition );
	return footPosition;
}
// Звук шага и пыль от поступи крематора, правый шаг.
Vector CNPC_Cremator::RightFootHit( float eventtime )
{	
	Vector footPosition;

	GetAttachment( "footright", footPosition );
	CPASAttenuationFilter filter( this );
	EmitSound( filter, entindex(), "NPC_Cremator.FootstepRight", &footPosition, eventtime );

	FootstepEffect( footPosition );
	return footPosition;
}
// Эффект пыли для шагов крематора (аналогично шагам страйдера или охотника, но в меньших масштабах
void CNPC_Cremator::FootstepEffect( const Vector &origin )
{
	trace_t tr;
	AI_TraceLine( origin, origin - Vector(0,0,0), MASK_SOLID_BRUSHONLY, this, COLLISION_GROUP_NONE, &tr );
	float yaw = random->RandomInt(0,0);
	for ( int i = 0; i < 2; i++ )
	{
		if ( UTIL_PointContents( tr.endpos + Vector( 0, 0, 1 ) ) & MASK_WATER )
		{
			float flWaterZ = UTIL_FindWaterSurface( tr.endpos, tr.endpos.z, tr.endpos.z + 100.0f );

			CEffectData	data;
			data.m_fFlags = 0;
			data.m_vOrigin = tr.endpos;
			data.m_vOrigin.z = flWaterZ;
			data.m_vNormal = Vector( 0, 0, 1 );
			data.m_flScale = random->RandomFloat( 10.0, 14.0 );

			// Если крематор идет по неглубокой воде, образуются всплески.
			DispatchEffect( "watersplash", data );
		}
		
		else
		{
			Vector dir = UTIL_YawToVector( yaw + i*180 ) * 10;
			VectorNormalize( dir );
			dir.z = 0.25;
			VectorNormalize( dir );
			g_pEffects->Dust( tr.endpos, dir, 12, 50 );

			/*g_pEffects->FootprintDecal( tr.endpos, dir, 12, 50 );

			virtual void FootprintDecal( IRecipientFilter& filer, float delay, const Vector *origin, const Vector* right, 
		int entity, int index, unsigned char materialType ) = 0;
			virtual void Dust( IRecipientFilter& filer, float delay,
				 const Vector &pos, const Vector &dir, float size, float speed ) = 0;*/
		}
	}
}
// Крематор игнорирует урон огнем
int CNPC_Cremator::OnTakeDamage_Alive( const CTakeDamageInfo &info )
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
// Далее следует массивая таблица просчета повреждений и урона от разных типов вооружения.
void CNPC_Cremator::TraceAttack( const CTakeDamageInfo &info, const Vector &vecDir, trace_t *ptr )
{	
	CTakeDamageInfo newInfo = info;
	// the head and the abdomen sphere are the only vulnerable parts,
	// the legs, arms and stuff under the cloak is likely just some machinery,
	// and the C. is encountered in City after the lab and in Canals when
	// the player doesn't have weapon apart from crowbar, pistol and SMG
	// TODO: torn clothes with bullets, exposing underlying machinery
	if( (newInfo.GetDamageType() & DMG_BULLET || DMG_CLUB || DMG_BUCKSHOT || DMG_ENERGYBEAM ) ) 
	{			
		// the multipliers here are so small because of the innate 
		// headshot multiplayer which makes killing a cremator with headshots
		// ridiculously easy even on Hard. So all of them were divided by 6.
		if( ptr->hitgroup == HITGROUP_HEAD ) 
		{			
			m_bHeadshot = 1;

			if( g_pGameRules->IsSkillLevel(SKILL_EASY) )
			{
				if( newInfo.GetAmmoType() == GetAmmoDef()->Index(".50BMG") ) // base damage 100
				{
					newInfo.ScaleDamage( 1.0 );
				}
				else if( newInfo.GetAmmoType() == GetAmmoDef()->Index("357") ) // base damage 40
				{
					newInfo.ScaleDamage( 0.6 );
				}
				else if( newInfo.GetAmmoType() == GetAmmoDef()->Index("9x19") )
				{
					newInfo.ScaleDamage( 0.1 );
				}
				else if( newInfo.GetAmmoType() == GetAmmoDef()->Index("10mm") )
				{
					newInfo.ScaleDamage( 0.75 );
				}
				else if( newInfo.GetAmmoType() == GetAmmoDef()->Index(".556x45") )
				{
					newInfo.ScaleDamage( 1.0 );
				}
				else if( newInfo.GetAmmoType() == GetAmmoDef()->Index("Buckshot") )
				{
					newInfo.ScaleDamage( 0.5 );
				}
				else if( newInfo.GetAmmoType() == GetAmmoDef()->Index("Uranium") )
				{
					newInfo.ScaleDamage( 0.192 );
				}
				else if( newInfo.GetAmmoType() == GetAmmoDef()->Index("CombineRounds") )
				{
					newInfo.ScaleDamage( 1.5 );
				}
			}
			else if( g_pGameRules->IsSkillLevel(SKILL_HARD) )
			{
				if( newInfo.GetAmmoType() == GetAmmoDef()->Index(".50BMG") ) // base damage 100
				{
					newInfo.ScaleDamage( 0.33 );
				}
				else if( newInfo.GetAmmoType() == GetAmmoDef()->Index("357") ) // base damage 40
				{
					newInfo.ScaleDamage( 0.33 );
				}
				else if( newInfo.GetAmmoType() == GetAmmoDef()->Index("9x19") )
				{
					newInfo.ScaleDamage( 0.1 );
				}
				else if( newInfo.GetAmmoType() == GetAmmoDef()->Index("10mm") )
				{
					newInfo.ScaleDamage( 0.5 );
				}
				else if( newInfo.GetAmmoType() == GetAmmoDef()->Index(".556x45") )
				{
					newInfo.ScaleDamage( 0.65 );
				}
				else if( newInfo.GetAmmoType() == GetAmmoDef()->Index("Buckshot") )
				{
					newInfo.ScaleDamage( 0.45 );
				}
				else if( newInfo.GetAmmoType() == GetAmmoDef()->Index("Uranium") )
				{
					newInfo.ScaleDamage( 0.146 );
				}
				else if( newInfo.GetAmmoType() == GetAmmoDef()->Index("CombineRounds") )
				{
					newInfo.ScaleDamage( 2.5 );
				}
			}			
			else
			{
				if( newInfo.GetAmmoType() == GetAmmoDef()->Index(".50BMG") )
				{
					newInfo.ScaleDamage( 0.66  );
				}
				else if( newInfo.GetAmmoType() == GetAmmoDef()->Index("357") )
				{
					newInfo.ScaleDamage( 0.5  );
				}
				else if( newInfo.GetAmmoType() == GetAmmoDef()->Index("9x19") )
				{
					newInfo.ScaleDamage( 0.1  );
				}
				else if( newInfo.GetAmmoType() == GetAmmoDef()->Index("10mm") )
				{
					newInfo.ScaleDamage( 0.66 );
				}
				else if( newInfo.GetAmmoType() == GetAmmoDef()->Index(".556x45") )
				{
					newInfo.ScaleDamage( 0.75 );
				}
				else if( newInfo.GetAmmoType() == GetAmmoDef()->Index("Buckshot") )
				{
					newInfo.ScaleDamage( 0.4 );
				}
				else if( newInfo.GetAmmoType() == GetAmmoDef()->Index("Uranium") )
				{
					newInfo.ScaleDamage( 0.16 );
				}
				else if( newInfo.GetAmmoType() == GetAmmoDef()->Index("CombineRounds") )
				{
					newInfo.ScaleDamage( 2.0 );
				}
			}
		}
		else if( ptr->hitgroup == CREMATOR_CANISTER )
		{
			m_bCanisterShot = 1; // попадание в канистру на животе.

			if( g_pGameRules->IsSkillLevel(SKILL_EASY) )
			{
				if( newInfo.GetAmmoType() == GetAmmoDef()->Index(".50BMG") )
				{
					newInfo.ScaleDamage( 1.5 );
				}
				else if( newInfo.GetAmmoType() == GetAmmoDef()->Index("357") )
				{
					newInfo.ScaleDamage( 2.0 );
				}
				else if( newInfo.GetAmmoType() == GetAmmoDef()->Index("9x19") )
				{
					newInfo.ScaleDamage( 0.66 );
				}
				else if( newInfo.GetAmmoType() == GetAmmoDef()->Index(".556x45") )
				{
					newInfo.ScaleDamage( 1.15 );
				}
				else if( newInfo.GetAmmoType() == GetAmmoDef()->Index("Buckshot") )
				{
					newInfo.ScaleDamage( 1.15 );
				}
				else if( newInfo.GetAmmoType() == GetAmmoDef()->Index("Uranium") )
				{
					newInfo.ScaleDamage( 1.15 );
				}
			}
			else if( g_pGameRules->IsSkillLevel(SKILL_HARD) )
			{
				if( newInfo.GetAmmoType() == GetAmmoDef()->Index(".50BMG") )
				{
					newInfo.ScaleDamage( 1.15 );
				}
				else if( newInfo.GetAmmoType() == GetAmmoDef()->Index("357") )
				{
					newInfo.ScaleDamage( 1.75 );
				}
				else if( newInfo.GetAmmoType() == GetAmmoDef()->Index("9x19") )
				{
					newInfo.ScaleDamage( 0.33 );
				}
				else if( newInfo.GetAmmoType() == GetAmmoDef()->Index(".556x45") )
				{
					newInfo.ScaleDamage( 0.75 );
				}
				else if( newInfo.GetAmmoType() == GetAmmoDef()->Index("Buckshot") )
				{
					newInfo.ScaleDamage( 0.75 );
				}
				else if( newInfo.GetAmmoType() == GetAmmoDef()->Index("Uranium") )
				{
					newInfo.ScaleDamage( 0.875 );
				}
			}
			else if (g_pGameRules->IsSkillLevel(SKILL_VERYHARD))
			{
				if (newInfo.GetAmmoType() == GetAmmoDef()->Index(".50BMG"))
				{
					newInfo.ScaleDamage(1.05);
				}
				else if (newInfo.GetAmmoType() == GetAmmoDef()->Index("357"))
				{
					newInfo.ScaleDamage(1.50);
				}
				else if (newInfo.GetAmmoType() == GetAmmoDef()->Index("9x19"))
				{
					newInfo.ScaleDamage(0.25);
				}
				else if (newInfo.GetAmmoType() == GetAmmoDef()->Index(".556x45"))
				{
					newInfo.ScaleDamage(0.50);
				}
				else if (newInfo.GetAmmoType() == GetAmmoDef()->Index("Buckshot"))
				{
					newInfo.ScaleDamage(0.50);
				}
				else if (newInfo.GetAmmoType() == GetAmmoDef()->Index("Uranium"))
				{
					newInfo.ScaleDamage(0.65);
				}
			}
			else if (g_pGameRules->IsSkillLevel(SKILL_HARD))
			{
				if (newInfo.GetAmmoType() == GetAmmoDef()->Index(".50BMG"))
				{
					newInfo.ScaleDamage(0.85);
				}
				else if (newInfo.GetAmmoType() == GetAmmoDef()->Index("357"))
				{
					newInfo.ScaleDamage(1.25);
				}
				else if (newInfo.GetAmmoType() == GetAmmoDef()->Index("9x19"))
				{
					newInfo.ScaleDamage(0.15);
				}
				else if (newInfo.GetAmmoType() == GetAmmoDef()->Index(".556x45"))
				{
					newInfo.ScaleDamage(0.25);
				}
				else if (newInfo.GetAmmoType() == GetAmmoDef()->Index("Buckshot"))
				{
					newInfo.ScaleDamage(0.25);
				}
				else if (newInfo.GetAmmoType() == GetAmmoDef()->Index("Uranium"))
				{
					newInfo.ScaleDamage(0.50);
				}
			}
			else
			{
				if( newInfo.GetAmmoType() == GetAmmoDef()->Index(".50BMG") )
				{
					newInfo.ScaleDamage( 1.25 );
				}
				else if( newInfo.GetAmmoType() == GetAmmoDef()->Index("357") )
				{
					newInfo.ScaleDamage( 1.5 );
				}
				else if( newInfo.GetAmmoType() == GetAmmoDef()->Index("9x19") )
				{
					newInfo.ScaleDamage( 0.5 );
				}
				else if( newInfo.GetAmmoType() == GetAmmoDef()->Index(".556x45") )
				{
					newInfo.ScaleDamage( 1.0 );
				}
				else if( newInfo.GetAmmoType() == GetAmmoDef()->Index("Buckshot") )
				{
					newInfo.ScaleDamage( 1.0 );
				}
				else if( newInfo.GetAmmoType() == GetAmmoDef()->Index("Uranium") )
				{
					newInfo.ScaleDamage( 1.0 );
				}
			}
		}
/*		// however, if the sprayer is damaged, something unpleasant may happen
		else if (ptr->hitgroup == CREMATOR_GUN )
		{
		// well, right now I can't think of any specific effect
			DevMsg( "Cremator: gear is damaged\n");
		}*/

		else
		{
			newInfo.ScaleDamage( 0.75 ); // В общем случае, крематор получает толко 25% урона (т.е. имеет сопротивление 75)
			DevMsg( "Generic 0.25 Non-specified\n" );
		}
	}
//	BaseClass::TraceAttack( newInfo, vecDir, ptr );
}
void CNPC_Cremator::Ignite( float flFlameLifetime, bool bNPCOnly, float flSize, bool bCalledByLevelDesigner )
{
	BaseClass::Ignite( CREMATOR_BURN_TIME, bNPCOnly, flSize, bCalledByLevelDesigner );
	//if( IsOnFire() && di_dynamic_lighting_npconfire.GetBool() == 1 )
	// NOT NEEDED ANYMORE SINCE ANY BURNING NPC WILL 
	// EMIT EF_DIMLIGHT
	//{
	//	GetEffectEntity()->AddEffects( EF_DIMLIGHT );
	//}
}
//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
void CNPC_Cremator::StartTask( const Task_t *pTask )
{
	switch ( pTask->iTask )
	{
	case TASK_CREMATOR_IDLE:
		{
			SetActivity( ACT_IDLE );
			if( IsActivityFinished() )
			{
				TaskComplete();
			}
			break;
		}
	case TASK_CREMATOR_RANGE_ATTACK1:
		{
			Vector flEnemyLKP = GetEnemyLKP();
			GetMotor()->SetIdealYawToTarget( flEnemyLKP );
			break;
		}
		default:
		BaseClass::StartTask( pTask );
		break;
	}
}
void CNPC_Cremator::RunTask( const Task_t *pTask )
{
	switch ( pTask->iTask )
	{
	case TASK_CREMATOR_RANGE_ATTACK1:
		{
			SetActivity( ACT_RANGE_ATTACK1 );

			Vector flEnemyLKP = GetEnemyLKP();
			GetMotor()->SetIdealYawToTargetAndUpdate( flEnemyLKP );

			if( m_iAmmo < 1 && IsActivityFinished() )
			{
				SetCondition( COND_CREMATOR_OUT_OF_AMMO );
								
				DevMsg( "NO PRIMARY AMMO\n" );

				StopParticleEffects(this);
				StopSound( "Weapon_Immolator.Single" );
				EmitSound( "Weapon_Immolator.Stop" );
				
				TaskComplete();
				SetNextThink( gpGlobals->curtime + 0.1f );
			}

			// THIS fixes the combat issue with the Cremator continuing to fire at a target that moved out of his reach
			if( GetEnemyLKP().DistTo( GetAbsOrigin()) > CREMATOR_MAX_RANGE )
			{
				// Cremator stops firing and attempts to close the distance.
				SetActivity( ACT_CREMATOR_DISARM );
				TaskComplete();
				Msg( "Enemy is too far\n" );

				SetNextThink( gpGlobals->curtime + 0.1f );
				return;
			}
	/*		// This is bugged and shouldn't be used. Necessary checks are made below, in OnChangeActivity( ).	
			if( IsActivityMovementPhased( ACT_WALK ) || IsActivityMovementPhased( ACT_RUN ) )
			{
				TaskFail( NULL );
				SetActivity( ACT_CREMATOR_DISARM );
				
				DevMsg( "ACT_CREMATOR_DISARM\n" );
				return;
			}
	*/
			break;
		}
		default:
		BaseClass::RunTask( pTask );
		break;
	}
}
// Если крематор начинает двигаться к удаляющейся от него цели, он прекращает атаку.
// If target moves out of reach and a cremator starts going after it, put the flame off.
void CNPC_Cremator::OnChangeActivity( Activity eNewActivity ) 
{
	if ( eNewActivity == ACT_WALK || eNewActivity == ACT_RUN )
	{
		StopParticleEffects( this );
		StopSound( "Weapon_Immolator.Single" );
		EmitSound( "Weapon_Immolator.Stop" );
		DevMsg( "Activity Changed\n" );
	}
}
// Здесь происходит назначение различных ф-ций во время анимации. Аним-ивенты назначаются в .qc.
void CNPC_Cremator::HandleAnimEvent( animevent_t *pEvent )
{
	switch( pEvent->event )
	{
		case CREMATOR_AE_FLLEFT: // левый шаг
			{	
				LeftFootHit( pEvent->eventtime );
			}
			break;

		case CREMATOR_AE_FLRIGHT: // правый шаг
			{				
				RightFootHit( pEvent->eventtime );
			}
			break;

		case CREMATOR_AE_IMMO_START: // начало анимации атаки
			{
				//CBaseEntity *pEntity;

				DispatchSpray( this );
				DevMsg( "%i ammo left\n", m_iAmmo );

				Vector flEnemyLKP = GetEnemyLKP();
				GetMotor()->SetIdealYawToTargetAndUpdate( flEnemyLKP );
			}
			break;

		case CREMATOR_AE_IMMO_PARTICLE: // Маркер для запуска системы частиц огнемета
			{				
				DispatchParticleEffect( "flamethrower", PATTACH_POINT_FOLLOW, this, "muzzle" ); 
				// Если нужно заменить зеленый огонь оранжевым, замени "flamethrower" на "flamethrower_orange".
				EmitSound( "Weapon_Immolator.Single" );
			}
			break;

		case CREMATOR_AE_IMMO_PARTICLEOFF: // Маркер для отключения системы частиц огнемета
			{				
				StopParticleEffects( this );
				StopSound( "Weapon_Immolator.Single" );
				EmitSound( "Weapon_Immolator.Stop" );
			}
			break;

		case CREMATOR_AE_RELOAD:
			{
				ClearCondition( COND_CREMATOR_OUT_OF_AMMO );
				ClearCondition( COND_NO_PRIMARY_AMMO );
				m_iAmmo += 54; // Put your own int here. This defines for how long a cremator would be able to fire at an enemy.
				
				DevMsg( "AE_RELOAD\n" );
			}
			break;

		case CREMATOR_AE_THROWGRENADE: // Маркер для броска гранаты
			{				
				DevMsg( "Throwing incendiary grenade!\n" );
				ThrowIncendiaryGrenade();

				if( g_pGameRules->IsSkillLevel(SKILL_EASY) )
				{
					m_flNextRangeAttack2Time = gpGlobals->curtime + random->RandomFloat( 15.0f, 30.0f );
				}
				else if( g_pGameRules->IsSkillLevel(SKILL_HARD) )
				{
					m_flNextRangeAttack2Time = gpGlobals->curtime + random->RandomFloat( 5.0f, 10.0f );
				}
				else if (g_pGameRules->IsSkillLevel(SKILL_VERYHARD))
				{
					m_flNextRangeAttack2Time = gpGlobals->curtime + random->RandomFloat(3.0f, 5.0f);
				}
				else if (g_pGameRules->IsSkillLevel(SKILL_NIGHTMARE))
				{
					m_flNextRangeAttack2Time = gpGlobals->curtime + random->RandomFloat(1.0f, 3.0f);
				}
				else
				{
					m_flNextRangeAttack2Time = gpGlobals->curtime + random->RandomFloat( 10.0f, 20.0f );
				}
			}
			break;

		default:
			BaseClass::HandleAnimEvent( pEvent );
			break;
	}
}
// Атака зажигательными гранатами. Отключена. Для включения см. комментарий в ф-ции Spawn()
// See the function and the and of npc_cremator.h
void CNPC_Cremator::ThrowIncendiaryGrenade( void )
{
	Vector vecStart;
	GetAttachment( "anim_attachment_LH", vecStart );

	Vector forward, up, right, vecThrow;

	GetVectors( &forward, &right, &up );
	vecThrow = forward * 450 + up * 175 + right * random->RandomFloat(-15, 5);
	
	CGrenadeIncendiary *pIncendiary = (CGrenadeIncendiary*)Create( "grenade_incendiary", vecStart, vec3_angle, this );

	pIncendiary->SetAbsVelocity( vecThrow );
	pIncendiary->SetGravity( 1.5f );
	pIncendiary->SetLocalAngularVelocity( RandomAngle( -400, 400 ) );
	pIncendiary->SetMoveType( MOVETYPE_FLYGRAVITY, MOVECOLLIDE_FLY_CUSTOM ); 
}
// Основная атака огнеметом
// This is the cremator's main attack, or more precisely, its damage function.
void CNPC_Cremator::DispatchSpray( CBaseEntity *pEntity )
{ 
	Vector vecSrc, vecAim;
	trace_t tr;	

	//const char *entityname = pEntity->GetClassname();

	Vector forward, right, up;
	AngleVectors( GetAbsAngles(), &forward, &right, &up );

	vecSrc = GetAbsOrigin() + up * 36;
	vecAim = GetShootEnemyDir( vecSrc );
	float deflection = 0.01;	
	vecAim = vecAim + 1 * right * random->RandomFloat( 0, deflection ) + up * random->RandomFloat( -deflection, deflection );
	UTIL_TraceLine ( vecSrc, vecSrc + vecAim * 512, MASK_SOLID, this, COLLISION_GROUP_NONE, &tr);
			
/*	if ( tr.DidHitWorld() ) // spawn flames on solid surfaces. 
							// It's not very important since it is extremely rare for a cremator to
							// hit brush geometry but might be a nice feature for a close-space combat
							// it also works fine but again is EXTREMELY hard to get in-game
	{
		Vector	ofsDir = ( tr.endpos - GetAbsOrigin() );
		float	offset = VectorNormalize( ofsDir );

		if ( offset > 128 )
			offset = 128;

		float scale	 = 0.1f + ( 0.75f * ( 1.0f - ( offset / 128.0f ) ) );
		float growth = 0.1f + ( 0.75f * (offset / 128.0f ) );

		if ( tr.surface.flags & CONTENTS_GRATE ) // get smaller flames on grates since they have a smaller burning area
		{
			scale = 0.1f + ( 0.15f * ( 1.0f - ( offset / 128.0f ) ) );
		}
		else
		{
			scale = 0.1f + ( 0.75f * ( 1.0f - ( offset / 128.0f ) ) );
		}
		FireSystem_StartFire( tr.endpos, scale, growth, 8.0, 10.0f, (SF_FIRE_START_ON|SF_FIRE_START_FULL), (CBaseEntity*) this, FIRE_NATURAL );

	}	
*/

	pEntity = tr.m_pEnt;

	if ( pEntity != NULL && m_takedamage )
	{
		CTakeDamageInfo firedamage( this, this, sk_cremator_firedamage.GetFloat(), DMG_BURN );
		CTakeDamageInfo radiusdamage( this, this, sk_cremator_radiusdamage.GetFloat(), DMG_PLASMA ); 
		CalculateMeleeDamageForce( &firedamage, vecAim, tr.endpos );
		RadiusDamage ( CTakeDamageInfo ( this, this, 2, DMG_PLASMA ), // AOE; this stuff makes cremators absurdly powerfull sometimes btw
			tr.endpos,
			64.0f,
			CLASS_NONE,
			NULL );

		pEntity->DispatchTraceAttack( ( firedamage ), vecAim, &tr );
		
		Vector flEnemyLKP = pEntity->GetAbsOrigin();
		GetMotor()->SetIdealYawToTargetAndUpdate( flEnemyLKP );

		ClearMultiDamage();
	}	

	m_iAmmo --;
}
// Выбор скина для глаз крематора в зависимости от обстановки (уместно сравнить с Большими Папочками в Bioshock). В настоящее время не работает.
void CNPC_Cremator::SelectSkin( void ) // doesn't quite work, it can turn eyes red ("combat" skin) when combating an enemy, but will not return to calm/alert skin when an enemy is dead
{
	if(m_NPCState == NPC_STATE_COMBAT)
	{
		m_nSkin = 3;
		if ( HasCondition( COND_ENEMY_DEAD ) )
		{
			m_nSkin = 0;
		}
		if ( GetEnemy() == NULL )
		{
			m_nSkin = 1;
		}
	}
	if(m_NPCState == NPC_STATE_ALERT)
	{
		m_nSkin = 3;
		if ( GetEnemy() == NULL )
		{
			m_nSkin = 1;
		}
	}
	if(m_NPCState == NPC_STATE_NONE)
	{
		m_nSkin = 1;
	}
}
NPC_STATE CNPC_Cremator::SelectIdealState( void )
{
	switch( m_NPCState )
	{
	case NPC_STATE_COMBAT:
		{
			if ( GetEnemy() == NULL )
			{
				if ( !HasCondition( COND_ENEMY_DEAD ) )
				{
					SetCondition( COND_ENEMY_DEAD ); // TODO: patrolling

				}
				return NPC_STATE_ALERT;
			}
			else if ( HasCondition( COND_ENEMY_DEAD ) )
			{
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
#if 0
Activity CNPC_Cremator::NPC_TranslateActivity( Activity baseAct )
{
	if ( baseAct == ACT_WALK || ACT_RUN )
	{
		if ( m_iHealth < ( sk_cremator_health.GetBool() * 0.5 ) )
		{
			//return (Activity)ACT_WALK_HURT; // the animaton is broken, don't use it
		}
	}
	//return BaseClass::NPC_TranslateActivity( baseAct );
	return baseAct;
}
#endif
// Условия для основной атаки.
int CNPC_Cremator::RangeAttack1Conditions( float flDot, float flDist )
{
	if ( flDist > CREMATOR_MAX_RANGE )
		return COND_TOO_FAR_TO_ATTACK; // если цель слишком далеко, не атаковать.

	if ( flDist <= CREMATOR_INITIAL_RANGE )
		return COND_CAN_RANGE_ATTACK1;

	if ( flDot < 0.7f )
		return COND_NOT_FACING_ATTACK; // если цель вне поля зрения, не атаковать.

	if ( m_iAmmo < 1 )
		return COND_CREMATOR_OUT_OF_AMMO;

	return COND_NONE; // в противном случае, атака разрешается.
}
// Условия для вторичной атаки.
int CNPC_Cremator::RangeAttack2Conditions( float flDot, float flDist )
{
	if ( flDist < CREMATOR_MAX_RANGE / 2 )
		return 0;

	if ( flDot < 0.7 )
		return 0;

	if ( HasCondition( COND_CREMATOR_HAS_THROWN_GRENADE ))
		return 0;

	return COND_CAN_RANGE_ATTACK2;
}
int CNPC_Cremator::SelectSchedule( void )
{
	switch	( m_NPCState )
	{
	case NPC_STATE_IDLE:
	case NPC_STATE_ALERT:
		{				
			if ( HasCondition ( COND_HEAR_COMBAT || COND_HEAR_PLAYER || COND_HEAR_BULLET_IMPACT ) )
				return SCHED_INVESTIGATE_SOUND;

			return SCHED_PATROL_WALK_LOOP;
		}
		break;
	case NPC_STATE_COMBAT:
		{
			if( HasCondition ( COND_CAN_RANGE_ATTACK1 ) && m_iAmmo > 0 )
			{
				return SCHED_RANGE_ATTACK1;
			}
			if( HasCondition( COND_TOO_FAR_TO_ATTACK && COND_SEE_ENEMY ) )
			{
				if( m_flNextRangeAttack2Time < gpGlobals->curtime )
				{
					return SCHED_RANGE_ATTACK2; // Это атака зажигательными гранатами. Они могут привести к вылету на дисплейсментах; в таком случае, закомментируй эту строку.
				}
				else
				{
					return SCHED_CREMATOR_CHASE;
				}
			}
			if( HasCondition( COND_TOO_FAR_TO_ATTACK && COND_ENEMY_OCCLUDED )) //&& HasCondition( COND_CREMATOR_HAS_THROWN_GRENADE ))
			{
				return SCHED_CREMATOR_CHASE;
				StopParticleEffects( this );
			}
		}
		break;
	}
	return BaseClass::SelectSchedule();
}
int CNPC_Cremator::TranslateSchedule( int type ) 
{
	switch( type )
	{
	case SCHED_RANGE_ATTACK1:
		return SCHED_CREMATOR_RANGE_ATTACK1;
		break;
	case SCHED_CHASE_ENEMY:
		return SCHED_CREMATOR_CHASE;
		break;
	}
	return BaseClass::TranslateSchedule( type );
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
int CNPC_Cremator::GetSoundInterests( void )
{
	return	SOUND_WORLD	|
			SOUND_COMBAT	|
			SOUND_BULLET_IMPACT |
		    SOUND_CARCASS	|
			SOUND_MEAT		|
			SOUND_GARBAGE	|
			SOUND_PLAYER	|
			SOUND_MOVE_AWAY;
}
AI_BEGIN_CUSTOM_NPC( npc_cremator, CNPC_Cremator )

	DECLARE_CONDITION( COND_CREMATOR_OUT_OF_AMMO )
	DECLARE_TASK( TASK_CREMATOR_RANGE_ATTACK1 )
	DECLARE_TASK( TASK_CREMATOR_RELOAD )
	DECLARE_TASK( TASK_CREMATOR_IDLE )
	DECLARE_ACTIVITY( ACT_FIREINOUT )
	DECLARE_ACTIVITY( ACT_CREMATOR_ARM )
	DECLARE_ACTIVITY( ACT_CREMATOR_DISARM )
	DECLARE_ACTIVITY( ACT_CREMATOR_RELOAD )

	DEFINE_SCHEDULE
	(
		SCHED_CREMATOR_RANGE_ATTACK1,

		"	Tasks"
		"		TASK_STOP_MOVING			0"
		"		TASK_FACE_ENEMY				0"
		"		TASK_ANNOUNCE_ATTACK		1"
		"		TASK_PLAY_SEQUENCE			ACTIVITY:ACT_CREMATOR_ARM"
		"		TASK_CREMATOR_RANGE_ATTACK1	0"
		"		TASK_PLAY_SEQUENCE			ACTIVITY:ACT_CREMATOR_RELOAD"
		"	"
		"	Interrupts"
		"		COND_NEW_ENEMY"
		"		COND_ENEMY_DEAD"
		"		COND_TOO_FAR_TO_ATTACK"
	)
	DEFINE_SCHEDULE
	(
		SCHED_CREMATOR_CHASE,

		"	Tasks"
		"		TASK_SET_TOLERANCE_DISTANCE		200"
		"		 TASK_GET_CHASE_PATH_TO_ENEMY	3072"
		"		 TASK_RUN_PATH					1"
		"		 TASK_WAIT_FOR_MOVEMENT			0"
		"		 TASK_FACE_ENEMY				0"
		"	"
		"	Interrupts"
		"		COND_ENEMY_DEAD"
		"		COND_NEW_ENEMY"
		"		COND_CAN_RANGE_ATTACK1"
		"		COND_CAN_RANGE_ATTACK2"
	)
	DEFINE_SCHEDULE
	(
		SCHED_CREMATOR_IDLE,

		"	Tasks"
		"	TASK_PLAY_SEQUENCE			ACTIVITY:ACT_IDLE"
		"	"
		"	Interrupts"
		"		COND_NEW_ENEMY"
	)	
AI_END_CUSTOM_NPC()