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

class GamepadUISpawnlistChooserButton;

class GamepadUISpawnlistChooser : public GamepadUIFrame
{
    DECLARE_CLASS_SIMPLE( GamepadUISpawnlistChooser, GamepadUIFrame );

public:
    GamepadUISpawnlistChooser( vgui::Panel *pParent, const char* pPanelName);

    void UpdateGradients();

	void OnThink() OVERRIDE;
    void Paint() OVERRIDE;
    void OnCommand( char const* pCommand ) OVERRIDE;
    void OnMouseWheeled( int nDelta ) OVERRIDE;

    MESSAGE_FUNC_HANDLE( OnGamepadUIButtonNavigatedTo, "OnGamepadUIButtonNavigatedTo", button );

private:
    void ScanSpawnlists();
    void LayoutSpawnlistButtons();

    CUtlVector<GamepadUISpawnlistChooserButton*> m_pSpawnlistPanels;

    GamepadUIScrollState m_ScrollState;

    GAMEPADUI_PANEL_PROPERTY( float, m_flSpawnlistFade, "Saves.Fade", "0", SchemeValueTypes::ProportionalFloat );
    GAMEPADUI_PANEL_PROPERTY( float, m_flSpawnlistOffsetX, "Saves.OffsetX", "0", SchemeValueTypes::ProportionalFloat );
    GAMEPADUI_PANEL_PROPERTY( float, m_flSpawnlistOffsetY, "Saves.OffsetY", "0", SchemeValueTypes::ProportionalFloat );
    GAMEPADUI_PANEL_PROPERTY( float, m_flSpawnlistSpacing, "Saves.Spacing", "0", SchemeValueTypes::ProportionalFloat );
};

class GamepadUISpawnlistChooserButton : public GamepadUIButton
{
public:
    DECLARE_CLASS_SIMPLE( GamepadUISpawnlistChooserButton, GamepadUIButton );

    GamepadUISpawnlistChooserButton( vgui::Panel* pParent, vgui::Panel* pActionSignalTarget, const char *pSchemeFile, const char* pCommand, const char* pText, const char* pDescription, const char *pChapterImage )
        : BaseClass( pParent, pActionSignalTarget, pSchemeFile, pCommand, pText, pDescription)
        , m_Image( pChapterImage )
    {
    }

