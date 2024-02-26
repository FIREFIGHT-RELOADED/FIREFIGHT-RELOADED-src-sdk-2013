#include "gamepadui_interface.h"
#include "gamepadui_image.h"
#include "gamepadui_util.h"
#include "gamepadui_genericconfirmation.h"
#include "gamepadui_scroll.h"

#include "ienginevgui.h"
#include "vgui/ILocalize.h"
#include "vgui/ISurface.h"
#include "vgui/IVGui.h"
#include "GameUI/IGameUI.h"

#include "vgui_controls/ComboBox.h"
#include "vgui_controls/ImagePanel.h"
#include "vgui_controls/ScrollBar.h"

#include "KeyValues.h"
#include "filesystem.h"
#include "materialsystem/imaterial.h"

#include "tier0/memdbgon.h"

class GamepadUIMapChooserButton;

class GamepadUIMapChooser : public GamepadUIFrame
{
    DECLARE_CLASS_SIMPLE( GamepadUIMapChooser, GamepadUIFrame );

public:
    GamepadUIMapChooser( vgui::Panel *pParent, const char* pPanelName);

    void UpdateGradients();

	void OnThink() OVERRIDE;
    void Paint() OVERRIDE;
    void OnCommand( char const* pCommand ) OVERRIDE;
    void OnMouseWheeled( int nDelta ) OVERRIDE;

    MESSAGE_FUNC_HANDLE( OnGamepadUIButtonNavigatedTo, "OnGamepadUIButtonNavigatedTo", button );

private:
    void ScanMaps();
    void LayoutMapButtons();

    CUtlVector<GamepadUIMapChooserButton*> m_pMapPanels;

    GamepadUIScrollState m_ScrollState;

    GAMEPADUI_PANEL_PROPERTY( float, m_flMapFade, "Saves.Fade", "0", SchemeValueTypes::ProportionalFloat );
    GAMEPADUI_PANEL_PROPERTY( float, m_flMapOffsetX, "Saves.OffsetX", "0", SchemeValueTypes::ProportionalFloat );
    GAMEPADUI_PANEL_PROPERTY( float, m_flMapOffsetY, "Saves.OffsetY", "0", SchemeValueTypes::ProportionalFloat );
    GAMEPADUI_PANEL_PROPERTY( float, m_flMapSpacing, "Saves.Spacing", "0", SchemeValueTypes::ProportionalFloat );
};

class GamepadUIMapChooserButton : public GamepadUIButton
{
public:
    DECLARE_CLASS_SIMPLE( GamepadUIMapChooserButton, GamepadUIButton );

    GamepadUIMapChooserButton( vgui::Panel* pParent, vgui::Panel* pActionSignalTarget, const char *pSchemeFile, const char* pCommand, const char* pText, const char* pDescription, const char *pChapterImage )
        : BaseClass( pParent, pActionSignalTarget, pSchemeFile, pCommand, pText, pDescription)
        , m_Image( pChapterImage )
    {
    }

    GamepadUIMapChooserButton(vgui::Panel* pParent, vgui::Panel* pActionSignalTarget, const char* pSchemeFile, const char* pCommand, const wchar* pText, const wchar* pDescription, const char* pChapterImage)
        : BaseClass(pParent, pActionSignalTarget, pSchemeFile, pCommand, pText, pDescription)
        , m_Image(pChapterImage)
    {
    }

    void Paint() OVERRIDE
    {
        int x, y, w, t;
        GetBounds( x, y, w, t );

        PaintButton();

		if ( m_Image.IsValid() )
		{
			vgui::surface()->DrawSetColor( Color( 255, 255, 255, 255 ) );
			vgui::surface()->DrawSetTexture( m_Image );
			// Save game icons are 180x100
			int imgW = ( t * 180 ) / 100;
			int imgH = t;
			// Half pixel offset to avoid leaking into pink + black
			vgui::surface()->DrawTexturedRect( 0, 0, imgW, imgH );
			vgui::surface()->DrawSetTexture( 0 );
		}
		else
		{
			vgui::surface()->DrawSetColor( Color( 0, 0, 0, 255 ) );
			int imgW = ( t * 180 ) / 100;
			int imgH = t;
			vgui::surface()->DrawFilledRect( 0, 0, imgW, imgH );
		}

        PaintText();
    }

private:
    GamepadUIImage m_Image;
};

