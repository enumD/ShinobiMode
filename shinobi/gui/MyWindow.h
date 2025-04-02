#pragma once
#include "Toolbar.h"
#include "CameraWindow.h"
#include "SettingWindow.h"
#include "ModeSelector.h"
#include <vector>
#include <memory>

class MyWindow {
public:

    void initialize();
    void render();
    void shutdown();

private:
    // WINDOWs
    Toolbar m_toolbar;
    CameraWindow m_mainWindow;
    SettingsWindow m_settingsWindow;
    ModeSelector m_modeSelector;
    
    enum class ActiveWindow { NONE, MAIN, SETTINGS, MODE_SELECTOR };
    ActiveWindow m_activeWindow = ActiveWindow::NONE;
    
    void setupToolbar();
  
    void renderMenu();
    void renderContent();
};