    GamepadUISpawnlistChooserButton(vgui::Panel* pParent, vgui::Panel* pActionSignalTarget, const char* pSchemeFile, const char* pCommand, const wchar* pText, const wchar* pDescription, const char* pChapterImage)
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

GamepadUISpawnlistChooser::GamepadUISpawnlistChooser( vgui::Panel* pParent, const char* pPanelName)
	: BaseClass( pParent, pPanelName )
{
    vgui::HScheme Scheme = vgui::scheme()->LoadSchemeFromFile( GAMEPADUI_DEFAULT_PANEL_SCHEME, "SchemePanel" );
    SetScheme( Scheme );
        
    static ConVarRef sk_spawner_defaultspawnlist("sk_spawner_defaultspawnlist");
    const char* spawnlist = sk_spawner_defaultspawnlist.GetString();

    char spawnlistname[1024];

    // FindFirst ignores the pszPathID, so check it here
    // TODO: this doesn't find maps in fallback dirs
    Q_strncpy(spawnlistname, spawnlist + 19, sizeof(spawnlistname) - 1); // scripts/spawnlists/ = 19

    char* ext = Q_strstr(spawnlistname, ".txt");
    if (ext)
    {
        *ext = 0;
    }

    for (int i = Q_strlen(spawnlistname); i >= 0; --i)
    {
        spawnlistname[i] = toupper(spawnlistname[i]);
    }

    wchar_t wzspawnlist[1024];

    g_pVGuiLocalize->ConvertANSIToUnicode(spawnlistname, wzspawnlist, sizeof(wzspawnlist));

    wchar_t string1[1024];
    g_pVGuiLocalize->ConstructString(string1, sizeof(string1), g_pVGuiLocalize->Find("#FR_Spawnlist_Title_Expanded"), 1, wzspawnlist);

    GetFrameTitle() = GamepadUIString(string1);

    Activate();

    ScanSpawnlists();

    if ( m_pSpawnlistPanels.Count() )
        m_pSpawnlistPanels[0]->NavigateTo();

    for ( int i = 1; i < m_pSpawnlistPanels.Count(); i++ )
    {
        m_pSpawnlistPanels[i]->SetNavUp( m_pSpawnlistPanels[i - 1] );
        m_pSpawnlistPanels[i - 1]->SetNavDown( m_pSpawnlistPanels[i] );
    }

	UpdateGradients();
}

void GamepadUISpawnlistChooser::UpdateGradients()
{
	const float flTime = GamepadUI::GetInstance().GetTime();
	GamepadUI::GetInstance().GetGradientHelper()->ResetTargets( flTime );
	GamepadUI::GetInstance().GetGradientHelper()->SetTargetGradient( GradientSide::Up, { 1.0f, 1.0f }, flTime );
	GamepadUI::GetInstance().GetGradientHelper()->SetTargetGradient( GradientSide::Down, { 1.0f, 1.0f }, flTime );
}

void GamepadUISpawnlistChooser::OnThink()
{
	BaseClass::OnThink();

	LayoutSpawnlistButtons();
}

void GamepadUISpawnlistChooser::Paint()
{
    BaseClass::Paint();
}

void GamepadUISpawnlistChooser::ScanSpawnlists()
{
    FileFindHandle_t findHandle = NULL;

    int mapIndex = 0;
	const char *pszFilename = g_pFullFileSystem->FindFirst("scripts/spawnlists/*.txt", &findHandle);
	while (pszFilename)
	{
        char spawnlistname[256];

        // FindFirst ignores the pszPathID, so check it here
        // TODO: this doesn't find maps in fallback dirs
        Q_snprintf(spawnlistname, sizeof(spawnlistname), "scripts/spawnlists/%s", pszFilename);

        const char* str = Q_strstr(pszFilename, "scripts/spawnlists/");
        if (str)
        {
            Q_strncpy(spawnlistname, str + 21, sizeof(spawnlistname) - 1);	// scripts/spawnlists/ + // = 21
        }
        else
        {
            Q_strncpy(spawnlistname, pszFilename, sizeof(spawnlistname) - 1);
        }
        
        char* ext = Q_strstr(spawnlistname, ".txt");
        if (ext)
        {
            *ext = 0;
        }
        
        char command[256];
        Q_snprintf(command, sizeof(command), "load_list %s.txt", spawnlistname);
        
        char chapterName[256];
        Q_snprintf(chapterName, sizeof(chapterName), "%s", spawnlistname);
        
        //uppercase all the map names.
        for (int i = Q_strlen(chapterName); i >= 0; --i)
        {
            chapterName[i] = toupper(chapterName[i]);
        }

        wchar_t text[32];
        wchar_t* chapter = g_pVGuiLocalize->Find("#FR_Spawnlist_Label");
        _snwprintf(text, ARRAYSIZE(text), L"%ls", chapter ? chapter : L"Spawnlist");
        
        GamepadUIString strChapterName(chapterName);

        char chapterImage[64];
        Q_snprintf(chapterImage, sizeof(chapterImage), "gamepadui/spawnlists/%s.vmt", spawnlistname);

        IMaterial* pMaterial = g_pMaterialSystem->FindMaterial(chapterImage, TEXTURE_GROUP_VGUI);

        if (IsErrorMaterial(pMaterial))
        {
            Q_snprintf(chapterImage, sizeof(chapterImage), "gamepadui/maps/unknown.vmt");
        }

        GamepadUISpawnlistChooserButton* button = new GamepadUISpawnlistChooserButton(
            this, this,
            GAMEPADUI_RESOURCE_FOLDER "schemesavebutton.res", command,
            strChapterName.String(), text, chapterImage);
        button->SetEnabled(true);
        button->SetPriority(mapIndex);
        button->SetForwardToParent(true);

        m_pSpawnlistPanels.AddToTail( button );
        mapIndex++;
		
		pszFilename = g_pFullFileSystem->FindNext(findHandle);
	}
	g_pFullFileSystem->FindClose(findHandle);
    
    SetFooterButtons( FooterButtons::Back | FooterButtons::Select /*, FooterButtons::Select*/);
    
	SetControlEnabled( "loadsave", false );
	SetControlEnabled( "delete", false );
}

void GamepadUISpawnlistChooser::OnGamepadUIButtonNavigatedTo( vgui::VPANEL button )
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

