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

class GamepadUIPlayerModelChooserButton;

class GamepadUIPlayerModelChooser : public GamepadUIFrame
{
    DECLARE_CLASS_SIMPLE( GamepadUIPlayerModelChooser, GamepadUIFrame );

public:
    GamepadUIPlayerModelChooser( vgui::Panel *pParent, const char* pPanelName);

    void UpdateGradients();

	void OnThink() OVERRIDE;
    void Paint() OVERRIDE;
    void OnCommand( char const* pCommand ) OVERRIDE;
    void OnMouseWheeled( int nDelta ) OVERRIDE;

    MESSAGE_FUNC_HANDLE( OnGamepadUIButtonNavigatedTo, "OnGamepadUIButtonNavigatedTo", button );

private:
    void ScanModels();
    void LayoutModelButtons();

    CUtlVector<GamepadUIPlayerModelChooserButton*> m_pModelPanels;

    GamepadUIScrollState m_ScrollState;

    GAMEPADUI_PANEL_PROPERTY( float, m_flModelFade, "Saves.Fade", "0", SchemeValueTypes::ProportionalFloat );
    GAMEPADUI_PANEL_PROPERTY( float, m_flModelOffsetX, "Saves.OffsetX", "0", SchemeValueTypes::ProportionalFloat );
    GAMEPADUI_PANEL_PROPERTY( float, m_flModelOffsetY, "Saves.OffsetY", "0", SchemeValueTypes::ProportionalFloat );
    GAMEPADUI_PANEL_PROPERTY( float, m_flModelSpacing, "Saves.Spacing", "0", SchemeValueTypes::ProportionalFloat );
};

class GamepadUIPlayerModelChooserButton : public GamepadUIButton
{
public:
    DECLARE_CLASS_SIMPLE( GamepadUIPlayerModelChooserButton, GamepadUIButton );

    GamepadUIPlayerModelChooserButton( vgui::Panel* pParent, vgui::Panel* pActionSignalTarget, const char *pSchemeFile, const char* pCommand, const char* pText, const char* pDescription, const char *pChapterImage )
        : BaseClass( pParent, pActionSignalTarget, pSchemeFile, pCommand, pText, pDescription)
        , m_Image( pChapterImage )
    {
    }

