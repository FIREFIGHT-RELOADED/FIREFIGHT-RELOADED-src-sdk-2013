//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//


#include "cbase.h"
#include "movevars_shared.h"

#if defined( TF_CLIENT_DLL ) || defined( TF_DLL )
#include "tf_gamerules.h"
#endif

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

// some cvars used by player movement system
#if defined( HL2_DLL ) || defined( HL2_CLIENT_DLL )
#define DEFAULT_GRAVITY_STRING	"600"
#else
#define DEFAULT_GRAVITY_STRING	"800"
#endif

// Mobility
#define DEFAULT_JUMP_HEIGHT_STRING "21.0"
#define DEFAULT_SLIDE_TIME_STRING "1000.0" 
#define DEFAULT_SLIDE_SPEED_BOOST_STRING "40.0"
#define DEFAULT_WALLRUN_TIME_STRING "1500.0"
#define DEFAULT_WALLRUN_SPEED_STRING "280.0"
#define DEFAULT_WALLRUN_BOOST_STRING "20.0"

float GetCurrentGravity( void )
{
#if defined( TF_CLIENT_DLL ) || defined( TF_DLL )
	if ( TFGameRules() )
	{
		return ( sv_gravity.GetFloat() * TFGameRules()->GetGravityMultiplier() );
	}
#endif 

	return sv_gravity.GetFloat();
}

float GetJumpHeight(void)
{
	return sv_jump_height.GetFloat();
}

float GetSlideTime( void )
{
	return sv_slide_time.GetFloat();
}
float GetWallRunTime( void )
{
	return sv_wallrun_time.GetFloat();
}
float GetSlideSpeedBoost( void )
{
	return sv_slide_speed_boost.GetFloat();
}
float GetWallRunSpeed( void )
{
	return sv_wallrun_speed.GetFloat();
}
float GetWallRunBoost( void )
{
	return sv_wallrun_boost.GetFloat();
}

ConVar	sv_gravity		( "sv_gravity", DEFAULT_GRAVITY_STRING, FCVAR_NOTIFY | FCVAR_REPLICATED | FCVAR_ARCHIVE, "World gravity." );

// Mobility 

ConVar  sv_jump_height  ( "sv_jump_height", DEFAULT_JUMP_HEIGHT_STRING, FCVAR_NOTIFY | FCVAR_REPLICATED | FCVAR_ARCHIVE, "Jump Height.");
ConVar  sv_slide("sv_slide", "1", FCVAR_NOTIFY | FCVAR_REPLICATED | FCVAR_ARCHIVE, "");
ConVar  sv_wallrun("sv_wallrun", "1", FCVAR_NOTIFY | FCVAR_REPLICATED | FCVAR_ARCHIVE, "");
ConVar  sv_slide_time( "sv_slide_time", DEFAULT_SLIDE_TIME_STRING, FCVAR_NOTIFY | FCVAR_REPLICATED | FCVAR_ARCHIVE, "Powerslide time." );
ConVar  
    sv_slide_speed_boost(
		"sv_slide_speed_boost",
		DEFAULT_SLIDE_SPEED_BOOST_STRING,
		FCVAR_NOTIFY | FCVAR_REPLICATED | FCVAR_ARCHIVE,
		"Speed boost for powerslide." );
ConVar 
    sv_wallrun_time( 
	    "sv_wallrun_time", 
		DEFAULT_WALLRUN_TIME_STRING, 
		FCVAR_NOTIFY | FCVAR_REPLICATED | FCVAR_ARCHIVE,
		"Wallrun max duration.");

ConVar
	sv_wallrun_anticipation(
		"sv_wallrun_anticipation",
		"0",
		FCVAR_NOTIFY | FCVAR_REPLICATED | FCVAR_ARCHIVE,
		"0 - none, 1 - Eye roll only, 2 - Full (bumps)." );

ConVar
	sv_wallrun_requiredirectcontrol(
		"sv_wallrun_requiredirectcontrol",
		"1",
		FCVAR_NOTIFY | FCVAR_REPLICATED | FCVAR_ARCHIVE,
		"Require a movement key to be pressed before attempting a wallrun.");

ConVar
	sv_wallrun_boost(
		"sv_wallrun_boost",
		DEFAULT_WALLRUN_BOOST_STRING,
		FCVAR_NOTIFY | FCVAR_REPLICATED | FCVAR_ARCHIVE,
		"Wallrun speed boost." );
ConVar
	sv_wallrun_jump_boost(
		"sv_wallrun_jump_boost",
		"0.15",
		FCVAR_NOTIFY | FCVAR_REPLICATED | FCVAR_ARCHIVE,
		"Fraction of wallrun speed to add to jump." );
