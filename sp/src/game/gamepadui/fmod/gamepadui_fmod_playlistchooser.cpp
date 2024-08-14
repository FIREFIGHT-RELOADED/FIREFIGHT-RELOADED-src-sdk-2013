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

class GamepadUIPlaylistChooserButton;

class GamepadUIPlaylistChooser : public GamepadUIFrame
{
    DECLARE_CLASS_SIMPLE( GamepadUIPlaylistChooser, GamepadUIFrame );

public:
    GamepadUIPlaylistChooser( vgui::Panel *pParent, const char* pPanelName);

    void UpdateGradients();

	void OnThink() OVERRIDE;
    void Paint() OVERRIDE;
    void OnCommand( char const* pCommand ) OVERRIDE;
    void OnMouseWheeled( int nDelta ) OVERRIDE;

    MESSAGE_FUNC_HANDLE( OnGamepadUIButtonNavigatedTo, "OnGamepadUIButtonNavigatedTo", button );

private:
    void ScanPlaylists();
    void LayoutPlaylistButtons();

    CUtlVector<GamepadUIPlaylistChooserButton*> m_pPlaylistPanels;

    GamepadUIScrollState m_ScrollState;

    GAMEPADUI_PANEL_PROPERTY( float, m_flPlaylistFade, "Saves.Fade", "0", SchemeValueTypes::ProportionalFloat );
    GAMEPADUI_PANEL_PROPERTY( float, m_flPlaylistOffsetX, "Saves.OffsetX", "0", SchemeValueTypes::ProportionalFloat );
    GAMEPADUI_PANEL_PROPERTY( float, m_flPlaylistOffsetY, "Saves.OffsetY", "0", SchemeValueTypes::ProportionalFloat );
    GAMEPADUI_PANEL_PROPERTY( float, m_flPlaylistSpacing, "Saves.Spacing", "0", SchemeValueTypes::ProportionalFloat );
};

class GamepadUIPlaylistChooserButton : public GamepadUIButton
{
public:
    DECLARE_CLASS_SIMPLE( GamepadUIPlaylistChooserButton, GamepadUIButton );

    GamepadUIPlaylistChooserButton( vgui::Panel* pParent, vgui::Panel* pActionSignalTarget, const char *pSchemeFile, const char* pCommand, const char* pText, const char* pDescription, const char *pChapterImage )
        : BaseClass( pParent, pActionSignalTarget, pSchemeFile, pCommand, pText, pDescription)
        , m_Image( pChapterImage )
    {
    }

