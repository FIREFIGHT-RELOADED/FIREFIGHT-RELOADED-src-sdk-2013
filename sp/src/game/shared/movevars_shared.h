//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#ifndef MOVEVARS_SHARED_H
#define MOVEVARS_SHARED_H
#ifdef _WIN32
#pragma once
#endif

#include "convar.h"

float GetCurrentGravity( void );
float GetJumpHeight( void );
float GetSlideTime( void );
float GetSlideSpeedBoost( void );
float GetWallRunTime( void );
float GetWallRunBoost( void );
float GetWallRunSpeed( void );

extern ConVar sv_wallrun_anticipation;
extern ConVar sv_wallrun_time;
extern ConVar sv_wallrun_speed;
extern ConVar sv_wallrun_accel;
extern ConVar sv_wallrun_boost;
extern ConVar sv_wallrun_jump_boost;
extern ConVar sv_wallrun_jump_push;
extern ConVar sv_wallrun_feet_z;
extern ConVar sv_wallrun_roll;
extern ConVar sv_wallrun_min_rise;
extern ConVar sv_wallrun_max_rise;
extern ConVar sv_wallrun_scramble_z;
extern ConVar sv_wallrun_lookahead;
extern ConVar sv_wallrun_inness;
extern ConVar sv_wallrun_outness;
extern ConVar sv_wallrun_look_delay;
extern ConVar sv_wallrun_lookness;
extern ConVar sv_wallrun_stick_angle;
extern ConVar sv_wallrun_corner_stick_angle;
extern ConVar sv_duck_time;
extern ConVar sv_slide_speed_boost;
extern ConVar sv_coyote_time;
extern ConVar sv_jump_height;
extern ConVar sv_slide_time;
extern ConVar sv_slide_lock;
extern ConVar certain_restrictions;
extern ConVar sv_wallrun_requiredirectcontrol;

extern ConVar sv_slide;
extern ConVar sv_wallrun;

extern ConVar sv_gravity;
extern ConVar sv_stopspeed;
extern ConVar sv_noclipaccelerate;
extern ConVar sv_noclipspeed;
extern ConVar sv_maxspeed;
extern ConVar sv_accelerate;
extern ConVar sv_airaccelerate;
extern ConVar sv_wateraccelerate;
extern ConVar sv_waterfriction;
extern ConVar sv_footsteps;
extern ConVar sv_rollspeed;
extern ConVar sv_rollangle;
extern ConVar sv_friction;
extern ConVar sv_bounce;
extern ConVar sv_maxvelocity;
extern ConVar sv_stepsize;
extern ConVar sv_skyname;
extern ConVar sv_backspeed;
extern ConVar sv_waterdist;
extern ConVar sv_specaccelerate;
extern ConVar sv_specspeed;
extern ConVar sv_specnoclip;

// Vehicle convars
extern ConVar r_VehicleViewDampen;

// Jeep convars
extern ConVar r_JeepViewDampenFreq;
extern ConVar r_JeepViewDampenDamp;
extern ConVar r_JeepViewZHeight;

// Airboat convars
extern ConVar r_AirboatViewDampenFreq;
extern ConVar r_AirboatViewDampenDamp;
extern ConVar r_AirboatViewZHeight;

#endif // MOVEVARS_SHARED_H
