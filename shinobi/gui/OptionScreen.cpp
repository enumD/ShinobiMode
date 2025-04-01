#include "OptionScreen.h"
#include "../utils/MessageBox.h"
#include <iostream>
#include <filesystem>
#include <fstream>

OptionScreen::OptionScreen()
{
  
}

void OptionScreen::Init()
{
    usbDevices = {"/dev/video0", "/dev/video1", "/dev/video2"};
    checkAndCreateFile(this->filename);
    LoadConfig();
}

void OptionScreen::Draw(bool *isOpen)
{
    if (!*isOpen)
        return;

    ImGui::Begin("Settings", isOpen, ImGuiWindowFlags_AlwaysAutoResize);

    if (ImGui::BeginCombo("Webcam USB", selectedUsbDevice.c_str()))
    {
        for (const auto &device : usbDevices)
        {
            if (ImGui::Selectable(device.c_str(), device == selectedUsbDevice))
            {
                selectedUsbDevice = device;
            }
        }
        ImGui::EndCombo();
    }


    ImGui::InputText("SSID Wi-Fi", this->settings.m_wifi_id.data() , this->settings.m_wifi_id.size()+1);
    ImGui::InputText("Psw Wifi", this->settings.m_wifi_pswd.data(), this->settings.m_wifi_pswd.size()+1, ImGuiInputTextFlags_Password);

    if (ImGui::Button("Save"))
    {
        SaveConfig();
        *isOpen = false;
    }

    ImGui::End();
}

 void OptionScreen::checkAndCreateFile(const std::string& filename) 
{
    if (!std::filesystem::exists(filename)) 
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



void OptionScreen::LoadConfig()
{
    try
    {
        AppSetting loadedSettings = AppSerializer::deserialize(filename);

        // Todo copy constructor or copy object
        this->settings.m_wifi_id = loadedSettings.m_wifi_id;
        this->settings.m_wifi_pswd = loadedSettings.m_wifi_pswd;
        std::cerr << "Load Setting"  << std::endl;
    }
    catch (const std::exception &e)
    {
          std::cerr << "Errore: " << e.what() << std::endl;
    }
}

void OptionScreen::SaveConfig()
{
    try
    {
        checkAndCreateFile(filename);
        MessageBox::Show("Config Saved" );
        AppSerializer::serialize(this->settings, filename);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
   
}