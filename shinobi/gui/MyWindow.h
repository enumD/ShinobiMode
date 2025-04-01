#pragma once
#include "Toolbar.h"
#include "CameraWindow.h"
#include "SettingWindow.h"
#include <vector>
#include <memory>

class MyWindow {
public:

    void initialize();
    void render();
    void shutdown();

private:
    Toolbar m_toolbar;
    CameraWindow m_mainWindow;
    SettingsWindow m_settingsWindow;
    
    enum class ActiveWindow { NONE, MAIN, SETTINGS };
    ActiveWindow m_activeWindow = ActiveWindow::NONE;
    
    void setupToolbar();
  
    void renderMenu();
    void renderContent();
};