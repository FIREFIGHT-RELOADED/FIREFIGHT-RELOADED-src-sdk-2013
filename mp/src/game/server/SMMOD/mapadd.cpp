#include "cbase.h"
#include "mapadd.h"
#include "filesystem.h"
#include "gameweaponmanager.h"
//#include "weapon_custom.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

void CC_CallLabel(const CCommand &args)
{
	CMapAdd *pMapadd = GetMapAddEntity();

	char szMapadd[128];
	Q_snprintf(szMapadd, sizeof(szMapadd), "mapadd/%s.txt", gpGlobals->mapname);
	if (!pMapadd)
	{
		pMapadd = CreateMapAddEntity();
		pMapadd->RunLabel(szMapadd, args[1]);
	}
	else
	{
		pMapadd->RunLabel(szMapadd, args[1]);
	}
}
static ConCommand mapadd_runlabel("mapadd_runlabel", CC_CallLabel, "Run a Mapadd label. Useful for testing.\n");

static CMapAdd *g_MapAddEntity = NULL;
LINK_ENTITY_TO_CLASS( mapadd, CMapAdd  );
CMapAdd* GetMapAddEntity()
{
//	if(!g_MapAddEntity)
//		return CreateMapAddEntity();
	return g_MapAddEntity;
}
CMapAdd* CreateMapAddEntity()
{
	return dynamic_cast<CMapAdd*>((CBaseEntity*)CBaseEntity::Create("mapadd", Vector(0,0,0), QAngle(0,0,0)));
}
void CMapAdd::Precache( void )
{
	g_MapAddEntity = this;
}
bool CMapAdd::RunLabel( const char *mapaddMap, const char *szLabel)
{

	if(AllocPooledString(mapaddMap) == AllocPooledString("") || !mapaddMap || !szLabel || AllocPooledString(szLabel) == AllocPooledString(""))
		return false; //Failed to load!
	//FileHandle_t fh = filesystem->Open(szMapadd,"r","MOD");
	// Open the mapadd data file, and abort if we can't
	KeyValues *pMapAdd = new KeyValues( "MapAdd" );
	if(pMapAdd->LoadFromFile( filesystem, mapaddMap, "MOD" ))
	{
		KeyValues *pMapAdd2 = pMapAdd->FindKey(szLabel);
		if(pMapAdd2)
		{
			KeyValues *pMapAddEnt = pMapAdd2->GetFirstTrueSubKey();
			while (pMapAddEnt)
			{
				if (!HandlePlayerEntity(pMapAddEnt, false) && !HandleRemoveEnitity(pMapAddEnt) && !HandleSMODEntity(pMapAddEnt) && !HandleSpecialEnitity(pMapAddEnt) && !HandleWeaponManagerEnitity(pMapAddEnt))
				{
					Vector SpawnVector = Vector(0,0,0);
					QAngle SpawnAngle = QAngle(0,0,0);
			
					SpawnVector.x = pMapAddEnt->GetFloat("x", SpawnVector.x);
					SpawnVector.y = pMapAddEnt->GetFloat("y", SpawnVector.y);
					SpawnVector.z = pMapAddEnt->GetFloat("z", SpawnVector.z);

					SpawnAngle[PITCH] = pMapAddEnt->GetFloat("pitch", SpawnAngle[PITCH]);
					SpawnAngle[YAW] = pMapAddEnt->GetFloat("yaw", SpawnAngle[YAW]);
					SpawnAngle[ROLL] = pMapAddEnt->GetFloat("roll", SpawnAngle[ROLL]);

					CBaseEntity *createEnt = CBaseEntity::CreateNoSpawn(pMapAddEnt->GetName(),SpawnVector,SpawnAngle);
					KeyValues *pEntKeyValues = pMapAddEnt->FindKey("KeyValues");
					KeyValues *pEntFlags = pMapAddEnt->FindKey("Flags");
					if(createEnt)
					{
						if (pEntKeyValues)
						{
							DevMsg("KeyValue for %s Found!\n", pMapAddEnt->GetName());
							KeyValues *pEntKeyValuesAdd = pEntKeyValues->GetFirstValue();
							while (pEntKeyValuesAdd && createEnt)
							{
								if (AllocPooledString(pEntKeyValuesAdd->GetName()) == AllocPooledString("model"))
								{
									PrecacheModel(pEntKeyValuesAdd->GetString(""));
									createEnt->SetModel(pEntKeyValuesAdd->GetString(""));
								}
								else
								{
									createEnt->KeyValue(pEntKeyValuesAdd->GetName(), pEntKeyValuesAdd->GetString(""));
								}
								pEntKeyValuesAdd = pEntKeyValuesAdd->GetNextValue();
							}
						}

						if (pEntFlags)
						{
							DevMsg("Flag for %s Found!\n", pMapAddEnt->GetName());
							KeyValues *pEntFlagsAdd = pEntFlags->GetFirstValue();
							while (pEntFlagsAdd && createEnt)
							{
								createEnt->AddSpawnFlags(pEntFlagsAdd->GetInt());
								pEntFlagsAdd = pEntFlagsAdd->GetNextValue();
							}
						}
					}
					//createEnt->Activate();//Is this a good idea? Not sure!
					//createEnt->Spawn();
					DispatchSpawn( createEnt ); //I derped
				}
				pMapAddEnt = pMapAddEnt->GetNextTrueSubKey(); //Got to keep this!
			}
		}
	}
	
	pMapAdd->deleteThis();
	return true;
}

