//==== Woots 2016. http://creativecommons.org/licenses/by/2.5/mx/ ===========//

#include "cbase.h"
#include "nodes_generation.h"

#include "player.h"

#include "nav.h"
#include "nav_mesh.h"
#include "nav_area.h"
#include "nav_ladder.h"
#include "nav_node.h"

#include "ai_initutils.h"
#include "ai_node.h"
#include "ai_network.h"
#include "ai_networkmanager.h"

#include "Sprite.h"

#include "editor_sendcommand.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

CNodesGeneration g_NodesGeneration;
CNodesGeneration *TheNodeGenerator = &g_NodesGeneration;

static bool bInNodePlacement = false;
static CUtlVector<CBaseEntity*> pNodes;

//================================================================================
// Comandos
//================================================================================

ConVar ai_generate_nodes_underwater("ai_generate_nodes_underwater", "0");
ConVar ai_generate_nodes_walkable("ai_generate_nodes_walkable", "1");
ConVar ai_generate_nodes_walkable_distance(" ai_generate_nodes_walkable_distance", "300");

CBaseEntity *CreateNode(Vector origin)
{
	CBaseEntity *pEnt = CSprite::SpriteCreate("sprites/glow01.vmt", origin, false);
	pNodes.AddToTail(pEnt);
	return pEnt;
}

//================================================================================
//================================================================================
void CNodesGeneration::Start()
{
	m_iNavAreaCount = TheNavMesh->GetNavAreaCount();

    if ( m_iNavAreaCount == 0 ) 
	{
		const char *classname = "";
		CBaseEntity *pSpotFinder;
		pSpotFinder = NULL;

		pSpotFinder = gEntList.FindEntityByClassname(NULL, "info_player_terrorist");

		if (pSpotFinder)
		{
			classname = pSpotFinder->GetClassname();
		}
		else
		{
			pSpotFinder = gEntList.FindEntityByClassname(NULL, "info_player_axis");

			if (pSpotFinder)
			{
				classname = pSpotFinder->GetClassname();
			}
			else
			{
				pSpotFinder = gEntList.FindEntityByClassname(NULL, "info_player_deathmatch");

				if (pSpotFinder)
				{
					classname = pSpotFinder->GetClassname();
				}
				else
				{
					pSpotFinder = gEntList.FindEntityByClassname(NULL, "info_player_start");

					if (pSpotFinder)
					{
						classname = pSpotFinder->GetClassname();
					}
					else
					{
						Warning("No player spawn on level. Navigation mesh generation will not work!\n");
						return;
					}
				}
			}
		}

		Warning("No Navigation Mesh found. Generating...\n");

		TheNavMesh->SetPlayerSpawnName(classname);
		TheNavMesh->BeginGeneration();
        return;
    }

    m_iWalkableNodesCount = 0;
    m_WalkLocations.Purge();

    Msg( "Examining %i navigation areas...\n", m_iNavAreaCount );

    GenerateWalkableNodes();

    Msg( "Process finished. %i nodes have been generated.\n\n", g_pAINetworkManager->GetNetwork()->NumNodes() );
}

