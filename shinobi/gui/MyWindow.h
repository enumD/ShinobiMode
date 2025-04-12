#pragma once
#include "CameraWindow.h"
#include "ModeSelector.h"
#include "SettingWindow.h"
#include "Toolbar.h"
#include <memory>
#include <vector>

class MyWindow
{
  public:
    void initialize();
    void render();
    void shutdown();

  private:
    // WINDOWs
    Toolbar m_toolbar;
    CameraWindow m_cameraWindow;
    SettingsWindow m_settingsWindow;
    ModeSelector m_modeSelector;

    enum class ActiveWindow
    {
        NONE,
        MAIN,
        SETTINGS,
        MODE_SELECTOR
    };
    ActiveWindow m_activeWindow = ActiveWindow::NONE;

    void setupToolbar();

    void renderMenu();
    void renderContent();
};