bool CMapAdd::HandlePlayerEntity( KeyValues *playerEntityKV, bool initLevel)
{
	if(AllocPooledString(playerEntityKV->GetName()) == AllocPooledString("player"))
	{
	//	if(initLevel)
	//	{
	//		return true; //Just pretend we did
	//	}
		CBasePlayer *playerEnt =UTIL_GetLocalPlayer();
		if(!playerEnt) //He doesn't exist, just pretend
			return true;
		Vector SpawnVector = playerEnt->GetAbsOrigin();
		QAngle SpawnAngle = playerEnt->GetAbsAngles();
			
		SpawnVector.x = playerEntityKV->GetFloat("x", SpawnVector.x);
		SpawnVector.y = playerEntityKV->GetFloat("y", SpawnVector.y);
		SpawnVector.z = playerEntityKV->GetFloat("z", SpawnVector.z);

		SpawnAngle[PITCH] = playerEntityKV->GetFloat("pitch", SpawnAngle[PITCH]);
		SpawnAngle[YAW] = playerEntityKV->GetFloat("yaw", SpawnAngle[YAW]);
		SpawnAngle[ROLL] = playerEntityKV->GetFloat("roll", SpawnAngle[ROLL]);
//		KeyValues *pEntKeyValues = playerEntityKV->FindKey("KeyValues");
		/*if(pEntKeyValues && playerEnt)
		{
			KeyValues *pEntKeyValuesAdd = pEntKeyValues->GetFirstValue();
			while(pEntKeyValuesAdd && playerEnt)
			{
				if(AllocPooledString(pEntKeyValuesAdd->GetName()) == AllocPooledString("model"))
				{
					char szModel[128];
					Q_snprintf( szModel, sizeof( szModel ), "%s", playerEnt->GetModelName() );
					PrecacheModel(pEntKeyValuesAdd->GetString(szModel));
					playerEnt->SetModel(pEntKeyValuesAdd->GetString(szModel) );
				}
				pEntKeyValuesAdd = pEntKeyValuesAdd->GetNextValue();
			}
		}*/
		playerEnt->SetAbsOrigin(SpawnVector);
		playerEnt->SetAbsAngles(SpawnAngle);
		return true;
	}
	return false;
}
bool CMapAdd::HandleSMODEntity( KeyValues *smodEntity)
{
	return false;
}
bool CMapAdd::HandleSpecialEnitity( KeyValues *specialEntity)
{
	return false;
}
bool CMapAdd::HandleRemoveEnitity( KeyValues *mapaddValue)
{
	if(AllocPooledString(mapaddValue->GetName()) == AllocPooledString("remove:sphere"))
	{
		Vector RemoveVector = Vector(0,0,0);
		CBaseEntity *ppEnts[256];
//		CBaseEntity *ppCandidates[256];
		RemoveVector.x = mapaddValue->GetFloat("x", RemoveVector.x);
		RemoveVector.y = mapaddValue->GetFloat("y", RemoveVector.y);
		RemoveVector.z = mapaddValue->GetFloat("z", RemoveVector.z);
		int nEntCount = UTIL_EntitiesInSphere( ppEnts, 256, RemoveVector, mapaddValue->GetFloat("radius", 0), 0 );

				//Look through the entities it found
			KeyValues *pEntKeyValues = mapaddValue->FindKey("entities");
			if(pEntKeyValues)
			{
				KeyValues *pEntKeyValuesRemove = pEntKeyValues->GetFirstValue();
				while(pEntKeyValuesRemove)
				{
					int i;
					for ( i = 0; i < nEntCount; i++ )
					{
			
						if ( ppEnts[i] == NULL )
							continue;
						if(AllocPooledString(pEntKeyValuesRemove->GetName()) == AllocPooledString("classname")) // || ( AllocPooledString(pEntKeyValuesRemove->GetName()) == ppEnts[i]->GetEntityName())
						{
							if(AllocPooledString(pEntKeyValuesRemove->GetString()) == AllocPooledString(ppEnts[i]->GetClassname()))
							{
								UTIL_Remove(ppEnts[i]);
								continue;
							}
						}
						if(AllocPooledString(pEntKeyValuesRemove->GetName()) == AllocPooledString("targetname")) // || ( AllocPooledString(pEntKeyValuesRemove->GetName()) == ppEnts[i]->GetEntityName())
						{
							if(AllocPooledString(pEntKeyValuesRemove->GetString()) == ppEnts[i]->GetEntityName())
							{
								UTIL_Remove(ppEnts[i]);
								continue;
							}
						}
					}
					pEntKeyValuesRemove = pEntKeyValuesRemove->GetNextValue();
				}
			}
			return true;
	}
	return false;
}
bool CMapAdd::HandleWeaponManagerEnitity(KeyValues *weaponManagerEntity)
{
	if (AllocPooledString(weaponManagerEntity->GetName()) == AllocPooledString("gameweaponmanager"))
	{
		const char *pWeaponName = "";
		int iMaxAllowed = 0;
		pWeaponName = weaponManagerEntity->GetString("weaponname", pWeaponName);
		iMaxAllowed = weaponManagerEntity->GetFloat("maxallowed", iMaxAllowed);
		CreateWeaponManager(pWeaponName, iMaxAllowed);
		return true;
	}
	return false;
}
void CMapAdd::InputRunLabel( inputdata_t &inputData ) //Input this directly!
{
	char szMapadd[128];
	Q_snprintf( szMapadd, sizeof( szMapadd ), "mapadd/%s.txt", gpGlobals->mapname );
	this->RunLabel( szMapadd, inputData.value.String());
}
BEGIN_DATADESC( CMapAdd )
	// Links our input name from Hammer to our input member function
	DEFINE_INPUTFUNC( FIELD_STRING, "RunLabel", InputRunLabel ),
