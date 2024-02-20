#include "gamepadui_interface.h"
#include "gamepadui_image.h"
#include "gamepadui_util.h"
#include "gamepadui_frame.h"
#include "gamepadui_scroll.h"
#include "gamepadui_genericconfirmation.h"

#include "ienginevgui.h"
#include "vgui/ILocalize.h"
#include "vgui/ISurface.h"
#include "vgui/IVGui.h"

#include "vgui_controls/ComboBox.h"
#include "vgui_controls/ImagePanel.h"
#include "vgui_controls/ScrollBar.h"

#include "KeyValues.h"
#include "filesystem.h"

#include "icommandline.h"

#include "tier0/memdbgon.h"

class GamepadUIMapButton;

#define GAMEPADUI_MAP_SCHEME GAMEPADUI_RESOURCE_FOLDER "schemechapterbutton.res"

class GamepadUIMapChooser : public GamepadUIFrame
{
    DECLARE_CLASS_SIMPLE( GamepadUIMapChooser, GamepadUIFrame );

public:
    GamepadUIMapChooser( vgui::Panel *pParent, const char* pPanelName );

    void UpdateGradients();

    void OnThink() OVERRIDE;
    void ApplySchemeSettings(vgui::IScheme* pScheme) OVERRIDE;
    void OnCommand( char const* pCommand ) OVERRIDE;

    MESSAGE_FUNC_HANDLE( OnGamepadUIButtonNavigatedTo, "OnGamepadUIButtonNavigatedTo", button );

    void LayoutMapButtons();

    void OnMouseWheeled( int delta ) OVERRIDE;

private:
    CUtlVector< GamepadUIMapButton* > m_pChapterButtons;

    GamepadUIScrollState m_ScrollState;

    GAMEPADUI_PANEL_PROPERTY( float, m_ChapterOffsetX, "Chapters.OffsetX", "0", SchemeValueTypes::ProportionalFloat );
    GAMEPADUI_PANEL_PROPERTY( float, m_ChapterOffsetY, "Chapters.OffsetY", "0", SchemeValueTypes::ProportionalFloat );
    GAMEPADUI_PANEL_PROPERTY( float, m_ChapterSpacing, "Chapters.Spacing", "0", SchemeValueTypes::ProportionalFloat );

    bool m_bCommentaryMode = false;
};

class GamepadUIMapButton : public GamepadUIButton
{
public:
    DECLARE_CLASS_SIMPLE( GamepadUIMapButton, GamepadUIButton );

    GamepadUIMapButton( vgui::Panel* pParent, vgui::Panel* pActionSignalTarget, const char* pSchemeFile, const char* pCommand, const char* pText, const char* pDescription, const char *pChapterImage )
        : BaseClass( pParent, pActionSignalTarget, pSchemeFile, pCommand, pText, pDescription )
        , m_Image( pChapterImage )
    {
    }

    GamepadUIMapButton( vgui::Panel* pParent, vgui::Panel* pActionSignalTarget, const char* pSchemeFile, const char* pCommand, const wchar* pText, const wchar* pDescription, const char *pChapterImage )
        : BaseClass( pParent, pActionSignalTarget, pSchemeFile, pCommand, pText, pDescription )
        , m_Image( pChapterImage )
    {
    }

    ~GamepadUIMapButton()
    {
        if ( s_pLastNewGameButton == this )
            s_pLastNewGameButton = NULL;
    }

    ButtonState GetCurrentButtonState() OVERRIDE
    {
        if ( s_pLastNewGameButton == this )
            return ButtonState::Over;
        return BaseClass::GetCurrentButtonState();
    }

    void Paint() OVERRIDE
    {
        int x, y, w, t;
        GetBounds( x, y, w, t );

        PaintButton();

        vgui::surface()->DrawSetColor( Color( 255, 255, 255, 255 ) );
        vgui::surface()->DrawSetTexture( m_Image );
        int imgH = ( w * 9 ) / 16;
        //vgui::surface()->DrawTexturedRect( 0, 0, w, );
        float offset = m_flTextOffsetYAnimationValue[ButtonStates::Out] - m_flTextOffsetY;
        vgui::surface()->DrawTexturedSubRect( 0, 0, w, imgH - offset, 0.0f, 0.0f, 1.0f, ( imgH - offset ) / imgH );
        vgui::surface()->DrawSetTexture( 0 );
        if ( !IsEnabled() )
        {
            vgui::surface()->DrawSetColor( Color( 255, 255, 255, 16 ) );
            vgui::surface()->DrawFilledRect( 0, 0, w, imgH - offset );
        }

        PaintText();
    }

    void ApplySchemeSettings(vgui::IScheme* pScheme)
    {
        BaseClass::ApplySchemeSettings(pScheme);

        if (GamepadUI::GetInstance().GetScreenRatio() != 1.0f)
        {
            float flScreenRatio = GamepadUI::GetInstance().GetScreenRatio();

            m_flHeight *= flScreenRatio;
            for (int i = 0; i < ButtonStates::Count; i++)
                m_flHeightAnimationValue[i] *= flScreenRatio;

            // Also change the text offset
            m_flTextOffsetY *= flScreenRatio;
            for (int i = 0; i < ButtonStates::Count; i++)
                m_flTextOffsetYAnimationValue[i] *= flScreenRatio;

            SetSize(m_flWidth, m_flHeight + m_flExtraHeight);
            DoAnimations(true);
        }
    }

    void NavigateTo() OVERRIDE
    {
        BaseClass::NavigateTo();
        s_pLastNewGameButton = this;
    }

    static GamepadUIMapButton* GetLastNewGameButton() { return s_pLastNewGameButton; }

private:
    GamepadUIImage m_Image;

    static GamepadUIMapButton *s_pLastNewGameButton;
};

