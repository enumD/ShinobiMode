#include "SettingWindow.h"
#include "AppSerializer.h"


void SettingsWindow::initialize() {
    // Initialize configuration values
    m_settings = AppSerializer::deserialize(m_filename);
}


void SettingsWindow::render() {
    // Create a child window that occupies the entire available space
    ImGui::BeginChild("SettingsView", ImVec2(0, 0), true);

    // Add a title for the settings window
    ImGui::Text("Application Settings");
    ImGui::Separator();
    
    // Add an editable text field for the WiFi SSID
    ImGui::InputText("WiFi ID", m_wifiSSID, sizeof(m_wifiSSID));

    // Add an editable text field for the WiFi password (with password masking)
    ImGui::InputText("WiFi Password", m_wifiPassword, sizeof(m_wifiPassword), ImGuiInputTextFlags_Password);

    // Example volume control, you can remove this if not needed
    static float volume = 0.5f;
    ImGui::SliderFloat("Volume", &volume, 0.0f, 1.0f);

    // Add a 'Save' button that triggers a function when pressed
    if (ImGui::Button("Save")) {
        this->saveSettings();  // Call the function that will save the settings
    }

    // End of child window
    ImGui::EndChild();
}


void SettingsWindow::saveSettings()
{

}


void SettingsWindow::shutdown() {
    // Cleanup if needed
}