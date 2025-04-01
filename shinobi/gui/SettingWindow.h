#pragma once
#include "../utils/AppSetting.h"
#include "imgui.h"
#include <iostream>
#include <filesystem>
#include <fstream>
#include "../utils/Notification.h"
#include "../utils/Audio.h"

class SettingsWindow {
public:
    void initialize();
    void render();
    void shutdown();

    
private:
    bool m_visible = false;
    std::string m_filename = "settings.dat";
    AppSetting m_settings;
    
    
    void checkAndCreateFile(const std::string& filename);
    void saveConfig();
    void setAudio();
    void loadConfig();
};