    GamepadUIPlayerModelChooserButton(vgui::Panel* pParent, vgui::Panel* pActionSignalTarget, const char* pSchemeFile, const char* pCommand, const wchar* pText, const wchar* pDescription, const char* pChapterImage)
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

GamepadUIPlayerModelChooser::GamepadUIPlayerModelChooser( vgui::Panel* pParent, const char* pPanelName)
	: BaseClass( pParent, pPanelName )
{
    vgui::HScheme Scheme = vgui::scheme()->LoadSchemeFromFile( GAMEPADUI_DEFAULT_PANEL_SCHEME, "SchemePanel" );
    SetScheme( Scheme );
        
    static ConVarRef cl_playermodel("cl_playermodel");
    const char* playermodel = cl_playermodel.GetString();

    char modelname[1024];

    // FindFirst ignores the pszPathID, so check it here
    // TODO: this doesn't find maps in fallback dirs
    Q_strncpy(modelname, playermodel + 27, sizeof(modelname) - 1); // models/player/playermodels/ = 27

    char* ext = Q_strstr(modelname, ".mdl");
    if (ext)
    {
        *ext = 0;
    }

    for (int i = Q_strlen(modelname); i >= 0; --i)
    {
        modelname[i] = toupper(modelname[i]);
    }

    wchar_t wzplayermodel[1024];

    g_pVGuiLocalize->ConvertANSIToUnicode(modelname, wzplayermodel, sizeof(wzplayermodel));

    wchar_t string1[1024];
    g_pVGuiLocalize->ConstructString(string1, sizeof(string1), g_pVGuiLocalize->Find("#FRMP_PlayerModel_Title_Expanded"), 1, wzplayermodel);

    GetFrameTitle() = GamepadUIString(string1);

    Activate();

    ScanModels();

    if ( m_pModelPanels.Count() )
        m_pModelPanels[0]->NavigateTo();

    for ( int i = 1; i < m_pModelPanels.Count(); i++ )
    {
        m_pModelPanels[i]->SetNavUp( m_pModelPanels[i - 1] );
        m_pModelPanels[i - 1]->SetNavDown( m_pModelPanels[i] );
    }

	UpdateGradients();
}

void GamepadUIPlayerModelChooser::UpdateGradients()
{
	const float flTime = GamepadUI::GetInstance().GetTime();
	GamepadUI::GetInstance().GetGradientHelper()->ResetTargets( flTime );
	GamepadUI::GetInstance().GetGradientHelper()->SetTargetGradient( GradientSide::Up, { 1.0f, 1.0f }, flTime );
	GamepadUI::GetInstance().GetGradientHelper()->SetTargetGradient( GradientSide::Down, { 1.0f, 1.0f }, flTime );
}

void GamepadUIPlayerModelChooser::OnThink()
{
	BaseClass::OnThink();

	LayoutModelButtons();
}

void GamepadUIPlayerModelChooser::Paint()
{
    BaseClass::Paint();
}

void GamepadUIPlayerModelChooser::ScanModels()
{
    FileFindHandle_t findHandle = NULL;

    int mapIndex = 0;
	const char *pszFilename = g_pFullFileSystem->FindFirst("models/player/playermodels/*.mdl", &findHandle);
	while (pszFilename)
	{
        char modelname[256];

        // FindFirst ignores the pszPathID, so check it here
        // TODO: this doesn't find maps in fallback dirs
        Q_snprintf(modelname, sizeof(modelname), "models/player/playermodels/%s", pszFilename);

        const char* str = Q_strstr(pszFilename, "models/player/playermodels/");
        if (str)
        {
            Q_strncpy(modelname, str + 31, sizeof(modelname) - 1);	// models/player/playermodels/ + // = 31
        }
        else
        {
            Q_strncpy(modelname, pszFilename, sizeof(modelname) - 1);
        }
        
        char command[256];
        Q_snprintf(command, sizeof(command), "load_model %s", modelname);

        char* ext = Q_strstr(modelname, ".mdl");
        if (ext)
        {
            *ext = 0;
        }
        
        char chapterName[256];
        Q_snprintf(chapterName, sizeof(chapterName), "%s", modelname);
        
        //uppercase all the map names.
        for (int i = Q_strlen(chapterName); i >= 0; --i)
        {
            chapterName[i] = toupper(chapterName[i]);
        }

        wchar_t text[32];
        wchar_t* chapter = g_pVGuiLocalize->Find("#FRMP_PlayerModel_Label");
        _snwprintf(text, ARRAYSIZE(text), L"%ls", chapter ? chapter : L"Model");
        
        GamepadUIString strChapterName(chapterName);

        char chapterImage[64];
        Q_snprintf(chapterImage, sizeof(chapterImage), "gamepadui/playermodels/%s.vmt", modelname);

        IMaterial* pMaterial = g_pMaterialSystem->FindMaterial(chapterImage, TEXTURE_GROUP_VGUI);

        if (IsErrorMaterial(pMaterial))
        {
            Q_snprintf(chapterImage, sizeof(chapterImage), "gamepadui/maps/unknown.vmt");
        }

        GamepadUIPlayerModelChooserButton* button = new GamepadUIPlayerModelChooserButton(
            this, this,
            GAMEPADUI_RESOURCE_FOLDER "schemesavebutton.res", command,
            strChapterName.String(), text, chapterImage);
        button->SetEnabled(true);
        button->SetPriority(mapIndex);
        button->SetForwardToParent(true);

        m_pModelPanels.AddToTail( button );
        mapIndex++;
		
		pszFilename = g_pFullFileSystem->FindNext(findHandle);
	}
	g_pFullFileSystem->FindClose(findHandle);
    
    SetFooterButtons( FooterButtons::Back | FooterButtons::Select , FooterButtons::Select);
    
	SetControlEnabled( "loadsave", false );
	SetControlEnabled( "delete", false );
}

void GamepadUIPlayerModelChooser::OnGamepadUIButtonNavigatedTo( vgui::VPANEL button )
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

            int nTargetY = pButton->GetPriority() * ( pButton->m_flHeightAnimationValue[ButtonStates::Out] + m_flModelSpacing );

            if ( nY < nParentH / 2 )
            {
                nTargetY += nParentH - m_flModelOffsetY;
                // Add a bit of spacing to make this more visually appealing :)
                nTargetY -= m_flModelSpacing;
            }
            else
            {
                nTargetY += pButton->m_flHeightAnimationValue[ButtonStates::Over];
                // Add a bit of spacing to make this more visually appealing :)
                nTargetY += (pButton->m_flHeightAnimationValue[ButtonStates::Over] / 2) + m_flModelSpacing;
            }