//================================================================================
//================================================================================
void CNodesGeneration::GenerateWalkableNodes()
{
    if (bInNodePlacement)
		return;
	
	if ( !ai_generate_nodes_walkable.GetBool() )
        return;

    Msg( "Generating walkable nodes...\n" );

    int nodesGenerated = m_iWalkableNodesCount;

    FOR_EACH_VEC( TheNavAreas, it )
    {
        CNavArea *pArea = TheNavAreas[it];

        if ( !pArea )
            continue;

        if ( pArea->IsUnderwater() && !ai_generate_nodes_underwater.GetBool() )
            continue;

        if ( pArea->IsBlocked( TEAM_ANY ) || pArea->HasAvoidanceObstacle() )
            continue;

        for ( int e = 0; e <= MAX_NODES_PER_AREA; ++e ) {
            // Obtenemos una posición al azar
            Vector vecPosition = pArea->GetRandomPoint();
            vecPosition.z += 10;

            bool tooClose = false;

            // Ya hemos generado un nodo aquí
            if ( m_WalkLocations.HasElement( vecPosition ) )
                continue;

            // Revisamos todos los nodos que hemos generado
            FOR_EACH_VEC( m_WalkLocations, it )
            {
                Vector vecTmp = m_WalkLocations[it];

                if ( !vecTmp.IsValid() )
                    continue;

                // Esta muy cerca de otro nodo
                if ( vecTmp.DistTo( vecPosition ) <= ai_generate_nodes_walkable_distance.GetFloat() ) {
                    if ( vecPosition.z == vecTmp.z || (vecPosition.z < (vecTmp.z + 15.0f) && vecPosition.z >( vecTmp.z - 15.0f )) ) {
                        tooClose = true;
                        break;
                    }
                }
            }

            // Esta muy cerca de otro nodo
            if ( tooClose )
                continue;

			Vector vecOrigin = Vector(vecPosition.x, vecPosition.y, vecPosition.z);
			CreateNode(vecOrigin);

			m_WalkLocations.AddToTail(vecPosition);
			++m_iWalkableNodesCount;
        }

        int max = MAX_NODES;

        // No se han generado todos los nodos que tenemos disponibles, para generar
        // un movimiento más fluido repetimos el proceso hasta completar o que no haya
        // lugares donde crear más
        if ( m_iWalkableNodesCount < max && nodesGenerated < m_iWalkableNodesCount ) {
            Msg( "%i / %i...\n", m_iWalkableNodesCount, max );
            GenerateWalkableNodes();
            return;
        }
    }

    Msg( "%i walkable nodes have been created...\n\n", m_iWalkableNodesCount );
}

void C_GenerateNodes(void)
{
	// if the text file already exists, load its current nodes
	char szNodeTextFilename[MAX_PATH];
	Q_snprintf(szNodeTextFilename, sizeof(szNodeTextFilename), "maps/graphs/%s%s.txt", STRING(gpGlobals->mapname), GetPlatformExt());
	CUtlBuffer buf(0, 0, CUtlBuffer::TEXT_BUFFER);
	if (filesystem->ReadFile(szNodeTextFilename, "game", buf))
	{
		if (!buf.Size())
			return;

		const int maxLen = 64;
		char line[maxLen];
		CUtlVector<char*> floats;
		int num = 0;

		// loop through every line of the file, read it in
		while (true)
		{
			buf.GetLine(line, maxLen);
			if (Q_strlen(line) <= 0)
				break; // reached the end of the file

			// we've read in a string containing 3 tab separated floats
			// we need to split this into 3 floats, which we put in a vector
			V_SplitString(line, "	", floats);
			Vector origin(atof(floats[0]), atof(floats[1]), atof(floats[2]));

			floats.PurgeAndDeleteElements();
			
			CreateNode(origin);
			num++;

			if (!buf.IsValid())
				break;
		}
	}

	bInNodePlacement = true;
	Msg("Generating Nodegraph...\n");
	TheNodeGenerator->Start();
	Msg("Saving Nodegraph...\n");
	// save the nodes
	CUtlBuffer buf2(0, 0, CUtlBuffer::TEXT_BUFFER);
	for (int i = 0; i<pNodes.Size(); i++)
	{
		buf2.PutString(UTIL_VarArgs("%f	%f	%f\n", pNodes[i]->GetAbsOrigin().x,
			pNodes[i]->GetAbsOrigin().y,
			pNodes[i]->GetAbsOrigin().z));
	}
	filesystem->WriteFile(szNodeTextFilename, "game", buf2);

	// clean up & exit node mode
	pNodes.PurgeAndDeleteElements();
	bInNodePlacement = false;
	Msg("Saved nodes. Reload the map to build the AI nodegraph.\n");
}
static ConCommand ai_generate_nodes("ai_generate_nodes", C_GenerateNodes, "Generates movement nodes for the AI navigation network.");