ConVar
	sv_wallrun_jump_push(
		"sv_wallrun_jump_push",
		"0.25",
		FCVAR_NOTIFY | FCVAR_REPLICATED | FCVAR_ARCHIVE,
		"Fraction of wall jump speed to go to pushing off wall." );
 ConVar 
	 sv_wallrun_speed (
		 "sv_wallrun_speed",
		 DEFAULT_WALLRUN_SPEED_STRING,
		 FCVAR_NOTIFY | FCVAR_REPLICATED | FCVAR_ARCHIVE,
		 "Wallrun speed.");
 ConVar
	 sv_wallrun_accel(
		 "sv_wallrun_accel",
		 "4.25",
		 FCVAR_NOTIFY | FCVAR_REPLICATED | FCVAR_ARCHIVE,
		 "Wallrun acceleration." );

 ConVar
	 sv_wallrun_roll (
		 "sv_wallrun_roll",
		 "7.0",
		 FCVAR_NOTIFY | FCVAR_REPLICATED | FCVAR_ARCHIVE,
		 "Wallrun view roll angle.");

 ConVar
	 sv_wallrun_max_rise (
		 "sv_wallrun_max_rise",
		 "25.0",
		 FCVAR_NOTIFY | FCVAR_REPLICATED | FCVAR_ARCHIVE,
		 "Wallrun upward limit.");

 ConVar
	 sv_wallrun_min_rise (
		 "sv_wallrun_min_rise",
		 "-50.0",
		 FCVAR_NOTIFY | FCVAR_REPLICATED | FCVAR_ARCHIVE,
		 "Wallrun downward limit.");

 ConVar
	 sv_wallrun_scramble_z (
		 "sv_wallrun_scramble_z",
		 "28.0",
		 FCVAR_NOTIFY | FCVAR_REPLICATED | FCVAR_ARCHIVE,
		 "Height we can climb to.");

 ConVar
	 sv_wallrun_lookahead(
		 "sv_wallrun_lookahead",
		 "0.22",
		 FCVAR_NOTIFY | FCVAR_REPLICATED | FCVAR_ARCHIVE,
		 "Amount of time (in seconds) to lookahead for bumps or corners when wallrunning." );

 ConVar
	 sv_wallrun_inness(
		 "sv_wallrun_inness",
		 "360",
		 FCVAR_NOTIFY | FCVAR_REPLICATED | FCVAR_ARCHIVE,
		 "Scaling factor for how much to steer inward toward the wall when wallrunning" );
 ConVar
	 sv_wallrun_outness(
		 "sv_wallrun_outness",
		 "300",
		 FCVAR_NOTIFY | FCVAR_REPLICATED | FCVAR_ARCHIVE,
		 "Scaling factor for how much to steer outward around obstacles when wallrunning" );

 ConVar
	 sv_wallrun_lookness(
		 "sv_wallrun_lookness",
		 "1",
		 FCVAR_NOTIFY | FCVAR_REPLICATED | FCVAR_ARCHIVE,
		 "Scaling factor for how much to adjust view to look where you're going when wallrunning" );
 ConVar
	 sv_wallrun_look_delay(
		 "sv_wallrun_look_delay",
		 "0.3",
		 FCVAR_NOTIFY | FCVAR_REPLICATED | FCVAR_ARCHIVE,
		 "How long to wait before aligning the view with the move direction when wallrunning." );

 ConVar
	 sv_wallrun_feet_z(
		 "sv_wallrun_feet_z",
		 "10",
		 FCVAR_NOTIFY | FCVAR_REPLICATED | FCVAR_ARCHIVE,
		 "Fudge for starting a wallrun with your feet below the bottom edge of the wall" );

 ConVar 
	 sv_wallrun_stick_angle(
		 "sv_wallrun_stick_angle", 
		 "45",
		 FCVAR_NOTIFY | FCVAR_REPLICATED | FCVAR_ARCHIVE,
		 "Min angle away from wall norm for player to wallrun");

 ConVar 
	 sv_wallrun_corner_stick_angle(
		 "sv_wallrun_corner_stick_angle", 
		 "80",
		 FCVAR_NOTIFY | FCVAR_REPLICATED | FCVAR_ARCHIVE,
		 "End wallrun at end of wall if facing within this angle of wall norm" );
 ConVar
	 sv_duck_time(
		 "sv_duck_time",
		 "0.15",
		 FCVAR_NOTIFY | FCVAR_REPLICATED | FCVAR_ARCHIVE,
		 "Amount of time (in seconds) it takes to duck.",
		 true, 0.001, true, 1);

 ConVar
	 sv_coyote_time(
		 "sv_coyote_time",
		 "0.2",
		 FCVAR_NOTIFY | FCVAR_REPLICATED | FCVAR_ARCHIVE,
		 "Time after leaving a surface that jumps are still allowed." );

 ConVar
	 certain_restrictions(
		 "certain_restrictions",
		 "1",
		 FCVAR_NOTIFY | FCVAR_REPLICATED | FCVAR_ARCHIVE,
		 "Limits the speed you can reach by jumping, sliding, or wallrunning" );

 ConVar
	 sv_slide_lock(
	     "sv_slide_lock", 
		 "0", 
		 FCVAR_NOTIFY | FCVAR_REPLICATED | FCVAR_ARCHIVE,
		 "Locks your move direction when sliding" );

