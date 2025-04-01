#pragma once
#include "Toolbar.h"
#include "MainWindow.h"
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
    MainWindow m_mainWindow;
    SettingsWindow m_settingsWindow;
    
    enum class ActiveWindow { NONE, MAIN, SETTINGS };
    ActiveWindow m_activeWindow = ActiveWindow::NONE;
    
    void setupToolbar();
  
    void renderMenu();
    void renderContent();
};