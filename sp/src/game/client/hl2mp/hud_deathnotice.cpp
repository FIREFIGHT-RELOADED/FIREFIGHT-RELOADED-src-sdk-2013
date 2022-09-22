//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: Draws CSPort's death notices
//
// $NoKeywords: $
//=============================================================================//
#include "cbase.h"
#include "hudelement.h"
#include "hud_macros.h"
#include "c_playerresource.h"
#include <vgui_controls/Controls.h>
#include <vgui_controls/Panel.h>
#include <vgui/ISurface.h>
#include <vgui/ILocalize.h>
#include <KeyValues.h>
#include "c_baseplayer.h"
#include "c_team.h"
#include "game_controls/baseviewport.h"
#include "clientmode_shared.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

static ConVar hud_deathnotice_time("hud_deathnotice_time", "10", 0);

// Player entries in a death notice
struct DeathNoticePlayer
{
	char		szName[MAX_PLAYER_NAME_LENGTH];
	int			iEntIndex;
};

// Contents of each entry in our list of death notices
struct DeathNoticeItem
{
	DeathNoticePlayer	Killer;
	DeathNoticePlayer   Victim;
	CHudTexture* iconDeath;
	int			iSuicide;
	float		flDisplayTime;
	bool		bHeadshot;
};

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class CHudDeathNotice : public CHudElement, public vgui::Panel
{
	DECLARE_CLASS_SIMPLE(CHudDeathNotice, vgui::Panel);
public:
	CHudDeathNotice(const char* pElementName);

	void Init(void);
	void VidInit(void);
	virtual bool ShouldDraw(void);
	virtual void Paint(void);
	virtual void ApplySchemeSettings(vgui::IScheme* scheme);

	void SetColorForNoticePlayer(int iTeamNumber);
	void RetireExpiredDeathNotices(void);

	virtual void FireGameEvent(IGameEvent* event);

private:

	CPanelAnimationVarAliasType(float, m_flLineHeight, "LineHeight", "15", "proportional_float");

	CPanelAnimationVar(float, m_flMaxDeathNotices, "MaxDeathNotices", "4");

	CPanelAnimationVar(bool, m_bRightJustify, "RightJustify", "1");

	CPanelAnimationVar(vgui::HFont, m_hTextFont, "TextFont", "HudNumbersTimer");

	CPanelAnimationVar(Color, m_cIconColor, "IconColor", "255 80 0 255");
	CPanelAnimationVar(float, m_flIconSize, "IconSize", "2");

	CPanelAnimationVar(float, m_flIconOffsetX, "IconOffsetX", "0");
	CPanelAnimationVar(float, m_flIconOffsetY, "IconOffsetY", "0");

	CPanelAnimationVar(float, m_flIconOffsetWepX, "IconOffsetWepX", "0");
	CPanelAnimationVar(float, m_flIconOffsetWepY, "IconOffsetWepY", "0");
	CPanelAnimationVar(float, m_flIconOffsetAmmoX, "IconOffsetAmmoX", "0");
	CPanelAnimationVar(float, m_flIconOffsetAmmoY, "IconOffsetAmmoY", "0");

	CHudTexture* m_icon;

	CUtlVector<DeathNoticeItem> m_DeathNotices;
};

using namespace vgui;

