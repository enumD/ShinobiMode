#include "OptionsScreen.h"

OptionsScreen::OptionsScreen() {
    usbDevices = {"/dev/video0", "/dev/video1", "/dev/video2"};
    strncpy(wifiSsid, "", sizeof(wifiSsid));
    strncpy(wifiPsw, "", sizeof(wifiPsw));
    LoadConfig();
}

void OptionsScreen::Draw(bool* isOpen) {
    if (!*isOpen) return;

    ImGui::Begin("Opzioni", isOpen, ImGuiWindowFlags_AlwaysAutoResize);

    if (ImGui::BeginCombo("Webcam USB", selectedUsbDevice.c_str())) {
        for (const auto& device : usbDevices) {
            if (ImGui::Selectable(device.c_str(), device == selectedUsbDevice)) {
                selectedUsbDevice = device;
            }
        }
        ImGui::EndCombo();
    }

    ImGui::InputText("SSID Wi-Fi", wifiSsid, sizeof(wifiSsid));
    ImGui::InputText("Password Wi-Fi", wifiPsw, sizeof(wifiPsw), ImGuiInputTextFlags_Password);

    if (ImGui::Button("Salva")) {
        SaveConfig();
        *isOpen = false;
    }

    ImGui::End();
}

void OptionsScreen::LoadConfig() {
    ConfigManager config;
    json data = config.Load("config.json");
    selectedUsbDevice = data.value("webcam", "/dev/video0");
    strncpy(wifiSsid, data.value("wifi_ssid", "").c_str(), sizeof(wifiSsid));
    strncpy(wifiPsw, data.value("wifi_psw", "").c_str(), sizeof(wifiPsw));
}

void OptionsScreen::SaveConfig() {
    ConfigManager config;
    json data;
    data["webcam"] = selectedUsbDevice;
    data["wifi_ssid"] = wifiSsid;
    data["wifi_psw"] = wifiPsw;
    config.Save("config.json", data);
}