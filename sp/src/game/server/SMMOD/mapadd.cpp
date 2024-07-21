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
	Q_snprintf(szMapadd, sizeof(szMapadd), "mapadd/%s.txt", STRING(gpGlobals->mapname));
	if (!pMapadd)
	{
		pMapadd = CreateMapAddEntity();
		pMapadd->RunLabel(args[1]);
	}
	else
	{
		pMapadd->RunLabel(args[1]);
	}
}
static ConCommand mapadd_runlabel("mapadd_runlabel", CC_CallLabel, "Run a Mapadd label. Useful for testing.\n");

static CMapAdd *g_MapAddEntity = NULL;

LINK_ENTITY_TO_CLASS( mapadd, CMapAdd  );

BEGIN_DATADESC(CMapAdd)
// Links our input name from Hammer to our input member function
DEFINE_INPUTFUNC(FIELD_STRING, "RunLabel", InputRunLabel),
END_DATADESC()

CMapAdd* GetMapAddEntity()
{
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

bool CMapAdd::RunLabel( const char *szLabel)
{
	if(!szLabel || AllocPooledString(szLabel) == AllocPooledString(""))
		return false; //Failed to load!

	CMapInfo* info = new CMapInfo();

	if (info)
	{
		KeyValues* pMapAdd = info->data->FindKey("MapAdd");

		if (pMapAdd)
		{
			KeyValues* pMapAdd2 = pMapAdd->FindKey(szLabel);
			if (pMapAdd2)
			{
				KeyValues* pMapAddEnt = pMapAdd2->GetFirstTrueSubKey();
				while (pMapAddEnt)
				{
					if (!HandlePlayerEntity(pMapAddEnt, false) && !HandleRemoveEnitity(pMapAddEnt) && !HandleSMODEntity(pMapAddEnt) && !HandleSpecialEnitity(pMapAddEnt))
					{
						Vector SpawnVector = Vector(0, 0, 0);
						QAngle SpawnAngle = QAngle(0, 0, 0);

						SpawnVector.x = pMapAddEnt->GetFloat("x", SpawnVector.x);
						SpawnVector.y = pMapAddEnt->GetFloat("y", SpawnVector.y);
						SpawnVector.z = pMapAddEnt->GetFloat("z", SpawnVector.z);

						SpawnAngle[PITCH] = pMapAddEnt->GetFloat("pitch", SpawnAngle[PITCH]);
						SpawnAngle[YAW] = pMapAddEnt->GetFloat("yaw", SpawnAngle[YAW]);
						SpawnAngle[ROLL] = pMapAddEnt->GetFloat("roll", SpawnAngle[ROLL]);

						CBaseEntity* createEnt = CBaseEntity::CreateNoSpawn(pMapAddEnt->GetName(), SpawnVector, SpawnAngle);
						KeyValues* pEntKeyValues = pMapAddEnt->FindKey("KeyValues");
						KeyValues* pEntFlags = pMapAddEnt->FindKey("Flags");
						if (createEnt)
						{
							if (pEntKeyValues)
							{
								DevMsg("KeyValue for %s Found!\n", pMapAddEnt->GetName());
								KeyValues* pEntKeyValuesAdd = pEntKeyValues->GetFirstValue();
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
								KeyValues* pEntFlagsAdd = pEntFlags->GetFirstValue();
								while (pEntFlagsAdd && createEnt)
								{
									createEnt->AddSpawnFlags(pEntFlagsAdd->GetInt());
									pEntFlagsAdd = pEntFlagsAdd->GetNextValue();
								}
							}
						}
						//createEnt->Activate();//Is this a good idea? Not sure!
						//createEnt->Spawn();
						DispatchSpawn(createEnt); //I derped
					}
					pMapAddEnt = pMapAddEnt->GetNextTrueSubKey(); //Got to keep this!
				}
			}
		}

		//info->Die();
		return true;
	}
	return false;
}

bool CMapAdd::HandlePlayerEntity( KeyValues *playerEntityKV, bool initLevel)
{
	if(AllocPooledString(playerEntityKV->GetName()) == AllocPooledString("player"))
	{
		CBasePlayer *playerEnt = UTIL_GetLocalPlayer();
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
	//weaponmanagers are moved to HandleSpecialEntity
	if (AllocPooledString(specialEntity->GetName()) == AllocPooledString("gameweaponmanager"))
	{
		const char* pWeaponName = "";
		int iMaxAllowed = 0;
		pWeaponName = specialEntity->GetString("weaponname", pWeaponName);
		iMaxAllowed = specialEntity->GetFloat("maxallowed", iMaxAllowed);
		CreateWeaponManager(pWeaponName, iMaxAllowed);
		return true;
	}
	return false;
}

bool CMapAdd::HandleRemoveEnitity( KeyValues *mapaddValue)
{
	static const auto CLASSNAME = AllocPooledString( "classname" );
	static const auto TARGETNAME = AllocPooledString( "targetname" );
	static const auto REMOVEALL = AllocPooledString( "remove:all" );
	static const auto REMOVESPHERE = AllocPooledString( "remove:sphere" );

	if (AllocPooledString(mapaddValue->GetName()) == REMOVESPHERE)
	{
		auto pEntKeyValues = mapaddValue->FindKey( "entities" );
		if ( pEntKeyValues == NULL )
			return true;

		Vector RemoveVector = Vector(0,0,0);
		CBaseEntity *ppEnts[256];
		RemoveVector.x = mapaddValue->GetFloat("x", RemoveVector.x);
		RemoveVector.y = mapaddValue->GetFloat("y", RemoveVector.y);
		RemoveVector.z = mapaddValue->GetFloat("z", RemoveVector.z);
		int nEntCount = UTIL_EntitiesInSphere( ppEnts, 256, RemoveVector, mapaddValue->GetFloat("radius", 0), 0 );

		//Look through the entities it found
		auto pEntKeyValuesRemove = pEntKeyValues->GetFirstValue();
		while (pEntKeyValuesRemove != NULL)
		{
			const auto name = AllocPooledString(pEntKeyValuesRemove->GetName());
			const auto str = AllocPooledString(pEntKeyValuesRemove->GetString());
			for ( int i = 0; i < nEntCount; ++i )
			{
				if ( ppEnts[i] == NULL )
					continue;
				if (name == CLASSNAME && str == AllocPooledString(ppEnts[i]->GetClassname()))
				{
					UTIL_Remove(ppEnts[i]);
					continue;
				}
				else if (name == TARGETNAME	&& str == ppEnts[i]->GetEntityName())
				{
					UTIL_Remove(ppEnts[i]);
					continue;
				}
			}
			pEntKeyValuesRemove = pEntKeyValuesRemove->GetNextValue();
		}
		return true;
	}
	else if ( AllocPooledString( mapaddValue->GetName() ) == REMOVEALL )
	{
		auto pEntKeyValues = mapaddValue->FindKey( "entities" );
		if ( pEntKeyValues == NULL )
			return true;

		auto pEntKeyValuesRemove = pEntKeyValues->GetFirstValue();
		while ( pEntKeyValuesRemove != NULL )
		{
			const auto name = AllocPooledString( pEntKeyValuesRemove->GetName() );
			const auto str = AllocPooledString( pEntKeyValuesRemove->GetString() );
			if ( name == CLASSNAME )
			{
				CBaseEntity* pent = NULL;
				while ( (pent = gEntList.FindEntityByClassname( pent, STRING(str) )) != 0 )
					UTIL_Remove( pent );
			}
			else if ( name == TARGETNAME )
			{
				CBaseEntity* pent = NULL;
				while ( (pent = gEntList.FindEntityByName( pent, STRING(str) )) != 0 )
					UTIL_Remove( pent );
			}
			pEntKeyValuesRemove = pEntKeyValuesRemove->GetNextValue();
		}
		return true;
	}
	else
		return false;
}

void CMapAdd::InputRunLabel( inputdata_t &inputData ) //Input this directly!
{
	char szMapadd[128];
	Q_snprintf( szMapadd, sizeof( szMapadd ), "mapadd/%s.txt", STRING(gpGlobals->mapname) );
	this->RunLabel( inputData.value.String());
}
 