DECLARE_HUDELEMENT(CHudDeathNotice);

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CHudDeathNotice::CHudDeathNotice(const char* pElementName) :
	CHudElement(pElementName), BaseClass(NULL, "HudDeathNotice")
{
	vgui::Panel* pParent = g_pClientMode->GetViewport();
	SetParent(pParent);

	m_icon = NULL;

	SetHiddenBits(HIDEHUD_MISCSTATUS);
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHudDeathNotice::ApplySchemeSettings(IScheme* scheme)
{
	BaseClass::ApplySchemeSettings(scheme);
	SetPaintBackgroundEnabled(false);
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHudDeathNotice::Init(void)
{
	ListenForGameEvent("player_death");
	ListenForGameEvent("npc_death");
	ListenForGameEvent("player_death_npc");
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHudDeathNotice::VidInit(void)
{
	m_icon = gHUD.GetIcon("deathnotice");
	m_DeathNotices.Purge();
}

//-----------------------------------------------------------------------------
// Purpose: Draw if we've got at least one death notice in the queue
//-----------------------------------------------------------------------------
bool CHudDeathNotice::ShouldDraw(void)
{
	return (CHudElement::ShouldDraw() && (m_DeathNotices.Count()));
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHudDeathNotice::SetColorForNoticePlayer(int iTeamNumber)
{
	surface()->DrawSetTextColor(GameResources()->GetTeamColor(iTeamNumber));
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHudDeathNotice::Paint()
{
	if (!m_icon)
		return;

	CBaseViewport* pViewport = dynamic_cast<CBaseViewport*>(GetClientModeNormal()->GetViewport());
	int yStart = pViewport->GetDeathMessageStartHeight();

	surface()->DrawSetTextFont(m_hTextFont);
	surface()->DrawSetTextColor(GameResources()->GetTeamColor(0));

	int iCount = m_DeathNotices.Count();
	for (int i = 0; i < iCount; i++)
	{
		CHudTexture* icon = m_DeathNotices[i].iconDeath;
		if (!icon)
			continue;

		wchar_t victim[256];
		wchar_t killer[256];

		// Get the team numbers for the players involved
		int iKillerTeam = 0;
		int iVictimTeam = 0;

		if (g_PR)
		{
			iKillerTeam = g_PR->GetTeam(m_DeathNotices[i].Killer.iEntIndex);
			iVictimTeam = g_PR->GetTeam(m_DeathNotices[i].Victim.iEntIndex);
		}

		if (m_DeathNotices[i].Victim.szName[0] == '#')
		{
			wchar_t* tempString = g_pVGuiLocalize->Find(m_DeathNotices[i].Victim.szName);

			if (tempString)
			{
				g_pVGuiLocalize->ConstructString(victim, sizeof(victim), tempString, 0);
			}
			else
			{
				const char* prunedName = m_DeathNotices[i].Victim.szName;
				if (strncmp(prunedName, "#fr_npc_", 8) == 0)
				{
					prunedName += 8;
				}

				g_pVGuiLocalize->ConvertANSIToUnicode(m_DeathNotices[i].Victim.szName, victim, sizeof(victim));
			}
		}
		else
		{
			const char* prunedName = m_DeathNotices[i].Victim.szName;
			if (strncmp(prunedName, "#fr_npc_", 8) == 0)
			{
				prunedName += 8;
			}

			g_pVGuiLocalize->ConvertANSIToUnicode(m_DeathNotices[i].Victim.szName, victim, sizeof(victim));
		}

		if (m_DeathNotices[i].Killer.szName[0] == '#')
		{
			wchar_t* tempString = g_pVGuiLocalize->Find(m_DeathNotices[i].Killer.szName);

			if (tempString)
			{
				g_pVGuiLocalize->ConstructString(killer, sizeof(killer), tempString, 0);
			}
			else
			{
				const char* prunedName = m_DeathNotices[i].Killer.szName;
				if (strncmp(prunedName, "#fr_npc_", 8) == 0)
				{
					prunedName += 8;
				}

				g_pVGuiLocalize->ConvertANSIToUnicode(prunedName, killer, sizeof(killer));
			}
		}
		else
		{
			const char* prunedName = m_DeathNotices[i].Killer.szName;
			if (strncmp(prunedName, "#fr_npc_", 8) == 0)
			{
				prunedName += 8;
			}

			g_pVGuiLocalize->ConvertANSIToUnicode(prunedName, killer, sizeof(killer));
		}

		// Get the local position for this notice
		int len = UTIL_ComputeStringWidth(m_hTextFont, victim);
		int y = yStart + (m_flLineHeight * i);

		int iconWide;
		int iconTall;

		if (icon->bRenderUsingFont)
		{
			iconWide = surface()->GetCharacterWidth(icon->hFont, icon->cCharacterInFont);
			iconTall = surface()->GetFontTall(icon->hFont);
		}
		else
		{
			float scale = ((float)ScreenHeight() / 480.0f) / ((float)ScreenWidth() / 640.0f);	//scale based on 640x480
			iconWide = (int)(scale * ((float)icon->Width() * m_flIconSize));
			iconTall = (int)(scale * ((float)icon->Height() * m_flIconSize));
		}

		int x;
		if (m_bRightJustify)
		{
			x = GetWide() - len - iconWide;
		}
		else
		{
			x = 0;
		}

		// Only draw killers name if it wasn't a suicide
		if (!m_DeathNotices[i].iSuicide)
		{
			if (m_bRightJustify)
			{
				x -= UTIL_ComputeStringWidth(m_hTextFont, killer);
			}

			SetColorForNoticePlayer(iKillerTeam);

			// Draw killer's name
			surface()->DrawSetTextPos(x, y);
			surface()->DrawSetTextFont(m_hTextFont);
			surface()->DrawUnicodeString(killer);
			surface()->DrawGetTextPos(x, y);
		}

		// Draw death weapon
		//If we're using a font char, this will ignore iconTall and iconWide
		int iconOffsetX = (m_DeathNotices[i].iconDeath->bIsNormalHL2WeaponIcon ? m_flIconOffsetWepX 
							: (m_DeathNotices[i].iconDeath->bIsNormalHL2AmmoIcon ? m_flIconOffsetAmmoX : m_flIconOffsetX));
		int iconOffsetY = (m_DeathNotices[i].iconDeath->bIsNormalHL2WeaponIcon ? m_flIconOffsetWepY 
							: (m_DeathNotices[i].iconDeath->bIsNormalHL2AmmoIcon ? m_flIconOffsetAmmoY : m_flIconOffsetY));
		icon->DrawSelf(x + iconOffsetX, y + iconOffsetY, iconWide, iconTall, m_cIconColor);
		x += iconWide;

		SetColorForNoticePlayer(iVictimTeam);

		// Draw victims name
		surface()->DrawSetTextPos(x, y);
		surface()->DrawSetTextFont(m_hTextFont);	//reset the font, draw icon can change it
		surface()->DrawUnicodeString(victim);
	}

	// Now retire any death notices that have expired
	RetireExpiredDeathNotices();
}

//-----------------------------------------------------------------------------
// Purpose: This message handler may be better off elsewhere
//-----------------------------------------------------------------------------
void CHudDeathNotice::RetireExpiredDeathNotices(void)
{
	// Loop backwards because we might remove one
	int iSize = m_DeathNotices.Size();
	for (int i = iSize - 1; i >= 0; i--)
	{
		if (m_DeathNotices[i].flDisplayTime < gpGlobals->curtime)
		{
			m_DeathNotices.Remove(i);
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: Server's told us that someone's died
//-----------------------------------------------------------------------------
void CHudDeathNotice::FireGameEvent(IGameEvent* event)
{
	if (!g_PR)
		return;

	if (hud_deathnotice_time.GetFloat() == 0)
		return;

	const char* type = event->GetName();

	if (Q_strcmp(type, "player_death") == 0)
	{
		int killer = engine->GetPlayerForUserID(event->GetInt("attacker"));
		int victim = engine->GetPlayerForUserID(event->GetInt("userid"));
		const char* killedwith = event->GetString("weapon");

		// Do we have too many death messages in the queue?
		if (m_DeathNotices.Count() > 0 &&
			m_DeathNotices.Count() >= (int)m_flMaxDeathNotices)
		{
			// Remove the oldest one in the queue, which will always be the first
			m_DeathNotices.Remove(0);
		}

		// Get the names of the players
		const char* killer_name = g_PR->GetPlayerName(killer);
		const char* victim_name = g_PR->GetPlayerName(victim);

		if (!killer_name)
			killer_name = "";
		if (!victim_name)
			victim_name = "";

		// Make a new death notice
		DeathNoticeItem deathMsg;
		deathMsg.Killer.iEntIndex = killer;
		deathMsg.Victim.iEntIndex = victim;
		Q_strncpy(deathMsg.Killer.szName, killer_name, MAX_PLAYER_NAME_LENGTH);
		Q_strncpy(deathMsg.Victim.szName, victim_name, MAX_PLAYER_NAME_LENGTH);
		deathMsg.flDisplayTime = gpGlobals->curtime + hud_deathnotice_time.GetFloat();
		deathMsg.iSuicide = (!killer || killer == victim);

		// Try and find the death identifier in the icon list
		deathMsg.iconDeath = gHUD.GetIcon(killedwith);

		if (!deathMsg.iconDeath || deathMsg.iSuicide)
		{
			// Can't find it, so use the default skull & crossbones icon
			deathMsg.iconDeath = m_icon;
		}

		// Add it to our list of death notices
		m_DeathNotices.AddToTail(deathMsg);

		const char* prunedVictName = deathMsg.Victim.szName;
		if (strncmp(prunedVictName, "#fr_npc_", 8) == 0)
		{
			prunedVictName += 8;
		}

		const char* prunedKillName = deathMsg.Killer.szName;
		if (strncmp(prunedKillName, "#fr_npc_", 8) == 0)
		{
			prunedKillName += 8;
		}

		char sDeathMsg[512];

		// Record the death notice in the console
		if (deathMsg.iSuicide)
		{
			Q_snprintf(sDeathMsg, sizeof(sDeathMsg), "%s suicided.\n", prunedVictName);
		}
		else
		{
			Q_snprintf(sDeathMsg, sizeof(sDeathMsg), "%s killed %s with %s", prunedKillName, prunedVictName, killedwith);
		}

		Msg("%s\n", sDeathMsg);
	}
	else if (Q_strcmp(type, "npc_death") == 0)
	{
		int killer = engine->GetPlayerForUserID(event->GetInt("attacker"));
		const char* victim = event->GetString("victimname");
		const char* killedwith = event->GetString("weapon");

		// Do we have too many death messages in the queue?
		if (m_DeathNotices.Count() > 0 &&
			m_DeathNotices.Count() >= (int)m_flMaxDeathNotices)
		{
			// Remove the oldest one in the queue, which will always be the first
			m_DeathNotices.Remove(0);
		}

		// Get the names of the players
		const char* killer_name = g_PR->GetPlayerName(killer);
		const char* victim_name = victim;

		if (!killer_name)
			killer_name = "";
		if (!victim_name)
			victim_name = "";

		// Make a new death notice
		DeathNoticeItem deathMsg;
		deathMsg.Killer.iEntIndex = killer;
		Q_strncpy(deathMsg.Killer.szName, killer_name, MAX_PLAYER_NAME_LENGTH);
		Q_strncpy(deathMsg.Victim.szName, victim_name, MAX_PLAYER_NAME_LENGTH);
		deathMsg.flDisplayTime = gpGlobals->curtime + hud_deathnotice_time.GetFloat();
		deathMsg.iSuicide = (!killer);

		// Try and find the death identifier in the icon list
		// Can't find it, so use the default skull & crossbones icon

		deathMsg.iconDeath = gHUD.GetIcon(killedwith);

		if (!deathMsg.iconDeath || deathMsg.iSuicide)
		{
			// Can't find it, so use the default skull & crossbones icon
			deathMsg.iconDeath = m_icon;
		}

		// Add it to our list of death notices
		m_DeathNotices.AddToTail(deathMsg);

		const char* prunedVictName = deathMsg.Victim.szName;
		if (strncmp(prunedVictName, "#fr_npc_", 8) == 0)
		{
			prunedVictName += 8;
		}

		const char* prunedKillName = deathMsg.Killer.szName;
		if (strncmp(prunedKillName, "#fr_npc_", 8) == 0)
		{
			prunedKillName += 8;
		}

		char sDeathMsg[512];

		// Record the death notice in the console
		if (deathMsg.iSuicide)
		{
			Q_snprintf(sDeathMsg, sizeof(sDeathMsg), "%s suicided.\n", prunedVictName);
		}
		else
		{
			Q_snprintf(sDeathMsg, sizeof(sDeathMsg), "%s killed %s with %s", prunedKillName, prunedVictName, killedwith);
		}

		Msg("%s\n", sDeathMsg);
	}
	else if (Q_strcmp(type, "player_death_npc") == 0)
	{
		const char* killer = event->GetString("attacker");
		int victim = engine->GetPlayerForUserID(event->GetInt("userid"));
		const char* killedwith = event->GetString("weapon");

		// Do we have too many death messages in the queue?
		if (m_DeathNotices.Count() > 0 &&
			m_DeathNotices.Count() >= (int)m_flMaxDeathNotices)
		{
			// Remove the oldest one in the queue, which will always be the first
			m_DeathNotices.Remove(0);
		}

		// Get the names of the players
		const char* killer_name = killer;
		const char* victim_name = g_PR->GetPlayerName(victim);

		if (!killer_name)
			killer_name = "";
		if (!victim_name)
			victim_name = "";

		// Make a new death notice
		DeathNoticeItem deathMsg;
		deathMsg.Victim.iEntIndex = victim;
		Q_strncpy(deathMsg.Killer.szName, killer_name, MAX_PLAYER_NAME_LENGTH);
		Q_strncpy(deathMsg.Victim.szName, victim_name, MAX_PLAYER_NAME_LENGTH);
		deathMsg.flDisplayTime = gpGlobals->curtime + hud_deathnotice_time.GetFloat();
		deathMsg.iSuicide = (FStrEq(killer_name, victim_name));

		// Try and find the death identifier in the icon list
		// Can't find it, so use the default skull & crossbones icon
		deathMsg.iconDeath = gHUD.GetIcon(killedwith);

		if (!deathMsg.iconDeath || deathMsg.iSuicide)
		{
			// Can't find it, so use the default skull & crossbones icon
			deathMsg.iconDeath = m_icon;
		}

		// Add it to our list of death notices
		m_DeathNotices.AddToTail(deathMsg);

		const char* prunedVictName = deathMsg.Victim.szName;
		if (strncmp(prunedVictName, "#fr_npc_", 8) == 0)
		{
			prunedVictName += 8;
		}

		const char* prunedKillName = deathMsg.Killer.szName;
		if (strncmp(prunedKillName, "#fr_npc_", 8) == 0)
		{
			prunedKillName += 8;
		}

		char sDeathMsg[512];

		// Record the death notice in the console
		if (deathMsg.iSuicide)
		{
			Q_snprintf(sDeathMsg, sizeof(sDeathMsg), "%s suicided.\n", prunedVictName);
		}
		else
		{
			Q_snprintf(sDeathMsg, sizeof(sDeathMsg), "%s killed %s with %s", prunedKillName, prunedVictName, killedwith);
		}

		Msg("%s\n", sDeathMsg);
	}
}



