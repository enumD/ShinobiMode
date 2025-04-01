#pragma once
#include "imgui.h"

class SettingsWindow {
public:
    void initialize();
    void render();
    void shutdown();
    
private:
    bool m_visible = false;
    char m_wifiSSID[128] = "";
    char m_wifiPassword[128] = "";
};