ConVar	sv_stopspeed("sv_stopspeed", "100", FCVAR_NOTIFY | FCVAR_REPLICATED, "Minimum stopping speed when on ground.");

ConVar	sv_noclipaccelerate( "sv_noclipaccelerate", "5", FCVAR_NOTIFY | FCVAR_ARCHIVE | FCVAR_REPLICATED);
ConVar	sv_noclipspeed	( "sv_noclipspeed", "5", FCVAR_ARCHIVE | FCVAR_NOTIFY | FCVAR_REPLICATED);
ConVar	sv_specaccelerate( "sv_specaccelerate", "5", FCVAR_NOTIFY | FCVAR_ARCHIVE | FCVAR_REPLICATED);
ConVar	sv_specspeed	( "sv_specspeed", "3", FCVAR_ARCHIVE | FCVAR_NOTIFY | FCVAR_REPLICATED);
ConVar	sv_specnoclip	( "sv_specnoclip", "1", FCVAR_ARCHIVE | FCVAR_NOTIFY | FCVAR_REPLICATED);

ConVar	sv_maxspeed		( "sv_maxspeed", "999", FCVAR_NOTIFY | FCVAR_REPLICATED);
ConVar	sv_accelerate	( "sv_accelerate", "30", FCVAR_NOTIFY | FCVAR_REPLICATED);

ConVar	sv_airaccelerate("sv_airaccelerate", "150", FCVAR_NOTIFY | FCVAR_REPLICATED);
ConVar	sv_wateraccelerate("sv_wateraccelerate", "30", FCVAR_NOTIFY | FCVAR_REPLICATED);
ConVar	sv_waterfriction("sv_waterfriction", "1", FCVAR_NOTIFY | FCVAR_REPLICATED);
ConVar	sv_footsteps("sv_footsteps", "1", FCVAR_NOTIFY | FCVAR_REPLICATED, "Play footstep sound for players");
ConVar	sv_rollspeed("sv_rollspeed", "200", FCVAR_ARCHIVE | FCVAR_REPLICATED);
ConVar	sv_rollangle("sv_rollangle", "0", FCVAR_ARCHIVE | FCVAR_REPLICATED, "Max view roll angle");

ConVar	sv_friction("sv_friction", "4", FCVAR_NOTIFY | FCVAR_REPLICATED, "World friction.");

ConVar	sv_bounce("sv_bounce", "0", FCVAR_NOTIFY | FCVAR_REPLICATED, "Bounce multiplier for when physically simulated objects collide with other objects.");
ConVar	sv_maxvelocity("sv_maxvelocity", "3500", FCVAR_REPLICATED, "Maximum speed any ballistically moving object is allowed to attain per axis.");
ConVar	sv_stepsize("sv_stepsize", "18", FCVAR_NOTIFY | FCVAR_REPLICATED);
ConVar	sv_backspeed("sv_backspeed", "0.6", FCVAR_ARCHIVE | FCVAR_REPLICATED, "How much to slow down backwards motion");
ConVar  sv_waterdist("sv_waterdist", "12", FCVAR_REPLICATED, "Vertical view fixup when eyes are near water plane.");

ConVar	sv_skyname		( "sv_skyname", "sky_urb01", FCVAR_ARCHIVE | FCVAR_REPLICATED, "Current name of the skybox texture" );

// Vehicle convars
ConVar r_VehicleViewDampen( "r_VehicleViewDampen", "1", FCVAR_CHEAT | FCVAR_NOTIFY | FCVAR_REPLICATED );

// Jeep convars
ConVar r_JeepViewDampenFreq( "r_JeepViewDampenFreq", "7.0", FCVAR_CHEAT | FCVAR_NOTIFY | FCVAR_REPLICATED );
ConVar r_JeepViewDampenDamp( "r_JeepViewDampenDamp", "1.0", FCVAR_CHEAT | FCVAR_NOTIFY | FCVAR_REPLICATED);
ConVar r_JeepViewZHeight( "r_JeepViewZHeight", "10.0", FCVAR_CHEAT | FCVAR_NOTIFY | FCVAR_REPLICATED );

// Airboat convars
ConVar r_AirboatViewDampenFreq( "r_AirboatViewDampenFreq", "7.0", FCVAR_CHEAT | FCVAR_NOTIFY | FCVAR_REPLICATED );
ConVar r_AirboatViewDampenDamp( "r_AirboatViewDampenDamp", "1.0", FCVAR_CHEAT | FCVAR_NOTIFY | FCVAR_REPLICATED);
ConVar r_AirboatViewZHeight( "r_AirboatViewZHeight", "0.0", FCVAR_CHEAT | FCVAR_NOTIFY | FCVAR_REPLICATED );