            int nTargetY = pButton->GetPriority() * ( pButton->m_flHeightAnimationValue[ButtonStates::Out] + m_flSpawnlistSpacing );

            if ( nY < nParentH / 2 )
            {
                nTargetY += nParentH - m_flSpawnlistOffsetY;
                // Add a bit of spacing to make this more visually appealing :)
                nTargetY -= m_flSpawnlistSpacing;
            }
            else
            {
                nTargetY += pButton->m_flHeightAnimationValue[ButtonStates::Over];
                // Add a bit of spacing to make this more visually appealing :)
                nTargetY += (pButton->m_flHeightAnimationValue[ButtonStates::Over] / 2) + m_flSpawnlistSpacing;
            }


            m_ScrollState.SetScrollTarget( nTargetY - ( nParentH - m_flSpawnlistOffsetY), GamepadUI::GetInstance().GetTime() );
        }
    }
}

void GamepadUISpawnlistChooser::LayoutSpawnlistButtons()
{
    int nParentW, nParentH;
	GetParent()->GetSize( nParentW, nParentH );

    float scrollClamp = 0.0f;
    for ( int i = 0; i < ( int )m_pSpawnlistPanels.Count(); i++ )
    {
        int size = ( m_pSpawnlistPanels[i]->GetTall() + m_flSpawnlistSpacing );

        if ( i < ( ( int )m_pSpawnlistPanels.Count() ) - 3 )
            scrollClamp += size;
    }

    m_ScrollState.UpdateScrollBounds( 0.0f, scrollClamp );

    int previousSizes = 0;
    for ( int i = 0; i < ( int )m_pSpawnlistPanels.Count(); i++ )
    {
        int tall = m_pSpawnlistPanels[i]->GetTall();
        int size = ( tall + m_flSpawnlistSpacing );

        int y = m_flSpawnlistOffsetY + previousSizes - m_ScrollState.GetScrollProgress();
        int fade = 255;
        if ( y < m_flSpawnlistOffsetY )
            fade = ( 1.0f - clamp( -( y - m_flSpawnlistOffsetY ) / m_flSpawnlistFade, 0.0f, 1.0f ) ) * 255.0f;
        if ( y > nParentH - m_flSpawnlistFade )
            fade = ( 1.0f - clamp( ( y - ( nParentH - m_flSpawnlistFade - size ) ) / m_flSpawnlistFade, 0.0f, 1.0f ) ) * 255.0f;
        if ( m_pSpawnlistPanels[i]->HasFocus() && fade != 0 )
            fade = 255;
        m_pSpawnlistPanels[i]->SetAlpha( fade );
        m_pSpawnlistPanels[i]->SetPos( m_flSpawnlistOffsetX, y );
        m_pSpawnlistPanels[i]->SetVisible( true );
        previousSizes += size;
    }

    m_ScrollState.UpdateScrolling( 2.0f, GamepadUI::GetInstance().GetTime() );
}

void GamepadUISpawnlistChooser::OnCommand( char const* pCommand )
{
    if ( !V_strcmp( pCommand, "action_back" ) )
    {
        Close();
    }
	else if ( StringHasPrefixCaseSensitive( pCommand, "load_list " ) )
    {
        const char* pszSpawnlist = pCommand + 10;
        
        if (*pszSpawnlist)
        {
            char szSpawnlistCommand[1024];

            // create the command to execute
            Q_snprintf(szSpawnlistCommand, sizeof(szSpawnlistCommand), "sk_spawner_defaultspawnlist scripts/spawnlists/%s\n", pszSpawnlist);

            // exec
            GamepadUI::GetInstance().GetEngineClient()->ClientCmd_Unrestricted(szSpawnlistCommand);
            Close();
        }
    }
    else
    {
        BaseClass::OnCommand( pCommand );
    }
}

void GamepadUISpawnlistChooser::OnMouseWheeled( int delta )
{
    m_ScrollState.OnMouseWheeled(delta * 200.0f, GamepadUI::GetInstance().GetTime());
}

CON_COMMAND( gamepadui_openspawnlistchooser, "" )
{
    new GamepadUISpawnlistChooser( GamepadUI::GetInstance().GetBasePanel(), "" );
}
