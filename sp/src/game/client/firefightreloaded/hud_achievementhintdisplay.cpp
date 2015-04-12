//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================

#include "cbase.h"
#include "hud.h"
#include "hudelement.h"
#include "hud_macros.h"
#include "iclientmode.h"
#include "vgui_controls/AnimationController.h"
#include "vgui_controls/Label.h"
#include "vgui/ILocalize.h"
#include "vgui/ISurface.h"
#include <vgui/IVGui.h>
#include "text_message.h"
#include "c_baseplayer.h"
#include "IGameUIFuncs.h"
#include "inputsystem/iinputsystem.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

#define HUD_ACHIEVEMENTDISPLAY_MINHIDETIME 7.0f

//-----------------------------------------------------------------------------
// Purpose: Displays small key-centric hints on the right hand side of the screen
//-----------------------------------------------------------------------------
class CHudAchievementHintDisplay : public vgui::Panel, public CHudElement
{
	DECLARE_CLASS_SIMPLE(CHudAchievementHintDisplay, vgui::Panel);

public:
	CHudAchievementHintDisplay(const char *pElementName);
	void Init();
	void Reset();
	void MsgFunc_AchievementHintText(bf_read &msg);
	bool ShouldDraw();

	bool SetHintText( const char *text );
	virtual void	OnTick(void);

protected:
	virtual void ApplySchemeSettings( vgui::IScheme *pScheme );
	virtual void OnThink();

private:
	CUtlVector<vgui::Label *> m_Labels;
	vgui::HFont m_hLargeFont;
	int		m_iBaseY;
	float m_flHudHintMinDisplayTime;
	bool  m_bHintDisplayed;

	CPanelAnimationVarAliasType( float, m_iTextX, "text_xpos", "8", "proportional_float" );
	CPanelAnimationVarAliasType( float, m_iTextY, "text_ypos", "8", "proportional_float" );
	CPanelAnimationVarAliasType( float, m_iTextGapX, "text_xgap", "8", "proportional_float" );
	CPanelAnimationVarAliasType( float, m_iTextGapY, "text_ygap", "8", "proportional_float" );
	CPanelAnimationVarAliasType( float, m_iYOffset, "YOffset", "0", "proportional_float" );
};

