//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#ifndef FR_MODDINGMENU_MAPPING_H
#define FR_MODDINGMENU_MAPPING_H
#ifdef _WIN32
#pragma once
#endif

#include <vgui_controls/Frame.h>
#include <vgui_controls/Button.h>

#include <game/client/iviewport.h>

#include <vgui/KeyCode.h>
#include <utlvector.h>


//-----------------------------------------------------------------------------
// Purpose: Displays the team menu
//-----------------------------------------------------------------------------
class CFRModdingMenu_Mapping : public vgui::Frame, public IViewPortPanel
{
private:
	DECLARE_CLASS_SIMPLE( CFRModdingMenu_Mapping, vgui::Frame );

public:
	CFRModdingMenu_Mapping(IViewPort *pViewPort);
	virtual ~CFRModdingMenu_Mapping();

	virtual const char *GetName( void ) { return PANEL_MODDING_MAPPING; }
	virtual void SetData(KeyValues *data) {};
	virtual void Reset() {};
	virtual void Update();
	virtual bool NeedsUpdate( void ) { return false; }
	virtual bool HasInputElements( void ) { return true; }
	virtual void ShowPanel( bool bShow );
	void OnCommand(const char *command);

	void MoveToCenterOfScreen();

	// both vgui::Frame and IViewPortPanel define these, so explicitly define them here as passthroughs to vgui
	vgui::VPANEL GetVPanel( void ) { return BaseClass::GetVPanel(); }
  	virtual bool IsVisible() { return BaseClass::IsVisible(); }
	virtual void SetParent( vgui::VPANEL parent ) { BaseClass::SetParent( parent ); }
	
protected:

	// int GetNumTeams() { return m_iNumTeams; }
	
	// VGUI2 overrides
	virtual void ApplySchemeSettings(vgui::IScheme *pScheme);
	virtual void OnKeyCodePressed(vgui::KeyCode code);

	// helper functions
	virtual void SetLabelText(const char *textEntryName, const char *text);
	//virtual void LoadMapPage( const char *mapName );
	// virtual void MakeTeamButtons( void );
	
	// command callbacks
	// MESSAGE_FUNC_INT( OnTeamButton, "TeamButton", team );

	IViewPort	*m_pViewPort;
	//vgui::RichText *m_pMapInfo;
	//vgui::HTML *m_pMapInfoHTML;
//	int m_iNumTeams;
	ButtonCode_t m_iJumpKey;
	ButtonCode_t m_iScoreBoardKey;

	//char m_szMapName[ MAX_PATH ];
};

#endif // TEAMMENU_H