END_DATADESC()

//-----------------------------------------------------------------------------
// Purpose: Handle a tick input from another entity
//-----------------------------------------------------------------------------

class CMapAddLabel : public CBaseEntity
{
	public:
	DECLARE_CLASS( CMapAddLabel, CBaseEntity );
	DECLARE_DATADESC();
 
	// Constructor
	CMapAddLabel()
	{
		m_nTriggerArea = 32.0f;
		m_bDeleteOnFire = true;
	}
	void Spawn()
	{
		BaseClass::Spawn();
		SetNextThink( gpGlobals->curtime ); // Think now
	}
	// Input function
	//void InputRunLabel( inputdata_t &inputData ); //Runmapadd
	void Think();
	private:
	float	m_nTriggerArea;	// Count at which to fire our output
//	int	m_nCounter;	// Internal counter
	bool	m_bDeleteOnFire;
	COutputEvent	m_OnTrigger;	// Output event when the counter reaches the threshold
};

 
void CMapAddLabel::Think()
{
	BaseClass::Think(); // Always do this if you override Think()
 
	

	//What this does, is look through entities in a sphere, and then
	//checks to see if they are valid, and if they are
	//adds them to a second list of valid entities.
	//Create an array of CBaseEntity pointers
	CBaseEntity *ppEnts[256];
	int nEntCount = UTIL_EntitiesInSphere( ppEnts, 256, this->GetAbsOrigin(), m_nTriggerArea, 0 );
	int i;
	for ( i = 0; i < nEntCount; i++ )
	{
			//Look through the entities it found
		if ( ppEnts[i] != NULL )
			if(ppEnts[i]->IsPlayer())
			{
				Msg( "mapadd_trig was triggered!\n" );
				m_OnTrigger.FireOutput(ppEnts[i],this);
			//	if(m_bDeleteOnFire)
			//	{
				//	SetNextThink( gpGlobals->curtime + 1 ); // Think again in 1 second
					UTIL_Remove( this );
					break;
			//	}
			//	SetNextThink( gpGlobals->curtime + 1 ); // Think again in 1 second
			}
	}
	SetNextThink( gpGlobals->curtime + 1 ); // Think again in 1 second
}

LINK_ENTITY_TO_CLASS( mapadd_trig, CMapAddLabel  );
 
// Start of our data description for the class
BEGIN_DATADESC( CMapAddLabel  )
 
// Links our member variable to our keyvalue from Hammer
DEFINE_KEYFIELD( m_nTriggerArea, FIELD_FLOAT, "radius" ),
// Links our member variable to our keyvalue from Hammer
DEFINE_KEYFIELD( m_bDeleteOnFire, FIELD_BOOLEAN, "deleteonfire" ),
// Links our input name from Hammer to our input member function
//DEFINE_INPUTFUNC( FIELD_VOID, "RunLabel", InputRunLabel ),
 
// Links our output member to the output name used by Hammer
DEFINE_OUTPUT( m_OnTrigger, "OnTrigger" ),
 
DEFINE_THINKFUNC( Think ), // Register new think function

END_DATADESC()
 