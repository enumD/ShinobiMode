#pragma once
#include <vector>
#include <string>
#include "imgui.h"
#include "utils/ConfigManager.h"

class OptionsScreen {
public:
    OptionsScreen();
    void Draw(bool* isOpen);
    void LoadConfig();
    void SaveConfig();

private:
    std::vector<std::string> usbDevices;
    std::string selectedUsbDevice;
    char wifiSsid[128];
    char wifiPsw[128];
};