GamepadUIMapButton* GamepadUIMapButton::s_pLastNewGameButton = NULL;

GamepadUIMapChooser::GamepadUIMapChooser( vgui::Panel *pParent, const char* PanelName ) : BaseClass( pParent, PanelName )
{
    vgui::HScheme hScheme = vgui::scheme()->LoadSchemeFromFile( GAMEPADUI_DEFAULT_PANEL_SCHEME, "SchemePanel" );
    SetScheme( hScheme );

    GetFrameTitle() = GamepadUIString( "#GameUI_NewGame" );
    FooterButtonMask buttons = FooterButtons::Back | FooterButtons::Select;
    SetFooterButtons(buttons/*, FooterButtons::Select*/);

    Activate();

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
        if (!g_pFullFileSystem->FileExists(chapterImage))
        {
            Q_snprintf(chapterImage, sizeof(chapterImage), "vgui/hud/icon_locked.vmt", mapname);
        }

        GamepadUIMapButton* pChapterButton = new GamepadUIMapButton(
            this, this,
            GAMEPADUI_MAP_SCHEME, command,
            strChapterName.String(), text, chapterImage);
        pChapterButton->SetEnabled(true);
        pChapterButton->SetPriority(mapIndex);
        pChapterButton->SetForwardToParent(true);

        m_pChapterButtons.AddToTail(pChapterButton);
        mapIndex++;

        // get the next file
        pszFilename = g_pFullFileSystem->FindNext(findHandle);
    }
    g_pFullFileSystem->FindClose(findHandle);

    if ( m_pChapterButtons.Count() > 0 )
        m_pChapterButtons[0]->NavigateTo();

    for ( int i = 1; i < m_pChapterButtons.Count(); i++ )
    {
        m_pChapterButtons[i]->SetNavLeft( m_pChapterButtons[i - 1] );
        m_pChapterButtons[i - 1]->SetNavRight( m_pChapterButtons[i] );
    }

    UpdateGradients();
}

void GamepadUIMapChooser::UpdateGradients()
{
    const float flTime = GamepadUI::GetInstance().GetTime();
    GamepadUI::GetInstance().GetGradientHelper()->ResetTargets( flTime );
    GamepadUI::GetInstance().GetGradientHelper()->SetTargetGradient( GradientSide::Up, { 1.0f, 1.0f }, flTime );
    GamepadUI::GetInstance().GetGradientHelper()->SetTargetGradient( GradientSide::Down, { 1.0f, 0.5f }, flTime );
}

void GamepadUIMapChooser::OnThink()
{
    BaseClass::OnThink();

    LayoutMapButtons();
}

void GamepadUIMapChooser::ApplySchemeSettings(vgui::IScheme* pScheme)
{
    BaseClass::ApplySchemeSettings(pScheme);

    if (GamepadUI::GetInstance().GetScreenRatio() != 1.0f)
    {
        float flScreenRatio = GamepadUI::GetInstance().GetScreenRatio();
        m_ChapterOffsetX *= (flScreenRatio * flScreenRatio);
    }
}

void GamepadUIMapChooser::OnGamepadUIButtonNavigatedTo( vgui::VPANEL button )
{
    GamepadUIButton *pButton = dynamic_cast< GamepadUIButton * >( vgui::ipanel()->GetPanel( button, GetModuleName() ) );
    if ( pButton )
    {
        int nParentW, nParentH;
	    GetParent()->GetSize( nParentW, nParentH );

        int nX, nY;
        pButton->GetPos( nX, nY );
        if ( nX + pButton->m_flWidth > nParentW || nX < 0 )
        {
            int nTargetX = pButton->GetPriority() * (pButton->m_flWidth + m_ChapterSpacing);

            if ( nX < nParentW / 2 )
            {
                nTargetX += nParentW - m_ChapterOffsetX;
                // Add a bit of spacing to make this more visually appealing :)
                nTargetX -= m_ChapterSpacing;
            }
            else
            {
                nTargetX += pButton->m_flWidth;
                // Add a bit of spacing to make this more visually appealing :)
                nTargetX += (pButton->m_flWidth / 2) + m_ChapterSpacing;
            }


            m_ScrollState.SetScrollTarget( nTargetX - ( nParentW - m_ChapterOffsetX ), GamepadUI::GetInstance().GetTime() );
        }
    }
}

void GamepadUIMapChooser::LayoutMapButtons()
{
    int nParentW, nParentH;
	GetParent()->GetSize( nParentW, nParentH );

    float flScrollClamp = 0.0f;
    for ( int i = 0; i < m_pChapterButtons.Count(); i++ )
    {
        int nSize = ( m_pChapterButtons[0]->GetWide() + m_ChapterSpacing );

        if ( i < m_pChapterButtons.Count() - 2 )
            flScrollClamp += nSize;
    }

    m_ScrollState.UpdateScrollBounds( 0.0f, flScrollClamp );

    for ( int i = 0; i < m_pChapterButtons.Count(); i++ )
    {
        int size = ( m_pChapterButtons[0]->GetWide() + m_ChapterSpacing );

        m_pChapterButtons[i]->SetPos( m_ChapterOffsetX + i * size - m_ScrollState.GetScrollProgress(), m_ChapterOffsetY );
        m_pChapterButtons[i]->SetVisible( true );
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

void GamepadUIMapChooser::OnMouseWheeled( int nDelta )
{
    m_ScrollState.OnMouseWheeled( nDelta * m_ChapterSpacing * 20.0f, GamepadUI::GetInstance().GetTime() );
}

CON_COMMAND( gamepadui_openmapchooser, "" )
{
    new GamepadUIMapChooser( GamepadUI::GetInstance().GetBasePanel(), "" );
}