    GamepadUIPlaylistChooserButton(vgui::Panel* pParent, vgui::Panel* pActionSignalTarget, const char* pSchemeFile, const char* pCommand, const wchar* pText, const wchar* pDescription, const char* pChapterImage)
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

GamepadUIPlaylistChooser::GamepadUIPlaylistChooser( vgui::Panel* pParent, const char* pPanelName)
	: BaseClass( pParent, pPanelName )
{
    vgui::HScheme Scheme = vgui::scheme()->LoadSchemeFromFile( GAMEPADUI_DEFAULT_PANEL_SCHEME, "SchemePanel" );
    SetScheme( Scheme );
        
    static ConVarRef snd_fmod_musicsystem_playlist("snd_fmod_musicsystem_playlist");
    const char* playlist = snd_fmod_musicsystem_playlist.GetString();

    char playlistname[1024];

    // FindFirst ignores the pszPathID, so check it here
    // TODO: this doesn't find maps in fallback dirs
    Q_strncpy(playlistname, playlist + 18, sizeof(playlistname) - 1); // scripts/playlists/ = 18

    char* ext = Q_strstr(playlistname, ".txt");
    if (ext)
    {
        *ext = 0;
    }

    for (int i = Q_strlen(playlistname); i >= 0; --i)
    {
        playlistname[i] = toupper(playlistname[i]);
    }

    wchar_t wzplaylist[1024];

    g_pVGuiLocalize->ConvertANSIToUnicode(playlistname, wzplaylist, sizeof(wzplaylist));

    wchar_t string1[1024];
    g_pVGuiLocalize->ConstructString(string1, sizeof(string1), g_pVGuiLocalize->Find("#FR_Playlist_Title_Expanded"), 1, wzplaylist);

    GetFrameTitle() = GamepadUIString(string1);

    Activate();

    ScanPlaylists();

    if ( m_pPlaylistPanels.Count() )
        m_pPlaylistPanels[0]->NavigateTo();

    for ( int i = 1; i < m_pPlaylistPanels.Count(); i++ )
    {
        m_pPlaylistPanels[i]->SetNavUp( m_pPlaylistPanels[i - 1] );
        m_pPlaylistPanels[i - 1]->SetNavDown( m_pPlaylistPanels[i] );
    }

	UpdateGradients();
}

void GamepadUIPlaylistChooser::UpdateGradients()
{
	const float flTime = GamepadUI::GetInstance().GetTime();
	GamepadUI::GetInstance().GetGradientHelper()->ResetTargets( flTime );
	GamepadUI::GetInstance().GetGradientHelper()->SetTargetGradient( GradientSide::Up, { 1.0f, 1.0f }, flTime );
	GamepadUI::GetInstance().GetGradientHelper()->SetTargetGradient( GradientSide::Down, { 1.0f, 1.0f }, flTime );
}

void GamepadUIPlaylistChooser::OnThink()
{
	BaseClass::OnThink();

	LayoutPlaylistButtons();
}

void GamepadUIPlaylistChooser::Paint()
{
    BaseClass::Paint();
}

void GamepadUIPlaylistChooser::ScanPlaylists()
{
    FileFindHandle_t findHandle = NULL;

    int mapIndex = 0;
	const char *pszFilename = g_pFullFileSystem->FindFirst("scripts/playlists/*.txt", &findHandle);
	while (pszFilename)
	{
        char playlistname[256];

        // FindFirst ignores the pszPathID, so check it here
        // TODO: this doesn't find maps in fallback dirs
        Q_snprintf(playlistname, sizeof(playlistname), "scripts/playlists/%s", pszFilename);

        const char* str = Q_strstr(pszFilename, "scripts/playlists/");
        if (str)
        {
            Q_strncpy(playlistname, str + 20, sizeof(playlistname) - 1);	// scripts/playlists/ + // = 20
        }
        else
        {
            Q_strncpy(playlistname, pszFilename, sizeof(playlistname) - 1);
        }
        
        char* ext = Q_strstr(playlistname, ".txt");
        if (ext)
        {
            *ext = 0;
        }
        
        char command[256];
        Q_snprintf(command, sizeof(command), "load_list %s.txt", playlistname);
        
        char chapterName[256];
        Q_snprintf(chapterName, sizeof(chapterName), "%s", playlistname);
        
        //uppercase all the map names.
        for (int i = Q_strlen(chapterName); i >= 0; --i)
        {
            chapterName[i] = toupper(chapterName[i]);
        }

        wchar_t text[32];
        wchar_t* chapter = g_pVGuiLocalize->Find("#FR_Playlist_Label");
        _snwprintf(text, ARRAYSIZE(text), L"%ls", chapter ? chapter : L"Playlist");
        
        GamepadUIString strChapterName(chapterName);

        char chapterImage[64];
        Q_snprintf(chapterImage, sizeof(chapterImage), "gamepadui/playlists/%s.vmt", playlistname);

        IMaterial* pMaterial = g_pMaterialSystem->FindMaterial(chapterImage, TEXTURE_GROUP_VGUI);

        if (IsErrorMaterial(pMaterial))
        {
            Q_snprintf(chapterImage, sizeof(chapterImage), "gamepadui/maps/unknown.vmt");
        }

        GamepadUIPlaylistChooserButton* button = new GamepadUIPlaylistChooserButton(
            this, this,
            GAMEPADUI_RESOURCE_FOLDER "schemesavebutton.res", command,
            strChapterName.String(), text, chapterImage);
        button->SetEnabled(true);
        button->SetPriority(mapIndex);
        button->SetForwardToParent(true);

        m_pPlaylistPanels.AddToTail( button );
        mapIndex++;
		
		pszFilename = g_pFullFileSystem->FindNext(findHandle);
	}
	g_pFullFileSystem->FindClose(findHandle);
    
    SetFooterButtons( FooterButtons::Back | FooterButtons::Select , FooterButtons::Select);
    
	SetControlEnabled( "loadsave", false );
	SetControlEnabled( "delete", false );
}

void GamepadUIPlaylistChooser::OnGamepadUIButtonNavigatedTo( vgui::VPANEL button )
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

