#include "SettingWindow.h"

void SettingsWindow::initialize() {
    // Inizializzazione eventuali valori di configurazione
    strncpy(m_wifiSSID, "MyWiFi", sizeof(m_wifiSSID));
    strncpy(m_wifiPassword, "", sizeof(m_wifiPassword));
}

void SettingsWindow::render() {
    // Occupa tutto lo spazio disponibile
    ImGui::BeginChild("SettingsView", ImVec2(0, 0), true);
    
    ImGui::Text("Impostazioni dell'applicazione");
    ImGui::Separator();
    
    // Esempio controllo
    static float volume = 0.5f;
    ImGui::SliderFloat("Volume", &volume, 0.0f, 1.0f);
    
    ImGui::EndChild();
}


void SettingsWindow::shutdown() {
    // Cleanup se necessario
}