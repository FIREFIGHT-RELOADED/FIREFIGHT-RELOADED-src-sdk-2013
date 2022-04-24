#include "cbase.h"
#include "const.h"
#include "utlbuffer.h"
#include "filesystem.h"
#include "Sprite.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

static bool bInNodePlacement = false;
static CUtlVector<CBaseEntity*> pNodes;

CBaseEntity* CreateNode(Vector origin)
{
	CBaseEntity::PrecacheModel("sprites/glow01.vmt");
	CSprite* pEnt = CSprite::SpriteCreate("sprites/glow01.vmt", origin, false);
	pEnt->SetTransparency(kRenderGlow, 255, 255, 255, 255, kRenderFxNoDissipation);
	pEnt->SetScale(0.35, 0.0);
	pNodes.AddToTail(pEnt);
	return pEnt;
}

void CC_StartNodes(void)
{
	if (bInNodePlacement)
	{
		Warning("Node placement mode is already on! Type ai_nodes_save to turn it off!\n");
		return;
	}

	// if the text file already exists, load its current nodes
	char szNodeTextFilename[MAX_PATH];
	Q_snprintf(szNodeTextFilename, sizeof(szNodeTextFilename), "maps/graphs/%s%s.txt",
		STRING(gpGlobals->mapname), GetPlatformExt());
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
	Msg("===============================================================================\n");
	Msg("Entered node placement mode. Use the 'ai_nodes_add' and 'ai_nodes_undo' commands to place/remove nodes at your feet.\n");
	Msg("Use the 'ai_nodes_save' command to finish.\n");
	Msg("===============================================================================\n");
}
static ConCommand cc_startnodes("ai_nodes_start", CC_StartNodes, "Start manually placing nodegraph elements, with 'ai_nodes_add' and 'ai_nodes_undo' commands. Finish with 'ai_nodes_save'.");


void CC_SaveNodes(void)
{
	if (!bInNodePlacement)
	{
		Warning("Node placement mode isn't on! Type ai_nodes_add to turn it off!\n");
		return;
	}

	// save the nodes
	char szNodeTextFilename[MAX_PATH];
	Q_snprintf(szNodeTextFilename, sizeof(szNodeTextFilename),
		"maps/graphs/%s%s.txt", STRING(gpGlobals->mapname), GetPlatformExt());

	CUtlBuffer buf(0, 0, CUtlBuffer::TEXT_BUFFER);
	for (int i = 0; i < pNodes.Size(); i++)
	{
		buf.PutString(UTIL_VarArgs("%f	%f	%f\n", pNodes[i]->GetAbsOrigin().x,
			pNodes[i]->GetAbsOrigin().y,
			pNodes[i]->GetAbsOrigin().z));
	}
	filesystem->WriteFile(szNodeTextFilename, "game", buf);

	// clean up & exit node mode
	pNodes.PurgeAndDeleteElements();
	bInNodePlacement = false;
	Msg("===============================================================================\n");
	Msg("Saved nodes & exited node placement mode. Reload map to build the AI nodegraph.\n");
	Msg("===============================================================================\n");
}
static ConCommand cc_savenodes("ai_nodes_save", CC_SaveNodes, "Finish manually placing nodegraph elements, and save the .txt. First, use 'ai_nodes_start'.");


void CC_AddNode(void)
{
	if (!bInNodePlacement)
	{
		Warning("Node placement mode isn't on! Type ai_nodes_add to turn it off!\n");
		return;
	}

	CBasePlayer* pPlayer = UTIL_PlayerByIndex(UTIL_GetCommandClientIndex());
	Vector vecOrigin = pPlayer->GetAbsOrigin() + Vector(0, 0, 10);
	CreateNode(vecOrigin);
	Msg("Added node at: X: %f, Y: %f, Z: %f\n", vecOrigin.x, vecOrigin.y, vecOrigin.z);
}
static ConCommand cc_addnode("ai_nodes_add", CC_AddNode, "Manually place a nodegraph element at your feet. First, use 'ai_nodes_start'.");


void CC_UndoNode(void)
{
	if (!bInNodePlacement)
	{
		Warning("Node placement mode isn't on! Type ai_nodes_add to turn it off!\n");
		return;
	}

	if (pNodes.Size() > 0)
	{
		UTIL_Remove(pNodes.Tail());
		pNodes.Remove(pNodes.Size() - 1);

		Msg("Last-placed node removed\n");
	}
	else
		Msg("Node list is empty\n");
}
static ConCommand cc_delnode("ai_nodes_undo", CC_UndoNode, "Delete the last placed node. First, use 'ai_nodes_start'.");