            int nTargetY = pButton->GetPriority() * ( pButton->m_flHeightAnimationValue[ButtonStates::Out] + m_flPlaylistSpacing );

            if ( nY < nParentH / 2 )
            {
                nTargetY += nParentH - m_flPlaylistOffsetY;
                // Add a bit of spacing to make this more visually appealing :)
                nTargetY -= m_flPlaylistSpacing;
            }
            else
            {
                nTargetY += pButton->m_flHeightAnimationValue[ButtonStates::Over];
                // Add a bit of spacing to make this more visually appealing :)
                nTargetY += (pButton->m_flHeightAnimationValue[ButtonStates::Over] / 2) + m_flPlaylistSpacing;
            }


            m_ScrollState.SetScrollTarget( nTargetY - ( nParentH - m_flPlaylistOffsetY), GamepadUI::GetInstance().GetTime() );
        }
    }
}

void GamepadUIPlaylistChooser::LayoutPlaylistButtons()
{
    int nParentW, nParentH;
	GetParent()->GetSize( nParentW, nParentH );

    float scrollClamp = 0.0f;
    for ( int i = 0; i < ( int )m_pPlaylistPanels.Count(); i++ )
    {
        int size = ( m_pPlaylistPanels[i]->GetTall() + m_flPlaylistSpacing );

        if ( i < ( ( int )m_pPlaylistPanels.Count() ) - 3 )
            scrollClamp += size;
    }

    m_ScrollState.UpdateScrollBounds( 0.0f, scrollClamp );

    int previousSizes = 0;
    for ( int i = 0; i < ( int )m_pPlaylistPanels.Count(); i++ )
    {
        int tall = m_pPlaylistPanels[i]->GetTall();
        int size = ( tall + m_flPlaylistSpacing );

        int y = m_flPlaylistOffsetY + previousSizes - m_ScrollState.GetScrollProgress();
        int fade = 255;
        if ( y < m_flPlaylistOffsetY )
            fade = ( 1.0f - clamp( -( y - m_flPlaylistOffsetY ) / m_flPlaylistFade, 0.0f, 1.0f ) ) * 255.0f;
        if ( y > nParentH - m_flPlaylistFade )
            fade = ( 1.0f - clamp( ( y - ( nParentH - m_flPlaylistFade - size ) ) / m_flPlaylistFade, 0.0f, 1.0f ) ) * 255.0f;
        if ( m_pPlaylistPanels[i]->HasFocus() && fade != 0 )
            fade = 255;
        m_pPlaylistPanels[i]->SetAlpha( fade );
        m_pPlaylistPanels[i]->SetPos( m_flPlaylistOffsetX, y );
        m_pPlaylistPanels[i]->SetVisible( true );
        previousSizes += size;
    }

    m_ScrollState.UpdateScrolling( 2.0f, GamepadUI::GetInstance().GetTime() );
}

void GamepadUIPlaylistChooser::OnCommand( char const* pCommand )
{
    if ( !V_strcmp( pCommand, "action_back" ) )
    {
        Close();
    }
	else if ( StringHasPrefixCaseSensitive( pCommand, "load_list " ) )
    {
        const char* pszPlaylist = pCommand + 10;
        
        if (*pszPlaylist)
        {
            char szPlaylistCommand[1024];

            // create the command to execute
            Q_snprintf(szPlaylistCommand, sizeof(szPlaylistCommand), "snd_fmod_musicsystem_playlist scripts/playlists/%s;snd_fmod_musicsystem_reload\n", pszPlaylist);

            // exec
            GamepadUI::GetInstance().GetEngineClient()->ClientCmd_Unrestricted(szPlaylistCommand);
            Close();
        }
    }
    else
    {
        BaseClass::OnCommand( pCommand );
    }
}

void GamepadUIPlaylistChooser::OnMouseWheeled( int delta )
{
    m_ScrollState.OnMouseWheeled(delta * 200.0f, GamepadUI::GetInstance().GetTime());
}

CON_COMMAND( gamepadui_openplaylistchooser, "" )
{
    new GamepadUIPlaylistChooser( GamepadUI::GetInstance().GetBasePanel(), "" );
}
