#ifndef MY_WINDOW_H
#define MY_WINDOW_H

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
    AlarmMode m_lastMode = AlarmMode::NUM_OF_ALARM_MODE;

    enum class ActiveWindow
    {
        NONE,
        MAIN,
        SETTINGS,
        MODE_SELECTOR
    };
    ActiveWindow m_activeWindow = ActiveWindow::NONE;

    void setupToolbar();
    void renderToolbar();
    void renderMenu();
    void renderContent();
};

#endif