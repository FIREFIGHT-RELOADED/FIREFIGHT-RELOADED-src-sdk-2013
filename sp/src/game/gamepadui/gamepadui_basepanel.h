#ifndef GAMEPADUI_BASEPANEL_H
#define GAMEPADUI_BASEPANEL_H
#ifdef _WIN32
#pragma once
#endif

#include "gamepadui_interface.h"

class GamepadUIMainMenu;

class GamepadUIBasePanel : public vgui::Panel
{
    DECLARE_CLASS_SIMPLE( GamepadUIBasePanel, vgui::Panel );
public:
    GamepadUIBasePanel( vgui::VPANEL parent );

    void ApplySchemeSettings( vgui::IScheme* pScheme ) OVERRIDE;

    GamepadUIMainMenu *GetMainMenuPanel() const;

    void OnMenuStateChanged();

    bool StartBackgroundMusic( float flVolume );
    void ReleaseBackgroundMusic();

private:
    GamepadUIMainMenu *m_pMainMenu = NULL;

    bool m_bBackgroundMusicEnabled;

    FMOD::Channel* m_pChannel;
    FMOD::Sound* m_pSound;
};

#endif // GAMEPADUI_BASEPANEL_H
