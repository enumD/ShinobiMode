
// AppSerializer.cpp
#include "AppSerializer.h"
#include <iostream>
#include <fstream>

void AppSerializer::serialize(const AppSetting& settings, const std::string& filename) {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Errore nell'aprire il file per la scrittura." << std::endl;
        return;
    }
    size_t id_size = settings.m_wifi_id.size();
    size_t pswd_size = settings.m_wifi_pswd.size();

    file.write(reinterpret_cast<const char*>(&id_size), sizeof(id_size));
    file.write(settings.m_wifi_id.c_str(), id_size);
    file.write(reinterpret_cast<const char*>(&pswd_size), sizeof(pswd_size));
    file.write(settings.m_wifi_pswd.c_str(), pswd_size);
    file.close();
}

AppSetting AppSerializer::deserialize(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Errore nell'aprire il file per la lettura." << std::endl;
        return {};
    }
    
    AppSetting settings;
    size_t id_size, pswd_size;
    
    file.read(reinterpret_cast<char*>(&id_size), sizeof(id_size));
    settings.m_wifi_id.resize(id_size);
    file.read(&settings.m_wifi_id[0], id_size);
    
    file.read(reinterpret_cast<char*>(&pswd_size), sizeof(pswd_size));
    settings.m_wifi_pswd.resize(pswd_size);
    file.read(&settings.m_wifi_pswd[0], pswd_size);
    
    file.close();
    return settings;
}