DECLARE_HUDELEMENT(CHudAchievementHintDisplay);
DECLARE_HUD_MESSAGE(CHudAchievementHintDisplay, AchievementHintText);

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CHudAchievementHintDisplay::CHudAchievementHintDisplay(const char *pElementName) : BaseClass(NULL, "HudAchievementHintDisplay"), CHudElement(pElementName)
{
	vgui::Panel *pParent = g_pClientMode->GetViewport();
	SetParent( pParent );
	m_flHudHintMinDisplayTime = 0;
	m_bHintDisplayed = false;
	SetVisible( false );
	SetAlpha( 0 );
	vgui::ivgui()->AddTickSignal(GetVPanel());
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHudAchievementHintDisplay::Init()
{
	HOOK_HUD_MESSAGE(CHudAchievementHintDisplay, AchievementHintText);
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHudAchievementHintDisplay::Reset()
{
	SetHintText( NULL );
	SetAlpha( 0 );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHudAchievementHintDisplay::ApplySchemeSettings(vgui::IScheme *pScheme)
{
	m_hLargeFont = pScheme->GetFont( "HudHintTextLarge", true );

	BaseClass::ApplySchemeSettings( pScheme );
}

//-----------------------------------------------------------------------------
// Purpose: Save CPU cycles by letting the HUD system early cull
// costly traversal.  Called per frame, return true if thinking and 
// painting need to occur.
//-----------------------------------------------------------------------------
bool CHudAchievementHintDisplay::ShouldDraw(void)
{
	return ( ( GetAlpha() > 0 ) && CHudElement::ShouldDraw() );
}

//-----------------------------------------------------------------------------
// Purpose: Called on each tick
//-----------------------------------------------------------------------------
void CHudAchievementHintDisplay::OnTick(void)
{
	if (gpGlobals->curtime < m_flHudHintMinDisplayTime)
	{
		if (m_bHintDisplayed)
		{
			SetHintText(NULL);
			m_bHintDisplayed = false;
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: Updates the label color each frame
//-----------------------------------------------------------------------------
void CHudAchievementHintDisplay::OnThink()
{
	for (int i = 0; i < m_Labels.Count(); i++)
	{
		m_Labels[i]->SetFgColor(GetFgColor());
	}

	int ox, oy;
	GetPos(ox, oy);
	SetPos( ox, m_iBaseY + m_iYOffset );
}

//-----------------------------------------------------------------------------
// Purpose: Sets the hint text, replacing variables as necessary
//-----------------------------------------------------------------------------
bool CHudAchievementHintDisplay::SetHintText(const char *text)
{
	if ( text == NULL || text[0] == L'\0' )
		return false;

	// clear the existing text
	for (int i = 0; i < m_Labels.Count(); i++)
	{
		m_Labels[i]->MarkForDeletion();
	}
	m_Labels.RemoveAll();

	// look up the text string
	wchar_t *ws = g_pVGuiLocalize->Find( text );

	wchar_t wszBuf[256];
	if ( !ws || wcslen(ws) <= 0)
	{
		if (text[0] == '#')
		{
			// We don't want to display a localization placeholder, do we?
			return false;
		}
		// use plain ASCII string 
		g_pVGuiLocalize->ConvertANSIToUnicode(text, wszBuf, sizeof(wszBuf));
		ws = wszBuf;
	}

	// parse out the text into a label set
	while ( *ws )
	{
		wchar_t token[256];

		wcsncpy(token, ws, ARRAYSIZE(token));
		token[ARRAYSIZE(token) - 1] = L'\0';	// force null termination

		ws += wcslen( token );

		// put it in a label
		vgui::Label *label = vgui::SETUP_PANEL(new vgui::Label(this, NULL, token));
		
		label->SetFont(m_hLargeFont);

		label->SetPaintBackgroundEnabled( false );
		label->SetPaintBorderEnabled( false );
		label->SizeToContents();
		label->SetContentAlignment( vgui::Label::a_west );
		label->SetFgColor(GetFgColor());
		m_Labels.AddToTail( vgui::SETUP_PANEL(label) );
	}

	// find the bounds we need to show
	int widest1 = 0, widest2 = 0;
	for (int i = 0; i < m_Labels.Count(); i++)
	{
		vgui::Label *label = m_Labels[i];

		if (i & 1)
		{
			// help text
			if (label->GetWide() > widest2)
			{
				widest2 = label->GetWide();
			}
		}
		else
		{
			// variable
			if (label->GetWide() > widest1)
			{
				widest1 = label->GetWide();
			}
		}
	}

	// position the labels
	int col1_x = m_iTextX;
	int col2_x = m_iTextX + widest1 + m_iTextGapX;
	int col_y = m_iTextY;

	for (int i = 0; i < m_Labels.Count(); i += 2)
	{
		int rowHeight = 0;
		vgui::Label *label0 = m_Labels[i];
		int tall0 = label0->GetTall();
		rowHeight = tall0;

		if (i + 1 < m_Labels.Count())
		{
			vgui::Label *label1 = m_Labels[i + 1];
			int tall1 = label1->GetTall();
			rowHeight = MAX(tall0, tall1);
			label1->SetPos( col2_x, col_y + (rowHeight - tall1) / 2 );
		}

		label0->SetPos( col1_x, col_y + (rowHeight - tall0) / 2 );

		col_y += rowHeight + m_iTextGapY;
	}

	// move ourselves relative to our start position
	int newWide = m_iTextX + col2_x + widest2;
	int newTall = col_y;
	int ox, oy;
	GetPos(ox, oy);

	if (IsRightAligned())
	{
		int oldWide = GetWide();
		int diff = newWide - oldWide;
		ox -= diff;
	}

	if (IsBottomAligned())
	{
		int oldTall = GetTall();
		int diff = newTall - oldTall;
		oy -= diff;
	}

	// set the size of the hint panel to fit
 	SetPos( ox, oy );
 	SetSize( newWide, newTall );

	m_iBaseY = oy;

	m_bHintDisplayed = true;
	m_flHudHintMinDisplayTime = gpGlobals->curtime + HUD_ACHIEVEMENTDISPLAY_MINHIDETIME;

	return true;
}

//-----------------------------------------------------------------------------
// Purpose: Activates the hint display
//-----------------------------------------------------------------------------
void CHudAchievementHintDisplay::MsgFunc_AchievementHintText(bf_read &msg)
{
	// how many strings do we receive ?
	int count = msg.ReadByte();

	// here we expect only one string
	if ( count != 1 )
	{
		DevMsg("CHudAchievementHintDisplay::MsgFunc_AchievementHintText: string count != 1.\n");
		return;
	}

	// read the string
	char szString[2048];
	msg.ReadString( szString, sizeof(szString) );

	// make it visible
	if ( SetHintText( szString ) )
	{
		SetVisible( true );
 		g_pClientMode->GetViewportAnimationController()->StartAnimationSequence( "AchievementHintMessageShow" ); 
	}
	else
	{
		// it's being cleared, hide the panel
		g_pClientMode->GetViewportAnimationController()->StartAnimationSequence( "AchievementHintMessageHide" ); 
	}
}
