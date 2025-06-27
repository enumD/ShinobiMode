#ifndef SETTING_WINDOW_H
#define SETTING_WINDOW_H

#include "../utils/AppSetting.h"
#include "../utils/Audio.h"
#include "../utils/Notification.h"
#include "imgui.h"
#include <filesystem>
#include <fstream>
#include <iostream>

class SettingsWindow
{
  public:
    void initialize();
    void render();
    void shutdown();


  private:
    bool m_visible = false;
    std::string m_filename = "settings.dat";
    AppSetting m_settings;


    void checkAndCreateFile(const std::string &filename);
    void saveConfig();
    void setAudio();
    void loadConfig();
};

#endif