GamepadUIMapChooser::GamepadUIMapChooser( vgui::Panel* pParent, const char* pPanelName)
	: BaseClass( pParent, pPanelName )
{
    vgui::HScheme Scheme = vgui::scheme()->LoadSchemeFromFile( GAMEPADUI_DEFAULT_PANEL_SCHEME, "SchemePanel" );
    SetScheme( Scheme );

    GetFrameTitle() = GamepadUIString( "#GameUI_NewGame" );

    Activate();

    ScanMaps();

    if ( m_pMapPanels.Count() )
        m_pMapPanels[0]->NavigateTo();

    for ( int i = 1; i < m_pMapPanels.Count(); i++ )
    {
        m_pMapPanels[i]->SetNavUp( m_pMapPanels[i - 1] );
        m_pMapPanels[i - 1]->SetNavDown( m_pMapPanels[i] );
    }

	UpdateGradients();
}

void GamepadUIMapChooser::UpdateGradients()
{
	const float flTime = GamepadUI::GetInstance().GetTime();
	GamepadUI::GetInstance().GetGradientHelper()->ResetTargets( flTime );
	GamepadUI::GetInstance().GetGradientHelper()->SetTargetGradient( GradientSide::Up, { 1.0f, 1.0f }, flTime );
	GamepadUI::GetInstance().GetGradientHelper()->SetTargetGradient( GradientSide::Down, { 1.0f, 1.0f }, flTime );
}

void GamepadUIMapChooser::OnThink()
{
	BaseClass::OnThink();

	LayoutMapButtons();
}

void GamepadUIMapChooser::Paint()
{
    BaseClass::Paint();
}

void GamepadUIMapChooser::ScanMaps()
{
	FileFindHandle_t findHandle = NULL;

    int mapIndex = 0;
    const char* pszFilename = g_pFullFileSystem->FindFirst("maps/*.bsp", &findHandle);
    while (pszFilename)
    {
        char mapname[256];

        // FindFirst ignores the pszPathID, so check it here
        // TODO: this doesn't find maps in fallback dirs
        Q_snprintf(mapname, sizeof(mapname), "maps/%s", pszFilename);

        // remove the text 'maps/' and '.bsp' from the file name to get the map name

        //skip credits.bsp
        const char* creditsstr = Q_strstr(pszFilename, "credits");
        if (creditsstr)
        {
            pszFilename = g_pFullFileSystem->FindNext(findHandle);
        }

        creditsstr = Q_strstr(pszFilename, "firefight_advisor");
        if (creditsstr)
        {
            pszFilename = g_pFullFileSystem->FindNext(findHandle);
        }

        const char* str = Q_strstr(pszFilename, "maps");
        if (str)
        {
            Q_strncpy(mapname, str + 5, sizeof(mapname) - 1);	// maps + \\ = 5
        }
        else
        {
            Q_strncpy(mapname, pszFilename, sizeof(mapname) - 1);
        }

        char* ext = Q_strstr(mapname, ".bsp");
        if (ext)
        {
            *ext = 0;
        }

        char command[256];
        Q_snprintf(command, sizeof(command), "load_map %s", mapname);

        char chapterName[256];
        Q_snprintf(chapterName, sizeof(chapterName), "%s", mapname);

        //uppercase all the map names.
        for (int i = Q_strlen(chapterName); i >= 0; --i)
        {
            chapterName[i] = toupper(chapterName[i]);
        }

        wchar_t text[32];
        wchar_t* chapter = g_pVGuiLocalize->Find("#GameUI_Map");
        _snwprintf(text, ARRAYSIZE(text), L"%ls", chapter ? chapter : L"Map");

        GamepadUIString strChapterName(chapterName);

        char chapterImage[64];
        Q_snprintf(chapterImage, sizeof(chapterImage), "gamepadui/maps/%s.vmt", mapname);

        IMaterial* pMaterial = g_pMaterialSystem->FindMaterial(chapterImage, TEXTURE_GROUP_VGUI);

        if (IsErrorMaterial(pMaterial))
        {
            Q_snprintf(chapterImage, sizeof(chapterImage), "gamepadui/maps/unknown.vmt");
        }

        GamepadUIMapChooserButton* button = new GamepadUIMapChooserButton(
            this, this,
            GAMEPADUI_RESOURCE_FOLDER "schemesavebutton.res", command,
            strChapterName.String(), text, chapterImage);
        button->SetEnabled(true);
        button->SetPriority(mapIndex);
        button->SetForwardToParent(true);

        m_pMapPanels.AddToTail( button );
        mapIndex++;

        // get the next file
        pszFilename = g_pFullFileSystem->FindNext(findHandle);
    }
    g_pFullFileSystem->FindClose(findHandle);
    
    SetFooterButtons( FooterButtons::Back | FooterButtons::Select /*, FooterButtons::Select*/);
    
	SetControlEnabled( "loadsave", false );
	SetControlEnabled( "delete", false );
}

