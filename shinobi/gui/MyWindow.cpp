#include "MyWindow.h"
#include "imgui.h"
#include "../utils/Notification.h"



void MyWindow::initialize() 
{
    m_mainWindow.initialize();
  
    m_settingsWindow.initialize();

    m_modeSelector.init();
 
    setupToolbar();
}

void MyWindow::setupToolbar() {
    // Aggiungi elementi alla toolbar (icona + callback)
    m_toolbar.AddIcon("home", "SHINOBI ON", true, [this] {
        Notification::Show("Hai cliccato sulla home", Notification::Level::SUCCESS);
    });
    
    m_toolbar.AddIcon("settings", "⚙️", true, [this] {
        Notification::Show("Hai cliccato su setting", Notification::Level::SUCCESS);
      
    });
}

void MyWindow::render() 
{
    // Finestra principale che occupa tutto lo schermo
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
    ImGui::Begin("My Window", nullptr, 
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoBringToFrontOnFocus);

    // 1. Render della toolbar fissa in alto
    const float toolbar_height = 40.0f;
    ImGui::BeginChild("Toolbar", ImVec2(0, toolbar_height), false, ImGuiWindowFlags_NoScrollbar);
    m_toolbar.Draw();
    ImGui::EndChild();

    // 2. Area sotto la toolbar (restante altezza)
    ImGui::BeginChild("ContentArea", ImVec2(0, -1), false, ImGuiWindowFlags_NoScrollbar);
    {
        // 3. Menu laterale fisso (50px)
        ImGui::BeginChild("LeftMenu", ImVec2(50, -1), true);
        renderMenu();
        ImGui::EndChild();

        // 4. Area contenuto adattabile (restante larghezza)
        ImGui::SameLine();
        ImGui::BeginChild("DynamicContent", ImVec2(-1, -1), true);
        renderContent();
        ImGui::EndChild();
    }
    ImGui::EndChild(); // ContentArea
    
    ImGui::End(); // End MyWindow
}


void MyWindow::renderMenu() {
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 20));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 15));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 15));

    
    // Bottoni verticali centrati
    ImGui::SetCursorPosX((50 - 40) * 0.5f); // Centra in 50px
    
    if (ImGui::Button("📷", ImVec2(40, 40))) {
        m_activeWindow =ActiveWindow::MAIN;
    }
    
    ImGui::SetCursorPosX((50 - 40) * 0.5f);
    
    if (ImGui::Button("⚙️", ImVec2(40, 40))) {
        m_activeWindow = ActiveWindow::SETTINGS;
    }

    ImGui::SetCursorPosX((50 - 40) * 0.5f);

    if (ImGui::Button("666", ImVec2(40, 40))) {
        m_activeWindow = ActiveWindow::MODE_SELECTOR;
    }
    
    
    ImGui::PopStyleVar(3);
}

void MyWindow::renderContent() {
    switch (m_activeWindow) {
        case ActiveWindow::MAIN:
            m_mainWindow.render();
            break;
        case ActiveWindow::SETTINGS:
            m_settingsWindow.render();
            break;
        case ActiveWindow::MODE_SELECTOR:
            m_modeSelector.render();
            break;
        case ActiveWindow::NONE:
            ImGui::Text("Seleziona una funzione dal menu a destra");
            break;
    }
}

void MyWindow::shutdown() {
    m_mainWindow.shutdown();
    m_settingsWindow.shutdown();
    m_modeSelector.shutdown();
    
}