            m_ScrollState.SetScrollTarget( nTargetY - ( nParentH - m_flModelOffsetY), GamepadUI::GetInstance().GetTime() );
        }
    }
}

void GamepadUIPlayerModelChooser::LayoutModelButtons()
{
    int nParentW, nParentH;
	GetParent()->GetSize( nParentW, nParentH );

    float scrollClamp = 0.0f;
    for ( int i = 0; i < ( int )m_pModelPanels.Count(); i++ )
    {
        int size = ( m_pModelPanels[i]->GetTall() + m_flModelSpacing );

        if ( i < ( ( int )m_pModelPanels.Count() ) - 3 )
            scrollClamp += size;
    }

    m_ScrollState.UpdateScrollBounds( 0.0f, scrollClamp );

    int previousSizes = 0;
    for ( int i = 0; i < ( int )m_pModelPanels.Count(); i++ )
    {
        int tall = m_pModelPanels[i]->GetTall();
        int size = ( tall + m_flModelSpacing );

        int y = m_flModelOffsetY + previousSizes - m_ScrollState.GetScrollProgress();
        int fade = 255;
        if ( y < m_flModelOffsetY )
            fade = ( 1.0f - clamp( -( y - m_flModelOffsetY ) / m_flModelFade, 0.0f, 1.0f ) ) * 255.0f;
        if ( y > nParentH - m_flModelFade )
            fade = ( 1.0f - clamp( ( y - ( nParentH - m_flModelFade - size ) ) / m_flModelFade, 0.0f, 1.0f ) ) * 255.0f;
        if ( m_pModelPanels[i]->HasFocus() && fade != 0 )
            fade = 255;
        m_pModelPanels[i]->SetAlpha( fade );
        m_pModelPanels[i]->SetPos( m_flModelOffsetX, y );
        m_pModelPanels[i]->SetVisible( true );
        previousSizes += size;
    }

    m_ScrollState.UpdateScrolling( 2.0f, GamepadUI::GetInstance().GetTime() );
}

bool CheckIfActorInstalled(const char *pName)
{
    const char* pszModelCopy = pName;

    char* ext = Q_strstr(pszModelCopy, ".mdl");
    if (ext)
    {
        *ext = 0;
    }

    KeyValues* m_pActor = new KeyValues("globalactors");
    const char* path = "scripts/global_actors.txt";

    if (m_pActor->LoadFromFile(g_pFullFileSystem, path))
    {
        const char* actorName = m_pActor->GetString(pszModelCopy, "");
        if (!actorName[0])
        {
            const char* gender = "";
            if (Q_stristr(pszModelCopy, "female"))
            {
                gender = "female";
            }
            else if (Q_stristr(pszModelCopy, "male"))
            {
                gender = "male";
            }
            else
            {
                int iRandomGender = RandomInt(0, 1);
                gender = (iRandomGender == 1) ? "female" : "male";
            }

            if (gender)
            {
                m_pActor->SetString(pszModelCopy, gender);
                if (m_pActor->SaveToFile(g_pFullFileSystem, path))
                {
                    Msg("%s saved as a %s actor. Look in 'scripts/global_actors.txt' if you wish to change it.\n", pszModelCopy, gender);
                    return true;
                }
            }
        }
    }

    return false;
}

void GamepadUIPlayerModelChooser::OnCommand( char const* pCommand )
{
    if ( !V_strcmp( pCommand, "action_back" ) )
    {
        Close();
    }
	else if ( StringHasPrefixCaseSensitive( pCommand, "load_model " ) )
    {
        const char* pszModel = pCommand + 11;
        
        if (*pszModel)
        {
            char szModelCommand[1024];

            // create the command to execute
            Q_snprintf(szModelCommand, sizeof(szModelCommand), "cl_playermodel models/player/playermodels/%s\n", pszModel);

            // exec
            GamepadUI::GetInstance().GetEngineClient()->ClientCmd_Unrestricted(szModelCommand);

            CheckIfActorInstalled(pszModel);

            Close();
        }
    }
    else
    {
        BaseClass::OnCommand( pCommand );
    }
}

void GamepadUIPlayerModelChooser::OnMouseWheeled( int delta )
{
    m_ScrollState.OnMouseWheeled(delta * 200.0f, GamepadUI::GetInstance().GetTime());
}

CON_COMMAND( gamepadui_openmodelchooser, "" )
{
    new GamepadUIPlayerModelChooser( GamepadUI::GetInstance().GetBasePanel(), "" );
}
