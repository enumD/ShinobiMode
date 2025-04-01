#pragma once
#include <vector>
#include <string>
#include "imgui.h"
#include "../utils/AppSerializer.h"

class OptionScreen {
public:
    OptionScreen();
    void Draw(bool* isOpen);
    void LoadConfig();
    void SaveConfig();
    void SetVisible(bool visible) { m_visible = visible; }
    void Init();

private:
bool m_visible = false;
    std::vector<std::string> usbDevices;
    std::string selectedUsbDevice;
    AppSetting settings;
    std::string filename = "settings.dat";
    void checkAndCreateFile(const std::string& filename);
};