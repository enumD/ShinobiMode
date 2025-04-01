#include "SettingWindow.h"
#include "../utils/AppSerializer.h"


void SettingsWindow::initialize() {
    // Initialize configuration values
    m_settings = AppSerializer::deserialize(m_filename);
}


/// @brief Render func called in mainloop
void SettingsWindow::render() 
{
    // Create a child window that occupies the entire available space
    ImGui::BeginChild("SettingsView", ImVec2(0, 0), true);

    // Add a title for the settings window
    ImGui::Text("Application Settings");
    ImGui::Separator();

    // Bind the WiFi ID directly to m_appSetting.m_wifi_id
    ImGui::InputText("WiFi ID", &m_settings.m_wifi_id[0], m_settings.m_wifi_id.size() + 1);

    // Bind the WiFi password directly to m_appSetting.m_wifi_pswd
    ImGui::InputText("WiFi Password", &m_settings.m_wifi_pswd[0], m_settings.m_wifi_pswd.size() + 1, ImGuiInputTextFlags_Password);

    // Example volume control
    ImGui::SliderInt("Volume", &m_settings.m_volume, 0, 100);

    // Add a 'Save' button that triggers a function when pressed
    if (ImGui::Button("Save")) {
        this->saveConfig();  // Call the function that will save the settings
        Audio audio;
        audio.setVolume(m_settings.m_volume);
        Notification::Show("Settings saved successfully", Notification::Level::SUCCESS);
    }

    // End of child window
    ImGui::EndChild();
}


void SettingsWindow::saveConfig()
{
    try
    {
        checkAndCreateFile(m_filename);
        AppSerializer::serialize(this->m_settings, m_filename);
        std::cerr << "Config saved" << '\n';
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}

void SettingsWindow::setAudio()
{


}

void SettingsWindow::loadConfig()
{
    try
    {
        m_settings = AppSerializer::deserialize(m_filename);
        std::cerr << "Setting loaded from file"  << std::endl;
    }
    catch (const std::exception &e)
    {
          std::cerr << "Errore: " << e.what() << std::endl;
    }
}


void SettingsWindow::checkAndCreateFile(const std::string& filename)
{
    if (!std::filesystem::exists(m_filename)) 
    {  // Controlla se il file esiste
        std::ofstream file(filename); // Crea il file
        if (file) 
        {
            std::cerr << "Setting file created" << std::endl;
            AppSetting setDef;
            AppSerializer::serialize(setDef, filename);
        } 
        else 
        {
            std::cerr << "Cannot create setting file" << std::endl;
        }
    }
}


void SettingsWindow::shutdown() 
{
    // Cleanup if needed
}