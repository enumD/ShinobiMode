#pragma once
#include "AppSetting.h"
#include "imgui.h"

class SettingsWindow {
public:
    void initialize();
    void render();
    void shutdown();
    void saveSettings();
    AppSetting m_settings;
    
private:
    bool m_visible = false;
    char m_wifiSSID[128] = "";
    char m_wifiPassword[128] = "";
    std::string m_filename = "settings.dat";
};