void GamepadUIMapChooser::OnGamepadUIButtonNavigatedTo( vgui::VPANEL button )
{
    GamepadUIButton *pButton = dynamic_cast< GamepadUIButton * >( vgui::ipanel()->GetPanel( button, GetModuleName() ) );
    if ( pButton )
    {
        if ( pButton->GetAlpha() != 255 )
        {
            int nParentW, nParentH;
			GetParent()->GetSize( nParentW, nParentH );

            int nX, nY;
            pButton->GetPos( nX, nY );

            int nTargetY = pButton->GetPriority() * ( pButton->m_flHeightAnimationValue[ButtonStates::Out] + m_flMapSpacing );

            if ( nY < nParentH / 2 )
            {
                nTargetY += nParentH - m_flMapOffsetY;
                // Add a bit of spacing to make this more visually appealing :)
                nTargetY -= m_flMapSpacing;
            }
            else
            {
                nTargetY += pButton->m_flHeightAnimationValue[ButtonStates::Over];
                // Add a bit of spacing to make this more visually appealing :)
                nTargetY += (pButton->m_flHeightAnimationValue[ButtonStates::Over] / 2) + m_flMapSpacing;
            }


            m_ScrollState.SetScrollTarget( nTargetY - ( nParentH - m_flMapOffsetY), GamepadUI::GetInstance().GetTime() );
        }
    }
}

void GamepadUIMapChooser::LayoutMapButtons()
{
    int nParentW, nParentH;
	GetParent()->GetSize( nParentW, nParentH );

    float scrollClamp = 0.0f;
    for ( int i = 0; i < ( int )m_pMapPanels.Count(); i++ )
    {
        int size = ( m_pMapPanels[i]->GetTall() + m_flMapSpacing );

        if ( i < ( ( int )m_pMapPanels.Count() ) - 3 )
            scrollClamp += size;
    }

    m_ScrollState.UpdateScrollBounds( 0.0f, scrollClamp );

    int previousSizes = 0;
    for ( int i = 0; i < ( int )m_pMapPanels.Count(); i++ )
    {
        int tall = m_pMapPanels[i]->GetTall();
        int size = ( tall + m_flMapSpacing );

        int y = m_flMapOffsetY + previousSizes - m_ScrollState.GetScrollProgress();
        int fade = 255;
        if ( y < m_flMapOffsetY )
            fade = ( 1.0f - clamp( -( y - m_flMapOffsetY ) / m_flMapFade, 0.0f, 1.0f ) ) * 255.0f;
        if ( y > nParentH - m_flMapFade )
            fade = ( 1.0f - clamp( ( y - ( nParentH - m_flMapFade - size ) ) / m_flMapFade, 0.0f, 1.0f ) ) * 255.0f;
        if ( m_pMapPanels[i]->HasFocus() && fade != 0 )
            fade = 255;
        m_pMapPanels[i]->SetAlpha( fade );
        m_pMapPanels[i]->SetPos( m_flMapOffsetX, y );
        m_pMapPanels[i]->SetVisible( true );
        previousSizes += size;
    }

    m_ScrollState.UpdateScrolling( 2.0f, GamepadUI::GetInstance().GetTime() );
}

void GamepadUIMapChooser::OnCommand( char const* pCommand )
{
    if ( !V_strcmp( pCommand, "action_back" ) )
    {
        Close();
    }
	 else if ( StringHasPrefixCaseSensitive( pCommand, "load_map " ) )
    {
        const char* pszMap = pCommand + 9;
        
        if (*pszMap)
        {
            char command[256];
            Q_snprintf(command, sizeof(command), "cmd disconnect;maxplayers 1;map %s;progress_enable", pszMap);

            GamepadUI::GetInstance().GetEngineClient()->ClientCmd_Unrestricted(command);
            Close();
        }
    }
    else
    {
        BaseClass::OnCommand( pCommand );
    }
}

void GamepadUIMapChooser::OnMouseWheeled( int delta )
{
    m_ScrollState.OnMouseWheeled(delta * 200.0f, GamepadUI::GetInstance().GetTime());
}

CON_COMMAND( gamepadui_openmapchooser, "" )
{
    new GamepadUIMapChooser( GamepadUI::GetInstance().GetBasePanel(), "" );
}
